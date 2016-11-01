
#include <stdio.h>

#include "nexus_audio_decoder.h"
#include "nexus_audio_input.h"
#include "nexus_simple_audio_decoder_server.h"

#include "main.h"
#include "output.h"
#include "global.h"




BDBG_MODULE(kwt_audio_decode);



typedef enum eHdmiAudioInput
{
    eHdmiAudioType_Pcm,
    eHdmiAudioType_Compressed,
    eHdmiAudioType_Multichannel,
    eHdmiAudioType_EncodeDts,
    eHdmiAudioType_EncodeAc3,
    eHdmiAudioType_None,
    eHdmiAudioType_Max
} eHdmiAudioType;

typedef enum eAudioDownmix
{
    eAudioDownmix_None,
    eAudioDownmix_Left,
    eAudioDownmix_Right,
    eAudioDownmix_Monomix,
    eAudioDownmix_Matrix,   /* aac only */
    eAudioDownmix_Arib,     /* aac only */
    eAudioDownmix_LtRt,     /* aac only */
    eAudioDownmix_Max
} eAudioDownmix;

typedef enum eAudioDualMono
{
    eAudioDualMono_Left,
    eAudioDualMono_Right,
    eAudioDualMono_Stereo,
    eAudioDualMono_Monomix,
    eAudioDualMono_Max
} eAudioDualMono;


typedef enum eDolbyDRC
{
    eDolbyDRC_None,
    eDolbyDRC_Light,
    eDolbyDRC_Medium,
    eDolbyDRC_Heavy,
    eDolbyDRC_Max
} eDolbyDRC;



eRet SetHdmiInputDecoderType(eHdmiAudioInput hdmiInput,NEXUS_SimpleAudioDecoderServerSettings * pSettings);
eRet SetSimpleAudioDecodeStc(int flag);
NEXUS_AudioInput GetSimpleAudioDecodeConnector(uint8_t num, NEXUS_AudioConnectorType type);

eRet SetSimpleAudioDecodeVolume(uint32_t level);





static NEXUS_AudioDecoderHandle    m_AuidoDecoder[2];






static bool IsAudioSourceChanged=false;





/* bwin io callback that is triggered when it is safe to handle callbacks */
static void bwinAudioDecodeCallback(bwidget_engine_t engine, bwin_io_handle handle)
{

    BSTD_UNUSED(engine);
    BSTD_UNUSED(handle);
    if (IsAudioSourceChanged)
    {
		printf("Audio Source Changed\n");
    }
    return;
}

static void NexusAudioDecodeSourceChangedCallback(void * context, int param)
{
    BSTD_UNUSED(context);
    BSTD_UNUSED(param);
    IsAudioSourceChanged=true;

    /* sync with bwin loop */
    bwidget_trigger_io(g_System.WidgetEngine, (bwin_io_handle)AUDIO_DECODE_HANDLE_0);
}

/*
CAudioDecode::~CAudioDecode()
{
	printf("---~~CAudioDecode\n");
    if (_sourceChangedCallbackHandle)
    {
        B_SchedulerCallback_Destroy(_sourceChangedCallbackHandle);
        _sourceChangedCallbackHandle = NULL;
    }
}
*/
eRet OpenAudioDecode(int _number)
{
    eRet                           ret       = eRet_Ok;
    NEXUS_AudioDecoderOpenSettings settings;

	if(_number==0)
	{
		bwidget_add_io_handle(g_System.WidgetEngine, (bwin_io_handle)AUDIO_DECODE_HANDLE_0, bwinAudioDecodeCallback);
	}
	else if(_number==1)
	{
		bwidget_add_io_handle(g_System.WidgetEngine, (bwin_io_handle)AUDIO_DECODE_HANDLE_1, bwinAudioDecodeCallback);

	}



    NEXUS_AudioDecoder_GetDefaultOpenSettings(&settings);

    {
        int multiChFormat = 1;
        if ((0 < multiChFormat) &&  (NEXUS_AudioMultichannelFormat_eMax > multiChFormat))
        {
            settings.multichannelFormat = (NEXUS_AudioMultichannelFormat)multiChFormat;
        }
    }


    m_AuidoDecoder[_number] = NEXUS_AudioDecoder_Open(_number, &settings);
    CHECK_PTR_GOTO("Nexus audio decoder open failed!", m_AuidoDecoder[_number], ret, eRet_NotAvailable, error);

    /* set audio decoder settings */
    {
        NEXUS_Error                 nError                = NEXUS_SUCCESS;
        NEXUS_AudioDecoderSettings  audioDecoderSettings;


        NEXUS_AudioDecoder_GetSettings(m_AuidoDecoder[_number], &audioDecoderSettings);
        audioDecoderSettings.sourceChanged.callback = NexusAudioDecodeSourceChangedCallback;
        audioDecoderSettings.sourceChanged.context  = NULL;
        audioDecoderSettings.sourceChanged.param    = 0;
        nError = NEXUS_AudioDecoder_SetSettings(m_AuidoDecoder[_number], &audioDecoderSettings);
        CHECK_NEXUS_ERROR_GOTO("Error applying audio decoder settings.", ret, nError, error);
    }

    /* dtt - open passthru? see bsettop_decode.c */
error:
    return ret;
}



