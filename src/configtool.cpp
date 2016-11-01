



#include "SnifDefines.h"
#include "SnifMsg.h"
#include "hi_type.h"


#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>


#include<arpa/inet.h>



#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/if.h>

#include "main.h"
#include "global.h"
#include "menu_duplexaudio.h"
#include "dib3000.h"
#include "rtc.h"
#include "menu_networkconfig.h"
#include "menu_descramblingconfig.h"


#define MAX_CLIENT  20
#define MAXDATASIZE 1024        //缓冲区大小


/** Indexes in the GLOBALVARS Array */
#define   DECODEDCHANNEL           0
#define   VOLUME                   1
#define   MUTE                     2
#define   STATE                    3
#define   LANGUAGE                 4
#define   OLDSTATE                 5
#define   RADIODECODEDCHANNEL      6
#define   FAVLISTSORT              7

#define   DISPLAYMODE_PARAM        9
#define   SIGNAL_PARAM             10
#define   DECODE_MODE              11
#define   NITSCAN_PARAM            12
#define   OLDDECODEDCHANNEL        13
#define   PARENTAL_CODE            14
#define   UTCOFFSET    		  	   15
#define   REBOOT_FOR_SW232         16
//#define   COUNTRY    			   17//has problem
#define   TIMEZONE    			   18
#define   UISTYLE                  19
#define   LOWPOWER_MODE            20
#define   INFOBAR_TIMEOUT          21
#define   INFOBAR_SETALPHA         22
#define   PARENTAL_MODE            23
#define   PARENTAL_CONTROL         24
#define   SUBTITLES_ONOFF          25
#define   SUBTITLES_LANGUAGE	   26
#define   ASI_MODE                 27
#define   Feed_ElECTRIC            28
#define   RADIO_BACKGROUND         29
#define	  PIDBPOINTER			   30
#define	  PIDBSIZE				   31
#define	  SPAIN_REGIONID		   32
#define   SATELLITEID 			   33
#define   LNBPOWER  			   34
#define   TUNERID  				   35
/*-------------- TCPIP -------------------*/
#define	  IP_PATTERN				36
#define	  DNS_PATTERN				37
#define	  IP_ADD					38
#define	  NETMASK					39
#define	  GATEWAY					40
/*----------------------------------------*/
#define	  GLOBAL_PROXY				41
//record config
#define   RECMIDIUM            42    //filelength<<8 + midium
#define   RECPART              43  //U32 partation +loopenabled <<8 + startup << 16
#define   HDMIINFO             44  //hdmi mode <<8 + hdmi fenbianlv
//#define   FILELENGTH         49
// net relay
#define   RelayInfo            45            //U32 : IPX<<24 + Type<<16 +protocol<<8 +status
#define   REMOTEIP             46
#define   Remoteport           47
#define   COUNTRY    		   48
#define   NUMOFGLGLOBALVARS    49
/* for AVQ menu */










typedef struct _CLIENT_TCP
{
    int fd;                     //客户端socket描述符
    char name[20];              //客户端名称
    struct sockaddr_in addr;    //客户端地址信息结构体
    char data[MAXDATASIZE];     //客户端私有数据指针
} CLIENT_TCP;



void process_client(CLIENT_TCP * client, char *recvbuf, int len);







#define PORT            9910
#define BUFFER_SIZE        128*1024
#define MAX_QUE_CONN_NM   5
#define MAX_SOCK_FD            FD_SETSIZE






#define MAXINTERFACES   16
#define LOCALVSIPPORT    6500
#define REMOTEVSIPPORT  6500
#define BUFFERLENGTH     2*1024
#define JADE_SW

int     socketFd;
struct sockaddr_in LocalAddr;
SNIF_DISCOVERY_PINGRESPONSE_MSG PingRespTemp;
SNIF_IC_RESPONSE_MSG                       GetCapabilitiesRespTemp;
SNIF_GET_CONFIG_RESP_MSG               GetConfigResp_Temp;
SNIF_ERROR_RESP_MSG                         ErrResp_Temp;


U8     BytesOfValueType[8] = {0x00,0x01,0x02,0x04,0x00,0x04,0x10,0x04};

U16    Config_ID_Of_Entity_Unit[CONFIG_COUNT_OF_ENTITY_UNIT] = {0x1001,0x1002,0x1003,0x1004,0x1005,0x1006};
U16    Config_ID_Of_Entity_Ethernet[CONFIG_COUNT_OF_ENTITY_ETHERNET] = {0x2001,0x2002,0x2003,0x2004};
U16    Config_ID_Of_Entity_NETAVRELAY[CONFIG_COUNT_OF_ENTITY_NETAVRELAY] = {0x3001,0x3002,0x3003,0x3004,0x3005};
U16    Config_ID_Of_Entity_SERIALPORT [CONFIG_COUNT_OF_ENTITY_SERIALPORT] = {0x4001,0x4002,0x4003,0x4004};
U16    Config_ID_Of_Entity_STORGE [CONFIG_COUNT_OF_ENTITY_STORGE] = {0x5001,0x5002,0x5003,0x5004};
U16    Config_ID_Of_Entity_RECEIVER[CONFIG_COUNT_OF_ENTITY_RECEIVER]={0x6001,0x6002,0x6003,0x6004,0x6005,0x6006,0x6007};
U16    Config_ID_Of_Entity_VIDEOOUT_HDMI[CONFIG_COUNT_OF_ENTITY_VIDEOOUT_HDMI]={0x7001,0x7002,0x7003,0x7004,0x7005,0x7006,0x7007,0x7008,0x7009};
U16    Config_ID_Of_Entity_MENU_SET[CONFIG_COUNT_OF_ENTITY_MENU_SET]={0x8001,0x8002,0x8003,0x8004};
U16    Config_ID_Of_Entity_VIDEOOUT_SD[CONFIG_COUNT_OF_ENTITY_VIDEOOUT_SD]={0x9001,0x9002,0x9003,0x9004,0x9005,0x9006,0x9007};
U16    Config_ID_Of_Entity_DUPLEX_AUDIO[CONFIG_COUNT_OF_ENTITY_DUPLEX_AUDIO] = {0xA001,0xA002,0xA003,0xA004,0xA005,0xA006,0xA007,0xA008};





void *ReceiveMssage(void *argu);
void *AutoUpHandle(void *argu );
void GetGuid(U8 ENTITY_TYPE,U8 *guid);


void SendPingResponse_Init(U8 *receivebuff,U16 *BytesNum, struct sockaddr_in *sourceaddr);
void SendGetCapabilitiesResponse_Init(U8 *receivebuff,U16 *BytesNum);
U8 GetConfigResp_Init(U8  *Data,U16 length);
U8 SetConfigResp_Init(U8  *Data,U16 length);
U8 SendCommandResp_Init(U8  *Data,U16 length);
void SendErrorCodeInit(U8 *temp);


void GetEthernetPar(U32 *ip,U8 *mac);
//void IP2U32(char* IP_Add, U32 *u_ip);



void GetUnitConfig(void *pbuf,U16 ItemID,U16* pSize);
void GetEthernetConfig(void *pbuf,U16 ItemID,U16* pSize);
void GetNetAVRelayConfig(void *pbuf,U16 ItemID,U16* pSize);
void GetSerialPortConfig(void *pbuf,U16 ItemID,U16* pSize);
void GetStorgeConfig(void *pbuf,U16 ItemID,U16* pSize);
void GetReceiverConfig(void *pbuf,U16 ItemID,U16* pSize);
void GetVideoOutHDMIConfig(void *pbuf,U16 ItemID,U16* pSize);
void GetMenuSet(void *pbuf,U16 ItemID,U16* pSize);
void GetVideoOutSDConfig(void *pbuf,U16 ItemID,U16* pSize);
void GetDuplexAudioConfig(void *pbuf,U16 ItemID,U16* pSize);

void SetUnitConfig(U8 *pbuf,U16 TempCount);
void SetEthernetConfig(U8 *pbuf,U16 TempCount);
void SetNetAVRelayConfig(U8 *pbuf,U16 TempCount);
void SetSerialPortConfig(U8 *pbuf,U16 TempCount);
void SetStorgeConfig(U8 *pbuf,U16 TempCount);
void SetReceiverConfig(U8 *pbuf,U16 TempCount);
void SetVideoOutHDMIConfig(U8 *pbuf,U16 TempCount);
void SetMenuSet(U8 *pbuf,U16 TempCount);
void SetVideoOutSDConfig(U8 *pbuf,U16 TempCount);
void SetDuplexAudioConfig(U8 *pbuf,U16 TempCount);
void FactorySetting();

void GetMAC(unsigned char *mac);


void *TcpConfiguration(void *argu);




U8 R750_strcmp(U8* str1,U8* str2,U32 num)
{
  U32 i=0;
  for(i=0;i<num;i++)
  	{
  	   if(str1[i] != str2[i])
  	   	{
  	   	   return 1;
  	   	}
  	}
    return 0;
}


eRet ConfigTool_Init(void)
{
    int ret;
    pthread_t  RecvMesId,AutoUpId,TcpThreadID;
    int temp[2]={0,0};
    int ReturnValue;

    socketFd = socket (AF_INET, SOCK_DGRAM, 0);
    if ( -1 == socketFd )
    {
	   printf( "Socket Creat Error\n" );
       return(eRet_InvalidParameter);
    }

    int option = 1;
    ReturnValue = setsockopt( socketFd, SOL_SOCKET , SO_REUSEADDR , &option, sizeof(option) );  // 设置地址可复用
    if(ReturnValue  != 0)
    {
    	printf("setsockopt  error  %d\n",ReturnValue);
       return(eRet_InvalidParameter);
    }

    int so_broadcast=1;
    ReturnValue = setsockopt(socketFd, SOL_SOCKET, SO_BROADCAST, &so_broadcast,sizeof(so_broadcast));
    if(ReturnValue != 0)
    {
        printf("setsockopt  error  %d\n",ReturnValue);
        return(eRet_InvalidParameter);
    }

    LocalAddr.sin_addr.s_addr = INADDR_ANY;
    LocalAddr.sin_port = htons (LOCALVSIPPORT);
    int BindResult = bind(socketFd, (struct sockaddr *) &LocalAddr, sizeof(LocalAddr));
    if ( -1 == BindResult )
    {
        printf( "bind error\n" );
        close(socketFd);
        return(eRet_InvalidParameter);
    }

    memset(&RecvMesId,0,sizeof(RecvMesId));
    if((temp[0]=pthread_create(&RecvMesId,NULL,ReceiveMssage,NULL)) !=0)
    {
       printf("Creat ReceiveMessage Thread Fail\n");
       return(eRet_InvalidParameter);
    }
    else
    {
        printf("Creat ReceiveMessage Thread Success\n");
    }

    memset(&AutoUpId,0,sizeof(AutoUpId));
    if((temp[1]=pthread_create(&AutoUpId,NULL,AutoUpHandle,NULL)) !=0)
    {
       printf("Creat ReceiveMessage Thread Fail\n");
       return(eRet_InvalidParameter);
    }
    else
    {
        printf("Creat ReceiveMessage Thread Success\n");
    }


    if((ret=pthread_create(&TcpThreadID,NULL,TcpConfiguration,NULL)) !=0)
    {
        printf("TCP Config Thread Fail\n");
        return(eRet_InvalidParameter);
    }
    else
    {
        printf("TCP Config Thread Success\n");
    }
    return(eRet_Ok);
}



