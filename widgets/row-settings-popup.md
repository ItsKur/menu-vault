---
title: Row settings popup
date: 2026-11-06
tags: [widget, popup, progressive-disclosure]
---

# Row settings popup

A small affordance at the right end of a row, usually three dots (⋯) or a gear, that opens a popup with that row's secondary settings: a duration slider, a style combo, a color. The main surface shows only the on/off toggle; the details are one click away.

## Anatomy (velocity.cat)

- **Trigger**: a 16 px wide hit area at the card's right content edge, as tall as the row. Three filled dots, each 1.5 px radius with 4 px between them, centered in it.
- **States**: dots are `text_dim` and tint toward the accent on hover or while the popup is open (speed about 12).
- **Popup**: 220 px wide, top-left corner 4 px below the trigger's bottom-left. Height fits the content, up to 400 px. It uses the window's own padding (10 px) and spacing (8 px), so any normal widget works inside it.
- **Open animation**: height grows from 0 with ease-out-cubic, and the alpha fades in with it (speed 14 open, 16 close).
- **Background**: `elevated` at 67% over a backdrop blur, radius 8, no border.

## Usage rules

- Use it for settings that only matter when the toggle is on. In velocity.cat, 70 of 153 checkboxes have one.
- Put color swatches in here rather than on the row. It keeps the main surface monochrome, with the accent only marking state.
- One level deep. A combo inside the popup can open its own dropdown, but don't nest row popups.
- Anchor the popup to the trigger, not the mouse, so it opens in the same place every time.
- Clamp it to the window or flip it leftward near the right edge. Anchored at the right column, a 220 px popup extends past the window.

## Compared with the Fatality gear

[Fatality](../references/fatality-misc-tab.md) shows a gear icon *before* the swatch and the control, on the same row. It solves the same problem, but the gear sits in the control cluster, while the dots sit alone at the far edge.

## ImGui approach

Use `LastItemData.Rect` to find the row just drawn. Add a hit area at `WorkRect.Max.x − 16` and call `ButtonBehavior` on it; on click, `OpenPopup`. Then `SetNextWindowPos` under the dots and `SetNextWindowSize(width, 0)` (0 = auto height) before `BeginPopup`. Code: `BeginRowPopup` / `EndRowPopup` in [Glass sidebar menu](../implementation/glass-sidebar-menu.md).

Without blur, make the popup near-opaque, or the rows behind it compete with its contents.

## Seen in

- [velocity.cat — main menu](../references/velocity-cat-menu.md): three-dot trigger on about half the toggles.
- [Fatality — misc tab](../references/fatality-misc-tab.md): gear-icon variant inside the row's control cluster.
- [Neverlose — popups](../references/neverlose-popups.md): the "⋯" button gets a rounded highlight while open; the popup opens to its right with a header (title + three icon buttons) and a scrolling list of cards, each with its own small icon buttons. A list-picker variant rather than a settings form.
- [Fatality — first tab](../references/fatality-rage-tab.md): gear icon before the checkbox.
- [Neverlose Recurrence — menu](../references/neverlose-recurrence-menu.md): three 2 px dots, 3 px apart, left of the row control; also chevron-only "drill-in" rows (trigger only; no popup drawn).

Related: [Toggles and checkboxes](toggles-and-checkboxes.md), [Color pickers](color-pickers.md), [Layout and density](../principles/layout-and-density.md), [Motion](../principles/motion.md)
