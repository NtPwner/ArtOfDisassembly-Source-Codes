.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include CODE II.inc
include misc.inc
include decoder.inc


.code

start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DIALOG1,NULL,addr DlgProc,NULL
	invoke ExitProcess,0

;-----------------------------------------------------------------------

DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG
		push hWin
		pop hWnd
		;------------------- Set up brushes ------------------
		invoke GetSysColor,COLOR_BTNFACE
		mov COLOR_DLG, eax						; Store the color of the dialog box
		invoke CreateSolidBrush,eax
		mov hBrushTransperant, eax				; Store Handle of the dialog-box Brush
		;------------------- Get Handles Of The Controls -----
		invoke GetControlhWnds
		;------------------- Set Control Fonts ---------------
		invoke SetWindowFont, FirstControlID, LastControlID
		;------------------- Choose Default Instruction ------	
		invoke PressButton, 1004, 1004,1009						; Instruction  --> 'MOV'
		invoke PressButton, IDC_OFF_1, IDC_ON_1, IDC_OFF_1		; Prefix (66h) --> OFF
		invoke PressButton, IDC_OFF_2, IDC_ON_2, IDC_OFF_2		; Prefix (66h) --> OFF	
		;
		;
		mov Instruction_1_Inst, offset _MOV
		;
		;
		mov Instruction_1_REG[0*4], offset _EAX
		mov Instruction_1_REG[1*4], offset _EAX		
		mov Instruction_2_REG[0*4], offset _EAX
		mov Instruction_2_REG[1*4], offset _EAX				
		;
		;
		mov Instruction_Bit_d[0*1], 1
		mov Instruction_Bit_d[1*1], 1		
		mov Instruction_Bit_w[0*1], 1
		mov Instruction_Bit_w[1*1], 1		
		;
		;
		mov Instruction_Bin, offset _MOV_BIN
		mov Instruction_Hex[0*0], 100010b				; 'MOV'
		mov Instruction_Hex[1*0], 100010b				; 'MOV'		
		;
		;
		invoke DecodeAll
		;			
		
	.elseif eax==WM_COMMAND
		mov eax, wParam
		and eax, 0FFFFh
		
		;--------------------------------------------- Instruction 'BUTTON'
		.if eax >= 1004 && eax <= 1009
			invoke PressButton, eax, 1004,1009
			sub eax, 1004
			lea esi, Instructions[8*eax]
			push esi					; --> save
			mov esi, [esi]				
			mov Instruction_1_Inst, esi
			pop esi						; --> restore
			mov esi, [esi+4]			; (Get Binary Form Of The Instruction)
			mov Instruction_Bin, esi
			invoke DecodeAll	
		;--------------------------------------------- Prefix (66h) 'BUTTON' -> First  Instruction
		.elseif eax==IDC_ON_1||eax==IDC_OFF_1
			.if eax==IDC_ON_1
				mov Instruction_Prefix[0*1], 1
			.else
				mov Instruction_Prefix[0*1], 0
			.endif
			invoke PressButton, eax, IDC_ON_1,IDC_OFF_1
			invoke DecodeAll			
		;--------------------------------------------- Prefix (66h) 'BUTTON' -> Second Instruction
		.elseif eax==IDC_ON_2||eax==IDC_OFF_2
			.if eax==IDC_ON_2
				mov Instruction_Prefix[1*1], 1
			.else
				mov Instruction_Prefix[1*1], 0
			.endif
			invoke PressButton, eax, IDC_ON_2,IDC_OFF_2
			invoke DecodeAll			
		;--------------------------------------------- Bit (d) (First  Instruction)
		.elseif eax==IDC_BIT_D_1
			invoke InverseBit, eax
			invoke GetDlgItemInt,hWnd,eax,0,0
			mov Instruction_Bit_d[0*1], al				; Save value of Bit (d)
			add al, 30h
			mov Inst_1_Decoding[4], al
			invoke DecodeAll			
		;--------------------------------------------- Bit (w) (First  Instruction)
		.elseif eax==IDC_BIT_W_1
			invoke InverseBit, eax
			invoke GetDlgItemInt,hWnd,eax,0,0
			mov Instruction_Bit_w[0*1], al				; Save value of Bit (w)
			add al, 30h
			mov Inst_1_Decoding[6], al
			invoke DecodeAll			
		;--------------------------------------------- Bit (d) (Second Instruction)
		.elseif eax==IDC_BIT_D_2
			invoke InverseBit, eax
			invoke GetDlgItemInt,hWnd,eax,0,0
			mov Instruction_Bit_d[1*1], al				; Save value of Bit (d)
			add al, 30h
			mov Inst_2_Decoding[4], al
			invoke DecodeAll			
		;--------------------------------------------- Bit (w) (Second Instruction)
		.elseif eax==IDC_BIT_W_2
			invoke InverseBit, eax
			invoke GetDlgItemInt,hWnd,eax,0,0
			mov Instruction_Bit_w[1*1], al				; Save value of Bit (w)
			add al, 30h
			mov Inst_2_Decoding[6], al
			invoke DecodeAll			
		;--------------------------------------------- ModRM Bits (First  Instruction)
		.elseif eax >= IDC_MODRM1_BIT_0 && eax <= IDC_MODRM1_BIT_5
			Invoke InverseBit, eax
			invoke DecodeAll
		;--------------------------------------------- ModRM Bits (Second Instruction)
		.elseif eax >= IDC_MODRM2_BIT_0 && eax <= IDC_MODRM2_BIT_5
			Invoke InverseBit, eax
			invoke DecodeAll
		.endif
		
	.elseif eax==WM_CLOSE
		invoke EndDialog,hWin,0
	.elseif eax==WM_CTLCOLORSTATIC				; Handle Colors Of The Controls
		mov eax, lParam
		;----------------------------------------- RED Controls -------------
		.if eax==RedControlHwnd[0*4] || eax==RedControlHwnd[1*4] \
		||	eax==RedControlHwnd[2*4] || eax==RedControlHwnd[3*4]
			invoke SetBkMode,wParam,TRANSPARENT
			invoke SetTextColor, wParam, COLOR_RED
			mov eax, hBrushTransperant
			ret
		;----------------------------------------- BLUE Controls -------------
		.elseif eax==BlueControlHwnd[0*4]||eax==BlueControlHwnd[1*4]\
			||  eax==BlueControlHwnd[2*4]
			invoke SetBkMode,wParam,TRANSPARENT
			invoke SetTextColor, wParam, COLOR_BLUE
			mov eax, hBrushTransperant
			ret
		;----------------------------------------- GREEN Controls ------------
		.elseif eax==GreenControlHwnd[0*4]||eax==GreenControlHwnd[1*4] \
			||  eax==GreenControlHwnd[2*4]||eax==GreenControlHwnd[3*4]
			invoke SetBkMode,wParam,TRANSPARENT
