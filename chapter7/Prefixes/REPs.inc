.const
REP_REPNE_FirstID		equ 	1001	; The ID of the first control (used for setting fonts)
REP_REPNE_LastID		equ 	1016	; The ID of the last  control (used for setting fonts)

;--------------------------------------------------------------------------------
IDC_REP_EXP1			equ 	1015
IDC_REP_EXP2			equ		1014
IDC_REP_EXP3			equ		1013
IDC_REP_EXP4			equ 	1012

IDC_REP_F3				equ		1008
IDC_REP_F2				equ 	1009
IDC_REP_NA				equ 	1001
;--------------------------------------------------------------------------------

;--------------------------------------------------------------------------------
REP_REPE_REPNE_F3_1		db	"F3:AD         REP LODS DWORD PTR DS:[ESI]",0
REP_REPE_REPNE_F2_1		db	"F2:AD       REPNE LODS DWORD PTR DS:[ESI]",0
REP_REPE_REPNE_NA_1		db	"   AD             LODS DWORD PTR DS:[ESI]",0

REP_REPE_REPNE_F3_2		db	"F3:AE          REP SCAS BYTE PTR ES:[EDI]",0
REP_REPE_REPNE_F2_2		db	"F2:AE        REPNE SCAS BYTE PTR ES:[EDI]",0
REP_REPE_REPNE_NA_2		db	"   AE              SCAS BYTE PTR ES:[EDI]",0

REP_REPE_REPNE_F3_3		db	"F3:AD         REP STOS DWORD PTR DS:[EDI]",0
REP_REPE_REPNE_F2_3		db	"F2:AD       REPNE STOS DWORD PTR DS:[EDI]",0
REP_REPE_REPNE_NA_3		db	"   AD             STOS DWORD PTR DS:[EDI]",0

REP_REPE_REPNE_F3_4		db	"F3:6D       REP INS DWORD PTR ES:[EDI],DX",0
REP_REPE_REPNE_F2_4		db	"F2:6D     REPNE INS DWORD PTR ES:[EDI],DX",0
REP_REPE_REPNE_NA_4		db	"   6D           INS DWORD PTR ES:[EDI],DX",0
;--------------------------------------------------------------------------------

.data?
REP_REPNE_ColorCTL		dd	10 dup (?)	; 10 Controls to be colored (10 HWNDs)

.code
REPsProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
	mov eax, uMsg
	.if eax==WM_INITDIALOG
;----------------------------------------Save some handles to COLOR the examples :)------
		invoke GetDlgItem,hWin,IDC_REP_EXP1		;
		mov REP_REPNE_ColorCTL[0*4],eax
		invoke GetDlgItem,hWin,IDC_REP_EXP2		;
		mov REP_REPNE_ColorCTL[1*4],eax
		invoke GetDlgItem,hWin,IDC_REP_EXP3		;
		mov REP_REPNE_ColorCTL[2*4],eax
		invoke GetDlgItem,hWin,IDC_REP_EXP4		; 
		mov REP_REPNE_ColorCTL[3*4],eax
		invoke GetDlgItem,hWin,1002				; Title
		mov REP_REPNE_ColorCTL[4*4],eax
		invoke GetDlgItem,hWin,1003				; Comment At The Button
		mov REP_REPNE_ColorCTL[5*4],eax		
		invoke GetDlgItem,hWin,1007				; Example1 (Static)
		mov REP_REPNE_ColorCTL[6*4],eax		
		invoke GetDlgItem,hWin,1006				; Example2 (Static)
		mov REP_REPNE_ColorCTL[7*4],eax		
		invoke GetDlgItem,hWin,1005				; Example3 (Static)
		mov REP_REPNE_ColorCTL[8*4],eax		
		invoke GetDlgItem,hWin,1004				; Example4 (Static)
		mov REP_REPNE_ColorCTL[9*4],eax		
;----------------------------------------------------------- Set fonts ------------------
		invoke SetWindowFont, hWin, REP_REPNE_FirstID, REP_REPNE_LastID
