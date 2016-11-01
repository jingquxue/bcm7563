


#include "nexus_playback.h"
#include "nexus_file.h"

#include "nexus_pid_channel.h"

#include <unistd.h> //file i/o
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "channel.h"
#include "stc.h"
#include "video_decode.h"
#include "audio_decode.h"
#include "global.h"
#include "menu_playback.h"
#include "menu_fileBrowser.h"

#include "bmedia_probe.h"
#include "bmpeg2ts_probe.h"
#include "bfile_stdio.h"

static unsigned video_pid=0x1011;
static unsigned audio_pid=0x1100;
static bvideo_codec video_codec=bvideo_codec_h264;
static baudio_format audio_codec=baudio_format_mpeg;

BDBG_MODULE(kwt_playback);


NEXUS_PlaybackSettings GetPlayBackSettings(void);

void SetPlaybackSettings(NEXUS_PlaybackSettings playbackSettings);


eRet StopPlayback(void);

eRet PausePlayback(void);

eRet PlayPlayback(void);



NEXUS_PlaypumpHandle            m_playpump=NULL;
NEXUS_PlaybackHandle            m_playback=NULL;

static int trickModeRate=0;
static NEXUS_FilePlayHandle file=0;
static NEXUS_PidChannelHandle videoPidChannel;
static NEXUS_PidChannelHandle audioPidChannel;
static NEXUS_PidChannelHandle pcrPidChannel;


/*

video_file=/mnt/media/usb/RecFile/1970-01-01/index/../00-38-20.ts
/mnt/media/usb/RecFile/2015-06-23/00-01-02.ts
/mnt/media/sdcard/RecFile/2015-06-23/00-01-02.ts


/mnt/media/usb/RecFile/2015-06-23/index/00-01-02.nfo
/mnt/media/sdcard/RecFile/2015-06-23/index/00-01-02.nfo



*/

