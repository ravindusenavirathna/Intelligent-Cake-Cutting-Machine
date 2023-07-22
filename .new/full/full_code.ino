// libraries
#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

// motors
#define stepPinHorizontal 3
#define dirPinHorizontal 2
#define stepPinVertical 8
#define dirPinVertical 9

// ultrasonics
#define Tw 100 // cake width + front ultrasonic reading
#define Tl 400 // cake length + side ultrasonic reading

#define trigFront 11
#define echoFront 12
#define trigSide 4
#define echoSide 5
#define MAX_DISTANCE 200
#define NUM_READINGS 20

NewPing sonarFront(trigFront, echoFront, MAX_DISTANCE);
NewPing sonarSide(trigSide, echoSide, MAX_DISTANCE);

int readingsFront[NUM_READINGS];
int readingsSide[NUM_READINGS];
int index = 0;

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

// variables
int weight = 0, slices = 0;
int step;

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
    int cakeWidth = frontUltrasonicLoop();
    int cakeLength = sideUltrasonicLoop();
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
    pinMode(trigFront, OUTPUT);
    pinMode(echoFront, INPUT);
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
    slices = enteredSlices();
    Serial.println(slices);
}

void cuttingTypeB()
{
    lcd.setCursor(0, 0);
    lcd.print("-----Type B-----");
    lcd.setCursor(0, 1);
    lcd.print("Weight : ");
    weight = enteredWeight();
    Serial.println(weight);
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

int frontUltrasonicLoop()
{
    int frontRead;
    int distanceFront = sonarFront.ping_cm();
    readingsFront[index] = distanceFront;
    index = (index + 1) % NUM_READINGS;

    if (index == 0)
    {
        for (int i = 0; i < NUM_READINGS - 1; i++)
        {
            for (int j = i + 1; j < NUM_READINGS; j++)
            {
                if (readingsFront[j] < readingsFront[i])
                {
                    int temp = readingsFront[i];
                    readingsFront[i] = readingsFront[j];
                    readingsFront[j] = temp;
                }
            }
        }
    }

    int q1_index = NUM_READINGS / 4;
    int q2_index = NUM_READINGS / 2;
    int q3_index = 3 * NUM_READINGS / 4;

    int q1_Front = readingsFront[q1_index];
    int q2_Front = readingsFront[q2_index];
    int q3_Front = readingsFront[q3_index];

    int iqrFront = q3_Front - q1_Front;

    int sumFront = 0;
    for (int i = 0; i < NUM_READINGS; i++)
    {
        sumFront += readingsFront[i];
    }

    int meanFront = sumFront / NUM_READINGS;
    int medianFront = q2_Front;

    if (iqrFront > 4 || iqrFront < -4)
    {
        frontRead = medianFront;
    }
    else
    {
        frontRead = meanFront;
    }

    return Tw - frontRead;
}

int sideUltrasonicLoop()
{
    int sideRead;
    int distanceSide = sonarSide.ping_cm();
    readingsSide[index] = distanceSide;
    index = (index + 1) % NUM_READINGS;

    if (index == 0)
    {
        for (int i = 0; i < NUM_READINGS - 1; i++)
        {
            for (int j = i + 1; j < NUM_READINGS; j++)
            {
                if (readingsSide[j] < readingsSide[i])
                {
                    int temp = readingsSide[i];
                    readingsSide[i] = readingsSide[j];
                    readingsSide[j] = temp;
                }
            }
        }
    }

    int q1_index = NUM_READINGS / 4;
    int q2_index = NUM_READINGS / 2;
    int q3_index = 3 * NUM_READINGS / 4;

    int q1_Side = readingsSide[q1_index];
    int q2_Side = readingsSide[q2_index];
    int q3_Side = readingsSide[q3_index];

    int iqrSide = q3_Side - q1_Side;

    int sumSide = 0;
    for (int i = 0; i < NUM_READINGS; i++)
    {
        sumSide += readingsSide[i];
    }

    int meanSide = sumSide / NUM_READINGS;
    int medianSide = q2_Side;

    if (iqrSide > 4 || iqrSide < -4)
    {
        sideRead = medianSide;
    }
    else
    {
        sideRead = meanSide;
    }

    return Tl - sideRead;
}