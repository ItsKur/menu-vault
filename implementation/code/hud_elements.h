// hud_elements.h — HUD visual elements (visuals/INDEX.md): status pills, keybind list, segmented capsule bar,
// event toasts, hit marker, countdown badge with direction arrow, ground-area markers, an info bar, and indicator
// cards with a fading accent bar.
// Standalone: everything draws into an ImDrawList you pass in, fed with placeholder data.
// Nothing here reads a game, attaches to a process, or projects with a game's matrices; the demo uses its own camera.
#pragma once
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"
#include "imgui_internal.h"
#include "menu_draw.h"   // SoftShadow
#include "menu_anim.h"   // ReduceMotion
#include <algorithm>
#include <cmath>
#include <vector>

namespace hud {

inline ImVec4 Rgba(int r, int g, int b, int a = 255) { return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); }
inline ImU32 U(ImVec4 c, float alpha_mul = 1.0f) { c.w *= alpha_mul; return ImGui::ColorConvertFloat4ToU32(c); }

// ---------------------------------------------------------------------------------------------
// Animation state outside any window: HUD elements live for many frames and have no ImGui window.

inline ImGuiStorage& Store() { static ImGuiStorage s; return s; }
inline ImGuiID Key(const char* a, int b = 0) { return ImHashStr(a, 0, (ImGuiID)b * 2654435761u + 1u); }

// Damped spring (stiffness k, damping c). Underdamped below c = 2*sqrt(k): it overshoots slightly.
// `snap` jumps straight to the target (use when an element first appears); so does menu::ReduceMotion().
inline float SpringTo(ImGuiID id, float target, float k = 200.0f, float c = 20.0f, bool snap = false) {
    ImGuiStorage& s = Store();
    const ImGuiID vid = id ^ 0x5bd1e995u;
    float x = s.GetFloat(id, target), v = s.GetFloat(vid, 0.0f);
    if (snap || menu::ReduceMotion()) { x = target; v = 0.0f; }
    const float dt = ImMin(ImGui::GetIO().DeltaTime, 1.0f / 30.0f);   // clamp so a hitch can't explode it
    v += ((target - x) * k - v * c) * dt;
    x += v * dt;
    s.SetFloat(id, x); s.SetFloat(vid, v);
    return x;
}

// Timed fade with ease-out: `in_s` seconds to appear, `out_s` to disappear. Returns 0..1 (0 or 1 under ReduceMotion).
inline float FadeTo(ImGuiID id, bool on, float in_s = 0.2f, float out_s = 0.15f) {
    float t = Store().GetFloat(id, 0.0f);
    t = menu::ReduceMotion() ? (on ? 1.0f : 0.0f) : ImSaturate(t + ImGui::GetIO().DeltaTime / (on ? in_s : -out_s));
    Store().SetFloat(id, t);
    return 1.0f - (1.0f - t) * (1.0f - t);
}

// ---------------------------------------------------------------------------------------------
// Low-level fills with per-vertex color (ImGui's helpers are single-color).

// Line segment whose color goes from ca (at a) to cb (at b).
inline void GradientLine(ImDrawList* dl, ImVec2 a, ImVec2 b, ImU32 ca, ImU32 cb, float thickness) {
    ImVec2 d = b - a;
    const float len = ImSqrt(d.x * d.x + d.y * d.y);
    if (len < 0.001f) return;
    const ImVec2 n = ImVec2(-d.y, d.x) * (thickness * 0.5f / len);
    const ImVec2 uv = ImGui::GetFontTexUvWhitePixel();
    const ImDrawIdx i0 = (ImDrawIdx)dl->_VtxCurrentIdx;
    dl->PrimReserve(6, 4);
    dl->PrimWriteVtx(a + n, uv, ca); dl->PrimWriteVtx(b + n, uv, cb);
    dl->PrimWriteVtx(b - n, uv, cb); dl->PrimWriteVtx(a - n, uv, ca);
    dl->PrimWriteIdx(i0); dl->PrimWriteIdx(i0 + 1); dl->PrimWriteIdx(i0 + 2);
    dl->PrimWriteIdx(i0); dl->PrimWriteIdx(i0 + 2); dl->PrimWriteIdx(i0 + 3);
}

// Closed band between two rings of n points (inner color -> outer color).
inline void RingBand(ImDrawList* dl, const ImVec2* inner, const ImVec2* outer, int n, ImU32 c_in, ImU32 c_out) {
    const ImVec2 uv = ImGui::GetFontTexUvWhitePixel();
    const ImDrawIdx i0 = (ImDrawIdx)dl->_VtxCurrentIdx;
    dl->PrimReserve(n * 6, n * 2);
    for (int i = 0; i < n; ++i) { dl->PrimWriteVtx(inner[i], uv, c_in); dl->PrimWriteVtx(outer[i], uv, c_out); }
    for (int i = 0; i < n; ++i) {
        const ImDrawIdx a = (ImDrawIdx)(i0 + i * 2), b = (ImDrawIdx)(i0 + ((i + 1) % n) * 2);
        dl->PrimWriteIdx(a); dl->PrimWriteIdx(b); dl->PrimWriteIdx((ImDrawIdx)(b + 1));
        dl->PrimWriteIdx(a); dl->PrimWriteIdx((ImDrawIdx)(b + 1)); dl->PrimWriteIdx((ImDrawIdx)(a + 1));
    }
}

// Triangle fan from a center: valid for any star-shaped outline, concave included.
inline void Fan(ImDrawList* dl, ImVec2 center, const ImVec2* ring, int n, ImU32 c_center, ImU32 c_ring) {
    const ImVec2 uv = ImGui::GetFontTexUvWhitePixel();
    const ImDrawIdx i0 = (ImDrawIdx)dl->_VtxCurrentIdx;
    dl->PrimReserve(n * 3, n + 1);
    dl->PrimWriteVtx(center, uv, c_center);
    for (int i = 0; i < n; ++i) dl->PrimWriteVtx(ring[i], uv, c_ring);
    for (int i = 0; i < n; ++i) {
        dl->PrimWriteIdx(i0); dl->PrimWriteIdx((ImDrawIdx)(i0 + 1 + i)); dl->PrimWriteIdx((ImDrawIdx)(i0 + 1 + (i + 1) % n));
    }
}

// ---------------------------------------------------------------------------------------------
// Small stroked glyphs (swap in an icon font if you have one).

enum Glyph { GlyphLock, GlyphCrosshair, GlyphTarget, GlyphSteps, GlyphKeyboard, GlyphFlame, GlyphCheck, GlyphCross,
             GlyphBars, GlyphSignal, GlyphChip, GlyphClock, GlyphUser };

