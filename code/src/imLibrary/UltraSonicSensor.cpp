#include "UltraSonicSensor.h"
#include "Arduino.h"

UltraSonicSensor::UltraSonicSensor(int trig, int echo)
{
    //StreamRecorder::getInstance().print("UltraSensor Initializing");
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);

    _trig = trig;
    _echo = echo;
}

void UltraSonicSensor::measure()
{
    //StreamRecorder::getInstance().print("UltraSensor Measureing");

    //Initializing
    digitalWrite(_trig, LOW);
    digitalWrite(_echo, LOW);
    delayMicroseconds(2);

    //Sensing
    digitalWrite(_trig, HIGH);
    delayMicroseconds(_sensingTime);
    digitalWrite(_trig, LOW);
    long duration = pulseIn(_echo, HIGH);

    if(duration == 0)
    {
	//String str = "sensing duration error : " + String(duration);
	//StreamRecorder::getInstance().print(str);
	return;
    }

    long distance = duration / _velocity;
    setDistance((int)distance);
}
