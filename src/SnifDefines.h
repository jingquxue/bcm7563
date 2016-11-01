/*
proc name:SnifDefines.h
Description: all kinds of  snif desciption

*/



#ifndef SNIF_DEFINES_H
#define SNIF_DEFINES_H

// ==========================================================================
//  Includes
//
// ==========================================================================
//  Defines
//
#define SNIF_PROTOCOL_VERSION                       0x0100
#define SNIFv2_PROTOCOL_VERSION                     0x0200

#define SNIF_CAPBILITY_VRESION                      0x0100


#define SNIF_CAPBILITY_COUNT                          0x000A

#define DEVICE_TYPE                                           "R780"



#define CONFIG_COUNT_OF_ENTITY_UNIT                   0x08
#define CONFIG_COUNT_OF_ENTITY_ETHERNET              0x04
#define CONFIG_COUNT_OF_ENTITY_NETAVRELAY             0x05
#define CONFIG_COUNT_OF_ENTITY_SERIALPORT             0x04
#define CONFIG_COUNT_OF_ENTITY_STORGE                 0x04
#define CONFIG_COUNT_OF_ENTITY_RECEIVER               0x07
#define CONFIG_COUNT_OF_ENTITY_VIDEOOUT_HDMI          0x09
#define CONFIG_COUNT_OF_ENTITY_MENU_SET               0x04
#define CONFIG_COUNT_OF_ENTITY_VIDEOOUT_SD            0x07
#define CONFIG_COUNT_OF_ENTITY_DUPLEX_AUDIO           0x08


#define SNIF_COMMON_HEADER_SIZE			    0x07
#define SNIF_GET_VERSION(buf) \
	((unsigned short)(*buf)[0]<<8) + (unsigned short)(*buf)[1]

#define SNIF_GET_TRANSID(buf) \
	((unsigned short)(*buf)[3]<<8) + (unsigned short)(*buf)[4]

#define SNIF_GET_PACKETSIZE(buf) \
	((unsigned short)(*buf)[5]<<8) + (unsigned short)(*buf)[6]








#define SNIF_VALUE_TYPE_CHAR 	       0x01
#define SNIF_VALUE_TYPE_SHORT  	0x02
#define SNIF_VALUE_TYPE_INT  	       0x03
#define SNIF_VALUE_TYPE_STRING	0x04
#define SNIF_VALUE_TYPE_UINT 	       0x05
#define SNIF_VALUE_TYPE_GUID	       0x06
#define SNIF_VALUE_TYPE_FLOAT  	0x07




// --------------------------------------------------------------------------
// ****************              Product type                ****************
// --------------------------------------------------------------------------


#define SNIF_MANUFACTURER_ID_UNKNOWN            0x2000
#define SNIF_MANUFACTURER_ID_ALLTECH          	0x2001


// --------------------------------------------------------------------------
// ****************              Product type                ****************
// --------------------------------------------------------------------------
#define PRODUCT_TYPE_R750 				0x00
#define PRODUCT_TYPE_R780 				0x01


// --------------------------------------------------------------------------
// ****************              Capabilities                ****************
// --------------------------------------------------------------------------
#define SNIF_ENTITY_UNIT                   0x00
#define SNIF_ENTITY_ETHERNET              0x01
#define SNIF_ENTITY_NETAVRELAY             0x02
#define SNIF_ENTITY_SERIALPORT             0x03
#define SNIF_ENTITY_STORGE                 0x04
#define SNIF_ENTITY_RECEIVER               0x05
#define SNIF_ENTITY_VIDEOOUT_HDMI          0x06
#define SNIF_ENTITY_MENUSET           0x07
#define SNIF_ENTITY_VIDEOOUT_SD            0x08
#define SNIF_ENTITY_DUPLEX_AUDIO           0x09








// --------------------------------------------------------------------------
// ****************              Connection Types            ****************
// --------------------------------------------------------------------------
#define SNIF_CONNECTION_NULL                        0x00
#define SNIF_CONNECTION_UNICAST                     0x01
#define SNIF_CONNECTION_UDP_UNICAST                 0x00  // replacing SNIF_CONNECTION_UNICAST

