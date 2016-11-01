#ifndef KWT_NANDFLASH_H
#define KWT_NANDFLASH_H

#include <sys/types.h>

int NandErase (unsigned int DeviceId, unsigned int offset, unsigned int bytes);
int NandRead (unsigned int DeviceId, u_int32_t offset, size_t len, char *buffer);
int NandWrite (unsigned int DeviceId, u_int32_t offset, u_int32_t len, char *buffer);
#endif

