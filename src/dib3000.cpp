


#include <stdio.h>
#include "nexus_types.h"
#include "nexus_platform.h"
#include "nexus_i2c.h"
#include "bstd.h"
#include "bkni.h"
#include "DiB3000M_FrontEnd_HwCfg.h"
#include "front_end_DiB3000M.h"
#include <sys/time.h>
#include <math.h>


#include "global.h"


#define SET_AGC
#define DEFAULT_I2C_PORT		1
#define I2C_PORT_NUM				3

#define I2C_DEFAULT_ADDRESS		16
#define I2C_MASTER_ADDRESS		30
#define I2C_SLAVER_ADDRESS			32

S16 Global_gain_slope_dB;
S16 Global_gain_delta_dB;
S16 Global_agc_ref_dB;
S16 TimingOffset;
S16 TimingOffsetCompDone;

#define DIBChipType_3000MC 0
#define DIBChipType_3000P 1
unsigned  char m_nDibChipType = DIBChipType_3000MC;

NEXUS_GpioHandle pin;

S32 random_num(U16 mod)
{
	struct timeval tv;
	S32 t;

	gettimeofday(&tv,NULL);
	srand((unsigned)(getpid() + tv.tv_usec));
	if(mod == 0)
		return 1;
	t = rand() % mod;

	return t;
}



int read_i2c_frame(NEXUS_I2cHandle i2cHandle,U16 reg,U16* pdata,U8 chip_addr)
{
    NEXUS_Error rc;
    U8 tmp_data[2]={0xFF,0xFF};
    rc=NEXUS_I2c_ReadA16(i2cHandle,chip_addr,reg,tmp_data,2);
    if(rc != NEXUS_SUCCESS)
    {
        printf(" read_i2c_frame error\n");
    }

    /*printf("tmp_data[0]=%02x\n",tmp_data[0]);
    printf("tmp_data[1]=%02x\n",tmp_data[1]);*/
    *pdata=(tmp_data[0]<<8) | (tmp_data[1]);
    /*printf(" READ-Chip_addr[%02x]-reg[%d]=%04x\n",chip_addr,reg,*pdata);*/
	return rc;
}


int write_i2c_frame(NEXUS_I2cHandle i2cHandle,U16 reg,U16 data,U8 chip_addr)
{
    U8 tmp_data[2]={0xFF,0xFF};
    U16 ReadBack=0;
    tmp_data[0]=(U8)(data>>8);
    tmp_data[1]=(U8)data;
    NEXUS_Error rc;
    U8 tmp_addr;
    do
    {
       /* printf("WRITE-Chip_addr[%02x]-reg[%d]=%04x\n",chip_addr,reg,data);*/
        rc=NEXUS_I2c_WriteA16(i2cHandle,chip_addr,reg,tmp_data,2);
        if(reg==1024)
        {
            tmp_addr=data>>4;
            printf("tmp_addr=%02x\n",tmp_addr);
        }
        else
        {
            tmp_addr=chip_addr;
        }

        if(chip_addr == 0x60)
        {
           // printf("0x60------\n");
            return 0;
        }


        if(reg == 5913)
        {
            printf("reg 5913------\n");
            return 0;
        }

        if(reg == 4417)
        {
            printf("reg 4417------\n");
            return 0;
        }

        read_i2c_frame(i2cHandle,reg,&ReadBack,tmp_addr);
        if(rc != NEXUS_SUCCESS)
        {
            printf("write_i2c_frame error\n");

        }
        if(ReadBack != data)
        {
           printf("write error\n");
        }

    }
    while(ReadBack != data);
	return rc;

 }





unsigned char dib_init(NEXUS_I2cHandle i2cHandle)
{
    S32 diversity_on = 1;

    NEXUS_GpioSettings gpioSettings;

	NEXUS_Platform_WriteRegister(0x00408504, 0x22334340);
	NEXUS_Gpio_GetDefaultSettings(NEXUS_GpioType_eAonStandard, &gpioSettings);
    gpioSettings.mode = NEXUS_GpioMode_eOutputPushPull;
    gpioSettings.value = NEXUS_GpioValue_eLow;
    pin = NEXUS_Gpio_Open(NEXUS_GpioType_eAonStandard, 6, &gpioSettings);

	gpioSettings.value = NEXUS_GpioValue_eLow;
	NEXUS_Gpio_SetSettings(pin,&gpioSettings);
	usleep(100000);
	gpioSettings.value = NEXUS_GpioValue_eHigh;
	NEXUS_Gpio_SetSettings(pin,&gpioSettings);
	usleep(10000);
    return front_end_init(i2cHandle,diversity_on);
}


S32  front_end_init ( NEXUS_I2cHandle i2cHandle,S16 diversity_on)
{
  DEF_DIB3000MC_I2C_ADDRESS;
  S16  i, device_address;
  S32 ret = 0;

  S16 default_address = 0x08;
  S16 vendor_id, device_id ;
  S16 _data, value ;
  NEXUS_Error   ErrCode=NEXUS_SUCCESS;

  vendor_id = 0;
  /*check if enumeration already done or not*/
  read_i2c_frame( i2cHandle, 1025, (U16 *)&vendor_id,  DIB3000MC_I2C_ADDRESS[0]) ;

  TimingOffsetCompDone = 0 ;

  printf("DiBccom device ID (Read value %x) before\n", vendor_id);

  if( vendor_id != DiBcom_ID)
  {
    for ( i=DEF_NUM_DEMODS-1; i>=0  ; i--)
	{
        ErrCode = read_i2c_frame( i2cHandle, 1025, (U16 *)&vendor_id,  default_address) ;
        printf("DiBccom device ID (Read value %x) \n", vendor_id);

		/* Read device_id*/
		read_i2c_frame( i2cHandle, 1026, (U16 *)&device_id,  default_address) ;

		if (device_id == 0x3000)
		{
		  printf("This driver cannot be used to control the DiB3000MB \n");
		  printf("Please use  the appropriate driver for that \n");
		  return 1;

		}
        else if (device_id == 0x3001)
        {
                printf("DiB3000MC device detected \n");
                m_nDibChipType = DIBChipType_3000MC;
				g_Status.TunerType=1;
        }
        else if (device_id == 0x3002)
        {
                printf("DiB3000P device detected \n");
                m_nDibChipType = DIBChipType_3000P;
				g_Status.TunerType=1;

        }
        else
        {
                printf("No DiBcom device detected \n");
		        return 1;

		}

		/* End: Need to be modified during integration*/
		write_i2c_frame( i2cHandle, 1040 , 1      ,  default_address) ;
		write_i2c_frame( i2cHandle, 244 , (1<<11) ,  default_address) ;


        value=DIB3000MC_I2C_ADDRESS[i]<<1;

        printf("i2c value=%d\n",value);


		value = ((value << 3) & 0x03F0)+1 ;
		write_i2c_frame( i2cHandle, 1024, value, default_address)    ;
		}

	  for ( i=DEF_NUM_DEMODS-1; i>=0  ; i--)
		{
		  read_i2c_frame( i2cHandle, 1024, (U16 *)&value, DIB3000MC_I2C_ADDRESS[i])    ;
		  write_i2c_frame( i2cHandle, 1024, value&0x03F0, DIB3000MC_I2C_ADDRESS[i])    ;
		}
  }



  for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	{

	  _data = 0xffff & ( ((0) << 7) | (1<<7) );
	  write_i2c_frame(i2cHandle, 769, _data,  DIB3000MC_I2C_ADDRESS[i]) ;
	}


  for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	{
	  device_address = DIB3000MC_I2C_ADDRESS[i] ;


	  write_i2c_frame( i2cHandle, 1027,0x8000, device_address) ;
	  write_i2c_frame( i2cHandle, 1027,0x0000, device_address) ;


	  write_i2c_frame( i2cHandle, 1031 , 0     ,  device_address ) ;
	  write_i2c_frame( i2cHandle, 1032 , 0     ,  device_address ) ;
	  write_i2c_frame( i2cHandle, 1033 , 0     ,  device_address ) ;

	  write_i2c_frame( i2cHandle, 1037,12592, device_address) ;


	  write_i2c_frame( i2cHandle, 18,0,  device_address);
	  write_i2c_frame( i2cHandle, 19,0,  device_address);
	  write_i2c_frame( i2cHandle, 33,    5,  device_address);
	  write_i2c_frame( i2cHandle, 36,81,  device_address);
	  write_i2c_frame( i2cHandle, 88,0x410, device_address);


	  write_i2c_frame( i2cHandle, 99, 0x220, device_address);
	  if ( DEF_PHASE_NOISE_MODE == 0)
		write_i2c_frame( i2cHandle, 111, 0x00, device_address);
	  else
		write_i2c_frame( i2cHandle, 111, 0x02, device_address);



	  if(DIBChipType_3000P == m_nDibChipType)
	  {
	    set_mobile_mode(i2cHandle, Portable_Reception, device_address);

	  }
      else
      {
        set_mobile_mode(i2cHandle, Auto_Reception, device_address);
      }



	  set_agc_loop_Bw(i2cHandle,0,  device_address) ;
	  write_i2c_frame(i2cHandle,50 , 0x8000 , device_address) ;
	  write_i2c_frame(i2cHandle,51, 37322, device_address);
	  write_i2c_frame(i2cHandle,52 , 0x1ba , device_address) ;
	  write_i2c_frame(i2cHandle,53 , 0x87 , device_address) ;
	  write_i2c_frame(i2cHandle,54 , 0x87 , device_address) ;


	  switch( DEF_TUNER_TYPE)
		{
		case TUNER_PANASONIC_ENV57H13D5:
#if defined(SET_AGC)
            write_i2c_frame(i2cHandle,36,0x196, device_address);
            write_i2c_frame(i2cHandle,37,0x1d, device_address);
            write_i2c_frame(i2cHandle,38,0x0, device_address);
            write_i2c_frame(i2cHandle,39,0x1cc7, device_address);
            write_i2c_frame(i2cHandle,42,0xffff, device_address);
            write_i2c_frame(i2cHandle,43,0xffff, device_address);
            write_i2c_frame(i2cHandle,44,0xffff, device_address);
            write_i2c_frame(i2cHandle,45,0x0000, device_address);
            write_i2c_frame(i2cHandle,46,0x0000, device_address);
            write_i2c_frame(i2cHandle,47,0x0000, device_address);
            write_i2c_frame(i2cHandle,48,0x0000, device_address);
            write_i2c_frame(i2cHandle,49,0x4040, device_address);
            write_i2c_frame(i2cHandle,52,0x1a2, device_address);
#else
		  write_i2c_frame(i2cHandle,36 , 0x51 , device_address) ;
		  write_i2c_frame(i2cHandle,37 , 0x301d , device_address) ;
		  write_i2c_frame(i2cHandle,38 , 0x0 , device_address) ;
		  write_i2c_frame(i2cHandle,39 , 0x1cc7 , device_address) ;
		  write_i2c_frame(i2cHandle,42 , 0xdc29 , device_address) ;
		  write_i2c_frame(i2cHandle,43 , 0x570a , device_address) ;
		  write_i2c_frame(i2cHandle,44 , 0xbae1 , device_address) ;
		  write_i2c_frame(i2cHandle,45 , 0x8ccd , device_address) ;
		  write_i2c_frame(i2cHandle,46 , 0x3b6d , device_address) ;
		  write_i2c_frame(i2cHandle,47 , 0x551d , device_address) ;
		  write_i2c_frame(i2cHandle,48 , 0xa , device_address) ;
		  write_i2c_frame(i2cHandle,49 , 0x951e , device_address) ;

#endif
		 // Global_gain_slope_dB  = 78.74;
		 // Global_gain_delta_dB  = -9.19  ;
		//  Global_agc_ref_dB    = -14  ;

		  Global_gain_slope_dB  = 100;
		  Global_gain_delta_dB  = -2  ;
		  Global_agc_ref_dB    = -14  ;

		  break;
		default:
		  break;
		}


	  write_i2c_frame( i2cHandle, 110,3277,  device_address);
	  write_i2c_frame( i2cHandle, 26,0x6680, device_address);
 	  write_i2c_frame( i2cHandle, 1,4, device_address);
 	  write_i2c_frame( i2cHandle, 2,4, device_address);
 	  write_i2c_frame( i2cHandle, 3,0x1000, device_address);
 	  write_i2c_frame( i2cHandle, 5,1, device_address);
	  DEF_REG_6_8   ;
	  DEF_REG_7_8   ;
	  DEF_REG_8_8   ;
	  DEF_REG_9_8   ;
	  DEF_REG_10_8  ;
	  DEF_REG_11_8  ;
	  write_i2c_frame(i2cHandle,12 , 0x0 , device_address) ;
	  write_i2c_frame(i2cHandle,13 , 0x3e8 , device_address) ;
	  write_i2c_frame(i2cHandle,14 , 0x0 , device_address) ;
	  write_i2c_frame(i2cHandle,15 , 0x3f2 , device_address) ;
	  DEF_REG_16_8  ;
	  DEF_REG_17_8  ;


	  write_i2c_frame( i2cHandle, 4,2068,  device_address);

	  DEF_REG_21_0 ;
	  DEF_REG_22 ;

	  printf("front_end_init\n");

	  set_timing(i2cHandle, 0,0, 8, DIB3000MC_I2C_ADDRESS[0]);

	  write_i2c_frame( i2cHandle, 120,8207,  device_address);
	  write_i2c_frame( i2cHandle, 134,0,  device_address);

	  write_i2c_frame( i2cHandle, 195,16, device_address);

	  write_i2c_frame( i2cHandle, 180,0xFFF0,  device_address);

	  set_impulse_noise (i2cHandle,0, 1, 8, device_address);

	  set_output_mode(i2cHandle ,1) ;
	}

  set_diversity_mode(i2cHandle, diversity_on);

  TimingOffset = 0 ;
  return ret;
}



