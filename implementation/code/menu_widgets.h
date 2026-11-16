// menu_widgets.h — custom controls: CheckRow, PillToggle, AccentSlider, ComboRow, ColorRow, HeaderTabs, group boxes.
// Every control spans the full row: label left, control right-aligned.
#pragma once
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"
#include "imgui_internal.h"
#include "menu_style.h"
#include "menu_anim.h"

namespace menu {

// Reserves a full-width row and returns its rect. Returns false if clipped.
inline bool RowBegin(const char* label, ImGuiID* out_id, ImRect* out_bb) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;
    const ImGuiID id = window->GetID(label);
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect bb(pos, pos + ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight()));
    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id)) return false;
    *out_id = id; *out_bb = bb;
    return true;
}

inline void RowLabel(ImDrawList* dl, const ImRect& bb, const char* label, float hover_t) {
    const Palette& p = Theme();
    const char* end = ImGui::FindRenderedTextEnd(label);
    const ImVec2 ts = ImGui::CalcTextSize(label, end);
    const ImVec4 col = ImLerp(p.label, p.text, hover_t * 0.6f);
    dl->AddText(ImVec2(bb.Min.x, bb.Min.y + (bb.GetHeight() - ts.y) * 0.5f), ImGui::GetColorU32(col), label, end);
}

// Fatality-style: small inset square, accent checkmark when on.
inline bool CheckRow(const char* label, bool* v) {
    ImGuiID id; ImRect bb;
    if (!RowBegin(label, &id, &bb)) return false;
    bool hovered, held;
    const bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
    if (pressed) { *v = !*v; ImGui::MarkItemEdited(id); }

    const Palette& p = Theme();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    RowLabel(dl, bb, label, Anim(SubId(id, "hover"), hovered));

    const float box = 11.0f;
    const ImVec2 bmin(bb.Max.x - box, bb.Min.y + (bb.GetHeight() - box) * 0.5f);
    dl->AddRectFilled(bmin, bmin + ImVec2(box, box), ImGui::GetColorU32(p.inset), 2.0f);

    const float t = Anim(SubId(id, "on"), *v);
    if (t > 0.01f) {
        const ImVec2 pts[3] = { bmin + ImVec2(box * 0.20f, box * 0.52f),
                                bmin + ImVec2(box * 0.42f, box * 0.74f),
                                bmin + ImVec2(box * 0.82f, box * 0.26f) };
        dl->AddPolyline(pts, 3, ImGui::GetColorU32(WithAlpha(p.accent, t)), ImDrawFlags_None, 1.6f);
    }
    return pressed;
}

// neverlose-style pill switch with sliding knob.
inline bool PillToggle(const char* label, bool* v) {
    ImGuiID id; ImRect bb;
    if (!RowBegin(label, &id, &bb)) return false;
    bool hovered, held;
    const bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
    if (pressed) { *v = !*v; ImGui::MarkItemEdited(id); }

    const Palette& p = Theme();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    RowLabel(dl, bb, label, Anim(SubId(id, "hover"), hovered));

    const float h = 14.0f, w = 28.0f, r = h * 0.5f - 2.0f;
    const ImVec2 tmin(bb.Max.x - w, bb.Min.y + (bb.GetHeight() - h) * 0.5f);
    const float t = Anim(SubId(id, "on"), *v);
    dl->AddRectFilled(tmin, tmin + ImVec2(w, h), ImGui::GetColorU32(ImLerp(p.inset, p.accent, t)), h * 0.5f);
    const float cx = ImLerp(tmin.x + 2.0f + r, tmin.x + w - 2.0f - r, t);
    dl->AddCircleFilled(ImVec2(cx, tmin.y + h * 0.5f), r, ImGui::GetColorU32(ImLerp(p.label, p.text, t)));
    return pressed;
}

