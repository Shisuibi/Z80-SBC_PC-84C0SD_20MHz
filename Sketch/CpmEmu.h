
//==============================================================================//
//	Z80 Single Board Computer "PC-84C0SD 20MHz" I/O Sketch for ESP32-S3-MINI-1	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
enum {
	BasicFileStarTrek   ,								//	スーパースタートレック
	BasicFileTrekInst   ,								//	スタートレック説明書
	BasicFileKnightRider,								//	ナイトライダー
	BasicFileMandel     ,								//	マンデルブロ集合
	BasicFileDownMaze   ,								//	ダウンメイズ
	BasicFileNeoPixel   ,								//	ネオピクセルHSV
	BasicFileRomDump    ,								//	ROMダンプリスト
	BasicFileClockTimer ,								//	システム時刻
	BasicFileMelody     ,								//	旋律♪自動演奏
	BasicFileZ3dApi     ,								//	幾何学演算ライブラリ

	BasicFileMax,										//	BASICファイル上限
};
//------------------------------------------------------------------------------//
#define		SdcFileFillerCode			0x1A			//	ファイル充填コード
#define		SdcUserDeleteFile			0xE5			//	ユーザー番号（消去ファイル）
//------------------------------------------------------------------------------//
typedef struct tSdcDirEntry {
	Uint08				iFileUser;						//	ユーザー番号
	Uint08				aiFileName[8];					//	ファイル名称

	Uint08				iFileType0 : 7;					//	ファイル種別
	Uint08				iRdOnly : 1;					//	読込専用ビット
	Uint08				iFileType1 : 7;					//	ファイル種別
	Uint08				iSystem : 1;					//	システムビット
	Uint08				iFileType2;						//	ファイル種別

	Uint08				iExtentCountL;					//	範囲カウント下位
	Uint08				iReserved;						//	予備
	Uint08				iExtentCountH;					//	範囲カウント上位
	Uint08				iRecCount;						//	範囲レコード端数

	Uint16				aiBlockAlloc[8];				//	区画割当番号
} SdcDirEntry;											//	管理情報登録
//==============================================================================//


//==============================================================================//
enum {
	CpmModeStandBy ,									//	CP/Mモード（待機）
	CpmModeMakeDisk,									//	CP/Mモード（MakeDisk呼出）
	CpmModeSysReset,									//	CP/Mモード（SystemReset）
	CpmModeCcprInit,									//	CP/Mモード（CCP転送開始）
	CpmModeCcprMove,									//	CP/Mモード（CCP転送動作）
	CpmModeCcprExit,									//	CP/Mモード（CCP転送終了）
	CpmModeBdosInit,									//	CP/Mモード（BDOS転送開始）
	CpmModeBdosMove,									//	CP/Mモード（BDOS転送動作）
	CpmModeBdosExit,									//	CP/Mモード（BDOS転送終了）
	CpmModeSecRead ,									//	CP/Mモード（セクタ読込）
	CpmModeSecWrite,									//	CP/Mモード（セクタ書込）

	CpmModeMax,											//	CP/Mモード上限
};
//------------------------------------------------------------------------------//
#define		CpmBootBaseAdrs				0x0000			//	基底アドレス（IPL）
#define		CpmBiosBaseAdrs				0xF200			//	基底アドレス（BIOS）

#define		CpmCcprBaseAdrs				0xDC00			//	基底アドレス（CCP）
#define		CpmBdosBaseAdrs				0xE400			//	基底アドレス（BDOS）
//------------------------------------------------------------------------------//
#define		CpmDiskDriveMax				0x0010			//	DISKドライブ上限
#define		CpmDiskUserMax				0x0010			//	DISKユーザー上限

#define		CpmDiskSectorMax			0x0100			//	DISKセクター上限
#define		CpmDiskTrackMax				0x0080			//	DISKトラック上限

#define		CpmDiskBlockMax				0x0400			//	DISKブロック上限
#define		CpmDiskEntryMax				0x0200			//	DISK管理情報上限

