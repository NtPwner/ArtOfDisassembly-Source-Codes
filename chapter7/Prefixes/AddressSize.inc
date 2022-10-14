.data?
AddressSize_ColorCTL	dd	10 dup (?)			; 10 Controls to be colored (10 HWNDs)

.const
AddressSizeFirstID		equ 	1001	; The ID of the first control (used for setting fonts)
AddressSizeLastID		equ 	1015	; The ID of the last  control (used for setting fonts)

;--------------------------------------------------------------------------------
IDC_ADDRESS_EXP1		equ 	1009	; ID of the (STATIC) control for 'Example 1'
IDC_ADDRESS_EXP2		equ		1007	; ID of the (STATIC) control for 'Example 2'
IDC_ADDRESS_EXP3		equ		1005	; ID of the (STATIC) control for 'Example 3'
IDC_ADDRESS_EXP4		equ 	1003	; ID of the (STATIC) control for 'Example 4'

IDC_ADDRESS_ON			equ		1012	; ID of the CommandButton 'On'
IDC_ADDRESS_OFF			equ 	1013	; ID of the CommandButton 'Off'
;--------------------------------------------------------------------------------

;--------------------------------------------------------------------------------
AddressSizeOn1			db	"   8B00        MOV EAX,DWORD PTR DS:[EAX]",0
AddressSizeOff1			db	"67:8B00      MOV EAX,DWORD PTR DS:[BX+SI]",0

AddressSizeOn2			db	"   8B01        MOV EAX,DWORD PTR DS:[ECX]",0
AddressSizeOff2			db	"67:8B01      MOV EAX,DWORD PTR DS:[BX+DI]",0

AddressSizeOn3			db	"   8B02        MOV EAX,DWORD PTR DS:[EDX]",0
AddressSizeOff3			db	"67:8B02      MOV EAX,DWORD PTR SS:[BP+SI]",0

AddressSizeOn4			db	"   8B03        MOV EAX,DWORD PTR DS:[EBX]",0
AddressSizeOff4			db	"67:8B03      MOV EAX,DWORD PTR SS:[BP+DI]",0
;--------------------------------------------------------------------------------

.code
AddressSizeProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov eax, uMsg
	.if eax==WM_INITDIALOG
;----------------------------------------Save some handles to COLOR the examples :)------
		invoke GetDlgItem,hWin,IDC_ADDRESS_EXP1
		mov AddressSize_ColorCTL[0*4],eax
		invoke GetDlgItem,hWin,IDC_ADDRESS_EXP2
		mov AddressSize_ColorCTL[1*4],eax
		invoke GetDlgItem,hWin,IDC_ADDRESS_EXP3
		mov AddressSize_ColorCTL[2*4],eax
		invoke GetDlgItem,hWin,IDC_ADDRESS_EXP4
		mov AddressSize_ColorCTL[3*4],eax
		invoke GetDlgItem,hWin,1001				; Title
		mov AddressSize_ColorCTL[4*4],eax
		invoke GetDlgItem,hWin,1002				; Comment At The Button
		mov AddressSize_ColorCTL[5*4],eax		
		invoke GetDlgItem,hWin,1010				; Example1 (Static)
		mov AddressSize_ColorCTL[6*4],eax		
		invoke GetDlgItem,hWin,1008				; Example2 (Static)
		mov AddressSize_ColorCTL[7*4],eax		
		invoke GetDlgItem,hWin,1006				; Example3 (Static)
		mov AddressSize_ColorCTL[8*4],eax		
		invoke GetDlgItem,hWin,1004				; Example4 (Static)
		mov AddressSize_ColorCTL[9*4],eax		
;----------------------------------------------------------- Set fonts ------------------
		invoke SetWindowFont, hWin, AddressSizeFirstID, AddressSizeLastID
;----------------------------------------------------------------------------------------
	.elseif eax==WM_COMMAND
		mov eax, wParam
		and eax, 0FFFFh
		.if eax==IDC_ADDRESS_ON
			invoke GetDlgItem,hWin,IDC_ADDRESS_ON		; Disable the 'ON' Button
			invoke EnableWindow,eax,FALSE
			invoke GetDlgItem,hWin,IDC_ADDRESS_OFF		; Enable the 'OFF' Button
			invoke EnableWindow,eax,TRUE	
			invoke SetDlgItemText,hWin,IDC_ADDRESS_EXP1,addr AddressSizeOff1		
			invoke SetDlgItemText,hWin,IDC_ADDRESS_EXP2,addr AddressSizeOff2		
			invoke SetDlgItemText,hWin,IDC_ADDRESS_EXP3,addr AddressSizeOff3
			invoke SetDlgItemText,hWin,IDC_ADDRESS_EXP4,addr AddressSizeOff4
;----------------------------------------------------------------------------------------
		.elseif eax==IDC_ADDRESS_OFF
			invoke GetDlgItem,hWin,IDC_ADDRESS_ON		; Enable the 'ON' Button
			invoke EnableWindow,eax,TRUE
			invoke GetDlgItem,hWin,IDC_ADDRESS_OFF		; Disable the 'OFF' Button
			invoke EnableWindow,eax,FALSE						
			invoke SetDlgItemText,hWin,IDC_ADDRESS_EXP1,addr AddressSizeOn1		
			invoke SetDlgItemText,hWin,IDC_ADDRESS_EXP2,addr AddressSizeOn2		
			invoke SetDlgItemText,hWin,IDC_ADDRESS_EXP3,addr AddressSizeOn3		
			invoke SetDlgItemText,hWin,IDC_ADDRESS_EXP4,addr AddressSizeOn4		
		.endif
;----------------------------------------------------Main Coloring Procedure-------------
	.elseif eax==WM_CTLCOLORSTATIC
		mov eax, lParam
		.if eax==AddressSize_ColorCTL[0*4] 	|| eax==AddressSize_ColorCTL[1*4] \ 
			|| eax==AddressSize_ColorCTL[2*4] ||  eax==AddressSize_ColorCTL[3*4]
			;-------------------------------------
			;		Controls With BLUE Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,00F73833h
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret	
		.elseif eax==AddressSize_ColorCTL[4*4]
			;-------------------------------------
			;		Controls With RED Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,000000FFh
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret			
		.elseif eax==AddressSize_ColorCTL[5*4]	|| eax==AddressSize_ColorCTL[6*4] \ 
			|| eax==AddressSize_ColorCTL[7*4] ||  eax==AddressSize_ColorCTL[8*4] \
			|| eax==AddressSize_ColorCTL[9*4]
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
AddressSizeProc endp