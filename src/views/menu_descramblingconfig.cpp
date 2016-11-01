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
#include "menu_videochannellist.h"
#include "menu_channeledit.h"
#include "DiB3000M_FrontEnd_HwCfg.h"
#include "front_end_DiB3000M.h"
#include "nexus_i2c.h"
#include "global.h"
#include "menu_circlelabel.h"
#include "menu_descramblingconfig.h"
#include "dib3000.h"

void DescramblingShow (bool show);

bwidget_t descrambling_bg;
bwidget_t descrambling_list[3];
bwidget_t descrambling_label[3];
bwidget_t descrambling_circle[3];
bwidget_t descrambling_title[2];
bwidget_t descrambling_left;
bwidget_t descrambling_right;
unsigned char descrambling_focuse = 0;
int left_or_right_num_descrambling_config = 0;
char *descarambling_type_buffer[] = {(char *) ("Disable"), (char *) ("ABS64"),
														(char *) ("ABS128"), (char *) ("AES128"), (char *) ("AES256")};





int num_key_password_aes256 = 1;
static char buf_password_aes256_temp[40] = {0};
int selected_num_aes256 = 0;
static char buf_password_aes256[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


int num_key_password_aes128 = 1;
static char buf_password_aes128_temp[20] = {0};
int selected_num = 0;
static char buf_password_aes128[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
																			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

int num_key_password_abs128 = 1;
static char buf_password_abs128_temp[20] = {0};
int selected_num_abs128 = 0;
static char buf_password_abs128[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
											0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

int num_key_password_abs64 = 1;
static char buf_password_abs64_temp[6] = {0};
int selected_num_abs64 = 0;
static char buf_password_abs64[4] = {0x00, 0x00, 0x00, 0x00};

int num_key_name = 1;
static char buf_name_temp[6] = {0};
int selected_num_name = 0;
static char buf_name[4] = {0x00, 0x00, 0x00, 0x00};

void DescramblingInit ()
{
	descrambling_bg = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50, 50, 830, 815), g_System.Font17, NULL);
	descrambling_left = Creatleftpicture ("descrambling_left", descrambling_bg,
																	MRect (305, 180 + 5, 30, 20), g_System.Font17, NULL);
	descrambling_right = Creatrightpicture ("networkconfig_right", descrambling_bg,
																	MRect (680, 180 + 5, 30, 20), g_System.Font17, NULL);
	for (int i = 0; i < 3; i++)
	{
		descrambling_circle[i] = CreatCirclepicture ("descrambling_circle", descrambling_bg,
																MRect (130, 180 + i * 40, 30, 30), g_System.Font17, NULL);
		descrambling_label[i] = CreatLabel ("descrambling_label", descrambling_bg,
																MRect (190, 180 + 40 * i, 100, 30), g_System.Font17, NULL);
		descrambling_list[i] = CreatButton ("descrambling_list", descrambling_bg,
																MRect (300, 180 + i * 40, 400, 30), g_System.Font17, NULL);
	}
	descrambling_title[0] = CreatLabel ("descrambling_title", descrambling_bg,
																MRect (70, 10, 120, 30), g_System.big_menu, NULL);
	descrambling_title[1] = CreatLabel ("descrambling_title", descrambling_bg,
																MRect (95, 40, 260, 30), g_System.small_menu, NULL);
	DescramblingShow (false);
	SetLabelText (descrambling_label[0], "Type");
	SetLabelText (descrambling_label[1], "UserName");
	SetLabelText (descrambling_label[2], "PassWord");
	SetButtonText (descrambling_list[0], descarambling_type_buffer[descrambling_focuse]);
	SetButtonText (descrambling_list[1], " ");
	SetButtonText (descrambling_list[2], " ");
	SetLabelText (descrambling_title[0], "MENU");
	SetLabelText (descrambling_title[1], "DescramblingConfig");



	for (int i = 0; i < 32; i++)
	{
		printf ("decryptKeys[%d] = %d\n", i,
				g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[i]);
	}

	if (g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0] == 3)
	{
		left_or_right_num_descrambling_config = 3;
		SetButtonText (descrambling_list[0], descarambling_type_buffer[3]);
	}
	else if (g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0] == 4)
	{
		left_or_right_num_descrambling_config = 4;
		SetButtonText (descrambling_list[0], descarambling_type_buffer[4]);
	}
	else if (g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0] == 2)
	{
		left_or_right_num_descrambling_config = 2;
		SetButtonText (descrambling_list[0], descarambling_type_buffer[2]);
	}
	else if (g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0] == 1)
	{
		left_or_right_num_descrambling_config = 1;
		SetButtonText (descrambling_list[0], descarambling_type_buffer[1]);
	}
	else if (g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0] == 0)
	{
		left_or_right_num_descrambling_config = 0;
		SetButtonText (descrambling_list[0], descarambling_type_buffer[0]);
	}



	for (int i = 0; i < 3; i++)
	{
		SetLabelBackGroundColor (descrambling_label[i], 0x85223284);
	}
	for (int i = 0; i < 2; i++)
	{
		SetLabelBackGroundColor (descrambling_title[i], 0x85223284);
	}
}

