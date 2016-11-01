#include <stdlib.h>

#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "label.h"
#include "button.h"
#include "menu_main.h"
#include "dib3000.h"
#include "channellist.h"
#include "menu_main.h"
#include "menu_channelsearch.h"
#include "menu_info.h"
#include "menu_null.h"
#include "menu_record.h"
#include "menu_fileBrowser.h"
#include "menu_playback.h"
#include "global.h"
#include "channel.h"
#include "menu_circlelabel.h"
#include "graphics.h"
#include "menu_displaysettings.h"
#include "menu_channeledit.h"
#include "menu_videochannellist.h"
#include "menu_generalsettings.h"
#include "menu_systeminfo.h"
#include "menu_datetimeset.h"
#include "menu_tcpipconfiguration.h"
#include "menu_networkconfig.h"
#include "menu_descramblingconfig.h"
#include "menu_duplexaudio.h"
#include "menu_record.h"
#include "video_decode.h"
#include "menu_volume.h"
#include "record.h"
#include "menu_recordSettings.h"
#include "transmitter.h"



void MenuInfoUpdate (int type);
void MenuInfoShow (bool show);
bwidget_t l_info_bg;
bwidget_t l_channel;
bwidget_t l_signal1;
bwidget_t l_signal2;

bwidget_t date_time;

bwidget_t l_rec;
bwidget_t l_disk;



bwin_image_t picture_clock;
bwidget_t test_clock;





static char l_channel_content[100] = {0};
eRet loadImage_button_Clock (const char *filename, bwin_image_render_mode renderMode, bwidget_t widget)
{
	eRet ret = eRet_Ok;
	bbutton_settings buttonSettings;

	BDBG_ASSERT (NULL != filename);

	if (NULL != picture_clock)
	{
		bwin_image_close (picture_clock);
	}


	picture_clock = bwin_image_load (getWinEngine(), filename);


	bbutton_get (widget, &buttonSettings);
	buttonSettings.label.image = picture_clock;
	buttonSettings.label.render_mode = renderMode;
	bbutton_set (widget, &buttonSettings);

	bwin_repaint (bwidget_win (widget), NULL);

	return ret;
}
void MenuInfoInit ()
{
	//l_info_bg = CreatLabel ("MenuMain_BackGround", L_BackGround, MRect (100,300,760,200), g_System.Font10,NULL);
	//l_info_bg = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50,50,830,815), g_System.Font17,NULL);背景OK!
	l_info_bg = CreatInfoLabel ("MenuMain_BackGround", L_BackGround, MRect (150,340,630,250), g_System.Font17,NULL);
	//SetLabelBackGroundColor(l_info_bg,0x00000000);

	test_clock = CreatButton ("test_clock", l_info_bg, MRect(95,25,76,101), g_System.Font17, NULL);
	loadImage_button_Clock ("images/clock1.png", bwin_image_render_mode_tile, test_clock);

	l_channel     =CreatLabel ("info_channel",    l_info_bg, MRect(235,17,380,23), g_System.Font17,NULL);
	l_signal1     =CreatLabel ("info_signal1",    l_info_bg, MRect(235,40,380,23), g_System.Font17,NULL);
	l_signal2     =CreatLabel ("info_signal2",    l_info_bg, MRect(235,63,380,23), g_System.Font17,NULL);

	date_time     =CreatLabel ("date_time",    l_info_bg, MRect(235,86,320,23), g_System.Font17,NULL);

	l_rec         =CreatLabel ("rec",    l_info_bg, MRect(235,109,340,23), g_System.Font17,NULL);
	l_disk        =CreatLabel ("disk",    l_info_bg, MRect(235,132,360,23), g_System.Font17,NULL);



	//MenuInfoUpdate();0x85223284
	SetLabelBackGroundColor(l_channel,0x85223284);
	SetLabelBackGroundColor(l_signal1,0x85223284);
	SetLabelBackGroundColor(l_signal2,0x85223284);
	SetLabelBackGroundColor(date_time,0x85223284);
	SetLabelBackGroundColor(l_rec,0x85223284);
	SetLabelBackGroundColor(l_disk,0x85223284);

	strcpy(l_channel_content,"Tuner");

	SetLabelText(l_channel,l_channel_content);


    MenuInfoShow(true);

	MenuMain();
	ChannelSearchInit();
	MenuRecordInit();
	MenuFileBrowser();
	MenuPlaybackInit();
	/* added by zh start */
	DisplaySettingsInit ();
	ChannelEditInit ();
	VideoChannelListInit ();
	GeneralSettingsInit ();
	SystemInfoInit ();
	DateTimeSetInit ();
	TCPIPConfigurationInit ();
	NetworkConfigInit ();
	DescramblingInit ();
    MenuRecordSettingsInit();
	//DuplexAudioInit ();
	MenuVolumeInit();
	/* added by zh end */

}


