@echo off
echo CS2 Modern ImGui UI - Build Script
echo ===================================
echo.

REM Check if ImGui is downloaded
if not exist "imgui" (
    echo ImGui not found. Downloading...
    git clone https://github.com/ocornut/imgui.git imgui
    if errorlevel 1 (
        echo Failed to download ImGui
        pause
        exit /b 1
    )
    echo ImGui downloaded successfully
    echo.
)

REM Create build directory
if not exist "build" mkdir build
cd build

REM Generate Visual Studio project
echo Generating Visual Studio project...
cmake ..
if errorlevel 1 (
    echo Failed to generate project
    cd ..
    pause
    exit /b 1
)

echo.
echo Build files generated successfully!
echo.
echo To build the project:
echo   1. Open build\CS2_ModernUI.sln in Visual Studio
echo   2. Build the solution (F7)
echo   3. Run the application
echo.
echo Or use: cmake --build . --config Release
echo.

cd ..
pause
