---
title: Glow lines
date: 2026-11-06
tags: [visuals, in-world, line, glow, trail]
---

# Glow lines

A line that reads as a lit tube: a solid core, a soft glow around it, optional taper and fades at both ends. Used for paths, trails, predicted arcs and beams drawn over the scene. The input is a polyline of screen points, each with a width and an optional depth for the fade.

## Anatomy (neverwin shader, exact constants)

- **Core**: a capsule around the segment `a → b`. For each pixel, take `t` = the projection onto the segment clamped to 0–1, and `d` = the distance to the nearest point. Half-width is `lerp(w_start, w_end, t)`, so the line can taper.
- **Antialiasing**: edge width `max(|ddx e| + |ddy e|, 1)` with `e = d − half_width`. Coverage is a 1 px ramp across the edge.
- **Glow**: outside the core, `(1 − e / glow_radius)³ · strength`. The cube makes it bright near the core and fall off fast, so it reads as a halo, not a fat line. The glow radius tapers like the width.
- **Highlight**: the center is whitened by up to 25% (`(1 − d²/hw²) · 0.25`, mixed toward white). This is the trick that turns a flat stroke into a tube.
- **End fades**: alpha ramps in over `start_fade` and out over `end_fade` along the line. The parameter is corrected for perspective, so on a line going into the distance the fade is even along its real length, not along its screen length.

## Variations

- **Points along the line**: small discs at even steps, with their own thickness and an exponent that bunches them toward one end (seen in the trajectory option list).
- **Shimmer**: a bright band that travels along the line (the option exists; its look is unknown).
- **Endpoint color**: the line switches to a second color when it ends on something.

## Rules

- Keep the core thin (1.5–3 px). The glow does the work of making it visible.
- Fade both ends, even slightly. A line that stops at full alpha looks clipped.
- Draw the glow on a shared glow layer, not per line, so overlapping lines don't stack into white blobs.

## ImGui approach

Without a shader: draw each segment as two quads with per-vertex color (core at full alpha, then a wider band from core alpha to 0 at its outer edge), and a third narrow lighter quad down the middle for the highlight. ImGui's own lines are single-color, so write the vertices with `PrimReserve` / `PrimWriteVtx`, as `GradientLine` in [HUD elements](../implementation/hud-elements.md) does (its gradient runs along the length, not across it). `Beam` in [Fatality-style in-world visuals](../implementation/fatality-visuals-code.md) approximates the falloff with stacked layers instead. Multiply each vertex's alpha by the end fade. With a shader: one quad per segment, enlarged by the glow radius, shaded as above.

## Seen in

- [neverwin — visuals](neverwin-visuals.md): trajectory line, beam lines; the tube shader.
- [Neverlose — HUD](neverlose-hud.md): tracer close-up: white hot core fading toward the far end, faint warm glow, small magenta dots along the line.

Related: [HUD principles](hud-principles.md), [Tracers](tracers.md), [Ground area markers](ground-area-markers.md), [Hit markers](hit-markers.md), [Depth, glass and glow](../principles/depth-glass-and-glow.md)
