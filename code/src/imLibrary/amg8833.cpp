#include "amg8833.h"
#include "..\utility\IOModule.h"
#include "..\utility\CompileCondition.h"

Amg8833::Amg8833(int threshold) : Camera(8, 8)
{
#if defined(DEBUG)
    CustomIO::output("AMG Start");
#endif
    setThreshold(threshold);
    _isThreshold = true;

    _thermCam = new Adafruit_AMG88xx();

    bool status = _thermCam->begin();
    if(!status)
    {
	CustomIO::output("Could not find a Valid AMG8833 sensor");
	while(1);
    }
}

Amg8833::~Amg8833()
{
    /*delete _fBuffer;
    delete _iBuffer;
    //delete _cBuffer;*/
}

void Amg8833::setThreshold(int temperature)
{
    _threshold = temperature;
}

void Amg8833::onThreshold()
{
    _isThreshold = true;
}

void Amg8833::offThreshold()
{
    _isThreshold = false;
    _isTrigger = false;
}

void Amg8833::onTrigger()
{
    _isThreshold = true;
    _isTrigger = true;
}

void Amg8833::offTrigger()
{
    _isTrigger = false;
}

void Amg8833::delay()
{
    long stamp = millis();

    while(1)
	if(millis() - stamp > delayTime) break;
}

void Amg8833::scan()
{
    _thermCam->readPixels(_buffer._fBuffer);
}

void Amg8833::read(LinearData* pixels)
{
    if(!_isThreshold)
    {
	//CustomIO::output("normal");
	privateRead(pixels);
	return;
    }

    if(_isTrigger)
    {
	//CustomIO::output("Trigger");
	triggerRead(pixels);
	return;
    }

    //CustomIO::output("Threshold");
    thresholdRead(pixels);
}

void Amg8833::privateRead(LinearData* pixels)
{
    //printPixels(_buffer._fBuffer);

    int pixelSize = getPixels();
    int i = 0;
    while(i < pixelSize)
    {
	_buffer._iBuffer[i] = (int)_buffer._fBuffer[i];
	i++;
    }

    pixels->storeData(_buffer._iBuffer, pixelSize);
    //printPixels(pixels);
}

void Amg8833::thresholdRead(LinearData* pixels)
{
    //printPixels(_buffer._fBuffer);
    int pixelSize = getPixels();
    int i = 0;
    while(i < pixelSize)
    {
	if(_buffer._fBuffer[i] > _threshold)
	    _buffer._iBuffer[i] = (int)_buffer._fBuffer[i];
	else
	    _buffer._iBuffer[i] = 0;

	i++;
    }

    pixels->storeData(_buffer._iBuffer, pixelSize);
    printPixels(pixels);
}

void Amg8833::triggerRead(LinearData* pixels)
{
    //printPixels(_buffer._fBuffer);
    int pixelSize = getPixels();
    int i = 0;
    while(i < pixelSize)
    {
	if(_buffer._fBuffer[i] > _threshold)
	    _buffer._iBuffer[i] = 1;
	else
	    _buffer._iBuffer[i] = 0;

	i++;
    }

    pixels->storeData(_buffer._iBuffer, pixelSize);
    //printPixels(pixels);
}

void Amg8833::printPixels(LinearData* pixels)
{
    Serial.begin(115200);
    int i = 0;
    Serial.print("[");
    for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
      if(pixels->getIndexValue(i-1) != 0)
        Serial.print(pixels->getIndexValue(i-1));
      else
        Serial.print("    ");
      Serial.print(", ");
      if( i%8 == 0 ) Serial.println();
    }
    Serial.println("]");
    Serial.println();
    Serial.end();
}

void Amg8833::printPixels(float* pixels)
{
    Serial.begin(115200);
    int i = 0;
    Serial.print("[");
    for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
      if(pixels[i-1] != 0)
        Serial.print(pixels[i-1]);
      else
        Serial.print("    ");
      Serial.print(", ");
      if( i%8 == 0 ) Serial.println();
    }
    Serial.println("]");
    Serial.println();
    Serial.end();
}
