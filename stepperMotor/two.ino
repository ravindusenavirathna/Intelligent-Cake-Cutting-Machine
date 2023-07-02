// #define stepPin 3
// #define dirPin 2
// #define stepPin2 5
// #define dirPin2 4
// int step = 5000;

// void setup()
// {
//     // Sets the two pins as Outputs
//     pinMode(stepPin, OUTPUT);
//     pinMode(dirPin, OUTPUT);
//     pinMode(stepPin2, OUTPUT);
//     pinMode(dirPin2, OUTPUT);
// }
// void loop()
// {
//     digitalWrite(dirPin, HIGH);  // Enables the motor to move in a particular direction
//     digitalWrite(dirPin2, HIGH); // Enables the motor to move in a particular direction
//     // Makes 200 pulses for making one full cycle rotation
//     for (int x = 0; x < step; x++)
//     {
//         digitalWrite(stepPin, HIGH);
//         digitalWrite(stepPin2, HIGH);
//         delayMicroseconds(500); // by changing this time delay between the steps we can change the rotation speed
//         digitalWrite(stepPin, LOW);
//         digitalWrite(stepPin2, LOW);
//         delayMicroseconds(500);
//     }
//     delay(1000); // One second delay

//     digitalWrite(dirPin, LOW);  // Changes the rotations direction
//     digitalWrite(dirPin2, LOW); // Changes the rotations direction
//     // Makes 400 pulses for making two full cycle rotation
//     for (int x = 0; x < step; x++)
//     {
//         digitalWrite(stepPin, HIGH);
//         digitalWrite(stepPin2, HIGH);
//         delayMicroseconds(500);
//         digitalWrite(stepPin, LOW);
//         digitalWrite(stepPin2, LOW);
//         delayMicroseconds(500);
//     }
//     delay(1000);
// }