inline void DrawGlyph(ImDrawList* dl, Glyph g, ImVec2 c, float s, ImU32 col) {   // s = half size
    const float th = ImMax(1.4f, s * 0.22f);
    switch (g) {
    case GlyphLock:
        dl->AddRectFilled(c + ImVec2(-s * 0.8f, -s * 0.1f), c + ImVec2(s * 0.8f, s), col, s * 0.2f);
        dl->PathArcTo(c + ImVec2(0, -s * 0.15f), s * 0.5f, IM_PI, IM_PI * 2.0f, 12); dl->PathStroke(col, 0, th);
        dl->AddLine(c + ImVec2(-s * 0.5f, -s * 0.15f), c + ImVec2(-s * 0.5f, -s * 0.05f), col, th);
        dl->AddLine(c + ImVec2(s * 0.5f, -s * 0.15f), c + ImVec2(s * 0.5f, -s * 0.05f), col, th);
        break;
    case GlyphCrosshair:
        dl->AddCircle(c, s * 0.65f, col, 20, th);
        dl->AddLine(c + ImVec2(0, -s), c + ImVec2(0, -s * 0.35f), col, th); dl->AddLine(c + ImVec2(0, s * 0.35f), c + ImVec2(0, s), col, th);
        dl->AddLine(c + ImVec2(-s, 0), c + ImVec2(-s * 0.35f, 0), col, th); dl->AddLine(c + ImVec2(s * 0.35f, 0), c + ImVec2(s, 0), col, th);
        break;
    case GlyphTarget:
        dl->AddCircle(c, s * 0.9f, col, 24, th); dl->AddCircle(c, s * 0.45f, col, 16, th); break;
    case GlyphSteps:
        dl->AddEllipseFilled(c + ImVec2(-s * 0.35f, -s * 0.3f), ImVec2(s * 0.3f, s * 0.5f), col, 0.3f);
        dl->AddEllipseFilled(c + ImVec2(s * 0.35f, s * 0.3f), ImVec2(s * 0.3f, s * 0.5f), col, 0.3f); break;
    case GlyphKeyboard:
        dl->AddRect(c + ImVec2(-s, -s * 0.6f), c + ImVec2(s, s * 0.6f), col, s * 0.2f, 0, th * 0.8f);
        for (int i = 0; i < 4; ++i) dl->AddRectFilled(c + ImVec2(-s * 0.7f + i * s * 0.38f, -s * 0.3f), c + ImVec2(-s * 0.48f + i * s * 0.38f, -s * 0.08f), col);
        dl->AddLine(c + ImVec2(-s * 0.5f, s * 0.25f), c + ImVec2(s * 0.5f, s * 0.25f), col, th * 0.8f); break;
    case GlyphFlame: {
        const ImVec2 p[7] = { c + ImVec2(0, -s), c + ImVec2(s * 0.55f, -s * 0.1f), c + ImVec2(s * 0.6f, s * 0.45f), c + ImVec2(0, s),
                              c + ImVec2(-s * 0.6f, s * 0.45f), c + ImVec2(-s * 0.45f, -s * 0.05f), c + ImVec2(-s * 0.1f, -s * 0.35f) };
        dl->AddConvexPolyFilled(p, 7, col); break; }
    case GlyphCheck:
        dl->AddLine(c + ImVec2(-s * 0.6f, 0), c + ImVec2(-s * 0.15f, s * 0.45f), col, th);
        dl->AddLine(c + ImVec2(-s * 0.15f, s * 0.45f), c + ImVec2(s * 0.65f, -s * 0.5f), col, th); break;
    case GlyphCross:
        dl->AddLine(c + ImVec2(-s * 0.55f, -s * 0.55f), c + ImVec2(s * 0.55f, s * 0.55f), col, th);
        dl->AddLine(c + ImVec2(-s * 0.55f, s * 0.55f), c + ImVec2(s * 0.55f, -s * 0.55f), col, th); break;
    case GlyphBars: {   // bar chart: three bottom-aligned bars, tallest in the middle
        const float hs[3] = { 0.9f, 1.8f, 1.3f };
        for (int i = 0; i < 3; ++i) {
            const float x = c.x - s * 0.85f + i * s * 0.62f;
            dl->AddRectFilled(ImVec2(x, c.y + s - hs[i] * s), ImVec2(x + s * 0.44f, c.y + s), col, s * 0.12f);
        }
        break; }
    case GlyphSignal:   // four ascending bars
        for (int i = 0; i < 4; ++i) {
            const float x = c.x - s * 0.9f + i * s * 0.5f;
            dl->AddRectFilled(ImVec2(x, c.y + s - (0.5f + i * 0.45f) * s), ImVec2(x + s * 0.32f, c.y + s), col, s * 0.1f);
        }
        break;
    case GlyphChip:     // square body with two pins per side
        dl->AddRect(c - ImVec2(s * 0.6f, s * 0.6f), c + ImVec2(s * 0.6f, s * 0.6f), col, s * 0.15f, 0, th * 0.9f);
        dl->AddRectFilled(c - ImVec2(s * 0.25f, s * 0.25f), c + ImVec2(s * 0.25f, s * 0.25f), col);
        for (int i = -1; i <= 1; i += 2) {
            const float o = i * s * 0.28f;
            dl->AddLine(c + ImVec2(o, -s), c + ImVec2(o, -s * 0.6f), col, th * 0.8f); dl->AddLine(c + ImVec2(o, s * 0.6f), c + ImVec2(o, s), col, th * 0.8f);
            dl->AddLine(c + ImVec2(-s, o), c + ImVec2(-s * 0.6f, o), col, th * 0.8f); dl->AddLine(c + ImVec2(s * 0.6f, o), c + ImVec2(s, o), col, th * 0.8f);
        }
        break;
    case GlyphClock:
        dl->AddCircle(c, s * 0.85f, col, 24, th);
        dl->AddLine(c, c + ImVec2(0, -s * 0.5f), col, th * 0.9f); dl->AddLine(c, c + ImVec2(s * 0.38f, 0), col, th * 0.9f);
        break;
    case GlyphUser:     // head + shoulders dome
        dl->AddCircleFilled(c + ImVec2(0, -s * 0.45f), s * 0.4f, col, 16);
        dl->PathArcTo(c + ImVec2(0, s * 0.95f), s * 0.78f, IM_PI, IM_PI * 2.0f, 14); dl->PathFillConvex(col);
        break;
    }
}

// ---------------------------------------------------------------------------------------------
// 1. Status pills: icon + short label in a dark stadium. Active state recolors icon and text only.

struct PillStyle {
    float h = 30.0f, pad_x = 11.0f, icon = 7.0f, gap = 7.0f;
    ImVec4 bg = Rgba(0, 0, 0, 115);         // ~45% black over the scene (sampled, approximate)
    ImVec4 text = Rgba(255, 255, 255, 240);
    ImVec4 active = Rgba(218, 49, 38);      // sampled from the "on" capture, approximate
    ImVec4 halo = Rgba(0, 0, 0, 50);
};

