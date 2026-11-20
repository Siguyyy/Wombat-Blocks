# Changelog
## v2.0.0 — sensor-aware blocks and guided AI handoff

### Added and improved

- Extend per-port analog conditioning across existing analog decisions: median-of-three sampling, clear/empty-band arming, target direction and margin, and consecutive confirmations now serve analog conditions, waits, stable waits, loops, drive-until actions, sensor reporters used in conditions, and start-light waits. Profiles remain off by default and raw diagnostic readings remain raw.
- Add shared debounce for digital inputs and the Wombat physical side button, plus consecutive-frame confirmation for camera detections.
- Add an explicit `if` block and stackable `else if` chains while preserving the original `if/else` block and saved-workspace behavior. Comment blocks can label each conditional branch without breaking its chain.
- Add repeat-forever behavior with controller-button or manual stopping, a safe stop-program block, and cooperative stop checks during supported waits, motion, servo actions, and sensor loops.
- Add console-based drive input following the macro recorder pattern: `w/b` forward/backward, `a/d` pivot, `s` stop, `r` toggle/report reverse gear, `o/c` claw, `p/l` arm, and `q` quit. The browser simulator skips console input; this is not remote browser keyboard control.
- Add variable and operator support, elapsed timers, stable sensor waits, analog sampling, encoder and console diagnostics, independent left/right distance speeds, direct MAV tracing for retrace, and timer/servo speed controls.
- Add a robot preview visibility toggle and saved dark/light theme, improve dark-mode purple contrast, and polish the generated-C panel with full-file default, Escape/outside-click close, and a red close button.
- Add an AI assignment handoff with an extensive block and generated-C reference plus a validated Blockly XML importer. The guide asks the AI to provide clean importable XML in a `.txt` attachment, map every assignment requirement, state sensor/setup assumptions, and disclose unsupported requirements.
- Expand the AI generation guidance: timeouts default to at least 45 seconds, percentage speeds stay at or above 50% and normally use 70%, and ET search uses a clear-before-target gate with repeated confirmed readings based on the user's prior sensor logic. These are AI recommendations, not UI-enforced limits.

### Compatibility and known limits

- Existing Blockly projects retain their blocks and setup. Removed experimental waypoint blocks migrate to explanatory comments; return tracing reverses recorded supported movement and is not map localization.
- Shared analog profiles are disabled until configured from real empty/target readings. Blockly XML does not contain Robot setup calibration.
- Custom student-defined functions/procedures are not available as blocks. The AI guide tells assistants to disclose this instead of claiming a function-based rubric is satisfied.
- The map preview cannot reproduce physical sensor/camera/button input, wheel slip, or every direct motor action. Run-on-robot remains dependent on the browser reaching the Wombat KISS API and its firmware routes.
- The 2.0 release consolidates the local 0.1.25–0.1.30 improvements into a new major version; it does not claim physical robot calibration or testing.

### Package

Download `WombatBlocks_2.0.0.zip`, extract it, and open `index.html` with its bundled `blockly/` directory. See `README.md` and `docs/` for setup, block, sensor, connection, and AI handoff details.

## v0.1.30-preview — comment-safe else-if chains (local build)

- Keep comment blocks between `if` and `else if` clauses attached to the same generated C conditional chain. Emit section comments immediately before their corresponding branch instead of treating each else-if as an orphan and stopping the program.
- Make the orphan-else-if console diagnostic explain that comments may separate clauses, while other actions still break the chain.
- Fix Blockly XML import for the bundled API by using `Blockly.utils.xml.textToDom` with a legacy fallback; accept an optional surrounding `xml` Markdown fence.
- Update the AI guide to require Blockly's exact XML namespace and explain where section comments can go in an else-if chain.
- Change the AI handoff instructions to request a downloadable `.txt` attachment containing only raw Blockly XML, with assumptions and calibration notes kept outside the file.
- Add requirement-by-requirement planning and a final execution-path audit; state that custom student functions are not supported by the current block palette.
- Set generation guidance to timeouts of at least 45 seconds, speed fields of at least 50% (normally 70%), and use the ET clear-then-confirmed-target sensor pattern from the user's earlier C example.

## v0.1.29-preview — AI prompt handoff, XML import, and guide (local build)

- Add a brain/question prompt builder that combines the user’s assignment with the build-specific AI programming guide, then copies or downloads the complete Gemini-ready Markdown prompt.
- Add a Blockly XML import tab that parses the AI response in a temporary workspace, requires a Wombat start block, and asks before replacing the current workspace. Robot setup and obstacle settings are preserved.
- Add a 3,000+ word reference covering every current block, placement rules, generated C patterns, sensor profiles, setup, safety, and the XML transfer format. Embed the same guide in the standalone HTML so prompt building works offline.
- Require the AI response to place the importable Blockly XML alone in a dedicated fenced `xml` code block, with assumptions and explanatory notes outside it for easier copying.
- Open View generated C on the full KIPR file by default, close on outside click or Escape, and replace the plain Close button with a styled red X.

