# Camaro Sim-Racing Interface

This project documents the conversion of a real-world Camaro steering wheel and column stalk assembly into a USB HID Game Controller for PC racing simulations.

The system uses two independent Arduino Pro Micros (ATmega32U4) to handle the high input count and separate the wiring harnesses for the steering wheel and the steering column.

---

## 🛠 Hardware Architecture

### 1. Steering Wheel Module
* **Controller:** Arduino Pro Micro
* **Input Count:** 15 Buttons + 1 8-Way Hat Switch (POV)
* **Custom Interface:** ASIC Bypass. Soldered directly to 220Ω ESD protection resistors to bypass the proprietary LIN-Bus chip.
* **Resistive Ladders:** * **A0:** Paddle Shifters
    * **A1:** Cruise Control functions
* **Logic:** D-Pad buttons are processed into a single POV Hat Switch (0°–315°) for better menu navigation.

### 2. Column Stalk Module
* **Controller:** Arduino Pro Micro
* **Input Count:** 15 Buttons (Digital + Analog)
* **Wiper Logic:** * **Analog Ladder (A0):** 5 Intermittent settings + Low speed.
    * **Digital Input (D10):** High-speed wiper override.
* **Signals & Lights:** 8 digital inputs for Turn Signals, High Beams, and Washer.

---

## 📐 Wiring & Pinout

### Steering Wheel (Pro Micro #1)
| Pin | Function | Wiring Type |
| :--- | :--- | :--- |
| **A0** | Paddle Shifters | Analog (10kΩ Pull-up) |
| **A1** | Cruise Control | Analog (10kΩ Pull-up) |
| **2-5** | Audio Buttons | Digital (Input Pull-up) |
| **6-9** | D-Pad (Up, Down, Left, Right) | Digital (POV Hat) |
| **10, 14, 15** | Select / Phone Buttons | Digital (Input Pull-up) |
| **VCC** | Resistor Rail | 5V Output to 10kΩ Pull-ups |
| **GND** | Ground Rail | Common Ground for all switches |

### Column Stalks (Pro Micro #2)
| Pin | Function | Logic / Range |
| :--- | :--- | :--- |
| **A0** | Wiper Ladder | 10kΩ Pull-up (Infinite = OFF) |
| **15** | Wiper High | Digital (Priority Override) |
| **2,3** | Turn Signals | Digital (Input Pull-up) |
| **4-6** | Light Switch (Low, Park, Toggle) | Digital (Input Pull-up) |
| **7,8** | High Beam (In/Out) | Digital (Input Pull-up) |
| **14** | Windshield Washer | Digital (Input Pull-up) |

---

## 💻 Code Logic Highlights

### 1. Priority Wiper Control
To prevent conflicting signals, the code uses a priority gate. If the physical "High" switch is active, the analog "Low/Intermittent" signals are automatically ignored.

### 2. POV Hat Switch Map
The D-pad is mapped using diagonal logic to allow for 8-way movement:
- **Up + Right:** 45°
- **Down + Right:** 135°
- **Down + Left:** 225°
- **Up + Left:** 315°

### 3. Global Variable Management
State variables are initialized globally to ensure memory efficiency and allow for future implementation of "Long Press" or "Double Tap" features without re-initializing variables in the main loop.

---

## 🔧 Installation & Calibration

1.  **Dependencies:** Requires the [Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary).
2.  **ADC Calibration:** Use a Serial Debug script to find your specific resistive values. 
    * *Current Build Profile:* Uses a 10kΩ Pull-up resistor for all analog ladders.
3.  **Hardware Assembly:** * Use double-sided perf board for durability.
    * Socket the Pro Micro using female headers for easy replacement.
    * Leave a 1-hole gap between the Pro Micro and male pin headers to prevent solder bridges.

---

## ⚠️ Disclaimer
Interfacing with automotive PCBs requires bypassing proprietary ASIC chips. This project involves precision soldering to surface-mount components (SMD). Proceed with caution to avoid lifting pads or damaging the original hardware.
