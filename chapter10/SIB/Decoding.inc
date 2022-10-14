.data?
Prefix66	dd	?		; OperandSize Prefix (66h) --> Default == 0

;-------- Prefix ------
Prefix		dd	?		; Prefix (whether == '66:' or NULL)
OperandSize	dd	?		; OperandSize (e.g. DWORD/WORD/BYTE)

;------- CODE ---------
BitD		dd	?		; Bit(d) --> Default == 1
BitW		dd	?		; Bit(w) --> Default == 1
Inst_HEX	dd	?		; Code HEX Value
Inst_Index	dd	?		; Index Of Instruction (e.g. 0==ADD/1==OR/2==SUB/etc..)
Inst_Name	dd	?		; Instruction Name (e.g. 'ADD'/'OR'/etc..)

;------ ModRM ---------
ModRM			dd	?		; ModRM HEX Value

ModRM_MOD		dd	?		; Mode Value (2-Bits == 2^2 Values = 4 Values)
ModRM_CODE_REG	dd	?		; Code/Reg Value (3-Bits == 2^3 Values = 8 Values)
ModRM_REG_MEM	dd	?		; Reg/Mem  Value (3-Bits == 2^3 Values = 8 Values)

ModRM_REG1		dd	?		; <reg1> Name (e.g. eax)
ModRM_REG2		dd	?		; <reg2> Name (e.g. ebx)

szModRM			db	30 dup (?)	; Buffer To Hold The ModRM Decoding (e.g. 'EAX, DWORD PTR [SIB+12]')

;------ SIB -------------
SIB				dd	?		; SIB HEX Value

SIB_SCALE		dd	?		; Scale Value (2-Bits)
SIB_INDEX		dd	?		; Index Value (3-Bits) (e.g. 000b)
SIB_BASE		dd	?		; Base  Value (3-Bits) (e.g. 000b)

SIB_IndexReg	dd	?		; Index Register (e.g. 'EAX')
SIB_BaseReg		dd	?		; Base  Register (e.g. 'EAX')

szSIB			db	30 dup (?)	; Buffer To Hold The SIB Decoding (e.g. 'EAX+ECX*8')

;------- DISPLACEMENT ---

EnableDISP		dd	?		; Flag --> Enable Displacement Buttons?
Displacement	dd	?		; Displacemetnt HEX Value

.data

;---------------------------------------------- Decoding Formats ------
szPlusSign	db	"+", 0
szMinusSign	db	"-", 0

szPrefix66	db	"66:",0
szNull		db	0

szHexDecode_1	db	" %s%.2X:%.2X",0				; ModRM --> 11b
szHexDecode_2	db	" %s%.2X:%.2X:%.8X",0			; ModRM --> 00b & ModRM_REG_MEM --> 101b
szHexDecode_3	db	" %s%.2X:%.2X:%.2X:%.2X",0		; ModRM --> 01b & ModRM_REG_MEM --> 100b
szHexDecode_4	db	" %s%.2X:%.2X:%.2X",0			; ModRM --> 01b 
szHexDecode_5	db	" %s%.2X:%.2X:%.2X:%.8X",0		; ModRM --> 10b & ModRM_REG_MEM --> 100b 

;------------------- ModRM (Mode 11) ----------------------------------
;											(e.g. ADD EAX, EAX) 
szModRM_11		db	" %s	%s, %s",0	
					
;------------------- ModRM (Mode 00) ----------------------------------
;												(ModRM_REG_MEM = 101b)
; 											(e.g. ADD EAX, DWORD PTR [1234567])
szModRM_00_1	db	" %s	%s, %s PTR [%.8X]",0	
; 											(e.g. ADD DWORD PTR [1234567], EAX)
szModRM_00_2	db	" %s	%s PTR [%.8X], %s",0	
; 											(e.g. ADD EAX, DWORD PTR [EAX])
szModRM_00_3	db	" %s	%s, %s PTR [%s]",0		
; 											(e.g. ADD DWORD PTR [EAX], EAX)
szModRM_00_4	db	" %s	%s PTR [%s], %s",0		
; 											(e.g. ADD EAX, DWORD PTR [ecx+edx*4])
szModRM_00_5	db	" %s	%s, %s PTR [%s]",0		
; 											(e.g. ADD DWORD PTR [SIB], EAX)
szModRM_00_6	db	" %s	%s PTR [%s], %s",0		

