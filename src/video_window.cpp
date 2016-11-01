#include "nexus_video_window.h"
#include "main.h"
#include "display.h"
#include "global.h"

BDBG_MODULE(kwt_video_window);





eRet OpenHDVideoWindow(void)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nerror = NEXUS_SUCCESS;

	g_System.VideoWindow_HD = NEXUS_VideoWindow_Open (g_System.Display[0], 0);
	CHECK_PTR_GOTO("Nexus video window open failed", g_System.VideoWindow_HD, ret, eRet_ExternalError, error);
	{
		NEXUS_VideoWindowSettings settings;
		NEXUS_VideoWindow_GetSettings (g_System.VideoWindow_HD, &settings);
		settings.contentMode = NEXUS_VideoWindowContentMode_eBox;
		nerror = NEXUS_VideoWindow_SetSettings (g_System.VideoWindow_HD, &settings);
		CHECK_NEXUS_ERROR_GOTO("unable to set content mode", ret, nerror, error);
	}
error:
	return ret;
}

eRet OpenSDVideoWindow(void)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nerror = NEXUS_SUCCESS;

	g_System.VideoWindow_SD = NEXUS_VideoWindow_Open (g_System.Display[1], 0);
	CHECK_PTR_GOTO("Nexus video window open failed", g_System.VideoWindow_SD, ret, eRet_ExternalError, error);
	{
		NEXUS_VideoWindowSettings settings;
		NEXUS_VideoWindow_GetSettings (g_System.VideoWindow_SD, &settings);
		settings.contentMode = NEXUS_VideoWindowContentMode_eBox;
		nerror = NEXUS_VideoWindow_SetSettings (g_System.VideoWindow_SD, &settings);
		CHECK_NEXUS_ERROR_GOTO("unable to set content mode", ret, nerror, error);
	}
error:
	return ret;
}

void CloseVideoWindow(void)
{
	if (NULL != g_System.VideoWindow_HD)
	{
		NEXUS_VideoWindow_RemoveAllInputs (g_System.VideoWindow_HD);
		NEXUS_VideoWindow_Close (g_System.VideoWindow_HD);
		g_System.VideoWindow_HD = NULL;
	}
	if (NULL != g_System.VideoWindow_SD)
	{
		NEXUS_VideoWindow_RemoveAllInputs (g_System.VideoWindow_SD);
		NEXUS_VideoWindow_Close (g_System.VideoWindow_SD);
		g_System.VideoWindow_SD = NULL;
	}
}

