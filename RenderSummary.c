#include "RenderSummary.h"
#include "Kernel.h"
#include "data.h"
#include "pics/Pics.h"
#include "GameSimYear.h"




void RS_ShowSummary(void) {
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
