#define goW 0
#define waitW 1
#define goS 2
#define waitS 3
#define goD 4
#define b1 5
#define b2 6
#define b3 7
#define b4 8
#define b5 9


int LED[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };
int lenLED = 7;
int BTN[3] = { 10, 11, 12 };
int lenBTN = 2;

struct State {
  unsigned long ST_Out;  // 6-bit pattern to street output
  unsigned long Time;    // delay in msunits
  unsigned long Next[8];
};  // next state for inputs 0,1,2,3

typedef const struct State SType;


SType FSM[10] = {
  { B10110011, 2000, { goW, waitW, waitW, waitW, goW, waitW, waitW, waitW } },  // goW
  { B10110101, 300, { waitW, goD, goS, goS, waitW, goD, goS, goS } },           //waitW
  { B10011110, 2000, { goS, waitS, goS, waitS, waitS, waitS, waitS, waitS } },  // goS
  { B10101110, 300, { waitS, goD, waitS, goD, goW, goD, goW, goD } },           //waitS
  { B01110110, 2000, { goD, goD, b1, b1, b1, b1, b1, b1 } },                    //goD
  { B11110110, 300, { b2, b2, b2, b2, b2, b2, b2, b2 } },                       //b1
  { B01110110, 300, { b3, b3, b3, b3, b3, b3, b3, b3 } },                       //b2
  { B11110110, 300, { b4, b4, b4, b4, b4, b4, b4, b4 } },                       //b3
  { B01110110, 300, { b5, b5, b5, b5, b5, b5, b5, b5 } },                       //b4
  { B11110110, 300, { goD, goD, goS, goS, goW, goW, goW, goW } }                //b5

};

unsigned long S = 0;

int input, input1, input2, input3;

void check_led() {
  for (int i = 0; i <= lenLED; i++) {
    digitalWrite(LED[i], LOW);
    delay(500);
    digitalWrite(LED[i], HIGH);
    delay(500);
  }
}

void check_btn() {
  for (int i = 0; i <= lenBTN; i++) {
    Serial.print("BTN ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(digitalRead(BTN[i]));
  }
  Serial.println(" ");
}

void traffic() {

  digitalWrite(LED[0], FSM[S].ST_Out & B00000001);
  digitalWrite(LED[1], FSM[S].ST_Out & B00000010);
  digitalWrite(LED[2], FSM[S].ST_Out & B00000100);
  digitalWrite(LED[3], FSM[S].ST_Out & B00001000);
  digitalWrite(LED[4], FSM[S].ST_Out & B00010000);
  digitalWrite(LED[5], FSM[S].ST_Out & B00100000);
  digitalWrite(LED[6], FSM[S].ST_Out & B10000000);
  digitalWrite(LED[7], FSM[S].ST_Out & B01000000);
  delay(FSM[S].Time);
  input1 = digitalRead(BTN[0]);
  input2 = digitalRead(BTN[1]);
  input3 = digitalRead(BTN[2]);
  input = input1 * 4 + input2 * 2 + input3;
  Serial.println(input);
  S = FSM[S].Next[input];
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i <= lenLED; i++) {
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], HIGH);
  }
  for (int i = 0; i <= lenBTN; i++) {
    pinMode(BTN[i], INPUT);
  }
  Serial.begin(9600);
}

void loop() {
  // check_led();
  // check_btn();
  //   input1 = digitalRead(BTN[0]);
  // input2 = digitalRead(BTN[1]);
  // input3 = digitalRead(BTN[2]);
  // input = input3 * 4 + input2 * 2 + input1;
  // Serial.println(input);
  traffic();
}