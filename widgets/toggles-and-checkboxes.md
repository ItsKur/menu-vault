---
title: Toggles and checkboxes
date: 2026-11-06
tags: [widgets, toggle, checkbox]
---

# Toggles and checkboxes

## Variant A: square checkmark (Fatality)

- ~10–12 px square, right-aligned on the row, inset background (`bg_bottom` `#110f23`, the body colour).
- Checked: accent checkmark drawn as a polyline, no filled box. The box stays dark.
- Unchecked: empty inset square, sometimes with a faint border.
- Whole row is clickable, not just the box.

## Variant B: pill switch (neverlose-style)

- Track ~28×14 px, fully rounded. Off: inset color. On: accent.
- Circular knob slides left→right with easing; track color fades with the same `t`.

## Variant C: filled square, box first (velocity.cat)

- 16 px square, radius 4, **left** of the label (8 px gap). It is not right-aligned, and the row is only as wide as box plus label.
- Off: `card` fill (32% black). On: the whole square fades to solid accent (smoothstep). It has **no checkmark**.
- The label goes dim → primary when on *or* hovered, so a column of rows shows at a glance which ones are on.
- Optional key badge after the label; its border tints toward the accent when the toggle is on. Right-click opens a bind-mode menu (toggle / hold on / hold off).
- Secondary settings go in a [row settings popup](row-settings-popup.md) behind "⋯" at the right edge.
- Weakness: off boxes nearly vanish on a `card` background. Add a faint border if contrast matters.

## States

| State | Treatment |
|---|---|
| Hover (row) | label brightens one tier |
| Checked | accent mark / accent track |
| Disabled | 40% alpha on the whole row |
| Has sub-settings | gear icon before the control (A), or "⋯" at the row's right edge (C) |

## ImGui approach

Use `ButtonBehavior` over the full row rect, store animation `t` in `ImGui::GetStateStorage()`, and draw with the window draw list. See `PillToggle` and `CheckRow` in [Custom widgets](../implementation/custom-widgets.md), and `Checkbox` (variant C) in [Glass sidebar menu](../implementation/glass-sidebar-menu.md).

## Seen in

- [Fatality — misc tab](../references/fatality-misc-tab.md): variant A.
- [neverlose-ui](../references/neverlose-ui.md): variant B with linear animation.
- [velocity.cat — main menu](../references/velocity-cat-menu.md): variant C, with key badges and "⋯" popups.
- [Pink sidebar menu](../references/pink-sidebar-menu.md): right-aligned outline squares with a keyboard icon before them on bindable rows.
- [gamesense — main menu](../references/gamesense-menu.md): 8 px square with a vertical accent gradient (dark gradient when off), in its own column left of the label indent; dim `[key]` tags at the row's right edge.
- [Fatality — first tab](../references/fatality-rage-tab.md): variant A, tiny inset squares at the panel's right edge.
- [Neverlose — themes](../references/neverlose-themes.md): variant B in five palettes; off switches tinted to the palette (pink theme).
- [Fatality CS2 — source](../references/fatality-cs2-source.md): two-stroke accent checkmark; a faint checkmark in the outline colour previews the click on hover.
- [Neverlose Recurrence — menu](../references/neverlose-recurrence-menu.md): 25 × 16 stadium, `#4b7dfe` on, near-black track with a grey knob off; a "⋯" trigger left of bindable rows (Figma concept, exact values).
- [Lumin framework — main menu](../references/lumin-framework-menu.md): 36 × 21 pill, ring knob (7 px with a dark hole) and a small tick on the far side; the track stays dark when on, only the knob turns accent.
- [imgui-menu-clone](../references/imgui-menu-clone.md): checklist for telling a ring knob from a filled one; ring drawn as two filled rects.

Related: [Motion](../principles/motion.md), [Color and contrast](../principles/color-and-contrast.md), [Layout and density](../principles/layout-and-density.md)
