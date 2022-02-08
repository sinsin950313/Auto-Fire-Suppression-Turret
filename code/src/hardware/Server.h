#ifndef SERVER_H
#define SERVER_H

#include "Hardware.h"
#include "CommunicationModule.h"
#include "..\utility\CommonData.h"
#include "..\utility\CompileCondition.h"

//#define SERVER_BLUETOOTH_AND_MANUAL

#if defined(SERVER_BLUETOOTH_AND_MANUAL)
#include "ServerManualCommunicationModule.h"
#include "BluetoothCommunicationModule.h"
#endif


using namespace CommonData;

class Server : public Hardware
{
    public:
	Server(CommunicationModule* tcm, CommunicationModule* ocm);

	~Server() override;

	int init();

	int wait();

	int fire();

	int extinguish();

    private:
	class HardwareGlobalPositionData
	{
	    public:
		HardwareGlobalPositionData() { }

		~HardwareGlobalPositionData();

		void registerHardware(char hardwareID, int globalX, int globalY, int globalZ);//cm unit

		CoordinateData* getGlobalPosition(char hardwareID);

	    private:
		class CoordNode
		{
		    public:
			CoordNode(char id, int x, int y, int z) { _id = id; _position = new CoordinateData(x, y, z); }

			~CoordNode() { delete _position; _pNextNode = nullptr; }

			void setNextNode(CoordNode* pNextNode) { _pNextNode = pNextNode; }

			bool checkID(char id) { return _id == id ? true : false; }

			CoordinateData* getPosition() { return _position; }

			CoordNode* getNextNode() { return _pNextNode; }

		    private:
			char _id;

			CoordinateData* _position = nullptr;

			CoordNode* _pNextNode = nullptr;
		};

		CoordNode* pHead = nullptr;
	};

	//condition : we just use only one observer and one turret
	class Observer_Turret_LinkedList
	{
	    public:
		Observer_Turret_LinkedList() { };

		~Observer_Turret_LinkedList() { };

		char getLinkedTurretID(char observerID) { return CommunicationAddressList::TURRET_ADDRESS; }

		char getLinkedObserverID(char turretID) { return CommunicationAddressList::OBSERVER_ADDRESS; }
	};

	HardwareGlobalPositionData _hpData;

	Observer_Turret_LinkedList _otData;

	char findOptimumTurretID();

	void optimumPathAlgorithm(char turretID, CoordinateData* fireCoord, CoordinateData* obsCoor, int obsHeight, ShpericalData* reAimShp);

	int findOptimumVelocity(CoordinateData* fireRelative, CoordinateData* obsRelative, int obsHeight);

	bool isCatched(int velocity, int angle, int aimDistance, int aimHeight);

	void requestTurretLaunch(char turretID, ShpericalData* aimShp);

	bool requestTurretCheck(char turretID, CoordinateData* fireCoord, ShpericalData* reObsData);

	void requestObserverCheck(char observerID);

	CommunicationModule* _turretCM = nullptr;

	CommunicationModule* _observerCM = nullptr;

	bool request(CommunicationModule* cm, char dstAddress, Data* data);
};

#endif
