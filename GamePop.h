#ifndef GamePop_h
#define GamePop_h
#include "FrameWork.h"
#include "GameStd.h"




extern int32 PopInFields;
extern int32 PopOnWall;
extern int32 PopDefending;

extern int32 UsedPop;


extern void GamePopInit(void);
extern void GamePopRun(void);
extern void GamePopValidate(void);



#endif
