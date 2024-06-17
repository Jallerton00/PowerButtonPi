#include <Arduino.h>

const int switchOutput = 22;

void setButtonState(bool newState)
{
    digitalWrite(switchOutput, newState);
    digitalWrite(LED_BUILTIN, newState);
}

void setup()
{
    pinMode(switchOutput, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    // wait for Mac Mini to have power
    delay(1000);
    
    // press button
    setButtonState(true);

    delay(200);

    // release button
    setButtonState(false);
}

void loop()
{
}