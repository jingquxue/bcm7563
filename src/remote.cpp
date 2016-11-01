#include <stdio.h>
#include "b_os_lib.h"
#include "nexus_base_types.h"
#include "nexus_ir_input.h"
#include "bwidgets.h"
#include "main.h"
#include "menu_main.h"
#include "menu_channelsearch.h"
#include "menu_info.h"
#include "menu_null.h"
#include "menu_fileBrowser.h"
#include "menu_playback.h"
#include "menu_record.h"
#include "record.h"
#include "global.h"
#include "menu_displaysettings.h"
#include "menu_channeledit.h"
#include "menu_videochannellist.h"
#include "menu_generalsettings.h"
#include "menu_systeminfo.h"
#include "menu_datetimeset.h"
#include "menu_tcpipconfiguration.h"
#include "menu_networkconfig.h"
#include "menu_descramblingconfig.h"
#include "menu_duplexaudio.h"
#include "menu_record.h"
#include "menu_recordSettings.h"


static NEXUS_IrInputHandle g_IrInput;
static bool repeat_flag;
static unsigned int tmp_code;




eRet GetIREvent(void);

BDBG_MODULE(kwt_remote);

int GlobalKeyDown (unsigned int key_code)
{
	eRet ret = eRet_Ok;
	if (key_code == bwidget_key_record)
	{
		if (g_System.RecFile == NULL)
		{
			ret = RecordStart ();
			if (ret == eRet_Ok)
			{
				MenuRecordShow (true);
			}
		}
		else
		{
			g_System.ManulStopRec = 1;
			MenuRecordShow (false);
			RecordStop ();
		}
	}
	return 0;
}

static const bwidget_key eKey2bwidgets[eKey_Max] =
{
	bwidget_key_invalid,
	bwidget_key_select,
	bwidget_key_up,
	bwidget_key_down,
	bwidget_key_left,
	bwidget_key_right,
	bwidget_key_backspace,
	bwidget_key_delete,
	bwidget_key_chup,
	bwidget_key_chdown,
	bwidget_key_volup,
	bwidget_key_voldown,
	bwidget_key_mute,
	bwidget_key_play,
	bwidget_key_stop,
	bwidget_key_pause,
	bwidget_key_rewind,
	bwidget_key_fastforward,
	bwidget_key_record,
	bwidget_key_menu,
	bwidget_key_info,
	bwidget_key_exit,
	bwidget_key_dot,
	bwidget_key_enter,
	bwidget_key_last,
	bwidget_key_pip,
	bwidget_key_swap,
	bwidget_key_jumpforward,
	bwidget_key_jumpreverse,
	bwidget_key_0,
	bwidget_key_1,
	bwidget_key_2,
	bwidget_key_3,
	bwidget_key_4,
	bwidget_key_5,
	bwidget_key_6,
	bwidget_key_7,
	bwidget_key_8,
	bwidget_key_9
};

static void BwinRemoteCallback (bwidget_engine_t engine, bwin_io_handle handle)
{
	eRet ret = eRet_Ok;
	int berror = 0;
	BSTD_UNUSED(engine);
	BSTD_UNUSED(handle);
	ret = GetIREvent ();
	if (eRet_Ok == ret)
	{
		BDBG_MSG(("Remote event code: %#x", tmp_code));
		if (tmp_code == bwidget_key_record)
		{
			GlobalKeyDown (tmp_code);
			return ;
		}

		switch (CurrentMenu)
		{
			case eMenu_Info:
			{
				MenuInfoKeyDown (tmp_code);
			}
			break;
			case eMenu_NULL:
			{
				MenuNullKeyDown (tmp_code);
			}
			break;
			case eMenu_Main:
			{
				berror = MenuMainKeyDown (tmp_code);
			}
			break;
			case eMenu_ChannelSearch:
			{
				berror = MenuChannelSearchKeyDown (tmp_code);
			}
			break;
			case eMenu_FileBrowser:
			{
				berror = MenuFileBrowserKeyDown (tmp_code);
			}
			break;
			case eMenu_Playback:
			{
				berror = MenuPlaybackKeyDown (tmp_code);
			}
			break;
			case eMenu_DisplaySettings:
			{
				berror = MenuDisplaySettingsKeyDown (tmp_code);
			}
			break;
			case eMenu_ChannelEdit:
			{
				berror = MenuChannelEditKeyDown (tmp_code);
			}
			break;
			case eMenu_VideoChannelList:
			{
				berror = MenuVideoChannelListKeyDown (tmp_code);
			}
			break;
			case eMenu_GeneralSettings:
			{
				berror = MenuGeneralSettingsKeyDown (tmp_code);
			}
			break;
			case eMenu_SystemInfo:
			{
				berror = MenuSystemInfoKeyDown (tmp_code);
			}
			break;
			case eMenu_DateTimeSet:
			{
				berror = MenuDateTimeSetKeyDown (tmp_code);
			}
			break;
			case eMenu_TCPIPConfiguration:
			{
				berror = MenuTCPIPConfigurationKeyDown (tmp_code);
			}
			break;
			case eMenu_NetworkConfg:
			{
				berror = MenuNetworkConfigKeyDown (tmp_code);
			}
			break;
			case eMenu_DescramblingConfig:
			{
				berror = MenuDescramblingKeyDown (tmp_code);
			}
			break;
			case eMenu_DuplexAudio:
			{
				berror = MenuDuplexAudioKeyDown (tmp_code);
			}
			break;
			case eMenu_RecordConfig:
			{
				berror = MenuRecordSettingsConfigKeyDown (tmp_code);
			}
			break;
			default:
			{
				berror = GlobalKeyDown (tmp_code);
			}
			break;
		}

		if (-1 == berror)
		{
			BDBG_MSG(("bwidgets did not consume key event so pass on to registered observers"));
		}
	}
}

