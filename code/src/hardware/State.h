#ifndef STATE_H
#define STATE_H

#include "Hardware.h"
#include "HardwareType.h"

using namespace HardwareType;

class State
{
    public:
	State(TypeList type);

	~State();

	TypeList getType();

	virtual int action(Hardware& hd) = 0;

    protected:
	bool checkHardwareType(Hardware& hd);

    private:
	HardwareTypeData* _typeData = nullptr;
};

#endif