void set_mobile_mode(NEXUS_I2cHandle i2cHandle ,S16 mode, S16 device_address)
{
	write_i2c_frame( i2cHandle, 140, 0x00, device_address);

	switch( mode)
	{
		case Fixed_Reception:
			write_i2c_frame( i2cHandle, 139, 0x01, device_address);
			write_i2c_frame( i2cHandle, 141, 0x00, device_address);
			write_i2c_frame( i2cHandle, 175, 0x00, device_address);
			write_i2c_frame( i2cHandle, 1032, 0x12C, device_address);
		break;

		case Portable_Reception:
			write_i2c_frame( i2cHandle, 139, 0x01, device_address);
			write_i2c_frame( i2cHandle, 141, 0x00, device_address);
			write_i2c_frame( i2cHandle, 175, 0x00, device_address);
			write_i2c_frame( i2cHandle, 1032, 0x12C, device_address);
		break;

		case Mobile_Reception:
			write_i2c_frame(i2cHandle, 139, 0x00, device_address);
			write_i2c_frame( i2cHandle, 141, 0x00, device_address);
			write_i2c_frame( i2cHandle, 175, 0x02, device_address);
			write_i2c_frame( i2cHandle, 1032, 0x000, device_address);
		break;

		default:
			write_i2c_frame( i2cHandle, 139, 0x00, device_address);
			write_i2c_frame( i2cHandle, 141, 0x00, device_address);
			write_i2c_frame( i2cHandle, 175, 0x02, device_address);
			write_i2c_frame( i2cHandle, 1032, 0x000, device_address);
		break;
	}

}


S16  set_agc_loop_Bw(NEXUS_I2cHandle i2cHandle ,S16 BwSelect,
					 S16 device_address)
{
  if (BwSelect == 1)
	{
	  DEF_REG_40_1 ;
	  DEF_REG_41_1 ;
	}
  else
	{
	  DEF_REG_40_0 ;
	  DEF_REG_41_0 ;

	}

  return 0;
}


void  set_timing(NEXUS_I2cHandle i2cHandle ,S16 UpdateOffset, S16 nfft, S16 bW, S16 device_address )
{
	DEF_DIB3000MC_I2C_ADDRESS ;

	U16 timf_msb, timf_lsb, i;
	S16 tim_offset ;
	S16 tim_sgn ;
	U32 comp1, comp2, comp ;


	if(bW == 1000)
		comp = (U32)Clock_Ref;
	else if(bW == 1500)
		comp = (U32)((Clock_Ref * 3)/2);
	else if(bW == 1750)
		comp = (U32)((Clock_Ref * 7)/4);
	else
		comp = (U32)Clock_Ref * (bW/1000);
	timf_msb = (comp >> 16)&0xFF;
	timf_lsb = (comp &0xFFFF);


	if (UpdateOffset > 0)
	  {
		if (TimingOffsetCompDone == 0)
		 {
			usleep(200000);

			TimingOffsetCompDone = 1 ;
		 }
		read_i2c_frame(i2cHandle, 416, (U16 *)&tim_offset , device_address );
		printf("tim_offset=%d\n",tim_offset);
  		if ((tim_offset&0x2000) == 0x2000) tim_offset |= 0xC000 ;
		printf("nfft=%d\n",nfft);
		if (nfft ==0) tim_offset = tim_offset << 2;
		TimingOffset += tim_offset ;
				printf("TimingOffset=%d\n",TimingOffset);

	  }



	tim_offset = TimingOffset ;
	if (tim_offset <0) {tim_sgn = 1; tim_offset = -tim_offset;}
	else {tim_sgn = 0;}

	comp1 =  (U16)tim_offset * (U16)timf_lsb ;
	comp2 =  (U16)tim_offset * (U16)timf_msb ;
	comp  = ((comp1 >> 16) + comp2) >> 7  ;


	if (tim_sgn == 0)
	  comp = (U16)timf_msb * ((U32)1<<16) + (U16)timf_lsb + comp;
	else
	  comp = (U16)timf_msb * ((U32)1<<16) + (U16)timf_lsb - comp ;
	timf_msb = (comp>>16)&0xFF ;
	timf_lsb = comp&0xFFFF;

	for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	  {
	  	//printf("------%04x\n",timf_msb);
	  	//printf("------%04x\n",timf_lsb);
		write_i2c_frame( i2cHandle, 23, timf_msb , DIB3000MC_I2C_ADDRESS[i] ) ;
		write_i2c_frame( i2cHandle, 24, timf_lsb ,  DIB3000MC_I2C_ADDRESS[i]  ) ;
	  }
  }


