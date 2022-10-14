.data?
LOCK_ColorCTL		dd	2 dup (?)
.code
LOCKProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
	mov eax, uMsg
	.if eax==WM_INITDIALOG
		invoke GetDlgItem,hWin,1002			; Title
		mov LOCK_ColorCTL[0*4], eax
		invoke GetDlgItem,hWin,1003
		mov LOCK_ColorCTL[1*4], eax			; Comment at the bottom
		
;----------------------------------------------------------- Set fonts ------------------
		invoke SetWindowFont, hWin, 1001, 1004
;----------------------------------------------------------------------------------------
	.elseif eax==WM_CTLCOLORSTATIC
		mov eax, lParam
		.if eax==LOCK_ColorCTL[0*4]
			;-------------------------------------
			;		Controls With RED Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,000000FFh
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret						
		.endif
		.if eax==LOCK_ColorCTL[1*4]
			;-------------------------------------
			;	Controls With DARK GREEN Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,001A7B39h
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret						
		.endif		
	.else
		mov		eax,FALSE
		ret
	.endif
	mov		eax,TRUE
	ret

LOCKProc endp