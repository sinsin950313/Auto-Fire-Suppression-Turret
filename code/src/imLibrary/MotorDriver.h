#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "Device.h"

class MotorDriver : public Device
{
    public:
	MotorDriver(int firstPin, int secondPin, int pwmPin);

	void setPower(int power);

    private:
	void clockWise();

	void counterClockWise();

	//bool isForward();

	//void switchDirection() { switchButton(); };

    private:
	int _first, _second, _pwmPin, _power = 0;
};

#endif
