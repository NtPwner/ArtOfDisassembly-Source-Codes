;==========================================================
;	Include some small usefull procedures that we'll need
;==========================================================

_xchg_esi_edi macro
	push eax
	mov eax, esi
	mov esi, edi
	mov edi, eax
	pop eax
endm

.data

HexCharsTable   db 	"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", 
					"0A", "0B", "0C", "0D", "0E", "0F", "10", "11", "12", "13", 
					"14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D"
			    db	"1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", 
					"28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31",
			    	"32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B"
				db	"3C", "3D", "3E", "3F", "40", "41", "42", "43", "44", "45", 
					"46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F", 
					"50", "51", "52", "53", "54", "55", "56", "57", "58", "59"
				db	"5A", "5B", "5C", "5D", "5E", "5F", "60", "61", "62", "63", 
					"64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", 
					"6E", "6F", "70", "71", "72", "73", "74", "75", "76", "77"
				db	"78", "79", "7A", "7B", "7C", "7D", "7E", "7F", "80", "81", 
					"82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", 
					"8C", "8D", "8E", "8F", "90", "91", "92", "93", "94", "95"
				db	"96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F", 
					"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", 
					"AA", "AB", "AC", "AD", "AE", "AF", "B0", "B1", "B2", "B3"
				db	"B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", 
					"BE", "BF", "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", 
					"C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF", "D0", "D1"
				db	"D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", 
					"DC", "DD", "DE", "DF", "E0", "E1", "E2", "E3", "E4", "E5", 
					"E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF"
				db	"F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", 
					"FA", "FB", "FC", "FD", "FE", "FF"

InstructionHex	db	"XXXXXXXXXXXXXXXXXXXX",0	

.data
lpInstHex		dd  offset InstructionHex				
					 
.code



strCat proc uses esi edi lpStrDst:DWORD, lpStrSrc:DWORD, bTerminate:BYTE
;=======================================================
;	The procedure conacatenates two strings together	
;	
;	Destination	 String:	lpStrDst
;	Source 		 String:	lpStrSrc
;	Termination  Flag:		bTerminate	
;	
;=======================================================
	mov esi, lpStrSrc
	mov edi, lpStrDst
	@@:
		.while byte ptr [esi] != 0
			mov al, byte ptr [esi]
			mov byte ptr [edi], al
			add esi, 1
			add edi, 1
		.endw
		
		.if bTerminate == 1
			mov byte ptr [edi], 0	; Null-Terminate the string
		.endif
		
		mov eax, edi
	ret
strCat endp

LongHex2Str proc uses eax ebx ecx edi lpHex:DWORD, dwLength:DWORD, lpBuffer:DWORD 
	;=========================================================
	;	This procedure will convert hex-bytes of any length
	;	to string. This is useful in our disassebmler.
	;=========================================================
	xor ecx, ecx
	mov esi, lpHex
	mov edi, lpBuffer
	xor eax, eax
@@:	
	
	mov al, byte ptr [esi]
	lea ebx, [HexCharsTable+eax*2]
	mov dx, word ptr [ebx]
	mov word ptr [edi], dx
	add esi, 1
	add edi, 2
	add ecx, 1
	cmp ecx, dwLength
	jnz @B
	mov byte ptr [edi], 0
	ret
LongHex2Str endp

dwHex2Str proc  Hex:DWORD, lpBuffer:DWORD
;==============================================
; Based on the fastadw procedure by The Svin ;
;==============================================
		pushad
		mov    eax, Hex
		mov    edi, lpBuffer 
       	push   ebx
	    mov    edx, eax
        shl    eax, 4  
	    and    edx, 0FFFF0000h 
	    shr    edx, 12
	    mov    ebx, eax
	    shr    bh, 4 
	    and    eax, 0FF0h 
	    shr    al, 4 
	    and    ebx, 0F0F00h 
	    shl    ebx, 8 
	    mov    ecx, edx
	    shr    ch, 4
	    and    edx, 0FF0h
	    shr    dl, 4
	    lea    eax, [eax+ebx+06060606h] 
	    and    ecx, 0F0F00h
	    mov    ebx, eax 
	    shl    ecx, 8
	    and    ebx, 10101010h
	    shr    ebx, 4 
	    lea    edx, [edx+ecx+06060606h]
	    sub    eax, ebx 
	    mov    ecx, edx
	    and    ecx, 10101010h
	    lea    eax, [eax+ebx*8+2a2a2a2ah]
	    shr    ecx, 4
	    pop    ebx
	    sub    edx, ecx
	    bswap  eax 
	    lea    edx, [edx+ecx*8+2a2a2a2ah]
	    bswap  edx
	    mov    [edi], edx
	    mov    [edi+4], eax
	    mov    byte ptr [edi+8], 0
	    popad
	    ret	
dwHex2Str endp