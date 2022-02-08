#ifndef HARDWARE_H
#define HARDWARE_H

#include "HardwareType.h"

using namespace HardwareType;

class Hardware
{
    public:
	Hardware(TypeList type);

	virtual ~Hardware();

	TypeList getType();

    private:
	HardwareTypeData* _typeData = nullptr;
};

#endif
