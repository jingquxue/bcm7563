/******************************************************************************

                  版权所有 (C), 2001-2011, 深圳市科卫泰实业发展有限公司

 ******************************************************************************
  文 件 名   : global.cpp
  版 本 号   : 初稿
  作    者   : 马登坤
  生成日期   : 2015年6月3日
  最近修改   :
  功能描述   : 全局变量的设置及初始化
  函数列表   :
              InitializeParameters
              LoadConfigFromFlash
  修改历史   :
  1.日    期   : 2015年6月3日
    作    者   : 马登坤
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "nandflash.h"

G_CONFIG g_Config;
G_SYSTEM g_System;
G_STATUS g_Status;
G_CHANNEL g_Channel;

#define MTD_OF_CONFIG 0
#define MIN_BLOCK_OF_CONFIG 16
#define MAX_BLOCK_OF_CONFIG 31
#define CONFIG_VERSION 0x12340001

BDBG_MODULE(kwt_global);

void LoadDefaultSetting ();

void InitializeParameters(void)
{
	memset ((void *) &g_Config, 0, sizeof (g_Config));
	memset ((void *) &g_System, 0, sizeof (g_System) );
	memset ((void *) &g_Status, 0, sizeof (g_Status));
	memset ((void *) &g_Channel, 0, sizeof (g_Channel));
	g_Status.ConfigBlock = 0xFF;
	g_Status.ChannelBlock = 0xFF;
	printf ("*** sizeof(g_Config) = %d\n", sizeof (g_Config));
	printf ("*** sizeof(g_System) = %d\n", sizeof (g_System));
	printf ("*** sizeof(g_Status) = %d\n", sizeof (g_Status));
	printf ("*** sizeof(g_Channel) = %d\n", sizeof (g_Channel));
	printf ("\n");
	usleep (10000);
}

eRet SaveConfigToFlash(void)
{
	int ret = 0;
	char *tmp_buf=(char *)malloc(0x20000);


	memcpy (tmp_buf, (void *) &g_Config, 0x20000);

	if ((g_Status.ConfigBlock < MIN_BLOCK_OF_CONFIG) || (g_Status.ConfigBlock > MAX_BLOCK_OF_CONFIG))
	{
		free(tmp_buf);
		printf ("Wrong ConfigBlock %d\n", g_Status.ConfigBlock);
		return eRet_NotSupported;
	}
	NandErase (0, g_Status.ConfigBlock * 0x20000, 0x20000);

	ret = NandWrite (0, g_Status.ConfigBlock * 0x20000, 0x20000, tmp_buf);
	printf ("***SaveConfigToFlash ret = %d\n", ret);
	if (ret == 0)
	{
		free(tmp_buf);
		return eRet_Ok;
	}
	else
	{
		free(tmp_buf);
		return eRet_NotSupported;
	}
}

void SetConfigToDefault(void)
{
	memset ((void *) &g_Config, 0, sizeof (g_Config));

	LoadDefaultSetting ();
	g_Config.NetRelaySettings.RemoteIP = 0xc0a80a64;
	g_Config.NetRelaySettings.RemotePort = 0x3039;
	g_Config.NetRelaySettings.protocal = IpProtocol_eUdp;
	g_Config.NetRelaySettings.Type = 0;
	g_Config.NetRelaySettings.Enable = 1;

	g_Config.Version = CONFIG_VERSION;
	g_Config.HdmiSettings.Format = 4;
    g_Config.Volume=50;

	g_Config.RecSettings.RecOnStartEnable=0;
    g_Config.RecSettings.LoopRecEnable=0;
    g_Config.RecSettings.PreferredMedia=0;
    g_Config.RecSettings.RecFileSize=2;







}

eRet LoadConfigFromFlash(void)
{
	int i = 0;
	int ret = 0;
	char tmp_buf[0x20000] = {0};

	for (i = MIN_BLOCK_OF_CONFIG; i < MAX_BLOCK_OF_CONFIG + 1; i++)
	{
		memset (tmp_buf, 0, 0x20000);
		ret = NandRead (MTD_OF_CONFIG, i * 0x20000, 0x20000, tmp_buf);
		if (ret == 1)
		{
			printf ("***LoadConfigFormFlash error\n");
		}
		else
		{
			printf ("version = %08x\n", *(unsigned int *) tmp_buf);
			if (*(unsigned int *) tmp_buf == CONFIG_VERSION)
			{
				printf ("Find Saved Config From MTD%d, Block %d\n", MTD_OF_CONFIG, i);
				g_Status.ConfigBlock = i;
				memcpy ((void *) &g_Config, tmp_buf, sizeof (g_Config));
				return eRet_Ok;
			}
		}
	}
	SetConfigToDefault ();

	if (i == MAX_BLOCK_OF_CONFIG + 1)
	{
		printf ("Cannot Find Config From MTD%d\n", MTD_OF_CONFIG);
		for (i = MIN_BLOCK_OF_CONFIG; i < MAX_BLOCK_OF_CONFIG + 1; i++)
		{
			ret = NandErase (MTD_OF_CONFIG, i * 0x20000, 0x20000);
			if (ret == 1)
			{
				printf ("Bad Block founded, at MTD%d BLock %d\n", MTD_OF_CONFIG, i);
			}
			else
			{
				printf ("Find Good Block To Save Config, at MTD%d BLock %d\n", MTD_OF_CONFIG, i);
				g_Status.ConfigBlock = i;
				printf ("Version = %08x\n", g_Config.Version);
				SaveConfigToFlash ();
				return eRet_Ok;
			}
		}
	}

	return eRet_Ok;
}

void LoadDefaultSetting ()
{
	unsigned int ip, ip2;
	struct timeval tv;

	gettimeofday (&tv, NULL);
	srand ((unsigned int) tv.tv_usec);
	ip = (unsigned int) (255.0 * rand () / (RAND_MAX + 1.0));
	ip2 = (unsigned int) (rand () / (RAND_MAX / 255 + 1.0));

	g_Config.NetWorkSettings.IPAddr = (0xa9 << 24) | (0xfe << 16) | (((unsigned char) ip) << 8) | ((unsigned char) ip2);
	g_Config.NetWorkSettings.NetMask = 0xffff0000;
	g_Config.NetWorkSettings.GateWay = (0xa9 << 24) | (0xfe << 16) | (((unsigned char) ip) << 8) | (0xfe);
	g_Config.NetWorkSettings.DHCPEnable = 0;
}

