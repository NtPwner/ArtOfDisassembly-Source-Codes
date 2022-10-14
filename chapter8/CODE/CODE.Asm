.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include CODE.inc
include misc.inc


.code

start:

	invoke GetModuleHandle,NULL
	mov		hInstance,eax

    invoke InitCommonControls
	invoke DialogBoxParam,hInstance,IDD_DIALOG1,NULL,addr DlgProc,NULL
	invoke ExitProcess,0

;-----------------------------------------------------------------------

DlgProc proc uses ebx hWin:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

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
		invoke PressButton, IDC_INC, IDC_INC, IDC_POP
		invoke PressButton, IDC_OFF, IDC_ON, IDC_OFF
		mov InstName, offset _INC_REG_FULL
		
	.elseif eax==WM_COMMAND
		mov eax, wParam
		and eax, 0FFFFh
		.if eax >= IDC_INC && eax <= IDC_POP				; Instruction 'COMMAND'
			invoke PressButton, eax, IDC_INC, IDC_POP	
			;----------------------- Display [CODE] Bits --------------------------------
			sub eax, IDC_INC							; Get Instruction ID (0-3)
			mov InstCode, eax							; Store Instruction Code
			mov ebx, eax
			mov eax, [ONE_BYTE_INST+(eax*8)+4]			; Get the Instruction [CODE] (5-Bits)
			invoke SetDlgItemText, hWnd, IDC_CODE, eax	; Display it on screen
			mov eax, [ONE_BYTE_INST+(ebx*8)]			; Get the Instruction Name
			mov InstName, eax
			
			invoke DecodeInstruction				
						
		.elseif eax >= IDC_RRR1 && eax <= IDC_RRR3			; <reg> 'COMMAND'
			push eax
			invoke GetDlgItemInt,hWnd,eax,0,0
			.if eax==1
				pop eax
				invoke SetDlgItemInt, hWnd, eax, 0, 0
			.else
				pop eax
				invoke SetDlgItemInt, hWnd, eax, 1, 0
			.endif

			xor ebx, ebx			
			invoke GetDlgItemInt, hWnd, IDC_RRR1, 0,0 
			mov bl, al
			invoke GetDlgItemInt, hWnd, IDC_RRR2, 0,0 
			rol bl, 1
			add bl, al
			invoke GetDlgItemInt, hWnd, IDC_RRR3, 0,0 
			rol bl, 1
			add bl, al
			mov RegCode, ebx		
			
		.elseif eax >= IDC_ON && eax <= IDC_OFF
			.if eax==IDC_ON
				mov OperandSize, 1
			.else
				mov OperandSize, 0
			.endif
			invoke PressButton, eax, IDC_ON, IDC_OFF	
		.endif
		
		invoke DecodeInstruction
			
	.elseif eax==WM_CLOSE
		invoke EndDialog,hWin,0
	.elseif eax==WM_CTLCOLORSTATIC				; Handle Colors Of The Controls
		mov eax, lParam
		;----------------------------------------- RED Controls -------------
		.if eax==RedControlHwnd[0*4]
			invoke SetBkMode,wParam,TRANSPARENT
			invoke SetTextColor, wParam, COLOR_RED
			mov eax, hBrushTransperant
			ret
		;----------------------------------------- BLUE Controls -------------
		.elseif eax==BlueControlHwnd[0*4] || eax==BlueControlHwnd[1*4]
			invoke SetBkMode,wParam,TRANSPARENT
			invoke SetTextColor, wParam, COLOR_BLUE
			mov eax, hBrushTransperant
			ret
		;----------------------------------------- GREEN Controls ------------
		.elseif eax==GreenControlHwnd[0*4] || eax==GreenControlHwnd[1*4] \
			||  eax==GreenControlHwnd[2*4] || eax==GreenControlHwnd[3*4] 
			invoke SetBkMode,wParam,TRANSPARENT
			invoke SetTextColor, wParam, COLOR_BLUE
			invoke CreateSolidBrush,COLOR_GREEN
			ret
		.elseif eax==GreenControlHwnd[4*4]
			invoke SetBkMode,wParam,TRANSPARENT
			invoke SetTextColor, wParam, COLOR_GREEN
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

DecodeInstruction  proc uses eax ebx ecx

			mov eax, InstCode
			mov ecx, 8
			mul ecx
			add eax, 40h
			add eax, RegCode
			mov Opcode, eax
	
			;=============================
			; 	eax == Instruction Name
			;=============================	

			mov eax, InstName	
			mov ebx, RegCode
						
			mov ecx, 8			; 8-Registers
 Findrrr:			
			.if byte ptr OperandSize == 0
				lea eax, [REG32+(ecx*8)-4]		
			.else
				lea eax, [REG16+(ecx*8)-4]		
			.endif 
			
			.if [eax] == ebx
				sub eax, 4
				mov ebx, [eax]
				xor ecx, ecx
				jmp Found
			.endif
			dec ecx
			test ecx, ecx
			jnz Findrrr
					
 Found:
 			;=============================
			;    ebx == Register Name
			;=============================			
			
			; Decode The Instruction
			.if byte ptr OperandSize == 0
				invoke wsprintf, addr Buffer, addr InstDecode, Opcode, InstName, ebx
			.else
				invoke wsprintf, addr Buffer, addr InstDecode2, Opcode, InstName, ebx
			.endif 			
			; Display it on screen
			invoke SetDlgItemText, hWnd, IDC_DECODING, addr Buffer			

	ret

DecodeInstruction endp

GetControlhWnds proc
	invoke GetDlgItem, hWnd, IDC_TITLE			; Title 'STATIC'
	mov RedControlHwnd[0*4], eax
	invoke GetDlgItem, hWnd, IDC_TITLE2			; Title 'STATIC'
	mov BlueControlHwnd[0*4], eax
	invoke GetDlgItem, hWnd, IDC_DECODING		; Decoding of the instruction
	mov BlueControlHwnd[1*4], eax			
	invoke GetDlgItem, hWnd, IDC_INSTBORDER1	; Instruction Border 1
	mov GreenControlHwnd[0*4], eax
	invoke GetDlgItem, hWnd, IDC_INSTBORDER2	; Instruction Border 2
	mov GreenControlHwnd[1*4], eax
	invoke GetDlgItem, hWnd, IDC_INSTBORDER3	; Instruction Border 3
	mov GreenControlHwnd[2*4], eax
	invoke GetDlgItem, hWnd, IDC_INSTBORDER4	; Instruction Border 4
	mov GreenControlHwnd[3*4], eax		
	invoke GetDlgItem, hWnd, IDC_CODE			; [CODE] 5-Bits
	mov GreenControlHwnd[4*4], eax			
	ret
GetControlhWnds endp

end start
