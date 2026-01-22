
			SECTION		KERNEL_CODE
			EXTERN		_VBlank, _Port123b, _SpriteData, _SpriteShape, _PrintOffset

			include		"includes.inc"



; ******************************************************************************************************************************
;   Init the Kernel
; ******************************************************************************************************************************
PUBLIC _InitKernel, InitKernel
InitKernel:
_InitKernel:
			LD		hl,0x3d00
			LD		de,_RomFont
			LD		bc,0x300
			CALL		DMACopy
			RET

; ******************************************************************************************************************************
;   Wipe all sprites
; ******************************************************************************************************************************
PUBLIC _WipeSprites, WipeSprites
WipeSprites:
_WipeSprites:
			ld		ix, _SpriteData
			ld		b,128
			ld		c,64
			ld		de,5
			xor		a
@InitSprites: 
			ld		(ix+0),a
			ld		(ix+1),a
			ld		(ix+2),a
			ld		(ix+3),c															; %01000000 - enable byte 4
			ld		(ix+0),a
			add		ix,de
			djnz		@InitSprites
			ret

; ******************************************************************************************************************************
;   Reset all sprite data, including the extra bit for using 5th byte
; ******************************************************************************************************************************
PUBLIC _InitSpriteData, InitSpriteData
InitSpriteData:
_InitSpriteData:
			ld		ix, _SpriteData
			ld		b,128
			ld		c,64
			ld		de,5
			xor		a   
@InitSprites: 
			ld		(ix+0),a
			ld		(ix+1),a
			ld		(ix+2),a
			ld		(ix+3),c															; %01000000 - enable byte 4
			ld		(ix+0),a
			add		ix,de
			djnz		@InitSprites
			ret


; ******************************************************************************************************************************
;   Wait for a Vertical Blank (uses VBlank IRQ)
; ******************************************************************************************************************************
PUBLIC _WaitVBlank, WaitVBlank
WaitVBlank:
_WaitVBlank:
			xor		a
			ld		(_VBlank),a

WaitForVBlank:
			ld		a,(_VBlank)
			and		a
			jr		z,WaitForVBlank
			ret

; ************************************************************************
;   Enable the 256 colour Layer 2 bitmap
; ************************************************************************
PUBLIC _InitL2, InitL2
InitL2:
_InitL2:
			LD	BC,$123B		; L2 Access Port
    			LD	A,%00000010		; Bit 1: Enable
    			OUT	(C),A		
			LD 	L,L2_Display1_Bank	; The initial visible screen (8K bank)
			LD 	H,L2_Display2_Bank	; The initial offscreen buffer (8K bank)
			LD 	(screen_banks),HL	; Initialise the paging register
			LD	A,L
			SRL	A 			; Divide by 2 for 16K bank multiplier
			NEXTREG DISPLAY_L2, A		; Display the visible screen 
    			RET                          



; ************************************************************************
; void swapL2(void)
; Swap the screen banks round and page the visible one in
; ************************************************************************
PUBLIC _SwapL2, SwapL2
SwapL2:
_SwapL2:
			LD	HL,(screen_banks)	; Swap the screen banks
			LD	A,H			; Get the current offscreen buffer 
			LD	H,L			; H: New offscreen buffer
			LD	L,A			; L: New visible buffer
			LD	(screen_banks), HL
			SRL 	A 			; Divide by 2 for 16K bank multiplier
			NEXTREG DISPLAY_L2, A		; Set the current visible buffer
			RET


; ************************************************************************
;
;   Function:   Clear the spectrum attribute screen
;   In:     L = colour
;
;   Format: F_B_PPP_III
;
;           F = Flash
;           B = Bright
;           P = Paper
;           I = Ink
;
; ************************************************************************
PUBLIC _Border, Border
_Border:
			ld		a,l

; ************************************************************************
;
;   Function:   Clear the spectrum attribute screen
;   In:     A = colour
;
;   Format: F_B_PPP_III
;
;           F = Flash
;           B = Bright
;           P = Paper
;           I = Ink
;
; ************************************************************************
Border:
			out		($fe),a
			ret



; ************************************************************************
;   Function:   Return current page in MMU_0
;   uses: bc
;   in a = mmu register to check
;   out a = page
; ************************************************************************
PUBLIC _CurrentMMUPage, CurrentMMUPage
_CurrentMMUPage:
CurrentMMUPage:
			LD      BC,$243B    		; Select NEXT register
			OUT     (C),A			; Read current bank register
			INC     B           		; $253b to access (read or write) value
			IN      A,(C)			; Get the current bank number
			ret


; ************************************************************************
;   Function:   clear the Layer2 (256x192) screen with a colour
;
; ************************************************************************
PUBLIC _ClsL2
_ClsL2:			LD 	E, L			; Get the colour from HL
;===========================================================================
; Function:   clear the Layer2 (256x192) screen with a colour
; E = colour
;===========================================================================
ClsL2:
			LD	A,(screen_banks+1)	; Get the offscreen screen bank
			LD	D,A 			; D: Offscreen bank
			LD 	B,6			; Number of banks to clear (6 * 8K = 48K for 256x192 L2)

@loop:
 			PUSH	BC
			LD	A,D			; D: The bank to clear
			NEXTREG	MMU_0,a			; Bank it in
			LD	HL,0
			LD 	BC,0x2000
			LD	A,E			; Get the colour 
			CALL	DMAFill			; Clear the screen
			POP	BC
			INC	D 			; Increment the bank number
			DJNZ	@loop 
			RET 
screen_banks:		DB 0				; LSB: The visible screen
			DB 0


;===========================================================================
; ClsLast2()
;
;===========================================================================
	PUBLIC _ClsLast2, ClsLast2
ClsLast2:
			LD 	E, L			; Get the colour from HL
_ClsLast2:
			LD	A,(screen_banks+1)	; Get the offscreen screen bank
			add	a,4			; get first of last 2 banks
			ld	d,a
			LD 	B,2
			jr	ClsL2@loop

;===========================================================================
; ClsFirst4()
;
;===========================================================================
	PUBLIC _ClsFirst4, ClsFirst4
ClsFirst4:
			LD 	E, L			; Get the colour from HL
_ClsFirst4:
			LD	A,(screen_banks+1)	; Get the offscreen screen bank
			ld	d,a
			LD 	B,4
			jr	ClsL2@loop


; ******************************************************************************
; Function: DMACopper
; In:       hl = Copper offset into bank
;           bc = size
; Uses:     A,HL,BC
; ******************************************************************************
PUBLIC _UploadCopper, UploadCopper
_UploadCopper:
UploadCopper:
			pop		hl																; get return address
			pop		bc																; get src
			ld		(DMASrcAdd),bc
			pop		bc																; get size
			ld		(DMASrcLen),bc
			push		hl																; push return address back

			xor		a																; 
			NextReg		$61,a																; reset copper address
			NextReg		$62,a																; Stop copper - copper address upper bits

        ; now select the copper "data" port
			ld		bc,$243b
			ld		a,$60
			out		(c),a

			ld		hl,DMACopyToReg															; 10
			ld		bc,DMACOPPERSIZE*256 + Z80DMAPORT												; 10
			otir																		; 21*20  + 240*4

			ret

; ******************************************************************************
; Function: DMACopy
; In:       hl = Src
;           de = Dest
;           bc = size
; ******************************************************************************
PUBLIC _DMACopy, DMACopy
_DMACopy:
			pop		hl																; get return address
			pop		bc																; get src
			ld		(DMASrc),bc
			pop		bc																; get dest
			ld		(DMADest),bc
			pop		bc																; get size
			ld		(DMALen),bc
			push		hl																; push return address back

DoDMACopy:
			ld		hl,DMACopyProg                  
			ld		bc,DMASIZE*256 + Z80DMAPORT 
			otir
			ret


