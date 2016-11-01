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
#include "display.h"
#include "menu_main.h"
#include "menu_displaysettings.h"
#include "menu_channeledit.h"
#include "global.h"
#include "menu_circlelabel.h"
#include "menu_duplexaudio.h"
#include "uart.h"
#include "gpio.h"

void DuplexAudioSet (DuplexAudio *m_DuplexAudio);
eRet DuplexAudioGet (DuplexAudio *m_DuplexAudio);
void DuplexAudioShow (bool show);

bwidget_t duplexaudio_bg;
bwidget_t duplex_audio_label[8];
bwidget_t duplex_audio_list[8];
bwidget_t duplex_audio_circle[8];
bwidget_t duplex_audio_left[8];
bwidget_t duplex_audio_right[8];
bwidget_t duplex_audio_title[2];
unsigned char duplexaudio_focuse = 0;
char *sensitivity_buffer[] = {(char *) ("L0"), (char *) ("L1"), (char *) ("L2"),
								(char *) ("L3"), (char *) ("L4"), (char *) ("L5"),
								(char *) ("L6"), (char *) ("L7"), (char *) ("L8"), (char *) ("L9")};
char *audio_in_set_buffer[] = {(char *) ("Normal"), (char *) ("High")};
char *audio_out_set[] = {(char *) ("Balanced"), (char *) ("Unbalanced")};
int left_or_right_num_duplexaudio[8] = {0};

unsigned char FieldValue[8] = {0};

void init_list ()
{
	DuplexAudio DuplexingAudio_Read;
	memset (&DuplexingAudio_Read, 0x00, sizeof (DuplexAudio));
	DuplexAudioGet (&DuplexingAudio_Read);

	if ((0 < DuplexingAudio_Read.ADCdelay) && (DuplexingAudio_Read.ADCdelay <= 60))
	{
		FieldValue[0] = DuplexingAudio_Read.ADCdelay - 1;
	}

	switch (DuplexingAudio_Read.CheckLowNUM)
	{
		case 1023:
			FieldValue[1] = 0;
			break;
		case 500:
			FieldValue[1] = 1;
			break;
		case 200:
			FieldValue[1] = 2;
			break;
		case 50:
			FieldValue[1] = 3;
			break;
		case 30:
			FieldValue[1] = 4;
			break;
		case 20:
			FieldValue[1] = 5;
			break;
		case 15:
			FieldValue[1] = 6;
			break;
		case 10:
			FieldValue[1] = 7;
			break;
		case 5:
			FieldValue[1] = 8;
			break;
		case 4:
			FieldValue[1] = 9;
			break;
		default:
			FieldValue[1] = 0;
			break;
	}

	if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x01)
	{
		FieldValue[2] = 0;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x02)
	{
		FieldValue[2] = 1;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x04)
	{
		FieldValue[2] = 2;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x08)
	{
		FieldValue[2] = 3;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x10)
	{
		FieldValue[2] = 4;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x20)
	{
		FieldValue[2] = 5;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x40)
	{
		FieldValue[2] = 6;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x80)
	{
		FieldValue[2] = 7;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x01 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 8;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x02 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 9;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x04 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 10;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x08 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 11;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x10 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 12;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x20 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 13;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x40 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 14;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x80 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 15;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x01 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 16;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x02 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 17;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x04 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 18;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x08 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 19;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x10 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 20;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x20 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 21;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x40 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 22;
	}
	else if (DuplexingAudio_Read.Ad1 == 0x80 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
	{
		FieldValue[2] = 23;
	}
	else if (DuplexingAudio_Read.Ad1 == 0xFF && DuplexingAudio_Read.Ad2 == 0xFF && DuplexingAudio_Read.Ad3 == 0xFF)
	{
		FieldValue[2] = 24;
	}
	else
	{
		FieldValue[2] = 0;
	}

	if (DuplexingAudio_Read.AudioInSet <= 1)
	{
		FieldValue[3] = DuplexingAudio_Read.AudioInSet;
	}

	if (DuplexingAudio_Read.AudioOutSet <= 1)
	{
		FieldValue[4] = DuplexingAudio_Read.AudioOutSet;
	}

	if (DuplexingAudio_Read.AudioPGA <= 7)
	{
		FieldValue[5] = DuplexingAudio_Read.AudioPGA;
	}

	if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x01)
	{
		FieldValue[6] = 0;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x02)
	{
		FieldValue[6] = 1;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x04)
	{
		FieldValue[6] = 2;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x08)
	{
		FieldValue[6] = 3;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x10)
	{
		FieldValue[6] = 4;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x20)
	{
		FieldValue[6] = 5;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x40)
	{
		FieldValue[6] = 6;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x80)
	{
		FieldValue[6] = 7;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x01 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 8;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x02 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 9;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x04 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 10;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x08 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 11;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x10 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 12;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x20 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 13;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x40 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 14;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x80 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 15;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x01 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 16;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x02 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 17;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x04 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 18;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x08 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 19;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x10 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 20;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x20 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 21;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x40 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 22;
	}
	else if (DuplexingAudio_Read.VOR_Ad1 == 0x80 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
	{
		FieldValue[6] = 23;
	}
	else if (DuplexingAudio_Read.Ad1 == 0xFF && DuplexingAudio_Read.Ad2 == 0xFF && DuplexingAudio_Read.Ad3 == 0xFF)
	{
		FieldValue[6] = 24;
	}
	else
	{
		FieldValue[6] = 0;
	}

	if ((0 < DuplexingAudio_Read.VOT_Channel) && (DuplexingAudio_Read.VOT_Channel <= 64))
	{
		FieldValue[7] = DuplexingAudio_Read.VOT_Channel - 1;
	}

	left_or_right_num_duplexaudio[0] = FieldValue[0] + 1;
	left_or_right_num_duplexaudio[1] = FieldValue[1];
	left_or_right_num_duplexaudio[2] = FieldValue[2] + 1;
	left_or_right_num_duplexaudio[3] = FieldValue[3] + 1;
	left_or_right_num_duplexaudio[4] = FieldValue[4] + 1;
	left_or_right_num_duplexaudio[5] = FieldValue[5];
	left_or_right_num_duplexaudio[6] = FieldValue[6] + 1;
	left_or_right_num_duplexaudio[7] = FieldValue[7] + 1;
}