#define		CpmDiskBlockSize			0x1000			//	DISKブロック長
//------------------------------------------------------------------------------//
#define		CpmCcprBinFileSize			0x0800			//	CCPバイナリファイル長
#define		CpmBdosBinFileSize			0x0E00			//	BDOSバイナリファイル長
//------------------------------------------------------------------------------//
#define		CpmCcprBinSecMax	(CpmCcprBinFileSize >> 7)	//	CCPバイナリセクタ上限
#define		CpmBdosBinSecMax	(CpmBdosBinFileSize >> 7)	//	BDOSバイナリセクタ上限
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static Uint08 iCpmCurrDrive;							//	現行ドライブ
static Uint32 iCpmBaseDrive;							//	基準ドライブ
//------------------------------------------------------------------------------//
static Uint16 iCpmDiskTrack;							//	DISKトラック番号
static Uint16 iCpmDiskSector;							//	DISKセクター番号
static Uint16 iCpmDiskDmaAdrs;							//	DISK転送アドレス
//==============================================================================//


//==============================================================================//
static Cint08* pSdcCcprBinFileName = "/CCP-Z80.BIN";	//	CCPバイナリファイル名称
static Cint08* pSdcBdosBinFileName = "/BDOS-DR.BIN";	//	BDOSバイナリファイル名称

static Cint08* pSdcCpmDiskFileName = "/CPMDISK.DAT";	//	DISKイメージファイル名称
static Cint08* pSdcAutoExecFileName = "AUTOEXEC.SUB";	//	自動実行ファイル名称

static Cint08* pSdcUserDirName = "0123456789ABCDEF .";	//	ユーザーディレクトリ名称
static Cint08 aiSdcDispString[0x0C] = {					//	進捗状況表示文字列
	'.',	'o',	':',	SP ,	'|',	NUL,
	'|',	SP ,	NUL,	CR ,	LF ,	NUL,
};
//------------------------------------------------------------------------------//
static SdFat SD;										//	SdFat識別子
static Sint08 iCurrCpmMode;								//	現行CP/Mモード

static SdcDirEntry DirEntryRecord;						//	管理情報登録レコード
static SdcDirEntry DirExtentRecord;						//	管理情報範囲レコード
//------------------------------------------------------------------------------//
static File32 SdcCcprBinFile;							//	CCPバイナリファイル識別子
static File32 SdcBdosBinFile;							//	BDOSバイナリファイル識別子

static File32 SdcCpmDiskFile;							//	DISKイメージファイル識別子

static File32 SdcUserDirFile;							//	ユーザー管理ファイル識別子
static File32 SdcUserPosFile;							//	ユーザー所有ファイル識別子
//==============================================================================//


//==============================================================================//
static Cint08* apBasicFileName[BasicFileMax] = {		//	BASICファイル名称
	"/B/0/STARTREK.BAS",								//	スーパースタートレック
	"/B/0/TREKINST.BAS",								//	スタートレック説明書
	"/B/0/KNIGHT2K.BAS",								//	ナイトライダー
	"/B/0/MANDEL.BAS"  ,								//	マンデルブロ集合
	"/B/0/DOWNMAZE.BAS",								//	ダウンメイズ
	"/B/0/NEOPIXEL.BAS",								//	ネオピクセルHSV
	"/B/0/ROMDUMP.BAS" ,								//	ROMダンプリスト
	"/B/0/CLKTIMER.BAS",								//	システム時刻
	"/B/0/MELODY.BAS"  ,								//	旋律♪自動演奏
	"/B/0/Z3DAPI.BAS"  ,								//	幾何学演算ライブラリ
};
//------------------------------------------------------------------------------//
static File32 SdcBasicFile;								//	BASICファイル識別子
static Uint16 iSdcBasicSize;							//	BASICファイル文字数
//==============================================================================//


//==============================================================================//
static void EspDmaInit(Sint08 iMode) {
	if(iMode != False) {	DataBusOutput();	DataBusWrite(0x00);		}
	PortBusOutput();	PortBusWrite(0x00);

	MultiData(CodeTelDmaInit);
	MultiWait(CodeTelDmaInit);

	if(iMode == False)	{	PioReaOutput();		PioReaHigh();	}
	else				{	PioWriOutput();		PioWriHigh();	}

	PioMemOutput();		PioMemHigh();
}
//------------------------------------------------------------------------------//
static void EspDmaPageAdrs(void) {
	MultiData(CodeTelDmaMove);	MultiData(iCpuAdrsBus);
	MultiWait(CodeTelDmaMove);
}
//------------------------------------------------------------------------------//
static void EspDmaRead(void) {
	PortBusWrite(iCpuPortBus);	PioMemLow();
	PioReaLow();	delayMicroseconds(1);

	iCpuDataBus = DataBusRead();

	PioReaHigh();
	PioMemHigh();
}
//------------------------------------------------------------------------------//
static void EspDmaWrite(void) {
	PortBusWrite(iCpuPortBus);	PioMemLow();
	DataBusWrite(iCpuDataBus);	PioWriLow();

	delayMicroseconds(1);

	PioWriHigh();
	PioMemHigh();
}
//------------------------------------------------------------------------------//
static void EspDmaExit(Sint08 iMode) {
	PioMemInput();

	if(iMode == False)	{	PioReaInput();	}
	else				{	PioWriInput();	}

	MultiData(CodeTelDmaExit);
	MultiWait(CodeTelDmaExit);

	PortBusInput();
	if(iMode != False) DataBusInput();

	BusReqHigh();
}
//==============================================================================//


