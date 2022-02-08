#include "CommunicationModule.h"
#include "..\utility\IOModule.h"
#include "..\utility\CompileCondition.h"

#if defined(_CPP_)
#include <ctime>
#endif

CommunicationModule::CommunicationModule(TypeList ht, char myAddress)
{
    _ht = ht;
    _myAddress = myAddress;
}

CommunicationModule::~CommunicationModule()
{
    delete _packetBuffer;
    delete _order;
    delete _request;

    if(_dataPointer != nullptr)
	delete _dataPointer;
}

bool CommunicationModule::send(Data& data, char dstAddress)
{
    bool lastPacket = false;
    bool transmissionConfirm = false;
    char seed = (char)((int)pRandomForSeed() % 255);

    char* serialDataPointer = nullptr;
    int remainDataSize = data.serialize(&serialDataPointer);

#if defined(DEBUG)
	CustomIO::output("============================");
	CustomIO::output("Transmission Data in CommunicationModule");
	data.print();
#if !defined(PASS_MANUAL_CHECK)
	CustomIO::output("Press Any Key");
	char c;
	CustomIO::input(&c);
#endif
	CustomIO::output("============================");
#endif

    int count = 0;
    while(!lastPacket)
    {
	packetClear();
	bool separate = false;
	if(PURE_DATA_BUFFER_SIZE < remainDataSize)
	    separate = true;

	int headerSize = setPacketHeader(false, separate, seed, dstAddress);

	lastPacket = fillPacketBody(headerSize, &serialDataPointer, PURE_DATA_BUFFER_SIZE * count, remainDataSize);

	setErrorDetectingCode();

#if defined(DEBUG)
	int i = 0;
	CustomIO::output("============================");
	CustomIO::output("Transmission Packet in CommunicationModule");
	while(i < BUFFER_SIZE)
	{
	    //outputer should show bit data
	    CustomIO::output(_packetBuffer[i], true);
	    i++;
	}
	CustomIO::output("============================");
#endif

	while(!transmissionConfirm)
	{
	    //add try maximum 3 times when transmissionComfirm false parse
	    write(&_packetBuffer);

	    transmissionConfirm = isResponse();
#if defined(DEBUG)
	    if(transmissionConfirm)
		CustomIO::output("Received Response in CommunicationModule");
	    else
		CustomIO::output("Fail to Receive Response in CommunicationModule");
#endif
	}

	transmissionConfirm = false;
	remainDataSize -= PURE_DATA_BUFFER_SIZE;
	seed++;
	count++;
    }

    delete serialDataPointer;
    serialDataPointer = nullptr;

    return transmissionConfirm;
}

void CommunicationModule::packetClear()
{
    int i = 0;
    while(i < BUFFER_SIZE)
    {
	_packetBuffer[i] = 0;
	i++;
    }
}

bool CommunicationModule::isResponse()
{
#if !defined(NRC)
    if(!receiveSimple(true) || _packetBuffer[5] & 0b00000001 != 0b00000001)
	return false;
#endif

    return true;
}

int CommunicationModule::setPacketHeader(bool ACK, bool separate, char seed, char dstAddress)
{
    _packetBuffer[0] = START_CLOCK;
    _packetBuffer[1] = _order->getSignalFlag();
    _packetBuffer[2] = getMyAddress();
    _packetBuffer[3] = dstAddress;
    _packetBuffer[4] = _order->getOrderFlag();

    char tmp = 0b00000000;
    if(ACK)
	tmp = tmp | 0b00000001;
    if(separate)
	tmp = tmp | 0b00000010;
    else
	tmp = tmp | 0b00000100;

    _packetBuffer[5] = tmp;
    _packetBuffer[6] = seed;

    return 7;
}

bool CommunicationModule::fillPacketBody(int bufferOffset, char** src, int srcOffset, int remainSize)
{
    int compare = (PURE_DATA_BUFFER_SIZE < remainSize) ? PURE_DATA_BUFFER_SIZE : remainSize;
    for(int i = 0; i < compare; i++)
	_packetBuffer[bufferOffset + i] = (*src)[srcOffset + i];

    return remainSize - PURE_DATA_BUFFER_SIZE > 0 ? false : true;
}