// Returns the pill's size. `t_active` 0..1 (animate it), `alpha` for fade in/out.
inline ImVec2 StatusPill(ImDrawList* dl, ImVec2 pos, Glyph g, const char* label, float t_active, float alpha = 1.0f,
                         ImFont* font = nullptr, const PillStyle& st = PillStyle()) {
    if (font) ImGui::PushFont(font, 0.0f);
    const ImVec2 ts = ImGui::CalcTextSize(label);
    const ImVec2 size(st.pad_x * 2.0f + st.icon * 2.0f + st.gap + ts.x, st.h);
    const float r = st.h * 0.5f;
    menu::SoftShadow(dl, pos, pos + size, r, ImVec4(st.halo.x, st.halo.y, st.halo.z, st.halo.w * alpha), 6.0f, 6);
    dl->AddRectFilled(pos, pos + size, U(st.bg, alpha), r);
    const ImVec4 fg = ImLerp(st.text, st.active, t_active);
    DrawGlyph(dl, g, ImVec2(pos.x + st.pad_x + st.icon, pos.y + r), st.icon, U(fg, alpha));
    dl->AddText(ImVec2(pos.x + st.pad_x + st.icon * 2.0f + st.gap, pos.y + (st.h - ts.y) * 0.5f), U(fg, alpha), label);
    if (font) ImGui::PopFont();
    return size;
}

// ---------------------------------------------------------------------------------------------
// 2. Keybind list: header (icon disc + title pill), then one pill per active bind: name left, key right (dim).
// Rows fade in/out and spring to their slot, so rows below a removed one glide up instead of jumping.

struct KeybindStyle {
    float row_h = 26.0f, spacing = 6.0f, pad_x = 12.0f, min_w = 150.0f;
    float fade_in = 0.2f, fade_out = 0.15f, stiffness = 200.0f, damping = 20.0f;
    ImVec4 bg = Rgba(0, 0, 0, 105), text = Rgba(255, 255, 255, 235), dim = Rgba(255, 255, 255, 110);
};
struct KeybindRow { const char* name; const char* key; bool active; };

// `anchor` is the top-right corner (right-aligned list). Rows with active=false animate out.
inline void KeybindList(ImDrawList* dl, ImVec2 anchor, const char* title, const KeybindRow* rows, int n,
                        float alpha = 1.0f, const KeybindStyle& st = KeybindStyle()) {
    float w = st.min_w;
    for (int i = 0; i < n; ++i)
        w = ImMax(w, st.pad_x * 2.0f + ImGui::CalcTextSize(rows[i].name).x + 24.0f + ImGui::CalcTextSize(rows[i].key).x);
    w = SpringTo(Key("kb_width"), w, 300.0f, 30.0f);
    const float x0 = anchor.x - w, h = st.row_h, r = h * 0.5f;

    // header: separate disc + title pill
    dl->AddCircleFilled(ImVec2(x0 + r, anchor.y + r), r, U(st.bg, alpha), 24);
    DrawGlyph(dl, GlyphKeyboard, ImVec2(x0 + r, anchor.y + r), 7.0f, U(st.text, alpha));
    const ImVec2 tp0(x0 + h + st.spacing, anchor.y);
    dl->AddRectFilled(tp0, ImVec2(anchor.x, anchor.y + h), U(st.bg, alpha), r);
    const ImVec2 tts = ImGui::CalcTextSize(title);
    dl->AddText(ImVec2((tp0.x + anchor.x - tts.x) * 0.5f, anchor.y + (h - tts.y) * 0.5f), U(st.text, alpha), title);

    float slot = h + st.spacing;
    for (int i = 0; i < n; ++i) {
        const ImGuiID id = Key(rows[i].name);
        const bool was_hidden = Store().GetFloat(id + 1, 0.0f) <= 0.001f;
        const float a = FadeTo(id + 1, rows[i].active, st.fade_in, st.fade_out);
        if (a <= 0.001f) continue;
        const float y = anchor.y + SpringTo(id + 2, slot, st.stiffness, st.damping, was_hidden);   // appear in place
        const ImVec2 p0(x0, y), p1(anchor.x, y + h);
        dl->AddRectFilled(p0, p1, U(st.bg, alpha * a), r);
        const ImVec2 ns = ImGui::CalcTextSize(rows[i].name), ks = ImGui::CalcTextSize(rows[i].key);
        dl->AddText(ImVec2(p0.x + st.pad_x, y + (h - ns.y) * 0.5f), U(st.text, alpha * a), rows[i].name);
        dl->AddText(ImVec2(p1.x - st.pad_x - ks.x, y + (h - ks.y) * 0.5f), U(st.dim, alpha * a), rows[i].key);
        if (rows[i].active) slot += h + st.spacing;   // removed rows give up their slot immediately
    }
}

// ---------------------------------------------------------------------------------------------
// 3. Segmented capsule (velocity.cat language): an outer glass capsule (radius 8) holding inner cells (radius 6)
// inset by 2 px. Cells: an accent "icon/logo" cell with dark content, and text cells built from spans
// where key words are accent and connector words are dim. Used for watermarks, toasts and bind rows.

struct Span { const char* text; bool accent; };
struct Cell { Glyph glyph; bool filled_accent; const Span* spans; int span_count; };
struct CapsuleStyle {
    float h = 24.0f, r = 8.0f, inner_r = 6.0f, inner_pad = 2.0f, text_pad = 8.0f, icon_w = 20.0f;
    ImVec4 outer = Rgba(31, 31, 35, 175), cell = Rgba(17, 17, 17, 82);
    ImVec4 accent = Rgba(173, 192, 255), dim = Rgba(173, 192, 255, 82), dark = Rgba(17, 17, 17);
};

inline float CellWidth(const Cell& c, const CapsuleStyle& st) {
    if (!c.spans) return st.icon_w;
    float w = st.text_pad * 2.0f;
    for (int i = 0; i < c.span_count; ++i) w += ImGui::CalcTextSize(c.spans[i].text).x;
    return w;
}
inline float CapsuleWidth(const Cell* cells, int n, const CapsuleStyle& st = CapsuleStyle()) {
    float w = st.inner_pad;
    for (int i = 0; i < n; ++i) w += CellWidth(cells[i], st) + st.inner_pad;
    return w;
}

