#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "label.h"
#include "menu_channelsearch.h"
#include "menu_info.h"
#include "menu_main.h"
#include "menu_playback.h"
#include "button.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>





#include "global.h"
#include "menu_circlelabel.h"
#include "graphics.h"
#include "playback.h"

#define U_DISK 0
#define SD_CARD 1
#define PART0 2
#define PART1 3
#define PART2 4
#define PART3 5
#define PART4 6
#define PART5 7

#define NUM_FILES_TO_DISPLAY 8


void MenuFileBrowserShow (bool show);

static bwidget_t l_menuFileBrowserBg;
static bwidget_t l_menu_file[2];
static bwidget_t b_media[2];
static bwidget_t b_menuFileBrowser[NUM_FILES_TO_DISPLAY];
static bwidget_t l_menuFileType[NUM_FILES_TO_DISPLAY];
static int media_selected = 0;
static int page_selected = 0;
static int max_page = 0;
static int focuse = 0;
static char current_dir[512] = {0};
static char current_file[512] = {0};
static char root_dir[512] = {0};
bwin_image_t picture_test_file[8];
bwidget_t test_file[8];
bwin_image_t picture_test_folder[8];
bwidget_t test_folder[8];

void SetRootAndCurDir(void)
{
	if (media_selected == 0)
	{
		memset (root_dir, 0, 512);
		memset (current_dir, 0, 512);
		strcpy (root_dir, "/mnt/media/usb");
		strcpy (current_dir, "/mnt/media/usb");
	}
	else
	{
		memset (root_dir, 0, 512);
		memset (current_dir, 0, 512);
		strcpy (root_dir, "/mnt/media/sdcard");
		strcpy (current_dir, "/mnt/media/sdcard");
	}
}

void UpdateFileBrowser(void)
{
	struct dirent **namelist;
	int i;
	int total;

	total = scandir (current_dir, &namelist, 0, alphasort);
	if (total < 0)
	{
		perror ("scandir");
	}
	else
	{
		if (total % NUM_FILES_TO_DISPLAY)
		{
			max_page = total / NUM_FILES_TO_DISPLAY + 1;
		}
		else
		{
			max_page = total / NUM_FILES_TO_DISPLAY;
		}

		for (i = 0; i < NUM_FILES_TO_DISPLAY; i++)
		{
			SetButtonText (b_menuFileBrowser[i], " ");
			SetLabelText (l_menuFileType[i], " ");

			bwidget_show (test_file[i], false);
			bwidget_show (test_folder[i], false);
		}

		for (i = page_selected * 8; (i < page_selected * 8 + 8) && (i < total); i++)
		{
			SetButtonText (b_menuFileBrowser[i - page_selected * 8], (const char *) (namelist[i]->d_name));
			if (namelist[i]->d_type == 4)
			{
				SetLabelText (l_menuFileType[i - page_selected * 8], "D");
				bwidget_show (test_file[i - page_selected * 8], false);
				bwidget_show (test_folder[i - page_selected * 8], true);
			}
			else if (namelist[i]->d_type == 8)
			{
				SetLabelText (l_menuFileType[i - page_selected * 8], "F");
				bwidget_show (test_file[i - page_selected * 8], true);
				bwidget_show (test_folder[i - page_selected * 8], false);
			}
			else
			{
				SetLabelText (l_menuFileType[i - page_selected * 8], "F");
				bwidget_show (test_file[i - page_selected * 8], true);
				bwidget_show (test_folder[i - page_selected * 8], false);
			}
		}
	}
}

eRet loadImage_button_file (const char *filename, bwin_image_render_mode renderMode, bwidget_t widget, int i)
{
	eRet ret = eRet_Ok;
	bbutton_settings buttonSettings;
	BDBG_ASSERT(NULL != filename);
	if (NULL != picture_test_file[i])
	{
		bwin_image_close (picture_test_file[i]);
	}
	picture_test_file[i] = bwin_image_load (getWinEngine (), filename);
	bbutton_get (widget, &buttonSettings);
	buttonSettings.label.image = picture_test_file[i];
	buttonSettings.label.render_mode = renderMode;
	bbutton_set (widget, &buttonSettings);
	bwin_repaint (bwidget_win (widget), NULL);
	return ret;
}

eRet loadImage_button_folder (const char *filename, bwin_image_render_mode renderMode, bwidget_t widget, int i)
{
	eRet ret = eRet_Ok;
	bbutton_settings buttonSettings;
	BDBG_ASSERT(NULL != filename);
	if (NULL != picture_test_folder[i])
	{
		bwin_image_close (picture_test_folder[i]);
	}
	picture_test_folder[i] = bwin_image_load (getWinEngine (), filename);
	bbutton_get (widget, &buttonSettings);
	buttonSettings.label.image = picture_test_folder[i];
	buttonSettings.label.render_mode = renderMode;
	bbutton_set (widget, &buttonSettings);
	bwin_repaint (bwidget_win (widget), NULL);
	return ret;
}

