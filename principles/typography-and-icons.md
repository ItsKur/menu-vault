---
title: Typography and icons
date: 2026-11-06
tags: [principles, typography, icons]
---

# Typography and icons

## Fonts

- **The owner's choice is Inter** (2026-11-06; they disliked Noto Sans in the first mockup). The static Regular, Medium, SemiBold and Bold files are in `assets/fonts/inter/`, from the official v4.1 release, with their SIL Open Font License. Use them for new menus and previews. ImGui's stb_truetype loader can't pick weights from a variable font, which is why the static files are kept.
- A clean geometric or grotesque sans (Inter, Manrope, Montserrat, Rubik, SF-like fonts). ImGui's default ProggyClean is the single fastest giveaway of an unstyled menu.
- Load the font at the size you render, with oversampling (`OversampleH = 2–3`), or use FreeType (`imgui_freetype`) for crisper hinting at small sizes.
- Load separate `ImFont`s for each size or weight you use rather than scaling one.

## Size and weight tiers

| Tier | Example | Size | Style |
|---|---|---|---|
| Brand / wordmark | "FATALITY" | 18–22 px | Heavy, uppercase, tracked |
| Tab label | nav items | 12–13 px | Medium, uppercase |
| Section header | VARIOUS / MOVEMENT | 11–12 px | Semibold, uppercase, light color |
| Row label | each option | 12–13 px | Regular, mid-gray |
| Meta text | subscription line, values | 10–11 px | Regular, dim or accent |

### Alternative: one size, all lowercase

[velocity.cat](../references/velocity-cat-menu.md) uses a single font at a single size (Inter, 15 px) for everything: tabs, labels, values and combo items. All text is **lowercase**, and cards have no headers. Hierarchy comes from color alone (dim vs primary vs dark-on-accent) and from the cards. The result is soft and calm, but it needs strong containers to replace the missing headers.

## Icons

- Use an icon font merged into the main font (Font Awesome, Material Symbols, Lucide via a TTF). Merge with `MergeMode = true` and a glyph range so icons inline with text.
- Icons go **before** tab labels and as small affordances (a gear for "more settings"). Keep them the same color as their label.
- One icon set per menu; mixing sets looks off immediately.
- An icon-only rail (no labels) works for up to about 7 tabs if the icons are distinct and stroked at the same weight. velocity.cat uses 24 × 24 stroked SVGs at 1.5–1.7 px, tinted at draw time, and turns them `dark` on the active tab's accent fill.

## Seen in

- [Fatality — misc tab](../references/fatality-misc-tab.md): heavy wordmark, uppercase icon tabs, gear affordances.
- [neverlose-ui](../references/neverlose-ui.md): Font Awesome, tuned rasterization.
- [velocity.cat — main menu](../references/velocity-cat-menu.md): single-size lowercase Inter, icon-only rail with SVG icons.
- [Fatality — first tab](../references/fatality-rage-tab.md): letter-spaced uppercase wordmark, uppercase tabs and panel titles.
- [Fatality CS2 — source](../references/fatality-cs2-source.md): Josefin Sans at one size, bold for selection; 18 px bold wordmark; a pixel font with an outline for overlays.

Related: [Avoiding AI defaults](avoiding-ai-defaults.md), [Layout and density](layout-and-density.md), [Tabs and navigation](../widgets/tabs-and-navigation.md), [Style baseline](../implementation/imgui-style-baseline.md)
