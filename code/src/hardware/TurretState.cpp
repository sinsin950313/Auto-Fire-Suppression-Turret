#include "TurretState.h"
#include "Turret.h"

using namespace TurretStateClasses;

/*bool TurretState::checkHardwareType(Hardware& hd)
{
    return HardwareType::isEqual(hd.getType(), getType());
}*/

int InitState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Turret* t = reinterpret_cast<Turret*>(&hd);
	return t->init();
    }

    return -1;
}

int WaitState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Turret* t = reinterpret_cast<Turret*>(&hd);
	return t->wait();
    }

    return -1;
}

int CheckState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Turret* t = reinterpret_cast<Turret*>(&hd);
	return t->check();
    }

    return -1;
}

int AimState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Turret* t = reinterpret_cast<Turret*>(&hd);
	return t->aim();
    }

    return -1;
}

int LaunchState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Turret* t = reinterpret_cast<Turret*>(&hd);
	return t->launch();
    }

    return -1;
}

int ExtinguishState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Turret* t = reinterpret_cast<Turret*>(&hd);
	return t->extinguish();
    }

    return -1;
}
