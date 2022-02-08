#ifndef OBSERVERCONTROLSYSTEM_H
#define OBSERVERCONTROLSYSTEM_H

#include "HardwareControlSystem.h"

class ObserverControlSystem : public HardwareControlSystem
{
    public:
	ObserverControlSystem();

	~ObserverControlSystem() override;

    protected:
	void changeState(int transfer) override;
};

#endif
