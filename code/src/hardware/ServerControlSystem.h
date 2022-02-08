#ifndef SERVERCONTROLSYSTEM_H
#define SERVERCONTROLSYSTEM_H

#include "HardwareControlSystem.h"

class ServerControlSystem : public HardwareControlSystem
{
    public:
	ServerControlSystem();

	~ServerControlSystem() override;

    protected:
	void changeState(int transfer) override;
};

#endif
