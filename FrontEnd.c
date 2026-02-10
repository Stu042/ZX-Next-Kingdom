//  ***************************************************************************************
//
//                              Simple Front End framework
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
#include <string.h>
#include <input.h>
#include <errno.h>

#include "Kernel.h"
#include "FrontEnd.h"
#include "data.h"
#include "GameStd.h"
#include "data.h"

#pragma output CRT_ORG_CODE = 0x4000


// internal protos

// Action user input, saveGameExists must of been set prior to calling
static StartGameChoice actionInput(void);
static void feBkg(void);




// internal variables






//  ***************************************************************************************
//  Init the Front End system
//  State_FrontEndInit
//  ***************************************************************************************
void FE_Init(void) {
	uint8 col = SaveGameExists ? 240 : 33;
	ClsL2(0);
	FE_Background();
	PrintProp(90, 48, col, "1. Continue Game");
	PrintProp(90, 64, 240, "2. New Game");
	PrintProp(90, 80, 240, "3. Hi Scores");
	PrintVersion();
	VBlankSwap();
}



//  ***************************************************************************************
//  Process the front end
//  State_FrontEnd
//  ***************************************************************************************
StartGameChoice FE_Run(void) {
	ReadKeyboard();
	if(SaveGameExists && Debounce(VK_1)) {
		return SGC_ContinueGame;
	}
	if(Debounce(VK_2)) {
		return SGC_NewGame;
	}
	if(Debounce(VK_3)) {
		return SGC_ShowHiScore;
	}
	return SGC_NoChoice;
}




//  ***************************************************************************************
//  Quit and shut down the front end
//  State_QuitFrontEnd
//  ***************************************************************************************
void FE_Quit(void) {    
} 


void FE_Background(void) {
	Border(INK_BLACK);
	BlitTransImage(68,0, 227, kingdom);
	Render(190,20, RightBannerPic);
	Render(10,20, LeftBannerPic);
}


//  ***************************************************************************************
//  Internal functions

