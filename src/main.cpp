/******************************************************************************

                  版权所有 (C), 2001-2011, 深圳市科卫泰实业发展有限公司

 ******************************************************************************
  文 件 名   : main.cpp
  版 本 号   : 初稿
  作    者   : 马登坤
  生成日期   : 2015年6月3日
  最近修改   :
  功能描述   :
  函数列表   :
              buildDate
              cleanExit
              GetDib3000LockStatus
              getWidgetEngine
              i2c_init
              idle_callback
              initialize
              initializeAtlas
              initializeNexus
              initializeOsLib
              InitializeParameters
              main
              mythread
              runScheduler
              uninitializeAtlas
              uninitializeNexus
              uninitializeOsLib
  修改历史   :
  1.日    期   : 2015年6月3日
    作    者   : 马登坤
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "global.h"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/******************************************************************************

                  版权所有 (C), 2001-2011, 深圳市科卫泰实业发展有限公司

 ******************************************************************************
  文 件 名   : main.cpp
  版 本 号   : 初稿
  作    者   : 马登坤
  生成日期   : 2015年6月2日
  最近修改   :
  功能描述   : 对平台进行初始化，并创建各个线程
  函数列表   :
              buildDate
              cleanExit
              GetDib3000LockStatus
              getWidgetEngine
              i2c_init
              idle_callback
              initialize
              initializeAtlas
              initializeNexus
              initializeOsLib
              main
              mythread
              runScheduler
              uninitializeAtlas
              uninitializeNexus
              uninitializeOsLib
  修改历史   :
  1.日    期   : 2015年6月2日
    作    者   : 马登坤
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "bstd.h"
#include "b_os_lib.h"
#include "bwin.h"
#include "bwidgets.h"


#include "nexus_simple_stc_channel.h"

#include "nexus_video_decoder.h"
#include "nexus_simple_video_decoder_server.h"
#include "nexus_core_utils.h"
#include "nexus_watchdog.h"





#include "main.h"
#include "display.h"
#include "graphics.h"
#include "output.h"
#include "screen.h"
#include "remote.h"
#include "menu_main.h"
#include "dib3000.h"
#include "stc.h"
#include "video_decode.h"
#include "video_window.h"
#include "channellist.h"
#include "menu_info.h"
#include "menu_channelsearch.h"
#include "hotplug.h"
#include "record.h"
#include "menu_fileBrowser.h"
#include "menu_record.h"
#include "audio_decode.h"
#include "channel.h"
#include "playback.h"
#include "menu_playback.h"
#include "menu_datetimeset.h"
#include "transmitter.h"
#include "menu_networkconfig.h"
#include "menu_volume.h"
#include "rtc.h"
#include "uart.h"


#include "menu_channeledit.h"
#include "menu_descramblingconfig.h"
#include "menu_displaysettings.h"
#include "menu_generalsettings.h"
#include "menu_systeminfo.h"
#include "menu_tcpipconfiguration.h"
#include "menu_videochannellist.h"
#include "menu_volume.h"
#include "menu_recordSettings.h"










#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/



bwidget_t L_BackGround;

eMenu CurrentMenu=eMenu_Info;
static unsigned int m_ChannelPlaying=0;

NEXUS_I2cHandle i2c=NULL;

















/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
BDBG_MODULE(kwt_main);

const char *BuildDate ()
{
    return __DATE__ " " __TIME__;
}

static void CleanExit(int sig)
{
	BSTD_UNUSED(sig);
	g_Status.ExitApp=1;
}

eRet initializeNexus()
{
    eRet                     ret                    = eRet_Ok;
    NEXUS_Error              nError                 = NEXUS_SUCCESS;
    const char             * envMaxHdDisplayFormat  = NEXUS_GetEnv("max_hd_display_format");//获取环境变量
    NEXUS_PlatformSettings   platformSettings;



    NEXUS_Platform_GetDefaultSettings(&platformSettings);///home/mdk/work/bcm7563/refsw/nexus/platforms/common/src/nexus_platform_settings.c
    /* DTT make modification to platformSettings based on config */

    /* initializing the frontend is slow so we'll do this after we have everything else in
       the app up and running */
    /*platformSettings.openFrontend = false;*//* dtt todo - change to false and then init in idle loop */

    /* DTT todo add dac bandgap support */
    platformSettings.openFrontend = false;

    if (envMaxHdDisplayFormat)
    {

        if (!strcasecmp(envMaxHdDisplayFormat, "1080p"))
        {
            if(platformSettings.displayModuleSettings.fullHdBuffers.count)
            {
				platformSettings.displayModuleSettings.fullHdBuffers.format = NEXUS_VideoFormat_e1080p;
            }
            else
            {
				unsigned primaryDisplayHeapIndex = platformSettings.displayModuleSettings.primaryDisplayHeapIndex;
                platformSettings.displayModuleSettings.displayHeapSettings[primaryDisplayHeapIndex].fullHdBuffers.format = NEXUS_VideoFormat_e1080p;
            }
        }
    }

    nError = NEXUS_Platform_Init(&platformSettings);
    CHECK_NEXUS_ERROR_ASSERT("error calling NEXUS_PLatformInit()", nError);

    return ret;
}



