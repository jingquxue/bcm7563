#include "bwidgets.h"
#include "mgeom.h"

#include "main.h"
#include "global.h"

#include <string.h>
#include <stdio.h>

#define COLORSETTINGS 0x85223284
#define COLOR_CIRCLE 0xFFFF0000
#define COLOR_LIST 0xaa868cf0

void grid_bwidget_draw_line (bwidget_t widget, const bwin_rect *cliprect)
{
	bwin_t win = bwidget_win (widget);
	bwin_settings win_settings;
	blabel_settings *label_settings;
	blabel_settings labelSettings;

	blabel_get (widget, &labelSettings);
	label_settings = &labelSettings;

	bwin_get (win, &win_settings);

	if (cliprect && !BWIN_RECT_ISINTERSECTING (&win_settings.rect, cliprect))
	{
		return;
	}
	bwin_draw_line (win, win_settings.rect.x, win_settings.rect.height / 2,
						win_settings.rect.width, win_settings.rect.height / 2,
						COLOR_CIRCLE,cliprect);
	/*bwin_draw_line (win, win_settings.rect.x, win_settings.rect.height / 2+1,
						win_settings.rect.width, win_settings.rect.height / 2+1,
						COLOR_CIRCLE,cliprect);*/

}

void grid_bwidget_draw_list_label (bwidget_t widget, const bwin_rect *cliprect)
{
	bwin_t win = bwidget_win (widget);
	bwin_settings win_settings;
	blabel_settings *label_settings;
	blabel_settings labelSettings;

	blabel_get (widget, &labelSettings);
	label_settings = &labelSettings;

	bwin_get (win, &win_settings);

	if (cliprect && !BWIN_RECT_ISINTERSECTING (&win_settings.rect, cliprect))
	{
		return;
	}
		for (int i = 1; i < 30; i++)
		{
			bwin_draw_line (win, win_settings.rect.x+31, win_settings.rect.height / 2-i,
						win_settings.rect.width, win_settings.rect.height / 2-i,
						COLORSETTINGS,cliprect);
		}
		{
			bwin_draw_line (win, win_settings.rect.x+31, win_settings.rect.height / 2-30,
						win_settings.rect.width, win_settings.rect.height / 2-30,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.1, win_settings.rect.height / 2-30-1,
						win_settings.rect.width-0.1, win_settings.rect.height / 2-30-1,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.2, win_settings.rect.height / 2-30-2,
						win_settings.rect.width-0.2, win_settings.rect.height / 2-30-2,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.5, win_settings.rect.height / 2-30-3,
						win_settings.rect.width-0.5, win_settings.rect.height / 2-30-3,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.1, win_settings.rect.height / 2-30-4,
						win_settings.rect.width-1.1, win_settings.rect.height / 2-30-4,
						COLORSETTINGS,cliprect);

			bwin_draw_line (win, win_settings.rect.x+31+1.9, win_settings.rect.height / 2-30-5,
						win_settings.rect.width-1.9, win_settings.rect.height / 2-30-5,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.8, win_settings.rect.height / 2-30-6,
						win_settings.rect.width-2.8, win_settings.rect.height / 2-30-6,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+4.1, win_settings.rect.height / 2-30-7,
						win_settings.rect.width-4.1, win_settings.rect.height / 2-30-7,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+5.9, win_settings.rect.height / 2-30-8,
						win_settings.rect.width-5.9, win_settings.rect.height / 2-30-8,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+9.8, win_settings.rect.height / 2-30-9,
						win_settings.rect.width-9.8, win_settings.rect.height / 2-30-9,
						COLORSETTINGS,cliprect);
		}

		{
			bwin_draw_line (win, win_settings.rect.x+31, win_settings.rect.height / 2,
						win_settings.rect.width, win_settings.rect.height / 2,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.1, win_settings.rect.height / 2+1,
						win_settings.rect.width-0.1, win_settings.rect.height / 2+1,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.2, win_settings.rect.height / 2+2,
						win_settings.rect.width-0.2, win_settings.rect.height / 2+2,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.5, win_settings.rect.height / 2+3,
						win_settings.rect.width-0.5, win_settings.rect.height / 2+3,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.1, win_settings.rect.height / 2+4,
						win_settings.rect.width-1.1, win_settings.rect.height / 2+4,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.9, win_settings.rect.height / 2+5,
						win_settings.rect.width-1.9, win_settings.rect.height / 2+5,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.8, win_settings.rect.height / 2+6,
						win_settings.rect.width-2.8, win_settings.rect.height / 2+6,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+4.1, win_settings.rect.height / 2+7,
						win_settings.rect.width-4.1, win_settings.rect.height / 2+7,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+5.9, win_settings.rect.height / 2+8,
						win_settings.rect.width-5.9, win_settings.rect.height / 2+8,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+9.8, win_settings.rect.height / 2+9,
						win_settings.rect.width-9.8, win_settings.rect.height / 2+9,
						COLORSETTINGS,cliprect);
		}

}

void grid_bwidget_draw_picture_right (bwidget_t widget, const bwin_rect *cliprect)
{
	bwin_t win = bwidget_win (widget);
	bwin_settings win_settings;
	blabel_settings *label_settings;
	blabel_settings labelSettings;

	blabel_get (widget, &labelSettings);
	label_settings = &labelSettings;

	bwin_get (win, &win_settings);

	if (cliprect && !BWIN_RECT_ISINTERSECTING (&win_settings.rect, cliprect))
	{
		return;
	}

	{
		for (int i = 0; i < 15; i++)
		{
			bwin_draw_line (win, win_settings.rect.x, win_settings.rect.height / 2 + i,
						win_settings.rect.width - i * 1.5-15, win_settings.rect.height / 2 + i,
						COLOR_CIRCLE,cliprect);
		}
		for (int i = 0; i < 15; i++)
		{
			bwin_draw_line (win, win_settings.rect.x, win_settings.rect.height / 2 - i,
						win_settings.rect.width - i * 1.5-15, win_settings.rect.height / 2 - i,
						COLOR_CIRCLE,cliprect);
		}
	}

}

