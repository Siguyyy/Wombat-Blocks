# Wombat Blocks — AI Programming and Block Reference

This guide describes Wombat Blocks 2.0 and is intended to be attached to a programming request for an AI assistant. It explains how to translate an assignment into a Blockly program that the current app can import and generate as KIPR C.

## Instructions for an AI generating a program

First identify the robot behavior, the sensors and actuators it requires, the known wiring, thresholds, and any safety conditions. Do not assume a sensor, camera, claw, arm, motor port, sensor polarity, or calibration that the user has not specified. Ask a concise question if an unknown value changes the program materially. Otherwise, label the assumption in a Wombat Blocks comment and tell the user what to confirm.

Use only the block types and fields listed in this guide. Do not invent block names, field values, motor APIs, or unsupported KIPR functions. Prefer existing distance, turn, sensor, servo, and control blocks. Use a repeat-forever block for a behavior that should keep reacting; set its stop option to controller button when the robot should stop from the Wombat's physical side button. Keep every reactive loop bounded where possible, with a timeout or a clear physical stop. Do not use a blocking console-drive block as a general remote-control system.

Return a complete Blockly XML document that this build can import. The root must be `<xml xmlns="https://developers.google.com/blockly/xml">` exactly. Include one top-level wombat_start block, with the whole program connected through its next statement chain. Use the field names, values, statement inputs, value inputs, and block type IDs exactly as documented below. Use standard Blockly math_number, logic_compare, logic_operation, logic_negate, logic_boolean, math_arithmetic, math_modulo, variables_set, variables_get, and math_change blocks where applicable. Sensor reporter blocks are value reporters and belong in an input value, not as a top-level action. Statement blocks belong in statement inputs or the start block’s next chain. Keep all XML well-formed.

Create a downloadable plain-text `.txt` file containing the complete Blockly XML document and nothing else. Do not print the XML into the response or put it in a code block. The file must start with `<xml xmlns="https://developers.google.com/blockly/xml">`, have one top-level `wombat_start`, and end with the matching `</xml>`; do not include Markdown fences, notes, or comments outside the XML document. Name it clearly, such as `wombat_program.xml.txt`. Put assumptions, calibration values to confirm, sensor thresholds, and stop behavior in a short response note beside the file, not inside it. The user can paste the file's contents into Wombat Blocks’ Paste Blockly XML tab. If file creation or attachment is unavailable, say so and ask before falling back to inline XML; do not silently dump a long XML document into the chat.

Use the exact Blockly XML namespace given above; do not substitute a generic website URL. The requested `.txt` file should contain clean raw XML without a Markdown code fence. When a user manually pastes XML from an older response, the importer may tolerate an optional surrounding fence, but do not use that as the output format.

Use a wombat_comment block before each meaningful section when the assignment asks for comments. The comment block is a real program statement and generates a C comment. Give the user a short explanation of assumptions, calibration, sensor thresholds, and how the stop condition works outside the XML.

If the assignment asks for a visual block result instead of XML, explain that the app imports Blockly XML to recreate editable blocks. Generated C is a separate view and is not a safe substitute for workspace XML.

## Assignment fidelity, rubrics, and capability limits

Before writing XML, reason through the task in phases: identify the starting state, triggers, actions, success condition, failure/timeout behavior, and final safe state. Convert every request and scored requirement into a checklist, map each item to a block sequence, then audit the finished XML against that checklist. Preserve the order stated in the assignment; example code is evidence about hardware and technique, but do not copy an example's order when it conflicts with the requested behavior. Prefer the simplest sequence that meets the requirements. Remove waits that do not add settling time, duplicate stop commands, repeated sensor reads that a configured profile already performs, and nested loops that do not add a condition. Keep explicit phase comments when requested. Fill every numeric or dropdown input deliberately; never leave a repeat count, threshold, timeout, speed, or port blank.

### Default timing and speed policy

