
.const
;-------------------------------------- Menu Items ----------------------
IDM_OPEN 			equ	10002
IDM_CLOSE 			equ	10003
IDM_EXIT 			equ 10004
IDM_IMPORTS			equ 10006

.data
;-------------------------------------- File manipulating ---------------
ofn		OPENFILENAME	<>
buffer 					db 	260  dup(0)
StrFilter 				db 	"Executable Files",0,"*.exe",0 
             			db 	"Dynamic Link Libraries",0,"*.dll",0,0

.data?
;-------------------------------------- File manipulating ----------------

hFile					dd	?		; Handle of the file
FileSize				dd	?		; The file size ( GetFileSize() )
FileOffset				dd	?		; The offset of the mapped file in memory
hFileMapping			dd	?		; Handle of the file mapping object ( CreateFileMapping() )

;-------------------------------------- Handles ---------------------------
hMenu					dd	?		; The handle to our manu


.code
IsFileLoaded proc 
	
	             ;======================================
                 ;		  Display Opening Event
                 ;======================================
                 invoke AddEvent,addr szFileLoading
                 
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
                 
                 invoke RaiseErr, addr MsgErrOpen
                 inc hFile						; Empty the file handle (-1 + 1 = 0) =)
                 dec eax						; eax = 1 - 1 = 0
                 dec eax						; eax = 0 - 1 = -1 (Err Happened)	 =)
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
                 
                 invoke RaiseErr, addr MsgErrFileMap
                 mov hFileMapping, 0			; Empty the FileMappingObject handle
                 mov eax, -1					;Err Happened
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
                 
                 invoke RaiseErr, addr MsgErrSize
                 mov eax, -1
                 ret
	@@:						                  
                 
                 ;======================================
                 ;			Event: File Size..
                 ;======================================
                 invoke wsprintf, addr szBuffer, addr szFileSize, FileSize
                 invoke AddEvent,addr szBuffer
 
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
                 
                 invoke RaiseErr, addr MsgErrMapView
                 mov eax, -1
                 ret
	@@:	
	
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
 	invoke CloseHandle,hFile				; Close the file handle
 	mov FileOffset, 0
 	mov hFile, 0 	
 	
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
	invoke DisplayWelcomeMsg

	ret
MnuFileClose endp

MnuFileOpen proc
		;------------------------------------- Open file -------------------
		
            ; Display the "Open File" dialog box
           	invoke GetOpenFileName, ADDR ofn 
           	
        .if eax==TRUE 			; Didn't click CANCEL
                 
				;======================================
				;	Load the file and map it to memory
				;======================================
				
				invoke IsFileLoaded
				inc eax					; is eax == -1 ?
				jnz @f
				
				;======================================
				;	File was not loaded successfully
				;======================================
				
				invoke MnuFileClose		; Reset disassembler
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
				invoke ParseCodeSection
				
         .endif 
	
        ;---------------------------------------------------------------------		
	ret
MnuFileOpen endp


