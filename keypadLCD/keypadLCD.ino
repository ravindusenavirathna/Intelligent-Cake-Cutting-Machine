// #include <Keypad.h>
// #include <LiquidCrystal_I2C.h>

// const int ROW_NUM = 4;    // four rows
// const int COLUMN_NUM = 4; // four columns

// char keys[ROW_NUM][COLUMN_NUM] = {
//     {'1', '2', '3', 'A'},
//     {'4', '5', '6', 'B'},
//     {'7', '8', '9', 'C'},
//     {'*', '0', '#', 'D'}};

// byte pin_rows[ROW_NUM] = {2, 3, 4, 5};      // connect to the row pinouts of the keypad
// byte pin_column[COLUMN_NUM] = {6, 7, 8, 9}; // connect to the column pinouts of the keypad

// Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address, column count, row count

// void setup()
// {
//     Serial.begin(9600);
//     lcd.begin(16, 2); // initialize the LCD display
//     lcd.print("Enter a 4-digit");
//     lcd.backlight();
//     lcd.setCursor(0, 1);
//     lcd.print("number:");
// }

// void loop()
// {
//     static char keyBuffer[5]; // buffer to store key presses
//     static int keyIndex = 0;  // index of the buffer

//     char key = keypad.getKey();

//     if (key)
//     {
//         if (key == '#')
//         {                                 // '#' key indicates end of number input
//             keyBuffer[keyIndex] = '\0';   // add null character to terminate the string
//             int number = atoi(keyBuffer); // convert the string to an integer

//             lcd.clear(); // clear the LCD display
//             lcd.print("Entered number:");
//             lcd.setCursor(0, 1);
//             lcd.print(number);

//             // Reset the buffer and index for the next input
//             memset(keyBuffer, 0, sizeof(keyBuffer));
//             keyIndex = 0;
//         }
//         else if (key == '*')
//         { // '*' key clears the input
//             memset(keyBuffer, 0, sizeof(keyBuffer));
//             keyIndex = 0;
//             lcd.setCursor(0, 1);
//             lcd.print("           "); // clear the line
//         }
//         else if (keyIndex < 4)
//         { // accept only four key presses
//             keyBuffer[keyIndex] = key;
//             keyIndex++;

//             lcd.setCursor(keyIndex - 1, 1);
//             lcd.print(key); // update the LCD display with the current digit
//         }
//     }
// }
#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
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
Keypad_I2C kypd(makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574);

void setup()
{
    Wire.begin();
    kypd.begin(makeKeymap(keys));
    Serial.begin(9600);
    Serial.println("start");
}

void loop()
{

    Serial.println(entered_weight());
}
int entered_weight()
{
    String inputString;
    // inputString.reserve(2);
    int inputInt;
    while (true)
    {
        char key = kypd.getKey();
        if (key)
        {
            Serial.println(key);
            // important the user should see what they are entering on the screen for each digit
            if (key >= '0' && key <= '9')
            {
                inputString += key;
                // append lcd cursor and then lcd.print(key);
            }
            else if (key == '#')
            {
                if (inputString.length() > 0)
                {
                    inputInt = inputString.toInt();
                    inputString = ""; // clear input
                    if (inputInt >= 0 && inputInt <= 12)
                    {
                        return inputInt;
                    }
                    return -2; // invalid hall
                }
                return -1; // please enter hall number
            }
            else if (key == '*')
            {
                inputString = ""; // clear input
            }
        }
    }
}