void grid_bwidget_draw_picture_left (bwidget_t widget, const bwin_rect *cliprect)
{
	bwin_t win = bwidget_win (widget);
	bwin_settings win_settings;
	blabel_settings *label_settings;
	blabel_settings labelSettings;

	blabel_get (widget, &labelSettings);
	label_settings = &labelSettings;

	bwin_get (win, &win_settings);

	if (cliprect && !BWIN_RECT_ISINTERSECTING (&win_settings.rect, cliprect))
	{
		return;
	}

	{
		for (int i = 0; i < 15; i++)
		{
			bwin_draw_line (win, win_settings.rect.x + i * 1.5, win_settings.rect.height / 2 + i,
						win_settings.rect.width-15, win_settings.rect.height / 2 + i,
						COLOR_CIRCLE,cliprect);
		}
		for (int i = 0; i < 15; i++)
		{
			bwin_draw_line (win, win_settings.rect.x + i * 1.5, win_settings.rect.height / 2 - i,
						win_settings.rect.width-15, win_settings.rect.height / 2 - i,
						COLOR_CIRCLE,cliprect);
		}
	}

}

void grid_bwidget_draw_picture (bwidget_t widget, const bwin_rect *cliprect)
{
	bwin_t win = bwidget_win (widget);
	bwin_settings win_settings;
	blabel_settings *label_settings;
	blabel_settings labelSettings;

	blabel_get (widget, &labelSettings);
	label_settings = &labelSettings;

	bwin_get (win, &win_settings);

	if (cliprect && !BWIN_RECT_ISINTERSECTING (&win_settings.rect, cliprect))
	{
		return;
	}

		{
			bwin_draw_line (win, win_settings.rect.x, win_settings.rect.height / 2,
						win_settings.rect.width, win_settings.rect.height / 2,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+0.2, win_settings.rect.height / 2-1,
						win_settings.rect.width-0.2, win_settings.rect.height / 2-1,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+0.4, win_settings.rect.height / 2-2,
						win_settings.rect.width-0.4, win_settings.rect.height / 2-2,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+0.7, win_settings.rect.height / 2-3,
						win_settings.rect.width-0.7, win_settings.rect.height / 2-3,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+1.2, win_settings.rect.height / 2-4,
						win_settings.rect.width-1.2, win_settings.rect.height / 2-4,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+1.5, win_settings.rect.height / 2-5,
						win_settings.rect.width-1.5, win_settings.rect.height / 2-5,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+1.9, win_settings.rect.height / 2-6,
						win_settings.rect.width-1.9, win_settings.rect.height / 2-6,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+2.7, win_settings.rect.height / 2-7,
						win_settings.rect.width-2.7, win_settings.rect.height / 2-7,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+3.6, win_settings.rect.height / 2-8,
						win_settings.rect.width-3.6, win_settings.rect.height / 2-8,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+4.7, win_settings.rect.height / 2-9,
						win_settings.rect.width-4.7, win_settings.rect.height / 2-9,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+5.8, win_settings.rect.height / 2-10,
						win_settings.rect.width-5.8, win_settings.rect.height / 2-10,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+7.1, win_settings.rect.height / 2-11,
						win_settings.rect.width-7.1, win_settings.rect.height / 2-11,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+10.2, win_settings.rect.height / 2-12,
						win_settings.rect.width-10.2, win_settings.rect.height / 2-12,
						COLOR_CIRCLE,cliprect);
		}
		{
			bwin_draw_line (win, win_settings.rect.x+0.2, win_settings.rect.height / 2+1,
						win_settings.rect.width-0.2, win_settings.rect.height / 2+1,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+0.4, win_settings.rect.height / 2+2,
						win_settings.rect.width-0.4, win_settings.rect.height / 2+2,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+0.7, win_settings.rect.height / 2+3,
						win_settings.rect.width-0.7, win_settings.rect.height / 2+3,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+1.2, win_settings.rect.height / 2+4,
						win_settings.rect.width-1.2, win_settings.rect.height / 2+4,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+1.5, win_settings.rect.height / 2+5,
						win_settings.rect.width-1.5, win_settings.rect.height / 2+5,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+1.9, win_settings.rect.height / 2+6,
						win_settings.rect.width-1.9, win_settings.rect.height / 2+6,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+2.7, win_settings.rect.height / 2+7,
						win_settings.rect.width-2.7, win_settings.rect.height / 2+7,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+3.5, win_settings.rect.height / 2+8,
						win_settings.rect.width-3.5, win_settings.rect.height / 2+8,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+4.4, win_settings.rect.height / 2+9,
						win_settings.rect.width-4.4, win_settings.rect.height / 2+9,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+5.3, win_settings.rect.height / 2+10,
						win_settings.rect.width-5.3, win_settings.rect.height / 2+10,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+6.8, win_settings.rect.height / 2+11,
						win_settings.rect.width-6.8, win_settings.rect.height / 2+11,
						COLOR_CIRCLE,cliprect);
			bwin_draw_line (win, win_settings.rect.x+9.9, win_settings.rect.height / 2+12,
						win_settings.rect.width-9.9, win_settings.rect.height / 2+12,
						COLOR_CIRCLE,cliprect);
		}

}

