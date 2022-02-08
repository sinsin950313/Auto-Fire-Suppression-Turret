#ifndef UV_H
#define UV_H

#include "Device.h"

class UV : public Device
{
    public:
	UV(int pin);

	int getLevel();

    private:
	int _pin;
};

#endif

