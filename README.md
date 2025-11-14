# CS2 Modern ImGui UI

A modern, minimal ImGui-based user interface for CS2 (Counter-Strike 2) inspired by FIRE.cc design principles.

## Features

### Modern Dark Theme
- Dark, flat backgrounds with subtle gradients
- Green/neon accent colors for highlights and active elements
- Smooth rounded corners and clear section separations
- Soft shadows and glow effects

### Vertical Sidebar Navigation
- Icon-based navigation for main sections
- Glowing indicator for current section
- Sections: General, Aim, Misc, Exploits, Settings

### Custom Widgets
- **Modern Checkboxes**: Larger, with glow effects and smooth animations
- **Custom Sliders**: Green gradient fill with inline value display
- **Toggle Switches**: Smooth switch-style controls
- **Collapsible Headers**: Section headers with icons and smooth expansion
- **Color Pickers**: Integrated color selection with preview

### Panels

#### General Panel
- Enable/disable options
- Auto wall, Auto fire toggles
- FOV slider with visual feedback
- Additional aimbot settings

#### Accuracy Panel (Aim)
- Automatic stop control
- Multipoints configuration
- Damage value sliders with precise control
- Double tap and lag peek options

#### Exploits Panel
- ESP preview toggle
- ESP variables configuration
- Live ESP preview with skeleton rendering

### ESP Preview
- Real-time skeleton rendering
- Bounding boxes with health bars
- Player nickname and distance display
- Weapon indicator

## Building

### Prerequisites
- Windows 10/11
- Visual Studio 2019 or later with C++ development tools
- DirectX 11 SDK (included with Windows SDK)
- Dear ImGui library

### Setup

1. Clone this repository:
```bash
git clone https://github.com/Bmxican93/Cs2_.git
cd Cs2_
```

2. Download Dear ImGui:
```bash
# Clone ImGui into the imgui directory
git clone https://github.com/ocornut/imgui.git imgui
```

3. Build with Visual Studio:
- Open Visual Studio
- Create a new C++ Windows Desktop Application project
- Add all `.cpp` files from the root and `Gui/` directory
- Add ImGui source files:
  - `imgui/imgui.cpp`
  - `imgui/imgui_demo.cpp`
  - `imgui/imgui_draw.cpp`
  - `imgui/imgui_tables.cpp`
  - `imgui/imgui_widgets.cpp`
  - `imgui/backends/imgui_impl_win32.cpp`
  - `imgui/backends/imgui_impl_dx11.cpp`
- Link against `d3d11.lib` and `d3dcompiler.lib`
- Build and run

### Alternative: CMake Build

```bash
# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build
cmake --build . --config Release
```

## Project Structure

```
Cs2_/
├── main.cpp                 # Application entry point and main UI
├── Gui/
│   ├── custom_widgets.h     # Custom widget declarations
│   └── custom_widgets.cpp   # Custom widget implementations
├── imgui/                   # Dear ImGui library (not included)
└── README.md               # This file
```

## Design Principles

This UI follows modern design principles:
- **Minimalism**: Clean, uncluttered interface
- **Consistency**: Uniform styling across all elements
- **Feedback**: Visual responses to user interactions
- **Accessibility**: Clear labels and intuitive navigation
- **Performance**: Efficient rendering and smooth animations

## Customization

### Colors
Edit the `setStyle()` function in `main.cpp` to customize the color scheme. Key colors:
- Background: `ImVec4(0.10f, 0.10f, 0.10f, 1.00f)`
- Accent (Green): `ImVec4(0.40f, 1.00f, 0.50f, 1.00f)`
- Hover: `ImVec4(0.30f, 0.80f, 0.40f, 1.00f)`

### Widgets
Custom widgets are defined in `Gui/custom_widgets.cpp`. You can modify:
- Sizes and padding
- Glow intensity
- Animation curves
- Color schemes

## Credits

- Inspired by [FIRE.cc](https://github.com/KingsleydotDev/ImGuiHub/) design
- Built with [Dear ImGui](https://github.com/ocornut/imgui)
- CS2 offset data from community research

## License

This project is for educational purposes only. Use responsibly and in accordance with game terms of service.

## Disclaimer

This software is provided "as is" without warranty of any kind. Use at your own risk. The authors are not responsible for any consequences resulting from the use of this software, including but not limited to game bans, account suspensions, or other penalties.