void grid_bwidget_draw_info (bwidget_t widget, const bwin_rect *cliprect)
{
	bwin_t win = bwidget_win (widget);
	bwin_settings win_settings;
	blabel_settings *label_settings;
	blabel_settings labelSettings;

	blabel_get (widget, &labelSettings);
	label_settings = &labelSettings;

	bwin_get (win, &win_settings);

	if (cliprect && !BWIN_RECT_ISINTERSECTING (&win_settings.rect, cliprect))
	{
		return;
	}
		for (int i = 1; i < 80; i++)
		{
			bwin_draw_line (win, win_settings.rect.x+31, win_settings.rect.height / 2-i,
						win_settings.rect.width, win_settings.rect.height / 2-i,
						COLORSETTINGS,cliprect);
		}
		{
			bwin_draw_line (win, win_settings.rect.x+31, win_settings.rect.height / 2-80,
						win_settings.rect.width, win_settings.rect.height / 2-80,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.1, win_settings.rect.height / 2-80-1,
						win_settings.rect.width-0.1, win_settings.rect.height / 2-80-1,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.2, win_settings.rect.height / 2-80-2,
						win_settings.rect.width-0.2, win_settings.rect.height / 2-80-2,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.3, win_settings.rect.height / 2-80-3,
						win_settings.rect.width-0.3, win_settings.rect.height / 2-80-3,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.4, win_settings.rect.height / 2-80-4,
						win_settings.rect.width-0.4, win_settings.rect.height / 2-80-4,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.6, win_settings.rect.height / 2-80-5,
						win_settings.rect.width-0.6, win_settings.rect.height / 2-80-5,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.8, win_settings.rect.height / 2-80-6,
						win_settings.rect.width-0.8, win_settings.rect.height / 2-80-6,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.1, win_settings.rect.height / 2-80-7,
						win_settings.rect.width-1.1, win_settings.rect.height / 2-80-7,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.4, win_settings.rect.height / 2-80-8,
						win_settings.rect.width-1.4, win_settings.rect.height / 2-80-8,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.7, win_settings.rect.height / 2-80-9,
						win_settings.rect.width-1.7, win_settings.rect.height / 2-80-9,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2, win_settings.rect.height / 2-80-10,
						win_settings.rect.width-2, win_settings.rect.height / 2-80-10,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.3, win_settings.rect.height / 2-80-11,
						win_settings.rect.width-2.3, win_settings.rect.height / 2-80-11,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.6, win_settings.rect.height / 2-80-12,
						win_settings.rect.width-2.6, win_settings.rect.height / 2-80-12,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.9, win_settings.rect.height / 2-80-13,
						win_settings.rect.width-2.9, win_settings.rect.height / 2-80-13,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+3.3, win_settings.rect.height / 2-80-14,
						win_settings.rect.width-3.3, win_settings.rect.height / 2-80-14,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+3.7, win_settings.rect.height / 2-80-15,
						win_settings.rect.width-3.7, win_settings.rect.height / 2-80-15,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+4.1, win_settings.rect.height / 2-80-16,
						win_settings.rect.width-4.1, win_settings.rect.height / 2-80-16,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+4.5, win_settings.rect.height / 2-80-17,
						win_settings.rect.width-4.5, win_settings.rect.height / 2-80-17,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+5.2, win_settings.rect.height / 2-80-18,
						win_settings.rect.width-5.2, win_settings.rect.height / 2-80-18,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+5.8, win_settings.rect.height / 2-80-19,
						win_settings.rect.width-5.8, win_settings.rect.height / 2-80-19,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+6.5, win_settings.rect.height / 2-80-20,
						win_settings.rect.width-6.5, win_settings.rect.height / 2-80-20,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+7.3, win_settings.rect.height / 2-80-21,
						win_settings.rect.width-7.3, win_settings.rect.height / 2-80-21,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+8.1, win_settings.rect.height / 2-80-22,
						win_settings.rect.width-8.1, win_settings.rect.height / 2-80-22,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+8.8, win_settings.rect.height / 2-80-23,
						win_settings.rect.width-8.8, win_settings.rect.height / 2-80-23,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+9.5, win_settings.rect.height / 2-80-24,
						win_settings.rect.width-9.5, win_settings.rect.height / 2-80-24,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+10.4, win_settings.rect.height / 2-80-25,
						win_settings.rect.width-10.4, win_settings.rect.height / 2-80-25,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+11.3, win_settings.rect.height / 2-80-26,
						win_settings.rect.width-11.3, win_settings.rect.height / 2-80-26,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+13.1, win_settings.rect.height / 2-80-27,
						win_settings.rect.width-13.1, win_settings.rect.height / 2-80-27,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+14.8, win_settings.rect.height / 2-80-28,
						win_settings.rect.width-14.8, win_settings.rect.height / 2-80-28,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+16.4, win_settings.rect.height / 2-80-29,
						win_settings.rect.width-16.4, win_settings.rect.height / 2-80-29,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+17.5, win_settings.rect.height / 2-80-30,
						win_settings.rect.width-17.5, win_settings.rect.height / 2-80-30,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+19.8, win_settings.rect.height / 2-80-31,
						win_settings.rect.width-19.8, win_settings.rect.height / 2-80-31,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+22.9, win_settings.rect.height / 2-80-32,
						win_settings.rect.width-22.9, win_settings.rect.height / 2-80-32,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+25.1, win_settings.rect.height / 2-80-33,
						win_settings.rect.width-25.1, win_settings.rect.height / 2-80-33,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+31.3, win_settings.rect.height / 2-80-34,
						win_settings.rect.width-31.3, win_settings.rect.height / 2-80-34,
						COLORSETTINGS,cliprect);
		}

		{
			bwin_draw_line (win, win_settings.rect.x+31, win_settings.rect.height / 2,
						win_settings.rect.width, win_settings.rect.height / 2,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.1, win_settings.rect.height / 2+1,
						win_settings.rect.width-0.1, win_settings.rect.height / 2+1,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.2, win_settings.rect.height / 2+2,
						win_settings.rect.width-0.2, win_settings.rect.height / 2+2,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.3, win_settings.rect.height / 2+3,
						win_settings.rect.width-0.3, win_settings.rect.height / 2+3,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.4, win_settings.rect.height / 2+4,
						win_settings.rect.width-0.4, win_settings.rect.height / 2+4,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.6, win_settings.rect.height / 2+5,
						win_settings.rect.width-0.6, win_settings.rect.height / 2+5,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.8, win_settings.rect.height / 2+6,
						win_settings.rect.width-0.8, win_settings.rect.height / 2+6,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.1, win_settings.rect.height / 2+7,
						win_settings.rect.width-1.1, win_settings.rect.height / 2+7,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.4, win_settings.rect.height / 2+8,
						win_settings.rect.width-1.4, win_settings.rect.height / 2+8,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.7, win_settings.rect.height / 2+9,
						win_settings.rect.width-1.7, win_settings.rect.height / 2+9,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2, win_settings.rect.height / 2+10,
						win_settings.rect.width-2, win_settings.rect.height / 2+10,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.3, win_settings.rect.height / 2+11,
						win_settings.rect.width-2.3, win_settings.rect.height / 2+11,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.6, win_settings.rect.height / 2+12,
						win_settings.rect.width-2.6, win_settings.rect.height / 2+12,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.9, win_settings.rect.height / 2+13,
						win_settings.rect.width-2.9, win_settings.rect.height / 2+13,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+3.3, win_settings.rect.height / 2+14,
						win_settings.rect.width-3.3, win_settings.rect.height / 2+14,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+3.7, win_settings.rect.height / 2+15,
						win_settings.rect.width-3.7, win_settings.rect.height / 2+15,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+4.1, win_settings.rect.height / 2+16,
						win_settings.rect.width-4.1, win_settings.rect.height / 2+16,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+4.5, win_settings.rect.height / 2+17,
						win_settings.rect.width-4.5, win_settings.rect.height / 2+17,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+5.2, win_settings.rect.height / 2+18,
						win_settings.rect.width-5.2, win_settings.rect.height / 2+18,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+5.8, win_settings.rect.height / 2+19,
						win_settings.rect.width-5.8, win_settings.rect.height / 2+19,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+6.5, win_settings.rect.height / 2+20,
						win_settings.rect.width-6.5, win_settings.rect.height / 2+20,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+7.3, win_settings.rect.height / 2+21,
						win_settings.rect.width-7.3, win_settings.rect.height / 2+21,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+8.1, win_settings.rect.height / 2+22,
						win_settings.rect.width-8.1, win_settings.rect.height / 2+22,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+8.8, win_settings.rect.height / 2+23,
						win_settings.rect.width-8.8, win_settings.rect.height / 2+23,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+9.5, win_settings.rect.height / 2+24,
						win_settings.rect.width-9.5, win_settings.rect.height / 2+24,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+10.4, win_settings.rect.height / 2+25,
						win_settings.rect.width-10.4, win_settings.rect.height / 2+25,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+11.3, win_settings.rect.height / 2+26,
						win_settings.rect.width-11.3, win_settings.rect.height / 2+26,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+13.1, win_settings.rect.height / 2+27,
						win_settings.rect.width-13.1, win_settings.rect.height / 2+27,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+14.8, win_settings.rect.height / 2+28,
						win_settings.rect.width-14.8, win_settings.rect.height / 2+28,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+16.4, win_settings.rect.height / 2+29,
						win_settings.rect.width-16.4, win_settings.rect.height / 2+29,
						COLORSETTINGS,cliprect);

			bwin_draw_line (win, win_settings.rect.x+31+17.5, win_settings.rect.height / 2+30,
						win_settings.rect.width-17.5, win_settings.rect.height / 2+30,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+19.8, win_settings.rect.height / 2+31,
						win_settings.rect.width-19.8, win_settings.rect.height / 2+31,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+22.9, win_settings.rect.height / 2+32,
						win_settings.rect.width-22.9, win_settings.rect.height / 2+32,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+25.1, win_settings.rect.height / 2+33,
						win_settings.rect.width-25.1, win_settings.rect.height / 2+33,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+31.3, win_settings.rect.height / 2+34,
						win_settings.rect.width-31.3, win_settings.rect.height / 2+34,
						COLORSETTINGS,cliprect);
		}

}