;----------------------------------------------------------------------------------------
	.elseif eax==WM_COMMAND
		mov eax, wParam
		and eax, 0FFFFh
		.if eax==IDC_REP_F3
			invoke GetDlgItem,hWin,IDC_REP_F3
			invoke EnableWindow,eax,FALSE
			invoke GetDlgItem,hWin,IDC_REP_F2
			invoke EnableWindow,eax,TRUE	
			invoke GetDlgItem,hWin,IDC_REP_NA
			invoke EnableWindow,eax,TRUE			
			invoke SetDlgItemText,hWin,IDC_REP_EXP1,addr REP_REPE_REPNE_F3_1		
			invoke SetDlgItemText,hWin,IDC_REP_EXP2,addr REP_REPE_REPNE_F3_2		
			invoke SetDlgItemText,hWin,IDC_REP_EXP3,addr REP_REPE_REPNE_F3_3
			invoke SetDlgItemText,hWin,IDC_REP_EXP4,addr REP_REPE_REPNE_F3_4
;----------------------------------------------------------------------------------------
		.elseif eax==IDC_REP_F2
			invoke GetDlgItem,hWin,IDC_REP_F3
			invoke EnableWindow,eax,TRUE		
			invoke GetDlgItem,hWin,IDC_REP_F2
			invoke EnableWindow,eax,FALSE
			invoke GetDlgItem,hWin,IDC_REP_NA
			invoke EnableWindow,eax,TRUE																		
			invoke SetDlgItemText,hWin,IDC_REP_EXP1,addr REP_REPE_REPNE_F2_1		
			invoke SetDlgItemText,hWin,IDC_REP_EXP2,addr REP_REPE_REPNE_F2_2		
			invoke SetDlgItemText,hWin,IDC_REP_EXP3,addr REP_REPE_REPNE_F2_3		
			invoke SetDlgItemText,hWin,IDC_REP_EXP4,addr REP_REPE_REPNE_F2_4
		.elseif eax==IDC_REP_NA
			invoke GetDlgItem,hWin,IDC_REP_F3
			invoke EnableWindow,eax,TRUE
			invoke GetDlgItem,hWin,IDC_REP_F2
			invoke EnableWindow,eax,TRUE	
			invoke GetDlgItem,hWin,IDC_REP_NA
			invoke EnableWindow,eax,FALSE									
			invoke SetDlgItemText,hWin,IDC_REP_EXP1,addr REP_REPE_REPNE_NA_1		
			invoke SetDlgItemText,hWin,IDC_REP_EXP2,addr REP_REPE_REPNE_NA_2		
			invoke SetDlgItemText,hWin,IDC_REP_EXP3,addr REP_REPE_REPNE_NA_3		
			invoke SetDlgItemText,hWin,IDC_REP_EXP4,addr REP_REPE_REPNE_NA_4					
		.endif
;----------------------------------------------------Main Coloring Procedure-------------
	.elseif eax==WM_CTLCOLORSTATIC
		mov eax, lParam
		.if eax==REP_REPNE_ColorCTL[0*4] 	|| eax==REP_REPNE_ColorCTL[1*4] \ 
			|| eax==REP_REPNE_ColorCTL[2*4] ||  eax==REP_REPNE_ColorCTL[3*4]
			;-------------------------------------
			;		Controls With BLUE Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,00F73833h
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret	
		.elseif eax==REP_REPNE_ColorCTL[4*4]
			;-------------------------------------
			;		Controls With RED Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,000000FFh
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret			
		.elseif eax==REP_REPNE_ColorCTL[5*4]	|| eax==REP_REPNE_ColorCTL[6*4] \ 
			|| eax==REP_REPNE_ColorCTL[7*4] ||  eax==REP_REPNE_ColorCTL[8*4] \
			|| eax==REP_REPNE_ColorCTL[9*4]
			;-------------------------------------
			;	Controls With DARK GREEN Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,001A7B39h
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret					
		.endif						
;----------------------------------------------------------------------------------------
	.else
		mov		eax,FALSE
		ret
	.endif
	mov		eax,TRUE
	ret

REPsProc endp
