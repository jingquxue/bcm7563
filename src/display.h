#ifndef KWT_DISPLAY_H
#define KWT_DISPLAY_H

#include "mgeom.h"

eRet SetVideoAspectRatio (int num,/*NEXUS_VideoFormat format,*/int graphics_opened,
                                                        NEXUS_DisplayAspectRatio aspectRatio);


NEXUS_DisplayHandle getDisplay(int num);
eRet OpenDisplay(int number);
MRect GetGraphicsGeometry(int num);
eRet SetVideoFormat(int num,NEXUS_VideoFormat format,int graphicsOpened);
eRet AddHdmiOutput(void);
eRet AddCvbsOutput(void);
void CloseDisplay(void);

NEXUS_VideoFormat ConvertVideoFormatToBroadcom(unsigned char HdorSD,unsigned int format);
unsigned int ConvertVideoFormatToKwt(unsigned char HdorSD,NEXUS_VideoFormat format);



#endif
