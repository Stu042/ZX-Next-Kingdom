#include <stdbool.h>
#include <stdlib.h>
#include "Kernel.h"
#include "GamePop.h"
#include "GameStd.h"
#include "GameData.h"
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
	{80, 10, popTopTextPos(32), true,"Working Fields", PopInFieldsStr, &PopInFields, &Population},
	{80, 10, popTopTextPos(40), false,"Working Wall",PopOnWallStr, &PopOnWall, &Population},
	{80, 10, popTopTextPos(48), false,"Defending",PopDefendingStr, &PopDefending, &Population},
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
	PopInFields = 0;
	PopOnWall = 0;
	PopDefending = 0;
	UsedPop = 0;
	UsedPopStr[0] = 0;
	popErrorStr = NULL;
	finished = false;
	ClsL2(0);
	SwapL2();
	ClsL2(0);
	SwapL2();
}


bool GamePopRun(void) {
	input();
	render();
	return finished;
}


bool GamePopValidate(void) {
	if (UsedPop <= Population) {
		return true;
	}
	popErrorStr = popErrorTooManyStr;
	return false;
}




//////////////////////////////
// Internal functions

static void input(void) {
	finished = KeyedInput(editFields, &popIndex, POP_FIELDS_COUNT);
	UsedPop = EditValueCalcTotal(editFields, POP_FIELDS_COUNT);
	ltoa(UsedPop, UsedPopStr, 10);
	if (finished) {
		SetState(State_PopValidate);
	}
}



static void render(void) {
	if (finished) {
		ClsL2(0);
		VBlankSwap();
		return;
	}
	updatePic();
	//ClsLast2(0);
	BlitLargeImageAt(128, ScrollPic, ScrollImageSize);
	PrintMainResources();
	PrintResourceValue(160, popTopTextPos(8), Population, UsedPop, UsedPopStr);
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
	BankGameData();
}