void grid_bwidget_draw (bwidget_t widget, const bwin_rect *cliprect)
{
	bwin_t win = bwidget_win (widget);
	bwin_settings win_settings;
	blabel_settings *label_settings;
	blabel_settings labelSettings;

	blabel_get (widget, &labelSettings);
	label_settings = &labelSettings;

	bwin_get (win, &win_settings);

	if (cliprect && !BWIN_RECT_ISINTERSECTING (&win_settings.rect, cliprect))
	{
		return;
	}
		for (int i = 1; i < 370; i++)
		{
			bwin_draw_line (win, win_settings.rect.x+31, win_settings.rect.height / 2-i,
						win_settings.rect.width, win_settings.rect.height / 2-i,
						COLORSETTINGS,cliprect);
		}
		{
			bwin_draw_line (win, win_settings.rect.x+31, win_settings.rect.height / 2-370,
						win_settings.rect.width, win_settings.rect.height / 2-370,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.1, win_settings.rect.height / 2-370-1,
						win_settings.rect.width-0.1, win_settings.rect.height / 2-370-1,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.2, win_settings.rect.height / 2-370-2,
						win_settings.rect.width-0.2, win_settings.rect.height / 2-370-2,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.3, win_settings.rect.height / 2-370-3,
						win_settings.rect.width-0.3, win_settings.rect.height / 2-370-3,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.4, win_settings.rect.height / 2-370-4,
						win_settings.rect.width-0.4, win_settings.rect.height / 2-370-4,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.6, win_settings.rect.height / 2-370-5,
						win_settings.rect.width-0.6, win_settings.rect.height / 2-370-5,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.8, win_settings.rect.height / 2-370-6,
						win_settings.rect.width-0.8, win_settings.rect.height / 2-370-6,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.1, win_settings.rect.height / 2-370-7,
						win_settings.rect.width-1.1, win_settings.rect.height / 2-370-7,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.4, win_settings.rect.height / 2-370-8,
						win_settings.rect.width-1.4, win_settings.rect.height / 2-370-8,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.7, win_settings.rect.height / 2-370-9,
						win_settings.rect.width-1.7, win_settings.rect.height / 2-370-9,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2, win_settings.rect.height / 2-370-10,
						win_settings.rect.width-2, win_settings.rect.height / 2-370-10,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.3, win_settings.rect.height / 2-370-11,
						win_settings.rect.width-2.3, win_settings.rect.height / 2-370-11,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.6, win_settings.rect.height / 2-370-12,
						win_settings.rect.width-2.6, win_settings.rect.height / 2-370-12,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.9, win_settings.rect.height / 2-370-13,
						win_settings.rect.width-2.9, win_settings.rect.height / 2-370-13,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+3.3, win_settings.rect.height / 2-370-14,
						win_settings.rect.width-3.3, win_settings.rect.height / 2-370-14,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+3.7, win_settings.rect.height / 2-370-15,
						win_settings.rect.width-3.7, win_settings.rect.height / 2-370-15,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+4.1, win_settings.rect.height / 2-370-16,
						win_settings.rect.width-4.1, win_settings.rect.height / 2-370-16,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+4.5, win_settings.rect.height / 2-370-17,
						win_settings.rect.width-4.5, win_settings.rect.height / 2-370-17,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+5.2, win_settings.rect.height / 2-370-18,
						win_settings.rect.width-5.2, win_settings.rect.height / 2-370-18,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+5.8, win_settings.rect.height / 2-370-19,
						win_settings.rect.width-5.8, win_settings.rect.height / 2-370-19,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+6.5, win_settings.rect.height / 2-370-20,
						win_settings.rect.width-6.5, win_settings.rect.height / 2-370-20,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+7.3, win_settings.rect.height / 2-370-21,
						win_settings.rect.width-7.3, win_settings.rect.height / 2-370-21,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+8.1, win_settings.rect.height / 2-370-22,
						win_settings.rect.width-8.1, win_settings.rect.height / 2-370-22,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+8.8, win_settings.rect.height / 2-370-23,
						win_settings.rect.width-8.8, win_settings.rect.height / 2-370-23,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+9.5, win_settings.rect.height / 2-370-24,
						win_settings.rect.width-9.5, win_settings.rect.height / 2-370-24,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+10.4, win_settings.rect.height / 2-370-25,
						win_settings.rect.width-10.4, win_settings.rect.height / 2-370-25,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+11.3, win_settings.rect.height / 2-370-26,
						win_settings.rect.width-11.3, win_settings.rect.height / 2-370-26,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+13.1, win_settings.rect.height / 2-370-27,
						win_settings.rect.width-13.1, win_settings.rect.height / 2-370-27,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+14.8, win_settings.rect.height / 2-370-28,
						win_settings.rect.width-14.8, win_settings.rect.height / 2-370-28,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+16.4, win_settings.rect.height / 2-370-29,
						win_settings.rect.width-16.4, win_settings.rect.height / 2-370-29,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+17.5, win_settings.rect.height / 2-370-30,
						win_settings.rect.width-17.5, win_settings.rect.height / 2-370-30,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+19.8, win_settings.rect.height / 2-370-31,
						win_settings.rect.width-19.8, win_settings.rect.height / 2-370-31,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+22.9, win_settings.rect.height / 2-370-32,
						win_settings.rect.width-22.9, win_settings.rect.height / 2-370-32,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+25.1, win_settings.rect.height / 2-370-33,
						win_settings.rect.width-25.1, win_settings.rect.height / 2-370-33,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+31.3, win_settings.rect.height / 2-370-34,
						win_settings.rect.width-31.3, win_settings.rect.height / 2-370-34,
						COLORSETTINGS,cliprect);
		}

		{
			bwin_draw_line (win, win_settings.rect.x+31, win_settings.rect.height / 2,
						win_settings.rect.width, win_settings.rect.height / 2,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.1, win_settings.rect.height / 2+1,
						win_settings.rect.width-0.1, win_settings.rect.height / 2+1,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.2, win_settings.rect.height / 2+2,
						win_settings.rect.width-0.2, win_settings.rect.height / 2+2,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.3, win_settings.rect.height / 2+3,
						win_settings.rect.width-0.3, win_settings.rect.height / 2+3,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.4, win_settings.rect.height / 2+4,
						win_settings.rect.width-0.4, win_settings.rect.height / 2+4,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.6, win_settings.rect.height / 2+5,
						win_settings.rect.width-0.6, win_settings.rect.height / 2+5,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+0.8, win_settings.rect.height / 2+6,
						win_settings.rect.width-0.8, win_settings.rect.height / 2+6,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.1, win_settings.rect.height / 2+7,
						win_settings.rect.width-1.1, win_settings.rect.height / 2+7,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.4, win_settings.rect.height / 2+8,
						win_settings.rect.width-1.4, win_settings.rect.height / 2+8,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+1.7, win_settings.rect.height / 2+9,
						win_settings.rect.width-1.7, win_settings.rect.height / 2+9,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2, win_settings.rect.height / 2+10,
						win_settings.rect.width-2, win_settings.rect.height / 2+10,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.3, win_settings.rect.height / 2+11,
						win_settings.rect.width-2.3, win_settings.rect.height / 2+11,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.6, win_settings.rect.height / 2+12,
						win_settings.rect.width-2.6, win_settings.rect.height / 2+12,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+2.9, win_settings.rect.height / 2+13,
						win_settings.rect.width-2.9, win_settings.rect.height / 2+13,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+3.3, win_settings.rect.height / 2+14,
						win_settings.rect.width-3.3, win_settings.rect.height / 2+14,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+3.7, win_settings.rect.height / 2+15,
						win_settings.rect.width-3.7, win_settings.rect.height / 2+15,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+4.1, win_settings.rect.height / 2+16,
						win_settings.rect.width-4.1, win_settings.rect.height / 2+16,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+4.5, win_settings.rect.height / 2+17,
						win_settings.rect.width-4.5, win_settings.rect.height / 2+17,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+5.2, win_settings.rect.height / 2+18,
						win_settings.rect.width-5.2, win_settings.rect.height / 2+18,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+5.8, win_settings.rect.height / 2+19,
						win_settings.rect.width-5.8, win_settings.rect.height / 2+19,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+6.5, win_settings.rect.height / 2+20,
						win_settings.rect.width-6.5, win_settings.rect.height / 2+20,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+7.3, win_settings.rect.height / 2+21,
						win_settings.rect.width-7.3, win_settings.rect.height / 2+21,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+8.1, win_settings.rect.height / 2+22,
						win_settings.rect.width-8.1, win_settings.rect.height / 2+22,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+8.8, win_settings.rect.height / 2+23,
						win_settings.rect.width-8.8, win_settings.rect.height / 2+23,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+9.5, win_settings.rect.height / 2+24,
						win_settings.rect.width-9.5, win_settings.rect.height / 2+24,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+10.4, win_settings.rect.height / 2+25,
						win_settings.rect.width-10.4, win_settings.rect.height / 2+25,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+11.3, win_settings.rect.height / 2+26,
						win_settings.rect.width-11.3, win_settings.rect.height / 2+26,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+13.1, win_settings.rect.height / 2+27,
						win_settings.rect.width-13.1, win_settings.rect.height / 2+27,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+14.8, win_settings.rect.height / 2+28,
						win_settings.rect.width-14.8, win_settings.rect.height / 2+28,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+16.4, win_settings.rect.height / 2+29,
						win_settings.rect.width-16.4, win_settings.rect.height / 2+29,
						COLORSETTINGS,cliprect);

			bwin_draw_line (win, win_settings.rect.x+31+17.5, win_settings.rect.height / 2+30,
						win_settings.rect.width-17.5, win_settings.rect.height / 2+30,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+19.8, win_settings.rect.height / 2+31,
						win_settings.rect.width-19.8, win_settings.rect.height / 2+31,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+22.9, win_settings.rect.height / 2+32,
						win_settings.rect.width-22.9, win_settings.rect.height / 2+32,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+25.1, win_settings.rect.height / 2+33,
						win_settings.rect.width-25.1, win_settings.rect.height / 2+33,
						COLORSETTINGS,cliprect);
			bwin_draw_line (win, win_settings.rect.x+31+31.3, win_settings.rect.height / 2+34,
						win_settings.rect.width-31.3, win_settings.rect.height / 2+34,
						COLORSETTINGS,cliprect);
		}

}

