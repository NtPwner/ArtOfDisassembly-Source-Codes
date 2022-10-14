.const
Practice_FirstID	equ 	1001	; The ID of the first control (used for setting fonts)
Practice_LastID		equ 	1031	; The ID of the last  control (used for setting fonts)

;-------------------------------------------------------- Constants 4 Decoding -------------
;------------------- Instruction Type

_LODS	db "LODS",0
_SCAS	db "SCAS",0
_STOS	db "STOS",0

InstTable		dd	offset _LODS,
					offset _SCAS, 
					offset _STOS,

;------------------- Segment Registers
_CS		db "CS",0 		
_SS		db "SS",0			
_DS		db "DS",0		
_ES		db "ES",0			
_FS		db "FS",0
_GS		db "GS",0

SegRegsTable	dd 	offset _CS, 
					offset _SS, 
					offset _DS,
					offset _ES,
					offset _FS,
					offset _GS,
						
;------------------- Operand Sizes

_Operand32	db	"DWORD",0
_Operand16	db	"WORD",0

OperandSizeTable	dd	offset _Operand16,
						offset _Operand32

;------------------- Address Sizes

;    32-Bit Registers    
_ESI	db	"ESI",0
_EDI	db	"EDI",0

;    16-Bit Registers
_SI		db	"SI",0
_DI		db	"DI",0

Reg32Table	dd	offset _ESI,
				offset _EDI
			
Reg16Table	dd	offset _SI,
				offset _DI			

;------------------- REP Prefixes
EmptyPrefix	db 0

_REP	db	"REP",0
_REPE	db	"REPE",0
_REPNE	db	"REPNE",0

REPTable1	dd	offset _REP,
				offset _REPNE
				
REPTable2	dd	offset _REPE,
				offset _REPNE		
				

_NULL	db	0		;		

.data
;----------------------------------------------Instructions Decoding Format-----------------
Inst_HEX				db	"%s:%s:%s%:%s:%s",0
Inst_DECODING			db	"%s %s %s PTR %s:[%s]",0

.data?
Pracice_ColorCTL		dd	9 dup (?)		; Handles To The Controls Need To Be Colored

;+----------------------------------------------
SegRegIndex				dd	?				; Index of the Segment Register To be Used
InstIndex				dd  ?				; Index of the Instruction To be used
OperandSizeIndex		dd  ?				; Index of the Operand-Size To be used
AddressSizeIndex		dd	?				; Index of the Address-Size -Register- to be used
DefaultAddressReg		dd	?				; Default Address Register (0 = ESI/1 = EDI)
REPPrefixIndex			dd	?				; Index of the REP Prefix To be used
DefaultREPPrefix		dd	?				; Default REP Prefix Mode (0 = REP/1 = REPE)
;+----------------------------------------------

Buffer					db 250 dup (?)		; Buffer used in wsprintfA
.code

EnableButton proc	uses eax Button:DWORD, FirstID:DWORD, LastID:DWORD, hWin:DWORD
;--------------------------------------------------------------
;	This procedure takes an ID to a CommandButton, and loop 
;	through a range of IDs, then makes the wanted button the
;	only disabled button.
;--------------------------------------------------------------
;------------------------------------- Enable ALL buttons -----
	mov ecx, LastID
	sub ecx, FirstID	
	mov eax, FirstID
	inc ecx
	dec eax
 @@:	
	pusha
	add eax, ecx
	invoke GetDlgItem,hWin,eax
	invoke EnableWindow,eax,TRUE
	popa
	dec ecx
	jnz @B
;---------------------------Disable the pressed button---------
	
;--------------------------------------------------------------
	invoke GetDlgItem,hWin,Button
	invoke EnableWindow,eax,FALSE
;--------------------------------------------------------------	
	ret
EnableButton endp

