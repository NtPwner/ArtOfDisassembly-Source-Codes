.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include Skeleton.inc

.code

start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DLGMAIN,NULL,addr DlgProc,NULL

	invoke ExitProcess,0

;########################################################################

PrepareListView	proc
;-----------------------------------------------------------------------
;   	Prepare the main ListView and changes its color & style
;-----------------------------------------------------------------------
   	Invoke GetDlgItem,hWnd,IDC_DASM
   	mov hDsmList, eax
    invoke SendMessage, hDsmList, LVM_SETTEXTCOLOR, 0, 0FE7000h
	invoke SendMessage, hDsmList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0,\
			   LVS_EX_FULLROWSELECT or \
			   LVS_EX_GRIDLINES + LVS_EX_FLATSB

	ret
PrepareListView endp

DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG
	
	.elseif eax==WM_COMMAND

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
