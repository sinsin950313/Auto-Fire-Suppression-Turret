#ifndef STEPMOTOR_H
#define STEPMOTOR_H

#include "StepperMotor.h"
//this library base on full step mode
#include "Stepper.h"

class StepMotor : public StepperMotor
{
    public:
	StepMotor(int step, int gearRatio, int pinA, int pinA_, int pinB, int pinB_);//A2B will clock wise

	void setRotatePerMinute(int rpm) override;

    protected:
	void rotate(int stepsToMove) override;

    private:
	Stepper* _stepper = nullptr;
};

#endif
