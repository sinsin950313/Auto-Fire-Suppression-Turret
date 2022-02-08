#include "Hardware.h"

Hardware::Hardware(TypeList type)
{
    _typeData = new HardwareTypeData(type);
}

Hardware::~Hardware()
{
    delete _typeData;
}

TypeList Hardware::getType()
{
    return _typeData->getType();
}
