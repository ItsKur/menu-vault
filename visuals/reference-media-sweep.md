---
title: Reference media sweep — indicators, configs and world HUD
date: 2026-11-16
tags: [index, references, media, research, hud]
---

# Reference media sweep — 2026-11-16

New to this vault does **not** mean recently published. Source dates and stock/community attribution are recorded in each case study. Selected images were inspected directly; a search result or a menu toggle was not treated as a rendered HUD example.

## Start with these

| Need | Best new reference | What is actually verified |
|---|---|---|
| Config saving / sharing UI | [Neverlose preset manager](../references/neverlose-preset-manager.md) | Official 2026-11-10 tutorial, save/profile toolbar, row menu, share flow; older close-up for inline rename |
| Explicit save/load page | [gamesense presets and key fields](../references/gamesense-config-and-key-fields.md) | Legacy list, name field, action buttons, clipboard pair.[3] |
| Keybind fields | [gamesense presets and key fields](../references/gamesense-config-and-key-fields.md) | Tiny bracketed row-end tags, bound and unbound examples |
| Keybind indicator panel | [Neverlose community indicators](neverlose-perfect-indicators.md) | Narrow header and active-state row, in a legacy script showcase.[5] |
| Watermark / information HUD | [Neverlose community indicators](neverlose-perfect-indicators.md) | Small brand/server/time strip; [2026 windows](neverlose-2026-world-and-windows.md) adds event rows and purchase-summary silhouettes |
| Other indicator geometry | [Fatality community Bilibili preview](fatality-placeholder-hud.md) | Segmented arc and tiny glyph cluster; state meanings not established |
| HUD configuration organization | [Fatality community NXC](../references/fatality-nxc-config-and-hud-settings.md) | Original PNGs of enable/style rows, theme controls, independent reset actions |
| Projectile path / endpoint | [Neverlose world/windows](neverlose-2026-world-and-windows.md) | Actual trajectory line, endpoint annotation and presentation settings |
| Numeric grenade-landing countdown | [Projectile timing](projectile-timing.md) | **Still a gap**: no newly inspected, readable numeric landing countdown |
| Smoke/fire ground boundaries | [Kizaru molly and tracers](kizaru-molly-and-tracers.md) | Follow-up now includes actual molly area frames and the owner's teal reference; smoke floor-radius capture still missing |

## Owner-directed follow-up

- [Kizaru — molly and tracers](kizaru-molly-and-tracers.md): supplied 00:17–00:21 passage, original 1080p/60fps inspection, area and tracer excerpts; exact lifetime-to-colour curve remains unmeasured.

- [unmatched rank overlays](unmatched-rank-overlays.md): two original demos, separate Neverlose and gamesense variants.

## Practical comparisons

- [Config profiles](../widgets/config-profiles.md): popup manager vs dedicated action page vs clipboard/reset page.
- [Projectile timing](projectile-timing.md): distinguish predicted arrival, active lifetime and ground footprint before choosing the visual.
- Stock layouts and community scripts are intentionally separate. A product name in a script title does not authenticate the host's native design.

## Additional inspected Fatality footage

![Fatality visual-settings/model-preview frame, 04:50](../assets/fatality-review-2024-model-preview.png)

A review published **2024-10-01** supplies another opaque purple menu/model-preview example. The metadata's “Configuration system” chapter did not yield a clear config-management interface in the inspected 02:02–02:43 samples; it was not promoted into a save-system case study.[8]

## Platform coverage and unresolved leads

- **YouTube:** direct video downloads and selected-frame inspection supplied the official Neverlose visual-settings study.[4] The official channel also exposed a 2026-11-10 config tutorial, used above.[1]
- **Bilibili:** directly downloaded and inspected the Fatality community preview. A Chinese mirror of the Neverlose visual tutorial also surfaced, but it is **not an independent design reference** and was not downloaded.[6][9]
- **Creator website:** downloaded and inspected three original NXC PNGs; their page calls this a Fatality framework.[7]
- **VK:** LoveSync search results mention indicator variants. Direct retrieval returned a compatibility/JS page rather than the posts. **Unverified lead only; no VK screenshot or visual claim added.**[10]
- **Official update forum:** the September 2026 release surfaced as a lead for updated watermark/radius presentation. Direct HTML/JSON requests hit Cloudflare; archive recovery and browser access did not recover the post. **Search lead only, not inspected floor-outline evidence.**[11]

## Asset provenance

The [asset manifest](../assets/reference-media-2026-11-16-manifest.json) records each selected PNG's source ID, timestamp where applicable, dimensions and SHA-256. The [source ledger](../assets/reference-media-2026-11-16-sources.json) maps IDs to retrieved URLs. No executable/script from these media sources was installed or run, and no game project was modified.

## Sources
[1] https://www.youtube.com/watch?v=TE_AsKy5GBU — Neverlose official: Misc and Skinchanger Part 2
[3] https://www.youtube.com/watch?v=6WYeedbSXfM — Gamesense legacy review
[4] https://www.youtube.com/watch?v=fCtdJn4k7vs — Neverlose official: Visuals Part 1
[5] https://www.youtube.com/watch?v=0Ax94kiatgU — Perfect NL Indicators release
[6] https://www.bilibili.com/video/BV1EK6uYwE45 — Fatality community Lua preview
[7] https://nxc-lua.com/showcase — NXC Lua creator showcase
[8] https://www.youtube.com/watch?v=pMb6hBihTZQ — Fatality CS2 review
[9] https://www.bilibili.com/video/BV1XaMH6uEHi — Bilibili mirror: Neverlose visual tutorial
[10] https://m.vk.com/lovesync?offset=10&own=1 — VK LoveSync search lead
[11] https://forum.neverlose.cc/t/major-cs2-product-update-double-precision-15-09-26/592958 — September official update search lead

Related: [Visuals](INDEX.md), [References](../references/INDEX.md), [Widgets](../widgets/INDEX.md), [Ground area markers](ground-area-markers.md)