#define SNIF_CONNECTION_MULTICAST                   0x02
#define SNIF_CONNECTION_UDP_MULTICAST               0x02  // replacing SNIF_CONNECTION_MULTICAST

#define SNIF_CONNECTION_TCPSERVER                   0x04
#define SNIF_CONNECTION_TCPCLIENT                   0x08
#define SNIF_CONNECTION_TCP                         0x0C   // 2 bits are used because Server and client are the same

#define SNIF_CONNECTION_BROADCAST                   0x10
#define SNIF_CONNECTION_UDP_BROADCAST               0x10 // replacing SNIF_CONNECTION_BROADCAST

#define SNIF_CONNECTION_SSL                         0x20
#define SNIF_CONNECTION_RTP                         0x40
#define SNIF_CONNECTION_VOLATILE                    0x80   // Add this bit for a volatile connection

#define SNIF_CONNECTION_TCP_VOLATILE                0x8C //(SNIF_CONNECTION_TCP | SNIF_CONNECTION_VOLATILE)
#define SNIF_CONNECTION_UNICAST_VOLATILE            0x81 //(SNIF_CONNECTION_UNICAST | SNIF_CONNECTION_VOLATILE)
#define SNIF_CONNECTION_UDP_VOLATILE                0x81 // replacing SNIF_CONNECTION_UNICAST_VOLATILE

#define SNIF_CONNECTION_RTP_VOLATILE                0xC0 //(SNIF_CONNECTION_RTP | SNIF_CONNECTION_VOLATILE)

#define SNIF_CONNECTION_RTP_TCP_VOLATILE            0x180 // (SNIF_CONNECTION_VOLATILE | SNIF_CONNECTION_RTP_TCP)

#define SNIF_CONNECTION_RTP_TCP                     0x100 // When doing a Get Config we have 32 bits available, we can use 'em all

//******************************************************************************************//
//                                  Configuration Parameters
//******************************************************************************************//

// --------------------------------------------------------------------------
// ****************          ENTITY_UNIT                    ****************
// --------------------------------------------------------------------------

#define CONFIG_SYSTEM_DEVICE_TYPE                0x1001
#define CONFIG_SYSTEM_FIRMWARE_VERSION   0x1002
#define CONFIG_SYSTEM_HARDWARE_VERSION  0x1003
#define CONFIG_SYSTEM_FPGA_VERSION            0x1004
#define CONFIG_SYSTEM_DATE_TIME                   0x1005
#define CONFIG_SYSTEM_FLASH_READ                0x1006
#define CONFIG_SYSTEM_MENUCON                0x1007
#define CONFIG_SYSTEM_IDENTIFY                0x1008

// --------------------------------------------------------------------------
// ****************            ENTITY_ETHERNET              ****************
// --------------------------------------------------------------------------
#define CONFIG_NET_LOCAL_IP_ADDRESS           0x2001
#define CONFIG_NET_LOCAL_NETMASK                 0x2002
#define CONFIG_NET_LOCAL_GATEWAY                0x2003
#define CONFIG_NET_DHCP_STATE                        0x2004

// --------------------------------------------------------------------------
// ****************           ENTITY_NETAVRELAY                  ****************
// --------------------------------------------------------------------------
#define CONFIG_NETAVRELAY_TYPE                       0x3001
#define CONFIG_NETAVRELAY_REMOTEIP              0x3002
#define CONFIG_NETAVRELAY_PORT                      0x3003
#define CONFIG_NETAVRELAY_PROTOCAL             0x3004
#define CONFIG_NETAVRELAY_STATUS                  0x3005

// --------------------------------------------------------------------------
// ****************      ENTITY_SERIALPORT             ****************
// --------------------------------------------------------------------------

#define CONFIG_SERIALPORT_BAUDRATE               0x4001
#define CONFIG_SERIALPORT_WORDLENGTH        0x4002
#define CONFIG_SERIALPORT_STOPBITS               0x4003
#define CONFIG_SERIALPORT_PARITY                    0x4004


// --------------------------------------------------------------------------
// ****************      ENTITY_STORGE            ****************
// --------------------------------------------------------------------------


