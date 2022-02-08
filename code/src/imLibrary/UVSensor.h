/*
 * code by jinhwa & juntae
 */
#ifndef UV_H
#define UV_H

#include "Device.h"

class UVSensor : public Device
{
    public:
	UVSensor(int pin);

	int getLevel();

    private:
	int _pin;
};

#endif

