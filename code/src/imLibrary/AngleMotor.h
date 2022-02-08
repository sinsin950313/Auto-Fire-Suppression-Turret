#ifndef ANGLEMOTOR_H
#define ANGLEMOTOR_H

#include "Device.h"

class AngleMotor : public Device
{
    public:
	AngleMotor() { };

	void setAngle(float angle);

	void addAngle(float addAngle);

	virtual void changeCurrentAngle(float angle);

	float getAngle();

    protected:
	virtual void setting(float angle) = 0;

	virtual float modAngle(float angle) = 0;
	
    private:
	float _angle = 0;
};

#endif
