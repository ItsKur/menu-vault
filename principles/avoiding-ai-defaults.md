---
title: Avoiding AI defaults
date: 2026-11-13
tags: [principles, audit, anti-slop, agents]
source: https://github.com/Vanszs/Anti-AI-UI (Anti-AI-SLop skill, v2.0.0, MIT)
---

# Avoiding AI defaults

An LLM that is not told otherwise fills every open decision with the most common choice in its training data. For web UI that means an indigo accent, Inter everywhere, three identical cards and hype copy. The [Anti-AI-UI](https://github.com/Vanszs/Anti-AI-UI) repo catalogues those tells for websites. This note keeps the rules that carry over to game menus and HUDs, rewrites them for Dear ImGui, and says where the vault deliberately overrules them.

The core rule is the repo's, and it applies here unchanged: **every color, size, gap, radius and animation must have a reason you can state.** If you cannot say why a value was chosen, it is a default; replace it with one from a reference or principle note.

> If you could swap the wordmark for another product's and nobody would notice, the menu has no identity.

## The menu equivalent of "AI slop"

For ImGui the statistical mean is not a SaaS landing page; it is **stock ImGui with a coat of paint**. Three or more of these together read as unstyled or generated at a glance:

- ProggyClean, or any font loaded at one size and scaled.
- ImGui's default blue (`#4296fa` family) left on any `ImGuiCol_*`, next to a custom accent.
- Pure gray or pure black surfaces with no hue tint.
- Every group box the same size, same radius, same header, laid out as a symmetric grid.
- One rounding value on everything (`FrameRounding = WindowRounding = PopupRounding`).
- An indigo or violet accent (`#6366f1`, `#8b5cf6`, `#a855f7`) or a purple-to-pink gradient that nobody asked for.
- Glow on every element, or gradients on text, fills, borders and background at once.
- Emoji as icons; icons from two different sets; each icon a different color.
- Instant state changes everywhere, or the opposite: every widget bouncing, pulsing or animating at 400 ms+.
- Labels like "Advanced Settings", "Powerful Features", "Enable Feature"; "Lorem ipsum" or `TODO` left in a preview.

## Rules, translated for ImGui

### Color
- **Pick the accent on purpose** and say where it came from (a reference, the game's art, the owner's request). Do not reach for indigo/violet as a "safe" dark-UI accent. Indigo is fine when it is the brief: the owner's [Fatality](../references/fatality-rage-tab.md) indigo scheme uses it as a **base** under a pink accent.
- **One accent at ~5% of pixels**, flat surfaces with value steps, and one gradient per surface at most. This is already [Color and contrast](color-and-contrast.md); the repo's 60-30-10 rule is the same idea.
- **Override every `ImGuiCol_*` that can show color.** A leftover default blue is the menu equivalent of Tailwind's indigo button. `menu::CoverStockBlues` in the [Style baseline](../implementation/imgui-style-baseline.md) covers the ones a style usually forgets (title bars, separator drag, resize grips, built-in tabs, text links, the nav cursor).
- **Contrast**: aim for 4.5:1 for labels and values and 3:1 for headers, icons and control outlines (WCAG AA). Dim tiers for disabled or inactive items are allowed below that, because "hard to read" is their job, but nothing the user must read should sit there. Measured on the vault's palettes:

| Pair | Ratio | Verdict |
|---|---|---|
| Fatality text `#f5f5f5` on box `#191534` | 16.1 | fine |
| Fatality misc label `#8f8ea0` on `#191635` | 5.4 | fine for labels |
| Fatality secondary `#6d6496` on box `#191534` | 3.3 | inactive tabs only; too dim for labels |
| Fatality misc inactive `#4c486d` on `#191635` | 2.0 | disabled only |
| Fatality accent `#eb055a` on body `#110f23` | 4.2 | good for marks and fills; small accent text is borderline |
| Recurrence accent `#4b7dfe` on black | 5.7 | fine |

### Type
- The repo bans Inter-everywhere. **The vault overrules this**: Inter is the owner's choice ([Typography and icons](typography-and-icons.md)). What keeps it from reading as default is the rest of the rule: a tier system (wordmark, tab, header, label, meta), at most three weights, each loaded as its own `ImFont` at its render size.
- No heavy-and-tracked styling on every heading. Reserve uppercase tracking for one tier (section headers *or* tabs) plus the wordmark.
- No gradient text except a wordmark, and only one.
- Monospace only for data: hit logs, numbers that must not jitter, key names. Never as "tech" decoration on labels.

### Layout and group boxes
- **Do not lay out identical group boxes in a symmetric grid.** Size each box to its content; let one column be taller or one box span two. The repo's "one dominant element per section" maps to one primary group per tab.
- **A radius scale, not one value**: for example inset controls 3–4 px, cards 6 px, popups 6–8 px, window 8–12 px, and full pills only where the shape means something (toggle tracks, status capsules).
- **Internal spacing ≤ external spacing**: row padding inside a box is smaller than the gap between boxes, which is smaller than the window margin. One `ItemSpacing` used for all three is the menu version of "one `gap-6` for everything". See [Layout and density](layout-and-density.md).

### Widgets
- **Every widget has all its states**: idle, hover, active, disabled and (where it applies) invalid. A keybind that collides with another, a text field that rejects input, a config name that already exists: each needs a visible error state, not a silent no-op. This is the repo's "forms with no validation states" rule.
- **Toggles are correct here.** The repo warns against toggle overuse because on the web a switch implies instant apply. In a game menu settings do apply instantly, so the [toggle](../widgets/toggles-and-checkboxes.md) is the right control.
- **Do not scale a widget on hover.** It moves click targets, as [Motion](motion.md) already says. Change color or fill instead.
- Shimmer means "loading". Use it for a config or model being fetched, never as decoration.

### Icons
- One icon font, one stroke weight, tinted the label's color. No emoji. No icon inside a gradient square.
- No glow behind icons, except on the HUD where a halo is about legibility over the scene, not emphasis.
- Every colored dot means a documented state. If you cannot name the state, remove the dot.
- No letter-in-a-rounded-square placeholder logo. Use a wordmark set in the brand tier, or nothing.

### Copy
- Labels name the thing and its unit: "Field of view", "Fade speed (px/s)". Avoid adjectives that carry no information ("Advanced", "Smart", "Powerful", "Enhanced").
- Tooltips say what changes and by how much, in one short sentence. No hype verbs, no exclamation marks.
- Placeholder labels follow the vault's scope rule (General, Display, Option A); they must be *obviously* placeholders, never invented features.
- Before rendering a preview, search the draw code for `lorem`, `TODO`, `placeholder` and `xxx`.

### Motion
- Timing and easing are [Motion](motion.md): 80–150 ms hovers and toggles, 150–250 ms tabs and popups, close faster than open, ease-out. The repo's numbers agree.
- **Reduced motion.** ImGui cannot read the OS `prefers-reduced-motion` setting, so make it a setting: one "Reduce motion" toggle that sets every lerp speed to instant and removes slides, staggers and springs (keep plain fades of ≤100 ms, since a hard cut can be harsher than a short fade). The repo treats missing reduced-motion support as a blocking accessibility failure; for a menu that ships animation, treat it the same way. The vault's menus get it from `menu::ReduceMotion()` in [Animation helpers](../implementation/animation-helpers.md).
- Nothing loops forever except a loading indicator. One deliberate ambient effect per menu at most (Fatality's wordmark glitch is one), and it must stop under reduced motion. On the HUD this is already a rule ([HUD principles](../visuals/hud-principles.md)).

## Where the vault overrules the repo

These are tells on a generic website and **deliberate, owner-approved choices** here. Do not "fix" them during an audit.

| Repo rule | Vault position | Why |
|---|---|---|
| Left-border accent on cards is a P0 tell | **Allowed when the bar shows state.** | The owner's favourite element is the neverlose indicator card with a fading 2–3 px accent bar in the state color ([Neverlose Recurrence — HUD](../visuals/neverlose-recurrence-hud.md)). The repo itself allows left borders for semantic status; that is exactly this use. A purely decorative accent bar on every card is still banned. |
| No Inter everywhere | Inter is the default font. | Owner's choice ([Typography and icons](typography-and-icons.md)). |
| No glassmorphism | Glass is a core technique. | Owner's direction ([Depth, glass and glow](depth-glass-and-glow.md)). The repo's conditions still apply: glass needs a real blur behind it and enough opacity for text contrast; without blur, make surfaces near-opaque. |
| No neon glow | Outer glow is optional, HUD halos are standard. | Glow is fine as a single tinted edge on the window or as a dark legibility halo on the HUD. Glow on every control is still banned. |
| No uppercase tracked eyebrows | Uppercase section headers are normal. | Game menus use them as a structural tier ([Fatality](../references/fatality-rage-tab.md)). Limit it to one tier. |
| No toggle overuse | Toggles are the main boolean control. | Settings apply instantly in a game menu. |

## Not adopted

The rest of the repo targets websites and is left out: hero, pricing, testimonial, navbar and footer rules; Tailwind and React snippets; the `ui-ux-pro-max` style and palette database, whose industry palettes (SaaS, healthcare, e-commerce) do not map to game menus; and the code-level a11y list (ARIA, semantic HTML), which has no ImGui counterpart. Read the source if you are building a web page for a menu (a landing page or config sharing site).

## Pre-delivery audit

Run this after designing or coding a menu or HUD from the vault, before calling it done. It goes alongside the visual check with [Preview rendering](../implementation/preview-rendering.md).

**Color and type**
- [ ] Accent has a stated source; no unrequested indigo/violet; no ImGui default blue left in any `ImGuiCol_*`.
- [ ] Surfaces are hue-tinted value steps; controls visibly inset or raised against their card.
- [ ] Labels and values ≥ 4.5:1, headers and icons ≥ 3:1; only disabled/inactive text below.
- [ ] Font loaded per size and weight; ≤3 weights; uppercase tracking on one tier at most.

**Layout and widgets**
- [ ] Group boxes sized to content, not a uniform grid; one primary group per tab.
- [ ] Radius follows a scale; spacing grows from inside a row to between boxes to the window margin.
- [ ] Every widget has hover, active, disabled and (if relevant) error states.
- [ ] One icon set, one weight, no emoji; every colored dot maps to a state.

**Copy**
- [ ] Labels name the setting and unit; no empty adjectives; no hype in tooltips.
- [ ] No `lorem` / `TODO` / `placeholder` left in the preview.

**Motion**
- [ ] Speeds match [Motion](motion.md); close faster than open; no scale-on-hover.
- [ ] A "Reduce motion" setting exists if the menu animates, and it stops every loop.

**Gestalt**
- [ ] Swap the wordmark for another product's: would anyone notice? If not, push the reference's distinctive traits harder (its palette, its signature element) instead of averaging several references together.

Related: [Principles](INDEX.md), [Color and contrast](color-and-contrast.md), [Typography and icons](typography-and-icons.md), [Motion](motion.md), [Layout and density](layout-and-density.md), [Depth, glass and glow](depth-glass-and-glow.md), [HUD principles](../visuals/hud-principles.md)