bwidget_t CreatLocalLabel(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin)
{

	blabel_settings labelSettings;
	bwidget_t _bwidget;

	BDBG_ASSERT((NULL != pParentWidget) || (NULL != parentWin));


	//printf("%s:x=%d,y=%d,w=%d,h=%d\n",strName,geometry.x(),geometry.y(),geometry.width(),geometry.height());

    blabel_get_default_settings(&labelSettings);

	if(NULL == parentWin)
	{
		labelSettings.widget.window.parent       =  bwidget_win(pParentWidget);
	}
	else
	{
		labelSettings.widget.window.parent       =  parentWin;
	}

    labelSettings.widget.window.x            = geometry.x();
    labelSettings.widget.window.y            = geometry.y();
    labelSettings.widget.window.rect.width   = geometry.width();
    labelSettings.widget.window.rect.height  = geometry.height();
    labelSettings.font                       = font;
    labelSettings.bevel                      = 0;
    labelSettings.bevel_color[bevel_eTop]    = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eRight]  = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eBottom] = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eLeft]   = 0xFFAAAAAA;
    labelSettings.text                       = NULL;
    labelSettings.text_color                 = 0xFFFFFFFF;
    labelSettings.text_justify_horiz         = bwidget_justify_horiz_left;
    labelSettings.text_margin                = 0;
    labelSettings.background_color           = 0xFF294a63;
    labelSettings.widget.key_down            = NULL;
    labelSettings.widget.data                = NULL; /* save ptr to this object */

	labelSettings.widget.draw = grid_bwidget_draw;

    _bwidget= blabel_create(g_System.WidgetEngine, &labelSettings);
	return _bwidget;
}


