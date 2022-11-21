long buttonstate[3];
long lastbuttonstate[3] = { 1, 1, 1 };
long lastdebouncetime[3];
int debouncedelay = 50;
int reading[3];
int pinbutton[3] = { 7, 8, 9 };
int pinLED[3] = { 10, 11, 12 };
int stateLED[3] = {0,0,0};

int debounce(int button) {
  reading[button] = digitalRead(pinbutton[button]);
  if (reading[button] != lastbuttonstate[button]) {
    lastdebouncetime[button] = millis();
  }
  if (millis() - lastdebouncetime[button] > debouncedelay) {
    if (reading[button] != buttonstate[button]) {
      buttonstate[button] = reading[button];
      // Serial.println(buttonstate[button]);
      if (buttonstate[button] == 0) {
        return 1;
      } 
      else {
        return 0;
      }
    }
  }
  lastbuttonstate[button] = reading[button];
  return 0 ;
}
int priority(int btn) {
  // Serial.println(digitalRead(pinLED[led]));
  int check = 0;
  if(btn == 0){
    check = 1;
  }
  else if(btn == 1){
    if(!digitalRead(pinLED[0])){
      check = 1;
    }
  }
  else if(btn == 2){
    if(!digitalRead(pinLED[0]) && !digitalRead(pinLED[1])){
      check = 1;
    }
  }
  return check;
}

int checkG = 0;
long startG = 0;
void Green()
{
  if(checkG)
  {
    startG = millis();
    checkG = 0;
    digitalWrite(pinLED[2],1);
  }
  if(millis() - startG >= 3000)
  {
    digitalWrite(pinLED[2],LOW);
  }
}

int checkR = 0;
long startR = 0;

void Red(){
  if(checkR){
    startR = millis();
    checkR = 0;
    digitalWrite(pinLED[0],HIGH);
  }

  if(millis() - startR >= 3000){
    digitalWrite(pinLED[0],LOW);
  }
  
}
int checkY= 0;
long startYyai = -3000;
long startYlek = 0;

void Yellow(){
  if(checkY)
  {
    startYyai = millis();
    startYlek = millis();
    checkY = 0;
    //digitalWrite(pinLED[1],1);
  }
  if(millis() - startYyai >= 3000)
  {
    digitalWrite(pinLED[1],0);
  }
  else{
    if(millis() - startYlek >= 1000)
    {
      digitalWrite(pinLED[1],0);
      startYlek = millis();
    }
    else if (millis() - startYlek >= 500)
    {
      digitalWrite(pinLED[1],1);
    }
  }
}

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(pinbutton[i], INPUT_PULLUP);
    pinMode(pinLED[i], OUTPUT);
  }
  Serial.begin(9600);
}
void loop() {

    if (debounce(0) && priority(0)) 
    {
      checkR = 1;   
      Serial.println(debounce(0));
    }
    if (debounce(1) && priority(1)) 
    {
      checkY = 1;   
    }
    if (debounce(2) && priority(2))
    {
      checkG = 1;
      Serial.println("Green");
    }


    Red();
    Yellow();
    Green();
  
}

