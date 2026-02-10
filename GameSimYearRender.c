#include "GameSimYear.h"
#include "Kernel.h"
#include "GameStd.h"
#include "data.h"
#include "pics/Pics.h"
#include "RenderPopChange.h"
#include "RenderLandChange.h"
#include "RenderGrainChange.h"
#include "RenderSummary.h"




//////////////////////////////
/// Global vars

//////////////////////////////
// Internal vars


//////////////////////////////
// Internal protos



//////////////////////////////
/// Global functions


void GameSimYearRender(void) {
	ClsL2(0);
	VBlankSwap();
	RPC_PopChange();
	RLC_ShowLandChange();
	RGC_ShowGrainChange();
	RS_ShowSummary();
	ClsL2(0);
	VBlankSwap();
}



//////////////////////////////
// Internal functions


void ShowChange(uint8 x, uint8 y, int32 value, char *valHeader, char *message, uint8 pic) {
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


void Show3WayChange(uint8 x, uint8 y, int32 valueDelta, int32 value, char *valHeader, char *valPlus, char *messagePlus, char *valMinus, char *messageMinus, char *messageConstant, uint8 pic) {
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

