


#include "bwidgets.h"
#include "mgeom.h"

#include "main.h"
#include "global.h"
#include "label.h"
#include "audio_decode.h"

#include <string.h>
#include <stdio.h>




static bool isRangeIndicatorVisible=false;
static unsigned RangeIndicatorSize=1;
static bool isProgressVisible=false;
static unsigned short int volume_level=0;

static bwidget_t l_volume_back;
static bwidget_t l_volume;




/* override default label draw routine to add progress option */
void progress_bwidget_draw(bwidget_t widget, const bwin_rect *cliprect)
{
    bwin_t win = bwidget_win(widget);
    bwin_settings win_settings;
    int right, bottom;
    blabel_settings * label_settings;
    blabel_settings labelSettings;
    bool down = false;

    blabel_get(widget, &labelSettings);
    label_settings = &labelSettings;


    /* TODO: for efficieny, we may want bwidgets to have access to bwin private structures. */
    bwin_get(win, &win_settings);

    /* only draw if something's within the cliprect */
    if (cliprect && !BWIN_RECT_ISINTERSECTING(&win_settings.rect, cliprect)) {
        return;
    }

    right = win_settings.rect.x + win_settings.rect.width;
    bottom = win_settings.rect.y + win_settings.rect.height;

    if (label_settings->image) {
        bwin_image_render(win, label_settings->image, label_settings->render_mode, &win_settings.rect, NULL, cliprect);
    }
    else {
        bwin_fill_rect(win, &win_settings.rect, label_settings->background_color, cliprect);

        if (label_settings->bevel) {
            int i;
            /* top */
            for (i=0;i<label_settings->bevel;i++) {
                bwin_draw_line(win, i, i, win_settings.rect.width-i, i,
                    label_settings->bevel_color[down?2:0], cliprect);
            }
            /* right */
            for (i=0;i<label_settings->bevel;i++) {
                bwin_draw_line(win, right-i-1, i, right-i-1, win_settings.rect.height-i,
                    label_settings->bevel_color[down?3:1], cliprect);
            }
            /* bottom */
            for (i=0;i<label_settings->bevel;i++) {
                bwin_draw_line(win, i, bottom-i-1, win_settings.rect.width-i, bottom-i-1,
                    label_settings->bevel_color[down?0:2], cliprect);
            }
            /* left */
            for (i=0;i<label_settings->bevel;i++) {
                bwin_draw_line(win, i, i, i, win_settings.rect.height-i,
                    label_settings->bevel_color[down?1:3], cliprect);
            }
        }
    }

    /* draw range indicator if necessary */
    if (true == isRangeIndicatorVisible)
    {
        bwin_rect rectProgress = win_settings.rect;
        int spacer = label_settings->bevel + 1;

        rectProgress.x = spacer;
        rectProgress.y = rectProgress.y + rectProgress.height / 2;
        rectProgress.width = rectProgress.width - (spacer * 2);
        rectProgress.height = RangeIndicatorSize;

        /*if (NULL != pProgress->getProgressImage())
	    {
            bwin_image_render(win, pProgress->getProgressImage(), pProgress->getProgressRenderMode(), &rectProgress, NULL, cliprect);
        }
        else
        */
		{
            bwin_fill_rect(win, &rectProgress, 0xffCCCCCC, cliprect);
        }
    }

    /* draw progress if necessary*/
    if (true == isProgressVisible)
	{
        uint16_t level;
        bwin_rect rectProgress = win_settings.rect;
        int spacer = label_settings->bevel + 1;

        rectProgress.x = spacer;
        rectProgress.y = spacer;
        rectProgress.width = rectProgress.width - (spacer * 2);
        rectProgress.height = rectProgress.height - (spacer * 2);

        /* adjust progress rect width based on actual level */
        level = volume_level;
        rectProgress.width = rectProgress.width * level / (uint16_t)-1;

        /*
        if (NULL != pProgress->getProgressImage())
		{
            bwin_image_render(win, pProgress->getProgressImage(), pProgress->getProgressRenderMode(), &rectProgress, NULL, cliprect);
        }
        else
        */
		{
            bwin_fill_rect(win, &rectProgress, 0xFF80C42F, cliprect);
        }
    }

    if (widget == bwidget_get_focus(g_System.WidgetEngine))
	{
        /* draw focus */
        int i;
        for (i=0;i<2;i++) {
            bwin_draw_line(win, i, i, win_settings.rect.width-i, i,
                label_settings->focus_color, cliprect);
            bwin_draw_line(win, right-i-1, i, right-i-1, win_settings.rect.height-i,
                label_settings->focus_color, cliprect);
            bwin_draw_line(win, i, bottom-i-1, win_settings.rect.width-i, bottom-i-1,
                label_settings->focus_color, cliprect);
            bwin_draw_line(win, i, i, i, win_settings.rect.height-i,
                label_settings->focus_color, cliprect);
        }
    }

    if (label_settings->text) {
        int width, height, base, x, y;
        bwin_measure_text(label_settings->font, label_settings->text, -1,
            &width, &height, &base);

        /* calculate vertical justification */
        y = 0;
        if (label_settings->text_justify_vert == bwidget_justify_vert_middle) {
            y = (win_settings.rect.height - height) / 2;
        }
        else if (label_settings->text_justify_vert == bwidget_justify_vert_bottom) {
            y += label_settings->text_margin;
        }
        else { /* bwidget_justify_vert_top */
            y = win_settings.rect.height - height;
            y -= label_settings->text_margin;
        }

        /* calculate horizontal justification */
        x = 0;
        if (label_settings->text_justify_horiz == bwidget_justify_horiz_center) {
            x = (win_settings.rect.width - width) / 2;
        }
        else if (label_settings->text_justify_horiz == bwidget_justify_horiz_right) {
            x = win_settings.rect.width - width;
            x -= label_settings->text_margin;
            x -= label_settings->bevel;
        }
        else { /* bwidget_justify_horiz_left */
            x += label_settings->text_margin;
            x += label_settings->bevel;
        }

        bwin_draw_text(win, label_settings->font, x, y,
            label_settings->text, -1, label_settings->text_color,
            cliprect);
    }
}








