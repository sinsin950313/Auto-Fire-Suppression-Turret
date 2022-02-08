#include "BluetoothCommunicationModule.h"
#include "CommunicationSignal.h"
#include "Arduino.h"
#include "..\utility\IOModule.h"

BluetoothCommunicationModule::pAddrToMacConverter::Node::Node(char pAddr, char* macAddr)
{
    _pAddr = pAddr;
    _macAddr = macAddr;
}

BluetoothCommunicationModule::pAddrToMacConverter::Node::~Node()
{
    delete _macAddr;
}

bool BluetoothCommunicationModule::pAddrToMacConverter::Node::compare(char pAddr)
{
    return _pAddr == pAddr ? true : false;
}

char const * BluetoothCommunicationModule::pAddrToMacConverter::Node::getMacAddr()
{
    return _macAddr;
}

BluetoothCommunicationModule::pAddrToMacConverter::pAddrToMacConverter()
{
    //list of reciever
    _nodeArray = new Node*[_nodeSize];
    char pAddr = 0;
    char* macAddr = nullptr;

    pAddr = CommunicationAddressList::OBSERVER_ADDRESS;
    macAddr = new char[12] {'6', 'C', 'C', '3', '7', '4', 'F', 'E', '0', '3', '7', '2'};
    _nodeArray[0] = new Node(pAddr, macAddr); //blue

    pAddr = CommunicationAddressList::CENTRAL_ADDRESS;
    macAddr = new char[12] {'6', 'C', 'C', '3', '7', '4', 'F', 'E', '9', '6', '7', 'F'};
    _nodeArray[1] = new Node(pAddr, macAddr); //red

    pAddr = CommunicationAddressList::TURRET_ADDRESS;
    macAddr = new char[12] {'6', 'C', 'C', '3', '7', '4', 'F', 'E', '9', 'E', '5', '2'};
    _nodeArray[2] = new Node(pAddr, macAddr); //green
}

BluetoothCommunicationModule::pAddrToMacConverter::~pAddrToMacConverter()
{
    int i = 0;
    while(i < _nodeSize)
    {
	delete _nodeArray[i];
	i++;
    }

    delete _nodeArray;
}

char const * BluetoothCommunicationModule::pAddrToMacConverter::convert(char pAddr)
{
    int i = 0;
    while(i < _nodeSize)
    {
	if(_nodeArray[i]->compare(pAddr))
	{
#if defined(DEBUG)
	    CustomIO::output("Converting Success");
	    CustomIO::output("Mac Address");
	    int j = 0;
	    while(j < 12)
	    {
		Serial.begin(115200);
		Serial.write(_nodeArray[i]->getMacAddr()[j]);
		Serial.end();
		j++;
	    }
	    Serial.begin(115200);
	    Serial.println("");
	    Serial.end();
#endif
	    return _nodeArray[i]->getMacAddr();
	}
	i++;
    }

#if defined(DEBUG)
    CustomIO::output("null convert");
#endif

    return nullptr;
}

BluetoothCommunicationModule::BluetoothCommunicationModule(TypeList ht, char myAddress, Bluetooth* sender, Bluetooth* receiver) : CommunicationModule(ht, myAddress)
{
    _converter = new pAddrToMacConverter();
    _sender = sender;
    _receiver = receiver;

    char * addr = _converter->convert(myAddress);
    while(_receiver->isConnected() || _sender->isConnected())
    {
	CustomIO::output("Bluetooth Checking...");
	_sender->at();
	_receiver->at();

	_sender->connect(&addr);
	delay(1000);
	_sender->disconnect();
	_receiver->disconnect();
	delay(1000);
	CustomIO::output(_sender->isConnected(), true);
	CustomIO::output(_receiver->isConnected(), true);
    }
}

BluetoothCommunicationModule::~BluetoothCommunicationModule()
{
    delete _converter;
    delete _sender;
    delete _receiver;
}

void BluetoothCommunicationModule::write(char* const* data)
{
    char const * mac = _converter->convert((*data)[3]);

    CustomIO::output("Sending...");

    delay(1000);
    _sender->connect(&mac);

#if defined(DEBUG)
    CustomIO::output("===================================");
    CustomIO::output("Sending Start");
    int i_ = 0;
    while(i_ < BUFFER_SIZE)
    {
	CustomIO::output((*data)[i_], true);
	i_++;
    }
    CustomIO::output("===================================");
#endif

    int i = 0;
    while(i < BUFFER_SIZE)
    {
	_sender->write((*data)[i]);
	i++;
    }

    delay(500);

    char c = 0;
    while(c != _privateAnswerSignal)
    {
#if defined(DEBUG)
	CustomIO::output("Bluetooth - wait Response");
#endif
	c = _sender->read();
#if defined(DEBUG)
	CustomIO::output(c, true);
#endif
    }

    delay(500);
    _sender->disconnect();
}

void BluetoothCommunicationModule::read(char* const* data)
{
    CustomIO::output("Receiving...");
    char c = 0;
    do
    {
	c = _receiver->read();

#if defined(DEBUG)
	CustomIO::output(c, true);
	/*
	CustomIO::output(START_CLOCK ^ c, true);
	bool b = START_CLOCK ^ c != 0;
	CustomIO::output(b);
	b = START_CLOCK ^ c != 0b00000000;
	CustomIO::output(b);
	*/
#endif

    } while(START_CLOCK != c );

    (*data)[0] = c;

    int i = 1;
    while(i < BUFFER_SIZE)
    {
	c = _receiver->read();
	(*data)[i] = c;
	i++;
    }

    delay(500);

    while(_receiver->isConnected())
    {
#if defined(DEBUG)
	CustomIO::output("Bluetooth - send Response");
	CustomIO::output(_privateAnswerSignal);
#endif
	_receiver->write(_privateAnswerSignal);
	delay(500);
    }

#if defined(DEBUG)
    CustomIO::output("===================================");
    CustomIO::output("Reception Finish");
    i = 0;
    while(i < BUFFER_SIZE)
    {
	CustomIO::output((*data)[i], true);
	i++;
    }
    CustomIO::output("===================================");
#endif
}
