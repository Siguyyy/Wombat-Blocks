# Changelog

## v0.1.8-preview — potential simulator build (2026-10-28)

- Use closed-loop PWM encoder feedback for automatic distance moves and pivots, avoiding native MRP completion state between consecutive blocks.
- Add a gradual PWM ramp and keep independent wheel progress correction and stopping.
- Correct preview pivot heading units so a requested 90° turn changes the simulated heading by 90°.
- Set default generated-C left/right turn calibration to 1.30; older local projects migrate from the prior defaults.
- Include a four-side square program example for simulator checks. This potential preview is packaged for user testing and has not been run by the agent in the simulator.

## v0.1.7-preview — position-target drive control

- Encoder-distance moves and pivots used native KIPR MRP relative-position goals by default, with both motor targets issued together and completion monitored independently.
- Kept PWM with encoder feedback as an alternative and retained direct MAV velocity commands.
- Migrated saved drive and pivot blocks from the old MAV loop to MRP.

## 2026-10-28 — Motion controller overshoot fix

- Clamp encoder progress to each wheel target before applying synchronization correction.
- Compute remaining distance from unfinished wheels only, preventing a completed wheel from producing a negative deceleration fraction and forcing the other wheel to minimum speed.
- Preserve separate per-wheel targets and the configured left/right motor ports.

## 2026-10-28 — Camera close compatibility

- Removed generated camera_close() calls because the Wombat KIPR headers do not declare that function.
- Kept camera open, frame update, and object-detection blocks available.

## 2026-10-28 — Startup hotfix

- Removed a duplicate top-level JavaScript declaration that stopped the app script from parsing and prevented Blockly and UI controls from initializing.

## 2026-10-28 — Claw pose visualization

- Connected the claw jaws to the single arm with a visible hinge mount and crossbar.
- Added an adjustable raised-arm top-view reach percentage; the beam, claw, and obstacle footprint shorten together when the arm is raised.

## Earlier repository baseline

See HISTORY.md for earlier checkpoints.

## 2026-10-28 — Direct robot run prototype

- Added the Voldigate API workflow to discover KISS users and C projects, write generated main.c, compile, stream output, and stop a run.
- Added an action-time warning that the selected project's src/main.c will be replaced and the program will run on physical hardware.
- Uses documented-in-source routes available in the Wombat OS Voldigate service; browser local-network permissions and firmware variation still need testing on an actual Wombat.

## 2026-10-28 — KISS IDE launcher

- Added an IP-address helper that opens the Wombat-hosted KISS IDE at port 8888.
- The helper remains available inside the robot connection dialog for opening the IDE manually.

## 2026-10-28 — Current repository baseline

- Rebuilt the editor around Blockly's Zelos renderer and kept the Blockly runtime locally vendored for offline use.
- Added a centimeter coordinate map, obstacle create/edit/move controls, route fit/zoom, and live preview coordinates from (0, 0) facing +Y.
- Added configurable chassis/track and arm length/width, with a single narrow central arm beam, one hinge, and two claw jaws.
- Added generated KIPR C for calibrated encoder-distance drive and pivot, MAV and PWM alternatives, per-side drive and turn calibration, motor encoder commands, servo/claw actions, wait/freeze/ao/off, and safe cleanup.
- Expanded sensor/control/output blocks: bounded analog/digital loops, conditions, camera object checks, sensor logging, beep, console messages, and digital output.
- Corrected analog/digital block port bounds and marked live sensor/camera/direct-motor paths as hardware-only in the map preview.
- Added portable project JSON save/load and browser-local autosave.
- Preserved the accessible pre-change app backups under history/iterations/ and the early standalone C sample under history/legacy/.
