.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

.code

start:
;-----------------------------------------------------------------
;	Can You See The Difference Afer We Used Opearnd Size Prefix?
;-----------------------------------------------------------------
	mov eax, 00EB0000h
	db 66h					 ; Operand Size Prefix
	mov eax, 00EB0000h
;-----------------------------------------------------------------
;   mov eax, 00EB0000  ---> mov ax, 0000 
;   The register size is changed from 32-Bits (EAX) to 16-Bits (AX)
;	Also the size of the value moved into the register changed
;   from 32-Bit DWORD to 16-Bit WORD.
;	
;	When this value size changed from DWORD to DWORD it means, 
;	that after this value we'll have another WORD which is not used
;	by the instruction. 
;	
;	e.g: 	 B8:0000EB00				mov eax, 00EB0000
;		  66:B8:0000				    mov ax, 0000
;		     EB00 --> (This WORD was originally from the above instruction)
;					  (Now it's considered as a new instruction itself)
;					  (That's why I picked this value (EB00) which is 
;					  (The OPCDODE for ( Jmp ToNextInstruction )
;
;-----------------------------------------------------------------	
	ret
end start