#include "Camera.h"

Camera::Camera(int width, int height)
{
    _width = width;
    _height = height;
}

int Camera::getWidth()
{
    return _width;
}

int Camera::getHeight()
{
    return _height;
}

int Camera::getPixels()
{
    return _width * _height;
}
