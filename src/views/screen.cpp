


#include "bwidgets.h"
#include "mgeom.h"

#include "main.h"
#include "label.h"
#include "global.h"


#include <string.h>
#include <stdio.h>



void SetBackGroundParentWin(bwin_t parentWin)
{
    bwidget_settings bwidgetSettings;

    bwidget_get_settings(L_BackGround, &bwidgetSettings);
    bwidgetSettings.window.parent = parentWin;
    bwidget_set_settings(L_BackGround, &bwidgetSettings);

    bwin_set(bwidget_win(L_BackGround), &bwidgetSettings.window);
}


void CreatBackGroundScreen(const char * strName,bwidget_t  pParentWidget, bwin_t win)
{

    blabel_settings labelSettings;
   L_BackGround=CreatLabel(strName, pParentWidget, MRect(0, 0, 960,540), NULL, win);

    blabel_get(L_BackGround, &labelSettings);
    labelSettings.background_color            = 0x00000000;
    blabel_set(L_BackGround, &labelSettings);
    bwin_repaint(bwidget_win(L_BackGround), NULL);

/*
    blabel_get(label_2, &labelSettings);
    labelSettings.background_color            = 0xFFFF0000;
    blabel_set(label_2, &labelSettings);
    bwin_repaint(bwidget_win(label_2), NULL);
*/


}





void CloseBackGrounScreen(void)
{
    SetBackGroundParentWin(NULL);
	bwidget_destroy(L_BackGround);

}
