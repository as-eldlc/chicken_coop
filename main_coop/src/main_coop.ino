/****************************************************************************/
// Authors : Jerome Mazzolini & Antoine Scherrer
/****************************************************************************/
#include <Wire.h>
#include <DS1307.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>


#define SERVO_IDLE_POSITION 92
#define SERVO_CW_POSITION 70
#define SERVO_CCW_POSITION 120

#define CW 0
#define CCW 1

#define STRIP_PIN 10
#define SERVO_PIN 9

#define BUTTON_TOP_PIN 3
#define BUTTON_DOWN_PIN 4
#define BUTTON_LED_PIN 5

#define STOP_TOP_PIN 6
#define STOP_DOWN_PIN 7

#define LED_INTENSITY_R 128
#define LED_INTENSITY_G 128
#define LED_INTENSITY_B 128

#define STATE_DOOR_IDLE 0
#define STATE_DOOR_OPEN 1
#define STATE_DOOR_CLOSE 2
#define STATE_DOOR_OPENED 3
#define STATE_DOOR_CLOSED 4

#define STATE_LED_ON 1
#define STATE_LED_OFF 2

#define TOTAL_LED 15
#define ACTIVE_LED 15

Servo myservo;

DS1307 clock; //RTC
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_LED, STRIP_PIN, NEO_GRB + NEO_KHZ800); // LED STRIP

unsigned int door_state = STATE_DOOR_IDLE;
unsigned int led_state = STATE_LED_OFF;

uint32_t color = 0;
uint32_t black = strip.Color(0, 0, 0);
uint32_t led_color = strip.Color(LED_INTENSITY_R, LED_INTENSITY_G, LED_INTENSITY_B);
bool is_servo_attached = false;
unsigned int cpt = 0;
String err = String("");
int current_time = 0;
int month_part = 0;

// opening/closing hours
int sunrise[12] = {830, 800, 730, 700, 600, 600, 600, 630, 700, 800, 800, 830};
int sunset[3][12] =  {1714, 1756, 1836, 2017, 2056, 2128, 2134, 2109, 2019, 1921, 1727, 1658,
    1726, 1811, 1850, 2031, 2108, 2133, 2130, 2055, 2000, 1902, 1714, 1656,
    1740, 1825, 2003, 2044, 2119, 2134, 2122, 2038, 1940, 1844, 1704, 1658
};

void setup()
{
    // Serial link to display status
    Serial.begin(115200);
    Serial.println("INIT");

    // RTC
    clock.begin();

    // LEDS
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'

    // Buttons
    pinMode(BUTTON_TOP_PIN, INPUT);
    pinMode(BUTTON_DOWN_PIN, INPUT);
    pinMode(BUTTON_LED_PIN, INPUT);

    pinMode(STOP_TOP_PIN, INPUT);
    pinMode(STOP_DOWN_PIN, INPUT);

}

