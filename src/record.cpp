#include <sys/vfs.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>




#include "nexus_pid_channel.h"
#include "nexus_parser_band.h"
#include "nexus_recpump.h"
#include "nexus_record.h"
#include "main.h"
#include "global.h"
#include "hotplug.h"
#include "menu_record.h"
#include "playback.h"


BDBG_MODULE(kwt_record);

NEXUS_PidChannelHandle pidChannel[5];

int CheckRecRootDirExist (const char *root_dir)
{
	char RecRootDir[100] = {0};
	sprintf (RecRootDir, "%s/RecFile/", root_dir);
	printf ("RecRootDir = %s\n", RecRootDir);
	if (access (RecRootDir, F_OK) == 0)
	{
		return 1;
	}
	else
	{
		char cmd[100] = {0};
		sprintf (cmd, "mkdir %s", RecRootDir);
		printf ("cmd = %s\n", cmd);
		system (cmd);
		return 0;
	}
}

int CheckDateDirExist (const char *root_dir, char *file_name, char *index_name)
{
	char DateDir[100] = {0};
	char date[20] = {0};
	time_t timep;
	struct tm *p;
	time (&timep);
	p = gmtime (&timep);

	sprintf (date, "%04d-%02d-%02d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);
	printf ("date = %s\n", date);
	sprintf (DateDir, "%s/RecFile/%s", root_dir,date);
	printf ("DateDir = %s\n", DateDir);
	if (access (DateDir, F_OK) == 0)
	{
		char index_dir[100] = {0};
		sprintf (file_name, "%s/RecFile/%s/%02d-%02d-%02d.ts", root_dir,date, p->tm_hour, p->tm_min, p->tm_sec);
		sprintf (index_dir, "%s/index", DateDir);
		if (access (index_dir, F_OK) == 0)
		{
			sprintf (index_name, "%s/RecFile/%s/index/%02d-%02d-%02d.nfo", root_dir, date, p->tm_hour, p->tm_min, p->tm_sec);
		}
		else
		{
			char cmd_t[100] = {0};
			sprintf (cmd_t, "mkdir %s/index", DateDir);
			printf ("cmd_t = %s\n", cmd_t);
			system (cmd_t);
		}
		return 1;
	}
	else
	{
		char cmd[100] = {0};
		sprintf (cmd, "mkdir %s", DateDir);
		printf ("cmd = %s\n", cmd);
		system (cmd);
		sprintf (cmd, "mkdir %s/index", DateDir);
		printf ("cmd = %s\n", cmd);
		system (cmd);

		sprintf (file_name, "%s/RecFile/%s/%02d-%02d-%02d.ts", root_dir, date, p->tm_hour, p->tm_min, p->tm_sec);
		sprintf (index_name, "%s/RecFile/%s/index/%02d-%02d-%02d.nfo", root_dir, date, p->tm_hour, p->tm_min, p->tm_sec);
		return 0;
	}
}

void InitializeRecord(void)
{
	NEXUS_RecordSettings recordSettings;
	g_System.Recpump = NEXUS_Recpump_Open (0, NULL);
	g_System.Record = NEXUS_Record_Create ();
	NEXUS_Record_GetSettings (g_System.Record, &recordSettings);
	recordSettings.recpump = g_System.Recpump;
	NEXUS_Record_SetSettings (g_System.Record, &recordSettings);
}

eRet RecordStart(void)
{
	eRet ret = eRet_Ok;
	NEXUS_Error rc = NEXUS_SUCCESS;
	NEXUS_RecordPidChannelSettings pidSettings;
	char file_path[100] = {0};
	char file_name[100] = {0};
	char index_name[100] = {0};

	int mounted_dev = 0;
	int sd_mounted = 0;
	int usb_mounted = 0;

	if(g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannelNum<1)
	{
		printf("no invalid ts info\n");
		return eRet_InvalidParameter;
	}

	if(g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannelNum<g_Channel.CurrentSubChannel)
	{
		printf("no invalid ts info\n");
		return eRet_InvalidParameter;
	}


	mounted_dev = GetMountedDeviceNum ();
	if (mounted_dev == 0)
	{
		printf ("No media found\n");
		return eRet_InvalidParameter;
	}
	sd_mounted = CheckSDMounted ();
	if (sd_mounted == 0)
	{
		printf ("SD Device not found\n");
	}
	usb_mounted = CheckUSBMounted ();
	if (usb_mounted == 0)
	{
		printf ("USB Device not found\n");
	}

	if ((sd_mounted == 0) && (usb_mounted > 0))
	{
		strcpy (file_path, "/mnt/media/usb");
	}
	if ((sd_mounted > 0) && (usb_mounted == 0))
	{
		strcpy (file_path, "/mnt/media/sdcard");
	}

	if ((sd_mounted > 0) && (usb_mounted > 0))
	{
		strcpy (file_path, "/mnt/media/usb");
	}
	printf ("file_path = %s\n", file_path);
	strcpy (g_System.RecDevPath, file_path);
	CheckRecRootDirExist (file_path);
	CheckDateDirExist (file_path, file_name, index_name);
	printf ("index_name = %s\n", index_name);
	printf ("file_name = %s\n", file_name);

	strcpy (g_System.RecFileName, file_name);

	g_System.RecFile = NULL;
	g_System.RecFile = NEXUS_FileRecord_OpenPosix (file_name, index_name);
	if (!g_System.RecFile)
	{
		BDBG_ERR(("can't open files: '%s' '%s'", file_name, index_name));
		return eRet_InvalidParameter;
	}

	NEXUS_Record_GetDefaultPidChannelSettings (&pidSettings);
	pidSettings.recpumpSettings.pidType = NEXUS_PidType_eVideo;
	pidSettings.recpumpSettings.pidTypeSettings.video.index = true;
	pidSettings.recpumpSettings.pidTypeSettings.video.codec = NEXUS_VideoCodec_eH264;
	pidChannel[0] = NEXUS_PidChannel_Open (g_System.ParserBand, g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].VidPid, NULL);
	rc = NEXUS_Record_AddPidChannel (g_System.Record, pidChannel[0], &pidSettings);
	if (rc != NEXUS_SUCCESS)
	{
		return eRet_InvalidParameter;
	}

	if (pidSettings.recpumpSettings.pidTypeSettings.video.codec == NEXUS_VideoCodec_eH264)
	{
		NEXUS_RecpumpTpitFilter filter;
		NEXUS_Recpump_GetDefaultTpitFilter (&filter);
		filter.config.mpeg.randomAccessIndicatorEnable = true;
		filter.config.mpeg.randomAccessIndicatorCompValue = true;
		rc = NEXUS_Recpump_SetTpitFilter (g_System.Recpump, pidChannel[0], &filter);
		if (rc != NEXUS_SUCCESS)
		{
			return eRet_InvalidParameter;
		}
	}

	pidChannel[1] = NEXUS_PidChannel_Open (g_System.ParserBand, g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].AudPid, NULL);
	rc = NEXUS_Record_AddPidChannel (g_System.Record, pidChannel[1], NULL);
	if (rc != NEXUS_SUCCESS)
	{
		return eRet_InvalidParameter;
	}

	pidChannel[2] = NEXUS_PidChannel_Open (g_System.ParserBand, 0x0000, NULL);//pat
	rc = NEXUS_Record_AddPidChannel (g_System.Record, pidChannel[2], NULL);
	if (rc != NEXUS_SUCCESS)
	{
		return eRet_InvalidParameter;
	}

	pidChannel[3] = NEXUS_PidChannel_Open (g_System.ParserBand, g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].PmtPid, NULL);//pmt
	rc = NEXUS_Record_AddPidChannel (g_System.Record, pidChannel[3], NULL);
	if (rc != NEXUS_SUCCESS)
	{
		return eRet_InvalidParameter;
	}

	pidChannel[4] = NEXUS_PidChannel_Open (g_System.ParserBand, g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].PcrPid, NULL);//pcr
	rc = NEXUS_Record_AddPidChannel (g_System.Record, pidChannel[4], NULL);
	if (rc != NEXUS_SUCCESS)
	{
		return eRet_InvalidParameter;
	}

	rc = NEXUS_Record_Start (g_System.Record, g_System.RecFile);
	if (rc != NEXUS_SUCCESS)
	{
		return eRet_InvalidParameter;
	}

	return eRet_Ok;
}

