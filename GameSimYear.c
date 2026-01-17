#include "GameSimYear.h"
#include "Kernel.h"
#include "GameStd.h"
#include "GameData.h"
#include "GamePop.h"
#include "GameGrain.h"


//////////////////////////////
/// Global vars





//////////////////////////////
// Internal vars


#define GRAIN_GROWTH_RATE (20)
#define GRAIN_PER_ACRE (20)

#define Frac (100)
#define GSY_TEXT_TOP (128)
#define GSY_TEXTY(offset) ((uint8)(GSY_TEXT_TOP + offset * 8))


#define DYKESTATE_MIN (Frac / 2)
#define DYKESTATE_MAX (Frac * 2)


//////////////////////////////
// Internal protos


static void doPop(void);
static void doDyke(void);
static void doLand(void);
static void doGrain(void);

static void renderYearResult(void);


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
	SetState(State_SimYearRun);
}


#ifdef DEBUG
	uint8 y;
#endif

void GameSimYear(void) {
	#ifdef DEBUG
		y=0;
		ClsL2(0);
	#endif
	doPop();
	doDyke();
	doLand();
	doGrain();
	Population += PopIncrease;
	Grains += GrainIncrease;
	DykeStateFrac += DykeStateFracIncrease;
	DykeStateFrac = clamp(DykeStateFrac, DYKESTATE_MIN, DYKESTATE_MAX);
	LandSize += LandIncrease;
	Year++;
	SetState(State_SimYearRender);
	#ifdef DEBUG
		VBlankSwap();
		HangForKey();
	#endif
}


void GameSimYearRender(void) {
	ClsL2(0);
	VBlankSwap();
	renderYearResult();
	SetState(State_PopInit);
	ClsL2(0);
	VBlankSwap();
}


void GameSimYearPause(void) {
	HangForKey();
	SetState(State_PopInit);
}



//////////////////////////////
// Internal functions


static void renderYearResult(void) {
	uint8 y=0;
	if (Births > 0) {
		ClsL2(0);
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "We were blessed with newborns this year.");
		PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Births ", Births);
		VBlankSwap();
		HangForKey();
	}

	if (DeathsStarvation > 0) {
		ClsL2(0);
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "We did not have enough food to feed our people.");
		PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Starved: ", DeathsStarvation);
		VBlankSwap();
		HangForKey();
	}

	if (DeathsNatural > 0) {
		ClsL2(0);
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "Unfortunately some people died due to natural causes.");
		PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Natural deaths: ", DeathsNatural);
		VBlankSwap();
		HangForKey();
	}

	if (DeathsDefending > 0) {
		ClsL2(0);
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "The bandits attacked and killed some of our people.");
		PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Killed: ", DeathsDefending);
		VBlankSwap();
		HangForKey();
	}
	
	ClsL2(0);
	if (PopIncrease > 0) {
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "All in, our population has increased.");
		PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Population increase: ", PopIncrease);
	} else if (PopIncrease < 0) {
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "This means we now have less people.");
		PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Population decrease: ", PopIncrease * -1);
	} else {
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "Over this year our population remains constant.");
	}
	VBlankSwap();
	HangForKey();


	ClsL2(0);
	if (GrainIncrease > 0) {
		y = 0;
		if (GrainPlanted > 0) {
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain planted: ", GrainPlanted);
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain grown: ", GrainGrown);
		}
		if (GrainFlooded > 0) {
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain lost to flooding: ", GrainFlooded);
		}
		if (GrainsStolen > 0) {
			PrintProp(0, GSY_TEXTY(y++), StdTextColour, "The bandits stole some of our grain.");
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain stolen: ", GrainsStolen);
		}
		PrintProp(0, GSY_TEXTY(y++), StdTextColour, "We produced more grain than we consumed.");
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain Increase: ", GrainIncrease);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain: ", Grains);
	} else if (GrainIncrease < 0) {
		if (GrainPlanted > 0) {
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain planted: ", GrainPlanted);
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain grown: ", GrainGrown);
		}
		if (GrainFlooded > 0) {
			PrintProp(0, GSY_TEXTY(y++), StdTextColour, "The fields flooded and we lost grain.");
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain lost: ", GrainFlooded);
		}
		if (GrainsStolen > 0) {
			PrintProp(0, GSY_TEXTY(y++), StdTextColour, "The bandits stole some of our grain.");
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain stolen: ", GrainsStolen);
		}
		PrintProp(0, GSY_TEXTY(y++), StdTextColour, "We lost grain.");
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Total Grain lost: ", GrainIncrease * - 1);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain: ", Grains);
	} else if (GrainIncrease == 0) {
		if (GrainPlanted > 0) {
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain planted: ", GrainPlanted);
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain grown: ", GrainGrown);
		}
		if (GrainFlooded > 0) {
			PrintProp(0, GSY_TEXTY(y++), StdTextColour, "The fields flooded and we lost grain.");
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain lost: ", GrainFlooded);
		}
		if (GrainsStolen > 0) {
			PrintProp(0, GSY_TEXTY(y++), StdTextColour, "The bandits stole some of our grain.");
			PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain stolen: ", GrainsStolen);
		}
		PrintProp(0, GSY_TEXTY(y++), StdTextColour, "Our grain reserves remain constant.");
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "Grain: ", Grains);
	}
	VBlankSwap();
	HangForKey();

	if (LandReclaimed > 0) {
		ClsL2(0);
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "Our dyke workers managed to reclaim some land.");
		PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Land reclaimed: ", LandReclaimed);
		VBlankSwap();
		HangForKey();
	}
	if (LandFlooded > 0) {
		ClsL2(0);
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "The rains were bad this year, we lost land to flooding.");
		PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Land flooded: ", LandFlooded);
		VBlankSwap();
		HangForKey();
	}

	ClsL2(0);
	if (LandIncrease > 0) {
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "Our land increased this year.");
		PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Land increase: ", LandIncrease);
		PrintSimpleValue(0, GSY_TEXTY(2), StdTextColour, "Land: ", LandSize);
	} else if (LandIncrease < 0) {
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "We lost land this year.");
		PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Land lost: ", LandIncrease * -1);
		PrintSimpleValue(0, GSY_TEXTY(2), StdTextColour, "Land: ", LandSize);
	} else if (LandIncrease == 0) {
		PrintProp(0, GSY_TEXTY(0), StdTextColour, "Our land remains constant.");
		PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Land: ", LandSize);
	}
	VBlankSwap();
	HangForKey();
	
	ClsL2(0);
	PrintProp(0, GSY_TEXTY(0), StdTextColour, "In summary our resources are.");
	PrintSimpleValue(0, GSY_TEXTY(1), StdTextColour, "Population: ", Population);
	PrintSimpleValue(0, GSY_TEXTY(2), StdTextColour, "Grain: ", Grains);
	PrintSimpleValue(0, GSY_TEXTY(3), StdTextColour, "Land: ", LandSize);
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
	DeathsStarvation = rndRange(0, Population - GrainAte);	// the unfed are likely to die
	DeathsNatural = rndPerc(Population, 10);			// plus random upto 10% die
	return DeathsStarvation + DeathsNatural;
}


