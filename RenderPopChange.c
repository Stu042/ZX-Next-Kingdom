#include "RenderPopChange.h"
#include "GameStd.h"
#include "data.h"
#include "pics/Pics.h"
#include "GameSimYear.h"


void RPC_PopChange(void) {
	ShowChange(LeftSideMargin, 0, Data.Births, "Births ", "We were blessed with newborns this year.", NewBornPic);
	ShowChange(LeftSideMargin, 0, Data.DeathsStarvation, "Starved ", "We did not have enough food to feed our people.", StarvedPic);
	ShowChange(LeftSideMargin, 0, Data.DeathsNatural, "Natural deaths ", "Unfortunately some people died due to natural causes.", NaturalDeathPic);
	ShowChange(LeftSideMargin, 0, Data.DeathsDefending, "Killed ", "The bandits attacked and killed some of our people.", BanditDeathsPic);
	Show3WayChange(LeftSideMargin, 0, Data.PopIncrease, Data.Population, "Population ", "Population increase ", "All in, our population has increased.", "Population decrease ", "This means we now have less people.", "Over this year our population remains constant.", PopulationPic);
}
