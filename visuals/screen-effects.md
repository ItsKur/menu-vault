---
title: Screen effects
date: 2026-11-06
tags: [visuals, post-process, vignette, chromatic-aberration, rain]
---

# Screen effects

Full-screen passes applied to the whole frame before the HUD is drawn: vignette, chromatic aberration, and rain on the lens. They set a mood; they carry no information. Input is the finished frame as a texture, plus a few numbers.

## Vignette (neverwin shader)

- Distance from center, with x scaled by the aspect ratio so the falloff is round, not oval: `d = (uv − 0.5) · (aspect, 1)`.
- Darken by `smoothstep(0.35, 1.05, |d|) · amount`. Nothing changes inside 35% of the half-height; the corners reach full strength.
- A fuller version exposes strength, radius, roundness, feather and a tint color.

## Chromatic aberration (neverwin shader)

- Sample red at `uv + shift`, green at `uv`, blue at `uv − shift`, with `shift = (uv − 0.5) · amount · 2`. The split is zero at the center and grows toward the edges, like a cheap lens.
- Keep `amount` tiny (a fraction of a percent of the screen). Past that, text in the scene becomes hard to read.

## Rain on the lens (neverwin shader)

The look: drops slide down the screen leaving trails, small static droplets sit still, the scene behind is blurred except where a drop refracts it sharp.

- **Drop height map**: two layers of moving drops on a grid (6 × 1 aspect cells, the second layer at 1.85× scale) plus a dense grid of static droplets (40 × 40). Each cell gets a hashed x offset, a wiggle as it falls and a saw-tooth fall timing. An `amount` value fades the three layers in one after another.
- **Normals**: sample the height map at `p`, `p + (ε, 0)` and `p + (0, ε)`; the differences offset the scene UV, which is the refraction.
- **Focus**: blur everywhere (`lerp(3, 6, amount)` levels), fading to sharp inside a drop. The blur is 12 taps on a golden-angle spiral (2.39996 rad steps, radius `√((i + 0.5)/12)`), which gives an even disc without a pattern.
- **Tunables**: amount, drop size, speed, blur, fog.
- The structure looks adapted from a well-known public Shadertoy rain shader that is licensed non-commercial (a guess). Write your own from the description above.

## Rules

- Draw these **before** the HUD. Nothing in the HUD should be blurred, split or darkened.
- Off by default. They tire the eye and cost legibility.
- Animate in and out over about half a second when toggled; a sudden full-screen change reads as a glitch.

## ImGui approach

Out of ImGui's reach on its own. In a standalone mock-up, render the background scene to a texture, run one full-screen pixel shader with these passes, then draw the ImGui HUD on top as usual.

## Seen in

- [neverwin — visuals](neverwin-visuals.md): vignette, chromatic aberration and rain in one pass.
- [Fatality — in-game visuals](fatality-visuals.md): startup overlay (dimmed, blurred backdrop) and the menu wordmark's circling red/blue fringe.

Related: [HUD principles](hud-principles.md), [Depth, glass and glow](../principles/depth-glass-and-glow.md), [Glow lines](glow-lines.md)