void RecordStop(void)
{
	if (g_System.RecFile != NULL)
	{
		NEXUS_Record_Stop (g_System.Record);
		NEXUS_Record_RemoveAllPidChannels (g_System.Record);
		NEXUS_PidChannel_Close (pidChannel[0]);
		NEXUS_PidChannel_Close (pidChannel[1]);
		NEXUS_PidChannel_Close (pidChannel[2]);
		NEXUS_PidChannel_Close (pidChannel[3]);
		NEXUS_PidChannel_Close (pidChannel[4]);
		NEXUS_FileRecord_Close (g_System.RecFile);

		g_System.RecFile = NULL;
		memset (g_System.RecFileName, 0, sizeof (g_System.RecFileName));
		memset (g_System.RecDevPath, 0, sizeof (g_System.RecDevPath));
	}
}

void RecordClose(void)
{
	NEXUS_Record_Destroy (g_System.Record);
}

static unsigned long kscale (unsigned long b, unsigned long bs)
{
	return (b * (unsigned long long) bs + 1024 / 2) / 1024;
}

unsigned int GetRecFileSize(void)
{
	int ret = 0;
	if (g_System.RecFile != NULL)
	{
		struct stat buf;
		ret = stat (g_System.RecFileName, &buf);
		if (ret == 0)
		{
			return (unsigned int) (buf.st_size / 1024);
		}
		else
		{
			return 0;
		}
	}
}


