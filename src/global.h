#ifndef __KWT_GLOBAL_H
#define __KWT_GLOBAL_H

#include "main.h"
#include "bstd.h"
#include "b_os_lib.h"
#include "nexus_recpump.h"
#include "nexus_record.h"


#define FOCUS_COLOR 0xa56b8cf0
#define UNFOCUS_COLOR 0xaa0000A0

#define DISPLAY_HD 0
#define DISPLAY_SD 1


#define BUILD_DATE "20150824"
#define HARD_VER  "V1.0"

typedef struct
{
    B_ThreadHandle SchedulerThread;
    B_ThreadHandle InfoThread;
    B_ThreadHandle HotPlugThread;
    B_ThreadHandle CostTimeThread;
    B_MutexHandle Lock;
    B_SchedulerHandle Scheduler;

    NEXUS_AudioDacHandle AudioDacHandle;
    NEXUS_CompositeOutputHandle CompositeHandle;
    NEXUS_HdmiOutputHandle HdmiHandle;
    B_EventHandle HdcpEvent;
    B_SchedulerEventId HdcpEventId;
    unsigned int HdmiConnected;

    NEXUS_RecpumpHandle Recpump;
    NEXUS_RecordHandle Record;
    NEXUS_FileRecordHandle RecFile;
    char RecFileName[128];
    char RecDevPath[128];
    NEXUS_DisplayHandle Display[2];

    bwin_font_t Font10;
    bwin_font_t Font12;
    bwin_font_t Font14;
    bwin_font_t Font17;
    bwin_font_t big_menu;
    bwin_font_t small_menu;

    bwin_engine_t BwinEngine;
    bwin_t WinFrameBuffer;
    bwidget_engine_t WidgetEngine;

    NEXUS_SimpleStcChannelHandle SimpleStcChannel;
    NEXUS_SimpleVideoDecoderHandle SimpleVideoDecoder;

    NEXUS_VideoWindowHandle VideoWindow_HD;
    NEXUS_VideoWindowHandle VideoWindow_SD;

    NEXUS_SimpleAudioDecoderHandle SimpleAudioDecoder;
    NEXUS_ParserBand ParserBand;
    unsigned int ManulStopRec;
    NEXUS_GpioHandle en_485;

} G_SYSTEM;

typedef struct
{
    unsigned int ExitApp;
    unsigned int TunerType;
    unsigned int ConfigBlock;
    unsigned int ChannelBlock;
    unsigned int OperatingMode;
    unsigned int ChanelTuned;
    unsigned int LivePlaying;
    unsigned int ReTuneNeed;
    unsigned int ChannelLocked;
    unsigned int ChannelChanged;
    B_Time ChannelChangedTime;
    unsigned int PlaybackStarted;
    unsigned int MenuVolumeDisplayed;
    unsigned int RecRestartNeeded;
    unsigned int RecNextBootUp;
    unsigned int RtcInited;
} G_STATUS;

typedef struct
{
    unsigned int Format;
    unsigned int AspectRatio;
} HDMI_SETTINGS;

typedef struct
{
    unsigned int Format;
    unsigned int AspectRatio;
} CVBS_SETTINGS;

typedef enum NetWork_Protocol
{
    IpProtocol_eUdp,
    IpProtocol_eRtp
} Network_Protocol;

typedef struct
{
    unsigned int Type;
    unsigned int RemoteIP;
    unsigned int RemotePort;
    unsigned int Enable;
    Network_Protocol protocal;
} NETRELAY_SETTINGS;

typedef struct
{
    unsigned int RecOnStartEnable;
    unsigned int LoopRecEnable;
    unsigned int PreferredMedia;
    unsigned int RecFileSize;
} RECORD_SETTINGS;

typedef struct
{
    unsigned int DHCPEnable;
    unsigned int IPAddr;
    unsigned int NetMask;
    unsigned int GateWay;
} NETWORK_SETTINGS;

typedef struct
{
    unsigned int Version;
    unsigned int Volume;
    HDMI_SETTINGS HdmiSettings;
    CVBS_SETTINGS CvbsSettings;
    NETRELAY_SETTINGS NetRelaySettings;
    NETWORK_SETTINGS NetWorkSettings;
    RECORD_SETTINGS RecSettings;
} G_CONFIG;

#define MAX_CHANNELS 32
#define MAX_SUB_CHANNELS 5

typedef struct
{
    unsigned int VidPid;
    unsigned int AudPid;
    unsigned int PcrPid;
    unsigned int PmtPid;
    unsigned int VidType;
    unsigned int AudType;
    unsigned int DecryptEnable;
    unsigned char EncryptKeys[128];
} SUB_CHANNEL;

typedef struct
{
    unsigned int Frequency;
    unsigned int BandWidth;
    unsigned int SubChannelNum;
    SUB_CHANNEL SubChannels[MAX_SUB_CHANNELS];
} CHANNEL_LIST;

typedef struct
{
    unsigned int Version;
    unsigned int CurrentChannel;
    unsigned int CurrentSubChannel;
    unsigned int ChannelCount;
    unsigned int OverLoop;
    CHANNEL_LIST ChannelList[MAX_CHANNELS];
} G_CHANNEL;

extern G_CONFIG g_Config;
extern G_SYSTEM g_System;
extern G_STATUS g_Status;
extern G_CHANNEL g_Channel;

extern void InitializeParameters(void);
extern eRet LoadConfigFromFlash(void);
extern eRet SaveConfigToFlash(void);
extern void SetConfigToDefault(void);
#endif

