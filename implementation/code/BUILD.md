---
title: Building the code recipes
date: 2026-11-06
tags: [implementation, build]
---

# Building the code recipes

The headers here are the tested source for the [Implementation](../INDEX.md) notes. They were compiled with `-Wall -Wextra` and run headless for 60 frames with simulated input against Dear ImGui 1.93.1 WIP (November 2026).

```sh
git clone --depth 1 https://github.com/ocornut/imgui.git
I=imgui
g++ -std=c++17 -Wall -Wextra -I$I -I. headless_test.cpp \
    $I/imgui.cpp $I/imgui_draw.cpp $I/imgui_widgets.cpp $I/imgui_tables.cpp $I/imgui_demo.cpp -o headless_test
./headless_test   # prints OK
```

The test runs `demo_menu.h` and `glass_sidebar_menu.h` (clicking a tab, a sub-tab, a row popup and the search button in the second), then `hud_elements.h` for 120 frames, so toasts spawn and a keybind row leaves and returns, then `gamesense_menu.h` for 120 frames (a rail tab, a checkbox, a combo and its list, a slider, a swatch), then `popup_menus.h` for 260 frames (right-click hotkey menu, New Hotkey, hover submenu, key capture, mode and value, a docked tooltip), then `fatality_visuals.h` for 90 frames (its in-world demo scene), then `menu_widgets.h`'s `ComboRow` and `ColorRow` for 70 frames (open the list and pick the third entry, open the picker, drag in its SV square); that last step returns 1 and prints `FAIL` if the pick, the open picker or the color change is missing.

To use in a real window, start from any Dear ImGui example (e.g. `examples/example_win32_directx11` or `example_glfw_opengl3`), include `demo_menu.h`, call `menu::ApplyStyle()` once after creating the context, and `menu::DrawDemoMenu()` each frame. For the velocity.cat-style menu, include `glass_sidebar_menu.h` and use `glass::ApplyStyle()` / `glass::DrawGlassMenu()`. The other headers (gamesense, popups, HUD, Fatality visuals) each have a usage block in their note.

If you edit a header, recompile the test, then update the matching note so the notes and files stay identical.

Related: [Preview rendering](../preview-rendering.md)
