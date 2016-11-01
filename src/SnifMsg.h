//
//
// Copyright 2001-2002 by ComLink Group Inc. All rights reserved.
//
// No part of this document may be reproduced, stored in
// a retrieval system, or transmitted, in any form or by any means,
// electronic, mechanical, photocopying, recording, or otherwise,
// without the prior written permission of ComLink Group Inc.
//
//
//    NAME  :  SNIFMsg.h
//    AUTHOR:  SmartSight Networks
//    Description: This file contains all the message definition
//                 for the SNIF protocol.
//
//
#ifndef SNIF_MSG_H_
#define SNIF_MSG_H_

//
// *************************
// **       Defines       **
// *************************
//
#include "SnifDefines.h"
#include <hi_type.h>
#include "pthread.h"
#include <time.h>
#include <semaphore.h>

//#include <objbase.h>
//#include "Guid.h"
//#pragma warning (disable : 4786)
//#include <list>

#pragma pack(push)

//#pragma once

#pragma pack(1)
//
// *************************
// **       Typedef       **
// *************************
 typedef struct _GUID {          // size is 16
	unsigned  char   Data0[4];				//the first byte is entity,second is item no,forth is product type
	 unsigned int  Data1;			//reserve
        unsigned char  Data2[8];		//device mac addresss

    } GUID;
//***************************************************
//              General Structure
//***************************************************
typedef struct _SNIF_HEADER
{
   unsigned short ProtocolVersion;
   char MsgType;
   unsigned short TransactionNumber;
   unsigned short Lenght;

}__attribute__ ((packed)) SNIF_HEADER, *PSNIF_HEADER;

//***************************************************
//           Family specific messages
//***************************************************
//Discovery
typedef struct _SNIF_DISCOVERY_PING_INFO
{
   unsigned int   IpAddress;
   unsigned short Port;
   unsigned char  ConnectionType;

}__attribute__ ((packed)) SNIF_DISCOVERY_PING_INFO, *PSNIF_DISCOVERY_PING_INFO;


//ping request

typedef struct _SNIF_DISCOVERY_PINGREQUEST_MSG
{
   SNIF_HEADER                SNIFHeader;
   SNIF_DISCOVERY_PING_INFO   SNIFDPingInfo;

}__attribute__ ((packed)) SNIF_DISCOVERY_PINGREQUEST_MSG, *PSNIF_DISCOVERY_PINGREQUEST_MSG;


//ping response

typedef struct _SNIF_DISCOVERY_PINGRESPONSE_MSG
{
   SNIF_HEADER                SNIFHeader;
   SNIF_DISCOVERY_PING_INFO   SNIFDPingInfo;
   GUID                       guid;
   unsigned short             Fabricant;
   unsigned short             ProductType;
  unsigned char              StatusChanged;

}__attribute__ ((packed)) SNIF_DISCOVERY_PINGRESPONSE_MSG, *PSNIF_DISCOVERY_PINGRESPONSE_MSG;


//get capability request

typedef struct _SNIF_IC_CAPABILITIES
{
   unsigned char     EntityType;
   GUID              guid;
   unsigned short    Fabricant;
   unsigned short    Version;

}__attribute__ ((packed)) SNIF_IC_CAPABILITIES, *PSNIF_IC_CAPABILITIES;

typedef struct _SNIF_IC_REQUEST_MSG
{
   SNIF_HEADER          SNIFHeader;
}__attribute__ ((packed)) SNIF_IC_REQUEST_MSG, *PSNIF_IC_REQUEST_MSG;

//get capability response
typedef struct _SNIF_IC_RESPONSE_MSG
{
   SNIF_HEADER          SNIFHeader;
   unsigned short       ListCount;
   SNIF_IC_CAPABILITIES Capabilities[SNIF_CAPBILITY_COUNT];

}__attribute__ ((packed)) SNIF_IC_RESPONSE_MSG, *PSNIF_IC_RESPONSE_MSG;

//get config item request
typedef struct _SNIF_GET_CONFIG_REQUEST_MSG
{
  SNIF_HEADER          SNIFHeader;
  GUID                      guid;
  unsigned short         ConfigCount;
  unsigned short         ConfigItemID[100];
}__attribute__ ((packed)) SNIF_GET_CONFIG_REQUEST_MSG, *PSNIF_GET_CONFIG_REQUEST_MSG;

//get config item response

typedef struct _CONFIG_DATA_STRING
{
   unsigned short    ValueSize;
   unsigned char     *pszValue;
}__attribute__ ((packed)) CONFIG_DATA_STRING, *PCONFIG_DATA_STRING;

union CONFIG_DATA
{
   unsigned char      cValue;
   unsigned short     sValue;
   unsigned int       uiValue;
   int                iValue;
   CONFIG_DATA_STRING szValue;
}__attribute__ ((packed)) ;


typedef struct _CONFIG_ITEM
{
   unsigned short    ConfigId;
   unsigned char     ValueType;
   unsigned char      ConfigData[1]; //pointer to one of the CONFIG_DATA union member
}__attribute__ ((packed)) CONFIG_ITEM, *PCONFIG_ITEM;

