#include "GameSimYear.h"
#include "Kernel.h"
#include "GameStd.h"
#include "data.h"


//////////////////////////////
/// Global vars



//////////////////////////////
// Internal vars


#define GRAIN_GROWTH_RATE (10)
#define GRAIN_PER_ACRE (20)


#define GSY_DEBUGTEXTY(offset) ((uint8)((offset) * 8))


#define DYKESTATE_MIN_FRAC (1)
#define DYKESTATE_OKAY_FRAC (Frac)
#define DYKESTATE_MAX_FRAC (Frac * 2)



//////////////////////////////
// Internal protos


static void doPop(void);
static void doDyke(void);
static void doLand(void);
static void doGrain(void);
static void addIncreases(void);


//////////////////////////////
/// Global functions


void GameSimYearInit(void) {
	Data.PopIncrease = 0;
	Data.Births = 0;
	Data.DeathsTotal = 0;
	Data.DeathsNatural = 0;
	Data.DeathsStarvation = 0;
	Data.DeathsDefending = 0;
	
	Data.GrainStolen = 0;
	Data.GrainGrown = 0;
	Data.GrainFlooded = 0;
	Data.GrainIncrease = 0;

	Data.DykeNaturalDamage = 0;
	Data.DykeRepair = 0;
	Data.DykeStateFracIncrease = 0;

	Data.LandFlooded = 0;
	Data.LandReclaimed = 0;
	Data.LandIncrease = 0;

	Data.BanditsKilled = 0;
	Data.AttackChance = 0;
}




void GameSimYearRun(void) {
	doPop();
	doDyke();
	doLand();
	doGrain();
	addIncreases();
	if (IsEndGame()) {
		Data.StillAlive = false;
	}
}



//////////////////////////////
// Internal functions


static void doBirths(void) {
	int32 lazyPop = Data.Population - Data.UsedPop;					// people doing nothing, they multiply more than working people
	int32 fedOkayFrac = Data.GrainAte * Frac / Data.Population;			// min amount of food to be happy
	fedOkayFrac = clamp(fedOkayFrac, Frac, 2 * Frac);
	int32 lazyBirthsMax = lazyPop * fedOkayFrac / Frac;
	int32 lazyBirths = rndRange32(0, lazyBirthsMax);
	int32 naturalBirths = rndPerc32(Data.Population, 10);		// whole pop gets a chance of 10% increase
	Data.Births = lazyBirths + naturalBirths;
}


static void doDeaths(void) {
	int32 hungry = Data.Population - Data.GrainAte;
	if (hungry > 0) {
		Data.DeathsStarvation = rndRange32(0, hungry);		// the unfed are likely to die
	} else {
		Data.DeathsStarvation = 0;
	}
	Data.DeathsNatural = rndPerc32(Data.Population * Frac, 10) / Frac;	// plus random upto 10% die
}


static void doBandits(void) {
	uint32 popDefendingFrac;
	if (Data.PopDefending == 0) {
		popDefendingFrac = 1;
	} else {
		popDefendingFrac = Data.PopDefending * Frac;
	}
	uint32 attackStrength = (Data.BanditCount * Data.BanditHealthFrac) / popDefendingFrac;
	attackStrength = min(attackStrength, INT32_MAX);
	Data.AttackChance = rndRange32(attackStrength >> 1, attackStrength);
	if (Data.AttackChance > 0) {							// Bandits are attacking
		int32 overkill = Data.BanditCount - (Data.PopDefending << 1);
		if (overkill > 0){
			Data.DeathsDefending += rndRange32(Data.PopDefending >> 1, Data.PopDefending);
			int32 generalPopAttack = min(overkill, Data.Population - Data.PopDefending);
			Data.DeathsDefending += rndRange32(generalPopAttack >> 2, generalPopAttack >> 1);
			Data.BanditHealthFrac -= Frac / 8;
			Data.BanditsKilled = rndRange32(Data.BanditCount >> 4, Data.BanditCount >> 3);

		} else {
			Data.DeathsDefending = rndRange32(Data.BanditCount >> 2, Data.BanditCount >> 1);
			Data.BanditHealthFrac -= Frac / 4;
			Data.BanditsKilled = rndRange32(Data.BanditCount >> 3, Data.BanditCount >> 1);
		}
	} else {
		Data.BanditHealthFrac += rndRange32(1, Frac / 4);
		Data.BanditCount += rndRange32(0, Data.BanditCount >> 1);
	}
	Data.BanditCount -= Data.BanditsKilled;
	Data.BanditHealthFrac = min(Data.BanditHealthFrac, Frac * 2);
	if (Data.BanditCount <= 0) {
		Data.BanditCount = rndRange32(1, 10);
	}
}

