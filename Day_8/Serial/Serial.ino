void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}
// char num[8]  = '';

void loop() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar >= 49 && inChar <= 57) {
      
      Serial.println(inChar);
      for(int i  = 1 ; i <= inChar-48 ;  i++){
        
        digitalWrite(LED_BUILTIN,HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN,LOW);
        delay(500);
      }

    }

  }
}