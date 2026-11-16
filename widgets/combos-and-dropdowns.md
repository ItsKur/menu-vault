---
title: Combos and dropdowns
date: 2026-11-06
tags: [widgets, combo, dropdown]
---

# Combos and dropdowns

## Field

- Inset background, ~110–130 px wide, right-aligned. Current value in label color, left-aligned inside; small accent or gray chevron on the right.
- Long values truncate with a trailing comma or ellipsis ("Viewangles, …").

### Label-over field (velocity.cat)

- Full card width. The label sits on its own line above a 20 px field (radius 4, `card` fill, no border). The value is 8 px in from the left, and a thin 6 × 3.5 px polyline chevron (1.5 px stroke) sits on the right and flips up when open.
- Hover or open: the fill darkens (`card` alpha 32% → 47%), and the label and value go dim → primary.

## Popup

- Same width as the field (or wider for long items), opens directly below with a small gap.
- Its own soft shadow; optionally blurred background (glass).
- Selected item marked in accent (text color or a left bar). Hover: one value step lighter.
- velocity.cat variant: 22 px items. Selected gets a 16% accent fill and text shifted 40% toward the accent. Only the first and last items round their outer corners. On open, items fade in and slide 8 px into place **one after another**: each starts a little later, and the last starts at half the open animation.

## Multi-select

Each item toggles; the field shows a comma-joined summary. Keep the popup open between clicks (`ImGuiSelectableFlags_DontClosePopups` in older ImGui versions, `ImGuiSelectableFlags_NoAutoClosePopups` in newer ones).

## ImGui approach

`BeginCombo` with `ImGuiComboFlags_NoArrowButton` and draw the chevron yourself, or a fully custom field + `BeginPopup`. Push `ImGuiStyleVar_PopupRounding` and draw a shadow behind the popup. `Combo` in [Glass sidebar menu](../implementation/glass-sidebar-menu.md) is a custom label-over field with `OpenPopupEx` / `BeginPopupEx` and hand-drawn item rows. `ComboRow` in [Custom widgets](../implementation/custom-widgets.md) is the row version: label left, value box right-aligned, list flush under the box.

## Seen in

- [Fatality — misc tab](../references/fatality-misc-tab.md): single and multi-select fields.
- [neverlose-ui](../references/neverlose-ui.md): glass secondary combos with blur and shadow.
- [velocity.cat — main menu](../references/velocity-cat-menu.md): label-over field, blurred dropdown with staggered items, multi-select.
- [Pink sidebar menu](../references/pink-sidebar-menu.md): full-width bordered field with a small chevron.
- [gamesense — main menu](../references/gamesense-menu.md): fixed 150 px flat field under its label, tiny filled triangle, comma list for multi-select.
- [Fatality — first tab](../references/fatality-rage-tab.md): inset field with an accent chevron.
- [Neverlose — themes](../references/neverlose-themes.md): dark field with chevron; truncated comma list for multi-select; a bordered list box as an alternative (light theme).
- [Fatality CS2 — source](../references/fatality-cs2-source.md): long text fades out before the chevron; the chevron flips while open; selected list items turn the accent.
- [Neverlose Recurrence — menu](../references/neverlose-recurrence-menu.md): translucent hairline field, 6 × 3.5 stroked chevron; a long multi-select list fades out (opaque to 86 %, clear at 92 %) instead of using an ellipsis.
- [Lumin framework — main menu](../references/lumin-framework-menu.md): full-width field under a title and description, thin chevron on the right.
- [NL loader — dashboard and modal](../references/nl-loader-dashboard.md): a square icon button opens a 30 px row list that animates in over 0.20 s.

Related: [Depth, glass and glow](../principles/depth-glass-and-glow.md), [Draw-list recipes](../implementation/drawlist-recipes.md)
