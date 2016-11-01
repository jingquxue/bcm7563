#include "b_os_lib.h"
#include "nexus_hdmi_output.h"
#include "nexus_hdmi_output_hdcp.h"
#include "main.h"
#include "global.h"
#include <stdio.h>

BDBG_MODULE(kwt_output);

eRet ProcessHotplugStatus (NEXUS_HdmiOutputStatus *pStatus);
NEXUS_AudioOutputSettings GetHdmiVolume ();
eRet SetHdmiVolume (NEXUS_AudioOutputSettings volume);
eRet SetHdmiMute (bool muted);
NEXUS_AudioOutput GetHdmiConnectorA ();
NEXUS_AudioOutput GetAudioDACConnectorA ();

eRet InitializeComposite(void)
{
	NEXUS_PlatformConfiguration platformConfig;
	eRet ret = eRet_Ok;

	NEXUS_Platform_GetConfiguration (&platformConfig);
	g_System.CompositeHandle = platformConfig.outputs.composite[0];

	return ret;
}

NEXUS_VideoOutput CompositeGetConnectorV(void)
{
	BDBG_ASSERT(g_System.CompositeHandle);
	return NEXUS_CompositeOutput_GetConnector (g_System.CompositeHandle);
}

bool CompositeIsValidVideoFormat (NEXUS_VideoFormat format)
{
	bool bValid = false;

	switch (format)
	{
		case NEXUS_VideoFormat_eNtsc:
		case NEXUS_VideoFormat_ePal:
			bValid = true;
			break;
		default:
			break;
	}

	return bValid;
}

void UninitializeHdmi(void)
{
	if (NULL != g_System.WidgetEngine)
	{
		bwidget_remove_io_handle (g_System.WidgetEngine, (bwin_io_handle) HDMI_HPD_IO_HANDLER);
	}

	if (NULL != g_System.HdcpEventId)
	{
		B_Scheduler_UnregisterEvent (g_System.Scheduler, g_System.HdcpEventId);
		g_System.HdcpEventId = NULL;
	}

	if (NULL != g_System.HdcpEvent)
	{
		B_Event_Destroy ( g_System.HdcpEvent);
		g_System.HdcpEvent = NULL;
	}
}

void HdmiHotplugCallback(void * context, int param)
{
	BSTD_UNUSED(context);
    BSTD_UNUSED(param);
	if(g_System.WidgetEngine != NULL)
	{
		bwidget_trigger_io(g_System.WidgetEngine, (bwin_io_handle)HDMI_HPD_IO_HANDLER);
	}
}

void HdcpStatusCallback (void *context, int param)
{
	BSTD_UNUSED(param);
	B_Event_Set (context);
}

void HdmiCecCallback (void *context, int param)
{
	BSTD_UNUSED(param);
	B_SchedulerCallback_Fire((B_SchedulerCallback *) context);
}

void bwinHdmiCallback (bwidget_engine_t engine, bwin_io_handle handle)
{

	NEXUS_HdmiOutputStatus status;
    BSTD_UNUSED(handle);
	BSTD_UNUSED(engine);
	NEXUS_HdmiOutput_GetStatus (g_System.HdmiHandle, &status);
	BDBG_MSG(("HDMI Hotplug Event status %d connected %d", status.connected, g_System.HdmiConnected));
	ProcessHotplugStatus (&status);
}

void SetWidgetEngine(void)
{
	bwidget_add_io_handle (g_System.WidgetEngine, (bwin_io_handle) HDMI_HPD_IO_HANDLER, bwinHdmiCallback);
}

bool isValidVideoFormat (NEXUS_VideoFormat format)
{
	NEXUS_Error nerror = NEXUS_SUCCESS;
	eRet ret = eRet_Ok;
	NEXUS_HdmiOutputStatus status;
	nerror = NEXUS_HdmiOutput_GetStatus (g_System.HdmiHandle, &status);
	CHECK_NEXUS_ERROR_GOTO("unable to get HDMI status", ret, nerror, error);

	return status.videoFormatSupported[format];
error:
	BDBG_WRN(("error checking valid hdmi video format - assume false"));
	return false;
}

eRet ProcessHotplugStatus (NEXUS_HdmiOutputStatus *pStatus)
{
	eRet ret = eRet_Ok;
	BDBG_ASSERT(NULL != pStatus);

	if (g_System.HdmiConnected == pStatus->connected)
	{
		return ret;
	}

	if (pStatus->connected)
	{

	}
	else
	{

	}
	g_System.HdmiConnected = pStatus->connected;

	return ret;
}

