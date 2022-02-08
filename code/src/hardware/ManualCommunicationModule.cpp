#include "ManualCommunicationModule.h"
#include "..\utility\IOModule.h"

using namespace CustomIO;

ManualCommunicationModule::ManualCommunicationModule(TypeList ht, char myAddress) : CommunicationModule(ht, myAddress)
{
    _ht = ht;
}

ManualCommunicationModule::~ManualCommunicationModule()
{

}

void ManualCommunicationModule::write(char* const* data)
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

    finish = assemblePacket(data, assembledData);

    if(finish)
    {
	output("============================");
	output("Sending Data");

	output("Signal Type");
	if((*data)[1] == T2C)
	    output("T2S");
	else if((*data)[1] == C2T)
	    output("S2T");
	else if((*data)[1] == C2O)
	    output("S2O");
	else if((*data)[1] == O2C)
	    output("O2S");

	output("");
	output("sAddr");
	if((*data)[2] == CENTRAL_ADDRESS)
	{
	    output("From S");
	    output((*data)[2], true);
	}
	else if((*data)[2] == OBSERVER_ADDRESS)
	{
	    output("From O");
	    output((*data)[2], true);
	}
	else if((*data)[2] == TURRET_ADDRESS)
	{
	    output("From T");
	    output((*data)[2], true);
	}

	output("DestAddr");
	if((*data)[3] == CENTRAL_ADDRESS)
	{
	    output("To S");
	    output((*data)[3], true);
	}
	else if((*data)[3] == OBSERVER_ADDRESS)
	{
	    output("To O");
	    output((*data)[3], true);
	}
	else if((*data)[3] == TURRET_ADDRESS)
	{
	    output("To T");
	    output((*data)[3], true);
	}

	output("");
	output("Order Type");
	if((*data)[4] == EXTINGUISH && (((*data)[1] == O2C) || (*data)[1] == C2T))
	    output("Extinguish");
	else if((*data)[4] == FIRE && (*data)[1] == O2C)
	    output("Fire");
	else if((*data)[4] == IMPACT_AREA_CHECK && (*data)[1] == C2O)
	    output("Request Check Extinguish");
	else if((*data)[4] == DATA_FOR_LAUNCH && (*data)[1] == C2T)
	    output("Request Launch");
	else if((*data)[4] == OBSTACLE_CHECK_REQUEST && (*data)[1] == C2T)
	    output("Request Obstacle");
	/*else if((*data)[4] == INIT)
	    output("Broadcasting Initialize");*/
	else if((*data)[4] == LAUNCH && (*data)[1] == T2C)
	    output("Launch");
	else if((*data)[4] == RETURN_OBSTACLE_CHECK_DATA && (*data)[1] == T2C)
	    output("Return Obstacle");
	//============================================

	output("");
	output("Trans Data");
	assembledData->print();

	output("============================");

	start = true;
	delete assembledData;
	assembledData = nullptr;
    }
}

bool ManualCommunicationModule::assemblePacket(char* const* packet, Data* assembleData)
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

void ManualCommunicationModule::read(char* const* data)
{
    static bool start = true;
    static bool finish = false;
    static char* serialData = nullptr;
    static int remainSize = 0;
    static int offset = 0;
    static DataPackConfig* dataPackConfig = nullptr;

    if(start)
    {
	output("============================");
	output("Reception Data");

	start = false;
	finish = false;
	dataPackConfig = new DataPackConfig();
	remainSize = createData(dataPackConfig, &serialData);
	offset = 0;

	output("============================");
    }

    finish = disAssemblePacket(data, dataPackConfig, &serialData, remainSize, offset);

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

int ManualCommunicationModule::createData(DataPackConfig* config, char** serialData)
{
    Data* d = nullptr;
    char signal;
    char order;
    char sAddress;

    if(_ht == TURRET)
    {
	output("From S To T");
	signal = C2T;
	sAddress = CENTRAL_ADDRESS;

	output("Order Type");
	output("2 : Obstacle, 3 : Launch, 4 : Extinguish");

	int orderType;
	input(&orderType);

	if(orderType == 2)
	{
	    order = OBSTACLE_CHECK_REQUEST;
	    config->setting(signal, order, sAddress);
	    output("Obstacle");

	    output("Yaw Angle");
	    int yaw;
	    input(&yaw);
	    output("Pitch Angle");
	    int pitch;
	    input(&pitch);
	    output("Radius");
	    int radius;
	    input(&radius);
	    d = new ShpericalData(yaw, pitch, radius);
	}
	else if(orderType == 3)
	{
	    order = LAUNCH;
	    config->setting(signal, order, sAddress);
	    output("Launch");

	    output("Yaw Angle");
	    int yaw;
	    input(&yaw);
	    output("Pitch Angle");
	    int pitch;
	    input(&pitch);
	    output("Velocity");
	    int radius;
	    input(&radius);
	    d = new ShpericalData(yaw, pitch, radius);
	}
	else
	{
	    order = EXTINGUISH;
	    config->setting(signal, order, sAddress);
	    output("Extinguish");

	    d = new DummyData();
	}

    }
    else if(_ht == OBSERVER)
    {
	output("From S To O");
	signal = C2O;
	sAddress = CENTRAL_ADDRESS;

	output("Order Type");
	output("2 : Extinguish");

	order = IMPACT_AREA_CHECK;
	config->setting(signal, order, sAddress);
	output("Extinguish");

	d = new DummyData();
    }
    else
    {
	output("Recieved Signal Type");
	output("1 : From T To S, 2 : From O To T");

	int signalType;
	input(&signalType);

	if(signalType == 1)
	{
	    output("From T To S");

	    signal = T2C;
	    sAddress = TURRET_ADDRESS;

	    output("Order Type");
	    output("1 : Obstacle, 2 : Launched");

	    int orderType;
	    input(&orderType);

	    if(orderType == 1)
	    {
		order = RETURN_OBSTACLE_CHECK_DATA;
		config->setting(signal, order, sAddress);
		output("Obstacle");

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
	    output("From O To T");

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
	    }
	    else
	    {
		order = EXTINGUISH;
		config->setting(signal, order, sAddress);
		output("Extinguish");

		d = new DummyData();
	    }
	}
    }

    int serialSize = d->serialize(serialData);
    delete d;

    return serialSize;
}

bool ManualCommunicationModule::disAssemblePacket(char* const* packet, DataPackConfig* config, char** serialData, int remainSize, int offset)
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
