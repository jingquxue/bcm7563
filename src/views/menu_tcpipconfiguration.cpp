#include "menu_tcpipconfiguration.h"
#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "label.h"
#include "button.h"
#include "menu_main.h"
#include "menu_info.h"
#include "menu_generalsettings.h"
#include "graphics.h"
#include "transmitter.h"
#include "menu_displaysettings.h"
#include "global.h"
#include "menu_circlelabel.h"
#include "menu_networkconfig.h"

void TCPIPConfigurationShow (bool show);

bwidget_t tic;
bwidget_t tcpipconfiguration_list[5];
bwidget_t tcpipconfiguration_circle[5];
bwidget_t tcpipconfiguration_label[5];
bwidget_t tcpipconfiguration_left[3];
bwidget_t tcpipconfiguration_right[3];
bwidget_t tcpipconfiguration_title[2];
int tcpipconfiguration_focuse = 0;
char tcpipconfiguration_underline = 0;
int left_or_right_num_tcpipconfiguration[3]= {0};
char *type_tic_text[] = {(char *) ("Transmitter"), (char *) ("Receiver")};
char *protocal_tic_text[] = {(char *) "UDP", (char *) "Multicast", (char *) "RTP"};
char *status_tic_text[] = {(char *) "Disabled", (char *) "Enabled"};

static char ip_buf[16] = {0};
static char port_buf[8] = {0};
bwidget_t test_line[12];
unsigned char tic_text_ip[12] = {0};
char send_ip_buf[16] = {0};

unsigned char had_enabled = 0;

