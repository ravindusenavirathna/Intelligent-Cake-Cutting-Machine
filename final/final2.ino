// libraries
#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

// weight sensor
#include <HX711_ADC.h>
#if defined(ESP8266) || defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

// pins:
const int HX711_dout = 4; // mcu > HX711 dout pin
const int HX711_sck = 5;  // mcu > HX711 sck pin

// HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;

// motors
#define stepPinHorizontal 7
#define dirPinHorizontal 6
#define stepPinVertical 8
#define dirPinVertical 9

#define speedHorizontal 500
#define speedVertical 800

#define lengthPerStepHorizontal 0.004125 // ################
#define stepVertical 2000                // ################

// ultrasonics
#define Tl 40 // cake length + side ultrasonic reading

#define trigSide 11
#define echoSide 12
// #define MAX_DISTANCE 50
// #define NUM_READINGS 20

// NewPing sonarSide(trigSide, echoSide, MAX_DISTANCE);

// int readingsSide[NUM_READINGS];
// int index = 0;

long duration;
long distance;

// keypad
#define I2CADDR 0x20
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};
Keypad_I2C keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574);

// display
LiquidCrystal_I2C lcd(0x27, 16, 4);

void setup()
{
    Serial.begin(9600);
    keypadAndLCDSetup();
    ultrasonicSetup();
    weightSensorSetup();
    motorSetup();
}

void loop()
{
    keypadAndLCDLoop();
}

void keypadAndLCDSetup()
{
    Wire.begin();
    keypad.begin(makeKeymap(keys));
    Serial.println("start");
    lcd.init();
    lcd.clear();
    lcd.backlight();
}

void ultrasonicSetup()
{
    pinMode(trigSide, OUTPUT);
    pinMode(echoSide, INPUT);
}

void motorSetup()
{
    pinMode(stepPinHorizontal, OUTPUT);
    pinMode(dirPinHorizontal, OUTPUT);
    pinMode(stepPinVertical, OUTPUT);
    pinMode(dirPinVertical, OUTPUT);
}

void weightSensorSetup()
{
  Serial.begin(57600);
    delay(10);
    Serial.println();
    Serial.println("Starting...");

    LoadCell.begin();
    // LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
    float calibrationValue;    // calibration value (see example file "Calibration.ino")
    calibrationValue = 280.91; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266) || defined(ESP32)
    // EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
    EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

    unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
    boolean _tare = true;                 // set this to false if you don't want tare to be performed in the next step
    LoadCell.start(stabilizingtime, _tare);
    if (LoadCell.getTareTimeoutFlag())
    {
        Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
        while (1)
            ;
    }
    else
    {
        LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
        Serial.println("Startup is complete");
    }
}

void keypadAndLCDLoop()
{
    lcd.setCursor(0, 0);
    lcd.print("Select The Type:");
    lcd.setCursor(0, 1);
    lcd.print("A-Slice B-Weight");
    cuttingType();
}

void cuttingType()
{
    char type = keypad.getKey();

    if (type)
    {
        // cutting type A --> slices
        if (type == 'A')
        {
            lcd.clear();
            cuttingTypeA();
        }

        // cutting type B --> weight
        else if (type == 'B')
        {
            lcd.clear();
            cuttingTypeB();
        }

        else
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Invalid Input.");
            delay(1000);
        }
    }
}

void cuttingTypeA()
{
    lcd.setCursor(0, 0);
    lcd.print("-----Type A-----");
    lcd.setCursor(0, 1);
    lcd.print("Slices : ");
    int slices = enteredSlices();
    Serial.println("Slices : ");
    Serial.println(slices);
    int cakeLength = sideUltrasonicLoop();
    Serial.println("Cake Length : ");
    Serial.println(cakeLength);

    int oneSliceLength = cakeLength / slices;
    int stepHorizontal = oneSliceLength / lengthPerStepHorizontal;
    int stepHorizontalTotal = (oneSliceLength * (slices - 1)) / lengthPerStepHorizontal;
    Serial.println("Horizontal Step Length : ");
    Serial.println(stepHorizontal);

    sliceMotor(stepHorizontal, stepHorizontalTotal, slices);
}