//==============================================================================//
static void SpiSdcDirFileName(Uint08 iUser, Uint08* pFileName) {
	Sint08 i;
	DirEntryRecord.iFileUser = iUser;

	for(i = 0;i < 8;i++) {
		if((*pFileName != pSdcUserDirName[0x11])&&(*pFileName != NUL))	DirEntryRecord.aiFileName[i] = *(pFileName++);
		else															DirEntryRecord.aiFileName[i] = pSdcUserDirName[0x10];
	}

	while((*pFileName != pSdcUserDirName[0x11])&&(*pFileName != NUL)) pFileName++;
	if(*pFileName == pSdcUserDirName[0x11]) pFileName++;

	if(*pFileName != NUL)	DirEntryRecord.iFileType0 = *(pFileName++);
	else					DirEntryRecord.iFileType0 = pSdcUserDirName[0x10];

	if(*pFileName != NUL)	DirEntryRecord.iFileType1 = *(pFileName++);
	else					DirEntryRecord.iFileType1 = pSdcUserDirName[0x10];

	if(*pFileName != NUL)	DirEntryRecord.iFileType2 = *(pFileName++);
	else					DirEntryRecord.iFileType2 = pSdcUserDirName[0x10];
}
//------------------------------------------------------------------------------//
static void SpiSdcPosFileName(Uint08* pFileName) {
	Sint08 i;

	for(i = 0;i < 8;i++) {
		if(DirEntryRecord.aiFileName[i] != pSdcUserDirName[0x10]) *(pFileName++) = DirEntryRecord.aiFileName[i];
	}

	if(DirEntryRecord.iFileType0 != pSdcUserDirName[0x10]) *(pFileName++) = pSdcUserDirName[0x11];

	if(DirEntryRecord.iFileType0 != pSdcUserDirName[0x10]) *(pFileName++) = DirEntryRecord.iFileType0;
	if(DirEntryRecord.iFileType1 != pSdcUserDirName[0x10]) *(pFileName++) = DirEntryRecord.iFileType1;
	if(DirEntryRecord.iFileType2 != pSdcUserDirName[0x10]) *(pFileName++) = DirEntryRecord.iFileType2;

	*(pFileName++) = NUL;
}
//------------------------------------------------------------------------------//
static Sint08 SpiSdcCmpUserFile(void) {
	Sint08 i;
	if(DirEntryRecord.iFileUser != DirExtentRecord.iFileUser) return(True);

	for(i = 0;i < 8;i++) {
		if(DirEntryRecord.aiFileName[i] != DirExtentRecord.aiFileName[i]) return(True);
	}

	if(DirEntryRecord.iFileType0 != DirExtentRecord.iFileType0) return(True);
	if(DirEntryRecord.iFileType1 != DirExtentRecord.iFileType1) return(True);
	if(DirEntryRecord.iFileType2 != DirExtentRecord.iFileType2) return(True);

	return(False);
}
//------------------------------------------------------------------------------//
static void SpiSdcDirEntryReset(void) {
	Sint08 i;

	SpiSdcDirFileName(SdcUserDeleteFile, (Uint08*)(pSdcUserDirName + 0x11));
	DirEntryRecord.iRdOnly = DirEntryRecord.iSystem = False;

	for(i = 0;i < 8;i++) DirEntryRecord.aiBlockAlloc[i] = 0x0000;

	DirEntryRecord.iExtentCountL = 0x00;	DirEntryRecord.iReserved = 0x00;
	DirEntryRecord.iExtentCountH = 0x00;	DirEntryRecord.iRecCount = 0x00;
}
//------------------------------------------------------------------------------//
static Uint08 SpiSdcAutoExecFile(void) {
	Uint16 iEntry;

	SpiSdcDirFileName(0x00, (Uint08*)pSdcAutoExecFileName);
	SdcCpmDiskFile.seek(0x00000000);

	for(iEntry = 0;iEntry < CpmDiskEntryMax;iEntry++) {
		SdcCpmDiskFile.read(&DirExtentRecord, sizeof(SdcDirEntry));
		if(SpiSdcCmpUserFile() == False) return(True);
	}

	return(False);
}
//==============================================================================//


