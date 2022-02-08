#ifndef CUSTOMMATH_H
#define CUSTOMMATH_H

#include "CommonData.h"

using namespace CommonData;

namespace SecondDimensionMath
{
    const float cPI = 3.14159265;

    float cSin(int degree);

    float cCos(int degree);

    float cTan(int degree);

    float cAtan(float origin);

    float cAtan(int dist, int height);

    float cDegreeToRadian(float degree);

    float cRadianToDegree(float radian);

    float cSquare(float origin);

    float cLength(int x, int y);

    float cLength(int x, int y, int z);

    float cRoot(int origin);

    float cRoot(float origin);

    int cRound(float num);
};

namespace ThirdDimensionMath
{
    void AbsCoordToRelativeCoord(CoordinateData* origin, CoordinateData* target, CoordinateData* reCoord);

    void RelativeCoordToAbsCoord(CoordinateData* source, CoordinateData* vector, CoordinateData* reCoord);

    void CoordToShp(CoordinateData* originVector, ShpericalData* reShp);

    void ShpToCoord(ShpericalData* originShp, CoordinateData* reVector);
};

#endif
