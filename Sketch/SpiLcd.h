
//==============================================================================//
//	Z80 Single Board Computer "PC-84C0SD 20MHz" I/O Sketch for ESP32-S3-MINI-1	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
#define		LcdScrnPixelX				480				//	液晶表示画面ピクセル（横）
#define		LcdScrnPixelY				320				//	液晶表示画面ピクセル（縦）

#define		LcdCnvsPixelX				480				//	液晶表示画布ピクセル（横）
#define		LcdCnvsPixelY				270				//	液晶表示画布ピクセル（縦）
//------------------------------------------------------------------------------//
#define		LcdStdRotation				1				//	液晶表示回転方向（基準値）
#define		LcdSegInterval				15625			//	液晶表示更新間隔（64[Hz]）
//------------------------------------------------------------------------------//
#define		LcdBitMapHeadSize			0x36			//	液晶表示BITMAP先頭サイズ
#define		LcdButtonMax				4				//	液晶表示ボタン上限
//------------------------------------------------------------------------------//
#define		LcdSorStarMax				20				//	液晶表示ソーサリアン上限
#define		LcdSorInterval				31250			//	液晶表示更新間隔（32[Hz]）
//------------------------------------------------------------------------------//
#define		LcdTchCalibSize				8				//	液晶表示タッチ調整サイズ
#define		LcdTchCalibExec				False			//	液晶表示タッチ調整実行
//------------------------------------------------------------------------------//
#define		LcdModeGraphic	(iCurrLcdMode == False)		//	液晶表示モード（グラフィック）
#define		LcdModeSegment	(iCurrLcdMode != False)		//	液晶表示モード（セグメント）
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static Sint08 iCurrLcdMode;								//	現行液晶表示モード
static Uint08 iLcdBrightness;							//	液晶表示LED輝度
//------------------------------------------------------------------------------//
static Uint08 iLcdRotation;								//	液晶表示回転方向
static Uint08 iLcdTextSize;								//	液晶表示文章寸法

static Uint16 iLcdRgbC0, iLcdRgbC1;						//	液晶表示色彩C0C1

static Sint16 iLcdPosX0, iLcdPosY0;						//	液晶表示座標X0Y0
static Sint16 iLcdPosX1, iLcdPosY1;						//	液晶表示座標X1Y1

static Sint16 iLcdDisX2, iLcdDisY2;						//	液晶表示変位X2Y2
static Sint16 iLcdRadX3, iLcdRadY3;						//	液晶表示半径X3Y3
//==============================================================================//


//==============================================================================//
static Cint08* pLcdStarLogoFileName = "/LCDSLOGO.BMP";	//	液晶表示起動装飾ファイル名称
static Cint08* pLcdScrnShotFileName = "/LCDSSHOT.BMP";	//	液晶表示画面撮影ファイル名称
//------------------------------------------------------------------------------//
static Uint08 aiBitMapHead[LcdBitMapHeadSize];			//	BITMAP先頭データ
static Uint08 aiBitMapBuff[LcdScrnPixelX * 3];			//	BITMAP緩衝エリア
static File32 LcdBitMapFile;							//	BITMAPファイル識別子
//------------------------------------------------------------------------------//
static Uint16 aiLcdGrpColor[4][6] = {					//	グラフィック基準色彩
	{	TFT_BLACK,	TFT_NAVY,		TFT_DARKGREEN,	TFT_DARKCYAN,	TFT_MAROON,		TFT_PURPLE,			},
	{	TFT_OLIVE,	TFT_LIGHTGREY,	TFT_DARKGREY,	TFT_BLUE,		TFT_GREEN,		TFT_CYAN,			},
	{	TFT_RED,	TFT_MAGENTA,	TFT_YELLOW,		TFT_WHITE,		TFT_ORANGE,		TFT_GREENYELLOW,	},
	{	TFT_PINK,	TFT_BROWN,		TFT_GOLD,		TFT_SILVER,		TFT_SKYBLUE,	TFT_VIOLET,			},
};
//==============================================================================//


