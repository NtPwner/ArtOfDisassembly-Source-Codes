;-----------------------------------------------------------------
; Some misc procedures which are used frequently in the program
;-----------------------------------------------------------------

.data
szFont			db	"Courier New",0			; Name of the font used in the program

ToHex			db 	"0x%.2X",0					; Temp to convert to hex
hFont			dd	0						; Handle of the font used in the program

.data?
szBuffer		db 64 dup (?)				; Buffer used with wsprintfA

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
	
	
    invoke CreateFont,14,10,0,0,400,0,0,0,DEFAULT_CHARSET,\ 
                                   OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,\ 
                                   DEFAULT_QUALITY,DEFAULT_PITCH or FF_SCRIPT,\ 
                                   ADDR szFont
	mov hFont, eax
	
 	invoke GetDlgItem, hWnd, IDC_MNE_DECODE				; Get control handle
	invoke SendMessage,eax,WM_SETFONT,hFont,TRUE		; Change its font
	
	ret
SetWindowFont endp

InverseBit proc uses eax BitToInverse:DWORD
;-----------------------------------------------------------------
;	This procedure takes an ID to a 'BUTTON' control represengin 
;	a bit (0/1) and then inverse it.
;	e.g.
;	Control with the ID 1003 holds the value --> 0
;	InverseBit (1003)
;	Now, the control with the ID 1003 holds the value --> 1
;	InverseBit (1003)
;	The control with the ID 1003 holds the value --> 0
;----------------------------------------------------------------
	
	invoke GetDlgItemInt,hWnd, BitToInverse, 0, 0
	test eax, eax

	push 0	
	.if ZERO?
		push 1												; Bit (0) --> 1
	.else
		push 0												; Bit (1) --> 0	
	.endif
	
	push BitToInverse
	push hWnd
	call SetDlgItemInt

	ret
InverseBit endp

GetControlhWnds proc
;------------------------------------------------------------------------------------------
; This procedure takes no parameters.
; It saves the handles of the important Controls that we want to color later
;------------------------------------------------------------------------------------------

;--------------- Red Controls ---------

	invoke GetDlgItem, hWnd, IDC_TITLE_I			; Title 'STATIC'
	mov RedControlHwnd[0*4], eax
	invoke GetDlgItem, hWnd, IDC_INST_DEC			; Hex Value For The Instruction 'STATIC'
	mov RedControlHwnd[1*4], eax
	invoke GetDlgItem, hWnd, IDC_MODRM_DEC			; Hex Value For The ModRM Byte  'STATIC'
	mov RedControlHwnd[2*4], eax
	invoke GetDlgItem, hWnd, IDC_MNE_DECODE			; 'Mnemonic Decoding' At the bottom (STATIC)
	mov RedControlHwnd[3*4], eax
	invoke GetDlgItem, hWnd, IDC_HEX_DECODE			; 'HEX Decoding' At the bottom (STATIC)
	mov RedControlHwnd[4*4], eax


;--------------- Blue Controls --------

	invoke GetDlgItem, hWnd, IDC_TITLE_II			; Title 'STATIC'
	mov BlueControlHwnd[0*4], eax
	
;--------------- Purpule Controls --------
	invoke GetDlgItem, hWnd, IDC_MODRM				; ModRM	'GROUP'
	mov PurpuleControlHwnd[0*4], eax
	invoke GetDlgItem, hWnd, IDC_INSTRUCTION		; 'Instruction' (GROUP)
	mov PurpuleControlHwnd[1*4], eax
	invoke GetDlgItem, hWnd, IDC_DECODING_I			; 'Decoding' For Instruction (GROUP)
	mov PurpuleControlHwnd[2*4], eax
	invoke GetDlgItem, hWnd, IDC_DECODING_II		; 'Decoding' For ModRM (GROUP)
	mov PurpuleControlHwnd[3*4], eax
	invoke GetDlgItem, hWnd, IDC_DISPLACEMENT		; 'Displacement' That Follows ModRM (GROUP)
	mov PurpuleControlHwnd[4*4], eax
	invoke GetDlgItem, hWnd, 1065					; 'HEX Decoding' (STATIC)
	mov PurpuleControlHwnd[5*4], eax
	invoke GetDlgItem, hWnd, 1066					; 'Mnemonics Decoding' (STATIC)
	mov PurpuleControlHwnd[6*4], eax

