# Contributing

This is a single-page Blockly/C-generation app with no install step. Before opening a pull request:

1. Keep `index.html` and the local `blockly/` assets together.
2. For a new block, update its Blockly definition, toolbox, action-tree parsing, C generation, and documentation together.
3. Respect Wombat port ranges and the exact KIPR API behavior. Do not infer what hardware is installed from the build-guide examples.
4. Label actions the route preview cannot model; do not show sensor-dependent code as a deterministic path.
5. Keep `docs/COMMANDS_AND_HARDWARE.md` and `CHANGELOG.md` current.
6. For every build, update `docs/AI_BLOCK_PROGRAMMING_GUIDE.md` so it covers all current blocks, their behavior, generated C patterns, placement and input syntax, setup, compatibility, and practical usage. Keep the guide at least 1,000 words; expand it as needed for completeness.
7. Embed the current guide in `index.html` so the AI prompt builder works offline, and include the standalone Markdown guide in the build package. Update the guide whenever blocks, generated code, or project behavior changes.
8. Manually inspect generated C for changed blocks and validate against the KISS IDE / simulator before relying on it on a physical robot. Begin physical motion checks with wheels lifted and low speed.

Project issues and pull requests are welcome. A project-level reuse license has not yet been selected; see `README.md`.
