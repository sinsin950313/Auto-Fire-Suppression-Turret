#ifndef SERVERSTATEORDER_H
#define SERVERSTATEORDER_H

#include "State.h"
#include "ServerState.h"

using namespace ServerStateClasses;

class ServerStateOrder
{
    public:
	~ServerStateOrder();

	static ServerStateOrder& getInstance();

	State* getInitState();

	State* getWaitState();

	State* getFireSuppressionState();

	State* getExtinguishState();

    private:
	ServerStateOrder();

	static ServerStateOrder* _instance;

	InitState* _initState = nullptr;

	WaitState* _waitState = nullptr;

	FireSuppressionState* _fireState = nullptr;

	ExtinguishState* _extinguishState = nullptr;
};

#endif
