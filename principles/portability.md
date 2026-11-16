---
title: Portability across games
date: 2026-11-06
tags: [principles, portability, color, scale, rendering]
---

# Portability across games

The rest of this vault is tuned to one family: a dark, desaturated shooter, one accent, 1920×1080. Its references often assume shaders and blur, while its recipes are stock ImGui with neither, so most notes carry a fallback (collected in section 4). That family is a good default and a bad universal rule. This note is the layer that lets the same vocabulary (pills, cards, boxes, tracers, chams) work on a stylised platformer, a bright mobile game or a 4K title with an HDR swapchain.

Adapt in four passes, in this order.

## 1. Art direction: three families, not one

Pick a family before picking a colour.

- **Dark base** (the vault default). Near-black surfaces at 6–14% lightness, near-white text, one saturated accent. Assumes the scene behind is busy and mid-to-dark. Breaks on a snow map or a white sky: the panel turns into a black rectangle floating in a bright field, and the [halo](../visuals/hud-principles.md#1-readable-over-anything) has to carry all the separation.
- **Light base.** Paper-white at 92–96%, text at 12–20% lightness, the same single accent used more sparingly. Reads on bright scenes and in stylised games; needs a *shadow* rather than a glow to separate from the scene, because there is no darker-than-black left to hide in.
- **Scene-matched.** Sample the game's own UI or map palette and borrow two or three of its values, then add one accent that the game does **not** use. This is what makes a HUD look native instead of imported, and it is the only one of the three that needs material from the game: screenshots or video frames of its UI and maps are enough.

Rules that hold in all three:

- **Take the base value from the scene's median, not from taste.** If the scene is bright, the panel must be either much darker or much lighter than it, not a middle value that merges.
- **At most three text tiers.** Primary, secondary and disabled for a menu; HUD text usually needs only the first two (see [HUD principles](../visuals/hud-principles.md)). Everything else is the accent or a surface.
- **The accent must be absent from the game's own HUD**, or your element reads as a native one. Check the health bar, the ammo counter and the killfeed before committing.
- **Saturation scales with the family.** A saturated accent on near-black needs less chroma than the same accent on white; the light family wants a deeper, less luminous hue to hit the same contrast.

## 2. Scale

Design at 1920×1080 in px, then express every number as `px × s`, with `s = clamp(min(w / 1920, h / 1080), 0.75, 1.5)`.

- **Round once, at the end.** Accumulating unrounded sizes across a layout drifts by several px on a 4K screen.
- **Snap corner radii and stroke widths to integers** after scaling. A 6 px radius at `s = 1.33` is 8, not 7.98.
- **1 px hairlines only survive at `s = 1`.** Below that they shimmer or vanish; use `max(1, round(s))` and accept 2 px at 4K. Above it, a 1 px border is nearly invisible, so the border should grow with `s`, not stay at 1.
- **Font sizes do not scale linearly forever.** Past about 1.5×, step the size and increase tracking instead; large text with tight tracking looks cramped.
- **Re-check density, not just size.** A 4K screen shows the same layout at four times the pixels; the element count that felt sparse at 1080p can read as empty. The inverse is worse: on 720p or a mobile screen, drop a tier of detail (labels before icons, icons before the container) rather than shrinking everything until the text is unreadable.

## 3. Colour space and compositing

- **sRGB vs linear.** Hex values sampled from a screenshot are sRGB. If the host composites in linear and you feed those values straight in, mid-tones come out too bright and dark surfaces separate badly. Convert on the way in, and compare against a screenshot of the same scene, not against your hex table.
- **HDR / bloom.** Additive elements (glows, tracers, additive sprites) multiply with a bloom pass. The same alpha that looks right in SDR blows out in HDR: cut additive alpha by roughly a third and re-check.
- **Alpha over bright backgrounds.** A 40% black container over a white sky is still a grey box with unreadable text. Raise the container's alpha as the scene's median value rises; 40–50% is tuned for mid-dark scenes, not for daylight.
- **Tone-mapped hosts tint everything.** A menu drawn after tone mapping can pick up the scene's colour grade; a menu drawn before it gets graded too. If the palette looks wrong only in-game, suspect the order before the values.

## 4. Renderer budget

What the host can do decides which variant of each element you can ship. Degrade rather than skip.

| Wanted | If the host cannot do it |
| --- | --- |
| Backdrop blur | Opaque surface at 88–94%, plus a 1 px lighter top edge to fake the highlight |
| Per-pixel fresnel (chams) | Layered silhouettes: a soft halo, a body pass enlarged by 1–2 px in the rim colour, then the body on top (as `DrawMaterial` does; see [Chams](../visuals/chams.md)) |
| Additive glow | Two nested strokes at 20% and 8% alpha, normal blending |
| Custom rounded corners | A 1 px inset border and square corners; do not fake rounding with a texture at odd radii |
| Scissor clipping | Clip by shrinking the element; text that spills is worse than an element that ends early |
| Sub-pixel AA | Integer positions and a 1 px darker edge line; the edge line hides the stair-step |
| Many draw calls | Batch one material per element group; the budget is usually 100–300 quads, not thousands |

Two budget rules that apply everywhere:

- **Draw order is the depth system.** Backdrop, then each element's glow or halo, then its container, then its content. A glow drawn after the text sits on top of it and washes it out, so draw it underneath, as the vault's `Halo` and shared glow layer do. Full-screen effects go before the whole HUD ([Screen effects](../visuals/screen-effects.md)).
- **One glow layer per element group.** Per-element glows stack where elements overlap and turn into white blobs; see [Glow lines](../visuals/glow-lines.md).

## Checklist

Before calling a design portable:

1. Does it survive a scene of the opposite value (dark panel on snow, light panel in a tunnel)?
2. Does it still read at `s = 0.75` and `s = 1.5`, with text legible at both?
3. Are all widths, radii and gutters integers at the target scale?
4. Is the accent absent from the game's own HUD?
5. Does every effect have a fallback that needs no shader and no blur?
6. Are glows drawn under the content they belong to, with additive alpha cut for HDR?

Related: [Color and contrast](color-and-contrast.md), [Layout and density](layout-and-density.md), [Depth, glass and glow](depth-glass-and-glow.md), [HUD principles](../visuals/hud-principles.md), [Chams](../visuals/chams.md)