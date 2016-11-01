#ifndef KWT_MAIN_H__
#define KWT_MAIN_H__

#include "nexus_platform.h"
#include "bwin.h"
#include "bwidgets.h"

#include "nexus_simple_stc_channel.h"

#include "nexus_video_decoder.h"
#include "nexus_simple_video_decoder_server.h"
#include "nexus_core_utils.h"
#include "nexus_simple_audio_decoder.h"





typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#define HDMI_HPD_IO_HANDLER 10
#define IR_HANDLER 100
#define AUDIO_DECODE_HANDLE_0 0x1000
#define AUDIO_DECODE_HANDLE_1 0x1001
#define SIMPLE_AUDIO_DECODE_HANDLE 0x1002
#define VIDEO_DECODE_HANDLE 0x1110
#define SIMPLE_VIDEO_DECODE_HANDLE 0x1111

typedef enum eMenu
{
    eMenu_NULL,
    eMenu_Main,
    eMenu_Info,
    eMenu_ChannelSearch,
    eMenu_FileBrowser,
    eMenu_Playback,
    eMenu_DisplaySettings,
    eMenu_ChannelEdit,
    eMenu_VideoChannelList,
    eMenu_GeneralSettings,
    eMenu_SystemInfo,
    eMenu_DateTimeSet,
    eMenu_TCPIPConfiguration,
    eMenu_NetworkConfg,
    eMenu_DescramblingConfig,
    eMenu_DuplexAudio,
    eMenu_RecordConfig
} eMenu;

extern bwidget_t L_BackGround;
extern eMenu CurrentMenu;
extern NEXUS_I2cHandle i2c;
extern NEXUS_AudioDacHandle g_OutputAudioDac;

#define BACKGROUND_COLOR 0xFF6b8ca5

typedef enum bevelParts_t
{
    bevel_eTop,
    bevel_eRight,
    bevel_eBottom,
    bevel_eLeft,
    bevel_eMax
} bevelParts_t;

typedef enum eStcType
{
    eStcType_ParserBand,
    eStcType_TtsPacing,
    eStcType_PcrPacing,
    eStcType_IpLivePlayback,
    eStcType_IpPullPlayback,
    eStcType_PvrPlayback
} eStcType;

typedef enum eKey
{
    eKey_Invalid,
    eKey_Select,
    eKey_UpArrow,
    eKey_DownArrow,
    eKey_LeftArrow,
    eKey_RightArrow,//5
    eKey_Backspace,
    eKey_Delete,
    eKey_ChannelUp,
    eKey_ChannelDown,
    eKey_VolumeUp,//10
    eKey_VolumeDown,
    eKey_Mute,
    eKey_Play,
    eKey_Stop,
    eKey_Pause,//15
    eKey_Rewind,
    eKey_FastForward,
    eKey_Record,
    eKey_Menu,
    eKey_Info,//20
    eKey_Exit,
    eKey_Dot,
    eKey_Enter,
    eKey_Last,
    eKey_Pip,//25
    eKey_Swap,
    eKey_JumpFwd,
    eKey_JumpRev,
    /* ascii codes */
    eKey_0,
    eKey_1,
    eKey_2,
    eKey_3,
    eKey_4,
    eKey_5,
    eKey_6,
    eKey_7,
    eKey_8,
    eKey_9,
    eKey_Max
} eKey;







/* print atlas error and return given error code */
#define ATLAS_ERROR(str, err) (BDBG_ERR(("ATLAS ERROR: %s - code:%d at %s: %d", (str), (err), __FILE__, __LINE__)), err)

/* print nexus error and return eRet_ExternalError code */
#define NEXUS_ERROR(str, err) (BDBG_ERR(("NEXUS ERROR: %s - code:%d at %s: %d", (str), (err), __FILE__, __LINE__)), eRet_ExternalError)

/* if nexus error, print error and return eRet_ExternalError.  otherwise return eRet_Ok */
#define CHECK_NEXUS_ERROR(str,nerr) \
        ((NEXUS_SUCCESS != (nerr)) ?  NEXUS_ERROR(str, nerr) : eRet_Ok)

/* if nexus error, print error then assert */
#define CHECK_NEXUS_ERROR_ASSERT(str,nerr) \
        do { \
            CHECK_NEXUS_ERROR((str), (nerr)); \
            BDBG_ASSERT(NEXUS_SUCCESS == (nerr)); \
        } while (0)

