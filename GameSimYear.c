#include "GameSimYear.h"
#include "Kernel.h"
#include "GameStd.h"
#include "GameData.h"
#include "GamePop.h"
#include "GameGrain.h"
#include "pics/Pics.h"


//////////////////////////////
/// Global vars





//////////////////////////////
// Internal vars


#define GRAIN_GROWTH_RATE (20)
#define GRAIN_PER_ACRE (40)

#define Frac (256)
#define GSY_TEXT_TOP (128)
#define GSY_TEXTY(offset) ((uint8)(GSY_TEXT_TOP + offset * 8))


#define DYKESTATE_MIN_FRAC (Frac / 2)
#define DYKESTATE_OKAY_FRAC (Frac)
#define DYKESTATE_MAX_FRAC (Frac * 2)


static int8 picIndex;
static uint8 pics[] = {
	NewBornPic
};



//////////////////////////////
// Internal protos


static void doPop(void);
static void doDyke(void);
static void doLand(void);
static void doGrain(void);

static void renderYearResult(void);
static void updatePic(int8 index);
static void showPopChange(void);
static void showGrainChange(void);
static void showLandChange(void);
static void showSummary(void);
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
	
	GrainsStolen = 0;
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


	picIndex = 0;
}


#ifdef DEBUG
	uint8 y;
#endif



void GameSimYearRun(void) {
	#ifdef DEBUG
		y=0;
		ClsL2(0);
	#endif
	doPop();
	doDyke();
	doLand();
	doGrain();
	addIncreases();
	#ifdef DEBUG
		VBlankSwap();
		HangForKey();
	#endif
}


void GameSimYearRender(void) {
	ClsL2(0);
	VBlankSwap();
	renderYearResult();
	ClsL2(0);
	VBlankSwap();
}



//////////////////////////////
// Internal functions


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



static void renderYearResult(void) {
	showPopChange();
	showLandChange();
	showGrainChange();
	showSummary();
}


static void showChange(uint8 x, uint8 y, int32 value, char *valHeader, char *message, uint8 pic) {
	if (value > 0) {
		if (pic != 0) {
			DoubleBlitLargeImage(pic, LargeImageSize);
			BankGameData();
		} else {
			ClsFirst4(0);
		}
		BlitLargeImageAt(128, ScrollPic, ScrollImageSize);
		PrintProp(x, GSY_TEXTY(y++), StdTextColour, message);
		PrintSimpleValue(x, GSY_TEXTY(y), StdTextColour, valHeader, value);
		VBlankSwap();
		HangForKey();
	}	
}


static void show3WayChange(uint8 x, uint8 y, int32 valueDelta, int32 value, char *valHeader, char *valPlus, char *messagePlus, char *valMinus, char *messageMinus, char *messageConstant, uint8 pic) {
	if (pic != 0) {
		BlitLargeImage(pic, LargeImageSize);
		ClsLast2(0);
	} else {
		ClsL2(0);
	}
	BlitLargeImageAt(128, ScrollPic, ScrollImageSize);
	if (valueDelta > 0) {
		PrintProp(x, GSY_TEXTY(y++), StdTextColour, messagePlus);
		PrintSimpleValue(x, GSY_TEXTY(y++), StdTextColour, valPlus, valueDelta);
	} else if (valueDelta < 0) {
		PrintProp(x, GSY_TEXTY(y++), StdTextColour, messageMinus);
		PrintSimpleValue(x, GSY_TEXTY(y++), StdTextColour, valMinus, valueDelta * -1);
	} else {
		PrintProp(x, GSY_TEXTY(y++), StdTextColour, messageConstant);
	}
	PrintSimpleValue(x, GSY_TEXTY(y), StdTextColour, valHeader, value);
	VBlankSwap();
	HangForKey();
}


static void showPopChange(void) {
	showChange(LeftSideMargin, 0, Births, "Births ", "We were blessed with newborns this year.", NewBornPic);
	showChange(LeftSideMargin, 0, DeathsStarvation, "Starved ", "We did not have enough food to feed our people.", StarvedPic);
	showChange(LeftSideMargin, 0, DeathsNatural, "Natural deaths ", "Unfortunately some people died due to natural causes.", NaturalDeathPic);
	showChange(LeftSideMargin, 0, DeathsDefending, "Killed ", "The bandits attacked and killed some of our people.", BanditDeathsPic);
	show3WayChange(LeftSideMargin, 0, PopIncrease, Population, "Population ", "Population increase ", "All in, our population has increased.", "Population decrease ", "This means we now have less people.", "Over this year our population remains constant.", PopulationPic);
}


