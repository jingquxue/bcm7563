#ifndef KWT_CIRCLELABEL_H
#define KWT_CIRCLELABEL_H

#include "bwidgets.h"
#include "mgeom.h"

#include "main.h"


#include <string.h>
#include <stdio.h>

bwidget_t CreatLocalLabel(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin);
bwidget_t CreatInfoLabel(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin);
bwidget_t CreatCirclepicture(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin);
bwidget_t Creatleftpicture(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin);
bwidget_t Creatrightpicture(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin);
bwidget_t CreatListLabel(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin);
bwidget_t CreatLine(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin);
#endif

