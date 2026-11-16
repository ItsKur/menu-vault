---
title: Segmented capsules
date: 2026-11-06
tags: [visuals, hud, watermark, toast, capsule]
---

# Segmented capsules

One small building block used for a whole family of HUD pieces: an outer glass capsule that holds a row of **inner cells**, each with its own rounded background. velocity.cat builds its watermark, keybind rows and event toasts from it, which is why they all look related.

## Anatomy (velocity.cat, exact from source)

| Part | Value |
|---|---|
| Outer capsule | 24 px tall, radius 8, `elevated` `#1f1f23` at 69% over a backdrop blur |
| Inner cells | inset 2 px from the capsule and 2 px apart, radius 6 |
| Text cell | `card` `#111111` at 32%, 8 px horizontal padding, text nudged down 0.5 px |
| Icon / brand cell | solid accent `#adc0ff`, content in `dark` `#111111`; 20 px wide for an icon |
| Text color | key words in accent, connector words and units in `text_dim` (accent at 32%) |

The colors are the [velocity.cat theme tokens](../references/velocity-cat-menu.md), so the HUD follows the menu's theme preset.

## Uses

- **Watermark** (top-right, 10 px margin): a brand cell (logo + name, accent filled), then cells for user, map, ping (`24` accent + ` ms` dim), speed, fps, tick and time. Cells are optional. The total width is eased toward the target (lerp at speed 8), so toggling a cell slides the left edge instead of jumping it.
- **Event toast** (top-left, 15 px margin, 3 px between toasts): an icon cell (hit / miss glyph, accent filled), then one text cell made of **spans** that alternate dim and accent, e.g. dim "changed " + accent "option a" + dim " to " + accent "on". The toast slides in from −150 px on a spring (stiffness 180, damping 16, a little overshoot), holds, and fades out over its last 20% (0.5 s). The blur is dropped once fading starts.
- **Keybind row**: a name cell plus an optional accent-filled value cell; see [Keybind list](keybind-list.md).

## Why it works

- A single recognizable shape across every HUD piece. Users learn it once.
- Cells give structure without dividers: the 2 px gaps act as separators.
- Coloring the spans makes a sentence scannable. The eye jumps to the accent words, so the dim connector words can be ignored.
- A filled accent cell marks the "identity" or most important cell (brand, value) and works like a bullet.

## ImGui approach

`Capsule(dl, pos, width, cells, n, alpha, style)` and `CapsuleWidth(cells, n)` in [HUD elements](../implementation/hud-elements.md). A `Cell` is either a glyph or a list of `Span {text, accent}`. `ToastStack` stacks them with the slide-in spring and fade-out. To keep the watermark's edge smooth, pass a spring-smoothed width; the capsule clips its cells while it animates.

## Seen in

- [velocity.cat — HUD](velocity-cat-hud.md): watermark, toasts, keybind rows.
- [neverwin — visuals](neverwin-visuals.md): watermark options only (shape unconfirmed): six anchor positions, compact / default / spacious density, selectable items, warning and critical value colors.
- [Fatality — in-game visuals](fatality-visuals.md): 200 × 42 toasts: blur + body colour at 74 %, accent title, slide down 20 px, 5 s life.
- [Neverlose Recurrence — HUD](neverlose-recurrence-hud.md): spectator chips that end in a round avatar flush with the chip's right end; a monospace hit log row with a steel accent bar.

Related: [Keybind list](keybind-list.md), [Status pills](status-pills.md), [HUD principles](hud-principles.md), [velocity.cat — main menu](../references/velocity-cat-menu.md)