/* if nexus error, print error, set given err_var with atlas error code, and goto given label */
#define CHECK_NEXUS_ERROR_GOTO(err_str,err_var,nerr_code,label)                                                \
        do {                                                                                                   \
            if (NEXUS_SUCCESS != (nerr_code)) {                                                                \
                BDBG_ERR(("NEXUS ERROR: %s - code:%d at %s: %d", (err_str), (nerr_code), __FILE__, __LINE__)); \
                (err_var) = eRet_ExternalError;                                                                \
                goto label;                                                                                    \
            }                                                                                                  \
            else {                                                                                             \
                (err_var) = eRet_Ok;                                                                           \
            }                                                                                                  \
        } while(0)

/* if nexus error, print warning, set given err_var with atlas error code, and goto given label */
#define CHECK_NEXUS_WARN_GOTO(err_str,err_var,nerr_code,label)                                                 \
        do {                                                                                                   \
            if (NEXUS_SUCCESS != (nerr_code)) {                                                                \
                BDBG_WRN(("NEXUS ERROR: %s - code:%d at %s: %d", (err_str), (nerr_code), __FILE__, __LINE__)); \
                (err_var) = eRet_ExternalError;                                                                \
                goto label;                                                                                    \
            }                                                                                                  \
            else {                                                                                             \
                (err_var) = eRet_Ok;                                                                           \
            }                                                                                                  \
        } while(0)

/* if b_os error, print error, set given err_var with atlas error code, and goto given label */
#define CHECK_BOS_ERROR_GOTO(err_str,err_var,berr_code,label)                                                     \
        do {                                                                                                      \
            if (B_ERROR_SUCCESS != (berr_code)) {                                                                 \
                BDBG_ERR(("B_OS_LIB ERROR: %s - code:%d at %s: %d", (err_str), (berr_code), __FILE__, __LINE__)); \
                (err_var) = eRet_ExternalError;                                                                   \
                goto label;                                                                                       \
            }                                                                                                     \
            else {                                                                                                \
                (err_var) = eRet_Ok;                                                                              \
            }                                                                                                     \
        } while(0)

/* if atlas error, print error and continue */
#define CHECK_ERROR(err_str,err_code)                                                               \
        do {                                                                                                   \
            if (eRet_Ok != (err_code)) {                                                                       \
                BDBG_ERR(("ATLAS ERROR: %s - code:%d at %s: %d", (err_str), (err_code), __FILE__, __LINE__));  \
            }                                                                                                  \
        } while(0)

/* if atlas error, print error and goto given label */
#define CHECK_ERROR_GOTO(err_str,err_code,label)                                                               \
        do {                                                                                                   \
            if (eRet_Ok != (err_code)) {                                                                       \
                BDBG_ERR(("ATLAS ERROR: %s - code:%d at %s: %d", (err_str), (err_code), __FILE__, __LINE__));  \
                goto label;                                                                                    \
            }                                                                                                  \
        } while(0)

/* if ptr equals NULL, print error, set given err_code to err_var and goto given label */
#define CHECK_PTR_GOTO(err_str,ptr,err_var,err_code,label)                                                          \
        do {                                                                                                        \
            if (NULL == (ptr)) {                                                                                    \
                BDBG_ERR(("ATLAS ERROR: %s - NULL pointer at %s: %d", (err_str), __FILE__, __LINE__));              \
                (err_var) = (err_code);                                                                             \
                goto label;                                                                                         \
            }                                                                                                       \
        } while(0)

/* if ptr equals NULL, print error, set given err_code to err_var */
#define CHECK_PTR(err_str,ptr,err_var,err_code)                                                                     \
        do {                                                                                                        \
            if (NULL == (ptr)) {                                                                                    \
                BDBG_ERR(("ATLAS ERROR: %s - NULL pointer at %s: %d", (err_str), __FILE__, __LINE__));              \
                (err_var) = (err_code);                                                                             \
            }                                                                                                       \
        } while(0)

typedef enum eRet
{
    eRet_Ok = 0,
    eRet_OutOfMemory = 1,
    eRet_InvalidParameter = 2,
    eRet_NotSupported     = 3,
    eRet_NotAvailable     = 4,
    eRet_Busy             = 5,
    eRet_ExternalError    = 6,
    eRet_InvalidState     = 7,
    eRet_Timeout          = 8
} eRet;

int GetDib3000LockStatus(void);
void U322IP (char *IP, unsigned int u_ip);

#endif
