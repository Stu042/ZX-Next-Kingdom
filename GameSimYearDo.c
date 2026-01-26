#include "GameSimYear.h"
#include "Kernel.h"
#include "GameStd.h"
#include "GameData.h"


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
	PopIncrease = 0;
	Births = 0;
	DeathsTotal = 0;
	DeathsNatural = 0;
	DeathsStarvation = 0;
	DeathsDefending = 0;
	
	GrainStolen = 0;
	GrainGrown = 0;
	GrainFlooded = 0;
	GrainIncrease = 0;

	DykeNaturalDamage = 0;
	DykeRepair = 0;
	DykeStateFracIncrease = 0;

	LandFlooded = 0;
	LandReclaimed = 0;
	LandIncrease = 0;

	BanditsKilled = 0;
	AttackChance = 0;
}


#ifdef DEBUG
	uint8 debugY;
#endif



void GameSimYearRun(void) {
	#ifdef DEBUG
		debugY=0;
		ClsL2(0);
	#endif
	doPop();
	doDyke();
	doLand();
	doGrain();
	addIncreases();
	#ifdef DEBUG
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


static int32 doBirths(void) {
	int32 lazyPop = Population - UsedPop;					// people doing nothing, they multiply more than working people
	int32 fedOkayFrac = GrainAte * Frac / Population;				// min amount of food to be happy
	fedOkayFrac = clamp(fedOkayFrac, Frac, 2 * Frac);
	int32 lazyBirthsMax = lazyPop * fedOkayFrac / Frac;
	int32 lazyBirths = rndRange32(0, lazyBirthsMax);
	int32 naturalBirths = rndPerc32(Population, 10);		// whole pop gets a chance of 10% increase
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "fedOkayFrac: ", fedOkayFrac);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "lazyBirthsMax: ", lazyBirthsMax);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "lazyBirths: ", lazyBirths);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "naturalBirths: ", naturalBirths);
	#endif
	return lazyBirths + naturalBirths;
}


static int32 doDeaths(void) {
	int32 hungry = Population - GrainAte;
	if (hungry > 0) {
		DeathsStarvation = rndRange32(0, (Population - GrainAte));	// the unfed are likely to die
	} else {
		DeathsStarvation = 0;
	}
	DeathsNatural = rndPerc32(Population * Frac, 10) / Frac;			// plus random upto 10% die
	return DeathsStarvation + DeathsNatural;
}


static int32 doBandits(void) {
	int32 popDefendingFrac;
	if (PopDefending == 0) {
		popDefendingFrac = 1;
	} else {
		popDefendingFrac = PopDefending * Frac;
	}
	int32 attackStrength = (BanditCount * BanditHealthFrac) / popDefendingFrac;
	AttackChance = rndRange32(attackStrength >> 1, attackStrength);
	if (AttackChance > 0) {						// Bandits are attacking
		int32 overkill = BanditCount - (PopDefending << 1);
		if (overkill > 0){
			DeathsDefending += rndRange32(PopDefending >> 1, PopDefending);
			int32 generalPopAttack = min(overkill, Population - PopDefending);
			DeathsDefending += rndRange32(generalPopAttack >> 2, generalPopAttack >> 1);
			BanditHealthFrac -= Frac / 8;
			BanditsKilled = rndRange32(BanditCount >> 4, BanditCount >> 3);

		} else {
			DeathsDefending = rndRange32(BanditCount >> 2, BanditCount >> 1);
			BanditHealthFrac -= Frac / 4;
			BanditsKilled = rndRange32(BanditCount >> 3, BanditCount >> 1);
		}
	} else {
		BanditHealthFrac += rndRange32(1, Frac / 4);
		BanditCount += rndRange32(0, BanditCount >> 1);
	}
	BanditCount -= BanditsKilled;
	BanditHealthFrac = min(BanditHealthFrac, Frac * 2);
	if (BanditCount <= 0) {
		BanditCount = rndRange32(1, 10);
	}
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "DeathsDefending: ", DeathsDefending);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "BanditsKilled: ", BanditsKilled);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY++), valDebugCol, "BanditHealthFrac: ", BanditHealthFrac);
	#endif
	return DeathsDefending;
}


