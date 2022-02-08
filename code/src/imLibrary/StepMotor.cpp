#include "StepMotor.h"
#include "..\utility\IOModule.h"

StepMotor::StepMotor(int step, int gearRatio, int pinA, int pinA_, int pinB, int pinB_) : StepperMotor(step, gearRatio)
{
    _stepper = new Stepper(step, pinA, pinA_, pinB, pinB_);
    setRotatePerMinute(30);
}

void StepMotor::setRotatePerMinute(int rpm)
{
    _stepper->setSpeed(rpm);
}

void StepMotor::rotate(int stepsToMove)
{
    _stepper->step(stepsToMove);
}