; ******************************************************************************
; Function: DMACopy
; In:       hl = Src
;           de = Dest
;           bc = size
; ******************************************************************************
DMACopy:
			ld		(DMASrc),hl
			ld		(DMADest),de
			ld		(DMALen),bc
			jp		DoDMACopy


; ******************************************************************************************************************************
;   Function: void DMAFill(uint16 dst, uint16 len, uint8 val)
;   Do a DMA fill
;
;   In:        a = val
;	      hl = dst
;             bc = len
; ******************************************************************************************************************************
PUBLIC _DMAFill, DMAFill
_DMAFill:		POP	IY 
			POP	HL	; Get dst
			POP	BC	; Get len 
			POP	DE
			LD	A,E	; Get val
			PUSH	IY

; ******************************************************************************************************************************
;   Function: void DMAFill(uint16 dst, uint16 len, uint8 val)
;   Do a DMA fill
;
;   In:        a = val
;	      hl = dst
;             bc = len
; ******************************************************************************************************************************
DMAFill:		LD	(DMAFillDst),HL
    			LD	(DMAFillLen),BC
			LD	(DMAFillVal),A

DMAFillInternal:	LD	HL,DMAFillProg                  
    			LD	BC,DMAFillProgL * 256 + Z80DMAPORT 
    			OTIR
			RET

; ******************************************************************************
; Function: Upload a set of sprites
; In:   E = sprite shape to start at
;       D = number of sprites
;       HL = shape data
;
; ******************************************************************************
PUBLIC _UploadSprites, UploadSprites
_UploadSprites:
			pop		bc	; pop return address
			pop		de	; get Start Shape
			pop		hl	; get number of shapes
			ld		d,l
			pop		hl	; get shape address
			push		bc	; restore return address

    ; Upload sprite graphics
			ld		a,e	; get start shape

; ******************************************************************************
; Function: Upload a set of sprites
; In:   A = sprite shape to start at
;       D = number of sprites
;       HL = shape data
;
; ******************************************************************************
UploadSprites:
			ld		e,0	; each pattern is 256 bytes

@AllSprites:               
    ; select pattern 2
			ld		bc, $303B
			out		(c),a

    ; upload ALL sprite sprite image data
			ld		bc, SpriteShapePort
@UpLoadSprite:           
			outi

			dec		de
			ld		a,d
			or		e
			jr		nz, @UpLoadSprite
			ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void BlitLargeImageAt(uint8 screenY, uint8 imageBank, uint8 bankCount)
; Render an image that spans across multiple banks, screenY must be on the start of a bank boundary.
; Image is full screen width (256 pixels)
	PUBLIC _BlitLargeImageAt, BlitLargeImageAt
_BlitLargeImageAt:
			pop	de		; return addr
			dec	sp
			pop	bc		; B = bank count, C = start bank
			pop	hl
			push	de		; restore return addr
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Render an image that spans across multiple banks
; Image is full screen width (256 pixels)
; B: Y screen pos
; image Bank		L	-> starts with image data only
; Bank count		H
BlitLargeImageAt:
			push	hl		; H = image bank count, L = start image bank
			ld	c,0
			call	GetPixelAddressMemBank
			pop	bc
@nextBank:
			push	af		; save display bank number
			push	bc		; save B = image bank count, C = image bank
			NEXTREG	MMU_0,A		; Grab display bank
			ld	a,c
			NEXTREG	MMU_1,A		; Grab image bank
			ld	bc,$2000	; copy $2000 to $000
			ld	hl,$2000
			ld	de,0
			call	DMACopy		; use dma for copy
			pop	bc
			pop	af
			inc	a
			inc	c
			djnz	@nextBank
			ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void BlitLargeImage(uint8 imageBank, uint8 bankCount)
; Render an image that spans across multiple banks
; Image is full screen width (256 pixels)
	PUBLIC _BlitLargeImage, BlitLargeImage
_BlitLargeImage:
			pop	hl		; return addr
			pop	bc		; B = bank count, C = start bank
			push	hl		; restore return addr
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Render an image that spans across multiple banks
; Image is full screen width (256 pixels)
; image Bank		C	-> starts with image data only
; Bank count		B
BlitLargeImage:
			push	bc		; B = image bank count, C = start image bank
			ld	bc,0
			call	GetPixelAddressMemBank
			pop	bc
@nextBank:
			push	af		; save display bank number
			push	bc		; save B = image bank count, C = image bank
			NEXTREG	MMU_0,A		; Grab display bank
			ld	a,c
			NEXTREG	MMU_1,A		; Grab image bank
			ld	bc,$2000	; copy $2000 to $000
			ld	hl,$2000
			ld	de,0
			call	DMACopy		; use dma for copy
			pop	bc
			pop	af
			inc	a
			inc	c
			djnz	@nextBank
			ret

;; GetPixelAddressMemBankOnscreen
	PUBLIC _DoubleBlitLargeImage, DoubleBlitLargeImage
_DoubleBlitLargeImage:
			pop	hl		; return addr
			pop	bc		; B = bank count, C = start bank
			push	hl		; restore return addr
DoubleBlitLargeImage:
			push	bc		; B = image bank count, C = start image bank
			ld	bc,0
			call	GetPixelAddressMemBank
			pop	bc
			push	bc
			call	BlitLargeImage@nextBank
			ld	bc,0
			call	GetPixelAddressMemBankOnscreen
			pop	bc
			jr	BlitLargeImage@nextBank


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BlitTransImage(uint8 x, uint8 y, uint16 transCol, uint8* imageSrc)
; Render an image that has a transparent pixel chosen by transCol (A)
; trans index		A
; image data addr	DE	-> started with width, height then image data
; y, x pos		BC
	PUBLIC _BlitTransImage, BlitTransImage
_BlitTransImage:
			pop	hl			; get return addr
			pop	bc			; yx pos
			pop	de			; image src
			ld	a,e
			pop	de			; transparent pixel index
			push	hl			; save return addr
BlitTransImage:
			push	iy
			ld	iy,ImageData
			ld	(iy+0),b		; save pos y
			ld	(iy+1),c		; save pos x
			ld	(iy+4),a		; save trans pix index
			ld	a,(de) : inc de
			ld	(iy+3),a		; save width
			ld	a,(de) : inc de
			ld	(iy+2),a		; save height
			ld	c,0			; get image height
			ld	b,a
@loop:	; bc= image height, de = image src
			push	bc			; save remaining height
			ld	b,(iy+0)		; current y pos
			ld	c,(iy+1)		; current x pos
			call	GetPixelAddress		; IN BC = YX pos, out HL = banked screen address
			ex	de,hl			; de now equals screen addr, hl current image source addr
			ld	c,(iy+3)		; get image width
			ld	b,0
			ld	a,(iy+4)
			ldirx				; if (HL) != A: (HL++)=(DE++), BC-1
			ex	de,hl			; hl now equals broken screen addr, de correct current image source addr
			ld	a,(iy+0)		; y pos
			inc	a			; next pos
			ld	(iy+0),a		; save next y pos
			
			pop	bc
			djnz	@loop
			pop	iy
			ret
ImageData:
@YPos:		db 0	; y pos     0
@XPos:		db 0	; x pos     1
@YSize:		db 0	; y Size    2
@XSize:		db 0	; x Size    3
@PicCol:	db 0	; trans pix 4


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void _Render1Bpp(uint8 x, uint8 y, uint16 col, uint8* oneBpp) {
;	uint8 xpos = x + *oneBpp++;
;	uint8 ypos = y + *oneBpp++;
;	Render1Bpp(xpos, ypos, col, oneBpp);
;}
;	sp -> ret addr, yx pos, col, img src
;
	PUBLIC _Render1Bpp, Render1Bpp