DecodeInstruction proc hWin:DWORD
	LOCAL Instruction:DWORD
	LOCAL SegReg:DWORD
	LOCAL OperandSize:DWORD
	LOCAL AddressSize:DWORD
	LOCAL REPPrefix:DWORD
	;----------------------------- Decode Instruction Name ------------------
	mov eax, InstIndex
	mov eax, InstTable+[eax*4]
	mov Instruction, eax; 								Instruction Name
	;----------------------------- Decode Segment Register ------------------
	mov eax, SegRegIndex
	mov eax, SegRegsTable+[eax*4]
	mov SegReg, eax; 									Segment Register Name
	;------------------------------- Decode Operand-Size --------------------
	mov eax, OperandSizeIndex
	mov eax, OperandSizeTable+[eax*4]
	mov OperandSize, eax; 								Operand Size
	;------------------------------- Decode Address-Size --------------------
	mov eax, AddressSizeIndex
	.If eax==1		; AddressSize == 32-Bit
		mov eax, DefaultAddressReg
		mov eax, Reg32Table+[eax*4]
	.else			; AddressSize == 16-Bit
		mov eax, DefaultAddressReg
		mov eax, Reg16Table+[eax*4]	
	.endif
	mov AddressSize, eax
	;--------------------------------- Decode REP Prefix --------------------
	xchg eax, ebx 
	xchg eax, ebx 	
	mov eax, REPPrefixIndex
	.If eax==0						; REP Prefixes  = (REP OR REPE)
		mov eax, DefaultREPPrefix
		.If eax==0						; REP
			mov eax, REPTable1+[0*4]	
		.else							; REPE
			mov eax, REPTable2+[0*4]						
		.endif
	.elseif eax==1					; REP Prefixes  = (REPE OR REPNE)
		mov eax, DefaultREPPrefix
		.If eax==0						; REP
			mov eax, REPTable1+[1*4]	
		.else							; REPE
			mov eax, REPTable2+[1*4]						
		.endif
	.else
		mov eax, offset EmptyPrefix		; No REP Prefix
	.endif
	mov REPPrefix, eax
	
	invoke wsprintfA, addr Buffer, addr Inst_DECODING, \
	REPPrefix, Instruction ,OperandSize,SegReg,AddressSize
	invoke SetDlgItemText,hWin,1029,addr Buffer		; Update the instruction (Static)	
	
	ret
DecodeInstruction endp

PracticeProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
	mov eax, uMsg
	.if eax==WM_INITDIALOG
;---------------------------------------------------------Saves Some HWNDS to color :)----
		invoke GetDlgItem,hWin,1001				; Title
		mov Pracice_ColorCTL[0*4], eax				
		invoke GetDlgItem,hWin,1003				; 'Instruction'  			(STATIC)
		mov Pracice_ColorCTL[1*4], eax				
		invoke GetDlgItem,hWin,1015				; 'Segment Prefix' 			(STATIC)
		mov Pracice_ColorCTL[2*4], eax				
		invoke GetDlgItem,hWin,1018				; 'Operand-Size Prefix' 	(STATIC)
		mov Pracice_ColorCTL[3*4], eax				
		invoke GetDlgItem,hWin,1022				; 'Address-Size Prefix' 	(STATIC)
		mov Pracice_ColorCTL[4*4], eax						
		invoke GetDlgItem,hWin,1027				; 'REPeat Prefixes' 		(STATIC)
		mov Pracice_ColorCTL[5*4], eax						
		invoke GetDlgItem,hWin,1030				; 'Decoding' (Title)		(STATIC)
		mov Pracice_ColorCTL[6*4], eax			
		invoke GetDlgItem,hWin,1029				; 'Decoding'(@ The Buttom) 	(STATIC)
		mov Pracice_ColorCTL[7*4], eax								
;---------------------------------------------------------Choose Defaults Options--------
		invoke EnableButton, 1004, 1004, 1006, hWin
		invoke EnableButton, 1008, 1008, 1013, hWin
		invoke EnableButton, 1016, 1016, 1017, hWin
		invoke EnableButton, 1020, 1020, 1021, hWin
		invoke EnableButton, 1024, 1024, 1026, hWin
