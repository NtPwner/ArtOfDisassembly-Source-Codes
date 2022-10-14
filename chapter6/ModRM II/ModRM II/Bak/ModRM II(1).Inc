
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

.const

IDD_DIALOG1			equ 101
IDC_DECODE			equ 1014
IDC_BYTE			equ 1015
;----------------------------- For font preparing -----
IDC_FIRST			equ 1001
IDC_LAST			equ	1014	
;------------------------------------------------------
IDC_BIT_0			equ 1001
IDC_BIT_1			equ 1002
IDC_BIT_2			equ 1003
IDC_BIT_3			equ 1004
IDC_BIT_4			equ 1005
IDC_BIT_5			equ 1006
IDC_BIT_6			equ 1007
IDC_BIT_7			equ 1008

.data
FontName			db	"Courier New",0
ToHex				db 	"%X",0

Mode				db "00",0 			; 2 High Bits in ModR/M
Reg1				db "000",0			; 3 Following Bytes
Reg2				db "000",0			; 3 Least Bytes

; 32-bit General Purpose Registers

REG_EAX				db	"000",0
REG_ECX				db	"001",0
REG_EDX				db	"010",0
REG_EBX				db	"011",0
REG_ESP				db	"100",0
REG_EBP				db	"101",0
REG_ESI				db	"110",0
REG_EDI				db	"111",0

_EAX				db	"eax",0
_ECX				db	"ecx",0
_EDX				db	"edx",0
_EBX				db	"ebx",0
_ESP				db	"esp",0
_EBP				db	"ebp",0
_ESI				db	"esi",0
_EDI				db	"edi",0

Registers32			dd	offset REG_EAX, offset _EAX,
						offset REG_ECX, offset _ECX,
						offset REG_EDX, offset _EDX,
						offset REG_EBX, offset _EBX,
						offset REG_ESP, offset _ESP,
						offset REG_EBP, offset _EBP,
						offset REG_ESI, offset _ESI,
						offset REG_EDI, offset _EDI

Decoding			db "[OPCODE] %s, %s",0
;#########################################################################

.data?

hInstance			dd ?
hWnd				dd ?
hFont				dd ?

dwMode				dd	?
dwReg1				dd	?
dwReg2				dd	?

OpcodeByte			db	?

szBuffer			db 512 dup (?)
;#########################################################################

.code
BitsToByte proc
;-------------------------
	xor eax, eax
	xor ecx, ecx
;-------------------------					
 @@:
	push eax
	push ecx
		add ecx, IDC_BIT_0
		invoke GetDlgItemInt, hWnd,ecx,0,0
		mov ebx, eax
	pop ecx
	pop eax
		rol eax, 1
		add eax, ebx
		inc ecx
		cmp ecx, 8
		jb @B		
		
	invoke wsprintf, addr szBuffer, addr ToHex, eax
	invoke SetDlgItemText,hWnd, IDC_BYTE, addr szBuffer
	ret
	 					
BitsToByte endp

UpdateReg1 proc
	lea esi, Reg1
	lea eax, Registers32
	mov ecx, 8
 SearchReg1:
	mov edi, dword ptr [eax]
		pusha
		mov ecx, 4
		repe cmpsb byte ptr [esi],  byte ptr [edi]
		popa
		
		jnz @F
			mov eax, dword ptr [eax+4] 
			mov dwReg1, eax
			invoke BitsToByte
			ret
	@@:			
	add eax, 8
	loopd SearchReg1 	

	invoke BitsToByte
	ret
UpdateReg1 endp

UpdateReg2 proc
	lea esi, Reg2
	lea eax, Registers32
	mov ecx, 8
 SearchReg2:
	mov edi, dword ptr [eax]
		pusha
		mov ecx, 4
		repe cmpsb byte ptr [esi],  byte ptr [edi]
		popa
		
		jnz @F
			mov eax, dword ptr [eax+4] 
			mov dwReg2, eax
			invoke BitsToByte
			ret
	@@:			
	add eax, 8
	loopd SearchReg2 	

	invoke BitsToByte
	ret
UpdateReg2 endp












