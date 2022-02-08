#include "Encoder.h"
#include "Arduino.h"

Encoder::Encoder(int pinA, int pinB, long PPR)
{
    _en = new Encoder_PaulStoffregen(pinA, pinB);
    _en->write(0);

    _ppr = PPR;
    _delta = 360.0 / _ppr;
    _ppr = _ppr << 2;
}

float Encoder::getAngle()
{
    long clock = getClock();

    float angle = -clock * _delta;

    while(angle >= 360)
	angle -= 360;

    while(angle < 0)
	angle += 360;

    return angle;
}

long Encoder::getClock()
{
    int32_t newPos = _en->read();

    if(_oldPos != newPos)
	_oldPos = newPos % _ppr;

    while(_oldPos >= _ppr)
	_oldPos -= _ppr;

    while(_oldPos < 0)
	_oldPos += _ppr;

    _en->write(_oldPos);

    return _oldPos >> 2;
}