;			invoke SetTextColor, wParam, COLOR_BLUE
			invoke CreateSolidBrush,COLOR_GREEN
			ret	
		;----------------------------------------- GREEN Controls ------------
		.elseif eax==PurpuleControlHwnd[0*4]||eax==PurpuleControlHwnd[1*4]\
			||	eax==PurpuleControlHwnd[2*4]||eax==PurpuleControlHwnd[3*4]
			invoke SetBkMode,wParam,TRANSPARENT
			invoke SetTextColor, wParam, COLOR_PURPULE	
			mov eax, hBrushTransperant
			ret		
		.endif
	.else
		mov		eax,FALSE
		ret
	.endif
	mov		eax,TRUE
	ret

DlgProc endp

GetControlhWnds proc
;--------------- Red Controls ---------

	invoke GetDlgItem, hWnd, IDC_TITLE			; Title 'STATIC'
	mov RedControlHwnd[0*4], eax
	invoke GetDlgItem, hWnd, IDC_TIP			; TIP 'STATIC'
	mov RedControlHwnd[1*4], eax
	invoke GetDlgItem, hWnd, IDC_66h_1			; 66h Prefix 'STATIC'
	mov RedControlHwnd[2*4], eax
	invoke GetDlgItem, hWnd, IDC_66h_2			; 66h Prefix 'STATIC'
	mov RedControlHwnd[3*4], eax

;--------------- Blue Controls --------

	invoke GetDlgItem, hWnd, IDC_TITLE2			; Title 'STATIC'
	mov BlueControlHwnd[0*4], eax
	invoke GetDlgItem, hWnd, IDC_DECODE_1		; First  instruction decoding (STATIC)
	mov BlueControlHwnd[1*4], eax
	invoke GetDlgItem, hWnd, IDC_DECODE_2		; Second instruction decoding (STATIC)
	mov BlueControlHwnd[2*4], eax
	
;--------------- Purpule Controls --------
	invoke GetDlgItem, hWnd, 1022				; [OPCODE] First  Instruction 'Group'
	mov PurpuleControlHwnd[0*4], eax
	invoke GetDlgItem, hWnd, 1024				; [OPCODE] Second Instruction 'Group'
	mov PurpuleControlHwnd[1*4], eax
	invoke GetDlgItem, hWnd, 1017				; [ModRM] First  Instruction 'Group'
	mov PurpuleControlHwnd[2*4], eax
	invoke GetDlgItem, hWnd, 1049				; [ModRM] Second Instruction 'Group'
	mov PurpuleControlHwnd[3*4], eax


;--------------- Green Controls -------

	invoke GetDlgItem, hWnd, 1010				; Instruction Border 'STATIC'
	mov GreenControlHwnd[0*4], eax
	invoke GetDlgItem, hWnd, 1011				; Instruction Border 'STATIC'
	mov GreenControlHwnd[1*4], eax
	invoke GetDlgItem, hWnd, 1012				; Instruction Border 'STATIC'
	mov GreenControlHwnd[2*4], eax
	invoke GetDlgItem, hWnd, 1013				; Instruction Border 'STATIC'
	mov GreenControlHwnd[3*4], eax
		
	ret
GetControlhWnds endp

end start
