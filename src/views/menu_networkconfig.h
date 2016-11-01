#ifndef KWT_MENU_NETWORKCONFIG_H
#define KWT_MENU_NETWORKCONFIG_H

unsigned int TCPIP_SetConfig (unsigned int ID, char *ip, char *netmask, char *gateway);
unsigned int change_addr (const char *str);

void CloseNetworkConfigMenu ();
void NetworkConfigInit ();
void NetworkConfigShow (bool show);
int MenuNetworkConfigKeyDown (unsigned int key_code);
#endif

