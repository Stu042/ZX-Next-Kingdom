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
#include "GameData.h"


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

void GP_SaveGame(void) {
	BankGameData();
	//SaveGame();
}


//  ***************************************************************************************
//  GP_IsEndGame the game and cleanup
//  ***************************************************************************************
bool GP_IsEndGame(void) {
	return (LandSize <= 0 || Population <= 0 || Grains <= 0);
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

