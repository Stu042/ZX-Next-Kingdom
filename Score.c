#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Score.h"
#include "data.h"
#include "GameStd.h"
#include "Kernel.h"
#include "FrameWork.h"
#include "Fireworks.h"


#pragma output CRT_ORG_CODE = 0x4000


// internal protos


static void saveGame(void);



// internal variables

static const char *hiScoreFilename = "hiscores.king";

static uint8 hiScoreColours[HI_SCORES_COUNT] = {252, 248, 248, 244,  244,  244, 240, 240, 240, 240 };

static Firework fireworks[FIREWORKS_MAX];


// Global Functions


//  ***************************************************************************************
//  Prepare scoring; hi scores and check we have a game to load.
//  State_ScoreInit
//  ***************************************************************************************
void SC_InitScores(void) {
	#ifdef SKIP_ESX
	#else
		SetCpu28Mhz();
		EsxDosError err = LoadData(hiScoreFilename, FOPEN_MODE_OPEN_EXIST | FOPEN_MODE_READ, &PlayerHiScores, sizeof(HiScores));
		if (err > ESX_Eok) {
			err = SaveData(hiScoreFilename, FOPEN_MODE_OPEN_CREATE | FOPEN_MODE_WRITE, &PlayerHiScores, sizeof(HiScores));
			// if (err > ESX_Eok) {
			// }
		}
		SetCpu14Mhz();
	#endif
}



//  ***************************************************************************************
//  Load the last existing game.
//  State_ContinueGame
//  ***************************************************************************************
bool SC_LoadGame(void) {
	#ifdef SKIP_ESX
		return false;
	#else
		SetCpu28Mhz();
		EsxDosError err = LoadData(SaveGameFilename, FOPEN_MODE_OPEN_EXIST | FOPEN_MODE_READ, &Data, sizeof(GameData));
		SetCpu14Mhz();
		if (err <= ESX_Eok) {
			return true;
		}
		return false;
	#endif
}


//  ***************************************************************************************
//  Setup the last existing game.
//  State_ContinueGame
//  ***************************************************************************************
void SC_SaveGame(void) {
	ClsL2(0);
	saveGame();
}


//  ***************************************************************************************
//  Setup a new game and start game.
//  State_NewGame
//  ***************************************************************************************
void SC_NewGame(void) {
	strcpy(Data.GameName, GameName);
	strcpy(Data.Version, GameVersion);
	Data.StillAlive = true;
	Data.Year = 0;
	Data.Grains = 100;
	Data.Population = 20;
	Data.LandSize = 10;
	Data.DykeStateFrac = Frac * 2;
	Data.BanditCount = 5;
	Data.BanditHealthFrac = Frac / 2;
	Data.TotalPopDied=0;
	Data.TotalPopKilled=0;
	Data.TotalPopStarved=0;
	Data.TotalPopDiedOldAge=0;
	Data.TotalPopBorn=0;
	Data.TotalBanditsKilled=0;
	Data.TotalGrainAte=0;
	Data.TotalGrainPlanted=0;
	Data.TotalGrainStolen=0;
	Data.TotalGrainGrown=0;
	Data.TotalGrainFlooded=0;
	Data.TotalLandFlooded=0;
	Data.TotalLandReclaimed=0;
	saveGame();
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



// ***************************************************************************************
// Enter a new hi score
// State_NewHiScore
// ***************************************************************************************

static HiScore *getHiScore(void);

bool SC_NewHiScore(void) {
	HiScore *hiScore = getHiScore();
	if (hiScore == NULL) {
		return false;
	}
	SetCpu28Mhz();

	memset(fireworks, 0, sizeof(Firework) * FIREWORKS_MAX);
	FireworksInit();
	bool alive = true;
	Firework *fw = fireworks;
	for(uint8 i=0; i<FIREWORKS_MAX; i++ ) {
		FireworkFire(fw++);
	}
	ClsL2(0);
	Render(73, 10, NewHiScore);
	VBlankSwap();
	ClsL2(0);
	Render(73, 10, NewHiScore);
	while(alive) {
		FireworksRender(fireworks, FIREWORKS_MAX);
		VBlankSwap();
		FireworksMove(fireworks, FIREWORKS_MAX);
		alive = FireworksAnyAlive(fireworks, FIREWORKS_MAX);
	}
	FireworksRender(fireworks, FIREWORKS_MAX);
	VBlankSwap();

	bool enterPressed = false;
	while(!enterPressed) {
		ReadKeyboard();
		enterPressed = StringInput(hiScore->Name, HI_SCORE_NAME_MAX_LEN);
		ClsLast2(0);
		PrintPropCentre(118, MenuStdGameCol, "Enter your name.");
		PrintPropCentre(128, MenuStdGameCol, hiScore->Name);
		VBlankSwap();
	}
	#ifdef SKIP_ESX
	#else
		SaveData(hiScoreFilename, FOPEN_MODE_OPEN_CREATE | FOPEN_MODE_WRITE, &PlayerHiScores, sizeof(HiScores));
	#endif
	SetCpu14Mhz();
	return true;
}


static HiScore *getHiScore(void) {
	HiScore *hiScore = PlayerHiScores.Scores;
	uint8 i;
	for(i=0; i < HI_SCORES_COUNT; i++) {
		if (Data.Year > hiScore->Years) {
			break;
		}
		hiScore++;
	}
	if (i==HI_SCORES_COUNT) {
		return NULL;
	}
	HiScore *hs1 = &PlayerHiScores.Scores[HI_SCORES_COUNT-1];
	HiScore *hs2 = &PlayerHiScores.Scores[HI_SCORES_COUNT-2];
	while(hs1 != hiScore) {
		hs1->Years = hs2->Years;
		strcpy(hs1->Name, hs2->Name);
		hs1--;
		hs2--;
	}
	hiScore->Years = Data.Year;
	hiScore->Name[0] = 0;
	return hiScore;
}



// ///////////////////////
// internal functions


static void saveGame(void) {
	#ifdef SKIP_ESX
	#else
		SetCpu28Mhz();
		SaveData(SaveGameFilename, FOPEN_MODE_CREATE_TRUNC | FOPEN_MODE_WRITE, &Data, sizeof(GameData));
		SetCpu14Mhz();
	#endif
}

