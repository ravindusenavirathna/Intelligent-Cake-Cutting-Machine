# Intelligent Cake Cutting Machine

An **Arduino-powered machine** designed to cut cakes into precise, user-defined portions by weight. This project automates cake slicing, ensuring each piece has a consistent weight for accuracy and uniformity, which is ideal for bakeries, restaurants, and home bakers looking to streamline the cake-cutting process.

## Table of Contents

1. [**`Project Overview`**](#project-overview)
2. [**`Features`**](#features)
3. [Components](#components)
4. [Installation](#installation)
5. [Usage](#usage)
6. [Code Structure](#code-structure)
7. [Future Enhancements](#future-enhancements)
8. [License](#license)

---

## Project Overview

The Intelligent Cake Cutting Machine is a microcontroller-based project that simplifies cake cutting by dividing it into equal, user-specified portions with accurate weight control. Using an Arduino, the machine leverages sensors to weigh each slice, ensuring consistency and precision. Users input the desired number of pieces and the weight per piece, and the machine automatically adjusts the cutting mechanism to match.

## Features

- **Customizable Slicing:** Specify the number of slices and target weight per slice.
- **Weight Consistency:** Sensors ensure each portion has an accurate, consistent weight.
- **User-Friendly Interface:** Easy-to-navigate display and controls for setting slicing preferences.
- **Automated Cutting:** Once set, the machine performs all cutting operations autonomously.

## Components

1. **Arduino Microcontroller:** Controls the cutting process and receives user inputs.
2. **Load Cell/Weight Sensors:** Measures the weight of each slice to ensure precision.
3. **Stepper Motor / Servo Motor:** Drives the cutting blade to achieve the required portion sizes.
4. **LCD Display:** Displays user options and feedback (number of pieces, weight per piece).
5. **Power Supply:** Provides necessary power for motors and sensors.

## Installation

### Hardware Setup

1. **Assemble the Components:** Attach the load cell to the cake platform, and connect the cutting blade to the motor.
2. **Connect to Arduino:** Wire the sensors, motor, and display to the Arduino as specified in the circuit diagram.
3. **Power Up:** Ensure the Arduino and all components are securely powered.

### Software Setup

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/ravindusenavirathna/IntelligentCakeCuttingMachine.git
2. **Upload to Arduino**

- Open the code in the Arduino IDE.
- Connect the Arduino to your computer and upload the code.

3. **Library Dependencies**

- Ensure the necessary libraries for the load cell and display are installed (e.g., `HX711` for the load cell).

## Usage

1. **Set Target Pieces and Weight:**
   - Use the display and buttons to enter the number of pieces and desired weight for each slice.
2. **Start Cutting:**
   - Initiate the process, and the machine will begin cutting the cake into the specified portions.
3. **Monitor Progress:**
   - The display will show each slice’s weight in real-time to confirm accuracy.

## Code Structure

- `main.ino` - Contains the core code to control the sensors, motor, and display.
- `loadcell.h/cpp` - Manages data from the weight sensor (e.g., `HX711` library).
- `motor_control.h/cpp` - Controls motor movements for accurate slicing.
- `display.h/cpp` - Interfaces with the display to show user inputs and progress.

## Future Enhancements

- **Additional Portion Sizes:** Allow more granular control of slice sizes.
- **Recipe Integration:** Program common cake weights and slice recommendations.
- **Mobile App Interface:** Enable remote control and monitoring through a smartphone app.

## License

This project is licensed under the MIT License. See the [**`LICENSE`**](LICENSE) file for details.
