#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "label.h"
#include "button.h"
#include "menu_main.h"
#include "dib3000.h"
#include "channellist.h"
#include "menu_info.h"
#include "global.h"
#include "graphics.h"
#include "menu_circlelabel.h"

void ChannelSearchShow (bool show);

bwidget_t l_bg;
bwidget_t channel_search_list[12];
bwidget_t left_right_channelsearch[2];
bwidget_t test[3];
unsigned char ChanelSearch_focuse = 0;
static char *SupportedBandWidth[] = {(char *) ("8"), (char *) ("7"), (char *) ("6"),
										(char *) ("5"), (char *) ("4"),(char *) ("3"), (char *) ("2"), (char *) ("1.5"), (char *) ("1")};
static unsigned char BandWidthSelect = 0;
int num_key = 3;
int num = 340;
int num_bandwidth = 9;

void MenuChannelSearchUpdate ()
{
	signed short strength_search, quality_search, strength1_search, quality1_search, strength2_search, quality2_search;
	static char buf[100] = {0};
	static char buf1[100] = {0};
	static char buf2[100] = {0};
	if (g_Channel.ChannelList[g_Channel.CurrentChannel].BandWidth == 1500)
	{
		sprintf (buf, "%d-%dMHz-%0.1fMHz", g_Channel.CurrentChannel,
											g_Channel.ChannelList[g_Channel.CurrentChannel].Frequency/1000,
											g_Channel.ChannelList[g_Channel.CurrentChannel].BandWidth/1000.0);
	}
	else
	{
		sprintf (buf, "%d-%dMHz-%dMHz", g_Channel.CurrentChannel,
											g_Channel.ChannelList[g_Channel.CurrentChannel].Frequency/1000,
											g_Channel.ChannelList[g_Channel.CurrentChannel].BandWidth/1000);
	}
	SetLabelText (channel_search_list[9], buf);
	dib_show_signal_strength_quality (i2c);
	dib_signal_strength_quality (i2c, &strength_search, &quality_search, &strength1_search,
																&strength2_search, &quality1_search, &quality2_search);
	sprintf (buf1, "sigal_1:%4ddBm     SNR_1:%4d", strength1_search, quality1_search);
	sprintf (buf2, "sigal_2:%4ddBm     SNR_2:%4d", strength2_search, quality2_search);
	SetLabelText (channel_search_list[10], buf1);
	SetLabelText (channel_search_list[11], buf2);
}

void ChannelSearchInit ()
{
	l_bg = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50, 50, 830, 815), g_System.Font17, NULL);

	left_right_channelsearch[0] = Creatleftpicture ("left_right_channelsearch", l_bg,
																MRect (383, 120 + 55, 30, 20), g_System.Font17, NULL);
	left_right_channelsearch[1] = Creatrightpicture ("left_right_channelsearch", l_bg,
																MRect (512, 120 + 55, 30, 20), g_System.Font17, NULL);

	for (int i = 0; i < 3; i++)
	{
		test[i] = CreatCirclepicture ("test", l_bg, MRect (200, 120 + i * 50, 30, 30), g_System.Font17, NULL);
	}

	for (int i = 3; i < 5; i++)
	{
		channel_search_list[i] = CreatLabel ("label", l_bg, MRect (265, 120 + (i - 3) * 50, 108, 30), g_System.Font17, NULL);
	}
	for (int i = 5; i < 7; i++)
	{
		channel_search_list[i] = CreatLabel ("label", l_bg, MRect (540, 120 + (i - 5) * 50, 58, 30), g_System.Font17, NULL);
	}
	for (int i = 0; i < 2; i++)
	{
		channel_search_list[i] = CreatButton ("text", l_bg, MRect (380, 120 + i * 50, 150, 30), g_System.Font17, NULL);
	}

	channel_search_list[2] = CreatButton ("scan", l_bg, MRect (266, 220, 320, 30), g_System.Font17, NULL);
	channel_search_list[7] = CreatLabel ("MenuChannelSearch", l_bg, MRect (70, 10, 120, 30), g_System.big_menu, NULL);
	channel_search_list[8] = CreatLabel ("MenuChannelSearch", l_bg, MRect (95, 40, 280, 30), g_System.small_menu, NULL);
	channel_search_list[9] = CreatLabel ("channel1", l_bg, MRect (266, 270, 480, 30), g_System.Font17, NULL);
	channel_search_list[10] = CreatLabel ("channel2", l_bg, MRect (266, 300, 480, 30), g_System.Font17, NULL);
	channel_search_list[11] = CreatLabel ("channel3", l_bg, MRect (266, 330, 480, 30), g_System.Font17, NULL);

	ChannelSearchShow (false);

	SetLabelText (channel_search_list[3], "Frequency");
	SetLabelText (channel_search_list[5], "MHz");
	SetLabelText (channel_search_list[4], "BandWidth");
	SetLabelText (channel_search_list[6], "MHz");


    BandWidthSelect=0;
    SetButtonText (channel_search_list[0], "340");
	SetButtonText (channel_search_list[1], "8");

	SetButtonText (channel_search_list[2], "SCAN");
	SetLabelText (channel_search_list[7], "MENU");
	SetLabelText (channel_search_list[8], "Channel Search");

	SetLabelBackGroundColor (channel_search_list[3], 0x85223284);
	SetLabelBackGroundColor (channel_search_list[4], 0x85223284);
	SetLabelBackGroundColor (channel_search_list[5], 0x85223284);
	SetLabelBackGroundColor (channel_search_list[6], 0x85223284);
	SetLabelBackGroundColor (channel_search_list[7], 0x85223284);
	SetLabelBackGroundColor (channel_search_list[8], 0x85223284);
	SetLabelBackGroundColor (channel_search_list[9], 0x85223284);
	SetLabelBackGroundColor (channel_search_list[10], 0x85223284);
	SetLabelBackGroundColor (channel_search_list[11], 0x85223284);
}