int CheckIndexExistAndSize(const char *video_file,char *index_file)
{
	char index_name[100]={0};
	int ret = 0;
	memset(index_name,0,100);
	printf("video_file=%s\n",video_file);
	strncpy(index_name,video_file,strlen(video_file)-12);
	sprintf(index_name,"%s/index",index_name);
	strncat(index_name,video_file+strlen(video_file)-12,9);
    sprintf(index_name,"%s.nfo",index_name);
	if(access(index_name,F_OK)==0)
	{
		struct stat buf;
		ret = stat (index_name, &buf);
		if (ret == 0)
		{
			if(buf.st_size>0)
			{
				strcpy(index_file,index_name);
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}




}



eRet ParseTsInfo(const char * mediafile)
{
    bmedia_probe_config         probe_config;
    const bmedia_probe_stream * stream;
    const bmedia_probe_track  * track;
    bfile_io_read_t             fd                      = NULL;
    bmedia_probe_t              probe                   = NULL;
    eRet                        retVal                  = eRet_Ok;

    FILE *fin = fopen64(mediafile, "rb");
    if (!fin)
    {
        BDBG_ERR(("can't open media file '%s'",mediafile ));
        return eRet_ExternalError;
    }

    probe = bmedia_probe_create();
    BDBG_ASSERT(probe);

    /* FUNCTION BELOW is from SETTOP API */
    fd = bfile_stdio_read_attach(fin);

    bmedia_probe_default_cfg(&probe_config);
    probe_config.file_name = mediafile;
    probe_config.type      = bstream_mpeg_type_unknown;
    stream = bmedia_probe_parse(probe, fd, &probe_config);

    /* now stream is either NULL, or stream descriptor with linked list of audio/video tracks */
    bfile_stdio_read_detach(fd);
    fclose(fin);

    if (NULL == stream)
    {
        BDBG_WRN(("can't parse stream '%s' ", mediafile));
        retVal = eRet_ExternalError;
        goto error;
    }

    /* media file probed */

	printf("stream->type=%d\n",stream->type);

    /* populate other structure with first audio and video tracks */
    {
		 video_pid=0x1011;
		 audio_pid=0x1100;
		 video_codec=bvideo_codec_h264;
		 audio_codec=baudio_format_mpeg;
        for(track=BLST_SQ_FIRST(&stream->tracks);track;track=BLST_SQ_NEXT(track, link))
        {
			printf("track->type=%d\n",track->type);
			printf("track->number=%d\n",track->number);
			printf("track->program=%d\n",track->program);

            switch(track->type)
            {
            case bmedia_track_type_audio:
                if(track->info.audio.codec != baudio_format_unknown /*&& audio_program < 1*/)
                {
					if(track->info.audio.bitrate != 0)
					{
    					printf("track->info.audio.codec=%d\n",track->info.audio.codec);
    					printf("track->info.audio.bitrate=%d\n",track->info.audio.bitrate);

    					audio_pid=track->number;
    					audio_codec=track->info.audio.codec;
					}
					else
					{

					}
                }
                break;
            case bmedia_track_type_video:
                if(track->info.video.codec != bvideo_codec_unknown)
                {
					printf("track->info.video.codec=%d\n",track->info.video.codec);
					video_pid=track->number;
					video_codec=track->info.video.codec;

                }
                break;
            default:
                break;
            }
        }


    }

error:
    if (NULL != stream)
    {
        bmedia_probe_stream_free(probe, stream);
    }

    if (NULL != probe)
    {
        bmedia_probe_destroy(probe);
    }

    return retVal;

}













void StartPlayBack(const char *file_name)
{
	NEXUS_PlaybackSettings playbackSettings;
	char index_name[100]={0};

	StopLiveChannel();

	ParseTsInfo(file_name);



	{
        playbackSettings = GetPlayBackSettings();
        playbackSettings.simpleStcChannel = g_System.SimpleStcChannel;
        playbackSettings.stcTrick = true;
        playbackSettings.playpumpSettings.transportType = NEXUS_TransportType_eTs;
        SetPlaybackSettings(playbackSettings);
    }

	{
		NEXUS_PlaybackPidChannelSettings playbackPidSettings_v;
		NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings_v);
		playbackPidSettings_v.pidSettings.pidType = NEXUS_PidType_eVideo;
        playbackPidSettings_v.pidTypeSettings.video.codec = NEXUS_VideoCodec_eH264; /* must be told codec for correct handling */
        //playbackPidSettings.pidTypeSettings.video.index = true;
        playbackPidSettings_v.pidTypeSettings.video.simpleDecoder = g_System.SimpleVideoDecoder;
        videoPidChannel = NEXUS_Playback_OpenPidChannel(m_playback, video_pid, &playbackPidSettings_v);
	}

	{
		NEXUS_PlaybackPidChannelSettings playbackPidSettings_a;
		NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings_a);
		playbackPidSettings_a.pidSettings.pidType = NEXUS_PidType_eAudio;
        //playbackPidSettings_a.pidTypeSettings.audio.codec = NEXUS_VideoCodec_eH264; /* must be told codec for correct handling */
        //playbackPidSettings.pidTypeSettings.video.index = true;
        playbackPidSettings_a.pidTypeSettings.audio.simpleDecoder = g_System.SimpleAudioDecoder;
        audioPidChannel = NEXUS_Playback_OpenPidChannel(m_playback, audio_pid, &playbackPidSettings_a);
	}
/*
	{
		NEXUS_PlaybackPidChannelSettings playbackPidSettings_p;
		NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings_p);
        pcrPidChannel = NEXUS_Playback_OpenPidChannel(m_playback, 0x1001, &playbackPidSettings_p);
	}
	*/


	if(CheckIndexExistAndSize(file_name,index_name)==1)
	{
		printf("index_name=%s\n",index_name);
        file = NEXUS_FilePlay_OpenPosix(file_name, index_name);
	}
	else
	{
		file = NEXUS_FilePlay_OpenPosix(file_name, NULL);

	}



    if (!file)
    {
        BDBG_ERR(("can't open files: '%s'",file_name));
		return ;
    }
	ConfigureStc(eStcType_PvrPlayback, NULL);



        	SetSimpleVideoDecoderStc();

	StartVideoDecode(videoPidChannel);

	if(audio_codec==baudio_format_mpeg)
	{
		StartSimpleAudioDecode(NEXUS_AudioCodec_eMpeg, audioPidChannel);
	}
	else if(audio_codec==baudio_format_aac)
	{
		StartSimpleAudioDecode(NEXUS_AudioCodec_eAac, audioPidChannel);
	}

/*
	    NEXUS_PlaybackStartSettings defaultStartSettings;
        NEXUS_Playback_GetDefaultStartSettings(&defaultStartSettings);
		defaultStartSettings.mode=NEXUS_PlaybackMode_eAutoBitrate;
		defaultStartSettings.mpeg2TsIndexType=NEXUS_PlaybackMpeg2TsIndexType_eSelf;
		defaultStartSettings.bitrate=5000*1000;
*/


    NEXUS_Playback_Start(m_playback, file, NULL);
}



static void endOfStreamCallback(void *context, int param)
{
    BSTD_UNUSED(context);

    if (param)
    {
        BDBG_WRN(("end of stream"));
		MenuPlaybackShow(false);
		MenuFileBrowserShow(true);
		CurrentMenu=eMenu_FileBrowser;
        StopPlayback();
        StartLiveChannel(g_Channel.CurrentChannel);
		g_Status.PlaybackStarted=0;

    }
    else
	{
        BDBG_WRN(("beginning of stream"));
    }
    return;
}

