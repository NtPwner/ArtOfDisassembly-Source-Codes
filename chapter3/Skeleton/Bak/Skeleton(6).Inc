
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

;-------------------------------------- Menu Items ----------------------
IDM_OPEN 			equ	10002
IDM_CLOSE 			equ	10003
IDM_EXIT 			equ 10004

.data

;-------------------------------------- File manipulating ---------------
ofn		OPENFILENAME	<>
buffer 					db 	260  dup(0)
StrFilter 				db 	"Executable Files",0,"*.exe",0 
             			db 	"Dynamic Link Libraries",0,"*.dll",0,0


;#########################################################################

.data?

;-------------------------------------- File manipulating ----------------

hFile					dd	?			; Handle of the file
FileSize				dd	?			; The file size ( GetFileSize() )
FileOffset				dd	?			; The offset of the mapped file in memory

hInstance				dd 	?
hDsmList				dd	?			; Handle of the main ListView
hWnd					dd	?			; Handle of the main dialog

;#########################################################################
