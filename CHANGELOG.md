# Changelog

## 2026-10-28 — KISS IDE launcher

- Added an IP-address helper that opens the Wombat-hosted KISS IDE at port 8888.
- Clarified in the interface and connection guide that direct upload/compile/run is not available through a documented external API.

## 2026-10-28 — Current repository baseline

- Rebuilt the editor around Blockly's Zelos renderer and kept the Blockly runtime locally vendored for offline use.
- Added a centimeter coordinate map, obstacle create/edit/move controls, route fit/zoom, and live preview coordinates from `(0, 0)` facing `+Y`.
- Added configurable chassis/track and arm length/width, with a single narrow central arm beam, one hinge, and two claw jaws.
- Added generated KIPR C for calibrated encoder-distance drive and pivot, MAV and PWM alternatives, per-side drive and turn calibration, motor encoder commands, servo/claw actions, wait/freeze/ao/off, and safe cleanup.
- Expanded sensor/control/output blocks: bounded analog/digital loops, conditions, camera object checks, sensor logging, beep, console messages, and digital output.
- Corrected analog/digital block port bounds and marked live sensor/camera/direct-motor paths as hardware-only in the map preview.
- Added portable project JSON save/load and browser-local autosave.
- Preserved the accessible pre-change app backups under `history/iterations/` and the early standalone C sample under `history/legacy/`.

## Earlier checkpoints

See [`HISTORY.md`](HISTORY.md) for the dated snapshots that were available when this repository was assembled.
