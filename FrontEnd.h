#ifndef __FRONTEND_H__
#define __FRONTEND_H__
#include "FrameWork.h"


#define BankFrontEnd()	ZXN_NEXTREG_helper(0x52,8); ZXN_NEXTREG_helper(0x53,9)

extern void FrontEndTest(void);

extern void FE_Init(void);

extern eGameState FE_Run(void);
extern void FE_ContinueGame(void);
extern void FE_NewGame(void);
extern void FE_LoadGame(void);

extern void FE_Quit(void);


extern uint8 Abstract_Tree_1bpp[];
extern uint8 Abstract_Tree_Highlight_1bpp[];
extern uint8 Abstract_Tree_Left_1bpp[];
extern uint8 Abstract_Tree_Left_Highlight_1bpp[];
extern uint8 kingdom[];


#endif //__FRONTEND_H__


