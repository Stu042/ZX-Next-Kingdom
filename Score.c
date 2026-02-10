#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Score.h"
#include "data.h"
#include "GameStd.h"
#include "Kernel.h"
#include "FrameWork.h"





// internal protos

static bool checkGameExists(void);

static bool loadData(const char *filename, FOpenMode mode, void *to, uint16 length);

static bool saveData(const char *filename, FOpenMode mode, void *from, uint16 length);


// internal variables

static const char *hiScoreFilename = "hiscores.king";

static uint8 hiScoreColours[HI_SCORES_COUNT] = {252, 248, 248, 244,  244,  244, 240, 240, 240, 240 };

static const HiScore defaultScores[HI_SCORES_COUNT] = {
	{ .Name = "Lizzie II", .Years = 70 },
	{ .Name = "Janaab", .Years = 68 },
	{ .Name = "Joey", .Years = 67 },
	{ .Name = "Smoke Jag", .Years = 67 },
	{ .Name = "Lil Vic", .Years = 63 },
	{ .Name = "Liz", .Years = 44 },
	{ .Name = "Augustus", .Years = 40 },
	{ .Name = "Sejong", .Years = 31 },
	{ .Name = "George VI", .Years = 15 },
	{ .Name = "Lionheart", .Years = 10 },
};


// Global Functions


//  ***************************************************************************************
//  Prepare scoring; hi scores and check we have a game to load.
//  State_ScoreInit
//  ***************************************************************************************
void SC_InitScores(void) {
	#ifdef IN_EMU
		checkGameExists();
		HiScore *hs = PlayerHiScores.Scores;
		const HiScore *defaultHiScore = defaultScores;
		for(uint8 i = 0; i < HI_SCORES_COUNT; i++) {
			strcpy(hs->Name, defaultHiScore->Name);
			hs->Years = defaultHiScore->Years;
			defaultHiScore++;
			hs++;
		}
	#else
		BankRom();
		EsxGetDrive();
		checkGameExists();
		if (!loadData(hiScoreFilename, FOPEN_MODE_OPEN_EXIST | FOPEN_MODE_READ, &PlayerHiScores, sizeof(HiScores))) {
			HiScore *hs = PlayerHiScores.Scores;
			const HiScore *defaultHiScore = defaultScores;
			for(uint8 i = 0; i < HI_SCORES_COUNT; i++) {
				strcpy(hs[i].Name, defaultHiScore->Name);
				hs[i].Years = defaultHiScore->Years;
				defaultHiScore++;
			}
			saveData(hiScoreFilename, FOPEN_MODE_OPEN_CREATE | FOPEN_MODE_WRITE, &PlayerHiScores, sizeof(HiScores));
		}
	#endif
}



//  ***************************************************************************************
//  Load the last existing game.
//  State_ContinueGame
//  ***************************************************************************************
bool SC_LoadGame(void) {
	#ifndef IN_EMU
		if (loadData(SaveGameFilename, FOPEN_MODE_OPEN_EXIST | FOPEN_MODE_READ, &Data, sizeof(GameData))) {
			return true;
		}
	#endif
	return false;
}


//  ***************************************************************************************
//  Setup the last existing game.
//  State_ContinueGame
//  ***************************************************************************************
void SC_SaveGame(void) {
	ClsL2(0);
	#ifndef IN_EMU
		saveData(SaveGameFilename, FOPEN_MODE_CREATE_TRUNC | FOPEN_MODE_WRITE, &Data, sizeof(GameData));
	#endif
}


//  ***************************************************************************************
//  Setup a new game and start game.
//  State_NewGame
//  ***************************************************************************************
void SC_NewGame(void) {
	strcpy(Data.GameName, GameName);
	strcpy(Data.Version, GameVersion);
	Data.Grains = 100;
	Data.Population = 20;
	Data.LandSize = 10;
	Data.DykeStateFrac = Frac * 2;
	Data.BanditCount = 5;
	Data.BanditHealthFrac = Frac / 2;
	#ifndef IN_EMU
		BankRom();
		saveData(SaveGameFilename, FOPEN_MODE_CREATE_TRUNC | FOPEN_MODE_WRITE, &Data, sizeof(GameData));
	#endif
}


//  ***************************************************************************************
//  Display hi scores
//  State_ShowHiScore
//  ***************************************************************************************
void SC_ShowHiScore(void) {
	HiScore *hs = PlayerHiScores.Scores;
	for(uint8 i = 0; i < HI_SCORES_COUNT; i++) {
		PrintProp(60, i*10+50, hiScoreColours[i], hs->Name);
		itoa(hs->Years, Buffer, 10);
		PrintProp(150, i*10+50, hiScoreColours[i], Buffer);
		hs++;
	}
	PrintVersion();
	VBlankSwap();
	HangForKey();
}




// ///////////////////////
// internal functions


static bool checkGameExists(void) {
	#ifdef IN_EMU
		SaveGameExists = false;
		return false;
	#else
		if (!loadData(SaveGameFilename, FOPEN_MODE_OPEN_EXIST | FOPEN_MODE_READ, (uint8 *)Buffer, 16)) {
			SaveGameExists = false;
			return false;
		}
		uint8 i;
		uint8 *buf = (uint8 *)Buffer;
		uint8 length = strlen(GameName);
		for(i=0; i<length; i++) {
			if (*buf++ != GameName[i]) {
				SaveGameExists = false;
				return false;
			}
		}
		length += strlen(GameVersion);
		for(; i<length; i++) {
			if (*buf++ != GameVersion[i]) {
				SaveGameExists = false;
				return false;
			}
		}
		SaveGameExists = true;
		return true;
	#endif
}


static bool loadData(const char *filename, FOpenMode mode, void *to, uint16 length) {
	#ifdef IN_EMU
		return false;
	#else
		BankRom();
		EsxDosError err = EsxOpen(filename, mode);
		if (err > ESX_Eok) {
			return false;
		}
		err = EsxRead(to, length);
		if (err > ESX_Eok) {
			return false;
		}
		err = EsxClose();
		if (err > ESX_Eok) {
			return false;
		}
		return true;
	#endif
}


static bool saveData(const char *filename, FOpenMode mode, void *from, uint16 length) {
	#ifdef IN_EMU
		return false;
	#else
		BankRom();
		EsxDosError err = EsxOpen(filename, mode);
		if (err > ESX_Eok) {
			return false;
		}
		err = EsxWrite(from, length);
		if (err > ESX_Eok) {
			return false;
		}
		err = EsxClose();
		if (err > ESX_Eok) {
			return false;
		}
		return true;
	#endif
}

