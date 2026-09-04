# GitHub Copilot Review Instructions

## CI Workflow Alignment & Testing Strategy
You are reviewing code that already undergoes Continuous Integration (CI) testing workflows. Follow these rules to avoid redundant noise and properly consider workflow outcomes:

- **Do Not Duplicate Linter/CI Outputs:** Our CI workflow automatically runs ESPHome config for YAML linting and clang-tiny for C++ linting. Do not comment on syntax formatting, missing type hints, or basic code style issues that a linter will catch.
- **Enforce Test Completeness:** If a pull request modifies source logic (`src/**/*.cpp`, `include/**/*.h`, etc.), verify that a corresponding test file under `tests/` has also been added or modified. Flag it as an issue if logic changes without accompanying test changes.
- **Triage Critical Test Failure Risks:** Focus your review on logic flaws that standard CI workflows fail to catch. Prioritize flagging edge cases, boundary conditions, race conditions in asynchronous actions, and mock pollution in unit tests.
- **Embedded Devices Risks:** The targets for deployment are small microcontrollers that have limited CPU power and memory. Flag any behaviour that could lead to memory leaks, CPU spikes or in general resources waste.
- **Target Deployment:** The deployment targets are supported from a web installer, each new target should be references in `docs/manifest.json`.