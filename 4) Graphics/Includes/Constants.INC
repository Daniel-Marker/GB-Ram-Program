;****************************************************************************************************************************************************
;*	user data (constants)
;****************************************************************************************************************************************************
cDefaultPalette		EQU %11100100
cTileSizeInBytes	EQU 16	
cNumTiles			EQU 38
cArrowTile			EQU 37
cArrowX				EQU 8 + 8 * 2
cArrowInitY			EQU 16 + 7 * 8
cArrowDeltaY		EQU 8
cUp					EQU $40	
cDown				EQU $80	
cLeft				EQU $20					
cRight				EQU $10	
cA					EQU $01	
cB					EQU $02	
cStart				EQU $08	
cSelect				EQU $04	
cNumOptions			EQU 2
cEntryPoint			EQU $0100
cFlashEntryPoint	EQU $0150
cEcho				EQU $E000
cNameLocation		EQU $A598
cSaveRamBank		EQU 1
cRamEnable			EQU $0A
cChecksumLocation	EQU $B523
cLastVisibleTile	EQU $9A33
cMemoryWidth		EQU 7
cMemoryHeight		EQU 18
cMemoryToRead		EQU $3FA6
cBankToReadFrom		EQU $1C
cHexFirstTile		EQU $40

;cReproCart			EQU 1
cReproCart			EQU 0


WriteDMACodeToHRAM	EQU $4BED
hLoadedROMBank 		EQU $FFB8
wTitleMonSpecies	EQU $CD3D
LoadTitleMonSprite	EQU $4524
cDoduo				EQU $46
cHDMAFuncLength		EQU $0A
wStack				EQU $DFFF
wAudioROMBank		EQU $C0EF
cTitleAudioBank		EQU $1F
cPokemonWidth		EQU 7
cPokemonHeight		EQU 7
cWindowX			EQU $D0
cWindowY			EQU $E0