_Render1Bpp:	; 85 90 01 02 03 34 12 97 81 FF
			pop	hl			; get return addr
			pop	bc			; yx pos
			pop	de			; image src
			ld	a,e
			pop	de			; pixel colour index
			push	hl			; save return addr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BlitTransImage(uint8 x, uint8 y, uint16 col, uint8* imageSrc)
; Note a 1bpp image has its width in bytes (8 pixels)
; Colour index		a
; Image data addr	de	-> started with width, height then image data
; Y, X pos		bc
Render1Bpp:
			ld	l,a
			ld	a,(de) : inc de : add a,c : ld c,a
			ld	a,(de) : inc de : add a,b : ld b,a
			ld	a,l
			jr	Blit1BppImage


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; BlitTransImage(uint8 x, uint8 y, uint16 col, uint8* imageSrc)
; Note a 1bpp image has its width in bytes (8 pixels)
; Colour index		a
; Image data addr	de	-> started with width, height then image data
; Y, X pos		bc
	PUBLIC _Blit1BppImage, Blit1BppImage
_Blit1BppImage:
			pop	hl			; get return addr
			pop	bc			; yx pos
			pop	de			; image src
			ld	a,e
			pop	de			; pixel colour index
			push	hl			; save return addr

Blit1BppImage:
			push	iy
			ld	iy,ImageData
			ld	(iy+0),b		; save pos y
			ld	(iy+1),c		; save pos x
			ld	(iy+4),a		; save pixel colour index
			ld	a,(de) : inc de
			ld	(iy+3),a		; save width
			ld	a,(de) : inc de
			ld	(iy+2),a		; save height
			ld	c,0			; get image height
			ld	b,a
@loop:	; bc= image height, de = image src
			push	bc			; save remaining height
			ld	b,(iy+0)		; current y pos
			ld	c,(iy+1)		; current x pos
			call	GetPixelAddress		; IN BC = YX pos, out HL = banked screen address
			ex	de,hl			; de now equals screen addr, hl current image source addr
			ld	b,(iy+3)		; get image width in bytes

@rowLoop:	; render a row,  DE = L2 destination, HL = image src, B = byte count
			push	bc
			ld	a,(hl) : inc hl		; get byte from src image
			ld	c,a			; save in C
			ld	a,(iy+4)		; A = colour to render
			ld	b,8			; number of pixels in a byte

@byteLoop:
			sla	c
			jr	nc,@noPix
			ld	(de),a
@noPix:
			inc	de
			djnz	@byteLoop

			pop	bc
			djnz	@rowLoop

			ex	de,hl			; hl now equals broken screen addr, de correct current image source addr
			ld	a,(iy+0)		; y pos
			inc	a			; next pos
			ld	(iy+0),a		; save next y pos
			
			pop	bc
			djnz	@loop
			pop	iy
			ret


PUBLIC _GetPixelAddress, GetPixelAddress
	_GetPixelAddress = GetPixelAddress
; ******************************************************************************
;   GetPixelAddress(X, Y)
; Get pixel position
; Pages in the correct 8K Layer 2 screen bank into 0x0000
;   B: Y coordinate
;   C: X coordinate
; Returns:
;  HL: Address in memory (between 0x0000 and 0x1FFF)
;   A is destroyed, BC remain the same, Result in HL
; ******************************************************************************
GetPixelAddress:
			ld	l,c
			LD 	A,(screen_banks+1)
			LD 	H,A			; H: Offscreen screen bank
			LD	A,B			; 0-31 per bank (8k)
			AND	%11100000		; 3 bits for the 8 banks we can use
			SWAPNIB
			RRCA
			ADD	A, H			; Add the bank in
			NEXTREG	MMU_0,A			; Grab that bank
			LD	A,B
			AND	%00011111
			LD	H,A
			RET

; ******************************************************************************
;   GetPixelAddressMemBank(Y)
; Get pixel position
; Pages in the correct 8K Layer 2 screen bank into 0x0000
;   B: Y coordinate
; Returns:
;  A = bank number, use NEXTREG	MMU_0,A to page it in
;  H is destroyed
; ******************************************************************************
GetPixelAddressMemBank:
			LD 	A,(screen_banks+1)
GetPixelAddressMemBankAeqBankbase:
			LD 	H,A			; H: Offscreen screen bank
			LD	A,B			; 0-31 per bank (8k)
			AND	%11100000		; 3 bits for the 8 banks we can use
			SWAPNIB
			RRCA
			ADD	A, H			; Add the bank in
			ret	
GetPixelAddressMemBankOnscreen:
			LD 	A,(screen_banks)
			jr	GetPixelAddressMemBankAeqBankbase


; ******************************************************************************
; Function: Print string to L2 display.
; A: colour, 
; BC: YX screen position
; HL: Text
; ******************************************************************************
	PUBLIC _PrintL2, PrintL2
_PrintL2:
			pop	de	; return addr
			pop	bc	; YX pos
			dec	sp
			pop	af
			pop	hl
			push	de

PrintL2:
			ld	(PrintCol),a

@nextChar:
			ld	a,192
			cp	b
			ret	z				; return if we are off bottom of display
			ld	a,(hl)				; get next character
			inc	hl
			cp	10				; is it 10 (LF)
			jr	z,@newLine
			cp	13				; is it 13, (RET)
			jr	z,@nextChar
			and	a
			ret	z				; return on null terminated str

			sub	32				; subtract space
			jr	c,@renderChar			; if overflow then an ascii char and render?
			jr	z,@moveRight			; if equal to space then next char

@renderChar:
			push	hl
			push	bc
			call	PrintCharL2
			pop	bc
			pop	hl

@moveRight:
			ld	a,8				; width of a char
			add	a,c				; inc x coord by width of a char

			ld	c,a
			jr	@nextChar			; jump to getting next char to render

@newLine:		; inc y to new line and x to 0 - possible todo, setup a frame for the text (top left, bottom right)
			ld	a,8
			add	a,b
			ld	b,a
			ld	c,0
			jr	@nextChar


; ******************************************************************************
; Function: Print 8x8 character.
; (PrintCol): Pixel colour
; A = Printable character - 32
; BC = YX screen pos
; On Exit: C unchanged
; Destroyed: A, B, B', DE, HL
; ******************************************************************************
PrintCharL2:
			ld	d,0				; calc offset into rom font
			sla	a				; char * 8
			rl	d
			sla	a
			rl	d
			sla	a
			rl	d
			ld	e,a				; rom font aligned to 256 bytes so just stick in offset
			add	de,_RomFont			; add on base

			exx
			ld	b,8

@loop:
			exx
			call	GetPixelAddress
			push	bc				; save yx pos
			ld	a,(de)				; A = next row of bits to print
			inc	de
			ld	c,a				; C = bits to render
			ld	a,(PrintCol)			; A = colour
			call	PrintByte
			; ld	a,b				; if not zero then at end of line
			pop	bc				; get YX back
			inc	b				; next Y pos
			ld	a,192
			cp	b
			ret	z				; return if we are off bottom of display
			exx
			djnz	@loop				; continue if more
			ret


; ******************************************************************************
; Function: Print 8 pixels in a row. Stop if going off right of display.
; C = bits to render
; A = colour
; HL = valid screen address
; On Exit: A = colour, C = 0, HL = right of last pixel rendered
; Destroyed: B (not zero if at end of line)
; ******************************************************************************
PrintByte:
			ld	b,8
@loop:			sla	c
			jr	nc,@bypassThisPixel
			ld	(hl),a
@bypassThisPixel:	inc	l
			ret	z
			djnz	@loop
			ret
PrintCol:	db 0





; ******************************************************************************
; Seeds XorShift with a random value, taken from R
; ******************************************************************************
	PUBLIC _XorShiftRndSeed, XorShiftRndSeed
