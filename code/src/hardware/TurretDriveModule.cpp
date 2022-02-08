#include "TurretDriveModule.h"
#include "Arduino.h"
#include "..\utility\IOModule.h"
#include "TurretI2CModuleData.h"
#include <Wire.h>

TurretDriveModule::TurretDriveModule(AngleMotor* yawMotor, AngleMotor* pitchMotor, Encoder* yawEncoder, Encoder* pitchEncoder, DistanceMeasurement* dm, LinearMotor* lm)
{
    _yawMotor = yawMotor;
    _pitchMotor = pitchMotor;

    _yawEncoder = yawEncoder;//code by jisup
    _pitchEncoder = pitchEncoder;//code by jisup

    //_rotator = rotator;
    //_rotator->setPower(0);

    //_dm = dm;
    _lm = lm;
    _lm->setLength(0);

    Wire.begin();
}

TurretDriveModule::~TurretDriveModule()
{
    delete _yawMotor;
    delete _pitchMotor;
    delete _yawEncoder;//code by jisup
    delete _pitchEncoder;//code by jisup
    //delete _rotator;
    //delete _dm;
}

//code by jisup
void TurretDriveModule::setAngle(short yaw, short pitch)
{
    int deltaYaw = 360;
    int deltaPitch = 360;
    do
    {
	_yawMotor->setAngle(yaw);
	_pitchMotor->setAngle(pitch);

#if defined(DEBUG)
	CustomIO::output("Current Yaw Motor Angle");
	CustomIO::output(_yawMotor->getAngle());
	CustomIO::output("Current Yaw Encoder Angle");
	CustomIO::output(_yawEncoder->getAngle());
	CustomIO::output("Current Pitch Motor Angle");
	CustomIO::output(_pitchMotor->getAngle());
	CustomIO::output("Current Yaw Encoder Angle");
	CustomIO::output(_yawEncoder->getAngle());
#endif
	_yawMotor->changeCurrentAngle(_yawEncoder->getAngle());
	deltaYaw = deltaAngleCheck(yaw, _yawMotor->getAngle());

	_pitchMotor->changeCurrentAngle(_pitchEncoder->getAngle());
	deltaPitch = deltaAngleCheck(pitch, _pitchMotor->getAngle());

#if defined(DEBUG)
	CustomIO::output("Changed Yaw Motor Angle");
	CustomIO::output(_yawMotor->getAngle());
	CustomIO::output("Delta Yaw Angle");
	CustomIO::output(deltaYaw);
	CustomIO::output("Changed Pitch Motor Angle");
	CustomIO::output(_pitchMotor->getAngle());
	CustomIO::output("Delta Pitch Angle");
	CustomIO::output(deltaPitch);
#endif
    } while(abs(deltaYaw) > 1 || abs(deltaPitch) > 1);
}

//code by jisup
int TurretDriveModule::deltaAngleCheck(int aim, int current)
{
    int result = 0;
    if(aim > current)
	result = aim - current;
    else
	result = current - aim;

    if(result < 0)
	result += 360;

    if(result >= 360)
	result -= 360;

    if(result > 180)
	result -= 360;

    return result;
}

void TurretDriveModule::setVelocity(int velocity)
{
    _power = getOptimumPower(velocity);

    int size = sizeof(_power);
    Wire.beginTransmission(TurretI2CModuleData::MUZZLE_ADDRESS);
    char* c = reinterpret_cast<char*>(&_power);
    int i = 0;
    while(i < size)
    {
	Wire.write(*(c + i));
	i++;
    }
    Wire.endTransmission();
}

int TurretDriveModule::getOptimumPower(int aimVelocity)
{
    CustomIO::output("TurretDriveModule : getOptimumPower is hardcoding");
    CustomIO::output("TurretDriveModule : getOptimumPower will return optimum Encoder goal");
    return 10;
}

void TurretDriveModule::launch()
{
    //this code is for ESC
    //_rotator->setPower(_power);
    //delay(7000);

    //wait until ok signal
    char c = 0;
    while(!c)
    {
	Wire.requestFrom(TurretI2CModuleData::MUZZLE_ADDRESS, 1);
	while(!Wire.available());
	c = Wire.read();
    }

    _lm->setLength(100);
    delay(2000);
    _lm->setLength(0);
    delay(2000);

    //this code is for ESC
    //_rotator->setPower(0);

    setVelocity(0);
}

int TurretDriveModule::getDistance()
{
    _dm->measure();
    return _dm->getDistance();
}
