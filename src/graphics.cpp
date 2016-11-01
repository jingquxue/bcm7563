#include "b_os_lib.h"
#include "bwidgets.h"
#include "nexus_surface_client.h"
#include "nexus_surface_compositor.h"
#include "nexus_graphics2d.h"
#include "nexus_core_utils.h"
#include "main.h"
#include "display.h"
#include "global.h"
#include <stdio.h>

static NEXUS_Graphics2DHandle g_Blitter = NULL;
static B_EventHandle g_CheckpointEvent = NULL;
static B_EventHandle g_InactiveEvent = NULL;
static B_EventHandle g_RecycledEvent = NULL;
static NEXUS_SurfaceHandle g_Surface = NULL;
static NEXUS_SurfaceCompositorHandle g_SurfaceCompositor = NULL;
static NEXUS_SurfaceClientHandle g_DesktopClient = NULL;
static bwin_framebuffer_t g_FrameBuffer = NULL;

BDBG_MODULE(kwt_graphics);



void UninitGraphics(void);



void GraphicsCheckpoint ()
{
	NEXUS_Error nerror = NEXUS_SUCCESS;
	BDBG_ASSERT(NULL != g_Blitter);

	nerror = NEXUS_Graphics2D_Checkpoint (g_Blitter, NULL);
	if (nerror == NEXUS_GRAPHICS2D_QUEUED)
	{
		B_Event_Wait(g_CheckpointEvent, B_WAIT_FOREVER);
	}
}

static void SetEventCallback (void *context, int param)
{
	BSTD_UNUSED(param);
	B_Event_Set((B_EventHandle) context);
}

void fbsync (bwin_framebuffer_t framebuffer)
{
	NEXUS_Error nerror = NEXUS_SUCCESS;
	B_Error berror = B_ERROR_SUCCESS;
	eRet ret = eRet_Ok;
	BSTD_UNUSED(framebuffer);

	NEXUS_Surface_Flush(g_Surface);
	nerror = NEXUS_SurfaceClient_UpdateSurface(g_DesktopClient, NULL);
	CHECK_NEXUS_ERROR_GOTO("surface compositor set surface error", ret, nerror, error);

	berror = B_Event_Wait(g_RecycledEvent, 5000);
	CHECK_BOS_ERROR_GOTO("problem while waiting for set surface to complete", ret, nerror, error);
error:
	return;
}

static void do_sync (bwin_framebuffer_t fb)
{
	BDBG_MSG(("TTTTTTTTTTTTTTTTTT DO_SYNC - Graphics"));
	fbsync (fb);
	return;
}

