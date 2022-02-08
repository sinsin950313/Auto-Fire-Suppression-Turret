#ifndef SERVERSTATE_H
#define SERVERSTATE_H

#include "State.h"
#include "Hardware.h"
#include "HardwareType.h"

using namespace HardwareType;

namespace ServerStateClasses
{
    class ServerState : public State
    {
	public:
	    ServerState() : State(TypeList::SERVER) {};

	    ~ServerState() {};

	//protected:
	    //bool checkHardwareType(HardwareType& hd);
    };

    class InitState : public ServerState
    {
	public:
	    InitState() {};

	    ~InitState() {};

	    int action(Hardware& hd) override;
    };

    class WaitState : public ServerState
    {
	public:
	    WaitState() {};

	    ~WaitState() {};

	    int action(Hardware& hd) override;
    };

    class FireSuppressionState : public ServerState
    {
	public:
	    FireSuppressionState() {};

	    ~FireSuppressionState() {};

	    int action(Hardware& hd) override;
    };

    class ExtinguishState : public ServerState
    {
	public:
	    ExtinguishState() {};

	    ~ExtinguishState() {};

	    int action(Hardware& hd) override;
    };
};

#endif
