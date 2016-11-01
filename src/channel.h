#ifndef KWT_CHANNEL_H
#define KWT_CHANNEL_H
#include "tspsimgr2.h"

void StartLiveChannel(unsigned int channel_num);

void StopLiveChannel(void);
void InitializeParseBand(void);
void PrintChannelInfo(void);
int ChannelInfoChanged(CHANNEL_INFO_T ChannelInfo);

#endif

