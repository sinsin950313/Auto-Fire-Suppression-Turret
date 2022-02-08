#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include "Servo.h"
#include "AngleMotor.h"

class ServoMotor : public AngleMotor
{
    public:
	ServoMotor(int pulsePinNum);

	ServoMotor(int pulsePinNum, int min, int max);

	Servo* getServo() { return _servo; }

    protected:
	void setting(float angle) override;

	float modAngle(float angle) override;

    private:
	Servo* _servo = nullptr;
};

#endif
