.const

;================================== Prefixes ===================
tPREFIXES struct
	; Group (1)	
	Prefix_F0		db		?		; LOCK
	Prefix_F2		db		?		; REPNE/REPNZ
	Prefix_F3		db		?		; REP or REPE/REPZ 
	; Group (2)
	Prefix_2E		db		?		; CS Segment Override / (Branch is NOT taken)
	Prefix_36		db		?		; SS Segment Override
	Prefix_3E		db		?		; DS Segment Override / (Branch is taken)
	Prefix_26		db		?		; ES Segment Override
	Prefix_64		db		?		; FS Segment Override
	Prefix_65		db		?		; GS Segment Override
	; Group (3)	
	Prefix_66		db		?		; Operand-Size Override
	; Group (4)	
	Prefix_67		db		?		; Address-Size Override
	; Others..
	NumOfPrefixes	db		?		; Number Of Prefixes
tPREFIXES ends
;===============================================================

;==================================== Segments =================
tSEGMENTS struct
	curSeg		db		?			; Current Segment
		SegCS	equ		0			; Segment CS
		SegSS	equ		1			; Segment SS
		SegDS	equ		2			; Segment DS
		SegES	equ		3			; Segment ES
		SegFS	equ		4			; Segment FS
		SegGS	equ		5			; Segment GS
tSEGMENTS ends
;===============================================================

;==================================== Instruction ==============
tINSTRUCTION	struct
	ByteToDecode	db		?		; Current byte that should be decoded
	ByteOffset		dd		?		; Offset if Current byte to be decoded
	OperandSize		db		?		; OperandSize	(32/16/8 Bit)
	AddressSize		db		?		; AddressSize	(32/16/8 Bit)
		REG32		equ 	0		;
		REG16		equ		1		;
		REG8		equ		2		;
	BitD			db		?		; Bit(d)		(Direction)
	BitW			db		?		; Bit(w)		(Full/Partial Operand Size)
									;	
	rrr				dd		?		; 3-Bits for register code 
									;				e.g. :
									;						B8+ rw MOV r16,imm16
									;						B8+ rd MOV r32,imm32
									; 							
	InstSize		db		?		; Size of instruction (How Many Bytes)
tINSTRUCTION ends
;===============================================================

;================================= Disassembler Info ===========
tDISASSEMBLY 	struct
	OpcodeOffset	dd		?			;   Column (1) In The Disassembler
	ptrOpcode		dd		?			;   Column (2) In The Disassembler
	ptrDisassembly	dd		?			;	Column (3) In The Disassembler
	ptrComment		dd		?			;	Column (4) In The Disassembler
tDISASSEMBLY ends
;===============================================================

;==================================== ModRM Byte ===============
tMODRM struct
	Mode	db		?			; Mode:xxx:xxx		(2-Bits)
	CodeReg	db		?			; xx:CodeReg:xxx	(3-Bits)
	RegMem	db		?			; xx:xxx:RegMem		(3-Bits)
tMODRM ends
;===============================================================

;==================================== SIB Byte =================
tSIB struct
	Scale	db		?			; Scale:xxx:xxx		(2-Bits)
	Index	db		?			; xx:Index:xxx		(3-Bits)
	Base	db		?			; xx:xxx:Base		(3-Bits)
tSIB ends
;===============================================================

;================================= DISPLACEMENT 32 Bit =========
tDISP32 struct
	Displacement	dd		?		; Value of the displacement
	Signed			db		?		; Is displacement signed?
tDISP32 ends
;===============================================================

;================================= DISPLACEMENT 16 Bit =========
tDISP16 struct
	Displacement	dd		?		; Value of the displacement
tDISP16 ends
;===============================================================

;================================= DISPLACEMENT 8 Bit ==========
tDISP8 struct
	Displacement	dd		?		; Value of the displacement
tDISP8 ends
;===============================================================