- Every timeout field must be at least **45 seconds** by default. Use a longer timeout when the expected search, travel, or task duration requires it; estimate generously and state why. Do not shorten a timeout simply to make a loop look tidy. A timeout must still stop the drive motors and take the program to a safe failure/cleanup path. For any movement that could hit an obstacle before timeout, add an independent encoder/travel limit or choose a safer bounded motion pattern; a long timeout is never a substitute for a safe travel limit.
- Every percentage-based drive, pivot, and servo speed must be **50% or higher**. Use **70%** as the normal choice for most actions. Use 50–69% only when the task needs more control, such as a sensor-based final approach; adjust the stop threshold to preserve a safe standoff rather than dropping below 50%. If safe behavior cannot be achieved under this minimum, disclose the conflict instead of generating a lower value.
- Direct MAV fields are ticks per second, not percentages. When using the conventional ±1000 ticks/s encoder-accuracy range documented here, use 500 ticks/s as the 50% floor and about 700 ticks/s as the usual 70% equivalent. Respect the app's configured motor limit and do not treat a raw MAV number as a percentage.

### Sensor-confirmed spin-and-stop pattern from the user's ET example

When a robot must rotate until its ET sensor sees an object, follow the confirmation structure in the user's earlier C example instead of stopping on the first raw reading:

1. Rotate continuously at a deliberate, steady MAV speed while sampling the analog port (roughly every 10–20 ms). Do not repeatedly start/stop the wheels for each sample.
2. Filter each measurement with a median of three readings. Require a confirmed empty/clear view before accepting a target, then require several consecutive target readings. The user's example uses four clear samples followed by four target samples; its code uses a clear range of 600–1350, target range of 1850–3600, and search speed of 500 ticks/s. Treat these as example settings, not universal ET calibration; reuse them only if the same sensor, port, mounting, and environment still produce those ranges.
3. The app's per-port analog profile provides median-of-three sampling, empty-band arming, target direction/margin, and confirmation samples. Since profile settings are outside the XML, state the exact Robot setup values needed. For the same measured setup, configure an empty band from the example's verified clear readings, target direction high, and four confirmation samples. Use a sensor comparison matching the measured target range; add an upper-bound comparison only when those readings are a meaningful limit.
4. Keep the rotation active while the filtered condition is checked. The reference exits its scan loop as soon as the target streak is confirmed, calls `ao()` immediately, waits 200 ms for the robot to settle, then reads the encoder counts. `ao()` coasts; the Blockly stop-drive block can instead freeze both wheels for a sharper stop. In either case, stop before any claw/arm action and use a short settle wait only before taking encoder measurements or retracing. Do not add an arbitrary one-second delay between detection and stopping. The existing continuous-drive, wait-until/stable-sensor, and stop-drive blocks can express this sequence.
5. Use a timeout of at least 45 seconds, or longer when the course/search requires it (the reference used 600 seconds). On timeout, stop both wheels, report/return failure when the available blocks permit, and do not continue as if a bottle had been found.

In the reference's approach stage, three close readings trigger a break, but the code then waits 500 ms before calling `ao()`. That leaves the motors commanded forward during the delay and can cause overshoot; do not copy this delay. Stop immediately when the three confirmed close readings occur. This pattern is a block-level equivalent, not a byte-for-byte reproduction of `analog_median`, `clear_streak`, and `detect_streak`. If exact custom C functions are required for a rubric, explain the separate function limitation above.

This build does **not** provide custom function/procedure definition and call blocks (Scratch “My Blocks”). The helper functions in generated C are part of the app's runtime support; they are not functions written by the student and do not satisfy a “use functions” rubric. If an assignment explicitly requires functions, say that this Blockly build cannot fulfill that rubric item. Do not pretend comments or repeated blocks count as functions. The user can either accept the best available Blockly version with that limitation stated, or use a separate hand-written C solution with named functions. Do not invent unsupported procedure XML.

