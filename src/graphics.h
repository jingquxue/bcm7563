#ifndef KWT_GRAPHICS_H
#define KWT_GRAPHICS_H
#include "bwin.h"

eRet InitializeGraphics (uint16_t width, uint16_t height, uint16_t backWidth,
                                               uint16_t backHeight, uint16_t backWidth_sd, uint16_t backHeight_sd);
void SetGraphicsActive (bool bActive);
eRet SetFramebufferSize (int num, uint16_t width, uint16_t height);
void UninitGraphics (void);
bwin_engine_t getWinEngine ();
#endif

