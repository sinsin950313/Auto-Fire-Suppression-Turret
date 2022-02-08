#include "Observer.h"
#include "..\imLibrary\ServoMotor.h"
#include "..\imLibrary\StepMotor.h"
#include "..\imLibrary\TfLidarSensor.h"
#include "..\imLibrary\Amg8833.h"
#include "HardwareStateCommon.h"
#include "..\utility\IOModule.h"
#include "..\utility\CompileCondition.h"
#include "..\imLibrary\UVSensor.h"
#include "..\imLibrary\SoundModule.h"

using namespace ObserverStateList;
using namespace CustomIO;

Observer::Observer(CommunicationModule* cm) : Hardware(HardwareType::TypeList::OBSERVER)
{
#if defined(DEBUG)
    output("Observer Start");
#endif
    AngleMotor* yaw = new StepMotor(256, 8, 8, 10, 9, 11);  //jinhwa change 64,32 to 256,8
    AngleMotor* pitch = new StepMotor(256, 8, 2, 4, 3, 5);  //jinhwa change 64,32 to 256,8 
    TfLidarSensor* tf = new TfLidarSensor(13, 12);
    Amg8833* camera = new Amg8833(35);
    UVSensor* uv = new UVSensor(A0);
    SoundModule* wt = new SoundModule(14,15);  // jinhwa add Mega Tx,Rx num

    _drive = new ObserverDriveModule(yaw, pitch, camera, tf, uv, wt, 6, 7);
    pitch->setAngle(0);  //jinhwa _pitchAngle to 0
    yaw->setAngle(0);  //jinhwa _pitchAngle to 0

    _drive->onThreshold();

    _pixels = new LinearData(0);

    _cm = cm;
    _cm->setO2Cmode();
#if defined(DEBUG)
    output("Observer Start Finish");
#endif
}

Observer::~Observer()
{
    delete _drive;
    delete _pixels;
    delete _cm;
}

int Observer::init()
{
#if defined(DEBUG)
    output("Init State");
#endif
    _drive->playMusic(1);  // jinhwa add
    return List::SEARCH;
}

int Observer::search()
{
#if defined(DEBUG)
    output("Search State");
#endif
    searchAlgorithm();

    _drive->onTrigger();
    _drive->scan(true);
    _drive->read(_pixels);

    int i = 0;
    int size = _pixels->getSize();
    while(i < size)
    {
	if(_pixels->getIndexValue(i) != 0)
	{
#if defined(DEBUG)
	    output("Search Check");
#endif
	    roughAiming();
	    _drive->scan(true);
  	    _drive->read(_pixels);
	    simpleAiming();
	    detailAiming();
	    break;
	}
	i++;
    }
    if(i == size)
	return List::SEARCH;

    return isFire() ? List::FIRE : List::SEARCH;
}

// jinhwa revise
void Observer::searchAlgorithm()
{
    const static int YawdeltaAngle = 30;
    const static int Yawmax = 180;
    const static int Yawmin = 0;

    const static int PitchdeltaAngle = 30;
    const static int Pitchmax = 60;
    const static int Pitchmin = 0;


    static bool Yawforward = true;
    static bool Pitchforward = true;
    static int YawcurrentAngle = 0;
    static int PitchcurrentAngle = 0;

//Yaw   
    if(YawcurrentAngle <= Yawmin)
	Yawforward = true;
    else if(YawcurrentAngle >= Yawmax)
	Yawforward = false;

    if(Yawforward)
	YawcurrentAngle += YawdeltaAngle;
    else
	YawcurrentAngle -= YawdeltaAngle;

// Pitch
     if(YawcurrentAngle >= Yawmax && PitchcurrentAngle >= Pitchmax)
    {
	YawcurrentAngle = 0;
	PitchcurrentAngle = 0;
    }
    if(YawcurrentAngle >= Yawmax)
    {
	PitchcurrentAngle += PitchdeltaAngle;
    }
    if(YawcurrentAngle <= Yawmin && PitchcurrentAngle == PitchdeltaAngle)
    {	
	PitchcurrentAngle += PitchdeltaAngle;
    }
    _drive->setAngle(YawcurrentAngle, PitchcurrentAngle);
}

