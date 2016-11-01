#include "bwidgets.h"


#include "main.h"
#include "label.h"
#include "button.h"
#include "menu_main.h"
#include "menu_info.h"
#include "menu_generalsettings.h"
#include "menu_networkconfig.h"
#include "menu_tcpipconfiguration.h"
#include "menu_datetimeset.h"
#include "menu_systeminfo.h"
#include "graphics.h"
#include "menu_displaysettings.h"
#include "global.h"
#include "menu_circlelabel.h"
#include "menu_recordSettings.h"

void GeneralSettingsShow (bool show);
static bwidget_t gs;
static bwidget_t generalsettings[7];
static unsigned char gGeneralSettings_focuse = 0;
//static bwin_image_t picture_test[4];
static bwidget_t test_gs[5];
/*
eRet loadImage_button_list_gs (const char *filename, bwin_image_render_mode renderMode,bwidget_t widget, int i)
{
	eRet ret = eRet_Ok;
	bbutton_settings buttonSettings;
	BDBG_ASSERT (NULL != filename);
	if (NULL != picture_test[i])
	{
		bwin_image_close (picture_test[i]);
	}
	picture_test[i] = bwin_image_load (getWinEngine (), filename);
	bbutton_get (widget, &buttonSettings);
	buttonSettings.label.image = picture_test[i];
	buttonSettings.label.render_mode = renderMode;
	bbutton_set (widget, &buttonSettings);
	bwin_repaint (bwidget_win (widget), NULL);

	return ret;
}*/

void GeneralSettingsInit ()
{
	gs = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50,50,830,815), g_System.Font17,NULL);

	for (int i = 0; i < 5; i++)
	{
		test_gs[i] = CreatCirclepicture ("test_gs", gs, MRect (200, 160 + i * 40, 30, 30), g_System.Font17, NULL);
		//loadImage_button_list_gs ("images/p1.png", bwin_image_render_mode_tile, test_gs[i], i);
	}

	for (int i = 0; i < 5; i++)
	{
		generalsettings[i] = CreatButton ("generalsettings", gs, MRect(260,160+i * 40,350,30), g_System.Font17, NULL);
	}
	generalsettings[5]  =CreatLabel("generalsettings",   gs,
																	MRect(70,10,120,30), g_System.big_menu, NULL);
	generalsettings[6]  =CreatLabel("generalsettings",   gs,
																	MRect(95,40,200,30), g_System.small_menu, NULL);
	GeneralSettingsShow (false);
	SetLabelText (generalsettings[5], "MENU");
	SetLabelText (generalsettings[6], "GeneralSettings");
	SetButtonText (generalsettings[0], "TCP/IP Configuration");
	SetButtonText (generalsettings[1], "Network Config");
	SetButtonText (generalsettings[2], "Date&Time Set");
	SetButtonText (generalsettings[3], "System Info");
	SetButtonText (generalsettings[4], "Record Config");
	for (int i = 0; i < 5; i++)
	{
		SetButtonBackGroundColor (generalsettings[i], BACKGROUND_COLOR);
	}
	SetLabelBackGroundColor (generalsettings[5], 0x85223284);
	SetLabelBackGroundColor (generalsettings[6], 0x85223284);
}

void GeneralSettingsShow (bool show)
{
	int ret = 0;
	bwidget_show (gs, show);
	for (int i = 0; i < 5; i++)
	{
		bwidget_show (generalsettings[i], show);
	}
	if (show == true)
	{
		ret = bwidget_set_focus (generalsettings[gGeneralSettings_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (i == gGeneralSettings_focuse)
			{
				bwidget_show (test_gs[i], true);
				continue;
			}
			bwidget_show (test_gs[i], false);
		}
		for (int i = 0; i < 5; i++)
		{
			SetButtonBackGroundColor (generalsettings[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (generalsettings[gGeneralSettings_focuse], FOCUS_COLOR);
		}
	}
}

int MenuGeneralSettingsKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		gGeneralSettings_focuse++;
		if (gGeneralSettings_focuse == 5)
		{
			gGeneralSettings_focuse = 0;
		}

		bwidget_set_focus (generalsettings[gGeneralSettings_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (i == gGeneralSettings_focuse)
			{
				bwidget_show (test_gs[i], true);
				continue;
			}
			bwidget_show (test_gs[i], false);
		}
	}
	else if (key_code == eKey_UpArrow)
	{
		if (gGeneralSettings_focuse == 0)
		{
			gGeneralSettings_focuse = 4;
		}
		else
		{
			gGeneralSettings_focuse--;
		}
		bwidget_set_focus (generalsettings[gGeneralSettings_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (i == gGeneralSettings_focuse)
			{
				bwidget_show (test_gs[i], true);
				continue;
			}
			bwidget_show (test_gs[i], false);
		}
	}
	else if (key_code == eKey_Select)
	{
		if (gGeneralSettings_focuse == 0)
		{
			GeneralSettingsShow (false);
			CurrentMenu = eMenu_NetworkConfg;
			NetworkConfigShow (true);
		}
		else if (gGeneralSettings_focuse == 1)
		{
			GeneralSettingsShow (false);
			CurrentMenu = eMenu_TCPIPConfiguration;
			TCPIPConfigurationShow (true);
		}
		else if (gGeneralSettings_focuse == 2)
		{
			GeneralSettingsShow (false);
			CurrentMenu = eMenu_DateTimeSet;
			DateTimeSetShow (true);
		}
		else if (gGeneralSettings_focuse == 3)
		{
			GeneralSettingsShow (false);
			CurrentMenu = eMenu_SystemInfo;
			SystemInfoShow (true);
		}
		else if (gGeneralSettings_focuse == 4)
		{
			GeneralSettingsShow (false);
			CurrentMenu = eMenu_RecordConfig;
			MenuRecordSettingsShow (true);
		}
	}
	else if (key_code == eKey_Menu)
	{
		GeneralSettingsShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{

	}
	else if (key_code == eKey_Exit)
	{
		GeneralSettingsShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	for (int i = 0; i < 5; i++)
	{
		SetButtonBackGroundColor (generalsettings[i], UNFOCUS_COLOR);
		SetButtonBackGroundColor (generalsettings[gGeneralSettings_focuse], FOCUS_COLOR);
	}

	return 0;
}

void CloseGeneralSettingsMenu ()
{
	bwidget_destroy (gs);
	for (int i = 0; i < 5; i++)
	{
		bwidget_destroy (test_gs[i]);
		//bwin_image_close (picture_test[i]);
	}

	for (int i = 0; i < 7; i++)
	{
		bwidget_destroy (generalsettings[i]);
	}


}
