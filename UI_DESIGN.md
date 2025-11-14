# UI Design Documentation

## Visual Layout

This document describes the visual appearance of the CS2 Modern ImGui UI.

### Overall Layout

```
┌─────────────────────────────────────────────────────────────────────────────┐
│ FIRE.cc                                                        github.com   │
│─────────────────────────────────────────────────────────────────────────────│
│                                                                              │
│  ┌────┐  ┌──────────────────────────────────────────────────────────────┐  │
│  │ ⊕  │  │  ⊕ General                                                    │  │
│  │    │  │                                                                │  │
│  └────┘  │  ☑ Enable                                                     │  │
│          │  ☐ Auto wall                                                  │  │
│  ┌────┐  │  ☐ Auto fire                                                  │  │
│  │ ⊙  │  │                                                                │  │
│  │    │  │  FOV                                                           │  │
│  └────┘  │  ████████████████░░░░░░░░  164                                │  │
│          │                                                                │  │
│  ┌────┐  │  ☐ Some other shit                                            │  │
│  │ ⚐  │  │  ☐ Prefer body aim                                            │  │
│  │    │  │                                                                │  │
│  └────┘  │  ⊙ Aimbot                                                     │  │
│          │                                                                │  │
│  ┌────┐  │  ☐ Quik Peek                                                  │  │
│  │ ⚡  │  │  ☐ Trigger                                                    │  │
│  │    │  │                                                                │  │
│  └────┘  │  Fov color  [████]                                            │  │
│          └──────────────────────────────────────────────────────────────┘  │
│  ┌────┐                                                                     │
│  │ ⚙  │                                                                     │
│  │    │                                                                     │
│  └────┘                                                                     │
│                                                                              │
│─────────────────────────────────────────────────────────────────────────────│
│ https://github.com/KingsleydotDev/ImGuiHub/                                │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Color Scheme

#### Primary Colors
- **Background**: RGB(25, 25, 25) - Very dark gray
- **Panel Background**: RGB(30, 30, 30) - Dark gray
- **Text**: RGB(242, 242, 242) - Off-white

#### Accent Colors
- **Primary Accent (Green)**: RGB(102, 255, 128) - Bright neon green
- **Hover Accent**: RGB(77, 204, 102) - Medium green
- **Active Accent**: RGB(51, 230, 115) - Bright green

#### Secondary Colors
- **Border**: RGB(64, 64, 64) - Medium gray
- **Disabled Text**: RGB(128, 128, 128) - Gray
- **Shadow**: RGBA(0, 0, 0, 80) - Semi-transparent black

### Typography
- **Primary Font**: System default (can be replaced with Inter or Roboto)
- **Size**: 14px for body text, 16px for headers
- **Weight**: Regular for body, Medium for headers

## Component Specifications

### 1. Sidebar Navigation

#### Dimensions
- Width: 80px
- Button Size: 56x56px
- Spacing: 8px between buttons
- Corner Radius: 8px

#### States
- **Default**: Dark gray background (RGB 38, 38, 38)
- **Hover**: Slightly lighter gray (RGB 64, 64, 64)
- **Active**: Green gradient with glow effect
  - Background: RGB(64, 191, 89)
  - Glow: Radial gradient, green at 30% opacity

#### Icons
Current implementation uses Unicode symbols:
- ⊕ (General)
- ⊙ (Aim)
- ⚐ (Misc)
- ⚡ (Exploits)
- ⚙ (Settings)

### 2. Modern Checkbox

#### Dimensions
- Size: 20x20px
- Border Radius: 5px
- Border Width: 2px

#### States
- **Unchecked**:
  - Background: RGB(38, 38, 38)
  - Border: RGB(77, 77, 77)
  
- **Checked**:
  - Background: RGB(51, 204, 77)
  - Border: RGB(77, 255, 102)
  - Checkmark: White, bold stroke
  - Glow: Green radial gradient

- **Hover**:
  - Border brightens to RGB(128, 255, 153)

### 3. Custom Slider

#### Dimensions
- Height: 28px
- Corner Radius: 4px
- Value text: Inside slider, right-aligned

#### Visual Design
- **Track Background**: RGB(30, 30, 30)
- **Filled Portion**: Green gradient
  - Start: RGB(102, 204, 77)
  - End: RGB(153, 255, 128)
- **Border**: 
  - Default: RGB(77, 77, 77)
  - Hover: RGB(128, 255, 153)
- **Value Display**: White text with black shadow for contrast

### 4. Collapsible Header

#### Dimensions
- Height: 36px
- Padding: 8px horizontal
- Corner Radius: 6px

#### Design
- **Background**: 
  - Default: RGB(25, 25, 25)
  - Hover: RGB(38, 38, 38)
- **Icon**: Green (RGB 102, 255, 128)
- **Arrow**: Gray, rotates on expand
- **Text**: Off-white

### 5. Toggle Switch

#### Dimensions
- Width: 40px
- Height: 20px
- Knob Diameter: 16px
- Border Radius: 10px (full circle)

#### States
- **Off**:
  - Background: RGB(51, 51, 51)
  - Knob: White, left position
  
- **On**:
  - Background: RGB(77, 230, 102)
  - Knob: White, right position
  - Glow: Green radial gradient

### 6. ESP Preview Panel

#### Dimensions
- Width: Auto (responsive)
- Height: 400px
- Border: 1px solid RGB(64, 64, 64)
- Corner Radius: 6px

#### Elements
- **Skeleton**: White lines, 2px thickness
- **Bounding Box**: Green (RGB 102, 255, 128), 2px thickness
- **Health Bar**: 
  - Width: 4px
  - Height: Matches box height
  - Position: Left of box, 8px offset
  - Fill: Green gradient
  - Background: Dark gray RGBA(51, 51, 51, 204)
- **Text**:
  - Nickname: White, top right
  - Distance: Light gray, below nickname
  - Weapon: White, below box

## Sections Detail

### General Section
Contains two collapsible panels:

**General Panel**:
- Enable checkbox
- Auto wall checkbox
- Auto fire checkbox
- FOV slider (0-180)
- Some other shit checkbox
- Prefer body aim checkbox

**Aimbot Panel**:
- Quik Peek checkbox
- Trigger checkbox
- Fov color picker

### Accuracy Section (Aim)
**Accuracy Panel**:
- Automatic stop checkbox
- Multipoints text input
- Damage value slider (0-300)
- Damage override value slider (0-100)
- Double tap checkbox
- Lag peek checkbox

### Exploits Section
**Exploits Panel**:
- Esp preview checkbox
- Esp variables text input

**ESP Preview** (when enabled):
- Large preview panel showing:
  - Player skeleton (white)
  - Bounding box (green)
  - Health bar (green, vertical)
  - Nickname (top right)
  - Distance indicator
  - Weapon icon (bottom center)

### Misc Section
Placeholder for future features

### Settings Section
Placeholder for configuration options

## Responsive Behavior

- **Minimum Width**: 800px
- **Recommended Width**: 1100px
- **Minimum Height**: 600px
- **Recommended Height**: 750px

The ESP Preview panel adjusts to available space while maintaining aspect ratio.

## Animations & Effects

### Glow Effect
- Used on: Active sidebar buttons, checked checkboxes, enabled toggles
- Implementation: Multiple concentric circles with decreasing opacity
- Intensity: 30% base opacity, fades over 8 steps
- Color: Green (RGB 77, 255, 102)

### Hover Effects
- Border color transition: 200ms ease
- Background color transition: 150ms ease
- Scale: No scaling (maintains crispness)

### State Transitions
- Checkbox check: Instant (no animation in current implementation)
- Toggle switch: Would animate position over 200ms (requires state storage)
- Collapsible panels: Instant expand/collapse

## Accessibility

- **Contrast Ratios**:
  - Text on background: 15:1 (excellent)
  - Green accent on dark: 8:1 (good)
  - Disabled text: 4.5:1 (minimum acceptable)

- **Interactive Elements**:
  - Minimum size: 20x20px (meets touch target guidelines)
  - Clear visual feedback on all interactions
  - Tooltips on sidebar icons

## Future Enhancements

1. **Custom Font Integration**: Replace system font with Inter or Roboto
2. **Smooth Animations**: Add state storage for smooth toggle transitions
3. **Icon Assets**: Replace Unicode with custom SVG icons
4. **Glass Effect**: Implement frosted glass blur on overlays
5. **More Gradients**: Add subtle gradients to backgrounds
6. **Particle Effects**: Optional particle effects on interactions
7. **Theme Variants**: Light mode, different accent colors
