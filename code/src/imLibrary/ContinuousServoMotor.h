#ifndef CONTINUOUSSERVOMOTOR_H
#define CONTINUOUSSERVOMOTOR_H

#include "Device.h"
#include "Servo.h"

class ContinuousServoMotor : public Device
{
    public:
	ContinuousServoMotor(int pwmPin);

	void rotate(bool cw);

	void stop();

    private:
	Servo _servo;

	int _min;

	int _max;

	int _mid;
};

#endif
