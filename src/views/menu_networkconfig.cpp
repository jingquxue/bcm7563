#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "menu_networkconfig.h"
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
#include <sys/time.h>
#include "nandflash.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "global.h"
#include "menu_circlelabel.h"

static char ip_nc_buf[16] = {0};
static char mask_nc_buf[16] = {0};
static char gateway_nc_buf[16] = {0};
static char mac_buffer[20] = {0};
unsigned char nc_text_ip[12] = {0};
bwidget_t nc_test_line[12];
unsigned char nc_text_mask[12] = {0};
bwidget_t nc_test_line_mask[12];
unsigned char nc_text_gateway[12] = {0};
bwidget_t nc_test_line_gateway[12];

void NetworkConfigShow (bool show);

bwidget_t nc;
bwidget_t networkconfig_list[5];
bwidget_t networkconfig_circle[5];
bwidget_t networkconfig_label[5];
bwidget_t networkconfig_title[2];
bwidget_t networkconfig_left;
bwidget_t networkconfig_right;
unsigned char nnetworkconfig_focuse = 0;
unsigned char nnetworkconfig_underline = 0;
unsigned char nnetworkconfig_underline_mask = 0;
unsigned char nnetworkconfig_underline_gateway = 0;
unsigned int left_or_right_num_ip = 0;
char *ip_pattern_text[] = {(char *) ("Custom"), (char *) ("Automatically")};

void GetMAC ()
{
	struct ifreq ifreq;
	int sock;

	if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror ("socket");
		return;
	}
	strcpy (ifreq.ifr_name, "eth0");
	if (ioctl (sock, SIOCGIFHWADDR, &ifreq) < 0)
	{
		perror ("ioctl");
		close (sock);
		return;
	}
	sprintf (mac_buffer, "%02x:%02x:%02x:%02x:%02x:%02x", (unsigned char) ifreq.ifr_hwaddr.sa_data[0],
													(unsigned char) ifreq.ifr_hwaddr.sa_data[1],
													(unsigned char) ifreq.ifr_hwaddr.sa_data[2],
													(unsigned char) ifreq.ifr_hwaddr.sa_data[3],
													(unsigned char) ifreq.ifr_hwaddr.sa_data[4],
													(unsigned char) ifreq.ifr_hwaddr.sa_data[5]);
	close (sock);
}


unsigned int TCPIP_SetConfig(unsigned int ID, char *ip, char *netmask, char *gateway)
{
	unsigned int ErrCode = 0;
	char ethName[5] = {0};
	char cmd[100] = {0};

	sprintf (ethName, "eth%d", (int) ID);

	strcpy (cmd, "ifconfig ");
	strcat (cmd, ethName);
	strcat (cmd, " ");
	strcat (cmd, ip);
	strcat (cmd, " netmask ");
	strcat (cmd, netmask);
	strcat (cmd, " up");
	printf ("%s\n", cmd);
	system (cmd);

	memset (cmd, 0, 100);
	strcpy (cmd, "route del default");
	printf ("%s\n", cmd);
	system (cmd);

	memset (cmd, 0, 100);
	strcpy (cmd, "route add default gw ");
	strcat (cmd, gateway);
	printf ("%s\n", cmd);
	system (cmd);

	return ErrCode;
}


unsigned int change_addr (const char *str)
{
	unsigned int address = 0;
	unsigned int a, b, c, d;
	sscanf (str, "%u.%u.%u.%u", &a, &b, &c, &d);

	address |= d << 24;
	address |= c << 16;
	address |= b << 8;
	address |= a;

	return address;
}


