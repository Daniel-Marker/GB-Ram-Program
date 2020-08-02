;a = input: (n/a), output: (n/a)
;bc = input: (n/a), output: (n/a)
;de = input: (n/a), output: (n/a)
;hl = input: (n/a), output: (n/a)
;Result stored in RAM
;registers used: a, b
GET_INPUT: MACRO

	
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
ENDM


;a = input: (n/a), output: (n/a)
;bc = input: delay counter, output: (n/a)
;de = input: (n/a), output: (n/a)
;hl = input: (n/a), output: (n/a)
;registers used: a, bc
DELAY: MACRO
.delay:
	dec bc
	ld a, b
	or c
	jr nz, .delay

ENDM


WAIT_FOR_A: MACRO
	di
	ld bc, $FFFF
	DELAY			;Wait for a short amount of time, so that input from options screen isn't immediately used again here
	
.loop:
	GET_INPUT
	ld a, [input_pressed]
	and cA
	or a
	jr z, .loop
ENDM