## v0.1.28-preview — forever control, console drive, and stronger dark accents (local build)

- Add a repeat-forever control block with a selectable physical-button stop mode, plus a safe stop-program block. In stop mode, button checks interrupt encoder motions, paced servo moves, ordinary waits, sensor loops, and action sequences; motor output is stopped before cleanup.
- Add a one-block continuous paired-wheel command for forward, reverse, and pivot motion at a speed percentage. It uses the configured drive ports and direction corrections; use a stop-drive block or the button-stoppable forever loop to end it.
- Replace the broken KISS Graphics keyboard window with the macro-recorder's KISS program-console input pattern (`scanf` one command, press Enter). Commands: `w`/`b` drive, `a`/`d` pivot, `s` stop, `r` toggle and announce reverse gear, `o`/`c` claw open/close, `p`/`l` arm up/down, `q` quit. It does not print exported C.
- Increase purple accent contrast in dark mode while keeping the same purple identity.
- Restore generated route-trace types, state, and MAV capture helpers before motion functions; declare the interruptible wait helper before paced servo code so exported C compiles in strict C99 toolchains.
- Add camera declarations for KISS builds and safe no-detection camera stubs for the WebAssembly simulator; implement elapsed timers locally with seconds() instead of undeclared timer functions.

## v0.1.27-preview — shared sensor conditioning and collapsible preview (local build)

- Replace the two ET-specific motion blocks with per-analog-port profiles used by existing analog conditions, waits, stable waits, repeat-until, and drive-until blocks. Profiles combine median-of-three reads, an empty-band clear gate, selectable higher/lower/either target direction, margin, and consecutive confirmation samples.
- Keep profiles disabled by default to preserve previously calibrated sensors and saved projects. Raw analog/digital logs and analog samples continue to show hardware readings. Old ET-specific blocks load as explanatory comments pointing to the shared profile setup.
- Debounce digital switches and the physical controller button with a shared setup value; require consecutive confirmed frames for camera conditions and waits. The physical side button uses `push_button()` on hardware and safely reads false in WebAssembly.
- Add a persisted show/hide toggle for the robot preview with a smooth collapse/restore transition. Preserve preview and theme preferences when resetting robot calibration or creating a blank project.

## v0.1.26-preview — physical controller button, conditional chains, dark theme (local build)

- Add a Boolean physical-button reporter generated through KIPR `push_button()`. Native programs read the built-in right-side Wombat button; WebAssembly simulator builds safely report false because no hardware button exists there.
- Add a simple `if` block and stackable `else if` chain generation. Keep the original fixed `if/else` block and its saved-project semantics unchanged. A misplaced else-if emits a clear runtime message and stops safely.
- Add a single animated sun/moon theme slider and persist its choice in local autosave and portable project settings. Apply the theme to the Blockly workspace and editor panels.
- This revision replaces the experimental ET-specific blocks with the shared port-profile path described below. No turning calibration or drive tuning changed.

## v0.1.25-preview — background-aware ET sensor blocks (local build)

- Add separate ET search and ET approach blocks with configurable empty/target bands, higher/lower/either target polarity, median-of-three reads, consecutive-sample confirmation, and timeouts. The search also stops spinning immediately when the target is confirmed.
- Bound the approach by both encoder distance and time, stop when the ET signal passes or falls below the target region, and preserve the existing raw analog blocks and saved-project behavior.
- Keep all sensor thresholds editable per block; defaults reflect the reported 1200–1800 empty readings and are starting points, not universal ET calibration.

## v0.1.24-preview — retrace direct MAV wheel segments

- Fix return-point retracing when a program uses direct MAV commands on the configured drive ports. The generator records encoder deltas and reverses that measured segment when a stop block closes it.
- Preserve existing drive/pivot replay and safely invalidate traces for unsupported direct PWM, motor-position, and sensor-triggered movement.
- Keep the 0.1.23 drive and pivot controller unchanged; no pivot calibration or behavior was altered in this build.

## v0.1.23-preview — keyboard compile fallback and project compatibility

- Compile the keyboard-drive block as an explicit no-op with a simulator message under WebAssembly, where KIPR Graphics input is unavailable; include the KIPR graphics header only for native KISS builds.
- Replace the generated keyboard block's inline key-speed variable with a shared helper, avoiding malformed C identifiers/declarations.
- When loading v0.1.21 waypoint projects, convert removed waypoint blocks to explanatory comments so projects still open without running unreliable estimated-pose navigation.
- Confirm the attached v0.1.18 build has no MAV selector on its normal distance-move block; MAV remains available in direct motor control and the newer independent-wheel distance block.

