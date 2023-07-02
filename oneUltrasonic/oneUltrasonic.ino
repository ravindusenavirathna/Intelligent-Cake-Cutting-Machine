// Ultrasonic Sensor Pins
const int trigPin = 2;  // Trigger pin
const int echoPin = 4;  // Echo pin

// Variables
long duration;  // Time taken for the ultrasonic pulse to return
long distance;   // Calculated distance in centimeters

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Set trigPin as OUTPUT and echoPin as INPUT
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send a 10us pulse to trigger the ultrasonic module
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the pulse duration on the echo pin
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distance = duration /29 / 2;  // Speed of sound is 343 meters/second or 0.034 cm/microsecond
  
  // Print the distance in centimeters
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Wait for a brief moment before taking the next measurement
  delay(500);
}