char tempbuf_time[80]={0};

void DisplayTime ()
{
	    time_t now;
	    struct tm   *timenow;
	    time(&now);
	    timenow = localtime(&now);
	    timenow->tm_year=timenow->tm_year+1900;
	    timenow->tm_mon=timenow->tm_mon+1;
		sprintf(tempbuf_time, "%04d-%02d-%02d-%02d:%02d:%02d", timenow->tm_year, timenow->tm_mon, timenow->tm_mday,
															timenow->tm_hour,timenow->tm_min,timenow->tm_sec);
}

/*
type
0:有频道
1:无频道
2:网络接收
*/



void MenuInfoUpdate(int type)
{
	if(type==0)
	{
		signed short strength,quality,strength1,quality1,strength2,quality2;

    	static char buf[100]={0};
    	static char buf1[100]={0};
    	static char buf2[100]={0};
		static char buf3[100]={0};
		static char buf4[100]={0};
		static char buf5[100]={0};
		if (g_Channel.ChannelList[g_Channel.CurrentChannel].BandWidth == 1500)
		{
    		sprintf(buf,"%d.%d-%dMHz-%0.1fMHz--V:%04x--A:%04x",g_Channel.CurrentChannel,g_Channel.CurrentSubChannel,
						g_Channel.ChannelList[g_Channel.CurrentChannel].Frequency/1000,
						g_Channel.ChannelList[g_Channel.CurrentChannel].BandWidth/1000.0,
						g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].VidPid,
						g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].AudPid);
		}
		else
		{
			sprintf(buf,"%d.%d-%dMHz-%dMHz--V:%04x--A:%04x",g_Channel.CurrentChannel,g_Channel.CurrentSubChannel,
						g_Channel.ChannelList[g_Channel.CurrentChannel].Frequency/1000,
						g_Channel.ChannelList[g_Channel.CurrentChannel].BandWidth/1000,
						g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].VidPid,
						g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].AudPid);
		}
    	SetLabelText(l_channel,buf);

    	dib_show_signal_strength_quality(i2c);

        dib_signal_strength_quality(i2c,&strength,&quality,&strength1,&strength2,&quality1,&quality2);

    	//printf("strength2=%d\n",strength2);

    	sprintf(buf1,"signal_1:%4ddBm     SNR_1:%4d",strength1,quality1);
    	sprintf(buf2,"signal_2:%4ddBm     SNR_2:%4d",strength2,quality2);
		DisplayTime ();
		sprintf(buf3,"Date&Time: %s",tempbuf_time);






    	SetLabelText(l_signal1,buf1);
    	SetLabelText(l_signal2,buf2);
		SetLabelText(date_time,buf3);

		if((g_System.RecFileName[0] != 0) &&(g_System.RecFile != NULL))
		{
			unsigned int file_size=GetRecFileSize();
			unsigned int disk_free_size=GetRecDevFreeSize();
			unsigned int disk_total_size=GetRecDevTotalSize();
			sprintf(buf4,"RecFile:%s--RecSize:%4dMB",g_System.RecFileName+strlen(g_System.RecFileName)-11,file_size/1000);
		    SetLabelText(l_rec,buf4);

			sprintf(buf5,"DiskFree:%4dMB--DiskTotal:%4dMB",disk_free_size/1000,disk_total_size/1000);
		    SetLabelText(l_disk,buf5);


		}
		else
		{
			sprintf(buf4,"%s","RecFile:NONE");
		    SetLabelText(l_rec,buf4);

			sprintf(buf5,"%s","");
		    SetLabelText(l_disk,buf5);
		}


	}
	else if(type==1)
	{
		SetLabelText(l_channel,"No Channel,Please search ofdm");
    	SetLabelText(l_signal1,"sigal_1:--dBm     SNR_1:--d");
    	SetLabelText(l_signal2,"sigal_2:--dBm     SNR_2:--d");
	}


}

