#include "ServerManualCommunicationModule.h"
#include "..\utility\IOModule.h"

#if defined(EXCEPTOBSERVER)
#include "..\utility\CustomMath.h"
using namespace ThirdDimensionMath;
#endif

using namespace CustomIO;

ServerManualCommunicationModule::ServerManualCommunicationModule(TypeList ht, char myAddress) : CommunicationModule(ht, myAddress)
{
    _ht = ht;
}

ServerManualCommunicationModule::~ServerManualCommunicationModule()
{

}

void ServerManualCommunicationModule::write(char* const* data)
{
    static bool start = true;
    static bool finish = true;
    static Data* assembledData = nullptr;

    if(start)
    {
	start = false;
	char dataType = (*data)[7];
	if(dataType == SHPERICAL)
	    assembledData = new ShpericalData();
	else if(dataType == COORDINATE)
	    assembledData = new CoordinateData();
	else if(dataType == DUMMY)
	    assembledData = new DummyData();
    }

    output("");
    output("Sending packet");
    for(int i = 0; i < BUFFER_SIZE; i++)
	output((*data)[i], true);
    output("");

    finish = assemblePacket(data, assembledData);

    if(finish)
    {
	output("");

	output("Sending Data");
	if((*data)[1] == C2T)
	    output("Server To Turret");
	else if((*data)[1] == C2O)
	    output("Server To Observer");

	output("Destination Addr");
	if((*data)[3] == OBSERVER_ADDRESS)
	    output("To Observer");
	else if((*data)[3] == TURRET_ADDRESS)
	    output("To Turret");

	output("");
	output("Order Type");
	if((*data)[4] == EXTINGUISH && (*data)[1] == C2T)
	    output("Extinguish");
	else if((*data)[4] == IMPACT_AREA_CHECK && (*data)[1] == C2O)
	    output("Request Check Extinguish");
	else if((*data)[4] == DATA_FOR_LAUNCH && (*data)[1] == C2T)
	    output("Request Launch");
	else if((*data)[4] == OBSTACLE_CHECK_REQUEST && (*data)[1] == C2T)
	    output("Request Obstacle Check");
	//============================================

	output("");
	output("Transmission Data");
	assembledData->print();

	output("");

	start = true;
	delete assembledData;
	assembledData = nullptr;
    }
}

bool ServerManualCommunicationModule::assemblePacket(char* const* packet, Data* assembleData)
{
    static bool start = true;
    static char* temp = nullptr;
    static int count = 0;

    if(start)
    {
	temp = new char[PURE_DATA_BUFFER_SIZE];
	start = false;
	count = 0;
    }
    else
    {
	char* pTemp = temp;
	temp = new char[(count + 1) * PURE_DATA_BUFFER_SIZE];

	int i = 0;
	while(i < PURE_DATA_BUFFER_SIZE * count)
	    temp[i] == pTemp[i];

	delete pTemp;
    }

    int i = 0;
    int headerSize = BUFFER_SIZE - PURE_DATA_BUFFER_SIZE - ERROR_CHECK_SIZE;
    while(i < PURE_DATA_BUFFER_SIZE)
    {
	temp[(count * PURE_DATA_BUFFER_SIZE) + i] = (*packet)[headerSize + i];
	i++;
    }

    bool finish = false;
    if((*packet)[5] == 0b00000100)
    {
	assembleData->deserialize(&temp);
	finish = true;
    }

    count++;

    if(finish)
    {
	start = true;
	delete temp;
	temp = nullptr;
	count = 0;
    }

    return finish;
}

void ServerManualCommunicationModule::read(char* const* data)
{
    static bool start = true;
    static bool finish = false;
    static char* serialData = nullptr;
    static int remainSize = 0;
    static int offset = 0;
    static DataPackConfig* dataPackConfig = nullptr;

    if(start)
    {
	output("");
	output("Reception Data");

	start = false;
	finish = false;
	dataPackConfig = new DataPackConfig();
	remainSize = createData(dataPackConfig, &serialData);
	offset = 0;

	output("");
    }

    finish = disAssemblePacket(data, dataPackConfig, &serialData, remainSize, offset);

    output("");
    output("received packet");
    for(int i = 0; i < BUFFER_SIZE; i++)
	output((*data)[i], true);
    output("");

    if(finish)
    {
	start = true;
	finish = false;
	delete serialData;
	serialData = nullptr;
	remainSize = 0;
	offset = 0;
	delete dataPackConfig;
	dataPackConfig = nullptr;
    }
    else
    {
	remainSize -= PURE_DATA_BUFFER_SIZE;
	offset += PURE_DATA_BUFFER_SIZE;
    }
}

