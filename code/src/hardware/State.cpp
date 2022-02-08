#include "State.h"

State::State(TypeList type)
{
    _typeData = new HardwareTypeData(type);
}

State::~State()
{
    delete _typeData;
}

TypeList State::getType()
{
    return _typeData->getType();
}

bool State::checkHardwareType(Hardware& hd)
{
    return HardwareType::isEqual(hd.getType(), getType());
}
