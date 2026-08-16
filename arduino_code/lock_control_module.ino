/*
  =====================================================
  ANTI-THEFT LOCKER - MODULE C: LOCK CONTROL
  =====================================================
  Purpose : Drive a servo motor (standing in for a
            solenoid lock) to an unlocked or locked
            angle, with a non-blocking auto-lock timer.

  Hardware:
    - Servo Signal -> D11
  =====================================================
*/

#include <Servo.h>

const int SERVO_PIN = 11;
const int UNLOCK_ANGLE = 90;
const int LOCK_ANGLE   = 0;
const unsigned long AUTO_LOCK_DELAY_MS = 8000; // stays unlocked for 8s

Servo lockServo;
bool isUnlocked = false;
unsigned long unlockTime = 0;

void unlockLocker() {
  lockServo.write(UNLOCK_ANGLE);
  isUnlocked = true;
  unlockTime = millis();
  Serial.println(F("Locker: UNLOCKED"));
}

void lockLocker() {
  lockServo.write(LOCK_ANGLE);
  isUnlocked = false;
  Serial.println(F("Locker: LOCKED"));
}

void setup() {
  Serial.begin(9600);
  lockServo.attach(SERVO_PIN);
  lockLocker(); // start locked

  Serial.println(F("Lock Control Module Ready. Send 'U' to unlock (simulated correct password)."));
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'U' || c == 'u') {
      unlockLocker();
    }
  }

  // Non-blocking auto-lock after timeout
  if (isUnlocked && (millis() - unlockTime >= AUTO_LOCK_DELAY_MS)) {
    lockLocker();
  }
}