void set_list_text ()
{
	static char set_list_text_buffer[8][4] = {{0}};

	sprintf (set_list_text_buffer[0], "%d", FieldValue[0]+1);
	SetButtonText (duplex_audio_list[0], set_list_text_buffer[0]);

	sprintf (set_list_text_buffer[1], "L%d", FieldValue[1]);
	SetButtonText (duplex_audio_list[1], set_list_text_buffer[1]);

	sprintf (set_list_text_buffer[2], "%d", FieldValue[2]+1);
	SetButtonText (duplex_audio_list[2], set_list_text_buffer[2]);

	if (FieldValue[3] == 0)
	{
		SetButtonText (duplex_audio_list[3], "Normal");
	}
	else
	{
		SetButtonText (duplex_audio_list[3], "High");
	}

	if (FieldValue[4] == 0)
	{
		SetButtonText (duplex_audio_list[4], "Balanced");
	}
	else
	{
		SetButtonText (duplex_audio_list[4], "Unbalanced");
	}

	sprintf (set_list_text_buffer[5], "%d", FieldValue[5]);
	SetButtonText (duplex_audio_list[5], set_list_text_buffer[5]);

	sprintf (set_list_text_buffer[6], "%d", FieldValue[6]+1);
	SetButtonText (duplex_audio_list[6], set_list_text_buffer[6]);

	sprintf (set_list_text_buffer[7], "%d", FieldValue[7]+1);
	SetButtonText (duplex_audio_list[7], set_list_text_buffer[7]);
}