//==============================================================================//
static Cint08* apSbcDeviceName[2] = {					//	SBCデバイス名称
	"Z80 Single Board Computer <<<< PC-84C0SD 20MHz >>>>",
	" Implemented by Shisuibi --Grand Master Sorcerian--",
};
//------------------------------------------------------------------------------//
static Uint16 aiSegBitColor[2][10] = {					//	セグメントビット色彩
	{	TFT_DARKGREY,	TFT_DARKGREY,	TFT_DARKGREY,	TFT_DARKGREY,	TFT_DARKGREY,
		TFT_DARKGREY,	TFT_DARKGREY,	TFT_DARKGREY,	TFT_DARKGREY,	TFT_DARKGREY,	},

	{	TFT_RED     ,	TFT_ORANGE  ,	TFT_YELLOW  ,	TFT_YELLOW  ,	TFT_YELLOW  ,
		TFT_GREEN   ,	TFT_GREEN   ,	TFT_GREEN   ,	TFT_SKYBLUE ,	TFT_BLUE    ,	},
};
//------------------------------------------------------------------------------//
static Cint08* apSegBitName[3][3][10] = {				//	セグメントビット名称
	{	{	"HL",	"CE",	"WE",	"NR",	"IR",	"B3",	"B2",	"B1",	"B0",	"BA",	},
		{	"MS",	"EC",	"CF",	"CN",	"CS",	"CK",	"CM",	"PH",	"MH",	"LC",	},
		{	"SD",	"T7",	"T6",	"T5",	"T4",	"T3",	"T2",	"T1",	"T0",	"BL",	},	},

	{	{	"ND",	"N7",	"N6",	"N5",	"N4",	"N3",	"N2",	"N1",	"N0",	"NC",	},
		{	"NB",	"P7",	"P6",	"P5",	"P4",	"P3",	"P2",	"P1",	"P0",	"NA",	},
		{	"N9",	"D7",	"D6",	"D5",	"D4",	"D3",	"D2",	"D1",	"D0",	"N8",	},	},

	{	{	"RF",	"AF",	"AE",	"AD",	"AC",	"AB",	"AA",	"A9",	"A8",	"M1",	},
		{	"IO",	"A7",	"A6",	"A5",	"A4",	"A3",	"A2",	"A1",	"A0",	"MR",	},
		{	"WR",	"D7",	"D6",	"D5",	"D4",	"D3",	"D2",	"D1",	"D0",	"RD",	},	},
};
//------------------------------------------------------------------------------//
static Uint08* apSegBitData[3][4] = {					//	セグメントビット現行
	{	&(iSysInfoBit),		&(iSysModeBit),		&(iUartTxData),		&(iSysCtrlBit),		},
	{	&(iAnyNumDt00),		&(iPioHistD01),		&(iPioHistD02),		&(iPioHistD03),		},
	{	&(iCpuAdrsBus),		&(iCpuPortBus),		&(iCpuDataBus),		&(iCpuCtrlBit),		},
};
//------------------------------------------------------------------------------//
static Uint08 aiSegBitMaskOYGS[10] = {					//	セグメントビット覆面（橙黄緑空）
	0x00,	0x80,	0x40,	0x20,	0x10,	0x08,	0x04,	0x02,	0x01,	0x00,
};

static Uint08 aiSegBitMaskRB[3][2] = {					//	セグメントビット覆面（赤青）
	{	0x20,	0x10,	},	{	0x08,	0x04,	},	{	0x02,	0x01,	},
};
//------------------------------------------------------------------------------//
static Uint32 iLcdSegMicros;							//	液晶表示マイクロ秒（セグメント）
//------------------------------------------------------------------------------//
static Uint08 aiSegBitPrev[3][4];						//	セグメントビット過去
static Uint08 aiSegBitDiff[3][4];						//	セグメントビット相違

static Sint16 aiSegBitPosX[3][10];						//	セグメントビット座標（X）
static Sint16 aiSegBitPosY[3];							//	セグメントビット座標（Y）
//==============================================================================//


