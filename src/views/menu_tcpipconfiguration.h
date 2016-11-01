#ifndef KWT_TCPIPCONFIGURATION_H
#define KWT_TCPIPCONFIGURATION_H

void CloseTCPIPConfigurationMenu ();
void TCPIPConfigurationInit ();
void TCPIPConfigurationShow (bool show);
int MenuTCPIPConfigurationKeyDown (unsigned int key_code);

extern unsigned char had_enabled;
#endif