static void showGrainChange(void) {
	uint8 y=0;
	//ClsL2(0);
	BlitLargeImage(GrainBundlePic, LargeImageSize);
	BlitLargeImageAt(128, ScrollPic, ScrollImageSize);
	if (GrainIncrease > 0) {
		if (GrainPlanted > 0) {
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain planted ", GrainPlanted);
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain grown ", GrainGrown);
		}
		if (GrainFlooded > 0) {
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain lost to flooding ", GrainFlooded);
		}
		if (GrainsStolen > 0) {
			PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The bandits stole some of our grain.");
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain stolen ", GrainsStolen);
		}
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "We produced more grain than we consumed.");
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain increase ", GrainIncrease);
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain ", Grains);
	} else if (GrainIncrease < 0) {
		if (GrainPlanted > 0) {
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain planted ", GrainPlanted);
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain grown ", GrainGrown);
		}
		if (GrainFlooded > 0) {
			PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The fields flooded and we lost grain.");
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain lost ", GrainFlooded);
		}
		if (GrainsStolen > 0) {
			PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The bandits stole some of our grain.");
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain stolen ", GrainsStolen);
		}
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "We consumed more grain than we produced.");
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain decrease ", GrainIncrease * -1);
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain: ", Grains);
	} else if (GrainIncrease == 0) {
		if (GrainPlanted > 0) {
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain planted ", GrainPlanted);
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain grown ", GrainGrown);
		}
		if (GrainFlooded > 0) {
			PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The fields flooded and we lost grain.");
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain lost ", GrainFlooded);
		}
		if (GrainsStolen > 0) {
			PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The bandits stole some of our grain.");
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain stolen ", GrainsStolen);
		}
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Our grain reserves remain constant.");
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain ", Grains);
	}
	VBlankSwap();
	HangForKey();
}


static void showLandChange(void) {
	showChange(LeftSideMargin,0, LandReclaimed, "Land reclaimed ", "Our dyke workers managed to reclaim some land.", ReclaimedPic);
	showChange(LeftSideMargin,0, LandFlooded, "Land flooded ", "The rains were bad this year, we lost land to flooding.", FloodPic);
	show3WayChange(LeftSideMargin,0, LandIncrease, LandSize, "Land ", "Land increase ", "Our land increased this year.", "Land lost ", "We lost land this year.", "Our land remains constant.", LandPic);
}


static void showSummary(void) {
	int8 y = 0;
	BlitLargeImageAt(128, ScrollPic, ScrollImageSize);
	BlitLargeImage(KingdomStatePic, LargeImageSize);
	if (Population <= 0) {
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "All of our people have died. You have failed to rule");
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "your people.");
	} else if (Grains <= 0) {
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "With no grains, our people have no hope for survival");
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "and have left to find a ruler who can rule.");
	} else if (LandSize <= 0) {
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "With no land, our people have no where to live and have");
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "left to find a ruler who can rule.");
	}
	if (Population <= 0 || Grains <= 0 || LandSize <= 0) {
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The kingdom is no more.");
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Your reign ends in the year ", Year);
	} else {
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "In the year ", Year);
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The state of our kingdom is...");
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Population: ", Population);
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain: ", Grains);
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Land: ", LandSize);
	}
	VBlankSwap();
	HangForKey();
}



static int32 doBirths(void) {
	int32 lazyPop = Population - UsedPop;					// people doing nothing, they multiply more than working people
	int32 fedOkayFrac = GrainAte * Frac / Population;				// min amount of food to be happy
	fedOkayFrac = clamp(fedOkayFrac, Frac, 2 * Frac);
	int32 lazyBirthsMax = lazyPop * fedOkayFrac / Frac;
	int32 lazyBirths = rndRange(0, lazyBirthsMax);
	int32 naturalBirths = rndPerc(Population, 10);		// whole pop gets a chance of 10% increase
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "fedOkay: ", fedOkay);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "lazyBirthsMax: ", lazyBirthsMax);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "lazyBirths: ", lazyBirths);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "naturalBirths: ", naturalBirths);
	#endif
	return lazyBirths + naturalBirths;
}

