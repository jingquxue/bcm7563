#include <stdio.h>
#include <stdlib.h>
#include "nexus_types.h"
#include "nexus_platform.h"
#include "nexus_gpio.h"
#include "bstd.h"
#include "bkni.h"
#include "gpio.h"
#include "global.h"

void PIO_BIT_Set (unsigned char p1, unsigned char p2, bool value)
{
	NEXUS_GpioSettings settings;


	if((p1==5)&&(p2==4))
	{
		NEXUS_Gpio_GetSettings(g_System.en_485,&settings);
		if(value==true)
		{
			settings.value=NEXUS_GpioValue_eHigh;
		}
		else
		{
			settings.value=NEXUS_GpioValue_eLow;
		}
		NEXUS_Gpio_SetSettings(g_System.en_485,&settings);
	}

}



