#ifndef AMG8833_H
#define AMG8833_H

#include <Wire.h>
#include "Camera.h"
#include "Adafruit_AMG88XX.h"

class Amg8833 : public Camera
{
    public:
	Amg8833(int threshold = -1000);

	~Amg8833();

	void scan() override;

	void read(LinearData* pixels) override;

	void setThreshold(int temperature);

	void onThreshold();

	void offThreshold();

	void onTrigger();

	void offTrigger();

	void delay();

    private:

	void printPixels(LinearData* pixels);

	void printPixels(float* pixels);

	union Buffer
	{
	    float _fBuffer[64];

	    int _iBuffer[64];
	};

	Buffer _buffer;

	void privateRead(LinearData* pixels);

	void thresholdRead(LinearData* pixels);

	void triggerRead(LinearData* pixels);

	/*float* _fBuffer = nullptr;

	int* _iBuffer = nullptr;

	char* _cBuffer = nullptr;*/

	int _threshold;

	bool _isTrigger = false;

	bool _isThreshold = false;

	Adafruit_AMG88xx* _thermCam = nullptr;

	const int delayTime = 500;
};

#endif
