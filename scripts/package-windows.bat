@echo off
REM Windows packaging script for Redis Assistant

setlocal enabledelayedexpansion

if "%1"=="" (
    set VERSION=1.0.0
) else (
    set VERSION=%1
)

set BUILD_DIR=build-windows
set PACKAGE_DIR=releases\v%VERSION%\windows
set PACKAGE_NAME=RedisAssistant-%VERSION%-windows-x64

echo Building Redis Assistant for Windows (version %VERSION%)...

REM Clean and create directories
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
mkdir "%BUILD_DIR%"
if not exist "%PACKAGE_DIR%\%PACKAGE_NAME%" mkdir "%PACKAGE_DIR%\%PACKAGE_NAME%"

REM Build the application
cd "%BUILD_DIR%"
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
cd ..

REM Copy executable
copy "%BUILD_DIR%\Release\RedisAssistant.exe" "%PACKAGE_DIR%\%PACKAGE_NAME%\"

REM Copy dependencies (wxWidgets DLLs and hiredis)
echo Copying dependencies...
REM Try vcpkg path first (CI builds)
set VCPKG_BIN=vcpkg\installed\x64-windows\bin
if exist "%VCPKG_BIN%" (
    echo Using vcpkg dependencies from %VCPKG_BIN%
    for %%D in ("%VCPKG_BIN%\*.dll") do (
        copy "%%D" "%PACKAGE_DIR%\%PACKAGE_NAME%\"
    )
) else (
    REM Fallback to manual wxWidgets installation
    echo Looking for manual wxWidgets installation...
    for %%D in (wxmsw32u_core_vc_x64_custom.dll wxbase32u_vc_x64_custom.dll hiredis.dll) do (
        if exist "C:\wxWidgets\lib\vc_x64_dll\%%D" (
            copy "C:\wxWidgets\lib\vc_x64_dll\%%D" "%PACKAGE_DIR%\%PACKAGE_NAME%\"
        )
    )
)

REM Copy Visual C++ Runtime (if needed)
for %%R in (vcruntime140.dll msvcp140.dll) do (
    where %%R >nul 2>&1
    if !errorlevel! equ 0 (
        for /f "delims=" %%P in ('where %%R') do (
            copy "%%P" "%PACKAGE_DIR%\%PACKAGE_NAME%\" >nul 2>&1
            goto :found_%%R
        )
    )
    :found_%%R
)

REM Create README
(
echo Redis Assistant v%VERSION% - Windows Build
echo.
echo To run the application:
echo     Double-click RedisAssistant.exe
echo.
echo Requirements:
echo - This is a self-contained build with all required dependencies included.
echo - Windows 10/11 x64 system
echo.
echo For more information, visit: https://github.com/JimmyKodu/wxWidgetsRedisAssistant
) > "%PACKAGE_DIR%\%PACKAGE_NAME%\README.txt"

REM Create ZIP archive using PowerShell
powershell -Command "Compress-Archive -Path '%PACKAGE_DIR%\%PACKAGE_NAME%' -DestinationPath '%PACKAGE_DIR%\%PACKAGE_NAME%.zip' -Force"

echo Windows package created: %PACKAGE_DIR%\%PACKAGE_NAME%.zip

endlocal
