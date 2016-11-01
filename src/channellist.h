
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "nandflash.h"




eRet SaveChannelToFlash(void);
eRet LoadChannelFromFlash(void);

eRet AddChannel(uint32 fre,uint32 bandwidth);
eRet DelChannel(uint8 ChannelNum);



