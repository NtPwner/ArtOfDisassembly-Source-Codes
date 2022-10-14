
.const
;-------------------------------------- Menu Items ----------------------
IDM_OPEN 			equ	10002
IDM_CLOSE 			equ	10003
IDM_EXIT 			equ 10004
IDM_IMPORTS			equ 10006
_MAX_PATH = 256    

.data
;-------------------------------------- File manipulating ---------------
ofn		OPENFILENAME	<>
szFileName              db  _MAX_PATH dup(?)
FileFilter              db  "Executable files (*.exe, *.dll)",0,"*.exe;*.dll",0,"All Files (*.*)",0,"*.*",0,0             			

.data?
;-------------------------------------- File manipulating ----------------

hFile					dd	?		; Handle of the file
FileSize				dd	?		; The file size ( GetFileSize() )
FileOffset				dd	?		; The offset of the mapped file in memory
hFileMapping			dd	?		; Handle of the file mapping object ( CreateFileMapping() )
hThread                 dd  ?       ; Handle to the Thread Function, Added ( CuTedEvil / Ben )
ThreadId				dd	?			
;-------------------------------------- Handles ---------------------------
hMenu					dd	?		; The handle to our manu


.code
IsFileLoaded proc
	
	             pushad ; Added ( CuTedEvil / Ben )
	             
	             ;======================================
                 ;		  Display Opening Event
                 ;======================================
                 invoke AddEvent,addr szFileLoading
                 
                 ;======================================
                 ;     Get the handle of the file
                 ;======================================
                 
                 ; Changed ( CuTedEvil / Ben )
                 invoke CreateFile,ADDR szFileName,\ 
                             GENERIC_READ or GENERIC_WRITE ,\ 
                             FILE_SHARE_READ + FILE_SHARE_WRITE,\ 
                             NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,\ 
                             NULL                              
                 mov hFile,eax 					; Stores the handle of the file
                 ; Added ( CuTedEvil / Ben )
                 .if eax!=0FFFFFFFFh            ; Is eax = -1 ?? (Error opening)
                    jmp @F
                 .endif
                 
                 ;======================================
                 ; 	   	  Couldn't open the file	
                 ;======================================
                 
                 ; Changed ( CuTedEvil / Ben )
                 invoke RaiseErr, addr MsgErrOpen
                 mov hFile,0					; Empty the file handle
                 popad 
                 mov eax,-1
                 ret
	@@:
				 invoke AddEvent, addr szFileName
                               
                 ;====================================
                 ;		Create file mapping object
                 ;====================================
                 
                 invoke CreateFileMapping,hFile,0,PAGE_READWRITE,0,0,NULL
                 mov hFileMapping,eax			; Store the handle of the file mapping object

                 ;===============================================
                 ; 	   	  Couldn't create file mapping object
                 ;===============================================
                 test eax, eax
                 jnz @f
                 
                 ; Changed ( CuTedEvil / Ben )
                 invoke RaiseErr, addr MsgErrFileMap
                 mov hFileMapping, 0			; Empty the FileMappingObject handle
                 popad
                 mov eax, -1					 ;Err Happened
                 ret
	@@:

                 ;====================================
                 ;			Get the file size
                 ;====================================
                                  
                 invoke GetFileSize,hFile,0
                 ; Added ( CuTedEvil / Ben )
                 .if eax==0FFFFFFFFh
                    invoke RaiseErr, addr MsgErrSize ;File is 0 byte  
                    mov eax,-1                       ; return value 
                    popad             
                    ret
                 .endif
                 mov FileSize, eax				; Store the file size
	@@:						                  
                 
                 ;======================================
                 ;			Event: File Size..
                 ;======================================
                 invoke wsprintf, addr szBuffer, addr szFileSize, FileSize
                 invoke AddEvent,addr szBuffer
 
                 ;====================================
                 ;		Map the file to memory
                 ;====================================
                                  
                 invoke MapViewOfFile,hFileMapping,FILE_MAP_ALL_ACCESS,0,0,0
                 mov FileOffset, eax

                 ;===============================================
                 ; 	 	Couldn't map the file to memory
                 ;===============================================
                 test eax, eax
                 jnz @f
                 
                 ; Changed ( CuTedEvil / Ben )
                 invoke RaiseErr, addr MsgErrMapView
                 popad
                 mov eax, -1
                 ret
	@@:	
	
	; Added ( CuTedEvil / Ben )
	popad 
	xor eax,eax  ; everything is fine!
	ret

IsFileLoaded endp

