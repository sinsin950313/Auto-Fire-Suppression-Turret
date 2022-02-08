#ifndef IOMODULE_H
#define IOMODULE_H

#include "CompileCondition.h"

#if defined(_CPP_)
#include <string>
#endif

#if defined(_ARDUINO_)
#include "Arduino.h"
#endif

namespace CustomIO
{
    void input(char* c);

    void input(int* i);

    void input(float* f);

    void output(char c, bool bit);

    void output(int i);

    void output(long l);

    void output(float f);

#if defined(_CPP_)
    void output(std::string s);
#endif

#if defined(_ARDUINO_)
    void output(String s);
#endif
};

#endif
