.const
IDC_IMPORTS		equ		1001
IDD_IMPORTS		equ 	1000

.data
;szToDecimal		db "%lu",0			; Convert to decimal -wsprintf-
szImportName	db "%s: (RVA %08lX)",0
szImportOrdinal db "N/A: (Ordinal %04lX)",0

.data?
hImportsDlg		dd		?			; Handle to the Imports Dialog
hImportTree		dd		?			; Handle to the Imports TreeView

.code
GetImports proc
	LOCAL tvi:TV_INSERTSTRUCT
	LOCAL pName:DWORD
	LOCAL dwRVA:DWORD
	
	mov esi, ImportSection					; We're at the first IMAGE_IMPORT_DESCRIPTOR structure
	assume esi: ptr IMAGE_IMPORT_DESCRIPTOR
	.while [esi].FirstThunk != 0    || [esi].OriginalFirstThunk != 0 || \ ; Empty structure 
		   [esi].TimeDateStamp != 0 || [esi].ForwarderChain != 0		  ; OR NULL structure
;-----------------------------------------------;
			mov tvi.hParent,NULL				;
			mov tvi.hInsertAfter,TVI_SORT		;
			mov tvi.item.hItem-4,TVIF_TEXT		;
;-----------------------------------------------;					   	
		   	mov eax, [esi].Name1								; Get the DLL name
		   	invoke RVAToOffset,eax
		   	add eax, FileOffset
			mov tvi.item.pszText, eax
		   	
		   	.if [esi].OriginalFirstThunk == 0					; Is OriginalFirstThunk = 0 ?
		   		mov eax, [esi].FirstThunk						; Get FirstThunk
		   	.else
		   		mov eax, [esi].OriginalFirstThunk		   		; Get OriginalFirstThunk
		   	.endif
		   
		    mov dwRVA, eax      
;----------------------------------------------------------------------------------------;
			pusha																		 ;
			invoke SendDlgItemMessage,hImportsDlg,IDC_IMPORTS,TVM_INSERTITEM,0,addr tvi	 ;		
			mov tvi.hParent,eax															 ;
			popa																		 ;			
;----------------------------------------------------------------------------------------;	

		    
			invoke RVAToOffset, eax
			add eax, FileOffset	
			mov edx, eax

		   .while dword ptr [edx] != 0
				test dword ptr [edx], IMAGE_ORDINAL_FLAG32		   ; Imported By Ordinal? 		
		   	   	.if ZERO?
		   			invoke RVAToOffset, dword ptr [edx]			   ; If not by Ordinal
		   			add eax, FileOffset
		   			mov edi, eax
					add edi, 2
					mov pName, edi					
;----------------------------------------------------------------------------------------;
			pusha																		 ;
			invoke wsprintf, addr szBuffer, addr szImportName,  pName,  dwRVA  	 		 ;
			mov tvi.item.pszText, offset szBuffer										 ;
			invoke SendDlgItemMessage,hImportsDlg,IDC_IMPORTS,TVM_INSERTITEM,0,addr tvi	 ;		
			popa																		 ;			
;----------------------------------------------------------------------------------------;	
					add edx, 4
		   		.else												; By Ordinal
		   			mov eax, dword ptr [edx]
		   			and eax, 0FFFFh									; Get the low WORD
;----------------------------------------------------------------------------------------;
			pusha																		 ;
			invoke wsprintf, addr szBuffer, addr szImportOrdinal,  eax  		 		 ;
			mov tvi.item.pszText, offset szBuffer										 ;
			invoke SendDlgItemMessage,hImportsDlg,IDC_IMPORTS,TVM_INSERTITEM,0,addr tvi	 ;		
			popa																		 ;			
;----------------------------------------------------------------------------------------;	
					add edx, 4	
				.endif	   
					
		   	.endw
		   		   
		add esi, sizeof IMAGE_IMPORT_DESCRIPTOR
	.endw
	ret
GetImports endp