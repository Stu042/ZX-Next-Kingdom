#ifndef __GAMEPLAY_H__
#define __GAMEPLAY_H__

#include <stdint.h>
#include "FrameWork.h"
#include "GameStd.h"


#define BankGamePlay()	ZXN_NEXTREG_helper(0x52, 6); ZXN_NEXTREG_helper(0x53, 7)


#define EDIT_VALUE_BUF_SIZE (16)



extern void GamePlayTest(void);

extern void GP_Init(void);
extern void GP_Run(void);
extern void GP_Quit(void);


 #endif // __GAMEPLAY_H__





