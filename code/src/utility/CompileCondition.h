#ifndef COMPILECONDITION_H
#define COMPILECONDITION_H

#define DEBUG
//#define _CPP_
#define EXCEPTOBSERVER
#if defined(EXCEPTOBSERVER)
#define OBSERVER_HEIGHT 100
#endif
#define _ARDUINO_
#define BAUDRATE 115200
//#define NRC//not really communication
#define SH//single hardware
//#define NR//not response
//#define IGNORE_INPUT
#define PASS_MANUAL_CHECK

#endif
