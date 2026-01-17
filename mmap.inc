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
