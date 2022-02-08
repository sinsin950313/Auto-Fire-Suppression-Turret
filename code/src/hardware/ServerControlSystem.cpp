#include "ServerControlSystem.h"
#include "HardwareStateCommon.h"
#include "ServerStateOrder.h"
#include "Server.h"
#include "BluetoothCommunicationModule.h"
#include "HardwareType.h"
#include "CommunicationSignal.h"

using namespace ServerStateList;

Bluetooth* oSender = new Bluetooth(50, 51, 30);
Bluetooth* oReceiver = new Bluetooth(52, 53, 31);
Bluetooth* tSender = new Bluetooth(50, 51, 30);
Bluetooth* tReceiver = new Bluetooth(52, 53, 31);

ServerControlSystem::ServerControlSystem() : HardwareControlSystem(new Server(new BluetoothCommunicationModule(TypeList::SERVER, CommunicationAddressList::CENTRAL_ADDRESS, oSender, oReceiver), new BluetoothCommunicationModule(TypeList::SERVER, CommunicationAddressList::CENTRAL_ADDRESS, tSender, tReceiver)), ServerStateOrder::getInstance().getInitState())
{

}

ServerControlSystem::~ServerControlSystem()
{

}

void ServerControlSystem::changeState(int transfer)
{
    State* state = nullptr;

    if(transfer == List::WAIT)
	state = ServerStateOrder::getInstance().getWaitState();

    if(transfer == List::FIRE)
	state = ServerStateOrder::getInstance().getFireSuppressionState();

    if(transfer == List::EXTINGUISH)
	state = ServerStateOrder::getInstance().getExtinguishState();

    HardwareControlSystem::changeState(state);
}
