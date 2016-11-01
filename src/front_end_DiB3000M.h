#ifndef front_end_DiB3000M_h
#define front_end_DiB3000M_h

#include <stdio.h>
#include <stdlib.h>
#include "DiB3000M_FrontEnd_HwCfg.h"

typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short int U16;
typedef signed short int S16;
typedef unsigned long int U32;
typedef signed long int S32;

typedef enum
{
    TUNER_THOMSON_CABLE,
    TUNER_THOMSON_COFDM,
    TUNER_PHILIPS_COFDM,
    TUNER_GRUNDIG_COFDM,
    TUNER_ALPS_COFDM,
    TUNER_THOMSON_CABLE_US,
    TUNER_SAMSUNG_COFDM,
    TUNER_ALPS_CABLE,
    TUNER_PANASONIC_ENV77H04D5,
    TUNER_THOMSON_COFDM_NERA,
    TUNER_TEMIC,
    TUNER_MICROTUNE_7403DT5,
    TUNER_MC44C801,
    TUNER_PANASONIC_ENV57H13D5,
    TUNER_PANASONIC_ENV57H12D5,
    TUNER_PANASONIC_ENV57H10D8,
    TUNER_PANASONIC_ENV57H11D8,
    TUNER_PANASONIC_ENV57H01D8,
    TUNER_PANASONIC_ENV57H15D8F,
    TUNER_PANASONIC_ENV77H17D7,
    TUNER_MICROTUNE_MT2060,
    TUNER_MICROTUNE_MT2060_BGA420,
    TUNER_SAMSUNG_DTOS333IH102,
    TUNER_SHARP_VA1H1ED6265,
    TUNER_SHENGYI_TMD2211,
    TUNER_RFAGCIN_UNKNOWN
} TUNER_TYPE;

typedef enum
{
    HIGH_Z,
    MPEG2_PARALLEL_GATED_CLOCK,
    MPEG2_PARALLEL_CONTINUOUS_CLOCK,
    MPEG2_SERIAL,
    MPEG2_SLAVE
} OUTPUT_TYPE;

#define DEF_SYS 0
#define Fixed_Reception 0
#define Portable_Reception 1
#define Mobile_Reception 2
#define Auto_Reception 3
#define IN_NoCompensation 0
#define IN_Enhanced 1
#define IN_OldAlgo 2
#define DiBcom_ID 0x01B3
#define DEF_WAITING_LOCK_AGC_us 40000
#define DEF_WAITING_TUNER_CFG_us 1000

typedef struct channel_descriptor
{
    U32 RF_KHz;
    S16 Bw;
    S16 nfft;
    S16 guard;
    S16 invspec;
    S16 nqam;
    S16 vit_hrch;
    S16 vit_select_hp;
    S16 vit_alpha;
    S16 vit_code_rate_hp;
    S16 vit_code_rate_lp;
} sTnimChannel, *PCHANNEL;

typedef enum TB_TUNER_Band_e
{
    TB_TUNER_Band_1_5MHz = 1500,
    TB_TUNER_Band_1_75MHz = 1750,
    TB_TUNER_Band_2MHz = 2000,
    TB_TUNER_Band_3MHz = 3000,
    TB_TUNER_Band_4MHz = 4000,
    TB_TUNER_Band_5MHz = 5000,
    TB_TUNER_Band_6MHz = 6000,
    TB_TUNER_Band_7MHz = 7000,
    TB_TUNER_Band_8MHz = 8000
} TB_TUNER_Band_t;

