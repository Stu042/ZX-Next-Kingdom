#include <stdlib.h>
#include "Kernel.h"
#include "GameGrain.h"
#include "GameStd.h"
#include "GameData.h"
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
	{80, 0, grainTopTextPos(32), true,"To Eat", GrainAteStr, &GrainAte, &Grains},
	{80, 0, grainTopTextPos(40), false, "To Plant",GrainPlantedStr, &GrainPlanted, &Grains}
};

static uint8 editFieldsPics[GRAIN_FIELDS_COUNT] = {
	PlateOfFoodPic,PlantSeedPic
};


static char* grainErrorTooManyStr = "Reduce the amount of grain.";


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
	GrainAte = 0;
	GrainPlanted = 0;
	UsedGrain = 0;
	grainErrorStr = NULL;
	SetState(State_GrainsRun);
	ClsL2(0);
	SwapL2();
	ClsL2(0);
	SwapL2();
	updatePic();
}



void GameGrainRun(void) {
	input();
	render();
}


void GameGrainValidate(void) {
	if (UsedGrain <= Grains) {
		SetState(State_SimYearInit);
	} else {	// show error
		grainErrorStr = grainErrorTooManyStr;
		SetState(State_GrainsRun);
	}
}



//////////////////////////////
// Internal functions

static void input(void) {
	int8 oldIndex = grainIndex;
	bool finished = KeyedInput(editFields, &grainIndex, GRAIN_FIELDS_COUNT);
	UsedGrain = EditValueCalcTotal(editFields, GRAIN_FIELDS_COUNT);
	ltoa(UsedGrain, UsedGrainStr, 10);
	if (finished) {
		SetState(State_GrainValidate);
	}
}



static void render(void) {
	updatePic();
	ClsLast2(0);
	PrintMainResources();
	PrintResourceValue(160, grainTopTextPos(24), Grains, UsedGrain, UsedGrainStr);
	PrintEditValues(editFields, GRAIN_FIELDS_COUNT);
	if(grainErrorStr) {
		PrintProp(0, grainTopTextPos(48), StdTextColour, grainErrorStr);
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
	BankGameData();
}