#define IR_REPEAT_THRESHOLD 100 /* millisecs */

static void NexusIrCallback (void *context, int param)
{
	static B_Time lastTime = B_TIME_ZERO;
	B_Time currentTime = B_TIME_ZERO;
	long diffTime = 0;
	BSTD_UNUSED(param);
	BSTD_UNUSED(context);
	B_Time_Get (&currentTime);
	diffTime = B_Time_Diff(&currentTime, &lastTime);
	lastTime = currentTime;

	if (IR_REPEAT_THRESHOLD < diffTime)
	{
		bwidget_trigger_io (g_System.WidgetEngine, (bwin_io_handle) IR_HANDLER);
	}
	else
	{
		NEXUS_IrInput_FlushEvents (g_IrInput);
	}
}




eRet OpenIrRemote(void)
{
	NEXUS_IrInputSettings settings;
	eRet ret = eRet_Ok;
	NEXUS_IrInput_GetDefaultSettings (&settings);
	settings.mode = NEXUS_IrInputMode_eCirNec;
	settings.dataReady.callback = NexusIrCallback;
	settings.dataReady.context = NULL;
	g_IrInput = NEXUS_IrInput_Open (0, &settings);
	CHECK_PTR_GOTO("nexus ir input open failed!", g_IrInput, ret, eRet_ExternalError, error);
	bwidget_add_io_handle (g_System.WidgetEngine, (bwin_io_handle) IR_HANDLER, BwinRemoteCallback);
	return ret;
error:
	return ret;
}

#define INVALID 0xFFFFFFFF

struct g_IrRemoteCodes
{
	eKey key;
	uint32_t codeBroadcom;
}

g_IrRemoteCodes[] =
{
	{eKey_Invalid, INVALID},
	{eKey_Select, 0xf807bf00},
	{eKey_UpArrow, 0xf906bf00},
	{eKey_DownArrow, 0xbb44bf00},
	{eKey_LeftArrow, 0xb847bf00},
	{eKey_RightArrow, 0xbf40bf00},
	{eKey_Backspace, INVALID},
	{eKey_Delete, INVALID},
	{eKey_ChannelUp, INVALID},
	{eKey_ChannelDown, INVALID},
	{eKey_VolumeUp, INVALID},
	{eKey_VolumeDown, INVALID},
	{eKey_Mute, 0xb946bf00},
	{eKey_Play, 0xf20dbf00},
	{eKey_Stop, 0xf10ebf00},
	{eKey_Pause, 0xea15bf00},
	{eKey_Rewind, 0xf708bf00},
	{eKey_FastForward, 0xb54abf00},
	{eKey_Record, 0xe11ebf00},
	{eKey_Menu, 0xfd02bf00},
	{eKey_Info, 0xe51abf00},
	{eKey_Exit, 0xaf50bf00},
	{eKey_Dot, 0xac53bf00},
	{eKey_Enter, INVALID},
	{eKey_Last, INVALID},
	{eKey_Pip, INVALID},
	{eKey_Swap, INVALID},
	{eKey_JumpFwd, INVALID},
	{eKey_JumpRev, INVALID},
	{eKey_0, 0xaa55bf00},
	{eKey_1, 0xf609bf00},
	{eKey_2, 0xe21dbf00},
	{eKey_3, 0xe01fbf00},
	{eKey_4, 0xe619bf00},
	{eKey_5, 0xe41bbf00},
	{eKey_6, 0xee11bf00},
	{eKey_7, 0xe817bf00},
	{eKey_8, 0xed12bf00},
	{eKey_9, 0xa659bf00},
	{eKey_Max, INVALID}
};