void DuplexAudioInit ()
{
	duplexaudio_bg = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50,50,830,815), g_System.Font17,NULL);

	for (int i = 0; i < 8; i++)
	{
		duplex_audio_circle[i] = CreatCirclepicture ("duplex_audio_circle", duplexaudio_bg,
														MRect (225, 90 + i * 40, 30, 30), g_System.Font17, NULL);
		duplex_audio_left[i] = Creatleftpicture ("duplex_audio_left", duplexaudio_bg,
														MRect (425, 90 + i * 40 +5, 30, 20), g_System.Font17, NULL);
		duplex_audio_right[i] = Creatrightpicture ("duplex_audio_right", duplexaudio_bg,
														MRect (590, 90 + i * 40+5, 30, 20), g_System.Font17, NULL);
		duplex_audio_label[i] = CreatLabel ("duplex_audio_label", duplexaudio_bg,
														MRect (280, 90 + i * 40, 140, 30), g_System.Font17, NULL);
		duplex_audio_list[i] = CreatButton ("duplex_audio_list", duplexaudio_bg,
														MRect (420, 90 + i * 40, 190, 30), g_System.Font17, NULL);
	}
	duplex_audio_title[0]  =CreatButton("duplex_audio_title", duplexaudio_bg,
																	MRect(70,10,120,30), g_System.big_menu, NULL);
	duplex_audio_title[1]  =CreatButton("duplex_audio_title", duplexaudio_bg,
																	MRect(118,40,160,30), g_System.small_menu, NULL);

	DuplexAudioShow (false);

	SetButtonText (duplex_audio_label[0], "Delay (s)");
	SetButtonText (duplex_audio_label[1], "Sensitivity");
	SetButtonText (duplex_audio_label[2], "T-Address");
	SetButtonText (duplex_audio_label[3], "AudioIn Set");
	SetButtonText (duplex_audio_label[4], "AudioOut Set");
	SetButtonText (duplex_audio_label[5], "AudioIn Gain");
	SetButtonText (duplex_audio_label[6], "R-Address");
	SetButtonText (duplex_audio_label[7], "Channel");

	SetButtonText (duplex_audio_title[0], "MENU");
	SetButtonText (duplex_audio_title[1], "DuplexAudio");


	init_list ();

	set_list_text ();


	for (int i = 0; i < 8; i++)
	{
		SetLabelBackGroundColor (duplex_audio_label[i], 0x85223284);
	}
	SetLabelBackGroundColor (duplex_audio_title[0], 0x85223284);
	SetLabelBackGroundColor (duplex_audio_title[1], 0x85223284);

}

void DuplexAudioShow (bool show)
{
	int ret = 0;
	bwidget_show (duplexaudio_bg, show);
	for (int i = 0; i < 8; i++)
	{
		bwidget_show (duplex_audio_label[i], show);
		bwidget_show (duplex_audio_list[i], show);
	}

	for (int i = 0; i < 2; i++)
	{
		bwidget_show (duplex_audio_title[i], show);
	}

	if (show == true)
	{
		for (int i = 0; i < 8; i++)
		{
			if (i == duplexaudio_focuse)
			{
				bwidget_show (duplex_audio_circle[i], true);
				bwidget_show (duplex_audio_left[i], true);
				bwidget_show (duplex_audio_right[i], true);
				continue;
			}
			bwidget_show (duplex_audio_circle[i], false);
			bwidget_show (duplex_audio_left[i], false);
			bwidget_show (duplex_audio_right[i], false);
		}
		for (int i = 0; i < 8; i++)
		{
			SetLabelBackGroundColor (duplex_audio_list[i], UNFOCUS_COLOR);
			SetLabelBackGroundColor (duplex_audio_list[duplexaudio_focuse], FOCUS_COLOR);
		}
		ret = bwidget_set_focus (duplex_audio_list[duplexaudio_focuse]);
		printf ("ret = %d\n", ret);
	}
}

