# Sensor and control blocks

Wombat Blocks reads analog ports 0–5 as raw 12-bit values (0–4095), digital ports 0–9 as switch states (0 or 1), and camera channels as configured object detections. Analog values are not distances in centimeters unless the installed sensor has been calibrated.

## Approach until a sensor threshold

1. Use **log analog** while the robot is safely stationary to observe the range sensor at several distances.
2. Select **drive until analog**, set direction, port, comparator, threshold, speed, and a conservative timeout.
3. The block checks the reading, drives both wheel motors while the threshold is unmet, then brakes both motors. If the threshold is never reached, it stops and exits through cleanup.

Use **drive until digital** for a bumper or limit switch. Choose the state that means contact for the wiring in use. Both blocks have a timeout; tune speed and stopping threshold to account for sensor delay and robot momentum.

## Analog profiles for noisy or nonzero background readings

Configure a profile once per analog port in **Robot setup**. The profile is off by default for compatibility with older projects and sensors whose thresholds are already calibrated. When enabled, all existing analog decision paths use the same median-of-three sampling and per-port rules: generic if/while/do-while/repeat-until/wait-until conditions, analog if/wait/stable-wait, drive-until-analog, repeat-until-analog, generic sensor reporters inside conditions, and start-light waits.

Set the **empty band** to the range observed with no target present. The signal direction chooses whether a target must rise above that range, fall below it, or either; the margin determines how far outside the empty band a reading must be. A stable group of samples in the empty band arms that profile. An analog comparison is treated as a target only after the configured number of median samples consistently leaves the empty band in the selected direction. Until a clear reading has first been confirmed, sensor conditions fail closed so a ghost value cannot trigger a movement. After arming, an empty-band reading makes a positive target comparison false; a negated target condition can then represent “no target detected.”

Use **sample analog** and **log analog** to record raw values while the robot is stationary, then configure the port’s empty band and direction from those measurements. The profile does not convert analog values to centimeters. If empty and target readings overlap, software cannot reliably distinguish them; adjust the sensor angle or distance, or disable the filter for an input where that signal range has a different meaning. The digital/button paths use a short configurable debounce, camera detection requires consecutive confirmed frames, and raw analog/digital diagnostics remain unfiltered.

## Physical controller button

The **physical controller button pressed?** Boolean block reads the small built-in button on the right side of the Wombat controller. Generated native C calls `push_button()`, which returns 1 while pressed and 0 otherwise. Use it directly inside an if, while, or wait-until condition, for example to start an action only after a deliberate button press. The web simulator has no physical controller input, so its generated WebAssembly helper returns false. KIPR documents the button in its [Wombat function reference](https://www.kipr.org/wp-content/uploads/2025%20Botball/Wombat-Function-Reference-Guide.pdf) and [controller-button workshop slides](https://www.kipr.org/wp-content/uploads/2024%20Botball/2024%20Botball%20Complete%20Resource%20Slides%20v2.1.pdf).

## Conditions and loops

- **if** checks a condition once and runs its body only when true.
- **else if** blocks stack directly after a simple if or another else-if and emit a C `if / else if` chain. The older fixed **if/else** block remains available and existing saved blocks retain their behavior.
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