void *ReceiveMssage(void *argu)
{
    printf("ReceiveMssage Starting\n");
    unsigned char TempBuff[BUFFERLENGTH];
    unsigned char MsgType=0x00;
    U16 BytesOfResp=0x00;
    struct sockaddr_in  fromAddr;
    ssize_t result=0x00;
    struct timeval tv= {10,0};
    fd_set readset;
    int ret;
    while(1)
    {
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        FD_ZERO(&readset);
        FD_SET(socketFd,&readset);
        ret = select(socketFd+1,&readset,NULL,NULL,&tv);
        if(ret > 0)
        {
            memset((char *)&fromAddr, 0, (int)sizeof(fromAddr));
            memset(TempBuff,0,BUFFERLENGTH);
            socklen_t fromLen = sizeof(fromAddr);
            result= recvfrom(socketFd, TempBuff, sizeof(TempBuff), 0, (struct sockaddr *)&fromAddr, &fromLen);
            if ( -1 == result )
            {
                printf("recvfrom error \n");
                continue;
            }
            else if(result < 7)
            {
                printf("length < snif Header\n");
            }
            else
            {
                MsgType = TempBuff[2];
                switch(MsgType)
                {
                    case SNIF_MSG_TYPE_D_PING_REQ:
                    {
                        SendPingResponse_Init(TempBuff,&BytesOfResp,&fromAddr);
                        sendto( socketFd,&PingRespTemp, BytesOfResp, 0, (struct sockaddr*)&fromAddr, sizeof(fromAddr));
                        break;
                    }
                    case SNIF_MSG_TYPE_IC_CAPABILITIES_REQ:
                    {
                        SendGetCapabilitiesResponse_Init(TempBuff,&BytesOfResp);
                        sendto( socketFd,&GetCapabilitiesRespTemp, BytesOfResp, 0, (struct sockaddr*)&fromAddr, sizeof(fromAddr));
                        break;
                    }
                    case SNIF_MSG_TYPE_MC_GETCONFIG_REQ:
                    {
                        if(GetConfigResp_Init(TempBuff,result)==1)
                        {
                            BytesOfResp = ntohs(GetConfigResp_Temp.SNIFHeader.Lenght);
                            sendto( socketFd,&GetConfigResp_Temp,BytesOfResp, 0, (struct sockaddr*)&fromAddr, sizeof(fromAddr));
                        }
                        else
                        {
                            SendErrorCodeInit(TempBuff);
                            BytesOfResp = ntohs(ErrResp_Temp.SNIFHeader.Lenght);
                            sendto( socketFd,&ErrResp_Temp,BytesOfResp, 0, (struct sockaddr*)&fromAddr, sizeof(fromAddr));
                        }
                        break;
                    }
                    case SNIF_MSG_TYPE_MC_SETCONFIG_REQ:
                    {
                        if(SetConfigResp_Init(TempBuff,result) == 1)
                        {
                            BytesOfResp = ntohs(ErrResp_Temp.SNIFHeader.Lenght);
                            sendto( socketFd,&ErrResp_Temp,BytesOfResp, 0, (struct sockaddr*)&fromAddr, sizeof(fromAddr));
     			        }
                        else
                        {
                            SendErrorCodeInit(TempBuff);
                            BytesOfResp = ntohs(ErrResp_Temp.SNIFHeader.Lenght);
                            sendto( socketFd,&ErrResp_Temp,BytesOfResp, 0, (struct sockaddr*)&fromAddr, sizeof(fromAddr));
     			        }
                        break;
                    }
                    case SNIF_MSG_TYPE_MC_SENDCOMMAND_REQ:
                    {
                        if(SendCommandResp_Init(TempBuff,result)==1)
                        {
                            BytesOfResp = ntohs(ErrResp_Temp.SNIFHeader.Lenght);
                            sendto( socketFd,&ErrResp_Temp,BytesOfResp,0, (struct sockaddr*)&fromAddr, sizeof(fromAddr));
     		 	        }
                        else
                        {
                            SendErrorCodeInit(TempBuff);
                            BytesOfResp = ntohs(ErrResp_Temp.SNIFHeader.Lenght);
                            sendto( socketFd,&ErrResp_Temp,BytesOfResp, 0, (struct sockaddr*)&fromAddr, sizeof(fromAddr));
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
}



void *AutoUpHandle(void *argu)
{
    printf("AutoUp Start \n");
    struct sockaddr_in desAddr;
    ssize_t send_result;
    U16 Bytes=10;
    static U8 pingreq[14]={0x01,0x00,0x01,0x00,0x01,0x00,0x0E,0xFF,0xFF,0xFF,0xFF,0x19,0x64,0x00};
    while(1)
    {

       memset(&desAddr, 0, sizeof(desAddr));
       desAddr.sin_family = AF_INET;
       desAddr.sin_addr.s_addr = INADDR_BROADCAST;
       desAddr.sin_port = htons (REMOTEVSIPPORT);
       SendPingResponse_Init(pingreq,&Bytes,&desAddr);
       send_result = sendto( socketFd,&PingRespTemp,Bytes, 0, (struct sockaddr*)&desAddr, sizeof(desAddr));
       sleep(30);
    }
}



/***********************************************************************
function:		得到设备的GUID，根据mac地址生成
guid			保存设备的guid
RETURN :		成功返回0 否则返回-1
**************************************************************************/


void SendPingResponse_Init(U8 *receivebuff,U16 *BytesNum, struct sockaddr_in *sourceaddr)
{
    U8 temp_guid[16];
    SNIF_DISCOVERY_PINGREQUEST_MSG *PingRequest = (SNIF_DISCOVERY_PINGREQUEST_MSG *)receivebuff;

    PingRespTemp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
    PingRespTemp.SNIFHeader.MsgType = SNIF_MSG_TYPE_D_PING_RESP;
    PingRespTemp.SNIFHeader.TransactionNumber =htons (0x8000|ntohs((*PingRequest).SNIFHeader.TransactionNumber));
    PingRespTemp.SNIFHeader.Lenght = htons(sizeof(SNIF_DISCOVERY_PINGRESPONSE_MSG));


    PingRespTemp.SNIFDPingInfo.IpAddress=htonl(g_Config.NetWorkSettings.IPAddr);
    PingRespTemp.SNIFDPingInfo.Port = htons(LOCALVSIPPORT);
    PingRespTemp.SNIFDPingInfo.ConnectionType = SNIF_CONNECTION_UDP_VOLATILE;


    GetGuid(0x00,temp_guid);
    memcpy((U8 *)(&(PingRespTemp.guid.Data0[0])),temp_guid,16);
    PingRespTemp.Fabricant = htons(SNIF_MANUFACTURER_ID_ALLTECH);
    PingRespTemp.ProductType=htons(PRODUCT_TYPE_R780);
    PingRespTemp.StatusChanged=HI_FALSE;

    *BytesNum = sizeof(SNIF_DISCOVERY_PINGRESPONSE_MSG) ;

    sourceaddr->sin_addr.s_addr = (*PingRequest).SNIFDPingInfo.IpAddress;
    sourceaddr->sin_port =  (*PingRequest).SNIFDPingInfo.Port;

}



void SendGetCapabilitiesResponse_Init(U8 *receivebuff,U16 *BytesNum)
{
    U8 temp_guid[16];
    U8 i=0;
    SNIF_IC_REQUEST_MSG *GetCapabilitiesRequest = (SNIF_IC_REQUEST_MSG *)receivebuff;

    GetCapabilitiesRespTemp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
    GetCapabilitiesRespTemp.SNIFHeader.MsgType = SNIF_MSG_TYPE_IC_CAPABILITIES_RESP;
    GetCapabilitiesRespTemp.SNIFHeader.TransactionNumber = htons(0x8000 | ntohs((*GetCapabilitiesRequest).SNIFHeader.TransactionNumber));
    GetCapabilitiesRespTemp.SNIFHeader.Lenght = htons(sizeof(SNIF_IC_RESPONSE_MSG));

    GetCapabilitiesRespTemp.ListCount = htons(SNIF_CAPBILITY_COUNT);

    for(i=0;i<SNIF_CAPBILITY_COUNT;i++)
    {
        GetCapabilitiesRespTemp.Capabilities[i].EntityType = i;
        GetGuid(i,temp_guid);
        memcpy(&GetCapabilitiesRespTemp.Capabilities[i].guid.Data0[0],temp_guid,16);
        GetCapabilitiesRespTemp.Capabilities[i].Fabricant= htons(SNIF_MANUFACTURER_ID_ALLTECH);
        GetCapabilitiesRespTemp.Capabilities[i].Version = htons(SNIF_CAPBILITY_VRESION);
    }
    *BytesNum = sizeof(SNIF_IC_RESPONSE_MSG);

}

U8 GetConfigResp_Init(U8  *Data,U16 length)
{
    U8 entity_type=0,i=0;
    U8 temp_guid[16]={0};
    U16 BytesCount=0,ConfigCount=0,RespLength=0,size=0;
    SNIF_GET_CONFIG_REQUEST_MSG *GetConfig_Req =(SNIF_GET_CONFIG_REQUEST_MSG *)Data ;

    U8  MAC_TEMP[6];
    GetMAC(MAC_TEMP);
    if(R750_strcmp((U8 *)MAC_TEMP,(U8 *)&((*GetConfig_Req).guid.Data2[2]),6) == 0)
    {
        entity_type = Data[7];
        ConfigCount = ntohs(GetConfig_Req->ConfigCount);
        BytesCount = sizeof(SNIF_HEADER)+sizeof(GUID)+2+ConfigCount*2;
        if(BytesCount>length)
        {
            return(0);
        }
        else
        {
            switch(entity_type)
            {
                case SNIF_ENTITY_UNIT:
                {
                    if(ConfigCount == 0x00)
                    {
                        for(i=0;i<CONFIG_COUNT_OF_ENTITY_UNIT;i++)
                        {
                            GetUnitConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),Config_ID_Of_Entity_Unit[i],&size);
                            RespLength = RespLength+size;
                        }
                        ConfigCount  = CONFIG_COUNT_OF_ENTITY_UNIT;
                    }
                    else
                    {
                        for(i=0;i<ConfigCount;i++)
                        {
                            GetUnitConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),ntohs(GetConfig_Req->ConfigItemID[i]),&size);
		                    RespLength = RespLength+size;
                        }
                    }
                    GetConfigResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
                    GetConfigResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_MC_GETCONFIG_RESP;
                    GetConfigResp_Temp.SNIFHeader.TransactionNumber =htons(0x8000 | ntohs((*GetConfig_Req).SNIFHeader.TransactionNumber));
                    GetConfigResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_HEADER)+sizeof(GUID)+2+RespLength);
                    GetGuid(entity_type,temp_guid);
                    memcpy(&GetConfigResp_Temp.guid.Data0[0],temp_guid,16);
                    GetConfigResp_Temp.ConfigCount = htons(ConfigCount);
                    break;
                }
                case SNIF_ENTITY_ETHERNET:
                {
                    if(ConfigCount == 0x00)
                    {
                        for(i=0;i<CONFIG_COUNT_OF_ENTITY_ETHERNET;i++)
                        {
                            GetEthernetConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),Config_ID_Of_Entity_Ethernet[i],&size);
		                    RespLength = RespLength+size;
                        }
                        ConfigCount=CONFIG_COUNT_OF_ENTITY_ETHERNET;
                    }
                    else
                    {
                        for(i=0;i<ConfigCount;i++)
                        {
                            GetEthernetConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),ntohs(GetConfig_Req->ConfigItemID[i]),&size);
		                    RespLength = RespLength+size;
                        }
                    }
                    GetConfigResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
                    GetConfigResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_MC_GETCONFIG_RESP;
                    GetConfigResp_Temp.SNIFHeader.TransactionNumber =htons(0x8000 | ntohs((*GetConfig_Req).SNIFHeader.TransactionNumber));
                    GetConfigResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_HEADER)+sizeof(GUID)+2+RespLength);
                    GetGuid(entity_type,temp_guid);
                    memcpy(&GetConfigResp_Temp.guid.Data0[0],temp_guid,16);
                    GetConfigResp_Temp.ConfigCount = htons(ConfigCount);
                    break;
                }
                case SNIF_ENTITY_NETAVRELAY:
                {
                    if(ConfigCount == 0x00)
                    {
                        for(i=0;i<CONFIG_COUNT_OF_ENTITY_NETAVRELAY;i++)
                        {
                            GetNetAVRelayConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),Config_ID_Of_Entity_NETAVRELAY[i],&size);
		                    RespLength = RespLength+size;
                        }
                        ConfigCount=CONFIG_COUNT_OF_ENTITY_NETAVRELAY;
                    }
                    else
                    {
                        for(i=0;i<ConfigCount;i++)
                        {
                            GetNetAVRelayConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),ntohs(GetConfig_Req->ConfigItemID[i]),&size);
		                    RespLength = RespLength+size;
                        }
                    }
                    GetConfigResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
                    GetConfigResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_MC_GETCONFIG_RESP;
                    GetConfigResp_Temp.SNIFHeader.TransactionNumber =htons(0x8000 | ntohs((*GetConfig_Req).SNIFHeader.TransactionNumber));
                    GetConfigResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_HEADER)+sizeof(GUID)+2+RespLength);
                    GetGuid(entity_type,temp_guid);
                    memcpy(&GetConfigResp_Temp.guid.Data0[0],temp_guid,16);
                    GetConfigResp_Temp.ConfigCount = htons(ConfigCount);
                    break;
                }
                case SNIF_ENTITY_SERIALPORT:
                {
                    if(ConfigCount == 0x00)
                    {
                        for(i=0;i<CONFIG_COUNT_OF_ENTITY_SERIALPORT;i++)
                        {
                            GetSerialPortConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),Config_ID_Of_Entity_SERIALPORT[i],&size);
		                    RespLength = RespLength+size;
                        }
                        ConfigCount=CONFIG_COUNT_OF_ENTITY_SERIALPORT;
                    }
                    else
                    {
                        for(i=0;i<ConfigCount;i++)
                        {
                            GetSerialPortConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),ntohs(GetConfig_Req->ConfigItemID[i]),&size);
		                    RespLength = RespLength+size;
                        }
                    }
                    GetConfigResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
                    GetConfigResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_MC_GETCONFIG_RESP;
                    GetConfigResp_Temp.SNIFHeader.TransactionNumber =htons(0x8000 | ntohs((*GetConfig_Req).SNIFHeader.TransactionNumber));
                    GetConfigResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_HEADER)+sizeof(GUID)+2+RespLength);
                    GetGuid(entity_type,temp_guid);
                    memcpy(&GetConfigResp_Temp.guid.Data0[0],temp_guid,16);
                    GetConfigResp_Temp.ConfigCount = htons(ConfigCount);
                    break;
                }
                case SNIF_ENTITY_STORGE:
                {
                    if(ConfigCount == 0x00)
                    {
                        for(i=0;i<CONFIG_COUNT_OF_ENTITY_STORGE;i++)
                        {
                            GetStorgeConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),Config_ID_Of_Entity_STORGE[i],&size);
		                    RespLength = RespLength+size;
                        }
                        ConfigCount=CONFIG_COUNT_OF_ENTITY_STORGE;
                    }
                    else
                    {
                        for(i=0;i<ConfigCount;i++)
                        {
                            GetStorgeConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),ntohs(GetConfig_Req->ConfigItemID[i]),&size);
		                    RespLength = RespLength+size;
                        }
                    }
                    GetConfigResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
                    GetConfigResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_MC_GETCONFIG_RESP;
                    GetConfigResp_Temp.SNIFHeader.TransactionNumber =htons(0x8000 | ntohs((*GetConfig_Req).SNIFHeader.TransactionNumber));
                    GetConfigResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_HEADER)+sizeof(GUID)+2+RespLength);
                    GetGuid(entity_type,temp_guid);
                    memcpy(&GetConfigResp_Temp.guid.Data0[0],temp_guid,16);
                    GetConfigResp_Temp.ConfigCount = htons(ConfigCount);
                    break;
                }
                case SNIF_ENTITY_RECEIVER:
                {
                    if(ConfigCount == 0x00)
                    {
                        for(i=0;i<CONFIG_COUNT_OF_ENTITY_RECEIVER;i++)
                        {
                            GetReceiverConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),Config_ID_Of_Entity_RECEIVER[i],&size);
		                    RespLength = RespLength+size;
                        }
                        ConfigCount	= CONFIG_COUNT_OF_ENTITY_RECEIVER;
                    }
                    else
                    {
                        for(i=0;i<ConfigCount;i++)
                        {
                            GetReceiverConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),ntohs(GetConfig_Req->ConfigItemID[i]),&size);
		                    RespLength = RespLength+size;
                        }
                    }
                    GetConfigResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
                    GetConfigResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_MC_GETCONFIG_RESP;
                    GetConfigResp_Temp.SNIFHeader.TransactionNumber = htons(0x8000 | ntohs((*GetConfig_Req).SNIFHeader.TransactionNumber));
                    GetConfigResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_HEADER)+sizeof(GUID)+2+RespLength);
                    GetGuid(entity_type,temp_guid);
                    memcpy(&GetConfigResp_Temp.guid.Data0[0],temp_guid,16);
                    GetConfigResp_Temp.ConfigCount = htons(ConfigCount);
                    break;
                }
                case SNIF_ENTITY_VIDEOOUT_HDMI:
                {
                    if(ConfigCount == 0x00)
                    {
                        for(i=0;i<CONFIG_COUNT_OF_ENTITY_VIDEOOUT_HDMI;i++)
                        {
                            GetVideoOutHDMIConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),Config_ID_Of_Entity_VIDEOOUT_HDMI[i],&size);
		                    RespLength = RespLength+size;
                        }
                        ConfigCount	= CONFIG_COUNT_OF_ENTITY_VIDEOOUT_HDMI;
                    }
                    else
                    {
                        for(i=0;i<ConfigCount;i++)
                        {
                            GetVideoOutHDMIConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),ntohs(GetConfig_Req->ConfigItemID[i]),&size);
    		                RespLength = RespLength+size;
                        }
                    }
                    GetConfigResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
                    GetConfigResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_MC_GETCONFIG_RESP;
                    GetConfigResp_Temp.SNIFHeader.TransactionNumber = htons(0x8000 | ntohs((*GetConfig_Req).SNIFHeader.TransactionNumber));
                    GetConfigResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_HEADER)+sizeof(GUID)+2+RespLength);
                    GetGuid(entity_type,temp_guid);
                    memcpy(&GetConfigResp_Temp.guid.Data0[0],temp_guid,16);
                    GetConfigResp_Temp.ConfigCount = htons(ConfigCount);
                    break;
                }
                case SNIF_ENTITY_MENUSET:
                {
                    if(ConfigCount == 0x00)
                    {
                        for(i=0;i<CONFIG_COUNT_OF_ENTITY_MENU_SET;i++)
                        {
                            GetMenuSet((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),Config_ID_Of_Entity_MENU_SET[i],&size);
		                    RespLength = RespLength+size;
                        }
                        ConfigCount	= CONFIG_COUNT_OF_ENTITY_MENU_SET;
                    }
                    else
                    {
                        for(i=0;i<ConfigCount;i++)
                        {
                            GetMenuSet((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),ntohs(GetConfig_Req->ConfigItemID[i]),&size);
		                    RespLength = RespLength+size;
                        }
                    }
                    GetConfigResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
                    GetConfigResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_MC_GETCONFIG_RESP;
                    GetConfigResp_Temp.SNIFHeader.TransactionNumber = htons(0x8000 | ntohs((*GetConfig_Req).SNIFHeader.TransactionNumber));
                    GetConfigResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_HEADER)+sizeof(GUID)+2+RespLength);
                    GetGuid(entity_type,temp_guid);
                    memcpy(&GetConfigResp_Temp.guid.Data0[0],temp_guid,16);
                    GetConfigResp_Temp.ConfigCount = htons(ConfigCount);
                    break;
                }
                case SNIF_ENTITY_VIDEOOUT_SD:
                {
                    if(ConfigCount == 0x00)
                    {
                        for(i=0;i<CONFIG_COUNT_OF_ENTITY_VIDEOOUT_SD;i++)
                        {
                            GetVideoOutSDConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),Config_ID_Of_Entity_VIDEOOUT_SD[i],&size);
		                    RespLength = RespLength+size;
                        }
                        ConfigCount	= CONFIG_COUNT_OF_ENTITY_VIDEOOUT_SD;
                    }
                    else
                    {
                        for(i=0;i<ConfigCount;i++)
                        {
                            GetVideoOutSDConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),ntohs(GetConfig_Req->ConfigItemID[i]),&size);
		                    RespLength = RespLength+size;
                        }
                    }
                    GetConfigResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
                    GetConfigResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_MC_GETCONFIG_RESP;
                    GetConfigResp_Temp.SNIFHeader.TransactionNumber = htons(0x8000 | ntohs((*GetConfig_Req).SNIFHeader.TransactionNumber));
                    GetConfigResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_HEADER)+sizeof(GUID)+2+RespLength);
                    GetGuid(entity_type,temp_guid);
                    memcpy(&GetConfigResp_Temp.guid.Data0[0],temp_guid,16);
                    GetConfigResp_Temp.ConfigCount = htons(ConfigCount);
                    break;
                }
                case SNIF_ENTITY_DUPLEX_AUDIO:
                {
                    if(ConfigCount == 0x00)
                    {
                        for(i=0;i<CONFIG_COUNT_OF_ENTITY_DUPLEX_AUDIO;i++)
                        {
                            GetDuplexAudioConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),Config_ID_Of_Entity_DUPLEX_AUDIO[i],&size);
		                    RespLength = RespLength+size;
                        }
                        ConfigCount	= CONFIG_COUNT_OF_ENTITY_DUPLEX_AUDIO;
                    }
                    else
                    {
                        for(i=0;i<ConfigCount;i++)
                        {
                            GetDuplexAudioConfig((void *)(((U8 *)GetConfigResp_Temp.ConfigItem)+RespLength),ntohs(GetConfig_Req->ConfigItemID[i]),&size);
		                    RespLength = RespLength+size;
                        }
                    }
                    GetConfigResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
                    GetConfigResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_MC_GETCONFIG_RESP;
                    GetConfigResp_Temp.SNIFHeader.TransactionNumber = htons(0x8000 | ntohs((*GetConfig_Req).SNIFHeader.TransactionNumber));
                    GetConfigResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_HEADER)+sizeof(GUID)+2+RespLength);
                    GetGuid(entity_type,temp_guid);
                    memcpy(&GetConfigResp_Temp.guid.Data0[0],temp_guid,16);
                    GetConfigResp_Temp.ConfigCount = htons(ConfigCount);
                    break;
                }
                default:
                    break;
            }
        }
        return(1);
    }
    else
    {
        printf("\n mac error \n");
        return(0);
    }
}




