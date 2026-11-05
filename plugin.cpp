
#include "PCH.h"

#include <Windows.h>
#include <vector>
#include <string>
#include <cwchar>

// --------------------------------------------------
// CONFIGURATION
// --------------------------------------------------

static int g_moduleAnchor = 0;

struct ModConfig
{
    std::wstring profile = L"P-Core";
    bool showConsoleLog = true;
};

// --------------------------------------------------
// FIND INI FILE NEXT TO THE DLL
// --------------------------------------------------

bool GetConfigPath(wchar_t* path, DWORD pathSize)
{
    HMODULE module = nullptr;

    if (!GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCWSTR>(&g_moduleAnchor),
        &module))
    {
        return false;
    }

    DWORD length = GetModuleFileNameW(
        module,
        path,
        pathSize
    );

    if (length == 0 || length >= pathSize)
        return false;

    wchar_t* lastSlash = wcsrchr(path, L'\\');

    if (!lastSlash)
        return false;

    *(lastSlash + 1) = L'\0';

    if (wcslen(path) + wcslen(L"SkyrimLagFix.ini") >= pathSize)
        return false;

    wcscat_s(path, pathSize, L"SkyrimLagFix.ini");

    return true;
}

// --------------------------------------------------
// READ CONFIGURATION
// --------------------------------------------------

ModConfig LoadConfig()
{
    ModConfig config;

    wchar_t configPath[MAX_PATH] = {};

    if (!GetConfigPath(configPath, MAX_PATH))
        return config;

    wchar_t profileBuffer[64] = {};

    GetPrivateProfileStringW(
        L"CPU",
        L"Profile",
        L"P-Core",
        profileBuffer,
        64,
        configPath
    );

    config.profile = profileBuffer;

    config.showConsoleLog =
        GetPrivateProfileIntW(
            L"CPU",
            L"ShowConsoleLog",
            1,
            configPath
        ) != 0;

    // Normalize profile name
    if (_wcsicmp(config.profile.c_str(), L"P-Core") != 0 &&
        _wcsicmp(config.profile.c_str(), L"Default") != 0)
    {
        config.profile = L"Default";
    }

    return config;
}

