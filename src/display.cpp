#include <stdio.h>


#include "nexus_display.h"
#include "mgeom.h"
#include "nexus_graphics2d.h"
#include "nexus_core_utils.h"



#include "main.h"
#include "graphics.h"
#include "output.h"
#include "global.h"



BDBG_MODULE(kwt_display);



eRet OpenDisplay(int number)
{
    NEXUS_DisplaySettings settings;
    eRet                  ret       = eRet_Ok;
    NEXUS_Error           nerror    = NEXUS_SUCCESS;

	if((number>2)||(number<0))
	{
		return eRet_InvalidParameter;
	}


    NEXUS_Display_GetDefaultSettings(&settings);

	settings.aspectRatio=NEXUS_DisplayAspectRatio_e16x9;

    g_System.Display[number] = NEXUS_Display_Open(number, &settings);
    CHECK_PTR_GOTO("nexus display open failed!", g_System.Display[number], ret, eRet_ExternalError, error);
    {
        NEXUS_VideoFormatInfo  videoFormatInfo;
        NEXUS_GraphicsSettings graphicsSettings;

        NEXUS_VideoFormat_GetInfo(settings.format, &videoFormatInfo);

        NEXUS_Display_GetGraphicsSettings(g_System.Display[number], &graphicsSettings);
        graphicsSettings.enabled     = true;
        graphicsSettings.clip.width  = videoFormatInfo.width;
        graphicsSettings.clip.height = videoFormatInfo.height;
        nerror = NEXUS_Display_SetGraphicsSettings(g_System.Display[number], &graphicsSettings);
        CHECK_NEXUS_ERROR_GOTO("Error setting graphics settings", ret, nerror, error);
    }

    return ret;

error:
    if (g_System.Display[number])
    {
        NEXUS_Display_Close(g_System.Display[number]);
        g_System.Display[number] = NULL;
    }
    return ret;
}


MRect GetGraphicsGeometry(int num)
{
    NEXUS_GraphicsSettings graphicsSettings;
    MRect                  rect;

    NEXUS_Display_GetGraphicsSettings(g_System.Display[num], &graphicsSettings);
	rect.set (graphicsSettings.clip.x, graphicsSettings.clip.y, graphicsSettings.clip.width, graphicsSettings.clip.height);

    return rect;
}


eRet SetVideoAspectRatio (int num/*,NEXUS_VideoFormat format*/,int graphics_opened, NEXUS_DisplayAspectRatio aspectRatio)
{
    eRet                      ret           = eRet_Ok;
    NEXUS_Error               nerror        = NEXUS_SUCCESS;
    NEXUS_DisplaySettings     settings;

	if (graphics_opened==1)
    {
        SetGraphicsActive(false);
    }

    //BDBG_ASSERT(NEXUS_VideoFormat_eUnknown != format);
   /* set display format */
    //BDBG_MSG(("set display %d format:%d", num,format ));
    NEXUS_Display_GetSettings(g_System.Display[num], &settings);
    //settings.format = format;
	settings.aspectRatio=aspectRatio;
    nerror = NEXUS_Display_SetSettings(g_System.Display[num], &settings);
    CHECK_NEXUS_ERROR_GOTO("unable to set video format", ret, nerror, error);

	/* update framebuffer size */
    if (graphics_opened==1)
    {
        MRect rect;

        rect = GetGraphicsGeometry(num);
        ret = SetFramebufferSize(num,rect.width(), rect.height());
        CHECK_ERROR_GOTO("unable to set frame buffer size", ret, error);

        SetGraphicsActive(true);
    }




error:
    return ret;
}


eRet SetVideoFormat(int num,NEXUS_VideoFormat format,int graphics_opened)
{
    eRet                      ret           = eRet_Ok;
    NEXUS_Error               nerror        = NEXUS_SUCCESS;
    NEXUS_DisplaySettings     settings;

	if (graphics_opened==1)
    {
        SetGraphicsActive(false);
    }

    BDBG_ASSERT(NEXUS_VideoFormat_eUnknown != format);
   /* set display format */
    BDBG_MSG(("set display %d format:%d", num,format ));
    NEXUS_Display_GetSettings(g_System.Display[num], &settings);
    settings.format = format;
	if(num==0)
	{

		if(g_Config.HdmiSettings.AspectRatio<NEXUS_DisplayAspectRatio_eMax)
		{
			settings.aspectRatio=(NEXUS_DisplayAspectRatio)g_Config.HdmiSettings.AspectRatio;
		}
		else
		{
			g_Config.HdmiSettings.AspectRatio=(unsigned int)NEXUS_DisplayAspectRatio_eAuto;
			settings.aspectRatio=(NEXUS_DisplayAspectRatio) g_Config.HdmiSettings.AspectRatio;
			SaveConfigToFlash();
		}
	}
	else	if(num==1)
	{

		if(g_Config.CvbsSettings.AspectRatio<NEXUS_DisplayAspectRatio_eMax)
		{
			settings.aspectRatio=(NEXUS_DisplayAspectRatio)g_Config.CvbsSettings.AspectRatio;
		}
		else
		{
			g_Config.CvbsSettings.AspectRatio=(unsigned int)NEXUS_DisplayAspectRatio_eAuto;
			settings.aspectRatio=(NEXUS_DisplayAspectRatio) g_Config.HdmiSettings.AspectRatio;
			SaveConfigToFlash();
		}
	}




    nerror = NEXUS_Display_SetSettings(g_System.Display[num], &settings);
    CHECK_NEXUS_ERROR_GOTO("unable to set video format", ret, nerror, error);

	/* update framebuffer size */
    if (graphics_opened==1)
    {
        MRect rect;

        rect = GetGraphicsGeometry(num);
        ret = SetFramebufferSize(num,rect.width(), rect.height());
        CHECK_ERROR_GOTO("unable to set frame buffer size", ret, error);

        SetGraphicsActive(true);
    }




error:
    return ret;
}