void uninitializeNexus()
{
    NEXUS_Platform_Uninit();
}

static void RunScheduler(void *pParam)
{
    B_SchedulerHandle scheduler = (B_SchedulerHandle)pParam;
    B_Scheduler_Run(scheduler);
}


int GetDib3000LockStatus(void)
{
	int ret=1;
	ret=dib3000_CheckLock(i2c,0);
	return ret;
}

void i2c_init(void)
{
	NEXUS_PlatformConfiguration platformConfig;
	NEXUS_I2cSettings i2cSettings;


 	NEXUS_Platform_GetConfiguration(&platformConfig);
 	NEXUS_I2c_GetDefaultSettings(&i2cSettings);
 	i2cSettings.interruptMode=0;
 	i2cSettings.burstMode=1;
 	i2cSettings.timeout=0;
 	i2cSettings.clockRate=NEXUS_I2cClockRate_e100Khz;
 	NEXUS_I2c_SetSettings(platformConfig.i2c[2], &i2cSettings);
 	i2c=platformConfig.i2c[2];
 	dib_init(i2c);

	init_rtc();

	g_Status.RtcInited=1;


}


void UpdateRFInfoBar(void)
{
	int ret;
    B_Time          currentTime = B_TIME_ZERO;
    long            diffTime    = 0;
	static int num=0;
	static unsigned int NumDecodedVid=0;
	unsigned int tmp_NumDecodedVid=0;

	static unsigned int NumDecodedAud=0;
	unsigned int tmp_NumDecodedAud=0;

	static unsigned un_equal_time=0;

    if(g_Status.TunerType != 0)
    {
    	if((g_Channel.ChannelCount==0)&&(g_Channel.OverLoop==0))
    	{
    		//no channel in channel list
    		MenuInfoUpdate(1);
			if(GetVideoDecodeStarted()==1)
    		{
    			StopLiveChannel();
    		}


    	}
    	else
    	{
    		if(g_Status.ChannelChanged==0)
    		{

					ret=GetDib3000LockStatus();
					g_Status.ChannelLocked=ret;
                 	if(ret != 1)
                 	{

						num++;
						if(num==10)
						{
							//printf("num=%d\n",num);
						dib_tune (i2c, g_Channel.ChannelList[g_Channel.CurrentChannel].Frequency,
																g_Channel.ChannelList[g_Channel.CurrentChannel].BandWidth);
                              num=0;

						}

                 	}
					else
					{

						if((GetVideoDecodeStarted()==0)&&(g_Status.PlaybackStarted==0))
						{
							StartLiveChannel(g_Status.ChanelTuned);
						}

						if((GetVideoDecodeStarted()==1)&&(g_Status.PlaybackStarted==0))
						{
							tmp_NumDecodedVid=GetSimpleVIdeoDecodeFrame();
							tmp_NumDecodedAud=GetSimpleAudioDecodeFrame();
							if((tmp_NumDecodedVid == NumDecodedVid)||(tmp_NumDecodedAud==NumDecodedAud))
							{
								un_equal_time++;

								if(un_equal_time==20)
								{
								//reboot
								un_equal_time=0;
								StopLiveChannel();
								usleep(200000);
								StartLiveChannel(g_Status.ChanelTuned);
								}


							}
							else
							{
								NumDecodedVid=tmp_NumDecodedVid;
								NumDecodedAud=tmp_NumDecodedAud;
								un_equal_time=0;

							}
						}








					}


    				//dib_show_signal_strength_quality(i2c);
                 	if(CurrentMenu==eMenu_Info)
                 	{
         				MenuInfoUpdate(0);
                 	}

    		}
    		else if(g_Status.ChannelChanged==1)
    		{
					if(CurrentMenu==eMenu_Info)
                 	{
         				MenuInfoUpdate(0);
                 	}


                    B_Time_Get(&currentTime);
                    diffTime = B_Time_Diff(&currentTime, &g_Status.ChannelChangedTime);
					if(diffTime>1500)
					{
						g_Status.ChannelChanged=0;
						g_Status.ChanelTuned=g_Channel.CurrentChannel;
						if((g_Channel.ChannelList[g_Status.ChanelTuned].Frequency>100000)
			            && (g_Channel.ChannelList[g_Status.ChanelTuned].Frequency<1000000))
			            {
							dib_tune(i2c,g_Channel.ChannelList[g_Status.ChanelTuned].Frequency,g_Channel.ChannelList[g_Status.ChanelTuned].BandWidth);
    						if(GetVideoDecodeStarted()==1)
    						{
    							StopLiveChannel();
    						}
							 if(GetVideoDecodeStarted()==0)
    						{
    							StartLiveChannel(g_Status.ChanelTuned);
    						}

					    }
						else
						{
							printf("not supported frequency\n");
			            }


						SaveChannelToFlash();
					}
    		}
    	}
    }
    else
    {

    }
}