bwidget_t CreatInfoLabel(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin)
{

	blabel_settings labelSettings;
	bwidget_t _bwidget;

	BDBG_ASSERT((NULL != pParentWidget) || (NULL != parentWin));


	//printf("%s:x=%d,y=%d,w=%d,h=%d\n",strName,geometry.x(),geometry.y(),geometry.width(),geometry.height());

    blabel_get_default_settings(&labelSettings);

	if(NULL == parentWin)
	{
		labelSettings.widget.window.parent       =  bwidget_win(pParentWidget);
	}
	else
	{
		labelSettings.widget.window.parent       =  parentWin;
	}

    labelSettings.widget.window.x            = geometry.x();
    labelSettings.widget.window.y            = geometry.y();
    labelSettings.widget.window.rect.width   = geometry.width();
    labelSettings.widget.window.rect.height  = geometry.height();
    labelSettings.font                       = font;
    labelSettings.bevel                      = 0;
    labelSettings.bevel_color[bevel_eTop]    = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eRight]  = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eBottom] = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eLeft]   = 0xFFAAAAAA;
    labelSettings.text                       = NULL;
    labelSettings.text_color                 = 0xFFFFFFFF;
    labelSettings.text_justify_horiz         = bwidget_justify_horiz_center/*bwidget_justify_horiz_left*/;
    labelSettings.text_margin                = 0;
    labelSettings.background_color           = 0xFF294a63;
    labelSettings.widget.key_down            = NULL;
    labelSettings.widget.data                = NULL; /* save ptr to this object */

	labelSettings.widget.draw = grid_bwidget_draw_info;

    _bwidget= blabel_create(g_System.WidgetEngine, &labelSettings);
	return _bwidget;
}

