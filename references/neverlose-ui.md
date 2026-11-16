---
title: neverlose-ui
date: 2026-11-06
tags: [reference, repo, imgui, d3d11, glass, animated]
source: https://github.com/Inshallah-me/neverlose-ui
---

# neverlose-ui

## Summary

A standalone Win32 menu showcase (C++17, Direct3D 11, Dear ImGui) recreating the visual style of the neverlose.cc CS2 V4 menu. It states it is an independent UI recreation for learning and interface experimentation, not affiliated with neverlose.cc. It runs as its own window with no game attachment, which makes it a clean design reference.

- Repo: https://github.com/Inshallah-me/neverlose-ui
- Fork of: https://github.com/Fdiue/neverlose-last (check the original for newer commits)
- Build: open `examples/imgui_examples.sln`, select `example_win32_directx11`, x64, build. Targets MSVC toolset `v145`; retarget if yours differs.

## Techniques worth studying (from the README)

- GPU Gaussian blur via D3D11 → [Depth, glass and glow](../principles/depth-glass-and-glow.md)
- Shadows from layered, linearly blended rectangles → [Draw-list recipes](../implementation/drawlist-recipes.md)
- Adjustable DPI scaling for the whole menu
- Tuned font rasterization; Font Awesome icons → [Typography and icons](../principles/typography-and-icons.md)
- Pill toggles with linear animation → [Toggles and checkboxes](../widgets/toggles-and-checkboxes.md)
- Editable, animated sliders templated for `int` and `float` → [Sliders](../widgets/sliders.md)
- Single- and multi-select combos; glass secondary combo menus → [Combos and dropdowns](../widgets/combos-and-dropdowns.md)
- Rounded SV color field, hue and alpha bars, multi-color picker popup → [Color pickers](../widgets/color-pickers.md)
- Startup animation → [Motion](../principles/motion.md)
- Config and Profile secondary menus
- One global accent color → [Color and contrast](../principles/color-and-contrast.md)
- A `DESIGN.md` in the repo root documents its design decisions; read it before the code.

## License

Project code is **AGPL v3**; bundled Dear ImGui stays MIT. Studying it is fine; copying its widget code into something you distribute means releasing your source under AGPL. The recipes in this vault are written independently.

Related: [Fatality — misc tab](fatality-misc-tab.md), [Starter bases](starter-bases.md), [Draw-list recipes](../implementation/drawlist-recipes.md), [neverwin — visuals](../visuals/neverwin-visuals.md) (a third-party copy of this product's HUD shaders, read from a DLL), [Neverlose — HUD](../visuals/neverlose-hud.md) (its on-screen elements, from video frames)
