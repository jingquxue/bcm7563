#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "label.h"
#include "button.h"
#include "menu_main.h"
#include "dib3000.h"
#include "channellist.h"
#include "menu_info.h"
#include "menu_systeminfo.h"
#include "graphics.h"
#include "menu_generalsettings.h"
#include "menu_displaysettings.h"
#include "global.h"
#include "menu_circlelabel.h"

void SystemInfoShow (bool show);
bwidget_t si_bg;
bwidget_t systeminfo_list[6];
unsigned char systeminfo_focuse = 0;


char build[100]={0};


void SystemInfoInit ()
{
	si_bg = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50,50,830,815), g_System.Font17,NULL);

	for (int i = 0; i < 4; i++)
	{
		systeminfo_list[i] = CreatLabel ("systeminfo_list", si_bg, MRect (280, 140 + i * 50, 330, 30), g_System.Font17, NULL);
	}
	systeminfo_list[4]  =CreatLabel("systeminfo_list",   si_bg,
																	MRect(70,10,120,30), g_System.big_menu, NULL);
	systeminfo_list[5]  =CreatLabel("systeminfo_list",   si_bg,
																	MRect(95,40,140,30), g_System.small_menu, NULL);
	SystemInfoShow (false);
	SetLabelText (systeminfo_list[0], "HW Ver:  R780-MB-V1");

	sprintf(build,"BuildDate:%s",BUILD_DATE);


	SetLabelText (systeminfo_list[1], build);
	SetLabelText (systeminfo_list[2], "D330 Ver:  2.2");
	SetLabelText (systeminfo_list[3], "All Running on R780");

	SetLabelText (systeminfo_list[4], "MENU");
	SetLabelText (systeminfo_list[5], "SystemInfo");
	for (int i = 0; i < 4; i++)
	{
		SetLabelBackGroundColor (systeminfo_list[i], 0x85223284);
	}
	SetLabelBackGroundColor (systeminfo_list[4], 0x85223284);
	SetLabelBackGroundColor (systeminfo_list[5], 0x85223284);
}

void SystemInfoShow (bool show)
{
	bwidget_show (si_bg, show);
	for (int i = 0; i < 4; i++)
	{
		bwidget_show (systeminfo_list[i], show);
	}
}

int MenuSystemInfoKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{

	}
	else if (key_code == eKey_UpArrow)
	{

	}
	else if (key_code == eKey_Select)
	{

	}
	else if (key_code == eKey_Menu)
	{
		SystemInfoShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{

	}
	else if (key_code == eKey_Exit)
	{
		SystemInfoShow (false);
		GeneralSettingsShow (true);
		CurrentMenu = eMenu_GeneralSettings;
	}

	return 0;
}

void CloseSystemInfoMenu ()
{
	bwidget_destroy (si_bg);
	for (int i = 0; i < 6; i++)
	{
		bwidget_destroy (systeminfo_list[i]);
	}
}