void Observer::roughAiming()
{
#if defined(DEBUG)
    CustomIO::output("rough");
#endif
    const int deltaAngle = 8;
    int yaw = 0;
    int pitch = 0;
    int count = 0;

    aimingAlgorithm(&yaw, &pitch, &count);

    yaw = yaw * deltaAngle;
    pitch = pitch * deltaAngle;

#if defined(DEBUG)
    CustomIO::output("Add Angle Yaw");
    CustomIO::output(yaw);
    CustomIO::output("Add Angle Pitch");
    CustomIO::output(pitch);
#endif

    yaw = yaw / count;
    pitch = pitch / count;

    _drive->addYawAngle(yaw);
    _drive->addPitchAngle(pitch);
}

void Observer::aimingAlgorithm(int* yaw, int* pitch, int* count)
{
    int size = _pixels->getSize();
    int index = 0;
    int mod = 0;

    while(index < size)
    {
	if(_pixels->getIndexValue(index) == 1)
	{
	    mod = index % 8;
	    int vec = mod - 4;
	    if(!(vec < 0)) vec++;
	    (*yaw) += vec;

	    mod = index / 8;
	    vec = 4 - mod;
	    vec = -vec;
	    if(!(vec < 0)) vec++;
	    (*pitch) += vec;//when hardware is installed in the same way as datasheet

	    (*count)++;
	}

	index++;
    }

    (*pitch) = -(*pitch);
}

//jinhwa add
void Observer::simpleAiming()
{
#if defined(DEBUG)
    CustomIO::output("simple");
#endif
    const int deltaAngle = 3;
    int yaw = 0;
    int pitch = 0;
    int count = 0;

    aimingAlgorithm(&yaw, &pitch, &count);

    yaw = yaw * deltaAngle;
    pitch = pitch * deltaAngle;

#if defined(DEBUG)
    CustomIO::output("Add Angle Yaw");
    CustomIO::output(yaw);
    CustomIO::output("Add Angle Pitch");
    CustomIO::output(pitch);
#endif

    yaw = yaw / count;
    pitch = pitch / count;

    _drive->addYawAngle(yaw);
    _drive->addPitchAngle(pitch);
}

//jinhwa add
void Observer::detailAiming()
{
#if defined(DEBUG)
    CustomIO::output("detail");
#endif
    _drive->onTrigger();

    int deltaAngle = 2;
    int MdeltaAngle = -deltaAngle;

    _drive->scan(true);
    _drive->read(_pixels);

     int yaw = 0;
     int pitch = 0;
     int yaw1 = 0;
     int pitch1 = 0;
     int yaw2 = 0;
     int pitch2 = 0;
     int yaw3 = 0;
     int pitch3 = 0;
     int yaw4 = 0;
     int pitch4 = 0;

     yaw = _drive->getCurrentYawAngle();
     pitch = _drive->getCurrentPitchAngle();

    output(yaw);
    output(pitch);

    // _drive->addYawAngle(deltaAngle);
    // _drive->addPitchAngle(MdeltaAngle);
//1	
     while(_pixels->getIndexValue(36) != 0)
     {
       _drive->scan(true);
       _drive->read(_pixels);
       _drive->addYawAngle(deltaAngle);
       _drive->addPitchAngle(MdeltaAngle);//-
       output("1");

     }
     yaw1 = _drive->getCurrentYawAngle();
     pitch1 = _drive->getCurrentPitchAngle();

    output(yaw1);
    output(pitch1);
//2
     while(_pixels->getIndexValue(35) != 0)
     {
       _drive->scan(true);
       _drive->read(_pixels);
       _drive->addYawAngle(MdeltaAngle); //-
       _drive->addPitchAngle(MdeltaAngle);//-
       output("2");

     }
     yaw2 = _drive->getCurrentYawAngle();
     pitch2 = _drive->getCurrentPitchAngle();

    output(yaw2);
    output(pitch2);

//3
     while(_pixels->getIndexValue(27) != 0)
     {
       _drive->scan(true);
       _drive->read(_pixels);
       _drive->addPitchAngle(deltaAngle); 
       _drive->addYawAngle(MdeltaAngle); //-
       output("3");

     }
     yaw3 = _drive->getCurrentYawAngle();
     pitch3 = _drive->getCurrentPitchAngle();

    output(yaw3);
    output(pitch3);

//4
     while(_pixels->getIndexValue(28) != 0)
     {
       _drive->scan(true);
       _drive->read(_pixels); 
       _drive->addYawAngle(deltaAngle);
       _drive->addPitchAngle(deltaAngle);
       output("4");

     }
    yaw4 = _drive->getCurrentYawAngle();
    pitch4 = _drive->getCurrentPitchAngle();

    output(yaw4);
    output(pitch4);

//calculation
    yaw = (yaw1 + yaw2 + yaw3 + yaw4)/4;
    pitch = (pitch1 + pitch2 + pitch4 + pitch4)/4;

    delay(200);

   output(yaw);
   output(pitch);

   _drive->setAngle(yaw, pitch);

   output(_drive->getCurrentYawAngle());
   output(_drive->getCurrentPitchAngle());
}

