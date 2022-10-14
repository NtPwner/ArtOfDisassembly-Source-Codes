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

REG32Table		dd	_EAX,	_ECX,	_EDX,	_EBX,	
					_ESP,	_EBP,	_ESI,	_EDI
		
;--------- 16-Bit ----------------
_AX				db	"AX",0
_CX				db	"CX",0
_DX				db	"DX",0
_BX				db	"BX",0		
_SP				db	"SP",0
_BP				db	"BP",0		
_SI				db	"SI",0	
_DI				db	"DI",0	

REG16Table		dd	_AX,	_CX,	_DX,	_BX,
					_SP,	_BP,	_SI,	_DI
									
;--------- 8-Bit ----------------
_AL				db	"AL",0
_CL				db	"CL",0
_DL				db	"DL",0
_BL				db	"BL",0		
_AH				db	"AH",0
_CH				db	"CH",0		
_DH				db	"DH",0	
_BH				db	"BH",0	

REG8Table		dd	_AL,	_CL,	_DL,	_BL,
					_AH,	_CH,	_DH,	_BH

;-------------------------- Operand-Size ----------------------

_DWORD			db	"DWORD",0
_WORD			db	"WORD ",0
_BYTE			db	"BYTE ",0

OpSizeTable		dd	_DWORD, _WORD, _BYTE

RegistersTable 	dd	REG32Table, REG16Table, REG8Table

;---------------------------- ModRM 16-Bit --------------------

_BXSI			db	"[BX+SI]",0
_BXDI			db	"[BX+DI]",0
_BPSI			db	"[BP+SI]",0
_BPDI			db	"[BP+DI]",0
_SI_MODRM		db	"[SI]",0
_DI_MODRM		db	"[DI]",0
_BP_MODRM		db	"[BP]",0
_BX_MODRM		db	"[BX]",0

MODRM16Table	dd	_BXSI,		_BXDI,		
					_BPSI,		_BPDI,		
					_SI_MODRM,	_DI_MODRM,	
					_BP_MODRM,	_BX_MODRM,
