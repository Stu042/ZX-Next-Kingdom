#include "Kernel.h"
#include "GameStd.h"
#include "data.h"
#include "pics/Pics.h"
#include "GameSimYear.h"
#include <stdbool.h>



bool grainIncrease(void) {
	uint8 y=0;
	if (Data.GrainIncrease < 0) {
		return false;
	}
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
	return true;
}


bool grainDecrease(void) {
	uint8 y=0;
	if (Data.GrainIncrease > 0) {
		return false;
	}
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
	return true;
}


void grainZero(void) {
	uint8 y=0;
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

void renderSection(void) {
	if(grainIncrease()) {
		return;
	}
	if(grainDecrease()) {
		return;
	}
	grainZero();
}


void RGC_ShowGrainChange(void) {
	BlitLargeImage(GrainBundlePic, LargeImageSize);
	BlitLargeImageAt(128, ScrollPic, ScrollImageSize);
	renderSection();
	VBlankSwap();
	HangForKey();
}

