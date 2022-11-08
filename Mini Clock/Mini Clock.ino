#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1
#define button1 9
#define button2 10
#define Audio A3
int Hr[2] = { EEPROM.read(0), EEPROM.read(1) };
int Min[2] = { EEPROM.read(2), EEPROM.read(3) };
int Sec[2] = { 0, 0 };
int hour;
int check_state = 0;
char time[6];
int minute;
int second;
int mess = 0;
char massage[20];
int checkMin = 0;
int checkSec = 0;
int button_s1, button_s2;
int delay_ = 50;
const unsigned int X_AXIS_PIN = 2;
const unsigned int Y_AXIS_PIN = 1;
const unsigned int Z_AXIS_PIN = 0;
const unsigned int NUM_AXES = 3;
const unsigned int PINS[NUM_AXES] = { X_AXIS_PIN, Y_AXIS_PIN, Z_AXIS_PIN };
const unsigned int BUFFER_SIZE = 16;
const unsigned int BAUD_RATE = 9600;
int buffer[NUM_AXES][BUFFER_SIZE];
int buffer_pos[NUM_AXES] = { 0 };
Adafruit_SSD1306 OLED(OLED_RESET);

void setup() {
  Serial.begin(9600);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(Audio, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  digitalWrite(Audio, LOW);
}


void loop() {
  Serial.print(Hr[0]);
  Serial.print(Hr[1]);
  Serial.print(":");
  Serial.print(Min[0]);
  Serial.print(Min[1]);
  Serial.print(":");
  Serial.print(Sec[0]);
  Serial.print(Sec[1]);
  Serial.println();



  button_s1 = digitalRead(button1);
  button_s2 = digitalRead(button2);
  //Serial.println(button_s1);

  //SECOND
  if (Sec[0] < 6) {
    Sec[1]++;
    if (Sec[1] == 10) {
      Sec[0]++;
      Sec[1] = 0;
      if (Sec[0] == 6) {
        Sec[0] = 0;
        Sec[1] = 0;
        checkSec = 1;
      }
    }
  }

  //MINUTE
  if (checkSec == 1) {
    if (Min[0] < 6) {
      Min[1]++;
      if (Min[1] == 10) {
        Min[0]++;
        Min[1] = 0;
        if (Min[0] == 6) {
          Min[0] = 0;
          Min[1] = 0;
          checkMin = 1;
        }
      }
    }
    checkSec = 0;
  }

  //HOUR

  if (checkMin == 1) {
    Hr[1]++;
    if (Hr[0] == 2) {
      if (Hr[1] >= 4) {
        Hr[1] = 0;
        Hr[0] = 0;
      }
    } else {
      if (Hr[1] >= 10) {
        Hr[0]++;
        Hr[1] = 0;
      }
    }
    checkMin = 0;
  }

  //input
  get_massage();
  check_save();

  Serial.println(hour);
  Oled();
}

void get_massage() {
  //check serial
  while (Serial.available() > 0) {
    char save = Serial.read();
    if (save != '\n') {
      massage[mess] = (char)save;
      mess++;
    } else {
      massage[mess] = '\0';
      mess = 0;
    }
  }
}
void check_rom() {
  Serial.println("---------------------");
  Serial.print(EEPROM.read(0));
  Serial.print(EEPROM.read(1));
  Serial.print(":");
  Serial.print(EEPROM.read(2));
  Serial.println(EEPROM.read(3));
  delay(1000);
}

void check_save() {
  if (massage[0] == 's') {
    Serial.println('----------Save------------');
    delay(200);
    EEPROM.write(0, (int)massage[2] - 48);
    EEPROM.write(1, (int)massage[3] - 48);
    EEPROM.write(2, (int)massage[5] - 48);
    EEPROM.write(3, (int)massage[6] - 48);
    Hr[0] = (int)massage[2] - 48;
    Hr[1] = (int)massage[3] - 48;
    Min[0] = (int)massage[5] - 48;
    Min[1] = (int)massage[6] - 48;
  }
  if (button_s1 == 1) {
    Serial.println('----------Save------------');
    delay(200);
    EEPROM.write(0, Hr[0]);
    EEPROM.write(1, Hr[1]);
    EEPROM.write(2, Min[0]);
    EEPROM.write(3, Min[1]);
    check_rom();
  }
  massage[0] = ' ';
  //if ((millis()- debouce_time > delay)
  if (button_s2 == 1) {
    //debounce_time = millis();
    check_state = !check_state;
    int timezone = 7;
    if (check_state == 1) {
      Hr[1] = Hr[1] + 7;
      if (Hr[0] >= 2 && Hr[1] >= 4) {
        Hr[0] = 0;
        Hr[1] -= 4;
      }
      if (Hr[0] <= 1 && Hr[1] >= 10) {
        Hr[0]++;
        Hr[1] = Hr[1] - 10;
        if (Hr[0] >= 2 && Hr[1] >= 4) {
          Hr[0] = 0;
          Hr[1] -= 4;
        }
      }
    } else {
      Hr[1] -= 7;
      if (Hr[1] < 0 && Hr[0] >= 1) {
        Hr[0]--;
        Hr[1] += 10;
      }
      if (Hr[1] < 0 && Hr[0] < 1) {
        Hr[0] = 2;
        Hr[1] += 4;
        if (Hr[1] < 0 && Hr[0] >= 1) {
          Hr[0]--;
          Hr[1] += 10;
        }
      }
    }
  }
}



void Oled() {
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  OLED.setCursor(10, 0);
  OLED.setTextSize(2);
  Serial.println(get_x());
  if (get_x() <= 391) {
    OLED.setRotation(2);
  } else if (get_x() >= 400) {
    OLED.setRotation(0);
  }

  OLED.print(Hr[0]);
  OLED.print(Hr[1]);
  OLED.print(":");
  OLED.print(Min[0]);
  OLED.print(Min[1]);
  OLED.display();
}
int get_axis(const int axis) {
  delay(1);
  buffer[axis][buffer_pos[axis]] = analogRead(PINS[axis]);
  buffer_pos[axis] = (buffer_pos[axis] + 1) % BUFFER_SIZE;
  long sum = 0;
  for (unsigned int i = 0; i < BUFFER_SIZE; i++) sum += buffer[axis][i];
  return round(sum / BUFFER_SIZE);
}
int get_x() {
  return get_axis(0);
}
int get_y() {
  return get_axis(1);
}
int get_z() {
  return get_axis(2);
}