typedef struct _SNIF_GET_CONFIG_RESP_MSG
{
  SNIF_HEADER          SNIFHeader;
  GUID                      guid;
  unsigned short         ConfigCount;
  unsigned char          ConfigItem[2048];
}__attribute__ ((packed)) SNIF_GET_CONFIG_RESP_MSG, *PSNIF_GET_CONFIG_RESP_MSG;




//set config item request

typedef struct _SNIF_SET_CONFIG_REQ_MSG
{
  SNIF_HEADER        SNIFHeader;
  GUID                      guid;
  unsigned short      ConfigCount;
  unsigned char        ConfigItem[2048];
}__attribute__ ((packed)) SNIF_SET_CONFIG_REQ_MSG, *PSNIF_SET_CONFIG_REQ_MSG;

//send command request

typedef struct _SNIF_SEND_COMMAND_REQ_MSG
{
  SNIF_HEADER          SNIFHeader;
  GUID                      guid;
  unsigned char         commandid;
  unsigned int            par[2];
}__attribute__ ((packed)) SNIF_SEND_COMMAND_REQ_MSG, *PSNIF_SEND_COMMAND_REQ_MSG;
//error code response

typedef struct _SNIF_ERROR_RESP_MSG
{
  SNIF_HEADER          SNIFHeader;
  unsigned char          request_type;
  unsigned int         error_code;
}__attribute__ ((packed)) SNIF_ERROR_RESP_MSG, *PSNIF_ERROR_RESP_MSG;

//Read Flash Request

typedef struct _SNIF_READ_FLASH_REQ_MSG
{
  SNIF_HEADER          SNIFHeader;
  GUID                      guid;
}__attribute__ ((packed)) SNIF_READ_FLASH_REQ_MSG, *PSNIF_READ_FLASH_REQ_MSG;

//Read Flash Response

typedef struct _CHANNEL_INFO
{
  unsigned int             frequensy;
  unsigned short         Polarity;
  unsigned short         Vpid;
  unsigned short         Apid;
  unsigned char          Vtype;
  unsigned char          Atype;
}__attribute__ ((packed)) CHANNEL_INFO, *PCHANNEL_INFO;


typedef struct _SNIF_READ_FLASH_RESP_MSG
{
  SNIF_HEADER          SNIFHeader;
  GUID                      guid;
  unsigned int             globalvarslist[49];
  unsigned int             descrablekey[33];
  short int                    favoritelist[2][33];
  CHANNEL_INFO       *channel_info;
}__attribute__ ((packed)) SNIF_READ_FLASH_RESP_MSG, *PSNIF_READ_FLASH_RESP_MSG;


//Push Data to Serial Port Request

typedef struct _SNIF_PUSH_DATA_TO_SERIALPORT_REQ_MSG
{
  SNIF_HEADER          SNIFHeader;
  GUID                      guid;
  unsigned char          whichcom;
  unsigned char          flag;
  unsigned short         bytes;
  unsigned char          value[1];
}__attribute__ ((packed)) SNIF_PUSH_DATA_TO_SERIALPORT_REQ_MSG, *PSNIF_PUSH_DATA_TO_SERIALPORT_REQ_MSG;


//Push Data to Serial Port Response

typedef struct _SNIF_PUSH_DATA_TO_SERIALPORT_RESP_MSG
{
  SNIF_HEADER          SNIFHeader;
  GUID                      guid;
  unsigned char          whichcom;
  unsigned short         bytes;
  unsigned char          value[1];
}__attribute__ ((packed)) SNIF_PUSH_DATA_TO_SERIALPORT_RESP_MSG, *PSNIF_PUSH_DATA_TO_SERIALPORT_RESP_MSG;


typedef struct _CHANNEL_ORG
{
  unsigned int Fre;
  unsigned short Band;
  unsigned short Vpid;
  unsigned short Apid;
  unsigned short Pcrvid;
  unsigned short AVtype;
}__attribute__ ((packed)) CHANNEL_ORG;




typedef struct _SNIF_GET_CLIENT_INFO_REQ_MSG
{
  SNIF_HEADER          SNIFHeader;
}__attribute__ ((packed)) SNIF_GET_CLIENT_INFO_REQ_MSG, *PSNIF_GET_CLIENT_INFO_REQ_MSG;

typedef struct _SNIF_GET_CLIENT_INFO_RESP_MSG
{
  SNIF_HEADER          SNIFHeader;
  signed short            locked;
  signed short            strength;
  signed short            quality;
}__attribute__ ((packed)) SNIF_GET_CLIENT_INFO_RESP_MSG, *PSNIF_GET_CLIENT_INFO_RESP_MSG;

typedef struct _SNIF_TCP_SEND_COMMAND_REQ_MSG
{
  SNIF_HEADER          SNIFHeader;
  unsigned char         commandid;   //0x00
  unsigned char            par[2];          //0x05 0x00,open ts;0x06 0x00;Close ts
}__attribute__ ((packed)) SNIF_TCP_SEND_COMMAND_REQ_MSG, *PSNIF_TCP_SEND_COMMAND_REQ_MSG;



#pragma pack(pop)


#endif // _SNIF_MSH_H_
