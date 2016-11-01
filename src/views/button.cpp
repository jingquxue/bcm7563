#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include "global.h"

BDBG_MODULE(kwt_button);

bwidget_t CreatButton (const char *strName, bwidget_t pParentWidget, MRect geometry, bwin_font_t font, bwin_t parentWin)
{
	bbutton_settings buttonSettings;
	bwidget_t _widget;
	//printf("button %s\n",strName);


	BDBG_ASSERT(NULL != pParentWidget);

	bbutton_get_default_settings (&buttonSettings);
	buttonSettings.label.widget.window.parent = (NULL == parentWin) ? bwidget_win(pParentWidget) : parentWin;
	buttonSettings.label.widget.window.x = geometry.x();
	buttonSettings.label.widget.window.y = geometry.y();
	buttonSettings.label.widget.window.rect.width = geometry.width();
	buttonSettings.label.widget.window.rect.height = geometry.height();
	buttonSettings.label.font = font;
	buttonSettings.label.bevel = 0;
	buttonSettings.label.bevel_color[bevel_eTop] = 0xFFAAAAAA;
	buttonSettings.label.bevel_color[bevel_eRight] = 0xFFDCDCDC;
	buttonSettings.label.bevel_color[bevel_eBottom] = 0xFFAAAAAA;
	buttonSettings.label.bevel_color[bevel_eLeft] = 0xFFDCDCDC;
	buttonSettings.label.text = NULL;
	buttonSettings.label.text_color = 0xFFFFFFFF;
	buttonSettings.label.text_justify_horiz = bwidget_justify_horiz_center;
	buttonSettings.label.text_justify_vert = bwidget_justify_vert_middle;
	buttonSettings.label.text_margin = 0;
	buttonSettings.label.background_color = BACKGROUND_COLOR;
	buttonSettings.label.focus_color = 0xFF310DF1;
	buttonSettings.click = NULL;
	buttonSettings.label.widget.data = NULL;
	_widget = bbutton_create (g_System.WidgetEngine, &buttonSettings);
	//printf("%08x\n",_widget);
	return _widget;

}

void SetButtonText (bwidget_t widget, const char *pText)
{
	bbutton_settings buttonSettings;

	bbutton_get (widget, &buttonSettings);
	buttonSettings.label.text = pText;
	bbutton_set (widget, &buttonSettings);
	bwin_repaint (bwidget_win(widget), NULL);
}

const char *GetButtonText (bwidget_t widget)
{
	bbutton_settings buttonSettings;
	bbutton_get (widget, &buttonSettings);
	return buttonSettings.label.text;
}

bwin_image_t _image;

eRet LoadButtonImage (bwidget_t button, const char *filename, bwin_image_render_mode renderMode)
{
	eRet ret = eRet_Ok;
	bbutton_settings buttonSettings;
	BDBG_ASSERT(NULL != filename);

	_image = bwin_image_load (g_System.BwinEngine, filename);
	CHECK_PTR_GOTO("unable to load bwin image", _image, ret, eRet_ExternalError, error);

	bbutton_get (button, &buttonSettings);
	buttonSettings.label.image = _image;
	buttonSettings.label.render_mode = renderMode;
	bbutton_set (button, &buttonSettings);

	bwin_repaint (bwidget_win(button), NULL);

error:
	return ret;
}


void SetButtonBackGroundColor(bwidget_t button, unsigned int color)
{
    bbutton_settings buttonSettings;

    bbutton_get(button, &buttonSettings);
    buttonSettings.label.background_color = color;
    bbutton_set(button, &buttonSettings);

    bwin_repaint(bwidget_win(button), NULL);
}






