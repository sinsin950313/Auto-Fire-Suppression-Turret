#include "ServerStateOrder.h"

ServerStateOrder* ServerStateOrder::_instance = nullptr;

ServerStateOrder::ServerStateOrder()
{
    _initState = new InitState();
    _waitState = new WaitState();
    _fireState = new FireSuppressionState();
    _extinguishState = new ExtinguishState();
}

ServerStateOrder::~ServerStateOrder()
{
    delete _initState;
    delete _waitState;
    delete _fireState;
    delete _extinguishState;
}

ServerStateOrder& ServerStateOrder::getInstance()
{
    if(_instance == nullptr)
	_instance = new ServerStateOrder();

    return *_instance;
}

State* ServerStateOrder::getInitState()
{
    return _initState;
}

State* ServerStateOrder::getWaitState()
{
    return _waitState;
}

State* ServerStateOrder::getFireSuppressionState()
{
    return _fireState;
}

State* ServerStateOrder::getExtinguishState()
{
    return _extinguishState;
}
