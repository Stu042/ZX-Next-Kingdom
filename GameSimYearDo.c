#include "GameSimYear.h"
#include "Kernel.h"
#include "GameStd.h"
#include "data.h"


//////////////////////////////
/// Global vars



//////////////////////////////
// Internal vars


#define GRAIN_GROWTH_RATE (20)
#define GRAIN_PER_ACRE (40)


#define GSY_DEBUGTEXTY(offset) ((uint8)((offset) * 8))


#define DYKESTATE_MIN_FRAC (Frac / 2)
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


#ifdef IN_DEBUG
	uint8 debugY;
#endif



void GameSimYearRun(void) {
	#ifdef IN_DEBUG
		debugY=0;
		ClsL2(0);
	#endif
	doPop();
	doDyke();
	doLand();
	doGrain();
	addIncreases();
	#ifdef IN_DEBUG
		if (debugY >= 21) {
			Border(INK_RED);
		}
		VBlankSwap();
		HangForKey();
		Border(INK_BLACK);
	#endif
}



//////////////////////////////
// Internal functions


static void doBirths(void) {
	int32 lazyPop = Data.Population - Data.UsedPop;					// people doing nothing, they multiply more than working people
	int32 fedOkayFrac = Data.GrainAte * Frac / Data.Population;				// min amount of food to be happy
	fedOkayFrac = clamp(fedOkayFrac, Frac, 2 * Frac);
	int32 lazyBirthsMax = lazyPop * fedOkayFrac / Frac;
	int32 lazyBirths = rndRange32(0, lazyBirthsMax);
	int32 naturalBirths = rndPerc32(Data.Population, 10);		// whole pop gets a chance of 10% increase
	#ifdef IN_DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "fedOkayFrac: ", fedOkayFrac);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "lazyBirthsMax: ", lazyBirthsMax);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "lazyBirths: ", lazyBirths);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "naturalBirths: ", naturalBirths);
	#endif
	Data.Births = lazyBirths + naturalBirths;
}


static void doDeaths(void) {
	int32 hungry = Data.Population - Data.GrainAte;
	if (hungry > 0) {
		Data.DeathsStarvation = rndRange32(0, hungry);	// the unfed are likely to die
	} else {
		Data.DeathsStarvation = 0;
	}
	Data.DeathsNatural = rndPerc32(Data.Population * Frac, 10) / Frac;			// plus random upto 10% die
}


