# Contributing

This is a single-page Blockly/C-generation app with no install step. Before opening a pull request:

1. Keep `index.html` and the local `blockly/` assets together.
2. For a new block, update its Blockly definition, toolbox, action-tree parsing, C generation, and documentation together.
3. Respect Wombat port ranges and the exact KIPR API behavior. Do not infer what hardware is installed from the build-guide examples.
4. Label actions the route preview cannot model; do not show sensor-dependent code as a deterministic path.
5. Keep `docs/COMMANDS_AND_HARDWARE.md` and `CHANGELOG.md` current.
6. Manually inspect the generated C for the changed block and validate against the KISS IDE / simulator before relying on it on a physical robot. Begin physical motion checks with wheels lifted and low speed.

Project issues and pull requests are welcome. A project-level reuse license has not yet been selected; see `README.md`.