void DisplayDuplexAudio (unsigned char key)
{
	switch (duplexaudio_focuse)
	{
		case 0:
		{
			static char test_buffer[5] = {0};
			if (key == eKey_RightArrow)
			{
				left_or_right_num_duplexaudio[0]++;
				if (left_or_right_num_duplexaudio[0] == 61)
				{
					left_or_right_num_duplexaudio[0] = 1;
				}
			}
			else
			{
				left_or_right_num_duplexaudio[0]--;
				if (left_or_right_num_duplexaudio[0] == 0)
				{
					left_or_right_num_duplexaudio[0] = 60;
				}
			}
			sprintf (test_buffer, "%d", left_or_right_num_duplexaudio[0]);
			SetButtonText (duplex_audio_list[0], test_buffer);
			break;
		}
		case 1:
		{
			if (key == eKey_RightArrow)
			{
				left_or_right_num_duplexaudio[1]++;
				if (left_or_right_num_duplexaudio[1] == 10)
				{
					left_or_right_num_duplexaudio[1] = 0;
				}
			}
			else
			{
				left_or_right_num_duplexaudio[1]--;
				if (left_or_right_num_duplexaudio[1] == -1)
				{
					left_or_right_num_duplexaudio[1] = 9;
				}
			}
			SetButtonText (duplex_audio_list[1], sensitivity_buffer[left_or_right_num_duplexaudio[1]]);
			break;
		}
		case 2:
		{
			static char test_buffer[5] = {0};
			if (key == eKey_RightArrow)
			{
				left_or_right_num_duplexaudio[2]++;
				if (left_or_right_num_duplexaudio[2] == 26)
				{
					left_or_right_num_duplexaudio[2] = 1;
				}
			}
			else
			{
				left_or_right_num_duplexaudio[2]--;
				if (left_or_right_num_duplexaudio[2] == 0)
				{
					left_or_right_num_duplexaudio[2] = 25;
				}
			}
			sprintf (test_buffer, "%d", left_or_right_num_duplexaudio[2]);
			SetButtonText (duplex_audio_list[2], test_buffer);
			break;
		}
		case 3:
		{
			if (left_or_right_num_duplexaudio[3] == 0)
			{
				SetButtonText (duplex_audio_list[3], audio_in_set_buffer[left_or_right_num_duplexaudio[3]]);
				left_or_right_num_duplexaudio[3] = 1;
			}
			else if (left_or_right_num_duplexaudio[3] == 1)
			{
				SetButtonText (duplex_audio_list[3], audio_in_set_buffer[left_or_right_num_duplexaudio[3]]);
				left_or_right_num_duplexaudio[3] = 0;
			}
			break;
		}
		case 4:
		{
			if (left_or_right_num_duplexaudio[4] == 0)
			{
				SetButtonText (duplex_audio_list[4], audio_out_set[left_or_right_num_duplexaudio[4]]);
				left_or_right_num_duplexaudio[4] = 1;
			}
			else if (left_or_right_num_duplexaudio[4] == 1)
			{
				SetButtonText (duplex_audio_list[4], audio_out_set[left_or_right_num_duplexaudio[4]]);
				left_or_right_num_duplexaudio[4] = 0;
			}
			break;
		}
		case 5:
		{
			static char test_buffer[5] = {0};
			if (key == eKey_RightArrow)
			{
				left_or_right_num_duplexaudio[5]++;
				if (left_or_right_num_duplexaudio[5] == 8)
				{
					left_or_right_num_duplexaudio[5] = 0;
				}
			}
			else
			{
				left_or_right_num_duplexaudio[5]--;
				if (left_or_right_num_duplexaudio[5] == -1)
				{
					left_or_right_num_duplexaudio[5] = 7;
				}
			}
			sprintf (test_buffer, "%d", left_or_right_num_duplexaudio[5]);
			SetButtonText (duplex_audio_list[5], test_buffer);
			break;
		}
		case 6:
		{
			static char test_buffer[5] = {0};
			if (key == eKey_RightArrow)
			{
				left_or_right_num_duplexaudio[6]++;
				if (left_or_right_num_duplexaudio[6] == 26)
				{
					left_or_right_num_duplexaudio[6] = 1;
				}
			}
			else
			{
				left_or_right_num_duplexaudio[6]--;
				if (left_or_right_num_duplexaudio[6] == 0)
				{
					left_or_right_num_duplexaudio[6] = 25;
				}
			}
			sprintf (test_buffer, "%d", left_or_right_num_duplexaudio[6]);
			SetButtonText (duplex_audio_list[6], test_buffer);
			break;
		}
		case 7:
		{
			static char test_buffer[5] = {0};
			if (key == eKey_RightArrow)
			{
				left_or_right_num_duplexaudio[7]++;
				if (left_or_right_num_duplexaudio[7] == 65)
				{
					left_or_right_num_duplexaudio[7] = 1;
				}
			}
			else
			{
				left_or_right_num_duplexaudio[7]--;
				if (left_or_right_num_duplexaudio[7] == 0)
				{
					left_or_right_num_duplexaudio[7] = 64;
				}
			}
			sprintf (test_buffer, "%d", left_or_right_num_duplexaudio[7]);
			SetButtonText (duplex_audio_list[7], test_buffer);
			break;
		}
	}
}


