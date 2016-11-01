#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "label.h"
#include "button.h"
#include "menu_main.h"
#include "dib3000.h"
#include "channellist.h"
#include "menu_info.h"
#include "graphics.h"
#include "menu_displaysettings.h"
#include "global.h"
#include "menu_circlelabel.h"
#include "menu_videochannellist.h"
#include "menu_descramblingconfig.h"
#include "menu_duplexaudio.h"
#include "menu_record.h"
#include "menu_recordSettings.h"

void ChannelEditShow (bool show);

bwidget_t c_bg;
bwidget_t csm[5];
unsigned char cPlayBack_focuse = 0;
//bwin_image_t picture_test_channel_dit[4];
bwidget_t test_channel_edit[3];
/*
eRet loadImage_button_list_channel_edit (const char *filename, bwin_image_render_mode renderMode,bwidget_t widget, int i)
{
	eRet ret = eRet_Ok;
	bbutton_settings buttonSettings;
	BDBG_ASSERT(NULL != filename);
	if (NULL != picture_test_channel_dit[i])
	{
		bwin_image_close (picture_test_channel_dit[i]);
	}
	picture_test_channel_dit[i] = bwin_image_load (getWinEngine (), filename);
	bbutton_get (widget, &buttonSettings);
	buttonSettings.label.image = picture_test_channel_dit[i];
	buttonSettings.label.render_mode = renderMode;
	bbutton_set (widget, &buttonSettings);
	bwin_repaint (bwidget_win (widget), NULL);
	return ret;
}*/

void ChannelEditInit ()
{
	c_bg = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50, 50, 830, 815), g_System.Font17, NULL);

	for (int i = 0; i < 3; i++)
	{
		test_channel_edit[i] = CreatCirclepicture ("test_channel_edit", c_bg,
																MRect (200, 160 + i * 40, 30, 30), g_System.Font17, NULL);
		//loadImage_button_list_channel_edit ("images/p1.png", bwin_image_render_mode_tile, test_channel_edit[i], i);
	}
	csm[0] = CreatButton ("video channel list", c_bg, MRect (260, 160, 350, 30), g_System.Font17, NULL);
	csm[1] = CreatButton ("descrambling config", c_bg, MRect (260, 200, 350, 30), g_System.Font17, NULL);
	csm[2] = CreatButton ("duplex audio", c_bg, MRect (260, 240, 350, 30), g_System.Font17, NULL);
	//csm[3] = CreatButton ("record config", c_bg, MRect (260,280,350,30), g_System.Font17, NULL);
	csm[3] = CreatLabel("csm", c_bg, MRect (70, 10, 120, 30), g_System.big_menu, NULL);
	csm[4] = CreatLabel ("csm", c_bg, MRect (95, 40, 180, 30), g_System.small_menu, NULL);

	ChannelEditShow (false);
	SetButtonText (csm[0], "Video Channel List");
	SetButtonText (csm[1], "Descrambling Config");
	SetButtonText (csm[2], "Duplex Audio");
	//SetButtonText (csm[3], "Record Config");
	SetLabelText (csm[3], "MENU");
	SetLabelText (csm[4], "Channel Edit");

	for (int i = 0; i < 3; i++)
	{
		SetButtonBackGroundColor (csm[i], BACKGROUND_COLOR);
	}
	SetLabelBackGroundColor (csm[3], 0x85223284);
	SetLabelBackGroundColor (csm[4], 0x85223284);
}

void ChannelEditShow (bool show)
{
	int ret = 0;
	bwidget_show (c_bg, show);
	for (int i = 0; i < 5; i++)
	{
		bwidget_show (csm[i], show);
	}
	if (show == true)
	{
		ret = bwidget_set_focus (csm[cPlayBack_focuse]);
		for (int i = 0; i < 3; i++)
		{
			if (i == cPlayBack_focuse)
			{
				bwidget_show (test_channel_edit[i], true);
				continue;
			}
			bwidget_show (test_channel_edit[i], false);
		}
		for (int i = 0; i < 3; i++)
		{
			SetButtonBackGroundColor (csm[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (csm[cPlayBack_focuse], FOCUS_COLOR);
		}
		printf ("ret = %d\n", ret);
	}
}

int MenuChannelEditKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		cPlayBack_focuse++;
		if (cPlayBack_focuse == 3)
		{
			cPlayBack_focuse = 0;
		}
		bwidget_set_focus (csm[cPlayBack_focuse]);
		for (int i = 0; i < 3; i++)
		{
			if (i == cPlayBack_focuse)
			{
				bwidget_show (test_channel_edit[i], true);
				continue;
			}
			bwidget_show (test_channel_edit[i], false);
		}
	}
	else if (key_code == eKey_UpArrow)
	{
		if (cPlayBack_focuse == 0)
		{
			cPlayBack_focuse = 2;
		}
		else
		{
			cPlayBack_focuse--;
		}
		bwidget_set_focus (csm[cPlayBack_focuse]);
		for (int i = 0; i < 3; i++)
		{
			if (i == cPlayBack_focuse)
			{
				bwidget_show (test_channel_edit[i], true);
				continue;
			}
			bwidget_show (test_channel_edit[i], false);
		}
	}
	else if (key_code == eKey_Select)
	{
		if (cPlayBack_focuse == 0)
		{
			ChannelEditShow (false);
			CurrentMenu = eMenu_VideoChannelList;
			VideoChannelListShow (true);
		}
		else if (cPlayBack_focuse == 1)
		{
			ChannelEditShow (false);
			CurrentMenu = eMenu_DescramblingConfig;
			DescramblingShow (true);
		}
		/*else if (cPlayBack_focuse == 2)
		{
			ChannelEditShow (false);
			CurrentMenu = eMenu_DuplexAudio;
			DuplexAudioShow (true);
		}*/
		/*else if (cPlayBack_focuse == 3)
		{
			ChannelEditShow (false);
			CurrentMenu = eMenu_RecordConfig;
			MenuRecordSettingsShow (true);
		}*/
	}
	else if (key_code == eKey_Menu)
	{
		ChannelEditShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{

	}
	else if (key_code == eKey_Exit)
	{
		ChannelEditShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	for (int i = 0; i < 3; i++)
	{
		SetButtonBackGroundColor (csm[i], UNFOCUS_COLOR);
		SetButtonBackGroundColor (csm[cPlayBack_focuse], FOCUS_COLOR);
	}
	return 0;
}

void CloseChannelEditMenu ()
{
	bwidget_destroy (c_bg);

	for (int i = 0; i < 3; i++)
	{
		bwidget_destroy (test_channel_edit[i]);
		//bwin_image_close (picture_test_channel_dit[i]);
	}
	for (int i = 0; i < 5; i++)
	{
		bwidget_destroy (csm[i]);
	}
}