void CloseAudioDecode(int _number)
{

    if (m_AuidoDecoder[_number])
    {
        NEXUS_AudioInput_Shutdown(NEXUS_AudioDecoder_GetConnector(m_AuidoDecoder[_number], NEXUS_AudioDecoderConnectorType_eStereo));
        NEXUS_AudioInput_Shutdown(NEXUS_AudioDecoder_GetConnector(m_AuidoDecoder[_number], NEXUS_AudioDecoderConnectorType_eCompressed));
        NEXUS_AudioInput_Shutdown(NEXUS_AudioDecoder_GetConnector(m_AuidoDecoder[_number], NEXUS_AudioDecoderConnectorType_eMultichannel));

        NEXUS_AudioDecoder_Close(m_AuidoDecoder[_number]);
    }

    if (NULL != g_System.WidgetEngine)
    {
		if(_number==0)
		{
			bwidget_remove_io_handle(g_System.WidgetEngine, (bwin_io_handle)AUDIO_DECODE_HANDLE_0);
		}
		else if(_number==1)
		{
			bwidget_remove_io_handle(g_System.WidgetEngine, (bwin_io_handle)AUDIO_DECODE_HANDLE_1);
		}

    }
}

NEXUS_AudioInput GetAudioDecodeConnector(int _number,NEXUS_AudioConnectorType type)
{
    NEXUS_AudioInput nexusInput = NULL;

    BDBG_ASSERT(NEXUS_AudioConnectorType_eMax > type);

    if (NULL != m_AuidoDecoder[_number])
    {
        nexusInput = NEXUS_AudioDecoder_GetConnector(m_AuidoDecoder[_number], type);
    }

    return nexusInput;
}

eRet GetAudioDecodeStatus(int _number,NEXUS_AudioDecoderStatus * pStatus)
{
    eRet        ret    = eRet_Ok;
    NEXUS_Error nerror = NEXUS_SUCCESS;
    BDBG_ASSERT(NULL != pStatus);

    nerror = NEXUS_AudioDecoder_GetStatus(m_AuidoDecoder[_number], pStatus);
    CHECK_NEXUS_ERROR_GOTO("error getting audio decoder status", ret, nerror, error);

error:
    return ret;
}








/*** simple audio decoder class */



eRet OpenSimpleAudioDecode(void)
{
    eRet                                   ret       = eRet_Ok;
    NEXUS_SimpleAudioDecoderServerSettings settings;


    bwidget_add_io_handle(g_System.WidgetEngine, (bwin_io_handle)SIMPLE_AUDIO_DECODE_HANDLE, bwinAudioDecodeCallback);



    OpenAudioDecode(0);
    OpenAudioDecode(1);
    NEXUS_SimpleAudioDecoder_GetDefaultServerSettings(&settings);
    settings.primary   = m_AuidoDecoder[0];
    settings.secondary = m_AuidoDecoder[1];


    {
        /* default hdmi to stereo output */
        SetHdmiInputDecoderType(eHdmiAudioType_Pcm, &settings);

        /* assign Hdmi outputs for simple audio decoder to use */
        settings.hdmi.outputs[0] = g_System.HdmiHandle;
    }

    g_System.SimpleAudioDecoder = NEXUS_SimpleAudioDecoder_Create(0, &settings);
    CHECK_PTR_GOTO("unable to create a simple audio decoder", g_System.SimpleAudioDecoder, ret, eRet_OutOfMemory, error);

    ret = SetSimpleAudioDecodeStc(1);
    CHECK_ERROR_GOTO("SetStcChannel simple audio decoder failed!", ret, error);

    /* if an audio dac resource has been assigned, set of audio output */
    if (NULL != g_System.AudioDacHandle)
    {
		ret = ConnectAudioDacOutput(GetSimpleAudioDecodeConnector(0, NEXUS_AudioConnectorType_eStereo));
        CHECK_ERROR_GOTO("unable to connect dac output", ret, error);
    }

    /* set audio decoder settings */
    {
        NEXUS_Error                       nError                = NEXUS_SUCCESS;
        NEXUS_SimpleAudioDecoderSettings  audioDecoderSettings;

        NEXUS_SimpleAudioDecoder_GetSettings(g_System.SimpleAudioDecoder, &audioDecoderSettings);
        audioDecoderSettings.primary.sourceChanged.callback = NexusAudioDecodeSourceChangedCallback;
        audioDecoderSettings.primary.sourceChanged.context  = NULL;
        audioDecoderSettings.primary.sourceChanged.param    = 0;
        audioDecoderSettings.secondary.sourceChanged.callback = NexusAudioDecodeSourceChangedCallback;
        audioDecoderSettings.secondary.sourceChanged.context  = NULL;
        audioDecoderSettings.secondary.sourceChanged.param    = 0;
        nError = NEXUS_SimpleAudioDecoder_SetSettings(g_System.SimpleAudioDecoder, &audioDecoderSettings);
        CHECK_NEXUS_ERROR_GOTO("Error applying simple audio decoder settings.", ret, nError, error);
    }
error:
    return ret;
}

