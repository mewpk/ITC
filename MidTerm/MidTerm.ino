int pinButton[4] = { 4, 5, 6, 7 };
int pinLED[4] = { 11, 10, 9, 8 };

unsigned long buttonState[4];
unsigned long lastButtonState[4] = { 0, 0, 0, 0 };
unsigned long lastDebounceTime[4];
unsigned long reading[4];
int debounceDelay = 50;

unsigned long stateLED[4] = { 0, 0, 0, 0 };

unsigned long bitSave[2][3];
unsigned long bitConvert[1][4] = { { 0, 0, 0, 0 }  };
int check = 0;
int pos;

void checkButton() {
  for (int i = 0; i <= 3; i++) {
    Serial.print("BTN ");
    Serial.print(i + 1);
    Serial.print(" : ");
    Serial.println(digitalRead(pinButton[i]));
    delay(1000);
  }
}
void checkLED() {
  for (int i = 0; i <= 3; i++) {
    digitalWrite(pinLED[i], HIGH);
    delay(1000);
  }
  for (int i = 0; i <= 3; i++) {
    digitalWrite(pinLED[i], LOW);
    delay(1000);
  }
}
void checkBit() {
  for (int i = 0; i <= 1; i++) {
    for (int j = 0; j <= 2; j++) {
      Serial.print(bitSave[i][j]);
    }
  }

  Serial.println();
}
void checkBitConvert() {
  for (int i = 0; i <= 3; i++) {
    // if(bitConvert[0][i] == 0 || bitConvert[0][i] == 1){
    Serial.print(bitConvert[0][i]);
    // }
  }
  Serial.println();
}
void calBit() {
  for (int i = 1; i <= 3; i++) {
    bitConvert[0][i] = int(bitSave[0][i - 1]) + int(bitSave[1][i - 1]);
    if (int(bitConvert[0][i]) == 2) {
      bitConvert[0][i] = 0;
      pos = i - 1;
      bitConvert[0][pos] = int(bitConvert[0][pos]) + 1;
      // Serial.println(bitConvert[0][i+1]);
    }
  }
  checkBitConvert();
}
void showLED() {
  // for (int i = 0; i <= 3; i++) {
  //   digitalWrite(pinLED[i], LOW);
  //   stateLED[i] = 0;
  // }
  for (int i = 0; i <= 3; i++) {
    // Serial.print(bitConvert[0][i]);
    // stateLED[i] = 0;
    if (int(bitConvert[0][i]) == 1) {
      stateLED[i] = 1;
    } else {
      stateLED[i] = 0;
    }
  }
}
int debounce(int button) {
  reading[button] = digitalRead(pinButton[button]);
  if (reading[button] != lastButtonState[button]) {
    lastDebounceTime[button] = millis();
  }
  if (millis() - lastDebounceTime[button] > debounceDelay) {
    if (reading[button] != buttonState[button]) {
      buttonState[button] = reading[button];
      // Serial.println(buttonState[button]);
      if (buttonState[button] == 1) {
        if (button == 3 && check == 0) {
          check = 1;
        }
        if (button == 3 && check == 2) {
          check = 3;
        }
        if (button == 3 && check == 4) {
          check = 5;
        }
        if (button == 3 && check == 6) {

          Serial.println("New Start");
          for (int i = 0; i <= 3; i++) {
            stateLED[i] = 0;
          }
          for (int i = 0; i <= 1; i++) {
            for (int j = 0; j <= 2; j++) {
              bitSave[i][j] = 0;
            }
          }
          for(int i = 0 ; i <= 3 ; i ++){
            bitConvert[0][i] = 0;
          }
          check = 0;
        }
        return 1;
      } else {
        return 0;
      }
    }
  }
  lastButtonState[button] = reading[button];
  return 0;
}
void toggleLED() {
  for (int i = 0; i <= 3; i++) {
    if (stateLED[i] == 1) {
      digitalWrite(pinLED[i], 1);
    } else if (stateLED[i] == 0) {
      digitalWrite(pinLED[i], 0);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i <= 3; i++) {
    pinMode(pinButton[i], INPUT);
    pinMode(pinLED[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  // for (int i = 0; i <= 3; i++) {
  //   debounce(i);
  // }

  for (int i = 0; i <= 2; i++) {
    if (debounce(i)) {
      stateLED[i] = !stateLED[i];
    }
  }

  toggleLED();

  if (debounce(3)) {
    if (check == 1) {
      for (int i = 0; i <= 2; i++) {
        bitSave[0][i] = stateLED[i];
      }
      checkBit();
      check = 2;
    }
    if (check == 3) {
      for (int i = 0; i <= 2; i++) {
        bitSave[1][i] = stateLED[i];
      }
      checkBit();
      check = 4;
    }
    if (check == 5) {
      calBit();
      showLED();
      check = 6;
    }
  }



  // put your main code here, to run repeatedly:
  // checkButton();
  // checkLED();
}
