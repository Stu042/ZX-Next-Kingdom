#ifndef GameSimYear_H
#define GameSimYear_H
#include "FrameWork.h"
#include "GameStd.h"
#include "Kernel.h"



#define GSY_TEXT_TOP (128)
// #ifdef IN_DEBUG
// 	uint8 GSY_TEXTY(uint8 offset) {
// 		if (((uint8)(GSY_TEXT_TOP + (offset) * 8)) > (DISPLAY_HEIGHT - 8)) {
// 			Border(INK_RED);	// warn if we try to print off screen
// 		}
// 		return ((uint8)(GSY_TEXT_TOP + (offset) * 8));
// 	}
// #else
	#define GSY_TEXTY(offset) ((uint8)(GSY_TEXT_TOP + (offset) * 8))
// #endif




extern void GameSimYearInit(void);
extern void GameSimYearRun(void);
extern void GameSimYearRender(void);

extern void ShowChange(uint8 x, uint8 y, int32 value, char *valHeader, char *message, uint8 pic);
extern void Show3WayChange(uint8 x, uint8 y, int32 valueDelta, int32 value, char *valHeader, char *valPlus, char *messagePlus, char *valMinus, char *messageMinus, char *messageConstant, uint8 pic);


#endif
