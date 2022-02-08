#ifndef STEPMOTORDRIVER_H
#define STEPMOTORDRIVER_H

#include "StepperMotor.h"

class StepMotorDriver : public StepperMotor
{
    public:
	StepMotorDriver(int step, int gearRatio, int enablePin, int directionPin, int pulsePin);

	void setRotatePerMinute(int rpm) override;

    protected:
	void rotate(int stepsToMove) override;

    private:
	int _enaPin;

	int _dirPin;

	int _pulsePin;

	long _pulseDelay = 10000;
};

#endif