_XorShiftRndSeed:
XorShiftRndSeed:
			ld	a,r
			ld	l,a
			ld	a,r
			ld	h,a
			add	hl,$7237
			ld	(RndSeed),hl
			ret

RndSeed:	dw 1


; ******************************************************************************
; uint16 XorShift()
;
; uint16 XorShift(){
; 	xs ^= xs << 7;
; 	xs ^= xs >> 9;
; 	xs ^= xs << 8;
; 	return xs;
; }
	PUBLIC _XorShift, XorShift
_XorShift:
; ******************************************************************************
; XorShift
; Return a random number in HL.
XorShift:
			ld	hl,(RndSeed)
			ld	a,h		; test if seed is 0
			or	l
			jr	nz, @okay
			call	XorShiftRndSeed	; if zero 
@okay:
			sra	a		; get bit 0 of H into cf
			ld	a,l		; byte we are working on
			rr	a		; right once and pull cf into this
			xor	h
			ld	h,a
			srl	a
			xor	l
			ld	l,a
			xor	h
			ld	h,a
			ld	(RndSeed),hl
			dec	hl
			ret



; ******************************************************************************
; uint8 PropPixelLength(char *text)
; Return the pixel length of a string
; ******************************************************************************
	PUBLIC _PropPixelLength, PropPixelLength
_PropPixelLength:
			ex	de,hl
; ******************************************************************************
; void PropPixelLength(char* text)
; Function: Get pixel length of a string if it was printed using a proportional font.
; HL = ptr to string, null terminated
; Destroyed: A, BC, DE, HL
; ******************************************************************************
PropPixelLength:
			ld	l,0
@nextChar:
			ld	a,(de)			; A = ascii character
			and	a
			ret	z			; ret if null
			inc	de
			sub	a,' '
			jr	nc,@printable
			xor	a
@printable:
			ld	c,a			; E = char
			ld	b,0			; mult char by 8 to get index into char data
			sla	c
			sla	c
			rl	b
			sla	c
			rl	b
			add	bc,PropFontCharData	; DE = char width then char row data
			ld	a,(bc)			; A = char width
			add	a,l			; calc next x pos
			ld	l,a
			jr	@nextChar


; ******************************************************************************
; void PrintProp(uint8 x, uint8 y, uint8 col, char *text)
; ******************************************************************************
	PUBLIC _PrintProp, PrintProp
_PrintProp:
			pop	de	; return addr
			pop	bc	; YX
			dec	sp
			pop	af	; col
			pop	hl	; text
			push 	de	; return addr
; ******************************************************************************
; Function: Print a null terminated string using a proportional sized font.
; A = colour
; BC = yx position on screen
; HL = ptr to string, null terminated
; ******************************************************************************
PrintProp:
			ld	(PrintCol),a
@NextChar:
			ld	(CharPosition),bc
			ld	a,(hl)			; A = ascii character
			and	a
			ret	z			; ret if null
			inc	hl
			push	hl			; save next char ptr
			sub	a,' '
			jr	nc,@printable
			xor	a
@printable:
			ld	e,a			; E = char
			ld	d,0			; mult char by 8 to get index into char data
			sla	e
			sla	e
			rl	d
			sla	e
			rl	d
			add	de,PropFontCharData	; DE = char width then char row data
			ld	a,(de)			; A = char width
			add	a,c			; calc next x pos
			ld	c,a
			push	bc			; save next yx position
			ld	a,(de)			; A = char width
			ld	b,a			; B = char width
			inc	de			; DE = first char row
			ld	c,7			; C = 7 rows in total
@nextRow:
			push	bc			; C = row count B = char width
			ld	bc,(CharPosition)
			inc	b
			ld	(CharPosition),bc
			call	GetPixelAddress		; HL = screen address
			pop	bc			; C = row count B = char width
			push	bc
			ld	a,(PrintCol)
			ld	c,a			; text pixel colour
			ld	a,(de)			; first row to print
@row:
			sla	a
			jr	nc, @nothingToPrint
			ld	(hl),c			; draw colour if a pixel reqd
@nothingToPrint:
			inc	l
			djnz	@row			; complete row
			inc	de			; next char data row
			pop	bc			; B = row count C = char width
			dec	c
			jr	nz,@nextRow
			pop	bc			; next yx position
			pop	hl			; next char ptr
			jr	@NextChar


CharPosition:		dw 0



PropFontCharData:
Space:
	db 4
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %00000000

Excalmation:
	db 2
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %00000000
	db %10000000
Quotes:
	db 4
	db %10100000
	db %10100000
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %00000000
Hash:
	db 6
	db %01010000
	db %11111000
	db %01010000
	db %01010000
	db %01010000
	db %11111000
	db %01010000
Dollar:
	db 6
	db %01110000
	db %10100000
	db %01110000
	db %00101000
	db %00101000
	db %11110000
	db %00100000
Percent:
	db 6
	db %11001000
	db %11010000
	db %00010000
	db %00100000
	db %01000000
	db %01011000
	db %10011000
Ambersand:
	db 6
	db %00110000
	db %01000000
	db %00100000
	db %01010000
	db %10000000
	db %10001000
	db %01110000
Apostrophe:
	db 3
	db %01000000
	db %10000000
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %00000000
OpenBracket:
	db 3
	db %01000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %01000000
CloseBracket:
	db 3
	db %10000000
	db %01000000
	db %01000000
	db %01000000
	db %01000000
	db %01000000
	db %10000000
Bang:
	db 6
	db %00000000
	db %00000000
	db %10001000
	db %01010000
	db %11111000
	db %01010000
	db %10001000
Plus:
	db 5
	db %00000000
	db %00000000
	db %00100000
	db %00100000
	db %11110000
	db %00100000
	db %00100000
Comma:
	db 3
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %01000000
	db %01000000
	db %10000000

Minus:
	db 5
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %11110000
	db %00000000
	db %00000000
Stop:
	db 3
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %11000000
	db %11000000
Divide:
	db 4
	db %00000000
	db %00100000
	db %00100000
	db %01000000
	db %01000000
	db %10000000
	db %10000000
Zero:
	db 5
	db %01100000
	db %10010000
	db %10010000
	db %10010000
	db %10010000
	db %10010000
	db %01100000
One:
	db 4
	db %01000000
	db %11000000
	db %01000000
	db %01000000
	db %01000000
	db %01000000
	db %11100000
Two:
	db 5
	db %01100000
	db %10010000
	db %00010000
	db %00100000
	db %00100000
	db %01000000
	db %11110000
Three:
	db 5
	db %01100000
	db %10010000
	db %00010000
	db %01100000
	db %00010000
	db %00010000
	db %11100000
Four:
	db 6
	db %00010000
	db %00110000
	db %01010000
	db %10010000
	db %11111000
	db %00010000
	db %00010000
Five:
	db 6
	db %11110000
	db %10000000
	db %11110000
	db %00001000
	db %00001000
	db %10001000
	db %01110000
Six:
	db 5
	db %00100000
	db %01000000
	db %10000000
	db %11100000
	db %10010000
	db %10010000
	db %01100000
Seven:
	db 5
	db %11110000
	db %00010000
	db %00100000
	db %00100000
	db %01000000
	db %01000000
	db %01000000
Eight:
	db 5
	db %01100000
	db %10010000
	db %01100000
	db %10010000
	db %10010000
	db %10010000
	db %01100000
Nine:
	db 5
	db %01100000
	db %10010000
	db %10010000
	db %01100000
	db %00100000
	db %01000000
	db %01000000
Colon:
	db 2
	db %00000000
	db %00000000
	db %00000000
	db %10000000
	db %00000000
	db %10000000
	db %00000000
