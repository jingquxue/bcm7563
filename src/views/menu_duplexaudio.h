#ifndef KWT_MENU_DUPLEXAUDIO_H
#define KWT_MENU_DUPLEXAUDIO_H

typedef struct
{
	unsigned char ADCdelay;
	unsigned short CheckLowNUM;
	unsigned char Ad1;
	unsigned char Ad2;
	unsigned char Ad3;
	unsigned char AudioInSet;
	unsigned char AudioOutSet;
	unsigned char AudioPGA;
	unsigned char VOR_Ad1;
	unsigned char VOR_Ad2;
	unsigned char VOR_Ad3;
	unsigned char VOT_Channel;
} DuplexAudio;

eRet DuplexAudioGet (DuplexAudio *m_DuplexAudio);
void DuplexAudioSet (DuplexAudio *m_DuplexAudio);

void CloseDuplexAudioMenu ();
void DuplexAudioInit (void);
void DuplexAudioShow (bool show);
int MenuDuplexAudioKeyDown (unsigned int key_code);
#endif

