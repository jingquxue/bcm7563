#include <stdlib.h>
#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "label.h"
#include "button.h"
#include "menu_main.h"
#include "menu_info.h"
#include "menu_generalsettings.h"
#include "graphics.h"
#include "menu_displaysettings.h"
#include "global.h"
#include "menu_circlelabel.h"
#include "rtc.h"

void DateTimeSetShow (bool show);
void ShowDisplayTime ();

bwidget_t dts;
bwidget_t datetimeset_list[5];
bwidget_t datetimeset_label[5];
bwidget_t datetimeset_left[5];
bwidget_t datetimeset_right[5];
bwidget_t datetimeset_circle[5];
bwidget_t datetimeset_title[3];
unsigned char datetimeset_focuse = 0;

char show_tempbuf_time[80] = {0};
time_t now;
struct tm *timenow;
char test_buffer_time[5][5];

void DateTimeSetInit ()
{
	dts = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50,50,830,815), g_System.Font17,NULL);
	for (int i = 0; i < 5; i++)
	{
		datetimeset_circle[i] = CreatCirclepicture ("datetimeset_circle", dts,
																MRect (270, 170 + i * 40, 30, 30), g_System.Font17, NULL);
		datetimeset_left[i] = Creatleftpicture ("datetimeset_left", dts,
																MRect (407, 170 + i * 40 + 5, 30, 20), g_System.Font17, NULL);

		datetimeset_right[i] = Creatrightpicture ("datetimeset_right", dts,
																MRect (558, 170 + i * 40 + 5, 30, 20), g_System.Font17, NULL);

		datetimeset_label[i] = CreatLabel ("datetimeset_label", dts,
																MRect (320, 170 + i * 40, 70, 30), g_System.Font17, NULL);

		datetimeset_list[i] = CreatButton ("datetimeset_list", dts,
																MRect (400, 170 + i * 40, 180, 30), g_System.Font17, NULL);
	}
	datetimeset_title[2] = CreatButton ("datetimeset_title", dts, MRect (230, 120, 400, 30), g_System.big_menu, NULL);

	datetimeset_title[0]  =CreatLabel("datetimeset_title",   dts,
																	MRect(70,10,120,30), g_System.big_menu, NULL);
	datetimeset_title[1]  =CreatLabel("datetimeset_title",   dts,
																	MRect(95,40,150,30), g_System.small_menu, NULL);
	DateTimeSetShow (false);

	SetButtonText (datetimeset_list[0], "2015");
	SetButtonText (datetimeset_list[1], "5");
	SetButtonText (datetimeset_list[2], "28");
	SetButtonText (datetimeset_list[3], "13");
	SetButtonText (datetimeset_list[4], "18");



	SetLabelText (datetimeset_label[0], "Year");
	SetLabelText (datetimeset_label[1], "Month");
	SetLabelText (datetimeset_label[2], "Day");
	SetLabelText (datetimeset_label[3], "Hour");
	SetLabelText (datetimeset_label[4], "Minute");

	SetLabelText (datetimeset_title[0], "MENU");
	SetLabelText (datetimeset_title[1], "DateTimeSet");

	for (int i = 0; i < 3; i++)
	{
		SetLabelBackGroundColor (datetimeset_title[i], 0x85223284);
	}
	for (int i = 0; i < 5; i++)
	{
		SetLabelBackGroundColor (datetimeset_label[i], 0x85223284);
	}
}