void DescramblingShow (bool show)
{
	int ret = 0;
	bwidget_show (descrambling_bg, show);



	SetButtonText (descrambling_list[1], " ");
	SetButtonText (descrambling_list[2], " ");
	num_key_password_aes256 = 1;
	num_key_password_aes128 = 1;
	num_key_password_abs128 = 1;
	num_key_password_abs64 = 1;
	num_key_name = 1;



	for (int i = 0; i < 3; i++)
	{
		bwidget_show (descrambling_list[i], show);
		bwidget_show (descrambling_label[i], show);
	}
	bwidget_show (descrambling_title[0], show);
	bwidget_show (descrambling_title[1], show);
	if (show == true)
	{
		ret = bwidget_set_focus (descrambling_list[descrambling_focuse]);
		for (int i = 0; i < 3; i++)
		{
			SetButtonBackGroundColor (descrambling_list[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (descrambling_list[descrambling_focuse], FOCUS_COLOR);
		}
		for (int i = 0; i < 3; i++)
		{
			if (descrambling_focuse == 0)
			{
				bwidget_show (descrambling_left, true);
				bwidget_show (descrambling_right, true);
			}
			else
			{
				bwidget_show (descrambling_left, false);
				bwidget_show (descrambling_right, false);
			}
			if (i == descrambling_focuse)
			{
				bwidget_show (descrambling_circle[i], true);
				continue;
			}
			bwidget_show (descrambling_circle[i], false);
		}
		printf ("ret = %d\n", ret);
	}
}

void DisplayDescramblingConfig (unsigned char key)
{
	if (descrambling_focuse == 0)
	{
		if (key == eKey_RightArrow)
		{
			left_or_right_num_descrambling_config++;
			if (left_or_right_num_descrambling_config == 5)
			{
				left_or_right_num_descrambling_config = 0;
			}
		}
		else
		{
			left_or_right_num_descrambling_config--;
			if (left_or_right_num_descrambling_config == -1)
			{
				left_or_right_num_descrambling_config = 4;
			}
		}
		SetButtonText (descrambling_list[0], descarambling_type_buffer[left_or_right_num_descrambling_config]);


		SetButtonText (descrambling_list[1], " ");
		SetButtonText (descrambling_list[2], " ");
		num_key_password_aes256 = 1;
		num_key_password_aes128 = 1;
		num_key_password_abs128 = 1;
		num_key_password_abs64 = 1;
		num_key_name = 1;
	}

}

void DisplayUserName (unsigned char key)
{
	switch (num_key_name)
	{
		case 1:
			buf_name[0] = key;
			sprintf (buf_name_temp, "%d", buf_name[0]);
			SetButtonText (descrambling_list[1], buf_name_temp);
			num_key_name = 2;
			selected_num_name = 1;
			break;
		case 2:
			buf_name[1] = key;
			sprintf (buf_name_temp, "%d%d", buf_name[0], buf_name[1]);
			SetButtonText (descrambling_list[1], buf_name_temp);
			num_key_name = 3;
			selected_num_name = 2;
			break;
		case 3:
			buf_name[2] = key;
			sprintf (buf_name_temp, "%d%d%d", buf_name[0], buf_name[1], buf_name[2]);
			SetButtonText (descrambling_list[1], buf_name_temp);
			num_key_name = 4;
			selected_num_name = 3;
			break;
		case 4:
			buf_name[3] = key;
			sprintf (buf_name_temp, "%d%d%d%d", buf_name[0], buf_name[1], buf_name[2], buf_name[3]);
			SetButtonText (descrambling_list[1], buf_name_temp);
			num_key_name = 1;
			selected_num_name = 4;
			break;
		}
}

void DisplayPassWord_ABS64 (unsigned char key)
{
	switch (num_key_password_abs64)
	{
		case 1:
			buf_password_abs64[0] = key;
			sprintf (buf_password_abs64_temp, "%d", buf_password_abs64[0]);
			SetButtonText (descrambling_list[2], buf_password_abs64_temp);
			num_key_password_abs64 = 2;
			selected_num_abs64 = 1;
			break;
		case 2:
			buf_password_abs64[1] = key;
			sprintf (buf_password_abs64_temp, "%d%d", buf_password_abs64[0], buf_password_abs64[1]);
			SetButtonText (descrambling_list[2], buf_password_abs64_temp);
			num_key_password_abs64 = 3;
			selected_num_abs64 = 2;
			break;
		case 3:
			buf_password_abs64[2] = key;
			sprintf (buf_password_abs64_temp, "%d%d%d", buf_password_abs64[0], buf_password_abs64[1],
												buf_password_abs64[2]);
			SetButtonText (descrambling_list[2], buf_password_abs64_temp);
			num_key_password_abs64 = 4;
			selected_num_abs64 = 3;
			break;
		case 4:
			buf_password_abs64[3] = key;
			sprintf (buf_password_abs64_temp, "%d%d%d%d", buf_password_abs64[0], buf_password_abs64[1],
												buf_password_abs64[2], buf_password_abs64[3]);
			SetButtonText (descrambling_list[2], buf_password_abs64_temp);
			num_key_password_abs64 = 1;
			selected_num_abs64 = 4;
			break;
	}
}

void DisplayPassWord_ABS128 (unsigned char key)
{
	switch (num_key_password_abs128)
	{
		case 1:
			buf_password_abs128[0] = key;
			sprintf (buf_password_abs128_temp, "%d", buf_password_abs128[0]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 2;
			selected_num_abs128 = 1;
			break;
		case 2:
			buf_password_abs128[1] = key;
			sprintf (buf_password_abs128_temp, "%d%d", buf_password_abs128[0], buf_password_abs128[1]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 3;
			selected_num_abs128 = 2;
			break;
		case 3:
			buf_password_abs128[2] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 4;
			selected_num_abs128 = 3;
			break;
		case 4:
			buf_password_abs128[3] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2], buf_password_abs128[3]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 5;
			selected_num_abs128 = 4;
			break;
		case 5:
			buf_password_abs128[4] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2], buf_password_abs128[3],
												buf_password_abs128[4]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 6;
			selected_num_abs128 = 5;
			break;
		case 6:
			buf_password_abs128[5] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2], buf_password_abs128[3],
												buf_password_abs128[4], buf_password_abs128[5]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 7;
			selected_num_abs128 = 6;
			break;
		case 7:
			buf_password_abs128[6] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2], buf_password_abs128[3],
												buf_password_abs128[4], buf_password_abs128[5],
												buf_password_abs128[6]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 8;
			selected_num_abs128 = 7;
			break;
		case 8:
			buf_password_abs128[7] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2], buf_password_abs128[3],
												buf_password_abs128[4], buf_password_abs128[5],
												buf_password_abs128[6], buf_password_abs128[7]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 9;
			selected_num_abs128 = 8;
			break;
		case 9:
			buf_password_abs128[8] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d%d%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2], buf_password_abs128[3],
												buf_password_abs128[4], buf_password_abs128[5],
												buf_password_abs128[6], buf_password_abs128[7],
												buf_password_abs128[8]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 10;
			selected_num_abs128 = 9;
			break;
		case 10:
			buf_password_abs128[9] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d%d%d%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2], buf_password_abs128[3],
												buf_password_abs128[4], buf_password_abs128[5],
												buf_password_abs128[6], buf_password_abs128[7],
												buf_password_abs128[8], buf_password_abs128[9]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 11;
			selected_num_abs128 = 10;
			break;
		case 11:
			buf_password_abs128[10] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d%d%d%d%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2], buf_password_abs128[3],
												buf_password_abs128[4], buf_password_abs128[5],
												buf_password_abs128[6], buf_password_abs128[7],
												buf_password_abs128[8], buf_password_abs128[9],
												buf_password_abs128[10]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 12;
			selected_num_abs128 = 11;
			break;
		case 12:
			buf_password_abs128[11] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d%d%d%d%d%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2], buf_password_abs128[3],
												buf_password_abs128[4], buf_password_abs128[5],
												buf_password_abs128[6], buf_password_abs128[7],
												buf_password_abs128[8], buf_password_abs128[9],
												buf_password_abs128[10], buf_password_abs128[11]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 13;
			selected_num_abs128 = 12;
			break;
		case 13:
			buf_password_abs128[12] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d%d%d%d%d%d%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2], buf_password_abs128[3],
												buf_password_abs128[4], buf_password_abs128[5],
												buf_password_abs128[6], buf_password_abs128[7],
												buf_password_abs128[8], buf_password_abs128[9],
												buf_password_abs128[10], buf_password_abs128[11],
												buf_password_abs128[12]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 14;
			selected_num_abs128 = 13;
			break;
		case 14:
			buf_password_abs128[13] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d", buf_password_abs128[0], buf_password_abs128[1],
												buf_password_abs128[2], buf_password_abs128[3],
												buf_password_abs128[4], buf_password_abs128[5],
												buf_password_abs128[6], buf_password_abs128[7],
												buf_password_abs128[8], buf_password_abs128[9],
												buf_password_abs128[10], buf_password_abs128[11],
												buf_password_abs128[12], buf_password_abs128[13]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 15;
			selected_num_abs128 = 14;
			break;
		case 15:
			buf_password_abs128[14] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d", buf_password_abs128[0],
												buf_password_abs128[1], buf_password_abs128[2],
												buf_password_abs128[3], buf_password_abs128[4],
												buf_password_abs128[5], buf_password_abs128[6],
												buf_password_abs128[7], buf_password_abs128[8],
												buf_password_abs128[9], buf_password_abs128[10],
												buf_password_abs128[11], buf_password_abs128[12],
												buf_password_abs128[13], buf_password_abs128[14]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 16;
			selected_num_abs128 = 15;
			break;
		case 16:
			buf_password_abs128[15] = key;
			sprintf (buf_password_abs128_temp, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d", buf_password_abs128[0],
												buf_password_abs128[1], buf_password_abs128[2],
												buf_password_abs128[3], buf_password_abs128[4],
												buf_password_abs128[5], buf_password_abs128[6],
												buf_password_abs128[7], buf_password_abs128[8],
												buf_password_abs128[9], buf_password_abs128[10],
												buf_password_abs128[11], buf_password_abs128[12],
												buf_password_abs128[13], buf_password_abs128[14],
												buf_password_abs128[15]);
			SetButtonText (descrambling_list[2], buf_password_abs128_temp);
			num_key_password_abs128 = 1;
			selected_num_abs128 = 16;
			break;
		default:
			break;
	}

	for (int i = 0; i < 16; i++)
	{
		printf ("buf_password_abs128[%d] = %d\n", i, buf_password_abs128[i]);
	}
}