// Thin accent bar with the value printed in the middle. Click or drag to set.
inline bool AccentSlider(const char* label, float* v, float v_min, float v_max,
                         const char* fmt = "%.0f%%", float bar_w = 90.0f) {
    ImGuiID id; ImRect bb;
    if (!RowBegin(label, &id, &bb)) return false;

    const float bar_h = 10.0f;
    const ImRect bar(ImVec2(bb.Max.x - bar_w, bb.Min.y + (bb.GetHeight() - bar_h) * 0.5f),
                     ImVec2(bb.Max.x, bb.Min.y + (bb.GetHeight() + bar_h) * 0.5f));
    bool hovered, held;
    ImGui::ButtonBehavior(bar, id, &hovered, &held, ImGuiButtonFlags_PressedOnClick);

    bool changed = false;
    if (held) {
        const float f = ImSaturate((ImGui::GetIO().MousePos.x - bar.Min.x) / bar.GetWidth());
        const float nv = v_min + f * (v_max - v_min);
        if (nv != *v) { *v = nv; changed = true; ImGui::MarkItemEdited(id); }
    }

    const Palette& p = Theme();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    RowLabel(dl, bb, label, Anim(SubId(id, "hover"), hovered || held));

    const float frac = (v_max > v_min) ? ImSaturate((*v - v_min) / (v_max - v_min)) : 0.0f;
    const float shown = AnimTo(SubId(id, "fill"), frac, 18.0f);
    dl->AddRectFilled(bar.Min, bar.Max, ImGui::GetColorU32(p.inset), bar_h * 0.5f);
    if (shown > 0.0f)
        dl->AddRectFilled(bar.Min, ImVec2(bar.Min.x + bar.GetWidth() * shown, bar.Max.y),
                          ImGui::GetColorU32(p.accent), bar_h * 0.5f);

    char buf[32];
    ImFormatString(buf, IM_ARRAYSIZE(buf), fmt, *v);
    const ImVec2 ts = ImGui::CalcTextSize(buf);
    dl->AddText(bar.GetCenter() - ts * 0.5f, ImGui::GetColorU32(p.text), buf);
    return changed;
}

// int convenience wrapper
inline bool AccentSliderInt(const char* label, int* v, int v_min, int v_max, const char* fmt = "%.0f") {
    float f = (float)*v;
    const bool changed = AccentSlider(label, &f, (float)v_min, (float)v_max, fmt);
    if (changed) *v = (int)(f + 0.5f);
    return changed;
}