Do not claim a robot was calibrated or tested. Ports and sensor profile settings live in Robot setup and are not carried by Blockly XML. State required setup values and unknowns outside the XML, based on the user's supplied wiring and measurements. Ask for sensor port or calibration readings when choosing them would change whether a robot moves or stops safely. A threshold is a raw analog count, not a distance.

For ET/range-sensor tasks with nonzero or ghost empty-scene readings:

1. Use measured empty and target readings to specify the analog profile for that port in Robot setup. Profiles cannot be encoded in the XML, so include the needed empty band, target direction, margin, and confirmation count in the notes. Do not use a raw threshold as a substitute for the profile when empty readings overlap the target threshold.
2. Make the search begin with a confirmed clear view before accepting a target, so an object-like background value at startup does not immediately end the search. The shared analog profile supports this clear-before-target behavior when enabled and configured from real readings.
3. Use a bounded search loop or wait with a timeout, then explicitly stop both drive motors on target and on failure. If the sensor may not be looking at the target yet, rotate slowly while checking it; do not start a blocking sensor wait while the motors are stopped.
4. Approach with the drive-until-sensor block at 50–70% speed and a threshold that leaves a physical standoff. Prefer about 70% unless the sensor approach needs more control; do not go below 50%. Stop both drive motors when it triggers. Explain that momentum and sensor placement affect the final gap.
5. Use the logger/sample blocks while stationary to collect readings if calibration data is missing. Never silently assume the ET sensor's port, polarity, empty range, or safe stopping value.

For a return-to-start task, place **mark return point and reset drive encoders** before the first chassis movement. Keep all subsequent drive and pivot actions on trace-supported blocks; if direct MAV is used, close each paired drive segment with an explicit stop action. Retrace is recorded motion replayed in reverse, not map localization: wheel slip, lifting the robot, collisions, or unrecorded motor actions can make it miss the start. Explain this limitation instead of promising a mathematically exact return.

For a multi-step pickup-and-return example, a sound block outline is: (1) initialize and open the claw; (2) comment and search while rotating until the filtered sensor confirms the bottle; (3) stop, then approach slowly until the calibrated standoff threshold; (4) close the claw and lift the arm; (5) retrace to the marked start if requested; (6) perform the requested number of full turns using an explicit repeat count; (7) wave the arm with a separate repeat count; (8) print the requested message and beep; (9) disable servos or stop safely as appropriate. Adjust the order to match the assignment exactly. “Print to console” and “beep” are not spoken speech; this build has no speech output block.

Before returning XML, mentally execute the program from the start through success and timeout paths. Check that each condition can become true, each loop makes progress, detection stops motion before the next action, and failure cannot fall through into pickup/return actions. Then audit the requirements: every assignment bullet has a block sequence; every loop has a stop/timeout strategy; every sensor decision uses the intended profile and a measured threshold; every servo starts from a sensible state; trace recording starts before motion; all repeat counts and values are filled; comments mark the requested sections; unsupported requirements are named plainly in the notes. If a requirement cannot be represented by current blocks, flag it rather than approximating it without disclosure.

## Program structure and placement rules

Start every runnable workspace with when program starts. Attach actions below its bottom connector, in execution order. A normal statement block has a previous connector at its top and a next connector at its bottom. A loop or conditional has a body connector; put the actions that belong to that branch inside its body. Value reporters and operators plug into rounded or diamond-shaped value inputs and cannot be stacked as actions.

The simple if block can be followed by one or more else if blocks in the same action stack. Comment blocks may sit between an if and an else if, or between else-if clauses; the generator keeps those comments with the chain and emits them immediately before the associated C branch. Do not put waits, movement, or other action blocks between clauses, because that ends the chain and leaves a standalone else-if. The fixed if/else block has its own then and else bodies and remains compatible with saved projects. The else if block cannot be used by itself.

While tests before each pass and may run zero times. Do while executes once before checking. Repeat until executes its body once before checking. Wait until only checks a condition. These conditional loops have a configured timeout; if it expires before the requested condition becomes true, generated C reports a timeout and exits through cleanup. Repeat N times runs its body the selected number of times. Repeat forever runs continuously until the configured controller button stop or an explicit stop-program block is reached. Empty forever loops are paced with a short delay.

