#include "CompileCondition.h"
#include "IOModule.h"

#if defined(_CPP_)
#include <iostream>
#include <bitset>

using namespace std;
#endif

#if defined(_ARDUINO_)
#include "Arduino.h"
#endif

void CustomIO::input(char* c)
{
#if !defined(IGNORE_INPUT)
#if defined(_CPP_)
    cin >> *c;
#endif

#if defined(_ARDUINO_)
    if(Serial)
    {
	Serial.begin(BAUDRATE);
	while(!Serial.available());
	*c = Serial.read();
	Serial.end();
    }
#endif
#endif
}

void CustomIO::input(int* i)
{
#if !defined(IGNORE_INPUT)
#if defined(_CPP_)
    cin >> *i;
#endif

#if defined(_ARDUINO_)
    if(Serial)
    {
	Serial.begin(BAUDRATE);
	while(!Serial.available());
	*i = Serial.parseInt();
	Serial.end();
    }
#endif
#endif
}

void CustomIO::input(float* f)
{
#if !defined(IGNORE_INPUT)
#if defined(_CPP_)
    cin >> *f;
#endif

#if defined(_ARDUINO_)
    if(Serial)
    {
	Serial.begin(BAUDRATE);
	while(!Serial.available());
	*f = Serial.parseFloat();
	Serial.end();
    }
#endif
#endif
}

void CustomIO::output(int i)
{
#if defined(_CPP_)
    cout << i << endl;
#endif

#if defined(_ARDUINO_)
    if(Serial)
    {
	Serial.begin(BAUDRATE);
	Serial.println(i);
	Serial.end();
    }
#endif
}

void CustomIO::output(char c, bool bit)
{
#if defined(_CPP_)
    cout << bitset<8>(c) << endl;
#endif

#if defined(_ARDUINO_)
    if(Serial)
    {
	Serial.begin(BAUDRATE);

	if(bit)
	    Serial.println(c, BIN);
	else
	    Serial.println(c);

	Serial.end();
    }
#endif
}

void CustomIO::output(long l)
{
#if defined(_CPP_)
    cout << l << endl;
#endif

#if defined(_ARDUINO_)
    if(Serial)
    {
	Serial.begin(BAUDRATE);
	Serial.println(l);
	Serial.end();
    }
#endif
}

void CustomIO::output(float f)
{
#if defined(_CPP_)
    cout << f << endl;
#endif

#if defined(_ARDUINO_)
    if(Serial)
    {
	Serial.begin(BAUDRATE);
	Serial.println(f);
	Serial.end();
    }
#endif
}

#if defined(_CPP_)
void CustomIO::output(std::string s)
{
    cout << s << endl;
}
#endif

#if defined(_ARDUINO_)
void CustomIO::output(String s)
{
    if(Serial)
    {
	Serial.begin(BAUDRATE);
	Serial.println(s);
	Serial.end();
    }
}
#endif