eRet InitializePlayback(void)
{
	NEXUS_PlaybackSettings          _playbackSettings;
	NEXUS_PlaybackTrickModeSettings _pTrickModeSettings;

    BDBG_MSG(("INIT!"));
    m_playpump = NEXUS_Playpump_Open(0, NULL);
    if (!m_playpump)
    {
        BDBG_ERR(("Cannot Open Playpump"));
        return eRet_ExternalError;
    }
    m_playback = NEXUS_Playback_Create();
    if (!m_playback)
    {
        BDBG_ERR(("Cannot Open Playback"));
        NEXUS_Playpump_Close(m_playpump);
        return eRet_ExternalError;
    }

    /* Initialize Playback Settings */
    NEXUS_Playback_GetDefaultSettings(&_playbackSettings);
    _playbackSettings.playpump = m_playpump;
    _playbackSettings.beginningOfStreamAction = NEXUS_PlaybackLoopMode_ePlay; /* Normal Play at the beginning */
    _playbackSettings.beginningOfStreamCallback.context = NULL;
    _playbackSettings.beginningOfStreamCallback.callback = endOfStreamCallback;
    _playbackSettings.beginningOfStreamCallback.param = 0;
    _playbackSettings.endOfStreamAction = NEXUS_PlaybackLoopMode_eLoop; /* Do Loop Mode for Now*/
    _playbackSettings.endOfStreamCallback.callback = endOfStreamCallback;
    _playbackSettings.endOfStreamCallback.context = NULL;
    _playbackSettings.endOfStreamCallback.param = 1;
    _playbackSettings.enableStreamProcessing = false;

	_playbackSettings.stcTrick=false;
    if (NEXUS_Playback_SetSettings(m_playback, &_playbackSettings)!= NEXUS_SUCCESS)
    {
        BDBG_ERR(("Cannot SetSettings"));
        return eRet_ExternalError;
    }

    NEXUS_Playback_GetDefaultTrickModeSettings(&_pTrickModeSettings);
    return eRet_Ok;
}

eRet UninitializePlayback(void)
{
    if (NULL != m_playback)
    {
        NEXUS_Playback_Destroy(m_playback);
        m_playback = NULL;
    }

    if (NULL != m_playpump)
    {
        NEXUS_Playpump_Close(m_playpump);
        m_playpump = NULL;
    }

    return eRet_Ok;
}



/* Playbck from Video Object */
eRet PlayPlaySeek(int add)
{
	NEXUS_Error ret = NEXUS_SUCCESS;
	NEXUS_PlaybackStatus status;
	NEXUS_PlaybackPosition position;

	if(m_playback == NULL)
	{
		return eRet_ExternalError;
	}
	ret=NEXUS_Playback_GetStatus(m_playback,  &status);
	if(ret == NEXUS_SUCCESS)
	{
		if(add == 1)
		{
		    if(status.position<status.last-5000)
    		{
    		   position=status.position+5000;

    		   PausePlayback();
    		   ret = NEXUS_Playback_Seek(m_playback,position);
    		   if (ret != NEXUS_SUCCESS)
    		   	{
    				BDBG_ERR(("Cannot Continue Playback"));
    				PlayPlayback();
    				return eRet_ExternalError;
    			}
    		   PlayPlayback();
    		}
		}
		else if(add==0)
		{
			if(status.position>status.first+5000)
    		{
    		   position=status.position-5000;

    		   PausePlayback();
    		   ret = NEXUS_Playback_Seek(m_playback,position);
    		   if (ret != NEXUS_SUCCESS)
    		   {
    				BDBG_ERR(("Cannot Continue Playback"));
    				PlayPlayback();
    				return eRet_ExternalError;
    		   }
    		   PlayPlayback();
    		}


		}
	}

   return eRet_Ok;
}


/* Playbck from Video Object */
eRet PlayPlayback(void)
{
   NEXUS_Error ret = NEXUS_SUCCESS;


   ret = NEXUS_Playback_Play(m_playback);
   if (ret != NEXUS_SUCCESS)
   {
      BDBG_ERR(("Cannot Continue Playback"));
      return eRet_ExternalError;
   }

   return eRet_Ok;
}



