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
#include "global.h"
#include "menu_circlelabel.h"
#include "menu_channeledit.h"
#include "record.h"
#include "menu_generalsettings.h"

void MenuRecordSettingsShow(bool show);

static bwidget_t l_record_bg;
bwidget_t menu_record[2];
bwidget_t record_config_label[4];
bwidget_t record_config_list[4];
bwidget_t record_config_circle[4];
bwidget_t record_config_left[4];
bwidget_t record_config_right[4];

int left_or_right_num_recordconfig[4] = {0, 0, 0, 0};
unsigned char recordconfig_focuse = 0;


char *record_config_media[] = {(char *) ("Disabled"), (char *) ("USB storage"), (char *) ("SD card")};
char *record_config_loop[] = {(char *) ("No"), (char *) ("Yes")};
char *record_config_file_length[] = {(char *) ("100"), (char *) ("300"), (char *) ("500"), (char *) ("1000"),
																(char *) ("2000"), (char *) ("3000"), (char *) ("3500")};
char *record_config_start_up[] = {(char *) ("No"), (char *) ("Yes")};

void MenuRecordSettingsInit(void)
{
	l_record_bg = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50,50,830,815), g_System.Font17,NULL);

	for (int i = 0; i < 4; i++)
	{
		record_config_circle[i] = CreatCirclepicture ("record_config_circle", l_record_bg,
														MRect (185, 120 + i * 40, 30, 30), g_System.Font17, NULL);
		record_config_left[i] = Creatleftpicture ("record_config_left", l_record_bg,
														MRect (435, 120 + i * 40 +5, 30, 20), g_System.Font17, NULL);
		record_config_right[i] = Creatrightpicture ("record_config_right", l_record_bg,
														MRect (600, 120 + i * 40+5, 30, 20), g_System.Font17, NULL);
		record_config_label[i] = CreatLabel ("record_config_label", l_record_bg,
														MRect (240, 120 + i * 40, 180, 30), g_System.Font17, NULL);
		record_config_list[i] = CreatButton ("record_config_list", l_record_bg,
														MRect (430, 120 + i * 40, 190, 30), g_System.Font17, NULL);
	}


	menu_record[0]  =CreatButton("menu_record", l_record_bg, MRect(70,10,120,30), g_System.big_menu, NULL);
	menu_record[1]  =CreatButton("menu_record", l_record_bg, MRect(118,40,180,30), g_System.small_menu, NULL);

	SetButtonText (record_config_label[0], "Storage Media");
	SetButtonText (record_config_label[1], "Loop Record");
	SetButtonText (record_config_label[2], "File Size(MB)");
	SetButtonText (record_config_label[3], "Record on start-up");

	SetButtonText (menu_record[0], "MENU");
	SetButtonText (menu_record[1], "Record Config");


	SetButtonText (record_config_list[0], record_config_media[g_Config.RecSettings.PreferredMedia]);
	SetButtonText (record_config_list[1], record_config_loop[g_Config.RecSettings.LoopRecEnable]);
	SetButtonText (record_config_list[2], record_config_file_length[g_Config.RecSettings.RecFileSize]);
	SetButtonText (record_config_list[3], record_config_start_up[g_Config.RecSettings.RecOnStartEnable]);

	left_or_right_num_recordconfig[0]=g_Config.RecSettings.PreferredMedia;
	left_or_right_num_recordconfig[1]=g_Config.RecSettings.LoopRecEnable;
	left_or_right_num_recordconfig[2]=g_Config.RecSettings.RecFileSize;
	left_or_right_num_recordconfig[3]=g_Config.RecSettings.RecOnStartEnable;

	printf("g_Config.RecSettings.RecOnStartEnable=%d\n",g_Config.RecSettings.RecOnStartEnable);






	for (int i = 0; i < 4; i++)
	{
		SetLabelBackGroundColor (record_config_label[i], 0x85223284);
	}


	SetLabelBackGroundColor (menu_record[0], 0x85223284);
	SetLabelBackGroundColor (menu_record[1], 0x85223284);

    MenuRecordSettingsShow(false);
}



void MenuRecordSettingsShow(bool show)
{
	int ret = 0;
	bwidget_show(menu_record[0],show);
    bwidget_show(menu_record[1],show);
    bwidget_show(l_record_bg,show);

	for (int i = 0; i < 4; i++)
	{
		bwidget_show (record_config_label[i], show);
		bwidget_show (record_config_list[i], show);
	}

	if (show == true)
	{
		for (int i = 0; i < 4; i++)
		{
			if (i == recordconfig_focuse)
			{
				bwidget_show (record_config_circle[i], true);
				bwidget_show (record_config_left[i], true);
				bwidget_show (record_config_right[i], true);
				continue;
			}
			bwidget_show (record_config_circle[i], false);
			bwidget_show (record_config_left[i], false);
			bwidget_show (record_config_right[i], false);
		}
		for (int i = 0; i < 4; i++)
		{
			SetLabelBackGroundColor (record_config_list[i], UNFOCUS_COLOR);
			SetLabelBackGroundColor (record_config_list[recordconfig_focuse], FOCUS_COLOR);
		}
		ret = bwidget_set_focus (record_config_list[recordconfig_focuse]);
		printf ("ret = %d\n", ret);
	}
}

