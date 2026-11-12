# Sensor and control blocks

Wombat Blocks reads analog ports 0–5 as raw 12-bit values (0–4095), digital ports 0–9 as switch states (0 or 1), and camera channels as configured object detections. Analog values are not distances in centimeters unless the installed sensor has been calibrated.

## Approach until a sensor threshold

1. Use **log analog** while the robot is safely stationary to observe the range sensor at several distances.
2. Select **drive until analog**, set direction, port, comparator, threshold, speed, and a conservative timeout.
3. The block checks the reading, drives both wheel motors while the threshold is unmet, then brakes both motors. If the threshold is never reached, it stops and exits through cleanup.

Use **drive until digital** for a bumper or limit switch. Choose the state that means contact for the wiring in use. Both blocks have a timeout; tune speed and stopping threshold to account for sensor delay and robot momentum.

## Conditions and loops

- **if** checks a condition once and chooses a branch.
- **while** checks before each pass and may run zero times.
- **do while** runs once, then continues while its condition stays true.
- **repeat until** runs at least once, then checks whether to stop repeating.
- **wait until** samples a condition without running a body.

These loops include a maximum time. If that limit expires before the condition is satisfied, generated code reports failure and reaches motor cleanup. A blocking move inside a loop finishes before the next condition check, so use direct motor commands for small reactive steps or a dedicated drive-until block for threshold stopping.

Use the **read analog** and **read digital** reporters with the **Operators** blocks to make conditions. Comparisons, AND/OR/NOT, arithmetic, modulo, division, and integer powers are available. Division by zero returns zero; modulo by zero returns zero; powers use an integer exponent limited to −32 through 32.

## Camera waits

**Wait until camera sees channel** opens the camera if needed, updates frames, and exits when the configured channel detects an object or the timeout expires. Configure the color channel in the Wombat KISS settings first.

## Line following

There is no one-size-fits-all line-follow block because reflectance sensor polarity, thresholds, and mounting vary. Compose one with **while**, two **read analog** reporters, **if/else**, comparisons, and motor commands. Use the analog logger to calibrate each sensor over the actual line and floor, and keep a timeout or other stop condition in the loop.
