.const
IDC_EVENTS			equ 	1002

.data
dwListBoxBack		dd 	0099E3F9h								; Back Color
dwListBoxFore		dd 	000490093h								; Fore Color
hListBoxBack		dd 	0										; Handle of the brush
FontName			db	"Courier New",0

;=======================================================================================

szEmpty			db		0
szLine			db		"+----------------------------------------------------------------------+",0
szMsg1			db		"         AoD: Welcome to chapter four (Journey Inside The PE)           ",0
;						"+----------------------------------------------------------------------+",0

szFileLoading	db		"File Is Loading...",0
szFileLoaded	db		"File Loaded Successfully To Memory At 0x%08lX.",0
szFileSize		db		"File Size Is 0x%08lX Bytes.",0
szDetectingMZ	db		"Detecting Valid 'MZ' Signature...",0
szDetectingPE	db		"Detecting Valid 'PE' Signature...",0
szValidMZ		db		"Valid 'MZ' Signature Found.",0
szValidPE		db		"Valid 'PE' Signature Found.",0
szNotValidMZ	db		"Invalid 'MZ' Signature Found.",0
szNotValidPE	db		"Invalid 'PE' Signature Found.",0
szSectionsNo	db		"0x%02lX Section(s) found",0

szSectionInfo1	db 		"(%s):", 0
szSectionInfo2	db 		"VirtualAddress:0x%08X - VirtualSize:0x%08X",0
szSectionInfo3	db 		"RawOffset:0x%08X - RawSize:0x%08X - Chars:0x%08X ", 0
szSectionInfo4	db 		"•••••••••••",0

szEntryPoint	db		"EntryPoint RVA: 0x%08X - EntryPoint Offset: 0x%08X",0

.data?
hEventHandler		dd		?			; Handler to the EventHandler Listbox
szBuffer			db		250 dup (?)	; Buffer fot the ListBox event handler

.code
AddEvent proc  pszEvent:DWORD
	;==================================================
	;	pszEvent: Pointer to the text we wanna insert
	;===================================================
	invoke SendMessage,hEventHandler,LB_ADDSTRING,0,pszEvent
	ret
AddEvent endp

DisplayWelcomeMsg	proc
	invoke AddEvent, addr szLine
	invoke AddEvent, addr szMsg1
	invoke AddEvent, addr szLine
	ret
DisplayWelcomeMsg 	endp

PrepareEventHandler proc


	;=================================================
	;		Prepare the EventHandler ListBox
	;=================================================	
	;-------------------------------------------------
	; 					Save its handle
	;-------------------------------------------------
	invoke GetDlgItem,hWnd,IDC_EVENTS			
	mov hEventHandler, eax
	;-------------------------------------------------
	; Create a brust for the ListBox BackGround
	;-------------------------------------------------
	invoke CreateSolidBrush,dwListBoxBack
	mov hListBoxBack, eax
	;-------------------------------------------------
	;		  Create the font for the ListBox
	;-------------------------------------------------
    invoke CreateFont,14,9,0,0,400,0,0,0,DEFAULT_CHARSET,\ 
                                   OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,\ 
                                   DEFAULT_QUALITY,DEFAULT_PITCH or FF_SCRIPT,\ 
                                   ADDR FontName	
	invoke SendMessage,hEventHandler,WM_SETFONT,eax,TRUE
	;-------------------------------------------------
	invoke DisplayWelcomeMsg
	;-------------------------------------------------
	ret
PrepareEventHandler endp

ResetEventHandler proc 
	invoke SendMessage,hEventHandler,LB_RESETCONTENT,0,0
	ret
ResetEventHandler endp