A sensor condition should combine a reporter with a comparison. For example, place analog reading from port 0 in the left input of a greater-than comparison and a calibrated raw threshold in the right input. Connect that comparison to an if, while, do-while, repeat-until, or wait-until condition. Analog readings are raw values from 0 to 4095; they are not centimeters unless that specific sensor has been calibrated. Set analog empty-reading profiles per port in Robot setup when background readings can resemble a target. Profiles are disabled by default. Logs and samples remain raw.

## Block catalog and generated C behavior

The snippets below show the generated statement or expression pattern. The full generated C file also includes calibration constants, helper functions, timeout handling, declarations, motor cleanup, and main. Port IDs and numeric values are generated from the block fields and Robot setup. Exact whitespace and helper details may change between builds.

### Motion

| Block | How to place/use it | Generated C pattern |
|---|---|---|
| move direction for centimeters at speed | Stack as an action; choose forward/backward, distance, speed, encoder feedback. | motion(FWD or REV, left_ticks, right_ticks, speed); |
| independent wheel distance move | Action for forward/backward travel where left and right speed percentages differ. | motion_wheels(FWD or REV, left_ticks, right_ticks, left_mav, right_mav); |
| drive continuously direction at speed | Starts paired drive motors and returns immediately. Follow with a stop action or use it inside a stoppable loop. | mav(LM, LS * signed_speed); mav(RM, RS * signed_speed); |
| pivot direction degrees at speed | Action; encoder feedback turns by the requested degrees using configured track and turn calibration. | motion(LEFT or RIGHT, left_ticks, right_ticks, speed); |
| drive until analog condition | Action with direction, analog port, comparison, raw threshold, speed, timeout. Brakes both drive motors at threshold. | sensor_analog_condition(...) loop with mav(...); freeze(LM); freeze(RM); |
| drive until digital state | Like analog drive-until, but stops when a debounced digital port reaches on/off. | sensor_digital_condition(...) loop with mav(...); freeze(LM); freeze(RM); |
| set motor port | Direct action. Choose MAV ticks per second or motor percent. MAV on configured drive ports may be recorded while a return trace is active; PWM invalidates it. | mav(port, value); or motor(port, percent); |
| move motor by/to encoder ticks | Direct motor position action with MRP relative or MTP absolute mode. This is not a chassis distance block. | mrp(port, speed, ticks); or mtp(port, speed, ticks); |
| wait for motor to finish | Stack after a motor-position command. Wait is cooperatively interruptible in button-stop mode. | motor_wait_interruptible(port); |
| reset encoder | Sets the chosen motor encoder counter to zero. | cmpc(port); |
| stop drive | Select all motors off/coast, active freeze, or a single-port stop. A stop closes a recorded direct-MAV segment for retracing. | ao();, freeze(port);, or off(port); |
| console drive | Blocking KISS program-console input, one command per line. w/b drive, a/d pivot, s stop, r toggles and reports reverse gear, o/c operate claw, p/l operate arm, q exits. It is not browser keyboard control. | keyboard_drive(speed); |

The normal distance and pivot blocks use the build’s encoder-feedback controller. Setup motor ports, forward signs, wheel diameter, encoder ticks per revolution, track, speed, and turn scales for the robot. The map preview estimates a route but cannot reproduce live sensor, camera, or arbitrary motor behavior. Do not interpret preview coordinates as robot localization.

### Servos

| Block | Behavior | Generated C pattern |
|---|---|---|
| servos on/off | Powers servo outputs on or off. | enable_servos(); or disable_servos(); |
| arm up/down at speed | Uses configured arm port and positions. The percentage controls paced servo steps. | servo_wait(ARM_PORT, ARM_UP or ARM_DOWN, speed, ARM_WAIT_MS); |
| claw open/close at speed | Uses configured claw port and open/closed positions. | servo_wait(CLAW_PORT, CLAW_OPEN or CLAW_CLOSE, speed, CLAW_WAIT_MS); |
| set servo port to position at speed, wait | Explicit port, 0–2047 target, pace, and settle delay. Mechanical travel may be narrower than the API range. | servo_wait(port, position, speed, wait_ms); |

