#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "FrameWork.h"
#include "Kernel.h"
#include "GameStd.h"
#include "data.h"






#define stdTopTextPos(offset) ((uint8)(128 + offset))
void PrintMainResources(void) {
	PrintProp(80, stdTopTextPos(0), LoLightTextColour, "Total");
	PrintProp(160, stdTopTextPos(0), LoLightTextColour, "Used");
	PrintProp(230, stdTopTextPos(0), LoLightTextColour, "Year");
	itoa(Data.Year, Buffer, 10);
	PrintProp(230, stdTopTextPos(8), LoLightTextColour, Buffer);

	PrintValue(LeftSideMargin, 80, stdTopTextPos(8), StdTextColour, "Population", Data.Population);
	PrintValue(LeftSideMargin, 80, stdTopTextPos(16), StdTextColour, "Land", Data.LandSize);
	PrintValue(LeftSideMargin, 80, stdTopTextPos(24), StdTextColour, "Grain", Data.Grains);

	ltoa(Data.UsedPop, Buffer, 10);
	PrintResourceValue(160, stdTopTextPos(8), Data.Population, Data.UsedPop, Buffer);
	ltoa(Data.UsedGrain, Buffer, 10);
	PrintResourceValue(160, stdTopTextPos(24), Data.Grains, Data.UsedGrain, Buffer);
}


/// Focus next editable item. Returns focus index.
int8 FocusNext(EditValue editFields[], int8 focus, int8 editFieldCount) {
	editFields[focus].highlight = false;
	focus++;
	if (focus >= editFieldCount) {
		focus = 0;
	}
	editFields[focus].highlight = true;
	return focus;
}


/// Focus previous editable item. Returns focus index.
int8 FocusPrev(EditValue editFields[], int8 focus, int8 editFieldCount) {
	editFields[focus].highlight = false;
	focus--;
	if (focus < 0) {
		focus = editFieldCount - 1;
	}
	editFields[focus].highlight = true;
	return focus;
}


/// Deal with number input, allows moving focus between fields as well.
bool KeyedInput(EditValue editFields[], int8 *focus, int8 editFieldCount) {
	ReadKeyboard();
	if(Keys[VK_CAPS] != 0 && Debounce(VK_7)) {
		*focus = FocusPrev(editFields, *focus, editFieldCount);
		return false;
	}
	if(Keys[VK_CAPS] != 0 && Debounce(VK_6)) {
		*focus = FocusNext(editFields, *focus, editFieldCount);
		return false;
	}
	if (Debounce(VK_ENTER)) {
		*focus = FocusNext(editFields, *focus, editFieldCount);
		return *focus == 0;
	}
	EditValue *ef = editFields;
	for(int8 i=0; i < editFieldCount; i++) {
		if (ef->highlight) {
			NumberInput(ef->text, EDIT_VALUE_BUF_SIZE);
			*ef->value = atol(ef->text);
			break;
		}
		ef++;
	}
	return false;
}


/// Print a currently used value, coloured comparing with total.
void PrintResourceValue(uint8 x, uint8 y, int32 total, int32 value, char* str) {
	uint8 col = valCol;
	if (value > total) {
		col = valColLarge;
	} else if (value < total) {
		col = valColLow;
	} else {
		col = valCol;
	}
	PrintProp(x, y, col, str);
}


// Print a header with following value.
void PrintSimpleValue(uint8 x, uint8 y, uint16 col, char *header, int32 value) {
	PrintProp(x, y, col, header);
	uint8 length = PropPixelLength(header);
	ltoa(value, Buffer, 10);
	uint8 xv = length + x;
	PrintProp(xv, y, col, Buffer);
}


// Print a header and value on same row. Useful for displaying total, i.e. total population, etc
void PrintValue(uint8 x, uint8 xv, uint8 y, uint16 col, char *header, int32 value) {
	PrintProp(x, y, col, header);
	ltoa(value, Buffer, 10);
	PrintProp(xv, y, col, Buffer);
}



/// Print an editable value, so coloured if too large or too low and if no value print a static cursor.
void PrintEditValue(EditValue *ef) {
	uint8 col = valCol;
	if (ef->highlight) {
		col = valHighlightCol;
	}
	PrintProp(ef->xh, ef->y, valCol, ef->header);
	if (ef->text[0] == 0) {
		PrintProp(ef->x, ef->y, col, "_");
	} else {
		PrintProp(ef->x, ef->y, col, ef->text);
	}
}

void PrintEditValues(EditValue editFields[], uint8 count) {
	EditValue *ef = editFields;
	for(uint8 i=0; i < count; i++) {
		PrintEditValue(ef);
		ef++;
	}
}

int EditValueCalcTotal(EditValue editFields[], int8 count) {
	int32 total = 0;
	EditValue *ef = editFields;
	for(int8 i=0; i < count; i++) {
		total += *ef->value;
		ef++;
	}
	return total;
}