void set_list ()
{
	DuplexAudio DuplexingAudio_Write;

	DuplexingAudio_Write.ADCdelay = FieldValue[0] + 1;

	switch (FieldValue[1])
	{
		case 0:
			DuplexingAudio_Write.CheckLowNUM = 1023;
			break;
		case 1:
			DuplexingAudio_Write.CheckLowNUM = 500;
			break;
		case 2:
			DuplexingAudio_Write.CheckLowNUM = 200;
			break;
		case 3:
			DuplexingAudio_Write.CheckLowNUM = 50;
			break;
		case 4:
			DuplexingAudio_Write.CheckLowNUM = 30;
			break;
		case 5:
			DuplexingAudio_Write.CheckLowNUM = 20;
			break;
		case 6:
			DuplexingAudio_Write.CheckLowNUM = 15;
			break;
		case 7:
			DuplexingAudio_Write.CheckLowNUM = 10;
			break;
		case 8:
			DuplexingAudio_Write.CheckLowNUM = 5;
			break;
		case 9:
			DuplexingAudio_Write.CheckLowNUM = 4;
			break;
		default:
			DuplexingAudio_Write.CheckLowNUM = 4;
			break;
	}

	switch (FieldValue[2])
	{
		case 0:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x01;
			break;
		}
		case 1:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x02;
			break;
		}
		case 2:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x04;
			break;
		}
		case 3:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x08;
			break;
		}
		case 4:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x10;
			break;
		}
		case 5:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x20;
			break;
		}
		case 6:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x40;
			break;
		}
		case 7:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x80;
			break;
		}
		case 8:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x01;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 9:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x02;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 10:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x04;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 11:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x08;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 12:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x10;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 13:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x20;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 14:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x40;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 15:
		{
			DuplexingAudio_Write.Ad1 = 0x00;
			DuplexingAudio_Write.Ad2 = 0x80;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 16:
		{
			DuplexingAudio_Write.Ad1 = 0x01;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 17:
		{
			DuplexingAudio_Write.Ad1 = 0x02;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 18:
		{
			DuplexingAudio_Write.Ad1 = 0x04;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 19:
		{
			DuplexingAudio_Write.Ad1 = 0x08;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 20:
		{
			DuplexingAudio_Write.Ad1 = 0x10;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 21:
		{
			DuplexingAudio_Write.Ad1 = 0x20;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 22:
		{
			DuplexingAudio_Write.Ad1 = 0x40;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 23:
		{
			DuplexingAudio_Write.Ad1 = 0x80;
			DuplexingAudio_Write.Ad2 = 0x00;
			DuplexingAudio_Write.Ad3 = 0x00;
			break;
		}
		case 24:
		{
			DuplexingAudio_Write.Ad1 = 0xFF;
			DuplexingAudio_Write.Ad2 = 0xFF;
			DuplexingAudio_Write.Ad3 = 0xFF;
			break;
		}
		default:
		{
			DuplexingAudio_Write.Ad1 = 0xFF;
			DuplexingAudio_Write.Ad2 = 0xFF;
			DuplexingAudio_Write.Ad3 = 0xFF;
			break;
		}
	}

	DuplexingAudio_Write.AudioInSet = FieldValue[3];
	DuplexingAudio_Write.AudioOutSet = FieldValue[4];
	DuplexingAudio_Write.AudioPGA = FieldValue[5];

	switch (FieldValue[6])
	{
		case 0:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x01;
			break;
		}
		case 1:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x02;
			break;
		}
		case 2:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x04;
			break;
		}
		case 3:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x08;
			break;
		}
		case 4:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x10;
			break;
		}
		case 5:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x20;
			break;
		}
		case 6:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x40;
			break;
		}
		case 7:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x80;
			break;
		}
		case 8:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x01;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 9:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x02;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 10:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x04;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 11:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x08;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 12:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x10;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 13:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x20;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 14:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x40;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 15:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x00;
			DuplexingAudio_Write.VOR_Ad2 = 0x80;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 16:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x01;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 17:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x02;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 18:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x04;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 19:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x08;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 20:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x10;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 21:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x20;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 22:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x40;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		case 23:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x80;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
		default:
		{
			DuplexingAudio_Write.VOR_Ad1 = 0x80;
			DuplexingAudio_Write.VOR_Ad2 = 0x00;
			DuplexingAudio_Write.VOR_Ad3 = 0x00;
			break;
		}
	}
	DuplexingAudio_Write.VOT_Channel = FieldValue[7] + 1;

	DuplexAudioSet (&DuplexingAudio_Write);
}

