#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "FrameWork.h"
#include "Kernel.h"
#include "GameStd.h"


GameRunState GrState;

const uint8 StdTextColour = 20;
const uint8 LoLightTextColour = 12;


const uint8 valCol = 20;
const uint8 valHighlightCol = 252;
const uint8 valColLarge = 224;
const uint8 valColLow = 252;




char buf[EDIT_VALUE_BUF_SIZE];


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
	DebounceReadKeyboard();
	if(Keys[VK_CAPS] != 0 && DebounceKeys[VK_7] != 0) {
		*focus = FocusPrev(editFields, *focus, editFieldCount);
		return false;
	}
	if(Keys[VK_CAPS] != 0 && DebounceKeys[VK_6] != 0) {
		*focus = FocusNext(editFields, *focus, editFieldCount);
		return false;
	}
	if (DebounceKeys[VK_ENTER] != 0) {
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
void PrintResource(uint8 x, uint8 y, int32 total, int32 value, char* str) {
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
	ltoa(value, buf, 10);
	uint8 xv = length + x;
	PrintProp(xv, y, col, buf);
}


// Print a header and value on same row. Useful for displaying total, i.e. total population, etc
void PrintValue(uint8 x, uint8 xv, uint8 y, uint16 col, char *header, int32 value) {
	PrintProp(x, y, col, header);
	ltoa(value, buf, 10);
	PrintProp(xv, y, col, buf);
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

