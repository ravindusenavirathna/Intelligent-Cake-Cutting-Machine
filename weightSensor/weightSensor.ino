// Define the pins
const int weightSensorPin = A0;
const int ledPin = 13;

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);

    // Set the LED pin as an output
    pinMode(ledPin, OUTPUT);
}

void loop()
{
    // Read the value from the weight sensor
    int sensorValue = analogRead(weightSensorPin);

    // Convert the sensor value to weight in grams
    float weight = map(sensorValue, 0, 1023, 0, 5000); // Assuming a 5kg weight sensor range

    // Print the weight to the serial monitor
    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" grams");

    // Turn on the LED if weight exceeds a certain threshold
    if (weight > 2000)
    { // Change the threshold value as needed
        digitalWrite(ledPin, HIGH);
    }
    else
    {
        digitalWrite(ledPin, LOW);
    }

    // Delay for stability
    delay(500);
}