void TCPIPConfigurationInit ()
{
	tic = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50,50,830,815), g_System.Font17,NULL);

	for (int i = 0; i < 3; i++)
	{
		tcpipconfiguration_left[i] = Creatleftpicture ("tcpipconfiguration_left", tic,
															MRect (405, 120 + i * 40 + 5, 30, 20), g_System.Font17, NULL);
		tcpipconfiguration_right[i] = Creatrightpicture ("tcpipconfiguration_right", tic,
															MRect (580, 120 + i * 40 + 5, 30, 20), g_System.Font17, NULL);
		test_line[i] = CreatLine ("test_line", tic,MRect (426 + i * 12, 251, 6, 30), g_System.Font17, NULL);
		test_line[3+i] = CreatLine ("test_line", tic,MRect (466+i * 12, 251, 6, 30), g_System.Font17, NULL);
		test_line[6+i] = CreatLine ("test_line", tic,MRect (505+12*i, 251, 6, 30), g_System.Font17, NULL);
		test_line[i+9] = CreatLine ("test_line", tic,MRect (544+12*i, 251, 6, 30), g_System.Font17, NULL);
	}

	for (int i = 0; i < 5; i++)
	{
		tcpipconfiguration_circle[i] = CreatCirclepicture ("tcpipconfiguration_circle", tic,
															MRect (250, 120 + i * 40, 30, 30), g_System.Font17, NULL);
		tcpipconfiguration_label[i] = CreatLabel ("tcpipconfiguration_label", tic,
															MRect (300, 120 + 40 * i, 100, 30), g_System.Font17, NULL);
		tcpipconfiguration_list[i] = CreatButton ("tcpipconfiguration_list", tic,
															MRect (400, 120 + i * 40, 200, 30), g_System.Font17, NULL);
	}

	tcpipconfiguration_title[0]  =CreatLabel("tcpipconfiguration_title",   tic,
																	MRect(70,10,120,30), g_System.big_menu, NULL);
	tcpipconfiguration_title[1]  =CreatLabel("tcpipconfiguration_title",   tic,
																	MRect(95,40,180,30), g_System.small_menu, NULL);


	TCPIPConfigurationShow (false);

	SetLabelText (tcpipconfiguration_title[0], "MENU");
	SetLabelText (tcpipconfiguration_title[1], "NetworkConfig");


	SetLabelText (tcpipconfiguration_label[0], "Type");
	SetLabelText (tcpipconfiguration_label[1], "Protocal");
	SetLabelText (tcpipconfiguration_label[2], "Status");
	SetLabelText (tcpipconfiguration_label[3], "IP");
	SetLabelText (tcpipconfiguration_label[4], "Port");

	if (g_Config.NetRelaySettings.Type == 0)
	{
		SetButtonText (tcpipconfiguration_list[0], type_tic_text[0]);
		left_or_right_num_tcpipconfiguration[0] = 0;
	}
	else
	{
		SetButtonText (tcpipconfiguration_list[0], type_tic_text[1]);
		left_or_right_num_tcpipconfiguration[0] = 1;
	}

	if (g_Config.NetRelaySettings.protocal == IpProtocol_eUdp)
	{
		SetButtonText (tcpipconfiguration_list[1], protocal_tic_text[0]);
		left_or_right_num_tcpipconfiguration[1] = 0;
	}
	else if (g_Config.NetRelaySettings.protocal == IpProtocol_eRtp)
	{
		SetButtonText (tcpipconfiguration_list[1], protocal_tic_text[2]);
		left_or_right_num_tcpipconfiguration[1] = 2;
	}

	if (g_Config.NetRelaySettings.Enable == 0)
	{
		SetButtonText (tcpipconfiguration_list[2], status_tic_text[0]);
		left_or_right_num_tcpipconfiguration[2] = 0;
		had_enabled = 0;
	}
	else if (g_Config.NetRelaySettings.Enable == 1)
	{
		SetButtonText (tcpipconfiguration_list[2], status_tic_text[1]);
		left_or_right_num_tcpipconfiguration[2] = 1;
		had_enabled = 1;
	}

	unsigned char tic_ip1 = (g_Config.NetRelaySettings.RemoteIP & 0xff000000) >> (24);
	unsigned char tic_ip2 = (g_Config.NetRelaySettings.RemoteIP & 0xff0000) >> (16);
	unsigned char tic_ip3 = (g_Config.NetRelaySettings.RemoteIP & 0xff00) >> (8);
	unsigned char tic_ip4 = g_Config.NetRelaySettings.RemoteIP & 0xff;
	tic_text_ip[0] = tic_ip1 / 100;
	tic_text_ip[1] = (tic_ip1 % 100) / 10;
	tic_text_ip[2] = (tic_ip1 % 100) % 10;
	tic_text_ip[3] = tic_ip2 / 100;
	tic_text_ip[4] = (tic_ip2 % 100) / 10;
	tic_text_ip[5] = (tic_ip2 % 100) % 10;
	tic_text_ip[6] = tic_ip3 / 100;
	tic_text_ip[7] = (tic_ip3 % 100) / 10;
	tic_text_ip[8] = (tic_ip3 % 100) % 10;
	tic_text_ip[9] = tic_ip4 / 100;
	tic_text_ip[10] = (tic_ip4 % 100) / 10;
	tic_text_ip[11] = (tic_ip4 % 100) % 10;
	sprintf (ip_buf, "%d%d%d.%d%d%d.%d%d%d.%d%d%d", tic_text_ip[0], tic_text_ip[1], tic_text_ip[2],
													tic_text_ip[3], tic_text_ip[4], tic_text_ip[5],
													tic_text_ip[6], tic_text_ip[7], tic_text_ip[8],
													tic_text_ip[9], tic_text_ip[10], tic_text_ip[11]);
	SetButtonText (tcpipconfiguration_list[3], ip_buf);

	sprintf (port_buf, "%u", g_Config.NetRelaySettings.RemotePort);
	SetButtonText (tcpipconfiguration_list[4], port_buf);

	for (int i = 0; i < 5; i++)
	{
		SetLabelBackGroundColor (tcpipconfiguration_label[i], 0x85223284);
	}

	for (int i = 0; i < 2; i++)
	{
		SetLabelBackGroundColor (tcpipconfiguration_title[i], 0x85223284);
	}
}