static void CostTimeThread(void *pParam)
{
	BSTD_UNUSED(pParam);
	static unsigned int tmp=1;

	static unsigned int LastChannel=0x00FF;
	static B_Time   lastTime    = B_TIME_ZERO;
    B_Time          currentTime = B_TIME_ZERO;
    long            diffTime    = 0;
	NEXUS_VideoDecoderStatus VidDecStatus;

	sleep(5);
	NEXUS_Watchdog_SetTimeout(20);
	while(1)
	{

#if 0
		if((g_Status.ChannelLocked==1)&&(g_Status.ChannelChanged==0)&&(g_Status.ReTuneNeed==0))
		{
			//check si info
			//printf("g_Status.ChannelLocked=%d\n",g_Status.ChannelLocked);
			if(ChannelInfoChanged()==1)
			{
				printf("channel info changed\n");
				if(g_Status.ChannelChanged==0)
				{
					SaveChannelToFlash();
				}
				if(g_Status.LivePlaying==1)
    			{
    				StopLiveChannel();
    				g_Status.LivePlaying=0;
    			}
    			StartLiveChannel(g_Channel.CurrentChannel);
    			g_Status.LivePlaying=1;
			}



		}
#endif
        NEXUS_Watchdog_StartTimer();
		//GetVideoDecodeStatus(&VidDecStatus);
		//printf("numdecode=%d\n",VidDecStatus.numDecoded);



//DumpPlaybackStatus();

		sleep(1);


	}


}




static void InfoThread(void *pParam)
{
    BSTD_UNUSED(pParam);
	sleep(1);
	static unsigned int count=0;


	if((g_Config.NetRelaySettings.Type==1)&&(g_Config.NetRelaySettings.Enable==1))
	{
		//网络接收
		g_Status.OperatingMode=1;

	}
	else
	{
		g_Status.OperatingMode=0;
		i2c_init();

    if(g_Status.TunerType != 0)
    {
		if((g_Channel.ChannelList[g_Channel.CurrentChannel].Frequency>100000)
	    &&(g_Channel.ChannelList[g_Channel.CurrentChannel].Frequency<1000000))
		{
			    g_Status.ChanelTuned=g_Channel.CurrentChannel;
				dib_tune(i2c,g_Channel.ChannelList[g_Channel.CurrentChannel].Frequency,g_Channel.ChannelList[g_Channel.CurrentChannel].BandWidth);
				StartLiveChannel(g_Status.ChanelTuned);
    			g_Status.LivePlaying=1;
	     }
    }



	}



	while(1)
	{
		if (CurrentMenu == eMenu_DateTimeSet)
		{
			MenuDateTimeSerUpdate ();
		}

		if (CurrentMenu == eMenu_ChannelSearch)
		{
			MenuChannelSearchUpdate ();
		}

		if(g_Status.OperatingMode==0)
		{
			UpdateRFInfoBar();
		}
		else
		{

		}

		if(CurrentMenu==eMenu_Playback)
		{
			MenuPlaybackUpdate(NULL);
		}

		if(g_Status.MenuVolumeDisplayed==1)
		{
			count++;
			if(count == 10)
			{
				count=0;
				g_Status.MenuVolumeDisplayed=0;
				MenuVolumeShow(false);
				SaveConfigToFlash();

			}
		}








		usleep(500000);


	}


}

