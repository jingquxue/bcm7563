#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "label.h"
#include "button.h"
#include "menu_main.h"
#include "dib3000.h"
#include "channellist.h"
#include "display.h"
#include "menu_circlelabel.h"
#include "global.h"
#include "display.h"

void DisplaySettingsShow (bool show);

bwidget_t d_bg;
bwidget_t menu_displaysettings[2];
bwidget_t display_settings_circle[7];
bwidget_t display_settings_left[7];
bwidget_t display_settings_right[7];
bwidget_t display_settings_label[7];
bwidget_t display_settings_button[7];
unsigned char dPlayBack_focuse = 0;
char *format_buffer[] = {(char *) ("576i"), (char *) ("576p"), (char *) ("480i"), (char *) ("480p"),
						(char *) ("720p 50Hz"), (char *) ("720p 60Hz"), (char *) ("1080i 50Hz"), (char *) ("1080i 60Hz"),
						(char *) ("1080p 24Hz"), (char *) ("1080p 25Hz"), (char *) ("1080p 30Hz"), (char *) ("1080p 50Hz"), (char *) ("1080p 60Hz")};
char *aspectratio_buffer[] = {(char *) ("Auto"), (char *) ("16X9"), (char *) ("4X3")};
char *colorspace_buffer[] = {(char *) ("Pal"), (char *) ("Ntsc")};
char *autoformat_buffer[] = {(char *) ("Auto"), (char *) ("16X9"), (char *) ("4X3")};
char *decodemode_buffer[] = {(char *) ("Normal Delay"), (char *) ("Low Delay")};
char *languages_direction[] = {(char *) ("English"), (char *) ("Chinese")};
char *transparency_direction[] = {(char *) ("0%"), (char *) ("20%"), (char *) ("50%")};
int left_or_right_num_display_settings[7] = {0};

void DisplaySettingsInit ()
{
	d_bg = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50, 50, 830, 815), g_System.Font17, NULL);

	for (int i = 0; i < 7; i++)
	{
		display_settings_circle[i] = CreatCirclepicture ("display_settings_circle", d_bg,
															MRect (205, 110 + i * 40, 30, 30), g_System.Font17, NULL);
	}
	for (int i = 0; i < 7; i++)
	{
		display_settings_left[i] = Creatleftpicture ("display_settings_left", d_bg,
															MRect (445, 110 + i * 40 + 5, 30, 20), g_System.Font17, NULL);
	}
	for (int i = 0; i < 7; i++)
	{
		display_settings_right[i] = Creatrightpicture ("display_settings_right", d_bg,
															MRect (610, 110 + i * 40 + 5, 30, 20), g_System.Font17, NULL);
	}
	for (int i = 0; i < 7; i++)
	{
		display_settings_label[i] = CreatLabel ("display_settings_label", d_bg,
															MRect (260, 110 + i * 40, 175, 30), g_System.Font17, NULL);
	}
	for (int i = 0; i < 7; i++)
	{
		display_settings_button[i] = CreatButton ("display_settings_button", d_bg,
															MRect (440, 110 + i * 40, 190, 30), g_System.Font17, NULL);
	}
	menu_displaysettings[0] = CreatLabel ("menu_displaysettings", d_bg,
															MRect (70, 10, 120, 30), g_System.big_menu, NULL);
	menu_displaysettings[1] = CreatLabel ("menu_displaysettings", d_bg,
															MRect (95, 40, 200, 30), g_System.small_menu, NULL);
	DisplaySettingsShow (false);


	left_or_right_num_display_settings[0]=g_Config.HdmiSettings.Format;
	left_or_right_num_display_settings[1]=g_Config.HdmiSettings.AspectRatio;
	if(g_Config.CvbsSettings.Format==2)
	{
		left_or_right_num_display_settings[2]=1;
	}
	else
	{
		left_or_right_num_display_settings[2]=0;
	}


	left_or_right_num_display_settings[3]=g_Config.CvbsSettings.AspectRatio;




	SetLabelText (menu_displaysettings[0], "MENU");
	SetLabelText (menu_displaysettings[1], "Display Settings");
	SetLabelText (display_settings_label[0], "HDMI Format");
	SetLabelText (display_settings_label[1], "HDMI AspectRatio");
	SetLabelText (display_settings_label[2], "CVBS Format");
	SetLabelText (display_settings_label[3], "CVBS AspectRatio");
	SetLabelText (display_settings_label[4], "Decode Mode");
	SetLabelText (display_settings_label[5], "Languages");
	SetLabelText (display_settings_label[6], "Transparency");
	SetButtonText (display_settings_button[0], format_buffer[left_or_right_num_display_settings[0]]);
	SetButtonText (display_settings_button[1], aspectratio_buffer[left_or_right_num_display_settings[1]]);
	SetButtonText (display_settings_button[2], colorspace_buffer[left_or_right_num_display_settings[2]]);
	SetButtonText (display_settings_button[3], autoformat_buffer[left_or_right_num_display_settings[3]]);
	SetButtonText (display_settings_button[4], decodemode_buffer[left_or_right_num_display_settings[4]]);
	SetButtonText (display_settings_button[5], languages_direction[left_or_right_num_display_settings[5]]);
	SetButtonText (display_settings_button[6], transparency_direction[left_or_right_num_display_settings[6]]);

	SetLabelBackGroundColor (menu_displaysettings[0], 0x85223284);
	SetLabelBackGroundColor (menu_displaysettings[1], 0x85223284);

	for (int i = 0; i < 7; i++)
	{
		SetLabelBackGroundColor (display_settings_label[i], 0x85223284);
	}
}

