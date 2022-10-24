//----------------------------Master------------------------//
#include <Wire.h>
#define SLAVE_ADDR1 8
char messageR[300];
char empty[10] = "Token#0#";
int mess = 0;
int len;

void setup() {
  // Initialize I2C communications as Master
  Serial.begin(9600);
  Wire.begin();
  setNormal();
}

void getChar(){
  while (Serial.available() > 0){
    char inByte = Serial.read();
    if(inByte != '\n'){
      messageR[mess] = (char)inByte;
      mess++;
    } else {
      len = mess;
      messageR[mess] = '\0';
      mess = 0;
    }
  }
}

int regex(){
  for(int i=0; i<6; i++){
    if(messageR[i] != empty[i]){
      return 0;
    }      
  }
  if((messageR[6]=='0' || messageR[6] == '1') && messageR[7]=='#' && messageR[9]=='#' && messageR[8]<=50 && messageR[8]>=48 ){
    return 1;
  } else {
    return 0;
  }
}

void request(){
  if(Wire.available() > 0){
    int i=0;
    while (Wire.available() > 0){
      int x = Wire.read();
      messageR[i] = (char)x;
      if(messageR[i] == '\0' || x==255){
        messageR[i] = '\0';
        break;
      }
      i++;
    }
    len = i;
    showMessage();
    if(isEmpty()){
      getChar();
      if(!regex()){
        setNormal();
      }
    }
  }
}

void setNormal(){
  for(int i=0; i<8; i++){
    messageR[i] = empty[i];
  }
  messageR[8] = '\0';
}

int isEmpty(){
  for(int i=0; i<8; i++){
    if(messageR[i] != empty[i]){
      return 0;
    }
  }
  return 1;
}

void showMessage(){
  if(messageR[6]=='1' && messageR[8]=='0'){
    for(int i=10; i<len; i++){
      Serial.print(messageR[i]);
    } 
    Serial.println();
    setNormal();
  }
}

void loop() {
  
  if(messageR[0] != '\0'){
    Wire.beginTransmission(SLAVE_ADDR1);
    Wire.write(messageR);
    Wire.endTransmission();
    messageR[0] = '\0';
  }
  delay(300);
  Wire.requestFrom(8, 100);    // request 6 bytes from slave device #8
  request();
  
}