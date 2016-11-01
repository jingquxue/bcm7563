#ifndef KWT_DIB3000_H
#define KWT_DIB3000_H
#include "nexus_i2c.h"
#include "front_end_DiB3000M.h"



unsigned char dib_init(NEXUS_I2cHandle i2cHandle);
NEXUS_Error dib_tune(NEXUS_I2cHandle i2cHandle, U32 freq, S16 bw);
void dib_show_signal_strength_quality(NEXUS_I2cHandle i2cHandle);

void CloseDib3000(void);
void GetDibcomTunerParams(S16* Locked, U32* Quality, U32* Strength, S16* Strength1, S16* Strength2, S16* Quality1, S16* Quality2);
#endif