;--------------- Green Controls -------

	invoke GetDlgItem, hWnd, 2014				; 'Mode' 		(STATIC)
	mov GreenControlHwnd[0*4], eax
	invoke GetDlgItem, hWnd, 2015				; 'Code/Reg' 	(STATIC)
	mov GreenControlHwnd[1*4], eax
	invoke GetDlgItem, hWnd, 2016				; 'Reg/Mem' 	(STATIC)
	mov GreenControlHwnd[2*4], eax
	invoke GetDlgItem, hWnd, 2025				; 'Bit (d)' 	(STATIC)
	mov GreenControlHwnd[3*4], eax
	invoke GetDlgItem, hWnd, 2026				; 'Bit (d)' 	(STATIC)
	mov GreenControlHwnd[4*4], eax
	invoke GetDlgItem, hWnd, 1061				; 'Operand-Size Pr...'  (STATIC)
	mov GreenControlHwnd[5*4], eax
		
	ret
GetControlhWnds endp

PressButton proc	uses eax Button:DWORD, FirstID:DWORD, LastID:DWORD
;--------------------------------------------------------------
;	This procedure takes an ID to a CommandButton, and loop 
;	through a range of IDs, then makes the wanted button the
;	only pressed button.
;--------------------------------------------------------------

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

EnableRange proc	uses eax Range_I_S:DWORD, Range_I_E:DWORD, Range_II_S:DWORD, Range_II_E:DWORD
;--------------------------------------------------------------
;	This procedure takes two ranges of IDs.
;
;	(Range_I_S --> Range_I_E)The controls within this range will
;	be enabled. 
;
;	(Range_II_S --> Range_II_E)The controls within this range will
;	be disabled. 
;	
;--------------------------------------------------------------

;------------------------------- Disable 2nd Range buttons ----
	mov ebx, Range_II_S
	.while ebx <= Range_II_E
		invoke GetDlgItem, hWnd, ebx
		invoke EnableWindow,eax, FALSE
		inc ebx
	.endw
;------------------------------- Enable 1st Range buttons ----
	mov ebx, Range_I_S
	
	.while ebx <= Range_I_E
		invoke GetDlgItem, hWnd, ebx
		invoke EnableWindow,eax, TRUE
		inc ebx
	.endw
;--------------------------------------------------------------	
	ret
EnableRange endp

BitsToByte proc		uses ecx FirstBit:DWORD, LastBit:DWORD, ControlID:DWORD
;---------------------------------------
; As the name says, this procedure gets
; the bits from all the controls, within 
; the range (FirstBit --> LastBit) and 
; then convert them to a hex value, and
; display the hex value in the staticbox
; with the ID ControlID.
; And also return the same hex value in 
; eax, in case we want it.
;---------------------------------------
	LOCAL BitsCount:DWORD
;-------------------------
	mov eax, FirstBit			; eax == FirstBit
	mov ecx, LastBit
	sub ecx, eax				
	inc ecx						; ecx == (LastBit-FirstBit) == Bits No.
	mov BitsCount, ecx
	xor eax, eax
;-------------------------					
 @@:
	push eax
	push ecx
		mov edi, BitsCount
		sub edi, ecx
		mov ecx, edi
		add ecx, FirstBit
		invoke GetDlgItemInt, hWnd,ecx,0,0
		mov ebx, eax
	pop ecx
	pop eax
		rol eax, 1
		add eax, ebx
		dec ecx
		jnz @B
		
	push eax		; --> Save Hex Value		
		
	invoke wsprintf, addr szBuffer, addr ToHex, eax
	invoke SetDlgItemText,hWnd, ControlID, addr szBuffer	 ; Display the new hex value
	
	pop eax			; --> Returns Hex Value
	
	ret
	 					
BitsToByte endp