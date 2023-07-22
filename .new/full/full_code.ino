// libraries
#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// motors
#define stepPinHorizontal 3
#define dirPinHorizontal 2
#define stepPinVertical 8
#define dirPinVertical 9

// ultrasonics
#define trigFront 11
#define echoFront 12
#define trigSide 4
#define echoSide 5

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
int p = 10;
int weight;
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
    weight = entered_weight();
    Serial.println(weight);
    step = weight;

    digitalWrite(dirPin, HIGH);
    for (int x = 0; x < step; x++)
    {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(500);
    }
    delay(1000);

    digitalWrite(dirPin, LOW);
    for (int x = 0; x < step; x++)
    {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(500);
    }
    delay(1000);
}

void keypadAndLCDSetup()
{
    Wire.begin();
    keypad.begin(makeKeymap(keys));
    Serial.println("start");
    lcd.init();
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Select The Type:");
    lcd.setCursor(0, 1);
    lcd.print("A-Slice B-Weight");
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
}

int entered_weight()
{
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
                lcd.setCursor(p, 0);
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
                lcd.setCursor(10, 0);
                lcd.print("       ");
                p = 9;
            }
            p++;
        }
    }
}