// Value box right-aligned on the row (current item + small chevron). Click opens the list flush under
// the box; the selected entry is drawn in the accent, the hovered one on a lighter inset.
inline bool ComboRow(const char* label, int* current, const char* const* items, int count, float box_w = 110.0f) {
    ImGuiID id; ImRect bb;
    if (count <= 0 || !RowBegin(label, &id, &bb)) return false;

    const float box_h = ImMin(bb.GetHeight() - 2.0f, 18.0f);
    const ImRect box(ImVec2(bb.Max.x - box_w, bb.Min.y + (bb.GetHeight() - box_h) * 0.5f),
                     ImVec2(bb.Max.x, bb.Min.y + (bb.GetHeight() + box_h) * 0.5f));
    const ImGuiID popup_id = SubId(id, "list");
    bool hovered, held;
    if (ImGui::ButtonBehavior(box, id, &hovered, &held)) ImGui::OpenPopupEx(popup_id);
    const bool open = ImGui::IsPopupOpen(popup_id, ImGuiPopupFlags_None);

    const Palette& p = Theme();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float hov = Anim(SubId(id, "hover"), hovered || open);
    RowLabel(dl, bb, label, hov);
    dl->AddRectFilled(box.Min, box.Max, ImGui::GetColorU32(ImLerp(p.inset, p.surface, hov * 0.5f)), 3.0f);
    if (open) dl->AddRect(box.Min, box.Max, ImGui::GetColorU32(WithAlpha(p.accent, 0.6f)), 3.0f);

    const char* cur = (*current >= 0 && *current < count) ? items[*current] : "";
    const ImVec2 cs = ImGui::CalcTextSize(cur);
    dl->PushClipRect(box.Min, box.Max - ImVec2(16.0f, 0.0f), true);
    dl->AddText(ImVec2(box.Min.x + 7.0f, box.Min.y + ImFloor((box_h - cs.y) * 0.5f)),
                ImGui::GetColorU32(ImLerp(p.label, p.text, hov)), cur);
    dl->PopClipRect();
    const float aw = 6.0f, ah = 3.0f, ax = box.Max.x - aw - 7.0f, ay = box.Min.y + ImFloor((box_h - ah) * 0.5f);
    const ImVec2 down[3] = { ImVec2(ax, ay), ImVec2(ax + aw * 0.5f, ay + ah), ImVec2(ax + aw, ay) };
    const ImVec2 up[3]   = { ImVec2(ax, ay + ah), ImVec2(ax + aw * 0.5f, ay), ImVec2(ax + aw, ay + ah) };
    dl->AddPolyline(open ? up : down, 3, ImGui::GetColorU32(open ? p.accent : p.label), ImDrawFlags_None, 1.4f);

    bool changed = false;
    if (open) {
        const float item_h = box_h + 2.0f;
        ImGui::SetNextWindowPos(ImVec2(box.Min.x, box.Max.y + 2.0f));
        ImGui::SetNextWindowSize(ImVec2(box_w, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.0f, 3.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        if (ImGui::BeginPopupEx(popup_id, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                                          ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGuiWindow* pw = ImGui::GetCurrentWindow();
            for (int i = 0; i < count; ++i) {
                ImGui::PushID(i);
                const ImVec2 ip = pw->DC.CursorPos;
                const ImRect ib(ip, ip + ImVec2(ImGui::GetContentRegionAvail().x, item_h));
                const ImGuiID iid = pw->GetID("item");
                ImGui::ItemSize(ib);
                if (ImGui::ItemAdd(ib, iid)) {
                    bool ih, iheld;
                    if (ImGui::ButtonBehavior(ib, iid, &ih, &iheld)) { *current = i; changed = true; ImGui::CloseCurrentPopup(); }
                    const float ihv = Anim(SubId(iid, "hover"), ih);
                    if (ihv > 0.01f) pw->DrawList->AddRectFilled(ib.Min, ib.Max, ImGui::GetColorU32(WithAlpha(p.inset, ihv)), 2.0f);
                    const ImVec2 is = ImGui::CalcTextSize(items[i]);
                    const ImVec4 tc = (i == *current) ? p.accent : ImLerp(p.label, p.text, ihv);
                    pw->DrawList->AddText(ImVec2(ib.Min.x + 6.0f, ib.Min.y + ImFloor((item_h - is.y) * 0.5f)),
                                          ImGui::GetColorU32(tc), items[i]);
                }
                ImGui::PopID();
            }
            ImGui::EndPopup();
        }
        ImGui::PopStyleVar(2);
    }
    return changed;
}

// Hand-drawn picker body (after the neverlose layout, see widgets/color-pickers.md): SV square, hue bar
// with a pill knob, optional alpha bar, hex readout. Hue is kept in state storage so it survives grey colors.
inline bool PickerBody(ImVec4* col, bool alpha, float w = 168.0f) {
    ImGuiWindow* win = ImGui::GetCurrentWindow();
    const Palette& p = Theme();
    ImDrawList* dl = win->DrawList;
    const ImGuiID hue_id = win->GetID("##hue_keep");
    float h, s, v;
    ImGui::ColorConvertRGBtoHSV(col->x, col->y, col->z, h, s, v);
    if (s <= 0.0001f || v <= 0.0001f) h = win->StateStorage.GetFloat(hue_id, h);
    bool changed = false;

    // SV square: white -> pure hue left to right, then clear -> black top to bottom.
    const ImVec2 o = win->DC.CursorPos;
    const ImRect sv(o, o + ImVec2(w, w * 0.72f));
    ImGui::ItemSize(sv);
    const ImGuiID sv_id = win->GetID("##sv");
    ImGui::ItemAdd(sv, sv_id);
    bool hov, held;
    ImGui::ButtonBehavior(sv, sv_id, &hov, &held, ImGuiButtonFlags_PressedOnClick);
    if (held) {
        const ImVec2 m = ImGui::GetIO().MousePos;
        s = ImSaturate((m.x - sv.Min.x) / sv.GetWidth());
        v = 1.0f - ImSaturate((m.y - sv.Min.y) / sv.GetHeight());
        changed = true;
    }
    ImVec4 pure(0, 0, 0, 1);
    ImGui::ColorConvertHSVtoRGB(h, 1.0f, 1.0f, pure.x, pure.y, pure.z);
    const ImU32 white = IM_COL32_WHITE, hue_c = ImGui::GetColorU32(pure);
    dl->AddRectFilledMultiColor(sv.Min, sv.Max, white, hue_c, hue_c, white);
    dl->AddRectFilledMultiColor(sv.Min, sv.Max, IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0), IM_COL32_BLACK, IM_COL32_BLACK);
    const ImVec2 sk(sv.Min.x + s * sv.GetWidth(), sv.Min.y + (1.0f - v) * sv.GetHeight());
    dl->AddCircle(sk, 4.5f, IM_COL32_BLACK, 12, 2.5f);
    dl->AddCircle(sk, 4.5f, IM_COL32_WHITE, 12, 1.5f);

    // Bars: hue (six segments) and alpha (checkerboard under a clear -> opaque gradient).
    const float bar_h = 8.0f, gap = 8.0f;
    auto bar = [&](const char* sid, float* t) {
        ImGui::Dummy(ImVec2(0.0f, gap - ImGui::GetStyle().ItemSpacing.y));
        const ImVec2 bo = win->DC.CursorPos;
        const ImRect bb(bo, bo + ImVec2(w, bar_h));
        ImGui::ItemSize(bb);
        const ImGuiID bid = win->GetID(sid);
        ImGui::ItemAdd(bb, bid);
        bool bh, bheld;
        ImGui::ButtonBehavior(bb, bid, &bh, &bheld, ImGuiButtonFlags_PressedOnClick);
        if (bheld) { *t = ImSaturate((ImGui::GetIO().MousePos.x - bb.Min.x) / bb.GetWidth()); changed = true; }
        return bb;
    };
    const ImRect hb = bar("##hue", &h);
    static const ImU32 seg[7] = { IM_COL32(255,0,0,255), IM_COL32(255,255,0,255), IM_COL32(0,255,0,255),
                                  IM_COL32(0,255,255,255), IM_COL32(0,0,255,255), IM_COL32(255,0,255,255), IM_COL32(255,0,0,255) };
    for (int i = 0; i < 6; ++i) {
        const float x0 = hb.Min.x + hb.GetWidth() * i / 6.0f, x1 = hb.Min.x + hb.GetWidth() * (i + 1) / 6.0f;
        dl->AddRectFilledMultiColor(ImVec2(x0, hb.Min.y), ImVec2(x1, hb.Max.y), seg[i], seg[i + 1], seg[i + 1], seg[i]);
    }
    const float hx = hb.Min.x + h * hb.GetWidth();
    dl->AddRectFilled(ImVec2(hx - 3.0f, hb.Min.y - 2.0f), ImVec2(hx + 3.0f, hb.Max.y + 2.0f), IM_COL32_WHITE, 3.0f);

    float a = col->w;
    if (alpha) {
        const ImRect ab = bar("##alpha", &a);
        for (float x = ab.Min.x; x < ab.Max.x; x += 4.0f)
            for (int r = 0; r < 2; ++r)
                dl->AddRectFilled(ImVec2(x, ab.Min.y + r * 4.0f), ImVec2(ImMin(x + 4.0f, ab.Max.x), ab.Min.y + r * 4.0f + 4.0f),
                                  ((int)((x - ab.Min.x) / 4.0f) + r) % 2 ? IM_COL32(200, 200, 200, 255) : IM_COL32(150, 150, 150, 255));
        const ImU32 opaque = ImGui::ColorConvertFloat4ToU32(ImVec4(col->x, col->y, col->z, 1.0f));
        const ImU32 clear = opaque & ~IM_COL32_A_MASK;
        dl->AddRectFilledMultiColor(ab.Min, ab.Max, clear, opaque, opaque, clear);
        const ImVec2 ak(ab.Min.x + a * ab.GetWidth(), ab.GetCenter().y);
        dl->AddCircleFilled(ak, 5.0f, IM_COL32_WHITE, 12);
    }

    if (changed) {
        win->StateStorage.SetFloat(hue_id, h);
        ImGui::ColorConvertHSVtoRGB(h, s, v, col->x, col->y, col->z);
        col->w = a;
    }

    // Hex readout in an inset field.
    ImGui::Dummy(ImVec2(0.0f, gap - ImGui::GetStyle().ItemSpacing.y));
    char hex[16];
    const ImU32 c32 = ImGui::ColorConvertFloat4ToU32(*col);
    ImFormatString(hex, IM_ARRAYSIZE(hex), alpha ? "#%02X%02X%02X%02X" : "#%02X%02X%02X",
                   (c32 >> IM_COL32_R_SHIFT) & 0xFF, (c32 >> IM_COL32_G_SHIFT) & 0xFF, (c32 >> IM_COL32_B_SHIFT) & 0xFF,
                   (c32 >> IM_COL32_A_SHIFT) & 0xFF);
    const ImVec2 fo = win->DC.CursorPos;
    const float fh = ImGui::GetTextLineHeight() + 6.0f;
    const ImRect fb(fo, fo + ImVec2(w, fh));
    ImGui::ItemSize(fb);
    dl->AddRectFilled(fb.Min, fb.Max, ImGui::GetColorU32(p.inset), 3.0f);
    dl->AddText(ImVec2(fb.Min.x + 6.0f, fb.Min.y + 3.0f), ImGui::GetColorU32(p.label), hex);
    char pct[8];
    ImFormatString(pct, IM_ARRAYSIZE(pct), "%d%%", (int)(col->w * 100.0f + 0.5f));
    if (alpha) dl->AddText(ImVec2(fb.Max.x - 6.0f - ImGui::CalcTextSize(pct).x, fb.Min.y + 3.0f), ImGui::GetColorU32(p.dim), pct);
    return changed;
}

// Small swatch right-aligned on the row (checkerboard under translucent colors). Click opens a
// picker popup anchored under the swatch's right edge, so it stays inside the group box.
inline bool ColorRow(const char* label, ImVec4* col, bool alpha = true) {
    ImGuiID id; ImRect bb;
    if (!RowBegin(label, &id, &bb)) return false;

    const float sw = 20.0f, sh = 11.0f;
    const ImVec2 s0(bb.Max.x - sw, bb.Min.y + ImFloor((bb.GetHeight() - sh) * 0.5f)), s1 = s0 + ImVec2(sw, sh);
    const ImGuiID popup_id = SubId(id, "picker");
    bool hovered, held;
    if (ImGui::ButtonBehavior(bb, id, &hovered, &held)) ImGui::OpenPopupEx(popup_id);
    const bool open = ImGui::IsPopupOpen(popup_id, ImGuiPopupFlags_None);

    const Palette& p = Theme();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float hov = Anim(SubId(id, "hover"), hovered || open);
    RowLabel(dl, bb, label, hov);
    if (col->w < 1.0f) {
        dl->AddRectFilled(s0, s1, IM_COL32(150, 150, 150, 255), 2.0f);
        for (float y = s0.y; y < s1.y; y += 4.0f)
            for (float x = s0.x; x < s1.x; x += 4.0f)
                if (((int)((x - s0.x) / 4.0f) + (int)((y - s0.y) / 4.0f)) % 2 == 0)
                    dl->AddRectFilled(ImVec2(x, y), ImVec2(ImMin(x + 4.0f, s1.x), ImMin(y + 4.0f, s1.y)), IM_COL32(200, 200, 200, 255));
    }
    dl->AddRectFilled(s0, s1, ImGui::ColorConvertFloat4ToU32(*col), 2.0f);
    dl->AddRect(s0 - ImVec2(1, 1), s1 + ImVec2(1, 1),
                ImGui::GetColorU32(ImLerp(p.border, open ? p.accent : p.label, hov * 0.7f)), 3.0f);

    bool changed = false;
    if (open) {
        ImGui::SetNextWindowPos(ImVec2(s1.x, s1.y + 4.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
        if (ImGui::BeginPopupEx(popup_id, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                                          ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize)) {
            changed = PickerBody(col, alpha);
            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }
    return changed;
}

// Header tabs: dim text, bright when active, sliding accent underline.
// Labels can start with an icon-font glyph, e.g. ICON_FA_GEAR " GENERAL".
inline bool HeaderTabs(const char* const* labels, int count, int* current, float spacing = 18.0f) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;
    const Palette& p = Theme();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImGui::PushID("##header_tabs");
    const ImGuiID bar_id = ImGui::GetID("bar");

    bool changed = false;
    float sel_x = 0.0f, sel_w = 0.0f, base_y = 0.0f;
    for (int i = 0; i < count; ++i) {
        if (i > 0) ImGui::SameLine(0.0f, spacing);
        ImGui::PushID(i);
        const ImVec2 ts = ImGui::CalcTextSize(labels[i]);
        const ImVec2 pos = ImGui::GetCursorScreenPos();
        if (ImGui::InvisibleButton("tab", ImVec2(ts.x, ts.y + 8.0f)) && *current != i) { *current = i; changed = true; }
        const bool hov = ImGui::IsItemHovered();
        const float t = AnimTo(ImGui::GetID("bright"), *current == i ? 1.0f : (hov ? 0.5f : 0.0f));
        dl->AddText(pos, ImGui::GetColorU32(ImLerp(p.dim, p.text, t)), labels[i]);
        if (*current == i) { sel_x = pos.x - window->Pos.x; sel_w = ts.x; }
        base_y = pos.y + ts.y + 5.0f;
        ImGui::PopID();
    }
    // Underline position is stored relative to the window so moving the window doesn't animate it.
    const float ux = AnimTo(SubId(bar_id, "x"), sel_x, 12.0f) + window->Pos.x;
    const float uw = AnimTo(SubId(bar_id, "w"), sel_w, 12.0f);
    dl->AddRectFilled(ImVec2(ux, base_y), ImVec2(ux + uw, base_y + 2.0f), ImGui::GetColorU32(p.accent), 1.0f);
    ImGui::PopID();
    return changed;
}

// Group box: bordered child with an uppercase header above the first row. Always pair with EndGroupBox().
inline bool BeginGroupBox(const char* title, ImVec2 size, ImFont* header_font = nullptr) {
    const Palette& p = Theme();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 10));
    const bool visible = ImGui::BeginChild(title, size, ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding);
    ImGui::PopStyleVar();
    if (header_font) ImGui::PushFont(header_font);
    ImGui::PushStyleColor(ImGuiCol_Text, p.header);
    ImGui::TextUnformatted(title, ImGui::FindRenderedTextEnd(title));
    ImGui::PopStyleColor();
    if (header_font) ImGui::PopFont();
    ImGui::Spacing();
    return visible;
}
inline void EndGroupBox() { ImGui::EndChild(); }

} // namespace menu