void CommunicationModule::setErrorDetectingCode()
{
    char parity = _packetBuffer[0];

    for(int i = 1; i < (BUFFER_SIZE - 1); i++)
	parity = _packetBuffer[i] ^ parity;

    _packetBuffer[BUFFER_SIZE - 1] = parity;
}

void CommunicationModule::receive()
{
    bool finish = false;
    while(!finish)
    {
	packetClear();
	bool receive = receiveSimple(false);

	if(receive)
	{
	    writeRequest();

	    finish = fillData();

#if defined(DEBUG)
	    int i = 0;
	    CustomIO::output("============================");
	    CustomIO::output("Reception Packet in CommunicationModule");
	    while(i < BUFFER_SIZE)
	    {
		//outputer should show bit data
		CustomIO::output(_packetBuffer[i], true);
		i++;
	    }
	    CustomIO::output("============================");
#endif

	    delay(3000);
	    response();
	}
    }

#if defined(DEBUG)
	CustomIO::output("============================");
	CustomIO::output("Reception Data in CommunicationModule");
	_dataPointer->print();
#if !defined(PASS_MANUAL_CHECK)
	CustomIO::output("Press Any Key");
	char c;
	CustomIO::input(&c);
#endif
	CustomIO::output("============================");
#endif
}

bool CommunicationModule::receiveSimple(bool timeLimit)
{
    if(!timeLimit)
    {
	while(1)
	{
	    read(&_packetBuffer);
	    if(_packetBuffer[0] == START_CLOCK && checkErrorDetecting() && _packetBuffer[3] == getMyAddress())
		return true;;
	}
    }
    else
    {
	long intervalTime = 1000;//ms
	long initTime = pTimeForWait();
	long currentTime = pTimeForWait();
	while(currentTime - initTime < intervalTime)
	{
	    read(&_packetBuffer);
	    if(_packetBuffer[0] == START_CLOCK && checkErrorDetecting() && _packetBuffer[3] == getMyAddress())
		return true;;
	    currentTime = pTimeForWait();
	}

	return false;
    }
}

bool CommunicationModule::checkErrorDetecting()
{
    char parity = _packetBuffer[0];

    for(int i = 1; i < BUFFER_SIZE; i++)
	parity = _packetBuffer[i] ^ parity;

    return parity == 0 ? true : false;
}

void CommunicationModule::writeRequest()
{
    _request->setSourceID(_packetBuffer[2]);
    char signalType = _packetBuffer[1];
    char orderType = _packetBuffer[4];

    if((signalType & O2C) != 0)
    {
	_request->setSignalFlag(O2C);
	if(orderType == FIRE)
	    _request->setOrderFlag(FIRE);
	else
	    _request->setOrderFlag(EXTINGUISH);
    }
    else if((signalType & C2O) != 0)
    {
	_request->setSignalFlag(C2O);
	if(orderType == IMPACT_AREA_CHECK)
	    _request->setOrderFlag(IMPACT_AREA_CHECK);
	else
	    _request->setOrderFlag(INIT);
    }
    else if((signalType & C2T) != 0)
    {
	_request->setSignalFlag(C2T);
	if(orderType == DATA_FOR_LAUNCH)
	    _request->setOrderFlag(DATA_FOR_LAUNCH);
	else if(orderType == OBSTACLE_CHECK_REQUEST)
	    _request->setOrderFlag(OBSTACLE_CHECK_REQUEST);
	else if(orderType == EXTINGUISH)
	    _request->setOrderFlag(EXTINGUISH);
	else
	    _request->setOrderFlag(INIT);
    }
    else
    {
	_request->setSignalFlag(T2C);
	if(orderType == LAUNCH)
	    _request->setOrderFlag(LAUNCH);
	else
	    _request->setOrderFlag(RETURN_OBSTACLE_CHECK_DATA);
    }
}

