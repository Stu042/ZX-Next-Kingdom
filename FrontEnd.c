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
#include "GameData.h"

#pragma output CRT_ORG_CODE = 0x4000


// internal protos
void PrintGameName(void);
void ActionInput(void);
void SetDefaultGameValues(void);


//  ***************************************************************************************
//  Init the Front End system
//  State_InitFrontEnd
//  ***************************************************************************************
void FE_Init(void) {
	Border(INK_BLACK);
	InitDebounce();

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

	// Set state when we're ready to move into the main processing
	SetState(State_FrontEnd);
} 


//  ***************************************************************************************
//  Process the front end
//  State_FrontEnd
//  ***************************************************************************************
void FE_Run(void) {    
	ActionInput();
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
}




// Action user input
void ActionInput(void) {
	DebounceReadKeyboard();
	if(DebounceKeys[VK_1] != 0) {
		SetState(State_ContinueGame);
		return;
	}
	if(DebounceKeys[VK_2] != 0) {
		SetState(State_NewGame);
		return;
	}
	if(DebounceKeys[VK_3] != 0) {
		SetState(State_LoadGame);
		return;
	}
}


//  ***************************************************************************************
//  Setup the last existing game and start game.
//  State_ContinueGame
//  ***************************************************************************************
void FE_ContinueGame(void) {	// TODO
	BankGameData();
	SetState(State_NewGame);
}


//  ***************************************************************************************
//  Setup a new game and start game.
//  State_NewGame
//  ***************************************************************************************
void FE_NewGame(void) {
	BankGameData();
	SetDefaultGameValues();
	SetState(State_QuitFrontEnd);
}


void SetDefaultGameValues(void) {
	Grains = 100;
	Population = 20;
	LandSize = 10;
	DykeState = 1000;
}


//  ***************************************************************************************
//  Setup the last existing game and start game.
//  State_ContinueGame
//  ***************************************************************************************
void FE_LoadGame(void) {	// TODO
	BankGameData();
	SetState(State_NewGame);
}


//  ***************************************************************************************
//  Quit and shut down the front end
//  State_QuitFrontEnd
//  ***************************************************************************************
void FE_Quit(void) {    
	SetState(State_InitGame);
} 

