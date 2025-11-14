#include "custom_widgets.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include <algorithm>

namespace CustomWidgets {
    
    // Helper function to draw glowing effect
    void DrawGlow(ImDrawList* draw_list, ImVec2 pos, float radius, ImU32 color, float intensity) {
        const int steps = 8;
        for (int i = steps; i > 0; i--) {
            float alpha = (float)i / steps * intensity * 0.3f;
            ImU32 glow_color = (color & 0x00FFFFFF) | ((int)(alpha * 255) << 24);
            draw_list->AddCircleFilled(pos, radius * (1.0f + i * 0.1f), glow_color);
        }
    }
    
    // Modern checkbox with glow effect and smooth animation
    bool Checkbox(const char* label, bool* v) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        
        // Make checkbox larger and more modern
        const float square_sz = 20.0f;
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect total_bb(pos, ImVec2(pos.x + square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + square_sz));
        const ImRect check_bb(pos, ImVec2(pos.x + square_sz, pos.y + square_sz));
        
        ImGui::ItemSize(total_bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(total_bb, id))
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed)
            *v = !(*v);

        // Color scheme - green accents
        ImVec4 bg_color = *v ? ImVec4(0.2f, 0.8f, 0.3f, 1.0f) : ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        ImVec4 border_color = hovered ? ImVec4(0.3f, 1.0f, 0.4f, 1.0f) : ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
        
        // Draw checkbox background with rounded corners
        ImDrawList* draw_list = window->DrawList;
        draw_list->AddRectFilled(check_bb.Min, check_bb.Max, 
            ImGui::GetColorU32(bg_color), 5.0f);
        
        // Draw border
        draw_list->AddRect(check_bb.Min, check_bb.Max, 
            ImGui::GetColorU32(border_color), 5.0f, 0, 2.0f);
        
        // Draw glow effect when checked
        if (*v) {
            ImVec2 center = ImVec2((check_bb.Min.x + check_bb.Max.x) * 0.5f, 
                                   (check_bb.Min.y + check_bb.Max.y) * 0.5f);
            DrawGlow(draw_list, center, square_sz * 0.5f, 
                    ImGui::GetColorU32(ImVec4(0.2f, 1.0f, 0.3f, 0.5f)), 0.8f);
        }
        
        // Draw checkmark when checked
        if (*v) {
            const float pad = ImMax(1.0f, (float)(int)(square_sz / 6.0f));
            const float check_sz = square_sz - pad * 2.0f;
            const ImVec2 check_min = ImVec2(check_bb.Min.x + pad, check_bb.Min.y + pad);
            const ImVec2 check_max = ImVec2(check_bb.Max.x - pad, check_bb.Max.y - pad);
            
            // Draw modern checkmark
            ImVec2 p1 = ImVec2(check_min.x + check_sz * 0.25f, check_min.y + check_sz * 0.5f);
            ImVec2 p2 = ImVec2(check_min.x + check_sz * 0.45f, check_max.y - check_sz * 0.25f);
            ImVec2 p3 = ImVec2(check_max.x - check_sz * 0.15f, check_min.y + check_sz * 0.2f);
            
            draw_list->AddLine(p1, p2, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 3.0f);
            draw_list->AddLine(p2, p3, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 3.0f);
        }
        
        // Draw label
        if (label_size.x > 0.0f) {
            ImGui::RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, 
                                     check_bb.Min.y + style.FramePadding.y), label);
        }

        return pressed;
    }
    
    // Modern slider with inline value display and smooth styling
    bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        
        const float w = ImGui::CalcItemWidth();
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        const ImRect frame_bb(window->DC.CursorPos, 
                             ImVec2(window->DC.CursorPos.x + w, window->DC.CursorPos.y + label_size.y + style.FramePadding.y * 2.0f));
        const ImRect total_bb(frame_bb.Min, 
                             ImVec2(frame_bb.Max.x + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), frame_bb.Max.y));

        ImGui::ItemSize(total_bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(total_bb, id))
            return false;

        // Slider behavior
        bool hovered = ImGui::ItemHoverable(frame_bb, id, 0);
        bool temp_input_allowed = true;
        bool value_changed = ImGui::SliderBehavior(frame_bb, id, ImGuiDataType_Float, v, &v_min, &v_max, format, 
                                                   ImGuiSliderFlags_None, nullptr);

        // Draw background
        ImDrawList* draw_list = window->DrawList;
        const float rounding = 4.0f;
        
        // Background color
        draw_list->AddRectFilled(frame_bb.Min, frame_bb.Max, 
            ImGui::GetColorU32(ImVec4(0.12f, 0.12f, 0.12f, 1.0f)), rounding);
        
        // Filled portion with green gradient
        float normalized = (*v - v_min) / (v_max - v_min);
        if (normalized > 0.0f) {
            ImVec2 fill_max = ImVec2(frame_bb.Min.x + (frame_bb.Max.x - frame_bb.Min.x) * normalized, frame_bb.Max.y);
            
            // Green fill with gradient effect
            ImU32 col_start = ImGui::GetColorU32(ImVec4(0.4f, 0.8f, 0.3f, 1.0f));
            ImU32 col_end = ImGui::GetColorU32(ImVec4(0.6f, 1.0f, 0.5f, 1.0f));
            
            draw_list->AddRectFilledMultiColor(frame_bb.Min, fill_max, 
                col_start, col_end, col_end, col_start, rounding);
        }
        
        // Draw border
        ImU32 border_col = hovered ? ImGui::GetColorU32(ImVec4(0.5f, 1.0f, 0.6f, 1.0f)) 
                                   : ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        draw_list->AddRect(frame_bb.Min, frame_bb.Max, border_col, rounding, 0, 1.5f);
        
        // Draw value text inside the slider
        char value_buf[64];
        const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), format, *v);
        ImVec2 value_size = ImGui::CalcTextSize(value_buf, value_buf_end);
        ImVec2 value_pos = ImVec2(frame_bb.Max.x - value_size.x - style.FramePadding.x, 
                                  frame_bb.Min.y + (frame_bb.Max.y - frame_bb.Min.y - value_size.y) * 0.5f);
        
        // Draw value with shadow for readability
        draw_list->AddText(ImVec2(value_pos.x + 1, value_pos.y + 1), 
            ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.5f)), value_buf, value_buf_end);
        draw_list->AddText(value_pos, 
            ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), value_buf, value_buf_end);
        
        // Draw label to the right
        if (label_size.x > 0.0f) {
            ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, 
                                     frame_bb.Min.y + style.FramePadding.y), label);
        }

        return value_changed;
    }
    
    // Integer slider variant
    bool SliderInt(const char* label, int* v, int v_min, int v_max) {
        float v_f = (float)*v;
        bool result = SliderFloat(label, &v_f, (float)v_min, (float)v_max, "%.0f");
        *v = (int)v_f;
        return result;
    }
    
    // Collapsible header with icon and modern styling
    bool CollapsingHeader(const char* label, const char* icon) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        
        ImVec2 pos = window->DC.CursorPos;
        ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        
        // Add icon size if present
        float icon_width = icon ? ImGui::CalcTextSize(icon).x + style.ItemInnerSpacing.x : 0.0f;
        
        ImRect bb(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + label_size.y + style.FramePadding.y * 2.0f));
        ImGui::ItemSize(bb, style.FramePadding.y);
        ImGui::ItemAdd(bb, id);

        bool* p_open = window->DC.StateStorage->GetBoolRef(id, false);
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);
        if (pressed)
            *p_open = !*p_open;

        // Draw background
        ImU32 bg_col = hovered ? ImGui::GetColorU32(ImVec4(0.15f, 0.15f, 0.15f, 1.0f)) 
                               : ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
        window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_col, 6.0f);
        
        // Draw icon with green color
        if (icon) {
            ImVec2 icon_pos = ImVec2(bb.Min.x + style.FramePadding.x, 
                                     bb.Min.y + style.FramePadding.y);
            window->DrawList->AddText(icon_pos, 
                ImGui::GetColorU32(ImVec4(0.4f, 1.0f, 0.5f, 1.0f)), icon);
        }
        
        // Draw label
        ImVec2 text_pos = ImVec2(bb.Min.x + style.FramePadding.x + icon_width, 
                                bb.Min.y + style.FramePadding.y);
        window->DrawList->AddText(text_pos, 
            ImGui::GetColorU32(ImVec4(0.9f, 0.9f, 0.9f, 1.0f)), label);
        
        // Draw arrow
        float arrow_size = label_size.y * 0.5f;
        ImVec2 arrow_pos = ImVec2(bb.Max.x - arrow_size - style.FramePadding.x, 
                                 bb.Min.y + (bb.Max.y - bb.Min.y) * 0.5f);
        
        ImGui::RenderArrow(window->DrawList, arrow_pos, 
            ImGui::GetColorU32(ImVec4(0.7f, 0.7f, 0.7f, 1.0f)), 
            *p_open ? ImGuiDir_Down : ImGuiDir_Right, 0.7f);

        if (*p_open) {
            window->DC.CursorPos.y += style.ItemSpacing.y;
        }

        return *p_open;
    }
    
    // Toggle switch style checkbox
    bool ToggleSwitch(const char* label, bool* v) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        
        const float height = 20.0f;
        const float width = 40.0f;
        const float radius = height * 0.5f;
        
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect total_bb(pos, ImVec2(pos.x + width + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + height));
        
        ImGui::ItemSize(total_bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(total_bb, id))
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
        if (pressed)
            *v = !(*v);

        // Animation value (would need state storage for smooth animation)
        float t = *v ? 1.0f : 0.0f;

        // Colors
        ImU32 bg_col = *v ? ImGui::GetColorU32(ImVec4(0.3f, 0.9f, 0.4f, 1.0f)) 
                          : ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        ImU32 knob_col = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        
        // Draw background
        ImDrawList* draw_list = window->DrawList;
        ImRect switch_bb(pos, ImVec2(pos.x + width, pos.y + height));
        draw_list->AddRectFilled(switch_bb.Min, switch_bb.Max, bg_col, radius);
        
        // Draw knob
        float knob_x = switch_bb.Min.x + radius + (width - 2.0f * radius) * t;
        ImVec2 knob_center = ImVec2(knob_x, switch_bb.Min.y + radius);
        draw_list->AddCircleFilled(knob_center, radius - 2.0f, knob_col, 16);
        
        // Draw glow when enabled
        if (*v) {
            DrawGlow(draw_list, knob_center, radius - 2.0f, 
                    ImGui::GetColorU32(ImVec4(0.3f, 1.0f, 0.4f, 0.3f)), 0.5f);
        }
        
        // Draw label
        if (label_size.x > 0.0f) {
            ImGui::RenderText(ImVec2(switch_bb.Max.x + style.ItemInnerSpacing.x, 
                                     switch_bb.Min.y + style.FramePadding.y), label);
        }

        return pressed;
    }
    
    // Color button
    bool ColorButton(const char* label, ImVec4* color) {
        bool result = ImGui::ColorEdit4(label, (float*)color, 
            ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | 
            ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
        
        ImGui::SameLine();
        ImGui::Text("%s", label);
        
        return result;
    }
    
    // Input text with modern styling
    bool InputText(const char* label, char* buf, size_t buf_size) {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.18f, 0.18f, 0.18f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
        
        bool result = ImGui::InputText(label, buf, buf_size);
        
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(4);
        
        return result;
    }
}
