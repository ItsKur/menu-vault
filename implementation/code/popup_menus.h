// popup_menus.h — neverlose-style popups (references/neverlose-popups.md): the right-click hotkey menu with a hover
// submenu per hotkey (key capture, mode, value, visibility, delete), and tooltips docked beside the menu window.
// Standalone: draws into your ImGui windows with placeholder data. It records which key was chosen; it never acts on it.
#pragma once
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"
#include "imgui_internal.h"
#include "menu_anim.h"
#include "menu_draw.h"   // SoftShadow
#include <cmath>
#include <vector>

namespace pm {

using menu::Anim;
using menu::AnimTo;
using menu::SubId;

inline ImVec4 Hex(unsigned rgb, float a = 1.0f) {
    return ImVec4(((rgb >> 16) & 0xFF) / 255.0f, ((rgb >> 8) & 0xFF) / 255.0f, (rgb & 0xFF) / 255.0f, a);
}
inline ImU32 U(const ImVec4& c) { return ImGui::GetColorU32(c); }
inline ImVec4 WithA(ImVec4 c, float a) { c.w = a; return c; }

// Sampled from the captures (approximate).
struct Theme {
    ImVec4 popup = Hex(0x1b1a20), edge = Hex(0x212127), hover = Hex(0x27262c), chip = Hex(0x222228), sep = Hex(0x212026);
    ImVec4 text = Hex(0xcfced4), text_dim = Hex(0x8e8d94), icon = Hex(0xc1c0c7);
    ImVec4 accent = Hex(0xff3134), danger = Hex(0xb27378);           // "on" states; destructive rows (muted)
    ImVec4 track_off = Hex(0x0f1116), knob_off = Hex(0x7c868f), knob_on = Hex(0xffffff);
    ImVec4 title = Hex(0xffffff), tip_text = Hex(0xc9c6cb), shadow = Hex(0x000000, 0.45f);
    float tip_alpha = 0.88f;                                          // tooltip background opacity
    // demo card
    ImVec4 card = Hex(0x1f1e24), row_line = Hex(0x26252b), row_text = Hex(0xe2e3ea), row_off = Hex(0x7e7e84), section = Hex(0x646468);
};
inline Theme& T() { static Theme t; return t; }

// Layout tokens, px (estimated from the captures, taking a menu row as 40 px).
namespace k {
constexpr float row_h = 32, pad = 5, row_gap = 2, icon_x = 14, label_x = 34, rounding = 9, row_r = 6, sub_gap = 6;
constexpr float menu_w = 184, sub_w = 236, sep_inset = 10, sep_h = 9;
constexpr float sw_w = 28, sw_h = 16;                       // switch
constexpr float tip_pad = 12, tip_wrap = 300, tip_delay = 0.35f, tip_fade = 0.1f, tip_gap = 6, tip_chain = 0.25f;
constexpr float card_row_h = 40, card_pad = 16;
}

enum BindMode { BindToggle, BindHold, BindAlways, BindModeCount };
inline const char* ModeName(int m) { static const char* n[BindModeCount] = { "Toggle", "Hold", "Always" }; return n[ImClamp(m, 0, BindModeCount - 1)]; }

// One binding. A setting holds any number of these.
struct Hotkey {
    ImGuiKey key = ImGuiKey_None;   // ImGuiKey_None = "Unbound"
    int mode = BindToggle;
    bool value = true;              // what the setting becomes while the binding is active
    bool visible = true;            // listed in the HUD keybind list
};

enum HotkeyAction { HotkeyNone, HotkeyShowAll, HotkeyReset };

inline const char* KeyLabel(ImGuiKey key) {
    switch (key) {
    case ImGuiKey_None: return "Unbound";
    case ImGuiKey_MouseRight: return "Mouse 2";
    case ImGuiKey_MouseMiddle: return "Mouse 3";
    case ImGuiKey_MouseX1: return "Mouse 4";
    case ImGuiKey_MouseX2: return "Mouse 5";
    default: return ImGui::GetKeyName(key);
    }
}

// ---------------------------------------------------------------------------------------------
// Icons: small stroked placeholders (swap in an icon font). Half size 7 px.

enum Icon { IconUnbound, IconList, IconPlus, IconReset, IconKeyboard, IconLink, IconEye, IconTrash, IconChevron, IconModeSwitch };

inline void DrawIcon(ImDrawList* dl, Icon ic, ImVec2 c, ImU32 col, ImU32 bg, float s = 7.0f) {
    const float th = 1.5f;
    switch (ic) {
    case IconUnbound:   // a horizon with rays: "nothing set yet"
        dl->AddLine(c + ImVec2(-s, s * 0.45f), c + ImVec2(-s * 0.55f, s * 0.45f), col, th);
        dl->AddLine(c + ImVec2(s * 0.55f, s * 0.45f), c + ImVec2(s, s * 0.45f), col, th);
        dl->PathArcTo(c + ImVec2(0, s * 0.45f), s * 0.38f, IM_PI, IM_PI * 2.0f, 10); dl->PathStroke(col, 0, th);
        for (int i = 0; i < 5; ++i) {
            const float a = IM_PI + IM_PI * (0.1f + 0.2f * i);
            const ImVec2 d(std::cos(a), std::sin(a));
            dl->AddLine(c + ImVec2(0, s * 0.45f) + d * (s * 0.62f), c + ImVec2(0, s * 0.45f) + d * (s * 0.9f), col, th);
        }
        break;
    case IconList:
        for (int i = -1; i <= 1; ++i) {
            dl->AddRectFilled(c + ImVec2(-s, i * s * 0.6f - 1.0f), c + ImVec2(-s * 0.65f, i * s * 0.6f + 1.0f), col);
            dl->AddLine(c + ImVec2(-s * 0.35f, i * s * 0.6f), c + ImVec2(s, i * s * 0.6f), col, th);
        }
        break;
    case IconPlus:
        dl->AddCircleFilled(c, s * 0.9f, col, 20);
        dl->AddLine(c + ImVec2(-s * 0.45f, 0), c + ImVec2(s * 0.45f, 0), bg, th);
        dl->AddLine(c + ImVec2(0, -s * 0.45f), c + ImVec2(0, s * 0.45f), bg, th);
        break;
    case IconReset: {   // counter-clockwise arrow
        const float a0 = IM_PI * 1.2f, r = s * 0.72f;
        dl->PathArcTo(c, r, a0, a0 + IM_PI * 1.6f, 18); dl->PathStroke(col, 0, th);
        const ImVec2 p0 = c + ImVec2(std::cos(a0), std::sin(a0)) * r;
        dl->AddTriangleFilled(p0 + ImVec2(-s * 0.45f, -s * 0.1f), p0 + ImVec2(s * 0.25f, -s * 0.35f), p0 + ImVec2(0.0f, s * 0.4f), col);
        break; }
    case IconKeyboard:
        dl->AddRect(c + ImVec2(-s, -s * 0.62f), c + ImVec2(s, s * 0.62f), col, 2.0f, 0, th);
        for (int i = 0; i < 4; ++i) dl->AddRectFilled(c + ImVec2(-s * 0.62f + i * s * 0.42f, -s * 0.28f), c + ImVec2(-s * 0.42f + i * s * 0.42f, -s * 0.08f), col);
        dl->AddLine(c + ImVec2(-s * 0.5f, s * 0.28f), c + ImVec2(s * 0.5f, s * 0.28f), col, th);
        break;
    case IconLink:
        dl->AddCircle(c + ImVec2(-s * 0.3f, s * 0.3f), s * 0.45f, col, 16, th);
        dl->AddCircle(c + ImVec2(s * 0.3f, -s * 0.3f), s * 0.45f, col, 16, th);
        break;
    case IconEye:
        dl->AddEllipse(c, ImVec2(s, s * 0.6f), col, 0.0f, 20, th);
        dl->AddCircleFilled(c, s * 0.32f, col, 12);
        break;
    case IconTrash:
        dl->AddLine(c + ImVec2(-s * 0.85f, -s * 0.6f), c + ImVec2(s * 0.85f, -s * 0.6f), col, th);
        dl->AddRectFilled(c + ImVec2(-s * 0.3f, -s * 0.9f), c + ImVec2(s * 0.3f, -s * 0.6f), col);
        dl->AddRectFilled(c + ImVec2(-s * 0.6f, -s * 0.4f), c + ImVec2(s * 0.6f, s), col, 1.5f);
        dl->AddLine(c + ImVec2(-s * 0.2f, -s * 0.15f), c + ImVec2(-s * 0.2f, s * 0.7f), bg, 1.0f);
        dl->AddLine(c + ImVec2(s * 0.2f, -s * 0.15f), c + ImVec2(s * 0.2f, s * 0.7f), bg, 1.0f);
        break;
    case IconChevron: {
        const ImVec2 pts[3] = { c + ImVec2(-s * 0.25f, -s * 0.5f), c + ImVec2(s * 0.25f, 0), c + ImVec2(-s * 0.25f, s * 0.5f) };
        dl->AddPolyline(pts, 3, col, ImDrawFlags_None, th);
        break; }
    case IconModeSwitch:   // the small pill shown inside the mode chip
        dl->AddRectFilled(c + ImVec2(-s, -s * 0.55f), c + ImVec2(s, s * 0.55f), col, s * 0.55f);
        dl->AddCircleFilled(c + ImVec2(s * 0.42f, 0), s * 0.28f, bg, 12);
        break;
    }
}

// Pill switch drawn by its right-center point. `on` 0..1 (animate it).
inline void DrawSwitch(ImDrawList* dl, ImVec2 right_center, float on) {
    const Theme& t = T();
    const ImVec2 a(right_center.x - k::sw_w, right_center.y - k::sw_h * 0.5f), b(right_center.x, right_center.y + k::sw_h * 0.5f);
    dl->AddRectFilled(a, b, U(ImLerp(t.track_off, t.accent, on)), k::sw_h * 0.5f);
    const float r = k::sw_h * 0.5f - 2.0f;
    const float x = ImLerp(a.x + k::sw_h * 0.5f, b.x - k::sw_h * 0.5f, on);
    dl->AddCircleFilled(ImVec2(x, right_center.y), r, U(ImLerp(t.knob_off, t.knob_on, on)), 16);
}

// ---------------------------------------------------------------------------------------------
// Popup building blocks.

inline void PushPopupStyle() {
    const Theme& t = T();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(k::pad, k::pad));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, k::row_gap));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, k::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, t.popup);
    ImGui::PushStyleColor(ImGuiCol_Border, t.edge);
}
inline void PopPopupStyle() { ImGui::PopStyleColor(2); ImGui::PopStyleVar(4); }

