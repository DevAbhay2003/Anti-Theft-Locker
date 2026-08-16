# Interview Preparation

### 1. Explain your project.
The Anti-Theft Locker System is an embedded access-control device: a user enters a PIN on a 4x4 keypad, the microcontroller compares it against a stored password, and on a match it drives a servo (standing in for a solenoid lock) to unlock, shows "Access Granted" on an LCD, and lights a green LED. On a mismatch it shows "Access Denied", lights a red LED, and increments a wrong-attempt counter; after 3 consecutive failures the system locks out further attempts and sounds a buzzer alarm for a fixed period. I implemented it as modular Embedded C, validated it fully in a Wokwi simulation, and also documented the real-hardware wiring.

### 2. Why did you use a matrix keypad instead of individual push buttons?
A 4x4 matrix keypad gives 16 keys using only 8 GPIO pins (4 rows + 4 columns) instead of 16 individual pins, by scanning rows against columns. That's a standard embedded technique for keeping pin usage low when you need many inputs.

### 3. How does the password comparison work, and is it secure?
The entered digits are built into a `String` as the user types, and on `#` it's compared with `==` against a stored password constant. For this student/demo project the password is hardcoded, which I explicitly call out as acceptable for learning but not for production — a real deployment should store it in EEPROM so it's changeable without re-flashing, and should never print the actual PIN to Serial or the LCD (I only ever display masked `*` characters).

### 4. How does the wrong-attempt lockout work?
Every failed comparison increments a counter. Once it reaches 3, the system enters a `LOCKED_OUT` state: the buzzer and red LED turn on, and the keypad handler ignores all further digit/submit input until a lockout timer (tracked with `millis()`, not `delay()`) elapses, at which point the counter resets and the system returns to normal.

### 5. Why did you use `millis()` instead of `delay()` for the auto-lock and lockout timers?
`delay()` blocks the whole program, so the system couldn't read the keypad or update the LCD while "waiting." Using `millis()` to track elapsed time keeps the main loop responsive — the keypad, LCD, and timers all keep working concurrently.

### 6. What's the difference between the servo-based lock and a solenoid lock in this design?
Functionally they play the same role — a physical actuator that locks/unlocks. The servo is driven directly by a PWM signal, easy to simulate and prototype on a breadboard. A real solenoid lock draws much more current and needs a transistor or relay driver stage (with a flyback diode) rather than a direct GPIO connection — I documented that distinction in the circuit notes for anyone moving from the servo prototype to a real lock.

### 7. How would you make this project production-grade?
Move the password to EEPROM with a "change PIN" keypad flow, add a second factor like RFID or fingerprint, add ESP32 Wi-Fi notifications on repeated failures, log failed-attempt timestamps to non-volatile storage, and replace the demo servo with a properly current-rated solenoid lock and a battery backup so the system still reports state during a power cut.

### 8. Why is the project split into separate modules before integration?
Each module — keypad, authentication, lock control, alarm — can be uploaded and tested completely independently, which makes debugging much faster: if the buzzer isn't triggering, I only need to look at `alarm_module.ino`. It also mirrors how real embedded firmware is structured into testable, single-responsibility components.

### 9. What embedded concepts does this project demonstrate?
Matrix keypad scanning, PWM servo control, I2C communication with the LCD, string-based password comparison, state-machine-style control flow (idle → entering → verify → unlocked/locked-out), non-blocking timing with `millis()`, and threshold-based security logic (attempt counting and lockout).

### 10. How did you validate this project without physical hardware?
I used Wokwi, a browser-based simulator that runs the actual compiled Arduino sketch against a simulated keypad, servo, LCD, LEDs, and buzzer. I could click through every test case — correct password, wrong password, three consecutive failures, auto-lock timing — directly in the simulator and confirm the exact same firmware behaves correctly, documenting each stage with an LCD/Serial Monitor screenshot as proof.
