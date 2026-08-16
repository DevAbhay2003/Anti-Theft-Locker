# Tech Stack Options & Implementation Plan

## Hardware Options

| Option | Components | Difficulty | Expected Output | Hardware Mandatory? |
|---|---|---|---|---|
| **A — Easy** | Arduino UNO, 4x4 keypad, servo, buzzer, LED | Beginner | Password unlock + buzzer alert only, no display | No — fully simulatable |
| **B — Recommended** ✅ | Arduino UNO/ESP32, 4x4 keypad, servo/solenoid, 16x2 LCD, buzzer, Green+Red LED | Intermediate | Full authentication flow with LCD feedback + lockout alarm (this repo's default build) | No — fully simulatable |
| **C — Advanced** | ESP32, keypad, RFID/fingerprint optional, solenoid lock, OLED, Wi-Fi notification, failed-attempt logging | Advanced | Everything in B + remote alerts + multi-factor auth + logging | No, but a cloud/Wi-Fi backend and a real solenoid add real-world value with real hardware |

**Recommended for students: Option B.** It covers every core embedded security concept (keypad scanning, state-based control, threshold logic, I2C display) without the added complexity of network programming or extra auth hardware, and is fully demonstrable in simulation — this is the option implemented in `arduino_code/anti_theft_locker_complete.ino`.

## Implementation Plan (12 Phases)

| Phase | Objective | Key Tasks | Verify By |
|---|---|---|---|
| 1. Environment setup | Install tools | Arduino IDE, Wokwi account, GitHub account | IDE compiles a blank sketch |
| 2. Keypad interfacing | Read keypad input | Upload `keypad_module.ino` | Serial Monitor echoes masked `*` per keypress |
| 3. LCD interfacing | Get display working | Wire I2C LCD, run an I2C scanner + "Hello World" | LCD shows text correctly |
| 4. Password input logic | Build entry buffer | Add digit accumulation, `*` clear, `#` submit | Buffer builds/clears correctly in Serial log |
| 5. Password verification | Compare against stored PIN | Upload `authentication_module.ino` | Correct PIN reports "Access Granted", wrong reports "Access Denied" |
| 6. Lock control | Move the servo | Upload `lock_control_module.ino` | Servo moves to unlock/lock angles on command |
| 7. Wrong-attempt counter | Track failures | Add counter logic to authentication module | Counter increments correctly, resets on success |
| 8. Buzzer alarm logic | Add alarm module | Upload `alarm_module.ino`, wire outputs | Buzzer/LEDs respond to G/R/L test commands |
| 9. Auto-lock feature | Add unlock timeout | Add `millis()`-based auto-lock timer | Servo re-locks automatically after timeout |
| 10. Full integration | Merge everything | Run `anti_theft_locker_complete.ino` | All 13 test cases in `testing_strategy.md` pass |
| 11. Testing & debugging | Validate full flow | Walk through correct/wrong/lockout sequences | LCD, LEDs, buzzer, servo all match expected states |
| 12. GitHub upload | Publish proof of work | Follow `github_strategy.md` and `proof_building_plan.md` | Repo public, README renders correctly, 8+ commits |

### Common Mistakes to Watch For
- Forgetting a common ground between the keypad, servo/solenoid, LCD, and microcontroller (causes erratic keypresses or a blank LCD).
- Using `delay()` for the auto-lock or lockout timers (freezes keypad reading) — use `millis()` instead.
- Printing the real password to Serial or LCD while debugging and forgetting to remove it before "finishing" the project.
- Not clearing the entered-password buffer after a submit, causing digits to carry over into the next attempt.
- Driving a real solenoid lock directly from a GPIO pin instead of through a transistor/relay stage (can damage the microcontroller).
