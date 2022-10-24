#define debounceTime 80

int pattern[9] = {
  0B00000110,   //1
  0B01011011,   //2
  0B01001111,   //3
  0B01100110,   //4
  0B01101101,   //5
  0B01111101,   //6
  0B000000111,  //7
  0B01111111,   // 8
  0B01101111    // 9
};
int result_pattern[3]  = {
  0B00111111, // 0
  0B00111101, // G
  0B00111000  // L
};

const byte numPins = 8;
const int segmentPins[8] = {3, 4, 5, 6, 9, 10, 11, 12};

int button[2] = {7, 8};
bool buttonState[2];
bool reading[2];
bool lastButtonState[2] = {HIGH, HIGH};

unsigned long long int lastDebounceTime[2];

int counter = 1;
int randomNo;

void setup()
{
  Serial.begin(9600);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  randomSeed(analogRead(A0));
  randomNo = random(1, 10);
  for (int n = 0; n < numPins; n++)
  {
    pinMode(segmentPins[n], OUTPUT);
  }
}

int debounce(int i)
{
  int holding = 0;
  reading[i] = digitalRead(button[i]);
  if (reading[i] != lastButtonState[i])
  {
    lastDebounceTime[i] = millis();
  }
  if ((millis() - lastButtonState[i]) > debounceTime)
  {
    if (reading[i] != buttonState[i])
    {
      buttonState[i] = reading[i];
      holding = 1;
    }
  }
  lastButtonState[i] = reading[i];
  return holding;
}

void displayNum(int i) {
  for (int segment = 0; segment < 8; segment++) {
    int isBitSet = !bitRead(pattern[i - 1], segment);
    digitalWrite(segmentPins[segment], isBitSet);
  }
}

void valid2nDisplay(int i) {
  for (int segment = 0; segment < 8; segment++) {
    int isBitSet = !bitRead(result_pattern[i], segment);
    digitalWrite(segmentPins[segment], isBitSet);
  }
}

void handle_guess_button()
{
  if (debounce(0)) {
    display_num(counter);
    if (!buttonState[0]) {
      counter++;
      if (counter > 9)
      {
        counter = 1;
      }
    }
  }
}

void handle_start_button()
{
  if (debounce(1)) {
    if (!buttonState[1])
    {
      if (counter == randomNo)
      {
        valid2nDisplay(0);
        // reset
        counter = 1;
        randomNo = random(1, 10); 
      }
      if (counter > randomNo)
      {
        valid2nDisplay(1); // G
      }
      if (counter < randomNo)
      {
        valid2nDisplay(2); // L
      }
      delay(500);
      display_num(counter);
    }
  }
}

void loop()
{
  handle_guess_button();
  handle_start_button();
  
}
