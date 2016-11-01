#ifndef KWT_STC_H
#define KWT_STC_H
#include "main.h"
#include "nexus_pid_channel.h"

eRet OpenStc(void);
eRet ConfigureStc (eStcType StcType, NEXUS_PidChannelHandle pcrhandle);
void CloseStc(void);
#endif

