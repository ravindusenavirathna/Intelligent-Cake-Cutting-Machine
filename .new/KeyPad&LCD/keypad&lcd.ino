#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#define I2CADDR 0x20
#include <LiquidCrystal_I2C.h>

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

LiquidCrystal_I2C lcd(0x27, 16, 4);
int p = 10;
int weight;
void setup()
{
    Serial.begin(9600);
    Wire.begin();
    keypad.begin(makeKeymap(keys));
    Serial.begin(9600);
    Serial.println("start");

    lcd.init();
    lcd.clear();
    lcd.backlight();

    lcd.setCursor(2, 0);
    lcd.print("WEIGHT : ");
}

void loop()
{
    weight = entered_weight();
    Serial.println(weight);
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
                lcd.print("    ");
                p = 9;
            }
            p++;
        }
    }
}