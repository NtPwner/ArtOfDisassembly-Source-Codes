.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include ModRM.inc
include Misc.inc
include Decoding.inc

.code

start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_MAIN,NULL,addr DlgProc,NULL
	invoke ExitProcess,0

;########################################################################

DlgProc proc hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

	mov		eax,uMsg
	.if eax==WM_INITDIALOG
		push hWin
		pop hWnd	; save hWnd
		;------------------- Set up brushes ------------------
		invoke GetSysColor,COLOR_BTNFACE
		mov COLOR_DLG, eax						; Store the color of the dialog box
		invoke CreateSolidBrush,eax
		mov hBrushTransperant, eax				; Store Handle of the dialog-box Brush
		;------------------- Get Handles Of The Controls -----
		invoke GetControlhWnds
		;------------------- Set Control Fonts ---------------
		invoke SetWindowFont, FirstControlID, LastControlID
		;------------------- Select Default Settings ---------
		invoke PressButton, IDC_ADD, IDC_ADD, IDC_XOR			; Selecet default instruction
		mov BitD, 1												; Bit(d) == 1
		mov BitW, 0												; Bit(w) == 0
		;----------------- No displacement (default) so disable the buttons ------------------
		invoke EnableRange, 0, 0, IDC_DISP_BIT_00, IDC_DISP_BIT_15
		;----------------- No Operand Size Prefix (default) ----------------------------------
		invoke PressButton, IDC_OFF, IDC_OFF, IDC_ON	
		;-------------------------------------------- Default ModRM Byte ---------------------
		mov ModRM, 0
		mov ModRM_REG1, offset _EAX
		mov ModRM_REG2, offset _EAX
		mov ModRM_MOD, 11b			; Default Mode == [11b]
		;-------------------------------------------- Default DISPLACEMENT data --------------
		mov EnableDISP, 1
		;------------------------------------------------------ Show Default Decoding --------
		invoke UpdateValues
			
	.elseif eax==WM_COMMAND
		mov eax, wParam
		and eax, 0FFFFh
		;------------------------------------------------- ModRM Bit Was Pressed --------
		.if eax >= IDC_MODRM_7 && eax <= IDC_MODRM_0 
			invoke InverseBit, eax
			invoke UpdateValues
		;--------------------------------------------- Bit(d) Or Bit(w) Was Pressed -----
		.elseif eax == IDC_BIT_D || eax == IDC_BIT_W 
			invoke InverseBit, eax	
			invoke UpdateValues
		.elseif eax >= IDC_ADD && eax <= IDC_XOR
		;---------------------------------------------- An Instruction Was Pressed ------
			invoke PressButton, eax, IDC_ADD, IDC_XOR
			sub eax, IDC_ADD
			mov Inst_Index, eax					; Save instruction index
			invoke UpdateValues
		.elseif eax >= IDC_DISP_BIT_00 && eax <= IDC_DISP_BIT_15
		;--------------------------------------------- Displacement Bit Was Pressed -----		
			invoke InverseBit, eax	
			invoke UpdateValues
		;--------------------------------- Operand-Size Prefix (On|Off) Was Pressed -----
		.elseif eax == IDC_ON || eax == IDC_OFF
			invoke PressButton, eax, IDC_OFF, IDC_ON
			sub eax, IDC_OFF							;  OpeandSize == 1 (if 'ON')
			mov Prefix66, eax
			invoke UpdateValues
		.endif
		
	.elseif eax==WM_CLOSE
		invoke EndDialog,hWin,0
	.elseif eax==WM_CTLCOLORSTATIC				; Handle Colors Of The Controls
		mov eax, lParam
		;----------------------------------------- RED Controls -------------
		.if eax==RedControlHwnd[0*4]||eax==RedControlHwnd[1*4]\
		||	eax==RedControlHwnd[2*4]||eax==RedControlHwnd[3*4]\
		||	eax==RedControlHwnd[4*4]
			invoke SetBkMode,wParam,TRANSPARENT
			invoke SetTextColor, wParam, COLOR_RED
			mov eax, hBrushTransperant
			ret
		;----------------------------------------- BLUE Controls -------------
		.elseif eax==BlueControlHwnd[0*4]
			invoke SetBkMode,wParam,TRANSPARENT
			invoke SetTextColor, wParam, COLOR_BLUE
			mov eax, hBrushTransperant
			ret
		;----------------------------------------- GREEN Controls ------------
		.elseif eax==GreenControlHwnd[0*4]||eax==GreenControlHwnd[1*4]\
			||	eax==GreenControlHwnd[2*4]||eax==GreenControlHwnd[3*4]\
			||	eax==GreenControlHwnd[4*4]||eax==GreenControlHwnd[5*4]
			invoke SetBkMode,wParam,TRANSPARENT
			invoke SetTextColor, wParam, COLOR_GREEN	
			mov eax, hBrushTransperant
			ret	
		;----------------------------------------- PURPULE Controls ------------
		.elseif eax==PurpuleControlHwnd[0*4]||eax==PurpuleControlHwnd[1*4]\
			||	eax==PurpuleControlHwnd[2*4]||eax==PurpuleControlHwnd[3*4]\
			||	eax==PurpuleControlHwnd[4*4]||eax==PurpuleControlHwnd[5*4]\
			||	eax==PurpuleControlHwnd[6*4]
			invoke SetBkMode,wParam,TRANSPARENT
			invoke SetTextColor, wParam, COLOR_PURPULE	
			mov eax, hBrushTransperant
			ret		
		.else													; For XP Version =)
			invoke SetBkMode,wParam,TRANSPARENT
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

end start
