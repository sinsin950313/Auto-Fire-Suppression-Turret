#include "ContinuousServoMotor.h"
#include "Arduino.h"

ContinuousServoMotor::ContinuousServoMotor(int pwmPin)
{
    _min = 544;
    _max = 2400;
    _mid = 1500;
    _servo.attach(pwmPin);
}

void ContinuousServoMotor::rotate(bool forward)
{
    if(forward)
	_servo.writeMicroseconds(_max);
    else
	_servo.writeMicroseconds(_min);
}

void ContinuousServoMotor::stop()
{
    _servo.writeMicroseconds(_mid);
}
