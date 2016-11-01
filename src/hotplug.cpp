#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mount.h>
#include <unistd.h>
#include <stdbool.h>


#include <ctype.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>





#define UEVENT_BUFFER_SIZE 2048
#define MAX_NAME_LEN                   64
#define MAX_PART_NUM                  6   //最多运行6个分区

typedef struct
{
	char partition_name[MAX_NAME_LEN];// /dev/sda1 /dev/sda2
	int fat32;             // 0 if not fat32,1 is fat32
	int mounted;           // 0 is not mounted,1 is mounted;
	char mount_path[MAX_NAME_LEN];   //path of mount

}USB_PARTITION;


typedef struct
{
	char device_name[MAX_NAME_LEN];  // /dev/sda/  /dev/sdb
    int  type; // 0 not check,1 is sd,2 is sd usb
    int  part_num;
	USB_PARTITION partition[MAX_PART_NUM];
}USB_DEV;


USB_DEV usb_dev[2];

int GetMountedDeviceNum(void)
{
	int i=0;
	int j=0;
	int ret=0;
	for(i=0;i<2;i++)
	{
		for(j=0;j<usb_dev[i].part_num;j++)
		{
			ret=ret+usb_dev[i].partition[j].mounted;
		}
	}
	printf("mounted device num = %d\n",ret);
	return ret;
}



int CheckSDMounted(void)
{
	int i=0;
	int j=0;
	int ret=0;
	for(i=0;i<2;i++)
	{
		if(usb_dev[i].type==1)
		{
			for(j=0;j<usb_dev[i].part_num;j++)
    		{
    			ret=ret+usb_dev[i].partition[j].mounted;
    		}
		}
	}
	printf("SD is mounted = %d\n",ret);
	return ret;
}





int CheckUSBMounted(void)
{
	int i=0;
	int j=0;
	int ret=0;
	for(i=0;i<2;i++)
	{
		if(usb_dev[i].type==2)
		{
			for(j=0;j<usb_dev[i].part_num;j++)
    		{
    			ret=ret+usb_dev[i].partition[j].mounted;
    		}
		}
	}
	printf("USB is mounted = %d\n",ret);
	return ret;
}








void dump(void)
{
    int i=0;
    int j=0;
    for(i=0;i<2;i++)
    {
        printf("usb_dev[%d].device_name                =%s\n",i,usb_dev[i].device_name);
        printf("usb_dev[%d].type                       =%d\n",i,usb_dev[i].type);
        printf("usb_dev[%d].part_num                   =%d\n",i,usb_dev[i].part_num);
        for(j=0;j<usb_dev[i].part_num;j++)
        {
        printf("usb_dev[%d].partition%d].partition_name=%s\n",i,j,usb_dev[i].partition[j].partition_name);
        printf("usb_dev[%d].partition%d].fat32         =%d\n",i,j,usb_dev[i].partition[j].fat32);
        printf("usb_dev[%d].partition%d].mounted       =%d\n",i,j,usb_dev[i].partition[j].mounted);
        printf("usb_dev[%d].partition%d].mount_path    =%s\n",i,j,usb_dev[i].partition[j].mount_path);

        }

    }

}





static int init_hotplug_sock()
{
  const int buffersize = 1024;
  int ret;

  struct sockaddr_nl snl;
  bzero(&snl, sizeof(struct sockaddr_nl));
  snl.nl_family = AF_NETLINK;
  snl.nl_pid = getpid();
  snl.nl_groups = 1;

  int s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
  if (s == -1)
  {
        perror("socket");
        return -1;
  }
  setsockopt(s, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));

  ret = bind(s, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl));
  if (ret < 0)
  {
        perror("bind");
        close(s);
        return -1;
  }

  return s;
}


