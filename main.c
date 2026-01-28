//
//      ZX Spectrum Next z88dk simple example
//

// Nightly Builds
// http://nightly.z88dk.org/

#include "GameData.h"
#pragma output REGISTER_SP = 0xbfff

#include <arch/zxn.h> // ZX Spectrum Next architecture specfic functions
#include <im2.h>
#include <input.h>
#include <intrinsic.h>
#include <stdbool.h>
#include <stdint.h> // standard names for ints with no ambiguity
#include <z80.h>
#include <stdlib.h>

#include "FrameWork.h"
#include "FrontEnd.h"
#include "GamePlay.h"
#include "Kernel.h"
#include "data.h"
#include "GamePop.h"
#include "GameGrain.h"
#include "GameSimYear.h"



// ****************************************************************************************
//  Handle the main loop and state changes
// ****************************************************************************************
void MainLoop(void) {
	while (1) {
		switch (GameState) {		// Do game states
			case State_InitFrontEnd:
				BankFrontEnd();
				FE_Init();
				SetState(State_FrontEnd);
				break;

			case State_FrontEnd:
				eGameState nextState = FE_Run();	// state set by the menu option selected
				SetState(nextState);
				break;

			case State_ContinueGame:
				FE_ContinueGame();
				SetState(State_NewGame);
				break;

			case State_NewGame:
				FE_NewGame();
				SetState(State_QuitFrontEnd);
				break;

			case State_LoadGame:
				FE_LoadGame();
				SetState(State_NewGame);
				break;

			case State_QuitFrontEnd:
				FE_Quit();
				SetState(State_InitGame);
				break;

			case State_InitGame:
				BankGamePlay();
				GP_Init();
				SetState(State_PopInit);
				break;

			case State_PopInit:
				GamePopInit();
				SetState(State_PopRun);
				break;

			case State_PopRun:
				if (GamePopRun()) {
					SetState(State_PopValidate);
				}
				break;

			case State_PopValidate:
				if (GamePopValidate()) {
					SetState(State_GrainsInit);
				} else {
					SetState(State_PopRun);
				}
				break;

			case State_GrainsInit:
				GameGrainInit();
				SetState(State_GrainsRun);
				break;

			case State_GrainsRun:
				if (GameGrainRun()) {
					SetState(State_GrainValidate);
				}
				break;

			case State_GrainValidate:
				if (GameGrainValidate()) {
					SetState(State_SimYearInit);
				} else {
					SetState(State_GrainsRun);
				}
				break;

			case State_SimYearInit:
				GameSimYearInit();
				SetState(State_SimYearRun);
				break;

			case State_SimYearRun:
				GameSimYearRun();
				SetState(State_SimYearRender);
				break;

			case State_SimYearRender:
				GameSimYearRender();
				SetState(State_SaveGame);
				break;

			case State_SaveGame:
				GP_SaveGame();
				SetState(State_IsEndGame);
				break;
			
			case State_IsEndGame:
				if (GP_IsEndGame()) {
					SetState(State_QuitGame);
				} else {
					SetState(State_PopInit);
				}
				break;

			case State_QuitGame:
				GP_Quit();
				SetState(State_InitFrontEnd);
				break;

			default:
				SetState(State_InitFrontEnd);
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
	NextReg(0x08, 8 + 2);	// 8=Enable 8-bit DACs (A,B,C,D), 2=Enable Turbosound
	//NextReg(0x5, 0x04);	// 60Hz mode
	NextReg(0x15, 32 + 1);	// 32=clipping works even in "over border" mode, 1 = Enable sprite visibility, SLU (Sprites are at top, Layer 2 under, Enhanced_ULA at bottom)
	NextReg(0x4b, 0xe3);	// sprite transparency
	SetTransparencyColourFallback(0xe3);
	BankKernel();		// page in kernel
	InitKernel();
	SetUpIRQs();
	XorShiftRndSeed();
	XorShiftRndSeed32();
	InitL2();
	Border(INK_BLACK);
	ClsL2(0);
	SwapL2();
	ClsL2(0);
	SwapL2();
	InitDebounce();
	SetState(State_InitFrontEnd);
	MainLoop();

	return 0;
}
