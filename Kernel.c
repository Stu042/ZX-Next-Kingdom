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


FileStatsBuffer FileStatsBuf;

char Buffer[BUFFER_SIZE];
char MadeBy[] = "Made By Stu v";



// internal vars


static uint8 debounce[VK_NUM_KEYS];

static const char Characters[VK_NUM_KEYS] = {
	255, 'Z', 'X', 'C', 'V',
	'A', 'S', 'D', 'F', 'G',
	'Q', 'W', 'E', 'R', 'T',
	'1', '2', '3', '4', '5',
	'0', '9', '8', '7', '6', 
	'P', 'O', 'I', 'U', 'Y',
	255, 'L', 'K', 'J', 'H',
	' ', 255, 'M', 'N', 'B'
};

// internal protos

static bool chkKeys(char* buf, int usedBufSize);
static void chkNumberKeys(char* buf, int usedBufSize);
static bool chkDelete(char* buf, int usedBufSize);



// /////////
// Display


void VBlankSwap(void) {
	WaitVBlank();
	SwapL2();
}


uint8 CentreText(char *text) {
	uint8 xpos = (DISPLAY_WIDTH - PropPixelLength(text)) >> 1;
	return xpos;
}

// print game name top centre
void PrintPropCentre(uint8 y, uint8 col, char *text) {
	uint8 xpos = CentreText(text);
	PrintProp(xpos, y, col, text);
}


void PrintVersion(void) {
	strcpy(Buffer, MadeBy);
	strcpy(&Buffer[strlen(MadeBy)], GameVersion);
	PrintPropCentre(184, 4, Buffer);
}

// /////////
// Input



// call ReadKeyboard() ideally only once per frame before calling.
bool Debounce(uint8 key) {
	if (Keys[key] == 0) {		// is not pressed
		debounce[key] = false;	// flag as previously not pressed
		return false;
	}
	if (debounce[key]) {		// previously pressed (and checked)
		return false;
	}
	if (key != VK_CAPS) {		// dont debounce caps key
		debounce[key] = true;	// flag as previously pressed
	}
	return true;			// return pressed
}



bool StringInput(char* buf, int totalBufSize) {
	int usedBufSize = strlen(buf);
	if (usedBufSize >= (totalBufSize - 1)) {
		chkDelete(buf, usedBufSize);
		return false;
	}
	return chkKeys(buf, usedBufSize);
}


void NumberInput(char* buf, int totalBufSize) {
	int usedBufSize = strlen(buf);
	if (usedBufSize >= (totalBufSize - 1)) {
		chkDelete(buf, usedBufSize);
		return;
	}
	chkNumberKeys(buf, usedBufSize);
}


// ReadKeyboard() is called internally
bool AnyKey(void) {
	ReadKeyboard();
	for(uint8 i=0; i<VK_NUM_KEYS; i++) {
		if (Debounce(i)) {
			return true;
		}
	}
	return false;
}


// ReadKeyboard() is called internally
void HangForKey(void) {
	while(true) {
		if (AnyKey()) {
			return;
		}
	}
}



static bool chkKeys(char* buf, int usedBufSize) {
	if (chkDelete(buf, usedBufSize)) {
		return false;
	}
	for(uint8 i=0; i < VK_NUM_KEYS; i++) {
		if (Debounce(i)) {
			if (i == VK_ENTER) {
				return true;
			}
			char c = Characters[i];
			if (c != (char)255) {
				buf[usedBufSize++] = c;
				buf[usedBufSize] = 0;
				return false;
			}
		}
	}
	return false;
}

static bool chkDelete(char* buf, int usedBufSize) {
	if (Debounce(VK_CAPS) && Debounce(VK_0)) {
		buf[usedBufSize-1] = 0;
		return true;
	}
	return false;
}

static void chkNumberKeys(char* buf, int usedBufSize) {
	if (chkDelete(buf, usedBufSize)) {
		return;
	}
	for(uint8 i=VK_1; i <= VK_6; i++) {
		if (Debounce(i)) {
			char c = Characters[i];
			buf[usedBufSize++] = c;
			buf[usedBufSize] = 0;
			return;
		}
	}
}





// /////////
// Random numbers


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




#include <stdarg.h>
#include <stdlib.h>


// Issue, when grabing a 1 byte vararg, grab it as 2 byte and cast.
void PPrintf(uint8 x, uint8 y, uint8 col, const char *mess, ...) {
	PrintCharSetCol(col);
	if (y > 191) {
		return;
	}
	va_list args;
	va_start(args, mess);
	uint8 dx = 0;
	const char *str = mess;
	while(*str != 0) {
		switch(*str) {
			case '%':
				str++;
				if (*str == '%') {		// printing a %
					dx = PrintChar(x, y, '%');
				} else {			// printing a var argument
					switch(*str) {
						case 'b': {	// signed integer byte
							int b = (int8)va_arg(args, int16);	// makes no sense this needs to be int16 to grab a int8 value
							itoa(b, Buffer, 10);
							break;
						}
						case 'B': {	// unsigned integer byte
							unsigned int ub = (uint8)va_arg(args, uint16);	// makes no sense this needs to be uint16 to grab a uint8 value
							itoa(ub, Buffer, 10);
							break;
						}
						case 'w': {	// signed 2 byte integer
							int w = (int)va_arg(args, int16);
							itoa(w, Buffer, 10);
						}
						case 'W': {	// unsigned 2 byte integer
							unsigned int uw = va_arg(args, uint16);
							itoa(uw, Buffer, 10);
							break;
						}
						case 'l': {	// signed 4 byte integer
							long l = (int32)va_arg(args, int32);
							ltoa(l, Buffer, 10);
							break;
						}
						case 'L': {	// unsigned 4 byte integer
							long ul = va_arg(args, uint32);
							ltoa(ul, Buffer, 10);
							break;
						}
					}
					dx = PropPixelLength(Buffer);
					if (x + dx < x) {
						y += 8;
						x = 0;
						if (y > 191) {
							break;
						}
					}
					PrintProp(x, y, col, Buffer);
				}
				break;
			case '/':
				str++;
				if (*str == '/') {
					dx = PrintChar(x, y, '/');
				} else if (*str == 'n') {
					dx = 0;
					y += 8;
					x = 0;
				}
				break;
			default:
				dx = PrintChar(x, y, *str);
				break;
		}
		if (x + dx < x) {
			y += 8;
			x = 0;
		} else {
			x += dx;
		}
		if (y > 191) {
			break;
		}
		str++;
	}
	va_end(args);
}
