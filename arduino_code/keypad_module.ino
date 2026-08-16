/*
  =====================================================
  ANTI-THEFT LOCKER - MODULE A: KEYPAD INPUT
  =====================================================
  Purpose : Read digits from a 4x4 matrix keypad, build
            an entered-password string, and echo masked
            (*) characters to Serial for standalone testing.

  Keys:
    0-9 : digits
    *   : clear current entry
    #   : submit current entry (Enter)

  Requires: "Keypad" library by Mark Stanley & Alexander
  Brevig (Arduino Library Manager -> search "Keypad").
  =====================================================
*/

#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};  // connect to keypad R1-R4
byte colPins[COLS] = {5, 4, 3, 2};  // connect to keypad C1-C4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int MAX_PASSWORD_LENGTH = 8;
String enteredPassword = "";

void setup() {
  Serial.begin(9600);
  Serial.println(F("Keypad Module Ready. Enter digits, # to submit, * to clear."));
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == '#') {
      Serial.println();
      Serial.print(F("Submitted: "));
      Serial.println(enteredPassword);
      enteredPassword = "";
    } else if (key == '*') {
      enteredPassword = "";
      Serial.println();
      Serial.println(F("Entry cleared."));
    } else if (isDigit(key) && enteredPassword.length() < MAX_PASSWORD_LENGTH) {
      enteredPassword += key;
      Serial.print('*'); // masked display - never echo the real digit
    }
  }
}
