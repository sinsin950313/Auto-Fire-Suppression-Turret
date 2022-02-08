#include "ElectronicSpeedController.h"
#include "Arduino.h"

ElectronicSpeedController::ElectronicSpeedController(int pwmPin)
{
    _esc.attach(pwmPin);
    reset();
}

void ElectronicSpeedController::setPower(int power)
{
    int temp = map(power, 0, 100, _min, _max);

    _esc.writeMicroseconds(temp);
}

void ElectronicSpeedController::reset()
{
    setPower(0);
}