void set_field_value ()
{
	FieldValue[0] = left_or_right_num_duplexaudio[0] - 1;
	FieldValue[1] = left_or_right_num_duplexaudio[0];
	FieldValue[2] = left_or_right_num_duplexaudio[2] - 1;
	FieldValue[3] = left_or_right_num_duplexaudio[3];
	FieldValue[4] = left_or_right_num_duplexaudio[4];
	FieldValue[5] = left_or_right_num_duplexaudio[5];
	FieldValue[6] = left_or_right_num_duplexaudio[6] - 1;
	FieldValue[7] = left_or_right_num_duplexaudio[7] - 1;
}

int MenuDuplexAudioKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		duplexaudio_focuse++;
		if (duplexaudio_focuse == 8)
		{
			duplexaudio_focuse = 0;
		}
		bwidget_set_focus (duplex_audio_list[duplexaudio_focuse]);
		for (int i = 0; i < 8; i++)
		{
			if (i == duplexaudio_focuse)
			{
				bwidget_show (duplex_audio_circle[i], true);
				bwidget_show (duplex_audio_left[i], true);
				bwidget_show (duplex_audio_right[i], true);
				continue;
			}
			bwidget_show (duplex_audio_circle[i], false);
			bwidget_show (duplex_audio_left[i], false);
			bwidget_show (duplex_audio_right[i], false);
		}
	}
	else if (key_code == eKey_UpArrow)
	{
		if (duplexaudio_focuse == 0)
		{
			duplexaudio_focuse = 7;
		}
		else
		{
			duplexaudio_focuse--;
		}
		bwidget_set_focus (duplex_audio_list[duplexaudio_focuse]);
		for (int i = 0; i < 8; i++)
		{
			if (i == duplexaudio_focuse)
			{
				bwidget_show (duplex_audio_circle[i], true);
				bwidget_show (duplex_audio_left[i], true);
				bwidget_show (duplex_audio_right[i], true);
				continue;
			}
			bwidget_show (duplex_audio_circle[i], false);
			bwidget_show (duplex_audio_left[i], false);
			bwidget_show (duplex_audio_right[i], false);
		}
	}
	else if (key_code == eKey_Select)
	{
		set_field_value ();
		set_list ();
	}
	else if (key_code == eKey_Menu)
	{
		DuplexAudioShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{

	}
	else if (key_code == eKey_Exit)
	{
		DuplexAudioShow (false);
		ChannelEditShow (true);
		CurrentMenu = eMenu_ChannelEdit;
	}
	else if (key_code == eKey_LeftArrow || key_code == eKey_RightArrow)
	{
		DisplayDuplexAudio (key_code);
	}
	for (int i = 0; i < 8; i++)
	{
		SetLabelBackGroundColor (duplex_audio_list[i], UNFOCUS_COLOR);
		SetLabelBackGroundColor (duplex_audio_list[duplexaudio_focuse], FOCUS_COLOR);
	}

	return 0;
}