//==============================================================================//
static void SpiSdcMultiDisp(Uint08 iData) {
	while(TransInterval() == False);
	MultiKeep(iData);	MultiSend();	TransData(iData);	TransWrite();
}
//------------------------------------------------------------------------------//
static void SpiSdcMultiString(Cint08* pString) {
	while(*pString != NUL) SpiSdcMultiDisp(*pString++);
}
//------------------------------------------------------------------------------//
static void SpiSdcExtentRead(Uint16* pBlock, Uint16* pEntry, Uint16* pReject) {
	Sint08 i, j;
	Uint16 iExtent, iRecord;
	Uint32 iFileRec;

	if((iFileRec = (SdcUserPosFile.size() + 0x0000007F) >> 7) == 0) {	(*pReject)++;	return;		}

	if(((Uint32)(*pBlock) + ((iFileRec + 0x0000001F) >> 5)) > CpmDiskBlockMax) {	(*pReject)++;	return;		}
	if(((Uint32)(*pEntry) + ((iFileRec + 0x000000FF) >> 8)) > CpmDiskEntryMax) {	(*pReject)++;	return;		}

	for(iExtent = 0;True;iExtent++) {
		if((*pEntry & 0x000F) == 0) SpiSdcMultiDisp(aiSdcDispString[0x01]);

		for(iRecord = i = 0;i < 8;i++) {
			if(iFileRec > 0) {
				DirEntryRecord.aiBlockAlloc[i] = (*pBlock)++;
				SdcCpmDiskFile.seek(iCpmBaseDrive + (Uint32)DirEntryRecord.aiBlockAlloc[i] * CpmDiskBlockSize);

				for(j = 0;j < (CpmDiskBlockSize >> 7);j++) {
					if((iSerialCountMx = SdcUserPosFile.read(aiSerialBufMx, SerialSegSizeTx)) == 0) break;
					for(;iSerialCountMx < SerialSegSizeTx;iSerialCountMx++) aiSerialBufMx[iSerialCountMx] = SdcFileFillerCode;

					SdcCpmDiskFile.write(aiSerialBufMx, SerialSegSizeTx);
					iSerialCountMx = 0;
				}

				if(iFileRec < (CpmDiskBlockSize >> 7)) {
					iRecord += iFileRec;
					iFileRec = 0;
				} else {
					iRecord += CpmDiskBlockSize >> 7;
					iFileRec -= CpmDiskBlockSize >> 7;
				}
			} else DirEntryRecord.aiBlockAlloc[i] = 0x0000;
		}

		DirEntryRecord.iExtentCountL = (Uint08)(((iExtent & 0x000F) << 1) + (iRecord > 0x0080));
		DirEntryRecord.iReserved = 0x00;

		DirEntryRecord.iExtentCountH = (Uint08)( (iExtent & 0x0FF0) >> 4);
		DirEntryRecord.iRecCount = (Uint08)((iRecord > 0x0080)?(iRecord - 0x0080):iRecord);

		SdcCpmDiskFile.seek(iCpmBaseDrive + ((Uint32)(*pEntry)++ << 5));
		SdcCpmDiskFile.write(&DirEntryRecord, sizeof(SdcDirEntry));

		if((iFileRec == 0)&&(iRecord < 0x0100)) break;
	}
}
//------------------------------------------------------------------------------//
static void SpiSdcExtentWrite(Uint16* pBlock, Uint16* pEntry) {
	Sint08 i, j;
	Uint16 iExtRec, iEntry;
	Uint16 iExtent, iRecord;

	for (iExtent = 0;iExtent < CpmDiskEntryMax;iExtent++) {
		SdcCpmDiskFile.seek(iCpmBaseDrive);

		for(iEntry = 0;iEntry < CpmDiskEntryMax;iEntry++) {
			SdcCpmDiskFile.read(&DirExtentRecord, sizeof(SdcDirEntry));
			if(SpiSdcCmpUserFile() == True) continue;

			if((	((Uint16)DirExtentRecord.iExtentCountL >> 1) |
					((Uint16)DirExtentRecord.iExtentCountH << 4)	) == iExtent) break;
		}

		iRecord = 	(Uint16)(DirExtentRecord.iExtentCountL & 0x01) * 0x0080 +
					(Uint16)DirExtentRecord.iRecCount;

		for(iExtRec = iRecord, i = 0;i < 8;i++) {
			if(iExtRec == 0) break;

			(*pBlock)++;
			SdcCpmDiskFile.seek(iCpmBaseDrive + (Uint32)DirExtentRecord.aiBlockAlloc[i] * CpmDiskBlockSize);

			for(j = 0;j < (CpmDiskBlockSize >> 7);j++, iExtRec--) {
				if(iExtRec == 0) break;
				iSerialCountMx = SdcCpmDiskFile.read(aiSerialBufMx, SerialSegSizeTx);

				SdcUserPosFile.write(aiSerialBufMx, SerialSegSizeTx);
				iSerialCountMx = 0;
			}
		}

		(*pEntry)++;
		if(iRecord < 0x0100) break;
	}
}
//==============================================================================//