;------------------- ModRM (Mode 01) ----------------------------------
;												(ModRM_REG_MEM = 100b)
; 											(e.g. ADD EAX, DWORD PTR [SIB+[2])
szModRM_01_1	db	" %s	%s, %s PTR [%s%s%.2X]",0	
; 											(e.g. ADD DWORD PTR [SIB+12], EAX)
szModRM_01_2	db	" %s	%s PTR [%s%s%.2X], %s",0	
; 											(e.g. ADD EAX, DWORD PTR [esi+(BYTE) DISPLACEMENT])
szModRM_01_3	db	" %s	%s, %s PTR [%s+%.2X]",0	
; 											(e.g. ADD EAX, DWORD PTR [esi+(BYTE) DISPLACEMENT])
szModRM_01_4	db	" %s	%s PTR [%s+%.2X], %s",0	

;------------------- ModRM (Mode 11) ----------------------------------
;												(ModRM_REG_MEM = 100b)
; 											(e.g. ADD EAX, DWORD PTR [SIB])
szModRM_10_1	db	" %s	%s, %s PTR [%s+%.8X]",0	
; 											(e.g. ADD EAX, DWORD PTR [SIB])
szModRM_10_2	db	" %s	%s PTR [%s+%.8X], %s",0	
; 											(e.g. ADD EAX, DWORD PTR [esi+(DWORD) DISPLACEMENT])
szModRM_10_3	db	" %s	%s, %s PTR [%s+%.8X]",0	
; 											(e.g. ADD EAX, DWORD PTR [esi+(DWORD) DISPLACEMENT])
szModRM_10_4	db	" %s	%s PTR [%s+%.8X], %s",0	

;------------------- SIB (ModRM Mode == 00 -----------------------------
;								(ModRM Mode == 00b && Reg/Mem == 100b)
;											(e.g. 'EAX+EBX*4)
szSIB_00_100	db	"%s",0
;								(ModRM Mode == 00b && Reg/Mem <> 100b)
szSIB_00		db	"%s+%s*%d",0
;								(ModRM Mode == 00b && Reg/Mem <> 100b && Scale == 0)
szSIB_00_2		db	"%s+%s",0
		
;----------------------------------- Instructions Values --------------

_ADD		db	"ADD",0
_OR			db	"OR",0
_SUB		db	"SUB",0
_AND		db	"AND",0
_MOV		db	"MOV",0
_XOR		db	"XOR",0

_ADD_HEX	dd	00h		;	03h		(Commented hex values are with bit(d) == 1 && Bit(w) == 1)
_OR_HEX		dd	08h		;	09h
_SUB_HEX	dd	28h		;	2Bh
_AND_HEX	dd	20h		;	21h
_MOV_HEX	dd	88h 	;	8Bh
_XOR_HEX	dd	30h 	; 	33h

Inst_Table	dd	_ADD, _ADD_HEX,
				_OR,  _OR_HEX ,
				_SUB, _SUB_HEX,
				_AND, _AND_HEX,
				_MOV, _MOV_HEX,
				_XOR, _XOR_HEX


;-------------------- Registers ---------------------------------

;--------- 32-Bit ----------------
_EAX			db	"EAX",0
_ECX			db	"ECX",0
_EDX			db	"EDX",0
_EBX			db	"EBX",0		
_ESP			db	"ESP",0
_EBP			db	"EBP",0		
_ESI			db	"ESI",0	
_EDI			db	"EDI",0	

REG32			dd	_EAX,	000b,
				 	_ECX,	001b,
					_EDX,	010b,
					_EBX,	011b,
					_ESP,	100b,
					_EBP,	101b,
					_ESI,	110b,
					_EDI,	111b
		
;--------- 16-Bit ----------------
_AX				db	"AX",0
_CX				db	"CX",0
_DX				db	"DX",0
_BX				db	"BX",0		
_SP				db	"SP",0
_BP				db	"BP",0		
_SI				db	"SI",0	
_DI				db	"DI",0	

REG16			dd	_AX,	000b,
					_CX,	001b,
					_DX,	010b,
					_BX,	011b,
					_SP,	100b,
					_BP,	101b,
					_SI,	110b,
					_DI,	111b
									
;--------- 8-Bit ----------------
_AL				db	"AL",0
_CL				db	"CL",0
_DL				db	"DL",0
_BL				db	"BL",0		
_AH				db	"AH",0
_CH				db	"CH",0		
_DH				db	"DH",0	
_BH				db	"BH",0	

REG8			dd	_AL,	000b,
					_CL,	001b,
					_DL,	010b,
					_BL,	011b,
					_AH,	100b,
					_CH,	101b,
					_DH,	110b,
					_BH,	111b

;-------------------------- Operand-Size ----------------------

_DWORD			db	"DWORD",0
_WORD			db	"WORD",0
_BYTE			db	"BYTE",0


.code

DecodeReg proc RegToDecode:DWORD, RegToSave:DWORD, RegTable:DWORD
;-----------------------------------------------------------------------------------
;	Decodes 3-Bits (Reg) in the ModRM Byte.
;
;	Pameters: 
;				(RegToDecode)	A DWORD wut register to decode
;				(RegToSave) 	A DWORD where to save the decoded reg's name (e.g. eax)
;
;				(RegTable)		A Pointer to the registers table to be used
;				
;				 RegTable = 0  --> 32 Bit registers
;				 RegTable = 1  --> 16 Bit registers
;				 RegTable = 2  --> 8  Bit registers
;-----------------------------------------------------------------------------------

;-----------------------------------------------------------------------------------
; NOTE: We could use a simple procedure that uses (mov eax, RegTable[Inst_Index*4]]
; But I decided to make it more -realistic- procedure, as in a real disassembler
; we won't have such thing like Inst_Index.
;-----------------------------------------------------------------------------------
	
	mov eax, RegToDecode
	xor ecx, ecx						
@@:	
	mov esi, RegTable
	lea esi, [esi+4+ecx*8]
	cmp [esi], eax
	
	.if ZERO?							; Register Found
		mov eax, [esi-4]
		mov ebx, RegToSave
		mov [ebx], eax					; Save its name (e.g. EAX/AX/AL/etc...)
		ret
	.endif
	
	inc ecx
	cmp ecx, 8							; Eight Registers To Search
	jb @B
	
	ret
DecodeReg endp

DecodeSIB proc SIBValue:DWORD
;-----------------------------------------------------------------------------------
;					As the name says, it decodes an SIB Byte =)
;-----------------------------------------------------------------------------------	
	xor ebx, ebx

	;------------------------------------ Extract Bit Fields (2:3:3) ---------------
	mov eax, SIB
	mov ecx, eax
	and cl, 11000000b					; Extract (Scale ----> 2-Bits)	
	rol cl, 2
	mov SIB_SCALE, ecx
	mov ecx, eax
	and cl, 00111000b					; Extract (Index ---> 3-Bits)
	ror cl, 3
	mov SIB_INDEX, ecx	
	mov ecx, eax
	and cl, 00000111b					; Extract (Base  ---> 3-Bits)	
	mov SIB_BASE, ecx	
	
	invoke DecodeReg, SIB_INDEX, addr SIB_IndexReg, addr REG32	; Decode Index Register
	invoke DecodeReg, SIB_BASE, addr SIB_BaseReg,   addr REG32	; Decode Base Register
	
	; -----------------  Get Scale (2^Value) ---------
	mov ecx, SIB_SCALE
	mov eax, 2
	
	.if ecx==0
		xor eax, eax
	.else
		.while ecx > 1
			shl eax, 1	
			dec ecx
		.endw
	.endif
	
	.if SIB_INDEX == 100b		;  Index Register Is Ignored & Therefore Scale Is Also Ignored
		invoke wsprintf, addr szSIB,
		addr szSIB_00_100,
		SIB_BaseReg
	.else
		.if SIB_SCALE == 0
			lea ecx, szSIB_00_2		; No Scale
		.else
			lea ecx, szSIB_00
		.endif

		invoke wsprintf, addr szSIB, 
		ecx, 
		SIB_BaseReg,		; Index Register (e.g. 'EAX')
		SIB_IndexReg,		; Base  Register (e.g. 'EBX')
		eax	
	.endif
	
	
	ret
DecodeSIB endp

DecodeModRM proc ModRMValue:DWORD
;-----------------------------------------------------------------------------------
;					As the name says, it decodes a ModRM Byte =)
;-----------------------------------------------------------------------------------
	LOCAL RegTable:DWORD
	;------------------------------------ Extract Bit Fields (2:3:3) ---------------
	mov eax, ModRMValue
	mov ecx, eax
	and cl, 11000000b					; Extract (Mode -------> 2-Bits)	
	rol cl, 2
	mov ModRM_MOD, ecx
	mov ecx, eax
	and cl, 00111000b					; Extract (Code/Reg ---> 3-Bits)
	ror cl, 3
	mov ModRM_CODE_REG, ecx	
	mov ecx, eax
	and cl, 00000111b					; Extract (Reg/Mem  ---> 3-Bits)	
	mov ModRM_REG_MEM, ecx	
	
	mov eax, BitW
	.if eax == 1					; FullSize Registers (32/16 Bits)
		mov eax, Prefix66
		.if eax == 0				; 32-Bit Registers
			lea eax, REG32
			lea ebx, _DWORD
		.else						; 16-Bit Registers
			lea eax, REG16
			lea ebx, _WORD
		.endif
	.else							; PartialSize Registers (8-Bits)
		lea eax, REG8
		lea ebx, _BYTE
	.endif
		
	mov RegTable, eax
	mov OperandSize, ebx	

	.if Prefix66 == 1			; 16-Bit (Put the prefix)
		lea eax, szPrefix66
		mov Prefix, eax
	.else
		lea eax, szNull
		mov Prefix, eax
	.endif
	
	;------------------------------ Addressing Mode -------
	mov eax, ModRM_MOD
	;------------------------------------------------------	
	.if eax == 11b

	invoke GetDlgItem, hWnd, IDC_DISPLACEMENT
	invoke EnableWindow,eax, FALSE
	invoke EnableDisplacement, 0
	invoke EnableSIB,0
	mov EnableDISP, 1

	;-------------------------------------------------------------
	;	Addressing Mode (11b) : reg, reg
	;-------------------------------------------------------------
		mov eax, BitD
		mov ebx, Inst_Index
		.if ebx==1||ebx==3							; AND/OR (Direction flag reversed)
			mov ebx, 0								; D==0 (reg1, reg2)
		.else										; Other Instructions 
			mov ebx, 1								; D==1 (reg1, reg2)
		.endif
			
		.if eax==ebx										
			invoke DecodeReg,ModRM_CODE_REG, addr ModRM_REG1, RegTable	; <Src == reg1>
			invoke DecodeReg,ModRM_REG_MEM,  addr ModRM_REG2,  RegTable	; <Dst == reg2>
		.else
			invoke DecodeReg,ModRM_CODE_REG, addr ModRM_REG2, RegTable	; <Src == reg2>	
			invoke DecodeReg,ModRM_REG_MEM,  addr ModRM_REG1,  RegTable	; <Dst == reg1>						
		.endif
		
		;----------------------------- Display The Decoding
				
		invoke wsprintf, addr szBuffer, addr szHexDecode_1, \
		Prefix,			; Prefix? 
		Inst_HEX, \
		ModRM
				
		invoke SetDlgItemText, hWnd, IDC_HEX_DECODE, addr szBuffer
		
		invoke wsprintf, addr szBuffer, addr szModRM_11, \
		Inst_Name,			
		ModRM_REG1, \
		ModRM_REG2
				
		invoke SetDlgItemText, hWnd, IDC_MNE_DECODE, addr szBuffer
		
		ret
	.endif		
	;-------------------------------------------------------------
	;	Addressing Mode (00b) : reg, [reg/mem]
	;-------------------------------------------------------------	
	.if eax == 00b
			invoke EnableSIB, 0
		.if ModRM_REG_MEM == 101b			; reg, mem
			;----------------- (1-DWORD) displacement so enable 32-Bits ---------
			.if EnableDISP==1
				invoke EnableDisplacement, 2
				mov EnableDISP, 0
			.endif
			;-------------------------------------
				invoke BitsToByte,IDC_DISP_BIT_00, IDC_DISP_BIT_31, 0	; Get Displacement in eax
				mov Displacement, eax									; Save it.
			;------------------------------------- Display Decoding -----	
				invoke wsprintf, addr szBuffer, addr szHexDecode_2, \
				Prefix,			; Prefix? 
				Inst_HEX, \
				ModRM, \
				Displacement
				
				invoke SetDlgItemText, hWnd, IDC_HEX_DECODE, addr szBuffer				

			;------------------------------------- (Reverse displacement for decoding)
			;	e.g.  12345678 -->  MOV EAX, 78563412
				mov eax, Displacement		; e.g    eax == 12345678
;				(No need for (xor ecx, ecx) since all bits are overriden))
				mov cl, al					; 		 ecx == 00000078
				rol ecx, 8					;		 ecx == 00007800
				mov cl, ah					;		 ecx == 00007856
				rol ecx, 8					;		 ecx == 00785600
				rol eax, 16					;		 eax == 56781234
				mov cl, al					;		 ecx == 00785634
				rol ecx, 8					;		 ecx == 78563400
				mov cl, ah					;		 ecx == 78563412
				mov Displacement, ecx								
			;------------------------------------- Display Decoding -----	


				invoke DecodeReg,ModRM_CODE_REG, addr ModRM_REG1, RegTable	; <Src == code/reg>
											
				.if BitD==1
					invoke wsprintf, addr szBuffer, addr szModRM_00_1, \
					Inst_Name,			
					ModRM_REG1, \
					OperandSize, \
					Displacement
				.else
					invoke wsprintf, addr szBuffer, addr szModRM_00_2, \
					Inst_Name,			
					OperandSize, \
					Displacement, \
					ModRM_REG1
				.endif	
						
				invoke SetDlgItemText, hWnd, IDC_MNE_DECODE, addr szBuffer
			;-------------------------------------
		.elseif ModRM_REG_MEM == 100b			; reg, SIB

			.if EnableDISP==0
				invoke EnableDisplacement, 0
				mov EnableDISP, 1
			.endif
			;----------------------------
			invoke wsprintf, addr szBuffer, addr szHexDecode_3, \
			Prefix,			; Prefix? 
			Inst_HEX, \
			ModRM, \
			SIB
				
			invoke SetDlgItemText, hWnd, IDC_HEX_DECODE, addr szBuffer	
						
			invoke DecodeReg,ModRM_CODE_REG, addr ModRM_REG1, RegTable	; <Src == code/reg>
			invoke EnableSIB, 1
			invoke DecodeSIB, SIB
										
			.if BitD==1
				invoke wsprintf, addr szBuffer, addr szModRM_00_5, \
				Inst_Name,			
				ModRM_REG1, \
				OperandSize, \
				addr szSIB
			.else
				invoke wsprintf, addr szBuffer, addr szModRM_00_6, \
				Inst_Name,			
				OperandSize,\
				addr szSIB, \
				ModRM_REG1
			.endif	
						
						
			invoke SetDlgItemText, hWnd, IDC_MNE_DECODE, addr szBuffer	
			
		.else
		
			.if EnableDISP==0
				invoke EnableDisplacement, 0
				mov EnableDISP, 1
			.endif
		
			;------------------------------------- Display Decoding -----	
			invoke wsprintf, addr szBuffer, addr szHexDecode_1, \
			Prefix,	\		; Prefix? 
			Inst_HEX, \
			ModRM
		
			invoke SetDlgItemText, hWnd, IDC_HEX_DECODE, addr szBuffer
			
			mov eax, BitD
			mov ebx, Inst_Index
			.if ebx==1||ebx==3							; AND/OR (Direction flag reversed)
				mov ebx, 0								; D==0 (reg1, reg2)
			.else										; Other Instructions 
				mov ebx, 1								; D==1 (reg1, reg2)
			.endif
				
			.if eax==ebx										
				invoke DecodeReg,ModRM_CODE_REG, addr ModRM_REG1, RegTable		; <Src == reg1>
				invoke DecodeReg,ModRM_REG_MEM,  addr ModRM_REG2,  addr REG32	; <Dst == reg2>
			.else
				invoke DecodeReg,ModRM_CODE_REG, addr ModRM_REG2, RegTable		; <Src == reg2>	
				invoke DecodeReg,ModRM_REG_MEM,  addr ModRM_REG1,  addr	REG32	; <Dst == reg1>						
			.endif
			
			.if BitD==1
				invoke wsprintf, addr szBuffer, addr szModRM_00_3, \
				Inst_Name,			
				ModRM_REG1,	\
				OperandSize,\
				ModRM_REG2
			.else
				invoke wsprintf, addr szBuffer, addr szModRM_00_4, \
				Inst_Name,			
				OperandSize,	\
				ModRM_REG1,\
				ModRM_REG2				
			.endif
			
				
			invoke SetDlgItemText, hWnd, IDC_MNE_DECODE, addr szBuffer
				
		.endif
		ret
	.endif
	;-------------------------------------------------------------
	;	Addressing Mode (01b) : reg, [reg+(BYTE)DISP]
	;-------------------------------------------------------------	
	
	.if eax==01b
		invoke EnableSIB, 0
		;-------------------------------------
		invoke BitsToByte,IDC_DISP_BIT_24, IDC_DISP_BIT_31, 0	; Get Displacement in eax
		mov Displacement, eax									; Save it.
		;-------------------------------------

		.if ModRM_REG_MEM == 100b				
			;--------------- SIB + (1-BYTE) ---------------------------------------
			invoke EnableDisplacement, 1
			mov EnableDISP, 1
			;---------------------
			invoke wsprintf, addr szBuffer, addr szHexDecode_3, \
			Prefix,	\	; Prefix? 
			Inst_HEX, \
			ModRM, \
			SIB, \
			Displacement

			invoke SetDlgItemText, hWnd, IDC_HEX_DECODE, addr szBuffer				
			invoke DecodeReg,ModRM_CODE_REG, addr ModRM_REG1, RegTable	; <Src == reg1>
			invoke EnableSIB, 1
			invoke DecodeSIB, SIB

			.if Displacement > 7Fh										; (BYTE) DISP > +127 
				mov eax, 0FFh
				sub eax, Displacement
				inc eax
				mov Displacement, eax									
				lea eax, szMinusSign
			.else
				lea eax, szPlusSign
			.endif
										
			.if BitD==1
				invoke wsprintf, addr szBuffer, addr szModRM_01_1, \
				Inst_Name,			
				ModRM_REG1, \
				OperandSize, \
				addr szSIB,\
				eax,\
				Displacement
			.else
				invoke wsprintf, addr szBuffer, addr szModRM_01_2, \
				Inst_Name,			
				OperandSize,\
				addr szSIB, \
				eax,\
				Displacement,\
				ModRM_REG1
			.endif	

			invoke SetDlgItemText, hWnd, IDC_MNE_DECODE, addr szBuffer
				
		.else

			;------------------------------- (1-BYTE) Displacement | No SIB Byte
				
			invoke wsprintf, addr szBuffer, addr szHexDecode_4, \
			Prefix,			; Prefix? 
			Inst_HEX, \
			ModRM,\
			Displacement
			
			invoke SetDlgItemText, hWnd, IDC_HEX_DECODE, addr szBuffer				
			
			;----------------------- Displacement is signed? check this -------------
			.if Displacement > 7Fh										; (BYTE) DISP > +127 
				mov eax, 0FFh
				sub eax, Displacement
				inc eax
				mov Displacement, eax									

				mov ecx, sizeof szModRM_01_3
				mov al, '+'
				lea edi, szModRM_01_3
				repne scasb
				test ecx, ecx
				.if !ZERO?
					mov byte ptr [edi-1], '-'
				.endif
			.else
				mov ecx, sizeof szModRM_01_3
				mov al, '-'
				lea edi, szModRM_01_3
				repne scasb
				test ecx, ecx
				.if !ZERO?
					mov byte ptr [edi-1], '+'
				.endif
			.endif
								
			invoke DecodeReg,ModRM_CODE_REG, addr ModRM_REG1, RegTable		; <Src == code/reg>
			invoke DecodeReg,ModRM_REG_MEM, addr ModRM_REG2, addr REG32		; <Dst == reg/mem>

	
			.if BitD==1
				invoke wsprintf, addr szBuffer, addr szModRM_01_3, \
				Inst_Name,			
				ModRM_REG1,	\
				OperandSize,\
				ModRM_REG2,\
				Displacement
			.else
				invoke wsprintf, addr szBuffer, addr szModRM_01_4, \
				Inst_Name,			
				OperandSize,\
				ModRM_REG2,\
				Displacement,\
				ModRM_REG1				
			.endif
				
			invoke SetDlgItemText, hWnd, IDC_MNE_DECODE, addr szBuffer											
		.endif
	.endif
			
	;-------------------------------------------------------------
	;	Addressing Mode (10b) : reg, [reg+(DWORD)DISP]
	;-------------------------------------------------------------	
	
	.if eax==10b
		invoke EnableDisplacement, 2
		
		;-------------------------------------
		invoke BitsToByte,IDC_DISP_BIT_00, IDC_DISP_BIT_31, 0	; Get Displacement in eax
		mov Displacement, eax		; Save it.

		.if ModRM_REG_MEM == 100b				
			;--------------- (1-DWORD) SIB ---------------------------------------

			invoke wsprintf, addr szBuffer, addr szHexDecode_5, \
			Prefix,	\	; Prefix? 
			Inst_HEX, \
			ModRM,\
			SIB,\
			Displacement

			invoke SetDlgItemText, hWnd, IDC_HEX_DECODE, addr szBuffer				
			invoke DecodeReg,ModRM_CODE_REG, addr ModRM_REG1, RegTable	; <Src == reg1>

			invoke EnableSIB, 1
			invoke DecodeSIB, SIB

			;------------------------------------- (Reverse displacement for decoding)\
			mov eax, Displacement
			;	e.g.  12345678 -->  MOV EAX, 78563412
	;		(No need for (xor ecx, ecx) since all bits are overriden))
			mov cl, al					; 		 ecx == 00000078
			rol ecx, 8					;		 ecx == 00007800
			mov cl, ah					;		 ecx == 00007856
			rol ecx, 8					;		 ecx == 00785600
			rol eax, 16					;		 eax == 56781234
			mov cl, al					;		 ecx == 00785634
			rol ecx, 8					;		 ecx == 78563400
			mov cl, ah					;		 ecx == 78563412	
			mov Displacement, ecx		; Save it.
														
			.if BitD==1
				invoke wsprintf, addr szBuffer, addr szModRM_10_1, \
				Inst_Name,			
				ModRM_REG1, \
				OperandSize, \
				addr szSIB,\
				Displacement
			.else
				invoke wsprintf, addr szBuffer, addr szModRM_10_2, \
				Inst_Name,			
				OperandSize,\
				addr szSIB,
				Displacement,\
				ModRM_REG1
			.endif	
			
			invoke SetDlgItemText, hWnd, IDC_MNE_DECODE, addr szBuffer
		
			ret
		.endif
			;--------------- (1-DWORD) Displacement -----------------------------------

		invoke DecodeReg,ModRM_CODE_REG, addr ModRM_REG1, RegTable		; <Src == code/reg>
		invoke DecodeReg,ModRM_REG_MEM,  addr ModRM_REG2, addr REG32	; <Dst == reg/mem>
			
		;------------------------------------- Display Decoding -----	
		invoke wsprintf, addr szBuffer, addr szHexDecode_2, \
		Prefix,			; Prefix? 
		Inst_HEX, \
		ModRM, \
		Displacement
		
		invoke SetDlgItemText, hWnd, IDC_HEX_DECODE, addr szBuffer				

			;------------------------------------- (Reverse displacement for decoding)\
			mov eax, Displacement
			;	e.g.  12345678 -->  MOV EAX, 78563412
	;		(No need for (xor ecx, ecx) since all bits are overriden))
			mov cl, al					; 		 ecx == 00000078
			rol ecx, 8					;		 ecx == 00007800
			mov cl, ah					;		 ecx == 00007856
			rol ecx, 8					;		 ecx == 00785600
			rol eax, 16					;		 eax == 56781234
			mov cl, al					;		 ecx == 00785634
			rol ecx, 8					;		 ecx == 78563400
			mov cl, ah					;		 ecx == 78563412	
			mov Displacement, ecx		; Save it.
									
		.if BitD==1
			invoke wsprintf, addr szBuffer, addr szModRM_10_3, \
			Inst_Name,			
			ModRM_REG1, \
			OperandSize, \
			ModRM_REG2,\
			Displacement
		.else
			invoke wsprintf, addr szBuffer, addr szModRM_10_4, \
			Inst_Name,			
			OperandSize, \
			ModRM_REG2, \
			Displacement, \
			ModRM_REG1
		.endif	
						
			invoke SetDlgItemText, hWnd, IDC_MNE_DECODE, addr szBuffer
			
			;---------------------			
		.endif
		ret
		
	
	
