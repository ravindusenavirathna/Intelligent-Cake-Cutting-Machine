// horizontal
#define stepPin1 3
#define dirPin1 2

int step1 = 400;
int speed1 = 500;

// vertical
#define stepPin2 8
#define dirPin2 9

int step2 = 3000;
int speed2 = 800;

void setup()
{
    // horizontal
    pinMode(stepPin1, OUTPUT);
    pinMode(dirPin1, OUTPUT);

    // vertical
    pinMode(stepPin2, OUTPUT);
    pinMode(dirPin2, OUTPUT);
}
void loop()
{
    for (int i = 0; i < 2; i++)
    {
        digitalWrite(dirPin1, HIGH);
        for (int x = 0; x < step1; x++)
        {
            digitalWrite(stepPin1, HIGH);
            delayMicroseconds(speed1);
            digitalWrite(stepPin1, LOW);
            delayMicroseconds(speed1);
        }

        digitalWrite(dirPin2, HIGH);
        for (int x = 0; x < step2; x++)
        {
            digitalWrite(stepPin2, HIGH);
            delayMicroseconds(speed2);
            digitalWrite(stepPin2, LOW);
            delayMicroseconds(speed2);
        }
        delay(1000);

        digitalWrite(dirPin2, LOW);
        for (int x = 0; x < step2; x++)
        {
            digitalWrite(stepPin2, HIGH);
            delayMicroseconds(speed2);
            digitalWrite(stepPin2, LOW);
            delayMicroseconds(speed2);
        }
    }

    delay(1000);
}