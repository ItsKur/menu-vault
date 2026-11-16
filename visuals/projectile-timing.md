---
title: Projectile timing — arrival versus remaining lifetime
date: 2026-11-16
tags: [visuals, projectile, timer, countdown, reference]
---

# Projectile timing — arrival versus remaining lifetime

## Evidence boundary

The new [Neverlose world-visuals case study](neverlose-2026-world-and-windows.md) preserves a presentation popup with a time-remaining option and a live trajectory/endpoint example. It does **not** establish a readable numeric grenade-landing countdown or the exact meaning of the tiny endpoint annotation. Keep that gap explicit rather than renaming a distance label as a timer.

## Separate these states in a new design

Design recommendations:

| State | Question answered | Appropriate visual |
|---|---|---|
| Predicted arrival | How long until the object reaches its predicted destination? | Small time label anchored to the predicted endpoint |
| Active lifetime | How long does the existing area remain? | Remaining-time ring/bar paired with the live area |
| Direction / proximity | Where is the relevant object relative to the viewer? | Clamped directional badge, with distance if useful |
| Ground footprint | Which part of the floor is covered? | Perspective-correct boundary and restrained fill |

Do not imply that a projected landing radius is the final live footprint. Do not use an active-area lifetime ring for travel time without an explicit change of label/state.

## Acquisition checklist

A useful future reference must show at least two moments from one continuous sequence: before arrival, at arrival, then the active area. Record timestamps and units. Preserve one unobstructed full frame plus a readable crop. Menu options alone are only evidence of configurability.

## Seen in

- [Fatality Halloween — proximity warnings](fatality-halloween-proximity-warning.md): inspected molotov pending/active states and grenade warning; seconds-style labels are visible, but fuse versus arrival semantics are not established.

- [Neverlose 2026 world/windows](neverlose-2026-world-and-windows.md): presentation controls and trajectory, numeric landing timer still unverified.
- [Countdown badges](countdown-badges.md): existing source-backed lifetime/directional construction.
- [Ground area markers](ground-area-markers.md): existing floor-footprint methods.

Related: [Countdown badges](countdown-badges.md), [Ground area markers](ground-area-markers.md), [Media sweep](reference-media-sweep.md)