U8 SetConfigResp_Init(U8  *Data,U16 length)
{
    SNIF_SET_CONFIG_REQ_MSG *pSetReq_Temp = (SNIF_SET_CONFIG_REQ_MSG*)Data;
    U8 entity_type;
    U16 ConfigCount;

    U8  MAC_TEMP[6];
    GetMAC(MAC_TEMP);

    if(R750_strcmp((U8 *)MAC_TEMP,(U8 *)&((*pSetReq_Temp).guid.Data2[2]),6) == 0)
    {
        ErrResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
        ErrResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_ER_ERROR;
        ErrResp_Temp.SNIFHeader.TransactionNumber = htons(0x8000 | ntohs((*pSetReq_Temp).SNIFHeader.TransactionNumber));
        ErrResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_ERROR_RESP_MSG));
        ErrResp_Temp.request_type = (*pSetReq_Temp).SNIFHeader.MsgType;
        ErrResp_Temp.error_code = htons(SNIF_ERROR_CODE_SUCCESS);

        entity_type=Data[7];
        ConfigCount = ntohs(pSetReq_Temp->ConfigCount);
        switch(entity_type)
        {
            case SNIF_ENTITY_UNIT:
            {
                SetUnitConfig(pSetReq_Temp->ConfigItem,ConfigCount);
                break;
            }
            case SNIF_ENTITY_ETHERNET:
		    {
                SetEthernetConfig(pSetReq_Temp->ConfigItem,ConfigCount);
                break;
            }
            case SNIF_ENTITY_NETAVRELAY:
            {
                SetNetAVRelayConfig(pSetReq_Temp->ConfigItem,ConfigCount);
		        break;
            }
            case SNIF_ENTITY_SERIALPORT:
            {
                SetSerialPortConfig(pSetReq_Temp->ConfigItem,ConfigCount);
                break;
            }
            case SNIF_ENTITY_STORGE:
            {
                SetStorgeConfig(pSetReq_Temp->ConfigItem,ConfigCount);
                break;
            }
            case SNIF_ENTITY_RECEIVER:
		    {
                SetReceiverConfig(pSetReq_Temp->ConfigItem,ConfigCount);
                break;
		    }
            case SNIF_ENTITY_VIDEOOUT_HDMI:
		    {
                SetVideoOutHDMIConfig(pSetReq_Temp->ConfigItem,ConfigCount);
                break;
		    }
            case SNIF_ENTITY_MENUSET:
            {
                SetMenuSet(pSetReq_Temp->ConfigItem,ConfigCount);
		        break;
		    }
            case SNIF_ENTITY_VIDEOOUT_SD:
		    {
                SetVideoOutSDConfig(pSetReq_Temp->ConfigItem,ConfigCount);
		        break;
		    }
            case SNIF_ENTITY_DUPLEX_AUDIO:
		    {
                SetDuplexAudioConfig(pSetReq_Temp->ConfigItem,ConfigCount);
		        break;
		    }
            default:
                break;
        }
        return(1);
    }
    else
    {
        printf("mac error\n");
        return(0);
    }
}








U8 SendCommandResp_Init(U8  *Data,U16 length)
{
    static U8 UpdateFlag=0;
    U32 par1 = 0;
    U32 par2 = 0;
    U32 DevType=0;
    U32 DevStat=0;
    pthread_t  RecvFileID;

    U8 entity_type;
    entity_type=Data[7];
    SNIF_SEND_COMMAND_REQ_MSG *pSendCommandReq = (SNIF_SEND_COMMAND_REQ_MSG *)Data;

   U8  MAC_TEMP[6];
   GetMAC(MAC_TEMP);

   if(R750_strcmp((U8 *)MAC_TEMP,(U8 *)&((*pSendCommandReq).guid.Data2[2]),6) == 0)
    {
        par1 = ntohl(pSendCommandReq->par[0]);
        par2 = ntohl(pSendCommandReq->par[1]);

        ErrResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
        ErrResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_ER_ERROR;
        ErrResp_Temp.SNIFHeader.TransactionNumber = htons(0x8000 | ntohs((*pSendCommandReq).SNIFHeader.TransactionNumber));
        ErrResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_ERROR_RESP_MSG));

        ErrResp_Temp.request_type = (*pSendCommandReq).SNIFHeader.MsgType;
        ErrResp_Temp.error_code = htons(SNIF_ERROR_CODE_SUCCESS);

        switch(pSendCommandReq->commandid)
        {
            case 0:
                if((par1 == 0x01)&&(par2 == 0x00))
                {
                    //WDT_Running_Flag = FALSE;
                    system("reboot");
                }
                else if((par1 == 0x02)&&(par2 == 0x00))
                {
                }
                else if((par1 == 0x03)&&(par2 == 0x00))
                {
                    FactorySetting();
                }
                else if((par1 == 0x04)&&(par2 == 0x00))
                {
                }
                else if((par1 == 0x05)&&(par2 == 0x00))//open ts
                {
                }
                else if((par1 == 0x06)&&(par2 == 0x00))//close ts
                {
                }
                break;
            default:
                break;
        }
        return(1);
    }
    else
   	{
        printf("GUID not eaqual\n");
	    return(0);
   	}
}







void SendErrorCodeInit(U8 *temp)
{
    SNIF_HEADER *pHeader = (SNIF_HEADER *)temp;
    ErrResp_Temp.SNIFHeader.ProtocolVersion = htons(SNIF_PROTOCOL_VERSION);
    ErrResp_Temp.SNIFHeader.MsgType = SNIF_MSG_TYPE_ER_ERROR;
    ErrResp_Temp.SNIFHeader.TransactionNumber = htons(0x8000 | ntohs((*pHeader).TransactionNumber));
    ErrResp_Temp.SNIFHeader.Lenght = htons(sizeof(SNIF_ERROR_RESP_MSG));
    ErrResp_Temp.request_type = (*pHeader).MsgType;
    ErrResp_Temp.error_code = htons(SNIF_ERROR_CODE_FAILURE);
}




void GetMAC(unsigned char *mac)
{
	struct ifreq ifreq_t;
	int sock;

	if ((sock = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror ("socket");
		return;
	}
	strcpy (ifreq_t.ifr_name, "eth0");
	if (ioctl (sock, SIOCGIFHWADDR, &ifreq_t) < 0)
	{
		perror ("ioctl");
		close (sock);
		return;
	}
	mac[0]=(unsigned char) ifreq_t.ifr_hwaddr.sa_data[0];
    mac[1]=(unsigned char) ifreq_t.ifr_hwaddr.sa_data[1];
    mac[2]=(unsigned char) ifreq_t.ifr_hwaddr.sa_data[2];
    mac[3]=(unsigned char) ifreq_t.ifr_hwaddr.sa_data[3];
    mac[4]=(unsigned char) ifreq_t.ifr_hwaddr.sa_data[4];
    mac[5]=(unsigned char) ifreq_t.ifr_hwaddr.sa_data[5];
	close (sock);
}

void GetGuid(U8 ENTITY_TYPE,U8 *guid)
{
   U8  MAC_TEMP[6];
   GetMAC(MAC_TEMP);
   memset(guid, 0, 16);
   guid[0] = ENTITY_TYPE;
   memcpy(&guid[10],MAC_TEMP,sizeof(MAC_TEMP));
}

void GetUnitConfig(void *pbuf,U16 ItemID,U16* pSize)
{
    U16 BytesOfString;
    U8   ValueType;
    U16 temp_size;
    CHANNEL_ORG ChannelList[32];
    U16 FavoriteListNumber;
	U16 ChannelIndex;
	U16 ChannelIndexInList;
	U16 TSIndex;
	char TmpBuf[128]={0};
    switch(ItemID)
    {
        case CONFIG_SYSTEM_DEVICE_TYPE:
            //printf("CONFIG_SYSTEM_DEVICE_TYPE\n");
            ValueType = SNIF_VALUE_TYPE_STRING;
            ItemID = htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            BytesOfString = strlen(DEVICE_TYPE);
            temp_size = htons(BytesOfString);
            memcpy(pbuf+3,(U8*)&temp_size,2);
            memcpy(pbuf+5,DEVICE_TYPE,BytesOfString);
            *pSize=5+BytesOfString;
            break;
        case CONFIG_SYSTEM_FIRMWARE_VERSION:
            //printf("CONFIG_SYSTEM_FIRMWARE_VERSION\n");
            ValueType = SNIF_VALUE_TYPE_STRING;
            ItemID = htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            BytesOfString = strlen(BUILD_DATE);
            temp_size = htons(BytesOfString);
            memcpy(pbuf+3,(U8*)&temp_size,2);
            memcpy(pbuf+5,BUILD_DATE,BytesOfString);
            *pSize=5+BytesOfString;
            break;
        case CONFIG_SYSTEM_HARDWARE_VERSION:
            //printf("CONFIG_SYSTEM_HARDWARE_VERSION\n");
            ValueType = SNIF_VALUE_TYPE_STRING;
            ItemID = htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            BytesOfString = strlen(HARD_VER);
            temp_size = htons(BytesOfString);
            memcpy(pbuf+3,(U8*)&temp_size,2);
            memcpy(pbuf+5,HARD_VER,BytesOfString);
            *pSize=5+BytesOfString;
            break;
        case CONFIG_SYSTEM_FPGA_VERSION:
            //printf("CONFIG_SYSTEM_FPGA_VERSION\n");
            {
                U8 version=0x22;
                U8 power_stat;
                char tempbuf[80]={0};

                Get_D330_Ver(&version); // tux add 20110624
                if(((version>>4)&0x0f)==0&&(version&0x0f)==0)
                {
                    BytesOfString = sprintf(tempbuf, "FPGA Ver : Unknown  Feed : Unknown");// tux add 20110624
		        }
                else
                {
                    BytesOfString = sprintf(tempbuf, "FPGA Ver : %d.%d  Feed : %d", (version>>4)&0x0f,version&0x0f,power_stat);// tux add 20110624
		        }
                ValueType = SNIF_VALUE_TYPE_STRING;
                ItemID = htons(ItemID);
                memcpy(pbuf,(U8 *)&ItemID,2);
                memcpy(pbuf+2,(U8 *)&ValueType,1);
                //BytesOfString = strlen(TmpBuf);
                temp_size = htons(BytesOfString);
                memcpy(pbuf+3,(U8*)&temp_size,2);
                memcpy(pbuf+5,tempbuf,BytesOfString);
                *pSize=5+BytesOfString;
                break;
            }
            case CONFIG_SYSTEM_DATE_TIME:
            //printf("CONFIG_SYSTEM_DATE_TIME\n");
            {
			    char tempbuf_time[80]={0};
			    time_t now;
			    struct tm   *timenow;
			    time(&now);
			    timenow = localtime(&now);
			    timenow->tm_year=timenow->tm_year+1900;
			    timenow->tm_mon=timenow->tm_mon+1;
			    BytesOfString=sprintf(tempbuf_time, "%04d%02d%02d%02d%02d%02d", timenow->tm_year, timenow->tm_mon, timenow->tm_mday,timenow->tm_hour,timenow->tm_min,timenow->tm_sec);
			    ValueType = SNIF_VALUE_TYPE_STRING;
			    ItemID = htons(ItemID);
			    memcpy(pbuf,(U8 *)&ItemID,2);
			    memcpy(pbuf+2,(U8 *)&ValueType,1);
			    temp_size = htons(BytesOfString);
			    memcpy(pbuf+3,(U8*)&temp_size,2);
			    memcpy(pbuf+5,tempbuf_time,BytesOfString);
			    *pSize=5+BytesOfString;
		        break;
            }
            case CONFIG_SYSTEM_FLASH_READ:
			{
				unsigned int GLOBALVARS[NUMOFGLGLOBALVARS]={0};
				memset((char *)&GLOBALVARS,0,sizeof(GLOBALVARS));
				GLOBALVARS[DECODEDCHANNEL]				=g_Channel.CurrentChannel;
				GLOBALVARS[VOLUME]        				=g_Config.Volume;
				GLOBALVARS[MUTE]         				=0;
				GLOBALVARS[STATE]        				=0;
				GLOBALVARS[LANGUAGE]     				=0;
				GLOBALVARS[OLDSTATE]      				=0;
				GLOBALVARS[RADIODECODEDCHANNEL]         =0;
				GLOBALVARS[FAVLISTSORT]         		=0;
				GLOBALVARS[8]         				    =0;
				GLOBALVARS[DISPLAYMODE_PARAM]         	=0;
				GLOBALVARS[SIGNAL_PARAM]         		=0;
				GLOBALVARS[DECODE_MODE]         		=0;
				GLOBALVARS[NITSCAN_PARAM]         		=0;
				GLOBALVARS[OLDDECODEDCHANNEL]         	=0;
				GLOBALVARS[PARENTAL_CODE]         		=0;
				GLOBALVARS[UTCOFFSET]         			=0;
				GLOBALVARS[REBOOT_FOR_SW232]         	=0;
				GLOBALVARS[17]         					=0;
				GLOBALVARS[TIMEZONE]         			=0;
				GLOBALVARS[UISTYLE]         			=0;
				GLOBALVARS[LOWPOWER_MODE]         		=0;
				GLOBALVARS[INFOBAR_TIMEOUT]        		=0;
				GLOBALVARS[INFOBAR_SETALPHA]         	=0;
				GLOBALVARS[PARENTAL_MODE]         		=0;
				GLOBALVARS[PARENTAL_CONTROL]         	=0;
				GLOBALVARS[SUBTITLES_ONOFF]         	=0;
				GLOBALVARS[SUBTITLES_LANGUAGE]         	=0;
				GLOBALVARS[ASI_MODE]         			=0;
				GLOBALVARS[Feed_ElECTRIC]         		=0;
				GLOBALVARS[RADIO_BACKGROUND]         	=0;
				GLOBALVARS[PIDBPOINTER]         		=0;
				GLOBALVARS[PIDBSIZE]         			=0;
				GLOBALVARS[SPAIN_REGIONID]         		=0;
			    GLOBALVARS[SATELLITEID]         		=0;
				GLOBALVARS[LNBPOWER]         			=0;
				GLOBALVARS[TUNERID]         			=0;
				GLOBALVARS[IP_PATTERN]         			=g_Config.NetWorkSettings.DHCPEnable;
				GLOBALVARS[DNS_PATTERN]         		=0;
				GLOBALVARS[IP_ADD]         				=g_Config.NetWorkSettings.IPAddr;
				GLOBALVARS[NETMASK]         			=g_Config.NetWorkSettings.NetMask;
				GLOBALVARS[GATEWAY]         			=g_Config.NetWorkSettings.GateWay;
				GLOBALVARS[GLOBAL_PROXY]        	 	=0;
				GLOBALVARS[RECMIDIUM]         			=((U8)g_Config.RecSettings.PreferredMedia) | (((U8)g_Config.RecSettings.RecFileSize)<<8);
				GLOBALVARS[RECPART]         			=(((U8)g_Config.RecSettings.LoopRecEnable)<<8)|(((U8)g_Config.RecSettings.RecOnStartEnable)<<16) ;
				GLOBALVARS[HDMIINFO]         			=0;
				GLOBALVARS[RelayInfo]         			=((U8)g_Config.NetRelaySettings.Enable) | (((U8)g_Config.NetRelaySettings.protocal)<<8) |(((U8)g_Config.NetRelaySettings.Type)<<16) ;
				GLOBALVARS[REMOTEIP]         			=0;
				GLOBALVARS[Remoteport]         			=0;
				GLOBALVARS[COUNTRY]         			=0;


                ValueType = SNIF_VALUE_TYPE_STRING;
                ItemID = htons(ItemID);
                memcpy(pbuf,(U8 *)&ItemID,2);
                memcpy(pbuf+2,(U8 *)&ValueType,1);
                BytesOfString = sizeof(GLOBALVARS)+1+2+14*g_Channel.ChannelCount;
                temp_size = htons(BytesOfString);
                memcpy(pbuf+3,(U8*)&temp_size,2);
                memcpy(pbuf+5,(U8*)GLOBALVARS,sizeof(GLOBALVARS));
                memcpy(pbuf+5+sizeof(GLOBALVARS),(U8*)&g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0],1);
                memcpy(pbuf+6+sizeof(GLOBALVARS),(U8*)&g_Channel.ChannelCount,2);
                for(unsigned int i=0;i<g_Channel.ChannelCount;i++)
             	{
                    ChannelList[i].Fre =  g_Channel.ChannelList[i].Frequency;
            		ChannelList[i].Band =  g_Channel.ChannelList[i].BandWidth;
            		ChannelList[i].Vpid = g_Channel.ChannelList[i].SubChannels[0].VidPid;
            		ChannelList[i].Apid = g_Channel.ChannelList[i].SubChannels[0].AudPid;
            		ChannelList[i].Pcrvid = g_Channel.ChannelList[i].SubChannels[0].PcrPid;
            		//ChannelList[i].AVtype = (g_Channel.ChannelList[i].SubChannels[0].AudType<<8) | g_Channel.ChannelList[i].SubChannels[0].VidType ;
            		ChannelList[i].AVtype =  g_Channel.ChannelList[i].SubChannelNum;
             	}
                memcpy(pbuf+8+sizeof(GLOBALVARS),(U8*)ChannelList,sizeof(CHANNEL_ORG)*g_Channel.ChannelCount);
		        *pSize=5+BytesOfString;
                break;
            }
            case CONFIG_SYSTEM_MENUCON:
			{
				unsigned int MenuCon[32]={1};
				for(int j=0;j<32;j++)
				{
					MenuCon[j]=1;
				}

                ValueType = SNIF_VALUE_TYPE_STRING;
                ItemID = htons(ItemID);
                memcpy(pbuf,(U8 *)&ItemID,2);
                memcpy(pbuf+2,(U8 *)&ValueType,1);
                BytesOfString = 32*4;
                temp_size = htons(BytesOfString);
                memcpy(pbuf+3,(U8*)&temp_size,2);
                memcpy(pbuf+5,(U8*)MenuCon,32*4);
                *pSize=5+BytesOfString;
                break;
            }

            default:
                break;
    }
}