void DisplayPassWord_AES128 (unsigned char key)
{
	switch (num_key_password_aes128)
	{
		case 1:
			buf_password_aes128[0] = key;
			sprintf (buf_password_aes128_temp, "%d", buf_password_aes128[0]);
			SetButtonText (descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 2;
			selected_num = 1;
			break;
		case 2:
			buf_password_aes128[1] = key;
			sprintf (buf_password_aes128_temp, "%d%d", buf_password_aes128[0], buf_password_aes128[1]);
			SetButtonText (descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 3;
			selected_num = 2;
			break;
		case 3:
			buf_password_aes128[2] = key;
			sprintf (buf_password_aes128_temp, "%d%d%d", buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2]);
			SetButtonText (descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 4;
			selected_num = 3;
			break;
		case 4:
			buf_password_aes128[3] = key;
			sprintf (buf_password_aes128_temp, "%d%d%d%d", buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3]);
			SetButtonText (descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 5;
			selected_num = 4;
			break;
		case 5:
			buf_password_aes128[4] = key;
			sprintf (buf_password_aes128_temp, "%d%d%d%d%d", buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4]);
			SetButtonText (descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 6;
			selected_num = 5;
			break;
		case 6:
			buf_password_aes128[5] = key;
			sprintf (buf_password_aes128_temp, "%d%d%d%d%d%d", buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4], buf_password_aes128[5]);
			SetButtonText (descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 7;
			selected_num = 6;
			break;
		case 7:
			buf_password_aes128[6] = key;
			sprintf (buf_password_aes128_temp, "%d%d%d%d%d%d%d", buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4], buf_password_aes128[5],
														buf_password_aes128[6]);
			SetButtonText (descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 8;
			selected_num = 7;
			break;
		case 8:
			buf_password_aes128[7] = key;
			sprintf (buf_password_aes128_temp, "%d%d%d%d%d%d%d%d", buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4], buf_password_aes128[5],
														buf_password_aes128[6], buf_password_aes128[7]);
			SetButtonText (descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 9;
			selected_num = 8;
			break;
		case 9:
			buf_password_aes128[8] = key;
			sprintf (buf_password_aes128_temp, "%d%d%d%d%d%d%d%d%d", buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4], buf_password_aes128[5],
														buf_password_aes128[6], buf_password_aes128[7],
														buf_password_aes128[8]);
			SetButtonText (descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 10;
			selected_num = 9;
			break;
		case 10:
			buf_password_aes128[9] = key;
			sprintf(buf_password_aes128_temp,"%d%d%d%d%d%d%d%d%d%d",buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4], buf_password_aes128[5],
														buf_password_aes128[6], buf_password_aes128[7],
														buf_password_aes128[8], buf_password_aes128[9]);
			SetButtonText(descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 11;
			selected_num = 10;
			break;
		case 11:
			buf_password_aes128[10] = key;
			sprintf(buf_password_aes128_temp,"%d%d%d%d%d%d%d%d%d%d%d",buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4], buf_password_aes128[5],
														buf_password_aes128[6], buf_password_aes128[7],
														buf_password_aes128[8], buf_password_aes128[9],
														buf_password_aes128[10]);
			SetButtonText(descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 12;
			selected_num = 11;
			break;
		case 12:
			buf_password_aes128[11] = key;
			sprintf(buf_password_aes128_temp,"%d%d%d%d%d%d%d%d%d%d%d%d",buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4], buf_password_aes128[5],
														buf_password_aes128[6], buf_password_aes128[7],
														buf_password_aes128[8], buf_password_aes128[9],
														buf_password_aes128[10], buf_password_aes128[11]);
			SetButtonText(descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 13;
			selected_num = 12;
			break;
		case 13:
			buf_password_aes128[12] = key;
			sprintf(buf_password_aes128_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d",buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4], buf_password_aes128[5],
														buf_password_aes128[6], buf_password_aes128[7],
														buf_password_aes128[8], buf_password_aes128[9],
														buf_password_aes128[10], buf_password_aes128[11],
														buf_password_aes128[12]);
			SetButtonText(descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 14;
			selected_num = 13;
			break;
		case 14:
			buf_password_aes128[13] = key;
			sprintf(buf_password_aes128_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d",buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4], buf_password_aes128[5],
														buf_password_aes128[6], buf_password_aes128[7],
														buf_password_aes128[8], buf_password_aes128[9],
														buf_password_aes128[10], buf_password_aes128[11],
														buf_password_aes128[12], buf_password_aes128[13]);
			SetButtonText(descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 15;
			selected_num = 14;
			break;
		case 15:
			buf_password_aes128[14] = key;
			sprintf(buf_password_aes128_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4], buf_password_aes128[5],
														buf_password_aes128[6], buf_password_aes128[7],
														buf_password_aes128[8], buf_password_aes128[9],
														buf_password_aes128[10], buf_password_aes128[11],
														buf_password_aes128[12], buf_password_aes128[13],
														buf_password_aes128[14]);
			SetButtonText(descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 16;
			selected_num = 15;
			break;
		case 16:
			buf_password_aes128[15] = key;
			sprintf(buf_password_aes128_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",buf_password_aes128[0], buf_password_aes128[1],
														buf_password_aes128[2], buf_password_aes128[3],
														buf_password_aes128[4], buf_password_aes128[5],
														buf_password_aes128[6], buf_password_aes128[7],
														buf_password_aes128[8], buf_password_aes128[9],
														buf_password_aes128[10], buf_password_aes128[11],
														buf_password_aes128[12], buf_password_aes128[13],
														buf_password_aes128[14], buf_password_aes128[15]);
			SetButtonText(descrambling_list[2], buf_password_aes128_temp);
			num_key_password_aes128 = 1;
			selected_num = 16;
			break;
		default:
			break;
	}

	for (int i = 0; i < 16; i++)
	{
		printf ("buf_password_aes128[%d] = %d\n", i, buf_password_aes128[i]);
	}
}

void DisplayPassWord_AES256 (unsigned char key)
{
	switch (num_key_password_aes256)
	{
		case 1:
			buf_password_aes256[0] = key;
			sprintf(buf_password_aes256_temp,"%d",buf_password_aes256[0]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 2;
			selected_num_aes256 = 1;
			break;
		case 2:
			buf_password_aes256[1] = key;
			sprintf(buf_password_aes256_temp,"%d%d",buf_password_aes256[0], buf_password_aes256[1]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 3;
			selected_num_aes256 = 2;
			break;
		case 3:
			buf_password_aes256[2] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 4;
			selected_num_aes256 = 3;
			break;
		case 4:
			buf_password_aes256[3] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 5;
			selected_num_aes256 = 4;
			break;
		case 5:
			buf_password_aes256[4] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 6;
			selected_num_aes256 = 5;
			break;
		case 6:
			buf_password_aes256[5] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 7;
			selected_num_aes256 = 6;
			break;
		case 7:
			buf_password_aes256[6] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 8;
			selected_num_aes256 = 7;
			break;
		case 8:
			buf_password_aes256[7] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 9;
			selected_num_aes256 = 8;
			break;
		case 9:
			buf_password_aes256[8] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 10;
			selected_num_aes256 = 9;
			break;
		case 10:
			buf_password_aes256[9] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 11;
			selected_num_aes256 = 10;
			break;
		case 11:
			buf_password_aes256[10] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 12;
			selected_num_aes256 = 11;
			break;
		case 12:
			buf_password_aes256[11] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 13;
			selected_num_aes256 = 12;
			break;
		case 13:
			buf_password_aes256[12] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 14;
			selected_num_aes256 = 13;
			break;
		case 14:
			buf_password_aes256[13] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 15;
			selected_num_aes256 = 14;
			break;
		case 15:
			buf_password_aes256[14] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 16;
			selected_num_aes256 = 15;
			break;
		case 16:
			buf_password_aes256[15] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 17;
			selected_num_aes256 = 16;
			break;
		case 17:
			buf_password_aes256[16] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 18;
			selected_num_aes256 = 17;
			break;
		case 18:
			buf_password_aes256[17] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 19;
			selected_num_aes256 = 18;
			break;
		case 19:
			buf_password_aes256[18] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 20;
			selected_num_aes256 = 19;
			break;
		case 20:
			buf_password_aes256[19] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 21;
			selected_num_aes256 = 20;
			break;
		case 21:
			buf_password_aes256[20] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 22;
			selected_num_aes256 = 21;
			break;
		case 22:
			buf_password_aes256[21] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20], buf_password_aes256[21]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 23;
			selected_num_aes256 = 22;
			break;
		case 23:
			buf_password_aes256[22] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20], buf_password_aes256[21],
														buf_password_aes256[22]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 24;
			selected_num_aes256 = 23;
			break;
		case 24:
			buf_password_aes256[23] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20], buf_password_aes256[21],
														buf_password_aes256[22], buf_password_aes256[23]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 25;
			selected_num_aes256 = 24;
			break;
		case 25:
			buf_password_aes256[24] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20], buf_password_aes256[21],
														buf_password_aes256[22], buf_password_aes256[23],
														buf_password_aes256[24]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 26;
			selected_num_aes256 = 25;
			break;
		case 26:
			buf_password_aes256[25] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20], buf_password_aes256[21],
														buf_password_aes256[22], buf_password_aes256[23],
														buf_password_aes256[24], buf_password_aes256[25]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 27;
			selected_num_aes256 = 26;
			break;
		case 27:
			buf_password_aes256[26] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20], buf_password_aes256[21],
														buf_password_aes256[22], buf_password_aes256[23],
														buf_password_aes256[24], buf_password_aes256[25],
														buf_password_aes256[26]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 28;
			selected_num_aes256 = 27;
			break;
		case 28:
			buf_password_aes256[27] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20], buf_password_aes256[21],
														buf_password_aes256[22], buf_password_aes256[23],
														buf_password_aes256[24], buf_password_aes256[25],
														buf_password_aes256[26], buf_password_aes256[27]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 29;
			selected_num_aes256 = 28;
			break;
		case 29:
			buf_password_aes256[28] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20], buf_password_aes256[21],
														buf_password_aes256[22], buf_password_aes256[23],
														buf_password_aes256[24], buf_password_aes256[25],
														buf_password_aes256[26], buf_password_aes256[27],
														buf_password_aes256[28]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 30;
			selected_num_aes256 = 29;
			break;
		case 30:
			buf_password_aes256[29] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20], buf_password_aes256[21],
														buf_password_aes256[22], buf_password_aes256[23],
														buf_password_aes256[24], buf_password_aes256[25],
														buf_password_aes256[26], buf_password_aes256[27],
														buf_password_aes256[28], buf_password_aes256[29]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 31;
			selected_num_aes256 = 30;
			break;
		case 31:
			buf_password_aes256[30] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20], buf_password_aes256[21],
														buf_password_aes256[22], buf_password_aes256[23],
														buf_password_aes256[24], buf_password_aes256[25],
														buf_password_aes256[26], buf_password_aes256[27],
														buf_password_aes256[28], buf_password_aes256[29],
														buf_password_aes256[30]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 32;
			selected_num_aes256 = 31;
			break;
		case 32:
			buf_password_aes256[31] = key;
			sprintf(buf_password_aes256_temp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
														buf_password_aes256[0], buf_password_aes256[1],
														buf_password_aes256[2], buf_password_aes256[3],
														buf_password_aes256[4], buf_password_aes256[5],
														buf_password_aes256[6], buf_password_aes256[7],
														buf_password_aes256[8], buf_password_aes256[9],
														buf_password_aes256[10], buf_password_aes256[11],
														buf_password_aes256[12], buf_password_aes256[13],
														buf_password_aes256[14], buf_password_aes256[15],
														buf_password_aes256[16], buf_password_aes256[17],
														buf_password_aes256[18], buf_password_aes256[19],
														buf_password_aes256[20], buf_password_aes256[21],
														buf_password_aes256[22], buf_password_aes256[23],
														buf_password_aes256[24], buf_password_aes256[25],
														buf_password_aes256[26], buf_password_aes256[27],
														buf_password_aes256[28], buf_password_aes256[29],
														buf_password_aes256[30], buf_password_aes256[31]);
			SetButtonText(descrambling_list[2], buf_password_aes256_temp);
			num_key_password_aes256 = 1;
			selected_num_aes256 = 32;
			break;
		default:
			break;
	}

	for (int i = 0; i < 32; i++)
	{
		printf ("buf_password_aes256[%d] = %d\n", i, buf_password_aes256[i]);
	}
}

