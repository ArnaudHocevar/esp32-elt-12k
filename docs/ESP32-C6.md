# ESP32-S6 specificities

This page describes the pinout when using a breadboard to connect a MAX485S to an ESP32-C6

Note that there is no opto-isolation for *most* of the cheap MAX485S, therefore damage could occur to either the PCS or the ESP32 board.

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

Pinout:
- GPIO22: TXD1
- GPIO23: RXD1
- GPIO21: EN