void GetEthernetConfig(void *pbuf,U16 ItemID,U16* pSize)
{
    U8   ValueType;
    U32 temp_u32;
	printf("GetEthernetConfig\n");
    switch(ItemID)
    {
        case CONFIG_NET_LOCAL_IP_ADDRESS:
            //printf("CONFIG_SYSTEM_DEVICE_TYPE\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            ItemID = htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            temp_u32 = htonl(g_Config.NetWorkSettings.IPAddr);
            memcpy(pbuf+3,(U8*)&temp_u32,4);
            *pSize=7;
            break;
        case CONFIG_NET_LOCAL_NETMASK:
            //printf("CONFIG_NET_LOCAL_NETMASK\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            ItemID = htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            temp_u32 = htonl(g_Config.NetWorkSettings.NetMask);
            memcpy(pbuf+3,(U8*)&temp_u32,4);
            *pSize=7;
            break;
        case CONFIG_NET_LOCAL_GATEWAY:
            //printf("CONFIG_NET_LOCAL_GATEWAY\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            ItemID = htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            temp_u32 = htonl(g_Config.NetWorkSettings.GateWay);
            memcpy(pbuf+3,(U8*)&temp_u32,4);
            *pSize=7;
            break;
        case CONFIG_NET_DHCP_STATE:
            //printf("CONFIG_NET_DHCP_STATE\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            ItemID = htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            temp_u32 = htonl(g_Config.NetWorkSettings.DHCPEnable);
            memcpy(pbuf+3,(U8*)&temp_u32,4);
            *pSize=7;
            break;
        default:
            //printf("default\n");
            break;
    }
}





void GetNetAVRelayConfig(void *pbuf,U16 ItemID,U16* pSize)
{
   U8   ValueType;
   //printf("ConfigId = %x\n",ItemID);
   U32 temp;
   switch(ItemID)
   {
   	    case CONFIG_NETAVRELAY_TYPE:
	 	{
			//printf("CONFIG_NETAVRELAY_TYPE\n");
			ValueType = SNIF_VALUE_TYPE_UINT;
			U32 t_data;
			t_data = g_Config.NetRelaySettings.Type;
			ItemID=htons(ItemID);
		    memcpy(pbuf,(U8 *)&ItemID,2);
			memcpy(pbuf+2,(U8 *)&ValueType,1);
			t_data=htonl(t_data);
	        memcpy(pbuf+3,(U8*)&t_data,4);
			*pSize=7;
		  	break;
		}
		case CONFIG_NETAVRELAY_REMOTEIP:
		{
			//printf("CONFIG_NETAVRELAY_REMOTEIP\n");
	   		ValueType = SNIF_VALUE_TYPE_UINT;
	   		ItemID=htons(ItemID);
	   	    memcpy(pbuf,(U8 *)&ItemID,2);
	   		memcpy(pbuf+2,(U8 *)&ValueType,1);
			U32 t_data;
			t_data=g_Config.NetRelaySettings.RemoteIP;
	   		t_data = htonl(t_data);
	        memcpy(pbuf+3,(U8*)&t_data,4);
	   		*pSize=7;
	   	  	break;
	    }
	  case CONFIG_NETAVRELAY_PORT:
	  	{
	 	//printf("CONFIG_NETAVRELAY_PORT\n");
		ValueType = SNIF_VALUE_TYPE_UINT;
		ItemID=htons(ItemID);
	    memcpy(pbuf,(U8 *)&ItemID,2);
		memcpy(pbuf+2,(U8 *)&ValueType,1);
		temp = htonl(g_Config.NetRelaySettings.RemotePort);
        memcpy(pbuf+3,(U8*)&temp,4);
		*pSize=7;
	  	break;
	  }
	  case CONFIG_NETAVRELAY_PROTOCAL:
	  	{
	 	//printf("CONFIG_NETAVRELAY_PROTOCAL\n");
		ValueType = SNIF_VALUE_TYPE_UINT;
		U32 t_data1;
		t_data1 = g_Config.NetRelaySettings.protocal;
		t_data1=htonl(t_data1);
		ItemID=htons(ItemID);
	    memcpy(pbuf,(U8 *)&ItemID,2);
		memcpy(pbuf+2,(U8 *)&ValueType,1);
        memcpy(pbuf+3,(U8*)&t_data1,4);
		*pSize=7;
	  	break;
	  }
	  case CONFIG_NETAVRELAY_STATUS:
	  	{
	 	//printf("CONFIG_NETAVRELAY_STATUS\n");
		ValueType = SNIF_VALUE_TYPE_UINT;
		U32 t_data2;
		t_data2 = g_Config.NetRelaySettings.Enable;
		ItemID=htons(ItemID);
	    memcpy(pbuf,(U8 *)&ItemID,2);
		memcpy(pbuf+2,(U8 *)&ValueType,1);
		t_data2=htonl(t_data2);
        memcpy(pbuf+3,(U8*)&t_data2,4);
		*pSize=7;
	  	break;
	  }
	  default:
	  	//printf("default\n");
	       break;

   }
}


void GetSerialPortConfig(void *pbuf,U16 ItemID,U16* pSize)
{

}

void GetStorgeConfig(void *pbuf,U16 ItemID,U16* pSize)
{
    U8   ValueType;
    U32 FieldValue[5];
	U32 t_data=0;

	FieldValue[0] = (U8)g_Config.RecSettings.PreferredMedia;
	FieldValue[1] = 0 ;
	FieldValue[2] = (U8)g_Config.RecSettings.LoopRecEnable;// tux add
	FieldValue[3] = (U8)g_Config.RecSettings.RecFileSize;
	FieldValue[4] = (U8)g_Config.RecSettings.RecOnStartEnable;
    switch(ItemID)
    {
        case CONFIG_STORGE_TYPE:
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[0];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_STORGE_PARTITION:
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[1];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_STORGE_ISLOOP:
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[2];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_STORGE_SIZE:
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[3];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_STARTUP_REC:
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[4];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        default:
            //printf("default\n");
            break;
    }
}

void GetReceiverConfig(void *pbuf,U16 ItemID,U16* pSize)
{

    U8   ValueType;

    switch(ItemID)
    {
        case CONFIG_RECEIVER_DESCRAMBLE_TYPE:
            //printf("CONFIG_RECEIVER_DESCRAMBLE_TYPE\n");
            ValueType = SNIF_VALUE_TYPE_STRING;
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            U16 tempsize;
            tempsize=ntohs(33);
            memcpy(pbuf+3,(U8 *)&tempsize,2);
            memcpy(pbuf+5,(U8*)&g_Channel.ChannelList[g_Channel.CurrentChannel].SubChannels[g_Channel.CurrentSubChannel].EncryptKeys[0],33);
            *pSize=38;
            break;
        case CONFIG_RECEIVER_DESCRAMBLE_PASSWORD:
            *pSize=0;
            break;
        case CONFIG_RECEIVER_DOCODED_CHANNEL:
            //printf("CONFIG_RECEIVER_DOCODED_CHANNEL\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            U32 t_data1;
            t_data1 = g_Channel.CurrentChannel;
            t_data1=htonl(t_data1);
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            memcpy(pbuf+3,(U8*)&t_data1,4);
            *pSize=7;
            break;
        case CONFIG_RECEIVER_DOCODED_FRE:
            //printf("CONFIG_RECEIVER_DOCODED_FRE\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            U32 t_data2;
            t_data2 = g_Channel.ChannelList[g_Channel.CurrentChannel].Frequency;
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            // t_data2 = htonl(t_data2);
            memcpy(pbuf+3,(U8*)&t_data2,4);
            *pSize=7;
            break;
        case CONFIG_RECEIVER_DOCODED_BAND:
            //printf("CONFIG_RECEIVER_DOCODED_BAND\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            U32 t_mod;
            t_mod = g_Channel.ChannelList[g_Channel.CurrentChannel].BandWidth;
            // t_mod = htons(t_mod);
            memcpy(pbuf+3,(U8*)&t_mod,4);
            *pSize=7;
            break;
        case CONFIG_RECEIVER_DOCODED_INFO:
		{
            //printf("CONFIG_RECEIVER_DOCODED_INFO\n");
            ValueType = SNIF_VALUE_TYPE_STRING;
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            U16 size_2 = htons(10);
            memcpy(pbuf+3,(U8 *)&size_2,2);
            S16 Locked;
            U32 Quality;
            U32 Strength;
            S16 Strength1;
            S16 Strength2;
            S16 Quality1;
            S16 Quality2;
            GetDibcomTunerParams(&Locked,&Quality,&Strength,&Strength1,&Strength2,&Quality1,&Quality2);
            Locked = (U16) Locked;
            memcpy(pbuf+5,(U8 *)&Locked,2);

            // Strength1 = htons(Strength1);
            memcpy(pbuf+7,(U8 *)&Strength1,2);

            // Quality1 = htons(Quality1);
            memcpy(pbuf+9,(U8 *)&Quality1,2);

            // Strength2 = htons(Strength2);
            memcpy(pbuf+11,(U8 *)&Strength2,2);

            // Quality2 = htons(Quality2);
            memcpy(pbuf+13,(U8 *)&Quality2,2);
            *pSize=15;
            break;
        }
        case CONFIG_RECEIVER_DELETED_CHAN:
            *pSize=0;
            break;
        default:
            //printf("default\n");
            break;
    }
}




void GetVideoOutHDMIConfig(void *pbuf,U16 ItemID,U16* pSize)
{
    U8   ValueType;
    //printf("ConfigId = %x\n",ItemID);
    U32 t_data;
    static U8 FieldValue[9];

    FieldValue[0] = 0;
    FieldValue[3] = 0;
    switch(ItemID)
    {
        case CONFIG_VIDEOOUT_HDMI_MODE:
            //printf("CONFIG_VIDEOOUT_HDMI_MODE\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[0];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_VIDEOOUT_HDMI_HDCP:
            //printf("CONFIG_VIDEOOUT_HDMI_HDCP\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[1];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_VIDEOOUT_HDMI_HPD:
            //printf("CONFIG_VIDEOOUT_HDMI_HPD\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[2];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_VIDEOOUT_HDMI_VIDEO_FORMATS:
            //printf("CONFIG_VIDEOOUT_HDMI_VIDEO_FORMATS\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[3];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_VIDEOOUT_HDMI_PIXEL_ENCODING:
            //printf("CONFIG_VIDEOOUT_HDMI_PIXEL_ENCODING\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[4];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_VIDEOOUT_HDMI_PICTURE_AR:
            //printf("CONFIG_VIDEOOUT_HDMI_PICTURE_AR\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[5];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_VIDEOOUT_HDMI_ACTIVE_AR:
            //printf("CONFIG_VIDEOOUT_HDMI_ACTIVE_AR\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[6];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_VIDEOOUT_HDMI_HDCP_TRACES:
            //printf("CONFIG_VIDEOOUT_HDMI_HDCP_TRACES\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[7];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        case CONFIG_VIDEOOUT_HDMI_MODE_DEBUG:
            //printf("CONFIG_VIDEOOUT_HDMI_MODE_DEBUG\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
            t_data = (U32)FieldValue[8];
            ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
            memcpy(pbuf+2,(U8 *)&ValueType,1);
            t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
            *pSize=7;
            break;
        default:
            //printf("default\n");
            break;
    }
}
void GetMenuSet(void *pbuf,U16 ItemID,U16* pSize)
{
    U8   ValueType;
    //printf("ConfigId = %x\n",ItemID);
    U32 t_data;
    U32 FieldValue[4];
	FieldValue[0] = 0;
	FieldValue[1] = 0;
	FieldValue[2] = 0;
	FieldValue[3] = 0;
    if ( FieldValue[0] > 3)
    {
        FieldValue[0] = 1;
    }
    if ( FieldValue[1] > 3)
    {
        FieldValue[1] = 0;
    }
    if ( FieldValue[2] > 3)
    {
        FieldValue[2] = 0;
    }
    if ( FieldValue[3] > 3)
    {
        FieldValue[3] = 0;
    }

    switch(ItemID)
    {
        case CONFIG_MENU_SET_LANGUAGE:
            ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[0];
    		ItemID=htons(ItemID);
            memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_MENU_SET_MENU_DISPLAY:
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[1];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_MENU_SET_INFO_BAR_TIMEOUT:
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[2];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_MENU_SET_TRANSPARENCY:
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[3];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        default:
            //printf("default\n");
            break;
    }
}


void GetVideoOutSDConfig(void *pbuf,U16 ItemID,U16* pSize)
{

    U8   ValueType;
    //printf("ConfigId = %x\n",ItemID);
    U32 t_data;
    static U8 FieldValue[7];

    switch(ItemID)
    {
        case CONFIG_VIDEOOUT_SD_DISPLAY_RATIO:
            //printf("CONFIG_VIDEOOUT_SD_DISPLAY_RATIO\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[0];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_VIDEOOUT_SD_169_PROCESSING:
    	 	//printf("CONFIG_VIDEOOUT_SD_169_PROCESSING\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[1];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_VIDEOOUT_SD_SCART_FORMAT:
    	 	//printf("CONFIG_VIDEOOUT_SD_SCART_FORMAT\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[2];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_VIDEOOUT_SD_DISPLAY_MODE:
    	 	//printf("CONFIG_VIDEOOUT_SD_DISPLAY_MODE\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[3];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_VIDEOOUT_SD_VIDEO_SIGNAL:
            //printf("CONFIG_VIDEOOUT_SD_VIDEO_SIGNAL\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[4];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_VIDEOOUT_SD_AUDIO_LANGUAGE:
    	 	//printf("CONFIG_VIDEOOUT_SD_AUDIO_LANGUAGE\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[5];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_VIDEOOUT_SD_DIGITAL_AUDIO_OUT:
            //printf("CONFIG_VIDEOOUT_SD_DIGITAL_AUDIO_OUT\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[6];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        default:
            //printf("default\n");
            break;
    }
}


