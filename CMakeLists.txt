
# Skyrim Lag Fix - SKSE Plugin

cmake_minimum_required(VERSION 3.21)

project(SkyrimLagFix VERSION 1.0.0 LANGUAGES CXX)

# Find CommonLibSSE-NG
find_package(CommonLibSSE CONFIG REQUIRED)

# Create SKSE plugin
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES plugin.cpp)

# Use C++23
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Precompiled header
target_precompile_headers(${PROJECT_NAME} PRIVATE PCH.h)

# Determine output folder
if(DEFINED ENV{SKYRIM_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_FOLDER}/Data")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_FOLDER}/Data")
endif()

if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME}")
endif()

# Deploy plugin to MO2
if(DEFINED OUTPUT_FOLDER)

    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

    message(STATUS "Skyrim Lag Fix output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:${PROJECT_NAME}>"
            "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )

    # Copy debug symbols
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "$<TARGET_PDB_FILE:${PROJECT_NAME}>"
                "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
        )
    endif()

endif()
