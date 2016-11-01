#include "bwidgets.h"
#include "mgeom.h"
#include "main.h"
#include "label.h"
#include "button.h"
#include "menu_main.h"
#include "dib3000.h"
#include "channellist.h"
#include "menu_info.h"
#include "graphics.h"
#include "menu_displaysettings.h"
#include "menu_videochannellist.h"
#include "menu_channeledit.h"
#include "global.h"
#include "channellist.h"
#include "menu_circlelabel.h"

void VideoChannelListShow (bool show);

bwidget_t video_channel_list_bg;
bwidget_t video_channel_list[8];
bwidget_t video_channel_picture[10];
unsigned char video_channel_list_focuse = 0;

static int page_selected_videolist=0;
static int max_page_videolist=0;

void UpdateChannelList ()
{
    int i, total;


	total = g_Channel.ChannelCount;

	printf ("total = %d\n", total);

    if(total <0)
    {
        perror("error");
    }
    else
    {
		if(total%8)
		{

            max_page_videolist =total/8+1;
		}
		else
		{
            max_page_videolist=total/8;
		}

		for(i=0;i<8;i++)
        {
            SetButtonText(video_channel_list[i], " ");
        }
		static char buf[32][100]={{0}};

        for(int j=page_selected_videolist*8; (j<page_selected_videolist*8+8)&&(j<total); j++)
        {
			if (g_Channel.ChannelList[j].BandWidth == 1500)
			{
				sprintf(buf[j],"%d-%dMHz-%0.1fMHz",j,g_Channel.ChannelList[j].Frequency/1000,
															g_Channel.ChannelList[j].BandWidth/1000.0);
			}
			else
			{
				sprintf(buf[j],"%d-%dMHz-%dMHz",j,g_Channel.ChannelList[j].Frequency/1000,
															g_Channel.ChannelList[j].BandWidth/1000);
			}
            SetButtonText(video_channel_list[j-page_selected_videolist*8], buf[j]);
        }
    }



}


void VideoChannelListInit ()
{
	video_channel_list_bg = CreatLocalLabel ("MenuMain_BackGround", L_BackGround, MRect (50,50,830,815), g_System.Font17,NULL);

	for (int i = 0; i < 8; i++)
	{
		video_channel_list[i] = CreatButton ("video_channel_list", video_channel_list_bg,
															MRect (270, 100 + i * 40, 320, 30), g_System.Font17, NULL);
		video_channel_picture[i] = CreatCirclepicture ("video_channel_picture", video_channel_list_bg,
															MRect (210, 100 + i * 40, 30, 30), g_System.Font17, NULL);
	}

	video_channel_picture[8]  =CreatLabel("video_channel_picture",   video_channel_list_bg,
																	MRect(70,10,120,30), g_System.big_menu, NULL);
	video_channel_picture[9]  =CreatLabel("video_channel_picture",   video_channel_list_bg,
																	MRect(95,40,180,30), g_System.small_menu, NULL);

	VideoChannelListShow (false);

	SetLabelText (video_channel_picture[8], "MENU");
	SetLabelText (video_channel_picture[9], "Channel List");

	SetLabelBackGroundColor (video_channel_picture[8], 0x85223284);
	SetLabelBackGroundColor (video_channel_picture[9], 0x85223284);

	UpdateChannelList ();
}

void VideoChannelListShow (bool show)
{
	int ret = 0;

	UpdateChannelList ();

	bwidget_show (video_channel_list_bg, show);
	for (int i = 0; i < 8; i++)
	{
		bwidget_show (video_channel_list[i], show);
	}
	if (show == true)
	{
		ret = bwidget_set_focus (video_channel_list[video_channel_list_focuse]);

		for (int i = 0; i < 8; i++)
		{
			SetButtonBackGroundColor (video_channel_list[i], UNFOCUS_COLOR);
			SetButtonBackGroundColor (video_channel_list[video_channel_list_focuse], FOCUS_COLOR);
		}
		for (int i = 0; i < 8; i++)
		{
			if (i == video_channel_list_focuse)
			{
				bwidget_show (video_channel_picture[i], true);
				continue;
			}
			bwidget_show (video_channel_picture[i], false);
		}

		printf ("ret = %d\n", ret);
	}
}

int MenuVideoChannelListKeyDown (unsigned int key_code)
{
	if (key_code == eKey_DownArrow)
	{
		video_channel_list_focuse++;
		if (video_channel_list_focuse == 8)
		{
			video_channel_list_focuse = 0;
		}

		bwidget_set_focus (video_channel_list[video_channel_list_focuse]);
		for (int i = 0; i < 8; i++)
		{
			if (i == video_channel_list_focuse)
			{
				bwidget_show (video_channel_picture[i], true);
				continue;
			}
			bwidget_show (video_channel_picture[i], false);
		}
	}
	else if (key_code == eKey_UpArrow)
	{
		if (video_channel_list_focuse == 0)
		{
			video_channel_list_focuse = 7;
		}
		else
		{
			video_channel_list_focuse--;
		}
		bwidget_set_focus (video_channel_list[video_channel_list_focuse]);
		for (int i = 0; i < 8; i++)
		{
			if (i == video_channel_list_focuse)
			{
				bwidget_show (video_channel_picture[i], true);
				continue;
			}
			bwidget_show (video_channel_picture[i], false);
		}
	}
	else if (key_code == eKey_Select)
	{
		DelChannel (video_channel_list_focuse + 8 * page_selected_videolist);
		VideoChannelListShow (true);
	}
	else if (key_code == eKey_Menu)
	{
		VideoChannelListShow (false);
		MenuMainShow (true);
		CurrentMenu = eMenu_Main;
	}
	else if ((key_code >= eKey_0) && (key_code <= eKey_9))
	{

	}
	else if (key_code == eKey_Exit)
	{
		VideoChannelListShow (false);
		ChannelEditShow (true);
		CurrentMenu = eMenu_ChannelEdit;
	}
	else if (key_code == eKey_LeftArrow)
	{
		printf("max_page=%d\n",max_page_videolist);
		if(page_selected_videolist==0)
		{
			page_selected_videolist=max_page_videolist-1;
		}
		else
		{
			page_selected_videolist--;
		}

		printf("page_selected=%d\n",page_selected_videolist);
		UpdateChannelList();
		bwidget_set_focus(video_channel_list[video_channel_list_focuse]);
	}
	else if (key_code == eKey_RightArrow)
	{
		printf("max_page=%d\n",max_page_videolist);

			page_selected_videolist++;
			if(page_selected_videolist==max_page_videolist)
			{
				page_selected_videolist=0;
			}



		printf("page_selected=%d\n",page_selected_videolist);
		UpdateChannelList();
		bwidget_set_focus(video_channel_list[video_channel_list_focuse]);
	}

	for (int i = 0; i < 8; i++)
	{
		SetButtonBackGroundColor (video_channel_list[i], UNFOCUS_COLOR);
		SetButtonBackGroundColor (video_channel_list[video_channel_list_focuse], FOCUS_COLOR);
	}

	return 0;
}

void CloseVideoChannelListMenu ()
{
	bwidget_destroy (video_channel_list_bg);
	for (int i = 0; i < 8; i++)
	{
		bwidget_destroy (video_channel_list[i]);
	}
	for (int i = 0; i < 10; i++)
	{
		bwidget_destroy (video_channel_picture[i]);
	}
}
