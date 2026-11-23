---
title: Menu UI audit validation
date: 2026-11-23
tags: [implementation, audit, validation, imgui]
---

# Menu UI audit validation

Independent adversarial validation of the 12 findings in the earlier Menu Vault UI Audit dated 2026-11-23. The earlier audit was supplied separately; its findings were treated as hypotheses.

Findings 6–11 identify concrete UI implementation defects. Findings 1–5 chiefly concern partial recipes or incomplete specifications; their original critical-blocker framing is overstated. Finding 4's proposed invisible-focus failure overlooks inherited toolkit behavior. Finding 12 is reproducible but concerns local maintenance documentation.

This review changed no implementation or reference specification. Recommendations below are proposals, not applied fixes.

## Evidence and validation limits

The Markdown reachability traversal and existing link checker were executed. The C++ interface and GPU renderer were not executed: no C++ compiler was found on PATH, and no bundled ImGui checkout was found in the vault. UI reproductions below are deterministic source traces or proposed harness conditions, not executed tests.

The upstream Dear ImGui source inspected identified itself as 1.93.1 WIP, matching the version named in the build instructions. The vault does not pin its dependency commit. Upstream links below point to a moving branch; their cited line numbers describe the snapshot inspected during validation.

Vault line references describe the local files as inspected before adding this report. The local checkout contained pre-existing uncommitted reference-note changes relative to GitHub main at d29918a8ea541c295441fa1a690bbbb91c7fd5d0. Those changes are not included in the report commit, so linked GitHub notes may differ from the audited local versions. The C++ headers cited in this report had no pre-existing tracked changes. Local-only maintenance files are named as plain paths because the repository deliberately excludes them from publication.

Validation levels: **Confirmed by execution** means a runnable check reproduced the issue; **Confirmed by source** means a complete relevant source trace establishes it; **Likely** requires runtime or external confirmation; **Unverified** means insufficient evidence; **Invalidated** means the claim relied on a false assumption. A confirmed documentation gap does not establish a runtime defect.

## 1 Category selection and content transitions

**Status:** Confirmed by source, limited to supplied demos. Original severity overstated.

**Classification:** Application-level concern for page routing; correct specification with an incomplete implementation recipe for content fades.

**Evidence:** [Glass sidebar menu](glass-sidebar-menu.md), line 9; [Motion](../principles/motion.md), line 15; [glass_sidebar_menu.h](code/glass_sidebar_menu.h), lines 435–441, 537–548, 614–636 and 673–679; [demo_menu.h](code/demo_menu.h), lines 31–64.

**Minimal reproduction:** Display DrawGlassMenu, change a setting, then select another rail category and subcategory.

**Expected:** If treating the demo as a functioning multipage menu, category-specific content changes and incoming content fades.

**Source-predicted result:** Category and subcategory decorations change, but the unconditional DrawBody call renders the same static settings. Search does select a different body-rendering path.

**Counterarguments:** The settings are explicitly placeholders. The example has no real category-specific settings to route. Standalone means it draws its own window, not necessarily that it supplies an application navigation framework. Per-category subtab selection and pill animation already exist.

**Root cause:** Body submission is independent of selected category; no content-transition state is supplied.

**Recommended correction:** State the demo's category behavior explicitly. A small two-page example would address the fidelity gap. Comprehensive routing, scroll restoration and page ownership are optional application architecture.

**Confidence:** High for demo behavior; weak justification for classifying routing as a critical library defect.

## 2 Blur composition and rendering order

**Status:** Confirmed by source as an incomplete rendering specification. No rendering failure reproduced.

**Classification:** Incomplete reference specification; renderer integration belongs to the consuming application.

**Evidence:** [Draw-list recipes](drawlist-recipes.md), lines 61–71; [Depth, glass and glow](../principles/depth-glass-and-glow.md), lines 18–25; [velocity.cat reference](../references/velocity-cat-menu.md), lines 70–72; explicit fallback in [Glass sidebar menu](glass-sidebar-menu.md), line 28.

**Minimal reproduction:** Consider a high-contrast scene, translucent menu, row popup over menu text, and dropdown over that popup. Attempt to derive every blur capture and composition stage solely from the notes.

