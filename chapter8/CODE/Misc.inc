;-----------------------------------------------------------------
; Some misc procedures which are used frequently in the program
;-----------------------------------------------------------------

.data
szFont			db	"Courier New",0			; Name of the font used in the program
hFont			dd	0						; Handle of the font used in the program

.code

SetWindowFont proc FirstID:DWORD, LastID:DWORD

;------------------------------------------------------------------------------------------
; This procedure takes three parameters:
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
 	invoke GetDlgItem, hWnd,eax				; Get control handle
	invoke SendMessage,eax,WM_SETFONT,hFont,TRUE		; Change its font
	pop ecx
	pop eax
	inc eax
	loop @B			; Change font of all the controls from (FirstID to LastID)
	ret
SetWindowFont endp

PressButton proc	uses eax Button:DWORD, FirstID:DWORD, LastID:DWORD
;--------------------------------------------------------------
;	This procedure takes an ID to a CommandButton, and loop 
;	through a range of IDs, then makes the wanted button the
;	only pressed button.
;--------------------------------------------------------------
	LOCAL hButtonhWnd:DWORD
;------------------------------------ Unpress ALL buttons -----
	mov ecx, LastID
	sub ecx, FirstID	
	mov eax, FirstID
	inc ecx
	dec eax
 @@:	
	pusha
	add eax, ecx
	invoke SendDlgItemMessage,hWnd,eax,BM_SETSTATE , FALSE ,0
	popa
	dec ecx
	jnz @B
;------------------------------Press the wanted button---------
	
;--------------------------------------------------------------
	invoke SendDlgItemMessage,hWnd,Button,BM_SETSTATE , BST_PUSHED ,0
;--------------------------------------------------------------	
	ret
PressButton endp