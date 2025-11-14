#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Gui/custom_widgets.h"
#include <d3d11.h>
#include <tchar.h>

// Data
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

// Forward declarations
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Application state
struct AppState {
    int currentSection = 0; // 0=General, 1=Aim, 2=Misc, 3=Exploits, 4=Settings
    
    // General settings
    bool enableAimbot = false;
    bool autoWall = false;
    bool autoFire = false;
    float fov = 164.0f;
    bool someOtherShit = false;
    bool preferBodyAim = false;
    
    // Aim/Accuracy settings
    bool automaticStop = false;
    char multipoints[128] = "Chest, Stromatch, Legs";
    float damageValue = 226.0f;
    float damageOverride = 35.0f;
    bool doubleTap = true;
    bool lagPeek = true;
    
    // Aimbot settings
    bool quikPeek = false;
    bool trigger = false;
    ImVec4 fovColor = ImVec4(0.6f, 1.0f, 0.5f, 1.0f);
    
    // Exploits settings
    bool espPreview = true;
    char espVariables[256] = "Box, Health, Armor, Nickname, Di";
    
    // ESP preview data
    char playerNickname[64] = "nickname";
    float playerDistance = 29.0f;
} g_AppState;

// Set modern ImGui style inspired by FIRE.cc
void setStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    
    // Window and background styling
    style.WindowPadding = ImVec2(15, 15);
    style.WindowRounding = 8.0f;
    style.FramePadding = ImVec2(8, 6);
    style.FrameRounding = 6.0f;
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 12.0f;
    style.GrabRounding = 5.0f;
    style.ChildRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.TabRounding = 6.0f;
    
    // Border styling
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.TabBorderSize = 0.0f;
    
    // Color scheme - Dark with green/neon accents
    colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.10f, 0.10f, 0.10f, 0.95f);
    colors[ImGuiCol_Border]                 = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.40f, 1.00f, 0.50f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.40f, 0.90f, 0.50f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.50f, 1.00f, 0.60f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.30f, 0.80f, 0.40f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.35f, 0.90f, 0.45f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.20f, 0.70f, 0.30f, 0.80f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.30f, 0.80f, 0.40f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.35f, 0.90f, 0.45f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.30f, 0.80f, 0.40f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.35f, 0.90f, 0.45f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.30f, 0.80f, 0.40f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.35f, 0.90f, 0.45f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.30f, 0.80f, 0.40f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.25f, 0.75f, 0.35f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.18f, 0.60f, 0.28f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.40f, 0.90f, 0.50f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.50f, 1.00f, 0.60f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.40f, 0.90f, 0.50f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.50f, 1.00f, 0.60f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.30f, 0.80f, 0.40f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.40f, 0.90f, 0.50f, 0.95f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.40f, 0.90f, 0.50f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
}

// Draw sidebar navigation
void DrawSidebar(float& sidebarWidth) {
    sidebarWidth = 80.0f;
    
    ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, 0), true, 
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    // Branding at top
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Use default font
    ImGui::SetCursorPosX((sidebarWidth - ImGui::CalcTextSize("FIRE.cc").x) * 0.5f);
    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.6f, 1.0f), "CS2");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::PopFont();
    
    // Navigation buttons with icons (using text placeholders for now)
    const char* sections[] = { "⊕", "⊙", "⚐", "⚡", "⚙" };
    const char* tooltips[] = { "General", "Aim", "Misc", "Exploits", "Settings" };
    
    for (int i = 0; i < 5; i++) {
        ImGui::PushID(i);
        
        bool isSelected = (g_AppState.currentSection == i);
        
        // Button styling for selected state
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.75f, 0.35f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.85f, 0.40f, 1.0f));
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
        }
        
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
        
        // Center the button
        float buttonSize = 56.0f;
        ImGui::SetCursorPosX((sidebarWidth - buttonSize) * 0.5f);
        
        if (ImGui::Button(sections[i], ImVec2(buttonSize, buttonSize))) {
            g_AppState.currentSection = i;
        }
        
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);
        
        // Tooltip
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("%s", tooltips[i]);
        }
        
        // Draw glow effect for selected button
        if (isSelected) {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 btn_min = ImGui::GetItemRectMin();
            ImVec2 btn_max = ImGui::GetItemRectMax();
            ImVec2 center = ImVec2((btn_min.x + btn_max.x) * 0.5f, (btn_min.y + btn_max.y) * 0.5f);
            
            CustomWidgets::DrawGlow(draw_list, center, buttonSize * 0.6f, 
                ImGui::GetColorU32(ImVec4(0.3f, 1.0f, 0.4f, 0.3f)), 0.6f);
        }
        
        ImGui::Spacing();
        ImGui::PopID();
    }
    
    ImGui::EndChild();
}

