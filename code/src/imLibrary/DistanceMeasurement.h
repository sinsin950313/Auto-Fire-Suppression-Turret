#ifndef DISTANCEMEASUREMENT_H
#define DISTANCEMEASUREMENT_H

#include "Device.h"

class DistanceMeasurement : public Device
{
    public:
	virtual void measure() = 0;

	int getDistance() { return _distance; }

    private:
	int _distance = 0;

    protected:
	DistanceMeasurement() { }

	void setDistance(int dist) { _distance = dist; }
};

#endif
