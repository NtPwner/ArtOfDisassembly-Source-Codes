
include windows.inc
include kernel32.inc
include user32.inc
include Comctl32.inc
include shell32.inc
include Comdlg32.inc

includelib kernel32.lib
includelib user32.lib
includelib Comctl32.lib
includelib shell32.lib
includelib Comdlg32.lib

DlgProc			PROTO	:HWND,:UINT,:WPARAM,:LPARAM

.const

IDD_DLG				equ 101

;#########################################################################

.data?

hInstance			dd 	?
hWnd				dd	?	; Handle of the main dialog

;#########################################################################