/*

# cat /proc/partitions
major minor  #blocks  name

  31        0       8192 mtdblock0
  31        1       4096 mtdblock1
  31        2      81920 mtdblock2
  31        3      36864 mtdblock3
  31        5       2048 mtdblock5
  31        6         64 mtdblock6
  31        7         64 mtdblock7
  31        8       8192 mtdblock8
  31        9        512 mtdblock9
  31       10      21888 mtdblock10
   8        0   15482880 sda
   8        1          1 sda1
   8        5   15470563 sda5
   8       16    3872256 sdb
   8       17    1232374 sdb1
   8       18          1 sdb2
   8       21      71680 sdb5
   8       22    2559488 sdb6
  31       11      74400 mtdblock11
*/
int find_devices(void)
{
	int len = 0;
	int dev_num = 0;
    int part_num = 0;
    char buf[1024] = {0};     //1024 is enough for save information of FILE partitions
    char hoststr[16] = {0};
    char *delim="\n";
    char *line = NULL;
    char *strtok_tmp_ptr = NULL;
    char *seek = NULL;
    char *seek_sd = NULL;      //USED FOR DEVICE MOUNTED ON SD*
    char *part_blocks = NULL;
    FILE *fd = NULL;


    fd = fopen("/proc/partitions", "r"); //直接操作open打开系统文件
    if(fd != NULL)
    {
        //printf("open file /proc/partitions success\n");
        len = fread(buf, 1, sizeof(buf), fd);
        fclose(fd);
        if(len > 0)
        {
            line = strtok_r(buf, delim, &strtok_tmp_ptr);//以"\n"结束符来截断字符串
            while(line)
            {
                seek_sd = strstr(line, "sd");//成功找到sd后指针seek指向s, seek_sd[0] = 's', seek_sd[1] = 'd'
                if (seek_sd)
                {
                    if(seek_sd[2] >= 'a' && seek_sd[2] <= 'z')
                    {
                        if(seek_sd[3]==0x00)
                        {
                            part_num=0;
                            dev_num++;
                            sprintf(usb_dev[dev_num-1].device_name,"/dev/%s",seek_sd);
                            //printf("usb_dev[%d].device_name=%s\n",dev_num-1,usb_dev[dev_num-1].device_name);
                        }


                        if(seek_sd[3] >= '1' && seek_sd[3] < '9')
                        {

                            sprintf(usb_dev[dev_num-1].partition[part_num].partition_name,"/dev/%s",seek_sd);
                            //printf("usb_dev[%d].partition[%d].partition_name=%s\n",dev_num-1,part_num,usb_dev[dev_num-1].partition[part_num].partition_name);
                            part_num ++;
                            usb_dev[dev_num-1].part_num=part_num;
                            //printf("usb_dev[%d].part_num=%d\n",dev_num-1,usb_dev[dev_num-1].part_num);


                            if (part_num == MAX_PART_NUM)
                            {
                                 //printf("too many parts ignore\n");
                                 break;//too many parts ignore
                            }
                        }
                    }
                }
                line = strtok_r(NULL, delim, &strtok_tmp_ptr);
            }
        }
    }
    else
    {
        perror("open /proc/partitions error");
    }
}






/*

# cat /tmp/yy

Disk /dev/sdb: 3965 MB, 3965190144 bytes
4 heads, 16 sectors/track, 121008 cylinders
Units = cylinders of 64 * 512 = 32768 bytes

   Device Boot      Start         End      Blocks  Id System
/dev/sdb1              33       38544     1232374+  e Win95 FAT16 (LBA)
/dev/sdb2           38657      121008     2635264  85 Linux extended
/dev/sdb5           38721       40960       71680   c Win95 FAT32 (LBA)
/dev/sdb6           41025      121008     2559488  83 Linux





*/

void check_filesystem(void)
{
    int len=0;
    int num_dev=0;
    int num_partitions=0;
    char tmp_command[256]={0};
    FILE *fd = NULL;
    char buf[1024] = {0};     //1024 is enough for save information of FILE partitions
    char *delim="\n";
    char *line = NULL;
    char *strtok_tmp_ptr = NULL;
    char *seek_partition_name = NULL;      //USED FOR DEVICE MOUNTED ON SD*
    char *seek_fat32 = NULL;      //USED FOR DEVICE MOUNTED ON SD*

    for(num_dev=0;num_dev<2;num_dev++)
    {
        //printf("%02x\n",usb_dev[num_dev].device_name[0]);


        if(usb_dev[num_dev].device_name[0] != 0x00)
        {
            sprintf(tmp_command,"fdisk -l %s > /tmp/tmp_fdisk",usb_dev[num_dev].device_name);
            //printf("%s\n",tmp_command);
            system(tmp_command);
            fd = fopen("/tmp/tmp_fdisk", "r"); //直接操作open打开系统文件
            if(fd != NULL)
            {
                //printf("open file /tmp/tmp_fdisk success\n");
                len = fread(buf, 1, sizeof(buf), fd);
                fclose(fd);
                system("rm /tmp/tmp_fdisk");
                if(len > 0)
                {
                    line = strtok_r(buf, delim, &strtok_tmp_ptr);//以"\n"结束符来截断字符串
                    while(line)
                    {
                        for(num_partitions=0;num_partitions<usb_dev[num_dev].part_num;num_partitions++)
                        {
                            seek_partition_name=strstr(line,usb_dev[num_dev].partition[num_partitions].partition_name);
                            if(seek_partition_name)
                            {
                                //printf("find partition_name %s\n",usb_dev[num_dev].partition[num_partitions].partition_name);
                                seek_fat32=strstr(line,"FAT32");
                                if(seek_fat32)
                                {
                                    usb_dev[num_dev].partition[num_partitions].fat32=1;
                                    //printf("usb_dev[%d].partition[%d].partition_name=%s\n",num_dev,num_partitions,usb_dev[num_dev].partition[num_partitions].partition_name);
                                    //printf("usb_dev[%d].partition[%d].fat32=1\n",num_dev,num_partitions);


                                }

                            }





                        }
                        line = strtok_r(NULL, delim, &strtok_tmp_ptr);
                    }
                }
            }
            else
            {
                perror("open /tmp/tmp_fdisk error");
            }

        }

    }


}


