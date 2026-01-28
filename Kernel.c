#include <arch/zxn.h>           // ZX Spectrum Next architecture specfic functions
#include <stdarg.h>
#include <stdint.h>             // standard names for ints with no ambiguity 
#include <z80.h>
#include <im2.h>
#include <intrinsic.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <input.h>
#include "FrameWork.h"
#include "data.h"
#include "Kernel.h"


uint8 DebounceKeys[VK_NUM_KEYS];
uint8 PrevDebounceKeys[VK_NUM_KEYS];

char Characters[VK_NUM_KEYS] = {
	255, 'Z', 'X', 'C', 'V',
	'A', 'S', 'D', 'F', 'G',
	'Q', 'W', 'E', 'R', 'T',
	'1', '2', '3', '4', '5',
	'0', '9', '8', '7', '6', 
	'P', 'O', 'I', 'U', 'Y',
	255, 'L', 'K', 'J', 'H',
	' ', 255, 'M', 'N', 'B'
};


// protos
static void chkKeys(char* buf, int usedBufSize);
static void chkNumberKeys(char* buf, int usedBufSize);


void InitDebounce(void) {
	ReadKeyboard();
	for(uint8 i = 0; i < VK_NUM_KEYS; i++) {
		PrevDebounceKeys[i] = Keys[i];
		DebounceKeys[i] = 0;
	}
}

void DebounceReadKeyboard(void) {
	ReadKeyboard();
	for(uint8 i = 0; i < VK_NUM_KEYS; i++) {
		if (i != VK_CAPS && Keys[i] != 0 && PrevDebounceKeys[i] != 0) {	// dont debounce caps
			DebounceKeys[i] = 0;
		} else if (Keys[i] != 0 && PrevDebounceKeys[i] == 0) {
			PrevDebounceKeys[i] = 1;
			DebounceKeys[i] = 1;
		} else if (Keys[i] == 0) {
			PrevDebounceKeys[i] = 0;
			DebounceKeys[i] = 0;
		}
	}
}

void VBlankSwap(void) {
	WaitVBlank();
	SwapL2();
}



uint8 CentreText(char *text) {
	uint8 xpos = (256 - PropPixelLength(text)) >> 1;
	return xpos;
}

// print game name top centre
void PrintPropCentre(uint8 y, uint8 col, char *text) {
	uint8 xpos = CentreText(text);
	PrintProp(xpos, y, col, text);
}


static bool chkDelete(char* buf, int usedBufSize) {
	if (DebounceKeys[VK_CAPS] != 0 && DebounceKeys[VK_0] != 0) {
		buf[usedBufSize-1] = 0;
		return true;
	}
	return false;
}

void StringInput(char* buf, int totalBufSize) {
	int usedBufSize = strlen(buf);
	if (usedBufSize >= (totalBufSize - 1)) {
		chkDelete(buf, usedBufSize);
		return;
	}
	chkKeys(buf, usedBufSize);
}


static void chkKeys(char* buf, int usedBufSize) {
	if (chkDelete(buf, usedBufSize)) {
		return;
	}
	for(uint8 i=0; i < VK_NUM_KEYS; i++) {
		if (DebounceKeys[i] != 0) {
			char c = Characters[i];
			if (c != (char)255) {
				buf[usedBufSize++] = c;
				buf[usedBufSize] = 0;
				return;
			}
		}
	}
}


void NumberInput(char* buf, int totalBufSize) {
	int usedBufSize = strlen(buf);
	if (usedBufSize >= (totalBufSize - 1)) {
		chkDelete(buf, usedBufSize);
		return;
	}
	chkNumberKeys(buf, usedBufSize);
}


static void chkNumberKeys(char* buf, int usedBufSize) {
	if (chkDelete(buf, usedBufSize)) {
		return;
	}
	for(uint8 i=VK_1; i <= VK_6; i++) {
		if (DebounceKeys[i] != 0) {
			char c = Characters[i];
			buf[usedBufSize++] = c;
			buf[usedBufSize] = 0;
			return;
		}
	}
}



bool AnyKey(void) {
	DebounceReadKeyboard();
	for(uint8 i=0; i<VK_NUM_KEYS; i++) {
		if (DebounceKeys[i] != 0) {
			return true;
		}
	}
	return false;
}

void HangForKey(void) {
	while(true) {
		if (AnyKey()) {
			return;
		}
	}
}





/// Returns random number starts at From and ends at To (inclusive)
uint16 rndRange(uint16 from, uint16 to) {
	if (from >= to) {
		return from;
	}
	uint32 diff = to - from;
	return ((XorShift()) % (diff+1)) + from;
}


uint16 rndPerc(uint16 val, uint16 perc) {
	if (perc <= 0) {
		return 0;
	}
	int32 result = rndRange(0, (val * perc + 1) / 100);
	return result;
}

/// Returns random number starts at From and ends at To (inclusive)
uint32 rndRange32(uint32 from, uint32 to) {
	if (from >= to) {
		return from;
	}
	uint16 diff = to - from;
	return ((XorShift32()) % (diff+1)) + from;
}


uint32 rndPerc32(uint32 val, uint32 perc) {
	uint32 result = rndRange32(0, (val * perc + 1) / 100);
	return result;
}


int32 max(int32 val, int32 max) {
	if (val < max) {
		val = max;
	}
	return val;
}

int32 min(int32 val, int32 min) {
	if (val > min) {
		val = min;
	}
	return val;
}


int32 clamp(int32 val, int32 min, int32 max) {
	if (val > max) {
		val = max;
	} else if (val < min) {
		val = min;
	}
	return val;
}