void GetSimpleAudioDecodeSettings(NEXUS_SimpleAudioDecoderServerSettings * pSettings)
{
    BDBG_ASSERT(NULL != g_System.SimpleAudioDecoder);

    NEXUS_SimpleAudioDecoder_GetServerSettings(g_System.SimpleAudioDecoder, pSettings);
}

eRet SetSimpleAudioDecodeSettings(NEXUS_SimpleAudioDecoderServerSettings * pSettings)
{
    NEXUS_Error nerror = NEXUS_SUCCESS;
    eRet        ret    = eRet_Ok;
    BDBG_ASSERT(NULL != g_System.SimpleAudioDecoder);

    nerror = NEXUS_SimpleAudioDecoder_SetServerSettings(g_System.SimpleAudioDecoder, pSettings);
    CHECK_NEXUS_ERROR_GOTO("unable to set simple audio decoder server settings", ret, nerror, error);

error:
    return ret;
}

/* if pSettings == NULL, we will get the settings from nexus and set them after making changes.
   if pSettings != NULL, we will fill in the given pSettings - changes will not be applies in nexus */
eRet SetHdmiInputDecoderType(eHdmiAudioInput hdmiInput,NEXUS_SimpleAudioDecoderServerSettings * pSettings)
{
    NEXUS_SimpleAudioDecoderServerSettings   settings;
    NEXUS_SimpleAudioDecoderServerSettings * pUseSettings   = NULL;
    NEXUS_AudioConnectorType                 hdmiType;
    int                                      i              = 0;
    eRet                                     ret            = eRet_Ok;
    int                                      decoder        = 0;


    BDBG_MSG(("setHdmiInputDecoderType:%d", hdmiInput));

    if (NULL == pSettings)
    {
        /* use settings from nexus */
        GetSimpleAudioDecodeSettings(&settings);
        pUseSettings = &settings;
    }
    else
    {
        /* use given setting structure */
        pUseSettings = pSettings;
    }



    switch (hdmiInput)
    {
    case eHdmiAudioType_Multichannel:
        hdmiType = NEXUS_AudioConnectorType_eMultichannel;
        break;
    case eHdmiAudioType_EncodeDts:
    case eHdmiAudioType_EncodeAc3:
        BDBG_WRN(("encode AC-3/DTS feature currently unsupported - default to compressed"));
        hdmiInput = eHdmiAudioType_Compressed;
        /* fallthru for now */
    case eHdmiAudioType_Compressed:
        hdmiType = NEXUS_AudioConnectorType_eCompressed;
        decoder=1;
        break;
    case eHdmiAudioType_Pcm:
    default:
        hdmiType = NEXUS_AudioConnectorType_eStereo;
        decoder =0;
        break;
    }

    for (i = 0; i < NEXUS_AudioCodec_eMax; i++)
    {
        switch (i)
        {
        case NEXUS_AudioCodec_eAac:
        case NEXUS_AudioCodec_eAacLoas:
        case NEXUS_AudioCodec_eAacPlus:
        case NEXUS_AudioCodec_eAacPlusAdts:
        case NEXUS_AudioCodec_eAc3:
        case NEXUS_AudioCodec_eDts:
        case NEXUS_AudioCodec_eLpcmDvd:
        case NEXUS_AudioCodec_eLpcmHdDvd:
        case NEXUS_AudioCodec_eLpcmBluRay:
        case NEXUS_AudioCodec_eDtsHd:
        case NEXUS_AudioCodec_eWmaStd:
        case NEXUS_AudioCodec_eWmaStdTs:
        case NEXUS_AudioCodec_eWmaPro:
        case NEXUS_AudioCodec_eDtsLegacy:
           /* TODO fix AC3 Plus case if Edid does not support */
        case NEXUS_AudioCodec_eAc3Plus:
        case NEXUS_AudioCodec_eMlp:
            pUseSettings->hdmi.input[i] = GetAudioDecodeConnector(decoder,hdmiType);
            break;
        case NEXUS_AudioCodec_eUnknown:
        default:
            pUseSettings->hdmi.input[i] = GetAudioDecodeConnector(0,NEXUS_AudioConnectorType_eStereo);
            break;
        }
    }

    if (NULL == pSettings)
    {
        /* no settings structure given so set in nexus */
        ret = SetSimpleAudioDecodeSettings(&settings);
        CHECK_ERROR_GOTO("set simple audio settings failed", ret, error);

       // _hdmiInput = hdmiInput;
    }

error:
    return ret;
}


