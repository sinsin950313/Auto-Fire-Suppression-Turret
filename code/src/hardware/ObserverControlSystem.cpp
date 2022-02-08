#include "ObserverControlSystem.h"
#include "ObserverStateOrder.h"
#include "HardwareStateCommon.h"
#include "Observer.h"
#include "BluetoothCommunicationModule.h"
#include "HardwareType.h"
#include "CommunicationSignal.h"

using namespace ObserverStateList;

ObserverControlSystem::ObserverControlSystem() : HardwareControlSystem(new Observer(new BluetoothCommunicationModule(TypeList::OBSERVER, CommunicationAddressList::OBSERVER_ADDRESS, new Bluetooth(50, 51, 30), new Bluetooth(52, 53, 31))), ObserverStateOrder::getInstance().getInitState())
{

}

ObserverControlSystem::~ObserverControlSystem()
{

}

void ObserverControlSystem::changeState(int transfer)
{
    State* state = nullptr;

    if(transfer == List::SEARCH)
	state = ObserverStateOrder::getInstance().getSearchState();

    if(transfer == List::FIRE)
	state = ObserverStateOrder::getInstance().getFireState();

    if(transfer == List::EXTINGUISH)
	state = ObserverStateOrder::getInstance().getExtinguishState();

    HardwareControlSystem::changeState(state);
}
