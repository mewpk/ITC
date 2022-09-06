#define Button_1 2
#define Button_2 3
#define Button_3 4
#define G 10
#define Y 11
#define R 12
#define Time_Delay 80


bool State_G_Y_R[3] = {LOW, LOW, LOW};=
bool stateButton[3] = {LOW, HIGH, HIGH};
bool WatchBTN[3];
bool State[3] = {LOW, HIGH, HIGH};
int button[3] = {Button_1, Button_2, Button_3};
long int Time[3] = {0, -2000, 0};
unsigned long long int Delay_LastTime[3];
unsigned long long int cooldown[3];


void Delay_Debounce_With_Me(int i)
{
    WatchBTN[i] = digitalRead(button[i]);
    if (WatchBTN[i] != State[i])
        Delay_LastTime[i] = millis();

    if (millis() - Delay_LastTime[i] > Time_Delay)
    {
        stateButton[i] = WatchBTN[i];
    }
    State[i] = WatchBTN[i];
}
void setup()
{
    Serial.begin(9600);
    pinMode(G, OUTPUT);
    pinMode(Y, OUTPUT);
    pinMode(R, OUTPUT);
    pinMode(Button_1, INPUT);        // PULLDOWN
    pinMode(Button_2, INPUT);        // PULLUP
    pinMode(Button_3, INPUT_PULLUP); // PULLUPBUILTIN
}
void loop()
{
    
    Delay_Debounce_With_Me(2); // Button_3 RED
    if (!State_G_Y_R[2] )
    {   Delay_Debounce_With_Me(0); // Button_1 GREEN
        if (!State_G_Y_R[0])
        {
             Delay_Debounce_With_Me(1); // Button_2 YELLOW
        } 
    }

    if (millis() - Time[0] > 3000) // RED
    {
        State_G_Y_R[0] = LOW;
        digitalWrite(G, State_G_Y_R[0]);
    }
    if (millis() - Time[2] > 3000) // GREEN
    {
        State_G_Y_R[2] = LOW;
        digitalWrite(R, State_G_Y_R[2]);
    }
    if (millis() - Time[1] > 2000) // YELLOW
    {
        State_G_Y_R[1] = LOW;
        digitalWrite(Y, State_G_Y_R[1]);
    }
    else
    {
        if (millis() - Time[1] < 500)
        {
            digitalWrite(Y, HIGH);
        }
        else if (millis() - Time[1] < 1000)
        {
           
            digitalWrite(Y, LOW);
        }
        else if (millis() - Time[1] < 1500)
        {
            
            digitalWrite(Y, HIGH);
        }
    }
    // Setting LED
    if (stateButton[0] == HIGH)
    {
        ChangLED(G, 0);
    }

    if (stateButton[1] == LOW)
    {
        ChangLED(Y, 1);
    }

    if (stateButton[2] == LOW)
    {
        ChangLED(R, 2);
    }
}

void ChangLED(int x, int i)                         
    {                                           
        if (millis() - cooldown[i] > Time_Delay) 
        {                                       
            Time[i] = millis();         
            State_G_Y_R[i] = !State_G_Y_R[i];         
            digitalWrite(x, State_G_Y_R[i]);       
        }                                       
        cooldown[i] = millis();                 
    }
