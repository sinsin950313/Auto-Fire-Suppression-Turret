#include "ObserverDriveModule.h"

ObserverDriveModule::ObserverDriveModule(AngleMotor* yawMotor, AngleMotor* pitchMotor, Amg8833* camera, DistanceMeasurement* dm, UVSensor* uv, SoundModule* wt, int redPin, int greenPin)
{
    _yawMotor = yawMotor;
    _pitchMotor = pitchMotor;
    _thermistorCamera = camera;
    _dm = dm;
    _wt = wt;  //code by jinhwa
    _uv = uv; //code by jinhwa

    //code by juntae
    _red = redPin;
    _green = greenPin;
    pinMode(_red, OUTPUT);
    pinMode(_green, OUTPUT);
}

ObserverDriveModule::~ObserverDriveModule()
{
    delete _yawMotor;
    delete _pitchMotor;
    delete _thermistorCamera;
    delete _dm;
    delete _uv;//code by jinhwa
    delete _wt;//code by jinhwa
}

void ObserverDriveModule::setAngle(int x, int y)
{
    _yawMotor->setAngle(x);
    _pitchMotor->setAngle(y);
}

void ObserverDriveModule::scan(bool deep)
{
    if(deep)
    _thermistorCamera->delay();
    _thermistorCamera->scan();
}

void ObserverDriveModule::read(LinearData* pixels)
{
    _thermistorCamera->read(pixels);
}

int ObserverDriveModule::getDistance()
{
    _dm->measure();
    return _dm->getDistance();
}

void ObserverDriveModule::addYawAngle(int angle)
{
    _yawMotor->addAngle(angle);
}

void ObserverDriveModule::addPitchAngle(int angle)
{
    _pitchMotor->addAngle(angle);
}

int ObserverDriveModule::getCurrentYawAngle()
{
    return _yawMotor->getAngle();
}

int ObserverDriveModule::getCurrentPitchAngle()
{
    return _pitchMotor->getAngle();
}

void ObserverDriveModule::onThreshold()
{
    _thermistorCamera->onThreshold();
}

void ObserverDriveModule::offThreshold()
{
    _thermistorCamera->offThreshold();
}

void ObserverDriveModule::onTrigger()
{
    _thermistorCamera->onTrigger();
}

void ObserverDriveModule::offTrigger()
{
    _thermistorCamera->offTrigger();
}

//code by jinhwa
int ObserverDriveModule::getUVLevel()
{
    _uv->getLevel();
}

//code by jinhwa
/*
void ObserverDriveModule::getSongList()
{
     _wt->getSong();
}
*/

//code by jinhwa
void ObserverDriveModule::playMusic(int song)
{
    _wt->play(song);
}

//code by jinhwa
void ObserverDriveModule::stopMusic()
{
    _wt->stop();
}

//code by juntae
void ObserverDriveModule::turnOnLight()
{
    digitalWrite(_red, HIGH);
    digitalWrite(_green, LOW);
}

//code by juntae
void ObserverDriveModule::turnOffLight()
{
    digitalWrite(_red, LOW);
    digitalWrite(_green, HIGH);
}