//==============================================================================//
static void SpiSdcFatToCpm(void) {
	Uint08 iDrive, iUser;
	Uint16 iBlock, iEntry, iExtent, iReject;

	SpiSdcMultiString(aiSdcDispString + 0x09);

	for(iDrive = 0;iDrive < CpmDiskDriveMax;iDrive++) {
		if(((iCpmBiosParamL - 'A') < CpmDiskDriveMax)&&(iDrive != (iCpmBiosParamL - 'A'))) continue;

		iBlock = (CpmDiskEntryMax << 5) / CpmDiskBlockSize;
		iEntry = iReject = 0;

		iCpmBaseDrive = (Uint32)CpmDiskBlockMax * (Uint32)CpmDiskBlockSize * iDrive;
		SpiSdcMultiDisp(pSdcUserDirName[0x0A] + iDrive);	SpiSdcMultiString(aiSdcDispString + 0x02);

		for(iUser = 0;iUser < CpmDiskUserMax;iUser++) {
			sprintf((char*)aiStringBuf, "/%c/%c/", pSdcUserDirName[0x0A] + iDrive, pSdcUserDirName[iUser]);

			if(SdcUserDirFile = SD.open((Cint08*)aiStringBuf, O_RDONLY)) {
				if(!(SdcUserDirFile.isDirectory())) {	SdcUserDirFile.close();		continue;	}
			} else continue;

			while(SdcUserPosFile = SdcUserDirFile.openNextFile(O_RDONLY)) {
				SdcUserPosFile.getName((char*)aiStringBuf, StringSizeL);
				if(SdcUserPosFile.isDirectory()) {	SdcUserPosFile.close();		continue;	}

				SpiSdcDirFileName(iUser, aiStringBuf);
				DirEntryRecord.iRdOnly = DirEntryRecord.iSystem = False;

				SpiSdcExtentRead(&iBlock, &iEntry, &iReject);
				SdcUserPosFile.close();
			}

			SdcUserDirFile.close();
		}

		SdcCpmDiskFile.seek(iCpmBaseDrive + ((Uint32)(iExtent = iEntry) << 5));

		for(SpiSdcDirEntryReset();iEntry < CpmDiskEntryMax;iEntry++) {
			if((iEntry & 0x000F) == 0) SpiSdcMultiDisp(aiSdcDispString[0x00]);
			SdcCpmDiskFile.write(&DirEntryRecord, sizeof(SdcDirEntry));
		}

		SpiSdcMultiString(aiSdcDispString + 0x06);
		sprintf((char*)aiStringBuf, "Block:%03X Extent:%03X Reject:%03X", iBlock - ((CpmDiskEntryMax << 5) / CpmDiskBlockSize), iExtent, iReject);
		SpiSdcMultiString((Cint08*)aiStringBuf);	SpiSdcMultiString(aiSdcDispString + 0x09);
	}

	SpiSdcMultiString(aiSdcDispString + 0x09);
}
//------------------------------------------------------------------------------//
static void SpiSdcCpmToFat(void) {
	Uint08 iDrive, iUser;
	Uint16 iBlock, iEntry, iExtent;

	SpiSdcMultiString(aiSdcDispString + 0x09);

	for(iDrive = 0;iDrive < CpmDiskDriveMax;iDrive++) {
		if(((iCpmBiosParamL - 'A') < CpmDiskDriveMax)&&(iDrive != (iCpmBiosParamL - 'A'))) continue;

		sprintf((char*)aiStringBuf, "/Z%c/", pSdcUserDirName[0x0A] + iDrive);
		SD.remove((Cint08*)aiStringBuf);

		iBlock = (CpmDiskEntryMax << 5) / CpmDiskBlockSize;
		iExtent = 0;

		iCpmBaseDrive = (Uint32)CpmDiskBlockMax * (Uint32)CpmDiskBlockSize * iDrive;
		SpiSdcMultiDisp(pSdcUserDirName[0x0A] + iDrive);	SpiSdcMultiString(aiSdcDispString + 0x02);

		for(iEntry = 0;iEntry < CpmDiskEntryMax;iEntry++) {
			SdcCpmDiskFile.seek(iCpmBaseDrive + ((Uint32)iEntry << 5));
			SdcCpmDiskFile.read(&DirEntryRecord, sizeof(SdcDirEntry));

			if((iUser = DirEntryRecord.iFileUser) != SdcUserDeleteFile) {
				if((iEntry & 0x000F) == 0) SpiSdcMultiDisp(aiSdcDispString[0x01]);

				if(((DirEntryRecord.iExtentCountL & 0xFE) == 0x00)&&(DirEntryRecord.iExtentCountH == 0x00)) {
					sprintf((char*)(aiStringBuf + 0x04), "%c/", pSdcUserDirName[iUser]);
					SD.mkdir((Cint08*)aiStringBuf);

					SpiSdcPosFileName(aiStringBuf + 0x06);

					if(SdcUserPosFile = SD.open((Cint08*)aiStringBuf, O_CREAT | O_WRONLY)) {
						SpiSdcExtentWrite(&iBlock, &iExtent);
						SdcUserPosFile.close();
					}
				}
			} else {	if((iEntry & 0x000F) == 0) SpiSdcMultiDisp(aiSdcDispString[0x00]);	}
		}

		SpiSdcMultiString(aiSdcDispString + 0x06);
		sprintf((char*)aiStringBuf, "Block:%03X Extent:%03X", iBlock - ((CpmDiskEntryMax << 5) / CpmDiskBlockSize), iExtent);
		SpiSdcMultiString((Cint08*)aiStringBuf);	SpiSdcMultiString(aiSdcDispString + 0x09);
	}

	SpiSdcMultiString(aiSdcDispString + 0x09);
}
//==============================================================================//