void GetDuplexAudioConfig(void *pbuf,U16 ItemID,U16* pSize)
{
    U8   ValueType;
    //printf("ConfigId = %x\n",ItemID);
    U32 t_data;
    U8 i=0;
    static U8 FieldValue[8];

	for (i = 0; i <8; i++)
	{
		FieldValue[i] = 0xFF;
	}

    DuplexAudio DuplexingAudio_Read;
	eRet ErrCode = eRet_InvalidParameter;
    memset(&DuplexingAudio_Read,0x00,sizeof(DuplexAudio));
	//printf("DuplexAudioGet\n");
	ErrCode = DuplexAudioGet(&DuplexingAudio_Read);
	if(ErrCode == eRet_Ok)
	{
        if ((0<DuplexingAudio_Read.ADCdelay) && (DuplexingAudio_Read.ADCdelay<=60))
		{
			FieldValue[0]=DuplexingAudio_Read.ADCdelay;
		}
		switch(DuplexingAudio_Read.CheckLowNUM)
		{
			case 1023 : FieldValue[1]=0;break;
			case 500  : FieldValue[1]=1;break;
			case 200  : FieldValue[1]=2;break;
			case 50   : FieldValue[1]=3;break;
			case 30   : FieldValue[1]=4;break;
			case 20   : FieldValue[1]=5;break;
			case 15   : FieldValue[1]=6;break;
			case 10   : FieldValue[1]=7;break;
			case 5    : FieldValue[1]=8;break;
			case 4    : FieldValue[1]=9;break;
			default   : FieldValue[1]=0;break;
		}

		if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x01)
		{
			FieldValue[2]=1;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x02)
		{
			FieldValue[2]=2;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x04)
		{
			FieldValue[2]=3;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x08)
		{
			FieldValue[2]=4;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x10)
		{
			FieldValue[2]=5;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x20)
		{
			FieldValue[2]=6;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x40)
		{
			FieldValue[2]=7;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x80)
		{
			FieldValue[2]=8;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x01 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=9;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x02 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=10;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x04 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=11;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x08 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=12;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x10 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=13;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x20 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=14;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x40 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=15;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x80 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=16;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x01 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=17;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x02 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=18;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x04 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=19;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x08 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=20;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x10 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=21;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x20 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=22;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x40 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=23;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x80 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=24;
		}
		else if(DuplexingAudio_Read.Ad1 == 0xFF && DuplexingAudio_Read.Ad2 == 0xFF && DuplexingAudio_Read.Ad3 == 0xFF)
		{
			FieldValue[2]=25;
		}
		else
		{
			FieldValue[2]=0;
		}

		if (DuplexingAudio_Read.AudioInSet<=1)
		{
			FieldValue[3]=DuplexingAudio_Read.AudioInSet;
		}
		if (DuplexingAudio_Read.AudioOutSet<=1)
		{
			FieldValue[4]=DuplexingAudio_Read.AudioOutSet;
		}
		if (DuplexingAudio_Read.AudioPGA<=7)
		{
			FieldValue[5]=DuplexingAudio_Read.AudioPGA;
		}

		if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x01)
		{
			FieldValue[6]=1;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x02)
		{
			FieldValue[6]=2;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x04)
		{
			FieldValue[6]=3;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x08)
		{
			FieldValue[6]=4;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x10)
		{
			FieldValue[6]=5;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x20)
		{
			FieldValue[6]=6;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x40)
		{
			FieldValue[6]=7;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x80)
		{
			FieldValue[6]=8;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x01 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=9;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x02 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=10;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x04 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=11;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x08 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=12;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x10 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=13;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x20 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=14;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x40 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=15;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x80 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=16;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x01 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=17;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x02 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=18;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x04 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=19;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x08 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=20;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x10 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=21;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x20 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=22;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x40 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=23;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x80 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=24;
		}
		else
		{
			FieldValue[6]=0;
		}

		if ((0<DuplexingAudio_Read.VOT_Channel) && (DuplexingAudio_Read.VOT_Channel<=64))
		{
			FieldValue[7]=DuplexingAudio_Read.VOT_Channel;
		}

	}

    switch(ItemID)
    {
        case CONFIG_DUPLEX_AUDIO_DELAY:
    	 	//printf("CONFIG_DUPLEX_AUDIO_DELAY\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[0];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_DUPLEX_AUDIO_SENSITIVITY:
            //printf("CONFIG_DUPLEX_AUDIO_SENSITIVITY\n");
            ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[1];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_DUPLEX_AUDIO_T_ADDRESS:
    	 	//printf("CONFIG_DUPLEX_AUDIO_T_ADDRESS\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[2];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_DUPLEX_AUDIO_IN_SET:
    	 	//printf("CONFIG_DUPLEX_AUDIO_IN_SET\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[3];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_DUPLEX_AUDIO_OUT_SET:
    	 	//printf("CONFIG_DUPLEX_AUDIO_OUT_SET\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[4];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_DUPLEX_AUDIO_IN_GAIN:
    	 	//printf("CONFIG_DUPLEX_AUDIO_IN_GAIN\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[5];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
            break;
        case CONFIG_DUPLEX_AUDIO_R_ADDRESS:
    	 	//printf("CONFIG_DUPLEX_AUDIO_R_ADDRESS\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[6];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
    	  	break;
        case CONFIG_DUPLEX_AUDIO_CHANNEL:
    	 	//printf("CONFIG_DUPLEX_AUDIO_CHANNEL\n");
    		ValueType = SNIF_VALUE_TYPE_UINT;
    		t_data = (U32)FieldValue[7];
    		ItemID=htons(ItemID);
    	    memcpy(pbuf,(U8 *)&ItemID,2);
    		memcpy(pbuf+2,(U8 *)&ValueType,1);
    		t_data=htonl(t_data);
            memcpy(pbuf+3,(U8*)&t_data,4);
    		*pSize=7;
            break;
        default:
            //printf("default\n");
            break;
    }
}




void SetUnitConfig(U8 *pbuf,U16 TempCount)
{
    U8 i=0;
    U16  ConfigID[20];
    U16  StartAddr=0;
    U8    ValueBuff[20][1024];
    U8     ValueType;
    U16  BytesOfString=0;
    U16 tempid;
    U8 *  BaseAddr;

    BaseAddr = (U8 *)0x00700000;//address of store the pata
    memset(ValueBuff,0,20*1024);
    for(i=0;i<TempCount;i++)
    {
        memcpy((U8 *)&tempid,pbuf+StartAddr,2);
        ConfigID[i] = ntohs(tempid);
        ValueType =  *((U8 *)(pbuf+StartAddr+2));
        switch(ValueType)
        {
            case SNIF_VALUE_TYPE_CHAR:
            {
                memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),1);
			    StartAddr = StartAddr+4;
			    break;
            }
            case SNIF_VALUE_TYPE_SHORT:
            {
                memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),2);
			    StartAddr = StartAddr+5;
                break;
            }
            case SNIF_VALUE_TYPE_INT:
            {
                memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
                break;
            }
            case SNIF_VALUE_TYPE_STRING:
            {
                BytesOfString = ntohs(*((U16 *)(pbuf+StartAddr+3)));
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+5),BytesOfString);
				StartAddr = StartAddr+5+BytesOfString;
                break;
            }
            case SNIF_VALUE_TYPE_UINT:
            {
                memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
                break;
            }
            case SNIF_VALUE_TYPE_GUID:
            {
                memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),16);
			    StartAddr = StartAddr+19;
				break;
            }
            case SNIF_VALUE_TYPE_FLOAT:
            {
                memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
                break;
            }
        }
    }

    for(i=0;i<TempCount;i++)
    {
        switch(ConfigID[i])
        {
            case CONFIG_SYSTEM_DEVICE_TYPE:
                break;
            case CONFIG_SYSTEM_FIRMWARE_VERSION:
                break;
            case CONFIG_SYSTEM_HARDWARE_VERSION:
                break;
            case CONFIG_SYSTEM_FPGA_VERSION:
                break;
            case CONFIG_SYSTEM_DATE_TIME:
            {
                char mtime[16];
                unsigned int rtc_tmp[6];
                U8 temp[6][10];
                int j=0;
                memset(temp,0,60);
                memcpy(&temp[0][0],&ValueBuff[i][0],4);
                memcpy(&temp[1][0],&ValueBuff[i][4],2);
                memcpy(&temp[2][0],&ValueBuff[i][6],2);
                memcpy(&temp[3][0],&ValueBuff[i][8],2);
                memcpy(&temp[4][0],&ValueBuff[i][10],2);
                memcpy(&temp[5][0],&ValueBuff[i][12],2);

                for(j=0;j<6;j++)
                {
                    rtc_tmp[j]=atoi((char *)&temp[j][0]);
                }
				SetRtcAndSystemTime(rtc_tmp);
                break;
            }
            case CONFIG_SYSTEM_FLASH_READ:
                break;
            case CONFIG_SYSTEM_MENUCON:
            {
				break;
            }
            default:
                break;
        }
    }
}
void SetEthernetConfig(U8 *pbuf,U16 TempCount)
{
    U8 i=0;
    U16  ConfigID[20];
    U16  StartAddr=0;
    U8    ValueBuff[20][1024];
    U8     ValueType;
    U16  BytesOfString=0;
    U16 tempid;
    bool saveFlag = false;

    memset(ValueBuff,0,20*1024);
    for(i=0;i<TempCount;i++)
    {
        memcpy((U8 *)&tempid,pbuf+StartAddr,2);
	   	ConfigID[i] = ntohs(tempid);
        ValueType =  *((U8 *)(pbuf+StartAddr+2));
        switch(ValueType)
        {
            case SNIF_VALUE_TYPE_CHAR:
            {
                memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),1);
			    StartAddr = StartAddr+4;
			    break;
            }
            case SNIF_VALUE_TYPE_SHORT:
            {
                memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),2);
			    StartAddr = StartAddr+5;
                break;
            }
            case SNIF_VALUE_TYPE_INT:
            {
                memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
                break;
            }
            case SNIF_VALUE_TYPE_STRING:
            {
                BytesOfString = ntohs(*((U16 *)(pbuf+StartAddr+3)));
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+5),BytesOfString);
                StartAddr = StartAddr+5+BytesOfString;
                break;
            }
            case SNIF_VALUE_TYPE_UINT:
            {
                memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
                break;
            }
            case SNIF_VALUE_TYPE_GUID:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),16);
			    StartAddr = StartAddr+19;
				break;
			}
            case SNIF_VALUE_TYPE_FLOAT:
            {
                memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
                break;
			}
        }
    }


    U32 FieldValue[8];
    FieldValue[0] = g_Config.NetWorkSettings.DHCPEnable;
    FieldValue[1] = g_Config.NetWorkSettings.IPAddr;
    FieldValue[2] = g_Config.NetWorkSettings.NetMask;
    FieldValue[3] = g_Config.NetWorkSettings.GateWay;
    FieldValue[4] = 0;
    FieldValue[5] = 0;
    FieldValue[6] = 0;
    FieldValue[7] = 0;
    for(i=0;i<TempCount;i++)
    {
        switch(ConfigID[i])
        {
            case CONFIG_NET_LOCAL_IP_ADDRESS:
            {
                memcpy((U8 *)&FieldValue[1],&ValueBuff[i][0],4);
                FieldValue[1] = ntohl(FieldValue[1]);
                break;
            }
            case CONFIG_NET_LOCAL_NETMASK:
            {
                memcpy((U8 *)&FieldValue[2],&ValueBuff[i][0],4);
                FieldValue[2] = ntohl(FieldValue[2]);
                break;
            }
            case CONFIG_NET_LOCAL_GATEWAY:
            {
                memcpy((U8 *)&FieldValue[3],&ValueBuff[i][0],4);
                FieldValue[3] = ntohl(FieldValue[3]);
                break;
            }
            case CONFIG_NET_DHCP_STATE:
            {
                memcpy((U8 *)&FieldValue[0],&ValueBuff[i][0],4);
                FieldValue[0] = ntohl(FieldValue[0]);
                break;
            }
            default:
                break;
        }
   }

    //if(FieldValue[0] == 1)
    {
        char ip[16];
        char netmask[16];
        char gateway[16];
        U32 IP_Add, Netmask, Gateway;
        IP_Add = FieldValue[1];
        Netmask = FieldValue[2];
        Gateway = FieldValue[3];

        U322IP(ip, IP_Add);
        U322IP(netmask, Netmask);
        U322IP(gateway, Gateway);

        if((IP_Add == 0xFFFFFFFF) ||(IP_Add&0xFFFFFF) ==0xFFFFFF ||(IP_Add&0xFFFF) ==0xFFFF ||((IP_Add&0xFF) ==0xFF))
	    {
            printf("invalid value   TCPIP  \n\n");
        }
        else
        {
            if((g_Config.NetWorkSettings.IPAddr!=IP_Add) || (g_Config.NetWorkSettings.NetMask!=Netmask) || (g_Config.NetWorkSettings.GateWay!=Gateway))
		    {
                g_Config.NetWorkSettings.IPAddr = IP_Add;
      			g_Config.NetWorkSettings.NetMask= Netmask;
      			g_Config.NetWorkSettings.GateWay= Gateway;
				TCPIP_SetConfig(0,ip,netmask,gateway); //set static IP address
      			saveFlag = true;
		    }
        }
    }
    if(saveFlag)
    {
        SaveConfigToFlash();
    }
}


void SetNetAVRelayConfig(U8 *pbuf,U16 TempCount)
{
    printf("TempCount=%d\n",TempCount);
    U8 i=0;
    U16  ConfigID[20];
    U16  StartAddr=0;
    U8    ValueBuff[20][1024];
    U8     ValueType;
    U16  BytesOfString=0;
    U16 tempid;
    memset(ValueBuff,0,20*1024);
    U32 FieldValue[5];
    for(i=0;i<TempCount;i++)
    {
        memcpy((U8 *)&tempid,pbuf+StartAddr,2);
        ConfigID[i] = ntohs(tempid);
        ValueType =  *((U8 *)(pbuf+StartAddr+2));
        switch(ValueType)
        {
            case SNIF_VALUE_TYPE_CHAR:
            {
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),1);
			    StartAddr = StartAddr+4;
			    break;
			}
			case SNIF_VALUE_TYPE_SHORT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),2);
			    StartAddr = StartAddr+5;
				break;
			}
			case SNIF_VALUE_TYPE_INT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_STRING:
			{
			    BytesOfString = ntohs(*((U16 *)(pbuf+StartAddr+3)));
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+5),BytesOfString);
				StartAddr = StartAddr+5+BytesOfString;
				break;
			}
			case SNIF_VALUE_TYPE_UINT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_GUID:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),16);
			    StartAddr = StartAddr+19;
				break;
			}
			case SNIF_VALUE_TYPE_FLOAT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
        }
    }

    FieldValue[0] = g_Config.NetRelaySettings.Type;
    FieldValue[1] = g_Config.NetRelaySettings.RemoteIP;
    FieldValue[2] = g_Config.NetRelaySettings.RemotePort;
    FieldValue[3] = g_Config.NetRelaySettings.protocal;
    FieldValue[4] = g_Config.NetRelaySettings.Enable;

    U32 DevType = FieldValue[0];
    U32 DevStat = FieldValue[4];
    U32 protocol = FieldValue[3];

    for(i=0;i<TempCount;i++)
    {
        switch(ConfigID[i])
        {
            case CONFIG_NETAVRELAY_TYPE:
            {
                memcpy((U8 *)&FieldValue[0],&ValueBuff[i][0],4);
                FieldValue[0] = ntohl(FieldValue[0]);
                break;
            }
            case CONFIG_NETAVRELAY_REMOTEIP:
            {
                memcpy((U8 *)&FieldValue[1],&ValueBuff[i][0],4);
                FieldValue[1] = ntohl(FieldValue[1]);
                break;
            }
            case CONFIG_NETAVRELAY_PORT:
            {
                memcpy((U8 *)&FieldValue[2],&ValueBuff[i][0],4);
                FieldValue[2] = ntohl(FieldValue[2]);
                break;
            }
            case CONFIG_NETAVRELAY_PROTOCAL:
            {
                memcpy((U8 *)&FieldValue[3],&ValueBuff[i][0],4);
                FieldValue[3] = ntohl(FieldValue[3]);
                break;
            }
            case CONFIG_NETAVRELAY_STATUS:
            {
                memcpy((U8 *)&FieldValue[4],&ValueBuff[i][0],4);
                FieldValue[4] = ntohl(FieldValue[4]);
                break;
            }
            default:
                break;
        }
    }
	SaveConfigToFlash();

}