unsigned int GetRecDevTotalSize(void)
{
	int ret = 0;
	unsigned int free;
	if (g_System.RecDevPath[0] != 0)
	{
		struct statfs buf;
		ret = statfs (g_System.RecDevPath, &buf);
		if (ret == 0)
		{
			free = (unsigned int) kscale (buf.f_blocks, buf.f_bsize);
			return free;
		}
		else
		{
			return 0;
		}
	}
}






unsigned int GetRecDevFreeSize(void)
{
	int ret = 0;
	unsigned int free;
	if (g_System.RecDevPath[0] != 0)
	{
		struct statfs buf;
		ret = statfs (g_System.RecDevPath, &buf);
		if (ret == 0)
		{
			free = (unsigned int) kscale (buf.f_bavail, buf.f_bsize);
			return free;
		}
		else
		{
			return 0;
		}
	}
}

int GetFirstValidDir(const char *dev_root,char *file_dir)
{
	struct dirent **namelist;
	int i;
	int total;

	total = scandir (dev_root, &namelist, 0, alphasort);
	if (total < 0)
	{
		perror ("scandir");
		return 1;
	}
	else if(total<3)
	{
		printf("No Delete Dir Found\n");
		return 1;
	}
	else
	{
		if((strcmp(namelist[2]->d_name,".")!=0)&&(strcmp(namelist[2]->d_name,"..")!=0))
		{
			sprintf(file_dir,"%s/%s",dev_root,namelist[2]->d_name);
			return 0;

		}
		else
		{
			return 1;
		}
	}
}


int GetFirstValidFile(const char *file_dir,char *file_name)
{
	struct dirent **namelist;
	int i;
	int total;

	total = scandir (file_dir, &namelist, 0, alphasort);
	if (total < 0)
	{
		perror ("scandir");
		return 1;
	}
	else if(total<3)
	{
		printf("No Delete Dir Found\n");
		strcpy(file_name,file_dir);
		return 0;
	}
	else
	{
		if((strcmp(namelist[2]->d_name,".")!=0)&&(strcmp(namelist[2]->d_name,"..")!=0))
		{
			sprintf(file_name,"%s/%s",file_dir,namelist[2]->d_name);
			return 0;

		}
		else
		{
			return 1;
		}
	}
}


int CheckIndexExist(const char *video_file,char *index_file)
{
	char index_name[100]={0};
	int ret = 0;
	memset(index_name,0,100);
	printf("video_file=%s\n",video_file);
	strncpy(index_name,video_file,strlen(video_file)-12);
	sprintf(index_name,"%s/index",index_name);
	strncat(index_name,video_file+strlen(video_file)-12,9);
    sprintf(index_name,"%s.nfo",index_name);
	if(access(index_name,F_OK)==0)
	{
		strcpy(index_file,index_name);
		return 1;
	}
	else
	{
		return 0;
	}
}