void do_mount(void)
{
    int num_dev=0;
    int num_partitions=0;
    char tmp_cmd[256]={0};
    for(num_dev=0;num_dev<2;num_dev++)
    {
        if(usb_dev[num_dev].device_name[0] != 0x00)
        {
            for(num_partitions=0;num_partitions<usb_dev[num_dev].part_num;num_partitions++)
            {
                if((usb_dev[num_dev].partition[num_partitions].fat32 == 1)
                 &&(usb_dev[num_dev].partition[num_partitions].mounted==0))
                 {
                    if(usb_dev[num_dev].type==1)
                    {
                        sprintf(tmp_cmd,"mount -t vfat %s  /mnt/media/sdcard",usb_dev[num_dev].partition[num_partitions].partition_name);
                        //printf("%s\n",tmp_cmd);
                        sprintf(usb_dev[num_dev].partition[num_partitions].mount_path,"/mnt/media/sdcard");

                    }
                    else if(usb_dev[num_dev].type==2)
                    {
                        sprintf(tmp_cmd,"mount -t vfat %s  /mnt/media/usb",usb_dev[num_dev].partition[num_partitions].partition_name);
                        //printf("%s\n",tmp_cmd);
                        sprintf(usb_dev[num_dev].partition[num_partitions].mount_path,"/mnt/media/usb");
                    }
                    system(tmp_cmd);
                    usb_dev[num_dev].partition[num_partitions].mounted=1;
                    //printf("%s mounted at %s\n",usb_dev[num_dev].partition[num_partitions].partition_name,usb_dev[num_dev].partition[num_partitions].mount_path);


                 }


            }

        }

    }

}




/*
remove@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host9/target9:0:0/9:0:0:0/scsi_generic/sg0
remove@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host9/target9:0:0/9:0:0:0/scsi_device/9:0:0:0
remove@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host9/target9:0:0/9:0:0:0/scsi_disk/9:0:0:0
remove@/block/sdg/sdg5
remove@/block/sdg/sdg1
remove@/devices/virtual/bdi/8:96
remove@/block/sdg
remove@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host9/target9:0:0/9:0:0:0
remove@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host9/target9:0:0
remove@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host9/scsi_host/host9
remove@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host9
remove@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0
remove@/devices/platform/ehci-brcm.0/usb1/1-1/usb_device/usbdev1.11
remove@/devices/platform/ehci-brcm.0/usb1/1-1



add@/devices/platform/ehci-brcm.0/usb1/1-1
add@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0
add@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host10
add@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host10/scsi_host/host10
add@/devices/platform/ehci-brcm.0/usb1/1-1/usb_device/usbdev1.12
add@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host10/target10:0:0
add@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host1sd 10:0:0:0:0/target10:0:0/10:0:0:0
add@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host10/target10:0:0/sd 10:0:0:0:10:0:0:0/scsi_disk/10:0:0:0
add@/devices/platform/ehci-brcm.0/usd 10:0:0:0:sb1/1-1/1-1:1.0/host10/target10:0:0/10:0:0:0/scsi_device/10:0:0:0
add@/devices/platform/ehci-brcm.0/usb1/1-1/1-1:1.0/host10/target10:0:0/10:0:0sd 10:0:0:0::0/scsi_generic/sg0
add@/devices/virtual/bdi/8:96
add@/block/sdg
add@/block/sdg/sdg1
add@/block/sdg/sdg5
*/