void DisplayRecordConfig (unsigned char key)
{
	switch (recordconfig_focuse)
	{
		case 0:
		{
			if (key == eKey_RightArrow)
			{
				left_or_right_num_recordconfig[0]++;
				if (left_or_right_num_recordconfig[0] == 3)
				{
					left_or_right_num_recordconfig[0] = 0;
				}
			}
			else
			{
				left_or_right_num_recordconfig[0]--;
				if (left_or_right_num_recordconfig[0] == -1)
				{
					left_or_right_num_recordconfig[0] = 2;
				}
			}
			SetButtonText (record_config_list[0], record_config_media[left_or_right_num_recordconfig[0]]);
			break;
		}
		case 1:
		{
			if (key == eKey_RightArrow)
			{
				left_or_right_num_recordconfig[1]++;
				if (left_or_right_num_recordconfig[1] == 2)
				{
					left_or_right_num_recordconfig[1] = 0;
				}
			}
			else
			{
				left_or_right_num_recordconfig[1]--;
				if (left_or_right_num_recordconfig[1] == -1)
				{
					left_or_right_num_recordconfig[1] = 1;
				}
			}
			SetButtonText (record_config_list[1], record_config_loop[left_or_right_num_recordconfig[1]]);
			break;
		}
		case 2:
		{
			if (key == eKey_RightArrow)
			{
				left_or_right_num_recordconfig[2]++;
				if (left_or_right_num_recordconfig[2] == 7)
				{
					left_or_right_num_recordconfig[2] = 0;
				}
			}
			else
			{
				left_or_right_num_recordconfig[2]--;
				if (left_or_right_num_recordconfig[2] == -1)
				{
					left_or_right_num_recordconfig[2] = 6;
				}
			}
			SetButtonText (record_config_list[2], record_config_file_length[left_or_right_num_recordconfig[2]]);
			break;
		}
		case 3:
		{
			if (key == eKey_RightArrow)
			{
				left_or_right_num_recordconfig[3]++;
				if (left_or_right_num_recordconfig[3] == 2)
				{
					left_or_right_num_recordconfig[3] = 0;
				}
			}
			else
			{
				left_or_right_num_recordconfig[3]--;
				if (left_or_right_num_recordconfig[3] == -1)
				{
					left_or_right_num_recordconfig[3] = 1;
				}
			}
			SetButtonText (record_config_list[3], record_config_start_up[left_or_right_num_recordconfig[3]]);
			break;
		}
	}
}

int MenuRecordSettingsConfigKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		recordconfig_focuse++;
		if (recordconfig_focuse == 4)
		{
			recordconfig_focuse = 0;
		}
		bwidget_set_focus (record_config_list[recordconfig_focuse]);
		for (int i = 0; i < 4; i++)
		{
			if (i == recordconfig_focuse)
			{
				bwidget_show (record_config_circle[i], true);
				bwidget_show (record_config_left[i], true);
				bwidget_show (record_config_right[i], true);
				continue;
			}
			bwidget_show (record_config_circle[i], false);
			bwidget_show (record_config_left[i], false);
			bwidget_show (record_config_right[i], false);
		}
	}
	else if (key_code == eKey_UpArrow)
	{
		if (recordconfig_focuse == 0)
		{
			recordconfig_focuse = 3;
		}
		else
		{
			recordconfig_focuse--;
		}
		bwidget_set_focus (record_config_list[recordconfig_focuse]);
		for (int i = 0; i < 4; i++)
		{
			if (i == recordconfig_focuse)
			{
				bwidget_show (record_config_circle[i], true);
				bwidget_show (record_config_left[i], true);
				bwidget_show (record_config_right[i], true);
				continue;
			}
			bwidget_show (record_config_circle[i], false);
			bwidget_show (record_config_left[i], false);
			bwidget_show (record_config_right[i], false);
		}
	}
	else if (key_code == eKey_Select)
	{

		if((g_Config.RecSettings.RecOnStartEnable==0)&&(left_or_right_num_recordconfig[3]==1))
		{
			g_Status.RecNextBootUp=1;
		}


		g_Config.RecSettings.PreferredMedia=left_or_right_num_recordconfig[0];
		g_Config.RecSettings.LoopRecEnable=left_or_right_num_recordconfig[1];
		g_Config.RecSettings.RecFileSize=left_or_right_num_recordconfig[2];
		g_Config.RecSettings.RecOnStartEnable=left_or_right_num_recordconfig[3];



		printf("g_Config.RecSettings.RecOnStartEnable=%d Save\n",g_Config.RecSettings.RecOnStartEnable);


		SaveConfigToFlash();
		MenuRecordSettingsShow (false);
		GeneralSettingsShow (true);
		CurrentMenu = eMenu_GeneralSettings;



	}
	else if (key_code == eKey_Menu)
	{

	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{

	}
	else if (key_code == eKey_Exit)
	{
		MenuRecordSettingsShow (false);
		GeneralSettingsShow (true);
		CurrentMenu = eMenu_GeneralSettings;
	}
	else if (key_code == eKey_LeftArrow || key_code == eKey_RightArrow)
	{
		DisplayRecordConfig (key_code);
	}

	for (int i = 0; i < 4; i++)
	{
		SetLabelBackGroundColor (record_config_list[i], UNFOCUS_COLOR);
		SetLabelBackGroundColor (record_config_list[recordconfig_focuse], FOCUS_COLOR);
	}

	return 0;
}

void CloseMenuRecordSettings(void)
{
	bwidget_destroy(l_record_bg);
	for (int i = 0; i < 4; i++)
	{
		bwidget_destroy (record_config_list[i]);
		bwidget_destroy (record_config_circle[i]);
		bwidget_destroy (record_config_left[i]);
		bwidget_destroy (record_config_right[i]);
		bwidget_destroy (record_config_label[i]);
	}
}

