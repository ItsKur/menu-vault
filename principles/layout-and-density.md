---
title: Layout and density
date: 2026-11-06
tags: [principles, layout]
---

# Layout and density

## Window skeleton

```
┌──────────────────────────────────────────────────────────────┐
│ BRAND   ◎ TAB  ◎ TAB  ◎ TAB  [◎ TAB]  ◎ TAB         user  (◯) │  header bar
│                         ‾‾‾‾‾                    meta line     │  accent underline on active tab
├──────────────────────────────────────────────────────────────┤
│ ┌ SECTION ─────────┐ ┌ SECTION ─────────┐ ┌ SECTION ───────┐ │
│ │ Label        [✓] │ │ Label        [✓] │ │ Label      [ ] │ │
│ │ Label   [combo ▾]│ │ Label ⚙ [combo ▾]│ │ Label [combo ▾]│ │
│ │ Label   ■ ■  [✓] │ │ Label  [■■■70%  ]│ │ ...            │ │
│ └──────────────────┘ └──────────────────┘ └────────────────┘ │
└──────────────────────────────────────────────────────────────┘
```

- **Header bar**: brand left, horizontal icon tabs, user/profile block right. Alternative: a vertical sidebar (common in neverlose-style menus).
- **Content**: 2–3 equal-width columns of group boxes. Columns scroll independently if long.
- **Aspect**: wide and short (roughly 16:10 or wider), ~800×500 at 100% scale.

### Alternative skeleton: icon rail + pill bar

```
┌───────────────────────────────────────────────┐
│ [▲] [ main ][ extra ][ advanced ]        [⌕] │  35 px bar, sliding accent pill
│ [◉] ┌─────────────────┐ ┌─────────────────┐  │
│ [▭] │ ■ option     ⋯ │ │ □ option     ⋯ │  │  untitled cards, auto height
│ [▮] │ amount     65% │ │ scale      1.2x │  │
│ [◍] │ ▬▬▬▬▬▬▬▬▬▬▬▬▬▬ │ │ ▬▬▬▬▬▬▬▬▬▬▬▬▬▬ │  │
│  •  └─────────────────┘ └─────────────────┘  │
│ [☺] ┌─────────────────┐                      │  theme dots + avatar at the rail's foot
└───────────────────────────────────────────────┘
```

- 42 px icon rail on the left, a sub-tab bar across the top, and two equal columns of cards. 700 × 450.
- **One gap (8 px) everywhere**: window edge, rail to content, bar to body, column to column, card to card. This menu uses no other spacing value outside the cards.
- See [velocity.cat](../references/velocity-cat-menu.md).

## Row anatomy

Label left-aligned; controls right-aligned to the group box's inner edge. Optional extras sit between label and control in a fixed order: **gear (sub-settings) → color swatches → main control**. Every row is the same height (~22–24 px), which is what makes the columns line up visually. Controls taller than the row (default ImGui frames are ~25 px) break this; size fields to the row instead.

Alternative (velocity.cat): **control first**. The checkbox or swatch sits left with its label right after it; sliders and combos put their label on a line above and span the full card width. The only thing on the right edge is a "⋯" [row settings popup](../widgets/row-settings-popup.md). This works well in narrower two-column cards. Sub-settings move into the popup instead of stacking gear → swatch → control on the row.

## Spacing

- Window padding 14–18 px; gap between group boxes 10–14 px.
- Group-box inner padding 10–12 px; row spacing 6–8 px.
- Consistency beats generosity: these menus are dense, but every gap is the same.

## Density cues

- Small controls (checkbox ~10–12 px square, combos ~110–130 px wide).
- Uppercase section headers sitting above the box content, not in a title bar.

## Seen in

- [Fatality — misc tab](../references/fatality-misc-tab.md): header tabs, three columns, gear-swatch-control rows.
- [velocity.cat — main menu](../references/velocity-cat-menu.md): icon rail + pill bar, uniform 8 px gaps, control-first rows, untitled cards.
- [Pink sidebar menu](../references/pink-sidebar-menu.md): grouped sidebar, a long scrolling list with a bottom fade, sliders with value boxes.
- [gamesense — main menu](../references/gamesense-menu.md): checkbox column plus one label indent, fixed-width controls in wide groups, key tags and swatches on the right edge.
- [Fatality — first tab](../references/fatality-rage-tab.md): header tabs, text sub-tabs, a section column, three full-height panels, footer icon bar.
- [Neverlose — themes](../references/neverlose-themes.md): sidebar + top bar + two columns, unchanged across five themes.

Related: [Group boxes](../widgets/group-boxes.md), [Tabs and navigation](../widgets/tabs-and-navigation.md), [Typography and icons](typography-and-icons.md)
