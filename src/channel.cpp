

#include <stdio.h>

#include "nexus_parser_band.h"

#include "nexus_stc_channel.h"
#include "nexus_pid_channel.h"
#include "nexus_input_band.h"


#include "tspsimgr2.h"

#include "ts_psi.h"
#include "ts_pat.h"



#include "main.h"
#include "video_decode.h"
#include "audio_decode.h"
#include "stc.h"
#include "global.h"
#include "dib3000.h"
#include "channellist.h"
#include "menu_descramblingconfig.h"

BDBG_MODULE(kwt_channel);
static    NEXUS_PidChannelHandle videoPidChannel, audioPidChannel,pcrPidChannel;






eRet GetChannelInfo(CHANNEL_INFO_T * pChanInfo, bool bScanning)
{
    BERR_Code err            = BERR_SUCCESS;
    int       patTimeout     = 100; /* in tsPsi_setTimeout2() this is 500msecs */
    int       patTimeoutOrig = 0;
    int       pmtTimeout     = 100; /* in tsPsi_setTimeout2() this is 500msecs */
    int       pmtTimeoutOrig = 0;
    if (true == bScanning)
    {
        /* adjust pat/pmt timeouts for faster scanning */
        tsPsi_getTimeout2(&patTimeoutOrig, &pmtTimeoutOrig);
        tsPsi_setTimeout2(patTimeout, pmtTimeout);
    }

    err = tsPsi_getChannelInfo2(pChanInfo, g_System.ParserBand);

    if (true == bScanning)
    {
        /* restore default pat/pmt timeouts */
        tsPsi_setTimeout2(patTimeoutOrig, pmtTimeoutOrig);
    }
    return ((BERR_SUCCESS == err) ? eRet_Ok : eRet_ExternalError);
}




int ChannelInfoChanged(CHANNEL_INFO_T ChannelInfo)
{
	int i=0;
	eRet ret;
	//ret=GetChannelInfo(&ChannelInfo,0);
	if(1)
	{
		if(g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannelNum != ChannelInfo.num_programs)
		{
			return 1;
		}
		for(i=0;i<ChannelInfo.num_programs;i++)
		{
			if(g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[i].VidPid != ChannelInfo.program_info[i].video_pids[0].pid)
			{
				return 1;
			}
			if(g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[i].VidType != ChannelInfo.program_info[i].video_pids[0].streamType)
			{
				return 1;
			}

			if(g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[i].AudPid != ChannelInfo.program_info[i].audio_pids[0].pid)
			{
				return 1;
			}

			if(g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[i].AudType != ChannelInfo.program_info[i].audio_pids[0].streamType)
			{
				return 1;
			}
            if(g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[i].PcrPid != ChannelInfo.program_info[i].pcr_pid)
			{
				return 1;
			}
            if(g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[i].PmtPid != ChannelInfo.program_info[i].map_pid)
			{
				return 1;
			}
		}

	}
	else
	{
		return 0;
	}
	return 0;


}






