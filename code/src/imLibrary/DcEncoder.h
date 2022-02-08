#ifndef	DCENCODER_H
#define DCENCODER_H

/*
#include "Device.h"
#include "Encoder.h"
#include "MotorDriver.h"

class DcEncoder : public Device
{
    public:
	DcEncoder(int mdFirstPin, int mdSecondPin, int mdPwmPin, int phaseA, int phaseB, int PPR);

	DcEncoder(int mdFirstPin, int mdSecondPin, int mdPwmPin, int phaseA, int phaseB, int phaseZ, int PPR);

	~DcEncoder();

	void setAngle(int degree);

	void tick();

	void reset();

    private:
	MotorDriver* md = nullptr;

	Encoder* e = nullptr;

	const int allowableError = 1;
};
*/

#endif