void MenuFileBrowser(void)
{
	l_menuFileBrowserBg = CreatLocalLabel ("MenuMain_BackGround", L_BackGround,
																		MRect (50, 50, 830, 815), g_System.Font17, NULL);
	b_media[0] = CreatButton ("MenuBrowser_UDisk", l_menuFileBrowserBg,
																		MRect (320, 70, 80, 30), g_System.Font17, NULL);
	b_media[1] = CreatButton ("MenuBrowser_SD", l_menuFileBrowserBg,
																		MRect (450, 70, 90, 30), g_System.Font17, NULL);

	for (int i = 0; i < 8; i++)
	{
		b_menuFileBrowser[i] = CreatButton ("MenuBrowser_part", l_menuFileBrowserBg,
																MRect (230, 115 + i * 40, 420, 30), g_System.Font17, NULL);
	}

	for (int i = 0; i < 8; i++)
	{
		test_folder[i] = CreatButton ("test_folder", l_menuFileBrowserBg,
																MRect (180, 115 + i * 40, 30, 30), g_System.Font17, NULL);
		loadImage_button_folder ("images/folder.png", bwin_image_render_mode_tile, test_folder[i], i);
		test_file[i] = CreatButton ("test_file", l_menuFileBrowserBg,
																MRect (180, 115 + i * 40, 30, 30), g_System.Font17, NULL);
		loadImage_button_file ("images/document.png", bwin_image_render_mode_tile, test_file[i], i);
	}

	for (int i = 0; i < 8; i++)
	{
		l_menuFileType[i] = CreatLabel ("MenuBrowser_Filetype", l_menuFileBrowserBg,
																MRect (192, 115 + i * 40, 20, 30), g_System.Font10, NULL);
	}

	for (int i = 0; i < 8; i++)
	{
		bwidget_show (test_file[i], false);
		bwidget_show (test_folder[i], false);
	}
	l_menu_file[0] = CreatLabel ("l_menu_file", l_menuFileBrowserBg, MRect (70, 10, 120, 30), g_System.big_menu, NULL);
	l_menu_file[1] = CreatLabel ("l_menu_file", l_menuFileBrowserBg, MRect (95, 40, 120, 30), g_System.small_menu, NULL);
	SetButtonText (b_media[0], "U DISK");
	SetButtonText (b_media[1], "SD CARD");
	SetLabelText (l_menu_file[0], "MENU");
	SetLabelText (l_menu_file[1], "PlayBack");

	for (int i = 0; i < 8; i++)
	{
		SetLabelBackGroundColor (l_menuFileType[i], 0x85223284);
	}
	for (int i = 0; i < 2; i++)
	{
		SetLabelBackGroundColor (l_menu_file[i], 0x85223284);
	}

	media_selected=0;
	SetButtonBackGroundColor(b_media[media_selected], 0x8500FF00);
	bwidget_set_focus (b_menuFileBrowser[focuse]);
	SetRootAndCurDir ();
	UpdateFileBrowser ();
	MenuFileBrowserShow (false);
}

void FileOrDirClicked(void)
{
	const char *text = GetButtonText (b_menuFileBrowser[focuse]);
	const char *type = GetLabelText (l_menuFileType[focuse]);

	if ((strcmp (text, " ") != 0) && (text != NULL))
	{
		GetLabelText (l_menuFileType[focuse]);
		if (strcmp (type, "D") == 0)
		{
			char path[200]={0};
			memset(path,0,200);
			page_selected = 0;
			sprintf (current_dir, "%s/%s", current_dir, text);
			printf("current_dir=%s\n",current_dir);
			if(realpath(current_dir,path)!=NULL)
			{
				printf("path=%s\n",path);
				if(strcmp(path,"/mnt/media")==0)
				{
					if(media_selected==0)
					{
						memset(current_dir,0,512);
						strcpy(current_dir,"/mnt/media/usb");
					}
					else
					{
						memset(current_dir,0,512);
						strcpy(current_dir,"/mnt/media/sdcard");
					}
				}
				else
				{
						memset(current_dir,0,512);
						strcpy(current_dir,path);

				}


			}
			else
			{
					if(media_selected==0)
					{
						memset(current_dir,0,512);
						strcpy(current_dir,"/mnt/media/usb");
					}
					else
					{
						memset(current_dir,0,512);
						strcpy(current_dir,"/mnt/media/sdcard");
					}

			}

			focuse=0;
			for (int i = 0; i < 8; i++)
			{
				SetButtonBackGroundColor (b_menuFileBrowser[i], UNFOCUS_COLOR);
				SetButtonBackGroundColor (b_menuFileBrowser[focuse], FOCUS_COLOR);
			}

			UpdateFileBrowser ();
			bwidget_set_focus (b_menuFileBrowser[focuse]);

		}

		if (strcmp (type, "F") == 0)
		{
			if (strcmp (text + strlen (text) - 3, ".ts") == 0)
			{
				printf ("this is a ts file\n");
				memset (current_file, 0, sizeof (current_file));
				sprintf (current_file, "%s/%s", current_dir,text);
				CurrentMenu = eMenu_Playback;
				MenuFileBrowserShow (false);
				MenuPlaybackInitPar ();
				MenuPlaybackShow (true);
				g_Status.PlaybackStarted=1;
				StartPlayBack (current_file);
				MenuPlaybackUpdate (text);
				printf ("***%s***\n", current_file);
			}
			else
			{
				printf ("this is not a ts file\n");
			}
		}
	}
}

int MenuFileBrowserKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		focuse++;
		if (focuse == 8)
		{
			focuse = 0;
			page_selected++;
			if (page_selected == max_page)
			{
				page_selected = 0;
			}
			UpdateFileBrowser();
		}
		bwidget_set_focus (b_menuFileBrowser[focuse]);
	}
	else if (key_code == eKey_UpArrow)
	{
		if (focuse == 0)
		{
			focuse = 7;
			if (page_selected == 0)
			{
				page_selected = max_page - 1;
			}
			else
			{
				page_selected--;
			}
			UpdateFileBrowser ();
		}
		else
		{
			focuse--;
		}
		bwidget_set_focus (b_menuFileBrowser[focuse]);
	}
	else if (key_code == eKey_LeftArrow)
	{
		if(media_selected==1)
		{
			media_selected=0;
    		for (int i = 0; i < 2; i++)
    		{
    			SetButtonBackGroundColor (b_media[i], 0xaa868cf0);
    			SetButtonBackGroundColor (b_media[media_selected], 0x8500FF00);
    		}

    		media_selected = 0;
    		page_selected = 0;
    		SetRootAndCurDir ();
    		UpdateFileBrowser ();
    		focuse=0;
    		for (int i = 0; i < 8; i++)
    		{
    			SetButtonBackGroundColor (b_menuFileBrowser[i], 0xaa868cf0);
    			SetButtonBackGroundColor (b_menuFileBrowser[focuse], 0xa56b8cf0);
    		}
    		bwidget_set_focus (b_menuFileBrowser[focuse]);

		}

	}
	else if (key_code == eKey_RightArrow)
	{
		if(media_selected==0)
		{
			media_selected=1;
    		for (int i = 0; i < 2; i++)
    		{
    			SetButtonBackGroundColor (b_media[i], 0xaa868cf0);
    			SetButtonBackGroundColor (b_media[media_selected], 0x8500FF00);
    		}
    		media_selected = 1;
    		page_selected = 0;
    		SetRootAndCurDir ();
    		UpdateFileBrowser ();
    		focuse=0;
    		for (int i = 0; i < 8; i++)
    		{
    			SetButtonBackGroundColor (b_menuFileBrowser[i], UNFOCUS_COLOR);
    			SetButtonBackGroundColor (b_menuFileBrowser[focuse], FOCUS_COLOR);
    		}
    		bwidget_set_focus (b_menuFileBrowser[focuse]);
		}

	}
	else if (key_code == eKey_Select)
	{

		FileOrDirClicked();
	}
	else if (key_code == eKey_Menu)
	{
		MenuFileBrowserShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{

	}
	else if (key_code == eKey_Exit)
	{
		MenuFileBrowserShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	for (int i = 0; i < 8; i++)
	{
		SetButtonBackGroundColor (b_menuFileBrowser[i], UNFOCUS_COLOR);
		SetButtonBackGroundColor (b_menuFileBrowser[focuse], FOCUS_COLOR);
	}
	return 0;
}

void MenuFileBrowserShow (bool show)
{
	bwidget_show (l_menu_file[0], show);
	bwidget_show (l_menu_file[1], show);


	bwidget_show (b_media[0], show);
	bwidget_show (b_media[1], show);
	for (int i = 0; i < 8; i++)
	{
		bwidget_show (b_menuFileBrowser[i], show);
	}
	for (int i = 0; i < 8; i++)
	{
		bwidget_show (l_menuFileType[i], show);
	}
	bwidget_show (l_menuFileBrowserBg, show);

	if (show == true)
	{
		bwidget_set_focus (b_menuFileBrowser[focuse]);
		for (int i = 0; i < 8; i++)
		{
			SetButtonBackGroundColor (b_menuFileBrowser[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (b_menuFileBrowser[focuse], FOCUS_COLOR);
		}
		for (int i = 0; i < 2; i++)
		{
			SetButtonBackGroundColor (b_media[i], 0xaa868cf0);
			SetButtonBackGroundColor (b_media[media_selected], 0x8500FF00);
		}
	}
}

void CloseMenuFileBrowser(void)
{
	for (int i = 0; i < 8; i++)
	{
		bwidget_destroy (l_menuFileType[i]);
		bwidget_destroy (test_folder[i]);
		bwidget_destroy (test_file[i]);
		bwin_image_close (picture_test_file[i]);
		bwin_image_close (picture_test_folder[i]);
	}
	bwidget_destroy (l_menuFileBrowserBg);
	for (int i = 0; i < 8; i++)
	{
		bwidget_destroy (b_menuFileBrowser[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		bwidget_destroy (b_media[i]);
		bwidget_destroy (l_menu_file[i]);
	}
}