SemiColon:
	db 3
	db %00000000
	db %00000000
	db %01000000
	db %00000000
	db %01000000
	db %01000000
	db %10000000
LessThan:
	db 4
	db %00100000
	db %01000000
	db %01000000
	db %10000000
	db %01000000
	db %01000000
	db %00100000
Equal:
	db 4
	db %00000000
	db %00000000
	db %11100000
	db %00000000
	db %11100000
	db %00000000
	db %00000000
GreaterThan:
	db 4
	db %10000000
	db %01000000
	db %01000000
	db %00100000
	db %01000000
	db %01000000
	db %10000000
Question:
	db 5
	db %01100000
	db %10010000
	db %00010000
	db %00100000
	db %01000000
	db %00000000
	db %01000000
At:
	db 7
	db %01111000
	db %10010100
	db %10100100
	db %10111100
	db %10000000
	db %10000000
	db %01111000
CapA:
	db 6
	db %00100000
	db %01010000
	db %01010000
	db %11111000
	db %10001000
	db %10001000
	db %10001000
CapB:
	db 5
	db %11000000
	db %10100000
	db %11100000
	db %10010000
	db %10010000
	db %10010000
	db %11100000
CapC:
	db 4
	db %01100000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %01100000
CapD:
	db 4
	db %11000000
	db %10100000
	db %10100000
	db %10100000
	db %10100000
	db %10100000
	db %11000000
CapE:
	db 5
	db %11100000
	db %10000000
	db %11000000
	db %10000000
	db %10000000
	db %10000000
	db %11110000
CapF:
	db 4
	db %11100000
	db %10000000
	db %11000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
CapG:
	db 5
	db %01110000
	db %10000000
	db %10000000
	db %10000000
	db %10110000
	db %10010000
	db %01110000
CapH:
	db 5
	db %10010000
	db %10010000
	db %11110000
	db %10010000
	db %10010000
	db %10010000
	db %10010000
CapI:
	db 4
	db %11100000
	db %01000000
	db %01000000
	db %01000000
	db %01000000
	db %01000000
	db %11100000
CapJ:
	db 5
	db %01110000
	db %00010000
	db %00010000
	db %00010000
	db %00010000
	db %10010000
	db %01100000
CapK:
	db 5
	db %10100000
	db %10100000
	db %11000000
	db %10100000
	db %10010000
	db %10010000
	db %10010000
CapL:
	db 5
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %11110000
CapM:
	db 6
	db %10001000
	db %11011000
	db %10101000
	db %10001000
	db %10001000
	db %10001000
	db %10001000

CapN:
	db 5
	db %10010000
	db %11010000
	db %11010000
	db %11010000
	db %10110000
	db %10110000
	db %10010000
CapO:
	db 6
	db %01110000
	db %10001000
	db %10001000
	db %10001000
	db %10001000
	db %10001000
	db %01110000
CapP:
	db 5
	db %11100000
	db %10010000
	db %10010000
	db %11100000
	db %10000000
	db %10000000
	db %10000000
CapQ:
	db 6
	db %01110000
	db %10001000
	db %10001000
	db %10001000
	db %10101000
	db %10011000
	db %01111000
CapR:
	db 4
	db %11000000
	db %10100000
	db %10100000
	db %11000000
	db %10100000
	db %10100000
	db %10100000
CapS:
	db 5
	db %01100000
	db %10000000
	db %01100000
	db %00010000
	db %00010000
	db %10010000
	db %01100000
CapT:
	db 6
	db %11111000
	db %00100000
	db %00100000
	db %00100000
	db %00100000
	db %00100000
	db %00100000
CapU:
	db 5
	db %10010000
	db %10010000
	db %10010000
	db %10010000
	db %10010000
	db %10010000
	db %01100000
CapV:
	db 6
	db %10001000
	db %10001000
	db %10001000
	db %10001000
	db %01010000
	db %01010000
	db %00100000
CapW:
	db 8
	db %10000010
	db %10000010
	db %10000010
	db %01000100
	db %01010100
	db %01010100
	db %00101000
CapX:
	db 6
	db %10001000
	db %01010000
	db %01010000
	db %00100000
	db %01010000
	db %01010000
	db %10001000
CapY:
	db 5
	db %10010000
	db %10010000
	db %01100000
	db %01000000
	db %01000000
	db %10000000
	db %10000000
CapZ:
	db 7
	db %11111000
	db %00010000
	db %00100000
	db %00100000
	db %01000000
	db %10000000
	db %11111000
OpenSquare:
	db 4
	db %11100000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %11100000
BackSlash:
	db 4
	db %00000000
	db %10000000
	db %10000000
	db %01000000
	db %01000000
	db %00100000
	db %00100000
CloseSquare:
	db 4
	db %11100000
	db %00100000
	db %00100000
	db %00100000
	db %00100000
	db %00100000
	db %11100000
UpArrow:
	db 4
	db %00100000
	db %01010000
	db %10001000
	db %00000000
	db %00000000
	db %00000000
	db %00000000
UnderScore:
	db 5
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %00000000
	db %11111000
GBP:
	db 5
	db %00100000
	db %01010000
	db %01000000
	db %11100000
	db %01000000
	db %01000000
	db %11110000
LowA:
	db 5
	db %00000000
	db %00000000
	db %01110000
	db %10010000
	db %10010000
	db %10010000
	db %01110000
LowB:
	db 5
	db %10000000
	db %10000000
	db %11100000
	db %10010000
	db %10010000
	db %10010000
	db %11100000
LowC:
	db 5
	db %00000000
	db %00000000
	db %01100000
	db %10010000
	db %10000000
	db %10010000
	db %01100000
LowD:
	db 5
	db %00010000
	db %00010000
	db %01110000
	db %10010000
	db %10010000
	db %10010000
	db %01110000
LowE:
	db 5
	db %00000000
	db %00000000
	db %01100000
	db %10010000
	db %11110000
	db %10000000
	db %01110000
LowF:
	db 3
	db %01000000
	db %10000000
	db %10000000
	db %11000000
	db %10000000
	db %10000000
	db %10000000
LowG:
	db 5
	db %00000000
	db %00000000
	db %01110000
	db %10010000
	db %01110000
	db %00010000
	db %11100000
LowH:
	db 5
	db %10000000
	db %10000000
	db %10000000
	db %11100000
	db %10010000
	db %10010000
	db %10010000
LowI:
	db 2
	db %00000000
	db %10000000
	db %00000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
LowJ:
	db 5
	db %00000000
	db %00010000
	db %00000000
	db %00010000
	db %00010000
	db %10010000
	db %01100000
LowK:
	db 5
	db %10000000
	db %10000000
	db %10000000
	db %10100000
	db %11000000
	db %10100000
	db %10010000
LowL:
	db 4
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %01100000
LowM:
	db 6
	db %00000000
	db %00000000
	db %11010000
	db %10101000
	db %10001000
	db %10001000
	db %10001000
LowN:
	db 4
	db %00000000
	db %00000000
	db %11000000
	db %10100000
	db %10100000
	db %10100000
	db %10100000
LowO:
	db 5
	db %00000000
	db %00000000
	db %01100000
	db %10010000
	db %10010000
	db %10010000
	db %01100000
LowP:
	db 5
	db %00000000
	db %00000000
	db %11100000
	db %10010000
	db %11100000
	db %10000000
	db %10000000
LowQ:
	db 6
	db %00000000
	db %00000000
	db %01110000
	db %10010000
	db %01110000
	db %00010000
	db %00011000
LowR:
	db 5
	db %00000000
	db %00000000
	db %11100000
	db %10010000
	db %10000000
	db %10000000
	db %10000000
LowS:
	db 5
	db %00000000
	db %00000000
	db %01100000
	db %10000000
	db %01100000
	db %00010000
	db %11100000
