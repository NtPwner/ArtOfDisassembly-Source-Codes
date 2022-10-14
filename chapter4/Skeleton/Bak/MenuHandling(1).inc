
.const
;-------------------------------------- Menu Items ----------------------
IDM_OPEN 			equ	10002
IDM_CLOSE 			equ	10003
IDM_EXIT 			equ 10004

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

				;====================================
				;  		Update the menu items        
				;====================================
			
				invoke EnableMenuItem,hMenu,IDM_OPEN,MF_BYCOMMAND or MF_GRAYED
 	        	invoke EnableMenuItem,hMenu,IDM_CLOSE,MF_BYCOMMAND or MF_ENABLED        	
			                        
         .endif 
	
        ;---------------------------------------------------------------------		
	ret
MnuFileOpen endp
