InsertItem PROTO  :DWORD, :DWORD, :DWORD

.data?
szTemp	db	250 dup (?)
szTemp2	db	250 dup (?)

.data
szFormat	db "%.2X",0
szFormat2	db "%.8X",0

.code

ParseCodeSection	proc
	invoke ShowWindow, hDsmList, FALSE
	mov esi, FileOffset
	add esi, CodeStartOffset
	xor ecx, ecx
	xor eax, eax
	mov edx, CodeSize
	.while ecx < edx
		mov al, byte ptr [esi]
		pusha
		invoke wsprintf, addr szTemp2, addr szFormat, eax
		popa
		pusha
		mov eax, 00400000h
		add eax, CodeStartRVA
		add eax, ecx
		invoke wsprintf, addr szTemp, addr szFormat2, eax
		popa
		invoke InsertItem, ecx, 0,addr szTemp
		invoke InsertItem, ecx, 1,addr szTemp2
		inc esi
		inc ecx
	.endw
	invoke ShowWindow, hDsmList, TRUE
	ret
ParseCodeSection endp