static void addPopTotals(void) {
	Data.TotalPopBorn += Data.Births;
	Data.TotalPopDiedOldAge += Data.DeathsNatural;
	Data.TotalPopStarved += Data.DeathsStarvation;
	Data.TotalPopKilled += Data.DeathsDefending;
	Data.TotalPopDied += Data.DeathsDefending + Data.DeathsStarvation + Data.DeathsNatural;
	Data.TotalBanditsKilled += Data.BanditsKilled;
}

static void doPop(void) {
	doBirths();
	doBandits();
	doDeaths();
	Data.DeathsTotal += Data.DeathsDefending + Data.DeathsStarvation + Data.DeathsNatural;
	Data.PopIncrease = Data.Births - Data.DeathsTotal;
	addPopTotals();
}


static void doDyke(void) {
	Data.DykeNaturalDamage = rndPerc32(Data.DykeStateFrac, 20);		// dyke strength decreases every eyar
	Data.DykeRepair = Data.PopOnWall * rndRange32(1, Frac * 4) / Data.LandSize;	// people repair wall (1 person per up to 4 acres)
	Data.DykeStateFracIncrease = Data.DykeRepair - Data.DykeNaturalDamage;		// state of the dyke, add repair remove damage
}


static void doLand(void) {
	Data.LandFlooded = 0;
	if (Data.DykeStateFrac < DYKESTATE_MAX_FRAC) {
		int32 LandFloodedFrac = ((Data.DykeStateFrac * 100) /  DYKESTATE_MAX_FRAC) / FRAC;
		Data.LandFlooded = rndPerc32(Data.LandSize, LandFloodedFrac);
	} else {
		Data.DykeStateFrac = DYKESTATE_MAX_FRAC;
	}
	uint32 landReclaimedMax = Data.PopOnWall << 1;
	landReclaimedMax = (landReclaimedMax >= INT32_MAX)? INT32_MAX : landReclaimedMax;
	Data.LandReclaimed = rndRange32(0, landReclaimedMax);
	Data.LandIncrease = Data.LandReclaimed - Data.LandFlooded;
	Data.TotalLandFlooded += Data.LandFlooded;
	Data.TotalLandReclaimed += Data.LandReclaimed;
}


static void doGrain(void) {
	Data.GrainStolen = 0;
	uint32 grainGrownMax = Data.GrainPlanted * GRAIN_GROWTH_RATE * Data.PopInFields;	// planted * typical growth * people looking after it
	grainGrownMax = min(grainGrownMax, Data.LandSize * GRAIN_PER_ACRE);
	uint32 grainGrown = rndRange32(grainGrownMax >> 1, grainGrownMax);
	Data.GrainGrown = min(grainGrown, INT32_MAX);
	if (Data.LandSize == 0 || Data.GrainGrown == 0) {
		Data.GrainFlooded = 0;
	} else {
		Data.GrainFlooded = ((Data.LandFlooded * Frac) / Data.LandSize) * Data.GrainGrown / Frac;	// ratio land flooded * grain in fields
	}
	if (Data.AttackChance > 0) {
		uint32 maxGrainStolen = min(Data.BanditCount * 40, Data.Grains - (Data.GrainPlanted >> 1));
		Data.GrainStolen = rndRange32(maxGrainStolen >> 2, maxGrainStolen);
	}
	uint32 grainDec = (uint32)Data.GrainFlooded + (uint32)Data.GrainAte + (uint32)Data.GrainPlanted + (uint32)Data.GrainStolen;
	grainDec = min(INT32_MAX, grainDec);
	Data.GrainIncrease = Data.GrainGrown - grainDec;
	Data.TotalGrainPlanted += Data.GrainPlanted;
	Data.TotalGrainAte += Data.GrainAte;
	Data.TotalGrainStolen += Data.GrainStolen;
	Data.TotalGrainGrown += Data.GrainGrown;
	Data.TotalGrainFlooded += Data.GrainFlooded;
}


static void addIncreases(void) {
	uint32 pop = Data.Population;
	pop += Data.PopIncrease;
	Data.Population = min(pop, INT32_MAX);
	Data.Grains += Data.GrainIncrease;
	Data.DykeStateFrac += Data.DykeStateFracIncrease;
	Data.DykeStateFrac = clamp(Data.DykeStateFrac, DYKESTATE_MIN_FRAC, DYKESTATE_MAX_FRAC);
	uint32 landSize = (uint32)Data.LandSize + (uint32)Data.LandIncrease;
	Data.LandSize = min(landSize, INT32_MAX);
	Data.Year++;
}
