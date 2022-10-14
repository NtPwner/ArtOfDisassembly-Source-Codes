;===========================================
;		Whole engine is under TESTING
;===========================================


.data
;===== ModRM 32-Bit ===========
szModRM_Dsp32		db		"XXXXX PTR [XXXXXXXX]", 0			;	e.g. ( BYTE PTR [12345678] )
szModRM_Ptr			db		"XXXXX PTR [XXX]", 0				;	e.g. ( BYTE PTR [EDI] )
szModRM_Ptr_8		db		"XXXXX PTR [XXX+XX]",0				;	e.g. ( BYTE PTR [EDI+12] )
szModRM_Ptr_32		db		"XXXXX PTR [XXX+XXXXXXXX]",0		;	e.g. ( BYTE PTR [EDI+12] )

;===== ModRM 16-Bit ===========
szModRM16_Ptr		db		"XXXXX PTR [XXXXX]",0				;	e.g. (WORD PTR [BX+SI])
szModRM16_Ptr_8		db		"XXXXX PTR [XXXXX+XX]",0			;	e.g. (WORD PTR [BX+SI+11])
szModRM16_Ptr_16	db		"XXXXX PTR [XXXXX+XX]",0			;	e.g. (WORD PTR [BX+SI+11])

szModRM_SIB			db		"XXXXX PTR [XXXXXXXXX]",0			;	e.g. ( DWORD PTR [EBX+EDX*4] )
szSIB_DISP8			db		"XXXXX PTR [XXXXXXXXX+XX]",0		;	e.g. ( DWORD PTR [EBX+EDX*4+AB] )
szSIB_DISP32		db		"XXXXX PTR [XXXXXXXXX+XXXXXXXX]",0	;	e.g. ( DWORD PTR [EBX+EDX*4+ABCDDCBA] )

szSIB_Only_Base 	db		"XXX",0								; 	e.g. ( EAX )
szSIB_No_Scale		db		"XXX+XXX",0							; 	e.g. ( EDX+ECX )
szSIB_All			db		"XXX+XXX*X",0						;	e.g. ( EDX+ECX*4 )

;szHEX32				db		"+%0.8lX",0							;  	e.g. ( 12345678h )
szHEX16				db		"+%0.4lX",0							;  	e.g. ( 1234h )
szHEX8				db		"+%0.2lX",0							;  	e.g. ( 12h )


szInstruction		db		"XXXXXXXXXXXX",0

szDblSpace			db		"  ",0
szCommaSpace		db		", ",0

dwListItem			dd		0


.data?

externdef hThread:DWORD                        
VirtualAddress	dd			?					;  Virtual Address Of current Byte (e.g. 00401002)

Prefixes 		tPREFIXES	 	<>				;  Define (Prefixes) As a (tPREFIXES) Struct
Segments		tSEGMENTS		<>				;  Define (Segments) As a (tSEGMENTS) Struct
Instruction		tINSTRUCTION  	<>				;  Define (Instruction) As a (tINSTRUCTION) Struct
Disassembly 	tDISASSEMBLY  	<>				;  Define (Disassembly) As a (tDISASSEMBLY) Struct
ModRM			tMODRM		 	<>				;  Define (ModRM) As a (tMODRM) Struct
SIB				tSIB		 	<>				;  Define (SIB) As a (tSIB) Struct
Disp32			tDISP32		 	<>				;  Define (Disp32) As a (tDISP32) Struct
Disp16			tDISP16		 	<>				;  Define (Disp16) As a (tDISP16) Struct
Disp8			tDISP8		 	<>				;  Define (Disp8) As a (tDISP8) Struct

RegTable		dd				?
OperandSize		dd				?

szDISP32		db 10 dup (?)
szDISP16		db 6 dup (?)
szDISP8			db 4 dup (?)
szBuf			db 250 dup (?)

.code

ShowDisassebmly proc
		;============= Just For Testing =======
		invoke dwHex2Str, VirtualAddress, addr szBuf
		invoke InsertItem,dwListItem,0,addr szBuf
		invoke strCat, addr szBuf, addr szInstruction, 0
		invoke strCat, eax, addr szDblSpace, 0
		invoke strCat, eax, esi, 0	
		mov byte ptr [eax], 0
		.if edi != 0
			invoke strCat, eax, addr szCommaSpace, 0
			invoke strCat, eax, edi, 1			
		.endif	
		invoke InsertItem,dwListItem,1,addr InstructionHex
		invoke InsertItem,dwListItem,2,addr szBuf
		inc dwListItem
		;======================================		
		ret
ShowDisassebmly endp

