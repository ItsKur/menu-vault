---
title: Depth, glass and glow
date: 2026-11-06
tags: [principles, depth, blur, shadow]
---

# Depth, glass and glow

Stock Dear ImGui draws flat rectangles. Premium-looking menus add depth in three ways, each with a cost.

## 1. Soft shadow (cheap, always worth it)

A wide, low-alpha, slightly tinted shadow around the main window and popups. Built from **layered rounded rectangles** with stepped alpha, drawn on the background draw list before the window. Separates the menu from whatever is behind it.

- Spread 12–24 px, total alpha 0.25–0.5.
- Popups and dropdowns get a smaller, tighter shadow than the main window.

## 2. Background blur / glass (expensive, high impact)

The region behind a window or popup is Gaussian-blurred, then a semi-transparent surface is drawn on top. This needs the renderer: capture the backbuffer to a texture, downsample, run separable blur passes, then draw the result clipped to the window's rounded rect (`AddImageRounded`). Not possible with stock ImGui drawing alone.

- Use for popups and secondary menus first; that's where it reads best.
- Pair with surface alpha 0.75–0.9; lower and text contrast suffers. The owner's preference is the top of that range (about 0.85–0.92; see [below](#the-owners-direction-texture-2026-11-06)).
- [velocity.cat](../references/velocity-cat-menu.md) goes lower: window at 0.69, cards at 0.32 on top, popups at 0.67, each over its own blur. It reads as clearly frosted, and only works because the blur removes detail. Without blur, popups at that alpha let the rows behind them show through, so make them near-opaque.
- The blur itself can animate: velocity.cat fades the blur in along with the window's scale-in.

## 3. Outer glow (situational)

A thin accent-colored glow around the window. The Fatality thumbnail shows a pink glow, but it may have been added for the video thumbnail rather than being part of the menu; treat it as optional. Same technique as the shadow, but accent-colored and tighter (4–10 px).

## Inner structure

Inside the window, depth comes from **value steps, not shadows**: group boxes are a slightly different value with a 1 px border; controls are inset (darker). Avoid stacking shadows inside the window.

## The owner's direction: texture (2026-11-06)

After trying a first full mockup menu (since removed), the owner found its surfaces **too flat**: the layout was fine, but the surfaces needed texture. They want two things combined:

1. **Neverlose's tasteful transparency and shading.** Surfaces are layered and slightly see-through: the window over the scene, then the sidebar, cards and popups over the window (the [color picker capture](../references/neverlose-popups.md#color-picker) shows the scene tinting a popup). Each surface has gentle vertical shading, a little lighter at the top. Keep it subtle: about 85–92% opacity for the window, and overlays of a few percent of white or black for the layers on top.
2. **Fatality's contrast between the background and the controls.** Controls must not sit at the same value as the card they're on. Fields, tracks and off-switches read as **inset** (darker than the card, with a faint inner shadow along the top). Buttons and the selected nav item read as **raised** (lighter, with a lit top edge). See [Color and contrast](color-and-contrast.md#surfaces-vs-controls).

How to draw it in ImGui:

- **Vertical gradients on rounded shapes**: draw the shape with `AddRectFilled` in white, then walk its new vertices and set each color by its y position, multiplying in the vertex's original alpha so the anti-aliased fringe survives. `AddRectFilledMultiColor` can't round corners.
- **Lit top edge**: a 1 px line of white at about 10% just inside the top edge, from corner to corner. It's the cheapest single trick that makes a panel look like glass.
- **Inset**: a darker gradient (black about 40% at the top, 28% at the bottom), a 1 px white edge at about 5%, and a black line at about 30% one pixel below the top edge.
- **Translucency needs something behind it.** Over a flat backdrop, transparency is invisible. A design sandbox should draw a soft, low-detail scene stand-in (large blurred color blobs) behind the menu.

## Seen in

- [neverlose-ui](../references/neverlose-ui.md): D3D11 Gaussian blur, layered-rect shadows, glass combos.
- [Fatality — misc tab](../references/fatality-misc-tab.md): outer glow (possibly thumbnail-only), flat interior.
- [velocity.cat — main menu](../references/velocity-cat-menu.md): low-alpha glass window, cards and popups, each blurred; no shadows or borders.
- [Owner HUD captures](../visuals/owner-hud-captures.md): dark translucent HUD pills with a soft halo instead of blur.
- [neverwin — visuals](../visuals/neverwin-visuals.md): shadow, glow, glass and fill all from one SDF rounded-rect shader with squircle corners; glow is a shared 25-tap Gaussian post-pass.
- [Fatality — first tab](../references/fatality-rage-tab.md): the owner's reference for an opaque menu; depth from three surface levels only.
- [Neverlose — themes](../references/neverlose-themes.md): light theme with a frosted translucent sidebar over a near-opaque content area.
- [Fatality CS2 — source](../references/fatality-cs2-source.md): opaque menu with 12 px shadow strips under the header and above the footer; blur (quarter-res, 5 × 5 Gaussian, none/half/full) only for toasts and the startup screen.
- [Neverlose Recurrence — menu](../references/neverlose-recurrence-menu.md): three stacked glass sheets (window 90 %, panel 50 %, card 30 %) that all share one 1 px white 5 % inside edge; 20 px backdrop blur on the window.
- [Neverlose Recurrence — HUD](../visuals/neverlose-recurrence-hud.md): every HUD element is black 70 % + 5 px backdrop blur + an unoffset black halo shadow; no strokes.
- [NL loader — dashboard and modal](../references/nl-loader-dashboard.md): shader blur on the cards past the fold and behind the modal.
- [imgui-menu-clone](../references/imgui-menu-clone.md): why compositor blur behind a window fails unless the frame is cleared to alpha 0.

Related: [Draw-list recipes](../implementation/drawlist-recipes.md), [Color and contrast](color-and-contrast.md), [Combos and dropdowns](../widgets/combos-and-dropdowns.md)