bwidget_t CreatProgress (const char *strName, bwidget_t pParentWidget, MRect geometry, bwin_font_t font, bwin_t parentWin)
{
	blabel_settings labelSettings;
	bwidget_t _bwidget;

	BDBG_ASSERT((NULL != pParentWidget) || (NULL != parentWin));

	blabel_get_default_settings (&labelSettings);
	if (NULL == parentWin)
	{
		labelSettings.widget.window.parent = bwidget_win(pParentWidget);
	}
	else
	{
		labelSettings.widget.window.parent = parentWin;
	}
	labelSettings.widget.window.x = geometry.x();
	labelSettings.widget.window.y = geometry.y();
	labelSettings.widget.window.rect.width = geometry.width();
	labelSettings.widget.window.rect.height = geometry.height();
	labelSettings.font = font;
	labelSettings.bevel = 0;
	labelSettings.bevel_color[bevel_eTop] = 0xFFAAAAAA;
	labelSettings.bevel_color[bevel_eRight] = 0xFFAAAAAA;
	labelSettings.bevel_color[bevel_eBottom] = 0xFFAAAAAA;
	labelSettings.bevel_color[bevel_eLeft] = 0xFFAAAAAA;
	labelSettings.text = NULL;
	labelSettings.text_color = 0xFFFFFFFF;
	labelSettings.text_justify_horiz = bwidget_justify_horiz_center;
	labelSettings.text_justify_vert = bwidget_justify_vert_middle;
	labelSettings.text_margin = 0;
	labelSettings.background_color = 0xFF294a63;
	labelSettings.widget.key_down = NULL;
	labelSettings.widget.data = NULL;
    labelSettings.widget.draw = progress_bwidget_draw;
	_bwidget = blabel_create (g_System.WidgetEngine, &labelSettings);
	return _bwidget;
}


void MenuVolumeShow(bool show);


void MenuVolumeInit(void)
{

	l_volume_back = CreatLabel ("MenuVolume_BackGround", L_BackGround, MRect (200,20,560,30), g_System.Font17,NULL);
	l_volume   =CreatProgress ("volume", l_volume_back, MRect(0,0,560,30), g_System.Font17, NULL);
	MenuVolumeShow(false);
}



void MenuVolumeShow(bool show)
{

    bwidget_show(l_volume_back,show);
    bwidget_show(l_volume,show);
}

void CloseMenuVolume(void)
{
	bwidget_destroy(l_volume_back);
	bwidget_destroy(l_volume);
}



void SetKwtVolume(void)
{
	uint32_t vol=0x800000;
	switch(g_Config.Volume)
	{
    case   0:vol=0;break;
	case   5:vol= 0x80000;break;
    case  10:vol=0x100000;break;
	case  15:vol=0x200000;break;
    case  20:vol=0x300000;break;
	case  25:vol=0x400000;break;
    case  30:vol=0x480000;break;
	case  35:vol=0x500000;break;
    case  40:vol=0x600000;break;
	case  45:vol=0x700000;break;
    case  50:vol=0x800000;break;
	case  55:vol=0x900000;break;
    case  60:vol=0xa00000;break;
	case  65:vol=0xb00000;break;
    case  70:vol=0xc00000;break;
	case  75:vol=0xd00000;break;
    case  80:vol=0xe00000;break;
	case  85:vol=0xf00000;break;
    case  90:vol=0x1000000;break;
	case  95:vol=0x1100000;break;
    case 100:vol=0x1200000;break;
	}

	SetSimpleAudioDecodeVolume(vol);
}






void SetVolumeLevel(bool addordel)
{


	if(addordel==1)//add
	{
		if(g_Config.Volume<96)
		{
			g_Config.Volume=g_Config.Volume+5;
		}
		else
		{
			g_Config.Volume=100;
		}
	}
	else//del
	{
		if(g_Config.Volume>4)
		{
			g_Config.Volume=g_Config.Volume-5;
		}
		else
		{
			g_Config.Volume=0;
		}
	}
    SetKwtVolume();
	volume_level=g_Config.Volume*65535/100;
	isProgressVisible=true;
}







