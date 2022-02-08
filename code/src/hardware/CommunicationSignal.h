#ifndef COMMUNICATIONSIGNAL_H
#define COMMUNICATIONSIGNAL_H

namespace CommunicationSignalList
{
    static char const T2C = 0b00000001;

    static char const C2T = 0b00000010;

    static char const C2O = 0b00000100;

    static char const O2C = 0b00001000;

    static char const EXTINGUISH = 0b00000001;

    static char const FIRE = 0b00000010;

    static char const IMPACT_AREA_CHECK = 0b00000001;

    static char const DATA_FOR_LAUNCH = 0b00000010;

    static char const OBSTACLE_CHECK_REQUEST = 0b00000100;

    static char const INIT = 0b00001000;

    static char const LAUNCH = 0b00000001;

    static char const RETURN_OBSTACLE_CHECK_DATA = 0b00000010;
};

namespace CommunicationAddressList
{
    static char const CENTRAL_ADDRESS = 0b00000000;

    static char const TURRET_ADDRESS = 0b11110000;

    static char const OBSERVER_ADDRESS = 0b00001111;
};

#endif