GetByte proc
	;===================================
	; Read 1-Byte and return it in (AL)
	;===================================
	xor eax, eax
	mov ebx, [Instruction.ByteOffset]				; Read 1-Byte
	mov al, byte ptr [ebx]
	ret
GetByte endp

GetDisp8 proc  uses ebx ecx edx bSigned:BYTE

	;==================================================
	;	This procedure extracts a  8-Bit Displacement
	;==================================================
	;	IN:	  bSigned == Displacement is signed?
	;	OUT:  ESI == Displacement
	;==================================================
	add Instruction.InstSize, 1 
	add Instruction.ByteOffset, 1
	;-------------------------
	; Convert HEX to String
	;-------------------------
	
	xor eax, eax
	mov ebx, dword ptr [Instruction.ByteOffset]
	mov al, byte ptr [ebx]
	
	;==========================
	;  Write the [CODE] byte
	;==========================
	
	lea ebx, [HexCharsTable+eax*2]
	mov ebx, [ebx]
	mov esi, [lpInstHex]
	mov word ptr [esi], bx
	mov byte ptr [esi+2], 0
	add lpInstHex, 2	

	.if bSigned == 1
		test al, 10000000y
		.if ZERO?
			mov byte ptr [szHEX8], '+'
		.else
			neg al
			mov byte ptr [szHEX8], '-'
		.endif
	.else
		mov byte ptr [szHEX8], '+'
	.endif
	
	invoke wsprintf, addr szDISP8, addr szHEX8, eax
	lea esi, szDISP8
	
		;==========================
		;  Write the [CODE] byte
		;==========================
		
		
	ret
GetDisp8 endp

GetDisp16 proc uses ebx ecx bSigned:BYTE
	;==================================================
	;	This procedure extracts a 16-Bit Displacement
	;==================================================
	;	IN:	  bSigned == Displacement is signed?
	;	OUT:  ESI == Displacement
	;==================================================


	mov ebx, dword ptr [Instruction.ByteOffset]
	add ebx, 1
	
	mov eax, lpInstHex
	mov byte ptr [eax+5],0
	invoke LongHex2Str, ebx, 2, eax
	add lpInstHex, 5	

	xor eax, eax	
	mov ax, [ebx]

	add Instruction.InstSize, 2
	add Instruction.ByteOffset, 2
	;-------------------------
	; Convert HEX to String
	;-------------------------
	
	.if bSigned == 1 && eax >= -3F00h
		test ax, 1000000000000000y
		.if ZERO?
			mov byte ptr [szHEX16], '+'
		.else
			neg ax
			mov byte ptr [szHEX16], '-'
		.endif
	.else
		mov byte ptr [szHEX16], '+'
	.endif
	
	invoke wsprintf, addr szDISP16, addr szHEX16, eax
	lea esi, szDISP16
		
	ret
GetDisp16 endp

GetDisp32 proc uses ebx eax ecx bSigned:BYTE ; Signed/Unsigned needs checking
	;==================================================
	;	This procedure extracts a 32-Bit Displacement
	;==================================================
	;	IN:	  bSigned == Displacement is signed?
	;	OUT:  ESI == Displacement
	;==================================================

	mov eax, dword ptr [Instruction.ByteOffset]
	add eax, 1

	
	mov ebx, lpInstHex
	mov byte ptr [ebx+10],0
	invoke LongHex2Str, eax, 4, ebx
	add lpInstHex, 9

	mov eax, [eax]
			
	add Instruction.InstSize, 4
	add Instruction.ByteOffset, 4
		
	;-------------------------
	; Convert HEX to String
	;-------------------------
	lea esi, szDISP32
	
	.if bSigned == 1 && eax >= -3F00h
		test eax, 1000000000000000000000000000000y
		.if ZERO?
			mov byte ptr [esi], '+'
		.else
			neg eax
			mov byte ptr [esi], '-'
		.endif
	.else
		mov byte ptr [esi], '+'
	.endif
	
	add esi, 1
	invoke dwHex2Str, eax, esi	
	sub esi, 1
				
		
	ret
GetDisp32 endp

