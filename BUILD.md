# Build and Release Process

This document describes how to build and release Redis Assistant for both Windows and Linux platforms.

## Overview

Each commit to the main branch triggers an automated build and release process that:

1. Builds the application for both Windows and Linux
2. Packages executables with all required dependencies (self-contained)
3. Organizes releases in version folders
4. Creates GitHub release artifacts

## Build Requirements

### Linux
- Ubuntu 20.04 or later
- GCC 9.0 or later
- CMake 3.15 or later
- wxWidgets 3.0 development libraries
- hiredis development libraries

Install dependencies:
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libwxgtk3.0-gtk3-dev libhiredis-dev
```

### Windows
- Windows 10/11
- Visual Studio 2022 or later
- CMake 3.15 or later
- vcpkg for dependency management

Setup vcpkg and dependencies:
```cmd
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install wxwidgets:x64-windows hiredis:x64-windows
```

## Manual Build Process

### Linux

1. **Configure and Build**
   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make -j$(nproc)
   ```

2. **Package**
   ```bash
   cd ..
   ./scripts/package-linux.sh 1.0.0
   ```

   This creates:
   - `releases/v1.0.0/linux/RedisAssistant-1.0.0-linux-x86_64.tar.gz`

### Windows

1. **Configure and Build**
   ```cmd
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=..\vcpkg\scripts\buildsystems\vcpkg.cmake -A x64
   cmake --build . --config Release
   ```

2. **Package**
   ```cmd
   cd ..
   .\scripts\package-windows.bat 1.0.0
   ```

   This creates:
   - `releases/v1.0.0/windows/RedisAssistant-1.0.0-windows-x64.zip`

## Automated Build Process (GitHub Actions)

The `.github/workflows/build.yml` workflow automatically:

1. **Triggers on**:
   - Push to main/master branch
   - Pull requests to main/master
   - Manual workflow dispatch

2. **Build Jobs**:
   - `build-linux`: Builds and packages Linux version
   - `build-windows`: Builds and packages Windows version
   - `create-release`: Creates GitHub release with artifacts (on push to main)

3. **Artifacts**:
   - Linux: `linux-release` artifact containing .tar.gz
   - Windows: `windows-release` artifact containing .zip

## Self-contained Packaging

Both platforms create self-contained packages that include all dependencies:

### Linux Package Contents
```
RedisAssistant-1.0.0-linux-x86_64/
├── RedisAssistant           # Main executable
├── run.sh                   # Launcher script
├── README.txt              # User instructions
└── lib*.so                 # Shared libraries (wxWidgets, hiredis)
```

The `run.sh` script sets `LD_LIBRARY_PATH` to load bundled libraries.

### Windows Package Contents
```
RedisAssistant-1.0.0-windows-x64/
├── RedisAssistant.exe      # Main executable
├── README.txt              # User instructions
└── *.dll                   # DLL files (wxWidgets, hiredis, VC++ runtime)
```

All DLLs are copied from vcpkg installation automatically.

## Version Management

Versions are managed through:
1. The `VERSION` environment variable in `.github/workflows/build.yml`
2. Version parameter in packaging scripts
3. CMakeLists.txt project version

To create a new version:
1. Update `VERSION` in `.github/workflows/build.yml`
2. Update version in `CMakeLists.txt`
3. Commit and push to trigger automated build

## Release Structure

```
releases/
├── README.md                    # Release documentation
└── v1.0.0/                     # Version folder
    ├── linux/
    │   ├── README.md
    │   ├── RedisAssistant-1.0.0-linux-x86_64.tar.gz
    │   └── RedisAssistant-1.0.0-linux-x86_64/
    └── windows/
        ├── README.md
        ├── RedisAssistant-1.0.0-windows-x64.zip
        └── RedisAssistant-1.0.0-windows-x64/
```

## Testing Releases

### Linux
```bash
cd releases/v1.0.0/linux
tar -xzf RedisAssistant-1.0.0-linux-x86_64.tar.gz
cd RedisAssistant-1.0.0-linux-x86_64
./run.sh
```

### Windows
1. Extract `RedisAssistant-1.0.0-windows-x64.zip`
2. Navigate to extracted folder
3. Double-click `RedisAssistant.exe`

## Verification Checklist

Before releasing, verify:
- [ ] Application builds successfully on both platforms
- [ ] Packaged executables run without external dependencies
- [ ] All required DLLs/shared libraries are included
- [ ] README files are present in packages
- [ ] Version numbers are consistent across all files
- [ ] GitHub Actions workflow completes successfully
- [ ] Release artifacts are uploaded correctly

## Troubleshooting

### Linux Issues
- **Missing libraries**: Check that all wxWidgets/hiredis libraries are copied
- **Permission denied**: Ensure `run.sh` is executable (`chmod +x run.sh`)

### Windows Issues
- **DLL not found**: Verify vcpkg installed all dependencies correctly
- **vcruntime140.dll missing**: Install Visual C++ Redistributable

## Additional Notes

- The build process ensures compilation succeeds before packaging
- Self-contained packages allow distribution without requiring users to install dependencies
- Version folders maintain historical releases
- GitHub releases are created automatically for main branch commits