void  CloseSimpleAudioDecode(void)
{

    if (NULL != g_System.SimpleAudioDecoder)
    {
        NEXUS_SimpleAudioDecoderServerSettings settings;

        /* Disconnect the STC */
        SetSimpleAudioDecodeStc(0);

        NEXUS_SimpleAudioDecoder_GetDefaultServerSettings(&settings);
        settings.enabled = false;
        NEXUS_SimpleAudioDecoder_SetServerSettings(g_System.SimpleAudioDecoder, &settings);

        NEXUS_SimpleAudioDecoder_Destroy(g_System.SimpleAudioDecoder);
        g_System.SimpleAudioDecoder = NULL;
    }



    for (int i = 0; i < 2; i++)
    {

        if (NULL != m_AuidoDecoder[i])
        {
            CloseAudioDecode(i);
            m_AuidoDecoder[i] = NULL;
        }
    }

    if (NULL != g_System.WidgetEngine)
    {
        bwidget_remove_io_handle(g_System.WidgetEngine, (bwin_io_handle)SIMPLE_AUDIO_DECODE_HANDLE);
    }

    return ;
}

eRet StartSimpleAudioDecode(NEXUS_AudioCodec audio_codec,NEXUS_PidChannelHandle audio_pid_channel)
{
    eRet                                  ret       = eRet_Ok;
    NEXUS_Error                           nerror    = NEXUS_SUCCESS;
    NEXUS_SimpleAudioDecoderStartSettings settings;



    ret = SetSimpleAudioDecodeStc(1);
    CHECK_ERROR_GOTO("SetStcChannel simple audio decoder failed!", ret, error);

    NEXUS_SimpleAudioDecoder_GetDefaultStartSettings(&settings);
    settings.primary.codec        = audio_codec;
    settings.primary.pidChannel   = audio_pid_channel;
    nerror = NEXUS_SimpleAudioDecoder_SetStcChannel(g_System.SimpleAudioDecoder, g_System.SimpleStcChannel);
    CHECK_NEXUS_ERROR_GOTO("starting simple audio decoder failed!", ret, nerror, error);

    /* set downmix */
    /* start optional audio capture if necessary */
    /* codec object call to enable 96khz mode (only applies to aac) */

    nerror = NEXUS_SimpleAudioDecoder_Start(g_System.SimpleAudioDecoder, &settings);
    CHECK_NEXUS_ERROR_GOTO("simple audio decoder failed to start", ret, nerror, error);

    /* save pid - also propogate to simple decoder's internal decoder objects */
    //setPid(pPid);
   // _started = true;

    //notifyObservers(eNotify_AudioDecodeStarted, this);

	//SetSimpleAudioDecodeVolume(NEXUS_AUDIO_VOLUME_LINEAR_NORMAL);
error:
    return ret;
}
void  StopSimpleAudioDecode(void)
{


    BDBG_ASSERT(NULL != g_System.SimpleAudioDecoder);
    NEXUS_SimpleAudioDecoder_Stop(g_System.SimpleAudioDecoder);

    SetSimpleAudioDecodeStc(0);

    /* return pid */
   // pPid     = getPid();
   //_started = false;

    //notifyObservers(eNotify_AudioDecodeStopped, this);
    return ;
}

/*
void CSimpleAudioDecode::setPid(CPid * pPid)
{
    _pPid = pPid;

   //propogate to child decoders
    if (NULL != _pDecoders[0])
    {
        _pDecoders[0]->setPid(pPid);
    }
    if (NULL != _pDecoders[1])
    {
        _pDecoders[1]->setPid(pPid);
    }
}
*/
NEXUS_AudioInput GetSimpleAudioDecodeConnector(uint8_t num, NEXUS_AudioConnectorType type)
{
    NEXUS_AudioInput nexusInput = NULL;

    BDBG_ASSERT(2 > num);
    BDBG_ASSERT(NEXUS_AudioConnectorType_eMax > type);

    if (NULL != m_AuidoDecoder[num])
    {
        nexusInput = GetAudioDecodeConnector(num,type);
    }

    return nexusInput;
}