void InitIPMaskGateway ()
{
	unsigned char ip1 = (g_Config.NetWorkSettings.IPAddr & 0xff000000) >> (24);
	unsigned char ip2 = (g_Config.NetWorkSettings.IPAddr & 0xff0000) >> (16);
	unsigned char ip3 = (g_Config.NetWorkSettings.IPAddr & 0xff00) >> (8);
	unsigned char ip4 = g_Config.NetWorkSettings.IPAddr & 0xff;
	nc_text_ip[0] = ip1 / 100;
	nc_text_ip[1] = (ip1 % 100) / 10;
	nc_text_ip[2] = (ip1 % 100) % 10;
	nc_text_ip[3] = ip2 / 100;
	nc_text_ip[4] = (ip2 % 100) / 10;
	nc_text_ip[5] = (ip2 % 100) % 10;
	nc_text_ip[6] = ip3 / 100;
	nc_text_ip[7] = (ip3 % 100) / 10;
	nc_text_ip[8] = (ip3 % 100) % 10;
	nc_text_ip[9] = ip4 / 100;
	nc_text_ip[10] = (ip4 % 100) / 10;
	nc_text_ip[11] = (ip4 % 100) % 10;
	sprintf (ip_nc_buf, "%d%d%d.%d%d%d.%d%d%d.%d%d%d", nc_text_ip[0], nc_text_ip[1], nc_text_ip[2],
													nc_text_ip[3], nc_text_ip[4], nc_text_ip[5],
													nc_text_ip[6], nc_text_ip[7], nc_text_ip[8],
													nc_text_ip[9], nc_text_ip[10], nc_text_ip[11]);

	unsigned char mask1 = (g_Config.NetWorkSettings.NetMask & 0xff000000) >> (24);
	unsigned char mask2 = (g_Config.NetWorkSettings.NetMask & 0xff0000) >> (16);
	unsigned char mask3 = (g_Config.NetWorkSettings.NetMask & 0xff00) >> (8);
	unsigned char mask4 = g_Config.NetWorkSettings.NetMask & 0xff;
	nc_text_mask[0] = mask1 / 100;
	nc_text_mask[1] = (mask1 % 100) / 10;
	nc_text_mask[2] = (mask1 % 100) % 10;
	nc_text_mask[3] = mask2 / 100;
	nc_text_mask[4] = (mask2 % 100) / 10;
	nc_text_mask[5] = (mask2 % 100) % 10;
	nc_text_mask[6] = mask3 / 100;
	nc_text_mask[7] = (mask3 % 100) / 10;
	nc_text_mask[8] = (mask3 % 100) % 10;
	nc_text_mask[9] = mask4 / 100;
	nc_text_mask[10] = (mask4 % 100) / 10;
	nc_text_mask[11] = (mask4 % 100) % 10;
	sprintf (mask_nc_buf, "%d%d%d.%d%d%d.%d%d%d.%d%d%d", nc_text_mask[0], nc_text_mask[1], nc_text_mask[2],
													nc_text_mask[3], nc_text_mask[4], nc_text_mask[5],
													nc_text_mask[6], nc_text_mask[7], nc_text_mask[8],
													nc_text_mask[9], nc_text_mask[10], nc_text_mask[11]);

	unsigned char gateway1 = (g_Config.NetWorkSettings.GateWay & 0xff000000) >> (24);
	unsigned char gateway2 = (g_Config.NetWorkSettings.GateWay & 0xff0000) >> (16);
	unsigned char gateway3 = (g_Config.NetWorkSettings.GateWay & 0xff00) >> (8);
	unsigned char gateway4 = g_Config.NetWorkSettings.GateWay & 0xff;
	nc_text_gateway[0] = gateway1 / 100;
	nc_text_gateway[1] = (gateway1 % 100) / 10;
	nc_text_gateway[2] = (gateway1 % 100) % 10;
	nc_text_gateway[3] = gateway2 / 100;
	nc_text_gateway[4] = (gateway2 % 100) / 10;
	nc_text_gateway[5] = (gateway2 % 100) % 10;
	nc_text_gateway[6] = gateway3 / 100;
	nc_text_gateway[7] = (gateway3 % 100) / 10;
	nc_text_gateway[8] = (gateway3 % 100) % 10;
	nc_text_gateway[9] = gateway4 / 100;
	nc_text_gateway[10] = (gateway4 % 100) / 10;
	nc_text_gateway[11] = (gateway4 % 100) % 10;
	sprintf (gateway_nc_buf, "%d%d%d.%d%d%d.%d%d%d.%d%d%d", nc_text_gateway[0], nc_text_gateway[1], nc_text_gateway[2],
													nc_text_gateway[3], nc_text_gateway[4], nc_text_gateway[5],
													nc_text_gateway[6], nc_text_gateway[7], nc_text_gateway[8],
													nc_text_gateway[9], nc_text_gateway[10], nc_text_gateway[11]);
}

