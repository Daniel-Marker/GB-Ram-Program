;****************************************************************************************************************************************************
;*	Functions
;****************************************************************************************************************************************************
SECTION "Functions", ROM0

;a = input: (n/a), output: (n/a)
;bc = input: number of bytes to copy, output: (n/a)
;de = input: destination, output: (n/a)
;hl = input: source, output: (n/a)
;registers used: a, bc, de, hl
memcpy::
	MEMCPY
	ret

;a = input: value to set, output: (n/a)
;bc = input: number of bytes to write, output: (n/a)
;de = input: destination, output: (n/a)
;hl = input: (n/a), output: (n/a)
;registers used: a, bc, de, l
memset::
	MEMSET
	ret

;a = input: (n/a), output: (n/a)
;b = input: map height (1-32), output: (n/a)
;c = input: map width (1-32), output: (n/a)
;de = input: destination, output: (n/a)
;hl = input: source, output: (n/a)
;registers used: a, bc, de, hl
load_map::
	push bc
	
	ld b, 0
	call memcpy
	
	pop bc
	
	ld a, 32
	sub a, c		;a = 32 - width
	
	add a, e
	ld e, a
	
	ld a, 0
	adc a, d
	ld d, a			;de = end of line + (32-width), which puts de at the start of the next line
	
	
	dec b
	jr nz, load_map
	
	ret

;a = input: (n/a), output: (n/a)
;bc = input: (n/a), output: (n/a)
;de = input: (n/a), output: (n/a)
;hl = input: (n/a), output: (n/a)
;registers used: a, bc, de, hl
handle_arrow::
	ld a, [input_pressed]
	
	bit 6, a
	jr nz, arrow_up
	bit 7, a
	jr nz, arrow_down
	jr end

arrow_up:
	ld a, [arrow_pos]
	or a
	jr z, end
	dec a
	ld [arrow_pos], a
	jr end

arrow_down:	
	ld a, [arrow_pos]
	cp cNumOptions
	jr z, end
	inc a
	ld [arrow_pos], a
	
end:

	ld a, cArrowX
	ld [_OAMRAM + 1], a
	
	ld a, [arrow_pos]
	ld b, a
	ld e, cArrowDeltaY
	ld hl, cArrowInitY
	
	call multiply
	
	ld a, l
	ld [_OAMRAM], a
	
	ret
	
;a = input: (n/a), output: (n/a)
;bc = input: (n/a), output: (n/a)
;de = input: (n/a), output: (n/a)
;hl = input: (n/a), output: (n/a)
;Result stored in RAM
;registers used: a, b
poll_input::
	GET_INPUT
	ret

;a = input: (n/a), output: (n/a)
;b = input: multiplicand, output: (n/a)
;c = input: (n/a), output: (n/a)
;d = input: (n/a), output: (n/a)
;e = input: multiplier, output: (n/a)
;hl = input: (n/a), output: result + inital hl value
;registers used: bc, de, hl
multiply::
	ld d, 0
	ld c, 8

loop:
	bit 0, b
	jr z, add_skip
	
	add hl, de
	
add_skip:
	srl b
	
	sla e
	rl d
	
	dec c
	jr nz, loop
	
	ret


menu_start::	
	WAIT_FOR_A
	jp cEntryPoint
menu_start_end
	
menu_save::
	WAIT_FOR_A
	
	IF cReproCart == 1
		;if cReproCart is 1, then we are building for a reprocart with flash memory
		;so we can use this code ripped from the cartridge to load from flash into SRAM
		
		push af
		ld a, $30
		ld [$FFB8], a
		ld [$2000], a
		call $7C00
		dec a
		ld bc, $B8E0
		ld [$2000], a
		pop af
	ENDC
	
	ld a, cSaveRamBank
	ld [rRAMB], a		;set the ram bank
	
	ld a, cRamEnable
	ld [rRAMG], a		;and enable it
	
	ld hl, cNameLocation
	
	;Overwrite the existing name with "Green"
	ld a, $86		;G
	ld [hl+], a
	ld a, $B1		;r
	ld [hl+], a
	ld a, $A4		;e
	ld [hl+], a
	ld a, $A4		;e
	ld [hl+], a
	ld a, $AD		;n
	ld [hl+], a
	ld a, $50		;terminator
	ld [hl], a
	
	
	xor a
	ld hl, cNameLocation
	ld bc, cChecksumLocation - cNameLocation
	ld d, 0