void SetPassWordABS64 ()
{
	char Descrambling_Uname[4];
	char tmp[4];
	memset (Descrambling_Uname, 0x00, 4);
	memset (tmp, 0x00, 4);

	char tmp_ukey[4];

	char Descrambling_key[32];
	memset (Descrambling_key, 0x00, 32);

	for (int i = 0; i < selected_num_name; i++)
	{
		tmp[i] = 48 + buf_name[i];
	}

	if (selected_num_name == 0)
	{
		memset (Descrambling_Uname, 0x30, sizeof (Descrambling_Uname));
	}
	if (selected_num_name == 1)
	{
		Descrambling_Uname[0] = 0x30;
		Descrambling_Uname[1] = 0x30;
		Descrambling_Uname[2] = 0x30;
		Descrambling_Uname[3] = tmp[0];
	}
	if (selected_num_name == 2)
	{
		Descrambling_Uname[0] = 0x30;
		Descrambling_Uname[1] = 0x30;
		Descrambling_Uname[2] = tmp[0];
		Descrambling_Uname[3] = tmp[1];
	}
	if (selected_num_name == 3)
	{
		Descrambling_Uname[0] = 0x30;
		Descrambling_Uname[1] = tmp[0];
		Descrambling_Uname[2] = tmp[1];
		Descrambling_Uname[3] = tmp[2];
	}
	if (selected_num_name == 4)
	{
		Descrambling_Uname[0] = tmp[0];
		Descrambling_Uname[1] = tmp[1];
		Descrambling_Uname[2] = tmp[2];
		Descrambling_Uname[3] = tmp[3];
	}

	for (int i = 0; i < 4; i++)
	{
		if ((47 < (unsigned char) Descrambling_Uname[i]) && ((unsigned char) Descrambling_Uname[i] < 58))
		{
			tmp_ukey[i] = (char) ((unsigned char) Descrambling_Uname[i] - 48);
		}
		else if ((64 < (unsigned char) Descrambling_Uname[i]) && ((unsigned char) Descrambling_Uname[i] < 71))
		{
			tmp_ukey[i] = (char) ((unsigned char) Descrambling_Uname[i] - 55);
		}
		else if ((96 < (unsigned char) Descrambling_Uname[i]) && ((unsigned char) Descrambling_Uname[i] < 103))
		{
			tmp_ukey[i] = (char) ((unsigned char) Descrambling_Uname[i] - 87);
		}
		else
		{
			break;
		}
	}
	char tmp_key[4];

	for (int i = 0; i < selected_num_abs64; i++)
	{
		tmp[i] = 48 + buf_password_abs64[i];
	}

	if (selected_num_abs64 == 0)
	{
		memset (Descrambling_key, 0x30, sizeof (Descrambling_key));
	}
	if (selected_num_abs64 == 1)
	{
		Descrambling_key[0] = 0x30;
		Descrambling_key[1] = 0x30;
		Descrambling_key[2] = 0x30;
		Descrambling_key[3] = tmp[0];
	}
	if (selected_num_abs64 == 2)
	{
		Descrambling_key[0] = 0x30;
		Descrambling_key[1] = 0x30;
		Descrambling_key[2] = tmp[0];
		Descrambling_key[3] = tmp[1];
	}
	if (selected_num_abs64 == 3)
	{
		Descrambling_key[0] = 0x30;
		Descrambling_key[1] = tmp[0];
		Descrambling_key[2] = tmp[1];
		Descrambling_key[3] = tmp[2];
	}
	if (selected_num_abs64 == 4)
	{
		Descrambling_key[0] = tmp[0];
		Descrambling_key[1] = tmp[1];
		Descrambling_key[2] = tmp[2];
		Descrambling_key[3] = tmp[3];
	}

	for (int i = 0; i < 4; i++)
	{
		if ((47 < (unsigned char) Descrambling_key[i]) && ((unsigned char) Descrambling_key[i] < 58))
		{
			tmp_key[i] = (char) ((unsigned char) Descrambling_key[i] - 48);
		}
		else if ((64 < (unsigned char) Descrambling_key[i]) && ((unsigned char) Descrambling_key[i] < 71))
		{
			tmp_key[i] = (char) ((unsigned char) Descrambling_key[i] - 55);
		}
		else if ((96 < (unsigned char) Descrambling_key[i]) && ((unsigned char) Descrambling_key[i] < 103))
		{
			tmp_key[i] = (char) ((unsigned char) Descrambling_key[i] - 87);
		}
		else
		{
			break;
		}
	}
	printf ("-->  key = 0x%x 0x%x 0x%x 0x%x  --   name = 0x%x 0x%x 0x%x 0x%x  <--",
								Descrambling_key[0], Descrambling_key[1], Descrambling_key[2], Descrambling_key[3],
								Descrambling_Uname[0], Descrambling_Uname[1], Descrambling_Uname[2], Descrambling_Uname[3]);

	Descrambling_key[0] = (char) (((unsigned char) tmp_ukey[0]) * 16 + (unsigned char) tmp_ukey[1]);
	Descrambling_key[1] = (char) (((unsigned char) tmp_ukey[2]) * 16 + (unsigned char) tmp_ukey[3]);
	Descrambling_key[2] = (char) (((unsigned char) tmp_key[0]) * 16 + (unsigned char) tmp_key[1]);
	Descrambling_key[3] = (char) (((unsigned char) tmp_key[2]) * 16 + (unsigned char) tmp_key[3]);

	U8 end = 0x11;
	char setbuf[33];
	memset (setbuf, 0x00, 33);
	setbuf[0] = 1;

	memcpy (&setbuf[1], Descrambling_key, 4);

	U8 Val;
	U32 i;
	for (i = 0; i <= 4; i++)
	{
		Val = *(setbuf + i);
		printf ("val = %x\n", Val);
		NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, DESCRAMBLE_REG_BASE_ADDRESS + i, &Val, 1);
	}
	NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, 4 + 1, &end, 1);
}

