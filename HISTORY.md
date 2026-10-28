# Development history

The repository was created from the Wombat Blocks project as it existed on 2026-10-28. The archived HTML files under `history/iterations/` are the pre-change checkpoints available in the development workspace. They are kept as snapshots, not as additional app entry points. The current app is always `index.html`.

| Snapshot | Development checkpoint |
|---|---|
| `00-early-recorder.html` | Early block recorder before the later UI rebuild. |
| `01-before-claw-blocks.html` | Before configured arm/claw actions were added. |
| `02-before-speed-turn-fix.html` | Before speed and turn calibration corrections. |
| `03-before-turn-scales.html` | Before separate left/right pivot calibration. |
| `04-before-byline.html` | Before the author byline was added. |
| `05-before-sprite-proportions.html` | Before chassis and wheel proportions were updated. |
| `06-before-single-arm.html` | Before the narrow single-hinge arm sprite revision. |
| `07-before-sensor-blocks.html` | Before sensor, camera, and output block expansion. |

`history/legacy/wombat_autonomous_fixed.c` is an earlier standalone C sample, retained as a reference; it is not the current block generator output. The snapshot files were copied from dated backups and do not imply that each file was a formally released version.

The current editor includes the accumulated UI, map, calibration, robot sprite, block, and C-generation work. See [`CHANGELOG.md`](CHANGELOG.md) for the feature-level summary.
