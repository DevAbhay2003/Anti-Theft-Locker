# Virtual Simulation Guide (Wokwi)

No physical hardware is required — build, run, and demo this entire project in [Wokwi](https://wokwi.com), a free browser-based Arduino/ESP32 simulator.

## Step-by-Step

**Step 1 — Create the project**
Go to wokwi.com → "New Project" → choose **Arduino Uno**.

**Step 2 — Add the 4x4 Keypad**
Search "Membrane Keypad" (4x4) in the parts panel and drag it onto the canvas.

**Step 3 — Add the Servo Motor**
Search "Servo Motor", drag it near the lock position.

**Step 4 — Add the LCD**
Search "LCD1602" (I2C variant), drag it onto the canvas.

**Step 5 — Add LEDs and Buzzer**
Add a Green LED, a Red LED (each with a 220Ω resistor), and a Buzzer.

**Step 6 — Wire the circuit**
Connect each part exactly per `circuit_diagram/circuit_diagram.md`:
- Keypad R1-R4 → D9, D8, D7, D6; C1-C4 → D5, D4, D3, D2
- Servo: VCC→5V, GND→GND, Signal→D11
- LCD (I2C): VCC→5V, GND→GND, SDA→A4, SCL→A5
- Buzzer → D12 → GND
- Green LED → A1 (through resistor) → GND
- Red LED → A2 (through resistor) → GND

**Step 7 — Paste the code**
Open the Wokwi code editor and paste the full contents of `arduino_code/anti_theft_locker_complete.ino`. Make sure the `PASSWORD` constant matches what you plan to type (default `"1234"`).

**Step 8 — Run the simulation**
Click the green ▶ Run button.

**Step 9 — Enter the correct password**
Click the keypad buttons in the simulator to type `1234`, then click `#`.

**Step 10 — Verify the locker unlocks**
Confirm the LCD shows "Access Granted / Locker Open", the servo rotates to the unlock angle, and the Green LED turns on.

**Step 11 — Wait and verify auto-lock**
Wait ~8 seconds (the default `AUTO_LOCK_DELAY_MS`) and confirm the servo returns to the locked position and the LCD shows "Locker Locked / Enter Password".

**Step 12 — Enter a wrong password**
Type a wrong PIN (e.g. `0000`) and press `#`. Confirm the LCD shows "Access Denied / Try Again" and the Red LED flashes.

**Step 13 — Repeat the wrong password 3 times**
Enter the wrong PIN two more times (3 total).

**Step 14 — Verify the alarm activates**
Confirm the LCD shows "Too Many / Attempts! Locked", the Red LED stays solid, and the buzzer activates (Wokwi shows an audio waveform/icon for the buzzer). Confirm keypad input is ignored while locked out, and that the system resets automatically after the lockout period (10s by default).

## What the Serial Monitor Should Display

```
=== Anti-Theft Locker System Ready ===
Access Denied. Wrong attempts: 1
Access Denied. Wrong attempts: 2
Access Denied. Wrong attempts: 3
ALARM: Too many wrong attempts. System locked out.
Lockout period ended. Ready for new attempts.
Access Granted. Locker: UNLOCKED
Locker: LOCKED
```

## What the LCD Should Display

| Event | LCD Line 1 | LCD Line 2 |
|---|---|---|
| Startup | Anti-Theft | Locker System |
| Idle | Locker Locked | Enter Password |
| Typing digits | Enter Password | `****` (masked) |
| Correct password | Access Granted | Locker Open |
| Wrong password | Access Denied | Try Again |
| 3rd wrong attempt | Too Many | Attempts! Locked |
| During lockout | System Locked | Please Wait... |

## Screenshots to Capture for GitHub Proof

1. Full Wokwi circuit view (all components wired)
2. LCD showing "Enter Password" with masked digits
3. LCD + servo showing the unlocked state, Green LED on
4. LCD showing "Access Denied" with Red LED
5. LCD showing "Too Many Attempts" + buzzer active (3rd fail)
6. Serial Monitor full log covering a correct and a failed sequence

Save these into the `screenshots/` folder using the naming convention in `docs/screenshots_checklist.md`.

## Using Tinkercad Circuits Instead

Tinkercad's workflow is similar: create a new circuit, add an Arduino Uno + Keypad (4x4) + Servo Motor + LCD 16x2 + 2 LEDs + Buzzer, wire per the same table, paste the code into the "Code → Text" editor, and use "Start Simulation." Click the keypad buttons directly on the Tinkercad canvas to enter digits.
