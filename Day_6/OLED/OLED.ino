#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1
Adafruit_SSD1306 OLED(OLED_RESET);

// #if (SSD1306_LCDHEIGHT != 64)
// #error ("Heightincorrect,pleasefixAdafruit_SSD1306.h!");
// #endif
void setup() {
  Serial.begin(115200);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}
void loop() {
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  OLED.setCursor(10, 0);
  OLED.setTextSize(2);
  OLED.println("Hello");
  OLED.setCursor(10, 15);
  OLED.println("World!");
  OLED.display();
}