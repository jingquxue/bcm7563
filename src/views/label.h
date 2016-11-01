#ifndef KWT_LABEL_H
#define KWT_LABEL_H

#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

bwidget_t CreatLabel (const char *strName, bwidget_t pParentWidget, MRect geometry, bwin_font_t font, bwin_t parentWin);
void SetLabelBackGroundColor (bwidget_t label, unsigned int color);
void SetLabelText (bwidget_t label, const char *text);
const char *GetLabelText (bwidget_t label);
eRet LoadLabelImage (bwidget_t label, const char *filename, bwin_image_render_mode renderMode);
#endif

