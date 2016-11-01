#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "global.h"
#include <string.h>
#include <stdio.h>

BDBG_MODULE(kwt_label);

bwidget_t CreatLabel (const char *strName, bwidget_t pParentWidget, MRect geometry, bwin_font_t font, bwin_t parentWin)
{
	blabel_settings labelSettings;
	bwidget_t _bwidget;

	//printf("%s\n",strName);

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
	labelSettings.text_justify_horiz = bwidget_justify_horiz_left;
	labelSettings.text_justify_vert = bwidget_justify_vert_middle;
	labelSettings.text_margin = 0;
	labelSettings.background_color = 0xFF294a63;
	labelSettings.widget.key_down = NULL;
	labelSettings.widget.data = NULL;
	_bwidget = blabel_create (g_System.WidgetEngine, &labelSettings);
	//printf("%08x\n",_bwidget);
	return _bwidget;
}

void SetLabelBackGroundColor (bwidget_t label, unsigned int color)
{
	blabel_settings labelSettings;

	blabel_get (label, &labelSettings);
	labelSettings.background_color = color;
	blabel_set (label, &labelSettings);

	bwin_repaint (bwidget_win(label), NULL);
}

void SetLabelText (bwidget_t label, const char *text)
{
	blabel_settings labelSettings;

	blabel_get (label, &labelSettings);
	labelSettings.text = text;
	blabel_set (label, &labelSettings);
	bwin_repaint (bwidget_win(label), NULL);
}

const char *GetLabelText (bwidget_t label)
{
	blabel_settings labelSettings;

	blabel_get (label, &labelSettings);
	return labelSettings.text;
}

eRet LoadLabelImage (bwidget_t label, const char *filename, bwin_image_render_mode renderMode)
{
	eRet ret = eRet_Ok;
	blabel_settings labelSettings;
	bwin_image_t image;

	BDBG_ASSERT(NULL != filename);

	image = bwin_image_load (g_System.BwinEngine, filename);
	CHECK_PTR_GOTO("unable to load bwin image", image, ret, eRet_ExternalError, error);

	blabel_get (label, &labelSettings);
	labelSettings.image = image;
	labelSettings.render_mode = renderMode;
	blabel_set (label, &labelSettings);

	bwin_repaint (bwidget_win(label), NULL);

error:
	return ret;
}

