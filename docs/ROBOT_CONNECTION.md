# Run on a Wombat over Wi-Fi

Wombat Blocks can use the Wombat's Voldigate HTTP service to send generated C to an existing KISS C project, compile it, start it, stream its console output, and request a stop. KIPR's Wombat OS starts Voldigate with `node express.js`; its server defaults to port `8888`.

## One-time setup

1. Connect the Chromebook/computer to the Wombat's own Wi-Fi network.
2. Open the Wombat KISS IDE once and create a dedicated **C** project, for example `Wombat Blocks`. This keeps its `src/main.c` separate from other robot programs.
3. Open the extracted Wombat Blocks folder in a browser. Keep the `blockly/` directory beside the HTML file.

## Run a program

1. In Wombat Blocks, select **Run on robot**.
2. Enter the Wombat's Wi-Fi IP address from its controller screen and select **Connect**.
3. Select the KISS user and the dedicated C project.
4. Select **Compile & run** and confirm the dialog. The app replaces that project's `src/main.c`, compiles it on the Wombat, and starts the resulting program.
5. Watch the output in the dialog. Select **Stop robot** to call the Wombat stop route.

The IDE can still be opened separately from the dialog. The interface uses the official project/user listing routes rather than creating accounts or projects behind the user's back.

## API mapping

The client uses routes in KIPR's Voldigate server source:

- `GET /get-users` and `GET /get-projects?userName=…` populate the selectors.
- `POST /save-file-content` writes generated code to the selected project's `src/main.c`.
- `POST /compile-code` compiles the C project and returns compile diagnostics.
- `GET /run-code?userName=…&projectName=…&activeLanguage=c` starts the program and streams console output as server-sent events.
- `POST /stop-code` stops the current process and requests motors off.

The source has a permissive CORS middleware for these later routes, so a browser-hosted editor can call them cross-origin. Project creation/upload routes are ordered before that middleware; Wombat Blocks therefore requires a project created in KISS and does not try to create one over the network.

## Browser and firmware limits

- The computer must remain connected to the robot's Wi-Fi. Use the IP shown by that Wombat; port `8888` is the documented default.
- Browser local-network permission and CORS behavior depend on the browser and how the HTML is opened. If a browser blocks access from the extracted `file://` page, open the app from a local HTTP server on the same computer. A public HTTPS host may block HTTP requests to the robot as mixed content.
- API behavior can vary by firmware/Voldigate version. Current KIPR `wombat-os` config starts the `voldigate-slim-wombat` service. The source reviewed includes the routes above, but older images may differ.
- A successful compile is not a physical validation. Confirm motor/servo ports and calibration, keep the robot supervised, and test with wheels lifted before driving. Sensor wiring and generated motion assumptions still matter.
- If the browser loses its event stream, the robot may continue running. The app will state that robot status is unknown; reconnect or use the controller's own stop controls. Do not assume a lost browser connection stopped the motors.
- No Wombat was connected during implementation, so the direct-run workflow still needs a live hardware check on the target robot and browser.

## KIPR references

- [Wombat OS](https://github.com/kipr/wombat-os) describes the OS build and lists Voldigate as a required package.
- [Wombat OS Voldigate service definition](https://github.com/kipr/wombat-os/blob/main/configFiles/voldigate.service) starts the Wombat IDE server with `node express.js`.
- [Voldigate server source](https://github.com/kipr/voldigate/blob/main/express.js) defines the project, compile, run, stop, CORS, and output-stream routes used here.
- [KIPR Wombat hardware/software page](https://www.kipr.org/kipr/hardware-software) describes the IDE hosted on the Wombat and accessed over Wi-Fi.
- [KIPR's Writing Your First Program guide](https://www.kipr.org/wp-content/uploads/2025/08/Writing_Your_First_Program.pdf) explains the controller IP and default IDE port `8888`.

