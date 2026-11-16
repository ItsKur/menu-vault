---
title: Dear ImGui
date: 2026-11-06
tags: [reference, library, imgui]
source: https://github.com/ocornut/imgui
---

# Dear ImGui

The immediate-mode C++ UI library most game-menu designs are built on or imitate. MIT licensed.

## Useful for design work

- `ImGui::ShowDemoWindow()` → Tools → Style Editor: live-tweak rounding, padding and every color, then export the style as code. Fastest way to prototype a palette.
- `imgui_demo.cpp`: reference usage for every built-in widget.
- `imgui_internal.h`: `ButtonBehavior`, `ItemAdd`, `ImRect`, `ImLerp`, `ImSaturate`, the pieces needed for custom widgets.
- `misc/freetype/`: FreeType font backend for crisper small text.
- Gallery threads in the repo's issues (search "Gallery"): screenshots of polished, legitimate ImGui apps; good for styling ideas outside the game-menu genre.

## Version notes

APIs shift between releases (e.g. child-window border flags, selectable popup flags, font loading in 1.92). Check the version a repo bundles before copying snippets.

Related: [Starter bases](starter-bases.md), [Style baseline](../implementation/imgui-style-baseline.md), [Custom widgets](../implementation/custom-widgets.md)