eKey convertRemoteCode (uint32_t code)
{
	eKey key = eKey_Invalid;
	unsigned i = 0;

	for (i = 0; i < sizeof (g_IrRemoteCodes) / sizeof (g_IrRemoteCodes[0]); i++)
	{
		if (code == g_IrRemoteCodes[i].codeBroadcom)
		{
			key = g_IrRemoteCodes[i].key;
			break;
		}
	}
	return key;
}

eRet GetIREvent(void)
{
	eRet ret = eRet_NotAvailable;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	NEXUS_IrInputEvent irEvent;
	unsigned num;
	bool overflow;

	nerror = NEXUS_IrInput_GetEvents (g_IrInput, &irEvent, 1, &num, &overflow);
	NEXUS_IrInput_FlushEvents (g_IrInput);

	if (!nerror && num)
	{
		eKey key;
		BDBG_MSG(("ir event: %#x repeat:%d", irEvent.code, irEvent.repeat));
		repeat_flag = irEvent.repeat;

		key = convertRemoteCode (irEvent.code);
		if ((eKey_Invalid != key) && (eKey_Max != key))
		{
			tmp_code = key;
		}
		else
		{
			tmp_code = eKey_Invalid;
		}

		switch (key)
		{
			case eKey_Select:
				BDBG_MSG(("select key"));
				break;
			case eKey_UpArrow:
				BDBG_MSG(("up key"));
				break;
			case eKey_DownArrow:
				BDBG_MSG(("down key"));
				break;
			case eKey_LeftArrow:
				BDBG_MSG(("left key"));
				break;
			case eKey_RightArrow:
				BDBG_MSG(("right key"));
				break;
			case eKey_Backspace:
				BDBG_MSG(("backspace key"));
				break;
			case eKey_Delete:
				BDBG_MSG(("delete key"));
				break;
			case eKey_ChannelUp:
				BDBG_MSG(("channel up key"));
				break;
			case eKey_ChannelDown:
				BDBG_MSG(("channel down key"));
				break;
			case eKey_Play:
				BDBG_MSG(("Play key"));
				break;
			case eKey_Stop:
				BDBG_MSG(("Stop key"));
				break;
			case eKey_Pause:
				BDBG_MSG(("Pause key"));
				break;
			case eKey_Rewind:
				BDBG_MSG(("Rewind key"));
				break;
			case eKey_FastForward:
				BDBG_MSG(("Fast Forward key"));
				break;
			case eKey_Record:
				BDBG_MSG(("Record key"));
				break;
			case eKey_Menu:
				BDBG_MSG(("menu key"));
				break;
			case eKey_Info:
				BDBG_MSG(("info key"));
				break;
			case eKey_Exit:
				BDBG_MSG(("exit key"));
				break;
			case eKey_Dot:
				BDBG_MSG(("dot key"));
				break;
			case eKey_Enter:
				BDBG_MSG(("enter key"));
				break;
			case eKey_0:
				BDBG_MSG(("0 key"));
				break;
			case eKey_1:
				BDBG_MSG(("1 key"));
				break;
			case eKey_2:
				BDBG_MSG(("2 key"));
				break;
			case eKey_3:
				BDBG_MSG(("3 key"));
				break;
			case eKey_4:
				BDBG_MSG(("4 key"));
				break;
			case eKey_5:
				BDBG_MSG(("5 key"));
				break;
			case eKey_6:
				BDBG_MSG(("6 key"));
				break;
			case eKey_7:
				BDBG_MSG(("7 key"));
				break;
			case eKey_8:
				BDBG_MSG(("8 key"));
				break;
			case eKey_9:
				BDBG_MSG(("9 key"));
				break;
			case eKey_Invalid:
			case eKey_Max:
			default:
				BDBG_MSG(("unhandled remote code 0x%0x", irEvent.code));
				break;
		}
		ret = eRet_Ok;
	}

	return ret;
}

void CloseIrRemote (void)
{
	if (NULL != g_System.WidgetEngine)
	{
		bwidget_remove_io_handle (g_System.WidgetEngine, (bwin_io_handle) IR_HANDLER);
	}
	if (NULL != g_IrInput)
	{
		NEXUS_IrInput_Close (g_IrInput);
		g_IrInput = NULL;
	}
}