eRet InitializeGraphics (uint16_t width, uint16_t height, uint16_t backWidth, uint16_t backHeight,
																			uint16_t backWidth_sd, uint16_t backHeight_sd)
{
	eRet ret = eRet_Ok;
	NEXUS_Error nerror = NEXUS_SUCCESS;
	NEXUS_Graphics2DSettings graphicsSettings;
	NEXUS_SurfaceCreateSettings surfaceSettings;
	NEXUS_Graphics2DFillSettings fillSettings;

	g_Blitter = NEXUS_Graphics2D_Open (0, NULL);
	g_CheckpointEvent = B_Event_Create(NULL);
	CHECK_PTR_GOTO("graphics checkpoint event create failed", g_CheckpointEvent, ret, eRet_ExternalError, error);
	g_InactiveEvent = B_Event_Create(NULL);
	CHECK_PTR_GOTO("graphics inactive event create failed", g_InactiveEvent, ret, eRet_ExternalError, error);
	NEXUS_Graphics2D_GetSettings(g_Blitter, &graphicsSettings);
	graphicsSettings.checkpointCallback.callback = SetEventCallback;
	graphicsSettings.checkpointCallback.context = g_CheckpointEvent;
	nerror = NEXUS_Graphics2D_SetSettings(g_Blitter, &graphicsSettings);
	CHECK_NEXUS_ERROR_GOTO("unable to set graphics2D settings", ret, nerror, error);

	NEXUS_Surface_GetDefaultCreateSettings(&surfaceSettings);
	surfaceSettings.pixelFormat = NEXUS_PixelFormat_eA8_R8_G8_B8;
	surfaceSettings.width = width;
	surfaceSettings.height = height;
	g_Surface = NEXUS_Surface_Create(&surfaceSettings);
	CHECK_PTR_GOTO("surface create failed.", g_Surface, ret, eRet_ExternalError, error);

	NEXUS_Graphics2D_GetDefaultFillSettings(&fillSettings);
	fillSettings.surface = g_Surface;
	fillSettings.color = 0xFF000000;
	nerror = NEXUS_Graphics2D_Fill(g_Blitter, &fillSettings);
	CHECK_NEXUS_ERROR_GOTO("graphics fill error", ret, nerror, error);

	GraphicsCheckpoint ();

	{
		NEXUS_SurfaceCompositorSettings settings;
		g_SurfaceCompositor = NEXUS_SurfaceCompositor_Create(0);
		NEXUS_SurfaceCompositor_GetSettings(g_SurfaceCompositor, &settings);
		settings.inactiveCallback.callback = SetEventCallback;
		settings.inactiveCallback.context = g_InactiveEvent;

		NEXUS_Display_GetGraphicsSettings(g_System.Display[0], &settings.display[0].graphicsSettings);
		settings.display[0].graphicsSettings.enabled = true;
		settings.display[0].display = g_System.Display[0];
		settings.display[0].framebuffer.number = 2;
		settings.display[0].framebuffer.width = backWidth;
		settings.display[0].framebuffer.height = backHeight;
		settings.display[0].framebuffer.backgroundColor = 0xFF000000;
		settings.display[0].framebuffer.heap = NEXUS_Platform_GetFramebufferHeap(0);

		NEXUS_Display_GetGraphicsSettings(g_System.Display[1], &settings.display[1].graphicsSettings);
		settings.display[1].graphicsSettings.enabled = true;
		settings.display[1].display = g_System.Display[1];
		settings.display[1].framebuffer.number = 2;
		settings.display[1].framebuffer.width = backWidth_sd;
		settings.display[1].framebuffer.height = backHeight_sd;
		settings.display[1].framebuffer.backgroundColor = 0xFF000000;
		settings.display[1].framebuffer.heap = NEXUS_Platform_GetFramebufferHeap(1);

		nerror = NEXUS_SurfaceCompositor_SetSettings(g_SurfaceCompositor, &settings);
		CHECK_NEXUS_ERROR_GOTO("surface compositor set settings error", ret, nerror, error);
	}

	{
		NEXUS_SurfaceClientSettings settingsClient;
		g_RecycledEvent = B_Event_Create(NULL);
		CHECK_PTR_GOTO("graphics displayed event create failed", g_RecycledEvent, ret, eRet_ExternalError, error);
		g_DesktopClient = NEXUS_SurfaceCompositor_CreateClient(g_SurfaceCompositor, 100);
		CHECK_NEXUS_ERROR_GOTO("surface compositor create failed", ret, nerror, error);
		NEXUS_SurfaceClient_GetSettings(g_DesktopClient, &settingsClient);
		settingsClient.recycled.callback = SetEventCallback;
		settingsClient.recycled.context = g_RecycledEvent;
		nerror = NEXUS_SurfaceClient_SetSettings(g_DesktopClient, &settingsClient);
		CHECK_NEXUS_ERROR_GOTO("surface client set settings failed", ret, nerror, error);
		nerror = NEXUS_SurfaceClient_SetSurface(g_DesktopClient, g_Surface);
		CHECK_NEXUS_ERROR_GOTO("surface compositor set surface error", ret, nerror, error);
	}

	{
		bwin_engine_settings bwinSettings;
		bwin_framebuffer_settings fbSettings;
		NEXUS_SurfaceMemory surfaceMem;

		bwin_engine_settings_init(&bwinSettings);
		g_System.BwinEngine = bwin_open_engine(&bwinSettings);
		CHECK_PTR_GOTO("Unable to open bwin engine", g_System.BwinEngine, ret, eRet_ExternalError, error);

		g_System.Font10 = bwin_open_font(g_System.BwinEngine, "fonts/verdana_12.bwin_font", -1, true);
		CHECK_PTR_GOTO("Font creation failed", g_System.Font10, ret, eRet_ExternalError, error);
		g_System.Font12 = bwin_open_font(g_System.BwinEngine, "fonts/verdana_14.bwin_font", -1, true);
		CHECK_PTR_GOTO("Font creation failed", g_System.Font12, ret, eRet_ExternalError, error);
		g_System.Font14 = bwin_open_font(g_System.BwinEngine, "fonts/verdana_17.bwin_font", -1, true);
		CHECK_PTR_GOTO("Font creation failed", g_System.Font14, ret, eRet_ExternalError, error);
		g_System.Font17 = bwin_open_font(g_System.BwinEngine, "fonts/verdana_18.bwin_font", -1, true);
		CHECK_PTR_GOTO("Font creation failed", g_System.Font17, ret, eRet_ExternalError, error);
		g_System.big_menu = bwin_open_font(g_System.BwinEngine, "fonts/arial_black_24_aa.bwin_font", -1, true);
		CHECK_PTR_GOTO("Font creation failed", g_System.big_menu, ret, eRet_ExternalError, error);
		g_System.small_menu = bwin_open_font(g_System.BwinEngine, "fonts/arial_black_25.bwin_font", -1, true);
		CHECK_PTR_GOTO("Font creation failed", g_System.small_menu, ret, eRet_ExternalError, error);

		NEXUS_Surface_GetMemory(g_Surface, &surfaceMem);
		BDBG_WRN(("bwin offscreen: framebuffer %p, %dx%d, pitch %d\n", surfaceMem.buffer, width, height, surfaceMem.pitch));
		bwin_framebuffer_settings_init(&fbSettings);
		fbSettings.width = width;
		fbSettings.height = height;
		fbSettings.pitch = surfaceMem.pitch;
		fbSettings.buffer = surfaceMem.buffer;
		fbSettings.second_buffer = NULL;
		fbSettings.pixel_format = bwin_pixel_format_a8_r8_g8_b8;
		fbSettings.drawops.sync = do_sync;
		fbSettings.data = NULL;
		g_FrameBuffer = bwin_open_framebuffer(g_System.BwinEngine, &fbSettings);
		CHECK_PTR_GOTO("bwin framebuffer failed to open", g_FrameBuffer, ret, eRet_ExternalError, error);
		bwin_get_framebuffer_settings(g_FrameBuffer, &fbSettings);

		g_System.WinFrameBuffer = fbSettings.window;
	}

	return ret;
error:
	UninitGraphics ();
	return ret;
}

