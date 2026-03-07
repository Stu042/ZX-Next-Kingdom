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

GameData Data;
HiScores PlayerHiScores = {
	.Scores = {
		{ .Name = "LIZZIE II", .Years = 70 },
		{ .Name = "JANAAB", .Years = 68 },
		{ .Name = "JOEY", .Years = 67 },
		{ .Name = "SMOKE JAG", .Years = 67 },
		{ .Name = "LIL VIC", .Years = 63 },
		{ .Name = "LIZ", .Years = 44 },
		{ .Name = "AUGUSTUS", .Years = 40 },
		{ .Name = "SEJONG", .Years = 31 },
		{ .Name = "GEORGE VI", .Years = 15 },
		{ .Name = "LIONHEART", .Years = 10 },
	}
};


bool IsEndGame(void) {
	return (Data.LandSize <= 0 || Data.Population <= 0 || Data.Grains <= 0);
}