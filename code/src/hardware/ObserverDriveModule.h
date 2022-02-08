#ifndef OBSERVERDRIVEMODULE_H
#define OBSERVERDRIVEMODULE_H

#include "..\imLibrary\AngleMotor.h"
#include "..\imLibrary\amg8833.h"
#include "..\imLibrary\DistanceMeasurement.h"
#include "..\imLibrary\UVSensor.h"
#include "..\imLibrary\SoundModule.h"

class ObserverDriveModule
{
    public:
	ObserverDriveModule(AngleMotor* yaw, AngleMotor* pitch, Amg8833* camera, DistanceMeasurement* dm, UVSensor* uv, SoundModule* wt, int redPin, int greenPin);

	~ObserverDriveModule();

	void setAngle(int yaw, int pitch);

	void scan(bool deep);

	void read(LinearData* pixels);

	int getDistance();

	void addYawAngle(int angle);

	void addPitchAngle(int angle);

	int getCurrentYawAngle();

	int getCurrentPitchAngle();

	void onThreshold();

	void offThreshold();

	void onTrigger();

	void offTrigger();

	int getUVLevel();

	void turnOnLight();

	void turnOffLight();

	//void getSongList(); //jinhwa add

	void playMusic(int song);  //jinhwa add...1: 시스템on 2: 소화완료 3: 소화중 4: 화재감지

	void stopMusic();  //jinhwa add

    private:
	AngleMotor* _yawMotor = nullptr;

	AngleMotor* _pitchMotor = nullptr;

	Amg8833* _thermistorCamera = nullptr;

	DistanceMeasurement* _dm = nullptr;

	UVSensor* _uv = nullptr;

	SoundModule* _wt = nullptr;

	int _red;

	int _green;
};

#endif