LowT:
	db 4
	db %00000000
	db %01000000
	db %11100000
	db %01000000
	db %01000000
	db %01000000
	db %00100000
LowU:
	db 5
	db %00000000
	db %00000000
	db %10010000
	db %10010000
	db %10010000
	db %10010000
	db %01100000
LowV:
	db 6
	db %00000000
	db %00000000
	db %10001000
	db %10001000
	db %01010000
	db %01010000
	db %00100000
LowW:
	db 6
	db %00000000
	db %00000000
	db %10001000
	db %10001000
	db %10001000
	db %10101000
	db %01010000
LowX:
	db 6
	db %00000000
	db %00000000
	db %10001000
	db %01010000
	db %00100000
	db %01010000
	db %10001000
LowY:
	db 4
	db %00000000
	db %00000000
	db %10100000
	db %10100000
	db %01100000
	db %00100000
	db %11000000
LowZ:
	db 5
	db %00000000
	db %00000000
	db %11110000
	db %00100000
	db %00100000
	db %01000000
	db %11110000
OpenCurly:
	db 5
	db %00110000
	db %01000000
	db %01000000
	db %10000000
	db %01000000
	db %01000000
	db %00110000
SplitLine:
	db 2
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
	db %10000000
CloseCurly:
	db 5
	db %11000000
	db %00100000
	db %00100000
	db %00010000
	db %00100000
	db %00100000
	db %11000000
Tilde:
	db 5
	db %00000000
	db %01010000
	db %10100000
	db %00000000
	db %00000000
	db %00000000
	db %00000000
CopyRight:
	db 7
	db %01111000
	db %10000100
	db %10010100
	db %10100100
	db %10010100
	db %10000100
	db %01111000



; ******************************************************************************
; Function: Scan the whole keyboard
; ******************************************************************************
PUBLIC _ReadKeyboard
_ReadKeyboard:
			ld		b,39
			ld		hl,_Keys
			xor		a
@lp1:			ld		(hl),a
			inc		hl
			djnz		@lp1

			ld		iy,_Keys
			ld		bc,$fefe															; Caps,Z,X,C,V
			ld		hl,_RawKeys
@ReadAllKeys:   
			in		a,(c)
			ld		(hl),a
			inc		hl
        
			ld		d,5
			ld		e,1
@DoAll: 
			srl		a
			jr		c,@notset
			ld		(iy+0),e
@notset:
			inc		iy
			dec		d
			jr		nz,@DoAll

			ld		a,b
			sla		a
			jr		nc,ExitKeyRead
			or		1
			ld		b,a
			jp		@ReadAllKeys
ExitKeyRead:
			ret


; ******************************************************************************
; Function: Copy sprite data (x,y etc) to BRAM (assumes extended data)
; In:   hl = Src
;       d = slot
;       a = count
;       
;
;   |*|*||0011 0000 0011 1011| 0x303b  |Sprite slot, flags
;   | |*||XXXX XXXX 0101 0111| 0x57    |Sprite attributes
;   | |*||XXXX XXXX 0101 1011| 0x5b    |Sprite pattern

; ******************************************************************************
PUBLIC _CopySpriteData
_CopySpriteData:
			ld		hl,_SpriteData
			ld		(DMASpSrc),hl															; 16
			ld		bc,$303b
			out		(c),d
			ld		hl,640																; 128 * 5
			ld		(DMASpLen),hl															; store size
			ld		hl,DMASpriteCopyProg														; 10
			ld		bc,Z80DMAPORT+(DMASPCOPYSIZE*256)												; 10
			otir																		; 21*20  + 240*4
			ret

; ******************************************************************************
; Function: Read a next register
;           uint16 v = ReadNextReg(uint16 reg)
; ******************************************************************************
PUBLIC _ReadNextReg
_ReadNextReg:
			pop		de																; get return address
			pop		hl

        ; read MSB of raster first
			ld		bc,$243b															; select NEXT register
			out		(c),l
			inc		b																; $253b to access (read or write) value
			in		l,(c)
			ld		h,0
			push		de																; push return address back
			ret																		; return in HL

; ******************************************************************************
; Function: Read a next register
; In:       A = reg
; Out:      A = value
; ******************************************************************************
PUBLIC ReadNextRegsSYS
ReadNextRegsSYS:

        ; read MSB of raster first
			ld		bc,$243b															; select NEXT register
			out		(c),a
			inc		b																; $253b to access (read or write) value
			in		a,(c)
			ret




; *******************************************************************************************************
; *******************************************************************************************************
;                               File System
; *******************************************************************************************************
; *******************************************************************************************************
filename:	db	"savegame.kingdom"

	PUBLIC _SaveGame, SaveGame
_SaveGame:
SaveGame:
			push	ix
			call	GetSetDrive
			ld	ix,filename
			call	fcreate
			and	a
			jr	nz, @save	; created new file so now save
			call	fOpen
			jr	z,@exit		; cant open the existing file, aarrrgghhh
@save:
			ld	ix,$2000
			ld	bc,$2000
			ld	a,(handle)
			call	fwrite
			ld	a,(handle)
			call	fClose
@exit:
			pop	ix
			ret


; *******************************************************************************************************
;   Get/Set the drive (get default drive)
;
; *******************************************************************************************************
PUBLIC GetSetDrive
GetSetDrive:    
			push		af																; no idea what it uses....
			push		bc
			push		de
			push		hl
			push		ix

			xor		a																; set drive. 0 is default
			rst		$08
			db		M_GETSETDRV
			ld		(DefaultDrive),a

			pop		ix
			pop		hl
			pop		de
			pop		bc
			pop		af
			ret


; *******************************************************************************************************
;   Function:   Create a new file
;   In:     ix = filename pointer
;   ret     a  = handle, 0 on error
; *******************************************************************************************************
PUBLIC fcreate
fcreate:
			ld		b,$0c
			push		ix
			pop		hl
			ld		a,42
			rst		$08
			db		$9a
			ld		(handle),a
			ret

; *******************************************************************************************************
;   Function:   Open a file read for reading/writing
;   In:     ix = filename
;           b  = Open filemode
;   ret     a  = handle, 0 on error
; *******************************************************************************************************
PUBLIC fOpen
fOpen:      
			push		hl
			push		ix
			pop		hl
			ld		a,(DefaultDrive)
			rst		$08
			db		F_OPEN
			pop		hl
			ret


; *******************************************************************************************************
;   Function    Read bytes from the open file
;   In:     ix  = address to read into
;           bc  = amount to read
;   ret:        carry set = error
; *******************************************************************************************************
PUBLIC fread
fread:
			or		a																; is it zero?
			ret		z																; if so return        

			push		hl

			push		ix
			pop		hl
			rst		$08
			db		F_READ

			pop		hl
			ret

; *******************************************************************************************************
;   Function    Read bytes from the open file
;   In:         ix  = address to read into
;               bc  = amount to read
;   ret:        carry set = error
; *******************************************************************************************************
PUBLIC fwrite
fwrite:
			or		a																; is it zero?
			ret		z																; if so return        

			push		hl

			push		ix
			pop		hl
			rst		$08
			db		F_WRITE

			pop		hl
			ret

; *******************************************************************************************************
;   Function:   Close open file
;   In:     a  = handle
;   ret     a  = handle, 0 on error
; *******************************************************************************************************
PUBLIC fClose
fClose:     
			or		a																; is it zero?
			ret		z																; if so return        
			rst		$08
			db		F_CLOSE
			ret



; *******************************************************************************************************
;   Function    Read bytes from the open file
;   In:         a   = file handle
;               L   = Seek mode (0=start, 1=rel, 2=-rel)
;               BCDE = bytes to seek
;   ret:        BCDE = file pos from start
; *******************************************************************************************************
PUBLIC fseek
fseek:
			push		ix
			push		hl
			rst		$08
			db		F_SEEK
			pop		hl
			pop		ix
			ret


