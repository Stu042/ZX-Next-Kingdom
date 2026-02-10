#ifndef Data_h
#define Data_h
#include "FrameWork.h"
#include "Kernel.h"



#define HI_SCORE_NAME_MAX_LEN (10)
typedef struct HI_SCORE {
	char Name[HI_SCORE_NAME_MAX_LEN];
	int Years;
}HiScore;


#define HI_SCORES_COUNT (10)
typedef struct HI_SCORES {
	HiScore Scores[HI_SCORES_COUNT];
}HiScores;




typedef struct GAME_DATA {
	char GameName[8];
	char Version[8];

	int Year;
	int32 Grains;
	int32 Population;
	int32 LandSize;
	int32 DykeStateFrac;
	int32 BanditCount;
	int32 BanditHealthFrac;
	int32 AttackChance;


	// grain

	int32 GrainAte;
	int32 GrainPlanted;
	int32 UsedGrain;


	// pop

	int32 PopInFields;
	int32 PopOnWall;
	int32 PopDefending;

	int32 UsedPop;


	// game sim year

	int32 PopIncrease;
	int32 Births;
	int32 DeathsTotal;
	int32 DeathsNatural;
	int32 DeathsStarvation;
	int32 DeathsDefending;

	int32 GrainStolen;
	int32 GrainGrown;
	int32 GrainFlooded;
	int32 GrainIncrease;

	int32 DykeNaturalDamage;
	int32 DykeRepair;
	int32 DykeStateFracIncrease;

	int32 LandFlooded;
	int32 LandReclaimed;
	int32 LandIncrease;

	int32 BanditsKilled;

	int32 TotalPopDied;
	int32 TotalPopKilled;
	int32 TotalPopStarved;
	int32 TotalPopDiedOldAge;

	int32 TotalPopBorn;

	int32 TotalBanditsKilled;

	int32 TotalGrainAte;
	int32 TotalGrainPlanted;
	int32 TotalGrainStolen;
	int32 TotalGrainGrown;
	int32 TotalGrainFlooded;

	int32 TotalLandFlooded;
	int32 TotalLandReclaimed;

	uint8 unused[128];
}GameData;


extern const char GameName[8];
extern const char GameVersion[8];
extern const char SaveGameFilename[];

extern GameData Data;
extern HiScores PlayerHiScores;

extern bool SaveGameExists;




// strcpy(Data.GameName, "Kingdom");
// strcpy(Data.Version, "0.1.001");

// extern uint8* StartOfGameData;
// extern uint16 GameSaveSize;

// extern const char GameName[];

// extern char Version[8];

// extern eGameState GameState;

// extern int Year;
// extern int32 Grains;
// extern int32 Population;
// extern int32 LandSize;
// extern int32 DykeStateFrac;
// extern int32 BanditCount;
// extern int32 BanditHealthFrac;
// extern int32 AttackChance;

// // grain


// extern int32 GrainAte;
// extern int32 GrainPlanted;

// extern int32 UsedGrain;


// // pop

// extern int32 PopInFields;
// extern int32 PopOnWall;
// extern int32 PopDefending;

// extern int32 UsedPop;



// // game sim year


// extern int32 PopIncrease;
// extern int32 Births;
// extern int32 DeathsTotal;
// extern int32 DeathsNatural;
// extern int32 DeathsStarvation;
// extern int32 DeathsDefending;
 
// extern int32 GrainStolen;
// extern int32 GrainGrown;
// extern int32 GrainFlooded;
// extern int32 GrainIncrease;
 
// extern int32 DykeNaturalDamage;
// extern int32 DykeRepair;
// extern int32 DykeStateFracIncrease;

// extern int32 LandFlooded;
// extern int32 LandReclaimed;
// extern int32 LandIncrease;

// extern int32 BanditsKilled;


// extern int32 TotalPopDied;
// extern int32 TotalPopKilled;
// extern int32 TotalPopStarved;
// extern int32 TotalPopDiedOldAge;

// extern int32 TotalPopBorn;

// extern int32 TotalBanditsKilled;

// extern int32 TotalGrainAte;
// extern int32 TotalGrainPlanted;
// extern int32 TotalGrainStolen;
// extern int32 TotalGrainGrown;
// extern int32 TotalGrainFlooded;

// extern int32 TotalLandFlooded;
// extern int32 TotalLandReclaimed;
// extern uint8 endOfData;


#endif
