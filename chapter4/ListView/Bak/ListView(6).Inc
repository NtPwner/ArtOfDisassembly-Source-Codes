
include windows.inc
include kernel32.inc
include user32.inc
include Comctl32.inc
include shell32.inc

includelib kernel32.lib
includelib user32.lib
includelib Comctl32.lib
includelib shell32.lib

DlgProc			PROTO	:HWND,:UINT,:WPARAM,:LPARAM

.const

IDD_DLG				equ 101
IDC_DSM				equ	1001

;#########################################################################
.data
Header1				db			"Offset",0
Header2				db			"Opcodes",0
Header3				db			"Disassembly",0
Header4				db			"Comments",0

;#########################################################################

.data?

hInstance			dd 	?
hWnd				dd	?		; Handle to the main dialog
hDsm				dd	?		; Handle to the ListView
dwIndex				dd	0		; No. of columns inserted

;#########################################################################
