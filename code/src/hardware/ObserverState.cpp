#include "ObserverState.h"
#include "Observer.h"

using namespace ObserverStateClasses;

/*bool ObserverState::checkHardwareType(Hardware& hd)
{
    return HardwareType::isEqual(hd.getType(), getType());
}*/

int InitState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Observer* o = reinterpret_cast<Observer*>(&hd);
	return o->init();
    }

    return -1;
}

int SearchState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Observer* o = reinterpret_cast<Observer*>(&hd);
	return o->search();
    }

    return -1;
}

int FireState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Observer* o = reinterpret_cast<Observer*>(&hd);
	return o->fire();
    }

    return -1;
}

int ExtinguishState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Observer* o = reinterpret_cast<Observer*>(&hd);
	return o->extinguish();
    }

    return -1;
}
