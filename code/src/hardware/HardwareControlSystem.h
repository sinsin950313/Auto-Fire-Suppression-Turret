#ifndef HARDWARECONTROLSYSTEM_H
#define HARDWARECONTROLSYSTEM_H

#include "State.h"
#include "Hardware.h"

class HardwareControlSystem
{
    public:
	HardwareControlSystem(Hardware* hardware, State* initState);

	virtual ~HardwareControlSystem();

	bool changeState(State* state);

	void run();

    protected:
	virtual void changeState(int transfer) = 0;

    private:
	Hardware* _hardware = nullptr;

	State* _currentState = nullptr;
};

#endif
