#include "StepMotorDriver.h"
#include "..\utility\IOModule.h"
#include "Arduino.h"

StepMotorDriver::StepMotorDriver(int step, int gearRatio, int enablePin, int directionPin, int pulsePin) : StepperMotor(step, gearRatio)
{
    _enaPin = enablePin;
    _dirPin = directionPin;
    _pulsePin = pulsePin;

    pinMode(_enaPin, OUTPUT);
    pinMode(_dirPin, OUTPUT);
    pinMode(_pulsePin, OUTPUT);
    digitalWrite(_enaPin, LOW);

    setRotatePerMinute(3);
}

void StepMotorDriver::setRotatePerMinute(int rpm)
{
    float degreePerSec = rpm * 360.0f / (60.0f * 1000.0f * 1000.0f);
    float degreePerPulse = 360.0f / (getStep() * getGearRatio());
    _pulseDelay = degreePerPulse / degreePerSec;
    _pulseDelay /= 2;
}

void StepMotorDriver::rotate(int stepsToMove)
{
    digitalWrite(_enaPin, LOW);

    if(stepsToMove < 0)
    {
	digitalWrite(_dirPin, LOW);
	stepsToMove = -stepsToMove;
    }
    else
	digitalWrite(_dirPin, HIGH);

    for(int i = 0; i < stepsToMove; i++)
    {
	digitalWrite(_pulsePin, HIGH);
	if(_pulseDelay > 16383)//referecen from arduino delaymicroseconds
	    delay(_pulseDelay / 1000);
	else
	    delayMicroseconds(_pulseDelay);

	digitalWrite(_pulsePin, LOW);

	if(_pulseDelay > 16383)
	    delay(_pulseDelay / 1000);
	else
	    delayMicroseconds(_pulseDelay);
    }

    //digitalWrite(_enaPin, HIGH);
}