static void doBandits(void) {
	int32 popDefendingFrac;
	if (Data.PopDefending == 0) {
		popDefendingFrac = 1;
	} else {
		popDefendingFrac = Data.PopDefending * Frac;
	}
	int32 attackStrength = (Data.BanditCount * Data.BanditHealthFrac) / popDefendingFrac;
	Data.AttackChance = rndRange32(attackStrength >> 1, attackStrength);
	if (Data.AttackChance > 0) {						// Bandits are attacking
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
	#ifdef IN_DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "DeathsDefending: ", DeathsDefending);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "BanditsKilled: ", BanditsKilled);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY++), valDebugCol, "BanditHealthFrac: ", BanditHealthFrac);
	#endif
}

static void addPopTotals(void) {
	Data.TotalPopBorn += Data.Births;
	Data.TotalPopDiedOldAge += Data.DeathsNatural;
	Data.TotalPopStarved += Data.DeathsStarvation;
	Data.TotalPopKilled += Data.DeathsDefending;
	Data.TotalPopDied +=Data. DeathsDefending + Data.DeathsStarvation + Data.DeathsNatural;
	Data.TotalBanditsKilled += Data.BanditsKilled;
}

static void doPop(void) {
	doBirths();
	doBandits();
	doDeaths();
	Data.DeathsTotal += Data.DeathsStarvation + Data.DeathsNatural;
	Data.PopIncrease = Data.Births - Data.DeathsTotal;
	addPopTotals();
	#ifdef IN_DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "DeathsStarvation: ", DeathsStarvation);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "DeathsNatural: ", DeathsNatural);
	#endif
}


static void doDyke(void) {
	Data.DykeNaturalDamage = rndPerc32(Data.DykeStateFrac, 10);		// dyke strength decreases every eyar
	Data.DykeRepair = Data.PopOnWall * rndRange32(1, Frac * 4) / Data.LandSize;	// people repair wall (1 person per up to 4 acres)
	Data.DykeStateFracIncrease = Data.DykeRepair - Data.DykeNaturalDamage;			// state of the dyke, add repair remove damage
	#ifdef IN_DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "DykeNaturalDamage: ", DykeNaturalDamage);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "DykeRepair: ", DykeRepair);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "DykeStateFracIncrease: ", DykeStateFracIncrease);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "DykeStateFrac: ", DykeStateFrac + DykeStateFracIncrease);
	#endif
}


static void doLand(void) {
	Data.LandFlooded = 0;
	if (Data.DykeStateFrac < DYKESTATE_MAX_FRAC) {
		int32 LandFloodedFrac = ((Data.DykeStateFrac * Frac) /  DYKESTATE_MAX_FRAC) >> 1;
		Data.LandFlooded = rndPerc32(Data.LandSize, LandFloodedFrac);
	}
	int32 landReclaimedMax = Data.PopOnWall << 1;
	Data.LandReclaimed = rndRange32(0, landReclaimedMax);
	Data.LandIncrease = Data.LandReclaimed - Data.LandFlooded;
	Data.TotalLandFlooded += Data.LandFlooded;
	Data.TotalLandReclaimed += Data.LandReclaimed;
	#ifdef IN_DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "LandFlooded: ", LandFlooded);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "landReclaimedMax: ", landReclaimedMax);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "LandReclaimed: ", LandReclaimed);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "LandIncrease: ", LandIncrease);
	#endif
}


static void doGrain(void) {
	Data.GrainStolen = 0;
	int32 grainGrownMax = Data.GrainPlanted * GRAIN_GROWTH_RATE * Data.PopInFields;	// planted * typical growth * people looking after it
	grainGrownMax = min(grainGrownMax, Data.LandSize * GRAIN_PER_ACRE);
	Data.GrainGrown = rndRange32(grainGrownMax >> 1, grainGrownMax);
	if (Data.LandSize == 0 || Data.GrainGrown == 0) {
		Data.GrainFlooded = 0;
	} else {
		Data.GrainFlooded = ((Data.LandFlooded * Frac) / Data.LandSize) * Data.GrainGrown / Frac;	// ratio land flooded * grain in fields
	}
	if (Data.AttackChance > 0) {
		int32 maxGrainStolen = min(Data.BanditCount * 20, Data.Grains - (Data.GrainPlanted >> 1));
		Data.GrainStolen = rndRange32(maxGrainStolen >> 2, maxGrainStolen);
	}
	Data.GrainIncrease = Data.GrainGrown - Data.GrainFlooded - Data.GrainAte - Data.GrainPlanted - Data.GrainStolen;
	Data.TotalGrainPlanted += Data.GrainPlanted;
	Data.TotalGrainAte += Data.GrainAte;
	Data.TotalGrainStolen += Data.GrainStolen;
	Data.TotalGrainGrown += Data.GrainGrown;
	Data.TotalGrainFlooded += Data.GrainFlooded;

	#ifdef IN_DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "grainGrownMax: ", grainGrownMax);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "GrainGrown: ", GrainGrown);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "GrainFlooded: ", GrainFlooded);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "GrainStolen: ", GrainStolen);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY++), valDebugCol, "GrainIncrease: ", GrainIncrease);
	#endif
}


static void addIncreases(void) {
	if (Data.Population + Data.PopIncrease < 0) {
		Data.PopIncrease = Data.Population * -1;
	}
	Data.Population += Data.PopIncrease;
	if (Data.Grains + Data.GrainIncrease < 0) {
		Data.GrainIncrease = Data.Grains * -1;
	}
	Data.Grains += Data.GrainIncrease;
	Data.DykeStateFrac += Data.DykeStateFracIncrease;
	Data.DykeStateFrac = clamp(Data.DykeStateFrac, DYKESTATE_MIN_FRAC, DYKESTATE_MAX_FRAC);
	if (Data.LandSize + Data.LandIncrease < 0) {
		Data.LandIncrease = Data.LandSize * -1;
	}
	Data.LandSize += Data.LandIncrease;
	Data.Year++;
}
