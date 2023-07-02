#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,4);  // set the LCD address to 0x3F for a 16 chars and 2 line display
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


  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  // Print a message on both lines of the LCD.
  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("Hello world!");
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

  int d = (int)duration;

  lcd.setCursor(5,1);   //Set cursor to character 2 on line 0
  lcd.print(d);

  Serial.print("Distance: ");
  Serial.print(d);
  Serial.println(" cm");

  delay(500);
}