# Skyrim Lag Fix

An SKSE plugin that detects CPU core performance classes and manages Skyrim's processor affinity.

**Version:** 1.0.0  
**Status:** Public Testing  
**Platform:** Windows  
**Game:** The Elder Scrolls V: Skyrim Special Edition

---

## Overview

Skyrim Lag Fix is a lightweight SKSE plugin designed to manage Skyrim's CPU processor affinity automatically.

Modern CPUs can contain different types of cores, such as Performance Cores (P-cores) and Efficiency Cores (E-cores).

This plugin detects the processor topology and allows Skyrim to run using the highest-efficiency CPU cores available, depending on the selected configuration.

The goal is to provide users with more control over how Skyrim uses their CPU.

**Important:** This plugin does not guarantee higher FPS, reduced stuttering, or improved performance on every system. Results depend on the CPU, operating system, game configuration, and workload.

## Features

- Automatic CPU topology detection.
- Performance Core (P-core) identification using Windows CPU efficiency information.
- Automatic processor affinity management.
- Configurable CPU profiles.
- Optional in-game console logging.
- Lightweight SKSE plugin.
- No external executable required.
- INI-based configuration.

## How It Works

When Skyrim loads the SKSE plugin, Skyrim Lag Fix:

1. Detects the available processor cores.
2. Retrieves CPU core performance information using Windows APIs.
3. Identifies the cores with the highest efficiency class.
4. Reads Skyrim's original processor affinity.
5. Applies the selected CPU affinity profile.
6. Verifies the resulting processor affinity.

The plugin performs these operations automatically during the SKSE `kDataLoaded` event.

## Requirements

- The Elder Scrolls V: Skyrim Special Edition.
- A compatible SKSE installation.
- Windows 10 or Windows 11 (64-bit).
- A supported CPU topology.

## Installation

### Mod Organizer 2

1. Download the release ZIP file.
2. Open Mod Organizer 2.
3. Install the downloaded archive as a mod.
4. Enable **Skyrim Lag Fix**.
5. Launch Skyrim through SKSE.

### Manual Installation

Extract the release archive into your Skyrim `Data` directory.

The expected folder structure is:

```text
Data/
└── SKSE/
    └── Plugins/
        ├── SkyrimLagFix.dll
        └── SkyrimLagFix.ini
```

## Configuration

Skyrim Lag Fix uses an INI configuration file located next to the DLL.

**File:** `SkyrimLagFix.ini`

Default configuration:

```ini
[CPU]
Profile=P-Core
ShowConsoleLog=1
```

### Available CPU Profiles

| Profile | Description |
|---|---|
| `P-Core` | Uses the detected highest-efficiency CPU cores. |
| `Default` | Retains the original Skyrim processor affinity. |

### Console Logging

| Value | Description |
|---|---|
| `1` | Enables console logging. |
| `0` | Disables console logging. |

Example: To disable console messages while keeping P-Core affinity enabled:

```ini
[CPU]
Profile=P-Core
ShowConsoleLog=0
```

Restart Skyrim after changing the configuration.

## Known Limitations

- Currently processes CPU affinity within Windows processor Group 0 only.
- CPU topology detection depends on the information provided by Windows.
- Performance improvements are not guaranteed.
- Compatibility and performance may vary across different CPU architectures and systems.

This is an early public testing release. Feedback from different hardware configurations is welcome.

## Testing

**Tested configuration:**

| Component | Configuration |
|---|---|
| CPU | Intel Core i5-1235U |
| Physical Cores | 10 |
| Logical Processors | 12 |
| Operating System | Windows 11 |
| Game | Skyrim Special Edition |
| CPU Profile | P-Core |

**Observed test results:**

- Original affinity mask: `4095`
- Detected P-core mask: `15`
- Applied affinity mask: `15`
- Default profile: Successfully retained original affinity mask (`4095`).

These results describe one tested system and should not be interpreted as universal performance results.

## Building from Source

### Development Requirements

- Visual Studio 2022 Community or later with C++ development tools.
- CMake.
- Git.
- vcpkg.
- CommonLibSSE-NG dependency.

### Build Instructions

1. Clone this repository:

```bash
git clone https://github.com/Pkk2001/SkyrimLagFix.git
cd SkyrimLagFix
```

2. Configure the `VCPKG_ROOT` environment variable to point to your vcpkg installation.

3. Configure the project:

```bash
cmake --preset release
```

4. Build the plugin:

```bash
cmake --build build/release --config Release
```

The generated DLL will be copied to the configured Skyrim mod output directory when `SKYRIM_MODS_FOLDER` is configured.

## Credits

- **SkyrimDev** – Original SKSE plugin project template.
- **CommonLibSSE-NG** – SKSE plugin development library.
- **Microsoft vcpkg** – Dependency management.

Third-party dependencies remain subject to their respective licenses.

## License

This project is licensed under the MIT License.

See the [LICENSE](LICENSE) file for details.

## Feedback and Contributions

This project is currently in public testing.

If you encounter issues or unexpected behavior, please report them through GitHub Issues.

When reporting a problem, include:

- CPU model.
- Windows version.
- Skyrim version.
- SKSE version.
- Selected CPU profile.
- Relevant console logs.

Community testing and feedback are appreciated.

## ⚠️ Warning

Skyrim Lag Fix modifies the CPU affinity of Skyrim to prioritize Performance Cores (P-Cores).

Depending on your CPU, system configuration, cooling solution, and workload, this may:

- Increase CPU usage on selected cores.
- Increase CPU temperature and power consumption.
- Cause additional fan noise.
- Affect performance or system stability on certain hardware configurations.

**Use this plugin at your own risk.**

Monitor your CPU temperatures and system performance while using this plugin. If you experience excessive temperatures, instability, or unexpected behavior, switch the CPU profile to `Default` or disable the plugin.

This plugin does not modify CPU voltage, clock speeds, or BIOS settings. However, its performance and thermal impact may vary between systems.

The author is not responsible for hardware issues, overheating, system instability, or other damage resulting from the use of this plugin.

## Disclaimer

Skyrim Lag Fix is an independent community project and is not affiliated with, endorsed by, or sponsored by Bethesda Game Studios or ZeniMax Media.
