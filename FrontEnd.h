#ifndef __FRONTEND_H__
#define __FRONTEND_H__
#include "FrameWork.h"


#define BankFrontEnd()	ZXN_NEXTREG_helper(0x52,8); ZXN_NEXTREG_helper(0x53,9)


typedef enum START_GAME_CHOICE {
	SGC_NoChoice,
	SGC_ContinueGame,
	SGC_NewGame
}StartGameChoice;



extern void FE_Init(void);

extern StartGameChoice FE_Run(void);
extern void FE_ContinueGame(void);
extern void FE_NewGame(void);
extern void FE_LoadGame(void);

extern void FE_Quit(void);


extern uint8 kingdom[];
extern uint8 RightBannerPic[];
extern uint8 LeftBannerPic[];


#endif //__FRONTEND_H__


