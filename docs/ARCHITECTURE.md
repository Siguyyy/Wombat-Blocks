# Architecture

## Runtime shape

Wombat Blocks is intentionally a small static app: `index.html` contains the layout, styling, workspace logic, robot map, preview, local settings, project import/export, and C generator. Blockly is loaded from the checked-in `blockly/` directory. This avoids a package install or external CDN for the core editor and lets the same folder run from a Chromebook file manager or a static host.

## Blockly to KIPR C

1. The toolbox registers Wombat-specific blocks for the controller's motors, servos, sensors, camera, and output.
2. `getActions()` reads the connected statement stacks into a small internal action tree, including nested branch/loop bodies.
3. `actionC()` turns that tree into KIPR C statements. `generateC()` wraps the statements with `kipr/wombat.h`, calibration constants, motion helpers, sensor helpers, cleanup, and `main()`.
4. The full-file and block-only views display that same generated output; project JSON serializes Blockly state, settings, and map obstacles.

The generator targets the KIPR Wombat API (`mav`, `motor`, encoder counters, `freeze`, servo calls, analog/digital input, camera, wait, and console helpers). It is not a general Scratch-to-C compiler. New blocks need a Blockly definition, toolbox entry, parser/model support, and generated C behavior.

## Map and preview

The robot starts at `(0, 0)`, faces `+Y`, and uses centimeters. The route estimator walks the supported calibrated distance/pivot and simple arm/claw actions to estimate position, heading, duration, and obstacle collisions. A hardware execution path is separate: analog/digital branches, camera results, and direct motor commands cannot be inferred from the workspace without real sensor feedback, so those actions do not pretend to have a deterministic map path.

## Persistence

- Browser autosave: local storage on that browser/device.
- Portable project: JSON file containing blocks, robot calibration/settings, and map obstacles.
- Robot source: downloadable generated `main.c` for the KISS Web IDE.

## Extending the app safely

When adding a block, keep the visual block, parser, generated C, hardware port limits, help text, and preview behavior aligned. For sensor branches, use bounded waits/loops and make the map preview limitations visible. Keep calibration values editable and avoid claiming a sensor or mechanism is installed unless the user's wiring confirms it.