.loop:				;calculate checksum from cNameLocation to cChecksumLocation
	ld a, [hl+]
	add d
	ld d, a
	
	dec bc
	ld a, b
	or c
	jr nz, .loop
	
	ld a, d
	cpl							;Inverse the checksum so that it matches the format used in the game
	ld [cChecksumLocation], a
	
	IF cReproCart == 1
		ld a, $1C
		ld [rROMB0], a
		
		;if cReproCart is 1, then we are building for a reprocart with flash memory
		;so we can use this code ripped from the cartridge to save to flash
		
		ld a, [$FFB8]
		push af
		call $0019
		call $6100
		call $DE00
		call $0019
		call $6200
		call $DE00
		call $0019
		call $6300
		call $DE00
		call $0019
		call $6400
		call $DE00
		call $0019
		call $6500
		call $DE00
		pop af
		
		jp cFlashEntryPoint
	ENDC
	
	jp cEntryPoint
menu_save_end

menu_graphics::
	WAIT_FOR_A
	
	ld sp, $DFFF
	
	ld a, 1
	ld [$FFB8], a
	ld [$2000], a
	call $4BED
	
	xor a
	ld bc, $FFFF - $FF8A
	ld de, $FF8A
	MEMSET
	
	ld a, 1
	ld [$FFB8], a
	ld [$2000], a
	
	ld a, $1F
	ld [$C0EF], a
	
	ei
	
	ld a, $46
	ld [$CD3D], a
	call $4524
	
	

	di
	xor a
	ld [rLCDC], a

	xor a
	ld bc, $9A34 - $9800
	ld de, $9800
	MEMSET
	
	ld b, 7
	ld c, 7
	ld d, 0
	ld hl, $9800
.output_loop:
	ld a, d
	ld [hl], a
	inc d
	
	push de
	
	ld de, $20
	add hl, de
	
	pop de
	
	dec b
	jr nz, .output_loop
	
	push de
	
	ld de, 32*7
	ld a, d
	cpl
	ld d, a
	ld a, e
	cpl
	ld e, a
	inc de
	
	inc de
	add hl, de
	
	pop de
	ld b, 07
	dec c
	jr nz, .output_loop
	
	ld a, $E0
	ld [rSCY], a
	ld a, $D0
	ld [rSCX], a
	
	ld a, (LCDCF_ON | LCDCF_WIN9800 | LCDCF_WINOFF | LCDCF_BG8800 | LCDCF_BG9800 | LCDCF_OBJ8 | LCDCF_OBJON | LCDCF_BGON)
	ld [rLCDC], a
	
main_loop:
	jr main_loop
menu_graphics_end

menu_advGraphics::
	jr menu_advGraphics	
menu_advGraphics_end

memory_viewer::
	WAIT_FOR_A

	ld a, [rLCDC]
	push af
	xor a
	ld [rLCDC], a		;turn off the screen
	
	ld bc, cLastVisibleTile - _SCRN0
	ld de, _SCRN0
	MEMSET				;clear the current map
	
	ld a, cBankToReadFrom
	ld [rROMB0], a		;set the bank
	
	ld bc, cMemoryWidth * cMemoryHeight
	ld de, buffer
	ld hl, cMemoryToRead
	MEMCPY				;copy cMemoryWidth * cMemoryHeight bytes into the buffer

	
	
	ld b, cMemoryWidth
	ld c, cMemoryHeight
	ld de, buffer
	ld hl, _SCRN0
.loop:
	ld a, [de]
	inc de
	
	push de
	
	ld d, a
	
	and $F0
	swap a
	add cHexFirstTile
	ld [hl+], a			;mask out lower nibble and add cHexFirstTile, so that a contains the tile no representing the hex digit
	
	ld a, d
	and $0F
	add cHexFirstTile
	ld [hl+], a
	
	pop de
	
	
	inc hl
	dec b
	jr nz, .loop
	
	push de
	ld de, 11			;add 11 so that we are at the start of the next line
	add hl, de
	pop de
	
	ld b, cMemoryWidth
	dec c
	jr nz, .loop
	
	pop af
	ld [rLCDC], a	;turn the screen back on
	
.main_loop:
	jr .main_loop
memory_viewer_end
