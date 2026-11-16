---
title: velocity.cat — HUD
date: 2026-11-06
tags: [visuals, reference, repo, hud, glass, segmented-capsule]
source: https://github.com/hypnomacka/velocity.cat
---

# velocity.cat — HUD

The on-screen elements from the same source as [velocity.cat — main menu](../references/velocity-cat-menu.md). All values are **read from source**, so they are exact. The same licensing caveat applies: it is a leaked source with no license, so study it but don't copy it. The recipe in this vault is written independently.

Only the drawing code was read: `core/rendering/impl/widgets.cpp` (watermark, keybind list), `core/features/misc/impl/impacts.cpp` (the toast and hit-marker drawing functions only), `core/features/esp/projectile/projectile.overlay.cpp` (the badge and area-outline drawing only) and `utilities/animation.hpp` (spring and fade). How the source gets its data from the game is out of scope and is not described here.

## Shared design language

Every HUD piece is a [segmented capsule](segmented-capsules.md) built from the menu's theme tokens: an outer glass capsule (24 px, radius 8, window color over blur) holding inner cells (inset 2 px, radius 6). Accent-filled cells hold the important bit, with content in `dark`. Text is accent for key words and dim for everything else. When the user switches the menu's theme preset, the HUD recolors with it.

## Elements

| Element | Placement | Notes | Vault note |
|---|---|---|---|
| Watermark | top-right, 10 px margin | brand cell (accent) + optional cells for user, map, ping, speed, fps, tick, time; values accent, units dim; width eased (speed 8); fps value smoothed (speed 2) so it doesn't flicker | [Segmented capsules](segmented-capsules.md) |
| Keybind list | left edge, vertically centered | header capsule (accent keyboard cell + "keybinds") and one capsule per active bind, ragged right edge. Accent value cell for binds with a number. Toggle binds dim, hold binds accent. Rows fade 0.2 s in / 0.15 s out and spring (200 / 20) into their slot; the list's top springs to keep it centered | [Keybind list](keybind-list.md) |
| Event toasts | top-left, 15 px margin, 3 px apart | icon cell + a sentence of dim and accent spans; slide in from −150 px (spring 180 / 16), fade out over the last 20% of their life (0.5 s), and drop the blur once fading starts | [Segmented capsules](segmented-capsules.md) |
| Hit marker | at the impact point | four 1.25 px arms that are transparent at the inner end; 6 px overshoot snaps in over about 50 ms; alpha 1 − p²; optional number 20 px above with a 125 ms shake; optional glow | [Hit markers](hit-markers.md) |
| Countdown badge | above the tracked point, clamped 52 px inside the screen | disc r18, timer arc r22 (2.75 px, clockwise from 12), arrow from r25.5 to r36.5; optional glow | [Countdown badges](countdown-badges.md) |
| Area outline | on the tracked area | 15 eased directions per emitter, screen-space convex hull, outline plus 8 shrunk fills for the gradient; fades out (speed 3) from its last shape | [Ground area markers](ground-area-markers.md), method B |

## Animation primitives

- **Spring**: `v += ((target − x)·k − v·c)·dt; x += v·dt`. The default is k = 200, c = 20; toasts use 180 / 16. Both are below critical damping (2√k ≈ 28 and 27), so they overshoot slightly, which gives a soft landing.
- **Fade**: a tween with ease-out over a fixed duration; `visible()` stays true until the fade-out finishes, so the element keeps drawing while it leaves.

## What makes it work

- One shape for everything. The watermark, binds and toasts are immediately recognizable as one product.
- Theme tokens are shared with the menu, so one preset switch recolors the whole UI.
- Accent spans make toast sentences skimmable.
- Every appearance and disappearance is animated, and nothing pulses in its steady state.

## What to avoid / weaknesses

- Dim text at 32% accent over a 69% glass capsule is low contrast over bright scenes. The blur helps, but the HUD has no halo like the [owner captures](owner-hud-captures.md) do.
- The ragged right edge of the keybind list looks busy with many rows. Variant A's uniform width is calmer.
- The area outline is a convex hull, so it fills in the concave gaps between lobes. See the comparison in [Ground area markers](ground-area-markers.md).

Related: [Visuals](INDEX.md), [velocity.cat — main menu](../references/velocity-cat-menu.md), [HUD elements recipe](../implementation/hud-elements.md), [HUD principles](hud-principles.md)