// Draws at `pos` (top-left) with width `w` (pass CapsuleWidth, or a smoothed value of it).
inline void Capsule(ImDrawList* dl, ImVec2 pos, float w, const Cell* cells, int n, float alpha = 1.0f,
                    const CapsuleStyle& st = CapsuleStyle()) {
    dl->AddRectFilled(pos, pos + ImVec2(w, st.h), U(st.outer, alpha), st.r);   // the source blurs behind this
    dl->PushClipRect(pos, pos + ImVec2(w, st.h), true);
    float x = pos.x + st.inner_pad;
    const float y0 = pos.y + st.inner_pad, y1 = pos.y + st.h - st.inner_pad;
    for (int i = 0; i < n; ++i) {
        const Cell& c = cells[i];
        const float cw = CellWidth(c, st);
        dl->AddRectFilled(ImVec2(x, y0), ImVec2(x + cw, y1), U(c.filled_accent ? st.accent : st.cell, alpha), st.inner_r);
        if (!c.spans) {
            DrawGlyph(dl, c.glyph, ImVec2(x + cw * 0.5f, pos.y + st.h * 0.5f), 5.5f, U(c.filled_accent ? st.dark : st.accent, alpha));
        } else {
            float tx = x + st.text_pad;
            for (int k = 0; k < c.span_count; ++k) {
                const ImVec2 ts = ImGui::CalcTextSize(c.spans[k].text);
                const ImVec4 col = c.filled_accent ? st.dark : (c.spans[k].accent ? st.accent : st.dim);
                dl->AddText(ImVec2(tx, pos.y + (st.h - ts.y) * 0.5f + 0.5f), U(col, alpha), c.spans[k].text);
                tx += ts.x;
            }
        }
        x += cw + st.inner_pad;
    }
    dl->PopClipRect();
}

// ---------------------------------------------------------------------------------------------
// 4. Event toasts: capsules stacked from a corner. Each slides in from -150 px on a spring (180 / 16, a little
// overshoot) and fades out over the last 20% of its life (0.5 s minimum).

struct Toast { double born; float life; Glyph glyph; const Span* spans; int span_count; };

inline void ToastStack(ImDrawList* dl, ImVec2 origin, const Toast* toasts, int n, double now,
                       const CapsuleStyle& st = CapsuleStyle()) {
    float y = origin.y;
    for (int i = 0; i < n; ++i) {
        const Toast& t = toasts[i];
        const float age = (float)(now - t.born);
        if (age < 0.0f || age > t.life) continue;
        const ImGuiID id = Key("toast", (int)(t.born * 1000.0));
        const float slide = SpringTo(id, 0.0f, 180.0f, 16.0f);   // seeded at -150 by ToastSpawn
        const float fade_len = ImMax(0.5f, t.life * 0.2f);
        const float a = ImSaturate((t.life - age) / fade_len);
        const Cell cells[2] = { { t.glyph, true, nullptr, 0 }, { GlyphCheck, false, t.spans, t.span_count } };
        const float w = CapsuleWidth(cells, 2, st);
        Capsule(dl, ImVec2(origin.x + slide, y), w, cells, 2, a * a * (3.0f - 2.0f * a), st);
        y += st.h + 3.0f;
    }
}
// Seed a toast's slide so it starts off-screen: call once when you create it.
inline void ToastSpawn(const Toast& t) {
    const ImGuiID id = Key("toast", (int)(t.born * 1000.0));
    Store().SetFloat(id, -150.0f); Store().SetFloat(id ^ 0x5bd1e995u, 0.0f);
}

// ---------------------------------------------------------------------------------------------
// 5. Hit marker: four diagonal arms, transparent at the inner end. They start 6 px longer and snap in
// (ease-out-quart, ~50 ms); alpha = 1 - p^2 over the duration. Optional value pops above it with a short shake.

inline void HitMarker(ImDrawList* dl, ImVec2 c, float age, float duration, ImVec4 col, const char* value = nullptr) {
    if (age < 0.0f || age > duration) return;
    const float p = age / duration, a = 1.0f - p * p;
    const float e = menu::ReduceMotion() ? 1.0f : 1.0f - std::pow(1.0f - ImMin(age * 20.0f, 1.0f), 4.0f);   // no pop-in expand
    const float expand = 6.0f * (1.0f - e), size = 10.0f + expand, gap = 3.0f + expand * 0.3f;
    const ImU32 solid = U(col, a), clear = U(col, 0.0f);
    for (int i = 0; i < 4; ++i) {
        const ImVec2 d(i % 2 ? 0.7071f : -0.7071f, i < 2 ? -0.7071f : 0.7071f);
        const ImVec2 inner = c + d * gap, mid = c + d * (gap + (size - gap) * 0.5f), outer = c + d * size;
        GradientLine(dl, inner, mid, clear, solid, 1.25f);
        GradientLine(dl, mid, outer, solid, solid, 1.25f);
    }
    if (value) {
        const float shake = menu::ReduceMotion() ? 0.0f : ImMax(0.0f, 1.0f - age * 8.0f);
        const ImVec2 ts = ImGui::CalcTextSize(value);
        const ImVec2 tp = c + ImVec2(-ts.x * 0.5f + std::sin(age * 50.0f) * shake * 3.0f, -20.0f - ts.y * 0.5f + std::cos(age * 45.0f) * shake * 2.0f);
        dl->AddText(tp + ImVec2(1, 1), U(Rgba(0, 0, 0, 160), a), value);
        dl->AddText(tp, solid, value);
    }
}

// ---------------------------------------------------------------------------------------------
// 6. Countdown badge: disc (r 18) with an icon, a timer arc (r 22, 2.75 px) that runs clockwise from 12 o'clock,
// and an optional arrow outside the arc pointing toward the thing it tracks.

struct BadgeStyle { ImVec4 bg = Rgba(17, 17, 17, 200), icon = Rgba(255, 255, 255, 235), arc = Rgba(255, 120, 70); };

inline void CountdownBadge(ImDrawList* dl, ImVec2 c, Glyph g, float frac, bool arrow, float dir, float alpha = 1.0f,
                           const BadgeStyle& st = BadgeStyle()) {
    const float r_icon = 18.0f, r_arc = 22.0f, arc_th = 2.75f, base_r = r_arc + 3.5f, tip_r = base_r + 11.0f, spread = 0.45f;
    dl->AddCircleFilled(c, r_icon, U(st.bg, alpha), 32);
    DrawGlyph(dl, g, c, 8.0f, U(st.icon, alpha));
    if (frac > 0.005f) {
        dl->PathArcTo(c, r_arc, -IM_PI * 0.5f, -IM_PI * 0.5f + frac * IM_PI * 2.0f, ImMax(3, (int)(32 * frac)));
        dl->PathStroke(U(st.arc, alpha), 0, arc_th);
    }
    if (arrow) {   // tip, then an arc along the base so the arrow hugs the ring
        ImVec2 pts[12];
        int k = 0;
        pts[k++] = c + ImVec2(std::cos(dir), std::sin(dir)) * tip_r;
        for (int i = 0; i <= 10; ++i) {
            const float a = dir + spread - (2.0f * spread) * i / 10.0f;
            pts[k++] = c + ImVec2(std::cos(a), std::sin(a)) * base_r;
        }
        dl->AddConvexPolyFilled(pts, k, U(st.arc, alpha));
    }
}

