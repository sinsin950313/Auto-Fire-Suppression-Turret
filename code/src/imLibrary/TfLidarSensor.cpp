#include "TfLidarSensor.h"

#define TFMINI_BAUDRATE 115200

TfLidarSensor::TfLidarSensor(int txPin, int rxPin)
{
    SoftwareSerial* swSerial = new SoftwareSerial(txPin, rxPin);
    _sw = swSerial;
    _sw->begin(TFMINI_BAUDRATE);
    _tf = new TFmini();
    _tf->attach(*swSerial);
}

void TfLidarSensor::measure()
{
    _sw->listen();
    while(!_tf->available());
    setDistance(_tf->getDistance());
}
