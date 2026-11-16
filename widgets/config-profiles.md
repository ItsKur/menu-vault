---
title: Config profiles — saving, sharing and reset surfaces
date: 2026-11-16
tags: [widget, config, profile, save, clipboard]
---

# Config profiles — saving, sharing and reset surfaces

## Three inspected patterns

| Pattern | Evidence | When useful |
|---|---|---|
| Compact preset popup | [Neverlose preset manager](../references/neverlose-preset-manager.md) | Current profile stays reachable without a dedicated page |
| List + name field + action stack | [gamesense preset page](../references/gamesense-config-and-key-fields.md) | Frequent explicit load/save and clipboard work |
| Metadata + scoped reset + clipboard pair | [NXC configuration](../references/fatality-nxc-config-and-hud-settings.md) | Sharing an addon setup and recovering misplaced HUD panels |

These are **visual/interaction references**. No storage backend was exercised in this vault.

## Anatomy for a new design

The following are design recommendations, not claims about any referenced product:

- **Identity:** show the selected profile name; do not make selection and renaming look identical.
- **Dirty state:** explicitly distinguish saved, modified and saving. Never use the same icon for “selected” and “saved successfully.”
- **Routine actions:** keep Load and Save close to the selected profile; choose explicit-save or autosave semantics and explain the choice.
- **Sharing:** make clipboard import/export distinct and report invalid input without discarding the current setup.
- **Recovery:** separate Reset Positions from Reset Settings. A misplaced HUD should not require deleting the user's whole configuration.
- **Destruction:** separate Delete from Save; provide confirmation or undo.
- **Feedback:** acknowledge successful persistence only after it succeeds, not merely after a button click.

## Neutral examples

Profile names: General, Display, Audio. Status text: “Unsaved changes”, “Saved”, “Import failed: unsupported version”. These are suggested labels, not transcriptions from closed-source products.

## Seen in

- [Neverlose — presets](../references/neverlose-preset-manager.md): toolbar, search, row overflow, inline rename, share flow.
- [gamesense — presets](../references/gamesense-config-and-key-fields.md): list, editable name and explicit action stack.
- [Fatality community — NXC](../references/fatality-nxc-config-and-hud-settings.md): independent layout/settings reset and clipboard actions.

Related: [Row settings popup](row-settings-popup.md), [Tooltips](tooltips.md), [Keybinds](keybinds.md), [Media sweep](../visuals/reference-media-sweep.md)
