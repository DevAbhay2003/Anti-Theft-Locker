# Testing Strategy

| # | Test Case | Input Condition | Expected Output | Pass/Fail Criteria |
|---|---|---|---|---|
| 1 | Correct password | Enter `1234` then `#` | Locker unlocks | Servo→90°, LCD "Access Granted", Green LED on |
| 2 | Wrong password | Enter `0000` then `#` | Access denied | LCD "Access Denied", Red LED flashes, counter = 1 |
| 3 | Three consecutive wrong passwords | Enter wrong PIN 3 times | System locks out | Buzzer + Red LED on, LCD "Too Many Attempts" |
| 4 | Incomplete password | Enter `12` then `#` | Treated as wrong password (doesn't match) | Access Denied, counter increments |
| 5 | Keypad clear key (`*`) | Enter `12` then `*` | Entry cleared | LCD returns to "Enter Password", buffer empty |
| 6 | Keypad enter key (`#`) with empty buffer | Press `#` with nothing entered | No action taken | No verify attempt, no counter change |
| 7 | Servo lock | System idle/locked state | Servo at 0° | Physically/visually confirmed locked position |
| 8 | Servo unlock | Correct password entered | Servo at 90° | Physically/visually confirmed open position |
| 9 | Auto-lock | Wait `AUTO_LOCK_DELAY_MS` after unlock | Servo returns to locked | LCD "Locker Locked", no manual action needed |
| 10 | Alarm activation | 3rd wrong attempt | Buzzer + Red LED active | Buzzer audible/visible in simulator |
| 11 | Reset after valid login | Correct password after prior wrong attempts | Wrong-attempt counter resets to 0 | Next wrong attempt starts counting from 1, not continuing old count |
| 12 | Power restart | Reset/re-run the sketch | System returns to locked, idle state, counter = 0 | LCD shows startup + "Enter Password" |
| 13 | Rapid keypad input | Press multiple keys quickly | All keypresses registered in order, no drops/duplicates | Entered string matches exactly what was pressed |

## Testing Approach

1. **Unit-level** — test each module (`keypad_module.ino`, `authentication_module.ino`, `lock_control_module.ino`, `alarm_module.ino`) independently before integration.
2. **Integration** — upload/run `anti_theft_locker_complete.ino` and walk through all 13 cases above, in both the simulator and (if available) real hardware.
3. **Security check** — confirm the password is never printed to Serial or LCD in plaintext, and that lockout genuinely blocks further keypad-triggered verification attempts.
4. **Regression** — after any code change, re-run at minimum test cases 1, 2, 3, and 9 (the core behaviors) before committing.

## Sample Expected Serial Output

```
=== Anti-Theft Locker System Ready ===
Access Denied. Wrong attempts: 1
Access Granted. Locker: UNLOCKED
Locker: LOCKED
Access Denied. Wrong attempts: 1
Access Denied. Wrong attempts: 2
Access Denied. Wrong attempts: 3
ALARM: Too many wrong attempts. System locked out.
Lockout period ended. Ready for new attempts.
```
