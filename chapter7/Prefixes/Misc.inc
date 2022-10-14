;-----------------------------------------------------------------
; Some misc procedures which are used frequently in the program
;-----------------------------------------------------------------

.data
szFont			db	"Courier New",0			; Name of the font used in the program
hFont			dd	0						; Handle of the font used in the program

.code

SetWindowFont proc hWindowHandle:DWORD, FirstID:DWORD, LastID:DWORD
;------------------------------------------------------------------------------------------
; This procedure takes three parameters:
; 1) Handle to the tab control we want to set the font to
; 2) The First control ID
; 3) The Last control ID
; then loop, get each control handle, and set the font (default is Courier New).
;------------------------------------------------------------------------------------------
	cmp hFont, 0
	jnz @F
    invoke CreateFont,0,0,0,0,400,0,0,0,DEFAULT_CHARSET,\ 
                                   OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,\ 
                                   DEFAULT_QUALITY,DEFAULT_PITCH or FF_SCRIPT,\ 
                                   ADDR szFont	
	mov hFont, eax
 @@:	
	
	mov ecx, LastID
	sub ecx, FirstID
	inc ecx
	mov eax, FirstID
 @@:
	push eax
	push ecx
 	invoke GetDlgItem, hWindowHandle,eax				; Get control handle
	invoke SendMessage,eax,WM_SETFONT,hFont,TRUE		; Change its font
	pop ecx
	pop eax
	inc eax
	loop @B			; Change font of all the controls from (FirstID to LastID)
	ret
SetWindowFont endp

CheckHexString proc uses esi edi ecx HexString:DWORD, HexLength:DWORD 
;------------------------------------------------------------------------------------------
; This procedure takes 2 paramter:
; Parameter 1: A pointer to a string
; Parameter 2: Length of the string
; Then the procedure checks, if the string can be convered to hex value, return TRUE/FALSE
; Valid hex string contains only digits '0-9' and/or chars 'A-F'
;------------------------------------------------------------------------------------------	
	mov esi, HexString
	mov ecx, HexLength
 StartCheck:	
	lodsb
	cmp al, 0
	jnz @F
	mov eax, TRUE
	ret
 @@:
	cmp al, '0'					;------------------------
	jb BadHex					; Check for digits '0-9'
	cmp al, '9'					;------------------------
	ja CheckHexLetters
	loop StartCheck
	mov eax, TRUE
	ret	
 CheckHexLetters:
	cmp al, 'A' 				;-----------------------
	jb BadHex					; Check for chars 'A-F'
	cmp al, 'F'					;-----------------------
	ja BadHex
	loop StartCheck
	mov eax, TRUE				; Set ValidHexString flag in eax
	ret							; return eax (TRUE)
 BadHex:
 	mov eax, FALSE				; Set InValidHexString flag in eax
	ret							; return eax (FALSE)
CheckHexString endp

HexStringToByte	proc HexString:DWORD	; This procedure is not used but it may come in handy =)
;------------------------------------------------------------------------
; This procedure takes 1 paramter:
; Parameter 1: A pointer to a string (2-Byte) we wanna convert to hex value
; This procedure takes a string containing a valid hex value, and then
; convert it from string to an actuall hex value (1 Byte) and return this
; value in eax (AL)
;------------------------------------------------------------------------
	mov esi, HexString
	mov ecx, 2
	xor eax, eax
	
 @@:	
	rol eax, 8
	lodsb
	sub eax, 30h
		
	.if Al > 9
		sub Al, 7
	.endif	
	loop @B
	
	ror al, 4			
	ror eax, 4			
	
	ret
HexStringToByte endp