Servo percentage is a pacing setting for intermediate position commands. It is not a guarantee of a particular physical speed in degrees per second. Confirm the configured endpoints and linkage so the mechanism does not bind.

### Control and branching

| Block | Meaning and placement | Generated C pattern |
|---|---|---|
| wait seconds | Timed pause; split into short sleeps so button-stop can be polled. | wait_interruptible(milliseconds); |
| repeat N times | Put actions in do; executes a finite count. | for (int loop = 0; loop < N; ++loop) { ... } |
| repeat forever | Put repeated actions in do. Choose controller button or manual stop. | for (;;) { ... } |
| stop program safely | Exits the generated program through cleanup after stopping motors. | ao(); goto cleanup; |
| if / else if chain | Connect a Boolean condition; stack else if under an if or earlier else if. | if (condition) { ... } else if (condition) { ... } |
| if/else | Separate then and else bodies. | if (condition) { ... } else { ... } |
| while | Checks before the body, with timeout. | while (condition && seconds() < deadline) { ... } |
| do while | Runs once before testing; bounded by timeout. | do { ... } while (condition && seconds() < deadline); |
| repeat until condition | Runs body then stops when condition becomes true; bounded by timeout. | do { ... } while (seconds() < deadline); |
| wait until condition | Waits and samples until true or timeout. | while (!condition && seconds() < deadline) msleep(10); |
| start/reset timer 1–4 | Resets one of four elapsed-time timers. | wb_timer_reset(slot); |
| elapsed timer reporter | Number expression returning seconds since reset/start. | wb_timer_elapsed(slot); |

The controller-button stop is checked during generated encoder moves, paced servo movement, ordinary waits, sensor loops, and between statement actions. KISS console input and the native wait-for-light call can block and cannot be interrupted until they return. The WebAssembly simulator has no physical button; its button reporter returns false and its repeat-forever loop must be stopped with simulator controls. The stop-program block remains available for explicit exit.

### Sensors, diagnostics, and output

| Block | Meaning | Generated C pattern |
|---|---|---|
| analog reading from port | Raw 12-bit reading; within decision conditions the shared port profile is applied when enabled. | analog12(port); or sensor_analog_value(port); |
| digital state from port | Raw 0/1 input; conditions use configured debounce. | digital(port); or sensor_digital_value(port); |
| physical controller button pressed? | Wombat side button state; debounced when used as a condition. | push_button(); |
| wait for digital state / analog threshold | Waits for state/threshold until configured timeout. | wait_digital(...) or wait_analog(...); |
| wait for stable digital / analog condition | Require a condition to remain true for a hold duration before success. | wait_digital_stable(...) or wait_analog_stable(...); |
| wait for start light | Uses the light wait helper and selected analog port. It may call native blocking wait_for_light(). | wait_light_conditioned(port); |
| wait until camera sees channel | Opens/updates camera frames, requires confirmed detections, and stops on timeout. Configure channels in KISS first. | camera_open(); camera_update(); get_object_count(channel); |
| if camera sees channel | Branches into then/else based on consecutive confirmed frames. | camera_confirm_frames(channel, frames); if (seen) { ... } |
| repeat until digital/analog | Loops a statement body until the selected sensor condition or timeout. | sensor_*_condition(...) inside a bounded loop. |
| set digital port output | Changes a digital port to output and writes high/low. Do not drive a connected sensor input. | set_digital_output(port, 1); set_digital_value(port, value); |
| encoder ticks on motor port | Numeric expression; returns encoder count since last reset. | gmpc(port); |
| sample analog port N times | Prints raw count, minimum, maximum, and average. | analog12(port); printf(...); |
| print number to console | Prints a plugged-in numeric expression. | printf("number = %.3f\n", (double)(value)); |
| print analog/digital reading | Prints the raw hardware reading or input state. | printf("analog %d = %d\n", port, analog12(port)); |
| beep | Plays the Wombat beep. | beep(); |
| print text to console | Prints a fixed message in the KISS program console. | printf("message\n"); |
| comment | Adds a readable section marker to generated C. | /* explanatory text */ |

