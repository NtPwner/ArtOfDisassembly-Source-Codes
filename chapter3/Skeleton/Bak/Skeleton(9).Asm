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

MnuFileOpen proc
		;------------------------------------- Open file -------------------
		
            ; Display the "Open File" dialog box
           	invoke GetOpenFileName, ADDR ofn 
           	
        .if eax==TRUE 			; Didn't click CANCEL
                 
                 ;======================================
                 ;     Get the handle of the file
                 ;======================================
                 
   
                 invoke CreateFile,ADDR buffer,\ 
                             GENERIC_READ or GENERIC_WRITE ,\ 
                             FILE_SHARE_READ + FILE_SHARE_WRITE,\ 
                             NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,\ 
                             NULL                              
                 mov hFile,eax 					; Stores the handle of the file
                 inc eax						; Is eax = -1 ?? (Error opening)
                 jnz @f							; Jump if no error (eax !- -1)
                 
                 ;======================================
                 ; 	   	  Couldn't open the file	
                 ;======================================
                 
                 invoke MessageBox, hWnd, addr MsgErrOpen, addr MsgErrCaption,MB_ICONINFORMATION
                 mov hFile, 0					; Empty the file handle
                 ret
	@@:
                               
                 ;====================================
                 ;		Create file mapping object
                 ;====================================
                 
                 invoke CreateFileMapping,hFile,0,PAGE_READWRITE,0,0,0
                 mov hFileMapping,eax			; Store the handle of the file mapping object

                 ;===============================================
                 ; 	   	  Couldn't create file mapping object
                 ;===============================================
                 test eax, eax
                 jnz @f
                 
                 invoke MessageBox, hWnd, addr MsgErrFileMap, addr MsgErrCaption,MB_ICONINFORMATION
                 mov hFileMapping, 0
                 ret
	@@:

                 ;====================================
                 ;			Get the file size
                 ;====================================
                                  
                 invoke GetFileSize,hFile,0
                 mov FileSize, eax				; Store the file size
                 
                 ;===============================================
                 ; 	   	  		File is 0 byte =)
                 ;===============================================
                 test eax, eax
                 jnz @f
                 
                 invoke MessageBox, hWnd, addr MsgErrSize, addr MsgErrCaption,MB_ICONINFORMATION
                 ret
	@@:						                  
                 
                 ;====================================
                 ;		Map the file to memory
                 ;====================================
                                  
                 invoke MapViewOfFile,hFileMapping,FILE_MAP_READ or FILE_MAP_WRITE,0,0,FileSize
                 mov FileOffset, eax

                 ;===============================================
                 ; 	 	Couldn't map the file to memory
                 ;===============================================
                 test eax, eax
                 jnz @f
                 
                 invoke MessageBox, hWnd, addr MsgErrMapView, addr MsgErrCaption,MB_ICONINFORMATION
                 ret
	@@:	
                                
         .endif 
	
        ;---------------------------------------------------------------------		
	ret
MnuFileOpen endp

MnuFileClose proc
	cmp hFileMapping, 0
	jnz @f
		ret
 @@:
 	invoke UnmapViewOfFile,FileOffset
 	invoke CloseHandle,hFile
 	mov FileOffset, 0
 	mov hFile, 0 	
	ret
MnuFileClose endp

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
			mov hMenu, eax							; Store the handle of the mennu
			
			invoke EnableMenuItem,hMenu,IDM_CLOSE,MF_BYCOMMAND or MF_GRAYED

	;-----------------------------------------------------------------------
	.elseif eax==WM_COMMAND
		mov eax, wParam
		.if ax==IDM_OPEN
          	invoke MnuFileOpen        	
		.elseif	ax==IDM_CLOSE
			invoke MnuFileClose
			invoke EnableMenuItem,hMenu,IDM_CLOSE,MF_BYCOMMAND or MF_GRAYED
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