/* jinhwa will make in Fire()
void Observer::warning()
{

}
*/

//code by jinhwa
bool Observer::UVcheck()
{
    //return !(_drive->getUVLevel() < 0) ? true : false;
    _drive->playMusic(4);  // jinhwa add
    return true;
}

bool Observer::isFire()
{
#if defined(DEBUG)
    CustomIO::output("isFire?");
#endif
    _drive->onThreshold();
    _drive->offTrigger();
    _drive->scan(true);
    _drive->read(_pixels);

    int dist = _drive->getDistance();
#if defined(DEBUG)
    CustomIO::output("distance");
    CustomIO::output(dist);
#endif
    int temp = sampling();
#if defined(DEBUG)
    CustomIO::output("temperature");
    CustomIO::output(temp);
#endif

    return distNtemp(dist, temp) ? true : false; 
}

int Observer::sampling()
{
#if defined(DEBUG)
    CustomIO::output("sampling");
#endif
    int value = 0;
    int count = 0;
    int temperature = 0;

    temperature = _pixels->getIndexValue(27);
    if(temperature != 0)
    {
	value += temperature;
	count++;
    }

    temperature = _pixels->getIndexValue(28);
    if(temperature != 0)
    {
	value += temperature;
	count++;
    }

    temperature = _pixels->getIndexValue(35);
    if(temperature != 0)
    {
	value += temperature;
	count++;
    }

    temperature = _pixels->getIndexValue(36);
    if(temperature != 0)
    {
	value += temperature;
	count++;
    }

    return value / count;
}

//code by jinhwa
bool Observer::distNtemp(int dist, int temp)
{
    //re write this code
    //return temp > 0 ? true : false;
    return (temp > 0 && UVcheck()) ? true : false;
}

// need jinhwa revise
int Observer::fire()
{
#if defined(DEBUG)
    output("Fire State");
#endif
    _drive->turnOnLight();//code by juntae    
    
    _drive->playMusic(3);
    _cm->setO2CFire();
    ShpericalData s(_drive->getCurrentYawAngle(), _drive->getCurrentPitchAngle(), _drive->getDistance());
    _cm->send(s);

#if defined(DEBUG)
    output("Wait for Impact Area Check Request");
#endif
    _cm->receive();

    if(_cm->getOrder() == IMPACT_AREA_CHECK)
    {
	if(isFire())
	    return List::FIRE;
	else
	    return List::EXTINGUISH;
    }

    return List::FIRE;
}

int Observer::extinguish()
{
#if defined(DEBUG)
    output("Extinguish State");
#endif
    _drive->turnOffLight();//code by juntae
    _drive->stopMusic();  // jinhwa add
    _drive->playMusic(2);  // jinhwa add
    _cm->setO2CExtinguish();
    DummyData d;
    _cm->send(d);

    return List::SEARCH;
}
