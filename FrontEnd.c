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
#include "GameData.h"

#pragma output CRT_ORG_CODE = 0x4000


// internal protos
static eGameState actionInput(void);


//  ***************************************************************************************
//  Init the Front End system
//  State_InitFrontEnd
//  ***************************************************************************************
void FE_Init(void) {
	Border(INK_BLACK);

	ClsL2(0);
	BlitTransImage(68,0, 227, kingdom);
	
	Render1Bpp(190, 20, 68, Abstract_Tree_1bpp);
	Render1Bpp(190, 20, 140, Abstract_Tree_Highlight_1bpp);

	Render1Bpp(10, 20, 68, Abstract_Tree_Left_1bpp);
	Render1Bpp(10, 20, 140, Abstract_Tree_Left_Highlight_1bpp);

	VBlankSwap();

	ClsL2(0);
	BlitTransImage(68,0, 227, kingdom);
	
	Render1Bpp(190, 20, 68, Abstract_Tree_1bpp);
	Render1Bpp(190, 20, 140, Abstract_Tree_Highlight_1bpp);

	Render1Bpp(10, 20, 68, Abstract_Tree_Left_1bpp);
	Render1Bpp(10, 20, 140, Abstract_Tree_Left_Highlight_1bpp);

	VBlankSwap();
} 


//  ***************************************************************************************
//  Process the front end
//  State_FrontEnd
//  ***************************************************************************************
eGameState FE_Run(void) {    
	eGameState choice = actionInput();
	BlitTransImage(68,0, 227, kingdom);
	
	Render1Bpp(190, 20, 68, Abstract_Tree_1bpp);
	Render1Bpp(190, 20, 140, Abstract_Tree_Highlight_1bpp);

	Render1Bpp(10, 20, 68, Abstract_Tree_Left_1bpp);
	Render1Bpp(10, 20, 140, Abstract_Tree_Left_Highlight_1bpp);
	
	PrintProp(90, 48, 240, "1. Continue Game");
	PrintProp(90, 64, 240, "2. New Game");
	PrintProp(90, 80, 240, "3. Load Game");
	PrintPropCentre(180, 4, "Made By Stu");

	VBlankSwap();
	return choice;
}


//  ***************************************************************************************
//  Setup the last existing game and start game.
//  State_ContinueGame
//  ***************************************************************************************
void FE_ContinueGame(void) {	// TODO
	BankGameData();
}


//  ***************************************************************************************
//  Setup a new game and start game.
//  State_NewGame
//  ***************************************************************************************
void FE_NewGame(void) {
	BankGameData();
	Year = 0;
	Grains = 100;
	Population = 20;
	LandSize = 10;
	DykeStateFrac = 1000;
	BanditCount = 10;
	BanditHealthFrac = 50;
}


//  ***************************************************************************************
//  Setup the last existing game and start game.
//  State_ContinueGame
//  ***************************************************************************************
void FE_LoadGame(void) {	// TODO
	BankGameData();
}


//  ***************************************************************************************
//  Quit and shut down the front end
//  State_QuitFrontEnd
//  ***************************************************************************************
void FE_Quit(void) {    
} 



//  ***************************************************************************************
//  Internal functions


// Action user input
static eGameState actionInput(void) {
	DebounceReadKeyboard();
	if(DebounceKeys[VK_1] != 0) {
		return State_ContinueGame;
	}
	if(DebounceKeys[VK_2] != 0) {
		return State_NewGame;
	}
	if(DebounceKeys[VK_3] != 0) {
		return State_LoadGame;
	}
	return State_None;
}

