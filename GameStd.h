#ifndef GameStd_h
#define GameStd_h
#include <stdint.h>
#include <stdbool.h>
#include "FrameWork.h"



#define EDIT_VALUE_BUF_SIZE (16)




typedef struct EDIT_VALUE {
	uint8 x;
	uint8 xh;
	uint8 y;
	bool highlight;
	char* header;
	char* text;
	int32* value;
	int32* total;
}EditValue;



/////////////////////
// Proto vars


// Start of text on left side
#define LeftSideMargin (8)

extern const uint8 StdTextColour;
extern const uint8 LoLightTextColour;

extern const uint8 valCol;
extern const uint8 valHighlightCol;
extern const uint8 valColLarge;
extern const uint8 valColLow;



/////////////////////
// Proto functions


extern void PrintMainResources(void);

/// Focus next editable item. Returns focus index.
extern int8 FocusNext(EditValue editFields[], int8 focus, int8 editFieldCount);

/// Focus previous editable item. Returns focus index.
extern int8 FocusPrev(EditValue editFields[], int8 focus, int8 editFieldCount);

/// Deal with number input, allows moving focus between fields as well.
extern bool KeyedInput(EditValue editFields[], int8* focus, int8 editFieldCount);

/// Print a currently used value, coloured comparing with total.
extern void PrintResourceValue(uint8 x, uint8 y, int32 total, int32 value, char* str);

// Print a header and value following.
extern void PrintSimpleValue(uint8 x, uint8 y, uint16 col, char *header, int32 value);

// Print a header and value on same row. Useful for displaying total, i.e. total population, etc
extern void PrintValue(uint8 x, uint8 xv, uint8 y, uint16 col, char *header, int32 value);

/// Print an editable value, so coloured if too large or too low and if no value print a static cursor.
extern void PrintEditValue(EditValue *ef);
extern void PrintEditValues(EditValue editFields[], uint8 count);

extern int EditValueCalcTotal(EditValue editFields[], int8 count);


/// Returns random number starts at From and ends at To - 1 (inclusive)
extern int32 rndRange(int32 from, int32 to);
extern int32 rndPerc(int32 val, int32 perc);
extern int32 max(int32 val, int32 max);
extern int32 min(int32 val, int32 min);
extern int32 clamp(int32 val, int32 min, int32 max);

#endif
