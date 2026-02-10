#include <im2.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <z80.h>
#include <arch/zxn.h>
#include <input.h>
#include <errno.h>
#include <arch/zxn.h>


#include "FrameWork.h"
#include "Kernel.h"
#include "data.h"

//-------------------------------------
//	Global Variables here
//-------------------------------------

const char GameName[8] = "Kingdom";
const char GameVersion[8] = "0.1.001";
const char SaveGameFilename[] = "savegame.king";

GameData Data;
HiScores PlayerHiScores;
bool SaveGameExists;

