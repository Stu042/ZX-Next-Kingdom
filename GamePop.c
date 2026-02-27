#include <stdbool.h>
#include <stdlib.h>
#include "Kernel.h"
#include "GamePop.h"
#include "GameStd.h"
#include "data.h"
#include "pics/Pics.h"


//////////////////////////////
/// Global vars



//////////////////////////////
// Internal vars


static char PopInFieldsStr[EDIT_VALUE_BUF_SIZE];
static char PopOnWallStr[EDIT_VALUE_BUF_SIZE];
static char PopDefendingStr[EDIT_VALUE_BUF_SIZE];


static char UsedPopStr[EDIT_VALUE_BUF_SIZE];


#define popTopTextPos(offset) ((uint8)(128 + offset))


#define POP_FIELDS_COUNT (3)
static EditValue editFields[POP_FIELDS_COUNT] = {
	{80, LeftSideMargin, popTopTextPos(32), true,"Working Fields", PopInFieldsStr, &Data.PopInFields, &Data.Population},
	{80, LeftSideMargin, popTopTextPos(40), false,"Working Wall",PopOnWallStr, &Data.PopOnWall, &Data.Population},
	{80, LeftSideMargin, popTopTextPos(48), false,"Defending",PopDefendingStr, &Data.PopDefending, &Data.Population},
};

static uint8 editFieldsPics[POP_FIELDS_COUNT] = {
	FieldsPic,DykePic,DefensePic
};

static int8 popIndex;
static int8 oldIndex;

static char* popErrorStr;
static char* popErrorTooManyStr = "You are trying to use people you don't have.";
static bool finished;



//////////////////////////////
// Internal protos


static void input(void);
static void render(void);
static void updatePic(void);



//////////////////////////////
/// Global functions


void GamePopInit(void) {
	popIndex = 0;
	oldIndex = -1;
	PopInFieldsStr[0] = 0;
	PopOnWallStr[0] = 0;
	PopDefendingStr[0] = 0;
	UsedPopStr[0] = 0;
	popErrorStr = NULL;
	finished = false;
}


bool GamePopRun(void) {
	input();
	render();
	return finished;
}


bool GamePopValidate(void) {
	if (Data.UsedPop <= Data.Population) {
		return true;
	}
	popErrorStr = popErrorTooManyStr;
	return false;
}




//////////////////////////////
// Internal functions

static void input(void) {
	#ifdef AUTOPLAY
		if (Data.Population > 3) {
			Data.PopInFields = Data.Population / 3;
			Data.PopOnWall = Data.Population / 4;
			Data.PopDefending = Data.Population / 5;
		} else {
			Data.PopInFields = Data.Population;
		}
		finished = true;
	#else
		finished = KeyedInput(editFields, &popIndex, POP_FIELDS_COUNT);
	#endif
	Data.UsedPop = EditValueCalcTotal(editFields, POP_FIELDS_COUNT);
	ltoa(Data.UsedPop, UsedPopStr, 10);
}



static void render(void) {
	if (finished) {
		ClsL2(0);
		VBlankSwap();
		return;
	}
	updatePic();
	BlitLargeImageAt(128, ScrollPic, ScrollImageSize);
	PrintMainResources();
	PrintResourceValue(160, popTopTextPos(8), Data.Population, Data.UsedPop, UsedPopStr);
	PrintEditValues(editFields, POP_FIELDS_COUNT);
	if(popErrorStr) {
		PrintProp(10, popTopTextPos(56), StdTextColour, popErrorStr);
	}
	VBlankSwap();
}


static void updatePic(void) {
	if (oldIndex == popIndex) {
		return;
	}
	oldIndex = popIndex;
	if (editFieldsPics[popIndex] != 0) {
		DoubleBlitLargeImage(editFieldsPics[popIndex], LargeImageSize);
	}
}

