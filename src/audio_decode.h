#ifndef KWT_AUDIO_DECODE_H
#define KWT_AUDIO_DECODE_H
#include "main.h"
#include "nexus_audio_types.h"
#include "nexus_pid_channel.h"
eRet OpenSimpleAudioDecode(void);
eRet StartSimpleAudioDecode(NEXUS_AudioCodec audio_codec,NEXUS_PidChannelHandle audio_pid_channel);
void  StopSimpleAudioDecode(void);
void  CloseSimpleAudioDecode(void);
eRet SetSimpleAudioDecodeVolume(uint32_t level);
uint32_t GetSimpleAudioDecodeVolume(void);
unsigned int GetSimpleAudioDecodeFrame(void);




#endif
