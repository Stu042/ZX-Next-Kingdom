
CC=zcc -v
CFLAGS= +zxn -mz80n -vn -c -SO3 --list --lstcwd --c-code-in-asm --opt-code-size -clib=sdcc_iy -pragma-include:zpragma.inc --max-allocs-per-node300000

ODIR=obj
LDIR=lib
LIBS=-lm



OBJS =	obj/main.o obj/data.o obj/Kernel.o obj/KernelAsm.o obj/IRQsAsm.o \
	obj/FrontEnd.o obj/FrontEndAsm.o obj/GameData.o \
	obj/GamePlay.o obj/GamePlayAsm.o obj/GamePlayAssignResources.o \
	obj/GameStd.o obj/GamePop.o obj/GameGrain.o obj/GameSimYear.o \
	obj/PlateOfFood.o obj/PlantSeed.o obj/Dyke.o obj/Fields.o obj/Defense.o \
	obj/NewBorn.o obj/GrainBundle.o obj/KingdomState.o obj/Starved.o \
	obj/Scroll.o obj/Flood.o obj/Reclaimed.o obj/Population.o obj/LandPic.o \
	obj/NaturalDeathPic.o obj/BanditDeathsPic.o


kingdom.nex: $(OBJS) 
	$(CC) +zxn -vn -m --list --lstcwd --c-code-in-asm -clib=sdcc_iy -Cz"--clean" -pragma-include:zpragma.inc -startup=1 $(OBJS) -o kingdom.nex -create-app -subtype=nex


# Main program at $8000
obj/main.o: 	main.c FrameWork.h Kernel.h FrontEnd.h GamePlay.h
		$(CC) $(CFLAGS) -o obj/main.o main.c

obj/data.o: 	data.c FrameWork.h Kernel.h
		$(CC) $(CFLAGS) -o obj/data.o data.c


# Kernel section
obj/Kernel.o: Kernel.c Kernel.h includes.inc
	$(CC) $(CFLAGS) --codesegPAGE_02_KERNEL_CODE --constsegPAGE_02_KERNEL_CODE -o obj/Kernel.o Kernel.c
obj/KernelAsm.o: KernelAsm.asm includes.inc
	$(CC) $(CFLAGS) --codesegPAGE_02_KERNEL_CODE --constsegPAGE_02_KERNEL_CODE -o obj/KernelAsm.o KernelAsm.asm

obj/IRQsAsm.o: IRQsAsm.asm includes.inc
	$(CC) $(CFLAGS) --codesegPAGE_02_KERNEL_IRQ --constsegPAGE_02_KERNEL_IRQ -o obj/IRQsAsm.o IRQsAsm.asm


# Front End overlay
obj/FrontEnd.o: FrontEnd.c FrontEnd.h FrameWork.h Kernel.h
	$(CC) $(CFLAGS) --codesegPAGE_08_FRONTEND_SEG --constsegPAGE_08_FRONTEND_SEG -o obj/FrontEnd.o FrontEnd.c 
obj/FrontEndAsm.o: FrontEndAsm.asm
	$(CC) $(CFLAGS) --codesegPAGE_08_FRONTEND_SEG --constsegPAGE_08_FRONTEND_SEG -o obj/FrontEndAsm.o FrontEndAsm.asm

# Game Data, used by Front End and Game Play
obj/GameData.o: GameData.c GameData.h
	$(CC) $(CFLAGS) --codesegPAGE_01_GAMEDATA_SEG --constsegPAGE_01_GAMEDATA_SEG -o obj/GameData.o GameData.c


# Game Play overlay
obj/GamePlay.o: GamePlay.c FrameWork.h Kernel.h
	$(CC) $(CFLAGS) --codesegPAGE_06_GAMEPLAY_SEG --constsegPAGE_06_GAMEPLAY_SEG -o obj/GamePlay.o GamePlay.c
obj/GamePlayAsm.o: GamePlayAsm.asm
	$(CC) $(CFLAGS) --codesegPAGE_08_FRONTEND_SEG --constsegPAGE_08_FRONTEND_SEG -o obj/GamePlayAsm.o GamePlayAsm.asm	



# Game Play, standard functions
obj/GameStd.o: GameStd.c FrameWork.h Kernel.h
	$(CC) $(CFLAGS) --codesegPAGE_06_GAMEPLAY_SEG --constsegPAGE_06_GAMEPLAY_SEG -o obj/GameStd.o GameStd.c

# Game Play, input population assignement
obj/GamePop.o: GamePop.c FrameWork.h Kernel.h
	$(CC) $(CFLAGS) --codesegPAGE_06_GAMEPLAY_SEG --constsegPAGE_06_GAMEPLAY_SEG -o obj/GamePop.o GamePop.c

# Game Play, input grain assignement
obj/GameGrain.o: GameGrain.c FrameWork.h Kernel.h
	$(CC) $(CFLAGS) --codesegPAGE_06_GAMEPLAY_SEG --constsegPAGE_06_GAMEPLAY_SEG -o obj/GameGrain.o GameGrain.c