int ServerManualCommunicationModule::createData(DataPackConfig* config, char** serialData)
{
    Data* d = nullptr;
    char signal;
    char order;
    char sAddress;

    output("Recieved Signal Type");
    output("1 : From Turret To Server, 2 : From Observer To Server");

    int signalType;
    input(&signalType);

    if(signalType == 1)
    {
	output("From Turret To Server");

	signal = T2C;
	sAddress = TURRET_ADDRESS;

	output("Order Type");
	output("1 : Obstacle Data, 2 : Launched");

	int orderType;
	input(&orderType);

	if(orderType == 1)
	{
	    order = RETURN_OBSTACLE_CHECK_DATA;
	    config->setting(signal, order, sAddress);
	    output("Obstacle Data");

	    output("Pitch Angle");
	    int pitch;
	    input(&pitch);
	    output("Distance");
	    int radius;
	    input(&radius);
	    d = new ShpericalData(0, pitch, radius);
	}
	else
	{
	    order = LAUNCH;
	    config->setting(signal, order, sAddress);
	    output("Launched");

	    d = new DummyData();
	}
    }
    else
    {
	output("From Observer To Server");

	signal = O2C;
	sAddress = OBSERVER_ADDRESS;

	output("Order Type");
	output("1 : Fire, 2 : Extinguish");

	int orderType;
	input(&orderType);

	if(orderType == 1)
	{
	    order = FIRE;
	    config->setting(signal, order, sAddress);
	    output("Fire");

#if defined(EXCEPTOBSERVER)
	    output("Except Observer Mode");
	    output("X Coordinate");
	    int x;
	    input(&x);
	    output("Y Coordinate");
	    int y;
	    input(&y);
	    output("Z Coordinate");
	    int z;
	    input(&z);
	    CoordinateData* temp = new CoordinateData(x, y, z - OBSERVER_HEIGHT);
	    output("Recieved Data");
	    temp->print();
	    d = new ShpericalData();
	    CoordToShp(temp, reinterpret_cast<ShpericalData*>(d));
	    output("Changed Data");
	    d->print();
#else
	    output("Yaw Angle");
	    int yaw;
	    input(&yaw);
	    output("Pitch Angle");
	    int pitch;
	    input(&pitch);
	    output("Distance");
	    int radius;
	    input(&radius);
	    d = new ShpericalData(yaw, pitch, radius);
#endif
	}
	else
	{
	    order = EXTINGUISH;
	    config->setting(signal, order, sAddress);
	    output("Extinguish");

	    d = new DummyData();
	}
    }

    int serialSize = d->serialize(serialData);
    delete d;

    return serialSize;
}

bool ServerManualCommunicationModule::disAssemblePacket(char* const* packet, DataPackConfig* config, char** serialData, int remainSize, int offset)
{
    //=================header===================
    (*packet)[0] = START_CLOCK;
    (*packet)[1] = config->_signal;
    (*packet)[2] = config->_sAddress;
    (*packet)[3] = getMyAddress();
    (*packet)[4] = config->_order;

    char tmp = 0b00000000;
    if(false)
	tmp = tmp | 0b00000001;

    bool finish = remainSize > PURE_DATA_BUFFER_SIZE ? false : true;
    if(!finish)
	tmp = tmp | 0b00000010;
    else
	tmp = tmp | 0b00000100;

    (*packet)[5] = tmp;
    (*packet)[6] = 0;//seed skip

    //================body=======================

    int compare = (PURE_DATA_BUFFER_SIZE < remainSize) ? PURE_DATA_BUFFER_SIZE : remainSize;
    for(int i = 0; i < compare; i++)
	(*packet)[7 + i] = (*serialData)[offset + i];

    //=================tail======================

    char parity = (*packet)[0];

    for(int i = 1; i < (BUFFER_SIZE - 1); i++)
	parity = (*packet)[i] ^ parity;

    (*packet)[BUFFER_SIZE - 1] = parity;

    return finish;
}