; *******************************************************************************************************
;   Function:   Get file stats
;   In:         a  = handle
;   ret         a  = error code
;               fc = 1 error
;               fc = 0 no error
;
; The following details are returned in the 11-byte buffer:
;   +0(1)  '*'
;   +1(1)  $81
;   +2(1)  file attributes (MS-DOS format)
;   +3(2)  timestamp (MS-DOS format)
;   +5(2)  datestamp (MS-DOS format)
;   +7(4)  file size in bytes
;
; *******************************************************************************************************
PUBLIC fStat
fStat:     
			or		a																; is it zero?
			ret		z																; if so return     
			push		ix
			ld		ix,FileStatsBuffer 
			rst		$08
			db		F_STAT
			pop		ix
			ret


; *******************************************************************************************************
; Function: Load a whole file into memory   (confirmed working on real machine)
; In:       Load(filename, bank, offset);
; Out:      none
; *******************************************************************************************************
PUBLIC SaveBanks
SaveBanks:
			ld		a,LOADING_BANK
			call		ReadNextRegsSYS
			ld		(LoadBankWorkspace),a

SaveBanksFast:
			ld		a,$50
			call		ReadNextRegsSYS
			ld		(Bank50),a
			ld		a,$51
			call		ReadNextRegsSYS
			ld		(Bank51),a
			ret


; *******************************************************************************************************
; Function: Load a whole file into memory   (confirmed working on real machine)
; In:       Load(filename, bank, offset);
; Out:      none
; *******************************************************************************************************
PUBLIC _Load
_Load:  
			pop		bc																; return address
			ld		(Load_Offset),bc														; stash here for now

        ; Copy filename, as it may be under the ROM and about to be banked out
			pop		hl																; get Filename
			ld		de,Load_Filename
			ld		b,MAX_FILENAME_LEN-1														;
@CountDone:
			ld		a,(hl)
			and		a
			jr		z,@EndOfString
			ld		(de),a
			inc		hl
			inc		de
			djnz		@CountDone
@EndOfString:
			xor		a																; ,0 terminate
			ld		(de),a
			ld		bc,(Load_Offset)														; get return address back

			pop		hl																; get bank (16bit for ease of access)
			ld		a,l
			ld		(Load_Bank),a															; store just the single byte
			pop		hl
			ld		(Load_Offset),hl														; store offset

			push		bc																; push return back
			push		ix																; remember IX as C uses it


        ; Get bank 0 and 1
			call		SaveBanks
			NextReg		$50,255																; make sure ROM is paged in
			NextReg		$51,255

			call		GetSetDrive															; need to do this each time?!?!?


			ld		ix,Load_Filename
			ld		b,FA_READ															; mode OPEN for reading
			call		fOpen
			jp		c,@error_opening														; carry set? so there was an error opening and A=error code
			and		a																; was file handle 0?
			jp		z,@error_opening														; of so there was an error opening.

			ld		(handle),a															; remember handle
			call		fStat																; get file STATS into the fStat buffer - date/time/size etc

        ; -------------------------
        ; Loading Block loop
        ; -------------------------
@LoadingLoop:
			ld		hl,$2000															; max size to load in a blob
			ld		bc,(Load_Offset)														; get the offset
			and		a																; clear carry
			sbc		hl,bc																; Work out the number of bytes left in the bank
			ld		(BytesLeftInBank),hl														; remember this (just use stats buffer as temp storage)
        

			ld		a,(BytesLeftToLoad+2)														; get 24bit high - if set, then fill the bank as we're >64k        
			and		a
			jr		nz,@FillBank
			ld		bc,(BytesLeftToLoad)														; get file size left
			and		a
			sbc		hl,bc																; sub total size, if "negative" then we can fill the remainder of the bank
			jr		c,@FillBank															; if bytes left, then file size (left) won't fill the bank - so use that
			ld		bc,(BytesLeftToLoad)														; Get total bytes left in file
			jr		@SkipFillBank
@FillBank:
			ld		bc,(BytesLeftInBank)														; get bytes left in buffer
@SkipFillBank:
			ld		(BytesToRequest),bc														; remember the number of bytes we're going to load



        ; -------------------------
        ; Now load the next block
        ; -------------------------
			ld		a,(Load_Bank)
			NextReg		LOADING_BANK,a															; bank in where we want to load to - this has been remembered
			ld		ix,(Load_Offset)
			ld		de,LOADING_BASE_ADD
			add		ix,de
			ld		a,(handle)

			call		fread																; read data from A to address IX of length BC                
			jr		c,@error_reading
			ld		(FileStatsBuffer+2),bc														; number of bytes actually read....



        ; -------------------------
        ; Work out if we've more to load, and if so work out next bank/offset/ bytes left
        ; -------------------------

        ; subtract off the bytes loaded to how much is left (full 32bit subtract) - if zero, then we're done.
			ld		hl,(BytesLeftToLoad)
			and		a																; reset carry
			sbc		hl,bc																; HL-(BC+C)
			push		bc
			ld		(BytesLeftToLoad),hl
			ld		hl,(BytesLeftToLoad+2)														; HL-(0+C)
			ld		bc,0
			sbc		hl,bc
			ld		(BytesLeftToLoad+2),hl        
			pop		bc

        ; are we at 0?
			ld		a,h
			or		l
			ld		hl,(BytesLeftToLoad)
			or		h
			or		l
			jr		z,@LoadedAll


        ; More to load, so move on load address
			ld		hl,(Load_Offset)
			add		hl,bc
			ld		a,h
			and		$e0																; have we overflowed the bank?
			jr		z,@NoOverlow
			ld		a,(Load_Bank)															; overflow - so increment bank
			inc		a
			ld		(Load_Bank),a

			ld		a,h																; wrap bank - hl should technically always be $0000 at this point
			and		$1f
			ld		h,a

@NoOverlow:
			ld		(Load_Offset),hl														; store new bank offset
			jr		@LoadingLoop


@LoadedAll:
			ld		a,(handle)
			call		fClose																; close file
			jr		c,@error_closing

			jp		@skiperrors
        ;pop     ix                     ; fall through instead (change if we get error codes etc)
        ;jp      RestoreBanks

;
; On error, display error code an lock up so we can see it
;
@error_opening:
@error_reading:     
@error_closing:
@NormalError:   
			pop		ix
			ld		a,255
			jp		@skip
@skiperrors:
			pop		ix
			xor		a
@skip:
			push		af
RestoreBanks:
        ; reset all the used banks
			ld		a,(LoadBankWorkspace)
			NextReg		LOADING_BANK,a
RestoreBanksFast:
			ld		a,(Bank50)
			NextReg		$50,a
			ld		a,(Bank51)
			NextReg		$51,a
			pop		af
			ld		l,a
			ret




; ******************************************************************************************************************************
; ******************************************************************************************************************************
; ******************************************************************************************************************************
;       Kernel Data
; ******************************************************************************************************************************
; ******************************************************************************************************************************
; ******************************************************************************************************************************
DefaultDrive:		db		0																; Current Drive
handle:			db		0																; current open file handle
Load_Bank:		db		0																; Loading bank
Load_Offset:		dw		0																; Loading bank
Load_Filename:		ds		MAX_FILENAME_LEN														; space to copy the filename - incase it's now under the ROM. "data/test.txt",0
Bank50:			db		0																; Remmeber Bank 50
Bank51:			db		0																; Remmeber Bank 51
LoadBankWorkspace:	db		0																; Rememebr Bank 56
_PrintOffset:		db		0																; offset from $4000


