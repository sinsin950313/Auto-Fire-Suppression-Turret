#ifndef TURRETSTATEORDER_H
#define TURRETSTATEORDER_H

#include "State.h"
#include "TurretState.h"

using namespace TurretStateClasses;

class TurretStateOrder
{
    public:
	~TurretStateOrder();

	static TurretStateOrder& getInstance();

	State* getInitState();

	State* getWaitState();

	State* getCheckState();

	State* getAimState();

	State* getLaunchState();

	State* getExtinguishState();

    private:
	TurretStateOrder();

	static TurretStateOrder* _instance;

	InitState* _initState = nullptr;

	WaitState* _waitState = nullptr;

	CheckState* _checkState = nullptr;

	AimState* _aimState = nullptr;

	LaunchState* _launchState = nullptr;

	ExtinguishState* _extinguishState = nullptr;
};

#endif
