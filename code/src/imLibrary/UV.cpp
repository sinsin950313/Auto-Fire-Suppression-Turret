#include "UV.h"
#include "..\utility\IOModule.h"

#include "Arduino.h"

UV::UV(int uv)
{
    _pin = uv;
}

int UV::getLevel()
{
    int sen_val = 0;
    long sum = 0;
    int i = 0;
    int vout = 0;

    for (int i = 0 ; i < 1024 ; i++ ) {
	sum = analogRead(_pin) + sum;
	delay(2);
    }

    vout = sum >> 10;
    vout = vout * 4980.0 / 1024;

    /*
    CustomIO::output("vout");
    CustomIO::output(vout);
    */

    int uv = -1;
    if (vout < 50) {
	uv = 0;
    }
    else if (vout < 227) {
	uv = 1;
    }
    else if (vout < 318) {
	uv = 2;
    }
    else if (vout < 408) {
	uv = 3;
    }
    else if (vout < 503) {
	uv = 4;
    }
    else if (vout < 606) {
	uv = 5;
    }
    else if (vout < 696) {
	uv = 6;
    }
    else if (vout < 795) {
	uv = 7;
    }
    else if (vout < 881) {
	uv = 8;
    }
    else if (vout < 976) {
	uv = 9;
    }
    else if (vout < 1079) {
	uv = 10;
    }
    else {
	uv = 11;
    }

    return uv;
}
