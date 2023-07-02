#define stepPin 8
#define dirPin 9
int step = 2000;

void setup()
{
    // Sets the two pins as Outputs
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
}
void loop()
{
    digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
    // Makes 200 pulses for making one full cycle rotation
    for (int x = 0; x < step; x++)
    {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500); // by changing this time delay between the steps we can change the rotation speed
        digitalWrite(stepPin, LOW);
        delayMicroseconds(500);
    }
    delay(1000); // One second delay

    digitalWrite(dirPin, LOW); // Changes the rotations direction
    // Makes 400 pulses for making two full cycle rotation
    for (int x = 0; x < step; x++)
    {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(500);
    }
    delay(1000);
}