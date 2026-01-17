
CC=zcc -v
CFLAGS= +zxn -mz80n -vn -c -SO3 --list --lstcwd --c-code-in-asm --opt-code-size -clib=sdcc_iy -pragma-include:zpragma.inc --max-allocs-per-node300000

ODIR=obj
LDIR=lib
LIBS=-lm



OBJS =	obj/main.o obj/data.o obj/Kernel.o obj/KernelAsm.o obj/IRQsAsm.o \
	obj/FrontEnd.o obj/FrontEndAsm.o obj/GameData.o \
	obj/GamePlay.o obj/GamePlayAsm.o obj/GamePlayAssignResources.o \
	obj/GameStd.o obj/GamePop.o obj/GameGrain.o obj/GameSimYear.o \
	obj/PlateOfFood.o obj/PlantSeed.o obj/Dyke.o obj/Fields.o


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
	$(CC) $(CFLAGS) --codesegPAGE_34_SEED_SEG --constsegPAGE_30_FOOD_SEG -o obj/PlantSeed.o pics/PlantSeed.asm

obj/Dyke.o: pics/Dyke.asm
	$(CC) $(CFLAGS) --codesegPAGE_38_DYKE_SEG --constsegPAGE_30_FOOD_SEG -o obj/Dyke.o pics/Dyke.asm

obj/Fields.o: pics/Fields.asm
	$(CC) $(CFLAGS) --codesegPAGE_42_FIELDS_SEG --constsegPAGE_30_FOOD_SEG -o obj/Fields.o pics/Fields.asm



clean:
	rm -f obj/*.o 
	rm -f *.o 
	rm -f *.lis
	rm -f *.bin
	rm -f *.nex
	rm -f *.map