void SetPassWordABS128 ()
{
	char aaa[16];
	char bbb[48];
	char pwd_end[4];

	char Descrambling_key[32];
	memset (Descrambling_key, 0x00, 32);

	for (int i = 0; i < selected_num_abs128; i++)
	{
		Descrambling_key[i] = 48 + buf_password_abs128[i];
	}
	memcpy (aaa, Descrambling_key, 16 * sizeof (char));
	int count_pwd = 0;
	int count_pwd1 = 0;

	for (count_pwd = 0; count_pwd < 24; count_pwd++)
	{
		if (aaa[count_pwd] > 0x63)
		{
			count_pwd1++;
		}
		if (aaa[count_pwd] == 0)
		{
			break;
		}
	}
	int length_pwd1;
	length_pwd1 = 2 * count_pwd + count_pwd1;
	int length_pwd2 = length_pwd1 / 2;

	for (int j = 0, w = 0; w < count_pwd; w++)
	{
		if (aaa[w] < 0x64)
		{
			bbb[j] = aaa[w] / 10;
			bbb[j+1] = aaa[w] % 10;
			j = j + 2;
		}
		else
		{
			bbb[j] = aaa[w] / 100;
			bbb[j+1] = (aaa[w] / 10) % 10;
			bbb[j+2] = aaa[w] % 10;
			j = j + 3;
		}
	}
	int skip1 = length_pwd2 / 4 + 1;
	int aa;
	for (aa = 0; length_pwd2 < length_pwd1;)
	{
		pwd_end[aa++] = bbb[length_pwd2];
		length_pwd2 = length_pwd2 + skip1;
	}
	char tmp_key1[4];
	memset (tmp_key1, 0x00, 4);
	if (aa == 1)
	{
		tmp_key1[0] = 0x00;
		tmp_key1[1] = 0x00;
		tmp_key1[2] = 0x00;
		tmp_key1[3] = pwd_end[0];
	}
	if (aa == 2)
	{
		tmp_key1[0] = 0x00;
		tmp_key1[1] = 0x00;
		tmp_key1[2] = pwd_end[0];
		tmp_key1[3] = pwd_end[1];
	}
	if (aa == 3)
	{
		tmp_key1[0] = 0x00;
		tmp_key1[1] = pwd_end[0];
		tmp_key1[2] = pwd_end[1];
		tmp_key1[3] = pwd_end[2];
	}
	if (aa == 4)
	{
		tmp_key1[0] = pwd_end[0];
		tmp_key1[1] = pwd_end[1];
		tmp_key1[2] = pwd_end[2];
		tmp_key1[3] = pwd_end[3];
	}
	Descrambling_key[0] = 0x00;
	Descrambling_key[1] = 0x00;
	Descrambling_key[2] = (char) (((unsigned char) tmp_key1[0]) * 16 + (unsigned char) tmp_key1[1]);
	Descrambling_key[3] = (char) (((unsigned char) tmp_key1[2]) * 16 + (unsigned char) tmp_key1[3]);

	U8 end = 0x11;
	char setbuf[33];
	memset (setbuf, 0x00, 33);
	setbuf[0] = 2;

	memcpy (&setbuf[1], Descrambling_key, 4);

	U8 Val;
	U32 i;
	for (i = 0; i <= 4; i++)
	{
		Val = *(setbuf + i);
		printf ("val = %x\n", Val);
		NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, DESCRAMBLE_REG_BASE_ADDRESS + i, &Val, 1);
	}
	NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, 4 + 1, &end, 1);
}
void SetPassWordAES128 ()
{
	U8 end = 0x11;

	char setbuf[33];
	memset (setbuf, 0x00, 33);
	setbuf[0] = 3;
	char Descrambling_key[32];
	memset (Descrambling_key, 0x00, 32);

	for (int i = 0; i < selected_num; i++)
	{
		Descrambling_key[i] = 48 + buf_password_aes128[i];
	}


	memcpy (&setbuf[1], Descrambling_key, 16);
	memcpy (&g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[1],
																								Descrambling_key, 16);
	g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0] = 3;

	U8 Val;
	U32 i;
	for (i = 0 ; i <= 16; i++)
	{
		Val = *(setbuf + i);
		printf ("val = %x\n", Val);
		NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, DESCRAMBLE_REG_BASE_ADDRESS + i, &Val, 1);
	}
	NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, 16 + 1, &end, 1);

	SaveChannelToFlash ();
}

