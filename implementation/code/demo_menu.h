// demo_menu.h — a complete standalone menu with placeholder settings. Call DrawDemoMenu() once per frame.
#pragma once
#include "menu_widgets.h"
#include "menu_draw.h"

namespace menu {

inline void DrawDemoMenu(const Fonts* fonts = nullptr) {
    static int tab = 0;
    static bool opt[8] = { true, false, true, true, false, false, true, false };
    static float volume = 70.0f;
    static int quality = 2;
    static int mode = 1;
    static ImVec4 tint = ImVec4(0.93f, 0.01f, 0.36f, 1.0f);

    const Palette& p = Theme();
    ImGui::SetNextWindowSize(ImVec2(760, 480), ImGuiCond_Once);
    ImGui::Begin("##menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
    const ImVec2 wp = ImGui::GetWindowPos(), ws = ImGui::GetWindowSize();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    SoftShadow(dl, wp, wp + ws, ImGui::GetStyle().WindowRounding, ImVec4(0, 0, 0, 0.45f));

    // Header: brand, tabs
    if (fonts && fonts->brand) ImGui::PushFont(fonts->brand);
    ImGui::PushStyleColor(ImGuiCol_Text, p.text);
    ImGui::TextUnformatted("BRAND");
    ImGui::PopStyleColor();
    if (fonts && fonts->brand) ImGui::PopFont();
    ImGui::SameLine(0.0f, 28.0f);
    const char* tabs[] = { "GENERAL", "DISPLAY", "AUDIO", "CONTROLS", "PROFILES" };
    HeaderTabs(tabs, IM_ARRAYSIZE(tabs), &tab);
    ImGui::Dummy(ImVec2(0, 6));

    // Three equal columns of group boxes
    const float gap = 12.0f;
    const float col_w = (ImGui::GetContentRegionAvail().x - gap * 2.0f) / 3.0f;
    const float col_h = ImGui::GetContentRegionAvail().y;
    ImFont* hf = fonts ? fonts->header : nullptr;

    if (BeginGroupBox("GENERAL", ImVec2(col_w, col_h), hf)) {
        CheckRow("Option A", &opt[0]);
        CheckRow("Option B", &opt[1]);
        PillToggle("Option C", &opt[2]);
        AccentSlider("Volume", &volume, 0.0f, 100.0f);
    }
    EndGroupBox();
    ImGui::SameLine(0.0f, gap);

    if (BeginGroupBox("DISPLAY", ImVec2(col_w, col_h), hf)) {
        CheckRow("Option D", &opt[3]);
        AccentSliderInt("Quality", &quality, 0, 4);
        PillToggle("Option E", &opt[4]);
        const char* modes[] = { "Option 1", "Option 2", "Option 3" };
        ComboRow("Mode", &mode, modes, IM_ARRAYSIZE(modes));
    }
    EndGroupBox();
    ImGui::SameLine(0.0f, gap);

    if (BeginGroupBox("AUDIO", ImVec2(col_w, col_h), hf)) {
        CheckRow("Option F", &opt[5]);
        CheckRow("Option G", &opt[6]);
        CheckRow("Option H", &opt[7]);
        ColorRow("Tint", &tint);
    }
    EndGroupBox();

    ImGui::End();
}

} // namespace menu
