---
title: Fatality CS2 — menu drawing code
date: 2026-11-06
tags: [reference, repo, fatality, dark, opaque, accent-pink, motion, exact-values, owner-favorite]
source: https://github.com/notphage/fatality.win-CS2 (commit f4124c6, Jan 2024)
---

# Fatality CS2 — menu drawing code

## Summary

Source code of Fatality's CS2 product. **Its README says it is a leak**, not an official release, so nothing from it is copied here: this note records **values and behaviour** read from its GUI drawing code (`src/gui/`, `src/menu/`), in its own words. Only drawing code was read. It gives exact numbers for the look in [Fatality — first tab](fatality-rage-tab.md), the owner's favourite colour scheme.

The GUI is **not Dear ImGui**. It is a retained-mode control tree (window → layouts → group boxes → rows of label + control) drawn through a separate renderer library (`ev0lve/renderer`, a submodule not included) with numbered draw layers, so popups and tooltips always draw above content. Every hover and selection change runs through small tweens of a colour or a float.

## Palette (exact, from the source)

One `color_list` struct holds the whole theme. Several roles share a value, which shows how few colours the look really needs.

| Role | Hex | Used for |
|---|---|---|
| `accent` | `#eb055a` | checkmark, slider fill, tab underline, combo chevron, scrollbar grab while dragging, focus ring, caret, toast titles, cursor |
| `text` / `text_light` | `#f5f5f5` | labels, values, active tabs |
| `text_mid` / `text_dark` | `#6d6496` | inactive tabs, unfilled slider text, placeholder, secondary words ("expires:") |
| `bg_bottom` | `#110f23` | window body, and **every control's inset field** (checkbox, slider, combo, text box, list) |
| `bg_block` | `#191534` | header, footer, group boxes, popups |
| `bg_block_light` | `#211b45` | popup title bars, key-cap fill, idle scrollbar grab |
| `outline` | `#3c355d` | group-box and popup borders; also the faint checkmark shown on hover |
| `warning` / `danger` / `success` | `#f5ec42` / `#f54242` / `#8fff66` | status messages |

These match the samples from the indigo screenshot (`#110f24`, `#191635`). For the grey variant, swap `bg_bottom`, `bg_block` and `outline` for greys and keep everything else.

The accent is a **live setting**: every animation that targets the accent registers itself, and changing the accent re-targets them all, so the whole menu fades to the new colour.

## Fonts

- **Josefin Sans** (regular and bold) for the whole menu, at one text size (12 px by default). Bold is used for the selected tab, selected sub-tab and highlighted list item, so weight marks selection along with colour.
- **Josefin Sans Bold 18 px** for the "FATALITY" wordmark.
- Overlays use different fonts: see [Fatality — in-game visuals](../visuals/fatality-visuals.md).

## Window and layout