eRet initializeOsLib()
{
    eRet             ret      = eRet_Ok;
    NEXUS_Error      nError   = NEXUS_SUCCESS;

    nError = B_Os_Init();
    CHECK_NEXUS_ERROR_GOTO("Failed to initialize os library", ret, nError, err_os);

    g_System.Lock = B_Mutex_Create(NULL);
    CHECK_PTR_GOTO("Failed to create mutex", g_System.Lock, ret, eRet_ExternalError, err_mutex);

    g_System.Scheduler = B_Scheduler_Create(NULL);
    CHECK_PTR_GOTO("failed to create scheduler", g_System.Scheduler, ret, eRet_ExternalError, err_scheduler);

    /* run scheduler with thread */
    {
        B_ThreadSettings settings;

        B_Thread_GetDefaultSettings(&settings);
        settings.terminate = false;

        g_System.SchedulerThread = B_Thread_Create("b_event", RunScheduler, g_System.Scheduler, &settings);
        CHECK_PTR_GOTO("failed to create scheduler thread", g_System.SchedulerThread, ret, eRet_ExternalError, err_thread);

        /* yield so sheduler can get up and running */
        B_Thread_Sleep(1);
    }

	{
		g_System.InfoThread=B_Thread_Create("InfoThread", InfoThread, NULL, NULL);
	    CHECK_PTR_GOTO("failed to create InfoThread", g_System.InfoThread, ret, eRet_ExternalError, err_info_thread);

	}

	{
		g_System.HotPlugThread=B_Thread_Create("HotPlugThread", HotPlugThread, NULL, NULL);
	    CHECK_PTR_GOTO("failed to create HotPlugThread", g_System.HotPlugThread, ret, eRet_ExternalError, err_hotplug_thread);
	}
	{
		g_System.CostTimeThread=B_Thread_Create("CostTimeThread", CostTimeThread, NULL, NULL);
	    CHECK_PTR_GOTO("failed to create CostTimeThread", g_System.CostTimeThread, ret, eRet_ExternalError, err_costtime_thread);
	}
B_Thread_Create("CostTimeThread", RecMonThread, NULL, NULL);



    goto done;
err_costtime_thread:
    B_Thread_Destroy(g_System.HotPlugThread);
err_hotplug_thread:
    B_Thread_Destroy(g_System.InfoThread);
err_info_thread:
    B_Thread_Destroy(g_System.SchedulerThread);
err_thread:
    B_Scheduler_Stop(g_System.Scheduler);
    B_Scheduler_Destroy(g_System.Scheduler);
err_scheduler:
    B_Mutex_Destroy(g_System.Lock);
err_mutex:
    B_Os_Uninit();
err_os:
done:
    return ret;
}

void uninitializeOsLib()
{
    if (g_System.Scheduler)
    {
        B_Scheduler_Stop(g_System.Scheduler);
        B_Scheduler_Destroy(g_System.Scheduler);
    }

    if (g_System.SchedulerThread)
    {
        B_Thread_Destroy(g_System.SchedulerThread);
    }

	if(g_System.InfoThread)
	{
        B_Thread_Destroy(g_System.InfoThread);
	}

	if(g_System.HotPlugThread)
	{
        B_Thread_Destroy(g_System.HotPlugThread);
	}
	if(g_System.CostTimeThread)
	{
        B_Thread_Destroy(g_System.CostTimeThread);
	}


    if (g_System.Lock)
    {
        B_Mutex_Destroy(g_System.Lock);
    }

    B_Os_Uninit();
}








eRet InitializeAtlas()
{
    eRet ret = eRet_Ok;

    ret = initializeNexus();
    CHECK_ERROR_GOTO("Nexus failed to initialize", ret, err_initNexus);

    BDBG_MSG(("*** initialize os lib"));
    ret = initializeOsLib();
    CHECK_ERROR_GOTO("OS lib failed to initialize", ret, err_initOsLib);

    goto done;

err_initOsLib:
    uninitializeOsLib();
err_initNexus:
    uninitializeNexus();
done:
    return ret;
}