void SetGraphicsActive (bool bActive)
{
	NEXUS_SurfaceCompositorSettings settings;
	BDBG_ASSERT(NULL != g_SurfaceCompositor);

	NEXUS_SurfaceCompositor_GetSettings(g_SurfaceCompositor, &settings);
	settings.enabled = bActive;
	NEXUS_SurfaceCompositor_SetSettings(g_SurfaceCompositor, &settings);

	if (false == bActive)
	{
		B_Event_Wait(g_InactiveEvent, B_WAIT_FOREVER);
	}
}

eRet SetFramebufferSize (int num, uint16_t width, uint16_t height)
{
	NEXUS_SurfaceCompositorSettings settings;
	eRet ret = eRet_InvalidState;
	NEXUS_Error nerror = NEXUS_SUCCESS;

	BDBG_ASSERT(NULL != g_System.Display[num]);
	BDBG_ASSERT(NULL != g_SurfaceCompositor);

	NEXUS_SurfaceCompositor_GetSettings(g_SurfaceCompositor, &settings);
	BDBG_ASSERT(false == settings.enabled);

	BDBG_MSG(("set framebuffer size w:%d h:%d", width, height));

	settings.display[num].framebuffer.width = width;
	settings.display[num].framebuffer.height = height;
	settings.display[num].framebuffer.pixelFormat = NEXUS_PixelFormat_eA8_R8_G8_B8;
	settings.display[num].graphicsSettings.clip.width = width;
	settings.display[num].graphicsSettings.clip.height = height;
	nerror = NEXUS_SurfaceCompositor_SetSettings(g_SurfaceCompositor, &settings);
	CHECK_NEXUS_ERROR_GOTO("unable to set simple compositor framebuffer size", ret, nerror, error);
	ret = eRet_Ok;
error:
	return ret;
}

void UninitGraphics(void)
{
	g_System.WinFrameBuffer = NULL;

	if (NULL != g_FrameBuffer )
	{
		bwin_close_framebuffer (g_FrameBuffer);
		g_FrameBuffer = NULL;
	}

	if (NULL != g_System.Font10)
	{
		bwin_close_font (g_System.Font10);
		g_System.Font10 = NULL;
	}

	if (NULL != g_System.Font12)
	{
		bwin_close_font (g_System.Font12);
		g_System.Font12 = NULL;
	}

	if (NULL != g_System.Font14)
	{
		bwin_close_font (g_System.Font14);
		g_System.Font14 = NULL;
	}

	if (NULL != g_System.Font17)
	{
		bwin_close_font (g_System.Font17);
		g_System.Font17 = NULL;
	}

	if (NULL != g_System.big_menu)
	{
		bwin_close_font (g_System.big_menu);
		g_System.big_menu = NULL;
	}

	if (NULL != g_System.small_menu)
	{
		bwin_close_font (g_System.small_menu);
		g_System.small_menu = NULL;
	}

	if (NULL != g_System.BwinEngine)
	{
		bwin_close_engine (g_System.BwinEngine);
		g_System.BwinEngine = NULL;
	}

	if (NULL != g_DesktopClient)
	{
		NEXUS_SurfaceClient_Clear (g_DesktopClient);
		NEXUS_SurfaceCompositor_DestroyClient (g_DesktopClient);
		g_DesktopClient = NULL;
	}

	if (NULL != g_RecycledEvent)
	{
		B_Event_Destroy (g_RecycledEvent);
		g_RecycledEvent = NULL;
	}

	if (NULL != g_SurfaceCompositor)
	{
		NEXUS_SurfaceCompositor_Destroy (g_SurfaceCompositor);
		g_SurfaceCompositor = NULL;
	}

	if (NULL != g_Surface)
	{
		NEXUS_Surface_Destroy (g_Surface);
		g_Surface = NULL;
	}

	if (NULL != g_Blitter)
	{
		NEXUS_Graphics2D_Close (g_Blitter);
		g_Blitter = NULL;
	}

	if (NULL != g_CheckpointEvent)
	{
		B_Event_Destroy (g_CheckpointEvent);
		g_CheckpointEvent = NULL;
	}

	if (NULL != g_InactiveEvent)
	{
		B_Event_Destroy (g_InactiveEvent);
		g_InactiveEvent = NULL;
	}
}

bwin_engine_t getWinEngine ()
{
	return g_System.BwinEngine;
}

