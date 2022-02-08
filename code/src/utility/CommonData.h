#ifndef COMMONDATA_H
#define COMMONDATA_H

#include "CompileCondition.h"

#define U8bit char

namespace CommonData
{
    enum DataType { SHPERICAL, COORDINATE, LINEAR, DUMMY };

    //static const U8bit DataTypeSize = 1;//byte

#if defined(_CPP_)
    static const U8bit vTableSize = 4;//byte
#elif defined(_ARDUINO_)
    static const U8bit vTableSize = 2;//byte
#endif

    static bool isEqual(DataType dt1, DataType dt2) { return dt1 == dt2; }

    class Data
    {
	public:
	    Data(DataType dt) : _type(dt) { };

	    virtual ~Data() { };

	    virtual int serialize(char** param) = 0;

	    virtual bool deserialize(char** param) = 0;

	    DataType getDataType() { return _type; }

	    virtual void print() = 0;

	protected:
	    //every size is byte unit
	    int initialize(char** param, int size);

	    int fill(char** param, int start, int size, void* src);

	    int extract(char** param, int start, int size, void* dst);

	private:
	    DataType _type;
    };

    class DummyData : public Data
    {
	public:
	    DummyData() : Data(DataType::DUMMY) { }

	    ~DummyData() override { }

	    int serialize(char** param) override;

	    bool deserialize(char** param) override;

	    void print() override;
    };

    class CoordinateData : public Data
    {
	public:
	    CoordinateData() : Data(DataType::COORDINATE), _x(0), _y(0), _z(0) {}

	    CoordinateData(int x, int y, int z) : Data(DataType::COORDINATE), _x(x), _y(y), _z(z) {}

	    ~CoordinateData() override { };

	    void setCoordinate(int x, int y, int z) { _x = x; _y = y; _z = z; }

	    int serialize(char** param) override;

	    bool deserialize(char** param) override;

	    void setX(int x);

	    void setY(int y);

	    void setZ(int z);

	    int getX();

	    int getY();

	    int getZ();

	    void print() override;

	private:
	    int _x, _y, _z;
    };

    //left hand coordinate
    class ShpericalData : public Data
    {
	public:
	    ShpericalData() : Data(DataType::SHPERICAL) {};

	    ShpericalData(int yaw, int pitch, int radius) : Data(SHPERICAL), _yaw(yaw), _pitch(pitch), _radius(radius) {};

	    ~ShpericalData() override { };

	    int getYawAngle() { return _yaw; }

	    int getPitchAngle() { return _pitch; }

	    int getRadius() { return _radius; }

	    void setYawAngle(int angle) { _yaw = angle; }

	    void setPitchAngle(int angle) { _pitch = angle; }

	    void setRadius(int radius) { _radius = radius; }

	    int serialize(char** param) override;

	    bool deserialize(char** param) override;

	    void print() override;

	private:
	    int _radius = 0;

	    int _yaw = 0;

	    int _pitch = 0;
    };

    class LinearData : public Data
    {
	public:
	    LinearData(int bufferSize = 0);

	    ~LinearData() override;

	    int getIndexValue(unsigned int index);

	    unsigned int getSize();

	    int serialize(char** param) override;

	    bool deserialize(char** param) override;

	    bool storeData(int* srcBuffer, unsigned int size);

	    bool storeData(char* srcBuffer, unsigned int size);

	    void print() override;

	private:
	    bool checkIndex(unsigned int index);

	    void privateCopy(void* src, int srcTypeSize, void* dst);

	    unsigned int _size;

	    int* _buffer = nullptr;

	    unsigned int _bufferUnitSize = 4;
    };
};

#endif
