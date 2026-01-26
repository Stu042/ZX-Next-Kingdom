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


static char buf[32];
static char madeBy[] = "Made By Stu v";

//  ***************************************************************************************
//  Init the Front End system
//  State_InitFrontEnd
//  ***************************************************************************************
void FE_Init(void) {
	Border(INK_BLACK);

	ClsL2(0);
	BlitTransImage(68,0, 227, kingdom);
	
	Render(190,20, RightBannerPic);
	Render(10,20, LeftBannerPic);

	PrintPropCentre(96, 240, "Any key to start game");
	// PrintProp(90, 48, 240, "1. Continue Game");
	// PrintProp(90, 64, 240, "2. New Game");
	// PrintProp(90, 80, 240, "3. Load Game");
	strcpy(buf, madeBy);
	strcpy(&buf[strlen(madeBy)], Version);
	PrintPropCentre(184, 4, buf);

	VBlankSwap();
	BankGameData();
} 


//  ***************************************************************************************
//  Process the front end
//  State_FrontEnd
//  ***************************************************************************************
eGameState FE_Run(void) {
	DebounceReadKeyboard();
	HangForKey();
	return State_NewGame;
	// eGameState choice;
	// do {
	// 	choice = actionInput();
	// } while(choice == State_None);
	// return choice;
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
	DykeStateFrac = Frac * 2;
	BanditCount = 10;
	BanditHealthFrac = Frac / 2;
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

