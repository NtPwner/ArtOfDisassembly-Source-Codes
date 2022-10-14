
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
InverseButton	PROTO	:DWORD					; Inverse Bit (e.g. 0 --> 1 / 1 --> 0)
DecodeReg		PROTO	:DWORD,:BYTE,:BYTE		; Read Decoder.inc Info ;)
DecodeAll		PROTO

.const

IDD_DIALOG1			equ 101
IDC_TITLE			equ 1001
IDC_TITLE2			equ 1002

;----------- Controls IDs ----------------
;CODE II.dlg
IDD_DIALOG1								equ 101
IDC_TITLE								equ 1001
IDC_TITLE2								equ 1002

IDC_MOV									equ 1004		; 'MOV' (BUTTON)
IDC_ADD									equ 1005		; 'ADD' (BUTTON)
IDC_SUB									equ 1006		; 'SUB' (BUTTON)
IDC_XOR									equ 1007		; 'XOR' (BUTTON)
IDC_OR									equ 1008		; 'OR'  (BUTTON)
IDC_AND									equ 1009		; 'AND' (BUTTON)

IDC_BIT_D_1								equ 1020		; Bit (d) for first  instruction
IDC_BIT_D_2								equ 1027		; Bit (d) for second instruction

IDC_BIT_W_1								equ 1021		; Bit (w) for first  instruction
IDC_BIT_W_2								equ 1028		; Bit (w) for second instruction

IDC_DECODE_1							equ 1030		; First  instruction decoding (STATIC)
IDC_DECODE_2							equ 1031		; Second instruction decoding (STATIC)

IDC_MODRM1_BIT_0						equ 1032		; ModR/M For First  Instruction Bit (0)
IDC_MODRM1_BIT_1						equ 1033		; ModR/M For First  Instruction Bit (1)
IDC_MODRM1_BIT_2						equ 1034		; ModR/M For First  Instruction Bit (2)
IDC_MODRM1_BIT_3						equ 1035		; ModR/M For First  Instruction Bit (3)
IDC_MODRM1_BIT_4						equ 1036		; ModR/M For First  Instruction Bit (4)
IDC_MODRM1_BIT_5						equ 1037		; ModR/M For First  Instruction Bit (5)

IDC_MODRM2_BIT_0						equ 1041		; ModR/M For second Instruction Bit (0)
IDC_MODRM2_BIT_1						equ 1042		; ModR/M For second Instruction Bit (1)
IDC_MODRM2_BIT_2						equ 1043		; ModR/M For second Instruction Bit (2)
IDC_MODRM2_BIT_3						equ 1045		; ModR/M For second Instruction Bit (3)
IDC_MODRM2_BIT_4						equ 1046		; ModR/M For second Instruction Bit (4)
IDC_MODRM2_BIT_5						equ 1047		; ModR/M For second Instruction Bit (5)

IDC_ON_1								equ 1051		; Prefix (66h) On  For First  Instruction
IDC_ON_2								equ 1053		; Prefix (66h) On  For Second Instruction

IDC_OFF_1								equ 1052		; Prefix (66h) Off For First  Instruction
IDC_OFF_2								equ 1054		; Prefix (66h) Off For Second Instruction

IDC_66h_1								equ 1056		; 66h Prefix 'STATIC'
IDC_66h_2								equ 1057		; 66h Prefix 'STATIC'

IDC_TIP									equ 1058		; TIP at the button (STATIC)

;--------------------
COLOR_RED			equ 000000FFh	; RED Color
COLOR_BLUE			equ 00FF0000h	; BLUE Color
COLOR_GREEN			equ 0031CE70h	; LIGHT GREEN Color
COLOR_PURPULE		equ 00FD02F0h	; PURPULE

;--------------------
FirstControlID		equ 1001		; ID of the first control
LastControlID		equ 1060		; ID of the last control

;#########################################################################

.data?

hWnd				dd	?			; Handle of the main window
hInstance			dd 	?			; Handle of the Instance to the program

;--------------------
COLOR_DLG			dd	?			; Dialog-Box Color
hBrushTransperant	dd	?			; Handle of a (Dialog-Color) brush
;--------------------

RedControlHwnd		dd	4 dup (?)	; Handles Of the controls that will be in RED
BlueControlHwnd		dd	3 dup (?)	; Handles Of the controls that will be in BLUE
GreenControlHwnd	dd	4 dup (?)	; Handles Of the controls that will be in LIGHT GREEN
PurpuleControlHwnd	dd	4 dup (?)	; Handles Of the controls that will be in PURPULE

;--------------------
Buffer_1			db	250 dup (?) ; Buffer for WsprintfA
Buffer_2			db	250 dup (?) ; Buffer for WsprintfA
;--------------------

InstName			dd	?			; Instruction Name (Pointer)
InstCode			dd	?			; Instruction Code (40h-->58h)
RegCode				dd	?			; Register Code (0-->7)
Opcode				dd	? 			; HEX value of the instruction
OperandSize			db	?			; Operand-Size (0 = 32/ 1 = 16) Bits

;#########################################################################
