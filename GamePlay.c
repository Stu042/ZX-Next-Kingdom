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

#undef DEBUG


//////////////////////////////
/// Global functions




//  ***************************************************************************************
//  Init the Game
//  ***************************************************************************************
void GP_Init( void ) {
	Border(INK_BLACK);
	ClsL2(0);
	VBlankSwap();
	GrState = GRS_PopInit;
	InitDebounce();
	SetState(State_Game);
}


//  ***************************************************************************************
//  Process the game
//  ***************************************************************************************
void GP_Run(void) {
	switch (GrState) {
		case GRS_PopInit:
			GamePopInit();
			break;
		case GRS_PopRun:
			GamePopRun();
			break;
		case GRS_PopValidate:
			GamePopValidate();
			break;
		case GRS_GrainsInit:
			GameGrainInit();
			break;
		case GRS_GrainsRun:
			GameGrainRun();
			break;
		case GRS_GrainValidate:
			GameGrainValidate();
			break;
		case GRS_SimYearInit:
			GameSimYearInit();
			break;
		case GRS_SimYearRun:
			GameSimYear();
			break;
		case GRS_SimYearRender:
			GameSimYearRender();
			break;
		case GRS_SimYearPause:
			GameSimYearPause();
			break;
			
		default:
			break;
	}
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

