#include <stdbool.h>
#include <stdlib.h>
#include "Kernel.h"
#include "GameGrain.h"
#include "GameStd.h"
#include "data.h"
#include "pics/Pics.h"



//////////////////////////////
/// Global vars


//////////////////////////////
// Internal vars

static int8 grainIndex;
static int8 oldIndex;

static char GrainAteStr[EDIT_VALUE_BUF_SIZE];
static char GrainPlantedStr[EDIT_VALUE_BUF_SIZE];



static char UsedGrainStr[EDIT_VALUE_BUF_SIZE];

static char* grainErrorStr;


#define grainTopTextPos(offset) ((uint8)(128 + offset))

#define GRAIN_FIELDS_COUNT (2)
static EditValue editFields[GRAIN_FIELDS_COUNT] = {
	{80, LeftSideMargin, grainTopTextPos(32), true,"To Eat", GrainAteStr, &Data.GrainAte, &Data.Grains},
	{80, LeftSideMargin, grainTopTextPos(40), false, "To Plant",GrainPlantedStr, &Data.GrainPlanted, &Data.Grains}
};

static uint8 editFieldsPics[GRAIN_FIELDS_COUNT] = {
	PlateOfFoodPic,PlantSeedPic
};


static char* grainErrorTooManyStr = "Reduce the amount of grain.";
static bool finished;

//////////////////////////////
// Internal protos


static void input(void);
static void render(void);
static void calcUsed(void);
static void updatePic(void);


//////////////////////////////
/// Global functions



void GameGrainInit(void) {
	grainIndex = 0;
	oldIndex = -1;
	GrainAteStr[0] = 0;
	GrainPlantedStr[0] = 0;
	UsedGrainStr[0] = 0;
	Data.GrainAte = 0;
	Data.GrainPlanted = 0;
	Data.UsedGrain = 0;
	grainErrorStr = NULL;
	finished = false;
	ClsL2(0);
	SwapL2();
	ClsL2(0);
	SwapL2();
	updatePic();
}



bool GameGrainRun(void) {
	input();
	render();
	return finished;
}


bool GameGrainValidate(void) {
	if (Data.UsedGrain <= Data.Grains) {
		return true;
	} else {	// show error
		grainErrorStr = grainErrorTooManyStr;
		return false;
	}
}



//////////////////////////////
// Internal functions

static void input(void) {
	int8 oldIndex = grainIndex;
	finished = KeyedInput(editFields, &grainIndex, GRAIN_FIELDS_COUNT);
	Data.UsedGrain = EditValueCalcTotal(editFields, GRAIN_FIELDS_COUNT);
	ltoa(Data.UsedGrain, UsedGrainStr, 10);
}



static void render(void) {
	if (finished) {
		ClsL2(0);
		VBlankSwap();
		return;
	}
	updatePic();
	BlitLargeImageAt(128, ScrollPic, ScrollImageSize);
	//ClsLast2(0);
	PrintMainResources();
	PrintResourceValue(160, grainTopTextPos(24), Data.Grains, Data.UsedGrain, UsedGrainStr);
	PrintEditValues(editFields, GRAIN_FIELDS_COUNT);
	if(grainErrorStr) {
		PrintProp(10, grainTopTextPos(48), StdTextColour, grainErrorStr);
	}
	VBlankSwap();
}


static void updatePic(void) {
	if (oldIndex == grainIndex) {
		return;
	}
	oldIndex = grainIndex;
	if (editFieldsPics[grainIndex] != 0) {
		DoubleBlitLargeImage(editFieldsPics[grainIndex], LargeImageSize);
	}
}

