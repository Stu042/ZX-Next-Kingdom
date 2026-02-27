//
//      ZX Spectrum Next z88dk simple example
//
#pragma output REGISTER_SP = 0xbfff


#include <arch/zxn.h> // ZX Spectrum Next architecture specfic functions
#include <arch/zxn/esxdos.h>
#include <im2.h>
#include <input.h>
#include <intrinsic.h>
#include <stdbool.h>
#include <stdint.h> // standard names for ints with no ambiguity
#include <z80.h>
#include <stdlib.h>
#include <string.h>

#include "FrameWork.h"
#include "FrontEnd.h"
#include "GamePlay.h"
#include "Kernel.h"
#include "data.h"
#include "GamePop.h"
#include "GameGrain.h"
#include "GameSimYear.h"
#include "Score.h"


#define	SetState(state)	(GameState=(state))


static eGameState GameState;


// ****************************************************************************************
//  Handle the main loop and state changes
// ****************************************************************************************
void MainLoop(void) {
	while (1) {
		switch (GameState) {		// Do game states
			case State_ScoreInit:
				BankScore();
				SC_InitScores();
				SetState(State_FrontEndRender);
				break;

			case State_FrontEndRender:	// prepare front end menu (render it)
				BankFrontEnd();
				FE_Init();
				SetState(State_FrontEndAction);
				break;

			case State_FrontEndAction:	// action the menu
				#ifdef AUTOPLAY
					SetState(State_NewGame);
				#else
					BankFrontEnd();
					StartGameChoice menuChoice = FE_Run();	// state set by the menu option selected
					switch (menuChoice) {
						case SGC_NewGame:
							SetState(State_NewGame);
							break;
						case SGC_ContinueGame:
							SetState(State_ContinueGame);
							break;
						case SGC_ShowHiScore:
							SetState(State_ShowHiScore);
							break;
						default:
							break;
					}
				#endif
				break;
			
			case State_ShowHiScore:		// show hi score and wait for key press
				BankFrontEnd();
				ClsL2(0);
				FE_Background();
				BankScore();
				SC_ShowHiScore();
				SetState(State_FrontEndRender);
				break;

			case State_ContinueGame:	// Load game, if okay start game
				BankScore();
				bool okay = SC_LoadGame();
				if (okay) {
					SetState(State_QuitFrontEnd);
				} else {
					SetState(State_ScoreInit);
				}
				break;

			case State_NewGame:		// reset game data and start game
				BankScore();
				SC_NewGame();
				SetState(State_QuitFrontEnd);
				break;

			case State_QuitFrontEnd:
				BankFrontEnd();
				FE_Quit();
				SetState(State_InitGame);
				break;

			case State_InitGame:
				BankGamePlay();
				GP_Init();
				SetState(State_StartGameLoop);
				break;

			case State_StartGameLoop:
				BankGamePlay();
				GP_ResetData();
				SetState(State_PopInit);
				break;

			case State_PopInit:
				BankGamePop();
				GamePopInit();
				SetState(State_PopRun);
				break;

			case State_PopRun:
				BankGamePop();
				if (GamePopRun()) {
					SetState(State_PopValidate);
				}
				break;

			case State_PopValidate:
				BankGamePop();
				if (GamePopValidate()) {
					SetState(State_GrainsInit);
				} else {
					SetState(State_PopRun);
				}
				break;

			case State_GrainsInit:
				BankGamePlay();
				GameGrainInit();
				SetState(State_GrainsRun);
				break;

			case State_GrainsRun:
				BankGamePlay();
				if (GameGrainRun()) {
					SetState(State_GrainValidate);
				}
				break;

			case State_GrainValidate:
				BankGamePlay();
				if (GameGrainValidate()) {
					SetState(State_SimYearInit);
				} else {
					SetState(State_GrainsRun);
				}
				break;

			case State_SimYearInit:
				BankGamePlay();
				GameSimYearInit();
				SetState(State_SimYearRun);
				break;

			case State_SimYearRun:
				BankGamePlay();
				GameSimYearRun();
				SetState(State_SaveGame);
				break;

			case State_SaveGame:
				BankScore();
				SC_SaveGame();
				SetState(State_SimYearRender);
				break;

			case State_SimYearRender:
				#ifdef AUTOPLAY
				#else
					BankGamePlay();
					GameSimYearRender();
				#endif
				SetState(State_IsEndGame);
				break;

			case State_IsEndGame:
				if (Data.StillAlive) {
					SetState(State_StartGameLoop);
				} else {
					SetState(State_QuitGame);
				}
				break;

			case State_QuitGame:
				BankGamePlay();
				GP_Quit();
				SetState(State_NewHiScore);
				break;

			case State_NewHiScore:
				#ifdef AUTOPLAY
					SetState(State_ScoreInit);
				#else
					BankScore();
					if (SC_NewHiScore()) {
						SetState(State_ShowHiScore);
					} else {
						SetState(State_ScoreInit);
					}
				#endif
				break;

			default:
				SetState(State_ScoreInit);
				break;
		}
	}
}


// ************************************************************************************************************************
//  Main program start
// ************************************************************************************************************************
int main(void) {
	intrinsic_label(Main_Label);
	SetCpu14Mhz();
	NextReg(0x08, 10);	// 8=Enable 8-bit DACs (A,B,C,D), 2=Enable Turbosound	memory contention?
	//NextReg(0x5, 0x04);	// 60Hz mode
	NextReg(0x15, 33);	// 32=clipping works even in "over border" mode, 1 = Enable sprite visibility, SLU (Sprites are at top, Layer 2 under, Enhanced_ULA at bottom)
	NextReg(0x4b, 0xe3);	// sprite transparency
	SetTransparencyColourFallback(0xe3);
	BankKernel();		// page in kernel
	#ifdef SKIP_ESX		// prepare filesystem if NOT using emulator
	#else
		EsxGetDrive();
	#endif
	SetUpIRQs();
	XorShiftRndSeed();
	XorShiftRndSeed32();
	InitL2();
	Border(INK_BLACK);
	SetState(State_ScoreInit);
	MainLoop();
	return 0;
}

