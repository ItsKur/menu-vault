---
title: Tracers
date: 2026-11-06
tags: [visuals, in-world, line, tracer, beam]
---

# Tracers

The short-lived streak left by a shot: a bright core from muzzle to impact, a glow around it, and a fade that starts before the eye can follow it. [Glow lines](glow-lines.md) covers the drawing primitive; this note covers the element built from it: timing, anchors, and the mistakes that make a tracer read as a laser pointer.

## Anatomy

- **Core**: a thin high-energy center, with a softer glow around it. The existing recipe uses a 1–2 px near-white center at 1080p, but this is **one style, not a universal rule**. [Kizaru's inspected footage](kizaru-molly-and-tracers.md) has a saturated pale-magenta core and coloured halo; preserve that when matching the reference.
- **Glow**: 4–8× the core width, tapering toward the far end, falling off fast (a cubic or squared falloff, as in [Glow lines](glow-lines.md)). Fatality exposes a single "width" per group, 1–20 with a default of 8. Whether that is the core or the total isn't known (the source has the setting but no drawing code); the vault's `Beam` treats it as the total.
- **Taper**: the near end is widest. A constant-width streak reads as a beam mounted to the screen; a taper reads as something that travelled.
- **Fades**: alpha ramps in over the first 10–15% and out over the last 30–40%. A tracer that ends at full alpha looks cut with scissors.
- **Lifetime**: 0.1–0.3 s for a shot, up to 0.6 s for a slow projectile arc. Never persistent: a tracer that stays on screen stops being an event and becomes clutter.
- **Anchors**: start at the muzzle, not the camera: offset the near end a few dozen pixels down and to one side, or the streak appears to grow out of the viewer's face. End at the impact point, and let a separate small mark (an impact cube, a cross, a spark) carry the hit. Do not fatten the line's end to fake it.
- **Dots along the line**: small discs at even steps, seen in the [Neverlose tracer close-up](neverlose-hud.md). They add travel direction; keep them subtle or they turn the line into a dashed one.

## Variants

- **Hit-tracer**: only on confirmation, one colour, 0.1 s. Feedback, not decoration.
- **Every-shot tracer**: all shots, alternating or tinted per team, with a cap on how many can exist at once (8–16).
- **Predicted arc**: the same tube along a parabola, wider and dimmer, lasting until the projectile lands. See [Glow lines](glow-lines.md) for the trajectory variants.
- **Impact-only**: no line at all, just a burst at the hit point. Common in games where tracers would give away positions.

## Rules

- **Hot core, softer coloured halo.** Near-white is the existing recipe's default; use the saturated pale-magenta center when matching the [Kizaru reference](kizaru-molly-and-tracers.md).
- **Bounded persistence.** Under 0.3 s is a low-clutter starting point, not a measured product constant. When matching footage, distinguish a single shot's fade from repeated-shot refresh or overlapping lines.
- **Cap the count.** Overlapping tracers stack additively into white blobs; draw all glows on one layer, or hard-limit live tracers.
- **Clip long paths.** A streak across the whole map is a line, not a tracer; fade out past roughly 1500 units of world distance.
- **Never the decorative accent on both sides.** If both teams' tracers use the menu accent, the element stops identifying anything.

## ImGui approach

`Beam(dl, from, to, col, width, age)` in [Fatality-style in-world visuals](../implementation/fatality-visuals-code.md) is the vault's tracer. It draws four glow layers up to `width` px wide and a 1.6 px core lightened toward white, each with `hud::GradientLine` from [HUD elements](../implementation/hud-elements.md) (one quad whose colour changes along its length). Alpha runs from 0 at the start to full at the impact, so the streak fades in from the muzzle, and the whole beam fades out as `age` goes from 0 to 1.

Two of the rules above aren't in it yet: it doesn't taper, and it ends at full alpha instead of fading out toward the far end. To add both, split the segment into a few pieces, shrink each piece's width toward the far end, and give the last 30–40% an alpha that falls back to 0. For the dots, step along the segment and draw 2 px discs at 60% alpha.

`ImpactCube` draws the hit mark: a small wireframe cube (12 one-pixel edges over faint faces) in one colour, projected with the demo camera. Fatality's two impact colours are for two kinds of impact, one colour per cube.

## Seen in

- [Kizaru — molly area and tracers](kizaru-molly-and-tracers.md): original 1080p/60fps inspection and video excerpt; saturated magenta center, softer halo and repeated-fire persistence example.

- [Neverlose — HUD](neverlose-hud.md): tracer close-up — white hot core fading toward the far end, faint warm glow, small magenta dots along the line.
- [Fatality — in-game visuals](fatality-visuals.md): per-group on/off and one colour each, width 1–20 (default 8); the rendering itself is not in that source. The vault's rendition is `Beam` and `ImpactCube`.

Related: [Glow lines](glow-lines.md), [Hit markers](hit-markers.md), [Chams](chams.md), [HUD principles](hud-principles.md)