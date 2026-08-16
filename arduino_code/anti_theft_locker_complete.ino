/*
  =====================================================
  ANTI-THEFT LOCKER SYSTEM - COMPLETE INTEGRATED CODE
  =====================================================
  Combines:
    A. Keypad password entry (masked with *)
    B. Authentication (compare + wrong-attempt counter)
    C. Lock control (servo, auto-lock timer)
    D. Alarm (LEDs + buzzer, lockout on 3 wrong attempts)
    E. 16x2 I2C LCD status display

  Board tested on: Arduino UNO (ESP32 pin-remap note at
  the bottom).

  ---------------- PIN MAP ----------------
    Keypad Rows       -> D9, D8, D7, D6
    Keypad Cols        -> D5, D4, D3, D2
    Servo (lock)        -> D11
    Buzzer               -> D12
    Green LED             -> A1
    Red LED                -> A2
    LCD (I2C)                -> SDA A4, SCL A5
  ==========================================

  Required libraries (Arduino Library Manager):
    - Keypad            (Mark Stanley / Alexander Brevig)
    - LiquidCrystal_I2C  (Frank de Brabander)
    - Servo              (bundled with Arduino IDE)
*/

#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- Keypad Setup ----------
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ---------- LCD Setup ----------
LiquidCrystal_I2C lcd(0x27, 16, 2); // use 0x3F if 0x27 doesn't work on your module

// ---------- Servo / Lock Setup ----------
const int SERVO_PIN     = 11;
const int UNLOCK_ANGLE   = 90;
const int LOCK_ANGLE     = 0;
Servo lockServo;

// ---------- Alarm Setup ----------
const int GREEN_LED_PIN = A1;
const int RED_LED_PIN   = A2;
const int BUZZER_PIN    = 12;

// ---------- Security Parameters ----------
const String PASSWORD = "1234";           // change for your own PIN
const int MAX_PASSWORD_LENGTH = 8;
const int MAX_WRONG_ATTEMPTS  = 3;
const unsigned long AUTO_LOCK_DELAY_MS  = 8000;  // stays unlocked this long
const unsigned long LOCKOUT_DURATION_MS = 10000; // lockout period after 3 fails

// ---------- State Variables ----------
enum SystemState { IDLE, UNLOCKED, LOCKED_OUT };
SystemState state = IDLE;

String enteredPassword = "";
int wrongAttemptCount = 0;
unsigned long unlockTime = 0;
unsigned long lockoutStartTime = 0;

// ---------------------------------------------------
// LCD helper: prints a two-line message, clearing first.
// ---------------------------------------------------
void showMessage(String line1, String line2 = "") {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  if (line2.length() > 0) {
    lcd.setCursor(0, 1);
    lcd.print(line2);
  }
}

void unlockLocker() {
  lockServo.write(UNLOCK_ANGLE);
  state = UNLOCKED;
  unlockTime = millis();
  wrongAttemptCount = 0;

  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  showMessage("Access Granted", "Locker Open");
  Serial.println(F("Access Granted. Locker: UNLOCKED"));
}

void lockLocker() {
  lockServo.write(LOCK_ANGLE);
  state = IDLE;

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);

  showMessage("Locker Locked", "Enter Password");
  Serial.println(F("Locker: LOCKED"));
}

void denyAccess() {
  wrongAttemptCount++;

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);

  Serial.print(F("Access Denied. Wrong attempts: "));
  Serial.println(wrongAttemptCount);

  if (wrongAttemptCount >= MAX_WRONG_ATTEMPTS) {
    triggerLockout();
  } else {
    showMessage("Access Denied", "Try Again");
  }
}

void triggerLockout() {
  state = LOCKED_OUT;
  lockoutStartTime = millis();

  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(RED_LED_PIN, HIGH);

  showMessage("Too Many", "Attempts! Locked");
  Serial.println(F("ALARM: Too many wrong attempts. System locked out."));
}

void endLockout() {
  state = IDLE;
  wrongAttemptCount = 0;
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);

  showMessage("Locker Locked", "Enter Password");
  Serial.println(F("Lockout period ended. Ready for new attempts."));
}

void setup() {
  Serial.begin(9600);

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  lockServo.attach(SERVO_PIN);
  lockServo.write(LOCK_ANGLE);

  lcd.init();
  lcd.backlight();
  showMessage("Anti-Theft", "Locker System");
  delay(1500);
  showMessage("Locker Locked", "Enter Password");

  Serial.println(F("=== Anti-Theft Locker System Ready ==="));
}

void loop() {
  unsigned long now = millis();

  // ---- Auto-lock after unlock timeout (non-blocking) ----
  if (state == UNLOCKED && (now - unlockTime >= AUTO_LOCK_DELAY_MS)) {
    lockLocker();
  }

  // ---- End lockout after its duration elapses (non-blocking) ----
  if (state == LOCKED_OUT && (now - lockoutStartTime >= LOCKOUT_DURATION_MS)) {
    endLockout();
  }

  // ---- Read keypad ----
  char key = keypad.getKey();
  if (!key) return;

  if (state == LOCKED_OUT) {
    // Ignore keypad input entirely during lockout
    showMessage("System Locked", "Please Wait...");
    return;
  }

  if (key == '#') {
    // Submit / verify
    if (enteredPassword.length() == 0) return;

    if (enteredPassword == PASSWORD) {
      unlockLocker();
    } else {
      denyAccess();
    }
    enteredPassword = "";

  } else if (key == '*') {
    // Clear current entry
    enteredPassword = "";
    showMessage("Entry Cleared", "Enter Password");

  } else if (isDigit(key) && enteredPassword.length() < MAX_PASSWORD_LENGTH) {
    enteredPassword += key;

    // Build a masked string of '*' matching entered length
    String masked = "";
    for (unsigned int i = 0; i < enteredPassword.length(); i++) masked += '*';
    showMessage("Enter Password", masked);
  }
}

/*
  ---------------- NOTE: PORTING TO ESP32 ----------------
  ESP32 GPIO numbering differs from UNO. Suggested mapping:
    Keypad rows -> GPIO 13, 12, 14, 27
    Keypad cols -> GPIO 26, 25, 33, 32
    Servo        -> GPIO 18
    Buzzer        -> GPIO 19
    Green LED      -> GPIO 21
    Red LED         -> GPIO 22
    LCD I2C          -> SDA GPIO 21 shared bus is fine; move
                         LED pins if conflicting on your board.
  Keypad.h, Servo.h, and Wire.h all work unchanged on ESP32,
  so only the pin numbers need updating.
  ----------------------------------------------------------
*/
