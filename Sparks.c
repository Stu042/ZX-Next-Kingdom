#include "Sparks.h"
#include "Kernel.h"


#define GRAVITY (8)



static void ParticleSetGradient(ParticleMovement *pm, uint16 gradLimit);


#define NUM_COLOURS (8)
static uint8 redColIdx[NUM_COLOURS] = {224, 192, 160, 128, 96, 64, 32, 0};
static uint8 greenColIdx[NUM_COLOURS] = {24, 20, 16, 8, 12, 8, 4, 0};
static uint8 purpleColIdx[NUM_COLOURS] = {249, 213, 177, 141, 105, 69, 33, 0};

#define NUM_COLOUR_INDICES (3)
static uint8* colIndices[NUM_COLOUR_INDICES] = {redColIdx, greenColIdx, purpleColIdx};


// clear screen buffer
#define FIREWORKS_CLEAR_BUFF_SIZE (FIREWORKS_MAX + FIREWORK_NUM_SPARKS * FIREWORKS_MAX)
static ParticleOld old[2][FIREWORKS_CLEAR_BUFF_SIZE];
static uint16 oldIndex[2] = {0,0};
static uint8 bufferNum;




void ParticleInit(void) {
	bufferNum = 0;
}

void SparksMove(Spark sparks[], uint16 numSparks) {
	Spark *spark = sparks;
	for(uint16 i = 0; i < numSparks; i++) {
		ParticleMove(&spark->move);
		spark++;
	}
}

void SparksRender(Spark sparks[], uint16 numSparks) {
	Spark *spark = sparks;
	for(uint16 i = 0; i < numSparks; i++) {
		ParticleMovement *pm = &spark->move;
		if (!ParticleIsAlive(pm)) {
			continue;
		}
		uint8* pix = ParticleScreenPos(pm);
		uint16 life = pm->life;
		uint16 maxLife = pm->maxLife;
		spark->colourIdx = (uint8)(life * spark->colCount / maxLife);
		ParticleOldSave(pm->xFrac, pm->yFrac, *pix);
		*pix = spark->colours[spark->colourIdx];
		spark++;
	}
}


bool SparksStillAlive(Spark sparks[], uint16 numSparks) {
	Spark *spark = sparks;
	for(uint16 i = 0; i < numSparks; i++) {
		if (ParticleIsAlive(&spark->move)) {
			return true;
		}
		spark++;
	}
	return false;
}


// set sparks to do a circlular explosion
void SparksSetExplosion(uint16 xFrac, uint16 yFrac, Spark sparks[], uint8 numSparks) {
	Spark *spark = sparks;
	ParticleMovement *move;
	uint8 *col = colIndices[rndRange(0, NUM_COLOUR_INDICES-1)];
	for(uint16 i = 0; i < numSparks; i++) {
		move = &spark->move;
		move->xFrac = xFrac;
		move->yFrac = yFrac;
		move->maxLife = rndRange(20, 50);
		move->life = 0;
		move->accFrac = 4 * FRAC;
		spark->colours = col;
		spark->colCount = NUM_COLOURS;
		ParticleSetGradient(&spark->move, 180);
		spark++;
	}
}



static void ParticleSetGradient(ParticleMovement *pm, uint16 gradLimit) {
	uint16 r = XorShift();
	pm->xGradientFrac = r % gradLimit;
	if (r & 0x8000) {
		pm->xGradientFrac *=  -1;
	}
	r = XorShift();
	pm->yGradientFrac = r % gradLimit;
	if (r & 0x8000) {
		pm->yGradientFrac *=  -1;
	}
	uint32 totalLength = pm->xGradientFrac * pm->xGradientFrac + pm->yGradientFrac * pm->yGradientFrac;
	uint32 gradSqrd = gradLimit * gradLimit;
	if (totalLength > gradSqrd / 2) {
		uint32 tlFrac = (gradSqrd * FRAC) / totalLength;
		pm->xGradientFrac = (pm->xGradientFrac * tlFrac) / FRAC;
		pm->yGradientFrac = (pm->yGradientFrac * tlFrac) / FRAC;
	}
}


bool ParticleMove(ParticleMovement *pm) {
	if (ParticleIsAlive(pm)) {
		uint32 lifeFrac = ((uint32)(pm->maxLife - pm->life) * FRAC) / pm->maxLife;
		int16 movement = (int32)pm->xGradientFrac * lifeFrac / FRAC * pm->accFrac / FRAC;
		pm->xFrac += movement;
		movement = (uint32)pm->yGradientFrac * lifeFrac / FRAC * pm->accFrac / FRAC;
		pm->yFrac += movement;
		pm->yGradientFrac += GRAVITY;
		pm->life++;
		return true;
	}
	return false;
}

inline uint8* ParticleScreenPos(ParticleMovement *pm) {
	uint8* pix = GetPixelAddress((uint8)(pm->xFrac >> 8), (uint8)(pm->yFrac >> 8));
	return pix;
}

inline bool ParticleIsAlive(ParticleMovement *pm) {
	return pm->life < pm->maxLife;
}


void ParticleOldSave(uint16 xFrac, uint16 yFrac, uint8 pix) {
	ParticleOld *p = old[bufferNum];
	uint8 x = (uint8)(xFrac >> 8);
	uint8 y = (uint8)(yFrac >> 8);
	uint16 count = oldIndex[bufferNum];
	for(uint16 i=0; i < count; i++){
		if (x == p->x && y == p->y) {
			return;
		}
		p++;
	}
	p->x = x;
	p->y = y;
	p->colour = pix;
	oldIndex[bufferNum]++;
}


void ParticleCls(void) {
	bufferNum ^= 1;
	ParticleOld *p = old[bufferNum];
	for(uint16 i=0; i < oldIndex[bufferNum]; i++) {
		uint8* pix = GetPixelAddress(p->x, p->y);
		*pix = p->colour;
		p++;
	}
	oldIndex[bufferNum] = 0;
}
