#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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