void cuttingTypeB()
{
    lcd.setCursor(0, 0);
    lcd.print("-----Type B-----");
    lcd.setCursor(0, 1);
    lcd.print("Weight : ");
    int weight = enteredWeight();
    Serial.println("Entered Weight : ");
    Serial.println(weight);
    int cakeLength = sideUltrasonicLoop();
    Serial.println("Cake Length : ");
    Serial.println(cakeLength);
    int cakeWeight = weightSensorLoop();
    Serial.println("Cake Weigth : ");
    Serial.println(cakeWeight);

    int requiredLength = cakeLength * weight / cakeWeight;
    int stepHorizontal = requiredLength / lengthPerStepHorizontal;
    Serial.println("Horizontal Step Length : ");
    Serial.println(stepHorizontal);

    weightMotor(stepHorizontal);
}

int enteredSlices()
{
    int s = 9;
    String inputString;
    int inputInt;
    while (true)
    {
        char key = keypad.getKey();
        if (key)
        {
            if (key >= '0' && key <= '9')
            {
                Serial.println(key);
                lcd.setCursor(s, 1);
                lcd.print(key);
                inputString += key;
            }
            else if (key == '#')
            {
                if (inputString.length() > 0)
                {
                    inputInt = inputString.toInt();
                    inputString = "";
                    if (inputInt >= 0 && inputInt <= 20)
                    {
                        return inputInt;
                    }
                    return -1;
                }
                return -1;
            }
            else if (key == '*')
            {
                inputString = "";
                lcd.setCursor(9, 1);
                lcd.print("       ");
                s = 8;
            }
            s++;
        }
    }
}

int enteredWeight()
{
    int w = 9;
    String inputString;
    int inputInt;
    while (true)
    {
        char key = keypad.getKey();
        if (key)
        {
            if (key >= '0' && key <= '9')
            {
                Serial.println(key);
                lcd.setCursor(w, 1);
                lcd.print(key);
                inputString += key;
            }
            else if (key == '#')
            {
                if (inputString.length() > 0)
                {
                    inputInt = inputString.toInt();
                    inputString = "";
                    if (inputInt >= 0 && inputInt <= 5000)
                    {
                        return inputInt;
                    }
                    return -1;
                }
                return -1;
            }
            else if (key == '*')
            {
                inputString = "";
                lcd.setCursor(9, 1);
                lcd.print("       ");
                w = 8;
            }
            w++;
        }
    }
}

int weightSensorLoop()
{
    static boolean newDataReady = 0;
    const int serialPrintInterval = 0; // increase value to slow down serial print activity
    float i;
    // check for new data/start next conversion:
    if (LoadCell.update())
        newDataReady = true;

    // get smoothed value from the dataset:
    if (newDataReady)
    {
        if (millis() > t + serialPrintInterval)
        {
            i = LoadCell.getData();
            Serial.print("Load_cell output val: ");
            Serial.println(i);
            newDataReady = 0;
            t = millis();
        }
    }

    // receive command from serial terminal, send 't' to initiate tare operation:
    if (Serial.available() > 0)
    {
        char inByte = Serial.read();
        if (inByte == 't')
            LoadCell.tareNoDelay();
    }

    // check if last tare operation is complete:
    if (LoadCell.getTareStatus() == true)
    {
        Serial.println("Tare complete");
    }
    return i;
}

