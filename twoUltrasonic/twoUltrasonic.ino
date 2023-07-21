// Ultrasonic Sensor 1 Pins
const int trigPin1 = 11; // Trigger pin for sensor 1
const int echoPin1 = 12; // Echo pin for sensor 1

// Ultrasonic Sensor 2 Pins
const int trigPin2 = 4; // Trigger pin for sensor 2
const int echoPin2 = 5; // Echo pin for sensor 2

// Variables for Sensor 1
long duration1; // Time taken for the ultrasonic pulse to return for sensor 1
long distance1; // Calculated distance in centimeters for sensor 1

// Variables for Sensor 2
long duration2; // Time taken for the ultrasonic pulse to return for sensor 2
long distance2; // Calculated distance in centimeters for sensor 2

void setup()
{
    // Initialize Serial Monitor
    Serial.begin(9600);

    // Set trigPin1 and trigPin2 as OUTPUT, echoPin1 and echoPin2 as INPUT
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
}

void loop()
{
    // Read values from Sensor 1
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration1 = pulseIn(echoPin1, HIGH);
    distance1 = duration1 / 29 / 2;

    // Read values from Sensor 2
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    duration2 = pulseIn(echoPin2, HIGH);
    distance2 = duration2 / 29 / 2;

    // Print the distances
    Serial.print("Distance from Sensor 1: ");
    Serial.print(distance1);
    Serial.println(" cm");

    Serial.print("Distance from Sensor 2: ");
    Serial.print(distance2);
    Serial.println(" cm");
    Serial.println(" ");

    // Wait for a brief moment before taking the next measurement
    delay(5000);
}