- Window **848 × 588**, corner radius 3, a soft drop shadow (24 px), body `bg_bottom`.
- **Header** 50 px tall in `bg_block` (top corners rounded), with a 12 px shadow fading downward from its bottom edge onto the body. **Footer** 28 px in `bg_block` with a 12 px shadow fading upward. These two shadow strips give the body its "sunk" look between the bars.
- Content area inset 24 px from the sides, starting 66 px from the top. Tabs sit in a strip starting 126 px from the left, between the wordmark and the profile block.
- Three group boxes per tab: width = (window width − 80) / 3, height = window height − 110. Tabs with more content stack two or three shorter boxes in one column.
- Group box content padding: 16 px left and right; content starts 16 px below the box's top.
- **Profile block**: 28 px round avatar (a dark circle with a subtle gradient when there's no picture); name right-aligned in `text`; the expiry value in the accent with "expires:" in `text_dark` in front of it.

## Wordmark: chromatic glitch

The "FATALITY" wordmark is drawn **three times**: once in pure red offset by (cos t, sin t), once in pure blue offset by the opposite, and once in `text` on top. `t` loops from 0 to 2π every 0.64 s, so a 1 px red/blue fringe circles the letters continuously. The About popup uses the same effect. A cheap, distinctive touch; see also [Screen effects](../visuals/screen-effects.md) for chromatic aberration at full-screen scale.

## Controls

All controls sit on `bg_bottom`, the same colour as the window body, so they read as **holes cut through the group box**. Corner radius 2 on controls, 3–4 on boxes and popups.

- **Group box**: `bg_block` fill, radius 4, 1 px `outline` border. The **title sits on the top border**: a 2 px strip of `bg_bottom` breaks the border behind the text, and the uppercase title is centred on the line, 8 px from the left corner. (Same idea as gamesense's broken border.)
- **Header tab**: 16 px icon + label. Inactive `text_mid`, hover `text`, selected `text` in bold. The 2 px accent underline **grows from the left** to the label's width (0.15 s, ease-out); colour fades in 0.15 s. A tab with something new gets a small accent dot (2.5 px, in a 4 px `bg_block` ring) on its icon.
- **Sub-tab** (child tab): text only, 28 px tall. Horizontal: a 2 px accent underline grows left to right. Vertical (section list): a 2 px accent bar at the left edge **grows downward** to full height. Both 0.15 s ease-out.
- **Checkbox**: a small `bg_bottom` square, radius 2. Checked: a 2 px accent checkmark (two strokes). **Hovering an unchecked box fades in a faint checkmark in `outline` colour** (0.1 s), previewing the click. Radio variant: a filled accent square inset 4 px.
- **Slider**: a `bg_bottom` bar the height of a row field; the fill is the accent, inset 2 px, radius 2. The value is centred in the bar and drawn **twice**: once in `text_dark`, then again in `text` clipped to the filled part, so the number **changes colour exactly where the fill passes under it**. Arrow keys step it; Ctrl + mouse wheel steps it; no thumb.
- **Combo**: `bg_bottom` field, text 5 px from the left, an accent chevron icon at the right (points up while open). Long text **fades out** into the field colour just before the chevron (a horizontal gradient) instead of being cut. Multi-select lists names joined by commas, "None" when empty.
- **Combo list / list box**: `bg_bottom`, items 16 px tall in `text_mid`; hover `text_light`; selected items turn the **accent** (colour tween 0.15 s); highlighted items go bold. A loading state dims the list to 75 % and spins an accent loader icon.
- **Text input**: `bg_bottom` field; placeholder in `text_dark`; selection highlight in the accent at 50 %; a 1 px accent caret that fades in and out (0.5 s ease-in-out).
- **Button**: `bg_bottom` fill, centred text in `text_dark`, brightening to `text` on hover (0.15 s). Icon buttons (footer) have no fill, only the tinted icon.
- **Toggle button** (icon toggle): icon in `text_dark`, `text` on hover, accent when on.
- **Colour swatch**: a plain rounded rectangle of the colour (radius 2).
- **Colour picker popup**: `bg_block` panel, radius 4, 1 px outline, 12 px shadow. Saturation/value square (white→hue gradient overlaid with transparent→black), a vertical **hue bar** and a horizontal **alpha bar**, each with a 1 px outline and corners masked to radius 4. Grips: a 3 px ring on the square, small `text_light` bars with shadows on the hue and alpha strips. Below: copy and paste icon buttons and a hex text field.
- **Scrollbar**: 3 px grab in a 3 px `bg_bottom` track at the right edge; grab is `bg_block_light`, accent while hovered or dragged. Scrolling eases (0.25 s ease-out).
- **"New" marker**: a label can carry a 2 px accent dot before its text.

## Popups, tooltips, keybinds

- Every popup fades in and out (0.15 s) as a whole layer, and draws on the popup layers above the menu.
- **Popup look**: `bg_block`, radius 3–4, 1 px `outline`, 8–12 px shadow. Titled popups (About, hotkey overview) add a `bg_block_light` title bar, a 1 px `outline` line under it, and the **title in the accent**. Dialog boxes are the inverse: `bg_bottom` body, `bg_block` title bar, and an **accent line** under the title.
- **Tooltip**: at the cursor (+16 px), `bg_bottom`, radius 4, 1 px `outline`, 4 px shadow, 8 px horizontal padding.
- **Keybinds**: **right-click any bindable control** to open its hotkey popup under it. The popup lists the control's binds, each a row of: key-capture box, mode combo (Toggle / Hold, 70 × 20), and the value to apply (a checkbox, slider or combo matching the control), then an "Add hotkey" button. A bind can set any value, not just on/off. See [Keybinds](../widgets/keybinds.md).
- **Key-capture box**: `bg_bottom` field, radius 4, with a raised **key cap** inside (`bg_block_light` with a soft vertical gradient on each half), showing the key as an icon or short text; while waiting for a key, a 1 px accent border pulses.
- **Hotkey overview** popup: one row per bind: name, then "Hold"/"Toggle" in `text_dark`, then the key in a small `bg_bottom` cap at the right. Long names fade out before the mode text.
- **Search**: a footer button opens a search popup (text field + results list). Choosing a result **navigates to the control**: switches tab and sub-tab, scrolls its box, then draws a **2 px accent ring** around the row that fades out over 2 s. See [Settings search](../widgets/settings-search.md).
- **Custom cursor**: a 32 px cursor icon tinted with the accent.

## Startup animation

Played once when the menu first loads:

1. The screen behind dims to 75 % black and blurs (0.25 s).
2. A large accent "F" glyph appears at the centre, shrinking from 2× to 1× while fading in (0.5 s, ease-out).
3. While loading, the "F" **breathes**: its opacity eases between 100 % and 60 % in 2 s swings.
4. When ready, the "F" slides left (0.5 s ease-out) and the rest of the word "ATALITY" slides up into place beside it while fading in (0.75 s ease-out).
5. The whole overlay fades out over 1 s.

## Blur

Translucent overlays (toasts, the startup screen) sample a **quarter-resolution copy** of the back buffer through a small 5 × 5 Gaussian, run once (half quality) or twice (full quality), with a setting for none / half / full. The menu itself is opaque; blur is only used outside it.

## Motion summary

| What | Duration | Easing |
|---|---|---|
| Tab label colour, underline | 0.15 s | ease-out (underline) |
| Sub-tab bar grow | 0.15 s | ease-out |
| Checkbox check / hover preview | 0.1 s | linear |
| List item colour | 0.15 s | linear |
| Button / toggle colour | 0.15 s | linear |
| Popup fade | 0.15 s | linear |
| Scroll | 0.25 s | ease-out |
| Caret blink | 0.5 s | ease-in-out |
| Search focus ring fade | 2 s | ease-in |
| Wordmark glitch loop | 0.64 s | linear |
| Toast slide / fade | 0.25 s | ease-in-out |

Everything is short and mostly linear: the menu feels quick, never bouncy.

## What makes it work

- **Two background shades and one accent** carry the whole look. Controls reuse the body shade, so the contrast between surfaces and controls costs no extra colour.
- **Shadow strips under the header and above the footer** give depth without a single gradient on the panels.
- **Details that reward attention**: the slider number changing colour under the fill, the hover-preview checkmark, the long combo text fading before the chevron, the circling red/blue fringe on the wordmark.
- **Bold plus bright for "selected"**, not colour alone.

## What to avoid / weaknesses

- `text_mid` (`#6d6496` on `#191534`) is fairly low contrast for small text; raise it a little for readability.
- The slider has no thumb, so it's hard to tell it apart from a value field at low values (as in the screenshots).
- The hard-coded red/blue glitch colours ignore the accent; on a non-pink theme they may clash.

## Implementation notes

For Dear ImGui: map `bg_bottom` → `WindowBg` and `FrameBg`, `bg_block` → `ChildBg` and `PopupBg`, `outline` → `Border`. The [Style baseline](../implementation/imgui-style-baseline.md) does **not** use these values yet: its defaults were sampled from the [misc tab](fatality-misc-tab.md) screenshot (accent `#ec035c`, lighter window with slightly darker group boxes), so set the roles above by hand to match this look. The two-pass slider text is two `AddText` calls with a `PushClipRect` around the fill for the second. The header and footer shadows are vertical gradient rectangles (`AddRectFilledMultiColor`) from 50 % black to transparent. The checkmark is two thick `AddLine`s. See [Custom widgets](../implementation/custom-widgets.md), [Animation helpers](../implementation/animation-helpers.md) and [Draw-list recipes](../implementation/drawlist-recipes.md).

Related: [Fatality — first tab, grey and indigo](fatality-rage-tab.md), [Fatality — misc tab](fatality-misc-tab.md), [Fatality — in-game visuals](../visuals/fatality-visuals.md), [Motion](../principles/motion.md), [Color and contrast](../principles/color-and-contrast.md), [Keybinds](../widgets/keybinds.md)
