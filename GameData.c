#include "GameData.h"


#pragma output CRT_ORG_CODE = 0x2000

char Version[8] = "0.1.001";

eGameState GameState;


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

uint8 unused[128];
