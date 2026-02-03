#ifndef __FRONTEND_H__
#define __FRONTEND_H__
#include "FrameWork.h"
#include "data.h"


#define BankFrontEnd()	ZXN_NEXTREG_helper(0x52,8); ZXN_NEXTREG_helper(0x53,9)

#define HI_SCORE_NAME_MAX_LEN (10)
typedef struct HI_SCORE {
	char Name[HI_SCORE_NAME_MAX_LEN];
	int Years;
}HiScore;


#define HI_SCORES_COUNT (10)
typedef struct HI_SCORES {
	HiScore Scores[HI_SCORES_COUNT];
}HiScores;


typedef enum START_GAME_CHOICE {
	SGC_NoChoice,
	SGC_ContinueGame,
	SGC_NewGame,
	SGC_ShowHiScore
}StartGameChoice;



extern void FE_Init(void);

extern StartGameChoice FE_Run(void);
extern void FE_ContinueGame(void);
extern void FE_NewGame(void);
extern void FE_LoadGame(void);
extern void FE_ShowHiScore(void);
extern void FE_Quit(void);


extern uint8 kingdom[];
extern uint8 RightBannerPic[];
extern uint8 LeftBannerPic[];


#endif //__FRONTEND_H__


