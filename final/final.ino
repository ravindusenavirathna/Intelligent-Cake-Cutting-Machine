#include <Keypad.h>
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

// ---------- keypad global define ----------
#define ROW_NUM = 4;
#define COLUMN_NUM = 4;

char keys[ROW_NUM][COLUMN_NUM] =
    {{'1', '2', '3', 'A'},
     {'4', '5', '6', 'B'},
     {'7', '8', '9', 'C'},
     {'*', '0', '#', 'D'}};

byte pin_rows[ROW_NUM] = {2, 3, 4, 5};
byte pin_column[COLUMN_NUM] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// ---------- LCD global define ----------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------- ultrasonic global define ----------

#define TRIGGER_PIN1 12
#define ECHO_PIN1 11
#define TRIGGER_PIN2 10
#define ECHO_PIN2 9
#define TRIGGER_PIN3 8
#define ECHO_PIN3 7
#define TRIGGER_PIN4 6
#define ECHO_PIN4 5

#define MAX_DISTANCE 200
#define NUM_READINGS 20

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN4, ECHO_PIN4, MAX_DISTANCE);

int readings1[NUM_READINGS];
int readings2[NUM_READINGS];
int readings3[NUM_READINGS];
int readings4[NUM_READINGS];
int index = 0;

// ---------- weight sensor global define ----------

// ---------- Calculation global variables ----------
int weight = 0;

// ---------- stepper motor global define ----------
#define stepsPerRevolution = 200;
#define dirPin1 = 2;
#define stepPin1 = 3;
#define dirPin2 = 2;
#define stepPin2 = 3;
#define dirPin3 = 2;
#define stepPin3 = 3;

// ---------- function phototype ----------
void keypad_loop();
void LCD_setup();
void ultrasonic_loop();
void weight_sensor_setup();
void weight_sensor_loop();
void calculation();
void stepper_motor_setup();
void stepper_motor_loop();

void setup()
{
    Serial.begin(9600);
    LCD_setup();
    ultrasonic_setup();
    weight_sensor_setup();
}

void loop()
{
    keypad_loop();
    if (weight != 0)
    {
        ultrasonic_loop();
    }
}

void keypad_loop()
{
    static char keyBuffer[5];
    static int keyIndex = 0;
    char key = keypad.getKey();
    if (key)
    {
        if (key == '#')
        {
            keyBuffer[keyIndex] = '\0';
            int number = atoi(keyBuffer);
            lcd.setCursor(0, 1);
            Serial.println(key);
            weight = number;
            Serial.println(weight);
            lcd.print(weight);
            memset(keyBuffer, 0, sizeof(keyBuffer));
            keyIndex = 0;
        }
        else if (key == '*')
        {
            memset(keyBuffer, 0, sizeof(keyBuffer));
            keyIndex = 0;
            lcd.setCursor(0, 1);
            Serial.println(key);
            lcd.print("     ");
        }
        else if (keyIndex < 4)
        {
            keyBuffer[keyIndex] = key;
            keyIndex++;
            lcd.setCursor(keyIndex - 1, 1);
            Serial.println(key);
            lcd.print(key);
        }
    }
}

void LCD_setup()
{
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("Enter Weight : ");
    lcd.backlight();
}

void ultrasonic_loop()
{
    int distance1 = sonar1.ping_cm();
    int distance2 = sonar2.ping_cm();
    int distance3 = sonar3.ping_cm();

    readings1[index] = distance1;
    readings2[index] = distance2;
    readings3[index] = distance3;
    index = (index + 1) % NUM_READINGS;

    if (index == 0)
    {
        for (int i = 0; i < NUM_READINGS - 1; i++)
        {
            for (int j = i + 1; j < NUM_READINGS; j++)
            {
                if (readings1[j] < readings1[i])
                {
                    int temp = readings1[i];
                    readings1[i] = readings1[j];
                    readings1[j] = temp;
                }
                if (readings2[j] < readings2[i])
                {
                    int temp = readings2[i];
                    readings2[i] = readings2[j];
                    readings2[j] = temp;
                }
                if (readings3[j] < readings3[i])
                {
                    int temp = readings3[i];
                    readings3[i] = readings3[j];
                    readings3[j] = temp;
                }
            }
        }

        int q1_index = NUM_READINGS / 4;
        int q3_index = 3 * NUM_READINGS / 4;
        int q1_1 = readings1[q1_index];
        int q3_1 = readings1[q3_index];
        int q1_2 = readings2[q1_index];
        int q3_2 = readings2[q3_index];
        int q1_3 = readings3[q1_index];
        int q3_3 = readings3[q3_index];

        int iqr1 = q3_1 - q1_1;
        int iqr2 = q3_2 - q1_2;
        int iqr3 = q3_3 - q1_3;

        int sum1 = 0;
        int sum2 = 0;
        int sum3 = 0;
        for (int i = 0; i < NUM_READINGS; i++)
        {
            sum1 += readings1[i];
            sum2 += readings2[i];
            sum3 += readings3[i];
        }
        float mean1 = (float)sum1 / NUM_READINGS;
        float mean2 = (float)sum2 / NUM_READINGS;
        float mean3 = (float)sum3 / NUM_READINGS;

        Serial.println("Sensor 1");
        Serial.print("Inter quartile Range: ");
        Serial.println(iqr1);
        Serial.print("Mean Value: ");
        Serial.println(mean1);

        Serial.println("Sensor 2");
        Serial.print("Inter quartile Range: ");
        Serial.println(iqr2);
        Serial.print("Mean Value: ");
        Serial.println(mean2);

        Serial.println("Sensor 3");
        Serial.print("Inter quartile Range: ");
        Serial.println(iqr3);
        Serial.print("Mean Value: ");
        Serial.println(mean3);
    }

    delay(100);
}

void weight_sensor_setup()
{
    pinMode(ledPin, OUTPUT);
}

void weight_sensor_loop()
{
    int sensorValue = analogRead(weightSensorPin);
    float weight = map(sensorValue, 0, 1023, 0, 5000);

    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" grams");

    if (weight > 2000)
    {
        digitalWrite(ledPin, HIGH);
    }
    else
    {
        digitalWrite(ledPin, LOW);
    }
}

void calculation()
{
}

void stepper_motor_setup()
{
}

void stepper_motor_loop()
{
}
