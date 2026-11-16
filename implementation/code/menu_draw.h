// menu_draw.h — draw-list effects: soft shadow ring, glow, header gradient.
#pragma once
#include "imgui.h"
#include "imgui_internal.h"

namespace menu {

// Shadow drawn as concentric outlines OUTSIDE the rect, fading out.
// Because it never covers the interior, it can be drawn on any draw list, before or after the window background.
inline void SoftShadow(ImDrawList* dl, ImVec2 min, ImVec2 max, float rounding, ImVec4 col,
                       float spread = 18.0f, int layers = 12) {
    const float step = spread / (float)layers;
    dl->PushClipRectFullScreen();
    for (int i = 0; i < layers; ++i) {
        const float k = (float)i / (float)layers;   // 0 = closest to the rect
        const float off = step * (i + 0.5f);
        ImVec4 c = col;
        c.w = col.w * (1.0f - k) * (1.0f - k);      // quadratic falloff
        dl->AddRect(ImVec2(min.x - off, min.y - off), ImVec2(max.x + off, max.y + off),
                    ImGui::GetColorU32(c), rounding + off, 0, step + 0.5f);
    }
    dl->PopClipRect();
}

// Accent glow: same technique, tighter and colored.
inline void Glow(ImDrawList* dl, ImVec2 min, ImVec2 max, float rounding, ImVec4 accent, float strength = 0.35f) {
    accent.w = strength;
    SoftShadow(dl, min, max, rounding, accent, 8.0f, 8);
}

// Subtle vertical gradient for a header strip (top lighter than bottom).
inline void VerticalGradient(ImDrawList* dl, ImVec2 min, ImVec2 max, ImVec4 top, ImVec4 bottom) {
    const ImU32 t = ImGui::GetColorU32(top), b = ImGui::GetColorU32(bottom);
    dl->AddRectFilledMultiColor(min, max, t, t, b, b);
}

} // namespace menu