void TCPIPConfigurationShow (bool show)
{
	int ret = 0;
	bwidget_show (tic, show);

	for (int i = 0; i < 5; i++)
	{
		bwidget_show (tcpipconfiguration_label[i], show);
		bwidget_show (tcpipconfiguration_list[i], show);
	}
	if (show == true)
	{
		ret = bwidget_set_focus (tcpipconfiguration_list[tcpipconfiguration_focuse]);

		for (int i = 0; i < 5; i++)
		{
			if (i == tcpipconfiguration_focuse)
			{
				bwidget_show (tcpipconfiguration_circle[i], true);
				continue;
			}
			bwidget_show (tcpipconfiguration_circle[i], false);
		}

		for (int i = 0; i < 3; i++)
		{
			if (i == tcpipconfiguration_focuse)
			{
				bwidget_show (tcpipconfiguration_left[i], true);
				bwidget_show (tcpipconfiguration_right[i], true);
				continue;
			}
			bwidget_show (tcpipconfiguration_left[i], false);
			bwidget_show (tcpipconfiguration_right[i], false);
		}

		for (int i = 0; i < 5; i++)
		{
			SetButtonBackGroundColor (tcpipconfiguration_list[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (tcpipconfiguration_list[tcpipconfiguration_focuse], FOCUS_COLOR);
		}

		if (tcpipconfiguration_focuse == 3)
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == tcpipconfiguration_underline)
				{
					bwidget_show (test_line[i], true);
					continue;
				}
				bwidget_show (test_line[i], false);
			}
		}
		else
		{
			for (int i = 0; i < 12; i++)
			{
				bwidget_show (test_line[i], false);
			}
		}
		printf ("ret = %d\n", ret);
	}
}


void DisplayTCPIPConfiguration (unsigned char key)
{
	if (tcpipconfiguration_focuse == 0)
	{
		if(left_or_right_num_tcpipconfiguration[0] == 0)
		{

			SetButtonText(tcpipconfiguration_list[0], type_tic_text[1]);
			left_or_right_num_tcpipconfiguration[0] = 1;
		}
		else if(left_or_right_num_tcpipconfiguration[0]== 1)
		{

			SetButtonText(tcpipconfiguration_list[0], type_tic_text[0]);
			left_or_right_num_tcpipconfiguration[0] = 0;
		}
	}
	else if (tcpipconfiguration_focuse == 1)
	{
		if (key == eKey_RightArrow)
		{
			left_or_right_num_tcpipconfiguration[1]++;
			if (left_or_right_num_tcpipconfiguration[1] == 3)
			{
				left_or_right_num_tcpipconfiguration[1] = 0;
			}
		}
		else
		{
			left_or_right_num_tcpipconfiguration[1]--;
			if (left_or_right_num_tcpipconfiguration[1] == -1)
			{
				left_or_right_num_tcpipconfiguration[1] = 2;
			}
		}
		SetButtonText (tcpipconfiguration_list[1], protocal_tic_text[left_or_right_num_tcpipconfiguration[1]]);
	}
	else if (tcpipconfiguration_focuse == 2)
	{
		if(left_or_right_num_tcpipconfiguration[2] == 0)
		{

			SetButtonText(tcpipconfiguration_list[2], status_tic_text[1]);
			left_or_right_num_tcpipconfiguration[2] = 1;
		}
		else if(left_or_right_num_tcpipconfiguration[2]== 1)
		{
			SetButtonText(tcpipconfiguration_list[2], status_tic_text[0]);
			left_or_right_num_tcpipconfiguration[2] = 0;
		}
	}
	else if (tcpipconfiguration_focuse == 3)
	{
		if (key == eKey_RightArrow)
		{
			tcpipconfiguration_underline++;
			if (tcpipconfiguration_underline == 12)
			{
				tcpipconfiguration_underline = 0;
			}
		}
		else
		{
			if (tcpipconfiguration_underline == 0)
			{
				tcpipconfiguration_underline = 11;
			}
			else
			{
				tcpipconfiguration_underline--;
			}
		}
		for (int i = 0; i < 12; i++)
		{
			if (i == tcpipconfiguration_underline)
			{
				bwidget_show (test_line[i], true);
				continue;
			}
			bwidget_show (test_line[i], false);
		}
	}
}


unsigned int port_num_key = 5;
unsigned int port_num = 12345;

