#ifndef GameData_h
#define GameData_h
#include "FrameWork.h"


#define BankGameData()	ZXN_NEXTREG_helper(0x51, 24)

extern eGameState GameState;

extern int Year;
extern int32 Grains;
extern int32 Population;
extern int32 LandSize;
extern int32 DykeStateFrac;
extern int32 BanditCount;
extern int32 BanditHealthFrac;
extern int32 AttackChance;

// grain


extern int32 GrainAte;
extern int32 GrainPlanted;

extern int32 UsedGrain;


// pop

extern int32 PopInFields;
extern int32 PopOnWall;
extern int32 PopDefending;

extern int32 UsedPop;




// game sim year


extern int32 PopIncrease;
extern int32 Births;
extern int32 DeathsTotal;
extern int32 DeathsNatural;
extern int32 DeathsStarvation;
extern int32 DeathsDefending;
 
extern int32 GrainsStolen;
extern int32 GrainGrown;
extern int32 GrainFlooded;
extern int32 GrainIncrease;
 
extern int32 DykeNaturalDamage;
extern int32 DykeRepair;
extern int32 DykeStateFracIncrease;

extern int32 LandFlooded;
extern int32 LandReclaimed;
extern int32 LandIncrease;

extern int32 BanditsKilled;


#endif
