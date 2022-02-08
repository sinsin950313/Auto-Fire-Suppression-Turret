/*
 * code by jinhwa & minkeun
 */

#ifndef SOUNDMODULE_H
#define SOUNDMODULE_H

#include <SoftwareSerial.h>
#include "Device.h"
#include "WT2003S_Player.h"

class SoundModule : public Device
{
    public:
	SoundModule(int txPin, int rxPin);  // pin number, UART

	~SoundModule();

	void play(int song);

	void stop();
	
    private:
	class Play_history
	{
	    public:
		Play_history() {};
		int disk;  // uint8_t
		int index;  // uint16_t  	
		char name[20];
	};

	Play_history* SDSong = nullptr;

	void getSong();

	void readSongName(struct Play_history* ph, uint32_t num);

	WT2003S<SoftwareSerial>* _wt = nullptr;

	int vol = 10;
};

#endif