eRet AddHdmiOutput(void)
{
    eRet ret = eRet_InvalidParameter; /* assume error */

	NEXUS_Error nerror = NEXUS_SUCCESS;
	nerror = NEXUS_Display_AddOutput(g_System.Display[0], GetHdmiConnectorV());
    CHECK_NEXUS_ERROR_GOTO("failure to add output", ret, nerror, error);
error:
    return ret;
}



eRet AddCvbsOutput(void)
{
    eRet ret = eRet_InvalidParameter; /* assume error */

	NEXUS_Error nerror = NEXUS_SUCCESS;
	nerror = NEXUS_Display_AddOutput(g_System.Display[1], CompositeGetConnectorV());
    CHECK_NEXUS_ERROR_GOTO("failure to add output", ret, nerror, error);
error:
    return ret;
}





void CloseDisplay(void)
{

    if (NULL != g_System.Display[0])
    {
        NEXUS_Display_RemoveAllOutputs(g_System.Display[0]);
		NEXUS_Display_Close(g_System.Display[0]);
        g_System.Display[0] = NULL;

    }

	if (NULL != g_System.Display[1])
    {
        NEXUS_Display_RemoveAllOutputs(g_System.Display[1]);
		NEXUS_Display_Close(g_System.Display[1]);
        g_System.Display[1] = NULL;

    }

}


struct g_SupportedVideoFormat
{
	unsigned int format;
    NEXUS_VideoFormat BcmFormat;

} SupporedVideoFormat[] =
{
    /* format */       /* BcmFormat */
	{0, NEXUS_VideoFormat_ePal},
	{1, NEXUS_VideoFormat_e576p},
	{2, NEXUS_VideoFormat_eNtsc},
	{3, NEXUS_VideoFormat_e480p},
	{4, NEXUS_VideoFormat_e720p50hz},
	{5, NEXUS_VideoFormat_e720p},
	{6, NEXUS_VideoFormat_e1080i50hz},
	{7, NEXUS_VideoFormat_e1080i},
	{8, NEXUS_VideoFormat_e1080p24hz},
	{9, NEXUS_VideoFormat_e1080p25hz},
	{10, NEXUS_VideoFormat_e1080p30hz},
	{11, NEXUS_VideoFormat_e1080p50hz},
	{12, NEXUS_VideoFormat_e1080p60hz}
};



NEXUS_VideoFormat ConvertVideoFormatToBroadcom(unsigned char HdorSD,unsigned int format)
{
	unsigned int i = 0;
	NEXUS_VideoFormat tmp_format;
	for(i=0;i< sizeof(SupporedVideoFormat)/sizeof(SupporedVideoFormat[0]);i++)
	{
		if(format==SupporedVideoFormat[i].format)
		{
			tmp_format=SupporedVideoFormat[i].BcmFormat;
			break;
		}
	}

	if(i==13)
	{
		printf("unsupported video format\n");
		if(HdorSD==0)
		{
			tmp_format=NEXUS_VideoFormat_e720p;
		}
		else if(HdorSD==1)
		{
			tmp_format=NEXUS_VideoFormat_ePal;
		}
	}

	if(HdorSD==1)
	{
		if((tmp_format != NEXUS_VideoFormat_ePal) && (tmp_format != NEXUS_VideoFormat_eNtsc))
		{
			tmp_format=NEXUS_VideoFormat_ePal;
		}

	}
	printf("ConvertVideoFormatToBroadcom from %d to bcm %d for display %d\n",format,tmp_format,HdorSD);

	return tmp_format;




}



unsigned int ConvertVideoFormatToKwt(unsigned char HdorSD,NEXUS_VideoFormat format)
{
	unsigned int i = 0;
	unsigned int tmp_format;
	for(i=0;i< sizeof(SupporedVideoFormat)/sizeof(SupporedVideoFormat[0]);i++)
	{
		if(format==SupporedVideoFormat[i].BcmFormat)
		{
			tmp_format=SupporedVideoFormat[i].format;
			break;
		}
	}

	if(i==13)
	{
		printf("unsupported video format\n");
		if(HdorSD==0)
		{
			tmp_format=5;
		}
		else if(HdorSD==1)
		{
			tmp_format=0;
		}
	}

	if(HdorSD==1)
	{
		if((tmp_format != 0) && (tmp_format != 2))
		{
			tmp_format=0;
		}

	}
	printf("ConvertVideoFormatToKwt from bcm %d to kwt %d for display %d\n",format,tmp_format,HdorSD);

	return tmp_format;




}





