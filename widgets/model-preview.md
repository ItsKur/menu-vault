---
title: Model preview
date: 2026-11-06
tags: [widget, preview, model, skeleton, bones]
---

# Model preview

A live picture of a character model beside the settings that change how characters are drawn. **The owner wants one in every visuals section about character models, ideally with bone highlighting.**

## Anatomy (neverlose)

- Takes the right half of the content area, with no card, so the model has room.
- The model stands centered, front-on or three-quarter, idle.
- A row of controls sits at the top right: a chip for the active group (accent icon and text on a faint accent-tinted pill), plus icon buttons for another group, a list, and expand.
- Settings on the left (colors, styles) apply to the preview immediately.

See [Neverlose — visuals page](../references/neverlose-visuals-page.md).

## Bone highlighting (owner's wish)

### The capture: points, not lines

[Neverlose — five colour themes](../references/neverlose-themes.md) (navy theme) shows a version of this: the preview sits in **its own floating window** beside the menu, with a group combo and a title at the top and a "?" help icon. Every selectable joint (head, neck, chest, stomach, pelvis, shoulders, elbows, hands, knees, feet) has a small **white "+" cross**; **selected joints** become filled accent circles (about 10 px) with the white cross inside and a soft translucent accent halo. **No bone lines** are drawn. It reads cleanly on a detailed model because the markers are tiny until selected. Missing: labels on hover.

### What it should do

The owner's wish, beyond the capture (which has joint points and a highlighted selection, but no bone lines, hover names or part grouping):

- Draw the **skeleton** over the model: bone lines between joints, small dots at the joints.
- **Highlighted bones** are drawn in the accent, thicker, with a soft glow, so the selected parts read at a glance.
- Group the bones into parts the user thinks in (head, torso, arms, legs) rather than dozens of individual bones.
- **Hovering** a bone highlights its part and names it; **clicking** toggles it. The same selection should also be exposed as normal rows (one switch per part, or a multi-select combo) for keyboard users and for precision.
- Keep the skeleton readable over both light and dark model colors: a dark under-stroke, or a light stroke at partial alpha.

Open choice for the owner: points only (as captured) or points plus bone lines.

## Rules

- The preview reacts **live**: no Apply button.
- Give the model a ground: a soft ellipse shadow under the feet, or a faint radial vignette, so it doesn't float.
- Gentle idle motion (breathing, slow sway) makes it read as live. Stop it on hover so bones are easy to hit.
- Label the icon buttons with tooltips.
- Let expand give the preview the whole content area, and restore with the same button.

## ImGui approach

There is no 3D model in a standalone mock-up, so draw a placeholder mannequin: capsules (a thick line plus a circle at each end) for the limbs, a torso polygon and a head circle, over a 2D skeleton of named joints (about 17). Use opaque fills, since the silhouette is many overlapping shapes and translucent fills darken every overlap. Hit-test each bone by its distance from the mouse to the segment (about 8 px), and draw highlighted parts as layered translucent strokes for the glow. Pause any idle motion while the preview is hovered, so the bones hold still for clicking. A real product would render the model to a texture and draw the bones from projected joint positions; the overlay is drawn the same way. (A first mockup built this way was shown to the owner on 2026-11-06 and then removed.)

## Seen in

- [Neverlose — visuals page](../references/neverlose-visuals-page.md): preview without bone highlighting.
- [Neverlose — themes](../references/neverlose-themes.md): navy theme, **bone points**: a floating preview window with white "+" markers at the joints and selected points as accent circles with a halo.

Related: [Toggles and checkboxes](toggles-and-checkboxes.md), [Color pickers](color-pickers.md), [Layout and density](../principles/layout-and-density.md), [Depth, glass and glow](../principles/depth-glass-and-glow.md)
