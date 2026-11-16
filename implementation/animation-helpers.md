---
title: Animation helpers
date: 2026-11-06
tags: [implementation, imgui, animation]
---

# Animation helpers

ImGui widgets are stateless per frame, so animation state lives in the window's `ImGuiStorage`, keyed by an id derived from the widget's id. `AnimTo` eases a float toward a target with exponential smoothing, which looks the same at 60 and 240 FPS.

```cpp
const float t = menu::Anim(menu::SubId(id, "hover"), hovered);   // 0..1
const ImVec4 col = ImLerp(p.label, p.text, t);
```

- First call returns the target, so nothing animates when the menu first appears.
- Use different `SubId` tags for each animated property of one widget.
- Store positions relative to the window (see `HeaderTabs`) so dragging the window doesn't trigger animation.
- `ReduceMotion()` is the "Reduce motion" setting from [Avoiding AI defaults](../principles/avoiding-ai-defaults.md#motion). Bind it to a toggle; while it is on, every `AnimTo`/`Anim` returns its target at once, so all three menu styles stop sliding, springing and fading. It snaps fades too; a widget that wants a short fade under reduced motion needs its own ≤100 ms timer. The HUD helpers in [HUD elements](hud-elements.md) read it too: `SpringTo` and `FadeTo` jump to their end values, the ground-area outline stops easing, and the hit marker drops its pop-in expand and number shake.

File: `code/menu_anim.h`

```cpp
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
```

Related: [Motion](../principles/motion.md), [Custom widgets](custom-widgets.md)
