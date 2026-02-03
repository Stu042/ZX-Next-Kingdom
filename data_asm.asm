	SECTION KERNEL_IRQ

; 	PUBLIC _StartOfGameData
; 	PUBLIC _GameName, _Version, _GameState, _Year, _Grains, _Population, _LandSize, _DykeStateFrac
; 	PUBLIC _BanditCount, _BanditHealthFrac, _AttackChance, _GrainAte, _GrainPlanted, _UsedGrain
; 	PUBLIC _PopInFields, _PopOnWall, _PopDefending, _UsedPop, _PopIncrease, _Births, _DeathsTotal
; 	PUBLIC _DeathsNatural, _DeathsStarvation, _DeathsDefending, _GrainStolen, _GrainGrown, _GrainFlooded
; 	PUBLIC _GrainIncrease, _DykeNaturalDamage, _DykeRepair, _DykeStateFracIncrease, _LandFlooded
; 	PUBLIC _LandReclaimed, _LandIncrease, _BanditsKilled, _TotalPopDied, _TotalPopKilled, _TotalPopStarved
; 	PUBLIC _TotalPopDiedOldAge, _TotalPopBorn, _TotalBanditsKilled, _TotalGrainAte, _TotalGrainPlanted
; 	PUBLIC _TotalGrainStolen, _TotalGrainGrown, _TotalGrainFlooded, _TotalLandFlooded, _TotalLandReclaimed
; 	PUBLIC _GameSaveSize



; _StartOfGameData:

; _GameName:	db "Kingdom", 0
; _Version:	db "0.1.001", 0

; _GameState:	db 0,0		; eGameState


; _Year:			db 0,0		; int16
; _Grains:		db 0,0,0,0	; int32
; _Population:		db 0,0,0,0	; int32
; _LandSize:		db 0,0,0,0	; int32
; _DykeStateFrac:		db 0,0,0,0	; int32
; _BanditCount:		db 0,0,0,0	; int32
; _BanditHealthFrac:	db 0,0,0,0	; int32
; _AttackChance:		db 0,0,0,0	; int32


; // grain

; _GrainAte:		db 0,0,0,0	; int32
; _GrainPlanted:		db 0,0,0,0	; int32
; _UsedGrain:		db 0,0,0,0	; int32


; // pop

; _PopInFields:		db 0,0,0,0	; int32
; _PopOnWall:		db 0,0,0,0	; int32
; _PopDefending:		db 0,0,0,0	; int32

; _UsedPop:		db 0,0,0,0	; int32


; // game sim year

; _PopIncrease:		db 0,0,0,0	; int32
; _Births:		db 0,0,0,0	; int32
; _DeathsTotal:		db 0,0,0,0	; int32
; _DeathsNatural:		db 0,0,0,0	; int32
; _DeathsStarvation:	db 0,0,0,0	; int32
; _DeathsDefending:	db 0,0,0,0	; int32

; _GrainStolen:		db 0,0,0,0	; int32
; _GrainGrown:		db 0,0,0,0	; int32
; _GrainFlooded:		db 0,0,0,0	; int32
; _GrainIncrease:		db 0,0,0,0	; int32

; _DykeNaturalDamage:	db 0,0,0,0	; int32
; _DykeRepair:		db 0,0,0,0	; int32
; _DykeStateFracIncrease:	db 0,0,0,0	; int32

; _LandFlooded:		db 0,0,0,0	; int32
; _LandReclaimed:		db 0,0,0,0	; int32
; _LandIncrease:		db 0,0,0,0	; int32

; _BanditsKilled:		db 0,0,0,0	; int32

; _TotalPopDied:		db 0,0,0,0	; int32
; _TotalPopKilled:	db 0,0,0,0	; int32
; _TotalPopStarved:	db 0,0,0,0	; int32
; _TotalPopDiedOldAge:	db 0,0,0,0	; int32

; _TotalPopBorn:		db 0,0,0,0	; int32

; _TotalBanditsKilled:	db 0,0,0,0	; int32

; _TotalGrainAte:		db 0,0,0,0	; int32
; _TotalGrainPlanted:	db 0,0,0,0	; int32
; _TotalGrainStolen:	db 0,0,0,0	; int32
; _TotalGrainGrown:	db 0,0,0,0	; int32
; _TotalGrainFlooded:	db 0,0,0,0	; int32

; _TotalLandFlooded:	db 0,0,0,0	; int32
; _TotalLandReclaimed:	db 0,0,0,0	; int32

; _unused:		ds 128		; uint8[128];


; endOfData:
; _GameSaveSize:	equ endOfData - _GameName
