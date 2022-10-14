.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

.code

start:
	push eax
	push ebx
	push ecx
	push edx
	pop edx
	pop ecx
	pop ebx
	pop eax
	ret
end start