// Soft shadow under the current popup, drawn on the window it was opened from (which renders first).
inline void PopupShadow() {
    ImGuiWindow* w = ImGui::GetCurrentWindow();
    if (w->ParentWindow && w->Size.x > 0.0f) menu::SoftShadow(w->ParentWindow->DrawList, w->Pos, w->Pos + w->Size, k::rounding, T().shadow, 14.0f, 8);
}

// Icon + label row inside a popup; a rounded lighter fill on hover. Disabled rows dim and don't respond.
// Hover is also taken from the hovered window directly, so a parent menu keeps reacting while its submenu has focus.
inline bool MenuRow(const char* str_id, Icon ic, const char* label, bool enabled = true, bool danger = false, bool* hovered_out = nullptr) {
    ImGuiContext& g = *GImGui;
    ImGuiWindow* w = g.CurrentWindow;
    const Theme& t = T();
    const ImVec2 pos = w->DC.CursorPos;
    const ImRect bb(pos, pos + ImVec2(ImGui::GetContentRegionAvail().x, k::row_h));
    const ImGuiID id = w->GetID(str_id);
    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id)) return false;
    bool hovered, held;
    const bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held) && enabled;
    hovered = hovered || (g.HoveredWindow == w && bb.Contains(g.IO.MousePos));
    if (hovered_out) *hovered_out = hovered && enabled;

    ImDrawList* dl = w->DrawList;
    const float hl = Anim(SubId(id, "hov"), hovered && enabled, 18.0f);
    if (hl > 0.01f) dl->AddRectFilled(bb.Min, bb.Max, U(WithA(t.hover, t.hover.w * hl)), k::row_r);
    const ImVec4 fg = danger ? t.danger : (enabled ? t.text : t.text_dim);
    const ImVec4 ic_col = danger ? t.danger : (enabled ? t.icon : t.text_dim);
    const float cy = bb.GetCenter().y;
    DrawIcon(dl, ic, ImVec2(bb.Min.x + k::icon_x, cy), U(ic_col), U(t.popup));
    const ImVec2 ts = ImGui::CalcTextSize(label);
    dl->AddText(ImVec2(bb.Min.x + k::label_x, cy - ts.y * 0.5f), U(fg), label);
    return pressed;
}