void  set_impulse_noise (NEXUS_I2cHandle i2cHandle ,
						   S16 mode,
						   S16 nfft ,
						   S16 Bw,
						   S16 device_address )
{
  S16 regval ;
  if (nfft == 0)
	{
	  DEF_REG_58_0;
	  DEF_REG_59_0;
	  DEF_REG_60_0;
	  DEF_REG_61_0;
	  DEF_REG_62_0;
	  DEF_REG_63_0;
	  DEF_REG_64_0;
	  DEF_REG_65_0;
	  DEF_REG_66_0;
	  DEF_REG_67_0;
	  DEF_REG_68_0;
	  DEF_REG_69_0;
	  DEF_REG_70_0;
	  DEF_REG_71_0;
	  DEF_REG_72_0;
	  DEF_REG_73_0;
	  DEF_REG_74_0;
	  DEF_REG_75_0;
	  DEF_REG_76_0;
	  DEF_REG_77_0;
	  DEF_REG_78_0;
	  DEF_REG_79_0;
	  DEF_REG_80_0;
	  DEF_REG_81_0;
	  DEF_REG_82_0;
	  DEF_REG_83_0;
	  DEF_REG_84_0;
	  DEF_REG_85_0;
	  DEF_REG_86_0;
	}
  else
	{
	  DEF_REG_58_1;
	  DEF_REG_59_1;
	  DEF_REG_60_1;
	  DEF_REG_61_1;
	  DEF_REG_62_1;
	  DEF_REG_63_1;
	  DEF_REG_64_1;
	  DEF_REG_65_1;
	  DEF_REG_66_1;
	  DEF_REG_67_1;
	  DEF_REG_68_1;
	  DEF_REG_69_1;
	  DEF_REG_70_1;
	  DEF_REG_71_1;
	  DEF_REG_72_1;
	  DEF_REG_73_1;
	  DEF_REG_74_1;
	  DEF_REG_75_1;
	  DEF_REG_76_1;
	  DEF_REG_77_1;
	  DEF_REG_78_1;
	  DEF_REG_79_1;
	  DEF_REG_80_1;
	  DEF_REG_81_1;
	  DEF_REG_82_1;
	  DEF_REG_83_1;
	  DEF_REG_84_1;
	  DEF_REG_85_1;
	  DEF_REG_86_1;
	}

  switch(Bw)
	{
	case TB_TUNER_Band_5MHz :
	  DEF_REG_55_5  ;
	  DEF_REG_56_5  ;
	  DEF_REG_57_5  ;
	  break ;
	case TB_TUNER_Band_6MHz :
	  DEF_REG_55_6	;
	  DEF_REG_56_6	;
	  DEF_REG_57_6	;
	  break ;
	case TB_TUNER_Band_7MHz :
	  DEF_REG_55_7	 ;
	  DEF_REG_56_7	 ;
	  DEF_REG_57_7	 ;
	  break ;
	default :
	  DEF_REG_55_8	  ;
	  DEF_REG_56_8	  ;
	  DEF_REG_57_8	  ;
	  break ;
	}

  read_i2c_frame(i2cHandle, 55, (U16 *)&regval, device_address) ;

  switch( mode )
	{
	case 0 :
	  DEF_REG_34_0   ;
	  DEF_REG_35_0   ;
	  break;
	case 1 :
	  DEF_REG_34_1   ;
	  DEF_REG_35_1   ;
	  regval = regval|(1<<10);
	  write_i2c_frame( i2cHandle, 55, regval, device_address) ;
	  break;
	default:
	  DEF_REG_34_4	   ;
	  DEF_REG_35_4	   ;
	  break;
	}
}

void set_output_mode(NEXUS_I2cHandle i2cHandle, S16 value)
{

  DEF_DIB3000MC_I2C_ADDRESS;
  S16  outreg, outmode = 0 ;
  S16 fifo_threshold = 1792 ;
  S16 smo_mode = 2 ;
  S16 Elecout = 1 ;
  U16 ss=0;
  read_i2c_frame(i2cHandle, 206, &ss,  DIB3000MC_I2C_ADDRESS[0]) ;

smo_mode=ss&0x10;

  printf("value=%d\n",value);
  switch(value)
	{
	case  HIGH_Z:
	  Elecout = 0 ;
	  break ;
	case MPEG2_PARALLEL_GATED_CLOCK:
	  outmode = 0 ;
	  break ;
	case MPEG2_PARALLEL_CONTINUOUS_CLOCK:
	  outmode = 1 ;
	  break ;
	case MPEG2_SERIAL:
	  outmode = 2 ;
	  break ;
	case MPEG2_SLAVE:
	  Elecout = 3 ;
	  smo_mode= 0x36 ;
	  fifo_threshold = 512 ;
	  outmode = 5 ;
	  break;
	default:
	  outmode = 0 ;
	  break ;
	}

  if ((DEF_MPEG2_OUTPUT_188 ==1) && (value!= MPEG2_SLAVE))
	smo_mode = (smo_mode & 0xFFDF) | ((DEF_MPEG2_OUTPUT_188&0x01) << 5) ;

  read_i2c_frame( i2cHandle, 244 , (U16 *)&outreg  , DIB3000MC_I2C_ADDRESS[0]);
  outreg = outreg &0x07FF ;
  outreg = outreg + (outmode << 11);
  write_i2c_frame( i2cHandle,   244 , outreg      ,  DIB3000MC_I2C_ADDRESS[0]);

  write_i2c_frame( i2cHandle,   206 , smo_mode        ,  DIB3000MC_I2C_ADDRESS[0]);
  write_i2c_frame( i2cHandle,   207 , fifo_threshold  ,  DIB3000MC_I2C_ADDRESS[0]);
  write_i2c_frame( i2cHandle,   1040 , Elecout      ,  DIB3000MC_I2C_ADDRESS[0])  ;
}


void set_diversity_mode (NEXUS_I2cHandle i2cHandle, S16 diversity_on)
{
	int		i;
  DEF_DIB3000MC_I2C_ADDRESS ;

	if ( (DEF_NUM_DEMODS > 1) && (diversity_on > 0) )
	{
		set_diversity (i2cHandle,  0 , 1 ,DIB3000MC_I2C_ADDRESS[DEF_NUM_DEMODS-1] );

		for ( i=1;i<DEF_NUM_DEMODS-1;i++)
			set_diversity ( i2cHandle, 1 , 1 ,DIB3000MC_I2C_ADDRESS[i] );

               /*slave : only output div is used*/
              set_diversity ( i2cHandle, 0 , 1 ,DIB3000MC_I2C_ADDRESS[1] );

              /*master : only input div is used*/
		set_diversity ( i2cHandle, 1 , 0 ,DIB3000MC_I2C_ADDRESS[0] );
	}
	else
		set_diversity (i2cHandle,  0 , 0 ,DIB3000MC_I2C_ADDRESS[0] );

}


void    set_diversity (NEXUS_I2cHandle i2cHandle, S16 divin_on ,
					   S16 divout_on,
					   S16 device_address )
{
  S16 dvsy, clkcfg, outmode;

  write_i2c_frame( i2cHandle, 177,6, device_address);
  write_i2c_frame( i2cHandle, 178,6, device_address);

  read_i2c_frame( i2cHandle, 180 , (U16 *)&dvsy  , device_address);
  read_i2c_frame( i2cHandle, 1037, (U16 *)&clkcfg, device_address);

  if (divin_on == 0)
	{
	  write_i2c_frame( i2cHandle, 1037, clkcfg|(1<<8), device_address);
	  write_i2c_frame( i2cHandle, 180 , dvsy&0xFFF0  , device_address);
	}
  else
	{
	  write_i2c_frame( i2cHandle, 180 , (dvsy&0xFFF0) + 6  , device_address);
	}

  if (divout_on == 1)
	{
	  read_i2c_frame( i2cHandle, 244 , (U16 *)&outmode  , device_address);
	  write_i2c_frame( i2cHandle, 244 , (outmode&0x7FF) + (4<<11)  , device_address);
	  write_i2c_frame( i2cHandle, 1040 , 1  , device_address);
	}
}




NEXUS_Error dib_tune(NEXUS_I2cHandle i2cHandle, U32 freq, S16 bw)
{

    struct channel_descriptor cd;
    S16 search_status ;
    U16 count;
    NEXUS_Error ret = NEXUS_SUCCESS;

    cd.RF_KHz             =  freq;
    cd.Bw                 =  bw;
	if (1)
    {
        cd.invspec            =  -1;
        cd.nfft               =  -1;
        cd.guard              =  -1;
        cd.nqam               =  -1;
        cd.vit_hrch           =  -1;
        cd.vit_alpha          =  -1;
        cd.vit_select_hp      =  -1;
        cd.vit_code_rate_hp   =  -1;
        cd.vit_code_rate_lp   =  -1;
    }
    printf("-I- dib_tune connect freq = %d, bw = %d\n", (int)cd.RF_KHz,(int)cd.Bw);
    tune_channel(i2cHandle,&cd,1,0);
    search_status =0 ;
    count = 0;
    while ( read_search_status(i2cHandle,&search_status) == 0)
    {
        count++;
        if(count > 1000)
        {
			printf("read_search_status == 0\n");
			break;

        }


        usleep(1500);
    }
    ret = get_channel(i2cHandle,&cd,0);
    return ret;
}