DecodeModRM endp

UpdateValues proc
;---------------------------------------------------------------------------
;	Update some decoding values (e.g. BitD & BitW & Instruction & etc...)
;---------------------------------------------------------------------------
	invoke GetDlgItemInt, hWnd, IDC_BIT_D, 0,0					; Save Bit(d)
	mov BitD, eax
	invoke GetDlgItemInt, hWnd, IDC_BIT_W, 0,0					; Save Bit(d)
	mov BitW, eax	
	invoke BitsToByte, IDC_MODRM_7, IDC_MODRM_0, IDC_MODRM_DEC	; Save & Display ModRM Byte
	mov ModRM, eax
	invoke BitsToByte, IDC_SIB_7, IDC_SIB_0, IDC_SIB_DEC		; Save & Display SIB Byte
	mov SIB, eax
	;------------------------------------------ Display Decoding ------------

	;----------------------------------------------------------- Save & Display Code Byte
	mov eax, Inst_Index
	lea esi, Inst_Table[8*eax]									; Get Instruction Name (e.g. 'OR')
	mov eax, [esi]
	mov Inst_Name, eax											; Save it.
	mov eax, [esi+4]
	mov eax, [eax]
	mov Inst_HEX, eax											; Get Instruction Hex Value -----\
																; But with no Bit(d) or Bit(w)<--|
															
	add eax, BitW												; 
	ror al, 1													; 
	add eax, BitD												;
	rol al, 1													; eax == xxxxxx(d)(w)
	mov Inst_HEX, eax											; Save Instruction Hex Value
	; ----- Display It ----
	invoke wsprintf, addr szBuffer, addr ToHex, eax
	invoke SetDlgItemText,hWnd, IDC_INST_DEC, addr szBuffer		; Display the new hex value
	
	;----------------------------------------------------------- Decode ModRM Byte 
	invoke DecodeModRM, ModRM
	ret
UpdateValues endp

