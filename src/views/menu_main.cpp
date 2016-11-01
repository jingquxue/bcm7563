#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "label.h"
#include "menu_channelsearch.h"
#include "menu_info.h"
#include "menu_fileBrowser.h"
#include "button.h"
#include "global.h"
#include "graphics.h"
#include "menu_circlelabel.h"
#include "menu_displaysettings.h"
#include "menu_channeledit.h"
#include "menu_generalsettings.h"


//#define bg_c 0xaa0000A0
//#define bg_f 0xa56b8cf0

void MenuMainShow (bool show);

bwidget_t b_mainmenu[7];
bwidget_t l_mainmenu_bg;
bwidget_t test_main[5];
static unsigned char menumain_focuse = 0;

void MenuMain (void)
{
	l_mainmenu_bg = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50, 50, 830, 815), g_System.Font17, NULL);

	for (int i = 0; i < 5; i++)
	{
		test_main[i] = CreatCirclepicture ("test_main", l_mainmenu_bg,
															MRect (200, 140 + i * 40, 30, 30), g_System.Font17, NULL);
	}

	for (int i = 0; i < 5; i++)
	{
		b_mainmenu[i] = CreatButton ("MenuMain_list", l_mainmenu_bg, MRect (260, 140 + i * 40, 350, 30),
																									g_System.Font17, NULL);
	}
	b_mainmenu[5] = CreatLabel("MenuMain_list", l_mainmenu_bg, MRect (70, 10, 120, 30), g_System.big_menu, NULL);
	b_mainmenu[6] = CreatLabel ("MenuMain_list", l_mainmenu_bg, MRect (95, 40, 120, 30), g_System.small_menu, NULL);

	bwidget_set_focus (b_mainmenu[menumain_focuse]);

	SetButtonText (b_mainmenu[0], "Channel Search");
	SetButtonText (b_mainmenu[1], "PlayBack");
	SetButtonText (b_mainmenu[2], "Display Settings");
	SetButtonText (b_mainmenu[3], "Channel Edit");
	SetButtonText (b_mainmenu[4], "General Settings");

	SetLabelText (b_mainmenu[5], "MENU");
	SetLabelText (b_mainmenu[6], "Menu List");

	MenuMainShow (false);

	SetLabelBackGroundColor (b_mainmenu[5], 0x85223284);
	SetLabelBackGroundColor (b_mainmenu[6], 0x85223284);
}

int MenuMainKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		menumain_focuse++;
		if (menumain_focuse == 5)
		{
			menumain_focuse = 0;
		}

		bwidget_set_focus (b_mainmenu[menumain_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (i == menumain_focuse)
			{
				bwidget_show (test_main[i], true);
				continue;
			}
			bwidget_show (test_main[i], false);
		}
	}
	else if (key_code == eKey_UpArrow)
	{
		if (menumain_focuse == 0)
		{
			menumain_focuse = 4;
		}
		else
		{
			menumain_focuse--;
		}
		bwidget_set_focus (b_mainmenu[menumain_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (i == menumain_focuse)
			{
				bwidget_show (test_main[i], true);
				continue;
			}
			bwidget_show (test_main[i], false);
		}
	}
	else if (key_code == eKey_Select)
	{
		if (menumain_focuse == 0)
		{
			MenuMainShow (false);
			CurrentMenu = eMenu_ChannelSearch;
			ChannelSearchShow (true);
		}
		if (menumain_focuse == 1)
		{
			MenuMainShow (false);
			CurrentMenu = eMenu_FileBrowser;
			MenuFileBrowserShow (true);
		}
		if (menumain_focuse == 2)
		{
			MenuMainShow (false);
			CurrentMenu = eMenu_DisplaySettings;
			DisplaySettingsShow (true);
		}
		if (menumain_focuse == 3)
		{
			MenuMainShow (false);
			CurrentMenu = eMenu_ChannelEdit;
			ChannelEditShow (true);
		}
		if (menumain_focuse == 4)
		{
			MenuMainShow (false);
			CurrentMenu = eMenu_GeneralSettings;
			GeneralSettingsShow (true);
		}
	}
	else if (key_code == eKey_Menu)
	{

	}
	else if (key_code == eKey_Exit)
	{
		MenuMainShow (false);
		MenuInfoShow (true);
		CurrentMenu = eMenu_Info;
	}

	for (int i = 0; i < 5; i++)
	{
		SetButtonBackGroundColor (b_mainmenu[i], UNFOCUS_COLOR);
		SetButtonBackGroundColor (b_mainmenu[menumain_focuse], FOCUS_COLOR);
	}
	return 0;
}

void MenuMainShow (bool show)
{
	int i = 0;
	for (i = 0; i < 7; i++)
	{
		bwidget_show (b_mainmenu[i], show);
	}
	bwidget_show (l_mainmenu_bg, show);
	if (show == true)
	{
		bwidget_set_focus (b_mainmenu[menumain_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (i == menumain_focuse)
			{
				bwidget_show (test_main[i], true);
				continue;
			}
			bwidget_show (test_main[i], false);
		}
		for (int i = 0; i < 5; i++)
		{
			SetButtonBackGroundColor (b_mainmenu[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (b_mainmenu[menumain_focuse], FOCUS_COLOR);
		}
	}
}

void CloseMenuMain (void)
{
	bwidget_destroy (l_mainmenu_bg);
	for (int i = 0; i < 7; i++)
	{
		bwidget_destroy (b_mainmenu[i]);
	}
	for (int i = 0; i < 5; i++)
	{
		bwidget_destroy (test_main[i]);
	}
}

