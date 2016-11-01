#include "nexus_types.h"
#include "nexus_simple_stc_channel.h"
#include "nexus_timebase.h"
#include "main.h"
#include "global.h"

BDBG_MODULE(kwt_stc);

eRet OpenStc(void)
{
	eRet ret = eRet_Ok;
	NEXUS_SimpleStcChannelSettings settings;
	NEXUS_SimpleStcChannel_GetDefaultSettings (&settings);
	g_System.SimpleStcChannel = NEXUS_SimpleStcChannel_Create (&settings);
	CHECK_PTR_GOTO("Stc channel open failure", g_System.SimpleStcChannel, ret, eRet_ExternalError, error);
error:
	return ret;
}

eRet ConfigureStc (eStcType StcType, NEXUS_PidChannelHandle pcrhandle)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	NEXUS_SimpleStcChannelSettings settings;
	NEXUS_SimpleStcChannel_GetDefaultSettings (&settings);

	switch (StcType)
	{
		case eStcType_ParserBand:
			settings.mode = NEXUS_StcChannelMode_ePcr;
			settings.modeSettings.pcr.pidChannel = pcrhandle;
			settings.modeSettings.highJitter.mode = NEXUS_SimpleStcChannelHighJitterMode_eNone;
			BDBG_MSG(("eStcType_ParserBand"));
			break;
		case eStcType_TtsPacing:
			BDBG_ASSERT("not supported yet");
			break;
		case eStcType_PcrPacing:
			BDBG_ASSERT("not supported yet");
			break;
		case eStcType_IpLivePlayback:
			settings.modeSettings.highJitter.threshold = 1000;
			settings.modeSettings.highJitter.mode = NEXUS_SimpleStcChannelHighJitterMode_eDirect;
			settings.modeSettings.Auto.transportType = NEXUS_TransportType_eTs;
			BDBG_MSG(("eStcType_IpLivePlayback"));
			break;
		case eStcType_IpPullPlayback:
		case eStcType_PvrPlayback:
			settings.mode = NEXUS_StcChannelMode_eAuto;
			settings.modeSettings.Auto.transportType = NEXUS_TransportType_eTs;
			break;
		default:
			BDBG_ASSERT("not supported yet");
			break;
	}

	nerror = NEXUS_SimpleStcChannel_SetSettings (g_System.SimpleStcChannel, &settings);
	CHECK_NEXUS_ERROR_GOTO("error setting stc channel settings", ret, nerror, error);
	BDBG_MSG(("Successful Call to %s", __FUNCTION__));
	return ret;
error:
	BDBG_ERR(("Reset STC Channel settings to Default"));
	NEXUS_SimpleStcChannel_GetDefaultSettings (&settings);
	nerror = NEXUS_SimpleStcChannel_SetSettings (g_System.SimpleStcChannel, &settings);
	BDBG_ERR(("error re-setting stc channel settings stc %p", g_System.SimpleStcChannel));
	return ret;
}

void CloseStc(void)
{
	if (NULL != g_System.SimpleStcChannel)
	{
		NEXUS_SimpleStcChannel_Destroy (g_System.SimpleStcChannel);
	}
}

