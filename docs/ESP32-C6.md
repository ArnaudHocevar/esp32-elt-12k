# ESP32-C6 RS485 interface notes

This page describes two common RS485 transceiver module wiring options for an ESP32-C6.

## MAX485 breakout module (DE/RE controlled)
Many low-cost MAX485 breakout modules are not galvanically isolated. In that case, wiring or surge faults on the RS485 side can propagate to the ESP32 and to the PCS interface.

On these modules, the UART-side logic level depends on the module implementation and supply rail. Because ESP32 GPIO is 3.3 V logic, ensure the RX path seen by the ESP32 is 3.3 V compatible. DE and /RE are connected together and controlled from one ESP32 GPIO to switch between transmit and receive.

```
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
```

Suggested ESP32 UART mapping:
- GPIO22: UART TXD
- GPIO23: UART RXD
- GPIO21: Direction control (DE + /RE)

## Waveshare opto-isolated RS485 module (auto-direction)

The Waveshare opto-isolated module is wired differently:
- DE/RE are handled internally, so no external direction-control GPIO is required.
- Its MCU-side UART interface is intended for direct controller connection on supported variants; verify the exact board revision, pin labels, and supply requirements before wiring.
- The module power rail can typically be either **3.3 V** or **5 V** (board-dependent), which simplifies integration with ESP32 designs.
- UART wiring is crossed on this board: **ESP32 TX connects to module RX**, and **ESP32 RX connects to module TX**.

```
       ESP32                                Waveshare RS485 (opto-isolated)
    +---------+                             +------------------------------+
    |         |                             |                              |
    |  GPIO22 |---------------------------->| RXD                          |
    |   (TX)  |                             |                              |
    |         |                             |                              |
    |  GPIO23 |<----------------------------| TXD                          |
    |   (RX)  |                             |                              |
    |         |                             |                              |
    |   3V3   |---------------------------->| VCC (or 5V per board spec)   |
    |   GND   |---------------------------->| GND                          |
    +---------+                             +------------------------------+
```

## Cabling recommendation (applies to both module types)

These recommendations apply to both MAX485 and Waveshare modules:
- Always use a twisted pair for RS485 **A/B**.
- Keep ESP32-to-transceiver UART jumpers short. If they are longer than a few centimeters, route them together (twisting helps in noisy environments) to reduce coupling and edge ringing.