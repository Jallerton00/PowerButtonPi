#include <Arduino.h>

const int powerButtonPin = 1;
const int powerLEDPin = 5;
const int LEDPin[] = {18, 19, 20, 21};

const int switchOutput = 22;
const int pwrLEDInput = 17;
const int hddLEDInput = 16;

constexpr int numberOfLEDs = (sizeof(LEDPin) / sizeof(int));

bool powerButtonState = false;
bool previousPowerButtonState = false;

bool reportedPCState = false;

bool ledDirectionUp = true;

int LEDToControl = 0;

void setLED(uint8_t index, bool on)
{
    digitalWrite(LEDPin[index], !on);
    // LEDs are powered on when GPIO pulled low
    // Therefore LED is on when digitalWrite(LEDPin[0], false);
}

void setup()
{

    pinMode(powerButtonPin, INPUT_PULLUP);
    pinMode(pwrLEDInput, INPUT);
    pinMode(hddLEDInput, INPUT);
    pinMode(powerLEDPin, OUTPUT);

    pinMode(switchOutput, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    for (int i = 0; i < numberOfLEDs; i++)
    {
        pinMode(LEDPin[i], OUTPUT);
        setLED(i, false);
    }

    
}

void loop()
{
    // read (and flip as active low) latching input
    powerButtonState = !digitalRead(powerButtonPin);

    reportedPCState = digitalRead(pwrLEDInput);

    // IF powerButtonState changes, we've pressed the button
    // AND the new state is not the same as the one reported by the PC Power LED
    // THEN temporarily press motherboard power
    if ((previousPowerButtonState != powerButtonState) && (powerButtonState!=reportedPCState))
    {
        digitalWrite(switchOutput, true);
    }
    else
    {
        digitalWrite(switchOutput, false);
    }
    
    digitalWrite(LED_BUILTIN, reportedPCState);
    setLED(2, reportedPCState);
    setLED(3, digitalRead(hddLEDInput));

    // LED on switch should copy powerButtonState
    digitalWrite(powerLEDPin, powerButtonState);

    // animate other LEDs. For some reason
    for (int i = 0; i < 2; i++)
    {
        if (i == LEDToControl)
        {
            setLED(i, true);
        }
        else
        {
            setLED(i, false);
        }
    }

    if (powerButtonState) //only animate when power is on
    {
        if (ledDirectionUp)
        {
            LEDToControl += 1;
        }
        else
        {
            LEDToControl -= 1;
        }
        
        // if reach ends, flip direction
        if ((LEDToControl == 2 - 1) || LEDToControl == 0)
        {
            ledDirectionUp = !ledDirectionUp;
        }
        
    }
    
    // delay for animations and switch output.
    // This may need splitting into millis vars rather than blocking with delay
    delay(200);

    previousPowerButtonState = powerButtonState;
}