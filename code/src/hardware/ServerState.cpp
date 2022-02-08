#include "ServerState.h"
#include "Server.h"

using namespace ServerStateClasses;

/*bool ServerState::checkHardwareType(HardwareType& hd)
{
    return HardwareType::isEqual(hd.getType(), getType());
}*/

int InitState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Server* s = reinterpret_cast<Server*>(&hd);
	return s->init();
    }

    return -1;
}

int WaitState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Server* s = reinterpret_cast<Server*>(&hd);
	return s->wait();
    }

    return -1;
}

int FireSuppressionState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Server* s = reinterpret_cast<Server*>(&hd);
	return s->fire();
    }

    return -1;
}

int ExtinguishState::action(Hardware& hd)
{
    if(checkHardwareType(hd))
    {
	Server* s = reinterpret_cast<Server*>(&hd);
	return s->extinguish();
    }

    return -1;
}