// Keeps a badge on screen: if `target` is outside `screen` shrunk by `pad`, slide it along the ray from the center.
inline ImVec2 ClampToEdge(ImVec2 target, ImRect screen, float pad = 52.0f) {
    const ImRect in(screen.Min + ImVec2(pad, pad), screen.Max - ImVec2(pad, pad));
    if (in.Contains(target)) return target;
    const ImVec2 c = in.GetCenter(), d = target - c;
    float t = 1.0f;
    if (d.x != 0.0f) t = ImMin(t, (d.x > 0 ? in.Max.x - c.x : in.Min.x - c.x) / d.x);
    if (d.y != 0.0f) t = ImMin(t, (d.y > 0 ? in.Max.y - c.y : in.Min.y - c.y) / d.y);
    return c + d * t;
}

// ---------------------------------------------------------------------------------------------
// 7. Ground-area markers. An area is a set of circular emitters on the ground plane (z = 0).
// A projector maps a ground point to the screen; the demo supplies its own simple perspective camera.

struct Emitter { float x, y, r; };
struct Projector {
    float ex, ey, ez, tx, ty, tz, focal; ImVec2 center;   // eye, look-at target, focal length (px), screen center
    ImVec2 operator()(float x, float y, float z = 0.0f) const {
        float fx = tx - ex, fy = ty - ey, fz = tz - ez; float l = std::sqrt(fx * fx + fy * fy + fz * fz); fx /= l; fy /= l; fz /= l;
        float rx = fy, ry = -fx, rz = 0.0f; l = std::sqrt(rx * rx + ry * ry); rx /= l; ry /= l;   // right = forward x up(z)
        const float ux = ry * fz - rz * fy, uy = rz * fx - rx * fz, uz = rx * fy - ry * fx;
        const float dx = x - ex, dy = y - ey, dz = z - ez;
        const float cx = dx * rx + dy * ry + dz * rz, cy = dx * ux + dy * uy + dz * uz, cz = ImMax(1.0f, dx * fx + dy * fy + dz * fz);
        return center + ImVec2(cx / cz * focal, -cy / cz * focal);
    }
};

struct AreaStyle {
    ImVec4 outline = Rgba(255, 40, 40), fill = Rgba(255, 40, 40, 45), edge = Rgba(255, 40, 40, 95);
    float outline_th = 2.5f, glow = 0.55f;
};

// Method A, world-space floor mesh. Cast `rays` rays from the area centroid; each radius is the farthest exit
// from the union of the discs. Smooth over angle, ease over time (speed 6, so growth looks organic), then build
// the mesh on the ground and project each vertex. Perspective-correct, keeps concave shapes, and the
// soft edge stays the same width in world units.
inline void AreaMarkerMesh(ImDrawList* dl, const char* id, const Emitter* em, int n, const Projector& proj,
                           float alpha = 1.0f, const AreaStyle& st = AreaStyle(), float feather = 14.0f) {
    constexpr int rays = 64;
    float cx = 0, cy = 0;
    for (int i = 0; i < n; ++i) { cx += em[i].x; cy += em[i].y; }
    cx /= (float)n; cy /= (float)n;
    float target[rays];
    for (int k = 0; k < rays; ++k) {
        const float a = IM_PI * 2.0f * k / rays, dx = std::cos(a), dy = std::sin(a);
        float best = 0.0f;
        for (int i = 0; i < n; ++i) {   // ray-circle: farthest intersection
            const float ox = em[i].x - cx, oy = em[i].y - cy, along = ox * dx + oy * dy;
            const float perp2 = ox * ox + oy * oy - along * along, r2 = em[i].r * em[i].r;
            if (perp2 < r2) best = ImMax(best, along + std::sqrt(r2 - perp2));
        }
        target[k] = best;
    }
    for (int pass = 0; pass < 2; ++pass) {   // neighbour smoothing rounds off the seams between discs
        float tmp[rays];
        for (int k = 0; k < rays; ++k) tmp[k] = (target[(k + rays - 1) % rays] + 2.0f * target[k] + target[(k + 1) % rays]) * 0.25f;
        std::copy(tmp, tmp + rays, target);
    }
    const float lt = menu::ReduceMotion() ? 1.0f : ImMin(6.0f * ImGui::GetIO().DeltaTime, 1.0f);
    ImVec2 edge[rays], inner[rays], outer[rays];
    for (int k = 0; k < rays; ++k) {
        const ImGuiID kid = Key(id, k);
        const float r = Store().GetFloat(kid, target[k] * 0.2f) + (target[k] - Store().GetFloat(kid, target[k] * 0.2f)) * lt;
        Store().SetFloat(kid, r);
        const float a = IM_PI * 2.0f * k / rays, dx = std::cos(a), dy = std::sin(a);
        edge[k]  = proj(cx + dx * r, cy + dy * r);
        inner[k] = proj(cx + dx * ImMax(0.0f, r - feather * 2.0f), cy + dy * ImMax(0.0f, r - feather * 2.0f));
        outer[k] = proj(cx + dx * (r + feather), cy + dy * (r + feather));
    }
    Fan(dl, proj(cx, cy), inner, rays, U(st.fill, alpha), U(st.fill, alpha));   // flat interior
    RingBand(dl, inner, edge, rays, U(st.fill, alpha), U(st.edge, alpha));      // denser toward the rim
    RingBand(dl, edge, outer, rays, U(st.outline, alpha * st.glow * 0.6f), U(st.outline, 0.0f));   // outer glow
    dl->AddPolyline(edge, rays, U(st.outline, alpha), ImDrawFlags_Closed, st.outline_th);
}

// Method B, screen-space hull (velocity.cat). Project 15 points around each emitter, take the convex hull on
// screen, draw it as a closed outline, and fake a radial fill with 8 copies shrunk toward the centroid at
// falling alpha. Cheap and robust, but always convex, and it isn't foreshortened like a real floor decal.
inline void ConvexHull(std::vector<ImVec2>& pts) {   // Andrew's monotone chain, in place, counter-clockwise
    if (pts.size() < 3) return;
    std::sort(pts.begin(), pts.end(), [](const ImVec2& a, const ImVec2& b) { return a.x < b.x || (a.x == b.x && a.y < b.y); });
    std::vector<ImVec2> h(pts.size() * 2);
    size_t k = 0;
    auto cross = [](ImVec2 o, ImVec2 a, ImVec2 b) { return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x); };
    for (size_t i = 0; i < pts.size(); ++i) { while (k >= 2 && cross(h[k - 2], h[k - 1], pts[i]) <= 0) --k; h[k++] = pts[i]; }
    for (size_t i = pts.size() - 1, t = k + 1; i > 0; --i) { while (k >= t && cross(h[k - 2], h[k - 1], pts[i - 1]) <= 0) --k; h[k++] = pts[i - 1]; }
    h.resize(k - 1);
    pts.swap(h);
}

