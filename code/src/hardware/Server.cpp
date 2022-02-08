#include "Server.h"
#include "HardwareStateCommon.h"
#include "CommunicationSignal.h"
#include "..\utility\CustomMath.h"
#include "..\utility\IOModule.h"
#include "..\utility\CompileCondition.h"

using namespace SecondDimensionMath;
using namespace ThirdDimensionMath;
using namespace CustomIO;

Server::HardwareGlobalPositionData::~HardwareGlobalPositionData()
{
    CoordNode* pNode = pHead;
    while(pNode != nullptr)
    {
	CoordNode* pNext = pNode->getNextNode();
	delete pNode;
	pNode = pNext;
    }
    pHead = nullptr;
}

void Server::HardwareGlobalPositionData::registerHardware(char hardwareID, int globalX, int globalY, int globalZ)
{
    CoordNode* pTail = pHead;

    if(pTail != nullptr)
    {
	while(pTail->getNextNode() != nullptr)
	    pTail = pTail->getNextNode();

	pTail->setNextNode(new CoordNode(hardwareID, globalX, globalY, globalZ));
    }
    else
    {
	pTail = new CoordNode(hardwareID, globalX, globalY, globalZ);
	pHead = pTail;
    }
}

CoordinateData* Server::HardwareGlobalPositionData::getGlobalPosition(char hardwareID)
{
    CoordNode* pNode = pHead;
    if(pNode != nullptr)
    {
	while(!(pNode->checkID(hardwareID)))
	{
	    //nullcheck algorithm is not exist. what happen if every node is not correct with hardwareID?
	    pNode = pNode->getNextNode();
	}
    }

    if(pNode == nullptr)
    {
#if defined(DEBUG)
	    CustomIO::output("Hardware does not registered!");
	    CustomIO::output(hardwareID, true);
#endif
	    return nullptr;
    }
    else
	return pNode->getPosition();
}

Server::Server(CommunicationModule* tcm, CommunicationModule* ocm) : Hardware(HardwareType::TypeList::SERVER)
{
#if defined(SH)
    _hpData.registerHardware(CommunicationAddressList::TURRET_ADDRESS, 0, 0, 0);
    _hpData.registerHardware(CommunicationAddressList::OBSERVER_ADDRESS, 0, 0, 100);
#endif

    _turretCM = tcm;
    _observerCM = ocm;
}

Server::~Server()
{
    delete _turretCM;
    delete _observerCM;
}

int Server::init()
{
#if defined(DEBUG)
    output("Init State");
#endif

#if defined(SERVER_BLUETOOTH_AND_MANUAL)

    output("Bluetooth and Manual Control is available");
    output("If Press key 1, this will be Bluetooth Mode");
    output("If Press key 2, this will be Manual Control Mode");

    int i = 0;
    while(i == 0)
    {
	output("Choose Observer Communication Module mode");

	CustomIO::input(&i);

	if(i == 1) 
	    output("Bluetooth Mode has chosen");
	else if(i == 2)
	{
	    delete _observerCM;

	    _observerCM = new ServerManualCommunicationModule();
	    output("Manual Mode has chosen");
	}
	else
	    i = 0;
    }
    i = 0;
    while(i == 0)
    {
	output("Choose Turret Communication Module mode");

	CustomIO::input(&i);

	if(i == 1) 
	    output("Bluetooth Mode has chosen");
	else if(i == 2)
	{
	    delete _turretCM;

	    _turretCM = new ServerManualCommunicationModule();
	    output("Manual Mode has chosen");
	}
	else
	    i = 0;
    }
#endif

    _turretCM->setC2Tmode();
    _turretCM->setC2TInit();
    //request(_turretCM, )

    _observerCM->setC2Omode();
    _observerCM->setC2OInit();
    //request(_observerCM, )

    return ServerStateList::WAIT;
}

int Server::wait()
{
#if defined(DEBUG)
    output("Wait State");
#endif

    _observerCM->receive();

    if(_observerCM->getOrder() == CommunicationSignalList::FIRE)
	return ServerStateList::FIRE;

    return ServerStateList::WAIT;
}

