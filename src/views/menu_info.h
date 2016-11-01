#ifndef KWT_MENU_INFO_H
#define KWT_MENU_INFO_H
void MenuInfoInit(void);


void MenuInfoUpdate(int type);


void MenuInfoShow(bool show);

int MenuInfoKeyDown(unsigned int  key_code);
void CloseMenuInfo(void);
#endif


