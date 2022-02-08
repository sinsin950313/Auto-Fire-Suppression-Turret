#include "MotorDriver.h"
#include "Arduino.h"

MotorDriver::MotorDriver(int firstPin, int secondPin, int pwmPin)
{
    //StreamRecorder::getInstance().print("MotorDriver Initializing");
    _first = firstPin;
    _second = secondPin;
    _pwmPin = pwmPin;

    pinMode(_first, OUTPUT);
    pinMode(_second, OUTPUT);
}

void MotorDriver::setPower(int power)
{
    if(power < 0)
	counterClockWise();
    else
	clockWise();

    power = abs(power);
    if(power > 255)
	power = 255;
    _power = power;

    //String str = "Power : " + String(_power);
    //StreamRecorder::getInstance().print(str);
    analogWrite(_pwmPin, _power);
}

void MotorDriver::clockWise()
{
    //StreamRecorder::getInstance().print("Set Forward");
    digitalWrite(_first, HIGH);
    digitalWrite(_second, LOW);
}

void MotorDriver::counterClockWise()
{
    //StreamRecorder::getInstance().print("Set Backward");
    digitalWrite(_first, LOW);
    digitalWrite(_second, HIGH);
}
