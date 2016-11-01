#include <stdio.h>
#include "nexus_types.h"
#include "nexus_platform.h"
#include "nexus_i2c.h"
#include "bstd.h"
#include "bkni.h"


#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "global.h"


#define HUNDREDTHS_REG      0x00
#define YEARS_REG			0x06
#define ALARM_HUNDRS_REG	0x08
#define STATUS_REG          0x0F
#define CONTROL_1_REG       0x10
#define CONTROL_2_REG		0x11
#define INT_MASK_REG		0x12
#define SQW_REG				0x13
#define CAL_XT_REG			0x14
#define CAL_RC_HI_REG		0x15
#define CAL_RC_LOW_REG		0x16
#define SLEEP_CTRL_REG		0x17
#define TIMER_CTRL_REG		0x18
#define TIMER_REG			0x19
#define TIMER_INITIAL_REG	0x1A
#define WDT_REG				0x1B
#define OSC_CONTROL_REG     0x1C
#define OSC_STATUS_REG		0x1D
#define CONFIG_KEY_REG      0x1F
#define TRICKLE_REG			0x20
#define ACAL_FLT_REG     	0x26
#define EXTENDED_ADDR_REG   0x3F

typedef struct
{
	uint8_t hundredth;
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t date;
	uint8_t month;
	unsigned int year;
	uint8_t weekday;
} TIME_REG_STRUCT;





