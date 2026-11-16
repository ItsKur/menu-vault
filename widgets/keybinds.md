---
title: Keybinds
date: 2026-11-06
tags: [widget, keybind, hotkey, popup]
---

# Keybinds

How a setting gets a key: where the binding UI lives, how a key is captured, and what a binding holds.

## What a binding holds

| Field | Meaning |
|---|---|
| key | any keyboard key, mouse button (not the left button, which drives the UI) or gamepad button; or none ("Unbound") |
| mode | **Toggle** (each press flips), **Hold** (active while held), **Always** (active regardless of the key), optionally **Off** (binding kept but ignored) |
| value | what the setting becomes while the binding is active (for a switch: on or off; for a slider: a number) |
| visible | whether the binding shows in the HUD [keybind list](../visuals/keybind-list.md) |

A setting can have several bindings: for example Hold F for one value, Toggle Mouse 4 for another.

## Variants

- **A: right-click hotkey menu** (neverlose). Nothing on the row. Right-click opens a menu listing the row's hotkeys (each with a `>` chevron), then All Hotkeys, New Hotkey, a separator and Reset. Hovering a hotkey opens its submenu: Key, Mode, Value, Visible, Delete. The cleanest option and the vault's recommended default. Details: [Neverlose — popups](../references/neverlose-popups.md).
- **B: key tag on the row** (gamesense). A dim `[-]` or `[F]` at the row's right edge; clicking it starts capture, right-clicking picks the mode. Compact and always visible, but it clutters every row and has no room for value or visibility.
  - *From porting this variant to a production menu (2026-11):* show the **unbound** `[-]` tag only while the row is hovered, so pages full of switches stay clean; bound tags always show. The tag reads `[...]` while capturing; Escape clears. Right-click opens a small flush mode list in the order **Always on, Toggle, Hold, Off**, the current mode in bold accent. A new bind starts as Toggle. For numeric rows (sliders), put the tag right after the label instead of at the right edge (the slider owns the edge) and add a Value slider under the mode list, which covers variant A's "value" without its submenu.
- **C: key badge after the label** (velocity.cat). A small bordered chip after the label shows the key.
- **D: keyboard icon** (pink sidebar menu). An icon before the control marks bindable rows.

## Capture

- Click the Key field: it changes to a listening state ("Press a key…") until a key is pressed.
- **Escape cancels** and keeps the old key; **Backspace or Delete unbinds**. (A common convention; Neverlose's own handling isn't visible in the captures.)
- Ignore the left mouse button, or a click to start capture binds itself. Clicking another row in the popup ends capture.
- Show key names, not codes: "Mouse 4", "Space", "F".

## Rules

- Keep binding UI out of the main surface when most settings aren't bound (variant A). Show a small mark on rows that *are* bound so the user can find them.
- Dim menu items that don't apply (All Hotkeys with no hotkeys, New Hotkey while an Unbound entry exists) instead of removing them.
- Destructive items (Reset, Delete) in a muted red at the bottom, after a separator.
- The submenu opens on hover, aligned with the hovered row, and closes when another row of the parent menu is hovered.

## ImGui approach

`HotkeyMenu(id, binds)` in [Popup menus](../implementation/popup-menus.md), called right after any row widget: it opens on right-click of the last item. The submenu is a second popup, opened from inside the first; hovering a different parent row calls `ClosePopupToLevel` to close it. Capture iterates `ImGuiKey_NamedKey_BEGIN..END` with `IsKeyPressed`, skipping the mod-reserved keys and the left mouse button.

## Seen in

- [gamesense — config and key fields](../references/gamesense-config-and-key-fields.md): newly inspected legacy video frame showing bound and unbound bracket tags; capture/mode behavior not established by the still.

- [Neverlose — popups](../references/neverlose-popups.md): variant A.
- [gamesense — main menu](../references/gamesense-menu.md): variant B.
- [velocity.cat — main menu](../references/velocity-cat-menu.md): variant C.
- [Pink sidebar menu](../references/pink-sidebar-menu.md): variant D.
- [Neverlose — themes](../references/neverlose-themes.md): a bind popup with a combo to pick one of several binds per row, then key, mode, show-in-list and value rows; keys shown as bordered boxes on rows.
- [Fatality CS2 — source](../references/fatality-cs2-source.md): right-click any bindable control; popup lists its binds (key cap, Toggle/Hold, value to apply) plus "Add hotkey"; a hotkey overview popup lists all binds.

Related: [Toggles and checkboxes](toggles-and-checkboxes.md), [Row settings popup](row-settings-popup.md), [Keybind list](../visuals/keybind-list.md), [Tooltips](tooltips.md)