**Expected:** A uniquely reproducible glass appearance needs sampled-layer ownership, blur parameters and composition order.

**Actual documentation result:** The notes establish Gaussian backdrop blur and surface-over-blur composition, but leave downsampling, repeated passes and nested capture policy open.

**Counterarguments:** Capturing before ImGui for scene blur and after the main window for popup blur are compatible stages, not contradictory instructions. The recipe openly substitutes near-opaque popups. Different blur parameters can produce acceptably similar appearances. The phrase each over its own blur already favors sampling the backdrop beneath each surface, so not all capture choices are equally faithful.

**Root cause:** A generic pipeline outline substitutes for a baseline-specific rendering contract.

**Recommended correction:** Add an optional reproducible blur profile and nested-layer example. Distinguish CPU submission of ImGui commands from actual GPU rendering before capture.

**Confidence:** High that the specification is underdetermined; unverified that any particular renderer produces an incorrect result.

## 3 Preferred picker completeness

**Status:** Confirmed by source as an acknowledged partial implementation and behavioral evidence gap. Mandatory-complete-picker framing overstated.

**Classification:** Incomplete reference specification.

**Evidence:** [Color pickers](../widgets/color-pickers.md), lines 26–34; [Neverlose popups](../references/neverlose-popups.md), lines 61–68; [Custom widgets](custom-widgets.md), line 21; [menu_widgets.h](code/menu_widgets.h), lines 273–288.

**Minimal reproduction:** Open ColorRow's picker. Attempt to edit its hex readout, switch format, or explicitly save a color.

**Expected:** The visual target includes format and saved-color affordances. Their exact interaction cannot be established from the capture.

**Source-predicted result:** Hex is rendered with AddText. Hex editing, format switching and saved-color controls are absent.

**Counterarguments:** The widget note explicitly allows a stock quick stand-in and calls PickerBody a first version. The source study marks the grid icon's meaning unconfirmed. These caveats prevent treating observations as established behavior.

**Root cause:** The target exceeds the implemented subset, while screenshot evidence does not establish persistence or commit semantics.

**Recommended correction:** Separate observed appearance, implemented behavior and unknown original behavior. Define project-specific semantics only when needed, without presenting them as historical facts.

**Confidence:** High for missing controls; unverified for the original product's intended behavior.

## 4 Custom control focus behavior

**Status:** Unverified as a defect. The proposed invisible-focus example is undermined by toolkit behavior.

**Evidence:** [Toggles and checkboxes](../widgets/toggles-and-checkboxes.md), lines 30–37; [menu_widgets.h](code/menu_widgets.h), lines 15–56; [menu_style.h](code/menu_style.h), lines 47–50.

**Minimal reproduction:** In a harness, enable keyboard navigation, submit CheckRow, navigate to it, wait for its animation, and activate it.

**Expected:** A navigable control provides feedback and responds to activation.

