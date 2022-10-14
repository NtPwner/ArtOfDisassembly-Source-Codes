.686
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include Misc.inc
include Instructions.inc
include Data.inc
include Structs.inc
include AoD Beta.inc
include DsmListView.inc
include EventHandler.inc
include EngineSkeleton.inc
include JumpTable.inc
include PEStuff.inc
include Msgs.inc
include MenuHandling.inc
include Functions.inc



.code


start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DLGMAIN,NULL,addr DlgProc,NULL

	invoke ExitProcess,0

ImportDlgProc proc uses ebx hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG
		push hWin
		pop hImportsDlg
		invoke GetDlgItem, hImportsDlg,IDC_IMPORTS
		mov hImportTree, eax
		invoke GetImportTable
		;----------------------------------------
			invoke GetImports
		;----------------------------------------
	.elseif eax==WM_COMMAND
	
	.elseif eax==WM_CLOSE
		invoke EndDialog,hWin,NULL
	.else
		mov eax,FALSE
		ret
	.endif
	mov  eax,TRUE
	ret

ImportDlgProc endp

DlgProc proc uses ebx hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG
		 Push hWin
		 Pop	 hWnd 
		
		 ;============================================
		 ;  		   Store our menu handle
		 ;============================================
		 
			invoke GetMenu,hWnd
			mov hMenu, eax							; Store the handle of the menu
			
			invoke EnableMenuItem,hMenu,IDM_CLOSE,MF_BYCOMMAND or MF_GRAYED ; Disable 'Close' item
			invoke EnableMenuItem,hMenu,IDM_IMPORTS,MF_BYCOMMAND or MF_GRAYED ; ... Tools/Imports
			
		 ;============================================
		 ;  		   Prepare our events handler
		 ;============================================
		 
			invoke PrepareListView
			invoke PrepareEventHandler
			
		 ;============================================
		 ;	 		Prepare the progress bar
		 ;============================================
		 
		 	invoke GetDlgItem, hWnd, IDC_PROGBAR
		 	mov hProgressBar, eax					
			
			invoke ShowWindow,hWnd, SW_SHOWMAXIMIZED			
			jmp WindowWasResized
			
		
	;========================================================================================
		 ;================================================
		 ;  		Handles our ListBox colors
		 ;================================================
	
	.elseif eax==WM_CTLCOLORLISTBOX
		invoke SetBkColor,wParam, dwListBoxBack					; Sets the ListBox BackColor
	    invoke SetTextColor, wParam, dwListBoxFore				; Sets the ListVox ForeColor
		mov eax, hListBoxBack
		ret
	;========================================================================================
	.elseif eax==WM_COMMAND
		mov eax, wParam
		.if ax==IDM_OPEN
          	invoke MnuFileOpen,hWnd ; Added By Ben [NEW]
		.elseif	ax==IDM_CLOSE
			invoke MnuFileClose
		.elseif	ax==IDM_EXIT
			invoke MnuFileClose
			invoke ExitProcess,0
		.elseif ax==IDM_IMPORTS
			;========================================
			; Pause the disassembler if it's working
			;========================================
			.if hThread != 0
				invoke SuspendThread,hThread	
			.endif
			invoke DialogBoxParam,hInstance,IDD_IMPORTS,hWnd,addr ImportDlgProc,NULL
			.if hThread != 0
				invoke ResumeThread,hThread	
			.endif			
		.endif
	.elseif eax==WM_CLOSE
		invoke EndDialog,hWin,0
	.elseif eax==WM_SIZE
WindowWasResized:	
		;========================================= 
		;	The disassembler' windows was resized
		;=========================================
		
		;-------------
		; Redraw ListView
		;-------------
		invoke GetWindowRect, hWnd, addr dsmRECT
		mov eax, dsmRECT.left
		mov ebx, dsmRECT.right
		add eax, 10
		sub ebx, eax				; Width  = right-left (ebx)
		mov eax, dsmRECT.bottom
		mov ecx, dsmRECT.top
		add ecx, 180				; (Event ListBox Height)
		sub eax, ecx				; Height = top-bottom (eax)
		
		push eax
		push ebx
		invoke MoveWindow,hDsmList,2,2,ebx, eax, 1
		pop ebx
		pop eax		
		push ebx
		push eax
		;-------------
		; Redraw ListBox
		;-------------
		add eax, 5
		invoke MoveWindow,hEventHandler,2,eax,ebx,110, 1
		pop eax
		pop ebx
		add eax, 117
		invoke MoveWindow, hProgressBar, 2, eax,ebx,8, 1 		
	.else
		mov		eax,FALSE
		ret
	.endif
	mov		eax,TRUE
	ret

DlgProc endp
end start