eRet GetSimpleAudioDecodeStatus(NEXUS_AudioDecoderStatus * pStatus)
{
    eRet        ret    = eRet_Ok;
    NEXUS_Error nerror = NEXUS_SUCCESS;
    BDBG_ASSERT(NULL != pStatus);

    nerror = NEXUS_SimpleAudioDecoder_GetStatus(g_System.SimpleAudioDecoder, pStatus);
    CHECK_NEXUS_ERROR_GOTO("error getting simple audio decoder status", ret, nerror, error);

error:
    return ret;
}



unsigned int GetSimpleAudioDecodeFrame(void)
{
    eRet        ret    = eRet_Ok;
    NEXUS_Error nerror = NEXUS_SUCCESS;
	NEXUS_AudioDecoderStatus status;

    nerror = NEXUS_SimpleAudioDecoder_GetStatus(g_System.SimpleAudioDecoder, &status);
	return status.framesDecoded;

}








uint32_t GetSimpleAudioDecodeVolume(void)
{
    NEXUS_SimpleAudioDecoderSettings         decoderSettings;
    NEXUS_SimpleAudioDecoderHandle           decoder                 = g_System.SimpleAudioDecoder;
    uint32_t                                 volume                  = NEXUS_AUDIO_VOLUME_LINEAR_NORMAL;

    NEXUS_SimpleAudioDecoder_GetSettings(decoder, &decoderSettings);

    volume =
        (decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft] +
         decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight]) / 2;

    return volume;
}


eRet SetSimpleAudioDecodeVolume(uint32_t level)
{
    NEXUS_SimpleAudioDecoderSettings         decoderSettings;
    NEXUS_SimpleAudioDecoderHandle           decoder                 = g_System.SimpleAudioDecoder;
    eRet                                     ret                     = eRet_Ok;
    NEXUS_Error                              nerror                  = NEXUS_SUCCESS;

    NEXUS_SimpleAudioDecoder_GetSettings(decoder, &decoderSettings);

    decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft]     = level;
    decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight]   = level;
    decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eRight]    = 0;
    decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eLeft]    = 0;

    decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft]   = level;
    decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight] = level;
    decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eRight]  = 0;
    decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eLeft]  = 0;

    nerror = NEXUS_SimpleAudioDecoder_SetSettings(decoder, &decoderSettings);
    CHECK_NEXUS_ERROR_GOTO("unable to set audio volume", ret, nerror, error);
    BDBG_MSG(("Setting volume level:%d", level));

	/*

    {
        CAudioVolume volume;

        volume._left       = level;
        volume._right      = level;
        volume._muted      = decoderSettings.primary.muted;
        volume._volumeType = NEXUS_AudioVolumeType_eLinear;
        notifyObservers(eNotify_VolumeChanged, &volume);
    }
    */

error:
    return ret;
}

bool GetSimpleAudioDecodeMute(void)
{
    NEXUS_SimpleAudioDecoderSettings         decoderSettings;
    NEXUS_SimpleAudioDecoderHandle           decoder                 = g_System.SimpleAudioDecoder;

    NEXUS_SimpleAudioDecoder_GetSettings(decoder, &decoderSettings);

    return decoderSettings.primary.muted;
}

eRet SetSimpleAudioDecodeMute(bool bMute)
{
    NEXUS_SimpleAudioDecoderSettings         decoderSettings;
    NEXUS_SimpleAudioDecoderHandle           decoder                 = g_System.SimpleAudioDecoder;
    uint32_t                                 level                   = NEXUS_AUDIO_VOLUME_LINEAR_NORMAL;
    eRet                                     ret                     = eRet_Ok;
    NEXUS_Error                              nerror                  = NEXUS_SUCCESS;

    NEXUS_SimpleAudioDecoder_GetSettings(decoder, &decoderSettings);

    decoderSettings.primary.muted   = bMute;
    decoderSettings.secondary.muted = bMute;

    level =
        (decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft] +
         decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight]) / 2;

    nerror = NEXUS_SimpleAudioDecoder_SetSettings(decoder, &decoderSettings);
    CHECK_NEXUS_ERROR_GOTO("unable to set audio mute", ret, nerror, error);
    BDBG_MSG(("Setting mute:%d", bMute));
/*
    {
        CAudioVolume volume;

        volume._left       = level;
        volume._right      = level;
        volume._muted      = bMute;
        volume._volumeType = NEXUS_AudioVolumeType_eLinear;
        notifyObservers(eNotify_MuteChanged, &volume);
    }
*/
error:
    return ret;
}