void Delete_Device(char *device_name)
{
    int num_dev=0;
    int num_partitions=0;
    char tmp_cmd[256]={0};

    for(num_dev=0;num_dev<2;num_dev++)
    {
        if(strncmp(usb_dev[num_dev].device_name, device_name, 8) == 0)
        {
            printf("find need remove device %s\n",device_name);
            for(num_partitions=0;num_partitions<usb_dev[num_dev].part_num;num_partitions++)
            {
                if(usb_dev[num_dev].partition[num_partitions].mounted==1)
                {

                   sprintf(tmp_cmd,"umount -l %s",usb_dev[num_dev].partition[num_partitions].mount_path);
                   //printf("%s\n",tmp_cmd);
                   system(tmp_cmd);
                   //printf("%s unmounted at %s\n",usb_dev[num_dev].partition[num_partitions].partition_name,usb_dev[num_dev].partition[num_partitions].mount_path);
                }


            }

            memset(usb_dev[num_dev].device_name,0,sizeof(USB_DEV));

        }

    }


}


void Find_Partitions(char *device_name,int num_dev)
 {
	int len = 0;
	int dev_num = 0;
    int part_num = 0;
    char buf[1024] = {0};     //1024 is enough for save information of FILE partitions
    char hoststr[16] = {0};
    char *delim="\n";
    char *line = NULL;
    char *strtok_tmp_ptr = NULL;
    char *seek = NULL;
    char *seek_sd = NULL;      //USED FOR DEVICE MOUNTED ON SD*
    char *part_blocks = NULL;
    FILE *fd = NULL;


    fd = fopen("/proc/partitions", "r"); //直接操作open打开系统文件
    if(fd != NULL)
    {
        //printf("open file /proc/partitions success\n");
        len = fread(buf, 1, sizeof(buf), fd);
        fclose(fd);
        if(len > 0)
        {
            line = strtok_r(buf, delim, &strtok_tmp_ptr);//以"\n"结束符来截断字符串
            while(line)
            {
                seek_sd = strstr(line, device_name);//成功找到sd后指针seek指向s, seek_sd[0] = 's', seek_sd[1] = 'd'
                if (seek_sd)
                {
                    printf("find %s in /proc/partitions\n",device_name);
                    if(seek_sd[2] >= 'a' && seek_sd[2] <= 'z')
                    {
                        if(seek_sd[3] >= '1' && seek_sd[3] < '9')
                        {

                            sprintf(usb_dev[num_dev].partition[part_num].partition_name,"/dev/%s",seek_sd);
                            //printf("usb_dev[%d].partition[%d].partition_name=%s\n",num_dev,part_num,usb_dev[num_dev].partition[part_num].partition_name);
                            part_num ++;
                            usb_dev[num_dev].part_num=part_num;
                            //printf("usb_dev[%d].part_num=%d\n",num_dev,usb_dev[num_dev].part_num);


                            if (part_num == MAX_PART_NUM)
                            {
                                 //printf("too many parts ignore\n");
                                 break;//too many parts ignore
                            }
                        }
                    }
                }
                else
                {
                    printf("can not find %s in /proc/partitions\n",device_name);

                }
                line = strtok_r(NULL, delim, &strtok_tmp_ptr);
            }
        }
    }
    else
    {
        perror("open /proc/partitions error");
    }

 }




