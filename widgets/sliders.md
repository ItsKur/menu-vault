---
title: Sliders
date: 2026-11-06
tags: [widgets, slider]
---

# Sliders

## Anatomy (Fatality)

- Thin rounded bar (~8–10 px tall, ~80–90 px wide), right-aligned like other controls.
- Track is the inset color; fill is solid accent from the left.
- **Value text centered on the bar** ("70%"), white, small. No separate value label and no visible grab handle.

## Variations

- neverlose-style: editable value (double-click or Ctrl+click to type), animated fill transitions, template for `int` and `float`.
- **Label-over** (velocity.cat): full card width. The label is top-left and the value top-right on one line, then a 7 px pill track 2 px below. The fill is solid accent and there is no thumb. The value tints toward the accent on hover (65%) and drag (85%). Click the value to type; arrow keys nudge it while hovered. It suits two-column layouts where a right-aligned 90 px bar would leave a gap.
- A small circular grab can be added for touch-like menus; leave it off for dense ones.

## Behavior

- Click anywhere on the bar to set; drag to scrub.
- Animate the displayed fill toward the true value so clicks feel smooth.
- Show units in the formatted text (`%d%%`, `%.1f ms`).

## ImGui approach

Either restyle `SliderFloat` (`GrabMinSize`, colors, `FrameRounding`) or draw a custom bar with `ButtonBehavior` + mouse position mapping. See `AccentSlider` in [Custom widgets](../implementation/custom-widgets.md), and `Slider` (label-over) in [Glass sidebar menu](../implementation/glass-sidebar-menu.md).

## Seen in

- [Fatality — misc tab](../references/fatality-misc-tab.md)
- [neverlose-ui](../references/neverlose-ui.md)
- [velocity.cat — main menu](../references/velocity-cat-menu.md): label-over variant.
- [Pink sidebar menu](../references/pink-sidebar-menu.md): short track with a round knob plus a separate editable value box (which clips long values).
- [gamesense — main menu](../references/gamesense-menu.md): 6 px track, gradient fill, no thumb; the value is centered on the end of the fill and overlaps the track's lower edge.
- [Fatality — first tab](../references/fatality-rage-tab.md): inset value fields with centred text (probably sliders at their minimum; unconfirmed).
- [Neverlose — themes](../references/neverlose-themes.md): thin track, round knob, separate value box, in five accents.
- [Fatality CS2 — source](../references/fatality-cs2-source.md): value text drawn twice, the bright copy clipped to the fill, so the number changes colour where the fill passes; arrow keys and Ctrl + wheel step it.
- [Neverlose Recurrence — menu](../references/neverlose-recurrence-menu.md): 3 px track, 9 px white knob, separate hairline value box at 8 px text (Figma concept, exact values).
- [Lumin framework — main menu](../references/lumin-framework-menu.md): label-over variant with a description line, value right-aligned on the title line, black knob dot at the end of the accent fill.
- [imgui-menu-clone](../references/imgui-menu-clone.md): knob taller than the track (10 px track, 19 px knob), drawn outside the item rect.

Related: [Toggles and checkboxes](toggles-and-checkboxes.md), [Motion](../principles/motion.md)