//==============================================================================//
static void CpmEmuMakeDisk(void) {
	SdcBusyHigh();

	if(Esp32Slave) {
				if(iCpmBiosParamH == '1') SpiSdcFatToCpm();
		else	if(iCpmBiosParamH == '2') SpiSdcCpmToFat();
	}

	iCurrCpmMode = CpmModeSysReset;
}
//------------------------------------------------------------------------------//
static void CpmEmuSysReset(void) {
	Uint08 iData;

	if(Esp32Master) {
		if((iData = SliceData(True)) != NUL) TransData(iData);
		return;
	} else TransCheck(CodeSysReset);

	iCurrCpmMode = CpmModeStandBy;
}
//==============================================================================//


//==============================================================================//
static void CpmEmuCcprInit(void) {
	iCpmDiskSector = 0x0000;
	iCpmDiskDmaAdrs = CpmCcprBaseAdrs;

	SdcBusyHigh();

	if(Esp32Master)		{	EspDmaInit(True);	MultiData(CodeTelSynchWait);	}
	else				iSynchWait = True;

	iCurrCpmMode = CpmModeCcprMove;
}
//------------------------------------------------------------------------------//
static void CpmEmuCcprMove(void) {
	Sint16 i;

	if(Esp32Master) {
		if(iSerialCountMx != SerialSegSizeTx) return;

		iCpuAdrsBus = (Uint08)(iCpmDiskDmaAdrs >> 8);
		iCpuPortBus = (Uint08)(iCpmDiskDmaAdrs & 0x00FF);
		EspDmaPageAdrs();	MultiData(CodeTelSynchWait);

		for(i = 0;i < SerialSegSizeTx;i++) {
			iCpuDataBus = aiSerialBufMx[i];
			EspDmaWrite();	iCpuPortBus++;
		}

		iSerialCountMx = 0;
	} else {
		if(iSynchWait != False) return;
		iSynchWait = True;

		SdcCcprBinFile.seek((Uint32)iCpmDiskSector << 7);
		iSerialCountMx = SdcCcprBinFile.read(aiSerialBufMx, SerialSegSizeTx);
		MultiSector();
	}

	iCpmDiskDmaAdrs += 0x0080;
	if(++iCpmDiskSector < CpmCcprBinSecMax) return;

	iCurrCpmMode = CpmModeCcprExit;
}
//------------------------------------------------------------------------------//
static void CpmEmuCcprExit(void) {
	if((Esp32Slave)&&(iCpmBiosParamL != False)) {
		iCpmBiosParamL = SpiSdcAutoExecFile();
		MultiData(CodeTelAutoExec);		MultiData(iCpmBiosParamL);
	}

	iCurrCpmMode = CpmModeBdosInit;
}
//==============================================================================//


