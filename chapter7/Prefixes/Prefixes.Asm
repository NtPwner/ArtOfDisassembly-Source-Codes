.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include Prefixes.inc
include Misc.inc
include Segments.inc
include OperandSize.inc
include AddressSize.inc
include REPs.inc
include LOCK.inc
include Practice.inc

.code

start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DIALOG1,NULL,addr DlgProc,NULL
	invoke ExitProcess,0

;########################################################################

DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
	LOCAL	ts:TC_ITEM
	
	mov		eax,uMsg
	.if eax==WM_INITDIALOG
;-------------------------------------------------------------------------
		push	hWin
		pop		hWnd
		invoke GetDlgItem,hWin,IDC_PREFIXES
		mov		hTab,eax
		mov		ts.imask,TCIF_TEXT
		mov		ts.lpReserved1,0
		mov		ts.lpReserved2,0
		mov		ts.iImage,-1
		mov		ts.lParam,0
		mov		ts.pszText,offset szSegments
		mov		ts.cchTextMax,sizeof szSegments
		invoke SendMessage,hTab,TCM_INSERTITEM,0,addr ts
		mov		ts.pszText,offset szOperand
		mov		ts.cchTextMax,sizeof szOperand
		invoke SendMessage,hTab,TCM_INSERTITEM,1,addr ts
		mov		ts.pszText,offset szAddress
		mov		ts.cchTextMax,sizeof szAddress
		invoke SendMessage,hTab,TCM_INSERTITEM,2,addr ts
		mov		ts.pszText,offset szREPs
		mov		ts.cchTextMax,sizeof szREPs
		invoke SendMessage,hTab,TCM_INSERTITEM,3,addr ts
		mov		ts.pszText,offset szLOCK
		mov		ts.cchTextMax,sizeof szLOCK
		invoke SendMessage,hTab,TCM_INSERTITEM,4,addr ts
		mov		ts.pszText,offset szPractice
		mov		ts.cchTextMax,sizeof szPractice
		invoke SendMessage,hTab,TCM_INSERTITEM,5,addr ts
;-------------------------------------------------------------------------
		invoke CreateDialogParam,hInstance,IDD_SEGMENTS,hTab,addr SegmentsProc,0
		mov hTabDlg,eax
;-------------------------------------------------------------------------
		invoke CreateDialogParam,hInstance,IDD_OPERAND_SIZE,hTab,addr OperandSizeProc,0
		mov hTabDlg[4],eax
;-------------------------------------------------------------------------
		invoke CreateDialogParam,hInstance,IDD_ADDRESS_SIZE,hTab,addr AddressSizeProc,0
		mov hTabDlg[8],eax
;-------------------------------------------------------------------------
		invoke CreateDialogParam,hInstance,IDD_REP_REPNE_SIZE,hTab,addr REPsProc,0
		mov hTabDlg[12],eax
;-------------------------------------------------------------------------
		invoke CreateDialogParam,hInstance,IDD_LOCk,hTab,addr LOCKProc,0
		mov hTabDlg[16],eax
;-------------------------------------------------------------------------
		invoke CreateDialogParam,hInstance,IDD_PRACTICE,hTab,addr PracticeProc,0
		mov hTabDlg[20],eax
;-------------------------------------------------------------------------
		invoke GetSysColor,COLOR_BTNFACE
		mov DLG_COLOR, eax					; Store the color of the dialog box
;-------------------------------------------------------------------------
	.elseif eax==WM_NOTIFY
		mov		eax,lParam
		mov		eax,[eax].NMHDR.code
		.if eax==TCN_SELCHANGE
			;Tab selection
			invoke SendMessage,hTab,TCM_GETCURSEL,0,0
			.if eax!=SelTab
				push	eax
				mov		eax,SelTab
				invoke ShowWindow,[hTabDlg+eax*4],SW_HIDE
				pop		eax
				mov		SelTab,eax
				invoke ShowWindow,[hTabDlg+eax*4],SW_SHOWDEFAULT
			.endif
		.endif
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