inline void MenuSeparator() {
    ImGuiWindow* w = ImGui::GetCurrentWindow();
    const ImVec2 pos = w->DC.CursorPos;
    const float width = ImGui::GetContentRegionAvail().x;
    ImGui::ItemSize(ImVec2(width, k::sep_h));
    const float y = ImFloor(pos.y + k::sep_h * 0.5f);
    w->DrawList->AddLine(ImVec2(pos.x + k::sep_inset, y), ImVec2(pos.x + width - k::sep_inset, y), U(T().sep));
}

// ---------------------------------------------------------------------------------------------
// Hotkey submenu for one binding. Returns true when Delete was clicked.
// Key capture: click Key, then press any key or mouse button (not the left button). Escape cancels,
// Backspace / Delete unbind. Clicking anywhere else in the submenu stops listening.

inline bool HotkeySubmenu(Hotkey& b) {
    const Theme& t = T();
    ImGuiWindow* w = ImGui::GetCurrentWindow();
    ImDrawList* dl = w->DrawList;
    ImGuiStorage* st = ImGui::GetStateStorage();
    const ImGuiID listen_id = w->GetID("##listen");
    bool listening = st->GetBool(listen_id, false);
    bool key_hovered = false;

    // Key: the bound key in a small chip at the right, or an empty box; "..." in the accent while listening.
    if (MenuRow("##key", IconUnbound, "Key", true, false, &key_hovered)) listening = !listening;
    {
        const ImRect row = GImGui->LastItemData.Rect;
        const char* txt = listening ? "..." : (b.key == ImGuiKey_None ? "" : KeyLabel(b.key));
        const ImVec2 ts = ImGui::CalcTextSize(txt);
        const float cw = ImMax(22.0f, ts.x + 14.0f), ch = 20.0f;
        const ImVec2 c1(row.Max.x - 8.0f, row.GetCenter().y + ch * 0.5f), c0(c1.x - cw, c1.y - ch);
        dl->AddRectFilled(c0, c1, U(t.chip), 5.0f);
        dl->AddText(ImVec2(c0.x + (cw - ts.x) * 0.5f, c0.y + (ch - ts.y) * 0.5f), U(listening ? t.accent : t.text), txt);
    }
    if (listening) {
        for (int ki = ImGuiKey_NamedKey_BEGIN; ki < ImGuiKey_NamedKey_END; ++ki) {
            const ImGuiKey key = (ImGuiKey)ki;
            if (key == ImGuiKey_MouseLeft || key == ImGuiKey_MouseWheelX || key == ImGuiKey_MouseWheelY ||
                (key >= ImGuiKey_ReservedForModCtrl && key <= ImGuiKey_ReservedForModSuper)) continue;
            if (!ImGui::IsKeyPressed(key, false)) continue;
            if (key == ImGuiKey_Backspace || key == ImGuiKey_Delete) b.key = ImGuiKey_None;
            else if (key != ImGuiKey_Escape) b.key = key;
            listening = false;
            break;
        }
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !key_hovered) listening = false;
    }
    st->SetBool(listen_id, listening);

    // Mode: a chip with the mode's pill icon and name in the accent; clicking the row cycles the mode.
    if (MenuRow("##mode", IconKeyboard, "Mode")) b.mode = (b.mode + 1) % BindModeCount;
    {
        const ImRect row = GImGui->LastItemData.Rect;
        const char* name = ModeName(b.mode);
        const ImVec2 ts = ImGui::CalcTextSize(name);
        const float cw = ts.x + 40.0f, ch = 24.0f;
        const ImVec2 c1(row.Max.x - 8.0f, row.GetCenter().y + ch * 0.5f), c0(c1.x - cw, c1.y - ch);
        dl->AddRectFilled(c0, c1, U(t.chip), 6.0f);
        DrawIcon(dl, IconModeSwitch, ImVec2(c0.x + 16.0f, row.GetCenter().y), U(t.accent), U(t.chip), 7.0f);
        dl->AddText(ImVec2(c0.x + 30.0f, row.GetCenter().y - ts.y * 0.5f), U(t.accent), name);
    }
    MenuSeparator();

    if (MenuRow("##value", IconLink, "Value")) b.value = !b.value;
    DrawSwitch(dl, ImVec2(GImGui->LastItemData.Rect.Max.x - 8.0f, GImGui->LastItemData.Rect.GetCenter().y),
               Anim(w->GetID("##value_sw"), b.value, 14.0f));
    if (MenuRow("##visible", IconEye, "Visible")) b.visible = !b.visible;
    DrawSwitch(dl, ImVec2(GImGui->LastItemData.Rect.Max.x - 8.0f, GImGui->LastItemData.Rect.GetCenter().y),
               Anim(w->GetID("##visible_sw"), b.visible, 14.0f));
    MenuSeparator();

    return MenuRow("##delete", IconTrash, "Delete", true, true);
}

