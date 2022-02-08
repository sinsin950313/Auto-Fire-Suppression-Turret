#include "HardwareControlSystem.h"

HardwareControlSystem::HardwareControlSystem(Hardware* hardware, State* initState)
{
    _hardware = hardware;
    _currentState = initState;
}

HardwareControlSystem::~HardwareControlSystem()
{
    delete _hardware;

    _currentState = nullptr;
}

bool HardwareControlSystem::changeState(State* state)
{
    if(HardwareType::isEqual(_hardware->getType(), state->getType()))
    {
	_currentState = state;

	return true;
    }

    return false;
}

void HardwareControlSystem::run()
{
    int stateTransfer = _currentState->action(*_hardware);
    changeState(stateTransfer);
}