;----------------------------------------------------------- Set fonts ------------------
		invoke SetWindowFont, hWin, Practice_FirstID, Practice_LastID
;----------------------------------------------------------------------------------------
	.elseif eax==WM_CTLCOLORSTATIC
		mov eax, lParam
		.if eax==Pracice_ColorCTL[0*4] || eax==Pracice_ColorCTL[6*4]
			;-------------------------------------
			;		Controls With RED Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,000000FFh
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret
		.elseif eax==Pracice_ColorCTL[1*4] || eax==Pracice_ColorCTL[2*4] \
		|| eax==Pracice_ColorCTL[3*4] || eax==Pracice_ColorCTL[4*4]		\
		|| eax==Pracice_ColorCTL[5*4]	
			;-------------------------------------
			;	Controls With DARK GREEN Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,001A7B39h
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret	
		.elseif eax==Pracice_ColorCTL[7*4]
			;-------------------------------------
			;		Controls With BLUE Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,00F73833h
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret				
		.endif			
	.elseif eax==WM_COMMAND
		mov eax, wParam
		and eax, 0FFFFh
		.if eax >= 1004 && eax <= 1006 				; Instructions' CommandButtons
			;-------------------------------------
			invoke EnableButton, eax, 1004, 1006, hWin
			sub eax, 1004								; Store the Instruction Index
			mov InstIndex, eax							; For Later Decoding
			.If eax==0		; If Instruction = LODSB
				mov DefaultAddressReg, 0;	Default Addres Register = [ESI/SI]
				mov DefaultREPPrefix, 0;	Default REP Prefixes  = (REP/REPNE)			
			.elseif eax==1	; Instruction = SCAS
				mov DefaultREPPrefix, 1;	Default REP Prefixes  = (REPE/REPNE)						
				mov DefaultAddressReg, 1;	Default Addres Register = [EDI/DI]				
			.else
				mov DefaultAddressReg, 1;	Default Addres Register = [EDI/DI]				
				mov DefaultREPPrefix, 0;	Default REP Prefixes  = (REP/REPNE)			
			.endif
			invoke DecodeInstruction,hWin				; Decode the instruction
			;-------------------------------------
		.elseif eax >= 1008 && eax <= 1013 				; Segment Prefix' CommandButtons
			invoke EnableButton, eax, 1008, 1013, hWin
			sub eax, 1008								; Store the SegmentRegister Index
			mov SegRegIndex, eax						; For Later Decoding
			invoke DecodeInstruction,hWin				; Decode the instruction
			;-------------------------------------
		.elseif eax >= 1016 && eax <= 1017 				; Operand-Size Prefix' CommandButtons
			invoke EnableButton, eax, 1016, 1017, hWin
			sub eax, 1016								; Store the OperandSize Index
			mov OperandSizeIndex, eax					; For Later Decoding
			invoke DecodeInstruction,hWin				; Decode the instruction			
			;-------------------------------------
		.elseif eax >= 1020 && eax <= 1021 				; Address-Size Prefix' CommandButtons
			invoke EnableButton, eax, 1020, 1021, hWin
			sub eax, 1020								; Store the Address-Size Index
			mov AddressSizeIndex, eax					; For Later decoding
			invoke DecodeInstruction,hWin				; Decode the instruction						
			;-------------------------------------
		.elseif eax >= 1024 && eax <= 1026 				; REPeat Prefixes' CommandButtons
			invoke EnableButton, eax, 1024, 1026, hWin
			sub eax, 1024								; Store the REP Prefix Index
			mov REPPrefixIndex, eax						; For Later Decoding
			.If eax==2				   					; Prefix = (N/A)
				mov DefaultREPPrefix, 2					; No REP Prefix (N/A)
			.endif
			invoke DecodeInstruction,hWin				; Decode the instruction
			;-------------------------------------
		.endif				
	.else
		mov		eax,FALSE
		ret
	.endif
	mov		eax,TRUE
	ret

PracticeProc endp