#include "data.h"
#include "pics/Pics.h"
#include "GameSimYear.h"



void RLC_ShowLandChange(void) {
	ShowChange(LeftSideMargin,0, Data.LandReclaimed, "Land reclaimed ", "Our dyke workers managed to reclaim some land.", ReclaimedPic);
	ShowChange(LeftSideMargin,0, Data.LandFlooded, "Land flooded ", "The rains were bad this year, we lost land to flooding.", FloodPic);
	Show3WayChange(LeftSideMargin,0, Data.LandIncrease, Data.LandSize, "Land ", "Land increase ", "Our land increased this year.", "Land lost ", "We lost land this year.", "Our land remains constant.", LandPic);
}
