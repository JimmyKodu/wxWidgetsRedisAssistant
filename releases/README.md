# Release Structure

This directory contains versioned releases of Redis Assistant.

## Directory Structure

```
releases/
  v1.0.0/
    linux/
      RedisAssistant-1.0.0-linux-x86_64.tar.gz
      RedisAssistant-1.0.0-linux-x86_64/
        RedisAssistant
        run.sh
        README.txt
        lib*.so (required dependencies)
    windows/
      RedisAssistant-1.0.0-windows-x64.zip
      RedisAssistant-1.0.0-windows-x64/
        RedisAssistant.exe
        README.txt
        *.dll (required dependencies)
```

## Building Releases

The GitHub Actions workflow automatically builds and packages releases for both platforms when code is pushed to the main branch.

You can also build releases manually using the packaging scripts:

### Linux
```bash
./scripts/package-linux.sh 1.0.0
```

### Windows
```cmd
.\scripts\package-windows.bat 1.0.0
```

## Self-contained Packages

All releases are self-contained, meaning they include all required dependencies:

- **Linux**: Includes wxWidgets and hiredis shared libraries
- **Windows**: Includes wxWidgets DLLs, hiredis DLL, and Visual C++ runtime

Users do not need to install any dependencies to run the application.
