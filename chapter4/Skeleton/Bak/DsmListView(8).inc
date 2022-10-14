
.const
IDC_DASM			equ 1001		; Resource constant for the main ListView

.data
dwIndex				dd	0		; No. of columns inserted
dwItems				dd	0

Header1				db			"Offset",0
Header2				db			"Opcodes",0
Header3				db			"Disassembly",0
Header4				db			"Comments",0

Item1				db			"Item One",0
Item2				db			"Item Two",0
Item3				db			"Item Three",0
Item4				db			"Item Four",0


.data?
hDsmList			dd	?		; Handle of the main ListView


.code
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
	invoke SendMessage,hDsmList, LVM_INSERTCOLUMN,dwIndex,addr lvc 
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
		invoke SendMessage,hDsmList, LVM_INSERTITEM,0, addr lvc
	.else
		invoke SendMessage,hDsmList, LVM_SETITEM,0, addr lvc	
	.endif
	inc dwItems
	ret
InsertItem endp

PrepareListView	proc
;-----------------------------------------------------------------------;
;   	Prepare the main ListView and changes its color & style			;
;-----------------------------------------------------------------------;
			   
    invoke SendMessage, hDsmList, LVM_SETTEXTCOLOR, 0, 00E41030h
    invoke SendMessage, hDsmList, LVM_SETBKCOLOR,0,00DEF5F3h
    invoke SendMessage, hDsmList, LVM_SETTEXTBKCOLOR,0,00DEF5F3h
	invoke SendMessage, hDsmList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0,\
		   LVS_EX_FULLROWSELECT or \
		   LVS_EX_GRIDLINES + LVS_EX_FLATSB
		
	invoke InsertColumn,addr Header1,85
	invoke InsertColumn,addr Header2,85
	invoke InsertColumn,addr Header3,190
	invoke InsertColumn,addr Header4,190
			   

	ret
PrepareListView endp
