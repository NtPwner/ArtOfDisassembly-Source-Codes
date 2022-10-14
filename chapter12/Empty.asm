.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

.code

start:
	rept 100
	nop
	endm
	ret
end start