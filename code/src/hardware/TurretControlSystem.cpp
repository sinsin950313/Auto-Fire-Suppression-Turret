#include "TurretControlSystem.h"
#include "HardwareStateCommon.h"
#include "Turret.h"
#include "TurretStateOrder.h"
#include "BluetoothCommunicationModule.h"
#include "TurretManualCommunicationModule.h"
#include "HardwareType.h"
#include "CommunicationSignal.h"

using namespace TurretStateList;

TurretControlSystem::TurretControlSystem() : HardwareControlSystem(new Turret(new BluetoothCommunicationModule(TypeList::TURRET, CommunicationAddressList::TURRET_ADDRESS, new Bluetooth(50, 51, 30), new Bluetooth(52, 53, 31))), TurretStateOrder::getInstance().getInitState())
//TurretControlSystem::TurretControlSystem() : HardwareControlSystem(new Turret(new TurretManualCommunicationModule()), TurretStateOrder::getInstance().getInitState())
{

}

TurretControlSystem::~TurretControlSystem()
{

}

void TurretControlSystem::changeState(int transfer)
{
    State* state = nullptr;

    if(transfer == List::WAIT)
	state = TurretStateOrder::getInstance().getWaitState();

    if(transfer == List::AIM)
	state = TurretStateOrder::getInstance().getAimState();

    if(transfer == List::CHECK)
	state = TurretStateOrder::getInstance().getCheckState();

    if(transfer == List::FIRE)
	state = TurretStateOrder::getInstance().getLaunchState();

    if(transfer == List::EXTINGUISH)
	state = TurretStateOrder::getInstance().getExtinguishState();

    HardwareControlSystem::changeState(state);
}