static void doPop(void) {
	Births = doBirths();
	DeathsTotal = doBandits();
	DeathsTotal += doDeaths();
	PopIncrease = Births - DeathsTotal;
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "DeathsStarvation: ", DeathsStarvation);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "DeathsNatural: ", DeathsNatural);
	#endif
}


static void doDyke(void) {
	DykeNaturalDamage = rndPerc32(DykeStateFrac, 10);		// dyke strength decreases every eyar
	DykeRepair = PopOnWall * rndRange32(1, Frac * 4) / LandSize;	// people repair wall (1 person per up to 4 acres)
	DykeStateFracIncrease = DykeRepair - DykeNaturalDamage;			// state of the dyke, add repair remove damage
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "DykeNaturalDamage: ", DykeNaturalDamage);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "DykeRepair: ", DykeRepair);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "DykeStateFracIncrease: ", DykeStateFracIncrease);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "DykeStateFrac: ", DykeStateFrac + DykeStateFracIncrease);
	#endif
}


static void doLand(void) {
	LandFlooded = 0;
	if (DykeStateFrac < DYKESTATE_MAX_FRAC) {
		int32 LandFloodedFrac = ((DykeStateFrac * Frac) /  DYKESTATE_MAX_FRAC) >> 1;
		LandFlooded = rndPerc32(LandSize, LandFloodedFrac);
	}
	int32 landReclaimedMax = PopOnWall << 1;
	LandReclaimed = rndRange32(0, landReclaimedMax);
	LandIncrease = LandReclaimed - LandFlooded;
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "LandFlooded: ", LandFlooded);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "landReclaimedMax: ", landReclaimedMax);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "LandReclaimed: ", LandReclaimed);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "LandIncrease: ", LandIncrease);
	#endif
}


static void doGrain(void) {
	GrainStolen = 0;
	int32 grainGrownMax = GrainPlanted * GRAIN_GROWTH_RATE * PopInFields;	// planted * typical growth * people looking after it
	grainGrownMax = min(grainGrownMax, LandSize * GRAIN_PER_ACRE);
	GrainGrown = rndRange32(grainGrownMax >> 1, grainGrownMax);
	if (LandSize == 0 || GrainGrown == 0) {
		GrainFlooded = 0;
	} else {
		GrainFlooded = ((LandFlooded * Frac) / LandSize) * GrainGrown / Frac;	// ratio land flooded * grain in fields
	}
	if (AttackChance > 0) {
		int32 maxGrainStolen = min(BanditCount * 20, Grains - (GrainPlanted >> 1));
		GrainStolen = rndRange32(maxGrainStolen >> 2, maxGrainStolen);
	}
	GrainIncrease = GrainGrown - GrainFlooded - GrainAte - GrainPlanted - GrainStolen;
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "grainGrownMax: ", grainGrownMax);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "GrainGrown: ", GrainGrown);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY), valDebugCol, "GrainFlooded: ", GrainFlooded);
		PrintSimpleValue(128, GSY_DEBUGTEXTY(debugY++), valDebugCol, "GrainStolen: ", GrainStolen);
		PrintSimpleValue(0, GSY_DEBUGTEXTY(debugY++), valDebugCol, "GrainIncrease: ", GrainIncrease);
	#endif
}


static void addIncreases(void) {
	if (Population + PopIncrease < 0) {
		PopIncrease = Population * -1;
	}
	Population += PopIncrease;
	if (Grains + GrainIncrease < 0) {
		GrainIncrease = Grains * -1;
	}
	Grains += GrainIncrease;
	DykeStateFrac += DykeStateFracIncrease;
	DykeStateFrac = clamp(DykeStateFrac, DYKESTATE_MIN_FRAC, DYKESTATE_MAX_FRAC);
	if (LandSize + LandIncrease < 0) {
		LandIncrease = LandSize * -1;
	}
	LandSize += LandIncrease;
	Year++;
}
