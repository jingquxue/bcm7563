#ifndef KWT_RECORD_H
#define KWT_RECORD_H

void InitializeRecord(void);
eRet RecordStart(void);
void RecordStop(void);
void RecordClose(void);
unsigned int GetRecFileSize(void);
unsigned int GetRecDevFreeSize(void);
unsigned int GetRecDevTotalSize(void);
void RecMonThread (void *pParam);
#endif

