/*
 * code by jinhwa & minkeun
 */

#include "SoundModule.h"
 
#define WT2003S_BAUDRATE 9600

SoundModule::SoundModule(int txPin, int rxPin)
{
    SoftwareSerial* swSer = new SoftwareSerial(txPin, rxPin);
    swSer->begin(WT2003S_BAUDRATE);
    _wt = new WT2003S<SoftwareSerial>();
    _wt->init(*swSer);
    getSong();
}

SoundModule::~SoundModule()
{
    delete _wt;
    delete[] SDSong;
}

void SoundModule::play(int song)
{
    _wt->playSDRootSong(song);
}

void SoundModule::stop()
{
    _wt->pause_or_play();
}

void SoundModule::getSong()
{
     int diskStatus = _wt->getDiskStatus();
     
     if (diskStatus && 0x02)
     {
	int sd_songs = _wt->getSDMp3FileNumber();
	
	if(sd_songs > 0)
	{
	    //SDSong = (struct Play_history*)malloc((sd_songs + 1) * sizeof(struct Play_history));
	    SDSong = new Play_history[sd_songs + 1];
	    readSongName(SDSong, sd_songs);
	}
     }
}

void SoundModule::readSongName(struct Play_history* ph, uint32_t num)
{
    _wt->volume(0);
    delay(100);

    _wt->playSDRootSong(0x0001);

    for(int i = 0; i < num; i++)
    {
	delay(300);
	ph[i].disk = SD;
	ph[i].index = _wt->getTracks();
	_wt->getSongName(ph[i].name);
	_wt->next();
    }

    _wt->pause_or_play();

    _wt->volume(vol);
    delay(100);
}