inline void AreaMarkerHull(ImDrawList* dl, const Emitter* em, int n, const Projector& proj, float alpha = 1.0f,
                           const AreaStyle& st = AreaStyle()) {
    std::vector<ImVec2> pts;
    for (int i = 0; i < n; ++i)
        for (int d = 0; d < 15; ++d) {
            const float a = IM_PI * 2.0f * d / 15.0f;
            pts.push_back(proj(em[i].x + std::cos(a) * em[i].r, em[i].y + std::sin(a) * em[i].r));
        }
    ConvexHull(pts);
    if (pts.size() < 3) return;
    ImVec2 c(0, 0);
    for (const ImVec2& p : pts) c += p;
    c = c / (float)pts.size();
    std::vector<ImVec2> shrunk(pts.size());
    for (int ring = 0; ring < 8; ++ring) {
        const float t = ring / 8.0f;
        for (size_t i = 0; i < pts.size(); ++i) shrunk[i] = ImLerp(pts[i], c, t);
        dl->AddConvexPolyFilled(shrunk.data(), (int)shrunk.size(), U(st.fill, alpha * st.glow * (1.0f - t)));
    }
    dl->AddPolyline(pts.data(), (int)pts.size(), U(st.outline, alpha * st.glow * 0.5f), ImDrawFlags_Closed, st.outline_th + 2.0f);
    dl->AddPolyline(pts.data(), (int)pts.size(), U(st.outline, alpha), ImDrawFlags_Closed, st.outline_th);
}

// ---------------------------------------------------------------------------------------------
// 8. Info bar: one flat strip of icon + value segments, brand first. Icons carry the accent; text stays neutral.
// The drawn width eases toward the content width, so a changing digit doesn't make a centered bar jump.

struct InfoItem { Glyph glyph; const char* text; };
struct InfoBarStyle {
    float h = 22.0f, rounding = 6.0f, pad_x = 10.0f, icon = 5.0f, icon_gap = 5.0f, gap = 12.0f;
    ImVec4 bg = Rgba(0, 0, 0, 102);          // ~40% black (sampled #2c231f over a #4a3e33 floor, approximate)
    ImVec4 text = Rgba(236, 232, 228);       // light warm grey (approximate)
    ImVec4 accent = Rgba(240, 100, 106);     // icon red #f0646a (approximate)
    ImVec4 brand = Rgba(255, 255, 255);
};

inline float InfoBarWidth(const char* brand, const InfoItem* items, int n, ImFont* bold = nullptr,
                          const InfoBarStyle& st = InfoBarStyle()) {
    float w = st.pad_x * 2.0f;
    if (brand) {
        if (bold) ImGui::PushFont(bold, 0.0f);
        w += ImGui::CalcTextSize(brand).x;
        if (bold) ImGui::PopFont();
    }
    for (int i = 0; i < n; ++i)
        w += ((i > 0 || brand) ? st.gap : 0.0f) + st.icon * 2.0f + st.icon_gap + ImGui::CalcTextSize(items[i].text).x;
    return w;
}

// `anchor` + `pivot` place the bar like SetNextWindowPos: (0.5, 1) = bottom-center, (0.5, 0) = top-center,
// (1, 0) = top-right. `id` keys the width spring.
inline void InfoBar(ImDrawList* dl, const char* id, ImVec2 anchor, ImVec2 pivot, const char* brand, const InfoItem* items,
                    int n, float alpha = 1.0f, ImFont* bold = nullptr, const InfoBarStyle& st = InfoBarStyle()) {
    const float target = InfoBarWidth(brand, items, n, bold, st);
    const float w = SpringTo(Key(id), target, 300.0f, 35.0f);
    const ImVec2 p0(anchor.x - w * pivot.x, anchor.y - st.h * pivot.y), p1(p0.x + w, p0.y + st.h);
    dl->AddRectFilled(p0, p1, U(st.bg, alpha), st.rounding);

    float x = p0.x + st.pad_x + (w - target) * 0.5f;   // content stays centered while the width catches up
    const float cy = p0.y + st.h * 0.5f;
    if (brand) {
        if (bold) ImGui::PushFont(bold, 0.0f);
        const ImVec2 ts = ImGui::CalcTextSize(brand);
        dl->AddText(ImVec2(x, cy - ts.y * 0.5f), U(st.brand, alpha), brand);
        if (bold) ImGui::PopFont();
        x += ts.x;
    }
    for (int i = 0; i < n; ++i) {
        if (i > 0 || brand) x += st.gap;
        DrawGlyph(dl, items[i].glyph, ImVec2(x + st.icon, cy), st.icon, U(st.accent, alpha));
        x += st.icon * 2.0f + st.icon_gap;
        const ImVec2 ts = ImGui::CalcTextSize(items[i].text);
        dl->AddText(ImVec2(x, cy - ts.y * 0.5f), U(st.text, alpha), items[i].text);
        x += ts.x;
    }
}

// ---------------------------------------------------------------------------------------------
// 9. Indicator cards (Neverlose Recurrence, exact values from the Figma file): a 70% black card with a 2 px
// accent bar on its left edge that fades in from 10% of the height, is solid at 50% and gone by 90%. The bar is
// clipped to the card's rounded corner. Icon, bar and status share one state color; a two-line card keeps its
// title white. IndicatorStack fades cards in and out and springs the ones below into the freed slot.

// Bar alpha along the height t (0..1): 0 until a, ramps to 1 at b, holds to c, back to 0 at d.
struct BarStops { float a = 0.10f, b = 0.50f, c = 0.50f, d = 0.90f; };
inline float BarAlpha(float t, const BarStops& s) {
    if (t <= s.a || t >= s.d) return 0.0f;
    if (t < s.b) return (t - s.a) / ImMax(s.b - s.a, 1e-4f);
    if (t <= s.c) return 1.0f;
    return 1.0f - (t - s.c) / ImMax(s.d - s.c, 1e-4f);
}

