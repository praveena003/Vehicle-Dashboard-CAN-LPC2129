# Vehicle-Dashboard-CAN-LPC2129
Vehicle Dashboard system using LPC2129 microcontroller and CAN (Controller Area Network) protocol for real-time communication of vehicle parameters such as temperature and fuel level.
# Vehicle Dashboard Using Automotive Industry Standard CAN Communication Protocol

## Overview
This project implements a Vehicle Dashboard System using the ARM7 LPC2129 microcontroller and CAN (Controller Area Network) protocol for real-time monitoring of vehicle parameters such as temperature and fuel level. The system demonstrates reliable multi-node communication used in automotive applications.

## Problem Statement
Traditional vehicle systems use complex point-to-point wiring and lack efficient real-time monitoring of critical parameters, resulting in increased wiring complexity and reduced reliability.

## Solution
Developed a CAN-based vehicle dashboard system that enables reliable communication between multiple nodes for real-time temperature and fuel level monitoring with appropriate warning indications.

## Features
- Real-time Temperature Monitoring
- Fuel Level Monitoring
- CAN-based Multi-node Communication
- Motor Control for High Temperature Conditions
- Low Fuel Warning Indication using LED
- LCD Display for Vehicle Parameter Visualization
- SPI-based ADC Interfacing using MCP3204

## Hardware Components Used
- LPC2129 ARM7 Microcontroller
- MCP3204 12-bit External ADC
- LM35 Temperature Sensor
- CAN Transceiver
- 16x2 LCD Display
- Potentiometer (Fuel Level Simulation)
- DC Motor
- LEDs
- Push Buttons

## Software Tools Used
- Embedded C
- Keil µVision IDE
- Flash Magic

## Project Architecture

### Dashboard Node (Main Node)
- Sends CAN remote requests for temperature and fuel data.
- Displays received information on LCD.
- Controls motor and warning LED based on threshold conditions.

### Temperature Node (Receiver 1)
- Reads LM35 temperature sensor values using MCP3204 ADC through SPI.
- Converts analog data into digital values.
- Sends temperature data through CAN communication.

### Fuel Node (Receiver 2)
- Reads fuel level voltage using MCP3204 ADC through SPI.
- Classifies fuel status as LOW, MEDIUM, or HIGH.
- Sends fuel data through CAN communication.

## Communication Protocols Used

### CAN (Controller Area Network)
- Standard CAN Frame (11-bit Identifier)
- Remote Frame (RTR = 1) for requesting data
- Data Frame (RTR = 0) for transmitting actual data
- CAN Baud Rate: 125 kbps

### SPI (Serial Peripheral Interface)
Used for interfacing MCP3204 ADC with LPC2129.

SPI Connections:
- SCK (Clock) → P0.4
- MISO (DOUT) → P0.5
- MOSI (DIN) → P0.6
- CS → P0.7

## Functional Flow
1. User presses SW1 to request temperature data.
2. Dashboard node sends CAN remote frame to temperature node.
3. Temperature node reads LM35 sensor value through MCP3204 ADC and sends temperature data.
4. Dashboard displays temperature and activates motor if temperature exceeds 35°C.
5. User presses SW2 to request fuel data.
6. Dashboard node sends CAN remote frame to fuel node.
7. Fuel node reads fuel level voltage and sends data through CAN.
8. Dashboard displays fuel status and activates warning LED when fuel level is low.

## Advantages
- Reduced wiring complexity.
- Reliable communication with built-in error detection.
- Scalable architecture supporting additional vehicle nodes.
- Suitable for automotive applications.
- Improved monitoring and safety.

## Future Enhancements
- Integration of GPS and GSM modules.
- Tire Pressure Monitoring System (TPMS).
- Engine Diagnostics and Fault Detection.
- IoT-based vehicle monitoring.
- Data logging and cloud connectivity.

## Author
**Praveena**

## Project Title
**Vehicle Dashboard Using Automotive Industry Standard CAN Communication Protocol**
