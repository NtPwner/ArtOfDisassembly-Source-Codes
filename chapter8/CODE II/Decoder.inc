
.data

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

REG32			dd	_EAX,	000b
				dd	_ECX,	001b
				dd	_EDX,	010b
				dd	_EBX,	011b
				dd	_ESP,	100b
				dd	_EBP,	101b
				dd	_ESI,	110b
				dd	_EDI,	111b
		
;--------- 16-Bit ----------------
_AX				db	"AX",0
_CX				db	"CX",0
_DX				db	"DX",0
_BX				db	"BX",0		
_SP				db	"SP",0
_BP				db	"BP",0		
_SI				db	"SI",0	
_DI				db	"DI",0	

REG16			dd	_AX,	000b
				dd	_CX,	001b
				dd	_DX,	010b
				dd	_BX,	011b
				dd	_SP,	100b
				dd	_BP,	101b
				dd	_SI,	110b
				dd	_DI,	111b
									
;--------- 8-Bit ----------------
_AL				db	"AL",0
_CL				db	"CL",0
_DL				db	"DL",0
_BL				db	"BL",0		
_AH				db	"AH",0
_CH				db	"CH",0		
_DH				db	"DH",0	
_BH				db	"BH",0	

REG8			dd	_AL,	000b
				dd	_CL,	001b
				dd	_DL,	010b
				dd	_BL,	011b
				dd	_AH,	100b
				dd	_CH,	101b
				dd	_DH,	110b
				dd	_BH,	111b
				
;-------------------- Instructions ------------------------------

;mov add sub xor or and

_MOV			db	"MOV",0
_ADD			db	"ADD",0
_SUB			db	"SUB",0
_XOR			db	"XOR",0
_OR				db	"OR" ,0
_AND			db	"AND",0

_MOV_BIN		db	"100010",0
_ADD_BIN		db	"100000",0
_SUB_BIN		db	"001010",0
_XOR_BIN		db	"001100",0
_OR_BIN			db	"000010" ,0
_AND_BIN		db	"001000",0


Instructions	dd 	offset	_MOV,	offset _MOV_BIN,	
					offset	_ADD,	offset _ADD_BIN,	
					offset	_SUB,	offset _SUB_BIN,	
					offset	_XOR,	offset _XOR_BIN,
					offset	_OR ,	offset _OR_BIN ,
					offset	_AND,	offset _AND_BIN

;-------------------- Instruction Decoding ----------------------
Inst_1_Decoding			db " %s[1:1]:[ModRM]	%s	 %s, %s", 0	
Inst_2_Decoding			db " %s[1:1]:[ModRM]	%s	 %s, %s", 0	

.data? 

;------ Instruction ----------
Instruction_Bit_d		db	2 dup (?)	;	Bit (d) == 0 (DEFAULT)
Instruction_Bit_w		db	2 dup (?)	;	Bit (w) == 0 (DEFAULT)

Instruction_1_REG		dd	2 dup (?)	;	<reg1> <reg2> (First  Instruction)
Instruction_2_REG		dd	2 dup (?)	;	<reg1> <reg2> (Second Instruction)

Instruction_1_Inst		dd	?			;	Instruction to be used (e.g. MOV/ADD/SUB)
;Instruction_2_Inst	 					;	Not used (as the above)

Instruction_Prefix		db	2 dup (?)	;	Operand-Size Prefix (66h)

Instruction_Bin			dd	?			;	Instruction Binary form
Instruction_Hex			db	2 dup (?)	; 	Instructions Hex form

.code