For a rangefinder or ET sensor, gather empty and target readings while the robot is stationary. Set the per-port empty band and target polarity from actual readings. Median sampling and confirmation reduce noise; they cannot disambiguate overlapping empty and target ranges. Camera simulation stubs deliberately report no detected objects.

### Variables and operators

Create variables with Blockly’s Variables category. Set variable emits assignment; change variable emits an addition. Number reporters and variables plug into value inputs. Operators include addition, subtraction, multiplication, division, modulo, integer power, comparisons, AND, OR, NOT, and Boolean true/false. Use parentheses through the Blockly tree rather than typing C syntax.

Generated numeric operations use C arithmetic. Division by zero and modulo by zero return zero through safe helpers. Power uses an integer exponent bounded to −32 through 32. Conditions combine analog/digital/button reporters with comparison blocks; sensor conditions are refreshed and conditioned before comparison. Raw diagnostic blocks remain raw.

## Editable block transfer format

This build’s AI workflow uses Blockly XML to recreate real workspace blocks. The XML must have a top-level xml node, include the Blockly namespace, and contain a wombat_start block connected to the full action chain. Nest control bodies under the DO, ELSE, or equivalent statement input; connect sensor/operator reporters under their value inputs. Use exact type IDs above. Deliver the XML as the contents of the requested `.txt` attachment, with no Markdown fences or prose in the file. The app validates pasted XML in a temporary workspace before asking permission to replace the current program. Pasting XML replaces the existing workspace after confirmation. It is not the same as pasting generated C.

## Compatibility and careful testing

Projects autosave in the current browser and can be exported as JSON. Importing Blockly XML replaces blocks but does not change Robot setup calibration. Existing block IDs and saved project fields should be retained when working from an older workspace; avoid deleting existing actions unless the assignment requires it.

Generated C targets the KIPR Wombat library and is intended for the KISS C IDE. The browser simulator can compile or run only its supported environment and cannot prove physical ports, camera detection, motor direction, friction, servo endpoints, or sensor calibration. The preview omits live hardware inputs. Review the full KIPR file, verify ports and thresholds, and test the physical robot cautiously under supervision. Clearly mark any unknown assumption rather than silently hardcoding a guess.

## Suggested output format for an AI response

1. A concise note describing the interpreted task, assumptions, robot-setup values to confirm, any unsupported rubric item, and the timeout/stop behavior.
2. One downloadable `.txt` attachment containing only the complete importable Blockly XML. Do not paste the XML into the chat or surround it with Markdown. If the AI cannot create an attachment, it should say so and ask before using inline XML as a fallback.
3. Do not claim the program was physically tested unless the user reports that test.



## Exact Blockly XML block IDs and connector names

The user-facing labels in the palette are descriptive; the XML type IDs and connector/field names below are the identifiers the importer expects. A name can be a visible field, a value input, or a statement input; use it only in the role specified by the block definition. Keep direction and mode values exactly as listed in the enum notes.