// --------------------------------------------------
// SKSE PLUGIN
// --------------------------------------------------

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    SKSE::Init(skse);

    SKSE::GetMessagingInterface()->RegisterListener(
        [](SKSE::MessagingInterface::Message* message)
        {
            if (message->type != SKSE::MessagingInterface::kDataLoaded)
                return;

            // Load user configuration
            ModConfig config = LoadConfig();

            auto* console = RE::ConsoleLog::GetSingleton();

            // Console logging helper
            auto log = [&](const char* format, auto... args)
            {
                if (config.showConsoleLog && console)
                {
                    console->Print(format, args...);
                }
            };

            log("===== SKYRIM LAG FIX =====");

            log("Selected Profile: %s",
                config.profile == L"P-Core" ? "P-Core" : "Default");

            // --------------------------------------------------
            // STEP 1: GET CPU TOPOLOGY
            // --------------------------------------------------

            DWORD bufferSize = 0;

            GetLogicalProcessorInformationEx(
                RelationProcessorCore,
                nullptr,
                &bufferSize
            );

            if (bufferSize == 0)
            {
                log("Failed to get CPU topology size!");
                return;
            }

            std::vector<BYTE> buffer(bufferSize);

            if (!GetLogicalProcessorInformationEx(
                RelationProcessorCore,
                reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(
                    buffer.data()),
                &bufferSize))
            {
                log("Failed to read CPU topology! Error: %lu",
                    GetLastError());
                return;
            }

            DWORD offset = 0;
            int coreNumber = 0;

            DWORD_PTR pCoreMask = 0;
            BYTE highestEfficiency = 0;
            bool foundEfficiencyClass = false;

            // --------------------------------------------------
            // STEP 2: DETECT P-CORES
            // --------------------------------------------------

            while (offset < bufferSize)
            {
                auto* info =
                    reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(
                        buffer.data() + offset);

                if (info->Relationship == RelationProcessorCore)
                {
                    coreNumber++;

                    BYTE efficiency =
                        info->Processor.EfficiencyClass;

                    log("Physical Core: %d | Efficiency Class: %u",
                        coreNumber,
                        static_cast<unsigned int>(efficiency));

                    for (WORD i = 0;
                         i < info->Processor.GroupCount;
                         i++)
                    {
                        const auto& groupMask =
                            info->Processor.GroupMask[i];

                        log("Group: %u | Logical CPU Mask: %llu",
                            static_cast<unsigned int>(groupMask.Group),
                            static_cast<unsigned long long>(groupMask.Mask));

                        // This version supports processor group 0
                        if (groupMask.Group != 0)
                            continue;

                        if (!foundEfficiencyClass ||
                            efficiency > highestEfficiency)
                        {
                            highestEfficiency = efficiency;

                            pCoreMask =
                                static_cast<DWORD_PTR>(groupMask.Mask);

                            foundEfficiencyClass = true;
                        }
                        else if (efficiency == highestEfficiency)
                        {
                            pCoreMask |=
                                static_cast<DWORD_PTR>(groupMask.Mask);
                        }
                    }
                }

                if (info->Size == 0)
                    break;

                offset += info->Size;
            }

            log("Total Physical Cores: %d", coreNumber);

            // --------------------------------------------------
            // STEP 3: READ ORIGINAL AFFINITY
            // --------------------------------------------------

            DWORD_PTR originalMask = 0;
            DWORD_PTR systemMask = 0;

            if (!GetProcessAffinityMask(
                GetCurrentProcess(),
                &originalMask,
                &systemMask))
            {
                log("Failed to read current affinity! Error: %lu",
                    GetLastError());
                return;
            }

            log("===== AFFINITY CONTROL =====");

            log("Original Mask: %llu",
                static_cast<unsigned long long>(originalMask));

            // --------------------------------------------------
            // STEP 4: SELECT PROFILE
            // --------------------------------------------------

            DWORD_PTR newMask = originalMask;

            if (config.profile == L"P-Core")
            {
                if (!foundEfficiencyClass || pCoreMask == 0)
                {
                    log("No suitable P-cores found!");
                    log("Original affinity will be retained.");
                    return;
                }

                newMask = pCoreMask;

                log("Highest Efficiency Class: %u",
                    static_cast<unsigned int>(highestEfficiency));

                log("Detected P-Core Mask: %llu",
                    static_cast<unsigned long long>(pCoreMask));
            }
            else
            {
                log("Default profile selected.");
                log("Original affinity will be retained.");
            }

            // --------------------------------------------------
            // STEP 5: VALIDATE MASK
            // --------------------------------------------------

            if (newMask == 0 || (newMask & ~systemMask) != 0)
            {
                log("Invalid affinity mask!");
                log("Original affinity will be retained.");
                return;
            }

            // --------------------------------------------------
            // STEP 6: APPLY AFFINITY
            // --------------------------------------------------

            if (newMask == originalMask)
            {
                log("Affinity is already set to the selected mask.");
            }
            else if (SetProcessAffinityMask(
                GetCurrentProcess(),
                newMask))
            {
                log("Affinity successfully changed!");
            }
            else
            {
                log("Failed to set affinity! Error: %lu",
                    GetLastError());
                return;
            }

            // --------------------------------------------------
            // STEP 7: VERIFY AFFINITY
            // --------------------------------------------------

            DWORD_PTR verifiedMask = 0;
            DWORD_PTR verifiedSystemMask = 0;

            if (GetProcessAffinityMask(
                GetCurrentProcess(),
                &verifiedMask,
                &verifiedSystemMask))
            {
                log("Verified Mask: %llu",
                    static_cast<unsigned long long>(verifiedMask));

                if (verifiedMask != newMask)
                {
                    log("WARNING: Verified mask does not match!");
                }
            }
            else
            {
                log("Affinity verification failed! Error: %lu",
                    GetLastError());
            }

            log("===== END SKYRIM LAG FIX =====");
        });

    return true;
}