int Server::fire()
{
#if defined(DEBUG)
    output("Fire State");
#endif
    char observerID = _observerCM->getSourceID();
    if(_observerCM->getTransmittedData().getDataType() == CommonData::DataType::SHPERICAL)
    {
	ShpericalData* pShp = reinterpret_cast<ShpericalData*>(&_observerCM->getTransmittedData());
	//this code maybe changed if observer install condition is changed
	//now observer installed right way
	ShpericalData fireShp(pShp->getYawAngle(), pShp->getPitchAngle(), pShp->getRadius());
	//fireShp.setPitchAngle(-fireShp.getPitchAngle());

#if defined(DEBUG)
	output("Observer To Server : Fire Shperical Coordinate");
	fireShp.print();
#endif

	CoordinateData relativeFireCoord;
	CoordinateData absFireCoord;
	ShpToCoord(&fireShp, &relativeFireCoord);
#if defined(DEBUG)
	output("Relative Fire Coordinate From Observer");
	relativeFireCoord.print();
#endif
	RelativeCoordToAbsCoord(_hpData.getGlobalPosition(observerID), &relativeFireCoord, &absFireCoord);

#if defined(DEBUG)
	output("Absolute Observer Coordinate");
	_hpData.getGlobalPosition(observerID)->print();
	output("Absolute Fire Coordinate");
	absFireCoord.print();
#endif

	//how can i control only one observer with multiply turret?
	char turretID = findOptimumTurretID();
	ShpericalData obsShp;
	bool confirm = requestTurretCheck(turretID, &absFireCoord, &obsShp);

	if(!confirm)
	    return ServerStateList::FIRE;

#if defined(DEBUG)
	output("Turret To Server : Obstacle Shperical Coordinate");
	obsShp.print();
#endif

	CoordinateData relObsCoord;
	ShpToCoord(&obsShp, &relObsCoord);
	CoordinateData absObsCoord;
	RelativeCoordToAbsCoord(_hpData.getGlobalPosition(turretID), &relObsCoord, &absObsCoord);
#if defined(DEBUG)
	output("Server, Fire State : need rewrite this code. Obstacle does not considered yet");
#endif
	int obsHeight = absObsCoord.getZ();
	absObsCoord.setZ(0);

#if defined(DEBUG)
	output("Absolute Obstacle Coordinate");
	absObsCoord.print();
	output("Obstacle Height");
	output(obsHeight);
#endif

	ShpericalData aimShp;
	optimumPathAlgorithm(turretID, &absFireCoord, &absObsCoord, obsHeight, &aimShp);

#if defined(DEBUG)
	output("Optimum Data : Velocity, Yaw, Pitch");
	aimShp.print();
#endif
	while(_observerCM->getOrder() != CommunicationSignalList::EXTINGUISH)
	{
	    requestTurretLaunch(turretID, &aimShp);

#if defined(DEBUG)
	output("Wait For Turret Launch Signal");
#endif
	    _turretCM->receive();

	    requestObserverCheck(observerID);

#if defined(DEBUG)
	output("Wait For Observer Impact Area Check Response");
#endif
	    _observerCM->receive();
	}

	return ServerStateList::EXTINGUISH;
    }

    return ServerStateList::FIRE;
}

char Server::findOptimumTurretID()
{
    return CommunicationAddressList::TURRET_ADDRESS;
}

int Server::extinguish()
{
#if defined(DEBUG)
    output("Extinguish State");
#endif
    //is it okay to get observerID from _observerCM?
    char turretID = _otData.getLinkedTurretID(_observerCM->getSourceID());
    _turretCM->setC2TExtinguish();
    DummyData d;
    request(_turretCM, turretID, &d);

    return ServerStateList::WAIT;
}

bool Server::requestTurretCheck(char turretID, CoordinateData* fireCoord, ShpericalData* reObsData)
{
    /*
    CoordinateData aimCoord;
    AbsCoordToRelativeCoord(_hpData.getGlobalPosition(turretID), fireCoord, &aimCoord);
    ShpericalData aimVector;
    CoordToShp(&aimCoord, &aimVector);

    _turretCM->setC2TCheck();
#if defined(DEBUG)
	output("Request To Turret for Obstacle Check");
#endif
    bool transmissionConfirm = request(_turretCM, turretID, &aimVector);
    if(transmissionConfirm)
    {
#if defined(DEBUG)
	output("Wait For Data of Obstacle From Turret");
#endif
	_turretCM->receive();
	transmissionConfirm = (_turretCM->getTransmittedData().getDataType() == CommonData::DataType::SHPERICAL);
	if(transmissionConfirm)
	{
	    ShpericalData* tmp = reinterpret_cast<ShpericalData*>(&_turretCM->getTransmittedData());
	    reObsData->setYawAngle(tmp->getYawAngle() + aimVector.getYawAngle());
	    reObsData->setPitchAngle(tmp->getPitchAngle());
	    reObsData->setRadius(tmp->getRadius());
	}
    }

    return transmissionConfirm;
    */

#if defined(DEBUG)
	output("Obstacle Check has removed");
#endif
	return true;
}