eRet SetSimpleAudioDecodeDownmix(eAudioDownmix audioDownmix)
{
    NEXUS_SimpleAudioDecoderSettings         decoderSettings;
    NEXUS_SimpleAudioDecoderHandle           decoder                 = g_System.SimpleAudioDecoder;
    NEXUS_AudioDecoderCodecSettings          codecSettings;
    int32_t                                  volumeLeft              = NEXUS_AUDIO_VOLUME_LINEAR_NORMAL;
    int32_t                                  volumeRight             = NEXUS_AUDIO_VOLUME_LINEAR_NORMAL;
    eRet                                     ret                     = eRet_Ok;
    NEXUS_Error                              nerror                  = NEXUS_SUCCESS;

    //BDBG_ASSERT(false == isStarted());

    NEXUS_SimpleAudioDecoder_GetSettings(decoder, &decoderSettings);

    switch (audioDownmix)
    {
    default:
    case eAudioDownmix_None:
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft]     = volumeLeft;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight]   = volumeRight;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eRight]    = 0;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eLeft]    = 0;

        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft]   = volumeLeft;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight] = volumeRight;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eRight]  = 0;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eLeft]  = 0;
        nerror = NEXUS_SimpleAudioDecoder_SetSettings(decoder, &decoderSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set audio downmix setting", ret, nerror, error);
        BDBG_MSG(("Setting standard downmix mode"));
        break;

    case eAudioDownmix_Left:
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft]     = volumeLeft;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight]   = 0;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eRight]    = 0;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eLeft]    = volumeRight;

        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft]   = volumeLeft;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight] = 0;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eRight]  = 0;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eLeft]  = volumeRight;
        nerror = NEXUS_SimpleAudioDecoder_SetSettings(decoder, &decoderSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set audio downmix setting", ret, nerror, error);
        BDBG_MSG(("Setting downmix left"));
        break;

    case eAudioDownmix_Right:
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft]     = 0;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight]   = volumeRight;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eRight]    = volumeLeft;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eLeft]    = 0;

        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft]   = 0;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight] = volumeRight;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eRight]  = volumeLeft;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eLeft]  = 0;
        nerror = NEXUS_SimpleAudioDecoder_SetSettings(decoder, &decoderSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set audio downmix setting", ret, nerror, error);
        BDBG_MSG(("Setting downmix right"));
        break;

    case eAudioDownmix_Monomix:
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft]     = volumeLeft/2;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight]   = volumeRight/2;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eRight]    = volumeLeft/2;
        decoderSettings.primary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eLeft]    = volumeRight/2;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eLeft]   = volumeLeft/2;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eRight] = volumeRight/2;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eLeft][NEXUS_AudioChannel_eRight]  = volumeLeft/2;
        decoderSettings.secondary.volumeMatrix[NEXUS_AudioChannel_eRight][NEXUS_AudioChannel_eLeft]  = volumeRight/2;
        nerror = NEXUS_SimpleAudioDecoder_SetSettings(decoder, &decoderSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set audio downmix setting", ret, nerror, error);

        BDBG_MSG(("Setting downmix monomix"));
        break;

    case eAudioDownmix_Matrix:
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacAdts, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eMatrix;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacLoas, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eMatrix;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacPlusAdts, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eMatrix;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacPlusLoas, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eMatrix;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);

        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAacAdts, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eMatrix;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAacLoas, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eMatrix;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAacPlusAdts, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eMatrix;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAacPlusLoas, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eMatrix;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        BDBG_MSG(("Setting AAC downmix type to (BRCM) matrix"));
        break;

    case eAudioDownmix_Arib:
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacAdts, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eArib;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacLoas, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eArib;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacPlusAdts, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eArib;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacPlusLoas, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eArib;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);

        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAacAdts, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eArib;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAacLoas, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eArib;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAacPlusAdts, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eArib;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAacPlusLoas, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eArib;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);

        BDBG_MSG(("Setting downmix type to ARIB"));
        break;

    case eAudioDownmix_LtRt:
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacAdts, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eLtRt;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacLoas, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eLtRt;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacPlusAdts, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eLtRt;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacPlusLoas, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eLtRt;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);

        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAacAdts, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eLtRt;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAacLoas, &codecSettings);
        codecSettings.codecSettings.aac.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eLtRt;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAacPlusAdts, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eLtRt;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAacPlusLoas, &codecSettings);
        codecSettings.codecSettings.aacPlus.downmixMode = NEXUS_AudioDecoderAacDownmixMode_eLtRt;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);

        BDBG_MSG(("Setting downmix type to ARIB"));
        break;
    }

   // _downmix = audioDownmix;
    //BDBG_MSG(("setting audio downmix:%d", _downmix));

error:
    return ret;
}

