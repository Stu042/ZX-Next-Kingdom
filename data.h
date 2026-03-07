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

	bool StillAlive;

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

	uint32 TotalPopDied;
	uint32 TotalPopKilled;
	uint32 TotalPopStarved;
	uint32 TotalPopDiedOldAge;

	uint32 TotalPopBorn;

	uint32 TotalBanditsKilled;

	uint32 TotalGrainAte;
	uint32 TotalGrainPlanted;
	uint32 TotalGrainStolen;
	uint32 TotalGrainGrown;
	uint32 TotalGrainFlooded;

	uint32 TotalLandFlooded;
	uint32 TotalLandReclaimed;
}GameData;


// extern const char* GameName;
// extern const char* GameVersion;
// extern const char SaveGameFilename[];

extern GameData Data;
extern HiScores PlayerHiScores;

//extern bool SaveGameExists;


extern bool IsEndGame(void);


#endif