void SetPassWordAES256 ()
{
	U8 end = 0x11;

	unsigned char setbuf[33];
	memset (setbuf, 0x00, 33);
	setbuf[0] = 4;
	unsigned char Descrambling_key[32];
	memset (Descrambling_key, 0x00, 32);

	printf ("selected_num_aes256 = %d\n", selected_num_aes256);

	for (int i = 0; i < selected_num_aes256; i++)
	{
		Descrambling_key[i] = 48 + buf_password_aes256[i];
	}


	memcpy (&setbuf[1], Descrambling_key, 32);
	memcpy (&g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[1],
																								Descrambling_key, 32);
	g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0] = 4;

	U8 Val;
	U32 i;
	for (i = 0 ; i <= 32; i++)
	{
		Val = *(setbuf + i);
		printf ("val = %x\n", Val);
		NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, DESCRAMBLE_REG_BASE_ADDRESS + i, &Val, 1);
	}
	NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, 32 + 1, &end, 1);


	SaveChannelToFlash ();
}

void SetPassWordDisable ()
{
	U8 end = 0x11;

	char setbuf[33];
	memset (setbuf, 0x00, 33);
	setbuf[0] = 0;
	char Descrambling_key[32];
	memset (Descrambling_key, 0x00, 32);

	memcpy (&setbuf[1], Descrambling_key, 32);
	memcpy (&g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[1],
																								Descrambling_key, 32);
	g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0] = 0;

	U8 Val;
	U32 i;
	for (i = 0 ; i <= 32; i++)
	{
		Val = *(setbuf + i);
		printf ("val = %x\n", Val);
		NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, DESCRAMBLE_REG_BASE_ADDRESS + i, &Val, 1);
	}
	NEXUS_I2c_Write (i2c, D330_I2C_ADDRESS, 32 + 1, &end, 1);

	SaveChannelToFlash ();
}