void NetworkConfigInit ()
{
	nc = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50,50,830,815), g_System.Font17,NULL);

	networkconfig_left = Creatleftpicture ("networkconfig_left", nc,
													MRect (405, 140 + 5, 30, 20), g_System.Font17, NULL);
	networkconfig_right = Creatrightpicture ("networkconfig_right", nc,
													MRect (580, 140 + 5, 30, 20), g_System.Font17, NULL);
	for (int i = 0; i < 3; i++)
	{
		nc_test_line[i] = CreatLine ("nc_test_line", nc,MRect (426 + i * 12, 191, 6, 30), g_System.Font17, NULL);
		nc_test_line[3+i] = CreatLine ("nc_test_line", nc,MRect (466+i * 12, 191, 6, 30), g_System.Font17, NULL);
		nc_test_line[6+i] = CreatLine ("nc_test_line", nc,MRect (505+12*i, 191, 6, 30), g_System.Font17, NULL);
		nc_test_line[i+9] = CreatLine ("nc_test_line", nc,MRect (544+12*i, 191, 6, 30), g_System.Font17, NULL);

		nc_test_line_mask[i] = CreatLine ("nc_test_line_mask", nc,MRect (426 + i * 12, 231, 6, 30), g_System.Font17, NULL);
		nc_test_line_mask[3+i] = CreatLine ("nc_test_line_mask", nc,MRect (466+i * 12, 231, 6, 30), g_System.Font17, NULL);
		nc_test_line_mask[6+i] = CreatLine ("nc_test_line_mask", nc,MRect (505+12*i, 231, 6, 30), g_System.Font17, NULL);
		nc_test_line_mask[i+9] = CreatLine ("nc_test_line_mask", nc,MRect (544+12*i, 231, 6, 30), g_System.Font17, NULL);

		nc_test_line_gateway[i] = CreatLine ("nc_test_line_gateway", nc,MRect (426 + i * 12, 271, 6, 30), g_System.Font17, NULL);
		nc_test_line_gateway[3+i] = CreatLine ("nc_test_line_gateway", nc,MRect (466+i * 12, 271, 6, 30), g_System.Font17, NULL);
		nc_test_line_gateway[6+i] = CreatLine ("nc_test_line_gateway", nc,MRect (505+12*i, 271, 6, 30), g_System.Font17, NULL);
		nc_test_line_gateway[i+9] = CreatLine ("nc_test_line_gateway", nc,MRect (544+12*i, 271, 6, 30), g_System.Font17, NULL);
	}

	for (int i = 0; i < 5; i++)
	{
		networkconfig_circle[i] = CreatCirclepicture ("networkconfig_circle", nc,
													MRect (240, 140 + i * 40, 30, 30), g_System.Font17, NULL);
		networkconfig_label[i] = CreatLabel ("networkconfig_label", nc,
													MRect (290, 140 + 40 * i, 100, 30), g_System.Font17, NULL);
		networkconfig_list[i] = CreatButton ("networkconfig_list", nc,
													MRect (400, 140 + i * 40, 200, 30), g_System.Font17, NULL);
	}

	networkconfig_title[0]  =CreatLabel("networkconfig_title",   nc,
																	MRect(70,10,120,30), g_System.big_menu, NULL);
	networkconfig_title[1]  =CreatLabel("networkconfig_title",   nc,
																	MRect(95,40,160,30), g_System.small_menu, NULL);

	NetworkConfigShow (false);

	SetLabelText (networkconfig_label[0], "IP Pattern");
	SetLabelText (networkconfig_label[1], "IP");
	SetLabelText (networkconfig_label[2], "Mask");
	SetLabelText (networkconfig_label[3], "Gateway");
	SetLabelText (networkconfig_label[4], "MAC");


	printf ("g_Config.NetWorkSettings.DHCPEnable = %d\n", g_Config.NetWorkSettings.DHCPEnable);
	if (g_Config.NetWorkSettings.DHCPEnable == 0)
	{
		SetButtonText (networkconfig_list[0], ip_pattern_text[0]);
		left_or_right_num_ip = 0;
	}
	else
	{
		SetButtonText (networkconfig_list[0], ip_pattern_text[1]);
		left_or_right_num_ip = 1;
	}

	InitIPMaskGateway ();
	SetButtonText (networkconfig_list[1], ip_nc_buf);
	SetButtonText (networkconfig_list[2], mask_nc_buf);
	SetButtonText (networkconfig_list[3], gateway_nc_buf);

	SetLabelText (networkconfig_title[0], "MENU");
	SetLabelText (networkconfig_title[1], "TCPIPConfig");

	GetMAC ();

	SetButtonText (networkconfig_list[4], mac_buffer);

	for (int i = 0; i < 2; i++)
	{
		SetLabelBackGroundColor (networkconfig_title[i], 0x85223284);
	}

	for (int i = 0; i < 5; i++)
	{
		SetLabelBackGroundColor (networkconfig_label[i], 0x85223284);
	}
}

