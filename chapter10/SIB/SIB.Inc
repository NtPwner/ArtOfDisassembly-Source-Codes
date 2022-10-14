
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

;---------------------------------------------------------- Control IDs -----------------

IDD_MAIN			equ 101
IDC_TITLE_I 		equ	1001	; Title (STATIC) Part I
IDC_TITLE_II		equ	1002	; Title (STATIC) Part II
IDC_MODRM			equ	1003	; 'ModRM'		(GROUP)
IDC_SIB				equ	1085	; 'SIB'		(GROUP)
IDC_INSTRUCTION		equ	1014	; 'Instruction' (GROUP)
IDC_DECODING_I		equ 1015	; 'Decoding' For Instruction (GROUP)
IDC_DECODING_II		equ 1016	; 'Decoding' For ModRM (GROUP)
IDC_DECODING_III	equ 1074	; 'Decoding' For SIB (GROUP)
IDC_DISPLACEMENT	equ 1027	; 'Displacement (GROUP)
IDC_HEX_DECODE		equ 1064	; 'HEX Decoding' 		At the bottom (STATIC)
IDC_MNE_DECODE		equ 1060	; 'Mnemonic Decoding' 	At the bottom (STATIC)

IDC_OFF				equ	1062	; 'OFF' Operand-Size Prefix (BUTTON)
IDC_ON				equ	1063	; 'ON'  Operand-Size Prefix (BUTTON)

;------------------------ IDs For ModRM Bits --------------------
IDC_MODRM_7								equ 1004
IDC_MODRM_6								equ 1005
IDC_MODRM_5								equ 1006
IDC_MODRM_4								equ 1007
IDC_MODRM_3								equ 1008
IDC_MODRM_2								equ 1009
IDC_MODRM_1								equ 1010
IDC_MODRM_0								equ 1011

;------------------------ IDs For SIB Bits --------------------
IDC_SIB_7								equ 1075
IDC_SIB_6								equ 1076
IDC_SIB_5								equ 1077
IDC_SIB_4								equ 1078
IDC_SIB_3								equ 1079
IDC_SIB_2								equ 1080
IDC_SIB_1								equ 1081
IDC_SIB_0								equ 1082

;--------------- IDs For Displacement (BYTE/DWORD) ---------------
IDC_DISP_BIT_00							equ 1028
IDC_DISP_BIT_01							equ 1029
IDC_DISP_BIT_02							equ 1030
IDC_DISP_BIT_03							equ 1031
IDC_DISP_BIT_04							equ 1032
IDC_DISP_BIT_05							equ 1033
IDC_DISP_BIT_06							equ 1034
IDC_DISP_BIT_07							equ 1035
IDC_DISP_BIT_08							equ 1036
IDC_DISP_BIT_09							equ 1037
IDC_DISP_BIT_10							equ 1038
IDC_DISP_BIT_11							equ 1039
IDC_DISP_BIT_12							equ 1040
IDC_DISP_BIT_13							equ 1041
IDC_DISP_BIT_14							equ 1042
IDC_DISP_BIT_15							equ 1043
IDC_DISP_BIT_16							equ 1044
IDC_DISP_BIT_17							equ 1045
IDC_DISP_BIT_18							equ 1046
IDC_DISP_BIT_19							equ 1047
IDC_DISP_BIT_20							equ 1048
IDC_DISP_BIT_21							equ 1049
IDC_DISP_BIT_22							equ 1050
IDC_DISP_BIT_23							equ 1051
IDC_DISP_BIT_24							equ 1052
IDC_DISP_BIT_25							equ 1053
IDC_DISP_BIT_26							equ 1054
IDC_DISP_BIT_27							equ 1055
IDC_DISP_BIT_28							equ 1056
IDC_DISP_BIT_29							equ 1057
IDC_DISP_BIT_30							equ 1058
IDC_DISP_BIT_31							equ 1059

;------------------------ IDs For Instructions -------------------
IDC_ADD									equ 1017
IDC_OR									equ 1018
IDC_SUB									equ 1019
IDC_AND									equ 1020
IDC_MOV									equ 1021
IDC_XOR									equ 1022
;------------------------ IDs for Bit(d) && Bit(w) ----------------
IDC_BIT_W								equ 1023
IDC_BIT_D								equ 1024

;------------------------ IDs for Decoded HEX values --------------
IDC_INST_DEC							equ 1025
IDC_MODRM_DEC							equ 1026
IDC_SIB_DEC								equ 1073

;--------------------
COLOR_RED			equ 000000FFh	; RED Color
COLOR_BLUE			equ 00FF0000h	; BLUE Color
COLOR_GREEN			equ 00239651h	; DARK GREEN Color
COLOR_PURPULE		equ 00FD02F0h	; PURPULE Color

;--------------------
FirstControlID		equ 1001		; ID of the first control
LastControlID		equ 1085		; ID of the last control
;--------------------

;-------------------------------------------------------------------------------------------

.data?

hInstance			dd 	?		; Instance of the program
hWnd				dd	?		; Handle of the main dialog

;------------------------------------------------------------ Colors ------------------------

RedControlHwnd		dd	6 dup (?)	; Handles Of the controls that will be in RED
BlueControlHwnd		dd	1 dup (?)	; Handles Of the controls that will be in BLUE
GreenControlHwnd	dd	9 dup (?)	; Handles Of the controls that will be in DARK GREEN
PurpuleControlHwnd	dd	8 dup (?)	; Handles Of the controls that will be in PURPULE

;--------------------
COLOR_DLG			dd	?			; Dialog-Box Color
hBrushTransperant	dd	?			; Handle of a (Dialog-Color) brush
;--------------------


