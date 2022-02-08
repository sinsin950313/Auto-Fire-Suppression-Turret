#include "ServoMotor.h"
#include "..\utility\IOModule.h"
#include "..\utility\CompileCondition.h"

ServoMotor::ServoMotor(int pulsePinNum)
{
#if defined(DEBUG)
    CustomIO::output("Servo Motor Initializing");
#endif
    _servo = new Servo();
    _servo->attach(pulsePinNum);
    setting(0);
}

ServoMotor::ServoMotor(int pulsePinNum, int min, int max)
{
#if defined(DEBUG)
    CustomIO::output("Servo Motor Initializing");
#endif
    _servo = new Servo();
    _servo->attach(pulsePinNum, min, max);
    setting(0);
}

void ServoMotor::setting(float angle) 
{
    _servo->write((int)angle);
}

float ServoMotor::modAngle(float angle) 
{
    if(angle < 0)
	return 0.0f;

    if(angle > 0)
	return 180.0f;

    return angle;
}