//==============================================================================//
static Cint08* apLcdBtnLabel[LcdButtonMax] = {			//	液晶表示ボタン名称
	"RESET",	"GRAPH",	"HELP!",	"CLEAR",
};
//------------------------------------------------------------------------------//
static Uint08 aiLcdBtnControl[LcdButtonMax] = {			//	液晶表示ボタン制御
	CodeSysReset,	CodeLcdMode,	CodeHelpText,	CodeClrScrn,
};
//------------------------------------------------------------------------------//
static Sint16 iLcdBtnCount;								//	液晶表示ボタン回数
static LGFX_Button asLcdButton[LcdButtonMax];			//	液晶表示ボタン識別子
//------------------------------------------------------------------------------//
static Uint16 aiSorStarCol[4] = {						//	ソーサリアン流星色彩
	0x0017,		0x05DF,		0x05C0,		0xFFFB,
};
//------------------------------------------------------------------------------//
static Sint16 aiSorStarPos[4][LcdSorStarMax][XYZ];		//	ソーサリアン流星座標
static Uint32 iLcdSorMicros;							//	液晶表示マイクロ秒（ソーサリアン）
//------------------------------------------------------------------------------//
static Cint08* pLcdTchCalibName = "/LcdTchCalibData";	//	液晶表示タッチ調整名称
//------------------------------------------------------------------------------//
static Uint16 aiTchCalibData[LcdTchCalibSize];			//	タッチ調整データ格納領域
static File TchCalibFile;								//	タッチ調整ファイル識別子
//==============================================================================//


//==============================================================================//
static void SpiLcdStarLogo(void) {
	Sint16 iPosY;

	LcdBitMapFile = SD.open(pLcdStarLogoFileName, O_RDONLY);
	LcdBitMapFile.read(aiBitMapHead, LcdBitMapHeadSize);

	for(iPosY = (LcdScrnPixelY - 1);iPosY >= 0;iPosY--) {
		LcdBitMapFile.read(aiBitMapBuff, LcdScrnPixelX * 3);
		SpiLCD.pushImage(0, iPosY, LcdScrnPixelX, 1, (void*)aiBitMapBuff);
	}

	LcdBitMapFile.close();
}
//------------------------------------------------------------------------------//
static void SpiLcdScrnShot(void) {
	Uint16 iColor;
	Sint16 iPosX, iPosY;

	LcdBitMapFile = SD.open(pLcdScrnShotFileName, O_CREAT | O_WRONLY);
	LcdBitMapFile.write(aiBitMapHead, LcdBitMapHeadSize);

	for(iPosY = (LcdScrnPixelY - 1);iPosY >= 0;iPosY--) {
		SpiLCD.startWrite();

		for(iPosX = 0;iPosX < LcdScrnPixelX;iPosX++) {
			iColor = SpiLCD.readPixel(iPosX, iPosY);

			aiBitMapBuff[iPosX * 3 + 0] = (Uint08)((iColor & 0x001F) << 3);
			aiBitMapBuff[iPosX * 3 + 1] = (Uint08)((iColor & 0x07E0) >> 3);
			aiBitMapBuff[iPosX * 3 + 2] = (Uint08)((iColor & 0xF800) >> 8);
		}

		SpiLCD.endWrite();
		LcdBitMapFile.write(aiBitMapBuff, LcdScrnPixelX * 3);
	}

	LcdBitMapFile.close();
}
//==============================================================================//


//==============================================================================//
static void SpiLcdGrpClear(void) {
	Sint16 i, j;
	SpiLCD.startWrite();

	for(i = 0;i < 4;i++) {
		for(j = 0;j < 6;j++) {
			SpiLCD.writeFillRect(j * 80, i * 80, 80, 80, aiLcdGrpColor[i][j]);
		}
	}

	SpiLCD.endWrite();
}
//------------------------------------------------------------------------------//
static void SpiLcdGraphic(void) {
														//	No Operation
}
//==============================================================================//