## v0.1.22-preview — local feature and interface refresh

- Remove the unreliable estimated-position waypoint experiment; retain the separate command-history return trace.
- Add Blockly variables and numeric reporters for elapsed timers and motor encoder counts.
- Add stable analog/digital waits, analog min/max/average sampling, and numeric console logging.
- Add an optional KISS Graphics keyboard-drive block using WASD/arrows, Space to brake, and Q to exit. It does not provide a browser-to-robot remote link.
- Add bounded timeouts and clearer browser/network diagnostics to Run on robot; bound the run-output stream handshake and report unknown robot state clearly.
- Refresh buttons, fields, panels, and spacing with a restrained slate/violet style.
- The v0.1.21 waypoint preview was withdrawn because encoder-only pose estimates drifted and produced inconsistent return paths.


## v0.1.20-preview - encoder return point and retrace

- Added blocks to mark a return point (resetting both drive encoders) and retrace recorded drive/pivot moves in reverse order.
- Retrace supports calibrated drive, wheel-speed drive, and pivot blocks, including movements executed inside loops and branches.
- Freeform motor and sensor-triggered drive commands invalidate the trace; return stops safely rather than guessing how to reverse them.

## v0.1.19-preview — independent wheel speed distance block

- Added a separate forward/reverse distance block with individual left and right MAV speed percentages.
- The new block tracks each wheel’s encoder target independently and the map preview models the resulting differential drive path.

## v0.1.18-preview — restore 1.12 encoder movement

- Drive and pivot blocks use the v0.1.12 closed-loop encoder controller again: percentage-power motor commands, per-wheel encoder targets, and the original gradual speed ramp.
- Removed the misleading per-block movement method dropdown. Existing saved drive and pivot blocks discard the old method field safely.
- Direct single-motor position blocks still use `mrp()` / `mtp()` as selected.


## v0.1.17-preview — sensor logic and convenience blocks

- Add analog/digital value reporters and Scratch-style comparison, AND/OR/NOT, arithmetic, modulo, division, and power operators.
- Add generic if, while, do-while, repeat-until, and wait-until blocks with finite timeout handling; sensor repeat-until blocks now run their body at least once.
- Add automatic drive-until analog-threshold and digital-switch blocks that stop the drive motors and fail safely on timeout.
- Add a camera wait block that updates frames until a configured channel detects an object or times out.
- Mark live sensor/control flows as hardware-only in preview; analog thresholds remain raw readings until the specific sensor is calibrated.


## v0.1.16-preview — newest build: generated C string fix

- Escape timeout diagnostic newlines correctly in the generated C source so timeout `printf` statements remain on one valid C line.

## v0.1.15-preview — servo speed control

- Base this build on the v0.1.13 encoder motion and direction-toggle code.
- Add 1–100% speed fields to arm, claw, and custom servo blocks. Generated C moves servo targets in paced increments, and the preview duration follows the selected percentage.
- Keep existing saved projects compatible; older servo blocks default to 70% when loaded.

## v0.1.14-preview — optional gyro-corrected pivots

- Add optional Wombat gyro feedback for pivots to stop at measured rotation instead of relying only on wheel-distance estimates.
- Add selectable X/Y/Z gyro axes, estimate sensor bias while motors are stopped at program startup, and report measured pivot angles or timeout diagnostics.
- Keep gyro correction off by default; MRP and PWM methods remain selectable.

## v0.1.13-preview — restore position-controlled motion mode

- Make the automatic drive and pivot method dropdowns select distinct generated C controllers: MRP relative-position control or PWM encoder feedback.
- Use calibrated ticks-per-second commands for MRP and wait for both wheel targets before braking, avoiding premature one-wheel stops that can add unintended yaw.
- Print target and encoder progress when either controller times out.

## v0.1.12-preview — refresh corrected block labels

- Force Blockly to redraw drive and pivot fields after their direction options are remapped, so the visible block label updates immediately as well as the dropdown selection.

## v0.1.11-preview — direction correction and robot API diagnostics

- Add saved, off-by-default setup toggles for reversed pivot and forward/backward response.
- Update existing block labels and dropdown choices without changing their stored direction values or generated motor commands.
- Apply the correction to the map preview so it matches the displayed direction.
- Include API URL, elapsed time, HTTP status/response diagnostics and a read-only **Check API route** button for `/get-users`.

## v0.1.10-preview — robot API connection diagnostics

- Show the exact API URL, elapsed time, and HTTP status or response body when connection fails.
- Add a read-only **Check API route** button that opens `/get-users` directly, separating endpoint reachability from browser cross-origin failures.

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