void uninitializeAtlas()
{

	UninitializePlayback();
	CloseMenuMain();
	CloseChannelSearchMenu();
	CloseMenuInfo();
	CloseChannelEditMenu();
	CloseDateTimeSetMenu();
	CloseDescramblingMenu();
    CloseDisplaySettingsMenu();
	CloseMenuFileBrowser();
	CloseGeneralSettingsMenu();
	CloseNetworkConfigMenu();
	CloseMenuPlayback();
	CloseMenuRecord();
	CloseSystemInfoMenu();
	CloseTCPIPConfigurationMenu();
	CloseVideoChannelListMenu();
	CloseMenuVolume();
	CloseMenuRecordSettings();

	CloseBackGrounScreen();




	//
	//
	CloseSimpleAudioDecode();
	StopSimpleVideoDecode();
    CloseVideoWindow();
	CloseSimpleVideoDecode();


	UninitializeHdmi();






	RecordClose();







	CloseIrRemote();

	bwidget_engine_close(g_System.WidgetEngine);
	g_System.WidgetEngine=NULL;

	UninitGraphics();
	CloseDisplay();

    CloseStc();

	CloseDib3000();

    uninitializeOsLib();
    uninitializeNexus();

/* added by zh start */
FreeMemTransmitter ();
/* added by zh end */

}











void idle_callback(bwidget_engine_t engine, void * context, int param)
{

    BSTD_UNUSED(param);
    BSTD_UNUSED(engine);
	BSTD_UNUSED(context);

	if(g_Status.ExitApp==1)
	{
        /* signal main loop to terminate */
        bwidget_terminate(g_System.WidgetEngine);
    }

    /* controller will handle idle loop processing */
	//printf("i=%d\n",i++);

}





eRet InitializeSystem(void)
{
    eRet                    ret                    = eRet_Ok;
	NEXUS_VideoFormat tmp_v_format = NEXUS_VideoFormat_e1080p;


    /* open display resource */
    ret = OpenDisplay(0);
    CHECK_ERROR_GOTO("display0 failed to open", ret, error);
	tmp_v_format=ConvertVideoFormatToBroadcom(0,g_Config.HdmiSettings.Format);
    SetVideoFormat(0,tmp_v_format,0); /* default might be changed at end of function */

	ret = OpenDisplay(1);
    CHECK_ERROR_GOTO("display1 failed to open", ret, error);
	tmp_v_format=ConvertVideoFormatToBroadcom(1,g_Config.CvbsSettings.Format);
    SetVideoFormat(1,tmp_v_format,0); /* default might be changed at end of function */


    /* open graphics resource */
    {
        MRect RectGraphics_HD;
        MRect RectGraphics_SD;

        RectGraphics_HD = GetGraphicsGeometry(0);
		RectGraphics_SD = GetGraphicsGeometry(1);
		ret = InitializeGraphics (960, 540, RectGraphics_HD.width (), RectGraphics_HD.height (),
																	RectGraphics_SD.width (), RectGraphics_SD.height ());
    }


    /* open bwidgets */
    {
        bwidget_engine_settings settings;
        settings.idle    = idle_callback;
        settings.context = NULL;
        settings.param   = 0;
        g_System.WidgetEngine = bwidget_engine_open(g_System.BwinEngine, &settings);
        CHECK_PTR_GOTO("Unable to open widget engine", g_System.WidgetEngine, ret, eRet_ExternalError, error);
    }


	 /* open ir remote if reserved for us */

	 OpenIrRemote();

     /* open STC channel for a/v sync */
	 ret=OpenStc();
	 CHECK_ERROR_GOTO("stc failed to open", ret, error);

	 ret = OpenSimpleVideoDecode();
     CHECK_ERROR_GOTO("video decode failed to open", ret, error);

	 /* connect decoder to display */

     ret = OpenHDVideoWindow();
     CHECK_ERROR_GOTO("video window failed to open", ret, error);


	 ret =AddHDVideoWindow();
     CHECK_ERROR_GOTO("unable to add video window to decoder", ret, error);


     ret = OpenSDVideoWindow();
     CHECK_ERROR_GOTO("video window failed to open", ret, error);


	 ret =AddSDVideoWindow();
     CHECK_ERROR_GOTO("unable to add video window to decoder", ret, error);






        /* add/setup HDMI output if in resource list */
        {
            NEXUS_Error              nError    = NEXUS_SUCCESS;
            NEXUS_HdmiOutputSettings settings;

            SetWidgetEngine();

            /* connect output to display */
            ret = AddHdmiOutput();
            CHECK_ERROR_GOTO("Error adding HDMI output", ret, error);

            NEXUS_HdmiOutput_GetSettings(g_System.HdmiHandle, &settings);
            settings.autoColorSpace = false;
            settings.colorSpace = NEXUS_ColorSpace_eRgb; /* or NEXUS_ColorSpace_eYCbCr444 */
            nError = NEXUS_HdmiOutput_SetSettings(g_System.HdmiHandle, &settings);
            CHECK_NEXUS_ERROR_GOTO("error setting hdmi output settings", ret, nError, error);
        }
        AddCvbsOutput();




		OpenSimpleAudioDecode();
		SetKwtVolume();

		InitializePlayback();



		 tmp_v_format=ConvertVideoFormatToBroadcom(0,g_Config.HdmiSettings.Format);
         SetVideoFormat(0,tmp_v_format,1); /* default might be changed at end of function */








    CreatBackGroundScreen("CAtlas::_pBackgroundScreen", NULL, g_System.WinFrameBuffer);
   // MenuMain();
   MenuInfoInit();




return eRet_Ok;
error:
    BDBG_ASSERT(false);
    return ret;
}

