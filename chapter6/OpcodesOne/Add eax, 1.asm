.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

.code

start:
	db 83h,0C0h, 01
	db 05,01,00,00,00	
	ret
end start