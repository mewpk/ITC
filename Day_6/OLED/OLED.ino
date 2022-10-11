#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1
#define BTN1 13
#define BTN2 12
#define BTN3 11
#define BTN4 10
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


// change this to make the song slower or faster
int tempo = 108;

// change this to whichever pin you want to use
int buzzer = 11;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {
  
  // Dart Vader theme (Imperial March) - Star wars 
  // Score available at https://musescore.com/user/202909/scores/1141521
  // The tenor saxophone part was used
  
  NOTE_AS4,8, NOTE_AS4,8, NOTE_AS4,8,//1
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,8, NOTE_C5,8, NOTE_C5,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4, //8  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,-8, NOTE_C5,16, 
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C5,-8, NOTE_C5,16,
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  
  NOTE_C6,-8, NOTE_G5,16, NOTE_G5,2, REST,8, NOTE_C5,8,//13
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C6,-8, NOTE_C6,16,
  NOTE_F6,4, NOTE_DS6,8, NOTE_CS6,4, NOTE_C6,8, NOTE_AS5,4, NOTE_GS5,8, NOTE_G5,4, NOTE_F5,8,
  NOTE_C6,1
  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

const unsigned long PADDLE_RATE = 8;
const unsigned long BALL_RATE = 0;
const uint8_t PADDLE_HEIGHT = 5;
uint8_t score_player1 = 0;
uint8_t score_player2 = 0;

uint8_t ball_x = 64, ball_y = 16;
uint8_t ball_dir_x = 1, ball_dir_y = 1;
unsigned long ball_update;

unsigned long paddle_update;

const uint8_t PLAYER_X = 115;
uint8_t player_y = 16;
const uint8_t PLAYER2_X = 12;
uint8_t player2_y = 16;

Adafruit_SSD1306 OLED(OLED_RESET);

// #if (SSD1306_LCDHEIGHT != 64)
// #error ("Heightincorrect,pleasefixAdafruit_SSD1306.h!");
// #endif
int BTN[4] = { 10, 11, 12, 13 };

void win(){
   // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration*0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    
    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}

void CheckBTN() {
  Serial.println("BTN1");
  Serial.println(digitalRead(BTN1));

  Serial.println("BTN2");
  Serial.println(digitalRead(BTN2));

  Serial.println("BTN3");
  Serial.println(digitalRead(BTN3));

  Serial.println("BTN4");
  Serial.println(digitalRead(BTN4));
}
void draw_feild() {
  OLED.drawLine(0, 0, 127, 0, WHITE);
  OLED.drawLine(0, 31, 127, 31, WHITE);
}

void game_init() {
  // delay(2000);
  music();
  OLED.clearDisplay();
  draw_feild();
  OLED.display();
  unsigned long start = millis();
  while (millis() - start < 1000)
    ;
  ball_update = millis();
  paddle_update = ball_update;
}

void music() {

  tone(3, 440);
  unsigned long start = millis();
  while (millis() - start < 20)
    ;
  noTone(3);
}
void show_score() {
  player_y = 16;
  player2_y = 16;
  if (score_player1 == 3) {
    score_player1 = 0;
    score_player2 = 0;
    // win();
    OLED.clearDisplay();
    OLED.setTextColor(WHITE);
    OLED.setCursor(30, 8);
    OLED.setTextSize(1);
    OLED.println("Player 1 WIN !!!");
    OLED.display();
    delay(2000);
    welcome();
  } else if (score_player2 == 3) {
    score_player1 = 0;
    score_player2 = 0;
    // win();
    OLED.clearDisplay();
    OLED.setTextColor(WHITE);
    OLED.setCursor(30, 8);
    OLED.setTextSize(1);
    OLED.println("Player 2 WIN !!!");
    OLED.display();
    delay(2000);
    welcome();
  } else {
    music();
    OLED.clearDisplay();
    OLED.setTextColor(WHITE);
    OLED.setCursor(60, 8);
    OLED.setTextSize(1);
    char str[] = "";
    sprintf(str, "%d : %d", score_player2, score_player1);
    OLED.println(str);
    OLED.display();
    unsigned long start = millis();
    while (millis() - start < 1000)
      ;
    OLED.clearDisplay();
  }
}
void welcome() {

  music();
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  OLED.setCursor(10, 0);
  OLED.setTextSize(1);
  OLED.println("Welcome To");
  OLED.setCursor(10, 15);
  OLED.println("Pong Game !!");
  OLED.display();
  player_y = 16;
  player2_y = 16;
  unsigned long start = millis();
  while (millis() - start < 1000)
    ;
  game_init();
}
void setup() {
  Serial.begin(115200);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  for (int i = 0; i <= 3; i++) {
    pinMode(BTN[i], INPUT);
  }
  welcome();
}

void loop() {
  bool update = false;
  unsigned long time = millis();

  static bool up_state = false;
  static bool down_state = false;
  static bool up2_state = false;
  static bool down2_state = false;

  up_state = (digitalRead(BTN1) == true);
  down_state = (digitalRead(BTN2) == true);
  up2_state = (digitalRead(BTN3) == true);
  down2_state = (digitalRead(BTN4) == true);

  if (time > ball_update) {
    uint8_t new_x = ball_x + ball_dir_x;
    uint8_t new_y = ball_y + ball_dir_y;

    // Increase Score of Player 2
    if (new_x == 0) {
      ++score_player1;
      show_score();

      ball_x = 64, ball_y = 16;
      new_x = ball_x + ball_dir_x;
      new_y = ball_y + ball_dir_y;
      game_init();
    }

    // Increase Score of Player 1
    if (new_x == 127) {
      ++score_player2;
      show_score();
      ball_x = 64, ball_y = 16;
      new_x = ball_x + ball_dir_x;
      new_y = ball_y + ball_dir_y;
      game_init();
    }

    // Check if we hit the horizontal walls.
    if (new_y == 0 || new_y == 31) {
      ball_dir_y = -ball_dir_y;
      new_y += ball_dir_y + ball_dir_y;
      music();
    }

    // Check if we hit the player paddle
    if (new_x == PLAYER_X
        && new_y >= player_y
        && new_y <= player_y + PADDLE_HEIGHT) {
      ball_dir_x = -ball_dir_x;
      new_x += ball_dir_x + ball_dir_x;
      music();
    }
    if (new_x == PLAYER2_X
        && new_y >= player2_y
        && new_y <= player2_y + PADDLE_HEIGHT) {
      ball_dir_x = -ball_dir_x;
      new_x += ball_dir_x + ball_dir_x;
      music();
    }


    OLED.drawPixel(ball_x, ball_y, BLACK);
    OLED.drawPixel(new_x, new_y, WHITE);
    ball_x = new_x;
    ball_y = new_y;

    ball_update += BALL_RATE;

    update = true;
  }

  if (time > paddle_update) {
    paddle_update += PADDLE_RATE;

    // Player paddle
    OLED.drawFastVLine(PLAYER_X, player_y, PADDLE_HEIGHT, BLACK);
    if (up_state && player_y > 1) {
      player_y -= 1;
    }
    if (down_state) {
      player_y += 1;
    }
    up_state = down_state = false;
    if (player_y < 1) player_y = 1;
    if (player_y + PADDLE_HEIGHT >= 31) player_y = 30 - PADDLE_HEIGHT;
    OLED.drawFastVLine(PLAYER_X, player_y, PADDLE_HEIGHT, WHITE);

    //--------------------------------PLayer 2 -----------------------------------//
    OLED.drawFastVLine(PLAYER2_X, player2_y, PADDLE_HEIGHT, BLACK);
    if (up2_state && player2_y > 1) {
      player2_y -= 1;
    }
    if (down2_state ) {
      player2_y += 1;
    }
    up2_state = down2_state = false;

    if (player2_y < 1) player2_y = 1;
    if (player2_y + PADDLE_HEIGHT >= 31) player2_y = 30 - PADDLE_HEIGHT;
    OLED.drawFastVLine(PLAYER2_X, player2_y, PADDLE_HEIGHT, WHITE);

    update = true;
  }

  if (update) {
    OLED.display();
  }
}