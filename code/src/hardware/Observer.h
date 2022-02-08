#ifndef OBSERVER_H
#define OBSERVER_H

#include "Hardware.h"
#include "ObserverDriveModule.h"
#include "..\utility\CommonData.h"
#include "CommunicationModule.h"

using namespace CommonData;

class Observer : public Hardware
{
    public:
	Observer(CommunicationModule* cm);

	~Observer() override;

	int init();

	int search();

	int fire();

	int extinguish();

    private:
	bool isFire();

	void searchAlgorithm();

	void aimingAlgorithm(int* yaw, int* pitch, int* count);

	void roughAiming();

	void simpleAiming(); //code by jinhwa

	void detailAiming();  //code by jinhwa

	bool UVcheck();	//code by jinhwa

	int sampling();

	bool distNtemp(int dist, int temp);
	
	ObserverDriveModule* _drive = nullptr;

	LinearData* _pixels = nullptr;

	short _pitchAngle = 0; //jinhwa revise

	CommunicationModule* _cm = nullptr;
};

#endif