void SetSerialPortConfig(U8 *pbuf,U16 TempCount)
{
    U8 i=0;
    U16  ConfigID[20];
    U16  StartAddr=0;
    U8    ValueBuff[20][1024];
    U8     ValueType;
    U16  BytesOfString=0;
    U16 tempid;
    memset(ValueBuff,0,20*1024);
    for(i=0;i<TempCount;i++)
    {
        memcpy((U8 *)&tempid,pbuf+StartAddr,2);
        ConfigID[i] = ntohs(tempid);
        ValueType =  *((U8 *)(pbuf+StartAddr+2));
        switch(ValueType)
        {
            case SNIF_VALUE_TYPE_CHAR:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),1);
			    StartAddr = StartAddr+4;
			    break;
			}
			case SNIF_VALUE_TYPE_SHORT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),2);
			    StartAddr = StartAddr+5;
				break;
			}
			case SNIF_VALUE_TYPE_INT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_STRING:
			{
			    BytesOfString = ntohs(*((U16 *)(pbuf+StartAddr+3)));
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+5),BytesOfString);
				StartAddr = StartAddr+5+BytesOfString;
				break;
			}
			case SNIF_VALUE_TYPE_UINT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_GUID:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),16);
			    StartAddr = StartAddr+19;
				break;
			}
			case SNIF_VALUE_TYPE_FLOAT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
		}
    }

    for(i=0;i<TempCount;i++)
    {
        switch(ConfigID[i])
        {
            case CONFIG_SERIALPORT_BAUDRATE:
            {
                break;
            }
            case CONFIG_SERIALPORT_WORDLENGTH:
            {
                break;
            }
            case CONFIG_SERIALPORT_STOPBITS:
            {
                break;
            }
            case CONFIG_SERIALPORT_PARITY:
            {
                break;
            }
            default:
                break;
        }
    }
}


void SetStorgeConfig(U8 *pbuf,U16 TempCount)
{
    U8   i=0;
    U16  ConfigID[20];
    U16  StartAddr=0;
    U8   ValueBuff[20][1024];
    U8   ValueType;
    U16  BytesOfString=0;
    U16  tempid;
    U32  FieldValue[5];
 	U8   ChangeFlag=0;
	FieldValue[0] = g_Config.RecSettings.PreferredMedia;
	FieldValue[1] = 0 ;
	FieldValue[2] = g_Config.RecSettings.LoopRecEnable;// tux add
	FieldValue[3] = g_Config.RecSettings.RecFileSize;
	FieldValue[4] = g_Config.RecSettings.RecOnStartEnable;
    memset(ValueBuff,0,20*1024);
    for(i=0;i<TempCount;i++)
    {
        memcpy((U8 *)&tempid,pbuf+StartAddr,2);
	   	ConfigID[i] = ntohs(tempid);
		ValueType =  *((U8 *)(pbuf+StartAddr+2));
		switch(ValueType)
		{
		 	case SNIF_VALUE_TYPE_CHAR:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),1);
			    StartAddr = StartAddr+4;
			    break;
			}
			case SNIF_VALUE_TYPE_SHORT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),2);
			    StartAddr = StartAddr+5;
				break;
			}
			case SNIF_VALUE_TYPE_INT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_STRING:
			{
			    BytesOfString = ntohs(*((U16 *)(pbuf+StartAddr+3)));
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+5),BytesOfString);
				StartAddr = StartAddr+5+BytesOfString;
				break;
			}
			case SNIF_VALUE_TYPE_UINT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_GUID:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),16);
			    StartAddr = StartAddr+19;
				break;
			}
			case SNIF_VALUE_TYPE_FLOAT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
		}
    }

    for(i=0;i<TempCount;i++)
    {
        switch(ConfigID[i])
        {
            case CONFIG_STORGE_TYPE:
            {
                ChangeFlag = 1;
                memcpy((U8 *)&FieldValue[0],&ValueBuff[i][0],4);
                FieldValue[0] = ntohl(FieldValue[0]);
                break;
            }
            case CONFIG_STORGE_PARTITION:
            {
                ChangeFlag = 1;
                memcpy((U8 *)&FieldValue[1],&ValueBuff[i][0],4);
                FieldValue[1] = ntohl(FieldValue[1]);
                break;
            }
            case CONFIG_STORGE_ISLOOP:
            {
                ChangeFlag = 1;
                memcpy((U8 *)&FieldValue[2],&ValueBuff[i][0],4);
                FieldValue[2] = ntohl(FieldValue[2]);
                break;
            }
            case CONFIG_STORGE_SIZE:
            {
                ChangeFlag = 1;
                memcpy((U8 *)&FieldValue[3],&ValueBuff[i][0],4);
                FieldValue[3] = ntohl(FieldValue[3]);
                break;
            }
            case CONFIG_STARTUP_REC:
            {
                ChangeFlag = 1;
                memcpy((U8 *)&FieldValue[4],&ValueBuff[i][0],4);
                FieldValue[4] = ntohl(FieldValue[4]);
                break;
            }
            default:
                break;
        }
    }
    if(ChangeFlag == 1)
    {
        ChangeFlag = 0;
		g_Config.RecSettings.PreferredMedia  =FieldValue[0];
		g_Config.RecSettings.LoopRecEnable   =FieldValue[2];// tux add
	 	g_Config.RecSettings.RecFileSize     =FieldValue[3];
	 	g_Config.RecSettings.RecOnStartEnable=FieldValue[4];
        SaveConfigToFlash();
    }
}


void SetReceiverConfig(U8 *pbuf,U16 TempCount)
{
    U16 i=0;
    U16  ConfigID[20];
    U16  StartAddr=0;
    U8    ValueBuff[20][1024];
    U8     ValueType;
    U16  BytesOfString=0;
    U16 tempid;
    memset(ValueBuff,0,20*1024);
    for(i=0;i<TempCount;i++)
    {
        memcpy((U8 *)&tempid,pbuf+StartAddr,2);
	   	ConfigID[i] = ntohs(tempid);
		ValueType =  *((U8 *)(pbuf+StartAddr+2));
		switch(ValueType)
		{
            case SNIF_VALUE_TYPE_CHAR:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),1);
			    StartAddr = StartAddr+4;
			    break;
			}
			case SNIF_VALUE_TYPE_SHORT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),2);
			    StartAddr = StartAddr+5;
				break;
			}
			case SNIF_VALUE_TYPE_INT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_STRING:
			{
			    BytesOfString = ntohs(*((U16 *)(pbuf+StartAddr+3)));
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+5),BytesOfString);
				StartAddr = StartAddr+5+BytesOfString;
				break;
			}
			case SNIF_VALUE_TYPE_UINT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_GUID:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),16);
			    StartAddr = StartAddr+19;
				break;
			}
			case SNIF_VALUE_TYPE_FLOAT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
		}
    }

    U8 SetKeyFlag=0;
    U8 ChangeChannelFlag=0;
    U8 ManualScanFlag=0;
    U8 DeleteChanFlag=0;
    U32 DecodedChannel=0;
    U32 T_FRE=0;
    U32 T_BAND=0;
    U32 T_D_CHANNEL;

    U16 FavoriteListNumber;
    U16 ChannelIndex;
    U16 ChannelIndexInList;
    U16 TSIndex;
    char DescrableKey[33];
    memset(DescrableKey,0,33);

    for(i=0;i<TempCount;i++)
    {
        switch(ConfigID[i])
        {
            case CONFIG_RECEIVER_DESCRAMBLE_TYPE:
            {
                SetKeyFlag = 1;
                memcpy(DescrableKey,(U8 *)&ValueBuff[i][0],33);
                //memcpy(STGUI_Layer_Transponder_List[TSIndex].DescrableKey,DescrableKey,MAX_DESCRABLE_NB);
                break;
            }
            case CONFIG_RECEIVER_DESCRAMBLE_PASSWORD:
            {
                break;
            }
            case CONFIG_RECEIVER_DOCODED_CHANNEL:
            {
                ChangeChannelFlag = 1;
                memcpy((U8 *)&DecodedChannel,&ValueBuff[i][0],4);
                DecodedChannel = ntohl(DecodedChannel);
                //printf("DecodedChannel[4] = %x \n",DecodedChannel) ;
                break;
            }
            case CONFIG_RECEIVER_DOCODED_FRE:
            {
                ManualScanFlag = 1;
                memcpy((U8 *)&T_FRE,&ValueBuff[i][0],4);
                T_FRE = ntohl(T_FRE);
                //printf("T_FRE[4] = %x \n",T_FRE) ;
                break;
            }
            case CONFIG_RECEIVER_DOCODED_BAND:
            {
                ManualScanFlag =1;
                memcpy((U8 *)&T_BAND,&ValueBuff[i][0],4);
                T_BAND = ntohl(T_BAND);
                //printf("T_BAND = %x \n",T_BAND) ;
                break;
            }
            case CONFIG_RECEIVER_DOCODED_INFO:
            {
                break;
            }
            case CONFIG_RECEIVER_DELETED_CHAN:
            {
                DeleteChanFlag =1;
                memcpy((U8 *)&T_D_CHANNEL,&ValueBuff[i][0],4);
                T_D_CHANNEL = ntohl(T_D_CHANNEL);
                //printf("T_D_CHANNEL = %x \n",T_D_CHANNEL) ;
                break;
            }
            default:
                break;
        }
    }
#if 0
    U16 len=0;
    U32 FieldValue[5]={0};
    U32 FieldValue1[5]={0};

    if(SetKeyFlag == 1)
    {
        switch(DescrableKey[0])
        {
            case DESCRAMBLE_TYPE_NONE:
            {
                len = DESCRAMBLE_NONE_LEN;
            }
            break;
            case DESCRAMBLE_TYPE_ABS_64:
            {
                len = DESCRAMBLE_ABS_64_LEN;
            }
            break;
            case DESCRAMBLE_TYPE_ABS_128:
            {
                len = DESCRAMBLE_ABS_128_LEN;
            }
            break;
            case DESCRAMBLE_TYPE_AES_128:
            {
                len = DESCRAMBLE_AES_128_LEN;
            }
            break;
            case DESCRAMBLE_TYPE_AES_256:
            {
                len = DESCRAMBLE_AES_256_LEN;
            }
            break;
        }
        TB_MLSaveListsInFlash(STGUI_SOFTWARE_VERSION);
        Set_DESCRAMBLE_Key(len,DescrableKey);                    //配置解密密码mdk

		TB_MLSaveListsInFlash(STGUI_SOFTWARE_VERSION);
    }

    U16 TunerID;
    if(ManualScanFlag == 1)
    {

        bThreadContinue = FALSE;
        //搜索频道时，若是有录像，要停止
        STOS_TaskDelay(ST_GetClocksPerSecond()*3);
        printf("\n\n-network scan--\n\n");
        if(bRecord == TRUE)
        {
            bRecord = FALSE;
            APPDVR_RecordControl(DVR_ACTION_kNormRecStop, NULL);
            sync();                      //0116 add 将缓冲区数据写入
        }
        if(FirstNetStart_T== TRUE)
        {
            printf("close network ts stream----FirstNetStart_T== TRUE-----\n");
            FirstNetStart_T= FALSE;
            TB_MLRECStop(0);
            TB_MLIPStream(0,2); //close IP_STREAM
        }
        //stop private thread
        if(FirstPrivate_Data == TRUE)
        {
            FirstPrivate_Data = FALSE;
        }

        bManualScan = TRUE;
        FieldValue[0] = T_FRE/1000;
        FieldValue[4] = 0;

        TB_UpdateFieldValue(FieldValue,FieldValue1);
        FieldValue1[0] = T_FRE/1000;
        FieldValue1[4] = T_BAND;
        TunerID = ((GLOBALVARS[TUNERID] >= (U32)TB_TUNER_ID_ALL) ? (U16)TB_TUNER_ID_0 : (U16)GLOBALVARS[TUNERID]);
        TB_SetTunerIDToSearch((TB_TUNER_ID_e)TunerID);
        TB_StartManualScan(TunerID, FieldValue1);
        bThreadContinue = TRUE;
    }

    if(DeleteChanFlag == 1)
    {
        S16 t_FAVORITELIST0[MAX_ELEMENTS_IN_FAVORITE];
        Channel_t t_STGUI_Layer_Channel_List[MAX_CHANNEL_NB];
        TransportStream_t  t_STGUI_Layer_Transponder_List[MAX_TRANSPONDER_NB];
        memcpy((U8 *)t_FAVORITELIST0,(U8 *)&FAVORITELIST[0][0],sizeof(t_FAVORITELIST0));
        memcpy((U8 *)&t_STGUI_Layer_Channel_List,(U8 *)&STGUI_Layer_Channel_List,sizeof(t_STGUI_Layer_Channel_List));
        memcpy((U8 *)&t_STGUI_Layer_Transponder_List,(U8 *)&STGUI_Layer_Transponder_List,sizeof(t_STGUI_Layer_Transponder_List));


        memset((U8 *)&FAVORITELIST[0][0],0,sizeof(t_FAVORITELIST0));
        memset((U8 *)&STGUI_Layer_Channel_List,0,sizeof(t_STGUI_Layer_Channel_List));
        memset((U8 *)&STGUI_Layer_Transponder_List,0,sizeof(t_STGUI_Layer_Transponder_List));
        for(i=0;i<T_D_CHANNEL;i++)
        {
            memcpy((U8 *)&FAVORITELIST[0][i+1],(U8 *)&t_FAVORITELIST0[i+1],2);
            memcpy((U8 *)&STGUI_Layer_Channel_List[i],(U8 *)&t_STGUI_Layer_Channel_List[i],sizeof(Channel_t));
            memcpy((U8 *)&STGUI_Layer_Transponder_List[i],(U8 *)&t_STGUI_Layer_Transponder_List[i],sizeof(TransportStream_t));
        }

        for(i=T_D_CHANNEL;i<(t_FAVORITELIST0[0]-1);i++)
        {
            FAVORITELIST[0][i+1] = i;
            memcpy((U8 *)&STGUI_Layer_Channel_List[i],(U8 *)&t_STGUI_Layer_Channel_List[i+1],sizeof(Channel_t));
            memcpy((U8 *)&STGUI_Layer_Transponder_List[i],(U8 *)&t_STGUI_Layer_Transponder_List[i+1],sizeof(TransportStream_t));
            STGUI_Layer_Channel_List[i].TSIndex = i;
            STGUI_Layer_Channel_List[i].ChlKey =  i;
        }
        FAVORITELIST[0][0] = t_FAVORITELIST0[0] - 1;

        GLOBALVARS[FAVLISTSORT]=2;
        //GLOBALVARS[DECODEDCHANNEL] = FAVORITELIST[0][0];//0x1
        GLOBALVARS[RADIODECODEDCHANNEL] = 0x10001;
        if(GLOBALVARS[DECODEDCHANNEL]>=T_D_CHANNEL)
        {
            GLOBALVARS[DECODEDCHANNEL] = FAVORITELIST[0][0];//0x1
        }
        TB_MLSaveListsInFlash(STGUI_SOFTWARE_VERSION);
    }

    if(ChangeChannelFlag == 1)
    {
        GLOBALVARS[DECODEDCHANNEL] = DecodedChannel+1;//0x1
        TB_MLSaveListsInFlash(STGUI_SOFTWARE_VERSION);
    }
	#endif
}