// Right-click hotkey menu for the item just drawn. Lists the row's hotkeys (hover one to open its submenu), then
// All Hotkeys (dimmed with none), New Hotkey (dimmed while an Unbound one exists), and Reset in muted red.
// Returns ShowAll / Reset so the caller can act (show a hotkey list, reset the setting's value).
inline HotkeyAction HotkeyMenu(const char* str_id, std::vector<Hotkey>& binds) {
    ImGuiContext& g = *GImGui;
    HotkeyAction action = HotkeyNone;
    ImGui::PushID(str_id);
    const ImGuiID menu_id = ImGui::GetID("##hotkeys");
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) ImGui::OpenPopupEx(menu_id);

    const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar;
    PushPopupStyle();
    ImGui::SetNextWindowSize(ImVec2(k::menu_w, 0.0f));
    if (ImGui::BeginPopupEx(menu_id, flags)) {
        PopupShadow();
        ImGuiWindow* mw = ImGui::GetCurrentWindow();
        ImGuiStorage* st = ImGui::GetStateStorage();
        const ImGuiID sub_id = mw->GetID("##sub");
        const ImGuiID idx_key = SubId(sub_id, "idx"), y_key = SubId(sub_id, "y");
        int sub_index = st->GetInt(idx_key, -1);

        bool any_unbound = false;
        for (const Hotkey& b : binds) any_unbound |= (b.key == ImGuiKey_None);
        int hovered_bind = -1;
        float hovered_y = 0.0f;
        for (int i = 0; i < (int)binds.size(); ++i) {
            ImGui::PushID(i);
            bool hov = false;
            MenuRow("##bind", binds[i].key == ImGuiKey_None ? IconUnbound : IconKeyboard, KeyLabel(binds[i].key), true, false, &hov);
            const ImRect row = g.LastItemData.Rect;
            DrawIcon(mw->DrawList, IconChevron, ImVec2(row.Max.x - 12.0f, row.GetCenter().y), U(T().icon), U(T().popup));
            if (hov) { hovered_bind = i; hovered_y = row.Min.y; }
            ImGui::PopID();
        }
        bool h_all = false, h_new = false, h_reset = false;
        if (MenuRow("##all", IconList, "All Hotkeys", !binds.empty(), false, &h_all)) { action = HotkeyShowAll; ImGui::CloseCurrentPopup(); }
        if (MenuRow("##new", IconPlus, "New Hotkey", !any_unbound, false, &h_new)) binds.push_back(Hotkey());
        MenuSeparator();
        if (MenuRow("##reset", IconReset, "Reset", true, true, &h_reset)) { binds.clear(); action = HotkeyReset; ImGui::CloseCurrentPopup(); }

        // Submenu: opens on hovering a hotkey row, switches when another hotkey row is hovered,
        // closes when one of the other rows is hovered.
        const int level = g.BeginPopupStack.Size;
        if (hovered_bind >= 0 && (hovered_bind != sub_index || !ImGui::IsPopupOpen(sub_id, ImGuiPopupFlags_None))) {
            if (ImGui::IsPopupOpen(sub_id, ImGuiPopupFlags_None)) ImGui::ClosePopupToLevel(level, false);
            sub_index = hovered_bind;
            st->SetFloat(y_key, hovered_y);
            ImGui::OpenPopupEx(sub_id);
        }
        if ((h_all || h_new || h_reset) && ImGui::IsPopupOpen(sub_id, ImGuiPopupFlags_None)) ImGui::ClosePopupToLevel(level, false);
        if (sub_index >= (int)binds.size()) {
            sub_index = -1;
            if (ImGui::IsPopupOpen(sub_id, ImGuiPopupFlags_None)) ImGui::ClosePopupToLevel(level, false);
        }
        st->SetInt(idx_key, sub_index);

        if (sub_index >= 0) {
            ImGui::SetNextWindowPos(ImVec2(mw->Pos.x + mw->Size.x + k::sub_gap, st->GetFloat(y_key, mw->Pos.y) - k::pad));
            ImGui::SetNextWindowSize(ImVec2(k::sub_w, 0.0f));
            // NoFocusOnAppearing: the parent menu keeps focus, so its rows stay hoverable while the submenu is open.
            if (ImGui::BeginPopupEx(sub_id, flags | ImGuiWindowFlags_NoFocusOnAppearing)) {
                PopupShadow();
                if (HotkeySubmenu(binds[sub_index])) {
                    binds.erase(binds.begin() + sub_index);
                    st->SetInt(idx_key, -1);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        ImGui::EndPopup();
    }
    PopPopupStyle();
    ImGui::PopID();
    return action;
}

// ---------------------------------------------------------------------------------------------
// Docked tooltip for the item just drawn: after a short hover it fades in just outside the current root window's
// right edge, vertically centered on the item. Bold title, then one wrapped sentence. Moving straight to another
// row shows its tooltip without a second delay.

inline void RowTooltip(const char* title, const char* text, ImFont* bold = nullptr) {
    ImGuiContext& g = *GImGui;
    struct State { ImGuiID id = 0; float t = 0.0f; double last_shown = -10.0; };
    static State s;
    if (!ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) return;
    const ImGuiID id = g.LastItemData.ID ? g.LastItemData.ID : g.CurrentWindow->GetID(title);
    const double now = ImGui::GetTime();
    const bool chained = now - s.last_shown < k::tip_chain;
    if (s.id != id) { s.id = id; s.t = chained ? k::tip_delay + k::tip_fade : 0.0f; }
    s.t += g.IO.DeltaTime;
    if (s.t < k::tip_delay) return;
    s.last_shown = now;
    const float alpha = ImSaturate((s.t - k::tip_delay) / k::tip_fade);

    const Theme& t = T();
    ImGuiWindow* root = g.CurrentWindow->RootWindow;
    const ImRect item = g.LastItemData.Rect;
    ImGui::SetNextWindowPos(ImVec2(root->Pos.x + root->Size.x + k::tip_gap, item.GetCenter().y), ImGuiCond_Always, ImVec2(0.0f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(k::tip_pad, k::tip_pad - 2.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, k::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, k::rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 4.0f));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, WithA(t.popup, t.tip_alpha));
    if (ImGui::BeginTooltip()) {
        ImGuiWindow* tw = ImGui::GetCurrentWindow();
        if (tw->Size.x > 0.0f)
            menu::SoftShadow(root->DrawList, tw->Pos, tw->Pos + tw->Size, k::rounding, WithA(t.shadow, t.shadow.w * alpha), 14.0f, 8);
        if (bold) ImGui::PushFont(bold, 0.0f);
        ImGui::TextColored(t.title, "%s", title);
        if (bold) ImGui::PopFont();
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + k::tip_wrap);
        ImGui::TextColored(t.tip_text, "%s", text);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(7);
}

// ---------------------------------------------------------------------------------------------
// Demo: a settings card with switch rows. Right-click a row for its hotkey menu; hover a row for its tooltip.
// Bound rows show their first key, dim, before the switch (the vault's addition; the captures show no marker).

// One full-width row: label left, switch right, 1 px line below. Returns true when toggled.
inline bool SwitchRow(const char* label, bool* v, bool enabled, const std::vector<Hotkey>* binds = nullptr, bool last = false) {
    ImGuiWindow* w = ImGui::GetCurrentWindow();
    const Theme& t = T();
    const ImGuiID id = w->GetID(label);
    const ImVec2 pos = w->DC.CursorPos;
    const ImRect bb(pos, pos + ImVec2(ImGui::GetContentRegionAvail().x, k::card_row_h));
    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id, nullptr, enabled ? ImGuiItemFlags_None : ImGuiItemFlags_Disabled)) return false;
    bool hovered, held;
    const bool pressed = enabled && ImGui::ButtonBehavior(bb, id, &hovered, &held);
    if (pressed) *v = !*v;
    ImDrawList* dl = w->DrawList;
    const float cy = bb.GetCenter().y;
    const char* end = ImGui::FindRenderedTextEnd(label);
    const ImVec2 ts = ImGui::CalcTextSize(label, end);
    dl->AddText(ImVec2(bb.Min.x, cy - ts.y * 0.5f), U(enabled ? t.row_text : t.row_off), label, end);
    const float on = Anim(SubId(id, "on"), *v, 14.0f);
    if (!enabled) ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.55f);   // dim knob and track
    DrawSwitch(dl, ImVec2(bb.Max.x, cy), enabled ? on : 0.0f);
    if (!enabled) ImGui::PopStyleVar();
    if (binds && !binds->empty() && (*binds)[0].key != ImGuiKey_None) {
        const char* key = KeyLabel((*binds)[0].key);
        const ImVec2 ks = ImGui::CalcTextSize(key);
        dl->AddText(ImVec2(bb.Max.x - k::sw_w - 10.0f - ks.x, cy - ks.y * 0.5f), U(t.section), key);
    }
    if (!last) dl->AddLine(ImVec2(bb.Min.x, bb.Max.y - 0.5f), ImVec2(bb.Max.x, bb.Max.y - 0.5f), U(t.row_line));
    return pressed;
}

