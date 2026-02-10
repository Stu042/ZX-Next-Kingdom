#ifndef __FRONTEND_H__
#define __FRONTEND_H__
#include "FrameWork.h"
#include "data.h"


#define BankFrontEnd()	ZXN_NEXTREG_helper(0x52,8);	/* bank8k page 8 & 9 to mem 0x4000 & 0x6000  */	\
			ZXN_NEXTREG_helper(0x53,9)


typedef enum START_GAME_CHOICE {
	SGC_NoChoice,
	SGC_ContinueGame,
	SGC_NewGame,
	SGC_ShowHiScore
}StartGameChoice;



extern void FE_Init(void);

extern StartGameChoice FE_Run(void);
extern bool FE_ContinueGame(void);
extern void FE_NewGame(void);
extern void FE_ShowHiScore(void);
extern void FE_Quit(void);
extern void FE_SaveGame(void);
extern void FE_Background(void);


extern uint8 kingdom[];
extern uint8 RightBannerPic[];
extern uint8 LeftBannerPic[];


#endif //__FRONTEND_H__


