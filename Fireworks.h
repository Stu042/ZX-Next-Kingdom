#ifndef FIREWORKS_H
#define FIREWORKS_H
#include "stdbool.h"
#include "FrameWork.h"
#include "Sparks.h"



typedef enum FIREWORK_FLAGS {
	FWF_NONE = 0,
	FWF_JUST_DO_SPARKS = 1,
	FWF_DEAD = 2
}FireworkFlags;



typedef struct FIREWORK {
	ParticleMovement move;
	FireworkFlags flags;
	Spark sparksBuffer[FIREWORK_NUM_SPARKS];
}Firework;




extern void FireworksInit(void);
extern void FireworksRender(Firework allFireworks[], uint8 count);
extern void FireworksMove(Firework allFireworks[], uint8 count);
extern bool FireworksAnyAlive(Firework allFireworks[], uint8 count);

extern void FireworkFire(Firework *fw);


#endif
