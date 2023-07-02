#include <NewPing.h>

#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define MAX_DISTANCE 200

#define NUM_READINGS 20

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int readings[NUM_READINGS];
int index = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int distance = sonar.ping_cm();

    // Store the distance in the readings array
    readings[index] = distance;
    index = (index + 1) % NUM_READINGS;

    if (index == 0)
    {
        // Sort the readings array in ascending order
        for (int i = 0; i < NUM_READINGS - 1; i++)
        {
            for (int j = i + 1; j < NUM_READINGS; j++)
            {
                if (readings[j] < readings[i])
                {
                    int temp = readings[i];
                    readings[i] = readings[j];
                    readings[j] = temp;
                }
            }
        }

        // Calculate the quartiles
        int q1_index = NUM_READINGS / 4;
        int q3_index = 3 * NUM_READINGS / 4;
        int q1 = readings[q1_index];
        int q3 = readings[q3_index];

        // Calculate the interquartile range (IQR)
        int iqr = q3 - q1;

        // Print the IQR to the serial monitor
        Serial.print("Interquartile Range: ");
        Serial.println(iqr);
    }

    delay(100);
}
