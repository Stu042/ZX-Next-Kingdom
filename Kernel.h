//
// Kernel functions
//
#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <stdbool.h>
#include "FrameWork.h"



/////////////////////
// defines

#define MIN(a,b) ((a) < (b)? (a) : (b))

#define DISPLAY_WIDTH (256)
#define DISPLAY_HEIGHT (192)

// bank8k page 2 to mem 0xe000
#define BankKernel()	ZXN_NEXTREG_helper(0x57,2)

//page in rom
#define BankRom()	\
			ZXN_NEXTREG_helper(0x50,255)	\
			ZXN_NEXTREG_helper(0x51,255)

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


typedef enum ESX_DOS_ERROR {
	ESX_Ok = 0,
	ESX_Eok = 1,
	ESX_Nonsense = 2,
	ESX_Stend = 3,
	ESX_Wrtype = 4,
	ESX_Noent = 5,
	ESX_Io = 6,
	ESX_Inval = 7,
	ESX_Acces = 8,
	ESX_Nospc = 9,
	ESX_Nxio = 10,
	ESX_Nodrv = 11,
	ESX_Nfile = 12,
	ESX_Badf = 13,
	ESX_Nodev = 14,
	ESX_Overflow = 15,
	ESX_Isdir = 16,
	ESX_Notdir = 17,
	ESX_Exist = 18,
	ESX_Path = 19,
	ESX_Sys = 20,
	ESX_Nametoolong = 21,
	ESX_Nocmd = 22,
	ESX_Inuse = 23,
	ESX_Rdonly = 24,
	ESX_Verify = 25,
	ESX_Loadingko = 26,
	ESX_Dirinuse = 27,
	ESX_Mapramactive = 28,
	ESX_Drivebusy = 29,
	ESX_Fsunknown = 30,
	ESX_Devicebusy = 31
}EsxDosError;


typedef enum FOPEN_MODE {
	FOPEN_MODE_READ			= 0x01,			// request read access
	FOPEN_MODE_WRITE		= 0x02,			// request write access
	FOPEN_MODE_USE_HEADER		= 0x40,			// read/write +3DOS header
	FOPEN_MODE_OPEN_EXIST		= 0x00,			// only open existing file
	FOPEN_MODE_OPEN_CREATE		= 0x08,			// open existing or create file
	FOPEN_MODE_CREATE_NOEXIST	= 0x04,			// create new file, error if exists
	FOPEN_MODE_CREATE_TRUNC		= 0x0c			// create new file, delete existing
}FOpenMode;


typedef struct FILE_STATS_BUFFER {
	uint8 Star;		// +0(1) '*';
	uint8 Q;		// +1(1) $81;
	uint8 Attr;		// +2(1) file attributes (MS-DOS format);
	uint16 TimeStamp;	// +3(2) timestamp (MS-DOS format);
	uint16 DateStamp;	// +5(2) datestamp (MS-DOS format);
	uint32 FileSize;	// +7(4) file size in bytes
}FileStatsBuffer;


#define BUFFER_SIZE (32)



/////////////////////
// Global variables


/// General text buffer, for itoa etc
extern char Buffer[BUFFER_SIZE];

extern char MadeBy[];
extern char Version[8];


// Buffer used by assembly functions fstat and stat
extern FileStatsBuffer FileStatsBuf;


extern	uint8		VBlank;
extern	uint8		Port123b;
extern	uint8		Keys[VK_NUM_KEYS];
extern	uint8		RawKeys[8];


/////////////////////
// Global functions


// From .c

extern bool Debounce(uint8 key);
extern void StringInput(char* buf, int totalBufSize);
extern void NumberInput(char* buf, int totalBufSize);

extern uint8 CentreText(const char *text);
extern void PrintPropCentre(uint8 y, uint8 col, char *text);

extern void VBlankSwap(void);

extern bool AnyKey(void);
extern void HangForKey(void);

extern void PrintVersion(void);


// From .asm

extern void	SetUpIRQs(void) __preserves_regs(b,c,d,e,h,l,iyl,iyh);
extern void	WaitVBlank(void)  __preserves_regs(b,c,d,e,h,l,iyl,iyh);
extern void	InitL2(void) __z88dk_fastcall __preserves_regs(d,e,iyl,iyh);

extern void	Border(uint8 colour) __z88dk_fastcall __preserves_regs(b,c,d,e,h,l,iyl,iyh);

extern void	ClsL2(uint8 col) __z88dk_fastcall;
extern void	ClsFirst4(uint8 col) __z88dk_fastcall;
extern void	ClsLast2(uint8 col) __z88dk_fastcall;

extern void	SwapL2(void);


extern void	UploadCopper(uint8* pCopper, uint16 length)  __z88dk_callee __preserves_regs(d,e,iyl,iyh);
extern void	DMACopy(uint16 src, uint16 dest, uint16 size) __z88dk_callee __preserves_regs(a,d,e,iyl,iyh);
extern void	DMAFill(uint16 dst, uint16 len, uint8 val) __z88dk_callee __preserves_regs(a,d,e,iyl,iyh);
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



// //////////////////////////////////////////////////////
// File system functions


// Set drive to default drive, pages in rom as well
extern EsxDosError EsxGetDrive(void) __z88dk_fastcall;
extern EsxDosError EsxOpen(const char *filename, FOpenMode mode) __z88dk_callee __preserves_regs(iyl,iyh);
extern EsxDosError EsxRead(uint8 *memory, int16 length) __z88dk_callee __preserves_regs(iyl,iyh);
extern EsxDosError EsxWrite(uint8 *memory, int16 length) __z88dk_callee __preserves_regs(iyl,iyh);
extern EsxDosError EsxClose(void) __z88dk_fastcall;

extern EsxDosError FileStats(const char *filename) __z88dk_fastcall __preserves_regs(iyl,iyh);


#endif	//__KERNEL_H__