void PrintChannelInfo(void)
{
	eRet ret=eRet_Ok;
	int i=0;
	int j=0;
	CHANNEL_INFO_T ChannelInfo;


    ret=GetChannelInfo(&ChannelInfo,1);
	if(ret==eRet_Ok)
	{
		printf("found channel info\n");
		printf("version             =%02x\n",ChannelInfo.version);
		printf("transport_stream_id =%04x\n",ChannelInfo.transport_stream_id);
		printf("sectionBitmap       =%08x\n",ChannelInfo.sectionBitmap);
		printf("num_programs        =%04x\n",ChannelInfo.num_programs);
		for(i=0;i<ChannelInfo.num_programs;i++)
		{
			printf("program_info[%d].program_number =%04x\n",i,ChannelInfo.program_info[i].program_number);
			printf("program_info[%d].map_pid        =%04x\n",i,ChannelInfo.program_info[i].map_pid);
			printf("program_info[%d].version        =%02x\n",i,ChannelInfo.program_info[i].version);
			printf("program_info[%d].pcr_pid        =%04x\n",i,ChannelInfo.program_info[i].pcr_pid);
			printf("program_info[%d].ca_pid         =%04x\n",i,ChannelInfo.program_info[i].ca_pid);
			printf("program_info[%d].num_video_pids =%02x\n",i,ChannelInfo.program_info[i].num_video_pids);
			for(j=0;j<ChannelInfo.program_info[0].num_video_pids;j++)
			{
			    printf("program_info[%d].video_pids[%d].pid        =%04x\n",i,j,ChannelInfo.program_info[i].video_pids[j].pid);
			    printf("program_info[%d].video_pids[%d].streamType =%02x\n",i,j,ChannelInfo.program_info[i].video_pids[j].streamType);
			    printf("program_info[%d].video_pids[%d].ca_pid     =%04x\n",i,j,ChannelInfo.program_info[i].video_pids[j].ca_pid);
			}
			printf("program_info[%d].num_audio_pids =%02x\n",i,ChannelInfo.program_info[i].num_audio_pids);
			for(j=0;j<ChannelInfo.program_info[0].num_audio_pids;j++)
			{
			    printf("program_info[%d].audio_pids[%d].pid        =%04x\n",i,j,ChannelInfo.program_info[i].audio_pids[j].pid);
			    printf("program_info[%d].audio_pids[%d].streamType =%02x\n",i,j,ChannelInfo.program_info[i].audio_pids[j].streamType);
			    printf("program_info[%d].audio_pids[%d].ca_pid     =%04x\n",i,j,ChannelInfo.program_info[i].audio_pids[j].ca_pid);
			}
			printf("program_info[%d].num_other_pids =%02x\n",i,ChannelInfo.program_info[i].num_other_pids);
			for(j=0;j<ChannelInfo.program_info[0].num_other_pids;j++)
			{
			    printf("program_info[%d].other_pids[%d].pid        =%04x\n",i,j,ChannelInfo.program_info[i].other_pids[j].pid);
			    printf("program_info[%d].other_pids[%d].streamType =%02x\n",i,j,ChannelInfo.program_info[i].other_pids[j].streamType);
			    printf("program_info[%d].other_pids[%d].ca_pid     =%04x\n",i,j,ChannelInfo.program_info[i].other_pids[j].ca_pid);
			}

		}


    }
}



/*

typedef struct
{
	uint16_t	pid;
	uint8_t		streamType;
	uint16_t  	ca_pid;
} EPID;

#define MAX_PROGRAM_MAP_PIDS	12
typedef struct
{
	uint16_t	program_number;
	uint16_t	map_pid;
	uint8_t		version;
	uint16_t	pcr_pid;
	uint16_t  	ca_pid;
	uint8_t		num_video_pids;
	EPID		video_pids[MAX_PROGRAM_MAP_PIDS];
	uint8_t		num_audio_pids;
	EPID		audio_pids[MAX_PROGRAM_MAP_PIDS];
	uint8_t		num_other_pids;
	EPID		other_pids[MAX_PROGRAM_MAP_PIDS];
} PROGRAM_INFO_T;

#define MAX_PROGRAMS_PER_CHANNEL 64
typedef struct
{
	uint8_t		version;
	uint16_t	transport_stream_id;
	uint32_t	sectionBitmap;
	uint16_t	num_programs;
	PROGRAM_INFO_T program_info[MAX_PROGRAMS_PER_CHANNEL];
} CHANNEL_INFO_T;

*/


void InitializeParseBand(void)
{
	NEXUS_InputBand inputBand;
    NEXUS_ParserBandSettings parserBandSettings;
    NEXUS_InputBandSettings inputband_settings;
    NEXUS_Platform_GetStreamerInputBand(0, &inputBand);
    NEXUS_InputBand_GetSettings(inputBand,&inputband_settings);
	inputband_settings.useInternalSync=0;
    NEXUS_InputBand_SetSettings(inputBand,&inputband_settings);

    //printf("video_pid=%04x\n",VIDEO_PID);
    //printf("audio_pid=%04x\n",AUDIO_PID);


    /* Map a parser band to the streamer input band. */
    g_System.ParserBand = NEXUS_ParserBand_Open(NEXUS_ANY_ID);
    NEXUS_ParserBand_GetSettings(g_System.ParserBand, &parserBandSettings);
    parserBandSettings.sourceType = NEXUS_ParserBandSourceType_eInputBand;
    parserBandSettings.sourceTypeSettings.inputBand = inputBand;
    parserBandSettings.transportType = NEXUS_TransportType_eTs;
    NEXUS_ParserBand_SetSettings(g_System.ParserBand, &parserBandSettings);
}