static int32 doDeaths(void) {
	DeathsStarvation = rndRange(0, (Population - GrainAte) + 1);	// the unfed are likely to die
	DeathsNatural = rndPerc(Population * Frac, 10) / Frac;			// plus random upto 10% die
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
	AttackChance = rndRange(attackStrength >> 1, attackStrength);
	if (AttackChance > 0) {						// Bandits are attacking
		int32 overkill = BanditCount - (PopDefending << 1);
		if (overkill > 0){
			DeathsDefending += rndRange(PopDefending >> 1, PopDefending);
			int32 generalPopAttack = min(overkill, Population - PopDefending);
			DeathsDefending += rndRange(generalPopAttack >> 2, generalPopAttack >> 1);
			BanditHealthFrac -= Frac >> 3;
			BanditsKilled = rndRange(BanditCount >> 4, BanditCount >> 3);

		} else {
			DeathsDefending = rndRange(BanditCount >> 2, BanditCount >> 1);
			BanditHealthFrac -= Frac >> 2;
			BanditsKilled = rndRange(BanditCount >> 3, BanditCount >> 1);
		}
	} else {
		BanditHealthFrac += rndRange(1, Frac / 8);
		BanditCount += rndRange(0, 2);
	}
	BanditCount -= BanditsKilled;
	BanditHealthFrac = min(BanditHealthFrac, Frac * 2);
	if (BanditCount <= 0) {
		BanditCount = rndRange(1, 10);
	}
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DeathsDefending: ", DeathsDefending);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "BanditsKilled: ", BanditsKilled);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "BanditHealth: ", BanditHealth);
	#endif
	return DeathsDefending;
}


static void doPop(void) {
	Births = doBirths();
	DeathsTotal = doBandits();
	DeathsTotal += doDeaths();
	PopIncrease = Births - DeathsTotal;
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DeathsStarvation: ", DeathsStarvation);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DeathsNatural: ", DeathsNatural);
	#endif
}


static void doDyke(void) {
	DykeNaturalDamage = rndPerc(DykeStateFrac, 10);		// dyke strength decreases every eyar
	DykeRepair = PopOnWall * rndRange(1, 4) / LandSize;		// people repair wall (1 person per up to 4 acres)
	DykeStateFracIncrease = DykeRepair - DykeNaturalDamage;			// state of the dyke, add repair remove damage
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeNaturalDamage: ", DykeNaturalDamage);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeRepair: ", DykeRepair);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeIncrease: ", DykeIncrease);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeStateFrac: ", DykeStateFrac + DykeStateFracIncrease);
	#endif
}


static void doLand(void) {
	LandFlooded = 0;
	if (DykeStateFrac < DYKESTATE_OKAY_FRAC) {
		int32 LandFloodedFrac = (DykeStateFrac * Frac /  DYKESTATE_MAX_FRAC) >> 1;
		LandFlooded = rndPerc(LandSize, LandFloodedFrac);
	}
	int32 landReclaimedMax = PopOnWall << 1;
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
	GrainsStolen = 0;
	int32 grainGrownMax = GrainPlanted * GRAIN_GROWTH_RATE * PopInFields;	// planted * typical growth * people looking after it
	grainGrownMax = min(grainGrownMax, LandSize * GRAIN_PER_ACRE);
	GrainGrown = rndRange(grainGrownMax >> 1, grainGrownMax);
	if (LandSize == 0 || GrainGrown == 0) {
		GrainFlooded = 0;
	} else {
		GrainFlooded = ((LandFlooded * Frac) / LandSize) * GrainGrown / Frac;	// ratio land flooded * grain in fields
	}
	if (AttackChance > 0) {
		int32 maxGrainsStolen = BanditCount * 5;
		maxGrainsStolen = min(BanditCount * 5, Grains - (GrainPlanted >> 1));
		GrainsStolen = rndRange(maxGrainsStolen >> 4, maxGrainsStolen);
	}
	GrainIncrease = GrainGrown - GrainFlooded - GrainAte - GrainPlanted - GrainsStolen;
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "grainGrownMax: ", grainGrownMax);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "GrainGrown: ", GrainGrown);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "GrainFlooded: ", GrainFlooded);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "GrainsStolen: ", GrainsStolen);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "GrainIncrease: ", GrainIncrease);
	#endif
}




static void updatePic(int8 index) {
	if (pics[index] != 0) {
		DoubleBlitLargeImage(pics[index], LargeImageSize);
		BankGameData();
	}
}

