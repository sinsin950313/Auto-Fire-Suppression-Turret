#ifndef COMMUNICATIONMODULE_H
#define COMMUNICATIONMODULE_H

#include "HardwareType.h"
#include "../utility/CommonData.h"
#include "CommunicationSignal.h"
#include "..\utility\CompileCondition.h"

using namespace HardwareType;
using namespace CommonData;
using namespace CommunicationSignalList;
using namespace CommunicationAddressList;

class CommunicationModule
{
    private:
	class CommunicationPaper
	{
	    public:
		void setSignalFlag(const char flag) { _signalFlag = flag; }

		void setOrderFlag(const char flag) { _orderFlag = flag; }

		void setSourceID(const char id) { _sourceID = id; }

		void resetFlag() { _signalFlag = _signalOriginFlag; _orderFlag = _orderOriginFlag; }

		char getSignalFlag() { return _signalFlag; }

		char getOrderFlag() { return _orderFlag; }

		char getSourceID() { return _sourceID; }

	    private:
		static char const _signalOriginFlag = 0b00000000;

		static char const _orderOriginFlag = 0b00000000;

		char _signalFlag;

		char _orderFlag;

		char _sourceID;
	};

    public:
	CommunicationModule(TypeList ht, char myAddress = CENTRAL_ADDRESS);

	virtual ~CommunicationModule();

	bool send(Data& data, char dstAddress = CENTRAL_ADDRESS);//return comfirmation of transmission

	void receive();

	void setT2Cmode() { _order->setSignalFlag(T2C); }

	void setC2Tmode() { _order->setSignalFlag(C2T); }

	void setC2Omode() { _order->setSignalFlag(C2O); }

	void setO2Cmode() { _order->setSignalFlag(O2C); }

	void setO2CExtinguish() { _order->setOrderFlag(EXTINGUISH); }

	void setO2CFire() { _order->setOrderFlag(FIRE); }

	void setC2OCheck() { _order->setOrderFlag(IMPACT_AREA_CHECK); }

	void setC2OInit() { _order->setOrderFlag(INIT); }

	void setC2TLaunch() { _order->setOrderFlag(DATA_FOR_LAUNCH); }

	void setC2TCheck() { _order->setOrderFlag(OBSTACLE_CHECK_REQUEST); }

	void setC2TExtinguish() { _order->setOrderFlag(EXTINGUISH); }

	void setC2TInit() { _order->setOrderFlag(INIT); }

	void setT2CLaunch() { _order->setOrderFlag(LAUNCH); }

	void setT2CData() { _order->setOrderFlag(RETURN_OBSTACLE_CHECK_DATA); }

	char getSignal() { return _request->getSignalFlag(); }

	char getOrder() { return _request->getOrderFlag(); }

	char getSourceID() { return _request->getSourceID(); }

	char getCentralAdress() const { return CENTRAL_ADDRESS; }

	char getMyAddress() { return _myAddress; }

	Data & getTransmittedData() { return *_dataPointer; }

	static int const BUFFER_SIZE = 24;

	static int const PURE_DATA_BUFFER_SIZE = 16;

	static int const ERROR_CHECK_SIZE = 1;

	static char const START_CLOCK = 0b01010101;

    protected:
	virtual void write(char* const* data) = 0;//return true when ACK signal is input

	virtual void read(char* const* data) = 0;

    private:
	long pTimeForWait();

	long pRandomForSeed();

	void packetClear();

	bool isResponse();//return true when receive response

	int setPacketHeader(bool ACK, bool separate, char seed, char dstAddress);//return header size

	bool fillPacketBody(int headerOffset, char** src, int srcOffset, int remainSize);//return packet is finished

	void setErrorDetectingCode();

	bool receiveSimple(bool timeLimit);

	bool checkErrorDetecting();

	void writeRequest();

	bool fillData();

	void response();

	void changeOrderToResponse();

	void restoreOrder(char storedType, char storedOrder);

	TypeList _ht;

	char* const _packetBuffer = new char[BUFFER_SIZE];//single thread

	char _myAddress = 0;

	Data* _dataPointer = nullptr;

	CommunicationPaper* const _order = new CommunicationPaper();

	CommunicationPaper* const _request = new CommunicationPaper();
};

#endif
