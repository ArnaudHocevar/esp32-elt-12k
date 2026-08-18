# esp32-elt-12k
Deploy an ESP32 to control the ELT-12K PCS through RS485.

## Notes specific to ELT-12K (and where it has limited functionality for full smart-home integration)
By default, Modbus is not enabled, and a custom firmware must be asked to APSystem. Modbus is enabled through the EMA Manager app, by connecting to the APStorage bluetooth module.

*DO NOT USE THE RS485/BATT PORT*, the system will go in fault if it cannot communicate with the batteries. This might only work with non-APSystem batteries, but I haven't tested it. Meanwhile, connect to the RS485 port (usually brown/striped brown pair on an RJ45 cable)

Once Modbus is enabled, the system will no longer use the internal modes for self-consumption for example, and these modes must be replicated through this deployment. I only need self-consumption, but probably other custom modes should be rather easy to implement.

Deactivating the Modbus mode will also re-enable the internal modes directly, so it is generally safe to play around.

## Supported ESP32 hardware
Tested with ESP32-C6-WROOM-1 with MAX485S

As the ESP32 uses 3.3v and the ELT-12K uses 5V TTL a voltage divider is required to safely connect the two. The 5V can be taken from the 5V of the ESP32 or a common bus, depending on how it is being powered.

       ESP32                                        MAX485
    +---------+                                  +---------+
    |         |                                  |         |
    |  GPIO22 |--------------------------------->| DI      | (Driver Input)
    |   (TX)  |                                  |         |
    |         |                                  |         |
    |  GPIO21 |-----------------------------+--->| DE      | (Driver Enable)
    |  (FLOW) |                             |    |         |
    |         |                             +--->| /RE     | (Receiver Enable)
    |         |                                  |         |
    |  GPIO23 |<-----------+                     |         |
    |   (RX)  |            |                     |         |
    |         |        [R1: 1kΩ]                 |         |
    |         |            |                     |         |
    |         |            +---------------------| RO      | (Receiver Output)
    |         |            |                     |         |
    |   GND   |----+---[R2: 2kΩ]                 |   GND   |
    +---------+    |                             +---------+
                   |                                  |
    ---------------+----------------------------------+----- COMMON GND


It is recommended to use a better system than the inexpensive MAX485S, some with optocouplers would be safer in case of surge on one side or the other.
Also check if your MAX485 already has a 120 Ohm resistor installed (usually labeled R7). If not, it should be added between A and B to avoid signal reflection.

## Development roadmap
- [X] Collect data from Modbus
- [ ] Implement cleaner handling of enumeration and bitmasks
- [ ] Support for dynamic exponent detection
- [ ] Multi-speed polling
- [ ] Support for custom modes (self-consumption, peak-shaving, etc.)
- [ ] Simple reporting UI
- [ ] MQTT support
- [ ] WebUI configuration (custom modes, MQTT, polling frequency, etc)