**Source-predicted result:** ItemAdd registers the control and ButtonBehavior supplies interaction state. Under its navigation conditions, ButtonBehavior reports the focused item as hovered, feeding the existing label-brightening animation. A separate navigation outline is absent. See [upstream ButtonBehavior](https://github.com/ocornut/imgui/blob/master/imgui_widgets.cpp#L646), lines 646–653.

**Counterarguments:** Missing RenderNavCursor does not establish missing visible feedback or activation. InvisibleButton itself calls RenderNavCursor, although navigation requires its enabling flag. See [upstream InvisibleButton](https://github.com/ocornut/imgui/blob/master/imgui_widgets.cpp#L789), lines 789–805.

**Root cause:** The audit searched for explicit focus-rendering functions without fully accounting for toolkit-supplied state. The documentation does lack a distinct focus contract.

**Recommended correction:** State whether focus deliberately shares hover treatment and document keyboard support per control. Test sliders and popup navigation separately.

**Confidence:** High that the original inference is insufficient; broader keyboard usability remains unverified.

## 5 Numeric binding values

**Status:** Confirmed by source, scoped to pm::Hotkey.

**Classification:** Correct specification with an incomplete implementation recipe.

**Evidence:** [Keybinds](../widgets/keybinds.md), lines 17 and 26; [popup_menus.h](code/popup_menus.h), lines 53–58 and 260–262; [Popup menus](popup-menus.md), line 9.

**Minimal reproduction:** Represent two bindings for one slider: F applies 20; Mouse 4 applies 80.

**Expected:** Each entry retains a numeric value and offers a suitable editor.

**Source-predicted result:** Hotkey::value stores a boolean and its editor negates it. Assigning either nonzero number collapses it to true.

**Counterarguments:** The demo edits switches. Numeric-value guidance exists for another binding variant. Binding execution is explicitly outside this header's purpose.

**Root cause:** A switch-oriented model accompanies broader any-widget integration guidance.

**Recommended correction:** Scope this recipe explicitly to boolean settings, or document a typed extension and editor.

**Confidence:** High. This is a representation/editor limitation, not evidence of broken application binding execution.

## 6 Row popup animation lifecycle

**Status:** Confirmed by source.

**Classification:** Correct specification with an incorrect implementation recipe.

**Evidence:** [Row settings popup](../widgets/row-settings-popup.md), lines 15–16; [glass_sidebar_menu.h](code/glass_sidebar_menu.h), lines 168–178; [menu_anim.h](code/menu_anim.h), lines 14–23.

**Minimal reproduction:** Submit a closed row popup for several frames, open it, then dismiss it after it becomes fully visible.

**Expected:** Height and alpha animate open at speed 14 and closed at speed 16.

**Source-predicted result:** Opening changes alpha only; height is content-driven. Closing ends popup body submission. BeginPopupEx immediately rejects closed popups, so decreasing stored alpha does not render a closing image. See [upstream popup lifecycle](https://github.com/ocornut/imgui/blob/master/imgui.cpp#L11781), lines 11781–11796.

**Counterarguments:** The source comment admits omitted height growth. Reduced motion intentionally disables animation.

**Root cause:** Geometry is never animated, both directions use the same speed, and logical closure ends rendering before an exit transition.

**Recommended correction:** Document the reduced recipe or retain closing visuals while animating height and alpha.

**Confidence:** High.

**Additional safeguard and edge case:** AnimTo initializes to its first target. If the first helper submission already sees the popup open, it starts fully opaque. Prior closed submissions initialize zero and permit opening fade. Quick reopening can retain a high animation value.

## 7 Opacity inheritance in custom primitives

**Status:** Confirmed by source.

**Classification:** Integration issue between a fading container and custom rendering primitives.

**Evidence:** [glass_sidebar_menu.h](code/glass_sidebar_menu.h), line 173; [menu_widgets.h](code/menu_widgets.h), lines 222–264 and 309–318.

**Minimal reproduction:** Inside a visible window, push style Alpha to 0.25, call PickerBody, and pop the style. Inspect submitted primitive colors.

**Expected:** Gradients, rings, checkerboard and text consistently reflect surrounding opacity while preserving intrinsic color alpha.

**Source-predicted result:** The pure-hue endpoint converted with GetColorU32 receives approximately 25% alpha. Raw white, black, hue segments and checkerboard colors retain their supplied alpha. The SV gradient therefore changes opacity across its width.

**Counterarguments:** An application could fade a completed offscreen layer. No such integration is supplied. Style alpha 1 masks the inconsistency.

**Root cause:** GetColorU32 multiplies style alpha, while gradient drawing writes supplied packed colors directly. See [upstream color conversion](https://github.com/ocornut/imgui/blob/master/imgui.cpp#L3485) and [gradient vertices](https://github.com/ocornut/imgui/blob/master/imgui_draw.cpp#L2407).

**Recommended correction:** Establish one opacity rule for custom primitives.

**Confidence:** High.

The original manual-picker example combines components, but the native glass path also exposes the issue: glass_sidebar_menu.h, lines 327–335, draws raw checkerboard and swatch colors inside the fading row popup.

## 8 Dropdown text clipping

**Status:** Confirmed by source for chevron overlap. Exact ellipsis policy is less certain for this variant.

**Classification:** Correct specification with an incorrect implementation recipe.

**Evidence:** [Combos and dropdowns](../widgets/combos-and-dropdowns.md), line 12; [glass_sidebar_menu.h](code/glass_sidebar_menu.h), lines 256–263; alternative protection in [menu_widgets.h](code/menu_widgets.h), lines 150–153.

**Minimal reproduction:** Render a glass combo with a selected string wide enough to reach its chevron, keeping the whole field visible.

**Expected:** Text ends before the chevron using the selected variant's overflow policy.

**Source-predicted result:** The full string is submitted without a field-specific clip and can occupy the arrow region.

**Counterarguments:** Enclosing-window clipping limits spill but does not reserve arrow space. The generic ellipsis rule precedes the label-over variant, while other references deliberately fade long text.

**Root cause:** No independent value-text region. AddText uses the current draw-list clip unless supplied a finer one. See [upstream text clipping](https://github.com/ocornut/imgui/blob/master/imgui_draw.cpp#L2695), lines 2695–2717.

**Recommended correction:** Define the variant's overflow policy and reserve space before the arrow.

**Confidence:** High for overlap.

ComboRow prevents overlap but hard-clips; it does not implement the documented ellipsis either.

## 9 Popup positioning and viewport boundaries

**Status:** Confirmed by source for the explicitly positioned helpers.

**Classification:** Correct specification with an incorrect implementation recipe.

**Evidence:** [Row settings popup](../widgets/row-settings-popup.md), line 25; [Tooltips](../widgets/tooltips.md), line 31; [glass_sidebar_menu.h](code/glass_sidebar_menu.h), lines 171–174; [popup_menus.h](code/popup_menus.h), lines 371–380.

**Minimal reproduction A:** Put a 220 px popup's trigger at x=666 in a menu spanning x=0–700 on a wide viewport.

**Expected A:** Clamp or flip within the menu.

**Source-predicted result A:** Requested popup extent is x=666–886.

**Minimal reproduction B:** Place the root menu's right edge at x=1900 on a 1920 px viewport; hover until its tooltip appears.

**Expected B:** Dock the tooltip left.

**Source-predicted result B:** Requested tooltip left edge is x=1906. Width above 14 px exceeds the viewport.

**Counterarguments:** Ordinary ImGui popups have automatic placement. However, explicit positioning bypasses automatic placement and general position clamping in the inspected source. Clipping hides pixels rather than repositioning content. See [upstream positioning](https://github.com/ocornut/imgui/blob/master/imgui.cpp#L7410), lines 7410–7426.

**Root cause:** Fixed anchors lack menu and viewport boundary policies.

**Recommended correction:** Define bounds and flip/clamp order separately for row popups, tooltips and submenus.

**Confidence:** High under the inspected dependency.

ImGui does supply boundary safeguards; these helpers bypass relevant ones. The explicitly positioned hotkey submenu at popup_menus.h, line 329, has the same exposure.

## 10 Tooltip hover timing

**Status:** Confirmed by source.

**Classification:** Correct specification with an incorrect implementation recipe.

**Evidence:** [Tooltips](../widgets/tooltips.md), lines 26–27; [popup_menus.h](code/popup_menus.h), lines 354–366.

**Minimal reproduction:** At 60 FPS, hover A for 12 frames, move to blank space for one second, then hover A for another 12 frames. Do not hover another tooltip-bearing item between visits.

**Expected:** Neither 0.20-second visit satisfies the 0.35-second delay.

**Deterministic trace:** First visit stores id=A and t approximately 0.20. Nonhovered calls return without resetting. The second visit retains A's ID and adds approximately 0.20, reaching approximately 0.40.

**Source-predicted result:** The tooltip becomes eligible during the second short visit.

**Counterarguments:** Another hovered tooltip item changes the stored ID and may reset it. Immediate chaining after a shown tooltip is intentional, but no tooltip was shown in this reproduction.

**Root cause:** State tracks item identity without hover continuity. The 0.25-second chaining rule initializes time only when the ID changes.

**Recommended correction:** Detect hover gaps and reset elapsed time independently of intentional chaining.

**Confidence:** High.

A previously shown A can reappear immediately after an arbitrarily long gap because its unchanged ID retains elapsed time.

## 11 Key capture state ownership

**Status:** Confirmed by source.

**Classification:** Integration issue between submenu switching and capture-state ownership.

**Evidence:** [Keybinds](../widgets/keybinds.md), capture rules beginning at line 30; [popup_menus.h](code/popup_menus.h), lines 216–244, 289–290 and 312–334.

**Minimal reproduction:** Create bound entries A and B. Open A's submenu, click Key to listen, move through the parent onto B without clicking, then press G after B's submenu is visible.

**Expected:** Capture remains associated with A or is cancelled when A's submenu is replaced.

**Deterministic trace:** A stores listening=true under the submenu's fixed listen key. Hovering B changes sub_index but reopens the same sub_id. B reads the same listening flag; capture writes G through the reference now pointing to B.

**Counterarguments:** Left-clicking elsewhere cancels capture; this sequence uses hover. Closing trims the popup stack; reopening the same ID reuses its named window. See [upstream closure](https://github.com/ocornut/imgui/blob/master/imgui.cpp#L11730), lines 11730–11750, [popup identity](https://github.com/ocornut/imgui/blob/master/imgui.cpp#L11781), and [window reuse](https://github.com/ocornut/imgui/blob/master/imgui.cpp#L7078).

**Root cause:** Capture has no binding identity, and submenu replacement does not cancel it. PushID(i) applies only to parent entry rows and is popped before HotkeySubmenu executes.

**Recommended correction:** Use stable binding identity and define cancellation on switching, dismissal and deletion.

**Confidence:** High.

## 12 Root reachability promise

**Status:** Confirmed by execution.

**Classification:** Incorrect reference specification.

**Evidence:** Local-only CLAUDE.md, line 20; local-only READMEFIRST.md, line 22; [README](../README.md), sharing scope beginning at line 49; local-only tools/check_links.py, lines 64–65.

**Minimal reproduction:** Strip code and parse local Markdown links using the checker rules, then breadth-first traverse from README.

**Expected:** Every note is reachable within two hops.

**Executed result before this report was added:** 87 Markdown notes, 83 reachable. CLAUDE.md, READMEFIRST.md, templates/new-reference.md and visuals/media-discovery-routes.md were unreachable. Every reachable note was within two hops. The existing checker reported 87 notes checked, 0 problems.

**Counterarguments:** README explicitly keeps maintenance instructions and acquisition queues local. Obsidian exposes files without link traversal. Their absence from public navigation may be intentional.

**Root cause:** An absolute promise conflicts with sharing scope; incoming-link checks do not establish root reachability.

**Recommended correction:** Restrict the promise to published reference content or deliberately revise local navigation. Do not publish maintenance material automatically to satisfy outdated wording.

**Confidence:** High.

## Impact on faithful reproduction

This ranks confirmed defects and confirmed documentation gaps by expected impact on an agent reproducing a visually faithful menu.

| Rank | Finding | Expected impact |
|---|---|---|
| 1 | 9 Boundary placement | Clips or hides entire overlays near edges. |
| 2 | 7 Opacity composition | Produces inconsistent fades and translucent controls. |
| 3 | 6 Popup lifecycle | Omits height animation and closing motion. |
| 4 | 2 Blur contract gap | Material glass-fidelity uncertainty when blur is required; no faulty renderer demonstrated. |
| 5 | 8 Dropdown overflow | Visible text and chevron collisions. |
| 6 | 3 Partial picker | Limits a preferred component; limitations are disclosed. |
| 7 | 11 Capture ownership | Can edit the wrong binding; highest interaction-correctness concern. |
| 8 | 10 Tooltip timing | Premature appearance after interrupted hover. |
| 9 | 1 Demo transitions | Missing navigation demonstration; routing is application-specific. |
| 10 | 5 Numeric bindings | Blocks an editor extension, with little static visual impact. |
| 11 | 12 Reachability wording | Low visual impact; local maintenance/distribution scope needs clarification. |

Finding 4 should not enter the confirmed-defect backlog without a narrower keyboard-navigation reproduction.

Related: [Implementation](INDEX.md), [Glass sidebar menu](glass-sidebar-menu.md), [Custom widgets](custom-widgets.md), [Popup menus](popup-menus.md), [Draw-list recipes](drawlist-recipes.md)