void CloseDuplexAudioMenu ()
{
	bwidget_destroy (duplexaudio_bg);

	for (int i = 0; i < 8; i++)
	{
		bwidget_destroy (duplex_audio_label[i]);
		bwidget_destroy (duplex_audio_list[i]);
		bwidget_destroy (duplex_audio_circle[i]);
		bwidget_destroy (duplex_audio_left[i]);
		bwidget_destroy (duplex_audio_right[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		bwidget_destroy (duplex_audio_title[i]);
	}
}

eRet DuplexAudioGet (DuplexAudio *m_DuplexAudio)
{
	char Buffer[32];
	unsigned int uart_written;
	unsigned int uart_timeout = 100;
	unsigned int Read=0;
	int i = 0;
	unsigned short Checksum = 0;
	memset (Buffer, 0, 32);
	sprintf ((char *) Buffer, "%s", "VOTFR");

	PIO_BIT_Set (5, 4, true);

	UART_Write (UART0, Buffer, strlen((char *) Buffer), &uart_written, uart_timeout);

	memset (Buffer, 0, 32);

	PIO_BIT_Set (5, 4, false);

	UART_Flush (UART0);

	UART_Read (UART0, Buffer, 19, &Read, 1000000);
	//printf("Read=%d\n",Read);
	if (Read == 19)
	{
		//printf("Read=19\n");
		if (strncmp ((char *) Buffer, "VOTF", 4) == 0)
		{
			for (i = 4; i < 17; i++)
			{
				Checksum += Buffer[i];
			}

			if (Checksum == (Buffer[Read-2] * 256 + Buffer[Read-1]))
			{
				m_DuplexAudio->ADCdelay = Buffer[4];
				m_DuplexAudio->CheckLowNUM = Buffer[5] * 256 + Buffer[6];
				m_DuplexAudio->Ad1 = Buffer[7];
				m_DuplexAudio->Ad2 = Buffer[8];
				m_DuplexAudio->Ad3 = Buffer[9];
				m_DuplexAudio->AudioInSet = Buffer[10];
				m_DuplexAudio->AudioOutSet = Buffer[11];
				m_DuplexAudio->AudioPGA= Buffer[12];
				m_DuplexAudio->VOR_Ad1 = Buffer[13];
				m_DuplexAudio->VOR_Ad2 = Buffer[14];
				m_DuplexAudio->VOR_Ad3 = Buffer[15];
				m_DuplexAudio->VOT_Channel = Buffer[16];
				return eRet_Ok;
			}
			else
			{
				return eRet_InvalidParameter;
			}
		}
		else
		{
			return eRet_InvalidParameter;
		}
	}
	else
	{
		return eRet_InvalidParameter;
	}
}

void DuplexAudioSet (DuplexAudio *m_DuplexAudio)
{
	char Buffer[32];
	unsigned int uart_written;
	unsigned int uart_timeout = 100;
	unsigned int Read;
	int i = 0;
	unsigned short Checksum = 0x00;

	memset (Buffer, 0, 32);
	sprintf ((char *) Buffer, "%s", "VOTG");

	Buffer[4] = m_DuplexAudio->ADCdelay ;
	Buffer[5] = m_DuplexAudio->CheckLowNUM / 256;
	Buffer[6] = m_DuplexAudio->CheckLowNUM % 256;
	Buffer[7] = m_DuplexAudio->Ad1;
	Buffer[8] = m_DuplexAudio->Ad2;
	Buffer[9] = m_DuplexAudio->Ad3;
	Buffer[10] = m_DuplexAudio->AudioInSet;
	Buffer[11] = m_DuplexAudio->AudioOutSet;
	Buffer[12] = m_DuplexAudio->AudioPGA;
	Buffer[13] = m_DuplexAudio->VOR_Ad1;
	Buffer[14] = m_DuplexAudio->VOR_Ad2;
	Buffer[15] = m_DuplexAudio->VOR_Ad3;
	Buffer[16] = m_DuplexAudio->VOT_Channel;

	for (i = 4; i < 17; i++)
	{
		Checksum += Buffer[i];
	}

	Buffer[17] = Checksum / 256;
	Buffer[18] = Checksum % 256;

	PIO_BIT_Set (5, 4, true);

	UART_Write (UART0, Buffer, 19, &uart_written, uart_timeout);

	PIO_BIT_Set (5, 4, false);

	UART_Flush (UART0);

	UART_Read (UART0, Buffer, 5, &Read, 1000);

	if (strncmp ((char *) Buffer, "OK", 2) == 0)
	{
		printf ("DuplexAudioSet Success\n");
	}
	else if (strncmp ((char *) Buffer, "ERROR", 5) == 0)
	{
		printf ("DuplexAudioSet Error\n");
	}
	else
	{
		printf ("Read: %x\n", Read);
	}
}

