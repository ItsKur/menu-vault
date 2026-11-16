---
title: Preview rendering
date: 2026-11-06
tags: [implementation, tooling, agents]
---

# Preview rendering

`tools/preview/render_preview.cpp` runs a menu headlessly with Dear ImGui's null backend and rasterizes the draw data on the CPU (3×3 supersampling) to `preview.ppm`. An agent can render a design, look at the image, and iterate without a GPU or window. The image in the [Implementation index](INDEX.md) came from it.

```sh
# from tools/preview/, with the vault's code and a Dear ImGui checkout
I=path/to/imgui
g++ -std=c++17 -O2 -I$I -I../../implementation/code render_preview.cpp \
    $I/imgui.cpp $I/imgui_draw.cpp $I/imgui_widgets.cpp $I/imgui_tables.cpp -o render_preview
./render_preview ../../assets/fonts/inter/Inter-Regular.ttf ../../assets/fonts/inter/Inter-Bold.ttf   # fonts optional
python -c "from PIL import Image; Image.open('preview.ppm').save('preview.png')"
```

- It draws `menu::DrawDemoMenu()`; swap in your own draw function.
- It runs 90 frames with no input so animations settle; inject `io.AddMousePosEvent` / `AddMouseButtonEvent` to capture hover or pressed states.
- Preview only: no texture filtering, no blur, and user callbacks are skipped. Fonts render correctly (both RGBA32 and Alpha8 atlases).

Related: [Custom widgets](custom-widgets.md), [Building the code](code/BUILD.md)