static int32 doBandits(void) {
	GrainsStolen = 0;
	DeathsDefending = 0;
	BanditsKilled = 0;
	int32 popDefendingFrac;
	if (PopDefending == 0) {
		popDefendingFrac = 1;
	} else {
		popDefendingFrac = PopDefending * Frac;
	}
	int32 attackStrength = (BanditCount * BanditHealthFrac) / popDefendingFrac;
	int32 attackChance = rndRange(attackStrength >> 1, attackStrength);
	if (attackChance >= Frac) {						// Bandits are attacking
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
		int32 maxGrainsStolen = BanditCount * 5;
		maxGrainsStolen = min(BanditCount * 5, Grains);
		GrainsStolen = rndRange(maxGrainsStolen >> 4, maxGrainsStolen);
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
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "GrainsStolen: ", GrainsStolen);
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
	DykeNaturalDamage = rndPerc(DykeStateFrac, 10);
	DykeRepair = PopOnWall * (XorShift() % 20) / LandSize;
	DykeStateFracIncrease = DykeRepair - DykeNaturalDamage;
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeNaturalDamage: ", DykeNaturalDamage);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeRepair: ", DykeRepair);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeIncrease: ", DykeIncrease);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "DykeStateFrac: ", DykeStateFrac + DykeStateFracIncrease);
	#endif
}


static void doLand(void) {
	LandFlooded = 0;
	if (DykeStateFrac < DYKESTATE_MAX) {
		int32 LandFloodedPerc = (DykeStateFrac * 100 /  DYKESTATE_MAX) / 2;
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
	int32 grainGrownMax = GrainPlanted * GRAIN_GROWTH_RATE * PopInFields;	// planted * typical growth * people looking after it
	grainGrownMax = min(grainGrownMax, LandSize * GRAIN_PER_ACRE);
	GrainGrown = rndRange(GrainPlanted << 1, grainGrownMax);
	if (LandSize == 0 || GrainGrown == 0) {
		GrainFlooded = 0;
	} else {
		GrainFlooded = ((LandFlooded * Frac) / LandSize) * GrainGrown / Frac;	// ratio land flooded * grain in fields
	}
	GrainIncrease = GrainGrown - GrainFlooded - GrainAte - GrainPlanted - GrainsStolen;
	#ifdef DEBUG
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "grainGrownMax: ", grainGrownMax);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "GrainGrown: ", GrainGrown);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "GrainFlooded: ", GrainFlooded);
		PrintSimpleValue(0, GSY_TEXTY(y++), StdTextColour, "GrainIncrease: ", GrainIncrease);
	#endif
}