void ShowDisplayTime ()
{
	time (&now);
	timenow = localtime (&now);
	timenow->tm_year = timenow->tm_year + 1900;
	timenow->tm_mon = timenow->tm_mon + 1;
	sprintf (show_tempbuf_time, "%04d-%02d-%02d %02d:%02d:%02d", timenow->tm_year, timenow->tm_mon, timenow->tm_mday,
																	timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
}

void MenuDateTimeSerUpdate ()
{
	static char buffer_show[100] = {0};
	ShowDisplayTime ();
	sprintf (buffer_show, "Date&Time:  %s", show_tempbuf_time);
	SetLabelText (datetimeset_title[2], buffer_show);
}

void DateTimeSetShow (bool show)
{
	int ret = 0;
	bwidget_show (dts, show);

	for (int i = 0; i < 5; i++)
	{
		bwidget_show (datetimeset_list[i], show);
		bwidget_show (datetimeset_label[i], show);
	}

	for (int i = 0; i < 3; i++)
	{
		bwidget_show (datetimeset_title[i], show);
	}
	if (show == true)
	{
		ret = bwidget_set_focus (datetimeset_list[datetimeset_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (i == datetimeset_focuse)
			{
				bwidget_show (datetimeset_left[i], true);
				bwidget_show (datetimeset_right[i], true);
				bwidget_show (datetimeset_circle[i], true);
				continue;
			}
			bwidget_show (datetimeset_left[i], false);
			bwidget_show (datetimeset_right[i], false);
			bwidget_show (datetimeset_circle[i], false);
		}
		for (int i = 0; i < 5; i++)
		{
			SetButtonBackGroundColor (datetimeset_list[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (datetimeset_list[datetimeset_focuse], FOCUS_COLOR);
		}
		printf ("ret = %d\n", ret);
	}
}

bool IsLeapYear (unsigned int year)
{
	static bool flag;

	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
	{
		flag = true;
	}
	else
	{
		flag = false;
	}

	return flag;
}

unsigned int datetimeset_num[] = {2015, 5, 28, 13, 18};

void DisplayDateTimeSet (unsigned char key)
{
	static char datetimeset_buf_right[5][5] = {{0}};
	static char datetimeset_buf_left[5][5] = {{0}};

	if (datetimeset_focuse == 0)
	{
		if(key == eKey_RightArrow)
		{
			if(datetimeset_num[0]<2030)
			{
				datetimeset_num[0]++;
			}
			else
			{
				datetimeset_num[0]=2000;
			}
			sprintf (datetimeset_buf_right[0], "%d", datetimeset_num[0]);
			SetButtonText (datetimeset_list[0], datetimeset_buf_right[0]);
		}
		else if(key == eKey_LeftArrow)
		{
			if(datetimeset_num[0]>2000)
			{
				datetimeset_num[0]--;
			}
			else
			{
				datetimeset_num[0]=2030;
			}
			sprintf (datetimeset_buf_left[0], "%d", datetimeset_num[0]);
			SetButtonText (datetimeset_list[0], datetimeset_buf_left[0]);
		}
	}
	else if (datetimeset_focuse == 1)
	{
		if(key == eKey_RightArrow)
		{
			if (datetimeset_num[1] >= 12)
			{
				datetimeset_num[1] = 0;
				sprintf (datetimeset_buf_right[1], "%d", ++datetimeset_num[1]);
				SetButtonText (datetimeset_list[1], datetimeset_buf_right[1]);
			}
			else
			{
				sprintf (datetimeset_buf_right[1], "%d", ++datetimeset_num[1]);
				SetButtonText (datetimeset_list[1], datetimeset_buf_right[1]);
			}
		}
		else if(key == eKey_LeftArrow)
		{
			if (datetimeset_num[1] == 1)
			{
				datetimeset_num[1] = 13;
				sprintf (datetimeset_buf_left[1], "%d", --datetimeset_num[1]);
				SetButtonText (datetimeset_list[1], datetimeset_buf_left[1]);
			}
			else
			{
				sprintf (datetimeset_buf_left[1], "%d", --datetimeset_num[1]);
				SetButtonText (datetimeset_list[1], datetimeset_buf_left[1]);
			}
		}
	}
	else if (datetimeset_focuse == 2)
	{
		if(key == eKey_RightArrow)
		{
			if (datetimeset_num[1] == 1 || datetimeset_num[1] == 3 || datetimeset_num[1] == 5
				|| datetimeset_num[1] == 7 || datetimeset_num[1] == 8 || datetimeset_num[1] == 10
				|| datetimeset_num[1] == 12)
			{
				if (datetimeset_num[2] >= 31)
				{
					datetimeset_num[2] = 0;
					sprintf (datetimeset_buf_right[2], "%d", ++datetimeset_num[2]);
					SetButtonText (datetimeset_list[2], datetimeset_buf_right[2]);
				}
				else
				{
					sprintf (datetimeset_buf_right[2], "%d", ++datetimeset_num[2]);
					SetButtonText (datetimeset_list[2], datetimeset_buf_right[2]);
				}
			}
			else if (datetimeset_num[1] == 4 || datetimeset_num[1] == 6
				|| datetimeset_num[1] == 9 || datetimeset_num[1] == 11)
			{
				if (datetimeset_num[2] >= 30)
				{
					datetimeset_num[2] = 0;
					sprintf (datetimeset_buf_right[2], "%d", ++datetimeset_num[2]);
					SetButtonText (datetimeset_list[2], datetimeset_buf_right[2]);
				}
				else
				{
					sprintf (datetimeset_buf_right[2], "%d", ++datetimeset_num[2]);
					SetButtonText (datetimeset_list[2], datetimeset_buf_right[2]);
				}
			}
			else if (datetimeset_num[1] == 2)
			{
				if (IsLeapYear (datetimeset_num[0]))
				{
					if (datetimeset_num[2] >= 29)
					{
						datetimeset_num[2] = 0;
						sprintf (datetimeset_buf_right[2], "%d", ++datetimeset_num[2]);
						SetButtonText (datetimeset_list[2], datetimeset_buf_right[2]);
					}
					else
					{
						sprintf (datetimeset_buf_right[2], "%d", ++datetimeset_num[2]);
						SetButtonText (datetimeset_list[2], datetimeset_buf_right[2]);
					}
				}
				else
				{
					if (datetimeset_num[2] >= 28)
					{
						datetimeset_num[2] = 0;
						sprintf (datetimeset_buf_right[2], "%d", ++datetimeset_num[2]);
						SetButtonText (datetimeset_list[2], datetimeset_buf_right[2]);
					}
					else
					{
						sprintf (datetimeset_buf_right[2], "%d", ++datetimeset_num[2]);
						SetButtonText (datetimeset_list[2], datetimeset_buf_right[2]);
					}
				}
			}
		}
		else if(key == eKey_LeftArrow)
		{
			if (datetimeset_num[1] == 1 || datetimeset_num[1] == 3 || datetimeset_num[1] == 5
				|| datetimeset_num[1] == 7 || datetimeset_num[1] == 8 || datetimeset_num[1] == 10
				|| datetimeset_num[1] == 12)
			{
				if (datetimeset_num[2] == 1)
				{
					datetimeset_num[2] = 32;
					sprintf (datetimeset_buf_left[2], "%d", --datetimeset_num[2]);
					SetButtonText (datetimeset_list[2], datetimeset_buf_left[2]);
				}
				else
				{
					sprintf (datetimeset_buf_left[2], "%d", --datetimeset_num[2]);
					SetButtonText (datetimeset_list[2], datetimeset_buf_left[2]);
				}
			}
			else if (datetimeset_num[1] == 4 || datetimeset_num[1] == 6
				|| datetimeset_num[1] == 9 || datetimeset_num[1] == 11)
			{
				if (datetimeset_num[2] == 1)
				{
					datetimeset_num[2] = 31;
					sprintf (datetimeset_buf_left[2], "%d", --datetimeset_num[2]);
					SetButtonText (datetimeset_list[2], datetimeset_buf_left[2]);
				}
				else
				{
					sprintf (datetimeset_buf_left[2], "%d", --datetimeset_num[2]);
					SetButtonText (datetimeset_list[2], datetimeset_buf_left[2]);
				}
			}
			else if (datetimeset_num[1] == 2)
			{
				if (IsLeapYear (datetimeset_num[0]))
				{
					if (datetimeset_num[2] == 1)
					{
						datetimeset_num[2] = 30;
						sprintf (datetimeset_buf_left[2], "%d", --datetimeset_num[2]);
						SetButtonText (datetimeset_list[2], datetimeset_buf_left[2]);
					}
					else
					{
						sprintf (datetimeset_buf_left[2], "%d", --datetimeset_num[2]);
						SetButtonText (datetimeset_list[2], datetimeset_buf_left[2]);
					}
				}
				else
				{
					if (datetimeset_num[2] == 1)
					{
						datetimeset_num[2] = 29;
						sprintf (datetimeset_buf_left[2], "%d", --datetimeset_num[2]);
						SetButtonText (datetimeset_list[2], datetimeset_buf_left[2]);
					}
					else
					{
						sprintf (datetimeset_buf_left[2], "%d", --datetimeset_num[2]);
						SetButtonText (datetimeset_list[2], datetimeset_buf_left[2]);
					}
				}
			}
		}
	}
	else if (datetimeset_focuse == 3)
	{
		if(key == eKey_RightArrow)
		{
			if (datetimeset_num[3] == 23)
			{
				datetimeset_num[3] = -1;
				sprintf (datetimeset_buf_right[3], "%d", ++datetimeset_num[3]);
				SetButtonText (datetimeset_list[3], datetimeset_buf_right[3]);
			}
			else
			{
				sprintf (datetimeset_buf_right[3], "%d", ++datetimeset_num[3]);
				SetButtonText (datetimeset_list[3], datetimeset_buf_right[3]);
			}
		}
		else if(key == eKey_LeftArrow)
		{
			if (datetimeset_num[3] == 0)
			{
				datetimeset_num[3] = 24;
				sprintf (datetimeset_buf_left[3], "%d", --datetimeset_num[3]);
				SetButtonText (datetimeset_list[3], datetimeset_buf_left[3]);
			}
			else
			{
				sprintf (datetimeset_buf_left[3], "%d", --datetimeset_num[3]);
				SetButtonText (datetimeset_list[3], datetimeset_buf_left[3]);
			}
		}
	}
	else if (datetimeset_focuse == 4)
	{
		if(key == eKey_RightArrow)
		{
			if (datetimeset_num[4] == 59)
			{
				datetimeset_num[4] = -1;
				sprintf (datetimeset_buf_right[4], "%d", ++datetimeset_num[4]);
				SetButtonText (datetimeset_list[4], datetimeset_buf_right[4]);
			}
			else
			{
				sprintf (datetimeset_buf_right[4], "%d", ++datetimeset_num[4]);
				SetButtonText (datetimeset_list[4], datetimeset_buf_right[4]);
			}
		}
		else if(key == eKey_LeftArrow)
		{
			if (datetimeset_num[4] == 0)
			{
				datetimeset_num[4] = 60;
				sprintf (datetimeset_buf_left[4], "%d", --datetimeset_num[4]);
				SetButtonText (datetimeset_list[4], datetimeset_buf_left[4]);
			}
			else
			{
				sprintf (datetimeset_buf_left[4], "%d", --datetimeset_num[4]);
				SetButtonText (datetimeset_list[4], datetimeset_buf_left[4]);
			}
		}
	}
}

int MenuDateTimeSetKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		datetimeset_focuse++;
		if (datetimeset_focuse == 5)
		{
			datetimeset_focuse = 0;
		}
		bwidget_set_focus (datetimeset_list[datetimeset_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (i == datetimeset_focuse)
			{
				bwidget_show (datetimeset_left[i], true);
				bwidget_show (datetimeset_right[i], true);
				bwidget_show (datetimeset_circle[i], true);
				continue;
			}
			bwidget_show (datetimeset_left[i], false);
			bwidget_show (datetimeset_right[i], false);
			bwidget_show (datetimeset_circle[i], false);
		}
	}
	else if (key_code == eKey_UpArrow)
	{
		if (datetimeset_focuse == 0)
		{
			datetimeset_focuse = 4;
		}
		else
		{
			datetimeset_focuse--;
		}
		bwidget_set_focus (datetimeset_list[datetimeset_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (i == datetimeset_focuse)
			{
				bwidget_show (datetimeset_left[i], true);
				bwidget_show (datetimeset_right[i], true);
				bwidget_show (datetimeset_circle[i], true);
				continue;
			}
			bwidget_show (datetimeset_left[i], false);
			bwidget_show (datetimeset_right[i], false);
			bwidget_show (datetimeset_circle[i], false);
		}
	}
	else if (key_code == eKey_Select)
	{
		SetRtcAndSystemTime(datetimeset_num);
		/*
		static char set_time_buffer[80] = {0};
		sprintf(set_time_buffer, "date -s %04d.%02d.%02d-%02d:%02d:%02d", datetimeset_num[0], datetimeset_num[1],
															datetimeset_num[2], datetimeset_num[3], datetimeset_num[4], 18);
		system (set_time_buffer);
		*/
	}
	else if (key_code == eKey_Menu)
	{
		DateTimeSetShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{

	}
	else if (key_code == eKey_Exit)
	{
		DateTimeSetShow (false);
		GeneralSettingsShow (true);
		CurrentMenu = eMenu_GeneralSettings;
	}
	else if (key_code == eKey_LeftArrow || key_code == eKey_RightArrow)
	{
		DisplayDateTimeSet (key_code);
	}
	for (int i = 0; i < 5; i++)
	{
		SetButtonBackGroundColor (datetimeset_list[i], UNFOCUS_COLOR);
		SetButtonBackGroundColor (datetimeset_list[datetimeset_focuse], FOCUS_COLOR);
	}

	return 0;
}

void CloseDateTimeSetMenu ()
{
	bwidget_destroy (dts);
	for (int i = 0; i < 5; i++)
	{
		bwidget_destroy (datetimeset_list[i]);
		bwidget_destroy (datetimeset_circle[i]);
		bwidget_destroy (datetimeset_left[i]);
		bwidget_destroy (datetimeset_right[i]);
		bwidget_destroy (datetimeset_label[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		bwidget_destroy (datetimeset_title[i]);
	}
}

