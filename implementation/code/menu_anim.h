// menu_anim.h — frame-rate independent easing stored in ImGui's per-window state storage.
#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include <cmath>

namespace menu {

// "Reduce motion" setting: when true, every AnimTo/Anim snaps to its target (no slides, springs or fades).
// Expose it as a toggle in the menu's settings; ImGui cannot read the OS preference.
inline bool& ReduceMotion() { static bool on = false; return on; }

// Moves a stored float toward `target`. The first call returns `target` (no animation on first show).
// speed ~10–16 for small controls, ~8 for larger moves.
inline float AnimTo(ImGuiID id, float target, float speed = 14.0f) {
    ImGuiStorage* st = ImGui::GetStateStorage();
    if (ReduceMotion()) { st->SetFloat(id, target); return target; }
    float v = st->GetFloat(id, target);
    v = ImLerp(v, target, 1.0f - std::exp(-speed * ImGui::GetIO().DeltaTime));
    if (ImFabs(v - target) < 0.001f) v = target;
    st->SetFloat(id, v);
    return v;
}

inline float Anim(ImGuiID id, bool on, float speed = 14.0f) { return AnimTo(id, on ? 1.0f : 0.0f, speed); }

// Derive a second animation id from a widget id (e.g. hover vs checked).
inline ImGuiID SubId(ImGuiID id, const char* tag) { return ImHashStr(tag, 0, id); }

} // namespace menu
