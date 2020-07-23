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
;registers used: a
move_arrow::
	ld a, cArrowX
	ld [_OAMRAM + 1], a

	ld a, [arrow_pos]
	or a
	jr z, option_1		;0 or 0 = 0, so if arrow_pos = 0, then it will jump to option_1
	cp 1
	jr z, option_2
	cp 2
	jr z, option_3
						;if the code gets to this point, arrow_pos is 3 or greater, so assume that it's 3
option_4:	
	ld a, cArrowOption4
	jr move_sprite
	
option_1:
	ld a, cArrowOption1
	jr move_sprite
	
option_2:
	ld a, cArrowOption2
	jr move_sprite

option_3:
	ld a, cArrowOption3
	
move_sprite:
	ld [_OAMRAM], a
	
	ret
	
;a = input: (n/a), output: (n/a)
;bc = input: (n/a), output: (n/a)
;de = input: (n/a), output: (n/a)
;hl = input: (n/a), output: (n/a)
;Result stored in RAM
;registers used: a, b
poll_input::
	ld a, P1F_4		;set P14 out port so that we can get the dpad input
	ld [rP1], a
	
	ld a, [rP1]		;Read input multiple times to debounce
	ld a, [rP1]
	ld a, [rP1]
	ld a, [rP1]
	cpl				;complement as the input is set high if not pressed
	and $0F			;input is only stored in lower nibble, so zero out everything else
	
	ld b, a			;store lower nibble in a
	
	ld a, P1F_5		;set P15 pin so that we can get other input (A,B, Start, Select)
	ld [rP1], a
	
	ld a, [rP1]
	ld a, [rP1]
	ld a, [rP1]
	ld a, [rP1]
	cpl
	and $0F
	
	swap a
	or b			;or b so that the lower half of a = lower half of b. This way a now holds the state of all inputs
	
	ld b, a
	ld a, [input_down]
	cpl
	and b					
	ld [input_pressed], a	;input_pressed = currenntInput and cpl(lastInput), so any input held from last frame gets zeroed out
	
	ld a, b
	ld [input_down], a
	
	ret
