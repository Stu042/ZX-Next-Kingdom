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
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <input.h>
#include <errno.h>

#include "FrameWork.h"
#include "Kernel.h"
#include "FrontEnd.h"
#include "data.h"

#include "GamePlay.h"
#include "GameData.h"
#include "GameStd.h"
#include "GamePop.h"
#include "GameGrain.h"
#include "GameSimYear.h"


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
	SetState(State_PopInit);
}



//  ***************************************************************************************
//  Quiit the game and cleanup
//  ***************************************************************************************
void GP_Quit(void) {
	ClsL2(0);
	VBlankSwap();
	SetState(State_InitFrontEnd);
}



//////////////////////////////
// internal functions

