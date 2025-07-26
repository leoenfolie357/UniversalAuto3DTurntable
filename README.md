# UniversalAuto3DTurntable
A fully open-source, automated photogrammetry turntable powered by dual ESP8266 boards. Triggers DSLR (IR ML-L3) and smartphones (via contact), displays status on OLED, and supports multiple capture cadences. Perfect for 3D scanning with PhotoCatch or similar software.

---

## 🧭 Table of Contents

- [🎯 Objective](#-objective)
- [🧠 System Architecture](#-system-architecture)
- [🔌 Wiring Overview](#-wiring-overview)
- [⚙️ Operating Principle](#️-operating-principle)
- [💻 Software & Programming](#-software--programming)
- [🔩 Motor & Mechanics](#-motor--mechanics)
- [📡 Bluetooth & Compatibility](#-bluetooth--compatibility)
- [🖥️ OLED Display](#-oled-display)
- [🔌 Power & Safety](#-power--safety)

---

## 🎯 Objective

Design a **universal motorized and automated turntable** for photogrammetry and object display.  
This system:

- Triggers **DSLRs via IR** (Nikon ML-L3 protocol).
- Simulates **Bluetooth remote shutter** via contact (3V3 idle, active LOW).
- Offers **5 shooting cadences**:
  - `0` = continuous rotation  
  - `10`, `20`, `30`, `40` = number of stops per full rotation  
- Displays status on an **OLED screen**.
- Controlled via **3 buttons**:
  - Manual trigger (SWBT),
  - Cadence selector (SW4),
  - Start/stop cycle (SW1).

---

## 🧠 System Architecture

- **NodeMCU 1** *(Remote Control)*:
  - Handles input buttons
  - Sends IR and contact shutter signals
  - Communicates via UART

- **NodeMCU 2** *(Motor Control)*:
  - Drives stepper motor
  - Manages OLED display

---

## 🔌 Wiring Overview

### NodeMCU 1 – Remote

| Function         | GPIO | Pin | Notes                                     |
|------------------|------|-----|-------------------------------------------|
| SW1 (Start/Stop) | 12   | D6  | Cycle control + beep                      |
| SW4 (Cadence)    | 13   | D7  | Cadence selector + beep                   |
| SWBT (Manual)    | 0    | D3  | Triggers IR + contact + beep              |
| IR LED           | 4    | D2  | Via 220Ω resistor                         |
| Remote Contact   | 5    | D1  | Active LOW output                         |
| Buzzer           | 14   | D5  | 1000 Hz / 200 ms                          |
| Onboard LED      | 2    | D4  | Status indication                         |
| UART TX/RX       | 1/3  | TX/RX | To NodeMCU 2                             |
| Power            | —    | J4  | 5V / 2A (external)                        |

### NodeMCU 2 – Motor & OLED

| Function         | GPIO | Pin | Notes                                     |
|------------------|------|-----|-------------------------------------------|
| Stepper IN1–IN4  | 5/4/0/2 | D1–D4 | Connected to ULN2003                     |
| OLED SDA         | 12   | D6  | I2C, rotated 180°                         |
| OLED SCL         | 13   | D7  | I2C, rotated 180°                         |
| Onboard LED      | 2    | D4  | Status indication                         |
| UART TX/RX       | 1/3  | TX/RX | To NodeMCU 1                             |
| Power            | —    | J2  | 5V / 2A (external)                        |

---

## ⚙️ Operating Principle

### On Power-Up
- **NodeMCU 1:**
  - Blue LED ON
  - Buzzer silent
  - Contact output (SWREM) idle (HIGH)
- **NodeMCU 2:**
  - OLED shows "Turntable Ready"
  - Default cadence = 10
  - Blue LED ON

### Button Functions (NodeMCU 1)

| Button | Action                                              |
|--------|-----------------------------------------------------|
| SW4    | Cycle cadence: 0 → 10 → 20 → 30 → 40 → 0 … (beep)   |
| SW1    | Starts/stops cycle (beep, LED OFF during cycle)     |
| SWBT   | Manual trigger: IR + contact + beep                 |

### Motor Cycle (NodeMCU 2)
- **Full turn** = 5808 steps (or 2940 in half-step mode)
- **Cadence = 0**: Continuous rotation
- **Cadence > 0**:
  - Divide 5808 steps into N stops
  - Pause 200 ms at each stop
  - Emit `SHOOT
` via UART to NodeMCU 1
- **Cycle End**: Motor stops, LED ON, OLED updates status

---

## 💻 Software & Programming

- **Platform:** Arduino IDE  
- **Language:** Arduino C++  
- **Libraries:**
  - `IRremoteESP8266` – Nikon ML-L3 IR trigger  
  - `U8g2` – OLED display (rotated 180°)  
  - `Stepper` – for 28BYJ-48 motor

### UART Serial Commands (9600 baud)
- `CADENCE:X` → Update cadence (X = 0,10,20,30,40)  
- `START` → Start rotation  
- `STOP` → Stop rotation  
- `SHOOT` → Trigger camera (IR + contact)

### IR Signal – Nikon ML-L3

```c++
uint16_t nikonPulse[] = {
  2000, 27850, 390, 1580, 410, 3580, 400, 63200
};
IrSender.sendRaw(nikonPulse, sizeof(nikonPulse) / sizeof(uint16_t), 38);
```

---

## 🔩 Motor & Mechanics

- **Motor:** 28BYJ-48 + ULN2003 driver  
- **Steps per turn:** 5808 (or 2940 half-step)  
- **Pause per photo:** 200 ms (adjustable)  
- **3D Printed Housing:** includes bearings, gear drive, PCB, buttons, OLED

---

## 📡 Bluetooth & Compatibility

- **Bluetooth** replaced with **wired contact trigger**  
- Works with **cheap selfie remotes** (3V3 idle, active LOW)  
- Supports **iPhone/Android** with appropriate button mapping

---

## 🖥️ OLED Display

- Shows:
  - Current cadence
  - Current photo count (e.g., `Image 3/30`)
  - Cycle status: Ready / In Progress / Finished
- **Orientation:** 180° rotated
- **Future improvements:** ETA timer, diagnostics mode (optional)

---

## 🔌 Power & Safety

- **Power Supply:** 5V / 2A via J2 & J4  
- **Protections:** External power, regulated, no known issues  
- **Stability:** Reliable for repeated scan cycles

---

## 🧰 Bill of Materials & Downloads

👉 See [🔗 project files section](#) for:
- PCB design (Gerber)
- STL files for 3D printing
- Arduino source code
- Parts list (BOM)
- Wiring diagrams

---

## 💬 License

This project is open-source and licensed under the MIT License.

---

## 🙌 Contributions

Feel free to fork, improve, or suggest features via Issues or Pull Requests!
