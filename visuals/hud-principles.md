---
title: HUD principles
date: 2026-11-06
tags: [visuals, hud, principles]
---

# HUD principles

A menu sits on its own surface and gets full attention for a moment. HUD elements sit on top of a moving, unpredictable scene for hours, so they follow different rules.

## 1. Readable over anything

- **Put a dark container behind all text.** Black at 40–50% (the [owner captures](owner-hud-captures.md) sample at about 45%) or a blurred glass panel. Bare text gets lost on bright floors and skies.
- **Add a soft halo** (a 4–8 px dark shadow) around the container. It separates the element from a background of similar value without a hard border.
- Text is near-white (90–95%); secondary text (key names, units) is the same white at 40–50%. Two tiers are enough.
- A heavy weight at 13–15 px for short codes. Bold reads better in motion than regular.

## 2. Small, at the edges, out of the center

- Put elements against the left and right edges at mid-height, or in the corners, with a margin of 10–18 px. Keep the center clear; the only exceptions are momentary feedback ([hit markers](hit-markers.md)).
- Vertically center a list on the screen and let it **grow from the middle**. velocity.cat's keybind list recenters itself on a spring as rows come and go.
- Fit the width to the content and animate width changes (spring or lerp), so the edge doesn't jump when a value changes length.

## 3. State is color, not shape

- An element's shape shouldn't change when it turns on. Change the **content color** (icon and text red in the owner captures) or fill an inner cell with the accent. Changing the layout would make the eye re-scan.
- Keep one "attention" color for active states, separate from the decorative accent.
- Dim (don't hide) elements when they matter less. In one owner capture the whole HUD drops to about 40% while the scene is dark, which is likely a "menu open" or low-priority state (unconfirmed).

## 4. Motion: appear softly, leave quickly

- Fade in over about 0.2 s, out over about 0.15 s, both with ease-out.
- Position changes use a **spring** (velocity.cat: stiffness 180–200, damping 16–20, slightly underdamped). Rows that slide up into a removed row's slot read as "the same list, updated" rather than a redraw.
- New transient items slide in from the edge they're anchored to (toasts: from −150 px).
- Feedback marks (hits) **overshoot and settle** within about 50 ms, then fade over about 0.5 s with a curve that holds early and drops late (alpha = 1 − p²).
- Nothing loops or pulses forever on a HUD. If something is active for a long time, its steady state must be calm.

## 5. In-world markers

- Draw on the surface the thing affects. A ground area belongs on the floor, foreshortened with perspective; see [Ground area markers](ground-area-markers.md).
- Use a strong outline plus a faint fill. The outline carries the shape; the fill only tints, so the scene stays visible inside it.
- Grow and shrink the shape smoothly over time (ease each radius toward its target), and fade it out using the last known shape instead of letting it vanish.

## 6. Exposing tunables

Menus that ship HUD elements often expose their motion. The [pink sidebar menu](../references/pink-sidebar-menu.md) has a "keybind animation" panel with fade speed, move and shift speeds (px/s), spacing, a remove distance and an overlap. Give users a few named presets (static, animated) plus these sliders; never only the sliders.

## 7. Other games and scales

The values in this note assume a dark scene at 1080p. When designing for a different art style, resolution or renderer, read [Portability across games](../principles/portability.md) first: it covers light and scene-matched palettes, scaling, colour space and fallbacks for hosts without shaders or blur.

Related: [Visuals](INDEX.md), [Portability across games](../principles/portability.md), [Motion](../principles/motion.md), [Color and contrast](../principles/color-and-contrast.md), [Depth, glass and glow](../principles/depth-glass-and-glow.md)
