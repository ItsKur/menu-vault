---
title: Color and contrast
date: 2026-11-06
tags: [principles, color]
---

# Color and contrast

## The pattern

1. **A dark base with a hue tint, never pure gray or black.** Fatality uses a navy-violet (`#110f23` body, `#191534` boxes). The tint is what makes it feel designed instead of default. Pick the tint from the accent's family or its complement.
2. **Three or four value steps of the same hue** for structure: window body, group-box surface, inset control background (darker than the surface so controls read as "cut in"), and a border. Steps between surfaces are small, often 3–8 in each RGB channel; the border can sit below or above the surface.
3. **One saturated accent**, used sparingly: the active tab, checked states, slider fill, small dots. If more than ~5% of the menu's pixels are accent, it stops being an accent.
4. **Text in three tiers**: near-white for titles and section headers, mid gray-violet for labels, dim for disabled or inactive tabs. Tint the grays toward the base hue.
5. **User colors are the exception.** Color swatches (user-picked colors) are the only place other saturated hues appear.

## Building a palette from one accent

| Role | Rule of thumb | Fatality, exact (source) | Fatality misc tab, sampled |
|---|---|---|---|
| Window body | very dark, accent-adjacent hue | `#110f23` | `#191635` |
| Group-box surface | one step lighter than the body | `#191534` | `#171229`–`#161736` |
| Raised detail (popup title, key cap) | one more step up | `#211b45` | |
| Inset / control bg | darker than the surface; can equal the body | `#110f23` | `#110f24` |
| Border | a step off the surface it edges | `#3c355d` (lighter) | `#101227` outer frame (darker) |
| Text primary | near-white, slightly warm or tinted | `#f5f5f5` | `#e8dbe4` |
| Section header | light gray-violet | `#f5f5f5` | `#c1c0d0` |
| Label | mid gray-violet | `#f5f5f5` | `#8f8ea0` |
| Inactive / secondary | dark gray-violet | `#6d6496` | `#4c486d` |
| Accent | one saturated hue | `#eb055a` | `#ec035c` |

The exact column is from [Fatality CS2 — source](../references/fatality-cs2-source.md); it uses only two text tiers, and weight (bold) for selection. The sampled column comes from a compressed thumbnail ([misc tab](../references/fatality-misc-tab.md)), so it is approximate. It arranges the shades differently (lighter window, near-invisible boxes), and it is what the [Style baseline](../implementation/imgui-style-baseline.md) currently defaults to. Thin accent marks sample darker (`#ac2560`, `#84225b`) because anti-aliasing blends them with the base.

## Alternative: tokens plus alpha (translucent menus)

[velocity.cat](../references/velocity-cat-menu.md) builds a whole palette from **six tokens**: `accent`, `dark`, `text`, `text_dim`, `card` and `elevated`. Instead of picking a separate hex for every surface, it uses the same few colors at different alphas:

| Role | Token × alpha |
|---|---|
| Window | `elevated` × 69% (over blur) |
| Card, inset field, track | `card` (near-black) × 32% |
| Hover fill | `card` × 47% |
| Selected row | `accent` × 16% |
| Separator | `accent` × 12% |
| Dim text | `accent` × 32% |
| Content on accent fills | `dark`, opaque |

- Because the dim text tier is the **accent at low alpha** rather than a gray, the whole UI picks up the theme's hue.
- `dark` and `text` are near-black and near-white **tinted toward the accent**.
- This makes **theme presets** trivial: swap six values and everything stays coherent. velocity.cat ships five (periwinkle `#adc0ff`, lavender `#bea4ff`, mint `#7ae0b5`, peach `#ffab70`, silver `#d2d6e0`).
- It only works on a translucent window over something with low detail (a blur). On an opaque window, use the value-step method above.

## Surfaces vs controls

The owner's preference (2026-11-06), taken from Fatality: **the controls must contrast with the surface they sit on**. In the first mockup the owner tried (since removed), the combo fields and switch tracks were nearly the same value as the cards, and the result felt flat.

- **Inset controls** (fields, slider tracks, off-switches, swatch wells) are clearly darker than their card: Fatality's inset `#110f23` on a `#191534` box is the model. The difference should be visible at a glance, not 2–3 points.
- **Raised controls** (buttons, the selected nav item, chips) are lighter than their card and get a lit top edge.
- Keep the steps in one hue family, tinted toward the base, so contrast doesn't turn into noise.
- **Theme colors were the owner's main complaint about that mockup.** The accent and base still need choosing together. Present options side by side (presets) rather than one guess.

## Common mistakes

- Using ImGui's default blue alongside a custom accent. Override every `ImGuiCol_*` that can show color.
- Pure `#000` shadows on a tinted base; tint the shadow too, or use the border color at low alpha.
- Bright white labels everywhere, which flattens the hierarchy.

## Seen in

- [Fatality — misc tab](../references/fatality-misc-tab.md): navy base, hot-pink accent.
- [neverlose-ui](../references/neverlose-ui.md): one globally controlled accent.
- [velocity.cat — main menu](../references/velocity-cat-menu.md): six alpha-driven tokens, five theme presets, accent-tinted dim text.
- [Pink sidebar menu](../references/pink-sidebar-menu.md): pure neutral grays (`#0d0d0d`–`#ffffff`), pale pink only on set values. Panels barely separate from the base.
- [gamesense — main menu](../references/gamesense-menu.md): `#0a0a0a`–`#111111` grays separated by 1 px lines, one user accent as a top-to-bottom gradient, and a hue stripe as the only decoration.
- [Fatality — first tab](../references/fatality-rage-tab.md): the owner's favourite scheme. Same layout on charcoal grey (`#151515`/`#1c1c1c`) and deep indigo (`#110f24`/`#191635`): body, raised surfaces, and inset controls back down to the body colour.
- [Neverlose — themes](../references/neverlose-themes.md): one layout in five palettes (lime, blue, navy, pink, light); the Edit Style window lists every colour role a theme sets; label brightness shows on/off.
- [Fatality CS2 — source](../references/fatality-cs2-source.md): the exact theme: accent `#eb055a`, two backgrounds (`#110f23` body and controls, `#191534` boxes), outline `#3c355d`, text `#f5f5f5` / `#6d6496`. Changing the accent re-targets every running animation.
- [Neverlose Recurrence — menu](../references/neverlose-recurrence-menu.md): cool blue accent `#4b7dfe` used only for on-state fills and the selected nav icon; HUD state colours are muted per indicator with one saturated critical red.
- [NL loader — dashboard and modal](../references/nl-loader-dashboard.md): near-black base, blue-black cards, three desaturated blues and one orange progress bar.
- [Lumin framework — main menu](../references/lumin-framework-menu.md): three close surface steps with no borders; the accent marks only on-state and values.

Related: [Avoiding AI defaults](avoiding-ai-defaults.md) (contrast ratios of these palettes), [Typography and icons](typography-and-icons.md), [Style baseline](../implementation/imgui-style-baseline.md), [Toggles and checkboxes](../widgets/toggles-and-checkboxes.md)
