#include <Arduino.h>

const int powerButtonPin = 1;
const int powerLEDPin = 5;
const int LEDPin[] = {18, 19, 20, 21};

const int switchOutput = 16; // bottom right

constexpr int numberOfLEDs = (sizeof(LEDPin) / sizeof(int));

bool powerButtonState = false;
bool previousPowerButtonState = false;

int LEDToControl = 0;

void setup()
{
    pinMode(powerButtonPin, INPUT_PULLUP);
    pinMode(powerLEDPin, OUTPUT);
    pinMode(switchOutput, OUTPUT);

    for (int i = 0; i < numberOfLEDs; i++)
    {
        pinMode(LEDPin[i], OUTPUT);
        digitalWrite(LEDPin[i], false);
    }

    // LEDs are powered on when GPIO pulled low
    // Therefore LED is on when digitalWrite(LEDPin[0], false);
}

void loop()
{
    powerButtonState = !digitalRead(powerButtonPin);

    // if powerButtonState changes, we've pressed the button, therefore temporarily press motherboard power
    if (previousPowerButtonState != powerButtonState)
    {
        digitalWrite(switchOutput, true);
        previousPowerButtonState = powerButtonState;
    }
    else
    {
        digitalWrite(switchOutput, false);
    }

    // LED on switch should copy powerButtonState
    digitalWrite(powerLEDPin, powerButtonState);

    // animate other LEDs. For some reason
    for (int i = 0; i < numberOfLEDs; i++)
    {
        if (i == LEDToControl)
        {
            digitalWrite(LEDPin[i], false);
        }
        else
        {
            digitalWrite(LEDPin[i], true);
        }
    }

    if (powerButtonState) //only animate when power is on
    {
        LEDToControl += 1;
        if (LEDToControl == numberOfLEDs)
        {
            LEDToControl = 0;
        }
    }
    
    // delay for animations and switch output.
    // This may need splitting into millis vars rather than blocking with delay
    delay(200);
}