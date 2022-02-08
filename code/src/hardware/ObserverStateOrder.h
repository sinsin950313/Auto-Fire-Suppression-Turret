#ifndef OBSERVERSTATEORDER_H
#define OBSERVERSTATEORDER_H

#include "State.h"
#include "ObserverState.h"

using namespace ObserverStateClasses;

class ObserverStateOrder
{
    public:
	~ObserverStateOrder();

	static ObserverStateOrder& getInstance();

	State* getInitState();

	State* getSearchState();

	State* getFireState();

	State* getExtinguishState();

    private:
	ObserverStateOrder();

	static ObserverStateOrder* _instance;

	InitState* _initState = nullptr;

	SearchState* _searchState = nullptr;

	FireState* _fireState = nullptr;

	ExtinguishState* _extinguishState = nullptr;
};

#endif