void tune_channel (NEXUS_I2cHandle i2cHandle, struct channel_descriptor * cd, S16 use_tps, S16 scanBoost )
{
  DEF_DIB3000MC_I2C_ADDRESS     ;
  S16 device_address ;

  U32 RF_KHz = cd->RF_KHz;
  S16 _data ;
  S16 regvalue ;
  S16 i ;


  for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	{
	  _data = 0xffff & (DEF_TUNER_I2C_ADDRESS << 7 );
	  write_i2c_frame( i2cHandle, 769, _data,  DIB3000MC_I2C_ADDRESS[i]) ;

	  switch( DEF_TUNER_TYPE)
	    {
		case TUNER_PANASONIC_ENV57H13D5:
		  set_rf_freq_panasonic_cofdm_ENV57H1XD5(i2cHandle, RF_KHz, DEF_TUNER_I2C_ADDRESS ) ;
		  break ;
		case  TUNER_RFAGCIN_UNKNOWN:
		  break;
	   default :
		  printf("Unknown tuner \n");

		  break;
		}

        _data = 0xffff & ( (DEF_TUNER_I2C_ADDRESS << 7) | (1<<7) );
	    write_i2c_frame( i2cHandle, 769, _data,  DIB3000MC_I2C_ADDRESS[i]) ;
	}

  usleep(DEF_WAITING_TUNER_CFG_us);


  for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	{
	  device_address = DIB3000MC_I2C_ADDRESS[i] ;
	  _data = 0xffff & ( (DEF_TUNER_I2C_ADDRESS << 7) | (1<<7) );
	  write_i2c_frame( i2cHandle, 769, _data,  DIB3000MC_I2C_ADDRESS[i]) ;
	}

  if (scanBoost == 0)
	{

		printf("tune_channel\n");

	  set_timing(i2cHandle,0,cd->nfft, cd->Bw, DIB3000MC_I2C_ADDRESS[i]);
	  set_init_auto_search(i2cHandle,cd->Bw);

	  for ( i=0; i<DEF_NUM_DEMODS  ; i++)
		{
		  device_address = DIB3000MC_I2C_ADDRESS[i] ;

		  set_agc_loop_Bw(i2cHandle, 0, device_address);
		  write_i2c_frame( i2cHandle, 1027, 0x800, device_address);
		  write_i2c_frame( i2cHandle, 1027, 0x000, device_address);


		  write_i2c_frame( i2cHandle, 26,0x6680,  device_address);
		  write_i2c_frame( i2cHandle, 29, 0x1273, device_address);
		  write_i2c_frame( i2cHandle, 33,5,  device_address);
		  set_adp_cfg(i2cHandle,1, device_address);
		  write_i2c_frame( i2cHandle, 133,15564,  device_address);

		  write_i2c_frame(i2cHandle,12 , 0x0 , device_address) ;
		  write_i2c_frame(i2cHandle,13 , 0x3e8 , device_address) ;
		  write_i2c_frame(i2cHandle,14 , 0x0 , device_address) ;
		  write_i2c_frame(i2cHandle,15 , 0x3f2 , device_address) ;


		  if (cd->Bw !=TB_TUNER_Band_8MHz)
			{
			  write_i2c_frame( i2cHandle, 93,0,  device_address);
			  write_i2c_frame( i2cHandle, 94,0,  device_address);
			  write_i2c_frame( i2cHandle, 95,0,  device_address);
			  write_i2c_frame( i2cHandle, 96,0,  device_address);
			  write_i2c_frame( i2cHandle, 97,0,  device_address);
			  write_i2c_frame( i2cHandle, 98,0,  device_address);
			}
		  else
			{
			  write_i2c_frame( i2cHandle, 93,0x20,  device_address);
			  write_i2c_frame( i2cHandle, 94,0x21,  device_address);
			  write_i2c_frame( i2cHandle, 95,0x20,  device_address);
			  write_i2c_frame( i2cHandle, 96,0x23,  device_address);
			  write_i2c_frame( i2cHandle, 97,0x20,  device_address);
			  write_i2c_frame( i2cHandle, 98,0x27,  device_address);

			}
		  set_general_cfg(i2cHandle, cd,  use_tps, device_address);
		  set_impulse_noise (i2cHandle,0,  cd->nfft, cd->Bw,  device_address );

		  read_i2c_frame ( i2cHandle, 0, (U16 *)&regvalue, device_address);
		  write_i2c_frame( i2cHandle, 0, regvalue+512, device_address);
		  write_i2c_frame( i2cHandle, 0, regvalue, device_address);
		}
	}


  usleep(DEF_WAITING_LOCK_AGC_us);


  if (( cd->vit_alpha != -1) && ( cd->vit_hrch  != -1) &&
	  ( cd->vit_code_rate_hp != -1) && ( cd->vit_code_rate_lp != -1) &&
	  ( cd->nqam != -1) && ( cd->nfft != -1) &&
	  ( cd->guard != -1) &&  ( cd->invspec != -1))
	{
	  for ( i=0; i<DEF_NUM_DEMODS  ; i++)
		{
		  device_address = DIB3000MC_I2C_ADDRESS[i];
		  set_impulse_noise (i2cHandle,DEF_IMPULSE_NOISE_MODE,
							 cd->nfft, cd->Bw,  device_address );


		  write_i2c_frame( i2cHandle, 29, 0x1073, device_address);
		  set_adp_cfg(i2cHandle ,cd->nqam, device_address) ;

		  if (cd->nfft == 1)
			{
			  write_i2c_frame( i2cHandle, 26,38528,  device_address);
			  write_i2c_frame( i2cHandle, 33,8,  device_address);
			}
		  else
			{
			  write_i2c_frame(i2cHandle,  26,30336, device_address);
			  write_i2c_frame(i2cHandle, 33,6, device_address);
			}
		}
	}
  else
	for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	  {
		device_address = DIB3000MC_I2C_ADDRESS[i];
		read_i2c_frame ( i2cHandle, 0, (U16 *)&regvalue, device_address);
		write_i2c_frame( i2cHandle, 0, regvalue+256, device_address);
		write_i2c_frame( i2cHandle, 0, regvalue, device_address);
	  }

}


S16  set_rf_freq_panasonic_cofdm_ENV57H1XD5 (NEXUS_I2cHandle i2cHandle, U32 rf_freq_KHz , S16 address)
{
  S16 divfact, divfactmsb, divfactlsb;
  U32 tuner_ol = rf_freq_KHz + 36125;
  S16 i2c_tx_buf[4] ;
  S16 value ;
  S16 TA = 0, T210 = 0, R210 = 0, ctrl1 = 0,cp210 = 0, p4321 = 0 ;

  divfact = (tuner_ol*6 +500)/1000;
  divfactmsb = (S16)(divfact/256);
  divfactlsb = (S16)(divfact-divfactmsb*256);

  i2c_tx_buf[0] = divfactmsb;
  i2c_tx_buf[1] = divfactlsb;

  TA = 1;
  T210 = 0;
  R210 = 0x2;
  ctrl1 = (1<<7) | (TA<<6) | (T210 <<3) | R210 ;

  i2c_tx_buf[2] = 0xff & ctrl1 ;

  if (rf_freq_KHz < 470000) cp210 = 2;
  else if (rf_freq_KHz < 526000) cp210 = 4;
  else if (rf_freq_KHz < 862000) cp210 = 5;
  else
    { printf("channel id can not be > E69 = RF = 858MHz ![%d]\n",(int)rf_freq_KHz);
  }

  if         (rf_freq_KHz < 153000) p4321  = 1;
  else if    (rf_freq_KHz < 470000) p4321  = 2;
  else if    (rf_freq_KHz < 862000) p4321  = 4;

	i2c_tx_buf[3] =  (cp210 << 5) | (p4321);

	value = i2c_tx_buf[2] * 256 + i2c_tx_buf[3] ;
    address=address>>1;
  write_i2c_frame( i2cHandle,  divfact , value       , address ) ;

  return 0;
}