//==============================================================================//
static void SpiLcdSegReset(void) {
	Sint16 i, j, k;

	for(j = 0;j < 3;j++) {
		for(k = 0;k < 10;k++) {
			aiSegBitPosX[j][k] = 60 + 130 * j + 10 * k;
		}
	}

	for(i = 0;i < 3;i++) {
		aiSegBitPosY[i] = 170 + 60 * i;
	}
}
//------------------------------------------------------------------------------//
static void SpiLcdSegClear(void) {
	Sint16 i, j, k;
	SpiLCD.startWrite();

	SpiLCD.setFont(&fonts::lgfxJapanGothic_16);

	SpiLCD.setTextColor(TFT_GOLD  , TFT_BLACK);
	SpiLCD.drawString(apSbcDeviceName[0], 40, 16);

	SpiLCD.setTextColor(TFT_VIOLET, TFT_BLACK);
	SpiLCD.drawString(apSbcDeviceName[1], 40, 32);

	SpiLCD.setFont(&fonts::lgfxJapanGothic_12);

	for(i = 0;i < 3;i++) {
		for(j = 0;j < 3;j++) {
			for(k = 0;k < 10;k++) {
				SpiLCD.setTextColor(aiSegBitColor[1][k], TFT_BLACK);
				SpiLCD.drawChar(apSegBitName[i][j][k][0], aiSegBitPosX[j][k] + 1, aiSegBitPosY[i] - 20);
				SpiLCD.drawChar(apSegBitName[i][j][k][1], aiSegBitPosX[j][k] + 1, aiSegBitPosY[i] - 10);

				SpiLCD.writeFillRect(aiSegBitPosX[j][k], aiSegBitPosY[i], 8, 20, aiSegBitColor[0][k]);
			}

			SpiLCD.drawRect(aiSegBitPosX[j][0] - 2, aiSegBitPosY[i] - 5, 102, 30, TFT_LIGHTGREY);
			aiSegBitPrev[i][j] = 0x00;
		}

		aiSegBitPrev[i][3] = 0x00;
	}

	SpiLCD.endWrite();
}
//------------------------------------------------------------------------------//
static void SpiLcdSegment(void) {
	Sint16 i, j, k;
	Uint08 iData;

	SpiLCD.startWrite();

	for(i = 0;i < 3;i++) {
		aiSegBitDiff[i][3] = aiSegBitPrev[i][3] ^ *(apSegBitData[i][3]);
		aiSegBitPrev[i][3] = *(apSegBitData[i][3]);

		if(aiSegBitDiff[i][3] != 0x00) {
			for(j = 0;j < 3;j++) {
				if((aiSegBitDiff[i][3] & aiSegBitMaskRB[j][0]) != 0x00) {
					iData = ((aiSegBitPrev[i][3] & aiSegBitMaskRB[j][0]) != 0);
					SpiLCD.writeFillRect(aiSegBitPosX[j][0], aiSegBitPosY[i], 8, 20, aiSegBitColor[iData][0]);
				}

				if((aiSegBitDiff[i][3] & aiSegBitMaskRB[j][1]) != 0x00) {
					iData = ((aiSegBitPrev[i][3] & aiSegBitMaskRB[j][1]) != 0);
					SpiLCD.writeFillRect(aiSegBitPosX[j][9], aiSegBitPosY[i], 8, 20, aiSegBitColor[iData][9]);
				}
			}
		}

		for(j = 0;j < 3;j++) {
			aiSegBitDiff[i][j] = aiSegBitPrev[i][j] ^ *(apSegBitData[i][j]);
			aiSegBitPrev[i][j] = *(apSegBitData[i][j]);

			if(aiSegBitDiff[i][j] != 0x00) {
				for(k = 1;k < 9;k++) {
					if((aiSegBitDiff[i][j] & aiSegBitMaskOYGS[k]) != 0x00) {
						iData = ((aiSegBitPrev[i][j] & aiSegBitMaskOYGS[k]) != 0);
						SpiLCD.writeFillRect(aiSegBitPosX[j][k], aiSegBitPosY[i], 8, 20, aiSegBitColor[iData][k]);
					}
				}
			}
		}
	}

	SpiLCD.endWrite();
}
//==============================================================================//


