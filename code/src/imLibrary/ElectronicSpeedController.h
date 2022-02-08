#ifndef ELECTRONICSPEEDCONTROLLER_H
#define ELECTRONICSPEEDCONTROLLER_H

#include "Device.h"
#include "Servo.h"

class ElectronicSpeedController : public Device
{
    public:
	ElectronicSpeedController(int pwmPin);

	void setPower(int power);//0 ~ 100

	void reset();

    private:
	Servo _esc;

	const int _min = 1000;

	const int _max = 2000;
};

#endif