void set_init_auto_search(NEXUS_I2cHandle i2cHandle,S16 Bw)
{
  DEF_DIB3000MC_I2C_ADDRESS     ;
  S16 i, device_address ;
  S16 val_msb, val_lsb ;
  U32 timeout ;


  S16 Boost = 0 ;



  for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	{
	  device_address = DIB3000MC_I2C_ADDRESS[i] ;

	  if (Boost != 0)
		{
		  write_i2c_frame( i2cHandle,  100 , (11<<6) + 6      , device_address) ;
		}
	  else
		{
		  write_i2c_frame( i2cHandle,  100 , (16<<6) + 9      , device_address) ;
		}

	  switch(Bw)
		{
	case TB_TUNER_Band_5MHz :
		  DEF_REG_6_5   ;
		  DEF_REG_7_5   ;
		  DEF_REG_8_5   ;
		  DEF_REG_9_5   ;
		  DEF_REG_10_5  ;
		  DEF_REG_11_5  ;

		  DEF_REG_16_5  ;
		  DEF_REG_17_5  ;
		  break ;

		case TB_TUNER_Band_6MHz :
		  DEF_REG_6_6   ;
		  DEF_REG_7_6   ;
		  DEF_REG_8_6   ;
		  DEF_REG_9_6   ;
		  DEF_REG_10_6  ;
		  DEF_REG_11_6  ;
		  DEF_REG_16_6  ;
		  DEF_REG_17_6  ;
		  break ;

		case TB_TUNER_Band_7MHz :
		  DEF_REG_6_7   ;
		  DEF_REG_7_7   ;
		  DEF_REG_8_7   ;
		  DEF_REG_9_7   ;
		  DEF_REG_10_7  ;
		  DEF_REG_11_7  ;
		  DEF_REG_16_7  ;
		  DEF_REG_17_7  ;
		  break ;

		default :
		  DEF_REG_6_8   ;
		  DEF_REG_7_8   ;
		  DEF_REG_8_8   ;
		  DEF_REG_9_8   ;
		  DEF_REG_10_8  ;
		  DEF_REG_11_8  ;
		  DEF_REG_16_8  ;
		  DEF_REG_17_8  ;
		  break ;
		}

	  if (Boost != 0)
		{
		  read_i2c_frame(i2cHandle,6,(U16 *)&val_msb, device_address);
		  read_i2c_frame(i2cHandle,7,(U16 *)&val_lsb, device_address);
		  timeout = ((U32)val_msb<<16) + val_lsb ;
		  timeout = timeout *85 ;
		  timeout = timeout >> 7;
		  val_msb = (timeout>>16)&0xFFFF;
		  val_lsb = (timeout&0xFFFF)    ;
		  write_i2c_frame(i2cHandle,6,val_msb, device_address);
		  write_i2c_frame(i2cHandle,7,val_lsb, device_address);
		}
	}
}

void set_adp_cfg(NEXUS_I2cHandle i2cHandle,S16 qam, S16 device_address)
{
  if(qam==2)
	{
	  DEF_REG_129_2	 ;
	  DEF_REG_130_2	 ;
	  DEF_REG_131_2	 ;
	  DEF_REG_132_2	 ;
	}
  else
	if(qam==1)
	  {
		DEF_REG_129_1  ;
		DEF_REG_130_1  ;
		DEF_REG_131_1  ;
		DEF_REG_132_1  ;
	  }
	else
	  {
		DEF_REG_129_0  ;
		DEF_REG_130_0  ;
		DEF_REG_131_0  ;
		DEF_REG_132_0  ;
	  }
}


void set_general_cfg(NEXUS_I2cHandle i2cHandle,struct channel_descriptor * cd ,
					 S16 use_tps,
					 S16 device_address)
{
  S16 tmp, val0 ;
  S16 Lg = 0;
  S16 seq = 0;
  S16 fs = (cd->nfft   < 0  );
  S16 gs = (cd->guard  < 0  );
  S16 is = (cd->invspec< 0  );

  S16 lnfft             = (fs) ? 1: cd->nfft   ;
  S16 lguard            = (gs) ? 0: cd->guard  ;
  S16 linvspec          = (is) ? 1: cd->invspec;

  S16 lnqam             = (cd->nqam             >= 0) ? cd->nqam             : 2;
  S16 lvit_alpha        = (cd->vit_alpha        >  0) ? cd->vit_alpha        : 1;
  S16 lvit_code_rate_hp = (cd->vit_code_rate_hp >  0) ? cd->vit_code_rate_hp : 2;
  S16 lvit_code_rate_lp = (cd->vit_code_rate_lp >  0) ? cd->vit_code_rate_lp : 2;
  S16 lvit_hrch         = (cd->vit_hrch         >= 0) ? cd->vit_hrch         : 0;
  S16 lvit_select_hp    = (cd->vit_select_hp    >= 0) ? cd->vit_select_hp    : 1;
  S16 vit_code_rate     = ((cd->vit_hrch==1) && (cd->vit_select_hp <=0)) ? lvit_code_rate_lp : lvit_code_rate_hp;
  lvit_select_hp        = (cd->vit_hrch != 1) ? 1 : lvit_select_hp ;



  tmp = (lvit_alpha&0x07) + ((lnqam &0x03) << 3) + ((lguard&0x03) << 5) + ((lnfft&0x01)<<7);
  write_i2c_frame(i2cHandle,  0 , tmp      , device_address) ;
  val0 = tmp ;

  tmp = (lvit_select_hp&0x01) + ((vit_code_rate&0x7)<<1) + ((lvit_hrch &0x01)<<4);
  write_i2c_frame(i2cHandle,  181 , tmp      , device_address) ;


  if (linvspec==1)  DEF_REG_21_1 ;
  else   	        DEF_REG_21_0 ;


  if ( ( fs) & (!gs) & (!is) ) seq = 2  ;
  if ( ( fs) & ( gs) & (!is) ) seq = 6  ;
  if ( ( fs) & (!gs) & ( is) ) seq = 5  ;
  if ( ( fs) & ( gs) & ( is) ) seq = 11 ;
  if ( (!fs) & (!gs) & (!is) ) seq = 0  ;
  if ( (!fs) & ( gs) & (!is) ) seq = 3  ;
  if ( (!fs) & (!gs) & ( is) ) seq = 1  ;
  if ( (!fs) & ( gs) & ( is) ) seq = 9  ;
  tmp =  (seq <<4) + ((use_tps&0x01) << 8) ;
  write_i2c_frame( i2cHandle,   5 , tmp     , device_address) ;

  read_i2c_frame( i2cHandle,  180 , (U16 *)&tmp  , device_address);
  tmp &= 0x000f;
  if(cd->nfft   <= 0  ) {
	tmp |= 0x2ff0;
  }
  else {
	Lg = ((cd->nfft == 0)? 64:256);
	if(cd->guard<0) Lg *= 8;
	else            Lg *= (1<<(cd->guard));
	Lg = Lg+Lg/2;
	tmp |= (Lg<<4);
  }
  write_i2c_frame( i2cHandle,  180,tmp,  device_address);
}


S16 read_search_status(NEXUS_I2cHandle i2cHandle,S16 *search_status_comb )
{
  DEF_DIB3000MC_I2C_ADDRESS ;
  S16 irq_pending       ;
  S16 event_search_fail ;
  S16 event_search_ok   ;
  S16 search_end        ;
  S16 device_address, i ;
  S16 tmp;

  read_i2c_frame( i2cHandle, 5, (U16 *)&tmp, DIB3000MC_I2C_ADDRESS[0]) ;


  for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	{


	  device_address = DIB3000MC_I2C_ADDRESS[i] ;
	  read_i2c_frame( i2cHandle, 511, (U16 *)&irq_pending, device_address) ;

	  event_search_fail = 0x1 & (irq_pending >> 0 );
	  event_search_ok   = 0x1 & (irq_pending >> 1 );
	  search_end        = event_search_fail | event_search_ok;
	  if (search_end)  (*search_status_comb) |= (1<<i) ;
	}
  if (((*search_status_comb) == (1<<DEF_NUM_DEMODS) -1) )
	return 1 ;
  else
	return 0 ;
}


NEXUS_Error get_channel (NEXUS_I2cHandle i2cHandle, struct channel_descriptor * cd, S16 scanboost )
{
  DEF_DIB3000MC_I2C_ADDRESS ;
  S16 device_address = 0;

  NEXUS_Error channel_status = NEXUS_INVALID_PARAMETER;
  S16 i, lock, lck ;
  S16 value_msb , value_lsb ;
  S16 test1 ;
  S16 test2, tmp=0 ;

  lock = 0 ;
  for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	{
	  read_i2c_frame( i2cHandle, 507, (U16 *)&tmp,  DIB3000MC_I2C_ADDRESS[i]  )         ;
	  lck = (tmp>>1)&0x01;
	  if (lck == 1) { device_address = DIB3000MC_I2C_ADDRESS[i];}
	  lock += lck ;
	}

  tmp = 0 ;
  if (lock >= 1)
	{
	  read_i2c_frame( i2cHandle, 414, (U16 *)&value_msb, device_address)     ;
	  read_i2c_frame( i2cHandle, 415, (U16 *)&value_lsb, device_address)     ;

	  if  ((value_msb &0x100) == 0)
		test1 = 0 ;
	  else if  ((value_msb  == 0x100) && (value_lsb  == 0x0000))
		test1 = 1 ;
	  else
		test1 = 2 ;

	  read_i2c_frame( i2cHandle, 21, (U16 *)&value_msb, device_address)     ;
	  read_i2c_frame( i2cHandle, 22, (U16 *)&value_lsb, device_address)     ;

	  if ((value_msb &0x100) == 0)
		test2 = 0 ;
	  else if ((value_msb  == 0x100) && (value_lsb  == 0x0000))
		test2 = 1 ;
	  else
		test2 = 2 ;

	  if ( ((test2 == 2)&&((test1==1) || (test1==0)))
		   ||
		   ((test2 == 0)&&((test1==1) || (test1==2))) )  cd->invspec = 1;
	  else cd->invspec = 0;

	  read_i2c_frame( i2cHandle, 458, (U16 *)&tmp, device_address)     ;
	  cd->nfft      = (tmp>>8)  & 0x01;
	  cd->guard     = tmp & 0x03;
	  read_i2c_frame( i2cHandle, 5, (U16 *)&tmp,  device_address )         ;


	  if(((tmp >> 8) & 0x01) == 1)
		{
          channel_status = NEXUS_SUCCESS;

		  read_i2c_frame( i2cHandle, 458, (U16 *)&tmp, device_address)     ;
		  cd->nqam      = (tmp>>13) & 0x03;
		  cd->vit_hrch  = (tmp>>12) & 0x01;
		  cd->vit_alpha = (tmp>>9)  & 0x07;
		  cd->vit_code_rate_hp = (tmp>>5)  & 0x07;
		  cd->vit_code_rate_lp = (tmp>>2)  & 0x07;
		}
	}
    else
    {
       //printf("no tunner is lock !\r\n");
    }
    printf("lock=%d\r\n",lock);




  if ((scanboost == 0) &&(lock >= 1))
	{
	  set_timing(i2cHandle,1, cd->nfft, cd->Bw, device_address );
	  if (lock >=1) set_WithKnownParams(i2cHandle,cd);
	}
   // printf("\n*******************************************************\n");
    //printf("\nin front_end_dib3000m.c get_channel channel_status = %d\n",channel_status);
   // printf("\n*******************************************************\n");
    return channel_status;
}



