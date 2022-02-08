#ifndef TURRET_H
#define TURRET_H

#include "Hardware.h"
#include "TurretDriveModule.h"
#include "CommunicationModule.h"

class Turret : public Hardware
{
    public:
	Turret(CommunicationModule* cm);

	~Turret() override;

	int init();

	int wait();

	int aim();

	int check();

	int launch();

	int extinguish();

    private:
	bool readSignal();

	void writeSignal();

	TurretDriveModule* _drive = nullptr;

	int _aimAngleYaw = 0;

	int _aimAnglePitch = 0;

	int _aimRadius = 0;

	CommunicationModule* _cm = nullptr;
};

#endif
