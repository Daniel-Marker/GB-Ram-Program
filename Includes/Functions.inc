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
;bc = input: (n/a), output: (n/a)
;de = input: destination, output: (n/a)
;hl = input: source, output: (n/a)
;registers used: a, bc, de, hl
load_map::
	ld c, 18
	
.loop:
	push bc
	
	ld bc, 20
	call memcpy
	
	ld a, 12
	add a, e
	ld e, a
	
	ld a, 0
	adc a, d
	ld d, a
	
	
	pop bc
	dec c
	jr nz, .loop
	
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
	jr z, option_1
	cp 1
	jr z, option_2
	cp 2
	jr z, option_3

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