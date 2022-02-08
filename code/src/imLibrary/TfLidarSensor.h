#include "DistanceMeasurement.h"
#include <SoftwareSerial.h>
#include "TFmini.h"

class TfLidarSensor : public DistanceMeasurement {
    public:
	TfLidarSensor(int txPin, int rxPin);

	void measure() override;

    private:
	SoftwareSerial* _sw = nullptr;

	TFmini* _tf = nullptr;
};

