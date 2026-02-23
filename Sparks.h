#ifndef SPARKS_H
#define SPARKS_H
#include "FrameWork.h"
#include <stdbool.h>


#define FIREWORK_NUM_SPARKS (32)
#define FIREWORKS_MAX (4)


typedef struct PARTICLE_MOVEMENT {
	uint16 xFrac, yFrac;
	int16 xGradientFrac;
	int16 yGradientFrac;
	uint16 accFrac;
	uint8 life;
	uint8 maxLife;
}ParticleMovement;

typedef struct PARTICLE_OLD {
	uint8 colour;
	uint8 x;
	uint8 y;
}ParticleOld;

typedef struct SPARK {
	ParticleMovement move;
	uint8 colourIdx;
	uint8 *colours;
	uint8 colCount;
}Spark;



extern void SparksMove(Spark sparks[], uint16 numSparks);
extern void SparksRender(Spark sparks[], uint16 numSparks);
extern bool SparksStillAlive(Spark sparks[], uint16 numSparks);

// set sparks to do a circlular explosion
extern void SparksSetExplosion(uint16 xFrac, uint16 yFrac, Spark sparks[], uint8 numSparks);

extern void ParticleInit(void);

extern inline uint8* ParticleScreenPos(ParticleMovement *pm);

extern inline bool ParticleIsAlive(ParticleMovement *pm);
extern void ParticleOldSave(uint16 xFrac, uint16 yFrac, uint8 pix);
extern void ParticleCls(void);
extern bool ParticleMove(ParticleMovement *pm);


#endif