void  set_WithKnownParams (NEXUS_I2cHandle i2cHandle, struct channel_descriptor * cd  )
{
  DEF_DIB3000MC_I2C_ADDRESS ;
  S16 device_address ;

  S16 i, regvalue;

  for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	{
	  device_address = DIB3000MC_I2C_ADDRESS[i] ;


	  set_general_cfg(i2cHandle, cd,  0,  device_address);
	  set_impulse_noise (i2cHandle,0,  cd->nfft, cd->Bw,  device_address );

	  read_i2c_frame ( i2cHandle, 0, (U16 *)&regvalue, device_address);
	  write_i2c_frame( i2cHandle, 0, regvalue+512, device_address);
	  write_i2c_frame( i2cHandle, 0, regvalue, device_address);
	}

  usleep(30000);

  for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	{
	  device_address = DIB3000MC_I2C_ADDRESS[i];
	  set_impulse_noise (i2cHandle,DEF_IMPULSE_NOISE_MODE,
						 cd->nfft, cd->Bw,  device_address );


	  write_i2c_frame( i2cHandle, 29, 0x1073, device_address);
	  set_adp_cfg(i2cHandle, cd->nqam, device_address) ;


	  if (cd->nfft == 1)
		{
		  write_i2c_frame( i2cHandle, 26,38528,  device_address);
		  write_i2c_frame( i2cHandle, 33,8,  device_address);
		}
	  else
		{
		  write_i2c_frame( i2cHandle, 26,30336, device_address);
		  write_i2c_frame( i2cHandle, 33,6, device_address);
		}
	}

}



NEXUS_Error dib_lockstatus(NEXUS_I2cHandle i2cHandle)
{
	U16  lock_status;
	NEXUS_Error isLock;

	lock_status  = dib3000_CheckLock(i2cHandle,1);

	if(lock_status == 1)
	      isLock = NEXUS_SUCCESS;
	else
	      isLock = NEXUS_INVALID_PARAMETER;


	return isLock;
}

S16 dib3000_CheckLock(NEXUS_I2cHandle i2cHandle,U8 bShow)
{
    S16  regvalue;
    S16 bLock,mpeg_data_lock,agc_lock,carrier_lock,tps_sync_lock,mpeg_sync_lock;
    S16 device_address;
    DEF_DIB3000MC_I2C_ADDRESS ;
    S16 i;

    bLock = 0;
    for(i=0;i<DEF_NUM_DEMODS;i++)
    {
        device_address = DIB3000MC_I2C_ADDRESS[i];

        read_i2c_frame(i2cHandle, 509, (U16 *)&regvalue  , device_address) ;
        agc_lock       = (regvalue>>15)&0x01;
        carrier_lock   = (regvalue>>13)&0x01;
        tps_sync_lock  = (regvalue>>4 )&0x01;
        mpeg_sync_lock = (regvalue>>7 )&0x01;
        mpeg_data_lock = (regvalue>>6 )&0x01;


        if(bShow)
        {
            printf("**********************************************\r\n");
            printf("agc_lock_%d       = %d \r\n",  i+1, agc_lock);
            printf("carrier_lock_%d   = %d \r\n",  i+1, carrier_lock);
            printf("tps_sync_lock_%d  = %d \r\n",  i+1, tps_sync_lock);
            printf("mpeg_sync_lock_%d = %d \r\n",  i+1, mpeg_sync_lock);
            printf("mpeg_data_lock_%d = %d \r\n",  i+1, mpeg_data_lock);
            printf("**********************************************\r\n");
        }
        if(tps_sync_lock && mpeg_data_lock && mpeg_sync_lock )
        {
            bLock = 1;
        }
    }

    return bLock;

}


void dib_show_signal_strength_quality(NEXUS_I2cHandle i2cHandle)
{
	S16 strength,quality,strength1,quality1,strength2,quality2;

	dib_signal_strength_quality(i2cHandle,&strength,&quality,&strength1,&strength2,&quality1,&quality2);

	//printf("**********************************************\r\n");
	//printf("strength       = %d \r\n",strength);
	//printf("quality   = %d \r\n",  quality);
	//printf("**********************************************\r\n");

}

#define DEF_gain_slope_dB  1456
#define DEF_gain_delta_dB  -193
#define DEF_agc_ref_dB     -14

#define DEF_SampFreq_KHz     27700

void get_rf_power(NEXUS_I2cHandle i2cHandle,S16 *strength1,S16 *strength2)
{
    DEF_DIB3000MC_I2C_ADDRESS ;
    S16  i;
	unsigned short int n_agc_power = 0xFFFF;
	unsigned short int rf_power =0xFFFF ;
	double ad_power_dB, minor_power,RF_POWER;
	S16  device_address;

#if 0
	for ( i=0; i<DEF_NUM_DEMODS  ; i++)
	{
		device_address = DIB3000MC_I2C_ADDRESS[i];



		read_i2c_frame( i2cHandle, 325, &n_agc_power, device_address ) ;

		read_i2c_frame( i2cHandle, 328, &rf_power, device_address ) ;

		printf("n_agc_power = %d\n",n_agc_power);
		printf("rf_power    = %d\n",rf_power);

		if (n_agc_power == 0)
			n_agc_power = 1;
	ad_power_dB = 10 * log10( (double)(n_agc_power) / (double)(1<<16));

			printf("ad_power_dB=%f\n",ad_power_dB);

	minor_power = ad_power_dB - DEF_agc_ref_dB ;
	RF_POWER = -DEF_gain_slope_dB * (double)rf_power/(double)(1<<16) + DEF_gain_delta_dB + minor_power;
	printf("RF POWER =%3.7g dbm\n",RF_POWER);
	}


#else
    U16  n_agc_power_msb, n_agc_power_lsb;
   double rf_power_dB;

    for ( i=0; i<DEF_NUM_DEMODS  ; i++)
    {
        device_address = DIB3000MC_I2C_ADDRESS[i];

        read_i2c_frame( i2cHandle, 392, &rf_power, device_address ) ;
        read_i2c_frame(i2cHandle,388, &n_agc_power_msb,  device_address);
        read_i2c_frame(i2cHandle,389, &n_agc_power_lsb,  device_address);

        n_agc_power = ((double)(n_agc_power_msb&0xf))*((unsigned long)1<<16)+n_agc_power_lsb;

		//printf("n_agc_power = %d\n",n_agc_power);
		//printf("rf_power    = %d\n",rf_power);


		//printf("n_agc_power=%f\n",n_agc_power);

        if (n_agc_power == 0 )
            n_agc_power = 1 ;
        ad_power_dB  = 10 * log10 ((double)n_agc_power/(double)((unsigned long)1<<20)) ;
		//printf("ad_power_dB=%f\n",ad_power_dB);

        minor_power = ad_power_dB - DEF_agc_ref_dB ;

        rf_power_dB  = 10 * log10 ((double)rf_power/(double)((unsigned long)1<<16)) ;

		//printf("minor_power=%f\n",minor_power);
		//printf("rf_power_dB=%f\n",rf_power_dB);
		//printf("signal lever = %f\n",((65535-rf_power)/65535.0)*100.0);


	 RF_POWER = rf_power_dB*(-66.66)-586;



			//RF_POWER = -DEF_gain_slope_dB * (double)rf_power/(double)(1<<16) + DEF_gain_delta_dB + minor_power;
	//printf("RF POWER =%3.7g dbm\n",RF_POWER);
			if(i==0)
			{
				*strength1=(signed short int) (RF_POWER-0.5);
				if(*strength1 < -120)
				{
					*strength1=-120;
				}
				else if(*strength1 > 0)
				{
					*strength1=0;
				}

			}
			else
			{
				*strength2=(signed short int) (RF_POWER-0.5);
				//printf("strength2=%d\n",*strength2);
				if(*strength2 < -120)
				{
					*strength2=-120;
				}
				else if(*strength2 > 0)
				{
					*strength2=0;
				}
				//printf("strength2=%d\n",*strength2);

			}

    }


		//printf("strength1 =%d dbm\n",*strength1);

		//printf("strength2 =%d dbm\n",*strength2);

#endif











}









