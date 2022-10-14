
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
hInstance				dd 	?
hDsmList				dd	?		; Handle of the main ListView
hWnd					dd	?		; Handle of the main dialog
hMenu					dd	?		; The handle to our manu

