#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const int ROW_NUM = 4;    // four rows
const int COLUMN_NUM = 4; // four columns

char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte pin_rows[ROW_NUM] = {2, 3, 4, 5};      // connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {6, 7, 8, 9}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address, column count, row count

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2); // initialize the LCD display
    lcd.print("Enter a 4-digit");
    lcd.backlight();
    lcd.setCursor(0, 1);
    lcd.print("number:");
}

void loop()
{
    static char keyBuffer[5]; // buffer to store key presses
    static int keyIndex = 0;  // index of the buffer

    char key = keypad.getKey();

    if (key)
    {
        if (key == '#')
        {                                 // '#' key indicates end of number input
            keyBuffer[keyIndex] = '\0';   // add null character to terminate the string
            int number = atoi(keyBuffer); // convert the string to an integer

            lcd.clear(); // clear the LCD display
            lcd.print("Entered number:");
            lcd.setCursor(0, 1);
            lcd.print(number);

            // Reset the buffer and index for the next input
            memset(keyBuffer, 0, sizeof(keyBuffer));
            keyIndex = 0;
        }
        else if (key == '*')
        { // '*' key clears the input
            memset(keyBuffer, 0, sizeof(keyBuffer));
            keyIndex = 0;
            lcd.setCursor(0, 1);
            lcd.print("           "); // clear the line
        }
        else if (keyIndex < 4)
        { // accept only four key presses
            keyBuffer[keyIndex] = key;
            keyIndex++;

            lcd.setCursor(keyIndex - 1, 1);
            lcd.print(key); // update the LCD display with the current digit
        }
    }
}
