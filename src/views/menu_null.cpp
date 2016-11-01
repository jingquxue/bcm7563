
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
#include "menu_volume.h"


#include "global.h"



void MenuNullInit(void)
{

}



void MenuNullShow(bool show)
{


}


int MenuNullKeyDown(unsigned int  key_code)
{
	int i=0;

	if(key_code==eKey_DownArrow)
	{
	}
	else if(key_code==eKey_UpArrow)
	{
	}
	else if(key_code==eKey_Select)
	{

	}
	else if(key_code==eKey_Menu)
	{
		MenuNullShow(false);
		MenuMainShow(true);
		CurrentMenu=eMenu_Main;
	}
	else if((key_code>=eKey_0)&&(key_code<=eKey_9))
	{
	}
	else if(key_code==eKey_Exit)
	{

	}
	else if(key_code==eKey_Info)
	{
		MenuNullShow(false);
		MenuInfoShow(true);
		CurrentMenu=eMenu_Info;
	}
	else if(key_code==eKey_LeftArrow)
	{
		SetVolumeLevel(0);
		MenuVolumeShow(true);
		g_Status.MenuVolumeDisplayed=1;

	}
	else if(key_code==eKey_RightArrow)
	{
		SetVolumeLevel(1);
		MenuVolumeShow(true);
		g_Status.MenuVolumeDisplayed=1;
	}


}







