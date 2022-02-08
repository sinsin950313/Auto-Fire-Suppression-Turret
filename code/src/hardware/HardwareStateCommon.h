#ifndef HARDWARESTATECOMMON_H
#define HARDWARESTATECOMMON_H

namespace ObserverStateList
{
    enum List { SEARCH, FIRE, EXTINGUISH };
}

namespace TurretStateList
{
    enum List { WAIT, AIM, CHECK, FIRE, EXTINGUISH };
};

namespace ServerStateList
{
    enum List { WAIT, FIRE, EXTINGUISH };
};

#endif