void Get_D330_Ver(unsigned char * ver)
{
	unsigned char version=0x00;
	NEXUS_I2c_Read(i2c,D330_I2C_ADDRESS,DESCRAMBLE_REG_VER_ADDRESS,&version,1);
	printf("verion=%02x\n",version);
	*ver=version;

}





int MenuDescramblingKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		if (left_or_right_num_descrambling_config != 0)
		{
			if (left_or_right_num_descrambling_config == 1)
			{
				descrambling_focuse++;
				if (descrambling_focuse == 3)
				{
					descrambling_focuse = 0;
				}
			}
			else
			{
				if (descrambling_focuse == 0)
				{
					descrambling_focuse = 2;
				}
				else if (descrambling_focuse == 2)
				{
					descrambling_focuse = 0;
				}
			}
			bwidget_set_focus (descrambling_list[descrambling_focuse]);
			for (int i = 0; i < 3; i++)
			{
				if (descrambling_focuse == 0)
				{
					bwidget_show (descrambling_left, true);
					bwidget_show (descrambling_right, true);
				}
				else
				{
					bwidget_show (descrambling_left, false);
					bwidget_show (descrambling_right, false);
				}
				if (i == descrambling_focuse)
				{
					bwidget_show (descrambling_circle[i], true);
					continue;
				}
				bwidget_show (descrambling_circle[i], false);
			}
		}
	}
	else if (key_code == eKey_UpArrow)
	{
		if (left_or_right_num_descrambling_config != 0)
		{
			if (left_or_right_num_descrambling_config == 1)
			{
				if (descrambling_focuse == 0)
				{
					descrambling_focuse = 2;
				}
				else
				{
					descrambling_focuse--;
				}
			}
			else
			{
				if (descrambling_focuse == 0)
				{
					descrambling_focuse = 2;
				}
				else if (descrambling_focuse == 2)
				{
					descrambling_focuse = 0;
				}
			}
			bwidget_set_focus (descrambling_list[descrambling_focuse]);
			for (int i = 0; i < 3; i++)
			{
				if (descrambling_focuse == 0)
				{
					bwidget_show (descrambling_left, true);
					bwidget_show (descrambling_right, true);
				}
				else
				{
					bwidget_show (descrambling_left, false);
					bwidget_show (descrambling_right, false);
				}
				if (i == descrambling_focuse)
				{
					bwidget_show (descrambling_circle[i], true);
					continue;
				}
				bwidget_show (descrambling_circle[i], false);
			}
		}
	}
	else if (key_code == eKey_Select)
	{
		if (left_or_right_num_descrambling_config == 1)
		{
			if (descrambling_focuse == 2)
			{
				//SetPassWordABS64 ();
			}
		}
		else if (left_or_right_num_descrambling_config == 2)
		{
			if (descrambling_focuse == 2)
			{
				//SetPassWordABS128 ();
			}
		}
		else if (left_or_right_num_descrambling_config == 3)
		{
			if (descrambling_focuse == 2)
			{
				SetPassWordAES128 ();
			}
		}
		else if (left_or_right_num_descrambling_config == 4)
		{
			if (descrambling_focuse == 2)
			{
				SetPassWordAES256 ();
			}
		}
		else if (left_or_right_num_descrambling_config == 0)
		{
			if (descrambling_focuse == 0)
			{
				SetPassWordDisable ();
			}
		}
	}
	else if (key_code == eKey_Menu)
	{
		DescramblingShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{
		if (left_or_right_num_descrambling_config == 1)
		{
			if (descrambling_focuse == 1)
			{
				DisplayUserName (key_code - 0x1d);
			}
			else if (descrambling_focuse == 2)
			{
				DisplayPassWord_ABS64 (key_code - 0x1d);
			}
		}
		else if (left_or_right_num_descrambling_config == 2)
		{
			if (descrambling_focuse == 2)
			{
				DisplayPassWord_ABS128 (key_code - 0x1d);
			}
		}
		else if (left_or_right_num_descrambling_config == 3)
		{
			if (descrambling_focuse == 2)
			{
				DisplayPassWord_AES128 (key_code - 0x1d);
			}
		}
		else if (left_or_right_num_descrambling_config == 4)
		{
			if (descrambling_focuse == 2)
			{
				DisplayPassWord_AES256 (key_code - 0x1d);
			}
		}
	}
	else if (key_code == eKey_Exit)
	{
		DescramblingShow (false);
		ChannelEditShow (true);
		CurrentMenu = eMenu_ChannelEdit;
	}
	else if (key_code == eKey_LeftArrow || key_code == eKey_RightArrow)
	{
		DisplayDescramblingConfig (key_code);
	}
	for (int i = 0; i < 3; i++)
	{
		SetButtonBackGroundColor (descrambling_list[i], UNFOCUS_COLOR);
		SetButtonBackGroundColor (descrambling_list[descrambling_focuse], FOCUS_COLOR);
	}

	return 0;
}

void CloseDescramblingMenu ()
{
	bwidget_destroy (descrambling_bg);
	for (int i = 0; i < 3; i++)
	{
		bwidget_destroy (descrambling_list[i]);
		bwidget_destroy (descrambling_label[i]);
		bwidget_destroy (descrambling_circle[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		bwidget_destroy (descrambling_title[i]);
	}
	bwidget_destroy (descrambling_left);
	bwidget_destroy (descrambling_right);
}

