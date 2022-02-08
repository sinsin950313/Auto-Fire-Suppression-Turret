#include "ObserverStateOrder.h"

ObserverStateOrder* ObserverStateOrder::_instance = nullptr;

ObserverStateOrder::ObserverStateOrder()
{
    _initState = new InitState();
    _searchState = new SearchState();
    _fireState = new FireState();
    _extinguishState = new ExtinguishState();
}

ObserverStateOrder::~ObserverStateOrder()
{
    delete _initState;
    delete _searchState;
    delete _fireState;
    delete _extinguishState;
}

ObserverStateOrder& ObserverStateOrder::getInstance()
{
    if(_instance == nullptr)
	_instance = new ObserverStateOrder();

    return *_instance;
}

State* ObserverStateOrder::getInitState()
{
    return _initState;
}

State* ObserverStateOrder::getSearchState()
{
    return _searchState;
}

State* ObserverStateOrder::getFireState()
{
    return _fireState;
}

State* ObserverStateOrder::getExtinguishState()
{
    return _extinguishState;
}
