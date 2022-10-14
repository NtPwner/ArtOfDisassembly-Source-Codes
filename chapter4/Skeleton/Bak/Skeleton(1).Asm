.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include Skeleton.inc
include Msgs.inc
include DsmListView.inc
include MenuHandling.inc

.code


start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DLGMAIN,NULL,addr DlgProc,NULL

	invoke ExitProcess,0

;########################################################################

PrepareListView	proc
;-----------------------------------------------------------------------;
;   	Prepare the main ListView and changes its color & style			;
;-----------------------------------------------------------------------;

   	Invoke GetDlgItem,hWnd,IDC_DASM										; Gets its hWnd
   	mov hDsmList, eax												
    invoke SendMessage, hDsmList, LVM_SETTEXTCOLOR, 0, 0FE7000h			; Sets its color 
	invoke SendMessage, hDsmList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0,\		; Sets its style
			   LVS_EX_FULLROWSELECT or \
			   LVS_EX_GRIDLINES + LVS_EX_FLATSB

	ret
PrepareListView endp


DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG
	;-------------------------------------- Initializing our dialog --------

		;===========================================
		; 	 Store the handle of our main dialog
		;===========================================
		push hWin
		pop hWnd
		
		;===========================================
		;			Prepare our ListView
		;===========================================
		
			invoke PrepareListView
	
		;===========================================
		;	Initialize the OPENFILENAME structure
		;===========================================
		
			mov ofn.lStructSize, SIZEOF ofn
			push hWnd
			pop ofn.hwndOwner
	        push hInstance 
        	pop  ofn.hInstance 
        	mov  ofn.lpstrFilter, OFFSET StrFilter 		; ( *.exe & *.dll )
        	mov  ofn.lpstrFile, OFFSET buffer 			; ( Store the file name )
        	mov  ofn.nMaxFile, 260 
        	
			; ( File must exists/ Hide read only files)
        	mov  ofn.Flags, OFN_FILEMUSTEXIST or \ 		
        		 OFN_PATHMUSTEXIST or OFN_LONGNAMES or\ 
                 OFN_EXPLORER or OFN_HIDEREADONLY 
		
		 ;============================================
		 ;  		   Store our menu handle
		 ;============================================
		 
			invoke GetMenu,hWnd
			mov hMenu, eax							; Store the handle of the menu
			
			invoke EnableMenuItem,hMenu,IDM_CLOSE,MF_BYCOMMAND or MF_GRAYED ; Disable 'Close' item

	;-----------------------------------------------------------------------
	.elseif eax==WM_COMMAND
		mov eax, wParam
		.if ax==IDM_OPEN
          	invoke MnuFileOpen
		.elseif	ax==IDM_CLOSE
			invoke MnuFileClose
		.elseif	ax==IDM_EXIT
			invoke MnuFileClose
			invoke ExitProcess,0
		.endif
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
