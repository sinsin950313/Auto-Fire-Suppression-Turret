#include "TurretStateOrder.h"

TurretStateOrder* TurretStateOrder::_instance = nullptr;

TurretStateOrder::TurretStateOrder()
{
    _initState = new InitState();
    _waitState = new WaitState();
    _checkState = new CheckState();
    _aimState = new AimState();
    _launchState = new LaunchState();
    _extinguishState = new ExtinguishState();
}

TurretStateOrder::~TurretStateOrder()
{
    delete _initState;
    delete _waitState;
    delete _checkState;
    delete _aimState;
    delete _launchState;
    delete _extinguishState;
}

TurretStateOrder& TurretStateOrder::getInstance()
{
    if(_instance == nullptr)
	_instance = new TurretStateOrder();

    return *_instance;
}

State* TurretStateOrder::getInitState()
{
    return _initState;
}

State* TurretStateOrder::getWaitState()
{
    return _waitState;
}

State* TurretStateOrder::getCheckState()
{
    return _checkState;
}

State* TurretStateOrder::getAimState()
{
    return _aimState;
}

State* TurretStateOrder::getLaunchState()
{
    return _launchState;
}

State* TurretStateOrder::getExtinguishState()
{
    return _extinguishState;
}
