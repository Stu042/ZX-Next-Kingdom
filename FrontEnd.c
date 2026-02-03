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
static StartGameChoice actionInput(void);


static char buf[32];
static const char madeBy[] = "Made By Stu v";

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
	PrintProp(90, 48, 240, "1. Continue Game");
	PrintProp(90, 64, 240, "2. New Game");
	strcpy(buf, madeBy);
	strcpy(&buf[strlen(madeBy)], Data.Version);
	PrintPropCentre(184, 4, buf);
	VBlankSwap();
} 



//  ***************************************************************************************
//  Process the front end
//  State_FrontEnd
//  ***************************************************************************************
StartGameChoice FE_Run(void) {
	StartGameChoice choice = actionInput();
	return choice;
}


//  ***************************************************************************************
//  Setup the last existing game and start game.
//  State_ContinueGame
//  ***************************************************************************************
void FE_ContinueGame(void) {	// TODO
}


//  ***************************************************************************************
//  Setup a new game and start game.
//  State_NewGame
//  ***************************************************************************************
void FE_NewGame(void) {
	Data.Year = 0;
	Data.Grains = 100;
	Data.Population = 20;
	Data.LandSize = 10;
	Data.DykeStateFrac = Frac * 2;
	Data.BanditCount = 5;
	Data.BanditHealthFrac = Frac / 2;

	Data.TotalPopDied = 0;
	Data.TotalPopKilled = 0;
	Data.TotalPopStarved = 0;
	Data.TotalPopDiedOldAge = 0;
	Data.TotalPopBorn = 0;
	Data.TotalBanditsKilled = 0;
	Data.TotalGrainAte = 0;
	Data.TotalGrainPlanted = 0;
	Data.TotalGrainStolen = 0;
	Data.TotalGrainGrown = 0;
	Data.TotalGrainFlooded = 0;
	Data.TotalLandFlooded = 0;
	Data.TotalLandReclaimed = 0;
}


//  ***************************************************************************************
//  Setup the last existing game and start game.
//  State_ContinueGame
//  ***************************************************************************************
void FE_LoadGame(void) {	// TODO
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
static StartGameChoice actionInput(void) {
	DebounceReadKeyboard();
	if(DebounceKeys[VK_1] != 0) {
		return SGC_ContinueGame;
	}
	if(DebounceKeys[VK_2] != 0) {
		return SGC_NewGame;
	}
	return SGC_NoChoice;
}

