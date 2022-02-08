#ifndef CAMERA_H
#define CAMERA_H

#include "Device.h"
#include "..\utility\CommonData.h"

using namespace CommonData;

class Camera : public Device
{
    public:
	Camera(int width, int height);

	int getWidth();

	int getHeight();

	int getPixels();

	virtual void scan() = 0;

	virtual void read(LinearData* data) = 0;

    private:
	int _width, _height;
};

#endif
