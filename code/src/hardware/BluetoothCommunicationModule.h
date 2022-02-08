#ifndef BLUETOOTHCOMMUNICATIONMODULE_H
#define BLUETOOTHCOMMUNICATIONMODULE_H

#include "CommunicationModule.h"
#include "..\imLibrary\Bluetooth.h"

class BluetoothCommunicationModule : public CommunicationModule
{
    public:
	BluetoothCommunicationModule(TypeList ht, char myAddress, Bluetooth* sender, Bluetooth* receiver);

	~BluetoothCommunicationModule() override;

    protected:
	void write(char* const* data) override;

	void read(char* const* data) override;

    private:
	void connect(char* pAddr);

	Bluetooth* _sender = nullptr;

	Bluetooth* _receiver = nullptr;

	class pAddrToMacConverter
	{
	    public:
		pAddrToMacConverter();

		~pAddrToMacConverter();

		char const * convert(char pAddr);

	    private:
		class Node
		{
		    public:
			Node() { }

			Node(char pAddr, char* macAddr);

			~Node();

			bool compare(char pAddr);

			char const * getMacAddr();

		    private:
			char _pAddr = 0;

			char* _macAddr = nullptr;
		};

		const int _nodeSize = 3;

		Node** _nodeArray = nullptr;
	};

	pAddrToMacConverter* _converter = nullptr;

	char const _privateAnswerSignal = 'a';
};

#endif
