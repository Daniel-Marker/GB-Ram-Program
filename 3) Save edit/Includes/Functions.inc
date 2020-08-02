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
	ld a, [hl+]
	ld [de], a
	inc de
	
	dec bc
	ld a, b
	or c
	jr nz, memcpy
	
	ret

;a = input: value to set, output: (n/a)
;bc = input: number of bytes to write, output: (n/a)
;de = input: destination, output: (n/a)
;hl = input: (n/a), output: (n/a)
;registers used: a, bc, de, l
memset::
	ld l, a
	
.loop:
	ld a, l
	ld [de], a
	inc de
	
	dec bc
	ld a, b
	or c
	
	jr nz, .loop
	
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
	
	;move this to 3) Save edit 
	
	;firstly, we need to check how the flashcart loads/saves
	;so we need to be able to display N bytes at a given memory address
	;to do this, we could figure out how to display 0-F and then display each nibble as a character
	
.main_loop:
	
	jr .main_loop
menu_save_end

menu_graphics::
	jr menu_graphics
menu_graphics_end

menu_advGraphics::
	jr menu_advGraphics	
menu_advGraphics_end
