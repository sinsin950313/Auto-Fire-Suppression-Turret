#include "Turret.h"
#include "..\imLibrary\StepMotorDriver.h"
#include "..\imLibrary\UltraSonicSensor.h"
#include "..\imLibrary\ElectronicSpeedController.h"
#include "..\imLibrary\LinearMotor.h"
#include "..\imLibrary\Encoder.h"
#include "HardwareStateCommon.h"
#include "..\utility\IOModule.h"
#include "..\utility\CompileCondition.h"

using namespace TurretStateList;
using namespace CustomIO;

Turret::Turret(CommunicationModule* cm) : Hardware(HardwareType::TypeList::TURRET)
{
#if defined(DEBUG)
    output("Turret System Start");
#endif
    _drive = new TurretDriveModule(new StepMotorDriver(3200, 1, 3, 4, 5), new StepMotorDriver(200, 77, 8, 9, 10), new Encoder(18, 40, 2500), new Encoder(19, 41, 3000L), new UltraSonicSensor(62, 63), new LinearMotor(45));
    _cm = cm;
    _cm->setT2Cmode();
}

Turret::~Turret()
{
    delete _drive;
    delete _cm;
}

int Turret::init()
{
#if defined(DEBUG)
    output("Init State");
#endif
    return List::WAIT;
}

int Turret::wait()
{
#if defined(DEBUG)
    output("Wait State");
#endif
    _cm->receive();
    char request = _cm->getOrder();

    if(request == CommunicationSignalList::OBSTACLE_CHECK_REQUEST || request == CommunicationSignalList::DATA_FOR_LAUNCH)
	return List::AIM;

    if(request == CommunicationSignalList::EXTINGUISH)
	return List::EXTINGUISH;

    return List::WAIT;
}

//remove aim state, aim is just action not state
int Turret::aim()
{
#if defined(DEBUG)
    output("Aim State");
#endif
    if(isEqual(_cm->getTransmittedData().getDataType(), SHPERICAL))
    {
	ShpericalData* data = reinterpret_cast<ShpericalData*>(&_cm->getTransmittedData());

#if defined(DEBUG)
	output("Aim Data");
	data->print();
#endif

	_aimAngleYaw = data->getYawAngle();
	_aimAnglePitch = data->getPitchAngle();
	_aimRadius = data->getRadius();
	_drive->setAngle(_aimAngleYaw, _aimAnglePitch);

	char request = _cm->getOrder();
	if(request == OBSTACLE_CHECK_REQUEST)
	    return List::CHECK;
	else
	    return List::FIRE;
    }
    else
	return List::WAIT;
}

int Turret::check()
{
#if defined(DEBUG)
    output("Check State");
    output("deprecated");
#endif
    /*
    int obstacleAngle;
    int obstacleRadius;

    int curr = 0;
    int prev = 0;

    int i = 0;
    while(_aimAnglePitch + i < 180)
    {
	_drive->setAngle(_aimAngleYaw, _aimAnglePitch + i);
	curr = _drive->getDistance();

#if defined(DEBUG)
	output("Current Obstacle Distance");
	output(curr);
#endif

	if(i == 0)
	{
	    obstacleRadius = curr;
	    prev = curr;
	}

	if(curr - prev > 50)
	{
	    obstacleAngle = i + _aimAnglePitch;
	    //radius will go extremly long
	    //obstacleRadius = curr;
	    obstacleRadius = prev;
	    break;
	}

	prev = curr;
	//i++;
	i += 2;//minimum delta for step
    }

    ShpericalData d(_aimAngleYaw, obstacleAngle, obstacleRadius);
    _cm->setT2CData();
#if defined(DEBUG)
    output("Send Obstacle Data");
#endif
    _cm->send(d);

    return List::WAIT;
    */
    return List::WAIT;
}

int Turret::launch()
{
#if defined(DEBUG)
    output("Launch State");
    CustomIO::output("Aim Velocity");
    CustomIO::output(_aimRadius);
#if !defined(PASS_MANUAL_CHECK)
    CustomIO::output("Press Any Key");
    char c;
    CustomIO::input(&c);
#endif
#endif
    _drive->setVelocity(_aimRadius);
    _drive->launch();

    DummyData d;
    _cm->setT2CLaunch();
#if defined(DEBUG)
    output("Send Launch Signal");
#endif
    _cm->send(d);

    return List::WAIT;
}

int Turret::extinguish()
{
#if defined(DEBUG)
    output("Extinguish State");
#endif

    _drive->setAngle(0, 0);

    return List::WAIT;
}
