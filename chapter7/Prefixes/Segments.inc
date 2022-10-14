.const
SegmentsFirstID		equ 	1001	; The ID of the first control (used for setting fonts)
SegmentsLastID		equ 	1014	; The ID of the last  control (used for setting fonts)

IDC_SEG_PREFIX		equ		1005	; ID of the (EDITBOX) which is used to enter new segment code
IDC_SEG_DECODE		equ 	1008	; ID of the 'Upadae' CommandButton to update the sgemnets

IDC_SEG_EXP1		equ 	1003	; ID of the (STATIC) control for 'Example 1' 
IDC_SEG_EXP2		equ		1010	; ID of the (STATIC) control for 'Example 3' 
IDC_SEG_EXP3		equ 	1012	; ID of the (STATIC) control for 'Example 2' 

;------------------------------------------------------------------------------------------
SegmentRegs			equ		6		; Number of Segments Registers (used for the table search)
;------------------------------------------------------------------------------------------

SegmentRegTable		dd 'E2',  	offset SegmentReg_CS,	;------------------------------
					   '63', 	offset SegmentReg_SS,	; This table is used to search
					   'E3',	offset SegmentReg_DS,	; for a Segment Prefix code which
					   '62',	offset SegmentReg_ES,	; the user can enter in the EditBox
					   '46',	offset SegmentReg_FS,	; And press update to update the 
					   '56',	offset SegmentReg_GS	; segment used in the examples.
;------------------------------------------------------------------------------------------

.data?
szSegPrefix				db 5 dup (?)	; DWORD to hold the Segment Prefix (e.g. '00004532' = 'CS')
										; And 2 Empty Bytes and a Null-Terminator at the end.

szBuffer				db 250 dup (?)	; Buffer needed by wsprnitfA(...) to update the examples

SegmentReg_ColorCTL		dd	9 dup (?)			; 9 Controls to be colored (9 HWNDs)

.data
;-------------------------------------------------Segment Registers Names------------------
SegmentReg_CS		db "CS",0
SegmentReg_SS		db "SS",0
SegmentReg_DS		db "DS",0
SegmentReg_ES		db "ES",0
SegmentReg_FS		db "FS",0
SegmentReg_GS		db "GS",0
;------------------------------------------------------------------------------------------

;-------- In the examples below we use (%s) which displays the needed Segment Register ----
SegRegExample1		db	"%s:8B00           MOV EAX,DWORD PTR %s:[EAX]",0
SegRegExample2		db	"%s:AC                 LODS BYTE PTR %s:[ESI]",0
SegRegExample3		db	"%s:C605 00000000 00   MOV BYTE PTR %s:[0], 0",0


.code

CheckSegPrefix proc SegReg:DWORD, hWin:DWORD
;------------------------------------------------------------------------------------------
; Loop through the Segment Registers Table, and if found, return a pointer to the string
; containing the Segment Register Name, if not found, BEEEEEP :)
;------------------------------------------------------------------------------------------
	lea esi, SegmentRegTable	; The table containing the Segment Registers 
	mov ecx, SegmentRegs		; The number of the Segment Registers (Always = 6) =)
	mov eax, SegReg				; The value to be checked :)
	mov eax, [eax]
 StartCheck:
 	cmp eax, dword ptr [esi]	; Is the code entered by the user is for a known Segment Register?
 	jnz @F						; If not, jump to the next item in the table.	>>--------------
 	;																						   	|
 	; Update the examples with the new found Segment Register									|
 	invoke wsprintf, addr szBuffer, addr SegRegExample1, addr szSegPrefix, dword ptr [esi+4];	|
 	invoke SetDlgItemText,hWin,IDC_SEG_EXP1,addr szBuffer									;	|
 	invoke wsprintf, addr szBuffer, addr SegRegExample2, addr szSegPrefix, dword ptr [esi+4];	|
 	invoke SetDlgItemText,hWin,IDC_SEG_EXP2,addr szBuffer 	 								;	|
 	invoke wsprintf, addr szBuffer, addr SegRegExample3, addr szSegPrefix, dword ptr [esi+4];	|
 	invoke SetDlgItemText,hWin,IDC_SEG_EXP3,addr szBuffer 	 								;	|
 	ret																						;	|
 @@: 																						;	|
 	add esi, 8					; Next Segment Register																		;	V
 	dec ecx						; Check counter (6 Registers)
	cmp ecx, 0					; If not reached
	ja StartCheck				; Jump to the next item

 	invoke MessageBeep,0FFFFFFFFh	; No Segment Register found for the value entered by the
 									; user, so we BEEEEP :)
	ret
