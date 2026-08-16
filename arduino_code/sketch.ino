/*
  =====================================================
  ANTI-THEFT LOCKER SYSTEM - WOKWI FIXED VERSION
  =====================================================

  Arduino UNO PIN MAP

  Keypad Rows:
    R1 -> D9
    R2 -> D8
    R3 -> D7
    R4 -> D6

  Keypad Columns:
    C1 -> D5
    C2 -> D4
    C3 -> D3
    C4 -> D2

  Servo:
    Signal -> D11
    VCC    -> 5V
    GND    -> GND

  Buzzer:
    + -> D12
    - -> GND

  Green LED:
    A1 -> 220R -> LED -> GND

  Red LED:
    A2 -> 220R -> LED -> GND

  LCD1602 I2C:
    SDA -> A4
    SCL -> A5
    VCC -> 5V
    GND -> GND

  PASSWORD: 1234
*/

// =====================================================
// LIBRARIES
// =====================================================

#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// =====================================================
// KEYPAD
// =====================================================

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// =====================================================
// LCD
// =====================================================

LiquidCrystal_I2C lcd(0x27, 16, 2);

// =====================================================
// SERVO
// =====================================================

const int SERVO_PIN = 11;

const int UNLOCK_ANGLE = 90;
const int LOCK_ANGLE = 0;

Servo lockServo;

// =====================================================
// ALARM
// =====================================================

const int GREEN_LED_PIN = A1;
const int RED_LED_PIN = A2;
const int BUZZER_PIN = 12;

// =====================================================
// SECURITY SETTINGS
// =====================================================

const String PASSWORD = "1234";

const int MAX_PASSWORD_LENGTH = 8;
const int MAX_WRONG_ATTEMPTS = 3;

const unsigned long AUTO_LOCK_DELAY_MS = 8000;
const unsigned long LOCKOUT_DURATION_MS = 10000;

// =====================================================
// SYSTEM STATE
// IMPORTANT:
// Do NOT use the name IDLE.
// Keypad.h already defines IDLE.
// =====================================================

enum SystemState {
  LOCKER_IDLE,
  UNLOCKED,
  LOCKED_OUT
};

SystemState state = LOCKER_IDLE;

// =====================================================
// VARIABLES
// =====================================================

String enteredPassword = "";

int wrongAttemptCount = 0;

unsigned long unlockTime = 0;
unsigned long lockoutStartTime = 0;

// =====================================================
// LCD MESSAGE FUNCTION
// =====================================================

void showMessage(String line1, String line2 = "") {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(line1);

  if (line2.length() > 0) {
    lcd.setCursor(0, 1);
    lcd.print(line2);
  }
}

// =====================================================
// UNLOCK LOCKER
// =====================================================

void unlockLocker() {

  lockServo.write(UNLOCK_ANGLE);

  state = UNLOCKED;

  unlockTime = millis();

  wrongAttemptCount = 0;

  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  showMessage(
    "Access Granted",
    "Locker Open"
  );

  Serial.println(
    F("Access Granted. Locker: UNLOCKED")
  );
}

// =====================================================
// LOCK LOCKER
// =====================================================

void lockLocker() {

  lockServo.write(LOCK_ANGLE);

  state = LOCKER_IDLE;

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  showMessage(
    "Locker Locked",
    "Enter Password"
  );

  Serial.println(
    F("Locker: LOCKED")
  );
}

// =====================================================
// DENY ACCESS
// =====================================================

void denyAccess() {

  wrongAttemptCount++;

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);

  Serial.print(
    F("Access Denied. Wrong attempts: ")
  );

  Serial.println(wrongAttemptCount);

  // Three wrong attempts
  if (wrongAttemptCount >= MAX_WRONG_ATTEMPTS) {

    triggerLockout();

  } else {

    showMessage(
      "Access Denied",
      "Try Again"
    );

    // Keep red LED visible briefly
    delay(500);

    digitalWrite(RED_LED_PIN, LOW);
  }
}

// =====================================================
// TRIGGER LOCKOUT
// =====================================================

void triggerLockout() {

  state = LOCKED_OUT;

  lockoutStartTime = millis();

  digitalWrite(GREEN_LED_PIN, LOW);

  digitalWrite(RED_LED_PIN, HIGH);

  digitalWrite(BUZZER_PIN, HIGH);

  showMessage(
    "Too Many",
    "Attempts! Locked"
  );

  Serial.println(
    F("ALARM: Too many wrong attempts. System locked out.")
  );
}

// =====================================================
// END LOCKOUT
// =====================================================

void endLockout() {

  state = LOCKER_IDLE;

  wrongAttemptCount = 0;

  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  showMessage(
    "Locker Locked",
    "Enter Password"
  );

  Serial.println(
    F("Lockout period ended. Ready for new attempts.")
  );
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);

  // Output pins
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initial outputs
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Servo
  lockServo.attach(SERVO_PIN);

  lockServo.write(LOCK_ANGLE);

  // LCD
  lcd.init();
  lcd.backlight();

  // Startup screen
  showMessage(
    "Anti-Theft",
    "Locker System"
  );

  delay(1500);

  // Idle screen
  showMessage(
    "Locker Locked",
    "Enter Password"
  );

  Serial.println(
    F("=== Anti-Theft Locker System Ready ===")
  );
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  unsigned long now = millis();

  // ---------------------------------------------------
  // AUTO LOCK
  // ---------------------------------------------------

  if (
    state == UNLOCKED &&
    (now - unlockTime >= AUTO_LOCK_DELAY_MS)
  ) {

    lockLocker();
  }

  // ---------------------------------------------------
  // END LOCKOUT
  // ---------------------------------------------------

  if (
    state == LOCKED_OUT &&
    (now - lockoutStartTime >= LOCKOUT_DURATION_MS)
  ) {

    endLockout();
  }

  // ---------------------------------------------------
  // READ KEYPAD
  // ---------------------------------------------------

  char key = keypad.getKey();

  if (!key) {
    return;
  }

  // ---------------------------------------------------
  // LOCKOUT
  // ---------------------------------------------------

  if (state == LOCKED_OUT) {

    showMessage(
      "System Locked",
      "Please Wait..."
    );

    return;
  }

  // ---------------------------------------------------
  // SUBMIT PASSWORD
  // # BUTTON
  // ---------------------------------------------------

  if (key == '#') {

    // Don't submit an empty password
    if (enteredPassword.length() == 0) {
      return;
    }

    // Correct password
    if (enteredPassword == PASSWORD) {

      unlockLocker();

    }

    // Wrong password
    else {

      denyAccess();
    }

    // Clear entered password
    enteredPassword = "";
  }

  // ---------------------------------------------------
  // CLEAR PASSWORD
  // * BUTTON
  // ---------------------------------------------------

  else if (key == '*') {

    enteredPassword = "";

    showMessage(
      "Entry Cleared",
      "Enter Password"
    );
  }

  // ---------------------------------------------------
  // DIGIT ENTRY
  // ---------------------------------------------------

  else if (
    isDigit(key) &&
    enteredPassword.length() < MAX_PASSWORD_LENGTH
  ) {

    enteredPassword += key;

    // Create masked password
    String masked = "";

    for (
      unsigned int i = 0;
      i < enteredPassword.length();
      i++
    ) {

      masked += '*';
    }

    showMessage(
      "Enter Password",
      masked
    );
  }
}