bwidget_t CreatCirclepicture(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin)
{

	blabel_settings labelSettings;
	bwidget_t _bwidget;

	BDBG_ASSERT((NULL != pParentWidget) || (NULL != parentWin));


	//printf("%s:x=%d,y=%d,w=%d,h=%d\n",strName,geometry.x(),geometry.y(),geometry.width(),geometry.height());

    blabel_get_default_settings(&labelSettings);

	if(NULL == parentWin)
	{
		labelSettings.widget.window.parent       =  bwidget_win(pParentWidget);
	}
	else
	{
		labelSettings.widget.window.parent       =  parentWin;
	}

    labelSettings.widget.window.x            = geometry.x();
    labelSettings.widget.window.y            = geometry.y();
    labelSettings.widget.window.rect.width   = geometry.width();
    labelSettings.widget.window.rect.height  = geometry.height();
    labelSettings.font                       = font;
    labelSettings.bevel                      = 0;
    labelSettings.bevel_color[bevel_eTop]    = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eRight]  = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eBottom] = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eLeft]   = 0xFFAAAAAA;
    labelSettings.text                       = NULL;
    labelSettings.text_color                 = 0xFFFFFFFF;
    labelSettings.text_justify_horiz         = bwidget_justify_horiz_center/*bwidget_justify_horiz_left*/;
    labelSettings.text_margin                = 0;
    labelSettings.background_color           = 0xFF294a63;
    labelSettings.widget.key_down            = NULL;
    labelSettings.widget.data                = NULL; /* save ptr to this object */

	labelSettings.widget.draw = grid_bwidget_draw_picture;

    _bwidget= blabel_create(g_System.WidgetEngine, &labelSettings);
	return _bwidget;
}

bwidget_t Creatleftpicture(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin)
{

	blabel_settings labelSettings;
	bwidget_t _bwidget;

	BDBG_ASSERT((NULL != pParentWidget) || (NULL != parentWin));


	//printf("%s:x=%d,y=%d,w=%d,h=%d\n",strName,geometry.x(),geometry.y(),geometry.width(),geometry.height());

    blabel_get_default_settings(&labelSettings);

	if(NULL == parentWin)
	{
		labelSettings.widget.window.parent       =  bwidget_win(pParentWidget);
	}
	else
	{
		labelSettings.widget.window.parent       =  parentWin;
	}

    labelSettings.widget.window.x            = geometry.x();
    labelSettings.widget.window.y            = geometry.y();
    labelSettings.widget.window.rect.width   = geometry.width();
    labelSettings.widget.window.rect.height  = geometry.height();
    labelSettings.font                       = font;
    labelSettings.bevel                      = 0;
    labelSettings.bevel_color[bevel_eTop]    = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eRight]  = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eBottom] = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eLeft]   = 0xFFAAAAAA;
    labelSettings.text                       = NULL;
    labelSettings.text_color                 = 0xFFFFFFFF;
    labelSettings.text_justify_horiz         = bwidget_justify_horiz_center/*bwidget_justify_horiz_left*/;
    labelSettings.text_margin                = 0;
    labelSettings.background_color           = 0xFF294a63;
    labelSettings.widget.key_down            = NULL;
    labelSettings.widget.data                = NULL; /* save ptr to this object */

	labelSettings.widget.draw = grid_bwidget_draw_picture_left;

    _bwidget= blabel_create(g_System.WidgetEngine, &labelSettings);
	return _bwidget;
}


