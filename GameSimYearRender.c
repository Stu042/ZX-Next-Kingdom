#include "GameSimYear.h"
#include "Kernel.h"
#include "GameStd.h"
#include "data.h"
#include "pics/Pics.h"


//////////////////////////////
/// Global vars





//////////////////////////////
// Internal vars


#define GSY_TEXT_TOP (128)
#ifdef DEBUG
	uint8 GSY_TEXTY(uint8 offset) {
		if (((uint8)(GSY_TEXT_TOP + (offset) * 8)) > (192 - 8)) {
			Border(INK_RED);	// warn if we try to print off screen
		}
		return ((uint8)(GSY_TEXT_TOP + (offset) * 8));
	}
#else
	#define GSY_TEXTY(offset) ((uint8)(GSY_TEXT_TOP + (offset) * 8))
#endif


static int8 picIndex;
static uint8 pics[] = {
	NewBornPic
};



//////////////////////////////
// Internal protos


static void renderYearResult(void);
static void updatePic(int8 index);
static void showPopChange(void);
static void showGrainChange(void);
static void showLandChange(void);
static void showSummary(void);


//////////////////////////////
/// Global functions


void GameSimYearRender(void) {
	picIndex = 0;
	ClsL2(0);
	VBlankSwap();
	renderYearResult();
	ClsL2(0);
	VBlankSwap();
}



//////////////////////////////
// Internal functions



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
	showChange(LeftSideMargin, 0, Data.Births, "Births ", "We were blessed with newborns this year.", NewBornPic);
	showChange(LeftSideMargin, 0, Data.DeathsStarvation, "Starved ", "We did not have enough food to feed our people.", StarvedPic);
	showChange(LeftSideMargin, 0, Data.DeathsNatural, "Natural deaths ", "Unfortunately some people died due to natural causes.", NaturalDeathPic);
	showChange(LeftSideMargin, 0, Data.DeathsDefending, "Killed ", "The bandits attacked and killed some of our people.", BanditDeathsPic);
	show3WayChange(LeftSideMargin, 0, Data.PopIncrease, Data.Population, "Population ", "Population increase ", "All in, our population has increased.", "Population decrease ", "This means we now have less people.", "Over this year our population remains constant.", PopulationPic);
}


static void showGrainChange(void) {
	uint8 y=0;
	BlitLargeImage(GrainBundlePic, LargeImageSize);
	BlitLargeImageAt(128, ScrollPic, ScrollImageSize);
	if (Data.GrainIncrease > 0) {
		if (Data.GrainPlanted > 0) {
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain planted ", Data.GrainPlanted);
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain grown ", Data.GrainGrown);
		}
		if (Data.GrainFlooded > 0) {
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain lost to flooding ", Data.GrainFlooded);
		}
		if (Data.GrainStolen > 0) {
			PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The bandits stole some of our grain.");
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain stolen ", Data.GrainStolen);
		}
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "We produced more grain than we consumed.");
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain increase ", Data.GrainIncrease);
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain ", Data.Grains);
	} else if (Data.GrainIncrease < 0) {
		if (Data.GrainPlanted > 0) {
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain planted ", Data.GrainPlanted);
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain grown ", Data.GrainGrown);
		}
		if (Data.GrainFlooded > 0) {
			PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The fields flooded and we lost grain.");
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain lost ", Data.GrainFlooded);
		}
		if (Data.GrainStolen > 0) {
			char banditsStoleTxt[] = "The bandits stole some of our grain.";
			uint8 banditsStoleTxtLen = PropPixelLength(banditsStoleTxt);
			PrintProp(LeftSideMargin, GSY_TEXTY(y), StdTextColour, banditsStoleTxt);
			PrintSimpleValue(LeftSideMargin + banditsStoleTxtLen, GSY_TEXTY(y++), StdTextColour, "Grain stolen ", Data.GrainStolen);
		}
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "We consumed more grain than we produced.");
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain decrease ", Data.GrainIncrease * -1);
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain ", Data.Grains);
	} else if (Data.GrainIncrease == 0) {
		if (Data.GrainPlanted > 0) {
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain planted ", Data.GrainPlanted);
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain grown ", Data.GrainGrown);
		}
		if (Data.GrainFlooded > 0) {
			PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The fields flooded and we lost grain.");
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain lost ", Data.GrainFlooded);
		}
		if (Data.GrainStolen > 0) {
			PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The bandits stole some of our grain.");
			PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain stolen ", Data.GrainStolen);
		}
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Our grain reserves remain constant.");
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain ", Data.Grains);
	}
	VBlankSwap();
	HangForKey();
}


static void showLandChange(void) {
	showChange(LeftSideMargin,0, Data.LandReclaimed, "Land reclaimed ", "Our dyke workers managed to reclaim some land.", ReclaimedPic);
	showChange(LeftSideMargin,0, Data.LandFlooded, "Land flooded ", "The rains were bad this year, we lost land to flooding.", FloodPic);
	show3WayChange(LeftSideMargin,0, Data.LandIncrease, Data.LandSize, "Land ", "Land increase ", "Our land increased this year.", "Land lost ", "We lost land this year.", "Our land remains constant.", LandPic);
}


static void showSummary(void) {
	int8 y = 0;
	BlitLargeImageAt(128, ScrollPic, ScrollImageSize);
	BlitLargeImage(KingdomStatePic, LargeImageSize);
	if (Data.Population <= 0) {
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "All of our people have died. You have failed to rule");
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "your people.");
	} else if (Data.Grains <= 0) {
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "With no grains, our people have no hope for survival");
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "and have left to find a ruler who can rule.");
	} else if (Data.LandSize <= 0) {
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "With no land, our people have no where to live and have");
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "left to find a ruler who can rule.");
	}
	if (Data.Population <= 0 || Data.Grains <= 0 || Data.LandSize <= 0) {
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The kingdom is no more.");
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Your reign ends in the year ", Data.Year);
	} else {
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "In the year ", Data.Year);
		PrintProp(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "The state of our kingdom is...");
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Population: ", Data.Population);
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Grain: ", Data.Grains);
		PrintSimpleValue(LeftSideMargin, GSY_TEXTY(y++), StdTextColour, "Land: ", Data.LandSize);
	}
	VBlankSwap();
	HangForKey();
}


static void updatePic(int8 index) {
	if (pics[index] != 0) {
		DoubleBlitLargeImage(pics[index], LargeImageSize);
	}
}