DecodeReg	proc BitsStart:DWORD, InstIndex:BYTE, RegIndex:BYTE
;----------------------------------------------------------------------
;	BitsStart: Control ID for Bit (0) of the register to be decoded
;
;	InstIndex: 
;						First  Instruction 	== 0
;						Second Instruction  == 1
;	RegIndex:
;						<reg1>	== 	0
;						<reg2>	== 	1
;
;	Return:	DWORD to be a pointer to the decoded reg
;
;	e.g. 		szEAX		= 	"EAX"	;
;				Return 		 	&szEAX	;
;
;----------------------------------------------------------------------
	
	xor eax, eax
	mov ecx, 3					
 @@:
	push eax
	push ecx
		add ecx, BitsStart
		dec ecx
		invoke GetDlgItemInt, hWnd,ecx,0,0
		mov ebx, eax
	pop ecx
	pop eax
		rol eax, 1
		add eax, ebx
		dec ecx					; 3-Bits for <reg1> or <reg2> (ModRM: 2:<3>:<3>)
		jnz @B		
	
	; EAX == Register Value (e.g. 000 / 001 / 010)
		
	xor ecx, ecx
	mov cl, InstIndex
	lea ecx, Instruction_Bit_w[ecx*1]				; Get Bit (w)	(Operand-Size)
	mov cl, [ecx]
	and ecx, 0FFh
	test ecx, ecx
	
	mov ecx, 8										; 8 Registers
	
	.if !ZERO?										; Opernad-Size
		xor ecx, ecx
		mov cl, InstIndex
		lea ecx, Instruction_Prefix[ecx*1]
		mov cl, [ecx]
		and ecx, 0FFh
		test ecx, ecx
		.if !ZERO?			
				lea esi, REG16						; 16-Bit Registers
		.else
				lea esi, REG32						; 32-Bit Registers
		.endif
	.else
		lea esi, REG8								; 8-Bit Registers		
	.endif
	
	mov ecx, 8	
	
 SearchReg:
		cmp eax, dword ptr [esi+4]
		.if ZERO?									; Register Found
			lea eax, [esi]
			mov eax, [eax]
			xor ecx, ecx
			inc ecx
		.endif
		add esi, 8
		loop SearchReg	
				
	
	xor ecx, ecx
	mov cl, InstIndex
	lea ecx, Instruction_Bit_d[ecx*1]				; Get Bit (d)	(direction)
	mov cl, [ecx]
	and ecx, 0FFh
	test ecx, ecx
	
	.if ZERO?										; (Direction Reversed) == 1
		.if RegIndex == 0
			mov RegIndex, 1
		.else
			mov RegIndex, 0
		.endif
	.endif
	

	xor ecx, ecx
	mov cl, RegIndex								; <reg1> or <reg2>
	
	.if InstIndex==0
		lea esi, Instruction_1_REG[ecx*4]
	.else
		lea esi, Instruction_2_REG[ecx*4]		
	.endif
	
	mov [esi], eax
	
	;------------------------------------------ First  Instruction
	mov eax, Instruction_1_REG[0*4]
	mov ebx, Instruction_1_REG[1*4]	
	xor ecx, ecx
	mov cl, Instruction_Hex[0*4]
	invoke wsprintf, offset Buffer_1, offset Inst_1_Decoding, Instruction_Bin, Instruction_1_Inst, eax, ebx
	invoke SetDlgItemText, hWnd, IDC_DECODE_1 , offset Buffer_1

	;------------------------------------------ Second Instruction
	mov eax, Instruction_2_REG[0*4]
	mov ebx, Instruction_2_REG[1*4]	
	xor ecx, ecx
	mov cl, Instruction_Hex[1*4]
	invoke wsprintf, offset Buffer_2, offset Inst_2_Decoding, Instruction_Bin, Instruction_1_Inst, eax, ebx
	invoke SetDlgItemText, hWnd, IDC_DECODE_2 , offset Buffer_2
	
	ret
DecodeReg endp

DecodeAll	proc
	invoke SetDlgItemText,hWnd, 1019, Instruction_Bin			
	invoke SetDlgItemText,hWnd, 1055, Instruction_Bin	
			
	invoke DecodeReg, IDC_MODRM1_BIT_3, 0, 0	; First  Instruction, <reg1>
	invoke DecodeReg, IDC_MODRM1_BIT_0, 0, 1	; First Instruction, <reg2>
	invoke DecodeReg, IDC_MODRM2_BIT_3, 1, 0	; Second  Instruction, <reg1>
	invoke DecodeReg, IDC_MODRM2_BIT_0, 1, 1	; Second Instruction, <reg2>				
	ret
DecodeAll endp