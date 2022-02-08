#include "AngleMotor.h"
#include "..\utility\IOModule.h"

void AngleMotor::setAngle(float angle)
{
    angle -= _angle;
    addAngle(angle);
}

void AngleMotor::addAngle(float addAngle)
{
    _angle = modAngle((float)(_angle + addAngle));
    setting(_angle);
}

void AngleMotor::changeCurrentAngle(float angle)
{
    _angle = modAngle(angle);
}

float AngleMotor::getAngle()
{
    return _angle;
}