// Fading bar `w` wide on the left edge of the rounded rect p0..p1, clipped by its corner radius `r`.
// Drawn as horizontal slices with per-vertex alpha, so it needs no clip rect and follows the curve exactly.
inline void AccentBar(ImDrawList* dl, ImVec2 p0, ImVec2 p1, float r, float w, ImVec4 col, float alpha = 1.0f,
                      const BarStops& stops = BarStops(), int slices = 24) {
    const float h = p1.y - p0.y;
    r = ImMin(r, ImMin(h, p1.x - p0.x) * 0.5f);
    auto edge = [&](float y) {   // x inset of the rounded left edge at height y (0..h)
        const float dy = y < r ? r - y : (y > h - r ? y - (h - r) : 0.0f);
        return r - ImSqrt(ImMax(r * r - dy * dy, 0.0f));
    };
    const ImVec2 uv = ImGui::GetFontTexUvWhitePixel();
    for (int i = 0; i < slices; ++i) {
        const float y0 = h * i / slices, y1 = h * (i + 1) / slices;
        const float x0 = ImMax(edge(y0), edge(y1));
        if (x0 >= w) continue;
        const ImU32 c0 = U(col, alpha * BarAlpha(y0 / h, stops)), c1 = U(col, alpha * BarAlpha(y1 / h, stops));
        if (((c0 | c1) & IM_COL32_A_MASK) == 0) continue;
        const ImDrawIdx i0 = (ImDrawIdx)dl->_VtxCurrentIdx;
        dl->PrimReserve(6, 4);
        dl->PrimWriteVtx(ImVec2(p0.x + x0, p0.y + y0), uv, c0); dl->PrimWriteVtx(ImVec2(p0.x + w, p0.y + y0), uv, c0);
        dl->PrimWriteVtx(ImVec2(p0.x + w, p0.y + y1), uv, c1); dl->PrimWriteVtx(ImVec2(p0.x + x0, p0.y + y1), uv, c1);
        dl->PrimWriteIdx(i0); dl->PrimWriteIdx(i0 + 1); dl->PrimWriteIdx(i0 + 2);
        dl->PrimWriteIdx(i0); dl->PrimWriteIdx(i0 + 2); dl->PrimWriteIdx(i0 + 3);
    }
}

struct IndicatorStyle {
    // one-line card: 22 tall, radius 10, icon box 14.5 at x 7, text at x 29
    float h1 = 22.0f, r1 = 10.0f, icon1 = 7.25f, text_x1 = 29.0f;
    // two-line card: 44 tall, radius 12, icon box 20 at x 7, lines centered at 30% and 70% of the height
    float h2 = 44.0f, r2 = 12.0f, icon2 = 10.0f, text_x2 = 36.0f, line1 = 0.30f, line2 = 0.70f;
    float icon_x = 7.0f, pad_r = 10.0f, bar_w = 2.0f, spacing = 10.0f;
    float fade_in = 0.2f, fade_out = 0.15f, stiffness = 200.0f, damping = 20.0f;
    ImVec4 bg = Rgba(0, 0, 0, 178);          // black 70%
    ImVec4 halo = Rgba(0, 0, 0, 80);         // Figma: black 60%, blur 5, no offset; a blur that size is ~30% at the edge
    float halo_spread = 7.0f;
    ImVec4 title = Rgba(255, 255, 255);
    BarStops stops;
};

// State colors from the file. Muted per indicator; only critical is saturated.
inline ImVec4 IndicatorCritical() { return Rgba(213, 34, 54); }    // #d52236
inline ImVec4 IndicatorWarning()  { return Rgba(194, 116, 99); }   // #c27463 muted salmon
inline ImVec4 IndicatorNeutral()  { return Rgba(152, 164, 184); }  // #98a4b8 steel
inline ImVec4 IndicatorOk()       { return Rgba(180, 164, 131); }  // #b4a483 tan (a spectator bar in the file)

// Width of a card, so a caller can right-align or measure a stack.
inline float IndicatorWidth(const char* title, const char* status, ImFont* font = nullptr,
                            const IndicatorStyle& st = IndicatorStyle()) {
    if (font) ImGui::PushFont(font, 0.0f);
    float tw = ImGui::CalcTextSize(title).x;
    if (status) tw = ImMax(tw, ImGui::CalcTextSize(status).x);
    if (font) ImGui::PopFont();
    return (status ? st.text_x2 : st.text_x1) + tw + st.pad_r;
}

// One card at `pos` (top-left). status == nullptr draws the one-line card with `title` in the state color.
// Returns the card's size.
inline ImVec2 IndicatorCard(ImDrawList* dl, ImVec2 pos, Glyph g, const char* title, const char* status, ImVec4 state,
                            float alpha = 1.0f, ImFont* font = nullptr, const IndicatorStyle& st = IndicatorStyle()) {
    const bool two = status != nullptr;
    const float h = two ? st.h2 : st.h1, r = two ? st.r2 : st.r1, icon = two ? st.icon2 : st.icon1;
    const ImVec2 size(IndicatorWidth(title, status, font, st), h), p1 = pos + size;
    menu::SoftShadow(dl, pos, p1, r, ImVec4(st.halo.x, st.halo.y, st.halo.z, st.halo.w * alpha), st.halo_spread, 7);
    dl->AddRectFilled(pos, p1, U(st.bg, alpha), r);
    AccentBar(dl, pos, p1, r, st.bar_w, state, alpha, st.stops);
    DrawGlyph(dl, g, ImVec2(pos.x + st.icon_x + icon, pos.y + h * 0.5f), icon * 0.8f, U(state, alpha));
    if (font) ImGui::PushFont(font, 0.0f);
    const float tx = pos.x + (two ? st.text_x2 : st.text_x1);
    if (two) {
        const float lh = ImGui::GetTextLineHeight();
        dl->AddText(ImVec2(tx, pos.y + h * st.line1 - lh * 0.5f), U(st.title, alpha), title);
        dl->AddText(ImVec2(tx, pos.y + h * st.line2 - lh * 0.5f), U(state, alpha), status);
    } else {
        dl->AddText(ImVec2(tx, pos.y + (h - ImGui::GetTextLineHeight()) * 0.5f), U(state, alpha), title);
    }
    if (font) ImGui::PopFont();
    return size;
}

struct Indicator { Glyph glyph; const char* title; const char* status; ImVec4 state; bool active; };

// Left-aligned stack from `anchor` (top-left), top-down. Inactive cards fade out; the cards below spring up.
// `id` keys the animation state, so two stacks can coexist.
inline void IndicatorStack(ImDrawList* dl, const char* id, ImVec2 anchor, const Indicator* items, int n,
                           float alpha = 1.0f, ImFont* font = nullptr, const IndicatorStyle& st = IndicatorStyle()) {
    float slot = 0.0f;
    for (int i = 0; i < n; ++i) {
        const ImGuiID k = Key(id, i);
        const bool was_hidden = Store().GetFloat(k + 1, 0.0f) <= 0.001f;
        const float a = FadeTo(k + 1, items[i].active, st.fade_in, st.fade_out);
        const float h = items[i].status ? st.h2 : st.h1;
        if (a > 0.001f) {
            const float y = SpringTo(k + 2, slot, st.stiffness, st.damping, was_hidden);
            const float slide = (1.0f - a) * -8.0f;   // enter from slightly left
            IndicatorCard(dl, anchor + ImVec2(slide, y), items[i].glyph, items[i].title, items[i].status,
                          items[i].state, alpha * a, font, st);
        }
        if (items[i].active) slot += h + st.spacing;
    }
}

