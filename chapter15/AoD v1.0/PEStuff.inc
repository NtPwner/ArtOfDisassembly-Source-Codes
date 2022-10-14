.const
MAX_SECTIONS		equ 30			; Max number of sections to handle in the PE file

.data
NTHeaderOffset		dd	?			; Offset of the IMAGE_NT_HEADERS structure
NumOfSections		dd	?			; Number of sections in the file	
SectionsOffset		dd	?			; Offset of the first section
EntryPointRVA		dd	?			; RVA of the Entry Point (AddressOfEntryPoint)
EntryPointOffset	dd	?			; Offset of the EntryPoint
ImportTableRVA		dd	?			; RVA of the Import Table Directory
ImportSection		dd	?			; Offset of the Import Section
ImageBase			dd	?			; ImageBase of the file

;----- Code Section Info ----
CodeSectionIndex	dd	?			; Index Of Code Section
CodeStartRVA		dd	?			; Code Section (RVA) Start
CodeStartOffset		dd	?			; Code Section (Offset) Start
CodeSize			dd	?			; Code Section Size

SECTION struc 		; 	Section Structure (used to save sections information)
	VirtualSize			dd	?			; VirtualSize 
 	VirtualAddress 		dd	?			; VirtualAddress 
	SizeOfRawData	 	dd	?			; PhysicalSize
	PointerToRawData	dd	?			; PhysicalOffset
	Characteristics		dd	?	 		; Characteristics
SECTION  ends


.data?

Sections	SECTION	MAX_SECTIONS dup ({})		; Define 20 Sections

.code

RVAToOffset proc uses edi edx ecx RVA:DWORD
	;===============================================
	; 		Convert An RVA To Offset
	;
	;		Return Values:
	;						eax == Offset
	;						ebx == Section Index
	;===============================================
	
	mov edx, SectionsOffset
	assume edx: ptr IMAGE_SECTION_HEADER
	mov ecx, NumOfSections
	mov edi, RVA
	.while ecx > 0									; Loop through all the sections
		.if edi >=  [edx].VirtualAddress			;
			mov eax,[edx].VirtualAddress			; 
			add eax,[edx].SizeOfRawData				;
			.if edi<eax								; Is the address in this section?
				mov eax,[edx].VirtualAddress		;
				sub edi,eax							; edi == Section RVA - Our RVA
				mov eax,[edx].PointerToRawData		;
				add eax,edi							; eax == file offset
				mov ebx, NumOfSections
				sub ebx, ecx
				ret
			.endif
		.endif
		add edx,sizeof IMAGE_SECTION_HEADER
		dec ecx
	.endw
	mov eax,edi
	xor ebx, ebx
	ret
RVAToOffset endp

GetImportTable proc uses ebx
	;============================================
	;		Get the Import Table Directory
	;============================================
	mov eax, eax
	mov esi, NTHeaderOffset
	assume esi: ptr IMAGE_NT_HEADERS
	mov eax, [esi].OptionalHeader.DataDirectory[8].VirtualAddress	; ( Import Symbols )
	mov ImportTableRVA, eax
	invoke RVAToOffset,eax
	add eax, FileOffset
	mov ImportSection, eax
	ret
GetImportTable endp
 
CheckSignatures proc
	;=======================================================
	;	Check the file for valid 'MZ' and 'PE' signatures
	;=======================================================
	mov esi, FileOffset	
	assume  esi: ptr IMAGE_DOS_HEADER		; esi is a IMAGE_DOS_HEADER structure
	;====================================
	;	Check for valid 'MZ' Signature
	;====================================
	cmp	word ptr [esi].e_magic, IMAGE_DOS_SIGNATURE		; Is 'MZ' Signature Is Valid?
	jz @f
	invoke AddEvent, addr szNotValidMZ		; Display the ( Invalid 'MZ' Sig... ) event
	mov eax, -1								; return -1
	ret
 @@:	
 	invoke AddEvent, addr szValidMZ			; Display the ( Valid 'MZ' Sig... ) event
 	;====================================
	;	Check for valid 'PE' Signature
	;====================================	
 	add esi, [esi].e_lfanew					; esi = FileOffset + SizeOfDOSHeader == PE Header
 	mov NTHeaderOffset, esi
 	assume esi:ptr IMAGE_NT_HEADERS
 	cmp	[esi].Signature, IMAGE_NT_SIGNATURE				; Is 'PE' Signature Is Valid?
	jz @f
	invoke AddEvent, addr szNotValidPE		; Display the ( Invalid 'PE' Sig... ) event
	mov eax, -1								; return -1
	ret
 @@:	
 	invoke AddEvent, addr szValidPE			; Display the ( Valid 'PE' Sig... ) event
	xor eax, eax
	ret
CheckSignatures endp

GetNumberOfSections proc
	;==========================================
	; Get the number of sections in the file
	;==========================================
	mov esi, NTHeaderOffset
 	assume esi:ptr IMAGE_NT_HEADERS
 	movsx eax, [esi].FileHeader.NumberOfSections
 	mov NumOfSections, eax
 	;===========================================
 	;     Display the number of section
 	;===========================================
	invoke AddEvent, addr szEmpty
	invoke AddEvent, addr szLine
	invoke wsprintf, addr szBuffer, addr szSectionsNo, NumOfSections
	invoke AddEvent, addr szBuffer

	ret
