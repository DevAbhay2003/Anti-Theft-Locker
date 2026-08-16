/*
  =====================================================
  ANTI-THEFT LOCKER - MODULE D: ALARM SYSTEM
  =====================================================
  Purpose : Drive Green/Red LEDs and a buzzer based on
            authentication result and lockout state.
            Standalone-testable via Serial commands:
              'G' -> simulate access granted
              'R' -> simulate access denied
              'L' -> simulate lockout (alarm)
              'I' -> return to idle

  Hardware:
    - Green LED -> A1
    - Red LED   -> A2
    - Buzzer    -> D12
  =====================================================
*/

const int GREEN_LED_PIN = A1;
const int RED_LED_PIN   = A2;
const int BUZZER_PIN    = 12;

void showAccessGranted() {
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void showAccessDenied() {
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
  delay(150); // brief red flash + note - short/local, doesn't block long system logic
  digitalWrite(RED_LED_PIN, LOW);
}

void showLockoutAlarm() {
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
}

void showIdle() {
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  showIdle();
  Serial.println(F("Alarm Module Ready. Send G/R/L/I over Serial to test states."));
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    switch (c) {
      case 'G': case 'g': showAccessGranted(); Serial.println(F("State: GRANTED")); break;
      case 'R': case 'r': showAccessDenied();  Serial.println(F("State: DENIED"));  break;
      case 'L': case 'l': showLockoutAlarm();  Serial.println(F("State: LOCKOUT ALARM")); break;
      case 'I': case 'i': showIdle();          Serial.println(F("State: IDLE")); break;
    }
  }
}
