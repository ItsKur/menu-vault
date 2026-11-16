---
title: Projectile timing — arrival versus remaining lifetime
date: 2026-11-16
tags: [visuals, projectile, timer, countdown, reference]
---

# Projectile timing — arrival versus remaining lifetime

## Separate these states in a new design

Design recommendations:

| State | Question answered | Appropriate visual |
|---|---|---|
| Predicted arrival | How long until the object reaches its predicted destination? | Small time label anchored to the predicted endpoint |
| Active lifetime | How long does the existing area remain? | Remaining-time ring/bar paired with the live area |
| Direction / proximity | Where is the relevant object relative to the viewer? | Clamped directional badge, with distance if useful |
| Ground footprint | Which part of the floor is covered? | Perspective-correct boundary and restrained fill |

Give arrival and active lifetime distinct labels or states so the timer remains clear across the transition.

## Seen in

- [Fatality Halloween — proximity warnings](fatality-halloween-proximity-warning.md): molotov pending/active states, grenade warning and seconds-style labels.

- [Neverlose 2026 world/windows](neverlose-2026-world-and-windows.md): presentation controls, time-remaining option and trajectory.
- [Countdown badges](countdown-badges.md): existing source-backed lifetime/directional construction.
- [Ground area markers](ground-area-markers.md): existing floor-footprint methods.

Related: [Countdown badges](countdown-badges.md), [Ground area markers](ground-area-markers.md), [Media sweep](reference-media-sweep.md)