//==============================================================================//
static void SpiLcdBtnReset(void) {
	Sint08 i;

	for(i = 0;i < LcdButtonMax;i++) {
		asLcdButton[i].initButton(&SpiLCD, 75 + 110 * i, 95, 80, 50,
			TFT_PINK, TFT_BLACK, TFT_PINK, apLcdBtnLabel[i]);
	}
}
//------------------------------------------------------------------------------//
static void SpiLcdBtnClear(void) {
	Sint08 i;

	SpiLCD.setFont(&fonts::lgfxJapanGothic_16);
	iLcdBtnCount = 1000000 / LcdSegInterval;

	for(i = 0;i < LcdButtonMax;i++) {
		asLcdButton[i].drawButton();
	}
}
//------------------------------------------------------------------------------//
static void SpiLcdButton(void) {
	Sint08 i;
	Uint16 iPosX, iPosY;
	Sint08 iTouched = SpiLCD.getTouch(&iPosX, &iPosY);

	if(iLcdBtnCount > 0) iLcdBtnCount--;

	for(i = 0;i < LcdButtonMax;i++) {
		if((iTouched != False)&&(asLcdButton[i].contains(iPosX, iPosY) != False)) {
			asLcdButton[i].press(True);
		} else {
			asLcdButton[i].press(False);
		}

		if(asLcdButton[i].justReleased() != False) {
			asLcdButton[i].drawButton(False);
		}

		if(asLcdButton[i].justPressed() != False) {
			if(iLcdBtnCount > 0) continue;
			iLcdBtnCount = 1000000 / LcdSegInterval;

			asLcdButton[i].drawButton(True);
			TransCheck(aiLcdBtnControl[i]);
		}
	}
}
//==============================================================================//


//==============================================================================//
static void SpiLcdSorClear(void) {
	Sint16 i, j;

	for(i = 0;i < 4;i++) {
		for(j = 0;j < LcdSorStarMax;j++) {
			aiSorStarPos[i][j][X] = random( LcdScrnPixelX   );
			aiSorStarPos[i][j][Y] = random(-LcdScrnPixelY, 0);
			aiSorStarPos[i][j][Z] = False;
		}
	}
}
//------------------------------------------------------------------------------//
static void SpiLcdSorcerian(void) {
	Sint16 i, j;
	SpiLCD.startWrite();

	for(i = 0;i < 4;i++) {
		for(j = 0;j < LcdSorStarMax;j++) {
			if(aiSorStarPos[i][j][Z] != False) {
				if(SpiLCD.readPixel(aiSorStarPos[i][j][X], aiSorStarPos[i][j][Y]) != TFT_PINK) {
					SpiLCD.writePixel(aiSorStarPos[i][j][X], aiSorStarPos[i][j][Y], TFT_BLACK);
				}

				aiSorStarPos[i][j][Z] = False;
			}

			aiSorStarPos[i][j][X] += 1 + i;
			aiSorStarPos[i][j][Y] += 1 + i;

			if(aiSorStarPos[i][j][X] >= LcdScrnPixelX) aiSorStarPos[i][j][X] -= LcdScrnPixelX;
			if(aiSorStarPos[i][j][Y] < 0) continue;
			if(aiSorStarPos[i][j][Y] >= LcdScrnPixelY) aiSorStarPos[i][j][Y] -= LcdScrnPixelY;

			if(SpiLCD.readPixel(aiSorStarPos[i][j][X], aiSorStarPos[i][j][Y]) == TFT_BLACK) {
				SpiLCD.writePixel(aiSorStarPos[i][j][X], aiSorStarPos[i][j][Y], aiSorStarCol[i]);
				aiSorStarPos[i][j][Z] = True;
			}
		}
	}

	SpiLCD.endWrite();
}
//==============================================================================//


