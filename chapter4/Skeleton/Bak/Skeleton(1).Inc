
include windows.inc
include kernel32.inc
include user32.inc
include Comctl32.inc
include shell32.inc
include comdlg32.inc 

includelib kernel32.lib
includelib user32.lib
includelib Comctl32.lib
includelib shell32.lib
includelib comdlg32.lib 


DlgProc			PROTO	:HWND,:UINT,:WPARAM,:LPARAM

.const

IDD_DLGMAIN			equ 101
IDC_DASM			equ 1001		; Resource constant for the main ListView


.data?
hInstance				dd 	?
hDsmList				dd	?		; Handle of the main ListView
hWnd					dd	?		; Handle of the main dialog

;#########################################################################
