/*
 * this is wrapper class
 */

#ifndef ENCODER_H
#define ENCODER_H

#include "Encoder_PaulStoffregen.h"

#include "Device.h"

class Encoder : public Device
{
    public:
	Encoder(int pinA, int pinB, long PPR);

	float getAngle();

	long getClock();

    private:
	int32_t _oldPos = 0;

	float _delta;

	int32_t _ppr;

	Encoder_PaulStoffregen* _en = nullptr;
};

#endif