eRet StopPlayback(void)
{
    eRet ret = eRet_Ok;
	NEXUS_PlaybackSettings PlaybackSettings;
	NEXUS_PlaybackTrickModeSettings TrickModeSettings;


	   	StopSimpleVideoDecode();
	StopSimpleAudioDecode();

    /* Close all Pids associate with this plyaback and close the PID channels */
    NEXUS_Playback_Stop(m_playback);



    {



    NEXUS_FilePlay_Close(file);
	NEXUS_Playback_ClosePidChannel(m_playback,videoPidChannel);
    videoPidChannel = NULL;
	NEXUS_Playback_ClosePidChannel(m_playback,audioPidChannel);
    audioPidChannel = NULL;
    //NEXUS_Playback_ClosePidChannel(m_playback,pcrPidChannel);
    //pcrPidChannel = NULL;
	}

    /* Reset structure ready to use again */
        /* Initialize Playback Settings */
    NEXUS_Playback_GetDefaultSettings(&PlaybackSettings);
    PlaybackSettings.playpump = m_playpump;
    PlaybackSettings.beginningOfStreamAction = NEXUS_PlaybackLoopMode_ePlay; /* Normal Play at the beginning */
    PlaybackSettings.beginningOfStreamCallback.callback = endOfStreamCallback;
    PlaybackSettings.beginningOfStreamCallback.param = 0;
    PlaybackSettings.endOfStreamAction = NEXUS_PlaybackLoopMode_eLoop; /* Do Loop Mode for Now*/
    PlaybackSettings.endOfStreamCallback.callback = endOfStreamCallback;
    PlaybackSettings.endOfStreamCallback.param = 1;
    PlaybackSettings.enableStreamProcessing = false;
    PlaybackSettings.playpumpSettings.timestamp.pacing = false;
    if (NEXUS_Playback_SetSettings(m_playback, &PlaybackSettings)!= NEXUS_SUCCESS)
    {
        BDBG_ERR(("Cannot SetSettings"));
        return eRet_ExternalError;
    }

    BKNI_Memset(&PlaybackSettings,0,sizeof(PlaybackSettings));
    NEXUS_Playback_GetDefaultTrickModeSettings(&TrickModeSettings);
    trickModeRate =1;

    return ret;
}


eRet PausePlayback(void)
{
    eRet ret = eRet_Ok;

    NEXUS_Playback_Pause(m_playback);

    return ret;
}

eRet TrickPlaybackMode(signed int  speed)
{
    eRet ret = eRet_Ok;
	NEXUS_PlaybackTrickModeSettings TrickModeSettings;



    NEXUS_Playback_GetDefaultTrickmodeSettings(&TrickModeSettings);
    TrickModeSettings.rate = NEXUS_NORMAL_DECODE_RATE * speed;
    NEXUS_Playback_TrickMode(m_playback,&TrickModeSettings);

   return ret;

}


NEXUS_PlaybackSettings GetPlayBackSettings(void)
{
    NEXUS_PlaybackSettings playbackSettings;
    NEXUS_Playback_GetSettings(m_playback, &playbackSettings);
    return playbackSettings;
}

void SetPlaybackSettings(NEXUS_PlaybackSettings playbackSettings)
{
    NEXUS_Playback_Stop(m_playback);

    if (NEXUS_Playback_SetSettings(m_playback, &playbackSettings)!= NEXUS_SUCCESS)
    {
        BDBG_ERR(("Cannot Playback SetSettings"));
    }
}

void SetPlayPumpSettings(NEXUS_PlaypumpSettings playpumpSettings)
{
    NEXUS_Playback_Stop(m_playback);

    if (NEXUS_Playpump_SetSettings(m_playpump, &playpumpSettings)!= NEXUS_SUCCESS)
    {
        BDBG_ERR(("Cannot SetSettings in Playpump"));
    }
}


void DumpPlaybackStatus(void)
{
	NEXUS_Error  ret=NEXUS_SUCCESS;
	NEXUS_PlaybackStatus status;
	if(m_playback == NULL)
	{
		return;
	}
	ret=NEXUS_Playback_GetStatus(m_playback,  &status);
	if(ret == NEXUS_SUCCESS)
	{
		printf("position    =%ld\n",status.position);
		printf("first       =%ld\n",status.first);
		printf("last        =%ld\n",status.last);
		printf("readPosition=%ld\n",status.readPosition);
	}
	return;

}

void GetPlaybackStatus(unsigned int *cur,unsigned int *total)
{
    NEXUS_Error  ret=NEXUS_SUCCESS;
	NEXUS_PlaybackStatus status;
	if(m_playback == NULL)
	{
		return;
	}
	ret=NEXUS_Playback_GetStatus(m_playback,  &status);
	if(ret == NEXUS_SUCCESS)
	{
		*cur=(unsigned int)(status.position/1000);
		*total=(unsigned int)((status.last-status.first)/1000);
	}
	return;

}



