---
title: Group boxes
date: 2026-11-06
tags: [widgets, layout, groupbox]
---

# Group boxes

## Anatomy

- Uppercase header text **above** the box's first row, inside the box's top padding (no separate title bar).
- Surface one value step off the window base, 1 px border a step darker, 3–5 px rounding.
- Equal widths per column; height fills the column (scroll inside if needed).

## Variant: untitled cards (velocity.cat)

- No header text and no border. The card is a rounded panel (radius 12, 10 px padding) at `card` (32% black) on a translucent window.
- Height fits the content, not the column; several cards stack per column with 8 px between them.
- Grouping comes only from which rows share a card. It works when cards are short (3–7 rows); add small headers if a column gets more than about three cards.
- In ImGui: `BeginChild(id, ImVec2(w, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysUseWindowPadding)`. See `BeginCard` in [Glass sidebar menu](../implementation/glass-sidebar-menu.md).

## ImGui approach

`BeginChild(id, size, ImGuiChildFlags_Borders)` (or `true` for the border argument in older versions) with pushed `ChildBg`, `Border` and `ChildRounding`. Draw the header with a bold font via `PushFont`, then the rows. Split columns with `ImGui::GetContentRegionAvail().x` divided by column count minus spacing.

## Seen in

- [Fatality — misc tab](../references/fatality-misc-tab.md): three columns.
- [velocity.cat — main menu](../references/velocity-cat-menu.md): untitled, auto-height cards in two columns.
- [gamesense — main menu](../references/gamesense-menu.md): 1 px `#2d2d2d` border, fill one step above the page, bold title on the top border with the line broken under it; boxes scroll on their own.
- [Fatality — first tab](../references/fatality-rage-tab.md): three full-height panels, title inside the top-left, panel edge darker (grey) or lighter (indigo) than the panel.
- [Neverlose — themes](../references/neverlose-themes.md): three styles: underline only, filled card with a title row, and small-caps label above an outlined card with row dividers.
- [Fatality CS2 — source](../references/fatality-cs2-source.md): radius 4, 1 px outline, title centred on the top border over a 2 px break; 16 px side padding; width (W − 80) / 3.
- [Neverlose Recurrence — menu](../references/neverlose-recurrence-menu.md): `#15141d` at 30 % with a 5 % white inside edge, radius 11.5, small-caps label above; row dividers inset 12 px so they stop short of the edge.
- [NL loader — dashboard and modal](../references/nl-loader-dashboard.md): 80 px cards with a 1.5 px outline used as list rows; inactive cards at 50 %, overflow cards blurred.

Related: [Layout and density](../principles/layout-and-density.md), [Style baseline](../implementation/imgui-style-baseline.md)
