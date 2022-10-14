
.data

MsgErrCaption	db		"Error Occured",0

;============================
;	Error in:  CreateFile()
;============================

MsgErrOpen		db		"There was an error opening the file.",13,10,
						"Make sure the file is not currently in use.",13,10,
						"If this error continues, please ",
						"report this error  ( Err No. 0x01 ) to us.",13,10,
						"Http://board.anticrack.de --> AoD Forum",0

;===============================
;	Error in:  CreateFileMap()
;===============================

MsgErrFileMap	db		"There was an error creating a file mapping object.",13,10,
						"If this error continues, please ",
						"report this error  ( Err No. 0x02 ) to us.",13,10,
						"Http://board.anticrack.de --> AoD Forum",0

;================================
;	Error in:  MapViewOfFile()
;================================

MsgErrSize		db		"There was an error mapping the file to memory.",13,10,
						"If this error continues, please ",
						"report this error  ( Err No. 0x02 ) to us.",13,10,
						"Http://board.anticrack.de --> AoD Forum",0