/////////////////////////////////////////////////////////////////////////////
void SetPassWordInitAES128 (unsigned int channel_num)
{
	U8 end = 0x11;

	char setbuf[33];
	memset (setbuf, 0x00, 33);

	setbuf[0] = 3;

	memcpy (&setbuf[1],
				&g_Channel.ChannelList[channel_num].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[1], 16);


	U8 Val;
	U32 i;
	for (i = 0 ; i <= 16; i++)
	{
		Val = *(setbuf + i);
		printf ("val = %x in the channel\n", Val);
		NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, DESCRAMBLE_REG_BASE_ADDRESS + i, &Val, 1);
	}
	NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, 16 + 1, &end, 1);
}


void SetPassWordInitAES256 (unsigned int channel_num)
{
	U8 end = 0x11;

	unsigned char setbuf[33];
	memset (setbuf, 0x00, 33);
	setbuf[0] = 4;

	memcpy (&setbuf[1],
				&g_Channel.ChannelList[channel_num].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[1], 32);

	U8 Val;
	U32 i;
	for (i = 0 ; i <= 32; i++)
	{
		Val = *(setbuf + i);
		printf ("val = %x in the channel\n", Val);
		NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, DESCRAMBLE_REG_BASE_ADDRESS + i, &Val, 1);
	}
	NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, 32 + 1, &end, 1);
}

void SetPassWordInitDisable (unsigned int channel_num)
{
	U8 end = 0x11;

	char setbuf[33];
	memset (setbuf, 0x00, 33);
	setbuf[0] = 0;

	memcpy (&setbuf[1],
			&g_Channel.ChannelList[channel_num].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[1], 32);

	U8 Val;
	U32 i;
	for (i = 0 ; i <= 32; i++)
	{
		Val = *(setbuf + i);
		printf ("val = %x in the channel\n", Val);
		NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, DESCRAMBLE_REG_BASE_ADDRESS + i, &Val, 1);
	}
	NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, 32 + 1, &end, 1);
}

void SetPassWordInit (unsigned int channel_num)
{
	if (g_Channel.ChannelList[channel_num].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0] == 3)
	{
		SetPassWordInitAES128 (channel_num);
	}
	else if (g_Channel.ChannelList[channel_num].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0] == 4)
	{
		SetPassWordInitAES256 (channel_num);
	}
	else if (g_Channel.ChannelList[channel_num].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0] == 0)
	{
		SetPassWordInitDisable (channel_num);
	}
}
/////////////////////////////////////////////////////////////////////////////


