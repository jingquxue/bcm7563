
#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "label.h"
#include "button.h"
#include "menu_main.h"
#include "dib3000.h"
#include "channellist.h"
#include "menu_main.h"
#include "menu_channelsearch.h"
#include "menu_info.h"
#include "menu_null.h"
#include "global.h"

void MenuRecordShow(bool show);

static bwidget_t l_record_bg;
static bwidget_t l_record;
static bwin_image_t picture_record=NULL;

eRet loadImage_label_record (bwidget_t widget,const char *filename,bwin_image_render_mode renderMode)
{
	eRet ret = eRet_Ok;
    blabel_settings labelSettings;

	BDBG_ASSERT (NULL != filename);
	if (NULL != picture_record)
	{
		bwin_image_close (picture_record);
	}
	picture_record = bwin_image_load (g_System.BwinEngine, filename);
	blabel_get (widget, &labelSettings);
	labelSettings.image = picture_record;
	labelSettings.render_mode = renderMode;
	blabel_set (widget, &labelSettings);
	bwin_repaint (bwidget_win (widget), NULL);

	return ret;
}

void MenuRecordInit(void)
{
	l_record_bg  =CreatLabel ("MenuRecord_BackGround", L_BackGround, MRect(100,10,50,50), g_System.Font10,NULL);
	SetLabelBackGroundColor(l_record_bg,0xFF000000);
	l_record  =CreatLabel("MenuRecord", l_record_bg, MRect(0,0,50,50), g_System.Font10,NULL);

	loadImage_label_record(l_record,"images/rec.png",bwin_image_render_mode_maximize_down);
	SetLabelBackGroundColor(l_record,0xFFFF0000);
    MenuRecordShow(false);
}

void MenuRecordShow(bool show)
{
    bwidget_show(l_record_bg,show);
    bwidget_show(l_record,show);

}
void CloseMenuRecord(void)
{
	printf("CloseMenuRecord\n");
	bwin_image_close (picture_record);
	bwidget_destroy(l_record_bg);
	bwidget_destroy(l_record);

}