/*Function: Display time on the serial monitor*/
void printState()
{
    Serial.print("RTC clock: ");
    Serial.print(clock.hour, DEC);
    Serial.print(":");
    Serial.print(clock.minute, DEC);
    Serial.print(":");
    Serial.print(clock.second, DEC);
    Serial.print("  ");
    Serial.print(clock.dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(clock.month, DEC);
    Serial.print("/");
    Serial.print(clock.year + 2000, DEC);
    Serial.print(" - ");

    switch (clock.dayOfWeek)// Friendly printout the weekday
    {
        case MON:
        Serial.print("MON");
        break;
        case TUE:
        Serial.print("TUE");
        break;
        case WED:
        Serial.print("WED");
        break;
        case THU:
        Serial.print("THU");
        break;
        case FRI:
        Serial.print("FRI");
        break;
        case SAT:
        Serial.print("SAT");
        break;
        case SUN:
        Serial.print("SUN");
        break;
    }

    Serial.print(" // DOOR: ");
    switch (door_state) {
        case STATE_DOOR_IDLE:
        Serial.print("IDLE");
        break;
        case STATE_DOOR_OPENED:
        Serial.print("OPENED");
        break;
        case STATE_DOOR_CLOSED:
        Serial.print("CLOSED");
        break;
        case STATE_DOOR_OPEN:
        Serial.print("OPENING");
        break;
        case STATE_DOOR_CLOSE:
        Serial.print("CLOSING");
        break;
    }

    Serial.print(" // LED: ");
    switch (led_state) {
        case STATE_LED_OFF:
        Serial.print("OFF ");
        break;
        case STATE_LED_ON:
        Serial.print("ON ");
        break;
    }

    Serial.print(" // STATUS: ");
    Serial.print(err);
    Serial.println("");
}

void motor_step(int way) {
    if (!is_servo_attached) {
        myservo.attach(SERVO_PIN);
        is_servo_attached = true;
    }
    if (way == CW) {
        myservo.write(SERVO_CW_POSITION);
    } else {
        myservo.write(SERVO_CCW_POSITION);
    }
}

void motor_stop() {
    if (is_servo_attached) {
        myservo.write(SERVO_IDLE_POSITION);
        delay(50);
        myservo.detach();
        is_servo_attached = false;
    }
}

void loop()
{
    // Read INPUTS and change state
    clock.getTime();

    // Buttons
    if (digitalRead(BUTTON_TOP_PIN) == HIGH) {
        Serial.println("OPEN");
        if (door_state == STATE_DOOR_CLOSED || door_state == STATE_DOOR_IDLE) {
            door_state = STATE_DOOR_OPEN;
        }
    } else if (digitalRead(BUTTON_DOWN_PIN) == HIGH) {
        Serial.println("CLOSE");
        if (door_state == STATE_DOOR_OPENED || door_state == STATE_DOOR_IDLE) {
            door_state = STATE_DOOR_CLOSE;
        }
    } else if (digitalRead(BUTTON_LED_PIN) == HIGH) {
        Serial.println("LED");
        if (led_state == STATE_LED_OFF) {
            led_state = STATE_LED_ON;
        } else {
            led_state = STATE_LED_OFF;
        }
    }

    // Stops
    if (digitalRead(STOP_TOP_PIN) == HIGH) {
        Serial.println("TOP STOP");
        if (door_state == STATE_DOOR_OPEN){
            door_state = STATE_DOOR_OPENED;
        }
    }

    if (digitalRead(STOP_DOWN_PIN) == LOW) {
        Serial.println("DOWN STOP");
        if (door_state == STATE_DOOR_CLOSE){
            door_state = STATE_DOOR_CLOSED;
        }
    }

    current_time = clock.hour * 100 + clock.minute;
    month_part = (int)(clock.dayOfMonth / 10.5);
    // Check if we need to open/close the door
    if (current_time == sunrise[clock.month - 1]) {
        if (door_state != STATE_DOOR_OPENED && door_state != STATE_DOOR_OPEN) {
            door_state = STATE_DOOR_OPEN;
        }
    }

    if (current_time == sunset[month_part][clock.month - 1]) {
        if (door_state != STATE_DOOR_CLOSED && door_state != STATE_DOOR_CLOSED) {
            door_state = STATE_DOOR_CLOSE;
        }
    }

    // Check if we have a problem...
    if (current_time - 5 > sunrise[clock.month - 1] && current_time + 5 < sunset[month_part][clock.month - 1]) {
        if (door_state != STATE_DOOR_OPENED){
            err = String("PB DOOR NOT OPENED");
        } else {
            err = String("OK");
        }
    } else if (current_time + 5 < sunrise[clock.month - 1] && current_time + 5 > sunset[month_part][clock.month - 1]) {
        if (door_state != STATE_DOOR_CLOSED){
            err = String("PB DOOR NOT CLOSED");
        } else {
            err = String("OK");
        }
    } else {
        err = String("?");
    }

    // FSM actions
    switch (door_state) {
        case STATE_DOOR_IDLE:
        case STATE_DOOR_OPENED:
        case STATE_DOOR_CLOSED:
        motor_stop();
        break;

        case STATE_DOOR_OPEN:
        // move motor to open the door
        motor_step(CW);
        break;

        case STATE_DOOR_CLOSE:
        // move motor to close the door
        motor_step(CCW);
        break;
    }

    switch (led_state) {
        case STATE_LED_OFF:
        color = black;
        break;
        case STATE_LED_ON:
        color = led_color;
        break;
    }
    // Write LED color
    for (uint16_t i = 0; i < TOTAL_LED; i++) {
        if (i <= ACTIVE_LED) {
            strip.setPixelColor(i, color);
        } else {
            strip.setPixelColor(i, black);
        }
    }
    strip.show();

    // display state on serial port
    cpt ++;
    if (cpt > 2){
        printState();
        cpt = 0;
    }
    // Run roughly every 500ms.
    delay(500);

}