void StartLiveChannel(unsigned int channel_num)
{

    eRet ret=eRet_Ok;
	int i=0;
	int j=0;
	unsigned int info_changed=0;


	CHANNEL_INFO_T ChannelInfo;

#if 0
	if(GetDib3000LockStatus()!=1)
	{
		videoPidChannel = NEXUS_PidChannel_Open(g_System.ParserBand,0x1011 , NULL);
        audioPidChannel = NEXUS_PidChannel_Open(g_System.ParserBand, 0x1100, NULL);
        pcrPidChannel = NEXUS_PidChannel_Open(g_System.ParserBand,  0x1001, NULL);

    	ConfigureStc(eStcType_ParserBand,pcrPidChannel);
    	SetSimpleVideoDecoderStc();
    	StartVideoDecode(videoPidChannel);
		StartSimpleAudioDecode(NEXUS_AudioCodec_eMpeg,audioPidChannel);

		g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannelNum=1;
		g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[0].VidPid=0x1011;
		g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[0].VidType=5;
		g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[0].AudPid=0x1100;
		g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[0].AudType=1;
        g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[0].PcrPid=0x1001;


		//SaveChannelToFlash();
		return ;

	}
#endif

	/* added by zh start */
		SetPassWordInit (channel_num);
	/* added by zh end */

    ret=GetChannelInfo(&ChannelInfo,1);



	if(ret==eRet_Ok)
	{
		printf("found channel info\n");
		printf("version             =%02x\n",ChannelInfo.version);
		printf("transport_stream_id =%04x\n",ChannelInfo.transport_stream_id);
		printf("sectionBitmap       =%08x\n",ChannelInfo.sectionBitmap);
		printf("num_programs        =%04x\n",ChannelInfo.num_programs);
		if(ChannelInfo.num_programs==0)
		{
			return;
		}

		for(i=0;i<ChannelInfo.num_programs;i++)
		{
			printf("program_info[%d].program_number =%04x\n",i,ChannelInfo.program_info[i].program_number);
			printf("program_info[%d].map_pid        =%04x\n",i,ChannelInfo.program_info[i].map_pid);
			printf("program_info[%d].version        =%02x\n",i,ChannelInfo.program_info[i].version);
			printf("program_info[%d].pcr_pid        =%04x\n",i,ChannelInfo.program_info[i].pcr_pid);
			printf("program_info[%d].ca_pid         =%04x\n",i,ChannelInfo.program_info[i].ca_pid);
			printf("program_info[%d].num_video_pids =%02x\n",i,ChannelInfo.program_info[i].num_video_pids);
			for(j=0;j<ChannelInfo.program_info[0].num_video_pids;j++)
			{
			    printf("program_info[%d].video_pids[%d].pid        =%04x\n",i,j,ChannelInfo.program_info[i].video_pids[j].pid);
			    printf("program_info[%d].video_pids[%d].streamType =%02x\n",i,j,ChannelInfo.program_info[i].video_pids[j].streamType);
			    printf("program_info[%d].video_pids[%d].ca_pid     =%04x\n",i,j,ChannelInfo.program_info[i].video_pids[j].ca_pid);
			}
			printf("program_info[%d].num_audio_pids =%02x\n",i,ChannelInfo.program_info[i].num_audio_pids);
			for(j=0;j<ChannelInfo.program_info[0].num_audio_pids;j++)
			{
			    printf("program_info[%d].audio_pids[%d].pid        =%04x\n",i,j,ChannelInfo.program_info[i].audio_pids[j].pid);
			    printf("program_info[%d].audio_pids[%d].streamType =%02x\n",i,j,ChannelInfo.program_info[i].audio_pids[j].streamType);
			    printf("program_info[%d].audio_pids[%d].ca_pid     =%04x\n",i,j,ChannelInfo.program_info[i].audio_pids[j].ca_pid);
			}
			printf("program_info[%d].num_other_pids =%02x\n",i,ChannelInfo.program_info[i].num_other_pids);
			for(j=0;j<ChannelInfo.program_info[0].num_other_pids;j++)
			{
			    printf("program_info[%d].other_pids[%d].pid        =%04x\n",i,j,ChannelInfo.program_info[i].other_pids[j].pid);
			    printf("program_info[%d].other_pids[%d].streamType =%02x\n",i,j,ChannelInfo.program_info[i].other_pids[j].streamType);
			    printf("program_info[%d].other_pids[%d].ca_pid     =%04x\n",i,j,ChannelInfo.program_info[i].other_pids[j].ca_pid);
			}

		}



		if(ChannelInfoChanged(ChannelInfo))
		{
			info_changed=1;
		}

		g_Channel.ChannelList[channel_num].SubChannelNum=ChannelInfo.num_programs;
		for(i=0;i<ChannelInfo.num_programs;i++)
		{
			g_Channel.ChannelList[channel_num].SubChannels[i].VidPid=ChannelInfo.program_info[i].video_pids[0].pid;
			g_Channel.ChannelList[channel_num].SubChannels[i].VidType=ChannelInfo.program_info[i].video_pids[0].streamType;

			g_Channel.ChannelList[channel_num].SubChannels[i].AudPid=ChannelInfo.program_info[i].audio_pids[0].pid;
			g_Channel.ChannelList[channel_num].SubChannels[i].AudType=ChannelInfo.program_info[i].audio_pids[0].streamType;

            g_Channel.ChannelList[channel_num].SubChannels[i].PcrPid=ChannelInfo.program_info[i].pcr_pid;
			g_Channel.ChannelList[channel_num].SubChannels[i].PmtPid=ChannelInfo.program_info[i].map_pid;
		}
		if(info_changed==1)
		{
			info_changed=0;
			printf("ChannelInfoChanged\n");
			SaveChannelToFlash();
		}





		if(g_Channel.CurrentSubChannel>=g_Channel.ChannelList[channel_num].SubChannelNum)
		{
            g_Channel.CurrentSubChannel=0;
		}



    	videoPidChannel = NEXUS_PidChannel_Open(g_System.ParserBand, ChannelInfo.program_info[ g_Channel.CurrentSubChannel].video_pids[0].pid, NULL);
        audioPidChannel = NEXUS_PidChannel_Open(g_System.ParserBand, ChannelInfo.program_info[ g_Channel.CurrentSubChannel].audio_pids[0].pid, NULL);
        pcrPidChannel = NEXUS_PidChannel_Open(g_System.ParserBand,  ChannelInfo.program_info[ g_Channel.CurrentSubChannel].pcr_pid, NULL);

    	ConfigureStc(eStcType_ParserBand,pcrPidChannel);
    	SetSimpleVideoDecoderStc();
    	StartVideoDecode(videoPidChannel);
        if(ChannelInfo.program_info[0].audio_pids[0].streamType==1)
        {
    		StartSimpleAudioDecode(NEXUS_AudioCodec_eMpeg,audioPidChannel);
        }
        else if(ChannelInfo.program_info[0].audio_pids[0].streamType==3)
        {
    		StartSimpleAudioDecode(NEXUS_AudioCodec_eAac,audioPidChannel);
        }
        else if(ChannelInfo.program_info[0].audio_pids[0].streamType==7)
        {
    		StartSimpleAudioDecode(NEXUS_AudioCodec_eAc3Plus,audioPidChannel);
        }

		if(g_System.RecFile != NULL)
		{
			g_Status.RecRestartNeeded=1;
		}
	}

#if 0
	else
	{
		videoPidChannel = NEXUS_PidChannel_Open(g_System.ParserBand,0x1011 , NULL);
        audioPidChannel = NEXUS_PidChannel_Open(g_System.ParserBand, 0x1100, NULL);
        pcrPidChannel = NEXUS_PidChannel_Open(g_System.ParserBand,  0x1001, NULL);

    	ConfigureStc(eStcType_ParserBand,pcrPidChannel);
    	SetSimpleVideoDecoderStc();
    	StartVideoDecode(videoPidChannel);
		StartSimpleAudioDecode(NEXUS_AudioCodec_eMpeg,audioPidChannel);

		g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannelNum=1;
		g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[0].VidPid=0x1011;
		g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[0].VidType=5;
		g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[0].AudPid=0x1100;
		g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[0].AudType=1;
        g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[0].PcrPid=0x1001;
		//SaveChannelToFlash();



	}



#endif

    return ;
}


void StopLiveChannel(void)
{
	StopSimpleVideoDecode();
	StopSimpleAudioDecode();
	if(videoPidChannel != NULL)
	{
		NEXUS_PidChannel_Close(videoPidChannel);
		videoPidChannel = NULL;
	}
	if(audioPidChannel != NULL)
	{
    	NEXUS_PidChannel_Close(audioPidChannel);
        audioPidChannel = NULL;
	}
	if(pcrPidChannel != NULL)
	{
		NEXUS_PidChannel_Close(pcrPidChannel);
		pcrPidChannel = NULL;
	}

}