//==============================================================================//
static void CpmEmuBdosInit(void) {
	iCpmDiskSector = 0x0000;
	iCpmDiskDmaAdrs = CpmBdosBaseAdrs;

	iCurrCpmMode = CpmModeBdosMove;
}
//------------------------------------------------------------------------------//
static void CpmEmuBdosMove(void) {
	Sint16 i;

	if(Esp32Master) {
		if(iSerialCountMx != SerialSegSizeTx) return;

		iCpuAdrsBus = (Uint08)(iCpmDiskDmaAdrs >> 8);
		iCpuPortBus = (Uint08)(iCpmDiskDmaAdrs & 0x00FF);
		EspDmaPageAdrs();	MultiData(CodeTelSynchWait);

		for(i = 0;i < SerialSegSizeTx;i++) {
			iCpuDataBus = aiSerialBufMx[i];
			EspDmaWrite();	iCpuPortBus++;
		}

		iSerialCountMx = 0;
	} else {
		if(iSynchWait != False) return;
		iSynchWait = True;

		SdcBdosBinFile.seek((Uint32)iCpmDiskSector << 7);
		iSerialCountMx = SdcBdosBinFile.read(aiSerialBufMx, SerialSegSizeTx);
		MultiSector();
	}

	iCpmDiskDmaAdrs += 0x0080;
	if(++iCpmDiskSector < CpmBdosBinSecMax) return;

	iCurrCpmMode = CpmModeBdosExit;
}
//------------------------------------------------------------------------------//
static void CpmEmuBdosExit(void) {
	if(Esp32Master) {
		EspDmaExit(True);
		SdcBusyLow();	MultiData(CodeTelSdcBusy);
	}

	iCurrCpmMode = CpmModeStandBy;
}
//==============================================================================//


//==============================================================================//
static void CpmEmuSecRead(void) {
	Sint16 i;
	SdcBusyHigh();

	if(Esp32Master) {
		if(iSerialCountMx != SerialSegSizeTx) return;
		EspDmaInit(True);

		iCpuAdrsBus = (Uint08)(iCpmDiskDmaAdrs >> 8);
		iCpuPortBus = (Uint08)(iCpmDiskDmaAdrs & 0x00FF);
		EspDmaPageAdrs();

		for(i = 0;i < SerialSegSizeTx;i++) {
			iCpuDataBus = aiSerialBufMx[i];
			EspDmaWrite();	iCpuPortBus++;
			if((iCpuPortBus == 0x00)&&(i < (SerialSegSizeTx - 1))) {	iCpuAdrsBus++;	EspDmaPageAdrs();	}
		}

		iSerialCountMx = 0;

		EspDmaExit(True);
		SdcBusyLow();	MultiData(CodeTelSdcBusy);
	} else {
		SdcCpmDiskFile.seek(iCpmBaseDrive + (Uint32)iCpmDiskTrack * ((Uint32)CpmDiskSectorMax << 7) + ((Uint32)iCpmDiskSector << 7));
		iSerialCountMx = SdcCpmDiskFile.read(aiSerialBufMx, SerialSegSizeTx);
		MultiSector();
	}

	iCurrCpmMode = CpmModeStandBy;
}
//------------------------------------------------------------------------------//
static void CpmEmuSecWrite(void) {
	Sint16 i;
	SdcBusyHigh();

	if(Esp32Master) {
		EspDmaInit(False);

		iCpuAdrsBus = (Uint08)(iCpmDiskDmaAdrs >> 8);
		iCpuPortBus = (Uint08)(iCpmDiskDmaAdrs & 0x00FF);
		EspDmaPageAdrs();

		for(i = 0;i < SerialSegSizeTx;i++) {
			EspDmaRead();	iCpuPortBus++;
			aiSerialBufMx[i] = iCpuDataBus;
			if((iCpuPortBus == 0x00)&&(i < (SerialSegSizeTx - 1))) {	iCpuAdrsBus++;	EspDmaPageAdrs();	}
		}

		EspDmaExit(False);
		iSerialCountMx = SerialSegSizeTx;	MultiSector();
	} else {
		if(iSerialCountMx != SerialSegSizeTx) return;
		SdcCpmDiskFile.seek(iCpmBaseDrive + (Uint32)iCpmDiskTrack * ((Uint32)CpmDiskSectorMax << 7) + ((Uint32)iCpmDiskSector << 7));
		SdcCpmDiskFile.write(aiSerialBufMx, SerialSegSizeTx);

		iSerialCountMx = 0;
		SdcBusyLow();	MultiData(CodeTelSdcBusy);
	}

	iCurrCpmMode = CpmModeStandBy;
}
//==============================================================================//


