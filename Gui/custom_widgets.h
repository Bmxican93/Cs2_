#pragma once
#include "../imgui/imgui.h"

namespace CustomWidgets {
    // Modern checkbox with glow effect
    bool Checkbox(const char* label, bool* v);
    
    // Modern slider with inline value display
    bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.0f");
    bool SliderInt(const char* label, int* v, int v_min, int v_max);
    
    // Collapsible section header with icon
    bool CollapsingHeader(const char* label, const char* icon = nullptr);
    
    // Modern toggle switch
    bool ToggleSwitch(const char* label, bool* v);
    
    // Color button with preview
    bool ColorButton(const char* label, ImVec4* color);
    
    // Multiline input with modern styling
    bool InputText(const char* label, char* buf, size_t buf_size);
    
    // Helper function to draw glowing effect
    void DrawGlow(ImDrawList* draw_list, ImVec2 pos, float radius, ImU32 color, float intensity = 1.0f);
}
