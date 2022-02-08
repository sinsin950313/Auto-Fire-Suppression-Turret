#ifndef OBSERVERSTATE_H
#define OBSERVERSTATE_H

#include "State.h"
#include "Hardware.h"
#include "HardwareType.h"

using namespace HardwareType;

namespace ObserverStateClasses
{
    class ObserverState : public State
    {
	public:
	    ObserverState() : State(TypeList::OBSERVER) {};

	    ~ObserverState() {};

	//protected:
	    //bool checkHardwareType(Hardware& hd);
    };

    class InitState : public ObserverState
    {
	public:
	    InitState() {};

	    ~InitState() {};

	    int action(Hardware& hd) override;
    };

    class SearchState : public ObserverState
    {
	public:
	    SearchState() {};

	    ~SearchState() {};

	    int action(Hardware& hd) override;
    };

    class FireState : public ObserverState
    {
	public:
	    FireState() {};

	    ~FireState() {};

	    int action(Hardware& hd) override;
    };

    class ExtinguishState : public ObserverState
    {
	public:
	    ExtinguishState() {};

	    ~ExtinguishState() {};

	    int action(Hardware& hd) override;
    };
};

#endif