DecodeSIB proc
	;=======================================
	;	This procedure decodes an SIB Byte
	;=======================================
	;	OUT:  EDI == SIB (ASCII String)
	;	OUT:  ECX == Length of SIB' String
	;=======================================
		
	add Instruction.InstSize, 1
	add Instruction.ByteOffset, 1
	
	;======= Read SIB Byte =====
	invoke GetByte
	;===========================
	
	;===========================
	; SIB:		SS:III:BBB
	;===========================	
	xor ecx, ecx					; 
	
	mov cl, al						;
	and cl, 11000000y				;  Extract 2-MSB(s) (Scale)
	rol cl, 2						;  		cl:  000000:Scale.
	mov SIB.Scale, cl				;
	mov cl, al						;
	and cl, 00111000y				;  Extract 3-Bits (Index)
	ror cl, 3						;		cl: 00000:Index
	mov SIB.Index, cl				;
	mov cl, al						;
	and cl, 00000111y				;  Extract 3-LSB(s) (Base)
									;		cl: 00000:Base
	mov SIB.Base, cl				;
									;
	mov al, SIB.Scale				; AL = (0/1/2/3)
	mov ebx, 2						;
	.while al > 1					;
		shl bl, 1					; bL = (2^0/2^1/2^2/2^3)
		dec al						;
	.endw							;
	add bl, '0'						; AL = ('0'/'2/'4'/'8')
	
	.if SIB.Index == 100b			; ( Only Base Register )
		lea edi, szSIB_Only_Base
		;--------------- Base ------
		mov cl, SIB.Base
		mov eax, [REG32Table+ecx*4]
		mov eax, [eax]
		mov [edi], eax
		mov ecx, sizeof szSIB_Only_Base
	.else
		.if SIB.Scale == 0
			lea edi, szSIB_No_Scale
			;------------ Base --------
			mov cl, SIB.Base
			mov eax, [REG32Table+ecx*4]
			mov eax, [eax]
			mov [edi], eax
			mov byte ptr [edi+3], '+'
			;------------- Index ------
			mov cl, SIB.Index
			mov eax, [REG32Table+ecx*4]
			mov eax, [eax]
			mov [edi+4], eax
			mov ecx, sizeof szSIB_No_Scale
		.else
			lea edi, szSIB_All
			;------------ Base --------
			mov cl, SIB.Base
			mov eax, [REG32Table+ecx*4]
			mov eax, [eax]
			mov [edi], eax
			mov byte ptr [edi+3], '+'
			;------------- Index ------
			mov cl, SIB.Index
			mov eax, [REG32Table+ecx*4]
			mov eax, [eax]
			mov [edi+4], eax
			mov byte ptr [edi+7], '*'
			;------------- Scale ------
			mov byte ptr [edi+8], bl
			mov ecx, sizeof szSIB_All
		.endif
	.endif
	
	;=========================
	;   Display the SIB Hex
	;=========================
	
	invoke GetByte		
	lea eax, dword ptr [HexCharsTable+eax*2]
	mov ax, word ptr [eax]
	mov ebx, lpInstHex
	mov word ptr [ebx], ax
	mov byte ptr [ebx+2], ':'	
	mov byte ptr [ebx+3],0
	add lpInstHex, 3	
	xor ebx, ebx	
	
	ret
DecodeSIB endp

ModRM16 proc 
	xor ebx, ebx
	.if ModRM.Mode==11b					; REG
		;===============================
		xor edx, edx					;
		mov bl, ModRM.RegMem			;
		mov eax, [RegTable]				; Decode Code/Reg field
		mov esi, [eax+ebx*4]			;
		mov dl, ModRM.CodeReg			; 
		mov edi, [eax+edx*4]			; Decode Reg/Mem field
		;===============================
	.elseif ModRM.Mode==00b				; [ModRM16] / DISP16
		.if ModRM.RegMem != 110b		
			;== Decode Code/Reg field ==;|
			mov bl, ModRM.RegMem		;|
			lea eax, MODRM16Table		;| 
			mov edi, [eax+ebx*4]		;|
			;---------------------------;|
			lea esi, szModRM16_Ptr
			;============================|
			; 	  Write Operand Size	 |
			;============================|
			mov eax, [OperandSize]
			invoke strCat, esi, eax, 0
			add esi, 10
			invoke strCat, esi, edi, 1
			
			
			
			lea esi, szModRM16_Ptr		
			;== Decode  Reg/Mem field ==;|
			mov bl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;| 
			mov edi, [eax+ebx*4]		;|
			;===========================;|		

			.if Instruction.BitD == 1	;|
				;xchg esi, edi			;|
				_xchg_esi_edi			;|
			.endif						;|			
					
		.else
			;===========================
			; 	DISPLACEMENT 16-BIT
			;===========================
			invoke GetDisp16, 1
		
			;== Decode  Reg/Mem field ==;|
			mov bl, ModRM.RegMem		;|
			mov eax, [RegTable]			;| 
			mov edi, [eax+ebx*4]		;|
			;===========================;|		
			
		.endif
	.elseif ModRM.Mode==01b				; [ModRM16]+DISP8	
			xor ebx, ebx
			;== Decode Code/Reg field ==;|
			mov bl, ModRM.RegMem		;|
			lea eax, MODRM16Table		;| 
			mov edi, [eax+ebx*4]		;|
			;---------------------------;|
			lea esi, szModRM16_Ptr_8
			;============================|
			; 	  Write Operand Size	 |
			;============================|


			mov eax, [OperandSize]
			invoke strCat, esi, eax, 0
			add esi, 10
			invoke strCat, esi, edi, 0

			mov edi, eax
			sub edi, 1
			
			;============================|
			;	   Get Displacement		 |
			;============================|
			invoke GetDisp8, 1
			
			invoke strCat, edi, esi, 0
			mov word ptr [eax], 005Dh ; ']', 0x00			

			lea esi, szModRM16_Ptr_8		
			xor ebx, ebx
			
			;== Decode  Reg/Mem field ==;|
			mov bl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;| 
			mov edi, [eax+ebx*4]		;|
			;===========================;|		

			.if Instruction.BitD == 1	;|
				;xchg esi, edi			;|
				_xchg_esi_edi			;|
			.endif						;|			
		
	.elseif ModRM.Mode==10b				; [ModRM16]+DISP16
			xor ebx, ebx
			;== Decode Code/Reg field ==;|
			mov bl, ModRM.RegMem		;|
			lea eax, MODRM16Table		;| 
			mov edi, [eax+ebx*4]		;|
			;---------------------------;|
			lea esi, szModRM16_Ptr_16
			;============================|
			; 	  Write Operand Size	 |
			;============================|
			
			mov eax, [OperandSize]
			invoke strCat, esi, eax, 0
			add esi, 10
			invoke strCat, esi, edi, 0
			
			mov edi, eax
			sub edi, 1
			
			;============================|
			;	   Get Displacement		 |
			;============================|
			invoke GetDisp16, 1
			invoke strCat, edi, esi, 0
			mov word ptr [eax], 005Dh ; ']', 0x00			
			
			lea esi, szModRM16_Ptr_16
					
			xor ebx, ebx
			
			;== Decode  Reg/Mem field ==;|
			mov bl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;| 
			mov edi, [eax+ebx*4]		;|
			;===========================;|		

			.if Instruction.BitD == 1	;|
				;xchg esi, edi			;|
				_xchg_esi_edi			;|
			.endif						;|		
	.endif

	ret
ModRM16 endp

ModRM32 proc
									
		xor ebx, ebx				;
		xor edx, edx				;
										
	.if ModRM.Mode==11b				; REG
									
		mov bl, ModRM.CodeReg		;
		mov eax, [RegTable]			; Decode Code/Reg field
		mov esi, [eax+ebx*4]		;
		mov dl, ModRM.RegMem		; 
		mov edi, [eax+edx*4]		; Decode Reg/Mem field
		
	.elseif ModRM.Mode==00b				; [REG32] / [SIB] / [DISP32] 
		.if ModRM.RegMem == 100b
			;-------------------
			;	Decode SIB
			;-------------------
			invoke DecodeSIB
						
			;============================|
			; 	  Write Operand Size	 |
			;============================|
			mov eax, [OperandSize]
			invoke strCat, addr szModRM_SIB, eax, 0
			;============================|
			; 	 						 |
			;== Decode Code/Reg field ==;|
			mov bl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;| 
			mov esi, [eax+ebx*4]		;|
			;============================|
			;    Write Source Operand    |
			;============================|
		
			invoke strCat, addr szModRM_SIB+11, edi, 0
			mov word ptr [eax], 005Dh	

			lea edi, szModRM_SIB
			
			.if Instruction.BitD == 0	;|
				;xchg esi, edi			;|
				_xchg_esi_edi			;|
			.endif						;|
		.elseif ModRM.RegMem == 101b
			;-------------------
			;  DISPLACEMENT 32
			;-------------------
			lea edi, szModRM_Dsp32
			

			;============================|
			; 	  Write Operand Size	 |
			;============================|
			
			mov eax, [OperandSize]
			invoke strCat, edi, eax, 0
			add edi, 11
			invoke GetDisp32, 0
			add esi, 1
			invoke strCat, edi, esi, 0
			mov word ptr [eax], 005Dh
			lea esi, szModRM_Dsp32
						
			
			xor ebx, ebx
			;== Decode Code/Reg field ==;|
			mov bl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;| 
			mov edi, [eax+ebx*4]		;|
			;---------------------------;|

			.if Instruction.BitD == 1	;|
				;xchg esi, edi			;|
				_xchg_esi_edi			;|
			.endif						;|
			
		.else
			;----------------------------
			; Decode REG1, ... PTR [REG2]
			;---------------------------;|
			xor ebx, ebx				;|
			xor edx, edx				;|
			;---------------------------;|
			;== Decode Code/Reg field ==;|
			mov bl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;| 
			mov esi, [eax+ebx*4]		;|
			;=============================
			;== Decode Reg/Mem field  ==;|
			mov dl, ModRM.RegMem		;|
			lea eax, REG32Table			;|
			mov edi, [eax+edx*4]		;|
			
			;============================|
			; 	  Write Operand Size	 |
			;============================|
			mov eax, [OperandSize]
			invoke strCat, addr szModRM_Ptr, eax, 0
			;============================|
			; 	 						 |
			;== Decode Code/Reg field ==;|
			mov bl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;| 
			mov esi, [eax+ebx*4]		;|
			;============================|
			;    Write Source Operand    |
			;============================|
		
			invoke strCat, addr szModRM_Ptr+11, edi, 0
			mov word ptr [eax], 005Dh	
			lea edi, szModRM_Ptr
			
			.if Instruction.BitD == 0	;|
				;xchg esi, edi			;|
				_xchg_esi_edi
			.endif						;|
						
		.endif
	.elseif ModRM.Mode==01b				; [REG32]+DISP8 / [SIB]+DISP8
			
		.if ModRM.RegMem != 100b
			;-------------------
			;	DISPLACEMENT 8
			;-------------------
			;---------------------------;|
			xor edx, edx				;|
			xor ebx, ebx				;|
			;---------------------------;|
			;=============================

			;============================|
			; 	  Write Operand Size	 |
			;============================|
			lea esi, szModRM_Ptr_8
			mov eax, [OperandSize]
			invoke strCat, esi, eax, 0

			;== Decode Reg/Mem field  ==;|
			mov dl, ModRM.RegMem		;|
			lea eax, REG32Table			;|
			mov edi, [eax+edx*4]		;|
			;===========================;|
			
			;============================|
			;	Write [REG32] Operand    |
			;============================|
			
			add esi, 11
			invoke strCat, esi, edi, 0		
			
			;============================|
			;  Write Displacement Value	 |
			;============================|
			add esi, 3
			mov edi, esi
			invoke GetDisp8,1
			invoke strCat, edi, esi, 0
			mov word ptr [eax], 005Dh
			
			lea esi, szModRM_Ptr_8

			;== Decode Code/Reg field ==;|
			mov bl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;| 
			mov esi, [eax+ebx*4]		;|
			;============================|
			;    Write Source Operand    |
			;============================|
		
;			invoke strCat, addr szModRM_Ptr_8+11, edi, 0
			lea esi, szModRM_Ptr_8
							
			;== Decode Code/Reg field ==;|
			mov bl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;| 
			mov edi, [eax+ebx*4]		;|
			;---------------------------;|
				
		.else
			;============================|
			; 	  Write Operand Size	 |
			;============================|
			
			mov eax, [OperandSize]
			lea esi, szSIB_DISP8
			invoke strCat, esi, eax, 0
			add esi, 11
			
			;============================|
			;  Decode & Write SIB Byte	 |
			;============================|
			invoke DecodeSIB
;			mov edi, [edi]
			invoke strCat, esi, edi, 0		
			
			;============================|
			;	Get & Write Displacemt 	 |
			;============================|
			mov edi, eax
			invoke GetDisp8,1
			invoke strCat, edi, esi, 0
			mov word ptr [eax], 005Dh

			lea esi, szSIB_DISP8
		
			;---------------------------;|
			xor edx, edx				;|
			;---------------------------;|

			;== Decode Code/Reg field  =;|
			mov dl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;|
			mov edi, [eax+edx*4]		;|			
			;=============================
			
			
			
		.endif
				
		.if Instruction.BitD == 1	;|
			;xchg esi, edi			;|
			_xchg_esi_edi			;|
		.endif						;|
		
	.elseif ModRM.Mode==10b				; [REG32]+DISP32 / [SIB]+DISP32
		.if ModRM.RegMem != 100b
			
			;-------------------
			;	DISPLACEMENT 32
			;-------------------

			;---------------------------;|
			xor edx, edx				;|
			;---------------------------;|
			lea esi, szModRM_Ptr_32

			;============================|
			; 	  Write Operand Size	 |
			;============================|
			
			mov eax, [OperandSize]
			invoke strCat, esi, eax, 0
			mov edi, eax
			add edi, 6
						
			;============================|
			; 	Write Source Operand	 |
			;============================|
			
			;== Decode Reg/Mem field  ==;|
			mov dl, ModRM.RegMem		;|
			lea eax, REG32Table			;|
			mov eax, [eax+edx*4]		;|
			;===========================;|
			
			invoke strCat, edi, eax, 0

			;============================|
			;     Write Displacement	 |
			;============================|
			invoke GetDisp32,1
			invoke strCat, eax, esi, 0
			mov word ptr [eax], 005Dh
			lea esi, szModRM_Ptr_32

			xor ebx, ebx							
			;== Decode Code/Reg field ==;|
			mov bl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;| 
			mov edi, [eax+ebx*4]		;|
			;---------------------------;|
				
		.else
			
			;============================|
			; 	  Write Operand Size	 |
			;============================|
			lea esi, szSIB_DISP32
			mov eax, [OperandSize]
			mov ebx, [eax]
			mov al, byte ptr [eax+4]
			mov dword ptr [esi], ebx
			add esi, 4
			mov [esi], al
			add esi, 7
			;-------------------
			;	 Decode SIB
			;-------------------			
			invoke DecodeSIB
			;============================|
			;	  Write SIB Decoding	 |
			;============================|

			invoke strCat, esi, edi, 0		
			
			;============================|
			;	Get & Write Displacemt 	 |
			;============================|
			mov edi, eax
			invoke GetDisp32,1
			invoke strCat, edi, esi, 0	
			mov word ptr [eax], 005Dh
			lea esi, szSIB_DISP32

			;---------------------------;|
			xor edx, edx				;|
			;---------------------------;|

			;== Decode Code/Reg field  =;|
			mov dl, ModRM.CodeReg		;|
			mov eax, [RegTable]			;|
			mov edi, [eax+edx*4]		;|			
			;=============================
			
		.endif
				
		.if Instruction.BitD == 1	;|
			;xchg esi, edi			;|
			_xchg_esi_edi			;|
		.endif						;|
	.endif	
	
	ret
ModRM32 endp

DecodeModRM proc uses ecx edx
	;-----------------------------------------------
	; 		Decode the ModRM Byte (32/16 Bit)
	;-----------------------------------------------
	add Instruction.InstSize, 1
	add Instruction.ByteOffset, 1
	
	;====================|
	; Read ModRM Byte	 |
	;===================;|
	invoke GetByte		;|
	;-------------------;|
	
	;========================================
	;	Extracts 3-Main-Parts In ModRM Byte
	;========================================
	mov cl, al						;
	and cl, 11000000y				;  Extract 2-MSB(s) (Mode)
	rol cl, 2						;  		cl:  000000:Mode.
	mov ModRM.Mode, cl				;
	mov cl, al						;
	and cl, 00111000y				;  Extract 3-Bits (Code/Reg field)
	ror cl, 3						;		cl: 00000:Code/Reg
	mov ModRM.CodeReg, cl			;
	mov cl, al						;
	and cl, 00000111y				;  Extract 3-LSB(s) (Reg/Mem Field)
									;		cl: 00000:Reg/Mem
	mov ModRM.RegMem, cl			;
	;====================================================================================
	xor eax, eax
	mov al, Instruction.OperandSize
	push eax						; Save (Eax)	 --> AL == OperandSize
	mov eax, [RegistersTable+eax*4]
	mov RegTable, eax	
	pop eax							; Restore (Eax)  --> AL == OperandSize
	mov eax, [OpSizeTable+eax*4]
	mov OperandSize, eax
	;====================================================================================

	;=========================
	; Display the ModRM Hex
	;=========================
	
	invoke GetByte		
	lea eax, dword ptr [HexCharsTable+eax*2]
	mov ax, word ptr [eax]
	mov ebx, lpInstHex
	mov word ptr [ebx], ax
	mov byte ptr [ebx+2], ':'	
	mov byte ptr [ebx+3],0
	add lpInstHex, 3	
	
	.if Instruction.AddressSize == REG32
		invoke ModRM32
	.else
		invoke ModRM16
	.endif
	ret
DecodeModRM endp

ExtractBits proc
	;=================================
	;  Extract Bits from [CODE] Byte
	;=================================
	;	AL = [CODE] Byte
	;---------------------------------
	
	;---------------------------------|
	;  Bit (D): 	000000:d:0		  |
	;---------------------------------|
	mov bl, al						 ;|
	and bl, 00000010y				 ;|
	ror bl, 1						 ;|
	mov Instruction.BitD, bl		 ;|
	;---------------------------------|
	;  Bit (W): 	0000000:w		  |
	;---------------------------------|
	mov bl, al						 ;|
	and bl, 00000001y				 ;|
	mov Instruction.BitW, bl		 ;|
	;---------------------------------|
	;  3-Bits (rrr):	00000:rrr	  |
	;---------------------------------|
	mov bl, al						 ;|
	and bl, 00000111y				 ;|
	mov Instruction.rrr, ebx		 ;|
	;---------------------------------|
	
	;--------------------------------------------|
	.if Instruction.BitW == 0					;|
		mov Instruction.OperandSize, REG8		;|
	.else										;|
		.if Prefixes.Prefix_66 == 0				;|
			mov Instruction.OperandSize,  REG32	;|
		.else									;|
			mov Instruction.OperandSize,  REG16	;|
		.endif									;|
	.endif										;|
	;--------------------------------------------|
	
	ret
ExtractBits endp

InvalidPrefix proc
	;-------------------------------;
	add Instruction.ByteOffset, 1	; Set pointer to the next byte
	;-------------------------------;
	;      Display The Bad Prefix   ;
	;-------------------------------;
	; 	....					    ;
	; 	....					    ;
	;-------------------------------;
	ret								; Return
InvalidPrefix endp

CheckPrefixes proc uses ebx
;---------------------------------------------------------------------------------------
;	This procedure checks if the current byte that need to be decoded is a prefix
;---------------------------------------------------------------------------------------
	;------------------------------------------------|
	; Reset All Prefixes.							 |
	;------------------------------------------------|
	mov edi, sizeof Prefixes						;|
 @@:												;|
 	mov byte ptr [Prefixes+edi-1], 0				;|
 	dec edi											;|
 	jnz @B											;|
 	;-------------------------------------------------
 	
 	lea edi, InstructionHex
	
	.while al==26h || al==2Eh  || al==36h  || al==3Eh  || al==64h || al==65h || al==66h || \
		al==67h || al==0F0h || al==0F2h || al==0F3h
		
		.if al==byte ptr [esi+1]				; The prefixes is repeated
			invoke InvalidPrefix
			ret
		.endif
		
		;------------------------------------------------------------------- Group (1) --
		.if al==0F0h
			mov Prefixes.Prefix_F0, 1			; Prefix F0h Flag == 1
		.elseif al==0F2h
			mov Prefixes.Prefix_F2, 1			; Prefix F2h Flag == 1
		.elseif al==0F3h	
			mov Prefixes.Prefix_F3, 1			; Prefix F3h Flag == 1
		;------------------------------------------------------------------- Group (2) --
		.elseif al==26h
			mov Prefixes.Prefix_26, 1			; Prefix 26h Flag == 1
			mov Segments.curSeg, SegES			; Current Segment == Segment ES
		.elseif al==2Eh
			mov Prefixes.Prefix_2E, 1			; Prefix 2Eh Flag == 1
			mov Segments.curSeg, SegCS			; Current Segment == Segment CS		
		.elseif al==36h
			mov Prefixes.Prefix_36, 1			; Prefix 36h Flag == 1
			mov Segments.curSeg, SegSS			; Current Segment == Segment SS		
		.elseif al==3Eh
			mov Prefixes.Prefix_3E, 1			; Prefix 3Eh Flag == 1
			mov Segments.curSeg, SegDS			; Current Segment == Segment DS			
		.elseif al==64h
			mov Prefixes.Prefix_64, 1			; Prefix 64h Flag == 1
			mov Segments.curSeg, SegFS			; Current Segment == Segment FS			
		.elseif al==65h
			mov Prefixes.Prefix_65, 1			; Prefix 65h Flag == 1
			mov Segments.curSeg, SegGS			; Current Segment == Segment GS					
		;------------------------------------------------------------------- Group (3) --
		.elseif al==66h
			mov Prefixes.Prefix_66, 1			; Prefix 66h Flag == 1
			mov Instruction.OperandSize, REG16	; Operand Size == 16-Bit
		;------------------------------------------------------------------- Group (4) --
		.elseif al==67h
			mov Prefixes.Prefix_67, 1			; Prefix 67h Flag == 1
			mov Instruction.AddressSize, REG16	; Address-Size = 16-Bit
		.endif

		;==========================
		;  Write the prefix (hex)
		;==========================
		
		lea ebx, [HexCharsTable+eax*2]
		mov ebx, [ebx]
		mov esi, [lpInstHex]
		mov word ptr [esi], bx
		mov byte ptr [esi+2], ':'
		mov byte ptr [esi+3], 0
		add lpInstHex, 3	
		
		add Prefixes.NumOfPrefixes,1			; Increase Prefixes Counter
		add Instruction.ByteOffset,1 
		invoke GetByte
		
	.endw
	 
	xor ebx, ebx
	mov bl, Prefixes.NumOfPrefixes				; At least 1-Byte [CODE]
	add bl, 1									; + SizeOf Prefixes
	mov Instruction.InstSize, bl				; InstSize = Prefixes + [CODE]
	
	ret
CheckPrefixes endp

InitializeEngine  proc
	;-----------------------;
	mov eax, ImageBase
	add eax, CodeStartRVA
	mov VirtualAddress, eax
	;-----------------------;
	mov esi, FileOffset						
	add esi, CodeStartOffset				
	mov Instruction.ByteOffset, esi			; ByteToDecode == FirstByte In Code Section
	
	mov Instruction.OperandSize, REG32
	mov Instruction.AddressSize, REG32
				
	mov edx, CodeSize						; edx = SizeOf CodeSection
 	invoke SendMessage, hProgressBar, PBM_SETRANGE32, 0, edx			
	
	mov edx, CodeSize
	xor ecx, ecx							; ecx = Zero
	xor ebx, ebx							; ebx = Zero
	xor eax, eax							; eax = Zero
	ret
InitializeEngine endp

EngineSkeleton	proc  
;---------------------------------------------------------------------------------------
;	The Skeleton Of The Decoding Engine, Which is discussed in AoD: Chapter Thirteen
;---------------------------------------------------------------------------------------
	
	pushad  ; Win2K Compatibility
		
	;----------------------------------------	
	;     Initialize Data For The Engine     
	;---------------------------------------;|	
	invoke InitializeEngine					;|
	;---------------------------------------;|		
											
	.while ecx < edx		
			
		mov lpInstHex, offset InstructionHex
		
		;-----------------------------------;|
		invoke GetByte						;|		Read 1-Byte.
		mov Instruction.ByteToDecode, al	;|		Save it. In case we need it again.
		invoke CheckPrefixes				;|		Check for prefixes.
		invoke ExtractBits					;|		Extract Bits from [CODE] byte.
		;-----------------------------------;|

		;==========================
		;  Write the [CODE] byte
		;==========================
	
		lea ebx, [HexCharsTable+eax*2]
		mov ebx, [ebx]
		mov esi, [lpInstHex]
		mov word ptr [esi], bx
		mov byte ptr [esi+2], ':'
		mov byte ptr [esi+3], 0
		add lpInstHex, 3	
		
		;---------------------------------------;|
		;    Using Our Opcodes Jump-Table   	;|
		;---------------------------------------;|
		lea esi, [InstructionsTable+eax*4] 		;|	 Address of Instruction Name. 	
		mov esi, [esi]							;|	 ESI = Address of Instruction Name.
		mov edi, eax							;|	 EAX = [CODE] Byte.
		invoke strCat, addr szInstruction, esi,1;|   Write Instruction Name.
		lea ebx, [OpcodesTable+edi*4]			;|	 Address of Opcode procedure.
		mov ebx, [ebx]							;|	 EBX = Address Of Opcode proc.
		;---------------------------------------;|
						
		;=================================================================
		; The following code will show the disassebmly of an un-supported
		; Opcode to the ListView.
		; To skip, remove the ';' from next line.
		;=================================================================
		; jmp @F
		 
		;======  Un supported opcode? ==========
		
		.if byte ptr [ebx] == 0C3h		; RET = 0xC3
			;==========================
			;  Write the [CODE] byte
			;==========================				
			invoke dwHex2Str, VirtualAddress, addr szBuf
			invoke InsertItem,dwListItem,0,addr szBuf
			invoke InsertItem,dwListItem,1,addr InstructionHex
			add dwListItem, 1
@@:
			jmp @F
		.endif
		;======================================
		
		;====================================|
		; 	Goto opcode decoding procedure	 |
		;====================================|
		; call ebx							;|
		;-----------------------------------;|

		;=================================================================
		; The following code will show the disassebmly in the ListView.
		; To skip, remove the ';' from next line.
		;=================================================================
		; jmp @F

		invoke ShowDisassebmly		
@@:		
		;===================================;|
		xor ebx, ebx						;|
		mov bl, Instruction.InstSize		;|		EBX  = Instruction Size
		add ecx, ebx						;|		ECX += EBX
		add VirtualAddress, ebx				;|		VA	+= EBX
		add Instruction.ByteOffset, 1		;|		
		;-----------------------------------;|

		;===============================================================
		; The following code will update the value of the progress bar.
		; To skip, remove the ';' from next line.
		;===============================================================
		; jmp @F
		 		
		push ecx
		push edx
		invoke PostMessage, hProgressBar, PBM_SETPOS, ecx, 0
		pop edx
		pop ecx
		
@@:		
		

		mov Instruction.OperandSize, REG32
		mov Instruction.AddressSize, REG32


	.endw									; Loop till end of Code Section

	
	invoke ShowWindow, hDsmList, SW_SHOW
	invoke ShowWindow, hProgressBar, SW_HIDE
	invoke ShowWindow, hWnd, SW_RESTORE
	invoke SetForegroundWindow,hWnd
	
	invoke TerminateThread,hThread,0
	invoke CloseHandle,hThread	
	mov hThread,0

	popad		  ; Win2K  										
	ret
EngineSkeleton endp