# Game Play, input grain assignement
obj/GameSimYear.o: GameSimYear.c FrameWork.h Kernel.h
	$(CC) $(CFLAGS) --codesegPAGE_06_GAMEPLAY_SEG --constsegPAGE_06_GAMEPLAY_SEG -o obj/GameSimYear.o GameSimYear.c



# Game Play, player setting resources
obj/GamePlayAssignResources.o: GamePlayAssignResources.c FrameWork.h Kernel.h
	$(CC) $(CFLAGS) --codesegPAGE_06_GAMEPLAY_SEG --constsegPAGE_06_GAMEPLAY_SEG -o obj/GamePlayAssignResources.o GamePlayAssignResources.c


# Pics
obj/PlateOfFood.o: pics/PlateOfFood.asm
	$(CC) $(CFLAGS) --codesegPAGE_30_FOOD_SEG --constsegPAGE_30_FOOD_SEG -o obj/PlateOfFood.o pics/PlateOfFood.asm

obj/PlantSeed.o: pics/PlantSeed.asm
	$(CC) $(CFLAGS) --codesegPAGE_34_SEED_SEG --constsegPAGE_34_SEED_SEG -o obj/PlantSeed.o pics/PlantSeed.asm

obj/Dyke.o: pics/Dyke.asm
	$(CC) $(CFLAGS) --codesegPAGE_38_DYKE_SEG --constsegPAGE_38_DYKE_SEG -o obj/Dyke.o pics/Dyke.asm

obj/Fields.o: pics/Fields.asm
	$(CC) $(CFLAGS) --codesegPAGE_42_FIELDS_SEG --constsegPAGE_42_FIELDS_SEG -o obj/Fields.o pics/Fields.asm

obj/Defense.o: pics/Defense.asm
	$(CC) $(CFLAGS) --codesegPAGE_46_DEFENSE_SEG --constsegPAGE_46_DEFENSE_SEG -o obj/Defense.o pics/Defense.asm

obj/NewBorn.o: pics/NewBorn.asm
	$(CC) $(CFLAGS) --codesegPAGE_50_NEWBORN_SEG --constsegPAGE_50_NEWBORN_SEG -o obj/NewBorn.o pics/NewBorn.asm

obj/GrainBundle.o: pics/GrainBundle.asm
	$(CC) $(CFLAGS) --codesegPAGE_54G_RAINBUNDLE_SEG --constsegPAGE_54G_RAINBUNDLE_SEG -o obj/GrainBundle.o pics/GrainBundle.asm

obj/KingdomState.o: pics/KingdomState.asm
	$(CC) $(CFLAGS) --codesegPAGE_58_KINGDOMSTATE_SEG --constsegPAGE_58_KINGDOMSTATE_SEG -o obj/KingdomState.o pics/KingdomState.asm

obj/Starved.o: pics/Starved.asm
	$(CC) $(CFLAGS) --codesegPAGE_62_STARVED_SEG --constsegPAGE_62_STARVED_SEG -o obj/Starved.o pics/Starved.asm

obj/Scroll.o: pics/Scroll.asm
	$(CC) $(CFLAGS) --codesegPAGE_66_SCROLL_SEG --constsegPAGE_66_SCROLL_SEG -o obj/Scroll.o pics/Scroll.asm

obj/Flood.o: pics/Flood.asm
	$(CC) $(CFLAGS) --codesegPAGE_68_FLOOD_SEG --constsegPAGE_68_FLOOD_SEG -o obj/Flood.o pics/Flood.asm

obj/Reclaimed.o: pics/Reclaimed.asm
	$(CC) $(CFLAGS) --codesegPAGE_72_RECLAIMED_SEG --constsegPAGE_72_RECLAIMED_SEG -o obj/Reclaimed.o pics/Reclaimed.asm

obj/Population.o: pics/Population.asm
	$(CC) $(CFLAGS) --codesegPAGE_76_POPULATION_SEG --constsegPAGE_76_POPULATION_SEG -o obj/Population.o pics/Population.asm

obj/LandPic.o: pics/LandPic.asm
	$(CC) $(CFLAGS) --codesegPAGE_80_LAND_SEG --constsegPAGE_80_LAND_SEG -o obj/LandPic.o pics/LandPic.asm

obj/NaturalDeathPic.o: pics/NaturalDeathPic.asm
	$(CC) $(CFLAGS) --codesegPAGE_84_NATURALDEATH_SEG --constsegPAGE_84_NATURALDEATH_SEG -o obj/NaturalDeathPic.o pics/NaturalDeathPic.asm

obj/BanditDeathsPic.o: pics/BanditDeathsPic.asm
	$(CC) $(CFLAGS) --codesegPAGE_88_BANDITDEATH_SEG --constsegPAGE_88_BANDITDEATH_SEG -o obj/BanditDeathsPic.o pics/BanditDeathsPic.asm



clean:
	rm -f obj/*.o 
	rm -f *.o 
	rm -f *.lis
	rm -f *.bin
	rm -f *.nex
	rm -f *.map


