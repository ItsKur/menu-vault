---
title: Motion
date: 2026-11-06
tags: [principles, animation]
---

# Motion

Default ImGui snaps instantly between states. Easing every state change over a few frames is one of the largest perceived-quality upgrades, and it's cheap.

## What to animate

- Hover: background or text brightens slightly.
- Toggle / checkbox: knob slides, fill fades into accent.
- Tab switch: underline slides to the new tab; content fades in.
- Slider: fill follows the value smoothly (for typed or clicked values).
- Popups and dropdowns: fade + 4–8 px slide or scale-in.
- Menu open: fade in over ~150 ms (a "startup animation" can be longer).

## Timing

- 80–150 ms for hover and toggles; 150–250 ms for tabs and popups.
- Use frame-rate independent interpolation: `t = lerp(t, target, 1 - exp(-speed * dt))` or the simpler `ImSaturate(dt * speed)` factor. Speed 10–16 feels right for small controls.
- Ease out (fast start, slow end). Never linear for movement; linear is fine for color fades.

### Example speeds (velocity.cat)

velocity.cat lerps everything with `v += (target − v) · min(speed · dt, 1)`. The speeds it uses are a good starting table:

| What | Speed | Curve |
|---|---|---|
| Checkbox fill | 8 | smoothstep |
| Hover tints | 10–15 | linear |
| Window / popup open (close) | 14 (16) | ease-out-cubic; window scales from center, popup grows in height |
| Sub-tab pill slide, search-field expand | 18 | linear / smoothstep |
| Slider fill | 20 | linear |

- Closing is slightly faster than opening.
- Dropdown items enter **one after another**: each fades and slides 8 px, and the last starts at half the open animation. It's cheap, and it makes the list feel alive without slowing the user.

## Don'ts

- No bounce or overshoot in a dense settings menu.
- Don't animate layout size of rows; it makes clicking targets move.
- Don't ship animation without a "Reduce motion" setting; see [Avoiding AI defaults](avoiding-ai-defaults.md#motion).

## Seen in

- [neverlose-ui](../references/neverlose-ui.md): animated pill toggles and sliders, startup animation.
- [velocity.cat — main menu](../references/velocity-cat-menu.md): per-widget speed table, center scale-in, staggered dropdown items, sliding sub-tab pill.
- [velocity.cat — HUD](../visuals/velocity-cat-hud.md): springs (200/20, 180/16) for HUD rows and toasts, a hit marker that overshoots and settles.
- [Fatality CS2 — source](../references/fatality-cs2-source.md): 0.1–0.15 s colour tweens, underlines and side bars that grow, popup fades, a looping red/blue wordmark glitch, and a startup sequence (dim + blur, breathing "F", wordmark slides in).
- [NL loader — dashboard and modal](../references/nl-loader-dashboard.md): splash square that grows to the window in 0.32 s on a custom bezier ease, then a 0.40 s content fade; staged load sequence.
- [Lumin framework — main menu](../references/lumin-framework-menu.md): per-id easing for every widget, click ripple on the primary button.

Related: [Animation helpers](../implementation/animation-helpers.md), [Toggles and checkboxes](../widgets/toggles-and-checkboxes.md), [Tabs and navigation](../widgets/tabs-and-navigation.md)