// Draw ESP preview panel
void DrawESPPreview() {
    ImGui::BeginChild("ESPPreview", ImVec2(0, 400), true);
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    
    // Draw player nickname at top
    ImVec2 nickname_size = ImGui::CalcTextSize(g_AppState.playerNickname);
    ImVec2 nickname_pos = ImVec2(canvas_pos.x + canvas_size.x - nickname_size.x - 20, canvas_pos.y + 20);
    draw_list->AddText(nickname_pos, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 
        g_AppState.playerNickname);
    
    // Draw distance
    char dist_text[32];
    snprintf(dist_text, sizeof(dist_text), "%.0fm", g_AppState.playerDistance);
    ImVec2 dist_size = ImGui::CalcTextSize(dist_text);
    ImVec2 dist_pos = ImVec2(canvas_pos.x + canvas_size.x - dist_size.x - 20, 
                             canvas_pos.y + 20 + nickname_size.y + 5);
    draw_list->AddText(dist_pos, ImGui::GetColorU32(ImVec4(0.8f, 0.8f, 0.8f, 1.0f)), dist_text);
    
    // Draw skeleton in center
    float center_x = canvas_pos.x + canvas_size.x * 0.7f;
    float center_y = canvas_pos.y + canvas_size.y * 0.5f;
    float scale = 80.0f;
    
    ImU32 skeleton_color = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    float thickness = 2.0f;
    
    // Head
    ImVec2 head_center = ImVec2(center_x, center_y - scale * 1.2f);
    draw_list->AddCircle(head_center, scale * 0.2f, skeleton_color, 16, thickness);
    
    // Neck to head
    draw_list->AddLine(ImVec2(center_x, center_y - scale), head_center, skeleton_color, thickness);
    
    // Spine
    draw_list->AddLine(ImVec2(center_x, center_y - scale), 
                      ImVec2(center_x, center_y + scale * 0.5f), skeleton_color, thickness);
    
    // Arms
    // Left arm
    draw_list->AddLine(ImVec2(center_x, center_y - scale * 0.7f), 
                      ImVec2(center_x - scale * 0.8f, center_y), skeleton_color, thickness);
    draw_list->AddLine(ImVec2(center_x - scale * 0.8f, center_y), 
                      ImVec2(center_x - scale * 0.8f, center_y + scale * 0.5f), skeleton_color, thickness);
    
    // Right arm
    draw_list->AddLine(ImVec2(center_x, center_y - scale * 0.7f), 
                      ImVec2(center_x + scale * 0.8f, center_y), skeleton_color, thickness);
    draw_list->AddLine(ImVec2(center_x + scale * 0.8f, center_y), 
                      ImVec2(center_x + scale * 0.8f, center_y + scale * 0.5f), skeleton_color, thickness);
    
    // Legs
    // Left leg
    draw_list->AddLine(ImVec2(center_x, center_y + scale * 0.5f), 
                      ImVec2(center_x - scale * 0.3f, center_y + scale * 1.2f), skeleton_color, thickness);
    draw_list->AddLine(ImVec2(center_x - scale * 0.3f, center_y + scale * 1.2f), 
                      ImVec2(center_x - scale * 0.3f, center_y + scale * 1.8f), skeleton_color, thickness);
    
    // Right leg
    draw_list->AddLine(ImVec2(center_x, center_y + scale * 0.5f), 
                      ImVec2(center_x + scale * 0.3f, center_y + scale * 1.2f), skeleton_color, thickness);
    draw_list->AddLine(ImVec2(center_x + scale * 0.3f, center_y + scale * 1.2f), 
                      ImVec2(center_x + scale * 0.3f, center_y + scale * 1.8f), skeleton_color, thickness);
    
    // Draw bounding box with health bar
    float box_top = center_y - scale * 1.5f;
    float box_bottom = center_y + scale * 1.9f;
    float box_left = center_x - scale * 0.5f;
    float box_right = center_x + scale * 0.5f;
    
    // Green box
    ImU32 box_color = ImGui::GetColorU32(ImVec4(0.4f, 1.0f, 0.5f, 1.0f));
    draw_list->AddRect(ImVec2(box_left, box_top), ImVec2(box_right, box_bottom), 
        box_color, 0.0f, 0, 2.0f);
    
    // Health bar (vertical, green)
    float health_width = 4.0f;
    float health_height = box_bottom - box_top;
    float health_x = box_left - 8.0f;
    
    // Background
    draw_list->AddRectFilled(ImVec2(health_x - health_width, box_top), 
                            ImVec2(health_x, box_bottom), 
                            ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 0.8f)));
    
    // Health fill (100%)
    draw_list->AddRectFilled(ImVec2(health_x - health_width, box_top), 
                            ImVec2(health_x, box_bottom), 
                            ImGui::GetColorU32(ImVec4(0.4f, 1.0f, 0.5f, 1.0f)));
    
    // Draw weapon icon placeholder
    const char* weapon = "AK";
    ImVec2 weapon_size = ImGui::CalcTextSize(weapon);
    ImVec2 weapon_pos = ImVec2(center_x - weapon_size.x * 0.5f, box_bottom + 10);
    draw_list->AddText(weapon_pos, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), weapon);
    
    ImGui::EndChild();
}