GetNumberOfSections endp

GetSections	proc
	
	;=============================================================
	;	LOCAL DWORDS to save temporarily the section information
	;=============================================================
	
	LOCAL pName:DWORD, dwVirtualAddress:DWORD, dwVirtualSize:DWORD, \
	dwPhysicalAddress:DWORD,dwPhysicalSize:DWORD, dwCharacteristics:DWORD

	
	;==================================================================================
	;	Simply this procedure display information about the sections in the PE
	;==================================================================================

	mov esi, NTHeaderOffset				; esi points to the Offset of IMAGE_NT_HEADER structure
	assume esi: ptr IMAGE_NT_HEADERS
	add esi, sizeof IMAGE_NT_HEADERS	; esi points to the first section
	mov SectionsOffset, esi				; Save the 1st section offset
	assume esi: ptr	IMAGE_SECTION_HEADER 
	lea edi, Sections					; Save sections info.
	assume edi: ptr SECTION
	xor ecx, ecx
 @@:
 	
	mov ebx, dword ptr [esi].Misc.VirtualSize 					; VirtualSize ( + 0x08 )
	mov dwVirtualSize, ebx										;
	mov [edi].VirtualSize, ebx									;
 	mov ebx, dword ptr [esi].VirtualAddress 						; VirtualAddress ( + 0x0C )
	mov dwVirtualAddress, ebx 									;
	mov [edi].VirtualAddress, ebx								;	
	mov ebx, dword ptr [esi].SizeOfRawData	 					; PhysicalSize	( + 0x10 )
	mov dwPhysicalSize, ebx									;
	mov [edi].SizeOfRawData, ebx								;	
	mov ebx, dword ptr [esi].PointerToRawData						; PhysicalOffset ( + 0x14 )
	mov dwPhysicalAddress, ebx										;
	mov [edi].PointerToRawData, ebx								;	
	mov ebx,  [esi].Characteristics								; Characteristics
	mov dwCharacteristics, ebx									;
 	mov [edi].Characteristics, ebx								;	
 	lea ebx, [esi].Name1
 	mov pName, ebx
	add esi, sizeof IMAGE_SECTION_HEADER						; Next Section
	add edi, sizeof SECTION										; Store Next Section
	push ecx	

    invoke AddEvent, addr szEmpty								;
	
	invoke wsprintfA, addr szBuffer, addr szSectionInfo1, pName	;
    invoke AddEvent, addr szBuffer								;
																; 
    invoke wsprintfA, addr szBuffer, addr szSectionInfo2 \		;
    , dwVirtualAddress, dwVirtualSize							; 
    invoke AddEvent, addr szBuffer								;
    
    invoke wsprintfA, addr szBuffer, addr szSectionInfo3,\		;
    dwPhysicalAddress, dwPhysicalSize, dwCharacteristics		; 
    
    invoke AddEvent, addr szBuffer								;

	pop ecx														; Restore Cur Index
		
	inc ecx
	cmp ecx, NumOfSections
	jl @B
		
	ret
GetSections endp

GetEntryPoint proc
	;=========================================================
	;	Gets the RVA of the EntryPoint (AddressOfEntryPoint)
	;=========================================================
	mov esi, NTHeaderOffset
	assume esi: ptr IMAGE_NT_HEADERS
	mov eax, [esi].OptionalHeader.AddressOfEntryPoint
	mov EntryPointRVA, eax
	
	;=====================================
	;	Get the ImageBase of the exe
	;=====================================
	
	mov eax, [esi].OptionalHeader.ImageBase
	mov ImageBase, eax
	
	;=====================================
	;	Get the Offset of the EntryPoint
	;=====================================				
	invoke RVAToOffset, EntryPointRVA
	mov EntryPointOffset, eax
	
	;=====================================
	;	Display the EntryPoint Info.
	;=====================================
    invoke wsprintfA, addr szBuffer, addr szEntryPoint, EntryPointRVA, EntryPointOffset
	invoke AddEvent, addr szLine
	invoke AddEvent, addr szEmpty
    invoke AddEvent, addr szBuffer
	ret
GetEntryPoint endp

GetCodeSection proc
	;=========================================================
	;	Gets the section that contains the executable code
	;=========================================================
	mov eax, EntryPointRVA				; RVA Of The EntryPoint
	invoke RVAToOffset,eax				; ebx == Section Index (Code Section)
	mov CodeSectionIndex, ebx			;
	mov eax, ebx						;
	mov ecx, sizeof SECTION				;
	mul ecx								;
	lea esi, [Sections]+eax				; esi = Sections[CodeSectionIndex]
	assume esi: ptr SECTION				;
	mov eax, [esi].VirtualAddress		;
	mov CodeStartRVA, eax				; Save Code Section Start (RVA)
	mov eax, [esi].PointerToRawData		;
	mov CodeStartOffset, eax			; And Offset.
	mov eax, [esi].SizeOfRawData		;
	mov CodeSize, eax					; Save Code Section Size
	ret
GetCodeSection endp