#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include "AngleMotor.h"

class StepperMotor : public AngleMotor
{
    public:
	StepperMotor(int step, int gearRatio);

	virtual void setAngleSpeed(int degreePerSecond);

	virtual void setRotatePerMinute(int rpm) = 0;

	void changeCurrentAngle(float angle) override;

    protected:
	void setting(float angle) override;

	float modAngle(float angle) override;

	virtual void rotate(int stepsToMove) = 0;

	int amplifier(float angle);

	float deplifier(int angle);

	int getStep() { return _step; }

	int getGearRatio() { return _gearRatio; }

    private:
	int _step;

	int _gearRatio;

	int _anglePerStep;

	float _beforeAngle = 0;

	int _precision = 10;
};

#endif