void DisplayPort (unsigned char key)
{
	static char port_buf[10] = {0};
	if (port_num_key == 3)
	{
		port_num = port_num * 10 + key;
		sprintf (port_buf, "%d", port_num);
		SetButtonText (tcpipconfiguration_list[4], port_buf);
		port_num_key = 4;
	}
	else if (port_num_key == 1)
	{
		port_num = port_num * 10 + key;
		sprintf (port_buf, "%d", port_num);
		SetButtonText (tcpipconfiguration_list[4], port_buf);
		port_num_key = 2;
	}
	else if (port_num_key == 2)
	{
		port_num = port_num * 10 + key;
		sprintf (port_buf, "%d", port_num);
		SetButtonText (tcpipconfiguration_list[4], port_buf);
		port_num_key = 3;
	}
	else if (port_num_key == 4)
	{
		port_num = port_num * 10 + key;
		sprintf (port_buf, "%d", port_num);
		SetButtonText (tcpipconfiguration_list[4], port_buf);
		port_num_key = 5;
	}
	else if (port_num_key == 5)
	{
		port_num = key;
		sprintf (port_buf, "%d", port_num);
		SetButtonText (tcpipconfiguration_list[4], port_buf);
		port_num_key = 1;
	}
}

void DisplayIP (unsigned char key)
{
	for (int i = 0; i < 12; i++)
	{
		if (i == tcpipconfiguration_underline)
		{
			tic_text_ip[i] = key;
		}
	}
	sprintf (ip_buf, "%d%d%d.%d%d%d.%d%d%d.%d%d%d", tic_text_ip[0], tic_text_ip[1], tic_text_ip[2],
													tic_text_ip[3], tic_text_ip[4], tic_text_ip[5],
													tic_text_ip[6], tic_text_ip[7], tic_text_ip[8],
													tic_text_ip[9], tic_text_ip[10], tic_text_ip[11]);
	SetButtonText (tcpipconfiguration_list[3], ip_buf);

	tcpipconfiguration_underline++;
	if (tcpipconfiguration_underline == 12)
	{
		tcpipconfiguration_underline = 0;
	}
	for (int i = 0; i < 12; i++)
	{
		if (i == tcpipconfiguration_underline)
		{
			bwidget_show (test_line[i], true);
			continue;
		}
		bwidget_show (test_line[i], false);
	}
}

int MenuTCPIPConfigurationKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		tcpipconfiguration_focuse++;
		if (tcpipconfiguration_focuse == 5)
		{
			tcpipconfiguration_focuse = 0;
		}
		bwidget_set_focus (tcpipconfiguration_list[tcpipconfiguration_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (i == tcpipconfiguration_focuse)
			{
				bwidget_show (tcpipconfiguration_circle[i], true);
				continue;
			}
			bwidget_show (tcpipconfiguration_circle[i], false);
		}

		for (int i = 0; i < 3; i++)
		{
			if (i == tcpipconfiguration_focuse)
			{
				bwidget_show (tcpipconfiguration_left[i], true);
				bwidget_show (tcpipconfiguration_right[i], true);
				continue;
			}
			bwidget_show (tcpipconfiguration_left[i], false);
			bwidget_show (tcpipconfiguration_right[i], false);
		}
	if (tcpipconfiguration_focuse == 3)
	{
		for (int i = 0; i < 12; i++)
		{
			if (i == tcpipconfiguration_underline)
			{
				bwidget_show (test_line[i], true);
				continue;
			}
			bwidget_show (test_line[i], false);
		}
	}
	else
	{
		for (int i = 0; i < 12; i++)
		{
			bwidget_show (test_line[i], false);
		}
	}

	}
	else if (key_code == eKey_UpArrow)
	{
		if (tcpipconfiguration_focuse == 0)
		{
			tcpipconfiguration_focuse = 4;
		}
		else
		{
			tcpipconfiguration_focuse--;
		}
		bwidget_set_focus (tcpipconfiguration_list[tcpipconfiguration_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (i == tcpipconfiguration_focuse)
			{
				bwidget_show (tcpipconfiguration_circle[i], true);
				continue;
			}
			bwidget_show (tcpipconfiguration_circle[i], false);
		}

		for (int i = 0; i < 3; i++)
		{
			if (i == tcpipconfiguration_focuse)
			{
				bwidget_show (tcpipconfiguration_left[i], true);
				bwidget_show (tcpipconfiguration_right[i], true);
				continue;
			}
			bwidget_show (tcpipconfiguration_left[i], false);
			bwidget_show (tcpipconfiguration_right[i], false);
		}
	if (tcpipconfiguration_focuse == 3)
	{
		for (int i = 0; i < 12; i++)
		{
			if (i == tcpipconfiguration_underline)
			{
				bwidget_show (test_line[i], true);
				continue;
			}
			bwidget_show (test_line[i], false);
		}
	}
	else
	{
		for (int i = 0; i < 12; i++)
		{
			bwidget_show (test_line[i], false);
		}
	}

	}
	else if (key_code == eKey_Select)
	{
		if (tcpipconfiguration_focuse == 1)
		{
			if (left_or_right_num_tcpipconfiguration[1] == 0)
			{
				g_Config.NetRelaySettings.protocal = IpProtocol_eUdp;
			}
			else if (left_or_right_num_tcpipconfiguration[1] == 2)
			{
				g_Config.NetRelaySettings.protocal = IpProtocol_eRtp;
			}
		}
		else if (tcpipconfiguration_focuse == 4)
		{
			g_Config.NetRelaySettings.RemotePort = port_num;
		}
		else if (tcpipconfiguration_focuse == 3)
		{
			g_Config.NetRelaySettings.RemoteIP = htonl (change_addr (ip_buf));
		}
		else if (tcpipconfiguration_focuse == 2)
		{
			if (left_or_right_num_tcpipconfiguration[2] == 0)
			{
				g_Config.NetRelaySettings.Enable = 0;

				ipStreamerCtx->ipDst->liveStreamingHandle->stop = true;
				had_enabled = 0;
			}
			else if (left_or_right_num_tcpipconfiguration[2] == 1)
			{
				g_Config.NetRelaySettings.Enable = 1;

				{
					if (had_enabled == 0)
					{
						FreeMemTransmitter ();
						TransmitterInit ();
						ipStreamerCtx->ipDst->liveStreamingHandle->stop = true;
						StartTransmitter ();
						had_enabled = 1;
					}
				}
			}
		}
		else if (tcpipconfiguration_focuse == 0)
		{
			if (left_or_right_num_tcpipconfiguration[0] == 0)
			{
				g_Config.NetRelaySettings.Type = 0;
			}
			else if (left_or_right_num_tcpipconfiguration[0] == 1)
			{
				g_Config.NetRelaySettings.Type = 1;
			}
		}
		SaveConfigToFlash ();
	}
	else if (key_code == eKey_Menu)
	{
		TCPIPConfigurationShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{
		if (tcpipconfiguration_focuse == 4)
		{
			DisplayPort (key_code - 0x1d);
		}
		else if (tcpipconfiguration_focuse == 3)
		{
			DisplayIP (key_code - 0x1d);
		}
	}
	else if (key_code == eKey_Exit)
	{
		TCPIPConfigurationShow (false);
		GeneralSettingsShow (true);
		CurrentMenu = eMenu_GeneralSettings;
	}
	else if (key_code == eKey_LeftArrow || key_code == eKey_RightArrow)
	{
		DisplayTCPIPConfiguration (key_code);
	}
	for (int i = 0; i < 5; i++)
	{
		SetButtonBackGroundColor (tcpipconfiguration_list[i], UNFOCUS_COLOR);
		SetButtonBackGroundColor (tcpipconfiguration_list[tcpipconfiguration_focuse], FOCUS_COLOR);
	}


	return 0;
}

void CloseTCPIPConfigurationMenu ()
{
	bwidget_destroy (tic);
	for (int i = 0; i < 5; i++)
	{
		bwidget_destroy (tcpipconfiguration_list[i]);
		bwidget_destroy (tcpipconfiguration_circle[i]);
		bwidget_destroy (tcpipconfiguration_label[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		bwidget_destroy (tcpipconfiguration_left[i]);
		bwidget_destroy (tcpipconfiguration_right[i]);
	}
	for (int i= 0; i < 2; i++)
	{
		bwidget_destroy (tcpipconfiguration_title[i]);
	}

	for (int i= 0; i < 12; i++)
	{
		bwidget_destroy (test_line[i]);
	}



}


