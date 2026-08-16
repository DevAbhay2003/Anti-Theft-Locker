# 🔐 Anti-Theft Locker System – Industry-Oriented Embedded System

An Arduino/ESP32-based electronic locker that unlocks only on correct keypad password entry, denies and counts wrong attempts, triggers a buzzer alarm after repeated failures, and auto-locks after a timeout. Built as a beginner-friendly but industry-relevant Embedded Systems project.

![Status](https://img.shields.io/badge/status-active-brightgreen) ![Platform](https://img.shields.io/badge/platform-Arduino%20%7C%20ESP32-blue) ![License](https://img.shields.io/badge/license-MIT-lightgrey)

---

## 📌 Overview

The Anti-Theft Locker System replaces a mechanical key/lock with a 4x4 keypad + microcontroller + servo/solenoid combination. A user enters a PIN; the system compares it against a stored password, unlocks on a match, and on repeated mismatches locks the system down and sounds an alarm — all state and feedback shown on an LCD.

## ❓ Problem Statement

Mechanical locks can be picked or key-duplicated with no record of the attempt. A basic electronic keypad lock adds PIN-based access control, visible feedback (LCD + LEDs), and — critically — an active response (alarm + lockout) to repeated unauthorized access attempts, which a physical lock cannot provide on its own.

## 🏭 Industry Relevance

| Sector | Value Added |
|---|---|
| Bank lockers | PIN-based access with automatic alarm on tampering |
| Office lockers | Controlled, code-based access without physical key management |
| Hotel safes | Guest-settable access without a master key handout |
| School/college lockers | Low-cost per-locker security for students |
| Warehouse security | Restrict storage-room access to authorized codes |
| Home security | DIY smart-lock entry point |
| Industrial storage cabinets | Access control for tools/hazardous materials |

**Business value:** controlled access → fewer unauthorized entries · electronic logic → auditable attempt counts · automatic alarm → faster incident response · low component cost → affordable per-unit security compared to commercial smart locks.

## ✨ Features

- 4x4 matrix keypad password entry with masked (`*`) display
- Password verification against a stored PIN
- Servo-motor (or solenoid) controlled physical lock/unlock
- Wrong-attempt counter with a 3-strikes lockout + buzzer alarm
- Auto-lock after a configurable timeout
- 16x2 LCD status messages (Enter Password / Access Granted / Access Denied / Too Many Attempts)
- Green/Red LED access indication
- Fully modular Embedded C / Arduino code
- Complete virtual simulation path (Wokwi) — **no physical hardware required**

## 🧩 Components Used

| Component | Purpose |
|---|---|
| Arduino UNO / ESP32 | Main controller (ESP32 adds Wi-Fi option) |
| 4x4 Matrix Keypad | Password entry |
| Servo Motor (SG90) or Solenoid Lock | Physical locking mechanism |
| 16x2 LCD (I2C) | Status / prompt display |
| Buzzer | Alarm on repeated wrong attempts |
| Green LED | Access granted indicator |
| Red LED | Access denied / alarm indicator |
| Breadboard + jumper wires | Prototyping connections |
| 5V power supply / USB | Power |

## 🧠 Embedded Systems Concepts Demonstrated

- Matrix keypad scanning (row/column GPIO multiplexing)
- PWM-based servo/lock actuation
- String-based password comparison
- State-machine style control flow (IDLE → ENTERING → VERIFY → UNLOCKED / LOCKED_OUT)
- Attempt counters and threshold-based lockout logic
- Non-blocking timing (`millis()`) for auto-lock and lockout periods
- I2C communication for the LCD
- (Optional) EEPROM for a persistent, changeable password

## 🏗️ Architecture

```
        ┌────────────────┐
        │  4x4 Keypad    │  user enters PIN
        └───────┬────────┘
                │ key presses
                ▼
     ┌────────────────────────┐
     │     Microcontroller     │   compares entered PIN
     │   (Arduino / ESP32)     │   to stored password,
     └───┬─────────┬─────┬────┘   tracks attempt count
         │         │     │
   PWM   │   I2C    │     │ digital
         ▼         ▼     ▼
   ┌──────────┐ ┌──────┐ ┌───────────────────┐
   │ Servo /  │ │ LCD  │ │ Green LED / Red LED│
   │ Solenoid │ │16x2  │ │ / Buzzer           │
   │  Lock    │ └──────┘ └───────────────────┘
   └──────────┘
```

### Authentication Flow

```
Enter Password
     │
     ▼
Keypad Input (build string until '#' = Enter, '*' = Clear)
     │
     ▼
Microcontroller
     │
     ▼
Password Verification
     │
   ┌─┴─┐
  YES   NO
   │     │
   ▼     ▼
Unlock  Increase Failed Attempt Count
Locker      │
Green LED   ▼
Reset     Red LED
counter      │
         Attempts ≥ 3?
           │        │
          YES       NO
           │        │
           ▼        ▼
   Lockout + Buzzer  Return to
   (keep locked)     Enter Password
```

### Input / Output Table

| Signal | Type | Direction | Pin (UNO example) |
|---|---|---|---|
| Keypad Rows (4) | Digital Out | MCU → Keypad | D9, D8, D7, D6 |
| Keypad Cols (4) | Digital In | Keypad → MCU | D5, D4, D3, D2 |
| Servo Signal | PWM Out | MCU → Servo | D11 |
| Buzzer | Digital Out | MCU → Buzzer | D12 |
| Green LED | Digital Out | MCU → LED | A1 |
| Red LED | Digital Out | MCU → LED | A2 |
| LCD SDA/SCL | I2C | MCU ↔ LCD | A4 / A5 |

### State / Control Table

| State | Trigger | Next State |
|---|---|---|
| IDLE / Enter Password | Any keypress | ENTERING |
| ENTERING | `#` pressed with full-length PIN | VERIFY |
| ENTERING | `*` pressed | IDLE (cleared) |
| VERIFY | Match found | UNLOCKED |
| VERIFY | No match | DENIED |
| DENIED | attempts < 3 | IDLE |
| DENIED | attempts ≥ 3 | LOCKED_OUT |
| UNLOCKED | timeout elapsed | LOCKED (auto-lock) |
| LOCKED_OUT | lockout timer elapsed | IDLE (counter reset) |

## 🔌 Circuit Connections

See [`circuit_diagram/circuit_diagram.md`](circuit_diagram/circuit_diagram.md) for full wiring instructions.

## 📁 Folder Structure

```
Anti-Theft-Locker-Embedded-System/
│
├── src/                   # Shared logic notes / pseudo-code
├── arduino_code/          # All .ino source files (modular + integrated)
├── simulation/            # Wokwi/Tinkercad setup instructions
├── circuit_diagram/       # Wiring diagram + connection tables
├── outputs/               # Sample serial monitor logs
├── screenshots/           # Proof-of-work images (add your own)
├── reports/               # Project report / documentation exports
├── docs/                  # Testing strategy, GitHub plan, interview prep
├── README.md
└── .gitignore
```

## ⚙️ Installation & How to Run

### Real Hardware
1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Wire the components per the circuit diagram.
3. `Tools → Board` → select **Arduino Uno** (or an ESP32 board via Boards Manager).
4. `Tools → Port` → select the correct COM port.
5. `Sketch → Include Library → Manage Libraries` → install **Keypad** (by Mark Stanley/Alexander Brevig), **Servo** (bundled), and **LiquidCrystal_I2C**.
6. Open `arduino_code/anti_theft_locker_complete.ino`, set your `PASSWORD` constant, click **Upload**.
7. Open **Serial Monitor** at 9600 baud to view live status.

### Virtual Simulation (no hardware needed)
Full guide in [`simulation/wokwi_instructions.md`](simulation/wokwi_instructions.md). Short version:
1. Go to [wokwi.com](https://wokwi.com) → New Project → Arduino Uno.
2. Add a 4x4 Membrane Keypad, Servo Motor, LCD1602 (I2C), 2 LEDs, and a Buzzer; wire per the diagram.
3. Paste `arduino_code/anti_theft_locker_complete.ino` into the editor.
4. Click ▶ Run, click keys on the simulated keypad to enter the PIN, and observe the LCD/servo/LED response.

## 🔒 Security Logic

- The password is stored as a hardcoded string constant for simulation/learning purposes — acceptable for a beginner project, **not** for production.
- For a real deployment, store the password in EEPROM so it can be changed without re-flashing firmware, and never print/log the actual password to Serial or LCD.
- The LCD masks entered digits with `*` — the real PIN is never shown on-screen as it's typed.
- 3 consecutive wrong attempts triggers a lockout: the buzzer sounds and further keypad input is ignored for a fixed lockout period, after which the attempt counter resets.

## 🧪 Testing

See [`docs/testing_strategy.md`](docs/testing_strategy.md) for the full test-case table.

## 📸 Screenshots

See [`docs/screenshots_checklist.md`](docs/screenshots_checklist.md) — add captured images under `screenshots/` and reference them here once captured.

## 🚀 Future Improvements

- EEPROM-stored, user-changeable password (change-PIN keypad flow)
- RFID or fingerprint module as a second authentication factor
- ESP32 Wi-Fi push notification on repeated failed attempts
- Failed-attempt logging with timestamps to an SD card or cloud log
- Battery backup so the lock still reports state during a power cut

## 🎓 Learning Outcomes

- Scanning a matrix keypad via row/column GPIO
- Designing state-machine-style control flow in Embedded C
- Implementing threshold-based security logic (attempt counting, lockout)
- Driving an I2C LCD for user feedback
- Structuring an embedded project into modular, testable functions
- Simulating a full access-control system without physical components

## 👤 Author

*Your Name* — Embedded Systems Student
GitHub: `[DevAbhay2003](https://github.com/DevAbhay2003?tab=repositories)` · 
LinkedIn: `[Abhishek Basu](https://www.linkedin.com/in/abhishek-basu-68b1b1342/)`

## 📄 License

MIT License — free to use and modify for learning purposes.
