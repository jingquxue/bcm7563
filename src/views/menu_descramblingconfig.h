#ifndef KWT_MENU_DESCRAMBLING_H
#define KWT_MENU_DESCRAMBLING_H

#define DESCRAMBLE_REG_BASE_ADDRESS 0x00
#define DESCRAMBLE_REG_VER_ADDRESS    	0x33// tux add 20110622 d330版本信息寄存器地址
#define D330_I2C_ADDRESS 0x30


void CloseDescramblingMenu ();
void DescramblingInit ();
void DescramblingShow (bool show);
int MenuDescramblingKeyDown (unsigned int key_code);
void Get_D330_Ver(unsigned char * ver);
#endif

