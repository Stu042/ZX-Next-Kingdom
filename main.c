//
//      ZX Spectrum Next z88dk simple example
//

// Nightly Builds
// http://nightly.z88dk.org/

#pragma output REGISTER_SP = 0xbfff

#include <arch/zxn.h> // ZX Spectrum Next architecture specfic functions
#include <errno.h>
#include <im2.h>
#include <input.h>
#include <intrinsic.h>
#include <stdbool.h>
#include <stdint.h> // standard names for ints with no ambiguity
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>

#include "FrameWork.h"
#include "FrontEnd.h"
#include "GamePlay.h"
#include "Kernel.h"
#include "data.h"




// ****************************************************************************************
//  Handle the main loop and state changes
// ****************************************************************************************
void MainLoop(void) {
	WipeSprites();
	SetState(State_InitFrontEnd);
	CopySpriteData();
	while (1) {
		switch (GameState) {		// Do game states
			case State_InitFrontEnd: {
				BankFrontEnd();
				FE_Init();
				break;
			}
			case State_FrontEnd: {
				FE_Run();
				break;
			}
			case State_ContinueGame: {
				FE_ContinueGame();
				break;
			}
			case State_NewGame: {
				FE_NewGame();
				break;
			}
			case State_LoadGame: {
				FE_LoadGame();
				break;
			}

			case State_QuitFrontEnd: {
				FE_Quit();
				break;
			}
			case State_InitGame: {
				BankGamePlay();
				GP_Init();
				break;
			}
			case State_Game: {
				GP_Run();
				break;
			}
			case State_QuitGame: {
				GP_Quit();
				break;
			}
			default: {
				SetState(State_InitFrontEnd);
			}
		}
	}
}



// ************************************************************************************************************************
//  Main program start
// ************************************************************************************************************************
int main(void) {
	intrinsic_label(Main_Label);
	SetCpu14Mhz();
	NextReg(0x8, 0x4A);	// Disable RAM contention, enable DAC and turbosound
	//NextReg(0x5, 0x04);	// 60Hz mode
	NextReg(0x15, 0x21);	// Enable sprites and clipping, SLU priority
	NextReg(0x4b, 0xe3);	// sprite transparency
	SetTransparencyColourFallback(0xe3);
	BankKernel();		// page in kernel
	InitKernel();
	SetUpIRQs();
	XorShiftRndSeed();

	InitL2();
	Border(INK_BLACK);
	ClsL2(0);
	SwapL2();
	ClsL2(0);

	// BREAK;
	// PrintProp(10,20,12, "Hi Stuart");
	SwapL2();
	// BREAK;

	//DMACopy(0x4000, 0x4800, 0x800);
	//UploadSprites(0, 0x04, (uint16 *)0x5678);

	MainLoop();

	return 0;
}
