#include "nexus_video_decoder.h"
#include "nexus_simple_video_decoder_server.h"
#include "nexus_core_utils.h"
#include "nexus_pid_channel.h"
#include "nexus_parser_band.h"
#include "nexus_video_input.h"
#include "main.h"
#include "stc.h"
#include "global.h"
#include <stdio.h>

BDBG_MODULE(kwt_video_decode);

static NEXUS_VideoDecoderHandle m_VideoDecoder = NULL;

static void bwinVideoDecodeCallback (bwidget_engine_t engine, bwin_io_handle handle)
{
	BSTD_UNUSED(engine);
	BSTD_UNUSED(handle);
	printf ("source changed\n");

	return;
}

static void NexusVideoDecodeSourceChangedCallback (void *context, int param)
{
	BSTD_UNUSED(context);
	BSTD_UNUSED(param);
	bwidget_trigger_io (g_System.WidgetEngine, (bwin_io_handle) SIMPLE_VIDEO_DECODE_HANDLE);
}

eRet OpenVideoDecode(void)
{
	eRet ret = eRet_Ok;
	NEXUS_VideoDecoderOpenSettings settings;
	bwidget_add_io_handle (g_System.WidgetEngine, (bwin_io_handle) VIDEO_DECODE_HANDLE, bwinVideoDecodeCallback);
	NEXUS_VideoDecoder_GetDefaultOpenSettings (&settings);
	m_VideoDecoder = NEXUS_VideoDecoder_Open (0, &settings);
	CHECK_PTR_GOTO("Nexus video decoder open failed!", m_VideoDecoder, ret, eRet_ExternalError, error);
error:
	return ret;
}

eRet SetSimpleVideoDecoderStc(void)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	nerror = NEXUS_SimpleVideoDecoder_SetStcChannel (g_System.SimpleVideoDecoder, g_System.SimpleStcChannel);
	CHECK_NEXUS_ERROR_GOTO("SetStcChannel simple video decoder failed!", ret, nerror, error);
error:
	return ret;
}

eRet OpenSimpleVideoDecode(void)
{
	eRet ret = eRet_Ok;
	bwidget_add_io_handle (g_System.WidgetEngine, (bwin_io_handle) SIMPLE_VIDEO_DECODE_HANDLE, bwinVideoDecodeCallback);
	ret = OpenVideoDecode ();
	CHECK_ERROR_GOTO("Video decode failed to open", ret, error);
	{
		NEXUS_SimpleVideoDecoderServerSettings settings;
		NEXUS_SimpleVideoDecoder_GetDefaultServerSettings (&settings);
		settings.videoDecoder = m_VideoDecoder;
		g_System.SimpleVideoDecoder = NEXUS_SimpleVideoDecoder_Create (0, &settings);
		CHECK_PTR_GOTO("unable to create simple video decoder", g_System.SimpleVideoDecoder, ret, eRet_NotAvailable, error);
	}
	ret = SetSimpleVideoDecoderStc ();
	CHECK_ERROR_GOTO("SetStcChannel simple video decoder failed!", ret, error);
	{
		NEXUS_Error nError = NEXUS_SUCCESS;
		NEXUS_VideoDecoderSettings videoDecoderSettings;
		NEXUS_SimpleVideoDecoder_GetSettings (g_System.SimpleVideoDecoder, &videoDecoderSettings);
		videoDecoderSettings.sourceChanged.callback = NexusVideoDecodeSourceChangedCallback;
		videoDecoderSettings.sourceChanged.context = NULL;
		videoDecoderSettings.sourceChanged.param = 0;
		videoDecoderSettings.dropFieldMode = false;
		nError = NEXUS_SimpleVideoDecoder_SetSettings (g_System.SimpleVideoDecoder, &videoDecoderSettings);
		CHECK_NEXUS_ERROR_GOTO("Error applying simple video decoder settings.", ret, nError, error);
	}
error:
	return ret;
}

eRet AddHDVideoWindow(void)
{
	NEXUS_SimpleVideoDecoderServerSettings settings;
	int i = 0;
	eRet ret = eRet_ExternalError;
	NEXUS_Error nerror = NEXUS_SUCCESS;

	NEXUS_SimpleVideoDecoder_GetServerSettings (g_System.SimpleVideoDecoder, &settings);
	for (i = 0; i < NEXUS_MAX_DISPLAYS; i++)
	{
		if (NULL == settings.window[i])
		{
			break;
		}
	}
	if (NEXUS_MAX_DISPLAYS > i)
	{
		settings.window[i] = g_System.VideoWindow_HD;
		nerror = NEXUS_SimpleVideoDecoder_SetServerSettings (g_System.SimpleVideoDecoder, &settings);
		CHECK_NEXUS_ERROR_GOTO("unable to set new video window in simple decoder server settings", ret, nerror, done);
	}
done:
	return ret;
}

eRet AddSDVideoWindow(void)
{
	NEXUS_SimpleVideoDecoderServerSettings settings;
	int i = 0;
	eRet ret = eRet_ExternalError;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	NEXUS_SimpleVideoDecoder_GetServerSettings (g_System.SimpleVideoDecoder, &settings);
	for (i = 0; i < NEXUS_MAX_DISPLAYS; i++)
	{
		if (NULL == settings.window[i])
		{
			break;
		}
	}
	if (NEXUS_MAX_DISPLAYS > i)
	{
		settings.window[i] = g_System.VideoWindow_SD;
		nerror = NEXUS_SimpleVideoDecoder_SetServerSettings (g_System.SimpleVideoDecoder, &settings);
		CHECK_NEXUS_ERROR_GOTO("unable to set new video window in simple decoder server settings", ret, nerror, done);
	}
done:
	return ret;
}

