---
title: Countdown badges
date: 2026-11-06
tags: [visuals, hud, timer, indicator, off-screen]
---

# Countdown badges

A round badge that says three things at once: *what* (icon), *how long* (a timer ring that runs out), and *where* (an arrow pointing toward the thing it tracks). When the tracked thing is off-screen, the badge is clamped to the screen edge and the arrow points outward toward it.

## Anatomy (velocity.cat, exact from source)

| Part | Value |
|---|---|
| Disc | radius 18, background color with alpha (user setting), 32 segments |
| Icon | centered, tinted (a fire or other type glyph) |
| Timer arc | radius 22, 2.75 px thick, starts at 12 o'clock and runs **clockwise**. It is drawn for the remaining fraction, with segments scaled to the arc length (minimum 3). |
| Arrow | tip at radius 36.5, base at radius 25.5 (3.5 px clear of the arc), ±0.45 rad wide. The base follows the ring's curve, so the arrow looks attached. |
| Glow | optional thicker copy of the arc and arrow on a glow layer |
| Edge clamp | 52 px inset from the screen edge |
| Fade | fades in and out at speed 3 (about 0.33 s) |
| Anchor | placed 80 units above the tracked point in the world, so it doesn't sit on the floor marker |

## Rules

- The ring shows time *remaining*: full when it starts, empty when it ends. Use one color for the ring and the arrow so they read as one object.
- Show the arrow only when the direction is useful (off-screen, or behind the viewer).
- Clamp along the ray from the screen center, not to the nearest edge point. Otherwise the arrow lies about the direction near the corners.
- Pair it with an in-world marker when the thing is visible ([Ground area markers](ground-area-markers.md)). The badge is for "how long"; the marker is for "where exactly".

## ImGui approach

`CountdownBadge(dl, center, glyph, frac, arrow, dir_radians, alpha)` and `ClampToEdge(target, screen_rect, 52)` in [HUD elements](../implementation/hud-elements.md). The arc uses `PathArcTo` + `PathStroke`. The arrow is a convex polygon: the tip plus an 11-point arc along the base.

## Seen in

- [Fatality Halloween — proximity warnings](fatality-halloween-proximity-warning.md): in-world striped/filled molotov area and grenade-local red glow, with separate small item/timer annotations.

- [Projectile timing](projectile-timing.md): separates predicted arrival from active lifetime.

- [velocity.cat — HUD](velocity-cat-hud.md)
- [neverwin — visuals](neverwin-visuals.md): a related bare offscreen arrow (no disc or timer) at a set radius from the screen center, with fill, background and shadow colors.
- [Fatality — in-game visuals](fatality-visuals.md): off-screen triangles on a screen-shaped ellipse, alpha blinking every 0.73 s.

Related: [HUD principles](hud-principles.md), [Ground area markers](ground-area-markers.md), [Hit markers](hit-markers.md)
