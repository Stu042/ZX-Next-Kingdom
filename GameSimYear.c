#include <stdlib.h>
#include "GameSimYear.h"
#include "Kernel.h"
#include "GameStd.h"
#include "GameData.h"
#include "GamePop.h"
#include "GameGrain.h"





//////////////////////////////
/// Global vars


int32 PopIncrease;
int32 Births;
int32 DeathsTotal;
int32 DeathsNatural;
int32 DeathsStarvation;

int32 GrainIncrease;
int32 LandIncrease;

int32 DykeNaturalDamage;
int32 DykeRepair;
int32 DykeIncrease;

int32 LandFlooded;
int32 LandReclaimed;
int32 LandIncrease;


//////////////////////////////
// Internal vars


#define GRAIN_GROWTH_RATE (20)
#define GRAIN_PER_ACRE (20)

#define Frac (100)
#define GSY_TEXT_TOP (0)
#define GSY_TEXTY(offset) ((uint8)(GSY_TEXT_TOP + offset * 8))


//////////////////////////////
// Internal protos


static void doPop(void);
static void doDyke(void);
static void doLand(void);
static void doGrain(void);

static void renderText(void);
static int32 rndPerc(int32 val, int32 perc);
static int32 clamp(int32 val, int32 min, int32 max);
static int32 max(int32 val, int32 max);
static int32 min(int32 val, int32 min);
static int32 rndRange(int32 from, int32 to);



//////////////////////////////
/// Global functions


void GameSimYearInit(void) {
	PopIncrease = 0;
	Births = 0;
	DeathsTotal = 0;
	DeathsNatural = 0;
	DeathsStarvation = 0;
	
	GrainIncrease = 0;

	DykeNaturalDamage = 0;
	DykeRepair = 0;
	DykeIncrease = 0;

	LandFlooded = 0;
	LandReclaimed = 0;
	LandIncrease = 0;
	GrState = GRS_SimYearRun;
}

uint8 y;
void GameSimYear(void) {
	#ifdef DEBUG
		y=0;
		ClsL2(0);
	#endif
	doPop();
	doDyke();
	doLand();
	doGrain();
	GrState = GRS_SimYearRender;
	#ifdef DEBUG
		VBlankSwap();
		InitDebounce();
		HangForKey();
	#endif
}

#define DYKESTATE_MIN (Frac / 2)
#define DYKESTATE_MAX (Frac * 2)

void GameSimYearRender(void) {
	ClsL2(0);
	Population += PopIncrease;
	Grains += GrainIncrease;
	DykeState += DykeIncrease;
	DykeState = clamp(DykeState, DYKESTATE_MIN, DYKESTATE_MAX);
	LandSize += LandIncrease;
	renderText();
	GrState = GRS_SimYearPause;
	VBlankSwap();
}


void GameSimYearPause(void) {
	InitDebounce();
	HangForKey();
	GrState = GRS_PopInit;
}



//////////////////////////////
// Internal functions

int32 lazyPop;
int32 fedOkay;

static void renderText(void) {
	uint8 y=0;
	PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Births: ", Births);

	PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Starved: ", DeathsStarvation);
	PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Natural deaths: ", DeathsNatural);

	PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Population Increase: ", PopIncrease);

	PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain Increase: ", GrainIncrease);
	PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Population: ", Population);
	PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain: ", Grains);

	PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Land Increase: ", LandIncrease);
	PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Land: ", LandSize);
}


static void doPop(void) {
	lazyPop = Population - UsedPop;						// people doing nothing, they multiply more than working people
	fedOkay = GrainAte * Frac / Population;					// min amount of food to be happy
	clamp(fedOkay, Frac, 2 * Frac);
	int32 lazyBirthsMax = lazyPop * fedOkay / Frac;
	int32 lazyBirths = rndRange(0, lazyBirthsMax);
	int32 naturalBirths = rndPerc(Population, 10);
	Births = lazyBirths + naturalBirths;					// whole pop gets a chance of 10% increase
	DeathsStarvation = rndRange(0, Population - GrainAte);	// the unfed are very likely to die
	DeathsNatural = rndPerc(Population, 10);			// plus random upto 10% die
	DeathsTotal = DeathsStarvation + DeathsNatural;
	// todo killed
	PopIncrease = Births - DeathsTotal;
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "fedOkay: ", fedOkay);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "lazyBirthsMax: ", lazyBirthsMax);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "lazyBirths: ", lazyBirths);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "naturalBirths: ", naturalBirths);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DeathsStarvation: ", DeathsStarvation);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DeathsNatural: ", DeathsNatural);
	#endif
}


static void doDyke(void) {
	DykeNaturalDamage = rndPerc(DykeState, 10);
	DykeRepair = PopOnWall * (XorShift() % 20) / LandSize;
	DykeIncrease = DykeRepair - DykeNaturalDamage;
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeNaturalDamage: ", DykeNaturalDamage);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeRepair: ", DykeRepair);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeIncrease: ", DykeIncrease);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeState: ", DykeState + DykeIncrease);
	#endif
}


static void doLand(void) {
	if (DykeState < DYKESTATE_MAX) {
		int32 LandFloodedPerc = (DykeState * 100 /  DYKESTATE_MAX) / 2;
		LandFlooded = rndPerc(LandSize, LandFloodedPerc);
	}
	int32 landReclaimedMax = LandSize * PopOnWall / 4;
	LandReclaimed = rndRange(0, landReclaimedMax);
	LandIncrease = LandReclaimed - LandFlooded;
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "LandFlooded: ", LandFlooded);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "landReclaimedMax: ", landReclaimedMax);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "LandReclaimed: ", LandReclaimed);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "LandIncrease: ", LandIncrease);
	#endif
}


static void doGrain(void) {
	int32 grainGrownMax = GrainPlanted * GRAIN_GROWTH_RATE * PopInFields;
	grainGrownMax = min(grainGrownMax, LandSize * GRAIN_PER_ACRE);
	int32 grainGrown = rndRange(grainGrownMax/2, grainGrownMax);
	int32 grainFlooded = LandFlooded * Frac / LandSize * grainGrown / Frac;
	GrainIncrease = grainGrown - grainFlooded - GrainAte - GrainPlanted;
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "grainGrownMax: ", grainGrownMax);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "grainGrown: ", grainGrown);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "grainFlooded: ", grainFlooded);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "GrainIncrease: ", GrainIncrease);
	#endif
}


/// Returns random number starts at From and ends at To - 1 (inclusive)
static int32 rndRange(int32 from, int32 to) {
	int32 diff = to - from;
	if (diff <= 0) {
		return from;
	}
	return (((int32)XorShift()) % diff) + from;
}


static int32 rndPerc(int32 val, int32 perc) {
	if (perc <= 0) {
		return 0;
	}
	int32 result = rndRange(0, (val * perc + 1) / 100);
	return result;
}


static int32 max(int32 val, int32 max) {
	if (val < max) {
		val = max;
	}
	return val;
}

static int32 min(int32 val, int32 min) {
	if (val > min) {
		val = min;
	}
	return val;
}


static int32 clamp(int32 val, int32 min, int32 max) {
	if (val > max) {
		val = max;
	} else if (val < min) {
		val = min;
	}
	return val;
}