void HdmiHdcpHandler (void *context)
{
	BSTD_UNUSED(context);
	printf ("HDCP Handler tripped\n");
}

NEXUS_AudioOutputSettings GetHdmiVolume ()
{
	NEXUS_AudioOutputSettings settings;
	NEXUS_AudioOutput_GetSettings (GetHdmiConnectorA (), &settings);
	return settings;
}

eRet SetHdmiVolume (NEXUS_AudioOutputSettings volume)
{
	NEXUS_AudioOutputSettings settings;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	eRet ret = eRet_Ok;
	NEXUS_AudioOutput_GetSettings (GetHdmiConnectorA (), &settings);
	settings.volumeType = volume.volumeType;
	settings.leftVolume = volume.leftVolume;
	settings.rightVolume = volume.rightVolume;
	settings.muted = volume.muted;
	NEXUS_AudioOutput_SetSettings (GetHdmiConnectorA (), &settings);
	CHECK_NEXUS_ERROR_GOTO("unable to set audio output", ret, nerror, error);
error:
	return ret;
}

eRet SetHdmiMute (bool muted)
{
	NEXUS_AudioOutputSettings settings;
	settings = GetHdmiVolume ();
	settings.muted = muted;
	return SetHdmiVolume (settings);
}

eRet InitializeHdmi(void)
{
	eRet ret = eRet_Ok;
	NEXUS_PlatformConfiguration platformConfig;
	NEXUS_Error nError;
	NEXUS_HdmiOutputSettings hdmiSettings;
	NEXUS_HdmiOutputHdcpSettings *pHdcpSettings = NULL;
	NEXUS_Platform_GetConfiguration (&platformConfig);

	pHdcpSettings = (NEXUS_HdmiOutputHdcpSettings *) B_Os_Malloc (sizeof (NEXUS_HdmiOutputHdcpSettings));
	if (!pHdcpSettings)
	{
		ret = ATLAS_ERROR("Unable to malloc memory for HDCP settings!", eRet_OutOfMemory);
		BDBG_ASSERT(eRet_Ok != ret);
	}

	g_System.HdmiHandle = platformConfig.outputs.hdmi[0];
	g_System.HdcpEvent = B_Event_Create (NULL);
	BDBG_ASSERT(NULL != g_System.HdcpEvent);

	g_System.HdcpEventId = B_Scheduler_RegisterEvent (g_System.Scheduler,
														g_System.Lock, g_System.HdcpEvent, HdmiHdcpHandler,NULL);
	NEXUS_HdmiOutput_GetSettings (g_System.HdmiHandle, &hdmiSettings);
	hdmiSettings.hotplugCallback.callback = HdmiHotplugCallback;
	hdmiSettings.hotplugCallback.context = NULL;

	hdmiSettings.autoColorSpace = false;
	hdmiSettings.colorSpace = NEXUS_ColorSpace_eRgb;

	nError = NEXUS_HdmiOutput_SetSettings (g_System.HdmiHandle, &hdmiSettings);
	CHECK_NEXUS_ERROR_ASSERT("error setting hdmi output settings", nError);

	NEXUS_HdmiOutput_GetHdcpSettings (g_System.HdmiHandle, pHdcpSettings);
	pHdcpSettings->stateChangedCallback.callback = HdcpStatusCallback;
	pHdcpSettings->stateChangedCallback.context = g_System.HdcpEvent;
	pHdcpSettings->failureCallback.callback = HdcpStatusCallback;
	pHdcpSettings->failureCallback.context = g_System.HdcpEvent;
	nError = NEXUS_HdmiOutput_SetHdcpSettings (g_System.HdmiHandle, pHdcpSettings);
	CHECK_NEXUS_ERROR_ASSERT("error setting hdcp output settings", nError);

	{
		NEXUS_AudioOutputSettings settings;
		settings = GetHdmiVolume ();
		settings.volumeType = NEXUS_AudioVolumeType_eLinear;
		settings.leftVolume = 0x00800000;
		settings.rightVolume = 0x00800000;
		SetHdmiVolume (settings);
	}
	if (pHdcpSettings)
	{
		B_Os_Free(pHdcpSettings);
	}

	return ret;
}

