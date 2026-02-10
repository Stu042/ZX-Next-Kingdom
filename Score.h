#ifndef _SCORE_CTRL_H
#define _SCORE_CTRL_H
#include <arch/zxn.h>
#include <intrinsic.h>
#include <stdbool.h>


/// bank8k page 24 & 25 to mem 0x4000 & 0x6000
#define BankScore()	\
	ZXN_NEXTREG_helper(0x52,26); \
	ZXN_NEXTREG_helper(0x53,27)




extern void SC_InitScores(void);
extern bool SC_CheckGameExists(void);
extern bool SC_LoadGame(void);
extern void SC_SaveGame(void);
extern void SC_NewGame(void);
extern void SC_ShowHiScore(void);


#endif