#include <stdlib.h>
#include "Kernel.h"
#include "GameGrain.h"
#include "GameStd.h"
#include "GameData.h"
#include "pics/Pics.h"



//////////////////////////////
/// Global vars

int32 GrainAte;
int32 GrainPlanted;

int32 UsedGrain;


//////////////////////////////
// Internal vars

static int8 grainIndex = 0;

static char GrainAteStr[EDIT_VALUE_BUF_SIZE];
static char GrainPlantedStr[EDIT_VALUE_BUF_SIZE];



static char UsedGrainStr[EDIT_VALUE_BUF_SIZE];

static char* grainErrorStr;


#define grainTopTextPos(offset) ((uint8)(128 + offset))

#define GRAIN_FIELDS_COUNT (2)
static EditValue editFields[GRAIN_FIELDS_COUNT] = {
	{72, 0, grainTopTextPos(28), true,"To Eat", GrainAteStr, &GrainAte, &Grains},
	{72, 0, grainTopTextPos(36), false, "To Plant",GrainPlantedStr, &GrainPlanted, &Grains}
};


static char* grainErrorTooManyStr = "Reduce the amount of grain.";


//////////////////////////////
// Internal protos


static void input(void);
static void render(void);
static void calcUsed(void);



//////////////////////////////
/// Global functions



void GameGrainInit(void) {
	grainIndex = 0;
	GrainAteStr[0] = 0;
	GrainPlantedStr[0] = 0;
	UsedGrainStr[0] = 0;
	GrainAte = 0;
	GrainPlanted = 0;
	UsedGrain = 0;
	grainErrorStr = NULL;
	GrState = GRS_GrainsRun;
	ClsL2(0);
	BlitLargeImage(PlateOfFood, PlateOfFoodSize);
	SwapL2();
	ClsL2(0);
	BlitLargeImage(PlateOfFood, PlateOfFoodSize);
	VBlankSwap();
	BankGameData();
}


void GameGrainRun(void) {
	input();
	render();
}


void GameGrainValidate(void) {
	if (UsedGrain <= Grains) {
		GrState = GRS_SimYearInit;
	} else {	// show error
		grainErrorStr = grainErrorTooManyStr;
		GrState = GRS_GrainsRun;
	}
}



//////////////////////////////
// Internal functions

static void input(void) {
	bool finished = KeyedInput(editFields, &grainIndex, GRAIN_FIELDS_COUNT);
	UsedGrain = EditValueCalcTotal(editFields, GRAIN_FIELDS_COUNT);
	ltoa(UsedGrain, UsedGrainStr, 10);
	if (finished) {
		GrState = GRS_GrainValidate;
	}
}



static void render(void) {
	ClsLast2(0);
	PrintProp(96, grainTopTextPos(0), LoLightTextColour, "Total");
	PrintValue(0, 96, grainTopTextPos(8), StdTextColour, "Grains", Grains);
	PrintProp(176, grainTopTextPos(0), LoLightTextColour, "Used");
	PrintResource(176, grainTopTextPos(8), Grains, UsedGrain, UsedGrainStr);
	
	PrintEditValues(editFields, GRAIN_FIELDS_COUNT);

	if(grainErrorStr) {
		PrintProp(0, grainTopTextPos(48), StdTextColour, grainErrorStr);
	}
	VBlankSwap();
}


