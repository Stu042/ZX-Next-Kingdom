//
// Kernel functions
//
#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdbool.h>
#include "FrameWork.h"



/////////////////////
// defines

#define BankKernel()	ZXN_NEXTREG_helper(0x57,2)

#define SetCpu3Mhz()	NextReg(0x7, 1)
#define SetCpu14Mhz()	NextReg(0x7, 2)
#define SetCpu28Mhz()	NextReg(0x7, 3)


// half row 1
#define VK_CAPS		0
#define VK_Z		1
#define VK_X		2
#define VK_C		3
#define VK_V		4
// half row 2
#define VK_A		5
#define VK_S		6
#define VK_D		7
#define VK_F		8
#define VK_G		9
// half row 3
#define VK_Q		10
#define VK_W		11
#define VK_E		12
#define VK_R		13
#define VK_T		14
// half row 4
#define VK_1		15
#define VK_2		16
#define VK_3		17
#define VK_4		18
#define VK_5		19
// half row 5
#define VK_0		20
#define VK_9		21
#define VK_8		22
#define VK_7		23
#define VK_6		24
// half row 6
#define VK_P		25
#define VK_O		26
#define VK_I		27
#define VK_U		28
#define VK_Y		29
// half row 7
#define VK_ENTER	30
#define VK_L		31
#define VK_K		32
#define VK_J		33
#define VK_H		34
// half row 8
#define VK_SPACE	35
#define VK_SYM		36
#define VK_M		37
#define VK_N		38
#define VK_B		39
#define VK_NUM_KEYS	40



/////////////////////
// Global variables

extern	uint8		VBlank;
extern	uint8		Port123b;
extern	uint8		Keys[VK_NUM_KEYS];
extern	uint8		RawKeys[8];
extern	SHWSprite	SpriteData[128];
extern	uint8		SpriteShape[512];
extern	uint8		PrintOffset;			// offset from $4000 for 


/////////////////////
// Global functions


// From .c

extern uint8 DebounceKeys[VK_NUM_KEYS];

extern void InitDebounce(void);
extern void DebounceReadKeyboard(void);
// extern void Render1Bpp(uint8 x, uint8 y, uint16 col, uint8* oneBpp);

extern void StringInput(char* buf, int totalBufSize);
extern void NumberInput(char* buf, int totalBufSize);

extern uint8 CentreText(const char *text);
extern void PrintPropCentre(uint8 y, uint8 col, char *text);

extern void VBlankSwap(void);

extern bool AnyKey(void);
extern void HangForKey(void);




// From .asm

extern void	InitKernel(void);
extern void	SetUpIRQs(void) __preserves_regs(b,c,d,e,h,l,iyl,iyh);
extern void	WaitVBlank(void)  __preserves_regs(b,c,d,e,h,l,iyl,iyh);
extern void	InitL2(void) __z88dk_fastcall __preserves_regs(d,e,iyl,iyh);
extern void	CopySpriteData(void) __z88dk_callee;

// Wipe all sprites
extern void	WipeSprites(void) __z88dk_callee;

extern void	Border(uint8 colour) __z88dk_fastcall __preserves_regs(b,c,d,e,h,l,iyl,iyh);

// Attribute format: F_B_PPP_III
extern void	ClsATTR(uint8 attrib) __z88dk_fastcall __preserves_regs(iyl,iyh);
extern void	ClsULA(void) __z88dk_fastcall __preserves_regs(iyl,iyh);

extern void	ClsL2(uint8 col) __z88dk_fastcall;
extern void	ClsFirst4(uint8 col) __z88dk_fastcall;
extern void	ClsLast2(uint8 col) __z88dk_fastcall;

extern void	SwapL2(void);

extern void	InitSpriteData(void) __z88dk_callee;

extern void	UploadCopper(uint8* pCopper, uint16 length)  __z88dk_callee __preserves_regs(d,e,iyl,iyh);
extern void	DMACopy(uint16 src, uint16 dest, uint16 size) __z88dk_callee __preserves_regs(a,d,e,iyl,iyh);
extern void	DMAFill(uint16 dst, uint16 len, uint8 val) __z88dk_callee __preserves_regs(a,d,e,iyl,iyh);
extern void	UploadSprites(uint8 StartShape, uint8 NumberOfShapes, uint16* pShapeAddress ) __z88dk_callee __preserves_regs(iyl,iyh);
extern void	ReadKeyboard(void) __z88dk_callee;

extern void	PrintULA(uint8 x, uint8 y, char* text) __z88dk_callee __preserves_regs(iyl,iyh);
extern void	PrintL2(uint8 x, uint8 y, uint8 colour, char* text) __z88dk_callee;

extern uint8	Load(char* pName, uint16 bank, uint16 offset) __z88dk_callee __preserves_regs(iyl,iyh);
extern uint16	ReadNextReg(uint16 reg) __z88dk_callee __preserves_regs(iyl,iyh);

extern void BlitTransImage(uint8 x, uint8 y, uint16 transCol, uint8* imageSrc) __z88dk_callee __preserves_regs(iyl,iyh);
extern void Render(uint8 x, uint8 y, uint8* imageSrc) __z88dk_callee __preserves_regs(iyl,iyh);
extern void Render1Bpp(uint8 x, uint8 y, uint16 col, uint8* oneBpp) __z88dk_callee __preserves_regs(iyl,iyh);


/// Blit full width image by the bank, i.e. starting image at imageBank, for bankCount renders to top of display.
extern void BlitLargeImage(uint8 imageBank, uint8 bankCount) __z88dk_callee;
// Render an image that spans across multiple banks, screenY must be on the start of a bank boundary.
extern void BlitLargeImageAt(uint8 screenY, uint8 imageBank, uint8 bankCount) __z88dk_callee;

/// Same as BlitLargeImage() but renders to both on and off screen displays.
extern void DoubleBlitLargeImage(uint8 imageBank, uint8 bankCount) __z88dk_callee;


extern void PrintProp(uint8 x, uint8 y, uint8 col, char *text) __z88dk_callee;
extern uint8 PropPixelLength(char *text) __z88dk_fastcall;

extern void XorShiftRndSeed(void) __z88dk_fastcall __preserves_regs(iyl,iyh);
#define MAX_XOR_SHIFT (65534)
extern uint16 XorShift(void) __z88dk_fastcall __preserves_regs(iyl,iyh);

extern void SaveGame(void) __z88dk_fastcall __preserves_regs(iyl,iyh);


/// Returns random number starts at From and ends at To - 1 (inclusive)
extern uint16 rndRange(uint16 from, uint16 to);
extern uint16 rndPerc(uint16 val, uint16 perc);
extern int32 max(int32 val, int32 max);
extern int32 min(int32 val, int32 min);
extern int32 clamp(int32 val, int32 min, int32 max);

extern uint32 XorShift32(void) __z88dk_fastcall __preserves_regs(iyl,iyh);
extern void XorShiftRndSeed32(void) __z88dk_fastcall __preserves_regs(b, c, d, e, h, l, iyl,iyh);

/// Returns random number starts at From and ends at To (inclusive)
extern uint32 rndRange32(uint32 from, uint32 to);
extern uint32 rndPerc32(uint32 val, uint32 perc);

#endif	//__KERNEL_H__