void Get_FileSystem(char *device_name,int num_dev)
 {
    int len=0;
    int num_partitions=0;
    char tmp_command[256]={0};
    FILE *fd = NULL;
    char buf[1024] = {0};     //1024 is enough for save information of FILE partitions
    char *delim="\n";
    char *line = NULL;
    char *strtok_tmp_ptr = NULL;
    char *seek_partition_name = NULL;      //USED FOR DEVICE MOUNTED ON SD*
    char *seek_fat32 = NULL;      //USED FOR DEVICE MOUNTED ON SD*

    {
        if(usb_dev[num_dev].device_name[0] != 0x00)
        {
            sprintf(tmp_command,"fdisk -l %s > /tmp/tmp_fdisk",usb_dev[num_dev].device_name);
            //printf("%s\n",tmp_command);
            system(tmp_command);
            fd = fopen("/tmp/tmp_fdisk", "r"); //直接操作open打开系统文件
            if(fd != NULL)
            {
                //printf("open file /tmp/tmp_fdisk success\n");
                len = fread(buf, 1, sizeof(buf), fd);
                fclose(fd);
                system("rm /tmp/tmp_fdisk");
                if(len > 0)
                {
                    line = strtok_r(buf, delim, &strtok_tmp_ptr);//以"\n"结束符来截断字符串
                    while(line)
                    {
                        for(num_partitions=0;num_partitions<usb_dev[num_dev].part_num;num_partitions++)
                        {
                            seek_partition_name=strstr(line,usb_dev[num_dev].partition[num_partitions].partition_name);
                            if(seek_partition_name)
                            {
                                //printf("find partition_name %s\n",usb_dev[num_dev].partition[num_partitions].partition_name);
                                seek_fat32=strstr(line,"FAT32");
                                if(seek_fat32)
                                {
                                    usb_dev[num_dev].partition[num_partitions].fat32=1;
                                    //printf("usb_dev[%d].partition[%d].partition_name=%s\n",num_dev,num_partitions,usb_dev[num_dev].partition[num_partitions].partition_name);
                                    //printf("usb_dev[%d].partition[%d].fat32=1\n",num_dev,num_partitions);


                                }

                            }

                        }
                        line = strtok_r(NULL, delim, &strtok_tmp_ptr);
                    }
                }
            }
            else
            {
                perror("open /tmp/tmp_fdisk error");
            }

        }

    }



 }


void Mount_NewDevice(int num_dev)
{
    int num_partitions=0;
    char tmp_cmd[256]={0};
    {
        if(usb_dev[num_dev].device_name[0] != 0x00)
        {
            for(num_partitions=0;num_partitions<usb_dev[num_dev].part_num;num_partitions++)
            {
                if((usb_dev[num_dev].partition[num_partitions].fat32 == 1)
                 &&(usb_dev[num_dev].partition[num_partitions].mounted==0))
                 {
                    if(usb_dev[num_dev].type==2)
                    {
                        sprintf(tmp_cmd,"mount -t vfat %s  /mnt/media/usb",usb_dev[num_dev].partition[num_partitions].partition_name);
                        //printf("%s\n",tmp_cmd);
                        sprintf(usb_dev[num_dev].partition[num_partitions].mount_path,"/mnt/media/usb");

                    }
                    else if(usb_dev[num_dev].type==1)
                    {
                        sprintf(tmp_cmd,"mount -t vfat %s  /mnt/media/sdcard",usb_dev[num_dev].partition[num_partitions].partition_name);
                        //printf("%s\n",tmp_cmd);
                        sprintf(usb_dev[num_dev].partition[num_partitions].mount_path,"/mnt/media/sdcard");
                    }
                    system(tmp_cmd);
                    usb_dev[num_dev].partition[num_partitions].mounted=1;
                    //printf("%s mounted at %s\n",usb_dev[num_dev].partition[num_partitions].partition_name,usb_dev[num_dev].partition[num_partitions].mount_path);


                 }


            }

        }

    }


}


void Check_Type(int num_dev)
{

    char *seek=NULL;
    char tmp_device_name[256]={0};
    char tmp_path[1024]={0};
    {
        if(usb_dev[num_dev].device_name[0] != 0x00)
        {
            tmp_device_name[0]='/';
            tmp_device_name[1]='s';
            tmp_device_name[2]='y';
            tmp_device_name[3]='s';
            tmp_device_name[4]='/';
            tmp_device_name[5]='b';
            tmp_device_name[6]='l';
            tmp_device_name[7]='o';
            tmp_device_name[8]='c';
            tmp_device_name[9]='k';
            tmp_device_name[10]='/';
            tmp_device_name[11]=usb_dev[num_dev].device_name[5];
            tmp_device_name[12]=usb_dev[num_dev].device_name[6];
            tmp_device_name[13]=usb_dev[num_dev].device_name[7];
            tmp_device_name[14]='/';
            tmp_device_name[15]='d';
            tmp_device_name[16]='e';
            tmp_device_name[17]='v';
            tmp_device_name[18]='i';
            tmp_device_name[19]='c';
            tmp_device_name[20]='e';
            readlink(tmp_device_name,tmp_path,sizeof(tmp_path));
            seek=strstr(tmp_path,"usb1/1-");
            seek=seek+7;
            usb_dev[num_dev].type=(*seek-0x30);
        }
    }


}





