#include "Relay.h"
#include "Arduino.h"

Relay::Relay(int pin)
{
    //StreamRecorder::getInstance().print("Relay Initializing");

    _pin = pin;

    pinMode(_pin, OUTPUT);
}

void Relay::on()
{
    //StreamRecorder::getInstance().print("Relay On");
    digitalWrite(_pin, HIGH);
}

void Relay::off()
{
    //StreamRecorder::getInstance().print("Relay Off");
    digitalWrite(_pin, LOW);
}