void ChannelSearchShow (bool show)
{
	int ret = 0;
	for (int i = 0; i < 11; i++)
	{
		bwidget_show (channel_search_list[i], show);
	}
	bwidget_show (l_bg, show);

	if (show == true)
	{
		ret = bwidget_set_focus (channel_search_list[ChanelSearch_focuse]);
		for (int i = 0; i < 3; i++)
		{
			if (i == ChanelSearch_focuse)
			{
				bwidget_show (test[i], true);
				continue;
			}
			bwidget_show (test[i], false);
		}
		if (ChanelSearch_focuse == 1)
		{
			bwidget_show (left_right_channelsearch[0], true);
			bwidget_show (left_right_channelsearch[1], true);
		}
		else
		{
			bwidget_show (left_right_channelsearch[0], false);
			bwidget_show (left_right_channelsearch[1], false);
		}

		for (int i = 0; i < 3; i++)
		{
			SetButtonBackGroundColor (channel_search_list[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (channel_search_list[ChanelSearch_focuse], FOCUS_COLOR);
		}
		printf ("ret = %d\n", ret);
	}
}

void DisplayFre (unsigned char key)
{
	static char buf[10] = {0};
	if (num_key == 3)
	{
		num = key;
		sprintf (buf, "%d", num);
		SetButtonText (channel_search_list[0], buf);
		num_key = 1;
	}
	else if (num_key == 1)
	{
		num = num * 10 + key;
		sprintf (buf, "%d", num);
		SetButtonText (channel_search_list[0], buf);
		num_key = 2;
	}
	else if (num_key == 2)
	{
		num = num * 10 + key;
		sprintf (buf, "%d", num);
		SetButtonText (channel_search_list[0], buf);
		num_key = 3;
	}
}

int MenuChannelSearchKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		ChanelSearch_focuse++;
		if (ChanelSearch_focuse == 3)
		{
			ChanelSearch_focuse = 0;
		}
		bwidget_set_focus (channel_search_list[ChanelSearch_focuse]);
		if (ChanelSearch_focuse == 1)
		{
			bwidget_show (left_right_channelsearch[0], true);
			bwidget_show (left_right_channelsearch[1], true);
		}
		else
		{
			bwidget_show (left_right_channelsearch[0], false);
			bwidget_show (left_right_channelsearch[1], false);
		}
	}
	else if (key_code == eKey_UpArrow)
	{
		if (ChanelSearch_focuse == 0)
		{
			ChanelSearch_focuse = 2;
		}
		else
		{
			ChanelSearch_focuse--;
		}
		bwidget_set_focus (channel_search_list[ChanelSearch_focuse]);
		if (ChanelSearch_focuse == 1)
		{
			bwidget_show (left_right_channelsearch[0], true);
			bwidget_show (left_right_channelsearch[1], true);
		}
		else
		{
			bwidget_show (left_right_channelsearch[0], false);
			bwidget_show (left_right_channelsearch[1], false);
		}
	}
	else if (key_code == eKey_Select)
	{
		if (ChanelSearch_focuse == 2)
		{
			uint32_t bandwidth_Khz = 0;
			uint32_t fre_Khz = 0;
			uint32_t fre_Mhz = num;
			NEXUS_PlatformConfiguration platformConfig;

			if (BandWidthSelect == 7)
			{
				bandwidth_Khz = 1500;
			}
			else
			{
				bandwidth_Khz = atoi (SupportedBandWidth[BandWidthSelect]) * 1000;
			}
			fre_Khz = fre_Mhz * 1000;
			NEXUS_Platform_GetConfiguration (&platformConfig);
			AddChannel (fre_Khz, bandwidth_Khz);
		}
	}
	else if (key_code == eKey_Menu)
	{
		ChannelSearchShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{
		if (ChanelSearch_focuse == 0)
		{
			DisplayFre (key_code - 0x1d);
		}
	}
	else if (key_code == eKey_Exit)
	{
		ChannelSearchShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if (key_code == eKey_LeftArrow)
	{
		if (ChanelSearch_focuse == 1)
		{
			if (BandWidthSelect == 0)
			{
				BandWidthSelect = 8;
			}
			else
			{
				BandWidthSelect--;
			}
			SetButtonText (channel_search_list[1], SupportedBandWidth[BandWidthSelect]);
		}
	}
	else if (key_code == eKey_RightArrow)
	{
		if (ChanelSearch_focuse == 1)
		{
			if (BandWidthSelect == 8)
			{
				BandWidthSelect = 0;
			}
			else
			{
				BandWidthSelect++;
			}
			SetButtonText (channel_search_list[1], SupportedBandWidth[BandWidthSelect]);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (i == ChanelSearch_focuse)
		{
			bwidget_show (test[i], true);
			continue;
		}
		bwidget_show (test[i], false);
	}

	for (int i = 0; i < 3; i++)
	{
		SetButtonBackGroundColor (channel_search_list[i], UNFOCUS_COLOR);
		SetButtonBackGroundColor (channel_search_list[ChanelSearch_focuse], FOCUS_COLOR);
	}

	return 0;
}

void CloseChannelSearchMenu ()
{
	bwidget_destroy (l_bg);
	for (int i = 0; i < 12; i++)
	{
		bwidget_destroy (channel_search_list[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		bwidget_destroy (left_right_channelsearch[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		bwidget_destroy (test[i]);
	}
}

