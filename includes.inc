; ******************************************************************************************************************************
;
;	Assembler Includes
;
; ******************************************************************************************************************************

	defc MMU_0 = 0x50	; mem addr $0000
	defc MMU_1 = 0x51	; mem addr $2000
	defc MMU_2 = 0x52	; mem addr $4000
	defc MMU_3 = 0x53	; mem addr $6000
	defc MMU_4 = 0x54	; mem addr $8000
	defc MMU_5 = 0x55	; mem addr $a000
	defc MMU_6 = 0x56	; mem addr $c000
	defc MMU_7 = 0x57	; mem addr $e000
	defc DISPLAY_L2 = 0x12
	defc CPU_SPEED = 0x07


	defc L2_Display1_Bank = $12
	defc L2_Display2_Bank = $18

	
	; ULA screen
        defc ULAScreen = 0x4000                                     ; ULA screen in z80 64k space
        defc AttrScreen = ((ULAScreen)+(32*192))                    ; Attribute screen  in z80 64k space
        defc ULAScreenPysical = (10*8192)                           ; ULA screen physical address
        defc AttrScreenPysical = ((ULAScreenPysical)+(32*192))      ; Attribute screen physical address
        

        ; ports
        defc Z80DMAPORT  		= 107				; DMA port
        defc SpriteRegPort		= 0x57
        defc SpriteShapePort 		= 0x5b

        ; File system
        defc M_GETSETDRV		= $89
        defc F_OPEN			= $9a
        defc F_CLOSE			= $9b
        defc F_SYNC			= $9c
        defc F_READ			= $9d
        defc F_WRITE			= $9e
        defc F_SEEK			= $9f
        defc F_FSTAT			= $a1                   ; get open file stats - 11 byte buffer needed
        defc FA_READ			= $01
        defc FA_APPEND			= $06
        defc FA_OVERWRITE		= $0c
	defc F_STAT			= $ac                   ; get unopened file stats - 11 byte buffer needed

	defc ESX_MODE_READ		= $01			; request read access
	defc ESX_MODE_WRITE		= $02			; request write access
	defc ESX_MODE_USE_HEADER	= $40			; read/write +3DOS header
	defc ESX_MODE_OPEN_EXIST	= $00			; only open existing file
	defc ESX_MODE_OPEN_CREATE	= $08			; open existing or create file
	defc ESX_MODE_CREATE_NOEXIST	= $04			; create new file, error if exists
	defc ESX_MODE_CREATE_TRUNC	= $0c			; create new file, delete existing

        defc MAX_FILENAME_LEN           = 64                    ; includes ,0 at the end
        defc LOADING_BANK               = $56                   ; the MMU register we're using
        defc LOADING_BASE_ADD           = $c000                 ; base of bank we'll be loading into

	; esxDOS-compatible error codes
	;------------------------------------------------------
	; Unknown error			; 0, esx_ok
	; OK				; 1, esx_eok
	; Nonsense in esxDOS		; 2, esx_nonsense
	; Statement end error		; 3, esx_estend
	; Wrong file type		; 4, esx_ewrtype
	; No such file or dir		; 5, esx_enoent
	; I/O error			; 6, esx_eio
	; Invalid filename		; 7, esx_einval
	; Access denied			; 8, esx_eacces
	; Drive full			; 9, esx_enospc
	; Invalid i/o request		; 10, esx_enxio
	; No such drive			; 11, esx_enodrv
	; Too many files open		; 12, esx_enfile
	; Bad file number		; 13, esx_ebadf
	; No such device		; 14, esx_enodev
	; File pointer overflow		; 15, esx_eoverflow
	; Is a directory		; 16, esx_eisdir
	; Not a directory		; 17, esx_enotdir
	; Already exists		; 18, esx_eexist
	; Invalid path			; 19, esx_epath
	; Missing system		; 20, esx_esys
	; Path too long			; 21, esx_enametoolong
	; No such command		; 22, esx_enocmd
	; In use			; 23, esx_einuse
	; Read only			; 24, esx_erdonly
	; Verify failed			; 25, esx_everify
	; Sys file load error		; 26, esx_eloadingko
	; Directory in use		; 27, esx_edirinuse
	; MAPRAM is active		; 28, esx_emapramactive
	; Drive busy			; 29, esx_edrivebusy
	; Unknown filesystem		; 30, esx_efsunknown
	; Device busy			; 31, esx_edevicebusy


	; half row 1
        defc VK_CAPS	=	0
        defc VK_Z	=	1
        defc VK_X	=	2
        defc VK_C	=	3
        defc VK_V	=	4
	; half row 2
        defc VK_A	=	5
        defc VK_S	=	6
        defc VK_D	=	7
        defc VK_F	=	8
        defc VK_G	=	9
	; half row 3
        defc VK_Q	=	10
        defc VK_W	=	11
        defc VK_E	=	12
        defc VK_R	=	13
        defc VK_T	=	14
	; half row 4
        defc VK_1	=	15
        defc VK_2	=	16
        defc VK_3	=	17
        defc VK_4	=	18
        defc VK_5	=	19

	; half row 5
        defc VK_0	=	20
        defc VK_9	=	21
        defc VK_8	=	22
        defc VK_7	=	23
        defc VK_6	=	24
	; half row 6
        defc VK_P	=	25
        defc VK_O	=	26
        defc VK_I	=	27
        defc VK_U	=	28
        defc VK_Y	=	29

	; half row 7
        defc VK_ENTER	=	30
        defc VK_L	=	31
        defc VK_K	=	32
        defc VK_J	=	33
        defc VK_H	=	34
	; half row 8
        defc VK_SPACE	=	35
        defc VK_SYM	=	36
        defc VK_M	=	37
        defc VK_N	=	38
        defc VK_B	=	39


