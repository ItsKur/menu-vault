---
title: Draw-list recipes
date: 2026-11-06
tags: [implementation, imgui, shadow, blur, glow]
---

# Draw-list recipes

## Shadow, glow, gradient

`SoftShadow` draws concentric outlines **outside** the rect with quadratic alpha falloff. Because it never covers the interior, it works on the window's own draw list, on popups, and after backgrounds are already drawn (no channel juggling). `Glow` is the same with the accent color and a tighter spread.

```cpp
// inside Begin()/End() of any window or popup
const ImVec2 wp = ImGui::GetWindowPos(), ws = ImGui::GetWindowSize();
menu::SoftShadow(ImGui::GetWindowDrawList(), wp, wp + ws, ImGui::GetStyle().WindowRounding, ImVec4(0, 0, 0, 0.45f));
```

File: `code/menu_draw.h`

```cpp
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
```

## Background blur (glass)

Not possible with draw-list calls alone; it needs the renderer backend. The usual D3D11 pipeline (what [neverlose-ui](../references/neverlose-ui.md) implements):

1. Before rendering ImGui, copy the backbuffer into a texture (`CopyResource` to a shader-resource texture).
2. Downsample to ½ or ¼ size (cheaper and blurrier).
3. Run a separable Gaussian blur: one horizontal pass, one vertical pass, in a pixel shader, ping-ponging between two render targets. Repeat for more blur.
4. In the menu, draw the blurred texture behind the window or popup with `ImDrawList::AddImageRounded`, using UVs that map the window's screen rect into the texture, and the window's rounding.
5. Draw the window surface on top at alpha 0.75–0.9.

In a standalone menu with nothing behind it, blur only matters for popups over the menu itself; capture the frame after drawing the main window, before popups.

Related: [Depth, glass and glow](../principles/depth-glass-and-glow.md), [Combos and dropdowns](../widgets/combos-and-dropdowns.md)
