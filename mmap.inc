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
#	|       Layer2 Banks 12,13,14,15,16,17=$0000		 |
#	+--------------------------------------------------------+
#	|   Layer2 Back Buffer Banks 18,19,20,21,22,23=$0000	 |
#	+--------------------------------------------------------+
#	|	     Game Data Banks 24 = $2000-$3fff	 	 |
#	+--------------------------------------------------------+
#	|	     L2_Display1_Bank = $12 = 18	 	 |
#	|	     L2_Display2_Bank = $18 = 24	 	 |
#	+--------------------------------------------------------+
#	|	     	   Pics Banks 30		 	 |
#	+--------------------------------------------------------+
#
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

# 24
SECTION PAGE_01_GAMEDATA_SEG
	org 0x2000
SECTION GAMEDATA_SEG

# 6+7
SECTION PAGE_06_GAMEPLAY_SEG
	org 0x4000
SECTION GAMEPLAY_SEG

# 8+9
SECTION PAGE_08_FRONTEND_SEG
	org 0x4000
SECTION FRONTEND_SEG


# 30
SECTION PAGE_30_FOOD_SEG
	org 0x2000
SECTION FOOD_SEG

# 34
SECTION PAGE_34_SEED_SEG
	org 0x2000
SECTION SEED_SEG

# 38
SECTION PAGE_38_DYKE_SEG
	org 0x2000
SECTION DYKE_SEG

# 42
SECTION PAGE_42_FIELDS_SEG
	org 0x2000
SECTION FIELDS_SEG

# 46
SECTION PAGE_46_DEFENSE_SEG
	org 0x2000
SECTION DEFENSE_SEG

# 50
SECTION PAGE_50_NEWBORN_SEG
	org 0x2000
SECTION NEWBORN_SEG

# 54
SECTION PAGE_54_GRAINBUNDLE_SEG
	org 0x2000
SECTION GRAINBUNDLE_SEG

# 58
SECTION PAGE_58_KINGDOMSTATE_SEG
	org 0x2000
SECTION KINGDOMSTATE_SEG

# 62
SECTION PAGE_62_STARVED_SEG
	org 0x2000
SECTION STARVED_SEG

# 66
SECTION PAGE_66_SCROLL_SEG
	org 0x2000
SECTION SCROLL_SEG

# 68
SECTION PAGE_68_FLOOD_SEG
	org 0x2000
SECTION FLOOD_SEG

# 72
SECTION PAGE_72_RECLAIMED_SEG
	org 0x2000
SECTION RECLAIMED_SEG

# 76
SECTION PAGE_76_POPULATION_SEG
	org 0x2000
SECTION POPULATION_SEG

# 80
SECTION PAGE_80_LAND_SEG
	org 0x2000
SECTION LAND_SEG

# 84
SECTION PAGE_84_NATURALDEATH_SEG
	org 0x2000
SECTION NATURALDEATH_SEG

# 88
SECTION PAGE_88_BANDITDEATH_SEG
	org 0x2000
SECTION BANDITDEATH_SEG



