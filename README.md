# wxWidgetsRedisAssistant

A cross-platform desktop Redis GUI client built with wxWidgets for visualization, management and monitoring of Redis databases.

## Features

- **Cross-platform**: Runs on Windows and Linux
- **Redis Operations**: Support for basic Redis operations (GET, SET, DEL, KEYS)
- **Connection Management**: Easy connection to Redis servers with optional authentication
- **Key Browser**: Visual listing of keys in the database
- **Self-contained**: Packaged releases include all required dependencies

## Building from Source

### Prerequisites

#### Linux
```bash
sudo apt-get install build-essential cmake libwxgtk3.0-gtk3-dev libhiredis-dev
```

#### Windows
- Visual Studio 2022 or later
- vcpkg for dependency management
```cmd
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install wxwidgets:x64-windows hiredis:x64-windows
```

### Build Instructions

#### Linux
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

#### Windows
```cmd
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=..\vcpkg\scripts\buildsystems\vcpkg.cmake -A x64
cmake --build . --config Release
```

## Packaging

The project includes packaging scripts that create self-contained distributions:

### Linux
```bash
./scripts/package-linux.sh [version]
```

### Windows
```cmd
.\scripts\package-windows.bat [version]
```

Packaged releases are placed in `releases/v[version]/[platform]/` directories.

## GitHub Actions

The project includes a GitHub Actions workflow that automatically:
- Builds the application for both Windows and Linux
- Creates self-contained packages with all dependencies
- Uploads artifacts organized in version folders
- Creates GitHub releases when pushing to the main branch

## Usage

1. Launch the application
2. Enter Redis server connection details (host, port, password)
3. Click "Connect" to establish connection
4. Use the operations panel to perform Redis operations:
   - **GET**: Retrieve value for a key
   - **SET**: Set a key-value pair
   - **DEL**: Delete a key
   - **KEYS ***: List all keys in the database

## Version Structure

Each release is organized in version folders:
```
releases/
  v1.0.0/
    linux/
      RedisAssistant-1.0.0-linux-x86_64.tar.gz
    windows/
      RedisAssistant-1.0.0-windows-x64.zip
```

## License

This project is open source. Please refer to the license file for details.