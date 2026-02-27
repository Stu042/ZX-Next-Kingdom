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
#include <stdbool.h>
#include <input.h>

#include "Kernel.h"
#include "FrontEnd.h"
#include "GameStd.h"



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
	ClsL2(0);
	FE_Background();
	#ifdef SKIP_ESX
		bool saveGameExists = false;
	#else
		if (CheckSaveGameExists()) {
			PrintProp(90, 48, MenuStdGameCol, "1. Continue Game");
		}
	#endif
	PrintProp(90, 64, MenuStdGameCol, "2. New Game");
	PrintProp(90, 80, MenuStdGameCol, "3. Hi Scores");
	PrintVersion();
	PrintCharSetCol(255);
	PPrintf(0, 0, 255, "Hi");
	VBlankSwap();
}



//  ***************************************************************************************
//  Process the front end
//  State_FrontEnd
//  ***************************************************************************************
StartGameChoice FE_Run(void) {
	ReadKeyboard();
	#ifdef SKIP_ESX
		bool saveGameExists = false;
	#else
		bool saveGameExists = CheckSaveGameExists();
	#endif
	if(saveGameExists && Debounce(VK_1)) {
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

