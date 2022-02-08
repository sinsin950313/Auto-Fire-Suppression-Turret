#include "StepperMotor.h"
#include "..\utility\IOModule.h"

StepperMotor::StepperMotor(int step, int gearRatio) : AngleMotor()
{
    _step = step;
    _gearRatio = gearRatio;
    _anglePerStep = amplifier(360.0f / (float)step);
}

void StepperMotor::setAngleSpeed(int degreePerSecond)
{
    int rpm = (int)((float)degreePerSecond * 60.0f / 360.0f);
    setRotatePerMinute(rpm);
}

void StepperMotor::setting(float angle)
{
    int ampAngle = amplifier(angle);
    int ampBeforeAngle = amplifier(_beforeAngle);
    int standard = 180;

    int temp = 0;
    if(ampAngle > ampBeforeAngle)
    {
	temp = ampAngle - ampBeforeAngle;
	if(temp > standard * _precision)
	    temp = -(ampBeforeAngle + (standard * _precision * 2 - ampAngle));
    }
    if(ampAngle < ampBeforeAngle)
    {
	temp = ampAngle - ampBeforeAngle;
	if(temp < -standard * _precision)
	    temp = (standard * _precision * 2 - ampBeforeAngle) + ampAngle;
    }

    int stepsToMove = temp / _anglePerStep * _gearRatio;
    rotate(stepsToMove);
    _beforeAngle = getAngle();
}

float StepperMotor::modAngle(float angle)
{
    int ampAngle = amplifier(angle);

    bool check = false;
    int count = 0;
    if(ampAngle > 0)
    {
	while(ampAngle > 0)
	{
	    ampAngle -= _anglePerStep;
	    count++;
	    if(ampAngle < 0 && -ampAngle > (_anglePerStep >> 1))
		count--;
	}
	ampAngle = count * _anglePerStep;
    }
    else
    {
	while(ampAngle < 0)
	{
	    ampAngle += _anglePerStep;
	    count++;
	    if(ampAngle > 0 && ampAngle > (_anglePerStep >> 1))
		count--;
	}
	ampAngle = -count * _anglePerStep;
    }

    angle = deplifier(ampAngle);

    while(angle >= 360)
	angle -= 360;

    while(angle < 0)
	angle += 360;

    return angle;    
}

int StepperMotor::amplifier(float angle)
{
    return (int)(angle * _precision);
}

float StepperMotor::deplifier(int angle)
{
    return (float)angle / _precision;
}

void StepperMotor::changeCurrentAngle(float angle)
{
    AngleMotor::changeCurrentAngle(angle);
    _beforeAngle = getAngle();
}
