int number;
int bitPattern[10] = {
  0B00111111, // 0
  0B00000110, // 1
  0B01111011
};
const byte numPins = 8;
const int segmentPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
void setup()
{
  Serial.begin(19200);
  for (int i = 0; i < numPins; i++)
    pinMode(segmentPins[i], OUTPUT);
}
void loop()
{
  boolean isBitSet;
  for (int i = 0; i < (sizeof(bitPattern) / sizeof(int)); i++) {
    for (int segment = 0; segment < 8; segment++)
    {
      isBitSet = !bitRead(bitPattern[i], segment);
      digitalWrite(segmentPins[segment], isBitSet);
    }
    delay(500);
  }
}
