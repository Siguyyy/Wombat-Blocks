# Connecting to a Wombat

## Current workflow

1. Connect the Chromebook/computer to the Wombat's Wi-Fi.
2. In Wombat Blocks, choose **Open KISS IDE**, enter the IP shown on the controller, and open the IDE. The documented default port is `8888`.
3. In Wombat Blocks, choose **View generated C** and copy or download the full C file.
4. In the KISS IDE tab, create/open a project, paste or import the C program, compile, and run it there.

The KISS IDE is hosted by the Wombat and is usable from a browser. The helper only opens `http://<robot-ip>:8888`; it does not send code or establish a robot-control connection.

## Direct Run button feasibility

A direct **Run on robot** workflow is feasible if the robot offers a supported interface for project/source upload, compile status, execution, stop, and output. Merely opening `http://<robot-ip>:8888` is not enough to transfer and execute the generated program. The KIPR material reviewed documents browser access to KISS but does not describe a stable external upload/build/run API.

A review of the KIPR material and public Voldigate IDE source confirmed how the browser IDE is reached and how the simulator executes programs, but did not identify a supported external robot-side API for source upload, compile, run, stop, or status. Voldigate is the simulator IDE, so its internal worker/compile mechanisms are not evidence of a Wombat firmware API. A direct-run implementation should wait for KIPR's documented interface or a known firmware endpoint. Browser-origin, local-network access, authentication, and firmware-version behavior would also need to be resolved.

References:
- [KIPR Wombat hardware/software page](https://www.kipr.org/kipr/hardware-software) describes the IDE hosted on the Wombat and accessed over Wi-Fi.
- [KIPR's Writing Your First Program guide](https://www.kipr.org/wp-content/uploads/2025/08/Writing_Your_First_Program.pdf) explains the controller IP and default IDE port `8888`.
- [KIPR Voldigate source](https://github.com/kipr/voldigate) describes the simulator IDE; it does not establish a robot web API.

## Safe robot bring-up

- Confirm motor ports/signs and servo ports on the actual robot.
- Lift the drive wheels for the first motor checks.
- Start at low speed and calibrate straight distance and each pivot direction separately.
- Move servos in small steps and record safe physical endpoints.
- Verify sensor port wiring and log raw analog/digital readings before tuning thresholds.
- Keep a reachable stop control and supervise the first field runs.