;   +0(1) '*'
;   +1(1) $81
;   +2(1) file attributes (MS-DOS format)
;   +3(2) timestamp (MS-DOS format)
;   +5(2) datestamp (MS-DOS format)
;   +7(4) file size in bytes
TempBuffer:		ds		64  
			defc		FileStatsBuffer = TempBuffer													;              ; 11 byte buffer
			defc		BytesLeftToLoad = FileStatsBuffer+7
			defc		BytesLeftInBank = FileStatsBuffer
			defc		BytesToRequest = FileStatsBuffer+2
			defc		PrintCoords = FileStatsBuffer





DMASpriteCopyProg:
			db		$C3																; R6-RESET DMA
			db		$C7																; R6-RESET PORT A Timing
			db		$CB																; R6-SET PORT B Timing same as PORT A

			db		$7D																; R0-Transfer mode, A -> B
DMASpSrc:
			dw		$1234																; R0-Port A, Start address               (source address)
DMASpLen:    
			dw		240																; R0-Block length                        (length in bytes)

			db		$54																; R1-Port A address incrementing, variable timing
			db		$02																; R1-Cycle length port A
      
			db		$78																; R2-Port B address fixed, variable timing Write to a "PORT"
			db		$02																; R2-Cycle length (2) port B
      
			db		$AD																; R4-Continuous mode  (use this for block tansfer)
DMASpDest:
			dw		$0057																; R4-Dest address (Sprite DATA)          (destination port)
          
			db		$82																; R5-Restart on end of block, RDY active LOW
     
			db		$CF																; R6-Load
			db		$B3																; R6-Force Ready
			db		$87																; R6-Enable DMA
ENDSPDMA:
			defc		DMASPCOPYSIZE   = ENDSPDMA-DMASpriteCopyProg


; ******************************************************************************
; Writable DMA Program
; ******************************************************************************
DMACopyProg:
			db		$C3																; R6-RESET DMA
			db		$C7																; R6-RESET PORT A Timing
			db		$CB																; R6-SET PORT B Timing same as PORT A
        
			db		$7D																; R0-Transfer mode, A -> B
DMASrc:			dw		$0000																; R0-Port A, Start address      (source address)
DMALen:			dw		6912																; R0-Block length           (length in bytes)
        
			db		$54																; R1-Port A address incrementing, variable timing
			db		$02																; R1-Cycle length port A
                
			db		$50																; R2-Port B address fixed, variable timing
			db		$02																; R2-Cycle length port B
                
			db		$AD																; R4-Continuous mode  (use this for block tansfer)
DMADest:		dw		$4000																; R4-Dest address           (destination address)
                
			db		$82																; R5-Restart on end of block, RDY active LOW
            
			db		$CF																; R6-Load
			db		$B3																; R6-Force Ready
			db		$87																; R6-Enable DMA
ENDDMA:

			defc		DMASIZE = ENDDMA-DMACopyProg



; ******************************************************************************************************************************
;   Writable DMA Programs
; ******************************************************************************************************************************

DMAFillVal:		DB 0		; Storage for the DMA value to fill with

DMAFillProg:		DB $83		; R6-Disable DMA
			DB %01111101	; R0-Transfer mode, A -> B, write address
DMAFillSrc:		DW DMAFillVal	; Address of the fill byte
DMAFillLen:		DW 0		; Number of bytes to fill
			DB %00100100	; R0-Block length, A->B
			DB %00010000	; R1-Port A address incrementing
			DB %10101101	; R4-Continuous mode
DMAFillDst:		DW 0		; Destination address
			DB $CF		; R6-Load	
			DB $87		; R6-Enable DMA

			DC DMAFillProgL = ASMPC - DMAFillProg


; ******************************************************************************
; DMA to a register (Copper)
; ******************************************************************************
PUBLIC DMACopyToReg
DMACopyToReg:
			db		$C3																;R6-RESET DMA
			db		$C7																;R6-RESET PORT A Timing
			db		$CB																;R6-SET PORT B Timing same as PORT A

			db		$7D																;R0-Transfer mode, A -> B (%01 = transfer, %1xx = A->B)
DMASrcAdd:
			dw		$1234																;R0-Port A, Start address               (source address)
DMASrcLen:
			dw		240																;R0-Block length                    (length in bytes)

			db		$54																;R1-Port A address incrementing, variable timing
			db		$02																;R1-Cycle length port A
          
			db		%01101000															;R2-Port B bit 3 = PORT dest, bit 6 = cycle len.
			db		$02																;R2-Cycle length port B
          
			db		$AD																;R4-Continuous mode  (%01 Continuous, %11 Port Add Lo/Hi, %01 = $ad )
			dw		$253B																;R4-Dest address
          
			db		$82																;R5-Restart on end of block, RDY active LOW
     
			db		$CF																;R6-Load
			db		$B3																;R6-Force Ready
			db		$87																;R6-Enable DMA
DMACopyToReg_End:

			defc		DMACOPPERSIZE = DMACopyToReg_End-DMACopyToReg



PUBLIC _Keys
_Keys:			ds		40
PUBLIC _RawKeys
_RawKeys:		ds		8
PUBLIC _RomFont
_RomFont:		ds		0x300																; copy of the ROM font
			incbin		"ROMFONT.FNT"

            ; xxxxxxxx
            ; yyyyyyyy
            ; PPPP_XM_YM_R_X8/PR
            ; V_E_NNNNNN
            ; H_N6_T_XX_YY_Y8
            ; +- 0_1_N6_XX_YY_PO
            ; +- 0_1_N6_0000_PO

PUBLIC _SpriteData
_SpriteData:		ds		128*5																; raw sprite data

PUBLIC _SpriteShape
_SpriteShape:
			db		$e3,$e3,$e3,$e3,$e3,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$e3,$ff,$ff,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$ff,$ff,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$e3,$ff,$ff,$e3,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$e3,$ff,$ff,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$e3,$ff,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$e3,$e3,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$e3,$ff,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$ff,$ff,$ff,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$ff,$ff,$ff,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$ff,$ff,$ff,$e3,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$ff,$ff,$ff,$ff,$e3,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$e3,$ff,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$ff,$ff,$ff,$e3,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$ff,$ff,$e3,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$e3,$ff,$ff,$ff,$e3,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3

			db		$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff
			db		$ff,$ff,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$ff
			db		$ff,$00,$ff,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$00,$ff
			db		$ff,$00,$00,$ff,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$00,$00,$ff
			db		$ff,$00,$00,$00,$ff,$00,$00,$00,$00,$00,$00,$ff,$00,$00,$00,$ff
			db		$ff,$00,$00,$00,$00,$ff,$00,$00,$00,$00,$ff,$00,$00,$00,$00,$ff
			db		$ff,$00,$00,$00,$00,$00,$ff,$00,$00,$ff,$00,$00,$00,$00,$00,$ff
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$ff,$00,$00,$00,$00,$00,$00,$ff
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$ff,$00,$00,$00,$00,$00,$00,$ff
			db		$ff,$00,$00,$00,$00,$00,$ff,$00,$00,$ff,$00,$00,$00,$00,$00,$ff
			db		$ff,$00,$00,$00,$00,$ff,$00,$00,$00,$00,$ff,$00,$00,$00,$00,$ff
			db		$ff,$00,$00,$00,$ff,$00,$00,$00,$00,$00,$00,$ff,$00,$00,$00,$ff
			db		$ff,$00,$00,$ff,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$00,$00,$ff
			db		$ff,$00,$ff,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$00,$ff
			db		$ff,$ff,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$ff,$ff
			db		$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff

			db		$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$00,$00,$00,$00,$00,$00,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3
			db		$ff,$ff,$ff,$ff,$ff,$ff,$ff,$ff,$e3,$e3,$e3,$e3,$e3,$e3,$e3,$e3

_EndKernel:


