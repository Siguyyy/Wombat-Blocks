# Connecting to a Wombat

## Current workflow

1. Generate and download `main.c` from Wombat Blocks.
2. Connect the Chromebook/computer to the Wombat's Wi-Fi.
3. Open the Wombat's KISS Web IDE at `http://<robot-ip>:8888` (use the IP shown on the controller), create/open a project, paste or import the C program, compile, and run it there.

The KISS IDE is hosted by the Wombat and is usable from a browser. This project currently generates C but does not communicate with the IDE, upload source, compile, start, stop, or stream robot status.

## Direct Run button feasibility

A direct **Run on robot** workflow is feasible if the robot offers a supported interface for project/source upload, compile status, execution, stop, and output. Merely opening `http://<robot-ip>:8888` is not enough to transfer and execute the generated program. The KIPR material reviewed documents browser access to KISS but does not describe a stable external upload/build/run API.

A safe implementation should first use a documented KISS API if KIPR provides one for the installed firmware. If no supported API exists, the alternatives are to coordinate with KIPR for an interface or build a small local companion service / firmware endpoint. That service would need to validate the robot address, report compile/run failures, allow an explicit stop, and avoid automatically running stale code. Browser-origin, network, authentication, and firmware-version behavior also need to be resolved before designing the connection.

## Safe robot bring-up

- Confirm motor ports/signs and servo ports on the actual robot.
- Lift the drive wheels for the first motor checks.
- Start at low speed and calibrate straight distance and each pivot direction separately.
- Move servos in small steps and record safe physical endpoints.
- Verify sensor port wiring and log raw analog/digital readings before tuning thresholds.
- Keep a reachable stop control and supervise the first field runs.
