
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "nandflash.h"
#include "global.h"



#define MTD_OF_CHANNEL 0
#define MIN_BLOCK_OF_CHANNEL 0
#define MAX_BLOCK_OF_CHANNEL 15
#define CHANNEL_VERSION 0xCDAB0001



eRet SaveChannelToFlash(void)
{

	int ret=0;
	char *tmp_buf=(char *)malloc(0x20000);

	memcpy(tmp_buf,(void *)&g_Channel,0x20000);

	if(g_Status.ChannelBlock>MAX_BLOCK_OF_CHANNEL)
	{
		free(tmp_buf);
		printf("Wrong ChannelBlock %d\n",g_Status.ChannelBlock);
		return eRet_NotSupported;
	}

	NandErase(0, g_Status.ChannelBlock*0x20000, 0x20000);

	ret=NandWrite(0, g_Status.ChannelBlock*0x20000, 0x20000,tmp_buf);
	printf("***SaveChannelToFlash ret=%d\n",ret);
	if(ret == 0)
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


void SetChannelToDefault(void)
{
    memset((void *)&g_Channel,0,sizeof(g_Channel));
	g_Channel.Version=CHANNEL_VERSION;
}





eRet LoadChannelFromFlash(void)
{
	int i=0;
	int ret=0;
	char tmp_buf[0x20000]={0};

	/*Find Channel From mtd0,block 16 to31,to skip bad block*/
	for(i=MIN_BLOCK_OF_CHANNEL;i<MAX_BLOCK_OF_CHANNEL+1;i++)
	{
		memset(tmp_buf,0,0x20000);
		ret=NandRead(MTD_OF_CHANNEL, i*0x20000, 0x20000, tmp_buf);
		if(ret==1)
		{
			printf("***LoadChannelFromFlash error\n");
		}
		else
		{
			printf("version=%08x\n",*(unsigned int *)tmp_buf);
			if(*(unsigned int *)tmp_buf==CHANNEL_VERSION)
			{
				printf("Find Saved Channel From MTD%d,Block %d\n",MTD_OF_CHANNEL,i);
                g_Status.ChannelBlock=i;
				memcpy((void *)&g_Channel,tmp_buf,sizeof(g_Channel));
				return eRet_Ok;
			}
		}
	}

	SetChannelToDefault();


	/*Can not find Channel,So save the default Channel to good block*/
	if(i==MAX_BLOCK_OF_CHANNEL+1)
	{
		printf("Cannot Find Channel From MTD%d\n",MTD_OF_CHANNEL);
		for(i=MIN_BLOCK_OF_CHANNEL;i<MAX_BLOCK_OF_CHANNEL+1;i++)
		{
			ret=NandErase(MTD_OF_CHANNEL, i*0x20000, 0x20000);
			if(ret==1)
			{
				printf("Bad Block founded,at MTD%d BLock %d\n",MTD_OF_CHANNEL,i);
			}
			else
			{
				printf("Find Good Block To Save Channel,at MTD%d BLock %d\n",MTD_OF_CHANNEL,i);
                g_Status.ChannelBlock=i;
				printf("Version=%08x\n",g_Channel.Version);
                SaveChannelToFlash();
				return eRet_Ok;
			}

		}
	}
	return eRet_Ok;
}




eRet AddChannel(uint32 fre,uint32 bandwidth)
{
	int i=0;
	if(g_Channel.OverLoop==1)
	{
		for(i=0;i<MAX_CHANNELS;i++)
		{
			if((g_Channel.ChannelList[i].Frequency==fre)&&(g_Channel.ChannelList[i].BandWidth==bandwidth))
			{
				break;
			}

		}
		if(i==MAX_CHANNELS)
		{
			g_Channel.ChannelList[g_Channel.ChannelCount].Frequency=fre;
        	g_Channel.ChannelList[g_Channel.ChannelCount].BandWidth=bandwidth;
        	g_Channel.CurrentChannel=g_Channel.ChannelCount;
        	g_Status.ChannelChanged=1;
        	B_Time_Get(&g_Status.ChannelChangedTime);
        	g_Channel.CurrentSubChannel=0;
        	g_Channel.ChannelCount++;
        	if(g_Channel.ChannelCount==MAX_CHANNELS)
        	{
        		g_Channel.ChannelCount=0;
        		g_Channel.OverLoop=1;
        	}
        	SaveChannelToFlash();
		}
		else
		{
			g_Channel.CurrentSubChannel=0;
			g_Channel.CurrentChannel=i;
        	g_Status.ChannelChanged=1;
        	B_Time_Get(&g_Status.ChannelChangedTime);
			SaveChannelToFlash();
		}
	}
	else
	{
		for(i=0;i<g_Channel.ChannelCount;i++)
		{
			if((g_Channel.ChannelList[i].Frequency==fre)&&(g_Channel.ChannelList[i].BandWidth==bandwidth))
			{
				break;
			}

		}
		if(i==g_Channel.ChannelCount)
		{
			g_Channel.ChannelList[g_Channel.ChannelCount].Frequency=fre;
        	g_Channel.ChannelList[g_Channel.ChannelCount].BandWidth=bandwidth;
        	g_Channel.CurrentChannel=g_Channel.ChannelCount;
        	g_Status.ChannelChanged=1;
        	B_Time_Get(&g_Status.ChannelChangedTime);
        	g_Channel.CurrentSubChannel=0;
        	g_Channel.ChannelCount++;
        	if(g_Channel.ChannelCount==MAX_CHANNELS)
        	{
        		g_Channel.ChannelCount=0;
        		g_Channel.OverLoop=1;
        	}
        	SaveChannelToFlash();
		}
		else
		{
			g_Channel.CurrentSubChannel=0;
			g_Channel.CurrentChannel=i;
        	g_Status.ChannelChanged=1;
        	B_Time_Get(&g_Status.ChannelChangedTime);
			SaveChannelToFlash();
		}

	}


/*
	g_Channel.ChannelList[g_Channel.ChannelCount].Frequency=fre;
	g_Channel.ChannelList[g_Channel.ChannelCount].BandWidth=bandwidth;
	g_Channel.CurrentChannel=g_Channel.ChannelCount;
	g_Status.ChannelChanged=1;
	B_Time_Get(&g_Status.ChannelChangedTime);
	g_Channel.CurrentSubChannel=0;
	g_Channel.ChannelCount++;
	if(g_Channel.ChannelCount==MAX_CHANNELS)
	{
		g_Channel.ChannelCount=0;
		g_Channel.OverLoop=1;
	}
	SaveChannelToFlash();
	return eRet_Ok;
	*/
}

eRet DelChannel(uint8 ChannelNum)
{
	unsigned int i = 0;
	printf("DelChannel %d\n",ChannelNum);


	if((ChannelNum>=g_Channel.ChannelCount)&&(g_Channel.OverLoop==0))
	{
		printf("invalid ChannelNum to del\n");
		return eRet_InvalidParameter;
	}

	if((g_Channel.ChannelCount == 0)&&(g_Channel.OverLoop==0))
	{
		printf("No channel to delete\n");
		return eRet_InvalidParameter;
	}



	if(g_Channel.OverLoop==1)
	{
    	for(i=ChannelNum;i<MAX_CHANNELS-1;i++)
    	{
    		g_Channel.ChannelList[i]=g_Channel.ChannelList[i+1];
    	}
		g_Channel.OverLoop=0;
		g_Channel.ChannelCount=31;
		if(ChannelNum<=g_Channel.CurrentChannel)
		{
			if(g_Channel.CurrentChannel>0)
			{
				g_Channel.CurrentChannel--;
			}
			else
			{
				g_Channel.CurrentChannel=0;
			}
			g_Channel.CurrentSubChannel=0;
        	g_Status.ChannelChanged=1;
        	B_Time_Get(&g_Status.ChannelChangedTime);
		}
		memset((void *)&g_Channel.ChannelList[31].Frequency,0,sizeof(CHANNEL_LIST));
		SaveChannelToFlash();
	}
	else
	{
		for(i=ChannelNum;i<g_Channel.ChannelCount-1;i++)
    	{
    		g_Channel.ChannelList[i]=g_Channel.ChannelList[i+1];
    	}
		if(g_Channel.ChannelCount>0)
		{
			g_Channel.ChannelCount--;
		}
		else
		{
			g_Channel.ChannelCount=0;
		}
		if(ChannelNum<=g_Channel.CurrentChannel)
		{
			if(g_Channel.CurrentChannel>0)
			{
				g_Channel.CurrentChannel--;
			}
			else
			{
				g_Channel.CurrentChannel=0;
			}
			g_Channel.CurrentSubChannel=0;
        	g_Status.ChannelChanged=1;
        	B_Time_Get(&g_Status.ChannelChangedTime);
		}
		SaveChannelToFlash();

	}

	return eRet_Ok;


}



