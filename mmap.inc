# *******************************************************************************************
#
#	Default Banks:	10,11,4,5,0,1
#	Free Banks:	3
#
#
#	+--------------------------------------------------------+
#	|             Kernel+IRQ 2=$E000-$FFFF			 |
#	+--------------------------------------------------------+
#	|                  Spare 3				 |
#	+--------------------------------------------------------+
#	|             MainCode 4,5=$8000-$BFFF			 |
#	+--------------------------------------------------------+
#	|             GamePlay 6,7=$4000-$7FFF			 |
#	+--------------------------------------------------------+
#	|             FrontEnd 8,9=$4000-$7FFF			 |
#	+--------------------------------------------------------+
#	|     NOT USED - ULA Screen Banks 10+11=$4000-$7FFF	 |
#	+--------------------------------------------------------+
#	|              Score 12,13=$4000-$7FFF			 |
#	+--------------------------------------------------------+
#	|                 Spare 14,15,16,17			 |
#	+--------------------------------------------------------+
#	|       Layer2 Banks 18,19,20,21,22,23			 | 
#	+--------------------------------------------------------+
#	|   Layer2 Back Buffer Banks 24,25,26,27,28,29		 | 
#	+--------------------------------------------------------+
#	|	     	   Pic Banks 30...		 	 |
#	+--------------------------------------------------------+
#
#
#	      +---------+---------------+-----------------------------------------------+
#	      | Bank8k	| Mem		| Section					|
#	      +---------+---------------+---------------+---------------+---------------+
#	      |  0x50	|    0x0000	|     ROM1	|    Free Use	|
#	      +---------+---------------+---------------+---------------+
#	      |  0x51	|    0x2000	|     ROM2 	|    Free Use 	|
#	      +---------+---------------+---------------+---------------+---------------+---------------+
#	      |  0x52	|    0x4000	|     ULA1?	|   Frontend1	|   Gameplay1 	|    Score1     |
#	      +---------+---------------+---------------+---------------+---------------+---------------+
#	      |  0x53	|    0x6000	|     ULA2?	|   Frontend2 	|   Gameplay2	|    Score2     |
#	      +---------+---------------+---------------+---------------+---------------+---------------+
#	      |  0x54	|    0x8000	|   MainCode1 	|
#	      +---------+---------------+---------------+
#	      |  0x55	|    0xA000	|   MainCode2	|
#	      +---------+---------------+---------------+
#	      |  0x56	|    0xC000	|    Kernel	|
#	      +---------+---------------+---------------+
#	      |  0x57	|    0xE000	|     IRQ	|
#	      +---------+---------------+---------------+
#
#
# 	Section		Compiles as:
#	Kernel		Bank 1
#	GamePlay	Bank 3
# 	FrontEnd	Bank 4
#	Score		Bank 14
#
#
# Notes
#   ZX Spectrum Next 1MB version has 95 bank8s and 47 bank16s
#
# *******************************************************************************************
SECTION PAGE_02_KERNEL_CODE
	org 0xE000
SECTION KERNEL_CODE
SECTION PAGE_02_KERNEL_IRQ
	org 0xFCFC
SECTION KERNEL_IRQ
SECTION PAGE_02_KERNEL_END
	org 0xFFFF
SECTION KERNEL_END

# 6+7
SECTION PAGE_06_GAMEPLAY_SEG
	org 0x4000
SECTION GAMEPLAY_SEG

# 8+9
SECTION PAGE_08_FRONTEND_SEG
	org 0x4000
SECTION FRONTEND_SEG

# 24
SECTION PAGE_12_SCORE_SEG
	org 0x4000
SECTION SCORE_SEG


# 30-33
SECTION PAGE_30_FOOD_SEG
	org 0x2000
SECTION FOOD_SEG

# 34-36
SECTION PAGE_34_SEED_SEG
	org 0x2000
SECTION SEED_SEG

# 38-41
SECTION PAGE_38_DYKE_SEG
	org 0x2000
SECTION DYKE_SEG

# 42-45
SECTION PAGE_42_FIELDS_SEG
	org 0x2000
SECTION FIELDS_SEG

# 46-49
SECTION PAGE_46_DEFENSE_SEG
	org 0x2000
SECTION DEFENSE_SEG

# 50-53
SECTION PAGE_50_NEWBORN_SEG
	org 0x2000
SECTION NEWBORN_SEG

# 54-57
SECTION PAGE_54_GRAINBUNDLE_SEG
	org 0x2000
SECTION GRAINBUNDLE_SEG

# 58-61
SECTION PAGE_58_KINGDOMSTATE_SEG
	org 0x2000
SECTION KINGDOMSTATE_SEG

# 62-65
SECTION PAGE_62_STARVED_SEG
	org 0x2000
SECTION STARVED_SEG

# 66-67
SECTION PAGE_66_SCROLL_SEG
	org 0x2000
SECTION SCROLL_SEG

# 68-71
SECTION PAGE_68_FLOOD_SEG
	org 0x2000
SECTION FLOOD_SEG

# 72-75
SECTION PAGE_72_RECLAIMED_SEG
	org 0x2000
SECTION RECLAIMED_SEG

# 76-79
SECTION PAGE_76_POPULATION_SEG
	org 0x2000
SECTION POPULATION_SEG

# 80-83
SECTION PAGE_80_LAND_SEG
	org 0x2000
SECTION LAND_SEG

# 84-87
SECTION PAGE_84_NATURALDEATH_SEG
	org 0x2000
SECTION NATURALDEATH_SEG

# 88-91
SECTION PAGE_88_BANDITDEATH_SEG
	org 0x2000
SECTION BANDITDEATH_SEG

# 92-
SECTION PAGE_92_TEST_SEG
	org 0x2000
SECTION TEST_SEG