void Server::optimumPathAlgorithm(char turretID, CoordinateData* fireAbsCoord, CoordinateData* obsAbsCoord, int obsHeight, ShpericalData* reAimShp)
{
    //what can we do if obstacle interrupt the pathway? how can i check it?
    CoordinateData relativeFire;
    AbsCoordToRelativeCoord(_hpData.getGlobalPosition(turretID), fireAbsCoord, &relativeFire);
#if defined(DEBUG)
    output("Relative Fire Coordinate From Turret");
    relativeFire.print();
#endif

    CoordinateData relativeObs;
    AbsCoordToRelativeCoord(_hpData.getGlobalPosition(turretID), obsAbsCoord, &relativeObs);

    int x = cRoot(cSquare(relativeFire.getX()) + cSquare(relativeFire.getY()));
    int y = relativeFire.getZ();
#if defined(DEBUG)
	output("====================");
	output("x");
	output(x);
	output("y");
	output(y);
	output("====================");
#endif
    if(y == 0)
	y = -1;

    const int gravity = 980;//cm per sec
    int yaw = (int)cAtan(relativeFire.getX(), relativeFire.getY());
    int pitch;
    float vx, vy;

    float alpha = 1.1f;
    if(y > 0)
    {
	float reachTime = cRoot(2 * alpha) + cRoot(2 * alpha - 2);
	vx = x / (reachTime * cRoot((float)y / gravity));
	vy = cRoot(2.0f * alpha * gravity * y);
	pitch = (int)cAtan(cRoot(2.0f * alpha) * reachTime * y / x);
	/*vx = (int)(((float)x / reachTime) * cRoot((float)gravity / (float)y));
	vy = (int)cRoot(2.0f * alpha * (float)gravity * (float)y);
	pitch = (int)cAtan((float)vy / (float)vx);*/
#if defined(DEBUG)
	output("====================");
	output("yaw");
	output(yaw);
	output("pitch");
	output(pitch);
	output("reachTime");
	output(reachTime);
	output("vx");
	output(vx);
	output("vy");
	output(vy);
	output("====================");
#endif
    }
    else
    {
	int maxHeight = alpha * (-y);
	float reachTime = cRoot(2.0f * (maxHeight - y) / gravity) + cRoot(2.0f * (maxHeight - 2 * y) / gravity);
	vx = x / reachTime;
	vy = cRoot(2.0f * gravity * (maxHeight - y));
	pitch = (int)cAtan(vy / vx);
#if defined(DEBUG)
	output("====================");
	output("yaw");
	output(yaw);
	output("pitch");
	output(pitch);
	output("maxHeight");
	output(maxHeight);
	output("reachTime");
	output(reachTime);
	output("vx");
	output(vx);
	output("vy");
	output(vy);
	output("====================");
#endif
    }
    float muzzleVelocity = cRoot(cSquare(vx) + cSquare(vy));

    reAimShp->setYawAngle(yaw);
    reAimShp->setPitchAngle(pitch);
    reAimShp->setRadius(((int)(muzzleVelocity / 100.0f)));// cm per sec To m per sec
}

int Server::findOptimumVelocity(CoordinateData* fireRelative, CoordinateData* obsRelative, int obsHeight)
{
    //if v0 is variable then how the algorithm will go?
    //muzzle velocity is const
#if defined(DEBUG)
    output("Server : findOptimumVelocity is hardcoding");
#endif
    return 10;//meter per second
}

bool Server::isCatched(int velocity, int angle, int aimDistance, int aimHeight)
{
    //need algorithm here
    return false;
}

void Server::requestTurretLaunch(char turretID, ShpericalData* aimShp)
{
    _turretCM->setC2TLaunch();
#if defined(DEBUG)
    output("Request To Turret for Launch");
#endif
    request(_turretCM, turretID, aimShp);
}

void Server::requestObserverCheck(char observerID)
{
    _observerCM->setC2OCheck();
    DummyData d;
#if defined(DEBUG)
    output("Request To Observer for Impact Area Check");
#endif
    request(_observerCM, observerID, &d);
}

bool Server::request(CommunicationModule* cm, char id, Data* data)
{
    return cm->send(*data, id);
}
