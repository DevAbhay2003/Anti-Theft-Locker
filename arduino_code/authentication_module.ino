/*
  =====================================================
  ANTI-THEFT LOCKER - MODULE B: AUTHENTICATION
  =====================================================
  Purpose : Compare an entered password against the
            stored password, and track wrong-attempt
            count with a lockout threshold.

  NOTE: Hardcoding the password below is acceptable for
  a beginner/simulation project only. For a real deployment,
  store it in EEPROM (see docs/testing_strategy.md and
  README "Security Logic" section) so it can be changed
  without re-flashing firmware, and never print it to
  Serial/LCD.
  =====================================================
*/

const String PASSWORD = "1234";

const int MAX_WRONG_ATTEMPTS = 3;
int wrongAttemptCount = 0;
bool isLockedOut = false;

// ---------------------------------------------------
// Returns true if the entered password matches the
// stored password. Resets the wrong-attempt counter
// on success.
// ---------------------------------------------------
bool verifyPassword(String entered) {
  if (isLockedOut) {
    Serial.println(F("System is in lockout. Try again later."));
    return false;
  }

  if (entered == PASSWORD) {
    wrongAttemptCount = 0;
    Serial.println(F("Access Granted."));
    return true;
  } else {
    wrongAttemptCount++;
    Serial.print(F("Access Denied. Wrong attempts: "));
    Serial.println(wrongAttemptCount);

    if (wrongAttemptCount >= MAX_WRONG_ATTEMPTS) {
      isLockedOut = true;
      Serial.println(F("Too many attempts! Locking out."));
    }
    return false;
  }
}

// ---------------------------------------------------
// Call periodically to release a lockout after the
// configured lockout period has elapsed.
// ---------------------------------------------------
unsigned long lockoutStartTime = 0;
const unsigned long LOCKOUT_DURATION_MS = 10000; // 10s for demo purposes

void updateLockoutState() {
  if (isLockedOut && lockoutStartTime == 0) {
    lockoutStartTime = millis();
  }
  if (isLockedOut && (millis() - lockoutStartTime >= LOCKOUT_DURATION_MS)) {
    isLockedOut = false;
    wrongAttemptCount = 0;
    lockoutStartTime = 0;
    Serial.println(F("Lockout period ended. System reset."));
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Authentication Module Ready. Send a password over Serial."));
}

void loop() {
  updateLockoutState();

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    verifyPassword(input);
  }
}