S32 front_end_init (NEXUS_I2cHandle i2cHandle, S16 diversity_on);
void tune_channel (NEXUS_I2cHandle i2cHandle, struct channel_descriptor *cd, S16 tps_auto_search, S16 scanBoost ) ;
void set_WithKnownParams (NEXUS_I2cHandle i2cHandle, struct channel_descriptor *cd) ;
S16 read_search_status (NEXUS_I2cHandle i2cHandle, S16 *search_status_comb) ;
S16 set_rf_freq_thomson_cable_eu (U32 rf_freq_KHz, S16 address);
S16 set_rf_freq_thomson_cable_us (U32 rf_freq_KHz, S16 address);
S16 set_rf_freq_panasonic_cofdm (U32 rf_freq_KHz, S16 address);
S16 set_rf_freq_temic (U32 rf_freq_KHz, S16 address, S16 bandwidth);
S16 set_rf_freq_microtune (U32 rf_freq_KHz, S16 address);
S16 set_rf_freq_panasonic_cofdm_ENV57H1XD5 (NEXUS_I2cHandle i2cHandle, U32 rf_freq_KHz, S16 address);
S16 set_rf_freq_panasonic_cofdm_ENV57H15D8F (U32 rf_freq_KHz, S16 address);
S16 set_rf_freq_panasonic_cofdm_ENV57P01D8 (U32 rf_freq_KHz, S16 address, S16 i);
S16 set_rf_freq_panasonic_cofdm_ENV77H17D7 (U32 rf_freq_KHz, S16 address);
S16 set_rf_freq_samsung_DTOS333IH102 (U32 rf_freq_KHz, S16 address);
S16 set_rf_freq_SHARP_VA1H1ED6265 (U32 rf_freq_KHz, U8 BandWidth, S16 address);
S16 set_rf_freq_shengyi_TMD2211 (U32 rf_freq_KHz, S16 address);
S16 set_rf_freq_MT2060 (U32 rf_freq_KHz, S16 address, S16 demod_num);
void set_general_cfg (NEXUS_I2cHandle i2cHandle, struct channel_descriptor *cd, S16 use_tps, S16 device_address);
void set_timing (NEXUS_I2cHandle i2cHandle, S16 UpdateOffset, S16 nfft, S16 bW, S16 device_address);
void set_init_auto_search (NEXUS_I2cHandle i2cHandle, S16 Bw);
void set_diversity (NEXUS_I2cHandle i2cHandle, S16 divin_on, S16 divout_on, S16 device_address);
S16 set_agc_loop_Bw(NEXUS_I2cHandle i2cHandle ,S16 BwSelect,S16 device_address);
void read_monitoring (U16 *rf_power, S16 *nfft, S16 *guard, S16 *fft_positoning, S16 *agc_lock, S16 *carrier_lock,
                            S16 *tps_sync_lock, S16 *mpeg_sync_lock, S16 *mpeg_data_lock, S16 *equal_noise_exp,
                            S16 *equal_noise_mant, S16 *equal_signal_exp, S16 *equal_signal_man, S16 *mer_mant,
                            S16 *mer_exp, S16 *ber_msb, S16 *ber_lsb, S16 *PacketErrors, S16 *PacketErrorCount,
                            U16 *tim_offset_msb, U16 *tim_offset_lsb, S16 *caroffset, S16 device_address );
void set_impulse_noise (NEXUS_I2cHandle i2cHandle, S16 mode, S16 nfft, S16 Bw, S16 device_address);
void set_adp_cfg (NEXUS_I2cHandle i2cHandle, S16 nqam, S16 device_address);
void set_powerdown (S16 value, S16 device_address);
void set_mobile_mode (NEXUS_I2cHandle i2cHandle, S16 mobile_mode, S16 device_address);
void set_diversity_mode (NEXUS_I2cHandle i2cHandle, S16 diversity_on);
void set_output_mode (NEXUS_I2cHandle i2cHandle, S16 value);
int dib_gets_modulation_para (int dev_num, U16 *modulation, U16 *gua_num, U16 *fec_rate, U16 *fft_num);
int dib_mychecklock (S16 dev_num);
S16 dib3000_CheckLock (NEXUS_I2cHandle i2cHandle, U8 bShow);
unsigned char dib_init (NEXUS_I2cHandle i2cHandle);
NEXUS_Error dib_tune (NEXUS_I2cHandle i2cHandle, U32 freq, S16 bw);
NEXUS_Error get_channel (NEXUS_I2cHandle i2cHandle, struct channel_descriptor *cd, S16 scanboost);
NEXUS_Error dib_lockstatus (NEXUS_I2cHandle i2cHandle);
void dib_show_signal_strength_quality (NEXUS_I2cHandle i2cHandle);
void dib_signal_strength_quality (NEXUS_I2cHandle i2cHandle, S16 *strength, S16 *quality,
                                                            S16 *strength1, S16 *strength2, S16 *quality1, S16 *quality2);
void dib3000_get_signal_strength (NEXUS_I2cHandle i2cHandle, S16 *strength1, S16 *strength2);
void dib3000_get_signal_cn (NEXUS_I2cHandle i2cHandle, S16 *quality1, S16 *quality2);
S16 dib_get_signal_strength_dBm (NEXUS_I2cHandle i2cHandle, S16 strength);
S16 dib_get_signal_strength_dBmEx (NEXUS_I2cHandle i2cHandle, S16 strength);
S16 dib9090_get_signal_strength_dBmEx (U8 ID, S16 strength);
#endif

