
include windows.inc
include kernel32.inc
include user32.inc
include Comctl32.inc
include shell32.inc
include gdi32.inc

includelib kernel32.lib
includelib user32.lib
includelib Comctl32.lib
includelib shell32.lib
includelib gdi32.lib

DlgProc			PROTO	:HWND,:UINT,:WPARAM,:LPARAM

GetControlhWnds PROTO							; Save Handles To Some Controls
SetWindowFont	PROTO   :DWORD,:DWORD			; Set Window Font :p
PressButton		PROTO 	:DWORD,:DWORD,:DWORD	; Press Needed Button And UnPress Other Buttons

DecodeInstruction PROTO

.const

IDD_DIALOG1			equ 101
IDC_TITLE			equ 1012
IDC_TITLE2			equ 1013
IDC_INSTBORDER1		equ 1014
IDC_INSTBORDER2		equ 1015
IDC_INSTBORDER3		equ 1016
IDC_INSTBORDER4		equ 1017
IDC_ON				equ 1021
IDC_OFF				equ 1022

;--------------------
; Instructions' Buttons: 		INC DEC PUSH POP
IDC_INC				equ 1002
IDC_DEC				equ 1003
IDC_PUSH			equ 1004
IDC_POP				equ 1005

IDC_CODE			equ 1008
IDC_RRR1			equ	1009
IDC_RRR2			equ	1010
IDC_RRR3			equ	1011

IDC_DECODING		equ 1019

;--------------------
COLOR_RED			equ 000000FFh	; RED Color
COLOR_BLUE			equ 00FF0000h	; BLUE Color
COLOR_GREEN			equ 0031CE70h	; LIGHT GREEN Color

;--------------------
FirstControlID		equ 1001		; ID of the first control
LastControlID		equ 1020		; ID of the last control

;--------------------1-Byte Instructions -----------------------
_INC_REG_FULL		db "INC",0		; INC 	<reg16/reg32>
_DEC_REG_FULL		db "DEC",0		; DEC 	<reg16/reg32>
_PUSH_REG_FULL		db "PUSH",0		; PUSH 	<reg16/reg32>
_POP_REG_FULL		db "POP",0		; POP 	<reg16/reg32>

_INC_BIN			db "01000",0	; [CODE] for 'INC'  instruction
_DEC_BIN			db "01001",0	; [CODE] for 'DEC'  instruction
_PUSH_BIN			db "01010",0	; [CODE] for 'PUSH' instruction
_POP_BIN			db "01011",0	; [CODE] for 'POP'  instruction
;--------------------------------------- Table ------------------

ONE_BYTE_INST		dd _INC_REG_FULL,  	_INC_BIN
					dd _DEC_REG_FULL,	_DEC_BIN
					dd _PUSH_REG_FULL,	_PUSH_BIN
					dd _POP_REG_FULL,	_POP_BIN

;-------------------- Prefixes ---------------------------------
PREFIX_OPERAND_SIZE	equ 66h

;-------------------- Full Size Registers -----------------------

;--------- 32-Bit ----------------
_EAX				db	"EAX",0
_ECX				db	"ECX",0
_EDX				db	"EDX",0
_EBX				db	"EBX",0		
_ESP				db	"ESP",0
_EBP				db	"EBP",0		
_ESI				db	"ESI",0	
_EDI				db	"EDI",0	

REG32				dd	_EAX,	000b
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

REG16				dd	_AX,	000b
					dd	_CX,	001b
					dd	_DX,	010b
					dd	_BX,	011b
					dd	_SP,	100b
					dd	_BP,	101b
					dd	_SI,	110b
					dd	_DI,	111b
					

;-------------------- Instruction Decoding ----------------------
InstDecode			db	"  %X				%s %s ",	0			; INSTRUCTION <reg16/reg32>
InstDecode2			db	"  66:%X				%s %s ",0			; INSTRUCTION <reg16/reg32>

;#########################################################################

.data?

hWnd				dd	?			; Handle of the main window
hInstance			dd 	?			; Handle of the Instance to the program

;--------------------
COLOR_DLG			dd	?			; Dialog-Box Color
hBrushTransperant	dd	?			; Handle of a (Dialog-Color) brush
;--------------------

RedControlHwnd		dd	1 dup (?)	; Handles Of the controls that will be in RED
BlueControlHwnd		dd	2 dup (?)	; Handles Of the controls that will be in BLUE
GreenControlHwnd	dd	5 dup (?)	; Handles Of the controls that will be in LIGHT GREEN

;--------------------
Buffer				db	250 dup (?) ; Buffer for WsprintfA
;--------------------

InstName			dd	?			; Instruction Name (Pointer)
InstCode			dd	?			; Instruction Code (40h-->58h)
RegCode				dd	?			; Register Code (0-->7)
Opcode				dd	? 			; HEX value of the instruction
OperandSize			db	?			; Operand-Size (0 = 32/ 1 = 16) Bits

;#########################################################################
