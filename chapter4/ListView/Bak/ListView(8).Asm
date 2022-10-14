.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include ListView.inc

.code

start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DLG,NULL,addr DlgProc,NULL
	invoke ExitProcess,0

InsertColumn proc pszHeading:DWORD, dwWidth:DWORD
	;===============================================================
	;	pszHeading:		Pointer to the name of the column
	;	dwWidth:		Width of the column we want to insert
	;===============================================================
	LOCAL lvc:LV_COLUMN
	mov lvc.imask, LVCF_TEXT + LVCF_WIDTH
	push pszHeading
	pop lvc.pszText
	push dwWidth
	pop lvc.lx
	invoke SendMessage,hDsm, LVM_INSERTCOLUMN,dwIndex,addr lvc 
	inc dwIndex
	ret
InsertColumn endp

InsertItem proc uses ecx Row:DWORD, Column:DWORD, pszCaption:DWORD
	;===============================================================
	;	Row:		Zero-Based Index (Row no. for the item)
	;	Column:		Zero-Based Index (Column no. for the item)
	;	pszCaption: Pointer to the name of the item to insert 	
	;===============================================================
	LOCAL lvc:LV_ITEM
	mov	lvc.imask, LVIF_TEXT
	push Row
	pop lvc.iItem
	push Column
	pop lvc.iSubItem
	push pszCaption
	pop lvc.pszText
	.if Column == 0
		invoke SendMessage,hDsm, LVM_INSERTITEM,0, addr lvc
	.else
		invoke SendMessage,hDsm, LVM_SETITEM,0, addr lvc	
	.endif
	inc dwItems
	ret
InsertItem endp

DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG
		push hWin
		pop hWnd
		invoke GetDlgItem,hWnd,IDC_DSM
		mov hDsm, eax
		
		;--------------------------------------- Prepare our ListView ------
    	invoke SendMessage, hDsm, LVM_SETTEXTCOLOR, 0, 0FE7000h
    	invoke SendMessage, hDsm, LVM_SETBKCOLOR,0,0C07000h
    	invoke SendMessage, hDsm, LVM_SETTEXTBKCOLOR,0,0C07000h
		invoke SendMessage, hDsm, LVM_SETEXTENDEDLISTVIEWSTYLE, 0,\
			   LVS_EX_FULLROWSELECT or \
			   LVS_EX_GRIDLINES + LVS_EX_FLATSB
		
		invoke InsertColumn,addr Header1,80
		invoke InsertColumn,addr Header2,80
		invoke InsertColumn,addr Header3,190
		invoke InsertColumn,addr Header4,190

		xor ecx, ecx
	InsertFake:		
		invoke InsertItem, ecx,0, addr Item1
		invoke InsertItem, ecx,1, addr Item2
		invoke InsertItem, ecx,2, addr Item3
		invoke InsertItem, ecx,3, addr Item4								
		inc ecx
		cmp ecx, 1000
		jnz InsertFake
		;-------------------------------------------------------------------
		
	.elseif eax==WM_COMMAND

	.elseif eax==WM_CLOSE
		invoke EndDialog,hWin,0
	.else
		mov		eax,FALSE
		ret
	.endif
	mov		eax,TRUE
	ret

DlgProc endp

end start
