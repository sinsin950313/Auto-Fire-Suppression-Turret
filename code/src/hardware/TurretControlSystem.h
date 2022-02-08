#ifndef TURRETCONTROLSYSTEM_H
#define TURRETCONTROLSYSTEM_H

#include "HardwareControlSystem.h"

class TurretControlSystem : public HardwareControlSystem
{
    public:
	TurretControlSystem();

	~TurretControlSystem() override;

    protected:
	void changeState(int transfer) override;
};

#endif
