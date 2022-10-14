.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include ModRM II.inc

.code

start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DIALOG1,NULL,addr DlgProc,NULL
	invoke ExitProcess,0

;########################################################################

DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
	LOCAL dwTemp:DWORD
	LOCAL ChangedBit:DWORD
	
	mov		eax,uMsg
	.if eax==WM_INITDIALOG
		push hWin
		pop hWnd
;==============================================================================
    	invoke CreateFont,0,0,0,0,500,0,0,0,DEFAULT_CHARSET,\ 
                                   OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,\ 
                                   DEFAULT_QUALITY,DEFAULT_PITCH or FF_SCRIPT,\ 
                                   ADDR FontName	
		mov hFont, eax
		mov dwTemp, IDC_FIRST
	@@:		
		invoke GetDlgItem,hWnd,dwTemp		
		invoke SendMessage,eax,WM_SETFONT,hFont,TRUE
		inc dwTemp
		cmp dwTemp, IDC_LAST
		jbe @B
;==============================================================================

	.elseif eax==WM_COMMAND
		mov		eax,wParam
		and		eax,0FFFFh
		mov 	ChangedBit, eax
;---------------------------------------------------------
		invoke GetDlgItemInt,hWnd,eax,0,0
		.if eax==0
			invoke SetDlgItemInt,hWnd,ChangedBit,1,0
		.else
			invoke SetDlgItemInt,hWnd,ChangedBit,0,0
		.endif		
;---------------------------------------------------------
			invoke GetDlgItemInt,hWnd,ChangedBit,0,0
			add al, '0'
;---------------------------------------------------------
		.if ChangedBit==IDC_BIT_0
			mov byte ptr [Mode], al
		.elseif ChangedBit==IDC_BIT_1
			mov byte ptr [Mode+1], al		
		.elseif ChangedBit==IDC_BIT_2
			mov byte ptr [Reg1], al
			invoke UpdateReg1
		.elseif ChangedBit==IDC_BIT_3
			mov byte ptr [Reg1+1], al
			invoke UpdateReg1
		.elseif ChangedBit==IDC_BIT_4
			mov byte ptr [Reg1+2], al
			invoke UpdateReg1
		.elseif ChangedBit==IDC_BIT_5
			mov byte ptr [Reg2], al
		.elseif ChangedBit==IDC_BIT_6
			mov byte ptr [Reg2+1], al
		.elseif ChangedBit==IDC_BIT_7
			mov byte ptr [Reg2+2], al		
		.endif		
;---------------------------------------------------------
		invoke wsprintf, addr szBuffer, addr Decoding, dwMode, dwReg1, dwReg2
		invoke SetDlgItemText,hWnd,IDC_DECODE,addr szBuffer
;---------------------------------------------------------
	.elseif eax==WM_CLOSE
		invoke EndDialog,hWin,0
	.else
		mov		eax,FALSE
		ret
	.endif
	mov		eax,TRUE
	ret

DlgProc endp

end start
