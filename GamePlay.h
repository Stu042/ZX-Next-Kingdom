#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include <stdint.h>
#include "FrameWork.h"
#include "GameStd.h"


/* bank8k page 6 & 7 to mem 0x4000 & 0x6000  */
#define BankGamePlay()	\
			ZXN_NEXTREG_helper(0x52, 6);	\
			ZXN_NEXTREG_helper(0x53, 7)



extern void GamePlayTest(void);

extern void GP_Init(void);
extern void GP_Run(void);
extern void GP_SaveGame(void);
extern bool GP_IsEndGame(void);
extern void GP_Quit(void);


 #endif // __GAMEPLAY_H__





