---
title: Hit markers
date: 2026-11-06
tags: [visuals, hud, feedback, animation]
---

# Hit markers

A brief mark at the point where an action landed, sometimes with a number. Its whole job is to confirm "that registered" in under half a second, without blocking the view.

## Anatomy (velocity.cat, exact from source)

- **Shape**: four diagonal arms around the point (an ×), 1.25 px thick. Each arm runs from `gap` to `size` px from the center, and **fades to transparent at its inner end**, so the center is left open.
- **Snap-in**: arms start 6 px longer than their rest length (rest size 10 px, rest gap 3 px) and contract to rest with ease-out-quart over about 50 ms (`expand = 6 · (1 − easeOutQuart(min(age · 20, 1)))`; the gap grows by 30% of that). It reads as a punch.
- **Fade**: alpha = 1 − p² over the duration (a user setting from 0.1 to 5 s). It stays nearly solid early and drops late.
- **Value pop** (optional): the number sits 20 px above the marker and shakes for its first 125 ms (`sin(age · 50) · 3` px horizontally, `cos(age · 45) · 2` px vertically, decaying linearly).
- **Glow** (optional): the same arms drawn again on a blurred glow layer.
- The color is a user setting.

## Variant: fully parametric (neverwin, options only)

The [neverwin DLL](neverwin-visuals.md) exposes every part of the shape as a setting: number of lines (not fixed at four), gap, size, thickness, rotation, a converge animation with its own time (arms close in, like the snap-in above), drift (the marker moves while it fades), separate fades for the inner and outer ends of each arm, an outline (color, size), glow (size, strength), and a second color for the stronger event. Arm count, rotation and inner fade change the character most.

## Rules

- Never draw the center. The thing you're confirming is under it.
- Shorter is better: 0.3–0.6 s total. Overlapping markers should each run their own timer.
- Use the overshoot only on the arms. Shaking the number is for emphasis and should stop before it can be read as motion.
- A dark 1 px shadow under the number keeps it legible on bright scenes.

## ImGui approach

`HitMarker(dl, center, age, duration, color, value)` in [HUD elements](../implementation/hud-elements.md). It is stateless: pass the age, so many markers can play at once. `GradientLine` writes a quad with per-vertex color, because ImGui's own lines are single-color.

## Seen in

- [velocity.cat — HUD](velocity-cat-hud.md)
- [Neverlose — HUD](neverlose-hud.md): a small white four-arm ×, about 12 px across, beside the target.
- [neverwin — visuals](neverwin-visuals.md): the parametric variant, plus a damage number and four-sided overlay lines with gap, length, rotation and glow.

Related: [HUD principles](hud-principles.md), [Motion](../principles/motion.md), [Countdown badges](countdown-badges.md)
