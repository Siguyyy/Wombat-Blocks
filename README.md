# Wombat Blocks

**A Blockly-based visual programming workspace for the KIPR Wombat robot.** Build a block program, estimate its route on a centimeter map, and generate KIPR C source for the Wombat's KISS Web IDE.

> **Robot safety:** The map is an estimate, not a live robot simulator. Calibrate wheel size, encoder scale, motor directions, track width, servo endpoints, and sensor thresholds on the assembled robot. Test new motion code with the wheels lifted first. Sensor branches, camera detections, and direct motor commands cannot be predicted by the map preview.

## Open the editor

1. Download this repository as a ZIP and extract it, or clone it with Git.
2. Open `index.html` in Chrome, Edge, or Firefox. Keep the `blockly/` folder beside it.
3. Blockly is included locally, so the editor itself does not rely on a CDN or a build step.

The app is also structured for static hosting. `index.html` is at the repository root; GitHub Pages or another host must be enabled separately in its settings.

## Features

- Blockly with the Zelos renderer and a Wombat-themed toolbox.
- Forward/reverse encoder-distance movement and calibrated pivots use closed-loop encoder-count feedback with PWM and a gradual power ramp. Explicit motor-position blocks retain native MRP; direct motor blocks retain MAV/PWM commands.
- Direct motor/encoder commands, servo controls, configurable arm/claw actions, and stop modes.
- Digital and analog sensor waits/conditions, bounded sensor loops, start-light waits, and camera object checks.
- Console messages, sensor logging, beep, and digital output.
- Centimeter map with editable obstacles, route preview, and live X/Y coordinates.
- Generated KIPR C, project JSON import/export, and browser-local autosave.
- Direct robot run flow: connect to the Wombat, select a KISS C project, save generated code, compile, run, stream console output, and stop.
- Preserved historical app checkpoints in [`history/iterations/`](history/iterations/).

See [`docs/COMMANDS_AND_HARDWARE.md`](docs/COMMANDS_AND_HARDWARE.md) for port limits and sensor notes, and [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) for the app's code-generation and preview design.

## Project files

- `index.html` — current application: UI, Blockly blocks, C generator, map, preview, settings, and project persistence.
- `blockly/` — local Blockly scripts, English messages, media, and Blockly's Apache 2.0 license.
- `docs/` — hardware, architecture, and robot-connection notes.
- `history/iterations/` — dated pre-change snapshots available from the development workspace.
- `history/legacy/` — earlier standalone C sample, retained as a historical reference.

## Save and move projects

The editor autosaves in the current browser's local storage. To move a project to another browser/device, use **File → Save project (.json)** and then **File → Load project (.json)** there. Use **File → Download main.c** to export the generated robot program.

## How it was built

The app is a static HTML/CSS/JavaScript project. Blockly supplies the visual workspace; custom Wombat blocks map to a small action model and a generator that emits KIPR C using `kipr/wombat.h`. A separate route model estimates supported calibrated driving and pivot motion for the 2D map. Sensor-dependent branches and camera detections generate robot-side C, but remain hardware-only in the map preview because their outcomes require live readings.

No package manager or compile step is needed to open the editor. **Run on robot** uses the Wombat's Voldigate/KISS HTTP routes to save `main.c` into a selected KISS project, compile it, start it, stream output, and request stop. Create a dedicated C project in KISS first. The run flow replaces that project's `src/main.c`; review the confirmation dialog each time. Physical robot connectivity and execution have not yet been verified on hardware; see [`docs/ROBOT_CONNECTION.md`](docs/ROBOT_CONNECTION.md).

## Development and hardware status

This is an evolving educational robotics project. The generated code has not been validated against every firmware and sensor configuration. Real movement depends on the assembled robot's motors, wiring, battery, surface, sensor placement, and calibration. The 2020 build guide's sensor builds are examples and do not confirm which parts are installed on a particular robot.

## License and third-party code

No project-level license has been selected. Until one is added, the Wombat Blocks app remains under its author's copyright; repository visibility does not grant reuse rights. Blockly's bundled library files are separately distributed under Apache License 2.0; see [`blockly/LICENSE`](blockly/LICENSE). Other acknowledgements are in [`NOTICE.md`](NOTICE.md).