void DisplaySettingsShow (bool show)
{
	int ret = 0;
	bwidget_show (d_bg, show);

	for (int i = 0; i < 7; i++)
	{
		bwidget_show (display_settings_label[i], show);
		bwidget_show (display_settings_button[i], show);
	}
	if (show == true)
	{
		for (int i = 0; i < 7; i++)
		{
			if (i == dPlayBack_focuse)
			{
				bwidget_show (display_settings_circle[i], true);
				bwidget_show (display_settings_right[i], true);
				bwidget_show (display_settings_left[i], true);
				continue;
			}
			bwidget_show (display_settings_circle[i], false);
			bwidget_show (display_settings_right[i], false);
			bwidget_show (display_settings_left[i], false);
		}
		ret = bwidget_set_focus (display_settings_button[dPlayBack_focuse]);
		for (int i = 0; i < 7; i++)
		{
			SetButtonBackGroundColor (display_settings_button[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (display_settings_button[dPlayBack_focuse], FOCUS_COLOR);
		}
		printf ("ret = %d\n", ret);
	}
}

void DisplayDisplaySettings (unsigned char key)
{
	if (dPlayBack_focuse == 2)
	{
		if (left_or_right_num_display_settings[2] == 0)
		{
			SetButtonText (display_settings_button[2], colorspace_buffer[1]);
			left_or_right_num_display_settings[2] = 1;
		}
		else if (left_or_right_num_display_settings[2] == 1)
		{
			SetButtonText (display_settings_button[2], colorspace_buffer[0]);
			left_or_right_num_display_settings[2] = 0;
		}
	}
	else if (dPlayBack_focuse == 3)
	{
		if (key == eKey_RightArrow)
		{
			left_or_right_num_display_settings[3]++;
			if (left_or_right_num_display_settings[3] == 3)
			{
				left_or_right_num_display_settings[3] = 0;
			}
		}
		else
		{
			left_or_right_num_display_settings[3]--;
			if (left_or_right_num_display_settings[3] == -1)
			{
				left_or_right_num_display_settings[3] = 2;
			}
		}
		SetButtonText (display_settings_button[3], aspectratio_buffer[left_or_right_num_display_settings[3]]);
	}
	else if (dPlayBack_focuse == 4)
	{
		if (left_or_right_num_display_settings[4] == 0)
		{
			SetButtonText (display_settings_button[4], decodemode_buffer[1]);
			left_or_right_num_display_settings[4] = 1;
		}
		else if (left_or_right_num_display_settings[4] == 1)
		{
			SetButtonText (display_settings_button[4], decodemode_buffer[0]);
			left_or_right_num_display_settings[4] = 0;
		}
	}
	else if (dPlayBack_focuse == 5)
	{
		if (left_or_right_num_display_settings[5] == 0)
		{
			SetButtonText (display_settings_button[5], languages_direction[1]);
			left_or_right_num_display_settings[5] = 1;
		}
		else if (left_or_right_num_display_settings[5] == 1)
		{
			SetButtonText (display_settings_button[5], languages_direction[0]);
			left_or_right_num_display_settings[5] = 0;
		}
	}
	else if (dPlayBack_focuse == 6)
	{
		if (key == eKey_RightArrow)
		{
			left_or_right_num_display_settings[6]++;
			if (left_or_right_num_display_settings[6] == 3)
			{
				left_or_right_num_display_settings[6] = 0;
			}
		}
		else
		{
			left_or_right_num_display_settings[6]--;
			if (left_or_right_num_display_settings[6] == -1)
			{
				left_or_right_num_display_settings[6] = 2;
			}
		}
		SetButtonText (display_settings_button[6], transparency_direction[left_or_right_num_display_settings[6]]);
	}
	else if (dPlayBack_focuse == 0)
	{
		if (key == eKey_RightArrow)
		{
			left_or_right_num_display_settings[0]++;
			if (left_or_right_num_display_settings[0] == 13)
			{
				left_or_right_num_display_settings[0] = 0;
			}
		}
		else
		{
			left_or_right_num_display_settings[0]--;
			if (left_or_right_num_display_settings[0] == -1)
			{
				left_or_right_num_display_settings[0] = 12;
			}
		}
		SetButtonText (display_settings_button[0], format_buffer[left_or_right_num_display_settings[0]]);
	}
	else if (dPlayBack_focuse == 1)
	{
		if (key == eKey_RightArrow)
		{
			left_or_right_num_display_settings[1]++;
			if (left_or_right_num_display_settings[1] == 3)
			{
				left_or_right_num_display_settings[1] = 0;
			}
		}
		else
		{
			left_or_right_num_display_settings[1]--;
			if (left_or_right_num_display_settings[1] == -1)
			{
				left_or_right_num_display_settings[1] = 2;
			}
		}
		SetButtonText (display_settings_button[1], aspectratio_buffer[left_or_right_num_display_settings[1]]);
	}
}

int MenuDisplaySettingsKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		dPlayBack_focuse++;
		if (dPlayBack_focuse == 7)
		{
			dPlayBack_focuse = 0;
		}
		bwidget_set_focus (display_settings_button[dPlayBack_focuse]);
		for (int i = 0; i < 7; i++)
		{
			if (i == dPlayBack_focuse)
			{
				bwidget_show (display_settings_circle[i], true);
				bwidget_show (display_settings_right[i], true);
				bwidget_show (display_settings_left[i], true);
				continue;
			}
			bwidget_show (display_settings_circle[i], false);
			bwidget_show (display_settings_right[i], false);
			bwidget_show (display_settings_left[i], false);
		}
		for (int i = 0; i < 7; i++)
		{
			SetButtonBackGroundColor (display_settings_button[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (display_settings_button[dPlayBack_focuse], FOCUS_COLOR);
		}
	}
	else if (key_code == eKey_UpArrow)
	{
		if (dPlayBack_focuse == 0)
		{
			dPlayBack_focuse = 6;
		}
		else
		{
			dPlayBack_focuse--;
		}
		bwidget_set_focus (display_settings_button[dPlayBack_focuse]);
		for (int i = 0; i < 7; i++)
		{
			if (i == dPlayBack_focuse)
			{
				bwidget_show (display_settings_circle[i], true);
				bwidget_show (display_settings_right[i], true);
				bwidget_show (display_settings_left[i], true);
				continue;
			}
			bwidget_show (display_settings_circle[i], false);
			bwidget_show (display_settings_right[i], false);
			bwidget_show (display_settings_left[i], false);
		}
		for (int i = 0; i < 7; i++)
		{
			SetButtonBackGroundColor (display_settings_button[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (display_settings_button[dPlayBack_focuse], FOCUS_COLOR);
		}
	}
	else if (key_code == eKey_Select)
	{
		if (dPlayBack_focuse == 0)
		{
			NEXUS_VideoFormat format = NEXUS_VideoFormat_e1080p50hz;
			format=ConvertVideoFormatToBroadcom(0,left_or_right_num_display_settings[0]);
/*
			switch (left_or_right_num_display_settings[0])
			{
				case 0:
					format = NEXUS_VideoFormat_e1080i;
					break;
				case 1:
					format = NEXUS_VideoFormat_e1080i50hz;
					break;
				case 2:
					format = NEXUS_VideoFormat_e1080p;
					break;
				case 3:
					format = NEXUS_VideoFormat_e1080p24hz;
					break;
				case 4:
					format = NEXUS_VideoFormat_e1080p25hz;
					break;
				case 5:
					format = NEXUS_VideoFormat_e1080p30hz;
					break;
				case 6:
					format = NEXUS_VideoFormat_e1080p50hz;
					break;
				case 7:
					format = NEXUS_VideoFormat_eNtsc;
					break;
				case 8:
					format = NEXUS_VideoFormat_e480p;
					break;
				case 9:
					format = NEXUS_VideoFormat_e576p;
					break;
				case 10:
					format = NEXUS_VideoFormat_e720p;
					break;
				case 11:
					format = NEXUS_VideoFormat_e720p50hz;
					break;
				case 12:
					format = NEXUS_VideoFormat_ePal;
					break;
			}
*/
			SetVideoFormat (0, (NEXUS_VideoFormat) format, 1);
			//g_Config.HdmiSettings.Format=ConvertVideoFormatToKwt(0,format);

			g_Config.HdmiSettings.Format=left_or_right_num_display_settings[0];
			SaveConfigToFlash();

		}



		else if (dPlayBack_focuse == 6)
		{

		}
		else if (dPlayBack_focuse == 5)
		{

		}
		else if (dPlayBack_focuse == 2)
		{
			NEXUS_VideoFormat format = NEXUS_VideoFormat_ePal;
			if(left_or_right_num_display_settings[2]==1)
			{
				format=ConvertVideoFormatToBroadcom(1,2);
				g_Config.CvbsSettings.Format=2;
			}
			else
			{
				format=ConvertVideoFormatToBroadcom(1,0);
				g_Config.CvbsSettings.Format=0;
			}

/*
			switch (left_or_right_num_display_settings[2])
			{
				case 7:
					format = NEXUS_VideoFormat_eNtsc;
					break;
				case 12:
					format = NEXUS_VideoFormat_ePal;
					break;
			}
*/
			SetVideoFormat (1, (NEXUS_VideoFormat) format, 1);
			//g_Config.CvbsSettings.Format=ConvertVideoFormatToKwt(1,format);
			SaveConfigToFlash();
		}
		else if (dPlayBack_focuse == 1)
		{
			NEXUS_DisplayAspectRatio aspectRatio = NEXUS_DisplayAspectRatio_eAuto;
			switch (left_or_right_num_display_settings[1])
			{
				case 0:
					aspectRatio = NEXUS_DisplayAspectRatio_eAuto;
					break;
				case 1:
					aspectRatio = NEXUS_DisplayAspectRatio_e16x9;
					break;
				case 2:
					aspectRatio = NEXUS_DisplayAspectRatio_e4x3;
					break;
			}
			SetVideoAspectRatio (0, 1, (NEXUS_DisplayAspectRatio) aspectRatio);
			g_Config.HdmiSettings.AspectRatio=(unsigned int)aspectRatio;
			SaveConfigToFlash();
		}
		else if (dPlayBack_focuse == 3)
		{
			NEXUS_DisplayAspectRatio aspectRatio = NEXUS_DisplayAspectRatio_eAuto;
			switch (left_or_right_num_display_settings[3])
			{
				case 0:
					aspectRatio = NEXUS_DisplayAspectRatio_eAuto;
					break;
				case 1:
					aspectRatio = NEXUS_DisplayAspectRatio_e16x9;
					break;
				case 2:
					aspectRatio = NEXUS_DisplayAspectRatio_e4x3;
					break;
			}
			SetVideoAspectRatio (1, 1, (NEXUS_DisplayAspectRatio) aspectRatio);
			g_Config.CvbsSettings.AspectRatio=(unsigned int)aspectRatio;
			SaveConfigToFlash();
		}
		else if (dPlayBack_focuse == 4)
		{

		}
	}
	else if (key_code == eKey_Menu)
	{
		DisplaySettingsShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{

	}
	else if (key_code == eKey_Exit)
	{
		DisplaySettingsShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if (key_code == eKey_LeftArrow || key_code == eKey_RightArrow)
	{
		DisplayDisplaySettings (key_code);
	}
	return 0;
}

void CloseDisplaySettingsMenu ()
{
	bwidget_destroy (d_bg);
	for (int i = 0; i < 7; i++)
	{
		bwidget_destroy (display_settings_circle[i]);
		bwidget_destroy (display_settings_left[i]);
		bwidget_destroy (display_settings_right[i]);
		bwidget_destroy (display_settings_label[i]);
		bwidget_destroy (display_settings_button[i]);
	}
	bwidget_destroy (menu_displaysettings[0]);
	bwidget_destroy (menu_displaysettings[1]);
}