#define TRANSPORT_TYPE NEXUS_TransportType_eTs
#define VIDEO_CODEC NEXUS_VideoCodec_eH264
#define AUDIO_CODEC NEXUS_AudioCodec_eMpeg
#define VIDEO_PID 0x1011
#define AUDIO_PID 0x1100

eRet StartVideoDecode (NEXUS_PidChannelHandle videoPidChannel)
{
	NEXUS_SimpleVideoDecoderStartSettings simpleSettings;
	NEXUS_VideoDecoderExtendedSettings extendSettings;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	eRet ret = eRet_Ok;
	NEXUS_SimpleVideoDecoder_GetExtendedSettings (g_System.SimpleVideoDecoder, &extendSettings);
	extendSettings.treatIFrameAsRap = true;
	NEXUS_SimpleVideoDecoder_SetExtendedSettings (g_System.SimpleVideoDecoder, &extendSettings);
	NEXUS_SimpleVideoDecoder_GetDefaultStartSettings (&simpleSettings);
	simpleSettings.settings.codec = NEXUS_VideoCodec_eH264;
	simpleSettings.settings.pidChannel = videoPidChannel;
	nerror = NEXUS_SimpleVideoDecoder_Start (g_System.SimpleVideoDecoder, &simpleSettings);
	CHECK_NEXUS_ERROR_GOTO("starting simple video decoder failed!", ret, nerror, error);
error:
	return ret;
}

eRet CloseVideoDecode ()
{
	eRet ret = eRet_Ok;
	if (m_VideoDecoder)
	{
		NEXUS_VideoInput_Shutdown (NEXUS_VideoDecoder_GetConnector (m_VideoDecoder));
		NEXUS_VideoDecoder_Close (m_VideoDecoder);
		m_VideoDecoder = NULL;
	}
	if (NULL != g_System.WidgetEngine)
	{
		bwidget_remove_io_handle (g_System.WidgetEngine, (bwin_io_handle) VIDEO_DECODE_HANDLE);
	}

	return ret;
}

eRet GetVideoDecodeStatus (NEXUS_VideoDecoderStatus *pStatus)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	BDBG_ASSERT(NULL != pStatus);
	nerror = NEXUS_VideoDecoder_GetStatus (m_VideoDecoder, pStatus);
	CHECK_NEXUS_ERROR_GOTO("getting video decoder status failed!", ret, nerror, error);
error:
	return ret;
}

int GetVideoDecodeStarted(void)
{
    eRet        ret    = eRet_Ok;
    NEXUS_Error nerror = NEXUS_SUCCESS;
	NEXUS_VideoDecoderStatus status;
    nerror = NEXUS_VideoDecoder_GetStatus(m_VideoDecoder, &status);
	return status.started;
}











eRet CloseSimpleVideoDecode(void)
{
	eRet ret = eRet_Ok;
	if (NULL != g_System.SimpleVideoDecoder)
	{
		NEXUS_SimpleVideoDecoderServerSettings settings;
		NEXUS_SimpleVideoDecoder_SetStcChannel (g_System.SimpleVideoDecoder, NULL);
		NEXUS_SimpleVideoDecoder_GetDefaultServerSettings (&settings);
		settings.enabled = false;
		NEXUS_SimpleVideoDecoder_SetServerSettings (g_System.SimpleVideoDecoder, &settings);
		NEXUS_SimpleVideoDecoder_Destroy (g_System.SimpleVideoDecoder);
		g_System.SimpleVideoDecoder = NULL;
	}
	if (NULL != m_VideoDecoder)
	{
		CloseVideoDecode ();
		m_VideoDecoder = NULL;
	}
	if (NULL != g_System.WidgetEngine)
	{
		bwidget_remove_io_handle (g_System.WidgetEngine, (bwin_io_handle) SIMPLE_VIDEO_DECODE_HANDLE);
	}

	return ret;
}

eRet StopSimpleVideoDecode(void)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	BDBG_ASSERT(NULL != g_System.SimpleVideoDecoder);
	NEXUS_SimpleVideoDecoder_Stop (g_System.SimpleVideoDecoder);
	nerror = NEXUS_SimpleVideoDecoder_SetStcChannel (g_System.SimpleVideoDecoder, NULL);
	CHECK_NEXUS_ERROR_GOTO("SetStcChannel simple video decoder failed!", ret, nerror, error);
error:
	return ret;
}

unsigned int GetSimpleVIdeoDecodeFrame(void)
{
	NEXUS_VideoDecoderStatus status;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	nerror = NEXUS_SimpleVideoDecoder_GetStatus (g_System.SimpleVideoDecoder, &status);
	return status.numDecoded;
}

eRet GetSimpleVideoDecodeStatus (NEXUS_VideoDecoderStatus *pStatus)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	BDBG_ASSERT(NULL != pStatus);
	nerror = NEXUS_SimpleVideoDecoder_GetStatus (g_System.SimpleVideoDecoder, pStatus);
	CHECK_NEXUS_ERROR_GOTO("getting simple video decoder status failed!", ret, nerror, error);
error:
	return ret;
}