// ---------------------------------------------------------------------------------------------
// Demo: every element at once over a perspective floor grid, with placeholder labels. Call once per frame.

inline void DrawHudDemo(ImFont* bold = nullptr) {
    const ImGuiIO& io = ImGui::GetIO();
    const double now = ImGui::GetTime();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("##hud_demo", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |
                                        ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings);
    ImDrawList* dl = ImGui::GetWindowDrawList();
    const ImVec2 ds = io.DisplaySize;

    // floor grid with a demo camera
    const Projector proj = { 0.0f, -560.0f, 330.0f, 0.0f, 120.0f, 0.0f, 640.0f, ImVec2(ds.x * 0.5f, ds.y * 0.42f) };
    for (int i = -8; i <= 8; ++i) {
        dl->AddLine(proj(i * 60.0f, -120.0f), proj(i * 60.0f, 700.0f), IM_COL32(255, 255, 255, 18));
    }
    for (int j = -2; j <= 11; ++j) dl->AddLine(proj(-480.0f, j * 60.0f), proj(480.0f, j * 60.0f), IM_COL32(255, 255, 255, 18));

    const Emitter area[] = { { -50, 40, 62 }, { 30, 90, 74 }, { 85, 10, 52 }, { -10, 165, 48 }, { -95, 120, 40 } };
    Emitter left[5], right[5];
    for (int i = 0; i < 5; ++i) { left[i] = area[i]; left[i].x -= 210.0f; right[i] = area[i]; right[i].x += 230.0f; }
    AreaMarkerMesh(dl, "area_a", left, 5, proj);
    AreaMarkerHull(dl, right, 5, proj);

    // status pills, left
    const float t_on = 0.5f + 0.5f * (float)std::sin(now * 1.3);
    ImVec2 p(18.0f, ds.y * 0.5f - 70.0f);
    p.y += StatusPill(dl, p, GlyphLock, "LOCK", ImSaturate(t_on * 1.6f), 1.0f, bold).y + 12.0f;
    p.y += StatusPill(dl, p, GlyphCrosshair, "LIMIT 26", 0.0f, 1.0f, bold).y + 12.0f;
    p.y += StatusPill(dl, p, GlyphTarget, "RANGE 100", 0.0f, 1.0f, bold).y + 12.0f;
    p.y += StatusPill(dl, p, GlyphSteps, "STEP", 0.0f, 1.0f, bold).y + 24.0f;

    // indicator stack under the pills; the first card leaves and returns so the others spring up and back
    const Indicator ind[] = { { GlyphTarget, "OPTION A", nullptr, IndicatorWarning(), std::fmod(now, 5.0) < 3.4 },
                              { GlyphFlame, "A - 29.6s", "STATUS 9m", IndicatorCritical(), true },
                              { GlyphCrosshair, "OPTION B", nullptr, IndicatorNeutral(), true } };
    IndicatorStack(dl, "indicators", p, ind, 3, 1.0f, bold);

    // keybind list, right
    const bool third_on = std::fmod(now, 4.0) < 2.6;
    const KeybindRow rows[] = { { "option a", "t", true }, { "option b", "mouse 3", third_on }, { "option c", "space", true } };
    KeybindList(dl, ImVec2(ds.x - 18.0f, ds.y * 0.5f - 70.0f), "keybinds", rows, 3);

    // segmented capsule watermark, top-right; width follows content on a spring
    static const Span brand[] = { { "brand", true } }, user[] = { { "user", true } };
    static const Span ping[] = { { "24", true }, { " ms", false } }, fps[] = { { "240", true }, { " fps", false } };
    const Cell wm[] = { { GlyphTarget, true, brand, 1 }, { GlyphCheck, false, user, 1 }, { GlyphCheck, false, ping, 2 }, { GlyphCheck, false, fps, 2 } };
    const float wm_w = SpringTo(Key("wm_w"), CapsuleWidth(wm, 4), 120.0f, 22.0f);
    Capsule(dl, ImVec2(ds.x - 10.0f - wm_w, 10.0f), wm_w, wm, 4);

    // toasts, top-left
    static const Span s1[] = { { "changed ", false }, { "option a", true }, { " to ", false }, { "on", true } };
    static const Span s2[] = { { "saved ", false }, { "profile 2", true } };
    static const Span s3[] = { { "reached ", false }, { "goal", true }, { " in ", false }, { "12.4s", true } };
    static Toast toasts[] = { { 0.0, 6.0f, GlyphCheck, s1, 4 }, { 0.35, 6.0f, GlyphCheck, s2, 2 }, { 0.8, 6.0f, GlyphCross, s3, 4 } };
    static bool spawned[3] = {};
    for (int i = 0; i < 3; ++i) if (!spawned[i] && now >= toasts[i].born) { ToastSpawn(toasts[i]); spawned[i] = true; }
    ToastStack(dl, ImVec2(15.0f, 15.0f), toasts, 3, now);

    // hit marker (loops) and a countdown badge clamped to the bottom edge
    HitMarker(dl, ImVec2(ds.x * 0.5f, ds.y * 0.5f - 40.0f), (float)std::fmod(now, 1.2), 0.6f, Rgba(255, 255, 255), "42");
    const ImVec2 off_screen(ds.x * 0.62f, ds.y + 300.0f);
    const ImVec2 bp = ClampToEdge(off_screen, ImRect(ImVec2(0, 0), ds));
    const ImVec2 to = off_screen - bp;
    CountdownBadge(dl, bp, GlyphFlame, 1.0f - (float)std::fmod(now * 0.15, 1.0), true, std::atan2(to.y, to.x));
    CountdownBadge(dl, ImVec2(ds.x * 0.5f + 130.0f, ds.y * 0.5f + 150.0f), GlyphTarget, 0.35f, false, 0.0f);

    // info bar, top-center; the frame-rate text changes length to show the width easing
    char fps_text[32];
    ImFormatString(fps_text, sizeof(fps_text), "%d FPS %d (1%%L)", 90 + (int)(170.0 * (0.5 + 0.5 * std::sin(now * 0.9))), 84);
    const InfoItem info[] = { { GlyphBars, fps_text }, { GlyphSignal, "12 MS" }, { GlyphChip, "32% / 79% CPU" },
                              { GlyphClock, "23:37" }, { GlyphUser, "user" } };
    InfoBar(dl, "info_bar", ImVec2(ds.x * 0.5f, 10.0f), ImVec2(0.5f, 0.0f), "HUD", info, 5, 1.0f, bold);

    ImGui::End();
}

} // namespace hud
