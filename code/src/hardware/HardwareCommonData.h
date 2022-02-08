#ifndef HARDWAREDATA_H
#define HARDWAREDATA_H

#include "..\utility\CommonData.h"

using namespace CommonData;

namespace HardwareCommonData
{
    static char const CENTRAL_ADDRESS = 0b00000000;

    const static char OBSERVER_ADDRESS = 0b1000000;

    const static CoordinateData OBSERVER_COORDINATE(0, 100, 0);

    const static char TURRET_ADDRESS = 0b00000001;

    const static CoordinateData TURRET_COORDINATE(0, 0, 0);
};

#endif
