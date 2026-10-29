# Command and hardware reference

## Blocks currently in the editor

### Motion

- Drive forward or backward by encoder-measured centimeters using PWM encoder feedback, wheel synchronization, a gradual power ramp, and per-wheel stopping at the requested count.
- Pivot left/right by degrees, with separate calibration scales.
- Set a motor directly by MAV or PWM; command relative or absolute encoder positions; wait for a position move; reset an encoder.
- Stop using `ao()` (all motors off/coast), `freeze(port)` (active brake), or `off(port)` (release a motor).

### Servos

- Enable or disable servo power.
- Move the configured arm and claw, or set one of the four servo ports to a position and wait.
- Servo commands use positions from 0 to 2047 in the API. The actual usable travel depends on the particular servo and linkage; calibrate slowly to avoid binding or stalling.

### Control and sensors

- Wait, repeat a fixed count, or repeat until a digital state or analog threshold is reached.
- Digital/analog conditions have then/else stacks. Sensor loops include a maximum duration and route failures to cleanup instead of running forever.
- Wait for digital input, wait for an analog threshold, or wait for the start light.
- Read sensor values to the robot program console for threshold calibration.
- Open/update/close the camera and branch if a configured camera channel detects an object.

### Output

- Beep, print a message to the program console, or set a digital port to high/low output.

## Controller API ranges

| Wombat resource | Ports | Command/value range |
|---|---:|---|
| DC motors | 0–3 | `mav`: -1500 to 1500 ticks/s; encoder accuracy may decrease beyond roughly ±1000. `motor`: -100 to 100 percent. |
| Servos | 0–3 | Position 0–2047; mechanical range is usually smaller. |
| Analog sensors | 0–5 | `analog12`: 0–4095. |
| Digital sensors | 0–9 | `digital`: 0 or 1. |

## Sensor examples in the 2020 build guide

The build guide shows separate example builds for the servo arm/claw, lever sensor, large touch sensor, rangefinder, IR sensor, and camera. These are examples, not confirmation that every part is installed on one robot. The port used by each sensor depends on the actual wiring.

KIPR's sensor documentation describes common uses:

- **Tophat/reflectance:** detect floor brightness and tape; commonly used for line following.
- **ET/rangefinder:** analog values generally increase as an object gets closer.
- **Light sensor:** detect the Botball starting light; the `wait_for_light(port)` call is available.
- **Linear slide:** analog reading increases as the slide is pushed.
- **Lever and touch sensors:** digital 0/1 switch inputs.
- **Camera:** camera processing uses configured color channels. Configure those in KISS before using the camera detection blocks.

An “IR sensor” label alone does not determine the output type or calibration; inspect the particular sensor and read its port before selecting an analog or digital block.

## Preview boundary

The 2D preview uses calibrated distance for straight moves and ideal pivots that turn by the requested angle exactly. Generated automatic-motion C uses PWM encoder feedback; left/right pivot scales adjust generated wheel targets. It cannot read physical sensors, camera frames, or arbitrary motor/encoder commands, so it cannot predict sensor branches or those motor paths. The app shows a preview notice when such blocks are present. Generated C runs on the robot and follows the real sensor values.

## Scratch categories and the robot mapping

Scratch's core palette includes Motion, Looks, Sound, Events, Control, Sensing, Operators, Variables, and My Blocks. Wombat Blocks carries over the hardware-relevant concepts as Motion, Servos, Sensors, Control, and Output. The start block represents the program-start event; beep and console output provide basic feedback. Sprite costumes, backdrops, keyboard/mouse input, clones, and broadcasts do not correspond directly to the physical Wombat. General-purpose variables, reusable procedures, and more typed math/logic are potential future additions once their C generation and simulation behavior are defined.
