#ifndef GamePop_h
#define GamePop_h
#include "FrameWork.h"
#include "GameStd.h"


/* bank8k page 6 & 7 to mem 0x4000 & 0x6000  */
#define BankGamePop()	\
			ZXN_NEXTREG_helper(0x52, 6);	\
			ZXN_NEXTREG_helper(0x53, 7)


extern void GamePopInit(void);
extern bool GamePopRun(void);
extern bool GamePopValidate(void);



#endif
