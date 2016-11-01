#ifndef KWT_PLAYBACK_H
#define KWT_PLAYBACK_H
#include "main.h"
eRet InitializePlayback(void);
void StartPlayBack(const char *file_name);
eRet StopPlayback(void);
eRet PausePlayback(void);

eRet PlayPlayback(void);

eRet UninitializePlayback(void);
void DumpPlaybackStatus(void);
void GetPlaybackStatus(unsigned int *cur,unsigned int *total);

eRet TrickPlaybackMode(signed int  speed);
eRet PlayPlaySeek(int add);
int CheckIndexExistAndSize(const char *video_file,char *index_file);






#endif

