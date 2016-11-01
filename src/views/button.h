#ifndef KWT_BUTTON_H
#define KWT_BUTTON_H

#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

bwidget_t CreatButton (const char *strName, bwidget_t pParentWidget, MRect geometry, bwin_font_t font, bwin_t parentWin);
void SetButtonText (bwidget_t widget, const char *pText);
const char *GetButtonText (bwidget_t widget);
eRet LoadButtonImage (bwidget_t button, const char *filename, bwin_image_render_mode renderMode);
void SetButtonBackGroundColor(bwidget_t button, unsigned int color);

#endif