void NetworkConfigShow (bool show)
{
	int ret = 0;
	bwidget_show (nc, show);

	for (int i = 0; i < 5; i++)
	{
		bwidget_show (networkconfig_label[i], show);
		bwidget_show (networkconfig_list[i], show);
	}

	if (show == true)
	{
		ret = bwidget_set_focus (networkconfig_list[nnetworkconfig_focuse]);
		for (int i = 0; i < 5; i++)
		{
			if (nnetworkconfig_focuse == 0)
			{
				bwidget_show (networkconfig_left, true);
				bwidget_show (networkconfig_right, true);
			}
			else
			{
				bwidget_show (networkconfig_left, false);
				bwidget_show (networkconfig_right, false);
			}
			if (i == nnetworkconfig_focuse)
			{
				bwidget_show (networkconfig_circle[i], true);
				continue;
			}
			bwidget_show (networkconfig_circle[i], false);
		}

		for (int i = 0; i < 5; i++)
		{
			SetButtonBackGroundColor (networkconfig_list[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (networkconfig_list[nnetworkconfig_focuse], FOCUS_COLOR);
		}

		if (nnetworkconfig_focuse == 1)
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == nnetworkconfig_underline)
				{
					bwidget_show (nc_test_line[i], true);
					continue;
				}
				bwidget_show (nc_test_line[i], false);
			}
		}
		else
		{
			for (int i = 0; i < 12; i++)
			{
				bwidget_show (nc_test_line[i], false);
			}
		}

		if (nnetworkconfig_focuse == 2)
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == nnetworkconfig_underline_mask)
				{
					bwidget_show (nc_test_line_mask[i], true);
					continue;
				}
				bwidget_show (nc_test_line_mask[i], false);
			}
		}
		else
		{
			for (int i = 0; i < 12; i++)
			{
				bwidget_show (nc_test_line_mask[i], false);
			}
		}

		if (nnetworkconfig_focuse == 3)
		{
			for (int i = 0; i < 12; i++)
			{
				if (i == nnetworkconfig_underline_gateway)
				{
					bwidget_show (nc_test_line_gateway[i], true);
					continue;
				}
				bwidget_show (nc_test_line_gateway[i], false);
			}
		}
		else
		{
			for (int i = 0; i < 12; i++)
			{
				bwidget_show (nc_test_line_gateway[i], false);
			}
		}

		printf ("ret = %d\n", ret);
	}
}

