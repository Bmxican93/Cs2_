# Build and Run Guide

## Prerequisites

### Required Software
1. **Windows 10 or 11** (64-bit recommended)
2. **Visual Studio 2019 or later**
   - Install "Desktop development with C++" workload
   - Includes Windows SDK with DirectX 11
3. **Git** (for cloning repositories)
4. **CMake 3.15 or later** (optional, for CMake build)

### System Requirements
- DirectX 11 compatible GPU
- Minimum 4GB RAM
- 100MB free disk space

## Quick Start (Automated)

### Option 1: Using build.bat (Recommended)

1. **Clone the repository**:
```bash
git clone https://github.com/Bmxican93/Cs2_.git
cd Cs2_
```

2. **Run the build script**:
```bash
build.bat
```

This will:
- Download ImGui library automatically
- Generate Visual Studio project files
- Create a `build` directory

3. **Build the project**:
```bash
cd build
cmake --build . --config Release
```

4. **Run the application**:
```bash
bin\Release\CS2_ModernUI.exe
```

## Manual Build Instructions

### Step 1: Download ImGui

```bash
cd Cs2_
git clone https://github.com/ocornut/imgui.git imgui
```

### Step 2: Build with CMake

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Step 3: Run

```bash
cd bin\Release
CS2_ModernUI.exe
```

## Alternative: Visual Studio Direct Build

### Step 1: Create Visual Studio Project

1. Open Visual Studio
2. **File → New → Project**
3. Select **"Windows Desktop Application"** (C++)
4. Set location to the repository folder
5. Name: `CS2_ModernUI`

### Step 2: Add Source Files

**Add to project**:
- `main.cpp`
- `Gui/custom_widgets.cpp`
- `Gui/custom_widgets.h`

**Add ImGui files** (from `imgui` directory):
- `imgui.cpp`
- `imgui_demo.cpp`
- `imgui_draw.cpp`
- `imgui_tables.cpp`
- `imgui_widgets.cpp`
- `backends/imgui_impl_win32.cpp`
- `backends/imgui_impl_dx11.cpp`

**Add ImGui headers** to include path:
- Right-click project → Properties
- C/C++ → General → Additional Include Directories
- Add: `$(ProjectDir)imgui;$(ProjectDir)imgui\backends;$(ProjectDir)`

### Step 3: Link Libraries

**Project Properties → Linker → Input → Additional Dependencies**:
- Add: `d3d11.lib;d3dcompiler.lib;dxgi.lib`

### Step 4: Build and Run

- **Build**: Press F7 or Build → Build Solution
- **Run**: Press F5 or Debug → Start Debugging

## Troubleshooting

### ImGui Not Found
**Error**: `Cannot open include file: 'imgui.h'`

**Solution**:
```bash
cd Cs2_
git clone https://github.com/ocornut/imgui.git imgui
```

### DirectX Libraries Not Found
**Error**: `Cannot open input file 'd3d11.lib'`

**Solution**: Install Windows SDK
- Visual Studio Installer
- Modify installation
- Check "Windows 10 SDK" or "Windows 11 SDK"

### CMake Not Found
**Error**: `'cmake' is not recognized`

**Solution**: 
- Download from https://cmake.org/download/
- Or use Visual Studio Developer Command Prompt

### Build Errors in ImGui
**Error**: Various compilation errors in ImGui files

**Solution**: Ensure you cloned the latest ImGui:
```bash
cd imgui
git pull origin master
```

### Runtime Error: Missing DLL
**Error**: `VCRUNTIME140.dll was not found`

**Solution**: Install Visual C++ Redistributable
- Download from Microsoft website
- Or build in Release mode instead of Debug

### Window Doesn't Appear
**Possible causes**:
1. DirectX 11 not supported on GPU
2. Display scaling issues
3. Antivirus blocking

**Solution**:
- Check GPU supports DirectX 11
- Try running as administrator
- Disable antivirus temporarily

### Black Screen
**Possible cause**: Render target issue

**Solution**: 
- Update graphics drivers
- Try windowed mode (already default)

## Verification

After successful build and run, you should see:

1. **Window**: "CS2 - Modern ImGui UI" (1280x800 default)
2. **Sidebar**: Left side with 5 icon buttons
3. **Header**: "FIRE.cc" branding top left
4. **Content**: General panel with checkboxes and sliders
5. **Footer**: GitHub link at bottom
6. **Theme**: Dark background with green accents

### Test Checklist

- [ ] Window opens without errors
- [ ] Sidebar buttons clickable
- [ ] Switching between sections works
- [ ] Checkboxes toggle on/off
- [ ] Sliders move smoothly
- [ ] Green theme visible
- [ ] ESP preview shows when Exploits section selected
- [ ] Window can be resized
- [ ] Application closes cleanly

## Performance

**Expected performance**:
- **FPS**: 60+ (vsync enabled)
- **CPU Usage**: <5% on modern CPUs
- **Memory**: ~50MB
- **GPU Usage**: Minimal (2D rendering only)

## Build Configurations

### Debug Build
```bash
cmake --build . --config Debug
```
- Includes debug symbols
- Slower performance
- Easier to debug
- Larger executable (~500KB)

### Release Build
```bash
cmake --build . --config Release
```
- Optimized code
- Better performance
- Smaller executable (~300KB)
- Recommended for distribution

## Advanced Options

### Custom ImGui Version

To use a specific ImGui version:
```bash
cd imgui
git checkout v1.89.9  # Replace with desired version
```

### Enable ImGui Demo

To see ImGui demo window for reference:

In `main.cpp`, add to main loop:
```cpp
ImGui::ShowDemoWindow();
```

### Change Window Size

In `main.cpp`, modify:
```cpp
HWND hwnd = ::CreateWindowW(..., 100, 100, 1920, 1080, ...);  // Width, Height
```

### Disable VSync

In `main.cpp`, modify:
```cpp
g_pSwapChain->Present(0, 0);  // Change first parameter from 1 to 0
```

## Development Tips

### Hot Reload Colors

Modify colors in `setStyle()` and rebuild for quick theme iterations.

### Add New Widgets

1. Declare in `Gui/custom_widgets.h`
2. Implement in `Gui/custom_widgets.cpp`
3. Use in `DrawMainContent()` in `main.cpp`

### Debug Rendering

Enable ImGui metrics window:
```cpp
ImGui::ShowMetricsWindow();
```

## Distribution

### Creating Standalone Executable

1. Build in Release mode
2. Copy executable from `build/bin/Release/`
3. Include Visual C++ Redistributable or:
   - Link statically: Add `/MT` to compiler flags
4. Test on clean Windows installation

### Packaging

Recommended structure:
```
CS2_ModernUI/
├── CS2_ModernUI.exe
├── README.txt
└── LICENSE.txt
```

## Next Steps

After successful build:
1. ✅ Verify all UI elements work
2. ✅ Test different sections
3. ✅ Customize colors if desired
4. ✅ Add custom icons (optional)
5. ✅ Integrate with CS2 memory reading (if applicable)