#define CONFIG_STORGE_TYPE                   0x5001
#define CONFIG_STORGE_PARTITION             0x5002
#define CONFIG_STORGE_ISLOOP       0x5003
#define CONFIG_STORGE_SIZE            0x5004
#define CONFIG_STARTUP_REC            0x5005




// --------------------------------------------------------------------------
// ****************      ENTITY_RECEIVER          ****************
// --------------------------------------------------------------------------
#define CONFIG_RECEIVER_DESCRAMBLE_TYPE                    0x6001
#define CONFIG_RECEIVER_DESCRAMBLE_PASSWORD         0x6002
#define CONFIG_RECEIVER_DOCODED_CHANNEL                  0x6003
#define CONFIG_RECEIVER_DOCODED_FRE                           0x6004
#define CONFIG_RECEIVER_DOCODED_BAND                         0x6005
#define CONFIG_RECEIVER_DOCODED_INFO                         0x6006
#define CONFIG_RECEIVER_DELETED_CHAN                         0x6007




// --------------------------------------------------------------------------
// ****************      ENTITY_VIDEOOUT_HDMI          ****************
// --------------------------------------------------------------------------
#define CONFIG_VIDEOOUT_HDMI_MODE                        0x7001
#define CONFIG_VIDEOOUT_HDMI_HDCP                        0x7002
#define CONFIG_VIDEOOUT_HDMI_HPD                          0x7003
#define CONFIG_VIDEOOUT_HDMI_VIDEO_FORMATS    0x7004
#define CONFIG_VIDEOOUT_HDMI_PIXEL_ENCODING   0x7005
#define CONFIG_VIDEOOUT_HDMI_PICTURE_AR            0x7006
#define CONFIG_VIDEOOUT_HDMI_ACTIVE_AR               0x7007
#define CONFIG_VIDEOOUT_HDMI_HDCP_TRACES          0x7008
#define CONFIG_VIDEOOUT_HDMI_MODE_DEBUG           0x7009

// --------------------------------------------------------------------------
// ****************      ENTITY_VIDEOOUT_DVO         ****************
// --------------------------------------------------------------------------
#define CONFIG_MENU_SET_LANGUAGE                          0x8001
#define CONFIG_MENU_SET_MENU_DISPLAY                        0x8002
#define CONFIG_MENU_SET_INFO_BAR_TIMEOUT                      0x8003
#define CONFIG_MENU_SET_TRANSPARENCY                     0x8004


// --------------------------------------------------------------------------
// ****************      ENTITY_VIDEOOUT_SD       ****************
// --------------------------------------------------------------------------
#define CONFIG_VIDEOOUT_SD_DISPLAY_RATIO 0x9001
#define CONFIG_VIDEOOUT_SD_169_PROCESSING 0x9002
#define CONFIG_VIDEOOUT_SD_SCART_FORMAT 0x9003
#define CONFIG_VIDEOOUT_SD_DISPLAY_MODE 0x9004
#define CONFIG_VIDEOOUT_SD_VIDEO_SIGNAL   0x9005
#define CONFIG_VIDEOOUT_SD_AUDIO_LANGUAGE    0x9006
#define CONFIG_VIDEOOUT_SD_DIGITAL_AUDIO_OUT 0x9007

// --------------------------------------------------------------------------
// ****************      ENTITY_DUPLEX_AUDIO       ****************
// --------------------------------------------------------------------------
#define CONFIG_DUPLEX_AUDIO_DELAY               0xA001
#define CONFIG_DUPLEX_AUDIO_SENSITIVITY    0xA002
#define CONFIG_DUPLEX_AUDIO_T_ADDRESS      0xA003
#define CONFIG_DUPLEX_AUDIO_IN_SET             0xA004
#define CONFIG_DUPLEX_AUDIO_OUT_SET          0xA005
#define CONFIG_DUPLEX_AUDIO_IN_GAIN           0xA006
#define CONFIG_DUPLEX_AUDIO_R_ADDRESS      0xA007
#define CONFIG_DUPLEX_AUDIO_CHANNEL          0xA008