inline void DrawPopupDemo(ImFont* bold = nullptr) {
    const Theme& t = T();
    static bool on[5] = { false, true, false, false, true };
    static std::vector<Hotkey> binds[5];
    static bool seeded = false;
    if (!seeded) { binds[1].push_back(Hotkey{ ImGuiKey_F, BindHold, true, true }); seeded = true; }
    static const char* labels[5] = { "Option A", "Option B", "Option C", "Option D", "Option E" };
    static const char* tips[5] = {
        "Placeholder description of what this option does.",
        "Hold the bound key to turn this on while it is held.",
        "A third option, explained in one sentence.",
        "Unavailable in this mode, so it is shown dimmed.",
        "The last option in the list." };

    ImGui::SetNextWindowSize(ImVec2(340.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(k::card_pad, 12.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, t.card);
    ImGui::Begin("##popup_demo", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PushStyleColor(ImGuiCol_Text, t.section);
    ImGui::TextUnformatted("FEATURES");
    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0.0f, 4.0f));
    for (int i = 0; i < 5; ++i) {
        const bool enabled = (i != 3);
        SwitchRow(labels[i], &on[i], enabled, &binds[i], i == 4);
        if (enabled && HotkeyMenu(labels[i], binds[i]) == HotkeyReset) on[i] = false;
        RowTooltip(labels[i], tips[i], bold);
    }
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(4);
}

} // namespace pm