bwidget_t Creatrightpicture(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin)
{

	blabel_settings labelSettings;
	bwidget_t _bwidget;

	BDBG_ASSERT((NULL != pParentWidget) || (NULL != parentWin));


	//printf("%s:x=%d,y=%d,w=%d,h=%d\n",strName,geometry.x(),geometry.y(),geometry.width(),geometry.height());

    blabel_get_default_settings(&labelSettings);

	if(NULL == parentWin)
	{
		labelSettings.widget.window.parent       =  bwidget_win(pParentWidget);
	}
	else
	{
		labelSettings.widget.window.parent       =  parentWin;
	}

    labelSettings.widget.window.x            = geometry.x();
    labelSettings.widget.window.y            = geometry.y();
    labelSettings.widget.window.rect.width   = geometry.width();
    labelSettings.widget.window.rect.height  = geometry.height();
    labelSettings.font                       = font;
    labelSettings.bevel                      = 0;
    labelSettings.bevel_color[bevel_eTop]    = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eRight]  = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eBottom] = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eLeft]   = 0xFFAAAAAA;
    labelSettings.text                       = NULL;
    labelSettings.text_color                 = 0xFFFFFFFF;
    labelSettings.text_justify_horiz         = bwidget_justify_horiz_center/*bwidget_justify_horiz_left*/;
    labelSettings.text_margin                = 0;
    labelSettings.background_color           = 0xFF294a63;
    labelSettings.widget.key_down            = NULL;
    labelSettings.widget.data                = NULL; /* save ptr to this object */

	labelSettings.widget.draw = grid_bwidget_draw_picture_right;

    _bwidget= blabel_create(g_System.WidgetEngine, &labelSettings);
	return _bwidget;
}


bwidget_t CreatListLabel(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin)
{

	blabel_settings labelSettings;
	bwidget_t _bwidget;

	BDBG_ASSERT((NULL != pParentWidget) || (NULL != parentWin));


	//printf("%s:x=%d,y=%d,w=%d,h=%d\n",strName,geometry.x(),geometry.y(),geometry.width(),geometry.height());

    blabel_get_default_settings(&labelSettings);

	if(NULL == parentWin)
	{
		labelSettings.widget.window.parent       =  bwidget_win(pParentWidget);
	}
	else
	{
		labelSettings.widget.window.parent       =  parentWin;
	}

    labelSettings.widget.window.x            = geometry.x();
    labelSettings.widget.window.y            = geometry.y();
    labelSettings.widget.window.rect.width   = geometry.width();
    labelSettings.widget.window.rect.height  = geometry.height();
    labelSettings.font                       = font;
    labelSettings.bevel                      = 0;
    labelSettings.bevel_color[bevel_eTop]    = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eRight]  = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eBottom] = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eLeft]   = 0xFFAAAAAA;
    labelSettings.text                       = NULL;
    labelSettings.text_color                 = 0xFFFFFFFF;
    labelSettings.text_justify_horiz         = bwidget_justify_horiz_center/*bwidget_justify_horiz_left*/;
    labelSettings.text_margin                = 0;
    labelSettings.background_color           = 0xFF294a63;
    labelSettings.widget.key_down            = NULL;
    labelSettings.widget.data                = NULL; /* save ptr to this object */

	labelSettings.widget.draw = grid_bwidget_draw_list_label;

    _bwidget= blabel_create(g_System.WidgetEngine, &labelSettings);
	return _bwidget;
}


bwidget_t CreatLine(const char       * strName,
                           bwidget_t          pParentWidget,
                           MRect              geometry,
                           bwin_font_t        font,
                           bwin_t             parentWin)
{

	blabel_settings labelSettings;
	bwidget_t _bwidget;

	BDBG_ASSERT((NULL != pParentWidget) || (NULL != parentWin));


	//printf("%s:x=%d,y=%d,w=%d,h=%d\n",strName,geometry.x(),geometry.y(),geometry.width(),geometry.height());

    blabel_get_default_settings(&labelSettings);

	if(NULL == parentWin)
	{
		labelSettings.widget.window.parent       =  bwidget_win(pParentWidget);
	}
	else
	{
		labelSettings.widget.window.parent       =  parentWin;
	}

    labelSettings.widget.window.x            = geometry.x();
    labelSettings.widget.window.y            = geometry.y();
    labelSettings.widget.window.rect.width   = geometry.width();
    labelSettings.widget.window.rect.height  = geometry.height();
    labelSettings.font                       = font;
    labelSettings.bevel                      = 0;
    labelSettings.bevel_color[bevel_eTop]    = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eRight]  = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eBottom] = 0xFFAAAAAA;
    labelSettings.bevel_color[bevel_eLeft]   = 0xFFAAAAAA;
    labelSettings.text                       = NULL;
    labelSettings.text_color                 = 0xFFFFFFFF;
    labelSettings.text_justify_horiz         = bwidget_justify_horiz_center/*bwidget_justify_horiz_left*/;
    labelSettings.text_margin                = 0;
    labelSettings.background_color           = 0xFF294a63;
    labelSettings.widget.key_down            = NULL;
    labelSettings.widget.data                = NULL; /* save ptr to this object */

	labelSettings.widget.draw = grid_bwidget_draw_line;

    _bwidget= blabel_create(g_System.WidgetEngine, &labelSettings);
	return _bwidget;
}


