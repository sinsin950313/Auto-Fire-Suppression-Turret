#include "CommonData.h"
#include "CompileCondition.h"
#include "IOModule.h"

using namespace CommonData;
using namespace CustomIO;

int Data::initialize(char** param, int size)
{
    if(*param != nullptr)
	delete *param;

    //size hardcoding
    *param = new char[size - vTableSize];
    //cout << "initial size : " << size - vTableSize << endl;

    return fill(param, 0, sizeof(DataType), &_type);
}

int Data::fill(char** param, int start, int size, void* src)
{
    char* p = reinterpret_cast<char*>(src);

    int i = 0;
    while(i < size)
    {
	(*param)[start + i] = *(p + i);
	i++;
    }

    return start + size;
}

int Data::extract(char** param, int start, int size, void* dst)
{
    char* p = reinterpret_cast<char*>(dst);

    int i = 0;
    while(i < size)
    {
	*(p + i) = (*param)[start + i];
	i++;
    }

    return start + size;
}

int DummyData::serialize(char** param)
{
    int end = 0;

    end = initialize(param, sizeof(DummyData));

    return end;
}

bool DummyData::deserialize(char** param)
{
    DataType dt;
    int start = extract(param, 0, sizeof(DataType), &dt);

    if(isEqual(dt, getDataType()))
	return true;

    return false;
}

int CoordinateData::serialize(char** param)
{
    int end = 0;

    end = initialize(param, sizeof(CoordinateData));

    end = fill(param, end, sizeof(_x), &_x);
    end = fill(param, end, sizeof(_y), &_y);
    end = fill(param, end, sizeof(_z), &_z);

    return end;
}

void CoordinateData::setX(int x)
{
    _x = x;
}

void CoordinateData::setY(int y)
{
    _y = y;
}

void CoordinateData::setZ(int z)
{
    _z = z;
}

int CoordinateData::getX()
{
    return _x;
}

int CoordinateData::getY()
{
    return _y;
}

int CoordinateData::getZ()
{
    return _z;
}

bool CoordinateData::deserialize(char** param)
{
    DataType dt;
    int start = extract(param, 0, sizeof(DataType), &dt);

    if(isEqual(dt, getDataType()))
    {
	start = extract(param, start, sizeof(_x), &_x);
	start = extract(param, start, sizeof(_y), &_y);
	start = extract(param, start, sizeof(_z), &_z);

	return true;
    }

    return false;
}

int ShpericalData::serialize(char** param)
{
    int end = 0;

    end = initialize(param, sizeof(ShpericalData));

    end = fill(param, end, sizeof(_radius), &_radius);
    end = fill(param, end, sizeof(_yaw), &_yaw);
    end = fill(param, end, sizeof(_pitch), &_pitch);

    return end;
}

bool ShpericalData::deserialize(char** param)
{
    DataType dt;
    int start = extract(param, 0, sizeof(DataType), &dt);

    if(isEqual(dt, getDataType()))
    {
	start = extract(param, start, sizeof(_radius), &_radius);
	start = extract(param, start, sizeof(_yaw), &_yaw);
	start = extract(param, start, sizeof(_pitch), &_pitch);

	return true;
    }

    return false;
}

LinearData::LinearData(int bufferSize) : Data(DataType::LINEAR), _size(bufferSize)
{
    _buffer = new int[_size]();
}

LinearData::~LinearData()
{
    delete _buffer;
}

int LinearData::serialize(char** param)
{
    int end = 0;

    end = initialize(param, sizeof(LinearData) - sizeof(_buffer) + _bufferUnitSize * _size);

    end = fill(param, end, sizeof(_bufferUnitSize), &_bufferUnitSize);
    end = fill(param, end, sizeof(_size), &_size);
    end = fill(param, end, _size * _bufferUnitSize, _buffer);

    return end;
}

bool LinearData::deserialize(char** param)
{
    DataType dt;
    int start = extract(param, 0, sizeof(DataType), &dt);
    
    int bufferUnitSize = 0;
    start = extract(param, start, sizeof(_bufferUnitSize), &bufferUnitSize);

    if(isEqual(dt, getDataType()) && _bufferUnitSize == bufferUnitSize)
    {
	if(_buffer != nullptr)
	    delete _buffer;

	start = extract(param, start, sizeof(_size), &_size);
	_buffer = new int[_size];
	start = extract(param, start, _size * _bufferUnitSize, _buffer);

	return true;
    }

    return false;
}

int LinearData::getIndexValue(unsigned int index)
{
    if(checkIndex(index) && _buffer != nullptr)
	return _buffer[index];
    else
    {
	//print("Index Error");
	return 0;
    }
}

bool LinearData::storeData(int* srcBuffer, unsigned int size)
{
    if(_buffer != nullptr)
    {
	delete _buffer;
	_size = size;
	_buffer = new int[_size]();
    }

    int i = 0;
    while(i < size)
    {
	privateCopy(&(srcBuffer[i]), sizeof(int), &(_buffer[i]));
	i++;
    }

    return true;
}

bool LinearData::storeData(char* srcBuffer, unsigned int size)
{
    if(_buffer != nullptr)
    {
	delete _buffer;
	_size = size;
	_buffer = new int[_size]();
    }

    int i = 0;
    while(i < size)
    {
	privateCopy(&(srcBuffer[i]), sizeof(char), &(_buffer[i]));
	i++;
    }

    return true;
}

void LinearData::privateCopy(void* src, int srcTypeSize, void* dst)
{
    char* pSrc = reinterpret_cast<char*>(src);
    char* pDst = reinterpret_cast<char*>(dst);

    int i = 0;
    for(int i = 0; i < srcTypeSize; i++)
    {
	(*pDst) = (*pSrc);
	pSrc++;
	pDst++;
    }
}

unsigned int LinearData::getSize()
{
    return _size;
}

bool LinearData::checkIndex(unsigned int index)
{
    return index < _size;
}

void DummyData::print()
{
    output("This is Dummy Data");
}

void CoordinateData::print()
{
    output("Coordinate");
    output(_x);
    output(_y);
    output(_z);
}

void ShpericalData::print()
{
    output("Shperical");
    output(_radius);
    output(_yaw);
    output(_pitch);
}

void LinearData::print()
{
    output("Linear");
    output((int)_size);
    int i = 0;
    while(i < _size)
    {
	output(_buffer[i]);
	i++;
    }
}