void SetVideoOutHDMIConfig(U8 *pbuf,U16 TempCount)
{
    U8 i=0;
    U16  ConfigID[20];
    U16  StartAddr=0;
    U8    ValueBuff[20][1024];
    U8     ValueType;
    U16  BytesOfString=0;
    U16 tempid;
    memset(ValueBuff,0,20*1024);
    for(i=0;i<TempCount;i++)
    {
        memcpy((U8 *)&tempid,pbuf+StartAddr,2);
	   	ConfigID[i] = ntohs(tempid);
		ValueType =  *((U8 *)(pbuf+StartAddr+2));
		switch(ValueType)
        {
		 	case SNIF_VALUE_TYPE_CHAR:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),1);
			    StartAddr = StartAddr+4;
			    break;
			}
			case SNIF_VALUE_TYPE_SHORT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),2);
			    StartAddr = StartAddr+5;
				break;
			}
			case SNIF_VALUE_TYPE_INT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_STRING:
			{
			    BytesOfString = ntohs(*((U16 *)(pbuf+StartAddr+3)));
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+5),BytesOfString);
				StartAddr = StartAddr+5+BytesOfString;
				break;
			}
			case SNIF_VALUE_TYPE_UINT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_GUID:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),16);
			    StartAddr = StartAddr+19;
				break;
			}
			case SNIF_VALUE_TYPE_FLOAT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
		}
    }

    static U8 FieldValue[9];
    FieldValue[0] = 0;
    FieldValue[3] = g_Config.HdmiSettings.Format;
    U8 ChangeFlag=0;
    U32 MidVal=0;

    for(i=0;i<TempCount;i++)
    {
        switch(ConfigID[i])
        {
            case CONFIG_VIDEOOUT_HDMI_MODE:
            {
                ChangeFlag =1;
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[0] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_HDMI_HDCP:
            {
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[1] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_HDMI_HPD:
            {
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[2] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_HDMI_VIDEO_FORMATS:
            {
                ChangeFlag =1;
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[3] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_HDMI_PIXEL_ENCODING:
            {
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[4] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_HDMI_PICTURE_AR:
            {
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[5] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_HDMI_ACTIVE_AR:
            {
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[6] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_HDMI_HDCP_TRACES:
            {
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[7] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_HDMI_MODE_DEBUG:
            {
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[8] = (U8)MidVal;
                break;
            }
            default:
                break;
        }
    }
    if(ChangeFlag == 1)
    {
        ChangeFlag =0;
        //GLOBALVARS[HDMIINFO] =  FieldValue[0] * 256 + FieldValue[3];//new HDMIINFO
        //HDMI_EnableOutput(0);
        //TB_SetVideoOutFormat (0, FieldValue[3]);
        //TB_MLSaveListsInFlash(STGUI_SOFTWARE_VERSION);
    }
}


void SetMenuSet(U8 *pbuf,U16 TempCount)
{
    U8 i=0;
    U16  ConfigID[20];
    U16  StartAddr=0;
    U8    ValueBuff[20][1024];
    U8     ValueType;
    U16  BytesOfString=0;
    U16 tempid;
    memset(ValueBuff,0,20*1024);
    for(i=0;i<TempCount;i++)
    {
        memcpy((U8 *)&tempid,pbuf+StartAddr,2);
	   	ConfigID[i] = ntohs(tempid);
		ValueType =  *((U8 *)(pbuf+StartAddr+2));
		switch(ValueType)
		{
		 	case SNIF_VALUE_TYPE_CHAR:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),1);
			    StartAddr = StartAddr+4;
			    break;
			}
			case SNIF_VALUE_TYPE_SHORT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),2);
			    StartAddr = StartAddr+5;
				break;
			}
			case SNIF_VALUE_TYPE_INT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_STRING:
			{
			    BytesOfString = ntohs(*((U16 *)(pbuf+StartAddr+3)));
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+5),BytesOfString);
				StartAddr = StartAddr+5+BytesOfString;
				break;
			}
			case SNIF_VALUE_TYPE_UINT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_GUID:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),16);
			    StartAddr = StartAddr+19;
				break;
			}
			case SNIF_VALUE_TYPE_FLOAT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
		}
    }

    U32 FieldValue[4];
	FieldValue[0] = 0;
	FieldValue[1] = 0;
	FieldValue[2] = 0;
	FieldValue[3] = 0;
    U8 ChangeFlag=0;
    U32 MidVal;
    for(i=0;i<TempCount;i++)
    {
        switch(ConfigID[i])
        {
            case CONFIG_MENU_SET_LANGUAGE:
                {
                    ChangeFlag =1;
                    memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                    MidVal = ntohl(MidVal);
                    FieldValue[0] = MidVal;
                    break;
                }
            case CONFIG_MENU_SET_MENU_DISPLAY:
                {
                    ChangeFlag =1;
                	memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                	MidVal = ntohl(MidVal);
                	FieldValue[1] = MidVal;
                	break;
                }
            case CONFIG_MENU_SET_INFO_BAR_TIMEOUT:
                {
                    ChangeFlag =1;
                    memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                    MidVal = ntohl(MidVal);
                    FieldValue[2] = MidVal;
                    break;
                }
            case CONFIG_MENU_SET_TRANSPARENCY:
                {
                    ChangeFlag =1;
                    memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                    MidVal = ntohl(MidVal);
                    FieldValue[3] = MidVal;
                    break;
                }
            default:
                break;
        }
    }

    if(ChangeFlag == 1)
    {
        ChangeFlag=0;
        //TB_MLSaveListsInFlash(STGUI_SOFTWARE_VERSION);
    }
}


