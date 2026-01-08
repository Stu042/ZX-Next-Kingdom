#include <stdlib.h>
#include "Kernel.h"
#include "GamePop.h"
#include "GameStd.h"
#include "GameData.h"



//////////////////////////////
/// Global vars


int32 PopInFields;
int32 PopOnWall;
int32 PopDefending;

int32 UsedPop;



//////////////////////////////
// Internal vars


static char PopInFieldsStr[EDIT_VALUE_BUF_SIZE];
static char PopOnWallStr[EDIT_VALUE_BUF_SIZE];
static char PopDefendingStr[EDIT_VALUE_BUF_SIZE];


static char UsedPopStr[EDIT_VALUE_BUF_SIZE];


#define popTopTextPos(offset) ((uint8)(128 + offset))


#define POP_FIELDS_COUNT (3)
static EditValue editFields[POP_FIELDS_COUNT] = {
	{96, 0, popTopTextPos(24), true,"Working Fields", PopInFieldsStr, &PopInFields, &Population},
	{96, 0, popTopTextPos(32), false,"Working Wall",PopOnWallStr, &PopOnWall, &Population},
	{96, 0, popTopTextPos(40), false,"Defending",PopDefendingStr, &PopDefending, &Population},
};

static int8 fieldIndex;

static char* popErrorStr;
static char* popErrorTooManyStr =   "Reduce the amount of Pop.";



//////////////////////////////
// Internal protos


static void input(void);
static void render(void);



//////////////////////////////
/// Global functions


void GamePopInit(void) {
	fieldIndex = 0;
	PopInFieldsStr[0] = 0;
	PopOnWallStr[0] = 0;
	PopDefendingStr[0] = 0;
	PopInFields = 0;
	PopOnWall = 0;
	PopDefending = 0;
	UsedPop = 0;
	UsedPopStr[0] = 0;
	popErrorStr = NULL;
	GrState = GRS_PopRun;
}


void GamePopRun(void) {
	input();
	render();
}


void GamePopValidate(void) {
	if (UsedPop <= Population) {
		GrState = GRS_GrainsInit;
	} else {	// show error
		popErrorStr = popErrorTooManyStr;
		GrState = GRS_PopRun;
	}
}




//////////////////////////////
// Internal functions

static void input(void) {
	bool finished = KeyedInput(editFields, &fieldIndex, POP_FIELDS_COUNT);
	UsedPop = EditValueCalcTotal(editFields, POP_FIELDS_COUNT);
	ltoa(UsedPop, UsedPopStr, 10);
	if (finished) {
		GrState = GRS_PopValidate;
	}
}



static void render(void) {
	ClsL2(0);
	PrintProp(96, popTopTextPos(0), LoLightTextColour, "Total");

	PrintValue(0, 96, popTopTextPos(8), StdTextColour, "Population", Population);
	PrintValue(0, 96, popTopTextPos(16), StdTextColour, "Land", LandSize);
	PrintProp(176, popTopTextPos(0), LoLightTextColour, "Used");
	PrintResource(176, popTopTextPos(8), Population, UsedPop, UsedPopStr);
	
	PrintEditValues(editFields, POP_FIELDS_COUNT);

	if(popErrorStr) {
		PrintProp(0, popTopTextPos(48), StdTextColour, popErrorStr);
	}
	VBlankSwap();
}


 