bool CommunicationModule::fillData()
{
    static bool finished = true;
    static bool separated = false;

    //maybe this code need synchronize
    if(_dataPointer != nullptr)
    {
	delete _dataPointer;
	_dataPointer = nullptr;
    }

    if(finished)
	separated = _packetBuffer[5] & 0b00000010 ? true : false;

    char dataType;
    if(separated)
    {
	finished = false;
	static int count = 0;
	static char* beforeBuffer = nullptr;
	static char* currentBuffer = nullptr;

	int bufferSizeBefore = PURE_DATA_BUFFER_SIZE * count;
	count++;
	int bufferSizeCurrent = PURE_DATA_BUFFER_SIZE * count;

	currentBuffer = new char[bufferSizeCurrent];
	if(beforeBuffer != nullptr)
	{
	    for(int i = 0; i < bufferSizeBefore; i++)
		currentBuffer[i] = beforeBuffer[i];

	    delete beforeBuffer;
	    beforeBuffer = nullptr;
	}

	for(int i = bufferSizeBefore; i < bufferSizeCurrent; i++)
	    currentBuffer[i] = _packetBuffer[i - bufferSizeBefore + 7];

	beforeBuffer = currentBuffer;
	currentBuffer = nullptr;

	if((_packetBuffer[5] & 0b00000100) == 0b00000100)
	{
	    dataType = beforeBuffer[0];
	    if(dataType == LINEAR)
		_dataPointer = new LinearData();

	    _dataPointer->deserialize(&beforeBuffer);

	    count = 0;
	    delete currentBuffer;
	    delete beforeBuffer;
	    currentBuffer = nullptr;
	    beforeBuffer = nullptr;
	    finished = true;
	    separated = false;
	}
    }
    else
    {
	dataType = _packetBuffer[7];
	if(dataType == SHPERICAL)
	    _dataPointer = new ShpericalData();
	else if(dataType == COORDINATE)
	    _dataPointer = new CoordinateData();
	else if(dataType == DUMMY)
	    _dataPointer = new DummyData();

	char* pureDataBuffer = &_packetBuffer[7];
	_dataPointer->deserialize(&pureDataBuffer);

	finished = true;
	separated = false;
    }

    return finished;
}

void CommunicationModule::response()
{
#if !defined(NRC)
    CustomIO::output("Response in CommunicationModule");

    char storeSignalType = _order->getSignalFlag();
    char storeOrder = _order->getOrderFlag();

    char dstAddress = _packetBuffer[2];
    char seed = _packetBuffer[6];

    changeOrderToResponse();

    packetClear();
    setPacketHeader(true, false, seed, dstAddress);
    setErrorDetectingCode();
    write(&_packetBuffer);

    restoreOrder(storeSignalType, storeOrder);
#endif
}

void CommunicationModule::changeOrderToResponse()
{
    char tempSignalFlag = _packetBuffer[1];

    char mask;
    if((tempSignalFlag & 0b00001100) != 0)
	mask = 0b00001100;
    else
	mask = 0b00000011;

    _order->setSignalFlag(tempSignalFlag ^ mask);
    _order->setOrderFlag(_packetBuffer[4]);
}

void CommunicationModule::restoreOrder(char storedType, char storedOrder)
{
    _order->setSignalFlag(storedType);
    _order->setOrderFlag(storedOrder);
}

long CommunicationModule::pTimeForWait()
{
    int time = -1;
#if defined(_CPP_)
    time = (int)(clock() / CLOCKS_PER_SEC);
#endif

#if defined(_ARDUINO_)
    time = (int)millis();
#endif
    return time;
}

long CommunicationModule::pRandomForSeed()
{
    int rand = 0;
#if defined(_CPP_)
    std::srand(pTimeForWait());
    rand = std::rand();
#endif

#if defined(_ARDUINO_)
    randomSeed(pTimeForWait());
    rand = random(255);
#endif
    return rand;
}
