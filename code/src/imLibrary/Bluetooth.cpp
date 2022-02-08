#include "Bluetooth.h"
#include "Arduino.h"
#include "..\utility\IOModule.h"

//this works on HM10 v540
//HM10 is only works on capital and no line ending

Bluetooth::Bluetooth(int tx, int rx, int statePin)
{
    _sw = new SoftwareSerial(tx, rx);
    _sw->begin(_baudrate);
    _statePin = statePin;
    pinMode(_statePin, INPUT);
    _lastAddr = new char[12];
}

Bluetooth::~Bluetooth()
{
    disconnect();
    _sw->end();
    delete _sw;
    delete _lastAddr;
}

void Bluetooth::at()
{
    static char const * const c = new char[2] {'A', 'T'};

    int i = 0;
    while(i < 2)
    {
	write(c[i]);
	i++;
    }

    delay(_delayTime);
}

bool Bluetooth::connect(char const * const * addr)
{
    while(!isConnected())
    {
	CustomIO::output("Connecting...");

	static char const * const c = new char[6] {'A', 'T', '+', 'C', 'O', 'N'};
	int i = 0;
	while(i < 6)
	{
	    write(c[i]);
	    i++;
	}

	const int size = 12;
	i = 0;
	while(i < size)
	{
	    char c = (*addr)[i];
	    if(96 < c && c < 123)
		c = c - 32;
	    write(c);
	    _lastAddr[i] = c;
	    i++;
	}

	delay(_delayTime);
    }

    return isConnected();
}

bool Bluetooth::isConnected()
{
    return digitalRead(_statePin) == HIGH ? true : false;
}

bool Bluetooth::available()
{
    return _sw->available();
}

char Bluetooth::read()
{
    _sw->listen();
    while(!available());

    return _sw->read();
}

void Bluetooth::write(char c)
{
    _sw->listen();
    _sw->write(c);
}

void Bluetooth::disconnect()
{
    static char const * const c = new char[8] {'A', 'T', '+', 'R', 'E', 'S', 'E', 'T'};

    while(isConnected())
    {
	CustomIO::output("DisConnecting...");

	at();

	int i = 0;
	while(i < 8)
	{
	    write(c[i]);
	    i++;
	}

	delay(_delayTime);

	if(isConnected())
	{
	    connect(&_lastAddr);
	    delay(_delayTime);
	}
    }
}
