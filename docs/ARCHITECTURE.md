# Architecture

## Runtime shape

Wombat Blocks is intentionally a small static app: `index.html` contains the layout, styling, workspace logic, robot map, preview, local settings, project import/export, and C generator. Blockly is loaded from the checked-in `blockly/` directory. This avoids a package install or external CDN for the core editor and lets the same folder run from a Chromebook file manager or a static host.

## Blockly to KIPR C

1. The toolbox registers Wombat-specific blocks for the controller's motors, servos, sensors, camera, and output.
2. `getActions()` reads connected statement stacks and value expressions into an internal action tree, including nested branches and timed loops.
3. `actionC()` turns that tree into KIPR C statements. `generateC()` wraps the statements with `kipr/wombat.h`, calibration constants, motion helpers, sensor helpers, cleanup, and `main()`.
4. The full-file and block-only views display that same generated output; project JSON serializes Blockly state, settings, and map obstacles.

The generator targets the KIPR Wombat API (`mav`, `motor`, encoder counters, `freeze`, servo calls, analog/digital input, camera, wait, and console helpers). The wheel-trim distance block sends independent MAV speed setpoints to each drive motor while monitoring each encoder target. Return-point blocks reset both encoders and keep a bounded runtime stack of calibrated drive/pivot commands for reverse-order replay; unsupported direct motor actions invalidate the trace. Its Operators category currently supports number values, analog/digital readings, comparisons, boolean combinations, and basic arithmetic. Loop blocks use finite timeouts so a missed sensor event cannot leave an autonomous program running indefinitely.

## Map and preview

The robot starts at `(0, 0)`, faces `+Y`, and uses centimeters. The preview treats pivots as ideal differential-drive rotations: both wheels travel equal distances in opposite directions, the chassis center stays fixed, and heading changes by exactly the requested angle. Turn calibration affects generated robot C only; preview timing comes from wheel track and requested speed. Saved direction-correction toggles remap block labels and dropdown choices while preserving each block’s stored direction value, and the preview applies the same correction so it follows the displayed label. Pivot collision checks sample more finely for a longer robot or arm. A hardware execution path is separate: analog/digital branches, camera results, and direct motor commands cannot be inferred from the workspace without real sensor feedback, so those actions do not pretend to have a deterministic map path.

Servo position commands can be paced by issuing intermediate targets every 10 ms. The speed percentage controls the step size across the configured 0–2047 range; `get_servo_position()` reports the last commanded target, which provides the starting point for the next paced move. The preview estimates move time from the same percentage and servo travel.

## Persistence

- Browser autosave: local storage on that browser/device.
- Portable project: JSON file containing blocks, robot calibration/settings, and map obstacles.
- Robot source: downloadable generated `main.c` for the KISS Web IDE.

## Extending the app safely

When adding a block, keep the visual block, parser, generated C, hardware port limits, help text, and preview behavior aligned. For sensor branches, use bounded waits/loops and make the map preview limitations visible. Keep calibration values editable and avoid claiming a sensor or mechanism is installed unless the user's wiring confirms it.
