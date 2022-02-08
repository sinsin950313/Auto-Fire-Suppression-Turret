#ifndef DRIVEMODULE_H
#define DRIVEMODULE_H

#include "..\utility\CommonData.h"
#include "..\imLibrary\AngleMotor.h"
#include "..\imLibrary\ElectronicSpeedController.h"
#include "..\imLibrary\DistanceMeasurement.h"
#include "..\imLibrary\LinearMotor.h"
#include "..\imLibrary\Encoder.h"

class TurretDriveModule
{
    public:
	TurretDriveModule(AngleMotor* yawMotor, AngleMotor* pitchMotor, Encoder* yawEncoder, Encoder* pitchEncoder, DistanceMeasurement* dm, LinearMotor* lm);

	~TurretDriveModule();

	void setAngle(short yaw, short pitch);

	void setVelocity(int velocity);

	void launch();

	int getDistance();

    private:
	int deltaAngleCheck(int aim, int current);

	int getOptimumPower(int power);

	AngleMotor* _yawMotor = nullptr;

	AngleMotor* _pitchMotor = nullptr;

	Encoder* _yawEncoder = nullptr;

	Encoder* _pitchEncoder = nullptr;

	DistanceMeasurement* _dm = nullptr;

	LinearMotor* _lm = nullptr;

	int _power = 0;
};

#endif