void U322IP (char *IP, unsigned int u_ip)
{
	unsigned char temp[4] = {0};
	int i = 0;

	for (i = 3; i >= 0; i--)
	{
		temp[i] = u_ip % 256;
		u_ip = u_ip / 256;
	}
	sprintf (IP, "%d.%d.%d.%d", temp[0], temp[1], temp[2], temp[3]);
}



eRet InitializeUart(void)
{
	unsigned int num=0;
	NEXUS_Platform_WriteRegister(0x00408500, 0x00111000);
	NEXUS_Platform_WriteRegister(0x00408504, 0x22334340);
	UART_Init(UART0);
	UART_SetParams(UART0,8,1,0,9600);
	//UART_Write(UART0,"Hello World\n",strlen("Hello World\n"),&num,100);
	//printf("num to write=%d\n",strlen("Hello World\n"));
	//printf("num         =%d\n",num);

	NEXUS_GpioSettings gpioSettings;

	NEXUS_Gpio_GetDefaultSettings(NEXUS_GpioType_eAonStandard, &gpioSettings);
    gpioSettings.mode = NEXUS_GpioMode_eOutputPushPull;
    gpioSettings.value = NEXUS_GpioValue_eLow;
    g_System.en_485 = NEXUS_Gpio_Open(NEXUS_GpioType_eAonStandard, 5, &gpioSettings);

	gpioSettings.value = NEXUS_GpioValue_eLow;
	NEXUS_Gpio_SetSettings(g_System.en_485,&gpioSettings);

}








eRet ConfigTool_Init(void);





int main ()
{
    eRet ret = eRet_Ok;

	InitializeParameters();
	LoadConfigFromFlash();

	char ip[24], mask[24], gateway[24];
	memset (ip, 0, 24);
	memset (mask, 0, 24);
	memset (gateway, 0, 24);
	U322IP (ip, g_Config.NetWorkSettings.IPAddr);
	U322IP (mask, g_Config.NetWorkSettings.NetMask);
	U322IP (gateway, g_Config.NetWorkSettings.GateWay);
	printf ("new ip = %s\n", ip);
	printf ("new mask = %s\n", mask);
	printf ("new gateway = %s\n", gateway);
	TCPIP_SetConfig (0, ip, mask, gateway);

	LoadChannelFromFlash();
    signal(SIGINT, CleanExit);//当有SIGINT信号时，如Ctrl+C，进入cleanExit函数
    ret = InitializeAtlas();
    CHECK_ERROR_GOTO("atlas initialization failed", ret, error);
    {
		InitializeAudioDac();
		InitializeComposite();
		InitializeHdmi();
		InitializeRecord();
		InitializeUart();

		//LoadChannel();

		InitializeSystem();
		InitializeParseBand();
		ConfigTool_Init();

		TransmitterInit ();
		StartTransmitter ();


		/* added by zh start */
		B_Thread_Create("transmitter", TransRebootThread, NULL, NULL);
		/* added by zh end */
		//sleep(3);
		//StartPlayBack("/mnt/usb/atlas/videos/hdmi.ts");
		bwidget_run(g_System.WidgetEngine);
		signal(SIGINT, SIG_DFL);
    }

    uninitializeAtlas();

    return 0;
error:
    return -1;

}








