#include "CustomMath.h"
#include "CompileCondition.h"

#if defined(_CPP_)
#include <cmath>
#include <iostream>
#endif

#if defined (_ARDUINO_)
#include "math.h"
#endif

float SecondDimensionMath::cSin(int degree)
{
    float value = 2;

    float radian = cDegreeToRadian(degree);
#if defined(_CPP_)
    value = sin(radian);
#elif defined(_ARDUINO_)
    value = sin(radian);
#endif

    return value;
}

float SecondDimensionMath::cCos(int degree)
{
    float value = 2;

    float radian = cDegreeToRadian(degree);
#if defined(_CPP_)
    value = cos(radian);
#elif defined(_ARDUINO_)
    value = cos(radian);
#endif

    return value;
}

float SecondDimensionMath::cTan(int degree)
{
    float value = 0b11111111;

    float radian = cDegreeToRadian(degree);
#if defined(_CPP_)
    value = tan(radian);
#elif defined(_ARDUINO_)
    value = tan(radian);
#endif

    return value;
}

float SecondDimensionMath::cAtan(float origin)
{
    float radian;
#if defined(_CPP_)
    radian = atan(origin);
#elif defined(_ARDUINO_)
    radian = atan(origin);
#endif
    return cRadianToDegree(radian);
}

float SecondDimensionMath::cAtan(int dist, int height)
{
    float radian;
    float origin = ((float)height) / ((float) dist);
#if defined(_CPP_)
    radian = atan(origin);
#elif defined(_ARDUINO_)
    radian = atan(origin);
#endif
    return cRadianToDegree(radian);
}

float SecondDimensionMath::cDegreeToRadian(float degree)
{
    return degree * (cPI / 180.0f);
}

float SecondDimensionMath::cRadianToDegree(float radian)
{
    return radian * (180.0f / cPI);
}

float SecondDimensionMath::cSquare(float origin)
{
    return origin * origin;
}

float SecondDimensionMath::cLength(int x, int y)
{
    return cRoot(cSquare(x) + cSquare(y));
}

float SecondDimensionMath::cLength(int x, int y, int z)
{
    return cRoot(cSquare(x) + cSquare(y) + cSquare(z));
}

float SecondDimensionMath::cRoot(int origin)
{
    float value = -1;

#if defined(_CPP_)
    value = sqrt(origin);
#elif defined(_ARDUINO_)
    value = sqrt(origin);
#endif

    return value;
}

float SecondDimensionMath::cRoot(float origin)
{
    float value = -1;

#if defined(_CPP_)
    value = sqrt(origin);
#elif defined(_ARDUINO_)
    value = sqrt(origin);
#endif

    return value;
}

int SecondDimensionMath::cRound(float num)
{
    int value = 0;

#if defined(_CPP_)
    value = round(num);
#elif defined(_ARDUINO_)
    value = round(num);
#endif

    return value;
}

void ThirdDimensionMath::AbsCoordToRelativeCoord(CoordinateData* origin, CoordinateData* target, CoordinateData* reCoord)
{
    reCoord->setX(target->getX() - origin->getX());
    reCoord->setY(target->getY() - origin->getY());
    reCoord->setZ(target->getZ() - origin->getZ());
}

void ThirdDimensionMath::RelativeCoordToAbsCoord(CoordinateData* source, CoordinateData* vector, CoordinateData* reCoord)
{
    reCoord->setX(vector->getX() + source->getX());
    reCoord->setY(vector->getY() + source->getY());
    reCoord->setZ(vector->getZ() + source->getZ());
}

void ThirdDimensionMath::CoordToShp(CoordinateData* originVector, ShpericalData* reShp)
{
    int x = originVector->getX();
    int y = originVector->getY();
    int z = originVector->getZ();

    float radius = SecondDimensionMath::cLength(x, y, z);
    float yaw = SecondDimensionMath::cAtan(x, y);
    float pitch = SecondDimensionMath::cAtan(SecondDimensionMath::cLength(x, y), z);

    reShp->setYawAngle(SecondDimensionMath::cRound(yaw));
    reShp->setPitchAngle(SecondDimensionMath::cRound(pitch));
    reShp->setRadius(SecondDimensionMath::cRound(radius));
}

void ThirdDimensionMath::ShpToCoord(ShpericalData* originShp, CoordinateData* reVector)
{
    int yaw = originShp->getYawAngle();
    int pitch = originShp->getPitchAngle();
    float radius = (float)originShp->getRadius();

    float x = radius * SecondDimensionMath::cCos(pitch) * SecondDimensionMath::cCos(yaw);
    float y = radius * SecondDimensionMath::cCos(pitch) * SecondDimensionMath::cSin(yaw);
    float z = radius * SecondDimensionMath::cSin(pitch);

    reVector->setX(SecondDimensionMath::cRound(x));
    reVector->setY(SecondDimensionMath::cRound(y));
    reVector->setZ(SecondDimensionMath::cRound(z));
}
