/*
#include "DcEncoder.h"
#include "Arduino.h"

DcEncoder::DcEncoder(int mdFirstPin, int mdSecondPin, int mdPwmPin, int phaseA, int phaseB, int PPR)
{
    md = new MotorDriver(mdFirstPin, mdSecondPin, mdPwmPin);
    e = new Encoder(phaseA, phaseB, PPR);
}

DcEncoder::DcEncoder(int mdFirstPin, int mdSecondPin, int mdPwmPin, int phaseA, int phaseB, int phaseZ, int PPR)
{
    md = new MotorDriver(mdFirstPin, mdSecondPin, mdPwmPin);
    e = new Encoder(phaseA, phaseB, phaseZ, PPR);
}

DcEncoder::~DcEncoder()
{
    delete md;
    delete e;
}

void DcEncoder::setAngle(int angle)
{
    const int amp = 30;

    while(allowableError < abs(angle - e->getAngle()))
    {
	int error = angle - e->getAngle();
	md->setPower(error * amp);
    }

    md->setPower(0);
}

void DcEncoder::tick()
{
    e->tick();
}

void DcEncoder::reset()
{
    e->reset();
}
*/