MnuFileClose proc
	cmp hFileMapping, 0						; File not mapped ?
	jnz @f									; 
		cmp hFile, 0						; File not created ?
		jnz @f								;
			invoke ResetEventHandler		; Reset the event handler
			invoke DisplayWelcomeMsg		; Display the welcome message
			ret
 @@:
 	invoke UnmapViewOfFile,FileOffset		; Unmap the file mapping object
 	invoke CloseHandle,hFileMapping         ; Close the Mapped File Handler, Added ( CuTedEvil / Ben )
 	invoke CloseHandle,hFile				; Close the file handle
 	mov FileOffset, 0
 	mov hFile, 0 
 	mov hFileMapping,0	                    ; Added ( CuTedEvil / Ben )
 	
 	;====================================
 	;  		Update the menu items		
 	;====================================

	invoke EnableMenuItem,hMenu,IDM_OPEN,MF_BYCOMMAND or MF_ENABLED
	invoke EnableMenuItem,hMenu,IDM_CLOSE,MF_BYCOMMAND or MF_GRAYED		
	invoke EnableMenuItem,hMenu,IDM_IMPORTS,MF_BYCOMMAND or MF_GRAYED
	
	;========================================
	;	Clear the date in the EventHandler
	;========================================
	
	invoke ResetEventHandler	
	invoke ResetListView
	invoke DisplayWelcomeMsg

	ret
MnuFileClose endp

MnuFileOpen proc _hWnd:HWND ; Changed ( CuTedEvil / Ben )
		
		;------------------------------------- Open file -------------------
		pushad
        
        
        ; Added ( CuTedEvil / Ben )
        ; Initializing the OFN Struct everytime we ReLoad a File
        ; Not Really needed, but i think its safer that way
        
        invoke RtlZeroMemory, addr ofn,sizeof OPENFILENAME
        mov ofn.lStructSize, sizeof OPENFILENAME
        mov eax, _hWnd
        mov ofn.hwndOwner,eax 
        mov ofn.lpstrFilter, offset FileFilter
        mov ofn.lpstrFile, offset szFileName
        mov ofn.nMaxFile, 256 ; MAX_FILE
        mov ofn.Flags,OFN_EXPLORER or OFN_FILEMUSTEXIST or OFN_HIDEREADONLY
        invoke GetOpenFileName, ADDR ofn 
           	
        .if eax!=0 			; Didn't click CANCEL, Changed ( CuTedEvil / Ben )
                 
				;======================================
				;	Load the file and map it to memory
				;======================================
				
				invoke IsFileLoaded
				.if eax!=-1	 ; if return value is != -1 than continue
				  jmp @F
				.endif
				;======================================
				;	File was not loaded successfully
				;======================================
				
				invoke MnuFileClose		; Reset disassembler
				popad
				ret
				 
	@@:				 
                ;======================================
                ;		Event: File Loaded Success...
                ;======================================
                 
                invoke wsprintf,addr szBuffer, addr szFileLoaded, FileOffset
                invoke AddEvent,addr szBuffer

				;====================================
				;  		Update the menu items        
				;====================================
			
				invoke EnableMenuItem,hMenu,IDM_OPEN,MF_BYCOMMAND or MF_GRAYED
 	        	invoke EnableMenuItem,hMenu,IDM_CLOSE,MF_BYCOMMAND or MF_ENABLED     
  	        	invoke EnableMenuItem,hMenu,IDM_IMPORTS,MF_BYCOMMAND or MF_ENABLED     
	        	  
 	        	 
 	        	;=======================================
 	        	;		Check for a valid Win32 file
 	        	;======================================= 	
 	        	invoke CheckSignatures
 	        	test eax, eax				; File Signatures Are Valid
 	        	jz @f
 	        	
 	        	;=======================================
 	        	;		Invalid Signatures Found
 	        	;=======================================
 	        	invoke MnuFileClose
 	        	popad
 	        	ret 	        	
	@@:
			    ;===================================
			    ;		Get Sections Number
			    ;===================================
			    invoke GetNumberOfSections
			    				
				;===================================
				;	Display Sections Information
				;===================================
				invoke GetSections
							
				;===================================
				;		Display the EntryPoint
				;===================================
				invoke GetEntryPoint
				
				;===================================
				;		  Get Code Section
				;===================================
				invoke GetCodeSection
				
				;===================================
				;		Parse Code Section
				;===================================
				;invoke EngineSkeleton (No multi-threading)
				invoke ShowWindow, hDsmList, SW_HIDE
				invoke ShowWindow, hProgressBar, SW_SHOW
				invoke ShowWindow, hWnd, SW_SHOWMINNOACTIVE
				
				COMMENT ( 				
				 The multi-threading in this disassembler will be improved in later chapters.
				(
	            invoke CreateThread,0,0,addr EngineSkeleton,0,0,addr ThreadId    ;create the new thread and tell it where the function is
                mov hThread,eax   		; Save handle of thread

				
         .endif 
	
        ;---------------------------------------------------------------------
    popad		
	ret
MnuFileOpen endp