void dib_signal_strength_quality(NEXUS_I2cHandle i2cHandle,S16 *strength, S16 *quality, S16 *strength1,S16 *strength2, S16 *quality1, S16 *quality2)
{
	S16 strength_1,strength_2,quality_1,quality_2;


	//S16 strength_1_1,strength_2_2;
	get_rf_power(i2cHandle,&strength_1,&strength_2);
	//dib3000_get_signal_strength(i2cHandle,&strength_1,&strength_2);

	dib3000_get_signal_cn(i2cHandle,&quality_1,&quality_2);

	if(quality_1 == 0)
		strength_1 = -120;
	if(quality_2 == 0)
		strength_2 = -120;
	if(strength != NULL)
	{
		if(strength_1 >= strength_2)
		{
			*strength = strength_1;
		}
		else
		{
			*strength = strength_2;
		}
	}

#if defined(SET_AGC)

//	strength_1 = dib_get_signal_strength_dBmEx(i2cHandle,strength_1);
//	strength_2 = dib_get_signal_strength_dBmEx(i2cHandle,strength_2);
#else
//	strength_1 = dib_get_signal_strength_dBm(i2cHandle,strength_1);
//	strength_2 = dib_get_signal_strength_dBm(i2cHandle,strength_2);
#endif

	if(strength1 != NULL)
	{
		*strength1 = strength_1;
	}
	if(strength2 != NULL)
	{
		*strength2 = strength_2;
	}

	if(quality1 != NULL)
	{
		*quality1 = quality_1;
	}
	if(quality2 != NULL)
	{
		*quality2 = quality_2;
	}
   if(quality != NULL)
   {
	if(quality_1 >= quality_2)
	{
		*quality = quality_1;
	}
	else
	{
		*quality = quality_2;
	}
   }
}





void dib3000_get_signal_strength(NEXUS_I2cHandle i2cHandle,S16 *strength1,S16 *strength2)
{
    DEF_DIB3000MC_I2C_ADDRESS ;
    S16  i;
    S16  equal_signal_mant;
    S16  device_address;
    U16  n_agc_power_msb, n_agc_power_lsb, rf_power;
    double n_agc_power;
    double ad_power_dB, minor_power ;

    for ( i=0; i<DEF_NUM_DEMODS  ; i++)
    {
        device_address = DIB3000MC_I2C_ADDRESS[i];

        read_i2c_frame( i2cHandle, 392, &rf_power, device_address ) ;
        read_i2c_frame(i2cHandle,388, &n_agc_power_msb,  device_address);
        read_i2c_frame(i2cHandle,389, &n_agc_power_lsb,  device_address);
		//printf("signal=%f\n",(65535-rf_power)/65535.0);

        n_agc_power = ((double)(n_agc_power_msb&0xf))*((unsigned long)1<<16)+n_agc_power_lsb;


		//printf("n_agc_power=%f\n",n_agc_power);

        if (n_agc_power == 0 )
            n_agc_power = 1 ;
        ad_power_dB  = 10 * log10 ((double)n_agc_power/(double)((unsigned long)1<<20)) ;

        minor_power = ad_power_dB - Global_agc_ref_dB ;
		//printf("minor_power=%f\n",minor_power);
        #define MAX_AGC_DB 97
        #define MIN_AGC_DB 10

           // equal_signal_mant = (-Global_gain_slope_dB*(double)rf_power/(double)((unsigned long)1<<16)+Global_gain_delta_dB)+minor_power ;

       // equal_signal_mant=(equal_signal_mant+100)/100*0xffff;


		equal_signal_mant = (MAX_AGC_DB + (-Global_gain_slope_dB*(double)rf_power/(double)((unsigned long)1<<16)+Global_gain_delta_dB)+minor_power) * 100 / (MAX_AGC_DB - MIN_AGC_DB);



        if(equal_signal_mant < 0)
        {
            equal_signal_mant = 0;
        }

        if(equal_signal_mant > 255)
        {
            equal_signal_mant = 254;
        }


	if(i == 0)
	{
		*strength1 = equal_signal_mant;
	}
	else
	{
		*strength2 = equal_signal_mant;
	}


    }


}


void dib3000_get_signal_cn(NEXUS_I2cHandle i2cHandle,S16 *quality1,S16 *quality2)
{
    DEF_DIB3000MC_I2C_ADDRESS ;
    S16  val, i;
    S16 device_address;
    S16 equal_noise_exp  = 0;
    S16 equal_noise_mant = 0;
    S16 equal_signal_exp = 0;
    S16 equal_signal_mant = 0;
    double CoN[DEF_NUM_DEMODS] ;



    for ( i=0; i<DEF_NUM_DEMODS  ; i++)
    {
        device_address = DIB3000MC_I2C_ADDRESS[i];

        read_i2c_frame( i2cHandle, 483, (U16 *)&val, device_address) ;
        equal_noise_mant = (val >>4)&0xFF;
        equal_noise_exp  = ((val&0xF) << 2);
        read_i2c_frame(i2cHandle,  484, (U16 *)&val, device_address) ;
        equal_noise_exp  += ((val >> 14)&0x3);
        equal_signal_mant = (val >>6)&0xFF;
        equal_signal_exp  = (val &0x3F);




        if ((equal_noise_exp&0x20) == 0x20)
        {
            equal_noise_exp|= 0xFFC0 ;
        }
        if ((equal_signal_exp&0x20) == 0x20)
        {
            equal_signal_exp |= 0xFFC0 ;
        }
        CoN[i] = 10.0*log10( (double)equal_signal_mant /  (double)equal_noise_mant * pow(2, (double)equal_signal_exp - (double)equal_noise_exp));

       // printf("%f\n",( (double)equal_signal_mant /  (double)equal_noise_mant * pow(2, (double)equal_signal_exp - (double)equal_noise_exp)));

        if(i == 0)
    	{
    		*quality1 = (S16)CoN[i];

    	}
    	else
    	{
    		*quality2 = (S16)CoN[i];
    	}
    }

	//printf("quality1=%d\n",*quality1);
	//printf("quality2=%d\n",*quality2);



/*
        #define MAX_CN 100
    	*quality1 = (*quality1 * 255 / MAX_CN);
    	*quality2 = (*quality2 * 255 / MAX_CN);
		if(*quality1<0)
		{
          *quality1=0;
		}
		if(*quality2<0)
		{
          *quality2=0;
		}
		*/
}