void Add_Device(char *device_name)
{
    int num_dev=0;
    int num_partitions=0;
    char tmp_device_name[256]={0};


    for(num_dev=0;num_dev<2;num_dev++)
    {
        if((usb_dev[num_dev].device_name[0] == 0x00)
         || (strncmp(device_name,usb_dev[num_dev].device_name,8)==0)   )

        {
            //printf("num_dev=%d\n",num_dev);
            memcpy(usb_dev[num_dev].device_name,device_name,8);
            tmp_device_name[0]=device_name[5];
            tmp_device_name[1]=device_name[6];
            tmp_device_name[2]=device_name[7];

            Find_Partitions(tmp_device_name,num_dev);


            Get_FileSystem(device_name,num_dev);

            Check_Type(num_dev);


            Mount_NewDevice(num_dev);

            break;
        }
    }

    if(num_dev == 2)
    {
        printf("an device not removed\n");
    }




}

/*
lrwxrwxrwx    1 root     root             0 Jan  1 00:00 device -> ../../devices/platform/ehci-brcm.0/usb1/1-2/1-2:1.0/host1/target1:0:0/1:0:0:0
*/
void Check_Type_Init(void)
{

    int num_dev=0;
    char *seek=NULL;
    char tmp_device_name[256]={0};
    char tmp_path[1024]={0};


    for(num_dev=0;num_dev<2;num_dev++)
    {
        if(usb_dev[num_dev].device_name[0] != 0x00)
        {
            tmp_device_name[0]='/';
            tmp_device_name[1]='s';
            tmp_device_name[2]='y';
            tmp_device_name[3]='s';
            tmp_device_name[4]='/';
            tmp_device_name[5]='b';
            tmp_device_name[6]='l';
            tmp_device_name[7]='o';
            tmp_device_name[8]='c';
            tmp_device_name[9]='k';
            tmp_device_name[10]='/';
            tmp_device_name[11]=usb_dev[num_dev].device_name[5];
            tmp_device_name[12]=usb_dev[num_dev].device_name[6];
            tmp_device_name[13]=usb_dev[num_dev].device_name[7];
            tmp_device_name[14]='/';
            tmp_device_name[15]='d';
            tmp_device_name[16]='e';
            tmp_device_name[17]='v';
            tmp_device_name[18]='i';
            tmp_device_name[19]='c';
            tmp_device_name[20]='e';
            readlink(tmp_device_name,tmp_path,sizeof(tmp_path));
            seek=strstr(tmp_path,"usb1/1-");
            seek=seek+7;
            usb_dev[num_dev].type=(*seek-0x30);
        }
    }


}




void HotPlugThread(void *pParam)
{
    char device_name[MAX_NAME_LEN]={0};

    int hotplug_sock = init_hotplug_sock();
	memset(usb_dev,0,sizeof(USB_DEV)*2);
	find_devices();



    check_filesystem();

    Check_Type_Init();
    do_mount();





    dump();

    while(1)
    {
        /* Netlink message buffer */
        char buf[UEVENT_BUFFER_SIZE * 2] = {0};
        recv(hotplug_sock, &buf, sizeof(buf), 0);
        if(strlen(buf)>0)
        {
            if(strncmp(buf, "add@/block/", 11) == 0)
            {
                //printf("%s\n",buf);
                memset(device_name,0,sizeof(device_name));
                device_name[0]='/';
                device_name[1]='d';
                device_name[2]='e';
                device_name[3]='v';
                device_name[4]='/';
                device_name[5]=buf[11];
                device_name[6]=buf[12];
                device_name[7]=buf[13];
                //printf("device_name=%s\n",device_name);
                Add_Device(device_name);
                 dump();

            }
            else if(strncmp(buf, "remove@/block/", 14) == 0)
            {
                //printf("%s\n",buf);
                memset(device_name,0,sizeof(device_name));
                device_name[0]='/';
                device_name[1]='d';
                device_name[2]='e';
                device_name[3]='v';
                device_name[4]='/';
                device_name[5]=buf[14];
                device_name[6]=buf[15];
                device_name[7]=buf[16];

                //printf("device_name=%s\n",device_name);
                Delete_Device(device_name);
                    dump();


            }


        }



       /* USB 设备的插拔会出现字符信息，通过比较不同的信息确定特定设备的插拔，在这添加比较代码 */
    }





}








