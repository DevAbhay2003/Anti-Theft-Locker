# Circuit Diagram & Wiring Instructions

## Text-Based Block Diagram

```
                         +------------------------+
   4x4 Keypad            |   Arduino UNO / ESP32  |
   R1 --------------------> D9                    |
   R2 --------------------> D8                    |
   R3 --------------------> D7                    |
   R4 --------------------> D6                    |
   C1 --------------------> D5                     |
   C2 --------------------> D4                     |
   C3 --------------------> D3                     |
   C4 --------------------> D2                     |
                            |                      |
   Servo (lock) --Signal--> D11 (PWM)              |
                            |                      |
   Buzzer       ----------> D12                    |
                            |                      |
   Green LED    <---------- A1                     |
   Red LED      <---------- A2                     |
                            |                      |
   LCD (I2C) SDA ---------- A4                     |
   LCD (I2C) SCL ---------- A5                     |
                            |                      |
                            |  5V ---- VCC (all)     |
                            |  GND --- GND (all)     |
                            +------------------------+
```

## Connection Tables

### 4x4 Matrix Keypad
| Keypad Pin | Arduino Pin |
|---|---|
| R1 | D9 |
| R2 | D8 |
| R3 | D7 |
| R4 | D6 |
| C1 | D5 |
| C2 | D4 |
| C3 | D3 |
| C4 | D2 |

> No external power needed — the keypad is a passive row/column matrix; the microcontroller drives rows and reads columns internally via the `Keypad` library.

### Servo Motor (SG90) / Solenoid Lock
| Servo/Solenoid Wire | Arduino Pin |
|---|---|
| VCC | 5V *(use an external 5V supply for a solenoid lock — it typically draws more current than the Arduino 5V pin can safely supply)* |
| GND | GND |
| Signal (servo) / Control (solenoid via transistor/relay) | D11 |

> If using a real solenoid lock instead of a servo, drive it through a flyback-diode-protected NPN transistor or a relay module — never connect a solenoid's coil directly to a GPIO pin.

### Buzzer
| Buzzer Pin | Arduino Pin |
|---|---|
| Positive (+) | D12 |
| Negative (−) | GND |

### LEDs
| LED | Anode (+) via ~220Ω resistor | Cathode (−) |
|---|---|---|
| Green LED | A1 | GND |
| Red LED | A2 | GND |

### 16x2 I2C LCD
| LCD Pin | Arduino Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

## Wiring Notes

- Common ground across the keypad, servo/solenoid, LCD, LEDs, and buzzer is essential — a floating ground is the most common cause of "phantom" keypresses.
- Keep the keypad ribbon cable short or well-seated in the breadboard; loose header pins are a common source of missed/double keypresses.
- If your I2C LCD doesn't light up, run an I2C scanner sketch first to confirm the address (commonly `0x27` or `0x3F`) before assuming the wiring is wrong.
- For ESP32, see the pin remapping note at the bottom of `arduino_code/anti_theft_locker_complete.ino`.
