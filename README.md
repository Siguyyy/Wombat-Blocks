# Wombat Blocks — Visual Programming for KIPR Wombat

A Blockly editor for building programs for the KIPR Wombat robot. Combine drive, pivot, motor, servo, sensor, and control blocks; preview supported motion on a centimeter map; and export C code for the Wombat KISS IDE.

**Latest preview: [v0.1.23](https://github.com/Siguyyy/Wombat-Blocks/releases/tag/v0.1.23-preview)** · [Browse all releases](https://github.com/Siguyyy/Wombat-Blocks/releases) · [View source](https://github.com/Siguyyy/Wombat-Blocks)

> **Robot safety:** The map is an estimate, not a live robot simulator. Test new motion code with the wheels lifted first. Calibrate the assembled robot. Encoder control cannot compensate for wheel slip, changing surfaces, or every motor difference. Sensor branches, camera results, and direct motor commands require hardware and cannot be predicted by the map preview.

---

## ✨ What it does

- **Build with blocks:** calibrated distance moves and pivots, independent left/right wheel-speed moves, motors, servos, waits, loops, conditionals, operators, and sensor actions.
- **Trace and retrace a route:** mark a return point, record supported encoder-based drive and pivot blocks, then retrace those steps in reverse.
- **Keep projects:** JSON projects from earlier builds retain their normal blocks and settings. The withdrawn waypoint experiment imports as explanatory comments because its estimated-position navigation was unreliable.
- **Use sensors and diagnostics:** read analog/digital inputs and encoders, wait for stable sensor states, sample analog readings, and print values to the KISS console.
- **Preview supported paths:** place obstacles on a centimeter map and inspect the estimated robot route and coordinates.
- **Export and run:** view or download generated KIPR C. The optional robot connection can save, compile, and run it through the Wombat KISS API; keyboard drive is generated code and requires KISS Graphics keyboard input (the web simulator skips it). Variables, timers, stable sensor waits, and diagnostics are included.

## 🚀 Get started

### Run locally

1. [Download the latest preview](https://github.com/Siguyyy/Wombat-Blocks/releases/tag/v0.1.23-preview) and extract the ZIP.
2. Open `index.html` in Chrome, Edge, or Firefox. Keep the `blockly/` folder beside it.
3. Connect blocks to **when program starts**, then choose **View generated C** to inspect or download `main.c`.

Blockly is bundled with the project; no package manager, build step, or external CDN is required to open the editor.

### Run on a Wombat

Create a dedicated C project in KISS first. **Run on robot** writes the generated code to that project's `src/main.c`, compiles it, and starts the physical robot. Review the overwrite prompt and supervise the robot during operation. See [robot connection notes](docs/ROBOT_CONNECTION.md).

## 🧰 Setup and calibration

Set the drive motor ports and forward signs in **Robot setup** to match your wiring. Measure wheel diameter and track, then tune distance and turn scales with short, supervised runs. The independent wheel-speed block can compensate for a consistent straight-line bias by adjusting one side's percentage. New tools include variables, elapsed timers, stable sensor waits, encoder and analog diagnostics, and an optional keyboard-drive block. Route retracing records calibrated drive/pivot blocks. Direct motor commands and sensor-triggered movement are not treated as reversible route steps.

Analog readings are raw sensor values, not distances until calibrated for the specific sensor. Review [sensor and control blocks](docs/SENSOR_BLOCKS.md), [commands and hardware](docs/COMMANDS_AND_HARDWARE.md), and [architecture notes](docs/ARCHITECTURE.md) before using advanced blocks.

## 📦 Project contents

- `index.html` — editor, Blockly definitions, map preview, project storage, and C generator.
- `blockly/` — locally bundled Blockly runtime, English messages, media, and its license.
- `docs/` — hardware limits, sensor/control guidance, architecture, and robot connection notes.
- `history/` — selected historical app checkpoints and a legacy C sample.

Projects autosave in the current browser. Use **File → Save project (.json)** to move a workspace between browsers. The generated program is C for `kipr/wombat.h`.

## ⚠️ Current status and limitations

This is an evolving preview project. Generated C has not been validated on every Wombat firmware or robot configuration. Supported map motion is deterministic; live sensor readings, camera detections, and direct motor outputs are not. Retrace replays supported encoder-based moves and may not return to the exact physical starting point if the robot slips or is obstructed.

## 📄 License

No project-level license has been selected, so the repository's source remains under its author's copyright. Blockly's bundled files are separately distributed under Apache License 2.0; see [`blockly/LICENSE`](blockly/LICENSE) and [`NOTICE.md`](NOTICE.md).