int CheckRtcChip(void)
{
	unsigned char ver=0;
	NEXUS_I2c_Read(i2c,0x69,0x28,&ver,1);
	if(ver==0x08)
	{
		printf("found rtc chip AM0805\n");
		return 0;
	}
	else
	{
		printf("can not found rtc chip AM0805\n");
		return 1;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// am_osc_sel - select an oscillator mode
//
//	osc - the oscillator to select
//		0 => 32 KHz XT oscillator, no automatic oscillator switching
//		1 => 32 KHz XT oscillator, automatic oscillator switching to RC on switch to battery power
//		2 => 128 Hz RC oscillator
//
//////////////////////////////////////////////////////////////////////////////////////////////////

int am_osc_sel(unsigned char osc)
{
	int ret = 0;
	unsigned char i;
	unsigned char temp=0xff;
	unsigned char tmp_w=0;


	// Read Oscillator Control register
	NEXUS_I2c_Read(i2c,0x69,OSC_CONTROL_REG,&temp,1);
	printf("OSC_CONTROL_REG:%x\n", temp);
	temp = temp & 0x67; 			    		// Clear OSEL, FOS, AOS

	// Enable Oscillator Register writes
	tmp_w=0xA1;
	NEXUS_I2c_Write(i2c,0x69,CONFIG_KEY_REG,&tmp_w,1);			    // Write the Key register

	switch (osc)
    {
		case 0:                                 // Do nothing, clear Key register
            break;

		case 1:
			temp = temp | 0x10;					// Set AOS
            break;

		default:
			temp = temp | 0x80;					// Set OSEL
            break;
	}
	NEXUS_I2c_Write(i2c,0x69,OSC_CONTROL_REG,&temp,1);
	NEXUS_I2c_Read(i2c,0x69,OSC_CONTROL_REG,&temp,1);
	printf("OSC_CONTROL_REG:%x\n", temp);

	// Wait to make sure switch occurred by testing OMODE

	for (i = 0; i < 100; i++)
	{
		usleep(100000);					// Wait 100 ms
		NEXUS_I2c_Read(i2c,0x69,OSC_STATUS_REG,&temp,1);
		printf("OSC_STATUS_REG:%x\n", temp);
		// Read OMODE
		temp = (temp & 0x10) >> 4;
		if (temp == (osc >> 1)) goto EXIT1;		// Successful switch
	}
	ret = 1;
	goto EXIT;

	//Execution has succeeded
EXIT1:
	ret = 0;

EXIT:
	return ret;
}

/////////////////////////////////////////////////////////////
//
// am_set_autocal(period);
//
//	period - the repeat period for autocalibration.
//		0 => disable autocalibration
//		1 => execute a single autocalibration cycle
//		2 => execute a cycle every 1024 seconds (~17 minutes)
//		3 => execute a cycle every 512 seconds (~8.5 minutes)
//
/////////////////////////////////////////////////////////////

int am_set_autocal(unsigned char period)
{
	int status = 0;
    unsigned char temp;
	unsigned char tmp_w=0;

	NEXUS_I2c_Read(i2c,0x69,OSC_CONTROL_REG,&temp,1);    			// Read Oscillator Control, mask ACAL
    temp &= 0x9F;

	tmp_w=0xA1;
	NEXUS_I2c_Write(i2c,0x69,CONFIG_KEY_REG,&tmp_w,1);			    // Write the Key register

	switch (period)
    {
		case 0:
			NEXUS_I2c_Write(i2c,0x69,OSC_CONTROL_REG,&temp,1);	        // Set ACAL to 0
            break;
		case 1:
			temp |= 0x40;
			NEXUS_I2c_Write(i2c,0x69,OSC_CONTROL_REG,&temp,1);	    // Set ACAL to 2
			usleep(10000);									// Wait for initiation of autocal (10 ms)


			tmp_w=0xA1;
			NEXUS_I2c_Write(i2c,0x69,CONFIG_KEY_REG,&tmp_w,1);				// Load the Key register
			temp = temp & 0x9F;							// Mask ACAL
			NEXUS_I2c_Write(i2c,0x69,OSC_CONTROL_REG,&temp,1);	 		// Set ACAL to 0
            break;
		case 2:
			temp = temp | 0x40;
			NEXUS_I2c_Write(i2c,0x69,OSC_CONTROL_REG,&temp,1);			// Set ACAL to 2
            break;
		case 3:
			temp = temp | 0x60;
			NEXUS_I2c_Write(i2c,0x69,OSC_CONTROL_REG,&temp,1);	    // Set ACAL to 3
            break;
     }

	//Execution has succeeded
	status = 0;

// ERRCHKEXIT:
	return status;
}

int am_config_Trickle_charger()  //ÅäÖÃVBATTä¸Á÷³äµç
{
    unsigned char temp;
	unsigned char tmp_w=0;


	tmp_w=0x9d;
	NEXUS_I2c_Write(i2c,0x69,CONFIG_KEY_REG,&tmp_w,1);
	tmp_w=0xa5;
	NEXUS_I2c_Write(i2c,0x69,TRICKLE_REG,&tmp_w,1);


	NEXUS_I2c_Read(i2c,0x69,TRICKLE_REG,&temp,1);
	if(temp == 0xa5)
	{
		return 0;
	}
	return 1;
}

unsigned char bcd2dec(unsigned char bcdno)
{
    return ((bcdno >> 4) * 10) + (bcdno & 0x0F);
}



unsigned char dec2bcd (unsigned char decno)
{
  return (((decno / 10) << 4) | (decno % 10));
}



unsigned int ReadRtcAndSetSystemTime(void)
{
	unsigned char temp_buff[8];
	unsigned int ret=0;
	TIME_REG_STRUCT time;
	char tmp[32];
	 // Read the counters.
	ret=NEXUS_I2c_Read(i2c,0x69,HUNDREDTHS_REG,temp_buff,8);

	if (ret != 0)
		return ret;
	for(int i=0;i<8;i++)
	{
		printf("temp_buff[%02d]=%02x\n",i,temp_buff[i]);
	}


//	 time.hundredth = bcd2dec(temp_buff[0]);
	 time.second= bcd2dec(temp_buff[1]);
	 time.minute= bcd2dec(temp_buff[2]);
	 time.hour= bcd2dec(temp_buff[3] & 0x3F);
	 time.date= bcd2dec(temp_buff[4]);
	 time.month= bcd2dec(temp_buff[5]);
	 time.year= bcd2dec(temp_buff[6]);
	 time.year=time.year+2000;
	 time.weekday= bcd2dec(temp_buff[7]);

	 memset(tmp,0x00,32);
	 sprintf(tmp,"date -s %04d.%02d.%02d-%02d:%02d:%02d", time.year, time.month, time.date, time.hour, time.minute, time.second);
     printf("%s\n",tmp);
	 system(tmp);
	 return 0;
}


int SetRtcAndSystemTime(unsigned int *date)
{

	int  ret = 0;
    uint8_t temp_buff[8];
	uint8_t temp;
	TIME_REG_STRUCT time_regs;
	char tmp[32];

    //Convert decimal to binary-coded decimal
    time_regs.hundredth	= dec2bcd(0);
    time_regs.second	= dec2bcd(0);
    time_regs.minute	= dec2bcd((unsigned char)date[4]);
    time_regs.hour		= dec2bcd((unsigned char)date[3]);
    time_regs.date		= dec2bcd((unsigned char)date[2]);
    time_regs.weekday	= dec2bcd(0);
    time_regs.month		= dec2bcd((unsigned char)date[1]);
    time_regs.year		= dec2bcd((unsigned char)(date[0]-2000));

    printf("Set time:%x.%x.%x-%x:%x:%x\n", time_regs.year, time_regs.month, time_regs.date, time_regs.hour, time_regs.minute, time_regs.second);
    // Set the WRTC bit to enable counter writes.
    NEXUS_I2c_Read(i2c,0x69,CONTROL_1_REG,&temp,1);
	temp = temp | 0x01;
	NEXUS_I2c_Write(i2c,0x69,CONTROL_1_REG,&temp,1);

    // Write all of the time counters
    temp_buff[0] = time_regs.hundredth;
    temp_buff[1] = time_regs.second;
    temp_buff[2] = time_regs.minute;
    temp_buff[3] = time_regs.hour;
    temp_buff[4] = time_regs.date;
    temp_buff[5] = time_regs.month;
    temp_buff[6] = (unsigned char)time_regs.year;
    temp_buff[7] = time_regs.weekday;

    // Write the values to the AM18XX
    NEXUS_I2c_Write(i2c,0x69,HUNDREDTHS_REG,temp_buff,8);


    // Load the final value of the WRTC bit based on the value of protect
    NEXUS_I2c_Read(i2c,0x69,CONTROL_1_REG,&temp,1);
	temp=temp & 0x7E;
	NEXUS_I2c_Write(i2c,0x69,CONTROL_1_REG,&temp,1);


	 memset(tmp,0x00,32);
	 sprintf(tmp,"date -s %04d.%02d.%02d-%02d:%02d:%02d", date[0], date[1], date[2], date[3], date[4], 0);
     printf("%s\n",tmp);
	 system(tmp);


	//Execution has succeeded
	ret = 0;

	return ret;
}







int init_rtc(void)
{
	int ret=0;
	ret=CheckRtcChip();
	if(ret==1)
	{
		return 1;
	}
	ret = am_osc_sel(1);
	if(ret == 1)
	{
		printf("am0805 select osc failed\n");
	}
	ret = am_set_autocal(3);
	if(ret == 1)
	{
		printf("am0805 autocal failed\n");
	}
	ret = am_config_Trickle_charger();
	if(ret == 1)
	{
		printf("am0805 charger failed\n");
	}
	ReadRtcAndSetSystemTime();


}









