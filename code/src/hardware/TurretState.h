#ifndef TURRETSTATE_H
#define TURRETSTATE_H

#include "State.h"
#include "Hardware.h"
#include "HardwareType.h"

using namespace HardwareType;

namespace TurretStateClasses
{
    class TurretState : public State
    {
	public:
	    TurretState() : State(TypeList::TURRET) {};

	    ~TurretState() {};

	//protected:
	    //bool checkHardwareType(Hardware& hd);
    };

    class InitState : public TurretState
    {
	public:
	    InitState() {};

	    ~InitState() {};

	    int action(Hardware& hd) override;
    };

    class WaitState : public TurretState
    {
	public:
	    WaitState() {};

	    ~WaitState() {};

	    int action(Hardware& hd) override;
    };

    class CheckState : public TurretState
    {
	public:
	    CheckState() {};

	    ~CheckState() {};

	    int action(Hardware& hd) override;
    };

    class AimState : public TurretState
    {
	public:
	    AimState() {};

	    ~AimState() {};

	    int action(Hardware& hd) override;
    };

    class LaunchState : public TurretState
    {
	public:
	    LaunchState() {};

	    ~LaunchState() {};

	    int action(Hardware& hd) override;
    };

    class ExtinguishState : public TurretState
    {
	public:
	    ExtinguishState() {};

	    ~ExtinguishState() {};

	    int action(Hardware& hd) override;
    };
};

#endif
