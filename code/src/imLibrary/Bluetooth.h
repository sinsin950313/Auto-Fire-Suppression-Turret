#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "Device.h"
#include "SoftwareSerial.h"
#include "..\utility\CommonData.h"

using namespace CommonData;

class Bluetooth : public Device
{
    public:
	Bluetooth(int tx, int rx, int statePin);

	~Bluetooth();

	void at();

	bool connect(char const * const * addr);

	bool isConnected();

	bool available();

	char read();

	void write(char c);

	void disconnect();

    private:
	const static int _baudrate = 9600;

	SoftwareSerial* _sw = nullptr;

	int _statePin;

	char* _lastAddr = nullptr;

	const int _delayTime = 500;
};

#endif
