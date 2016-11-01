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
#include "global.h"
#include "playback.h"
#include "channel.h"
#include "menu_circlelabel.h"

static bwidget_t l_PlaybackBackGround;
static bwidget_t l_PlaybackStatus;
static bwidget_t l_PlaybackPoision;
static bwidget_t l_PlaybackFileName;

static unsigned int m_PlaybackStatus=0;
static signed int m_Speed[12]={-32,-16,-8,-4,-2,-1,1,2,4,8,16,32};
static unsigned int m_SpeedSelect=6;

void MenuPlaybackShow(bool show);

void MenuPlaybackInit(void)
{
	l_PlaybackBackGround = CreatListLabel ("MenuMain_BackGround", L_BackGround, MRect (50,70,830,815), g_System.Font17,NULL);
	l_PlaybackStatus=CreatLabel ("l_PlaybackStatus",l_PlaybackBackGround, MRect(80,378,150,30),g_System.big_menu,NULL);
	l_PlaybackPoision=CreatLabel ("l_PlaybackPoision",l_PlaybackBackGround, MRect(380,378,150,30),g_System.big_menu,NULL);
	l_PlaybackFileName=CreatLabel ("l_PlaybackFileName",l_PlaybackBackGround, MRect(660,378,150,30),g_System.big_menu,NULL);

	SetLabelBackGroundColor(l_PlaybackStatus,  0x85223284);
	SetLabelBackGroundColor(l_PlaybackPoision, 0x85223284);
	SetLabelBackGroundColor(l_PlaybackFileName,0x85223284);

	SetLabelText(l_PlaybackStatus,  "playing");
	SetLabelText(l_PlaybackPoision, "0/0");
	SetLabelText(l_PlaybackFileName,"video.ts");
    MenuPlaybackShow(false);
}

void	MenuPlaybackInitPar(void)
{
	m_PlaybackStatus=0;
	m_SpeedSelect=6;
}

void MenuPlaybackShow(bool show)
{
    bwidget_show(l_PlaybackBackGround,show);
    bwidget_show(l_PlaybackStatus,    show);
    bwidget_show(l_PlaybackPoision,   show);
    bwidget_show(l_PlaybackFileName,  show);
}


int MenuPlaybackKeyDown(unsigned int  key_code)
{
	int i=0;

	if(key_code==eKey_DownArrow)
	{
	}
	else if(key_code==eKey_UpArrow)
	{
	}
	else if(key_code==eKey_RightArrow)
	{
      PlayPlaySeek(1);
	}
	else if(key_code==eKey_LeftArrow)
	{
      PlayPlaySeek(0);
	}
	else if(key_code==eKey_Select)
	{
	}
	else if(key_code==eKey_Menu)
	{
	}
	else if((key_code>=eKey_0)&&(key_code<=eKey_9))
	{
	}
	else if(key_code==eKey_Exit)
	{
		MenuPlaybackShow(false);
		MenuFileBrowserShow(true);
		CurrentMenu=eMenu_FileBrowser;
        StopPlayback();
        StartLiveChannel(g_Channel.CurrentChannel);
		g_Status.PlaybackStarted=0;
	}
	else if(key_code==eKey_Backspace)
	{
	}
	else if(key_code==eKey_Pause)
	{
       PausePlayback();
	   m_PlaybackStatus=1;
	}
	else if(key_code==eKey_Play)
	{
       PlayPlayback();
	   m_PlaybackStatus=0;
	}
	else if(key_code==eKey_FastForward)
	{
		if(m_SpeedSelect<11)
		{
			m_SpeedSelect++;
		}
		if(m_SpeedSelect==6)
		{
			m_PlaybackStatus=0;
		}
		else
		{
			m_PlaybackStatus=2;
		}
        TrickPlaybackMode(m_Speed[m_SpeedSelect]);
	}
	else if(key_code==eKey_Rewind)
	{
		if(m_SpeedSelect>0)
		{
			m_SpeedSelect--;
		}
		if(m_SpeedSelect==6)
		{
			m_PlaybackStatus=0;
		}
		else
		{
			m_PlaybackStatus=2;
		}
        TrickPlaybackMode(m_Speed[m_SpeedSelect]);
	}
}





void MenuPlaybackUpdate(const char *file)
{
	unsigned int cur=0;
	unsigned int total=0;
	static char process[100]={0};
	static char fast[100]={0};
	if(m_PlaybackStatus==0)
	{
		SetLabelText(l_PlaybackStatus,  "playing");
	}
	else if(m_PlaybackStatus==1)
	{
		SetLabelText(l_PlaybackStatus,  "pause");
	}
	else if(m_PlaybackStatus==2)
	{
		if(m_SpeedSelect<6)
		{
			sprintf(fast,"%d  Rewind",m_Speed[m_SpeedSelect]);
			SetLabelText(l_PlaybackStatus,  fast);
		}
		else if(m_SpeedSelect>6)
		{
		    sprintf(fast,"%d  FastForward",m_Speed[m_SpeedSelect]);
			SetLabelText(l_PlaybackStatus,  fast);
		}
	}
	memset(process,0,sizeof(process));
	GetPlaybackStatus(&cur,&total);
    sprintf(process,"%d/%d",cur,total);
	SetLabelText(l_PlaybackPoision, process);
	if(file != NULL)
	{
	    SetLabelText(l_PlaybackFileName,file);
	}
}

void CloseMenuPlayback(void)
{
	bwidget_destroy(l_PlaybackBackGround);
	bwidget_destroy(l_PlaybackStatus);
	bwidget_destroy(l_PlaybackPoision);
	bwidget_destroy(l_PlaybackFileName);
}