int sideUltrasonicLoop()
{
    // int sideRead;
    // int distanceSide = sonarSide.ping_cm();
    // readingsSide[index] = distanceSide;
    // index = (index + 1) % NUM_READINGS;

    // if (index == 0)
    // {
    //     for (int i = 0; i < NUM_READINGS - 1; i++)
    //     {
    //         for (int j = i + 1; j < NUM_READINGS; j++)
    //         {
    //             if (readingsSide[j] < readingsSide[i])
    //             {
    //                 int temp = readingsSide[i];
    //                 readingsSide[i] = readingsSide[j];
    //                 readingsSide[j] = temp;
    //             }
    //         }
    //     }
    // }

    // int q1_index = NUM_READINGS / 4;
    // int q2_index = NUM_READINGS / 2;
    // int q3_index = 3 * NUM_READINGS / 4;

    // int q1_Side = readingsSide[q1_index];
    // int q2_Side = readingsSide[q2_index];
    // int q3_Side = readingsSide[q3_index];

    // int iqrSide = q3_Side - q1_Side;

    // int sumSide = 0;
    // for (int i = 0; i < NUM_READINGS; i++)
    // {
    //     sumSide += readingsSide[i];
    // }

    // int meanSide = sumSide / NUM_READINGS;
    // int medianSide = q2_Side;

    // if (iqrSide > 4 || iqrSide < -4)
    // {
    //     sideRead = medianSide;
    // }
    // else
    // {
    //     sideRead = meanSide;
    // }

    digitalWrite(trigSide, LOW);
    delayMicroseconds(2);
    digitalWrite(trigSide, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigSide, LOW);

    duration = pulseIn(echoSide, HIGH);
    distance = duration / 29 / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    delay(500);
    return Tl - distance;
}

void sliceMotor(int stepHorizontal, int stepHorizontalTotal, int slices)
{
    for (int i = 1; i < slices; i++)
    {
        digitalWrite(dirPinHorizontal, HIGH);
        for (int x = 0; x < stepHorizontal; x++)
        {
            digitalWrite(stepPinHorizontal, HIGH);
            delayMicroseconds(speedHorizontal);
            digitalWrite(stepPinHorizontal, LOW);
            delayMicroseconds(speedHorizontal);
        }

        digitalWrite(dirPinVertical, LOW);
        for (int x = 0; x < stepVertical; x++)
        {
            digitalWrite(stepPinVertical, HIGH);
            delayMicroseconds(speedVertical);
            digitalWrite(stepPinVertical, LOW);
            delayMicroseconds(speedVertical);
        }
        delay(1000);

        digitalWrite(dirPinVertical, HIGH);
        for (int x = 0; x < stepVertical; x++)
        {
            digitalWrite(stepPinVertical, HIGH);
            delayMicroseconds(speedVertical);
            digitalWrite(stepPinVertical, LOW);
            delayMicroseconds(speedVertical);
        }
    }

    digitalWrite(dirPinHorizontal, LOW);
    for (int x = 0; x < stepHorizontalTotal; x++)
    {
        digitalWrite(stepPinHorizontal, HIGH);
        delayMicroseconds(speedHorizontal);
        digitalWrite(stepPinHorizontal, LOW);
        delayMicroseconds(speedHorizontal);
    }

    delay(1000);
}

void weightMotor(int stepHorizontal)
{
    digitalWrite(dirPinHorizontal, HIGH);
    for (int x = 0; x < stepHorizontal; x++)
    {
        digitalWrite(stepPinHorizontal, HIGH);
        delayMicroseconds(speedHorizontal);
        digitalWrite(stepPinHorizontal, LOW);
        delayMicroseconds(speedHorizontal);
    }

    digitalWrite(dirPinVertical, LOW);
    for (int x = 0; x < stepVertical; x++)
    {
        digitalWrite(stepPinVertical, HIGH);
        delayMicroseconds(speedVertical);
        digitalWrite(stepPinVertical, LOW);
        delayMicroseconds(speedVertical);
    }
    delay(1000);

    digitalWrite(dirPinVertical, HIGH);
    for (int x = 0; x < stepVertical; x++)
    {
        digitalWrite(stepPinVertical, HIGH);
        delayMicroseconds(speedVertical);
        digitalWrite(stepPinVertical, LOW);
        delayMicroseconds(speedVertical);
    }

    digitalWrite(dirPinHorizontal, LOW);
    for (int x = 0; x < stepHorizontal; x++)
    {
        digitalWrite(stepPinHorizontal, HIGH);
        delayMicroseconds(speedHorizontal);
        digitalWrite(stepPinHorizontal, LOW);
        delayMicroseconds(speedHorizontal);
    }
}