S16 dib_get_signal_strength_dBmEx(NEXUS_I2cHandle i2cHandle,S16 strength)
{
	S16 strength_dBm = 0;
	if(strength >88)
	{
		strength_dBm = 0 -random_num(33);
	}
	else
	{
		switch(strength)
		{
			case 88:
				switch(random_num(7))
				{
					case 0:
						strength_dBm = -33;
						break;
					case 1:
						strength_dBm = -34;
						break;
					case 2:
						strength_dBm = -35;
						break;
					case 3:
						strength_dBm = -36;
						break;
					case 4:
						strength_dBm = -37;
						break;
					case 5:
						strength_dBm = -38;
						break;
					case 6:
						strength_dBm = -39;
						break;
				}
				break;

			case 87:
				switch(random_num(4))
				{
					case 0:
						strength_dBm = -48;
						break;
					case 1:
						strength_dBm = -49;
						break;
					case 2:
						strength_dBm = -50;
						break;
					case 3:
						strength_dBm = -51;
						break;
				}
				break;

			case 86:
				switch(random_num(3))
				{
					case 0:
						strength_dBm = -58;
						break;
					case 1:
						strength_dBm = -59;
						break;
					case 2:
						strength_dBm = -60;
						break;
				}
				break;

			case 85:
				strength_dBm = -61;
				break;
			case 84:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -62;
						break;
					case 1:
						strength_dBm = -63;
						break;
				}
				break;

			case 83:
				strength_dBm = -64;
				break;
			case 82:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -65;
						break;
					case 1:
						strength_dBm = -66;
						break;
				}
				break;

			case 81:
				strength_dBm = -67;
				break;

			case 80:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -68;
						break;
					case 1:
						strength_dBm = -69;
						break;
				}
				break;

			case 79:
				strength_dBm = -70;
				break;
			case 78:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -71;
						break;
					case 1:
						strength_dBm = -72;
						break;
				}
				break;

			case 77:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -73;
						break;
					case 1:
						strength_dBm = -74;
						break;
				}
				break;
			case 76:
				strength_dBm = -75;
				break;
			case 75:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -76;
						break;
					case 1:
						strength_dBm = -77;
						break;
				}
				break;
			case 74:
				strength_dBm = -78;
				break;
			case 73:
				strength_dBm = -79;
				break;
			case 72:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -80;
						break;
					case 1:
						strength_dBm = -81;
						break;
				}
				break;
			case 71:
				strength_dBm = -82;
				break;
			case 70:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -83;
						break;
					case 1:
						strength_dBm = -84;
						break;
				}
				break;
			case 69:
				strength_dBm = -85;
				break;
			case 68:
				strength_dBm = -86;
				break;
			case 67:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -87;
						break;
					case 1:
						strength_dBm = -88;
						break;
				}
				break;
			case 66:
				strength_dBm = -89;
				break;
			case 65:
				strength_dBm = -90;
				break;
			case 64:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -91;
						break;
					case 1:
						strength_dBm = -92;
						break;
				}
				break;
			case 63:
				strength_dBm = -93;
				break;
			case 62:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -94;
						break;
					case 1:
						strength_dBm = -95;
						break;
				}
				break;
			case 61:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -96;
						break;
					case 1:
						strength_dBm = -97;
						break;
				}
				break;
			case 60:
				switch(random_num(3))
				{
					case 0:
						strength_dBm = -98;
						break;
					case 1:
						strength_dBm = -99;
						break;
					case 2:
						strength_dBm = -100;
						break;
				}
				break;
			default:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -102;
						break;
					case 1:
						strength_dBm = -103;
						break;
				}
				break;
			}

	}
	return strength_dBm;

}
S16 dib_get_signal_strength_dBm(NEXUS_I2cHandle i2cHandle,S16 strength)
{
	S16 strength_dBm = 0;
	if(strength >=116)
	{
		strength_dBm = 0 -random_num(22);
	}
	else
	{
		switch(strength)
		{
			case 130:
			case 129:
			case 128:
			case 127:
			case 126:
			case 125:
			case 124:
			case 123:
			case 122:
			case 121:
			case 120:
			case 119:
			case 118:
			case 117:
			case 116:
			case 115:
				switch(random_num(4))
				{
					case 0:
						strength_dBm = -20;
						break;
					case 1:
						strength_dBm = -21;
						break;
					case 2:
						strength_dBm = -22;
						break;
					case 3:
						strength_dBm = -23;
						break;
				}
				break;
			case 114:
				switch(random_num(5))
				{
					case 0:
						strength_dBm = -24;
						break;
					case 1:
						strength_dBm = -25;
						break;
					case 2:
						strength_dBm = -26;
						break;
					case 3:
						strength_dBm = -27;
						break;
					case 4:
						strength_dBm = -28;
						break;
				}
				break;
			case 113:
				switch(random_num(3))
				{
					case 0:
						strength_dBm = -29;
						break;
					case 1:
						strength_dBm = -30;
						break;
					case 2:
						strength_dBm = -31;
						break;
				}
				break;
			case 112:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -32;
						break;
					case 1:
						strength_dBm = -33;
						break;
				}
				break;
			case 111:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -34;
						break;
					case 1:
						strength_dBm = -35;
						break;
				}
				break;
			case 110:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -36;
						break;
					case 1:
						strength_dBm = -37;
						break;
				}
				break;
			case 109:
				strength_dBm = -38;
					break;
			case 108:
				strength_dBm = -39;
					break;
			case 107:
				strength_dBm = -40;
					break;
			case 106:
				strength_dBm = -41;
					break;
			case 105:
				strength_dBm = -42;
					break;
			case 104:
				strength_dBm = -43;
					break;
			case 103:
				strength_dBm = -44;
					break;
			case 102:
			case 101:
			case 100:
			case 99:
			case 98:
			case 97:
			case 96:
			case 95:
			case 94:
				strength_dBm = -45;
					break;
			case 93:
			case 92:
				strength_dBm = -46;
					break;
			case 91:
				strength_dBm = -47;
					break;
			case 90:
			case 89:
				strength_dBm = -48;
					break;
			case 88:
				strength_dBm = -49;
					break;
			case 87:
				strength_dBm = -50;
					break;
			case 86:
			case 85:
				strength_dBm = -51;
					break;
			case 84:
				strength_dBm = -52;
					break;
			case 83:
			case 82:
				strength_dBm = -53;
					break;
			case 81:
			case 80:
				strength_dBm = -54;
					break;
			case 79:
			case 78:
				strength_dBm = -55;
					break;
			case 77:
			case 76:
			case 75:
				strength_dBm = -56;
					break;
			case 74:
			case 73:
				strength_dBm = -57;
					break;
			case 72:
			case 71:
			case 70:
				strength_dBm = -58;
					break;
			case 69:
			case 68:
			case 67:
			case 66:
				strength_dBm = -59;
					break;
			case 65:
			case 64:
				strength_dBm = -60;
					break;
			case 63:
			case 62:
			case 61:
				strength_dBm = -61;
					break;
			case 60:
			case 59:
			case 58:
				strength_dBm = -62;
					break;
			case 57:
			case 56:
			case 55:
				strength_dBm = -63;
					break;
			case 54:
			case 53:
				strength_dBm = -64;
					break;
			case 52:
			case 51:
				strength_dBm = -65;
					break;
			case 50:
			case 49:
				strength_dBm = -66;
					break;
			case 48:
				strength_dBm = -67;
					break;
			case 47:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -68;
						break;
					case 1:
						strength_dBm = -69;
						break;
				}
				break;
			case 46:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -70;
						break;
					case 1:
						strength_dBm = -71;
						break;
				}
				break;
			case 45:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -72;
						break;
					case 1:
						strength_dBm = -73;
						break;
				}
				break;
			case 44:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -74;
						break;
					case 1:
						strength_dBm = -75;
						break;
				}
				break;
			case 43:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -76;
						break;
					case 1:
						strength_dBm = -77;
						break;
				}
				break;
			case 42:
				switch(random_num(3))
				{
					case 0:
						strength_dBm = -78;
						break;
					case 1:
						strength_dBm = -79;
						break;
					case 2:
						strength_dBm = -80;
						break;
				}
				break;
			case 41:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -81;
						break;
					case 1:
						strength_dBm = -82;
						break;
				}
				break;
			case 40:
				switch(random_num(3))
				{
					case 0:
						strength_dBm = -83;
						break;
					case 1:
						strength_dBm = -84;
						break;
					case 2:
						strength_dBm = -85;
						break;
				}
				break;
			case 39:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -86;
						break;
					case 1:
						strength_dBm = -87;
						break;
				}
				break;
			case 38:
				strength_dBm = -88;
					break;
			case 37:
				strength_dBm = -89;
					break;
			case 36:
				strength_dBm = -90;
					break;
			case 35:
			case 34:
			case 33:
			case 32:
				strength_dBm = -91;
					break;
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
			case 16:
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
				strength_dBm = -92;
					break;
			case 10:
				strength_dBm = -93;
					break;
			case 9:
				strength_dBm = -94;
					break;
			case 8:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -95;
						break;
					case 1:
						strength_dBm = -96;
						break;
				}
			case 7:
				strength_dBm = -97;
				break;
			case 6:
				strength_dBm = -98;
					break;
			case 5:
				strength_dBm = -99;
					break;
			case 4:
				strength_dBm = -100;
					break;
			case 3:
				strength_dBm = -101;
					break;
			default:
				switch(random_num(2))
				{
					case 0:
						strength_dBm = -102;
						break;
					case 1:
						strength_dBm = -103;
						break;
				}

		}
	}

	return strength_dBm;
}



void CloseDib3000(void)
{
	NEXUS_Gpio_Close(pin);
}


extern NEXUS_I2cHandle i2c;


void GetDibcomTunerParams(S16* Locked, U32* Quality, U32* Strength, S16* Strength1, S16* Strength2, S16* Quality1, S16* Quality2)
{

	S16 strength,quality;
	S16 strength1,strength2,quality1,quality2;

	if(g_Status.TunerType ==0 )
	{
		* Locked=0;
		* Quality=0;
		* Strength=120;
		* Strength1=-120;
		* Strength2=-120;
		* Quality1=0;
		* Quality2=0;
		return;
	}




	if(Locked != NULL)  /* modified by zyf  */
	{
	      *Locked = dib3000_CheckLock(i2c,0);
	}

	dib_signal_strength_quality(i2c,&strength,&quality,&strength1,&strength2,&quality1,&quality2);
	if(Strength1 != NULL)
	{
		*Strength1 = strength1;
	}
	if(Strength2 != NULL)
	{
		*Strength2 = strength2;
	}
	if(Quality1 != NULL)
	{
		*Quality1= quality1;
	}
	if(Quality2 != NULL)
	{
		*Quality2= quality2;
	}


	if(Quality1 != NULL)
	{
		*Quality1 = 0;
		if(quality1 >= 15)
		{
			quality1 = quality1 -15;
		}
		if(quality1 >= 100)
		{
			quality1 = 100;
		}
		if(Locked !=NULL)
		{
			if(*Locked)
			{
				*Quality1 = quality1;
			}
		}
	}
	if(Quality2 != NULL)
	{
		*Quality2 = 0;
		if(quality2 >= 15)
		{
			quality2 = quality2 -15;
		}
		if(quality2 >= 100)
		{
			quality2 = 100;
		}
		if(Locked !=NULL)
		{
		   if(*Locked)
		   {
		   	   *Quality2 = quality2;
		   }
		}
	}
	if(Quality != NULL)
	{
		*Quality = 0;
		if(quality >= 15)
		{
			quality = quality -15;
		}
		if(quality >= 100)
		{
			quality = 100;
		}
		if(Locked !=NULL)
		{
		    if(*Locked)
			*Quality = quality;
		}
	}

	if(Strength != NULL)
	{
		*Strength = 0;
		S16 strength_1;

		strength_1 = strength;
		if(strength_1 < -100)
			strength_1 = -100;
		if(Locked !=NULL)
		{
		   if(*Locked)
			*Strength = 100 + strength_1;
		}
		if(strength <= 85)
			strength = strength + 15;
		if(strength >= 100)
			strength = 100;
		if(Locked !=NULL)
		{
		if(*Locked)
			*Strength = strength;
		}

	}
}
// tux add <--





