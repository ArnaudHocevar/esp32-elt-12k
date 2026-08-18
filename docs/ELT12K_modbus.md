# Documentation for Modbus protocol

| Address | Name                     | Type       | Size     | Scale Factor     | Unit | Description                                                                              |
|---------|--------------------------|------------|----------|------------------|------|------------------------------------------------------------------------------------------|
| 40004   | Device manufacturer      | ansi       | 16 bytes |                  |      | Device manufacturer                                                                      |
| 40020   | Device model             | ansi       | 16 bytes |                  |      | Device model                                                                             |
| 40036   |                          | ansi       | 8 bytes  |                  |      | Skip                                                                                     |
| 40044   | Software version         | ansi       | 8 bytes  |                  |      | Device SW version                                                                        |
| 40052   | Serial number            | ansi       | 8 bytes  |                  |      | Device S/N                                                                               |
| 40073   | WHRtg                    | uint16     | 1 byte   | WHRtg_SF(*0.01)  | kwH  | Energy Capacity                                                                          |
| 40074   | Max Charge Rate          | uint16     | 1 byte   | W_SF(*1)         | W    | Maximum rate of energy transfer into the storage device in DC watts.                     |
| 40075   | Max Discharge Rate       | uint16     | 1 byte   | W_SF(*1)         | W    | Maximum rate of energy transfer out of the storage device in DC watts.                   |
| 40077   | SoCMax                   | uint16     | 1 byte   | SoC_SF(*0.1)     | %    | Manufacturer maximum state of charge, expressed as a percentage.                         |
| 40078   | SoCMin                   | uint16     | 1 byte   | SoC_SF(*0.1)     | %    | Manufacturer minimum state of charge, expressed as a percentage.                         |
| 40079   | SoCRsvMax                | uint16     | 1 byte   | SoC_SF(*0.1)     | %    | Setpoint for maximum reserve for storage as a percentage of the nominal maximum storage. |
| 40080   | SoCRsvMin                | uint16     | 1 byte   | SoC_SF(*0.1)     | %    | Setpoint for minimum reserve for storage as a percentage of the nominal maximum storage. |
| 40081   | SoC                      | uint16     | 1 byte   | SoC_SF(*0.1)     | %    | State of charge, expressed as a percentage.                                              |
| 40083   | SoH                      | uint16     | 1 byte   | SoH_SF(*1)       | %    | Percentage of battery life remaining.                                                    |
| 40086   | Charge Status            | enum16     | 1 byte   |                  | enum | Charge status of storage device.                                                         |
| 40096   | Battery Event 1 Bitfield | bitfield32 | 2 bytes  |                  |      | Alarms and warnings. Bit flags.                                                          |
| 40104   | DC_Bus_Voltage(Battery)  | uint16     | 1 byte   | V_SF(*0.1)       | V    | DC Bus Voltage                                                                           |
| 40114   | DC_current               | int16      | 1 byte   | A_SF(*0.1)       | A    | Total DC current flowing to/from the battery bank.(+:in. -out)                           |
| 40117   | Battery Power            | int16      | 1 byte   | W_SF(*1)         | W    | Total power flowing to/from the battery bank.(+:discharge. -:charge)                     |
| 40134   | Battery Voltage          | uint16     | 1 byte   | V_SF(*0.1)       | V    | Total power flowing to/from the battery bank.(+:discharge. -:charge)                     |
| 40135   | Active_Power_A           | int16      | 1 byte   | W_SF(*1)         | W    | A-phase active power (+:discharge. -:charge)                                             |
| 40136   | Active_Power_B           | int16      | 1 byte   | W_SF(*1)         | W    | B-phase active power (+:discharge. -:charge)                                             |
| 40137   | Active_Power_C           | int16      | 1 byte   | W_SF(*1)         | W    | C-phase active power (+:discharge. -:charge)                                             |
| 40138   | Reactive_Power_A         | int16      | 1 byte   | W_SF(*1)         | Var  | A-phase reactive power                                                                   |
| 40139   | Reactive_Power_B         | int16      | 1 byte   | W_SF(*1)         | Var  | B-phase reactive power                                                                   |
| 40140   | Reactive_Power_C         | int16      | 1 byte   | W_SF(*1)         | Var  | C-phase reactive power                                                                   |
| 40146   | Daily_Charge_Energy      | uint16     | 1 byte   | Energy_SF(*0.01) | kWh  |                                                                                          |
| 40147   | Daily_Discharge_Energy   | uint16     | 1 byte   | Energy_SF(*0.01) | kWh  |                                                                                          |
| 40148   | Charge_Energy            | uint32     | 2 bytes  | Energy_SF(*0.01) | kWh  |                                                                                          |
| 40150   | Discharge_Energy         | uint32     | 2 bytes  | Energy_SF(*0.01) | kWh  |                                                                                          |
| 40153   | Grid_Power_A             | int16      | 1 byte   | W_SF(*1)         | W    | A-phase grid side power (+:Grid discharge. -:Grid countercurrent)                        |
| 40154   | Grid_Power_B             | int16      | 1 byte   | W_SF(*1)         | W    | B-phase grid side power (+:Grid discharge. -:Grid countercurrent)                        |
| 40155   | Grid_Power_C             | int16      | 1 byte   | W_SF(*1)         | W    | C-phase grid side power (+:Grid discharge. -:Grid countercurrent)                        |
| 40156   | Battery_temp             | int16      | 1 byte   | Temp_SF(*0.1)    | °C   |                                                                                          |
| 40157   | PCS_temp                 | int16      | 1 byte   | Temp_SF(*0.1)    | °C   |                                                                                          |