void MenuInfoShow(bool show)
{

    bwidget_show(l_info_bg,show);
    bwidget_show(l_channel,show);
    bwidget_show(l_signal1,show);
    bwidget_show(l_signal2,show);

	bwidget_show(date_time,show);
}


int MenuInfoKeyDown(unsigned int  key_code)
{
	int i=0;

	if(key_code==eKey_DownArrow)
	{
		if((g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannelNum>0)&&((g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannelNum-1)>g_Channel.CurrentSubChannel))
		{
			g_Channel.CurrentSubChannel++;
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
			g_Channel.CurrentSubChannel=0;
			if(g_Channel.OverLoop==1)
    		{
    			if(g_Channel.CurrentChannel==MAX_CHANNELS-1)
    			{
                    g_Channel.CurrentChannel=0;
					g_Status.ChannelChanged=1;
	                B_Time_Get(&g_Status.ChannelChangedTime);
    			}
    			else
    			{
    				g_Channel.CurrentChannel++;
					g_Status.ChannelChanged=1;
	                B_Time_Get(&g_Status.ChannelChangedTime);
    			}
    		}
    		else
    		{
    			if(g_Channel.CurrentChannel==g_Channel.ChannelCount-1)
    			{
                    g_Channel.CurrentChannel=0;
					g_Status.ChannelChanged=1;
	                B_Time_Get(&g_Status.ChannelChangedTime);
    			}
    			else
    			{
    				g_Channel.CurrentChannel++;
					g_Status.ChannelChanged=1;
	                B_Time_Get(&g_Status.ChannelChangedTime);
    			}

    		}

		}


		MenuInfoUpdate(0);


	}
	else if(key_code==eKey_UpArrow)
	{

		if(g_Channel.CurrentSubChannel>0)
		{
			g_Channel.CurrentSubChannel--;
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
			if(g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannelNum>0)
			{
				g_Channel.CurrentSubChannel=g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannelNum-1;
			}
			else
			{
				g_Channel.CurrentSubChannel=0;

			}
    		if(g_Channel.OverLoop==1)
    		{
    			if(g_Channel.CurrentChannel==0)
    			{
                    g_Channel.CurrentChannel=MAX_CHANNELS-1;
					g_Status.ChannelChanged=1;
	                B_Time_Get(&g_Status.ChannelChangedTime);
    			}
    			else
    			{
    				g_Channel.CurrentChannel--;
					g_Status.ChannelChanged=1;
	                B_Time_Get(&g_Status.ChannelChangedTime);
    			}
    		}
    		else
    		{
    			if(g_Channel.CurrentChannel==0)
    			{
                    g_Channel.CurrentChannel=g_Channel.ChannelCount-1;
					g_Status.ChannelChanged=1;
	                B_Time_Get(&g_Status.ChannelChangedTime);
    			}
    			else
    			{
    				g_Channel.CurrentChannel--;
					g_Status.ChannelChanged=1;
	                B_Time_Get(&g_Status.ChannelChangedTime);
    			}

    		}
		}
		//g_Status.FirstRun=1;

				MenuInfoUpdate(0);


	}
	else if(key_code==eKey_Select)
	{

	}
	else if(key_code==eKey_Menu)
	{
		MenuInfoShow(false);
		MenuMainShow(true);
		CurrentMenu=eMenu_Main;
	}
	else if((key_code>=eKey_0)&&(key_code<=eKey_9))
	{
	}
	else if(key_code==eKey_Exit)
	{
		MenuInfoShow(false);
		MenuNullShow(true);
		CurrentMenu=eMenu_NULL;
	}
	else if(key_code==eKey_LeftArrow)
	{
		SetVolumeLevel(0);
		MenuVolumeShow(true);
		g_Status.MenuVolumeDisplayed=1;

	}
	else if(key_code==eKey_RightArrow)
	{
		SetVolumeLevel(1);
		MenuVolumeShow(true);
		g_Status.MenuVolumeDisplayed=1;
	}







}


void CloseMenuInfo(void)
{
	bwidget_destroy(l_info_bg);
	bwidget_destroy(l_channel);
	bwidget_destroy(l_signal1);
	bwidget_destroy(l_signal2);
	bwidget_destroy(l_rec);
	bwidget_destroy(l_disk);
	bwin_image_close (picture_clock);
	bwidget_destroy(date_time);
	bwidget_destroy(test_clock);
}








