//
//      ZX Spectrum Next z88dk simple example
//

#include "GameStd.h"
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
#include "data.h"


void PrintEsxDosError(EsxDosError fileHandle) {
	char buf[32];
	ClsL2(0);
	itoa(fileHandle, buf, 10);
	PrintProp(0,16, StdTextColour, "fileHandle:");
	PrintProp(0,24, StdTextColour, buf);
	SwapL2();
	HangForKey();
}

void Save(void) {
	char buf[32];
	ClsL2(0);
	PrintProp(0,0, StdTextColour, "Attempting to save game");
	PrintProp(0,8, StdTextColour, "Start:");
	itoa((uint16)&Data, buf, 10);
	PrintProp(28,8, StdTextColour, buf);
	PrintSimpleValue(0,16, StdTextColour, "Length: ", (int32)sizeof(GameData));
	SwapL2();
	HangForKey();
	
	EsxDosError fileHandle = OpenSaveGame();
	if (fileHandle > ESX_Eok) {
		Border(1);
		PrintEsxDosError(fileHandle);
		return;
	}
	PrintEsxDosError(fileHandle);

	uint16 lengthWritten = 0;
	uint8* dataStart = (uint8*)(&Data);
	uint16 fileSize = sizeof(GameData);
	while(lengthWritten != fileSize) {
		lengthWritten = WriteSaveGame(fileHandle, dataStart, fileSize);
		if (lengthWritten != sizeof(GameData)) {
			dataStart += lengthWritten;
			fileSize -= lengthWritten;
			Border(3);
			ClsL2(0);
			ultoa(lengthWritten, buf, 10);
			PrintProp(0,16, StdTextColour, "lengthWritten:");
			PrintProp(0,24, StdTextColour, buf);
			SwapL2();
			HangForKey();
			return;
		}
	}
	// EsxDosError error = SyncSaveGame(fileHandle);
	// if (error > ESX_Eok) {
	// 	Border(3);
	// 	PrintEsxDosError(fileHandle);
	// 	return;
	// }
	// PrintEsxDosError(fileHandle);

	EsxDosError error = CloseSaveGame(fileHandle);
	if (error > ESX_Eok) {
		Border(4);
		PrintEsxDosError(error);
		return;
	}
	PrintEsxDosError(error);

	ClsL2(0);
	PrintProp(0,0, StdTextColour, "Saved game");
	ultoa((uint16)&Data, buf, 10);
	PrintProp(25,8, StdTextColour, buf);
	PrintSimpleValue(0,16, StdTextColour, "Length: ", (int32)sizeof(GameData));
	PrintSimpleValue(0,24, StdTextColour, "Return: ", (int32)lengthWritten);
	SwapL2();
	HangForKey();
}


// ****************************************************************************************
//  Handle the main loop and state changes
// ****************************************************************************************
void MainLoop(void) {
	while (1) {
		switch (Data.GameState) {		// Do game states
			case State_InitFrontEnd:
				BankFrontEnd();
				FE_Init();
				SetState(State_FrontEnd);
				break;

			case State_FrontEnd:
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
				}
				break;
			
			case State_ShowHiScore:
				FE_ShowHiScore();
				SetState(State_InitFrontEnd);
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

			case State_SaveGame: {
				ClsL2(0);
				uint8 saveErr = SaveGame((uint8*)(&Data), sizeof(GameData));
				if (saveErr > 0) {
					char buf[10];
					int e = (int)saveErr;
					itoa(e, buf, 10);
					PrintProp(0,8,StdTextColour, buf);
					SwapL2();
					HangForKey();
				}
				ClsL2(0);
				SetState(State_IsEndGame);
				break;
			}
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
	strcpy(Data.GameName, "Kingdom");
	strcpy(Data.Version, "0.1.001");
	NextReg(0x08, 10);	// 8=Enable 8-bit DACs (A,B,C,D), 2=Enable Turbosound	memory contention?
	//NextReg(0x5, 0x04);	// 60Hz mode
	NextReg(0x15, 33);	// 32=clipping works even in "over border" mode, 1 = Enable sprite visibility, SLU (Sprites are at top, Layer 2 under, Enhanced_ULA at bottom)
	NextReg(0x4b, 0xe3);	// sprite transparency
	SetTransparencyColourFallback(0xe3);
	BankKernel();		// page in kernel
	InitKernel();
	SetUpIRQs();
	XorShiftRndSeed();
	XorShiftRndSeed32();
	InitL2();
	//GetSetDrive();
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
