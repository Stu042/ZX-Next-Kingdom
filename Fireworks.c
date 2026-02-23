#include "Fireworks.h"
#include "Kernel.h"



static void FireworkRender(Firework *fw);




void FireworksInit(void) {
	ParticleInit();
}


void FireworksRender(Firework allFireworks[], uint8 count) {
	ParticleCls();
	Firework *fw = allFireworks;
	for(uint8 i=0; i < count; i++) {
		FireworkRender(fw++);
	}
}


void FireworksMove(Firework allFireworks[], uint8 count) {
	Firework *fw = allFireworks;
	for(uint8 i=0; i < count; i++) {
		if (fw->flags & FWF_JUST_DO_SPARKS && !(fw->flags & FWF_DEAD)) {
			Spark *spark = fw->sparksBuffer;
			ParticleMovement *move = &spark->move;
			for(uint8 i=0; i < FIREWORK_NUM_SPARKS; i++) {
				ParticleMove(move);
				spark++;
				move = &spark->move;
			}
		} else {
			ParticleMovement *move = &fw->move;
			if (!ParticleMove(move)) {
				fw->flags |= FWF_JUST_DO_SPARKS;
			} else {
				SparksSetExplosion(move->xFrac, move->yFrac, fw->sparksBuffer, FIREWORK_NUM_SPARKS);
			}
		}
		fw++;
	}
}


bool FireworksAnyAlive(Firework allFireworks[], uint8 count) {
	Firework *fw = allFireworks;
	for(uint8 i=0; i < count; i++) {
		if (ParticleIsAlive(&fw->move)) {
			return true;
		}
		fw++;
	}
	fw = allFireworks;
	for(uint8 i=0; i < count; i++) {
		if (!(fw->flags & FWF_DEAD) && SparksStillAlive(fw->sparksBuffer, FIREWORK_NUM_SPARKS)) {
			return true;
		}
		fw->flags |= FWF_DEAD;
		fw++;
	}
	return false;
}




void FireworkFire(Firework *fw) {
	ParticleMovement *move = &fw->move;
	move->xFrac = (uint16)128 << 8;
	move->yFrac = (uint16)191 << 8;
	move->maxLife = rndRange(30, 50);
	move->life = 0;
	move->accFrac = 1 * FRAC;
	// set gradient
	uint16 r = XorShift();
	move->xGradientFrac = r % 600;
	if (r & 0x8000) {
		move->xGradientFrac *= -1;
	}
	r = XorShift();
	move->yGradientFrac = ((r % 400) + 1200) * -1;
	fw->flags = FWF_NONE;
}


static void FireworkRender(Firework *fw) {
	if (fw->flags & FWF_DEAD) {
		return;
	}
	ParticleMovement *move = &fw->move;
	if (ParticleIsAlive(move)) {
		uint8* pix = ParticleScreenPos(move);
		ParticleOldSave(move->xFrac, move->yFrac, *pix);
		*pix = 248;
		return;
	}
	if (fw->flags & FWF_JUST_DO_SPARKS) {
		SparksRender(fw->sparksBuffer, FIREWORK_NUM_SPARKS);
	}
}
