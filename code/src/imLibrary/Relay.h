#ifndef RELAY_H
#define RELAY_H

#include "Device.h"

class Relay : public Device
{
    public:
	Relay(int pin);

	void on();

	void off();

	//bool isOn();

	//void switchButton();
    private:
	int _pin;
};

#endif
