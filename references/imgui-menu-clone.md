---
title: imgui-menu-clone — screenshot-to-menu workflow
date: 2026-11-13
tags: [reference, repo, imgui, d3d11, workflow, agents]
source: https://github.com/SharimAli/imgui-menu-clone
---

# imgui-menu-clone — screenshot-to-menu workflow

## Summary

An agent "skill" repo (MIT) for rebuilding a Dear ImGui menu from one screenshot. It ships a Win32 + D3D11 Visual Studio template with a small widget set, Python helpers for reading pixels, and checklists of the details clones usually get wrong. It has no single look of its own; it is in the vault because its **method and pitfalls** fit the screenshot-to-software workflow: sample, write, render, compare. It is UI only.

- Repo: https://github.com/SharimAli/imgui-menu-clone (7 stars, last push 2026-10)
- Template pins Dear ImGui v1.91.9b (modern draw flags, before the 1.92 font rework).

## The pipeline (from `SKILL.md` / README)

1. **Bootstrap** a clean Win32 + D3D11 + ImGui project.
2. **Analyse**: `scripts/extract_palette.py` probes exact RGB at given points; pixel bounds are measured into a `spec.md`.
3. **Checkpoint**: the agent stops for the owner to confirm `spec.md` before writing code.
4. **Generate**: a theme table (`ui/theme.h`), the layout, then custom widgets.
5. **Compare**: build, capture the window, and diff it against the reference **at 4× zoom** (`scripts/crop_zoom.py` stacks the same crop of both images).

The vault's equivalent tools are `tools/sample_colors.py` and the headless renderer in [Preview rendering](../implementation/preview-rendering.md). The 4× crop comparison is the step the vault's workflow does not yet spell out.

## Details clones get wrong (`references/detail-checklist.md`)

Checked against the vault's notes; these add to them.

- **Ring or filled knob.** A ring knob is a 1 px accent outline over a *dark* fill, and the track never takes the accent. Draw it as two filled rounded rects (outer accent, inner dark, 1 px smaller) instead of `AddRect`, so the outline stays crisp at high rounding. At 4×, a knob darker in the centre than at the edge is a ring → [Toggles and checkboxes](../widgets/toggles-and-checkboxes.md).
- **Overhang.** Knobs are often taller than their track (a 10 px track with a 19 px knob is normal). Measure both. Draw the overhang outside the item rect; do not grow the item to fit it, or the row height breaks → [Sliders](../widgets/sliders.md), [Layout and density](../principles/layout-and-density.md).
- **Track fill.** Check a mid-value slider, not one at zero, to see whether the accent fills up to the knob.

## Template values (`assets/template/ui/theme.h`)

Defaults for its "vision" theme, kept as a reference point, not a recommendation: sidebar 155 px, header 60 px, content padding 10; sidebar tab 40 px tall with a **2 px accent bar** on the selected edge, icon at x 15, label at x 45; group box rounding 5, 1.2 px border, 31 px header with a rule line at 50 % alpha, body alpha about 40/255; toggle track 58 × 15 with a 38 × 21 knob that travels 20 px; slider track 10 px with a 32 × 19 knob; combo field 35 px tall.

## Pitfalls (`references/pitfalls.md`)

- **Blur behind the window is not drawn by ImGui.** It comes from the OS compositor on the host window, and silently fails unless the window is layered, the DWM frame is extended over the client area, and the frame is cleared to **alpha 0**; any panel at alpha 255 shows no blur. In-window blur of your own content needs a shader pass instead (see [NL loader](nl-loader-dashboard.md), [neverlose-ui](neverlose-ui.md)) → [Depth, glass and glow](../principles/depth-glass-and-glow.md).
- **Version drift**: `BeginChild` flags changed in 1.90, `ImDrawCornerFlags_*` were removed in 1.91, fonts were reworked in 1.92. Code from a forked ImGui with a changed `BeginCombo` signature is its own API → [Portability](../principles/portability.md).
- **Immediate-mode state**: a function-local `static` in a widget is shared by every instance, so key animation state by `ImGuiID`. Call `ItemSize` before `ItemAdd`. Hit-test with `ButtonBehavior`, not `IsMouseHoveringRect`. Background-draw-list drawing ignores `ImGuiStyleVar_Alpha`, so a fading page leaves its background opaque → [Custom widgets](../implementation/custom-widgets.md).

## What to avoid / weaknesses

- The template theme itself is generic; use the checklists, not the look.
- The tooling is Windows-only (MSBuild, PowerShell capture). On this vault's Linux setup, use the headless preview renderer for the compare step.

Related: [Preview rendering](../implementation/preview-rendering.md), [Custom widgets](../implementation/custom-widgets.md), [Toggles and checkboxes](../widgets/toggles-and-checkboxes.md), [Sliders](../widgets/sliders.md), [Depth, glass and glow](../principles/depth-glass-and-glow.md), [Portability](../principles/portability.md), [Dear ImGui](dear-imgui.md)