NEXUS_VideoOutput GetHdmiConnectorV ()
{
	BDBG_ASSERT(g_System.HdmiHandle);
	return NEXUS_HdmiOutput_GetVideoConnector (g_System.HdmiHandle);
}

NEXUS_AudioOutput GetHdmiConnectorA ()
{
	BDBG_ASSERT(g_System.HdmiHandle);
	return NEXUS_HdmiOutput_GetAudioConnector (g_System.HdmiHandle);
}

eRet setHDMIColorSpace (NEXUS_ColorSpace colorSpace)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nError = NEXUS_SUCCESS;
	NEXUS_HdmiOutputSettings settings;

	BDBG_ASSERT(NEXUS_ColorSpace_eMax > colorSpace);
	NEXUS_HdmiOutput_GetSettings (g_System.HdmiHandle, &settings);
	settings.autoColorSpace = false;
	settings.colorSpace = colorSpace;
	nError = NEXUS_HdmiOutput_SetSettings (g_System.HdmiHandle, &settings);
	CHECK_NEXUS_ERROR_GOTO("error setting hdmi color space settings", ret, nError, error);
error:
	return ret;
}

eRet HDMIconnect (NEXUS_AudioInput input)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	BDBG_ASSERT(NULL != input);
	nerror = NEXUS_AudioOutput_AddInput (GetHdmiConnectorA (), input);
	CHECK_NEXUS_ERROR_GOTO("error adding input to hdmi output", ret, nerror, error);
error:
	return ret;
}

NEXUS_AudioOutputSettings GetAudioDACVolume(void)
{
	NEXUS_AudioOutputSettings settings;
	NEXUS_AudioOutput_GetSettings (GetAudioDACConnectorA (), &settings);
	return settings;
}

eRet SetAudioDACVolume (NEXUS_AudioOutputSettings volume)
{
	NEXUS_AudioOutputSettings settings;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	eRet ret = eRet_Ok;
	NEXUS_AudioOutput_GetSettings (GetAudioDACConnectorA (), &settings);
	settings.volumeType = volume.volumeType;
	settings.leftVolume = volume.leftVolume;
	settings.rightVolume = volume.rightVolume;
	settings.muted = volume.muted;
	NEXUS_AudioOutput_SetSettings (GetAudioDACConnectorA (), &settings);
	CHECK_NEXUS_ERROR_GOTO("unable to set audio output", ret, nerror, error);
error:
	return ret;
}

eRet SetAudioDACMute (bool muted)
{
	NEXUS_AudioOutputSettings settings;
	settings = GetAudioDACVolume ();
	settings.muted = muted;
	return SetAudioDACVolume (settings);
}

eRet InitializeAudioDac(void)
{
	NEXUS_PlatformConfiguration platformConfig;
	eRet ret = eRet_Ok;
	NEXUS_Platform_GetConfiguration (&platformConfig);
	if (NULL != platformConfig.outputs.audioDacs[0])
	{
		g_System.AudioDacHandle = platformConfig.outputs.audioDacs[0];
		{
			NEXUS_AudioOutputSettings settings;
			settings = GetAudioDACVolume ();
			settings.volumeType = NEXUS_AudioVolumeType_eLinear;
			settings.leftVolume = NEXUS_AUDIO_VOLUME_LINEAR_NORMAL;
			settings.rightVolume = NEXUS_AUDIO_VOLUME_LINEAR_NORMAL;
			SetAudioDACVolume (settings);
		}
	}
	return ret;
}

NEXUS_AudioOutput GetAudioDACConnectorA ()
{
	BDBG_ASSERT(g_System.AudioDacHandle);
	return NEXUS_AudioDac_GetConnector (g_System.AudioDacHandle);
}

eRet ConnectAudioDacOutput (NEXUS_AudioInput input)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	BDBG_ASSERT(NULL != input);
	nerror = NEXUS_AudioOutput_AddInput (GetAudioDACConnectorA (), input);
	CHECK_NEXUS_ERROR_GOTO("error adding input to audio output", ret, nerror, error);
error:
	return ret;
}

eRet DisconnectAudioDacOutput(void)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	nerror = NEXUS_AudioOutput_RemoveAllInputs (GetAudioDACConnectorA ());
	CHECK_NEXUS_ERROR_GOTO("error removing all inputs from audio output", ret, nerror, error);
error:
	return ret;
}