//==============================================================================//
static void CpmEmuStandBy(void) {
	iCurrCpmMode = CpmModeStandBy;
}
//------------------------------------------------------------------------------//
static void (* apCpmEmuMode[CpmModeMax])(void) = {
	CpmEmuStandBy ,		CpmEmuMakeDisk,		CpmEmuSysReset,
	CpmEmuCcprInit,		CpmEmuCcprMove,		CpmEmuCcprExit,
	CpmEmuBdosInit,		CpmEmuBdosMove,		CpmEmuBdosExit,
	CpmEmuSecRead ,		CpmEmuSecWrite,
};
//==============================================================================//


//==============================================================================//
static void BasicControl(Sint08 iBasicFile) {
	if(SdcBusyRead() != False) return;
	if((Esp32Slave)&&(SdcBasicFile)) return;

	TransMsgDisp(apBasicFileName[iBasicFile]);
	SdcBusyHigh();
	if(Esp32Master) {	iSynchWait = True;	return;		}

	if(SdcBasicFile = SD.open(apBasicFileName[iBasicFile], O_RDONLY)) {
		if((iSdcBasicSize = SdcBasicFile.size()) > 0) return;
		SdcBasicFile.close();
	}

	SdcBusyLow();	MultiData(CodeTelSdcBusy);
}
//==============================================================================//


//==============================================================================//
static void CpmEmuInit(Sint08 iReset) {
	iCpmCurrDrive = 0x00;	iCpmBaseDrive = 0x00000000;
	iCpmDiskTrack = iCpmDiskSector = iCpmDiskDmaAdrs = 0x0000;

	iCurrCpmMode = CpmModeStandBy;
	iCpmBiosParamL = iCpmBiosParamH = 0x00;

	iSdcBasicSize = 0;
	if(Esp32Master) return;

	if(iReset == False) {
		SPI.end();
		SPI.begin(GpioLcdSck, GpioLcdSdo, GpioLcdSdi, GpioSdcScs);

		SD.begin(GpioSdcScs);

		SdcCcprBinFile = SD.open(pSdcCcprBinFileName, O_RDONLY);
		SdcBdosBinFile = SD.open(pSdcBdosBinFileName, O_RDONLY);

		SdcCpmDiskFile = SD.open(pSdcCpmDiskFileName, O_RDWR);
	} else {
		if(SdcBasicFile) SdcBasicFile.close();
	}
}
//------------------------------------------------------------------------------//
static void CpmEmuMove(void) {
	if((Esp32Master)&&(!(IsPioStandBy()))) return;
	(* apCpmEmuMode[iCurrCpmMode])();

	if((Esp32Master)||(!SdcBasicFile)) return;

	if(iSynchWait != False) return;
	iSynchWait = True;

	iSerialCountDx = SdcBasicFile.read(aiSerialBufDx, SerialSegSizeTx);
	iSdcBasicSize -= iSerialCountDx;	MultiSend();
	if(iSdcBasicSize > 0) return;

	SdcBasicFile.close();	iSynchWait = False;			//	BASICファイル読込Slave側Wait解除（変更禁止）
	SdcBusyLow();	MultiData(CodeTelSdcBusy);
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