void DisplayNetworkConfig (unsigned char key)
{
	if (nnetworkconfig_focuse == 0)
	{
		if (left_or_right_num_ip == 0)
		{
			SetButtonText (networkconfig_list[0], ip_pattern_text[1]);
			left_or_right_num_ip = 1;
		}
		else if (left_or_right_num_ip == 1)
		{

			SetButtonText (networkconfig_list[0], ip_pattern_text[0]);
			left_or_right_num_ip = 0;
		}
	}
	if (left_or_right_num_ip == 0)
	{
		if (nnetworkconfig_focuse == 1)
		{
			if (key == eKey_RightArrow)
			{
				nnetworkconfig_underline++;
				if (nnetworkconfig_underline == 12)
				{
					nnetworkconfig_underline = 0;
				}
			}
			else
			{
				if (nnetworkconfig_underline == 0)
				{
					nnetworkconfig_underline = 11;
				}
				else
				{
					nnetworkconfig_underline--;
				}
			}
			for (int i = 0; i < 12; i++)
			{
				if (i == nnetworkconfig_underline)
				{
					bwidget_show (nc_test_line[i], true);
					continue;
				}
				bwidget_show (nc_test_line[i], false);
			}
		}
		else if (nnetworkconfig_focuse == 2)
		{
			if (key == eKey_RightArrow)
			{
				nnetworkconfig_underline_mask++;
				if (nnetworkconfig_underline_mask== 12)
				{
					nnetworkconfig_underline_mask= 0;
				}
			}
			else
			{
				if (nnetworkconfig_underline_mask== 0)
				{
					nnetworkconfig_underline_mask= 11;
				}
				else
				{
					nnetworkconfig_underline_mask--;
				}
			}
			for (int i = 0; i < 12; i++)
			{
				if (i == nnetworkconfig_underline_mask)
				{
					bwidget_show (nc_test_line_mask[i], true);
					continue;
				}
				bwidget_show (nc_test_line_mask[i], false);
			}
		}
		else if (nnetworkconfig_focuse == 3)
		{
			if (key == eKey_RightArrow)
			{
				nnetworkconfig_underline_gateway++;
				if (nnetworkconfig_underline_gateway== 12)
				{
					nnetworkconfig_underline_gateway= 0;
				}
			}
			else
			{
				if (nnetworkconfig_underline_gateway== 0)
				{
					nnetworkconfig_underline_gateway= 11;
				}
				else
				{
					nnetworkconfig_underline_gateway--;
				}
			}
			for (int i = 0; i < 12; i++)
			{
				if (i == nnetworkconfig_underline_gateway)
				{
					bwidget_show (nc_test_line_gateway[i], true);
					continue;
				}
				bwidget_show (nc_test_line_gateway[i], false);
			}
		}
	}
}

void DisplayNetworkConfigIP (unsigned char key)
{
	for (int i = 0; i < 12; i++)
	{
		if (i == nnetworkconfig_underline)
		{
			nc_text_ip[i] = key;
		}
	}
	sprintf (ip_nc_buf, "%d%d%d.%d%d%d.%d%d%d.%d%d%d", nc_text_ip[0], nc_text_ip[1], nc_text_ip[2],
													nc_text_ip[3], nc_text_ip[4], nc_text_ip[5],
													nc_text_ip[6], nc_text_ip[7], nc_text_ip[8],
													nc_text_ip[9], nc_text_ip[10], nc_text_ip[11]);
	SetButtonText (networkconfig_list[1], ip_nc_buf);

	nnetworkconfig_underline++;
	if (nnetworkconfig_underline == 12)
	{
		nnetworkconfig_underline = 0;
	}
	for (int i = 0; i < 12; i++)
	{
		if (i == nnetworkconfig_underline)
		{
			bwidget_show (nc_test_line[i], true);
			continue;
		}
		bwidget_show (nc_test_line[i], false);
	}
}

void DisplayNetworkConfigMask (unsigned char key)
{
	for (int i = 0; i < 12; i++)
	{
		if (i == nnetworkconfig_underline_mask)
		{
			nc_text_mask[i] = key;
		}
	}
	sprintf (mask_nc_buf, "%d%d%d.%d%d%d.%d%d%d.%d%d%d", nc_text_mask[0], nc_text_mask[1], nc_text_mask[2],
													nc_text_mask[3], nc_text_mask[4], nc_text_mask[5],
													nc_text_mask[6], nc_text_mask[7], nc_text_mask[8],
													nc_text_mask[9], nc_text_mask[10], nc_text_mask[11]);
	SetButtonText (networkconfig_list[2], mask_nc_buf);

	nnetworkconfig_underline_mask++;
	if (nnetworkconfig_underline_mask== 12)
	{
		nnetworkconfig_underline_mask= 0;
	}
	for (int i = 0; i < 12; i++)
	{
		if (i == nnetworkconfig_underline_mask)
		{
			bwidget_show (nc_test_line_mask[i], true);
			continue;
		}
		bwidget_show (nc_test_line_mask[i], false);
	}
}