// --------------------------------------------------------------------------
// ****************                  Commands                ****************
// --------------------------------------------------------------------------

   // Video commands (SNIF_ENTITY_VIDEOENCODER)
#define SNIF_COMMAND_SEND_KEYFRAME                  0x01  // Argument 1 is the number of Key Frame to send

   // Other commands
#define SNIF_COMMAND_PROPRIETARY                    0x02  // For proprietary commands

   // Set TCP connection as VITAL
#define SNIF_COMMAND_SET_CONNECTION_VITAL           0x03  // A broken VITAL connection will cause all streams to be terminated

   // Reset the wireless passkey (SNIF_ENTITY_WIRELESS_INTERFACE)
#define SNIF_COMMAND_RESET_WLS_PASS_KEY             0x04  // Reset the wireless passkey (802.11 or SPCF)

   // Remove a multicast stream using ip and port
#define SNIF_COMMAND_REMOVE_MULTI_STREAM            0x05  //Arg 1 is ip, arg 2 is port

   // Keep alive the stream specified byt the arguments
#define SNIF_COMMAND_KEEPALIVE                      0x06  // Arg1: Ip, Arg2: port, Arg3: Target guid

   // Not Supported Yet
   // Clear the Stats
#define SNIF_COMMAND_CLEAR_STATS                    0x07  // Clear the stats of the selected capability (GUID)

   // Clear the Logs
#define SNIF_COMMAND_CLEAR_LOGS                     0x08  // Clear the Logs Arg1 is log to be cleared

   // Send a ICMP echo request
#define SNIF_COMMAND_PING_REMOTE                    0x09  // arg 1 is dest ip, arg 2 is ping size, optional arg 3 is timeout


// --------------------------------------------------------------------------
// ****************           SNIF Message types             ****************
// --------------------------------------------------------------------------
#define SNIF_MSG_TYPE_NULL                                        0x00
#define SNIF_MSG_TYPE_D_PING_REQ                           0x01
#define SNIF_MSG_TYPE_D_PING_RESP                         0x02
#define SNIF_MSG_TYPE_IC_CAPABILITIES_REQ           0x03
#define SNIF_MSG_TYPE_IC_CAPABILITIES_RESP         0x04
#define SNIF_MSG_TYPE_MC_GETCONFIG_REQ              0x05
#define SNIF_MSG_TYPE_MC_GETCONFIG_RESP            0x06
#define SNIF_MSG_TYPE_MC_SETCONFIG_REQ              0x07
#define SNIF_MSG_TYPE_MC_SENDCOMMAND_REQ        0x08
#define SNIF_MSG_TYPE_ER_ERROR                               0x09
#define SNIF_MSG_TYPE_READ_FLASH_REQ                    0x0A
#define SNIF_MSG_TYPE_READ_FLASH_RESP                   0x0B
#define SNIF_MSG_TYPE_SERIALPORT_DATA_REQ          0x0C
#define SNIF_MSG_TYPE_SERIALPORT_DATA_RESP         0x0D
#define SNIF_MSG_TYPE_GET_CLIENT_INFO_REQ            0x0E
#define SNIF_MSG_TYPE_GET_CLIENT_INFO_RESP           0x0F

// --------------------------------------------------------------------------
// ****************           SNIF Error Codes               ****************
// --------------------------------------------------------------------------
#define SNIF_ERROR_CODE_SUCCESS                     0x00    // Success
#define SNIF_ERROR_CODE_FAILURE                     0x01    // Replacing SNIF_ERROR_CODE_FAIL
#define SNIF_ERROR_CODE_UNKNOWNCAPABILITY           0x02    // Unknown Capability


#define PRO_VERSION		0x0001
#define CAP_VERSION		0x0001

typedef unsigned char eMsgType;
typedef unsigned char eValueType;
typedef unsigned char eSNIFConnectionType;
// ==========================================================================
//  Forward references
//
// ==========================================================================
//  Structures and Classes
//
// ==========================================================================
//  Inline functions
//
// ==========================================================================
//  Prototypes
//
// ==========================================================================
//  External data
//
// ==========================================================================
//  Debug
//
// ==========================================================================

#endif
