//  ***************************************************************************************
//
//                              Simple Game framework
//
//  ***************************************************************************************
#include <arch/zxn.h>           // ZX Spectrum Next architecture specfic functions
#include <stdint.h>             // standard names for ints with no ambiguity 
#include <z80.h>
#include <im2.h>
#include <intrinsic.h>
#include <stdbool.h>
#include <stdint.h>
#include <input.h>

#include "FrameWork.h"
#include "Kernel.h"
#include "data.h"

#include "GamePlay.h"


#pragma output CRT_ORG_CODE = 0x4000


//////////////////////////////
/// Global functions




//  ***************************************************************************************
//  Init the Game
//  ***************************************************************************************
void GP_Init(void) {
	Border(INK_BLACK);
	ClsL2(0);
	VBlankSwap();
}

void GP_ResetData(void) {
	Data.PopInFields = 0;
	Data.PopOnWall = 0;
	Data.PopDefending = 0;
	Data.UsedPop = 0;
	Data.GrainAte = 0;
	Data.GrainPlanted = 0;
	Data.UsedGrain = 0;
}


//  ***************************************************************************************
//  Quit the game and cleanup
//  ***************************************************************************************
void GP_Quit(void) {
	ClsL2(0);
	VBlankSwap();
}



//////////////////////////////
// internal functions

