---
title: Chams
date: 2026-11-06
tags: [visuals, in-world, model, material, chams]
---

# Chams

Recolouring a model so its silhouette reads through the scene. The element is usually called *chams* (after "chameleon"), sometimes "model materials". The input is a mesh plus a camera; the output is a flat or rim-lit version of that mesh in one or two colours. This note is about how the result looks. Nothing here says how a product finds models or decides what to paint.

## Anatomy

Every variant below is the same four decisions: base, lighting, edge, and what happens behind geometry.

- **Base**: the model's own texture, kept and multiplied by a tint; or discarded for a solid colour. Keeping the texture preserves the shape's internal detail (straps, panels, folds); discarding it turns the model into a silhouette. Solid fills read best in motion and at distance, textured fills read better up close.
- **Lighting**: the scene's lighting is replaced, not added to. A tint multiplied by the scene's shading picks up shadows and looks like a painted object; a constant colour with no shading looks like a sticker. Most "flat" chams are constant colour with one exception: a **rim**.
- **Rim (fresnel)**: brightness as a function of how edge-on the surface is: `pow(1 - dot(normal, view), p)`. With `p` around 2–4 the outline glows and the centre stays dark, which gives a filled silhouette a readable 3D shape without any lighting. This is the single most useful trick in the element. Below `p = 2` it fogs the whole model; above `p = 6` it becomes a thin outline and you have built a contour instead.
- **Behind geometry**: a second colour, drawn with depth testing off, for the parts hidden by walls. Standard practice is the same hue at lower value and 40–60% alpha, so the scene is still visible through it. A fully opaque through-walls colour turns the map into a mess of blobs.

## Variants

Named after the material list in [Fatality — in-game visuals](fatality-visuals.md); that source ships the settings but no drawing code, so what each looks like is inferred from the name and from standard practice. Treat the descriptions as guesses, not observations.

- **Normal**: the model's texture at full brightness, tinted. The closest to "just recolour it".
- **Flat**: one solid colour, no shading, no texture. Maximum contrast, minimum shape; always pair with a rim or a contour.
- **Rim glow**: flat fill plus a fresnel rim in the accent. The most common good-looking default.
- **Contour**: an outline only: either an inverted hull (the mesh drawn slightly enlarged, back faces, solid colour) or a depth/normal edge detect. 1–2 px at 1080p.
- **Flow**: a texture or gradient scrolled along one UV axis, 2–4 px/s. Reads as energy; use low contrast or it becomes a strobe.
- **Acrylic / glass**: a translucent fill with a stronger rim. Works over dark scenes, disappears over bright ones.
- **Sprite**: an additive billboard behind the model rather than a recolour: a blob of light with the silhouette in front of it. Cheap, and it separates the target from busy backgrounds.
- **Self / delayed copy**: the same materials applied to your own model, or a ghost of it offset by ~100 ms. The second reads as motion blur and is easy to overdo.

## Rules

- **One hue per role.** Team A one hue, team B another, hidden-behind-wall a darker step of the same hue. Three unrelated colours make the scene unreadable.
- **Value contrast beats saturation.** A mid-value fill on a bright map vanishes no matter how saturated it is. Pick the fill's value from the map, not from taste.
- **Let the rim do the shape work.** A flat fill with no rim reads as a hole cut out of the screen.
- **Thin contours.** 1–2 px; anything thicker and the outline swallows the model at distance.
- **Through-walls at partial alpha.** 40–60%, and never the same value as the visible version, or you lose track of what is actually in front of you.
- **Animate nothing that is always on.** Flow and pulse are per-target or on-hit effects; a permanently scrolling fill is exhausting over a long session.

## ImGui approach

A standalone mock-up has no normals, so the rim is faked with layered silhouettes. `DrawMaterial` in [Fatality-style in-world visuals](../implementation/fatality-visuals-code.md) does this on a placeholder mannequin built from capsules:

- **Flat**: one pass of the body in the fill colour.
- **Rim glow**: a soft halo (stacked, slightly enlarged body passes at falling alpha), then a body pass enlarged by 1.5 px in the rim colour, then the body on top in a dark tint of the fill. The 1.5 px that shows around the edge is the rim.
- **Contour**: a body pass enlarged by about 1 px in the outline colour, then a near-black body on top.
- **Glow outline**: the halo, then the body as is.

`DrawThroughWall` draws the hidden colour over the whole figure, then the visible material clipped to the part above the occluder. Flow, acrylic and sprite aren't in the recipe yet. With a real mesh and shader, replace the layered passes with the fresnel term above.

## Seen in

- [Fatality — in-game visuals](fatality-visuals.md): the material list (None, Normal, Flat, Rim Glow, Contour, Flow, Acrylic, Legacy, Sprite), per-group primary/secondary/behind-wall colours and a through-walls switch, with no drawing code.

Related: [HUD principles](hud-principles.md), [Glow lines](glow-lines.md), [Portability across games](../principles/portability.md), [Depth, glass and glow](../principles/depth-glass-and-glow.md)