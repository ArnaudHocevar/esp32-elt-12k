# Project Instructions and guidelines

This document provides context for AI models interacting with this project.
Adhering to these guidelines will promote consistency and content quality.

## Context
- **Primary goal:** This is an ESPHome project allowing interaction with an APSystems ELT-12K PCS over serial modbus. 
  It allows collecting metrics in a timely manner and replaces the PCS automatic modes that are deactivated when modbus 
  is activated. Metrics publication can be performed either through the Home Assistant ESPHome integration or MQTT 
  transmission. The module should not require an active Home Assistant connection to perform its tasks. 
- **Deployments:** The deployment targets can be multiple, for now it was tested with ESP32-C6 and ESP32-C3. 
  It is not meant for the older 8266 module that is not powerful enough.
- **Business Domain:** Solar power auto-consumption, Home Automation, Internet-of-Things (IoT).

## Code Conventions

The project uses YAML files for ESPHome configuration and basic functions, and C++ for advanced features.

### YAML conventions
- ESPHome configuration files should be included from `common/base_package.yaml` and should not nest more than one level
- YAML constants should go to `common/base_config.yaml`
- ESP32 devices specification go under `common/devices/`
- YAML linting is performed with esphome config and standard YAML practices

### C++ conventions
- C++20 is preferred and should align with the ESPHome conventions when available
- C++ tests should use doctest
- The Google C++ styling should be preferred
- C++ linting should be done through clang-tidy

## Architecture Constraints
- C++ files go under `include` for header files, `src` for source files and `tests` for test files. Packages as subfolders are permitted
- `common` folder will contain ESPHome YAML files
- `docs` holds documentation, manually written or generated
- a device build target should have a root-level YAML file and an entry under `common/devices/`
- Modbus registers must be documented in `docs/ELT12K_modbus.md`

## Device constraints
- The modbus calls should not range over more than 100 registers.
- The PCS has a slow CPU, modbus calls should not be performed too often if not needed. Calls required for operation 
  (power measurement or settings) need to be performed as quickly as possible, while not having a sub-second frequency. 

## Documentation and workflows
- GitHub workflows handle automated testing and listing (under `.github/workflows`) 
- a device build target should be covered by `esphome-lint.yml` and `deploy.yml` and configured in `docs/manifest.json` for browser installation
- all device targets should have documentation entries under `docs/`