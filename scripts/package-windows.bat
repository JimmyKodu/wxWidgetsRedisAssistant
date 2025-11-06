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
REM You may need to adjust these paths based on your system
for %%D in (wxmsw32u_core_vc_x64_custom.dll wxbase32u_vc_x64_custom.dll hiredis.dll) do (
    if exist "C:\wxWidgets\lib\vc_x64_dll\%%D" (
        copy "C:\wxWidgets\lib\vc_x64_dll\%%D" "%PACKAGE_DIR%\%PACKAGE_NAME%\"
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

REM Create archive using tar (available in Windows 10+)
cd "%PACKAGE_DIR%"
tar -czf "%PACKAGE_NAME%.tar.gz" "%PACKAGE_NAME%"
cd ..\..\..

echo Windows package created: %PACKAGE_DIR%\%PACKAGE_NAME%.tar.gz

endlocal