void SetVideoOutSDConfig(U8 *pbuf,U16 TempCount)
{
    U8 i=0;
    U16  ConfigID[20];
    U16  StartAddr=0;
    U8    ValueBuff[20][1024];
    U8     ValueType;
    U16  BytesOfString=0;
    U16 tempid;
    memset(ValueBuff,0,20*1024);
    for(i=0;i<TempCount;i++)
    {
        memcpy((U8 *)&tempid,pbuf+StartAddr,2);
	    ConfigID[i] = ntohs(tempid);
		ValueType =  *((U8 *)(pbuf+StartAddr+2));
		switch(ValueType)
		{
		 	case SNIF_VALUE_TYPE_CHAR:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),1);
			    StartAddr = StartAddr+4;
			    break;
			}
			case SNIF_VALUE_TYPE_SHORT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),2);
			    StartAddr = StartAddr+5;
				break;
			}
			case SNIF_VALUE_TYPE_INT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_STRING:
			{
			    BytesOfString = ntohs(*((U16 *)(pbuf+StartAddr+3)));
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+5),BytesOfString);
				StartAddr = StartAddr+5+BytesOfString;
				break;
			}
			case SNIF_VALUE_TYPE_UINT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_GUID:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),16);
			    StartAddr = StartAddr+19;
				break;
			}
			case SNIF_VALUE_TYPE_FLOAT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
		}
    }

    static U8 FieldValue[7];
    U8 signal = 0;
    //TB_GetParam(&FieldValue[0], &FieldValue[1], &FieldValue[2], &FieldValue[3], &signal, &FieldValue[5], &FieldValue[6]);
    FieldValue[0]=0;
	FieldValue[1]=0;
	FieldValue[2]=0;
	FieldValue[3]=0;
	FieldValue[4]=0;
	FieldValue[5]=0;
	FieldValue[6]=0;

	U8 ChangeFlag=0;
    U32 MidVal=0;
    for(i=0;i<TempCount;i++)
    {
        switch(ConfigID[i])
        {
            case CONFIG_VIDEOOUT_SD_DISPLAY_RATIO:
            {
                ChangeFlag =1;
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[0] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_SD_169_PROCESSING:
            {
                ChangeFlag =1;
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[1] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_SD_SCART_FORMAT:
            {
                ChangeFlag =1;
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[2] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_SD_DISPLAY_MODE:
            {
                ChangeFlag =1;
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[3] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_SD_VIDEO_SIGNAL:
            {
                ChangeFlag =1;
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[4] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_SD_AUDIO_LANGUAGE:
            {
                ChangeFlag =1;
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[5] = (U8)MidVal;
                break;
            }
            case CONFIG_VIDEOOUT_SD_DIGITAL_AUDIO_OUT:
            {
                ChangeFlag =1;
                memcpy((U8 *)&MidVal,&ValueBuff[i][0],4);
                MidVal = ntohl(MidVal);
                FieldValue[6] = (U8)MidVal;
                break;
            }
            default:
                break;
        }
    }
    if(ChangeFlag ==1)
    {
        ChangeFlag=0;
        //TB_SetParam(FieldValue[0], FieldValue[1], FieldValue[2], FieldValue[3], FieldValue[4],FieldValue[5],FieldValue[6]);
    }
    if(signal != FieldValue[4])
    {
        //display cannot refresh,run watchdog
        //WDT_Running_Flag = FALSE;
    }
}
void SetDuplexAudioConfig(U8 *pbuf,U16 TempCount)
{
    U8 i=0;
    U16  ConfigID[20];
    U16  StartAddr=0;
    U8    ValueBuff[20][1024];
    U8     ValueType;
    U16  BytesOfString=0;
    U16 tempid;
    memset(ValueBuff,0,20*1024);
    for(i=0;i<TempCount;i++)
    {
        memcpy((U8 *)&tempid,pbuf+StartAddr,2);
	   	ConfigID[i] = ntohs(tempid);
		ValueType =  *((U8 *)(pbuf+StartAddr+2));
		switch(ValueType)
		{
		 	case SNIF_VALUE_TYPE_CHAR:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),1);
			    StartAddr = StartAddr+4;
			    break;
			}
			case SNIF_VALUE_TYPE_SHORT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),2);
			    StartAddr = StartAddr+5;
				break;
			}
			case SNIF_VALUE_TYPE_INT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_STRING:
			{
			    BytesOfString = ntohs(*((U16 *)(pbuf+StartAddr+3)));
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+5),BytesOfString);
				StartAddr = StartAddr+5+BytesOfString;
				break;
			}
			case SNIF_VALUE_TYPE_UINT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
			case SNIF_VALUE_TYPE_GUID:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),16);
			    StartAddr = StartAddr+19;
				break;
			}
			case SNIF_VALUE_TYPE_FLOAT:
			{
			    memcpy((U8 *)&ValueBuff[i][0],(U8 *)(pbuf+StartAddr+3),4);
			    StartAddr = StartAddr+7;
				break;
			}
		}
    }

    DuplexAudio DuplexingAudio_Read;
	eRet ErrCode = eRet_InvalidParameter;
    static U32 FieldValue[8];
    U8 ChangeFlag=0;

    memset(&DuplexingAudio_Read,0x00,sizeof(DuplexAudio));
	//ErrCode = DuplexAudioGet(&DuplexingAudio_Read);
	if(ErrCode == eRet_Ok)
	{
        if ((0<DuplexingAudio_Read.ADCdelay) && (DuplexingAudio_Read.ADCdelay<=60))
		{
			FieldValue[0]=DuplexingAudio_Read.ADCdelay;
		}
		switch(DuplexingAudio_Read.CheckLowNUM)
		{
			case 1023 : FieldValue[1]=0;break;
			case 500  : FieldValue[1]=1;break;
			case 200  : FieldValue[1]=2;break;
			case 50   : FieldValue[1]=3;break;
			case 30   : FieldValue[1]=4;break;
			case 20   : FieldValue[1]=5;break;
			case 15   : FieldValue[1]=6;break;
			case 10   : FieldValue[1]=7;break;
			case 5    : FieldValue[1]=8;break;
			case 4    : FieldValue[1]=9;break;
			default   : FieldValue[1]=0;break;
		}

		if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x01)
		{
			FieldValue[2]=1;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x02)
		{
			FieldValue[2]=2;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x04)
		{
			FieldValue[2]=3;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x08)
		{
			FieldValue[2]=4;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x10)
		{
			FieldValue[2]=5;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x20)
		{
			FieldValue[2]=6;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x40)
		{
			FieldValue[2]=7;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x80)
		{
			FieldValue[2]=8;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x01 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=9;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x02 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=10;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x04 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=11;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x08 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=12;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x10 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=13;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x20 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=14;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x40 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=15;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x00 && DuplexingAudio_Read.Ad2 == 0x80 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=16;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x01 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=17;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x02 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=18;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x04 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=19;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x08 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=20;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x10 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=21;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x20 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=22;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x40 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=23;
		}
		else if(DuplexingAudio_Read.Ad1 == 0x80 && DuplexingAudio_Read.Ad2 == 0x00 && DuplexingAudio_Read.Ad3 == 0x00)
		{
			FieldValue[2]=24;
		}
		else if(DuplexingAudio_Read.Ad1 == 0xFF && DuplexingAudio_Read.Ad2 == 0xFF && DuplexingAudio_Read.Ad3 == 0xFF)
		{
			FieldValue[2]=25;
		}
		else
		{
			FieldValue[2]=0;
		}

		if (DuplexingAudio_Read.AudioInSet<=1)
		{
			FieldValue[3]=DuplexingAudio_Read.AudioInSet;
		}
		if (DuplexingAudio_Read.AudioOutSet<=1)
		{
			FieldValue[4]=DuplexingAudio_Read.AudioOutSet;
		}
		if (DuplexingAudio_Read.AudioPGA<=7)
		{
			FieldValue[5]=DuplexingAudio_Read.AudioPGA;
		}

		if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x01)
		{
			FieldValue[6]=1;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x02)
		{
			FieldValue[6]=2;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x04)
		{
			FieldValue[6]=3;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x08)
		{
			FieldValue[6]=4;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x10)
		{
			FieldValue[6]=5;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x20)
		{
			FieldValue[6]=6;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x40)
		{
			FieldValue[6]=7;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x80)
		{
			FieldValue[6]=8;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x01 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=9;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x02 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=10;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x04 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=11;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x08 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=12;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x10 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=13;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x20 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=14;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x40 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=15;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x00 && DuplexingAudio_Read.VOR_Ad2 == 0x80 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=16;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x01 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=17;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x02 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=18;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x04 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=19;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x08 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=20;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x10 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=21;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x20 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=22;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x40 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=23;
		}
		else if(DuplexingAudio_Read.VOR_Ad1 == 0x80 && DuplexingAudio_Read.VOR_Ad2 == 0x00 && DuplexingAudio_Read.VOR_Ad3 == 0x00)
		{
			FieldValue[6]=24;
		}
		else
		{
			FieldValue[6]=0;
		}

		if ((0<DuplexingAudio_Read.VOT_Channel) && (DuplexingAudio_Read.VOT_Channel<=64))
		{
			FieldValue[7]=DuplexingAudio_Read.VOT_Channel ;
		}
    }
    for(i=0;i<TempCount;i++)
    {
        switch(ConfigID[i])
        {
            case CONFIG_DUPLEX_AUDIO_DELAY:
            {
                ChangeFlag =1;
                memcpy((U8 *)&FieldValue[0],&ValueBuff[i][0],4);
              	FieldValue[0] = ntohl(FieldValue[0]);
                break;
            }
            case CONFIG_DUPLEX_AUDIO_SENSITIVITY:
            {
                ChangeFlag =1;
                memcpy((U8 *)&FieldValue[1],&ValueBuff[i][0],4);
                FieldValue[1] = ntohl(FieldValue[1]);
                break;
            }
            case CONFIG_DUPLEX_AUDIO_T_ADDRESS:
            {
                ChangeFlag =1;
                memcpy((U8 *)&FieldValue[2],&ValueBuff[i][0],4);
                FieldValue[2] = ntohl(FieldValue[2]);
                break;
            }
            case CONFIG_DUPLEX_AUDIO_IN_SET:
            {
                ChangeFlag =1;
                memcpy((U8 *)&FieldValue[3],&ValueBuff[i][0],4);
                FieldValue[3] = ntohl(FieldValue[3]);
                break;
            }
            case CONFIG_DUPLEX_AUDIO_OUT_SET:
            {
                ChangeFlag =1;
                memcpy((U8 *)&FieldValue[4],&ValueBuff[i][0],4);
                FieldValue[4] = ntohl(FieldValue[4]);
                break;
            }
            case CONFIG_DUPLEX_AUDIO_IN_GAIN:
            {
                ChangeFlag =1;
                memcpy((U8 *)&FieldValue[5],&ValueBuff[i][0],4);
                FieldValue[5] = ntohl(FieldValue[5]);
                break;
            }
            case CONFIG_DUPLEX_AUDIO_R_ADDRESS:
            {
                ChangeFlag =1;
                memcpy((U8 *)&FieldValue[6],&ValueBuff[i][0],4);
                FieldValue[6] = ntohl(FieldValue[6]);
                break;
            }
            case CONFIG_DUPLEX_AUDIO_CHANNEL:
            {
                ChangeFlag =1;
                memcpy((U8 *)&FieldValue[7],&ValueBuff[i][0],4);
                FieldValue[7] = ntohl(FieldValue[7]);
                break;
            }
            default:
                break;
        }
    }

    DuplexAudio DuplexingAudio_Write;
    //if(ChangeFlag == 1)
    if(0)
    {
        ChangeFlag =0;
        DuplexingAudio_Write.ADCdelay=FieldValue[0];
	    switch(FieldValue[1])
		{
            case 0: DuplexingAudio_Write.CheckLowNUM=1023;break;
			case 1: DuplexingAudio_Write.CheckLowNUM=  500;break;
			case 2: DuplexingAudio_Write.CheckLowNUM=  200;break;
			case 3: DuplexingAudio_Write.CheckLowNUM=    50;break;
			case 4: DuplexingAudio_Write.CheckLowNUM=    30;break;
			case 5: DuplexingAudio_Write.CheckLowNUM=    20;break;
			case 6: DuplexingAudio_Write.CheckLowNUM=    15;break;
			case 7: DuplexingAudio_Write.CheckLowNUM=    10;break;
			case 8: DuplexingAudio_Write.CheckLowNUM=      5;break;
			case 9: DuplexingAudio_Write.CheckLowNUM=      4;break;
			default:DuplexingAudio_Write.CheckLowNUM=      4;break;
	    }

        switch(FieldValue[2])
	    {
            case 1:
			{
                DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x01;
				break;
			}
            case 2:
    	    {
    		  	DuplexingAudio_Write.Ad1=0x00;
    			DuplexingAudio_Write.Ad2=0x00;
    			DuplexingAudio_Write.Ad3=0x02;
    			break;
    		}
    		case 3:
    		{
    		  	DuplexingAudio_Write.Ad1=0x00;
    			DuplexingAudio_Write.Ad2=0x00;
    			DuplexingAudio_Write.Ad3=0x04;
    			break;
    		}
    		case 4:
    		{
    		  	DuplexingAudio_Write.Ad1=0x00;
    			DuplexingAudio_Write.Ad2=0x00;
    			DuplexingAudio_Write.Ad3=0x08;
    			break;
    		}
			case 5:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x10;
				break;
			}
			case 6:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x20;
				break;
			}
			case 7:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x40;
				break;
			}
			case 8:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x80;
				break;
			}
			case 9:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x01;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 10:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x02;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 11:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x04;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 12:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x08;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 13:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x10;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 14:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x20;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 15:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x40;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 16:
			{
			  	DuplexingAudio_Write.Ad1=0x00;
				DuplexingAudio_Write.Ad2=0x80;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 17:
			{
			  	DuplexingAudio_Write.Ad1=0x01;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 18:
			{
			  	DuplexingAudio_Write.Ad1=0x02;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 19:
			{
			  	DuplexingAudio_Write.Ad1=0x04;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 20:
			{
			  	DuplexingAudio_Write.Ad1=0x08;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 21:
			{
			  	DuplexingAudio_Write.Ad1=0x10;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 22:
			{
			  	DuplexingAudio_Write.Ad1=0x20;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 23:
			{
			  	DuplexingAudio_Write.Ad1=0x40;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 24:
			{
			  	DuplexingAudio_Write.Ad1=0x80;
				DuplexingAudio_Write.Ad2=0x00;
				DuplexingAudio_Write.Ad3=0x00;
				break;
			}
			case 25:
			{
			  	DuplexingAudio_Write.Ad1=0xFF;
				DuplexingAudio_Write.Ad2=0xFF;
				DuplexingAudio_Write.Ad3=0xFF;
				break;
			}
			default:
			{
			  	DuplexingAudio_Write.Ad1=0xFF;
				DuplexingAudio_Write.Ad2=0xFF;
				DuplexingAudio_Write.Ad3=0xFF;
				break;
			}
        }
        DuplexingAudio_Write.AudioInSet=FieldValue[3];
	    DuplexingAudio_Write.AudioOutSet=FieldValue[4];
		DuplexingAudio_Write.AudioPGA=FieldValue[5];

        switch(FieldValue[6])
		{
            case 1:
            {
                DuplexingAudio_Write.VOR_Ad1=0x00;
				DuplexingAudio_Write.VOR_Ad2=0x00;
				DuplexingAudio_Write.VOR_Ad3=0x01;
				break;
		    }
            case 2:
            {
                DuplexingAudio_Write.VOR_Ad1=0x00;
				DuplexingAudio_Write.VOR_Ad2=0x00;
				DuplexingAudio_Write.VOR_Ad3=0x02;
				break;
			}
            case 3:
			{
                DuplexingAudio_Write.VOR_Ad1=0x00;
				DuplexingAudio_Write.VOR_Ad2=0x00;
				DuplexingAudio_Write.VOR_Ad3=0x04;
				break;
			}
            case 4:
			{
                DuplexingAudio_Write.VOR_Ad1=0x00;
				DuplexingAudio_Write.VOR_Ad2=0x00;
				DuplexingAudio_Write.VOR_Ad3=0x08;
				break;
			}
			case 5:
			{
                DuplexingAudio_Write.VOR_Ad1=0x00;
				DuplexingAudio_Write.VOR_Ad2=0x00;
				DuplexingAudio_Write.VOR_Ad3=0x10;
				break;
			}
            case 6:
			{
                DuplexingAudio_Write.VOR_Ad1=0x00;
				DuplexingAudio_Write.VOR_Ad2=0x00;
				DuplexingAudio_Write.VOR_Ad3=0x20;
				break;
			}
            case 7:
		    {
                DuplexingAudio_Write.VOR_Ad1=0x00;
				DuplexingAudio_Write.VOR_Ad2=0x00;
				DuplexingAudio_Write.VOR_Ad3=0x40;
				break;
			}
            case 8:
			{
                DuplexingAudio_Write.VOR_Ad1=0x00;
				DuplexingAudio_Write.VOR_Ad2=0x00;
				DuplexingAudio_Write.VOR_Ad3=0x80;
				break;
            }
            case 9:
			{
                DuplexingAudio_Write.VOR_Ad1=0x00;
				DuplexingAudio_Write.VOR_Ad2=0x01;
				DuplexingAudio_Write.VOR_Ad3=0x00;
				break;
			}
            case 10:
			{
				DuplexingAudio_Write.VOR_Ad1=0x00;
				DuplexingAudio_Write.VOR_Ad2=0x02;
				DuplexingAudio_Write.VOR_Ad3=0x00;
				break;
			}
            case 11:
			{
                DuplexingAudio_Write.VOR_Ad1=0x00;
				DuplexingAudio_Write.VOR_Ad2=0x04;
				DuplexingAudio_Write.VOR_Ad3=0x00;
				break;
		    }
            case 12:
            {
                DuplexingAudio_Write.VOR_Ad1=0x00;
 			    DuplexingAudio_Write.VOR_Ad2=0x08;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 13:
 		    {
 		  	    DuplexingAudio_Write.VOR_Ad1=0x00;
 			    DuplexingAudio_Write.VOR_Ad2=0x10;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 14:
 		    {
 		  	    DuplexingAudio_Write.VOR_Ad1=0x00;
 			    DuplexingAudio_Write.VOR_Ad2=0x20;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 15:
 		    {
 		  	    DuplexingAudio_Write.VOR_Ad1=0x00;
 			    DuplexingAudio_Write.VOR_Ad2=0x40;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 16:
 		    {
 		    	DuplexingAudio_Write.VOR_Ad1=0x00;
 			    DuplexingAudio_Write.VOR_Ad2=0x80;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 17:
 		    {
 		  	    DuplexingAudio_Write.VOR_Ad1=0x01;
 			    DuplexingAudio_Write.VOR_Ad2=0x00;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 18:
 		    {
 		    	DuplexingAudio_Write.VOR_Ad1=0x02;
 			    DuplexingAudio_Write.VOR_Ad2=0x00;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 19:
 		    {
 		    	DuplexingAudio_Write.VOR_Ad1=0x04;
 			    DuplexingAudio_Write.VOR_Ad2=0x00;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 20:
 		    {
 		  	    DuplexingAudio_Write.VOR_Ad1=0x08;
 			    DuplexingAudio_Write.VOR_Ad2=0x00;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 21:
 		    {
 		  	    DuplexingAudio_Write.VOR_Ad1=0x10;
 			    DuplexingAudio_Write.VOR_Ad2=0x00;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 22:
 		    {
 		  	    DuplexingAudio_Write.VOR_Ad1=0x20;
 			    DuplexingAudio_Write.VOR_Ad2=0x00;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 23:
 		    {
 		  	    DuplexingAudio_Write.VOR_Ad1=0x40;
 			    DuplexingAudio_Write.VOR_Ad2=0x00;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    case 24:
 		    {
 		  	    DuplexingAudio_Write.VOR_Ad1=0x80;
 			    DuplexingAudio_Write.VOR_Ad2=0x00;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
 		    default:
 		    {
 		  	    DuplexingAudio_Write.VOR_Ad1=0x80;
 			    DuplexingAudio_Write.VOR_Ad2=0x00;
 			    DuplexingAudio_Write.VOR_Ad3=0x00;
 			    break;
 		    }
        }
        DuplexingAudio_Write.VOT_Channel = FieldValue[7];
		DuplexAudioSet(&DuplexingAudio_Write);
    }
}



void FactorySetting()
{

}




void *TcpConfiguration(void *argu)
{

   	struct sockaddr_in server_sockaddr;
	int listenfd;
	CLIENT_TCP client[MAX_CLIENT];
	fd_set rset,allset;
	int i=0,j=0;
	int maxfd=0;
	int nready=0;
	struct timeval timeout={3,0}; //select等待3秒，3秒轮询，要非阻塞就置0
	socklen_t sin_size;
	struct sockaddr_in addr;
	int connectfd;
	char recvbuf[MAXDATASIZE];  //缓冲区
	ssize_t n;
	int sockfd;

	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
	   perror("socket");
	   exit(1);
	}
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(6500);
	server_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_sockaddr.sin_zero), 8);

	int  option = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	int keepalive = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));

	if (bind(listenfd, (struct sockaddr *)&server_sockaddr,sizeof(struct sockaddr)) == -1)
	{
	   perror("bind");
	   close(listenfd);
	   exit(1);
	}

	if(listen(listenfd, 5) == -1)
	{
	   perror("listen");
	   close(listenfd);
	   exit(1);
	}
    memset(client,0,sizeof(client));
	printf("sizeof(client)=%d\n",sizeof(client));
	for(i=0;i<MAX_CLIENT;i++)
	{
	  	client[i].fd=-1;
	}
	maxfd=listenfd;
    FD_ZERO(&allset);           //清空
    FD_SET(listenfd, &allset);  //将监听socket加入select检测的描述符集合
    while(1)
    {
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;
        rset = allset;
    	nready = select(maxfd + 1, &rset, NULL, NULL, &timeout);    //调用select
    //	printf("nready=%d\n",nready);
    	if(nready<0)
    	{
    		continue;
    	}
    	else if(nready==0)
    	{
    		continue;
    	}
    	else
    	{
    		if (FD_ISSET(listenfd, &rset))
    		{
                printf("Accept a connection.\n");//检测是否有新客户端请求
                //调用accept，返回服务器与客户端连接的socket描述符
                sin_size = sizeof(struct sockaddr_in);
                if ((connectfd = accept(listenfd, (struct sockaddr *)&addr, &sin_size)) == -1)
                {
                    perror("Accept() error\n");
                    continue;
                }

                //int keepalive = 1; // 开启keepalive属性
				int keepidle = 30; // 如该连接在60秒内没有任何数据往来,则进行探测
				int keepinterval = 5; // 探测时发包的时间间隔为5 秒
				int keepcount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.
				setsockopt(connectfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
				setsockopt(connectfd, SOL_TCP, TCP_KEEPIDLE, (void*)&keepidle , sizeof(keepidle ));
				setsockopt(connectfd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval ));
				setsockopt(connectfd, SOL_TCP, TCP_KEEPCNT, (void *)&keepcount , sizeof(keepcount ));
                //将新客户端的加入数组
                for (i = 0; i < MAX_CLIENT; i++)
                {
                    if (client[i].fd < 0)
                    {
                        char buffer[20];
                        client[i].fd = connectfd;   //保存客户端描述符
                        memset(buffer, '0', sizeof(buffer));
                        sprintf(buffer, "Client[%.2d]", i);
                        memcpy(client[i].name, buffer, strlen(buffer));
                        client[i].addr = addr;
                        memset(buffer, '0', sizeof(buffer));
                        sprintf(buffer, "Only For Test!");
                        memcpy(client[i].data, buffer, strlen(buffer));
                        printf("You got a connection from %s:%d.\n", inet_ntoa(client[i].addr.sin_addr),ntohs(client[i].addr.sin_port));
                        printf("Add a new connection:%s\n",client[i].name);
                        break;
                    }
                }

                if (i == MAX_CLIENT)
                {
                    printf("Too many clients\n");
            	}

                FD_SET(connectfd, &allset); //将新socket连接放入select监听集合
                if (connectfd > maxfd)
                {
                    maxfd = connectfd;  //确认maxfd是最大描述符
                }
            }

            for (i = 0; i < MAX_CLIENT; i++)
            {
                if ((sockfd = client[i].fd) < 0)    //如果客户端描述符小于0，则没有客户端连接，检测下一个
                    continue;
                // 有客户连接，检测是否有数据
                if (FD_ISSET(sockfd, &rset))
                {
                	memset(recvbuf,0,sizeof(recvbuf));
                    //  printf("Receive from connect fd[%d].\n", i);
                    if ((n = recv(sockfd, recvbuf, MAXDATASIZE, 0)) <= 0)
                    {               //从客户端socket读数据，等于0表示网络中断
                        close(sockfd);  //关闭socket连接
                        printf("%s closed. User's data: %s\n", client[i].name, client[i].data);
                        FD_CLR(sockfd, &allset);    //从监听集合中删除此socket连接
                        client[i].fd = -1;  //数组元素设初始值，表示没客户端连接
                        if(maxfd == sockfd)
                        {
                        	maxfd=listenfd;
    	                    for(j=0;j<MAX_CLIENT;j++)
    	                    {
    	                    	if(client[j].fd>maxfd)
    	                    	{
    	                    		maxfd=client[j].fd;
    	                    	}
    	                    }
                        }
                    }
                    else
                    {
                       process_client(&client[i], recvbuf, n); //接收到客户数据，开始处理
                    }
                }
            }
    	}
    }
}



void process_client(CLIENT_TCP * client, char *recvbuf, int len)
{
    //print("process_client Starting\n");
	unsigned char TempBuff[BUFFERLENGTH];
	unsigned char MsgType=0x00;
	U16 BytesOfResp=0x00;
	struct sockaddr_in  fromAddr;
	memcpy(TempBuff,recvbuf,len);
	MsgType = TempBuff[2];
	 switch(MsgType)
    {
        case SNIF_MSG_TYPE_D_PING_REQ:
        {
            SendPingResponse_Init(TempBuff,&BytesOfResp,&fromAddr);
            send(client->fd, &PingRespTemp, BytesOfResp, 0);
            break;
        }
        case SNIF_MSG_TYPE_IC_CAPABILITIES_REQ:
        {
            SendGetCapabilitiesResponse_Init(TempBuff,&BytesOfResp);
            send(client->fd, &GetCapabilitiesRespTemp, BytesOfResp, 0);
            break;
        }
        case SNIF_MSG_TYPE_MC_GETCONFIG_REQ:
        {
            if(GetConfigResp_Init(TempBuff,len)==1)
            {
                BytesOfResp = ntohs(GetConfigResp_Temp.SNIFHeader.Lenght);
                send(client->fd, &GetConfigResp_Temp, BytesOfResp, 0);
            }
            else
            {
                SendErrorCodeInit(TempBuff);
                BytesOfResp = ntohs(ErrResp_Temp.SNIFHeader.Lenght);
                send(client->fd, &ErrResp_Temp, BytesOfResp, 0);
            }
            break;
        }
        case SNIF_MSG_TYPE_MC_SETCONFIG_REQ:
        {
            if( SetConfigResp_Init(TempBuff,len) == 1)
            {
                BytesOfResp = ntohs(ErrResp_Temp.SNIFHeader.Lenght);
                send(client->fd, &ErrResp_Temp, BytesOfResp, 0);
            }
            else
            {
                SendErrorCodeInit(TempBuff);
                BytesOfResp = ntohs(ErrResp_Temp.SNIFHeader.Lenght);
                send(client->fd, &ErrResp_Temp, BytesOfResp, 0);
            }
            break;
        }
        case SNIF_MSG_TYPE_MC_SENDCOMMAND_REQ:
        {
            if( SendCommandResp_Init(TempBuff,len)==1)
            {
                BytesOfResp = ntohs(ErrResp_Temp.SNIFHeader.Lenght);
                send(client->fd, &ErrResp_Temp, BytesOfResp, 0);
            }
            else
            {
                SendErrorCodeInit(TempBuff);
                BytesOfResp = ntohs(ErrResp_Temp.SNIFHeader.Lenght);
                send(client->fd, &ErrResp_Temp, BytesOfResp, 0);
            }
            break;
        }
        default:
            break;
    }
}








