.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

.code

start:
	xchg eax, eax
	nop
	xchg eax, eax
	nop
	xchg eax, eax
	nop
	xchg eax, eax
	nop
	ret
end start