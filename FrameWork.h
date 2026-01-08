//
//	Main Framework definitions
//
#ifndef __FRAMEWORK_H__
#define __FRAMEWORK_H__

#include <arch/zxn.h>
#include <intrinsic.h>
#include <stdint.h>

#define DEBUG (1)


#define BREAK		{ intrinsic_emit(0xFD); intrinsic_emit(0x00); }
#define EXIT		{ intrinsic_emit(0xDD); intrinsic_emit(0x00); }
#define NextReg(r,v)	ZXN_NEXTREG_helper(r,v)
#define NextRegA(r,var)	ZXN_NEXTREGA_helper(r,var)


// mem addr $0000
#define BankIn00(v) ZXN_NEXTREG_helper(0x50,v)
// mem addr $2000
#define BankIn20(v) ZXN_NEXTREG_helper(0x51,v)
// mem addr $4000
#define BankIn40(v) ZXN_NEXTREG_helper(0x52,v)
// mem addr $6000
#define BankIn60(v) ZXN_NEXTREG_helper(0x53,v)
// mem addr $8000
#define BankIn80(v) ZXN_NEXTREG_helper(0x54,v)
// mem addr $a000
#define BankInA0(v) ZXN_NEXTREG_helper(0x55,v)
// mem addr $c000
#define BankInC0(v) ZXN_NEXTREG_helper(0x56,v)
// mem addr $e000
#define BankInE0(v) ZXN_NEXTREG_helper(0x57,v)

#define SetTransparencyColourFallback(v)	ZXN_NEXTREG_helper(0x4A,v)
#define SetULAControl(v)			ZXN_NEXTREG_helper(0x68,v)


typedef	uint8_t		uint8;
typedef	int8_t		int8;
typedef	uint16_t	uint16;
typedef	int16_t		int16;
typedef	uint32_t	uint32;
typedef	int32_t		int32;
typedef	uint64_t	uint64;
typedef	int64_t		int64;


#define	SetState(state)	GameState=state

typedef enum eGameStateType {
	State_InitFrontEnd = 1,		// init the front end
	State_FrontEnd,			// Process front end
	State_ContinueGame,
	State_NewGame,
	State_LoadGame,

	State_QuitFrontEnd,		// quit the front end

	State_InitGame,			// init the game
	State_Game,			// process the game
	State_QuitGame,			// quit the game
} eGameState;

typedef	struct SHWSprite {
	uint8	X;
	uint8	Y;
	uint8	Palette_FlipRotate;
	uint8	Visible_1_Name;
	uint8	H_N6_Scale_Y8;
} SHWSprite,*PSHWSprite;


#endif // __FRAMEWORK_H__


