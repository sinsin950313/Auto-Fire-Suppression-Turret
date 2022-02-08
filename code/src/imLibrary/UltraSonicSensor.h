#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include "DistanceMeasurement.h"

class UltraSonicSensor : public DistanceMeasurement
{
    public:
	UltraSonicSensor(int trig, int echo);

	void measure() override;

    private:
	const int _velocity = 58;

	const int _sensingTime = 10;

	int _trig;

	int _echo;
};

#endif
