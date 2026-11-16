// menu_style.h — palette + ImGuiStyle for a dark, single-accent menu.
#pragma once
#include "imgui.h"

namespace menu {

inline ImVec4 Hex(unsigned rgb, float a = 1.0f) {
    return ImVec4(((rgb >> 16) & 0xFF) / 255.0f, ((rgb >> 8) & 0xFF) / 255.0f, (rgb & 0xFF) / 255.0f, a);
}

// Default values: the Fatality-like palette (see references/fatality-misc-tab.md).
struct Palette {
    ImVec4 border  = Hex(0x101227); // outer frame
    ImVec4 base    = Hex(0x191635); // window background
    ImVec4 surface = Hex(0x171430); // group boxes, popups
    ImVec4 inset   = Hex(0x110f24); // control backgrounds
    ImVec4 text    = Hex(0xe8dbe4); // titles, active tab
    ImVec4 header  = Hex(0xc1c0d0); // section headers
    ImVec4 label   = Hex(0x8f8ea0); // row labels
    ImVec4 dim     = Hex(0x4c486d); // inactive tabs, disabled
    ImVec4 accent  = Hex(0xec035c); // the one accent
};

inline Palette& Theme() { static Palette p; return p; }

inline ImVec4 WithAlpha(ImVec4 c, float a) { c.w = a; return c; }

// Stock ImGui paints blue into colours a menu style often forgets: title bars, separator drag states,
// resize grips, built-in tabs, text links and the keyboard-nav cursor. Call at the end of every ApplyStyle.
// base = window background, fill = neutral control fill, accent = the one accent.
inline void CoverStockBlues(ImGuiStyle& s, ImVec4 base, ImVec4 fill, ImVec4 accent) {
    ImVec4* c = s.Colors;
    c[ImGuiCol_TitleBg] = base; c[ImGuiCol_TitleBgActive] = base; c[ImGuiCol_TitleBgCollapsed] = base;
    c[ImGuiCol_SeparatorHovered] = accent; c[ImGuiCol_SeparatorActive] = accent;
    c[ImGuiCol_ResizeGrip] = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_ResizeGripHovered] = WithAlpha(accent, 0.4f); c[ImGuiCol_ResizeGripActive] = WithAlpha(accent, 0.7f);
    c[ImGuiCol_Tab] = base; c[ImGuiCol_TabHovered] = WithAlpha(accent, 0.25f);
#if IMGUI_VERSION_NUM >= 19090   // 1.90.9 renamed TabActive/TabUnfocused* and added the overlines
    c[ImGuiCol_TabSelected] = fill; c[ImGuiCol_TabSelectedOverline] = accent;
    c[ImGuiCol_TabDimmed] = base;   c[ImGuiCol_TabDimmedSelected] = fill; c[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0, 0, 0, 0);
#else
    c[ImGuiCol_TabActive] = fill; c[ImGuiCol_TabUnfocused] = base; c[ImGuiCol_TabUnfocusedActive] = fill;
#endif
#if IMGUI_VERSION_NUM >= 19100   // 1.91.0 added TextLink
    c[ImGuiCol_TextLink] = accent;
#endif
#if IMGUI_VERSION_NUM >= 19140   // 1.91.4 renamed NavHighlight to NavCursor
    c[ImGuiCol_NavCursor] = accent;
#else
    c[ImGuiCol_NavHighlight] = accent;
#endif
}

inline void ApplyStyle(const Palette& p = Theme()) {
    ImGuiStyle& s = ImGui::GetStyle();

    s.WindowRounding = 5.0f;  s.ChildRounding = 4.0f;  s.FrameRounding = 3.0f;
    s.PopupRounding = 4.0f;   s.GrabRounding = 3.0f;   s.ScrollbarRounding = 3.0f;
    s.WindowBorderSize = 1.0f; s.ChildBorderSize = 1.0f; s.PopupBorderSize = 1.0f; s.FrameBorderSize = 0.0f;
    s.WindowPadding = ImVec2(16, 14); s.FramePadding = ImVec2(8, 4);
    s.ItemSpacing = ImVec2(8, 7);     s.ItemInnerSpacing = ImVec2(6, 4);
    s.ScrollbarSize = 6.0f; s.GrabMinSize = 8.0f;

    ImVec4* c = s.Colors;
    c[ImGuiCol_Text]                 = p.label;
    c[ImGuiCol_TextDisabled]         = p.dim;
    c[ImGuiCol_WindowBg]             = p.base;
    c[ImGuiCol_ChildBg]              = p.surface;
    c[ImGuiCol_PopupBg]              = p.surface;
    c[ImGuiCol_Border]               = p.border;
    c[ImGuiCol_BorderShadow]         = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_FrameBg]              = p.inset;
    c[ImGuiCol_FrameBgHovered]       = Hex(0x1a1733);
    c[ImGuiCol_FrameBgActive]        = Hex(0x221d40);
    c[ImGuiCol_MenuBarBg]            = p.base;
    c[ImGuiCol_ScrollbarBg]          = ImVec4(0, 0, 0, 0);
    c[ImGuiCol_ScrollbarGrab]        = p.dim;
    c[ImGuiCol_ScrollbarGrabHovered] = p.label;
    c[ImGuiCol_ScrollbarGrabActive]  = p.accent;
    c[ImGuiCol_CheckMark]            = p.accent;
    c[ImGuiCol_SliderGrab]           = p.accent;
    c[ImGuiCol_SliderGrabActive]     = p.accent;
    c[ImGuiCol_Button]               = p.inset;
    c[ImGuiCol_ButtonHovered]        = Hex(0x221d40);
    c[ImGuiCol_ButtonActive]         = WithAlpha(p.accent, 0.6f);
    c[ImGuiCol_Header]               = WithAlpha(p.accent, 0.18f);
    c[ImGuiCol_HeaderHovered]        = WithAlpha(p.accent, 0.12f);
    c[ImGuiCol_HeaderActive]         = WithAlpha(p.accent, 0.25f);
    c[ImGuiCol_Separator]            = p.border;
    c[ImGuiCol_TextSelectedBg]       = WithAlpha(p.accent, 0.3f);
    c[ImGuiCol_ModalWindowDimBg]     = ImVec4(0, 0, 0, 0.5f);
    CoverStockBlues(s, p.base, p.inset, p.accent);
}

// Fonts: one ImFont per size/weight tier. Paths are yours; any clean sans works (Inter, Manrope, Rubik).
struct Fonts { ImFont* label = nullptr; ImFont* header = nullptr; ImFont* brand = nullptr; };

inline Fonts LoadFonts(const char* regular_ttf, const char* bold_ttf) {
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig cfg;
    cfg.OversampleH = 3; cfg.OversampleV = 1;
    Fonts f;
    f.label  = io.Fonts->AddFontFromFileTTF(regular_ttf, 13.0f, &cfg);
    f.header = io.Fonts->AddFontFromFileTTF(bold_ttf, 12.0f, &cfg);
    f.brand  = io.Fonts->AddFontFromFileTTF(bold_ttf, 20.0f, &cfg);
    return f;
}

} // namespace menu