| XML type ID | Field and input names |
|---|---|
| wombat_start | none |
| wombat_drive | DIR, CM, SPEED |
| wombat_set_return_point | none |
| wombat_retrace_return | none |
| wombat_drive_wheels | DIR, CM, LEFT_SPEED, RIGHT_SPEED |
| wombat_drive_continuous | DIR, SPEED |
| wombat_turn | DIR, DEG, SPEED |
| wombat_motor | PORT, METHOD, VALUE |
| wombat_motor_position | PORT, MODE, SPEED, TICKS |
| wombat_motor_wait | PORT |
| wombat_encoder_reset | PORT |
| wombat_keyboard_drive | SPEED |
| wombat_stop | HOW, PORT |
| wombat_wait | SEC |
| wombat_repeat | TIMES, DO |
| wombat_forever | STOP, DO |
| wombat_program_stop | none |
| wombat_if | COND, DO, ELSE |
| wombat_if_simple | COND, DO |
| wombat_elseif | COND, DO |
| wombat_while | COND, TIMEOUT, DO |
| wombat_do_while | DO, COND, TIMEOUT |
| wombat_repeat_until_condition | COND, TIMEOUT, DO |
| wombat_wait_until | COND, TIMEOUT |
| wombat_servo_power | STATE |
| wombat_arm | ACTION, SPEED |
| wombat_claw | ACTION, SPEED |
| wombat_servo | PORT, POS, SPEED, WAIT |
| wombat_wait_digital | PORT, VALUE, TIMEOUT |
| wombat_wait_analog | PORT, OP, VALUE, TIMEOUT |
| wombat_read_analog | PORT |
| wombat_read_digital | PORT |
| wombat_push_button | none |
| wombat_timer_reset | TIMER |
| wombat_timer_elapsed | TIMER |
| wombat_wait_light | PORT |
| wombat_wait_camera | CHANNEL, TIMEOUT |
| wombat_if_digital | PORT, VALUE, DO, ELSE |
| wombat_if_analog | PORT, OP, VALUE, DO, ELSE |
| wombat_drive_until_analog | DIR, PORT, OP, VALUE, SPEED, TIMEOUT |
| wombat_drive_until_digital | DIR, PORT, VALUE, SPEED, TIMEOUT |
| wombat_repeat_until_digital | PORT, VALUE, TIMEOUT, DO |
| wombat_repeat_until_analog | PORT, OP, VALUE, TIMEOUT, DO |
| wombat_digital_output | PORT, VALUE |
| wombat_wait_analog_stable | PORT, OP, VALUE, STABLE, TIMEOUT |
| wombat_wait_digital_stable | PORT, VALUE, STABLE, TIMEOUT |
| wombat_read_encoder | PORT |
| wombat_sample_analog | PORT, COUNT, INTERVAL |
| wombat_log_number | VALUE |
| wombat_log_analog | PORT |
| wombat_log_digital | PORT |
| wombat_camera | ACTION |
| wombat_if_camera | CHANNEL, DO, ELSE |
| wombat_beep | none |
| wombat_print | TEXT |
| wombat_comment | TEXT |

Standard Blockly value blocks used by this build:

| XML type ID | Fields/inputs |
|---|---|
| math_number | NUM |
| logic_compare | OP; value inputs A, B |
| logic_operation | OP; value inputs A, B |
| logic_negate | value input BOOL |
| logic_boolean | BOOL |
| math_arithmetic | OP; value inputs A, B |
| math_modulo | value inputs DIVIDEND, DIVISOR |
| variables_set | VAR; value input VALUE |
| variables_get | VAR |
| math_change | VAR; value input DELTA |

Important dropdown values: drive direction is FORWARD or BACKWARD; pivot direction is LEFT or RIGHT; continuous drive is FWD, REV, LEFT, or RIGHT; motor output method is MAV or MOTOR; motor position mode is MRP or MTP; stop mode is AO, FREEZE_BOTH, FREEZE_LEFT, FREEZE_RIGHT, FREEZE_PORT, or OFF_PORT; forever stop mode is BUTTON or NEVER; servo states are ON/OFF; arm actions UP/DOWN; claw actions OPEN/CLOSE; analog comparisons GT/LT; digital state values are the strings 1 and 0. Timer slots are the strings 0 through 3. Blockly XML field values are strings even when the UI displays a number.

For correct nested XML, connect the start block's next chain with next/block; connect a value reporter in a value element whose name matches the target value input; connect an action body in a statement element whose name matches the target statement input. The start block is wombat_start and actions are stacked below it. Do not place sensor reporters or Boolean operators as top-level statements.