// Draw main content area based on selected section
void DrawMainContent() {
    ImGui::BeginChild("MainContent", ImVec2(0, 0), false);
    
    switch (g_AppState.currentSection) {
        case 0: // General
            if (CustomWidgets::CollapsingHeader("General", "⊕")) {
                ImGui::Spacing();
                CustomWidgets::Checkbox("Enable", &g_AppState.enableAimbot);
                CustomWidgets::Checkbox("Auto wall", &g_AppState.autoWall);
                CustomWidgets::Checkbox("Auto fire", &g_AppState.autoFire);
                
                ImGui::Spacing();
                ImGui::Text("FOV");
                CustomWidgets::SliderFloat("##fov", &g_AppState.fov, 0.0f, 180.0f);
                
                ImGui::Spacing();
                CustomWidgets::Checkbox("Some other shit", &g_AppState.someOtherShit);
                CustomWidgets::Checkbox("Prefer body aim", &g_AppState.preferBodyAim);
            }
            
            ImGui::Spacing();
            
            if (CustomWidgets::CollapsingHeader("Aimbot", "⊙")) {
                ImGui::Spacing();
                CustomWidgets::Checkbox("Quik Peek", &g_AppState.quikPeek);
                CustomWidgets::Checkbox("Trigger", &g_AppState.trigger);
                
                ImGui::Spacing();
                ImGui::Text("Fov color");
                CustomWidgets::ColorButton("##fovcolor", &g_AppState.fovColor);
            }
            break;
            
        case 1: // Aim (Accuracy)
            if (CustomWidgets::CollapsingHeader("Accuracy", "⊕")) {
                ImGui::Spacing();
                CustomWidgets::Checkbox("Automatic stop", &g_AppState.automaticStop);
                
                ImGui::Spacing();
                ImGui::Text("Multipoints");
                CustomWidgets::InputText("##multipoints", g_AppState.multipoints, 
                    sizeof(g_AppState.multipoints));
                
                ImGui::Spacing();
                ImGui::Text("Damage value");
                CustomWidgets::SliderFloat("##damagevalue", &g_AppState.damageValue, 0.0f, 300.0f);
                
                ImGui::Spacing();
                ImGui::Text("Damage override value");
                CustomWidgets::SliderFloat("##damageoverride", &g_AppState.damageOverride, 0.0f, 100.0f);
                
                ImGui::Spacing();
                CustomWidgets::Checkbox("Double tap", &g_AppState.doubleTap);
                CustomWidgets::Checkbox("Lag peek", &g_AppState.lagPeek);
            }
            break;
            
        case 2: // Misc
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Misc settings coming soon...");
            break;
            
        case 3: // Exploits
            if (CustomWidgets::CollapsingHeader("Exploits", "⊕")) {
                ImGui::Spacing();
                CustomWidgets::Checkbox("Esp preview", &g_AppState.espPreview);
                
                ImGui::Spacing();
                ImGui::Text("Esp variables");
                CustomWidgets::InputText("##espvariables", g_AppState.espVariables, 
                    sizeof(g_AppState.espVariables));
            }
            
            // ESP Preview section
            if (g_AppState.espPreview) {
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                
                ImGui::Text("ESP Preview");
                DrawESPPreview();
            }
            break;
            
        case 4: // Settings
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Settings coming soon...");
            break;
    }
    
    ImGui::EndChild();
}

// Main code
int main(int, char**)
{
    // Create application window
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui CS2 UI", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"CS2 - Modern ImGui UI", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    setStyle();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Main window
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(1100, 750), ImGuiCond_FirstUseEver);
            
            ImGui::Begin("CS2 Modern UI", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
            
            // Draw branding header
            ImGui::PushFont(io.Fonts->Fonts[0]);
            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.6f, 1.0f), "FIRE.cc");
            ImGui::PopFont();
            
            ImGui::SameLine(ImGui::GetWindowWidth() - 200);
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "github.com");
            
            ImGui::Separator();
            ImGui::Spacing();
            
            // Layout: Sidebar + Main Content
            float sidebarWidth = 0;
            DrawSidebar(sidebarWidth);
            
            ImGui::SameLine();
            
            // Main content area (two columns for some sections)
            ImGui::BeginGroup();
            
            if (g_AppState.currentSection == 3) {
                // Exploits section: two columns
                ImGui::BeginChild("LeftColumn", ImVec2(450, 0), false);
                DrawMainContent();
                ImGui::EndChild();
            } else {
                // Single column for other sections
                DrawMainContent();
            }
            
            ImGui::EndGroup();
            
            // Footer
            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30);
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), "https://github.com/KingsleydotDev/ImGuiHub/");
            
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        const float clear_color[4] = { 0.08f, 0.08f, 0.08f, 1.00f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