void DisplayNetworkConfigGateway (unsigned char key)
{
	for (int i = 0; i < 12; i++)
	{
		if (i == nnetworkconfig_underline_gateway)
		{
			nc_text_gateway[i] = key;
		}
	}
	sprintf (gateway_nc_buf, "%d%d%d.%d%d%d.%d%d%d.%d%d%d", nc_text_gateway[0], nc_text_gateway[1], nc_text_gateway[2],
													nc_text_gateway[3], nc_text_gateway[4], nc_text_gateway[5],
													nc_text_gateway[6], nc_text_gateway[7], nc_text_gateway[8],
													nc_text_gateway[9], nc_text_gateway[10], nc_text_gateway[11]);
	SetButtonText (networkconfig_list[3], gateway_nc_buf);

	nnetworkconfig_underline_gateway++;
	if (nnetworkconfig_underline_gateway== 12)
	{
		nnetworkconfig_underline_gateway= 0;
	}
	for (int i = 0; i < 12; i++)
	{
		if (i == nnetworkconfig_underline_gateway)
		{
			bwidget_show (nc_test_line_gateway[i], true);
			continue;
		}
		bwidget_show (nc_test_line_gateway[i], false);
	}
}

int MenuNetworkConfigKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		if (left_or_right_num_ip == 0)
		{
			nnetworkconfig_focuse++;
			if (nnetworkconfig_focuse == 5)
			{
				nnetworkconfig_focuse = 0;
			}
			bwidget_set_focus (networkconfig_list[nnetworkconfig_focuse]);

			for (int i = 0; i < 5; i++)
			{
				if (nnetworkconfig_focuse == 0)
				{
					bwidget_show (networkconfig_left, true);
					bwidget_show (networkconfig_right, true);
				}
				else
				{
					bwidget_show (networkconfig_left, false);
					bwidget_show (networkconfig_right, false);
				}
				if (i == nnetworkconfig_focuse)
				{
					bwidget_show (networkconfig_circle[i], true);
					continue;
				}
				bwidget_show (networkconfig_circle[i], false);
			}
			if (nnetworkconfig_focuse == 1)
			{
				for (int i = 0; i < 12; i++)
				{
					if (i == nnetworkconfig_underline)
					{
						bwidget_show (nc_test_line[i], true);
						continue;
					}
					bwidget_show (nc_test_line[i], false);
				}
			}
			else
			{
				for (int i = 0; i < 12; i++)
				{
					bwidget_show (nc_test_line[i], false);
				}
			}

			if (nnetworkconfig_focuse == 2)
			{
				for (int i = 0; i < 12; i++)
				{
					if (i == nnetworkconfig_underline_mask)
					{
						bwidget_show (nc_test_line_mask[i], true);
						continue;
					}
					bwidget_show (nc_test_line_mask[i], false);
				}
			}
			else
			{
				for (int i = 0; i < 12; i++)
				{
					bwidget_show (nc_test_line_mask[i], false);
				}
			}

			if (nnetworkconfig_focuse == 3)
			{
				for (int i = 0; i < 12; i++)
				{
					if (i == nnetworkconfig_underline_gateway)
					{
						bwidget_show (nc_test_line_gateway[i], true);
						continue;
					}
					bwidget_show (nc_test_line_gateway[i], false);
				}
			}
			else
			{
				for (int i = 0; i < 12; i++)
				{
					bwidget_show (nc_test_line_gateway[i], false);
				}
			}
		}
	}
	else if (key_code == eKey_UpArrow)
	{
		if (left_or_right_num_ip == 0)
		{
			if (nnetworkconfig_focuse == 0)
			{
				nnetworkconfig_focuse = 4;
			}
			else
			{
				nnetworkconfig_focuse--;
			}
			bwidget_set_focus (networkconfig_list[nnetworkconfig_focuse]);
			for (int i = 0; i < 5; i++)
			{
				if (nnetworkconfig_focuse == 0)
				{
					bwidget_show (networkconfig_left, true);
					bwidget_show (networkconfig_right, true);
				}
				else
				{
					bwidget_show (networkconfig_left, false);
					bwidget_show (networkconfig_right, false);
				}
				if (i == nnetworkconfig_focuse)
				{
					bwidget_show (networkconfig_circle[i], true);
					continue;
				}
				bwidget_show (networkconfig_circle[i], false);
			}
			if (nnetworkconfig_focuse == 1)
			{
				for (int i = 0; i < 12; i++)
				{
					if (i == nnetworkconfig_underline)
					{
						bwidget_show (nc_test_line[i], true);
						continue;
					}
					bwidget_show (nc_test_line[i], false);
				}
			}
			else
			{
				for (int i = 0; i < 12; i++)
				{
					bwidget_show (nc_test_line[i], false);
				}
			}

			if (nnetworkconfig_focuse == 2)
			{
				for (int i = 0; i < 12; i++)
				{
					if (i == nnetworkconfig_underline_mask)
					{
						bwidget_show (nc_test_line_mask[i], true);
						continue;
					}
					bwidget_show (nc_test_line_mask[i], false);
				}
			}
			else
			{
				for (int i = 0; i < 12; i++)
				{
					bwidget_show (nc_test_line_mask[i], false);
				}
			}

			if (nnetworkconfig_focuse == 3)
			{
				for (int i = 0; i < 12; i++)
				{
					if (i == nnetworkconfig_underline_gateway)
					{
						bwidget_show (nc_test_line_gateway[i], true);
						continue;
					}
					bwidget_show (nc_test_line_gateway[i], false);
				}
			}
			else
			{
				for (int i = 0; i < 12; i++)
				{
					bwidget_show (nc_test_line_gateway[i], false);
				}
			}
		}
	}
	else if (key_code == eKey_Select)
	{
		if (nnetworkconfig_focuse == 1 || nnetworkconfig_focuse == 2 || nnetworkconfig_focuse == 3)
		{
			TCPIP_SetConfig (0, ip_nc_buf, mask_nc_buf, gateway_nc_buf);
			g_Config.NetWorkSettings.IPAddr = htonl (change_addr (ip_nc_buf));
			g_Config.NetWorkSettings.NetMask = htonl (change_addr (mask_nc_buf));
			g_Config.NetWorkSettings.GateWay = htonl (change_addr (gateway_nc_buf));
			SaveConfigToFlash ();
		}
		else if (nnetworkconfig_focuse == 0)
		{
			if (left_or_right_num_ip == 0)
			{
				g_Config.NetWorkSettings.DHCPEnable = 0;
			}
			else
			{
				g_Config.NetWorkSettings.DHCPEnable = 1;
			}
			SaveConfigToFlash ();
		}
	}
	else if (key_code == eKey_Menu)
	{
		NetworkConfigShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{
		if (nnetworkconfig_focuse == 1)
		{
			DisplayNetworkConfigIP (key_code - 0x1d);
		}
		else if (nnetworkconfig_focuse == 2)
		{
			DisplayNetworkConfigMask (key_code - 0x1d);
		}
		else if (nnetworkconfig_focuse == 3)
		{
			DisplayNetworkConfigGateway (key_code - 0x1d);
		}
	}
	else if (key_code == eKey_Exit)
	{
		NetworkConfigShow (false);
		GeneralSettingsShow (true);
		CurrentMenu = eMenu_GeneralSettings;
	}
	else if (key_code == eKey_LeftArrow || key_code == eKey_RightArrow)
	{
		DisplayNetworkConfig (key_code);
	}

	for (int i = 0; i < 5; i++)
	{
		SetButtonBackGroundColor (networkconfig_list[i], UNFOCUS_COLOR);
		SetButtonBackGroundColor (networkconfig_list[nnetworkconfig_focuse], FOCUS_COLOR);
	}

	return 0;
}

void CloseNetworkConfigMenu ()
{
	bwidget_destroy (nc);
	for (int i = 0; i < 5; i++)
	{
		bwidget_destroy (networkconfig_list[i]);
		bwidget_destroy (networkconfig_circle[i]);
		bwidget_destroy (networkconfig_label[i]);
	}

	bwidget_destroy (networkconfig_title[0]);
	bwidget_destroy (networkconfig_title[1]);
	bwidget_destroy (networkconfig_left);
	bwidget_destroy (networkconfig_right);

	for(int i=0;i<12;i++)
	{
		bwidget_destroy (nc_test_line[i]);
		bwidget_destroy (nc_test_line_mask[i]);
		bwidget_destroy (nc_test_line_gateway[i]);
	}
}

