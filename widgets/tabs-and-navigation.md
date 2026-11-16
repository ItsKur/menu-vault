---
title: Tabs and navigation
date: 2026-11-06
tags: [widgets, tabs, navigation]
---

# Tabs and navigation

## Header icon tabs (Fatality)

- Horizontal row next to the brand wordmark: icon + uppercase label per tab.
- Inactive: dim (`text_mid` `#6d6496` in the source; `#4c486d` sampled from the misc tab). Active: light text, bold, plus a 2 px accent underline under the label.
- Underline slides between tabs on change; content area cross-fades.

## Sidebar (neverlose-style)

- Vertical list down the left with grouped sections and small headers.
- Active item: accent text or icon, a subtle lighter background pill, sometimes a left accent bar.

## Icon rail + pill sub-tabs (velocity.cat)

- **Rail**: 42 px wide, icons only, no labels. Each tab is a 35 × 35 button stacked 4 px apart on one card; above it sits a separate small card with the logo on an accent pill. Active: solid accent square (radius 8) with the icon in `dark`. Hover: icon tints dim → accent.
- **Sub-tab bar**: 35 px tall card across the top of the content area, split into equal-width buttons. A solid accent pill (radius 8, inset 4 px) slides under the active one (lerp speed 18); its label turns `dark`. Inactive labels are dim. Hover adds a faint `elevated` fill.
- The pill position is stored per tab: it slides within a tab and snaps when the tab changes.
- The bottom of the rail holds the theme preset dots and the avatar.
- A [search](settings-search.md) button shares the bar row as its own square card.

## Profile block

Right end of the header: username, a small meta line (often in accent), round avatar. Purely decorative in a reference build.

## ImGui approach

Not `BeginTabBar`: draw custom `Selectable`-like buttons, store the active index, and animate an underline x/width toward the active tab's rect. See [Animation helpers](../implementation/animation-helpers.md). The rail and the pill bar are in `DrawGlassMenu` in [Glass sidebar menu](../implementation/glass-sidebar-menu.md).

## Seen in

- [Fatality — misc tab](../references/fatality-misc-tab.md): header icon tabs.
- [neverlose-ui](../references/neverlose-ui.md): config and profile secondary menus.
- [velocity.cat — main menu](../references/velocity-cat-menu.md): icon rail, sliding pill sub-tabs, search button.
- [Pink sidebar menu](../references/pink-sidebar-menu.md): sidebar with dim section headers, lighter active row, accent and scale controls in the footer.
- [gamesense — main menu](../references/gamesense-menu.md): 75 px icon rail; the selected tab takes the page color and the divider breaks beside it, so it opens into the page. No accent.
- [Fatality — first tab](../references/fatality-rage-tab.md): header icon tabs with an underline, plain text sub-tabs, a section list with a 2 px left accent bar.
- [Neverlose — themes](../references/neverlose-themes.md): grouped sidebar with a selected pill (neutral, accent-tinted or gradient), user-added items in the accent, and a segmented icon-and-label tab group in the top bar (light theme).
- [Fatality CS2 — source](../references/fatality-cs2-source.md): underline that grows from the left (0.15 s ease-out), bold selected label, an accent "new" dot on the icon; vertical sub-tab bar that grows downward.
- [Neverlose Recurrence — menu](../references/neverlose-recurrence-menu.md): grouped sidebar with no separate surface; the selected item is a white 10 % pill where only the icon takes the accent; a joined save + profile combo in the top bar.
- [Lumin framework — main menu](../references/lumin-framework-menu.md): sidebar icon + label tabs with a lifted fill and a left accent bar; header sub-tabs on a rounded pill next to a search field.

Related: [Layout and density](../principles/layout-and-density.md), [Typography and icons](../principles/typography-and-icons.md), [Motion](../principles/motion.md)