//==============================================================================//
static void SpiLcdTchCalib(void) {
	SpiLCD.setTextColor(TFT_WHITE, TFT_BLACK);
	SpiLCD.println("Touch corners as indicated.");
	SpiLCD.println();

	if(LcdTchCalibExec != False) {
		SpiLCD.setTextColor(TFT_RED, TFT_BLACK);
		SpiLCD.println("Set CalibExec to False to stop this running again!");
		SpiLCD.println();
	}

	SpiLCD.calibrateTouch(aiTchCalibData, TFT_MAGENTA, TFT_BLACK, 40);

	SpiLCD.setTextColor(TFT_GREEN, TFT_BLACK);
	SpiLCD.println("Calibration complete!");
}
//------------------------------------------------------------------------------//
static void SpiLcdTchReset(void) {
	Sint08 iCalibData = False;
	Uint08 iCalibSize = LcdTchCalibSize * sizeof(Uint16);

	SpiLCD.fillScreen(TFT_BLACK);
	SpiLCD.setCursor(0, 100);
	SpiLCD.setFont(&fonts::lgfxJapanGothic_16);

	if(!SPIFFS.begin()) {
		SpiLCD.setTextColor(TFT_YELLOW, TFT_BLACK);
		SpiLCD.println("Formatting file system");
		SpiLCD.println();

		SPIFFS.format();
		SPIFFS.begin();
	}

	if(SPIFFS.exists(pLcdTchCalibName)) {
		if(LcdTchCalibExec == False) {
			if(TchCalibFile = SPIFFS.open(pLcdTchCalibName, FILE_READ)) {
				if(TchCalibFile.read((Uint08*)aiTchCalibData, iCalibSize) == iCalibSize) iCalibData = True;
				TchCalibFile.close();
			}
		} else {
			SPIFFS.remove(pLcdTchCalibName);
		}
	}

	if((iCalibData == False)||(LcdTchCalibExec != False)) {
		SpiLcdTchCalib();
		delay(2000);

		if(TchCalibFile = SPIFFS.open(pLcdTchCalibName, FILE_WRITE)) {
			TchCalibFile.write((Uint08*)aiTchCalibData, iCalibSize);
			TchCalibFile.close();
		}
	} else SpiLCD.setTouchCalibrate(aiTchCalibData);
}
//==============================================================================//


//==============================================================================//
static void SpiLcdInit(Sint08 iReset) {
	iCurrLcdMode = False;
	iLcdSegMicros = iLcdSorMicros = micros();

	iLcdBrightness = 0x80;	iLcdRgbC0 = iLcdRgbC1 = 0x0000;
	iLcdRotation = LcdStdRotation;	iLcdTextSize = 1;

	iLcdPosX0 = iLcdPosY0 = iLcdPosX1 = iLcdPosY1 = 0;
	iLcdDisX2 = iLcdDisY2 = iLcdRadX3 = iLcdRadY3 = 0;

	if(Esp32Master) return;

	if(iReset == False) {
		SpiLCD.init();
		SpiLCD.setSwapBytes(True);
	}

	SpiLCD.setBrightness(iLcdBrightness);
	SpiLCD.setRotation(iLcdRotation);	SpiLCD.setTextSize(iLcdTextSize);

	if(iReset == False) {
		SpiLcdSegReset();	SpiLcdBtnReset();
		if(SpiLCD.touch()) SpiLcdTchReset();

		SpiLcdStarLogo();	delay(2000);
	}
}
//------------------------------------------------------------------------------//
static void SpiLcdMove(void) {
	Sint08 iLcdMode = LcdModeRead();
	Uint32 iMicros = micros();

	if(Esp32Master) {
		if(LcdUpdtRead() != False) {
			MultiData(CodeTelLcdUpdt);	MultiData(iCpuPortBus);		MultiData(iCpuDataBus);		LcdUpdtLow();
		}

		if(iLcdMode != iCurrLcdMode) iCurrLcdMode = iLcdMode;
		if((LcdModeSegment)&&((iMicros - iLcdSegMicros) > LcdSegInterval)) {	iLcdSegMicros = iMicros;	LcdUpdtHigh();	}

		return;
	}

	if(iLcdMode != iCurrLcdMode) {
		iCurrLcdMode = iLcdMode;

		if(LcdModeGraphic) {
			SpiLCD.setRotation(iLcdRotation);	SpiLCD.setTextSize(iLcdTextSize);
			SpiLCD.fillScreen(iLcdRgbC1);	SpiLcdGrpClear();	LcdUpdtLow();
		} else {
			SpiLCD.setRotation(LcdStdRotation);		SpiLCD.setTextSize(1);
			SpiLCD.fillScreen(TFT_BLACK);	SpiLcdSegClear();	SpiLcdBtnClear();	SpiLcdSorClear();
		}
	}

	if(LcdModeGraphic) {	SpiLcdGraphic();	LcdUpdtLow();	return;		}
	if(LcdUpdtRead() != False) {	SpiLcdSegment();	SpiLcdButton();		LcdUpdtLow();	}
	if((iMicros - iLcdSorMicros) > LcdSorInterval) {	iLcdSorMicros = iMicros;	SpiLcdSorcerian();	}
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