eRet SetSimpleAudioDecodeDualMono(eAudioDualMono dualMono)
{
    NEXUS_SimpleAudioDecoderSettings         decoderSettings;
    NEXUS_SimpleAudioDecoderHandle           decoder                 = g_System.SimpleAudioDecoder;
    eRet                                     ret                     = eRet_Ok;
    NEXUS_Error                              nerror                  = NEXUS_SUCCESS;

    //BDBG_ASSERT(false == isStarted());

    NEXUS_SimpleAudioDecoder_GetSettings(decoder, &decoderSettings);

    switch (dualMono)
    {
    case eAudioDualMono_Left:
        decoderSettings.primary.dualMonoMode = NEXUS_AudioDecoderDualMonoMode_eLeft;
        decoderSettings.secondary.dualMonoMode = NEXUS_AudioDecoderDualMonoMode_eLeft;
        break;
    case eAudioDualMono_Right:
        decoderSettings.primary.dualMonoMode = NEXUS_AudioDecoderDualMonoMode_eRight;
        decoderSettings.secondary.dualMonoMode = NEXUS_AudioDecoderDualMonoMode_eRight;
        break;
    default:
    case eAudioDualMono_Stereo:
        decoderSettings.primary.dualMonoMode = NEXUS_AudioDecoderDualMonoMode_eStereo;
        decoderSettings.secondary.dualMonoMode = NEXUS_AudioDecoderDualMonoMode_eStereo;
        break;
    case eAudioDualMono_Monomix:
        decoderSettings.primary.dualMonoMode = NEXUS_AudioDecoderDualMonoMode_eMix;
        decoderSettings.secondary.dualMonoMode = NEXUS_AudioDecoderDualMonoMode_eMix;
        break;
    }

    nerror = NEXUS_SimpleAudioDecoder_SetSettings(decoder, &decoderSettings);
    CHECK_NEXUS_ERROR_GOTO("unable to set audio decoder settings", ret, nerror, error);

    //_dualMono = dualMono;
    //BDBG_MSG(("setting dual mono:%d", _dualMono));

error:
    return ret;
}
/*
eRet SetSimpleAudioDecodeDolbyDRC(eDolbyDRC dolbyDRC)
{
    NEXUS_AudioDecoderCodecSettings          codecSettings;
    NEXUS_SimpleAudioDecoderHandle           decoder                 = getSimpleDecoder();
    CPid                                   * pPid                    = NULL;
    eRet                                     ret                     = eRet_Ok;
    NEXUS_Error                              nerror                  = NEXUS_SUCCESS;

    BDBG_ASSERT(false == isStarted());

    pPid = getPid();
    CHECK_PTR_GOTO("audio decoder has missing pid", pPid, ret, eRet_InvalidState, error);

    switch (pPid->getAudioCodec())
    {
    case NEXUS_AudioCodec_eAc3:
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAc3, &codecSettings);
        switch (dolbyDRC)
        {
        case eDolbyDRC_Light:
        case eDolbyDRC_Medium:
            codecSettings.codecSettings.ac3.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eLine;
            codecSettings.codecSettings.ac3.boost   = GET_INT(_pCfg, AUDIO_DOLBY_DRC_AC3_BOOST);
            codecSettings.codecSettings.ac3.cut     = GET_INT(_pCfg, AUDIO_DOLBY_DRC_AC3_CUT);
            break;
        case eDolbyDRC_Heavy:
            codecSettings.codecSettings.ac3.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eRf;
            codecSettings.codecSettings.ac3.boost   = 0;
            codecSettings.codecSettings.ac3.cut     = 0;
            break;
        case eDolbyDRC_None:
        default:
            codecSettings.codecSettings.ac3.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eLine;
            codecSettings.codecSettings.ac3.boost   = 0;
            codecSettings.codecSettings.ac3.cut     = 0;
            break;
        }
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);

        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAc3, &codecSettings);
        switch (dolbyDRC)
        {
        case eDolbyDRC_Light:
        case eDolbyDRC_Medium:
            codecSettings.codecSettings.ac3.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eLine;
            codecSettings.codecSettings.ac3.boost   = GET_INT(_pCfg, AUDIO_DOLBY_DRC_AC3_BOOST);
            codecSettings.codecSettings.ac3.cut     = GET_INT(_pCfg, AUDIO_DOLBY_DRC_AC3_CUT);
            break;
        case eDolbyDRC_Heavy:
            codecSettings.codecSettings.ac3.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eRf;
            codecSettings.codecSettings.ac3.boost   = 0;
            codecSettings.codecSettings.ac3.cut     = 0;
            break;
        case eDolbyDRC_None:
        default:
            codecSettings.codecSettings.ac3.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eLine;
            codecSettings.codecSettings.ac3.boost   = 0;
            codecSettings.codecSettings.ac3.cut     = 0;
            break;
        }
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        break;

    case NEXUS_AudioCodec_eAc3Plus:
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAc3Plus, &codecSettings);
        switch (dolbyDRC)
        {
        case eDolbyDRC_Light:
        case eDolbyDRC_Medium:
            codecSettings.codecSettings.ac3Plus.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eLine;
            codecSettings.codecSettings.ac3Plus.boost   = GET_INT(_pCfg, AUDIO_DOLBY_DRC_AC3_BOOST);
            codecSettings.codecSettings.ac3Plus.cut     = GET_INT(_pCfg, AUDIO_DOLBY_DRC_AC3_CUT);
            break;
        case eDolbyDRC_Heavy:
            codecSettings.codecSettings.ac3Plus.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eRf;
            codecSettings.codecSettings.ac3Plus.boost   = 0;
            codecSettings.codecSettings.ac3Plus.cut     = 0;
            break;
        case eDolbyDRC_None:
        default:
            codecSettings.codecSettings.ac3Plus.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eLine;
            codecSettings.codecSettings.ac3Plus.boost   = 0;
            codecSettings.codecSettings.ac3Plus.cut     = 0;
            break;
        }
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);

        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAc3Plus, &codecSettings);
        switch (dolbyDRC)
        {
        case eDolbyDRC_Light:
        case eDolbyDRC_Medium:
            codecSettings.codecSettings.ac3Plus.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eLine;
            codecSettings.codecSettings.ac3Plus.boost   = GET_INT(_pCfg, AUDIO_DOLBY_DRC_AC3_BOOST);
            codecSettings.codecSettings.ac3Plus.cut     = GET_INT(_pCfg, AUDIO_DOLBY_DRC_AC3_CUT);
            break;
        case eDolbyDRC_Heavy:
            codecSettings.codecSettings.ac3Plus.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eRf;
            codecSettings.codecSettings.ac3Plus.boost   = 0;
            codecSettings.codecSettings.ac3Plus.cut     = 0;
            break;
        case eDolbyDRC_None:
        default:
            codecSettings.codecSettings.ac3Plus.drcMode = NEXUS_AudioDecoderDolbyDrcMode_eLine;
            codecSettings.codecSettings.ac3Plus.boost   = 0;
            codecSettings.codecSettings.ac3Plus.cut     = 0;
            break;
        }
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        break;

    default:
        BDBG_WRN(("ignoring attempt to set dolby drc for invalid codec:%d", pPid->getAudioCodec()));
        ret = eRet_InvalidState;
        goto error;
    }

    _dolbyDRC = dolbyDRC;
    BDBG_MSG(("setting dolby drc:%d", _dolbyDRC));

error:
    return ret;
}


eRet CSimpleAudioDecode::setDolbyDialogNorm(bool dolbyDialogNorm)
{
    NEXUS_AudioDecoderCodecSettings          codecSettings;
    NEXUS_SimpleAudioDecoderHandle           decoder                 = getSimpleDecoder();
    CPid                                   * pPid                    = NULL;
    eRet                                     ret                     = eRet_Ok;
    NEXUS_Error                              nerror                  = NEXUS_SUCCESS;

    BDBG_ASSERT(false == isStarted());

    pPid = getPid();
    CHECK_PTR_GOTO("audio decoder has missing pid", pPid, ret, eRet_InvalidState, error);

    switch (pPid->getAudioCodec())
    {
    case NEXUS_AudioCodec_eAc3:
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, NEXUS_AudioCodec_eAc3, &codecSettings);
        codecSettings.codecSettings.ac3.dialogNormalization = dolbyDialogNorm;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        break;

    case NEXUS_AudioCodec_eAc3Plus:
        NEXUS_SimpleAudioDecoder_GetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, NEXUS_AudioCodec_eAc3Plus, &codecSettings);
        codecSettings.codecSettings.ac3Plus.dialogNormalization = dolbyDialogNorm;
        nerror = NEXUS_SimpleAudioDecoder_SetCodecSettings(decoder, NEXUS_SimpleAudioDecoderSelector_eSecondary, &codecSettings);
        CHECK_NEXUS_ERROR_GOTO("unable to set codec settings", ret, nerror, error);
        break;

    default:
        BDBG_WRN(("ignoring attempt to set dolby dialog normalization for invalid codec:%d", pPid->getAudioCodec()));
        ret = eRet_InvalidState;
        goto error;
    }

    _dolbyDialogNorm = dolbyDialogNorm;
    BDBG_MSG(("setting dolby dialog normalization:%d", _dolbyDialogNorm));

error:
    return ret;
}
*/
eRet SetSimpleAudioDecodeStc(int flag)
{
    eRet        ret    = eRet_Ok;
    NEXUS_Error nerror = NEXUS_SUCCESS;

    nerror = NEXUS_SimpleAudioDecoder_SetStcChannel(g_System.SimpleAudioDecoder, (0 != flag) ? g_System.SimpleStcChannel : NULL);
    CHECK_NEXUS_ERROR_GOTO("SetStcChannel simple audio decoder failed!", ret, nerror, error);

error:
    return ret;
}

