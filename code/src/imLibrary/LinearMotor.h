#ifndef LINEARMOTOR_H
#define LINEARMOTOR_H

#include "Device.h"
#include <Servo.h>

class LinearMotor : public Device
{
    public:
	LinearMotor(int pwmPin);

	LinearMotor(int pwmPin, int min, int max);

	~LinearMotor();

	void setLength(int length);//0 ~ 100

    private:
	Servo* s = nullptr;

	int _min = 1000;

	int _max = 2000;
};

#endif
