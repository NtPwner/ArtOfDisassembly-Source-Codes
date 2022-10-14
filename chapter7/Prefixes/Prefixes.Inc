
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
IDC_PREFIXES		equ 1001

IDD_SEGMENTS		equ 2000
IDD_OPERAND_SIZE	equ 3000
IDD_ADDRESS_SIZE	equ 4000
IDD_REP_REPNE_SIZE	equ 5000
IDD_LOCk			equ 6000
IDD_PRACTICE		equ 7000

;#########################################################################

.data
szSegments			db "Segments Prefixes",0
szOperand			db "Operand-Size Prefix",0
szAddress			db "Address-Size Prefix",0
szREPs				db "REP/REPE/REPNE Prefixes",0
szLOCK				db "Bus LOCK Prefix",0
szPractice			db "Practice On Ur Own",0

.data?

hWnd				dd ?				; Handle of the main program
hInstance			dd ?				; Instance of the program 

hTab				dd ?				; Handle of the main Tab control
SelTab				dd ?				; Selected Tab
hTabDlg				dd 6 dup (?)		; Hadnle to the Tabs Dialogs
DLG_COLOR			dd ?				; Dialog Color -Used in Coloring controls =)-

;#########################################################################
