.const
OperandSizeFirstID		equ 	1001	; The ID of the first control (used for setting fonts)
OperandSizeLastID		equ 	1015	; The ID of the last  control (used for setting fonts)

;--------------------------------------------------------------------------------
IDC_OPERAND_EXP1		equ 	1004	; ID of the (STATIC) control for 'Example 1'
IDC_OPERAND_EXP2		equ		1010	; ID of the (STATIC) control for 'Example 3'
IDC_OPERAND_EXP3		equ		1011	; ID of the (STATIC) control for 'Example 2'
IDC_OPERAND_EXP4		equ 	1013

IDC_ON					equ		1005	; ID of the CommandButton 'On'
IDC_OFF					equ 	1007	; ID of the CommandButton 'Off'
;--------------------------------------------------------------------------------

;--------------------------------------------------------------------------------
OperandSizeOn1			db	"   8BC0                      MOV EAX, EAX",0
OperandSizeOff1			db	"66:8BC0                        MOV AX, AX",0

OperandSizeOn2			db	"   8B00       MOV EAX, DWORD PTR DS:[EAX]",0
OperandSizeOff2			db	"66:8B00          MOV AX, WORD PTR DS:[AX]",0

OperandSizeOn3			db	"   B8 01000000                  MOV EAX,1",0
OperandSizeOff3			db	"66:B8 0100                       MOV AX,1",0

OperandSizeOn4			db	"   AD             LODS DWORD PTR DS:[ESI]",0
OperandSizeOff4			db	"66:AD              LODS WORD PTR DS:[ESI]",0
;----------------------------------------------------------------------------------------

.data?
OperandSize_ColorCTL	dd	10 dup (?)			; 10 Controls to be colored (10 HWNDs)
.code
OperandSizeProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov eax, uMsg
	.if eax==WM_INITDIALOG
;----------------------------------------Save some handles to COLOR the examples :)------
		invoke GetDlgItem,hWin,IDC_OPERAND_EXP1
		mov OperandSize_ColorCTL[0*4],eax
		invoke GetDlgItem,hWin,IDC_OPERAND_EXP2
		mov OperandSize_ColorCTL[1*4],eax
		invoke GetDlgItem,hWin,IDC_OPERAND_EXP3
		mov OperandSize_ColorCTL[2*4],eax
		invoke GetDlgItem,hWin,IDC_OPERAND_EXP4
		mov OperandSize_ColorCTL[3*4],eax
		invoke GetDlgItem,hWin,1001				; Title
		mov OperandSize_ColorCTL[4*4],eax
		invoke GetDlgItem,hWin,1003				; Comment At The Button
		mov OperandSize_ColorCTL[5*4],eax		
		invoke GetDlgItem,hWin,1006				; Example1 (Static)
		mov OperandSize_ColorCTL[6*4],eax		
		invoke GetDlgItem,hWin,1009				; Example2 (Static)
		mov OperandSize_ColorCTL[7*4],eax		
		invoke GetDlgItem,hWin,1012				; Example3 (Static)
		mov OperandSize_ColorCTL[8*4],eax		
		invoke GetDlgItem,hWin,1014				; Example4 (Static)
		mov OperandSize_ColorCTL[9*4],eax		
;----------------------------------------------------------- Set fonts ------------------
		invoke SetWindowFont, hWin, OperandSizeFirstID, OperandSizeLastID
;----------------------------------------------------------------------------------------
	.elseif eax==WM_COMMAND
		mov eax, wParam
		and eax, 0FFFFh
		.if eax==IDC_ON
			invoke GetDlgItem,hWin,IDC_ON		; Disable the 'ON' Button
			invoke EnableWindow,eax,FALSE
			invoke GetDlgItem,hWin,IDC_OFF		; Enable the 'OFF' Button
			invoke EnableWindow,eax,TRUE		
			invoke SetDlgItemText,hWin,IDC_OPERAND_EXP1,addr OperandSizeOff1  ;------------	
			invoke SetDlgItemText,hWin,IDC_OPERAND_EXP2,addr OperandSizeOff2  ; Update the		
			invoke SetDlgItemText,hWin,IDC_OPERAND_EXP3,addr OperandSizeOff3  ;  examples.
			invoke SetDlgItemText,hWin,IDC_OPERAND_EXP4,addr OperandSizeOff4  ;------------
;----------------------------------------------------------------------------------------
		.elseif eax==IDC_OFF
			invoke GetDlgItem,hWin,IDC_ON		; Enable the 'ON' Button
			invoke EnableWindow,eax,TRUE	
			invoke GetDlgItem,hWin,IDC_OFF		; Disable the 'OFF' Button
			invoke EnableWindow,eax,FALSE						
			invoke SetDlgItemText,hWin,IDC_OPERAND_EXP1,addr OperandSizeOn1	 ;------------
			invoke SetDlgItemText,hWin,IDC_OPERAND_EXP2,addr OperandSizeOn2	 ; Update the
			invoke SetDlgItemText,hWin,IDC_OPERAND_EXP3,addr OperandSizeOn3  ;  examples.
			invoke SetDlgItemText,hWin,IDC_OPERAND_EXP4,addr OperandSizeOn4	 ;------------
		.endif
;----------------------------------------------------Main Coloring Procedure-------------
	.elseif eax==WM_CTLCOLORSTATIC
		mov eax, lParam
		.if eax==OperandSize_ColorCTL[0*4] 	|| eax==OperandSize_ColorCTL[1*4] \ 
			|| eax==OperandSize_ColorCTL[2*4] ||  eax==OperandSize_ColorCTL[3*4]
			;-------------------------------------
			;		Controls With BLUE Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,00F73833h
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret	
		.elseif eax==OperandSize_ColorCTL[4*4]
			;-------------------------------------
			;		Controls With RED Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,000000FFh
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret			
		.elseif eax==OperandSize_ColorCTL[5*4]	|| eax==OperandSize_ColorCTL[6*4] \ 
			|| eax==OperandSize_ColorCTL[7*4] ||  eax==OperandSize_ColorCTL[8*4] \
			|| eax==OperandSize_ColorCTL[9*4]
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
OperandSizeProc endp