CheckSegPrefix endp

SegmentsProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
	LOCAL SegPrefixEditBox:DWORD
	mov eax, uMsg
	.if eax==WM_INITDIALOG
;----------------------------------------Save some handles to COLOR the examples :)------
		invoke GetDlgItem,hWin,IDC_SEG_EXP1
		mov SegmentReg_ColorCTL[0*4],eax
		invoke GetDlgItem,hWin,IDC_SEG_EXP2
		mov SegmentReg_ColorCTL[1*4],eax
		invoke GetDlgItem,hWin,IDC_SEG_EXP3
		mov SegmentReg_ColorCTL[2*4],eax
		invoke GetDlgItem,hWin,1001 			; Title
		mov SegmentReg_ColorCTL[3*4],eax
		invoke GetDlgItem,hWin,1013				; Comment At The Button
		mov SegmentReg_ColorCTL[4*4],eax
		invoke GetDlgItem,hWin,1007				; Example1 (Static)
		mov SegmentReg_ColorCTL[5*4],eax		
		invoke GetDlgItem,hWin,1009				; Example2 (Static)
		mov SegmentReg_ColorCTL[6*4],eax		
		invoke GetDlgItem,hWin,1011				; Example3 (Static)
		mov SegmentReg_ColorCTL[7*4],eax		
		invoke GetDlgItem,hWin,1006				; Segment Prefix (Static)
		mov SegmentReg_ColorCTL[8*4],eax		
;------------------------------------------------------------------------------------------
;   Set fonts, limit the edit box accepts only two bytes, and convert them to UPPER case
;------------------------------------------------------------------------------------------
		invoke SetWindowFont, hWin, SegmentsFirstID, SegmentsLastID
		invoke GetDlgItem,hWin,IDC_SEG_PREFIX
		mov SegPrefixEditBox, eax
		invoke SendMessage,hWin,EM_LIMITTEXT,3,0
		invoke GetWindowLong,SegPrefixEditBox,GWL_STYLE
		or eax, ES_UPPERCASE
		invoke SetWindowLong,SegPrefixEditBox,GWL_STYLE,eax
;---------------------------------------------------------------------------------------
	.elseif eax==WM_COMMAND
		mov eax, wParam
		and eax, 0FFFFh
		.if eax==IDC_SEG_DECODE
			invoke GetDlgItemText,hWin,IDC_SEG_PREFIX,addr szSegPrefix,3
			invoke CheckHexString,addr szSegPrefix,2   ; Check that user entered a valid hex 
													   ; value. Actually it's not a MUST :)			
			test eax, eax
			jnz @F
			invoke MessageBeep,0FFFFFFFFh; If didn't enter a valid hex value, BEEEEEEP :)
			ret
 @@:
			invoke CheckSegPrefix, addr szSegPrefix, hWin ; Update the examples with the new prefix
		.endif
;----------------------------------------------------Main Coloring Procedure-------------
	.elseif eax==WM_CTLCOLORSTATIC
		mov eax, lParam
		.if eax==SegmentReg_ColorCTL[0*4] 	|| eax==SegmentReg_ColorCTL[1*4] \ 
			|| eax==SegmentReg_ColorCTL[2*4]
			;-------------------------------------
			;		Controls With BLUE Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,00F73833h
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret	
		.elseif eax==SegmentReg_ColorCTL[3*4]
			;-------------------------------------
			;		Controls With RED Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,000000FFh
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret			
		.elseif eax==SegmentReg_ColorCTL[4*4]	|| eax==SegmentReg_ColorCTL[5*4] \ 
			|| eax==SegmentReg_ColorCTL[6*4] ||  eax==SegmentReg_ColorCTL[7*4] \
			|| eax==SegmentReg_ColorCTL[8*4]
			;-------------------------------------
			;	Controls With DARK GREEN Color
			;-------------------------------------
			invoke SetBkMode,wParam,TRANSPARENT 
			invoke SetTextColor,wParam,001A7B39h
			invoke CreateSolidBrush, DLG_COLOR		; BK = Dialog Color = Transparent
			ret					
		.endif						
;----------------------------------------------------------------------------------------
	.else
		mov		eax,FALSE
		ret
	.endif
	mov		eax,TRUE
	ret

SegmentsProc endp