int DeleteOldFile(void)
{
	int ret=0;
	char dev_root[512]={0};
	char file_dir[512]={0};
	char file_name[512]={0};
	char index_name[512]={0};
	char cmd_tmp[512]={0};
	memset(dev_root,0,512);
	memset(file_dir,0,512);
	memset(file_name,0,512);
	memset(index_name,0,512);
	memset(cmd_tmp,0,512);
	if (g_System.RecFile != NULL)
	{
		if (g_System.RecDevPath[0] != 0)
		{
			if(strcmp(g_System.RecDevPath,"/mnt/media/usb")==0)
			{
				strcpy(dev_root,"/mnt/media/usb");

			}
			else if(strcmp(g_System.RecDevPath,"/mnt/media/sdcard")==0)
			{
				strcpy(dev_root,"/mnt/media/sdcard");
			}
			sprintf(dev_root,"%s/RecFile",dev_root);
			printf("dev_root=%s\n",dev_root);
			if(access(dev_root,F_OK)==0)
			{
				ret=GetFirstValidDir(dev_root,file_dir);
				if(ret==0)
				{
					printf("file_dir=%s\n",file_dir);
					ret=GetFirstValidFile(file_dir,file_name);
					if(ret==0)
					{
						if(strcmp(file_name,g_System.RecFileName)==0)
						{
							return 1;
						}
						memset(cmd_tmp,0,512);
						sprintf(cmd_tmp,"rm %s -Rf",file_name);
						printf("cmd_tmp=%s\n",cmd_tmp);
						system(cmd_tmp);

						if(CheckIndexExist(file_name,index_name)==1)
    					{
    						memset(cmd_tmp,0,512);
    						sprintf(cmd_tmp,"rm %s -Rf",index_name);
    						printf("cmd_tmp=%s\n",cmd_tmp);
    						system(cmd_tmp);

    					}
    					else
    					{
    						return 1;
    					}

					}

				}
				else
				{
					return 1;
				}

			}
			else
			{
				return 1;
			}









		}
	}



}



extern char *record_config_file_length[];




void RecMonThread (void *pParam)
{
	unsigned int file_size = 0;
	unsigned int free_size = 0;
	eRet ret = eRet_Ok;

	while(g_Status.RtcInited==0)
	{
		sleep(1);
	}

	while (1)
	{

		if((g_Status.RecRestartNeeded==1)&&(g_System.RecFile != NULL))
		{
			g_Status.RecRestartNeeded=0;
			RecordStop ();
			MenuRecordShow (false);
			usleep(500000);
			ret = RecordStart ();
			if (ret == eRet_Ok)
			{
				MenuRecordShow (true);
			}
		}





		if ((g_Config.RecSettings.RecOnStartEnable == 1) && (g_System.RecFile == NULL) && (g_System.ManulStopRec == 0)&&(g_Status.RecNextBootUp==0))
		{
			ret = RecordStart ();
			if (ret == eRet_Ok)
			{
				MenuRecordShow (true);
			}
		}
		if (g_System.RecFile != NULL)
		{
			file_size = GetRecFileSize ();
			if((file_size>atoi(record_config_file_length[g_Config.RecSettings.RecFileSize])*1000)&&(g_Config.RecSettings.RecFileSize<7))
			{
					RecordStop ();
					MenuRecordShow (false);
					usleep(500000);
					ret = RecordStart ();
					if (ret == eRet_Ok)
					{
						MenuRecordShow (true);
					}
			}

			free_size = GetRecDevFreeSize ();


			if (free_size < 50 * 1024)
			{
				if (g_Config.RecSettings.LoopRecEnable == 1)
				{
					DeleteOldFile();
				}
				else
				{
					RecordStop ();
					MenuRecordShow (false);
				}
			}

	/*
			if (1)
			{
				if (1)
				{
					DeleteOldFile();
				}
				else
				{
					RecordStop ();
					MenuRecordShow (false);
				}
			}
*/


		}
		sleep (1);
	}
}

