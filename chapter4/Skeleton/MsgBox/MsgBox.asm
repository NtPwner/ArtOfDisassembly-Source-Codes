.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include user32.inc
includelib user32.lib

.code

;=======================================
; Small program displays a message box 
;=======================================

 text:	
	db "This is a test",0
 caption:
	db "caption",0
start:
	invoke MessageBox,0, text,caption,0
	ret
end start
