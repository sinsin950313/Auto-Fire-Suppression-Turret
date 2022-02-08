#include "TurretManualCommunicationModule.h"
#include "..\utility\IOModule.h"

using namespace CustomIO;

TurretManualCommunicationModule::TurretManualCommunicationModule(TypeList ht, char myAddress) : CommunicationModule(ht, myAddress)
{
    _ht = ht;
}

TurretManualCommunicationModule::~TurretManualCommunicationModule()
{

}

void TurretManualCommunicationModule::write(char* const* data)
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

#if defined(DEBUG)
    output("");
    output("Sending packet in TMCM");
    for(int i = 0; i < BUFFER_SIZE; i++)
	output((*data)[i]);
    output("");
#endif

    finish = assemblePacket(data, assembledData);

    if(finish)
    {
	output("");

	output("Sending Data in TMCM");
	output("Turret To Server");

	output("");
	output("Order Type");
	if((*data)[4] == LAUNCH && (*data)[1] == T2C)
	    output("Launch");
	else if((*data)[4] == RETURN_OBSTACLE_CHECK_DATA && (*data)[1] == T2C)
	    output("Return Obstacle");

	output("");
	output("Transmission Data");
	assembledData->print();

	output("");

	start = true;
	delete assembledData;
	assembledData = nullptr;
    }
}

bool TurretManualCommunicationModule::assemblePacket(char* const* packet, Data* assembleData)
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
    if((*packet)[5] & 0b00000100)
    {
	assembleData->deserialize(&temp);
	finish = true;
    }

    CustomIO::output(111);
    CustomIO::output((*packet)[5]);
    CustomIO::output(111);

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

void TurretManualCommunicationModule::read(char* const* data)
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
	output("Reception Data in TMCM");

	start = false;
	finish = false;
	dataPackConfig = new DataPackConfig();
	remainSize = createData(dataPackConfig, &serialData);
	offset = 0;

	output("");
    }

    finish = disAssemblePacket(data, dataPackConfig, &serialData, remainSize, offset);

#if defined(DEBUG)
    output("");
    output("received packet in TMCM");
    for(int i = 0; i < BUFFER_SIZE; i++)
	output((*data)[i]);
    output("");
#endif

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

int TurretManualCommunicationModule::createData(DataPackConfig* config, char** serialData)
{
    Data* d = nullptr;
    char signal;
    char order;
    char sAddress;

    output("From Server To Turret");
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
	order = DATA_FOR_LAUNCH;
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

    int serialSize = d->serialize(serialData);
    delete d;

    return serialSize;
}

bool TurretManualCommunicationModule::disAssemblePacket(char* const* packet, DataPackConfig* config, char** serialData, int remainSize, int offset)
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
