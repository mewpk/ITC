#define button 2
#define ledPin 11
void setup() {
  pinMode(button, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(0, EXTI0_ISR, FALLING);
  Serial.begin(9600);
}

void loop() {
}

void EXTI0_ISR() {
  digitalWrite(ledPin, digitalRead(HIGH));
               // Toggle LED  
   delay(150  );
  digitalWrite(ledPin, digitalRead(LOW));
}
