#include "LinearMotor.h"
#include <Arduino.h>

LinearMotor::LinearMotor(int pwmPin)
{
    s = new Servo();
    s->attach(pwmPin, _min, _max);
}

LinearMotor::LinearMotor(int pwmPin, int min, int max)
{
    _min = min;
    _max = max;

    s = new Servo();
    s->attach(pwmPin, _min, _max);
}

LinearMotor::~LinearMotor()
{
    delete s;
}

void LinearMotor::setLength(int length)
{
    int tempLength = map(length, 0, 100, _min, _max);

    s->writeMicroseconds(tempLength);
}
