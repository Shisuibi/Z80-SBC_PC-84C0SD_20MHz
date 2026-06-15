
//==============================================================================//
//	Z80 Single Board Computer "PC-84C0SD 20MHz" I/O Sketch for ESP32-S3-MINI-1	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
enum {
	X, Y, Z, XY = Z, W, XYZ = W, XYZW,					//	マトリクス参照
};
//------------------------------------------------------------------------------//
#define		False						0				//	論理値（偽）
#define		True						1				//	論理値（真）
//------------------------------------------------------------------------------//
#define		StringSizeS					0x0010			//	文字列幅（短）
#define		StringSizeL					0x0100			//	文字列幅（長）
//------------------------------------------------------------------------------//
typedef		const char					Cint08;			//	符号無し文字（08bit）
typedef		signed short				Sfix88;			//	固定小数点数（88bit）
//------------------------------------------------------------------------------//
typedef		unsigned char				Uint08;			//	符号無し整数（08bit）
typedef		signed char					Sint08;			//	符号有り整数（08bit）

typedef		unsigned short				Uint16;			//	符号無し整数（16bit）
typedef		signed short				Sint16;			//	符号有り整数（16bit）

typedef		unsigned long				Uint32;			//	符号無し整数（32bit）
typedef		signed long					Sint32;			//	符号有り整数（32bit）
//------------------------------------------------------------------------------//
typedef		signed long					Sfix08;			//	固定小数点数（08bit）
typedef		signed long					Sfix16;			//	固定小数点数（16bit）
typedef		signed long					Sfix24;			//	固定小数点数（24bit）
//------------------------------------------------------------------------------//
typedef		float						Sflt32;			//	浮動小数点数（32bit）
typedef		double						Sflt64;			//	浮動小数点数（64bit）
//==============================================================================//


//==============================================================================//
//								+3V3	|	GRND								//
//		Z80 CLOCK				GP00	|	ENBL	RST							//
//		Z80 RD					GP01	|	GP46	Z80 RESET					//
//		Z80 WR					GP02	|	GP45	Z80 WAIT					//
//		Z80 MREQ				GP03	|	GP44	U0RXD (Serial0 RXD)			//
//		Z80 IORQ				GP04	|	GP43	U0TXD (Serial0 TXD)			//
//		Z80 M1					GP05	|	GP42	Adrs Bus00					//
//		Z80 RFSH				GP06	|	GP41	Adrs Bus01					//
//		Z80 BUSACK				GP07	|	GP40	Adrs Bus02					//
//		Z80 HALT				GP08	|	GP39	Adrs Bus03					//
//		Data Bus00				GP09	|	GP38	Adrs Bus04					//
//		Data Bus01				GP10	|	GP37	Adrs Bus05					//
//		Data Bus02				GP11	|	GP36	Adrs Bus06					//
//		Data Bus03				GP12	|	GP35	Adrs Bus07					//
//		Data Bus04				GP13	|	GP34	RamChpEnbl					//
//		Data Bus05				GP14	|	GP33	PioWitEnbl					//
//		Data Bus06				GP15	|	GP26	Z80 NMI						//
//		Data Bus07				GP16	|	GP21	Z80 INT						//
//		U1TXD (Serial1 TXD)		GP17	|	GP20	U1CTS (H/W CDC)				//
//		U1RXD (Serial1 RXD)		GP18	|	GP19	U1RTS (H/W CDC)				//
//								+5V0	|	GP48	RGB LED (NeoPixel)			//
//								GRND	|	GP47	Z80 BUSREQ					//
//------------------------------------------------------------------------------//
//								+3V3	|	GRND								//
//		LCD LED					GP00	|	ENBL	RST							//
//		Z80 RD					GP01	|	GP46	LCD RST (& CTP RST)			//
//		Z80 WR					GP02	|	GP45	CTP INT						//
//		Z80 MREQ				GP03	|	GP44	U0RXD (Serial0 RXD)			//
//		Z80 IORQ				GP04	|	GP43	U0TXD (Serial0 TXD)			//
//		Z80 M1					GP05	|	GP42	Adrs Bus08					//
//		Z80 RFSH				GP06	|	GP41	Adrs Bus09					//
//		Z80 BUSACK				GP07	|	GP40	Adrs Bus10					//
//		Z80 HALT				GP08	|	GP39	Adrs Bus11					//
//		SDC SCS					GP09	|	GP38	Adrs Bus12					//
//		LCD SCS					GP10	|	GP37	Adrs Bus13					//
//		LCD SDI					GP11	|	GP36	Adrs Bus14					//
//		LCD SCK					GP12	|	GP35	Adrs Bus15					//
//		LCD SDO					GP13	|	GP34	LowBnkPage03				//
//		LCD DCR					GP14	|	GP33	LowBnkPage02				//
//		CTP SCL					GP15	|	GP26	LowBnkPage01				//
//		CTP SDA					GP16	|	GP21	LowBnkPage00				//
//		U1TXD (Serial1 TXD)		GP17	|	GP20	U1CTS (H/W CDC)				//
//		U1RXD (Serial1 RXD)		GP18	|	GP19	U1RTS (H/W CDC)				//
//								+5V0	|	GP48	RGB LED (NeoPixel)			//
//								GRND	|	GP47	Buzzer Tone					//
//------------------------------------------------------------------------------//
enum {
	GpioCpuClk,

	GpioPioRea,		GpioPioWri,		GpioPioMem,		GpioPioReq,
	GpioPioFet,		GpioPioRfs,		GpioPioBus,		GpioPioHal,

	GpioDatB00,		GpioDatB01,		GpioDatB02,		GpioDatB03,
	GpioDatB04,		GpioDatB05,		GpioDatB06,		GpioDatB07,

	GpioUa1Txd,		GpioUa1Rxd,		GpioUa1Rts,		GpioUa1Cts,

	GpioCpuInt,		NotAvail22,		NotAvail23,		NotAvail24,
	NotAvail25,		GpioCpuNmi,		NotAvail27,		NotAvail28,

	NotAvail29,		NotAvail30,		NotAvail31,		NotAvail32,
	GpioPioWit,		GpioRamChp,

	GpioAdrB07,		GpioAdrB06,		GpioAdrB05,		GpioAdrB04,
	GpioAdrB03,		GpioAdrB02,		GpioAdrB01,		GpioAdrB00,

	GpioUa0Txd,		GpioUa0Rxd,
	GpioCpuWit,		GpioCpuRst,		GpioBusReq,		GpioNeoPix,
//------------------------------------------------------------------------------//
	GpioLcdLed	=	GpioCpuClk,

	GpioSdcScs	=	GpioDatB00,		GpioLcdScs,		GpioLcdSdi,		GpioLcdSck,
	GpioLcdSdo,						GpioLcdDcr,		GpioCtpScl,		GpioCtpSda,

	GpioBnkP00	=	GpioCpuInt,		GpioBnkP01	=	GpioCpuNmi,
	GpioBnkP02	=	GpioPioWit,		GpioBnkP03,

	GpioAdrB15,		GpioAdrB14,		GpioAdrB13,		GpioAdrB12,
	GpioAdrB11,		GpioAdrB10,		GpioAdrB09,		GpioAdrB08,

	GpioCtpInt	=	GpioCpuWit,		GpioLcdRst,		GpioBuzTon,
};
//------------------------------------------------------------------------------//
#define		CpuClkOutput()				{	pinMode(GpioCpuClk, OUTPUT);	}

#define		CpuClkLow()					{	digitalWrite(GpioCpuClk, LOW );		}
#define		CpuClkHigh()				{	digitalWrite(GpioCpuClk, HIGH);		}
//------------------------------------------------------------------------------//
#define		PioReaInput()				{	pinMode(GpioPioRea, INPUT_PULLUP);	}
#define		PioReaOutput()				{	pinMode(GpioPioRea, OUTPUT);	}

#define		PioReaRead()				(	digitalRead(GpioPioRea)		)

#define		PioReaLow()					{	digitalWrite(GpioPioRea, LOW );		}
#define		PioReaHigh()				{	digitalWrite(GpioPioRea, HIGH);		}
//------------------------------------------------------------------------------//
#define		PioWriInput()				{	pinMode(GpioPioWri, INPUT_PULLUP);	}
#define		PioWriOutput()				{	pinMode(GpioPioWri, OUTPUT);	}

#define		PioWriRead()				(	digitalRead(GpioPioWri)		)

#define		PioWriLow()					{	digitalWrite(GpioPioWri, LOW );		}
#define		PioWriHigh()				{	digitalWrite(GpioPioWri, HIGH);		}
//------------------------------------------------------------------------------//
#define		PioMemInput()				{	pinMode(GpioPioMem, INPUT_PULLUP);	}
#define		PioMemOutput()				{	pinMode(GpioPioMem, OUTPUT);	}

#define		PioMemRead()				(	digitalRead(GpioPioMem)		)

#define		PioMemLow()					{	digitalWrite(GpioPioMem, LOW );		}
#define		PioMemHigh()				{	digitalWrite(GpioPioMem, HIGH);		}
//------------------------------------------------------------------------------//
#define		PioReqInput()				{	pinMode(GpioPioReq, INPUT_PULLUP);	}

#define		PioReqRead()				(	digitalRead(GpioPioReq)		)
//------------------------------------------------------------------------------//
#define		PioFetInput()				{	pinMode(GpioPioFet, INPUT_PULLUP);	}

#define		PioFetRead()				(	digitalRead(GpioPioFet)		)
//------------------------------------------------------------------------------//
#define		PioRfsInput()				{	pinMode(GpioPioRfs, INPUT_PULLUP);	}

#define		PioRfsRead()				(	digitalRead(GpioPioRfs)		)
//------------------------------------------------------------------------------//
#define		PioBusInput()				{	pinMode(GpioPioBus, INPUT_PULLUP);	}

#define		PioBusRead()				(	digitalRead(GpioPioBus)		)
//------------------------------------------------------------------------------//
#define		PioHalInput()				{	pinMode(GpioPioHal, INPUT_PULLUP);	}

#define		PioHalRead()				(	digitalRead(GpioPioHal)		)
//------------------------------------------------------------------------------//
#define		CpuIntOutput()				{	pinMode(GpioCpuInt, OUTPUT);	}

#define		CpuIntLow()					{	digitalWrite(GpioCpuInt, LOW );		}
#define		CpuIntHigh()				{	digitalWrite(GpioCpuInt, HIGH);		}
//------------------------------------------------------------------------------//
#define		CpuNmiOutput()				{	pinMode(GpioCpuNmi, OUTPUT);	}

#define		CpuNmiLow()					{	digitalWrite(GpioCpuNmi, LOW );		}
#define		CpuNmiHigh()				{	digitalWrite(GpioCpuNmi, HIGH);		}
//------------------------------------------------------------------------------//
#define		PioWitOutput()				{	pinMode(GpioPioWit, OUTPUT);	}

#define		PioWitLow()					{	digitalWrite(GpioPioWit, LOW );		}
#define		PioWitHigh()				{	digitalWrite(GpioPioWit, HIGH);		}
//------------------------------------------------------------------------------//
#define		RamChpOutput()				{	pinMode(GpioRamChp, OUTPUT);	}

#define		RamChpLow()					{	digitalWrite(GpioRamChp, LOW );		}
#define		RamChpHigh()				{	digitalWrite(GpioRamChp, HIGH);		}
//------------------------------------------------------------------------------//
#define		CpuWitOutput()				{	pinMode(GpioCpuWit, OUTPUT);	}

#define		CpuWitLow()					{	digitalWrite(GpioCpuWit, LOW );		}
#define		CpuWitHigh()				{	digitalWrite(GpioCpuWit, HIGH);		}
//------------------------------------------------------------------------------//
#define		CpuRstOutput()				{	pinMode(GpioCpuRst, OUTPUT);	}

#define		CpuRstLow()					{	digitalWrite(GpioCpuRst, LOW );		}
#define		CpuRstHigh()				{	digitalWrite(GpioCpuRst, HIGH);		}
//------------------------------------------------------------------------------//
#define		BusReqOutput()				{	pinMode(GpioBusReq, OUTPUT);	}

#define		BusReqLow()					{	digitalWrite(GpioBusReq, LOW );		}
#define		BusReqHigh()				{	digitalWrite(GpioBusReq, HIGH);		}
//------------------------------------------------------------------------------//
#define		BuzTonOutput()				{	pinMode(GpioBuzTon, OUTPUT);	}

#define		BuzTonLow()					{	digitalWrite(GpioBuzTon, LOW );		}
#define		BuzTonHigh()				{	digitalWrite(GpioBuzTon, HIGH);		}
//------------------------------------------------------------------------------//
#define		NeoPixInput()				{	pinMode(GpioNeoPix, INPUT_PULLDOWN);	}
#define		NeoPixOutput()				{	pinMode(GpioNeoPix, OUTPUT);	}

#define		NeoPixRead()				(	digitalRead(GpioNeoPix)		)
#define		NeoPixWrite(iCR, iCG, iCB)	{	neopixelWrite(GpioNeoPix, iCR, iCG, iCB);	}
//==============================================================================//


//==============================================================================//
//				|	Bit 07/03	|	Bit 06/02	|	Bit 05/01	|	Bit 04/00	//
//--------------+---------------+---------------+---------------+---------------//
//	SysInfoBit	|	RamChpEnbl	|	PioWitEnbl	|	CpuNmiRqst	|	CpuIntRqst	//
//				|	LowBnkPage	|	LowBnkPage	|	LowBnkPage	|	LowBnkPage	//
//--------------+---------------+---------------+---------------+---------------//
//	SysModeBit	|	SysExeCycl	|	CpuClkMode	|	CpuClkMode	|	CpuClkMode	//
//				|	CpuClkMode	|	CpuClkMode	|	CpuPioHist	|	CpuPioHist	//
//--------------+---------------+---------------+---------------+---------------//
//	SysCtrlBit	|	SysCtrlKey	|	SysLcdUpdt	|	(CtrlHALT)	|	(CtrlBSAK)	//
//				|	SysMsgDisp	|	SysLcdMode	|	SysSdcBusy	|	SysBleUart	//
//--------------+---------------+---------------+---------------+---------------//
//	CpuCtrlBit	|	CpuCtrHALT	|	CpuCtrBSAK	|	CpuCtrRFSH	|	CpuCtrM1	//
//				|	CpuCtrIORQ	|	CpuCtrMREQ	|	CpuCtrWR	|	CpuCtrRD	//
//--------------+---------------+---------------+---------------+---------------//
//				|		R		|	OYYY|GGGG	|		B		|				//
//--------------+---------------+---------------+---------------|				//
//	Segment A0	|	CpuCtrHALT	|	SysInfoBit	|	CpuCtrBSAK	|				//
//	Segment A1	|	SysMsgDisp	|	SysModeBit	|	SysLcdMode	|				//
//	Segment A2	|	SysSdcBusy	|	UartTxData	|	SysBleUart	|				//
//--------------+---------------+---------------+---------------|				//
//	Segment B0	|	AnyNumDt3D	|	AnyNumDt00	|	AnyNumDt3C	|				//
//	Segment B1	|	AnyNumDt3B	|	AnyNumDt01	|	AnyNumDt3A	|				//
//	Segment B2	|	AnyNumDt39	|	AnyNumDt02	|	AnyNumDt38	|				//
//--------------+---------------+---------------+---------------|				//
//	Segment C0	|	CpuCtrRFSH	|	CpuAdrsBus	|	CpuCtrM1	|				//
//	Segment C1	|	CpuCtrIORQ	|	CpuPortBus	|	CpuCtrMREQ	|				//
//	Segment C2	|	CpuCtrWR	|	CpuDataBus	|	CpuCtrRD	|				//
//--------------+---------------+---------------+---------------+---------------//
//				|				|		0		|		1		|		2		//
//				|---------------+---------------+---------------+---------------//
//				|		A		|	SysInfoBit	|	SysModeBit	|	UartTxData	//
//				|		B		|	AnyNumDt00	|	AnyNumDt01	|	AnyNumDt02	//
//				|		C		|	CpuAdrsBus	|	CpuPortBus	|	CpuDataBus	//
//------------------------------------------------------------------------------//
#define		BnkPageRead()				(	(iSysInfoBit & 0x0F) >> 0	)

#define		BnkPageWrite(iBnkPage)		{	iSysInfoBit = (iSysInfoBit & 0xF0) |	\
											((iBnkPage) & 0x0F);	}
//------------------------------------------------------------------------------//
#define		IntRqstRead()				(	(iSysInfoBit & 0x10) >> 4	)

#define		IntRqstLow()				{	iSysInfoBit &= 0xEF;	}
#define		IntRqstHigh()				{	iSysInfoBit |= 0x10;	}
//------------------------------------------------------------------------------//
#define		NmiRqstRead()				(	(iSysInfoBit & 0x20) >> 5	)

#define		NmiRqstLow()				{	iSysInfoBit &= 0xDF;	}
#define		NmiRqstHigh()				{	iSysInfoBit |= 0x20;	}
//------------------------------------------------------------------------------//
#define		WitEnblRead()				(	(iSysInfoBit & 0x40) >> 6	)

#define		WitEnblLow()				{	iSysInfoBit &= 0xBF;	}
#define		WitEnblHigh()				{	iSysInfoBit |= 0x40;	}
//------------------------------------------------------------------------------//
#define		ChpEnblRead()				(	(iSysInfoBit & 0x80) >> 7	)

#define		ChpEnblLow()				{	iSysInfoBit &= 0x7F;	}
#define		ChpEnblHigh()				{	iSysInfoBit |= 0x80;	}
//------------------------------------------------------------------------------//
#define		PioHistRead()				(	iSysModeBit & 0x03	)

#define		PioHistWrite(iPioHist)		{	iSysModeBit = (iSysModeBit & 0xFC) |	\
											((iPioHist) & 0x03);	}
//------------------------------------------------------------------------------//
#define		ClkModeWrite(iClkMode)		{	iSysModeBit = (iSysModeBit & 0x83) |	\
											(asClockInfo[iClkMode].iDisp);		}
//------------------------------------------------------------------------------//
#define		ExeCyclRead()				(	(iSysModeBit & 0x80) >> 7	)

#define		ExeCyclLow()				{	iSysModeBit &= 0x7F;	}
#define		ExeCyclHigh()				{	iSysModeBit |= 0x80;	}
//------------------------------------------------------------------------------//
#define		BleUartRead()				(	(iSysCtrlBit & 0x01) >> 0	)

#define		BleUartLow()				{	iSysCtrlBit &= 0xFE;	}
#define		BleUartHigh()				{	iSysCtrlBit |= 0x01;	}
//------------------------------------------------------------------------------//
#define		SdcBusyRead()				(	(iSysCtrlBit & 0x02) >> 1	)

#define		SdcBusyLow()				{	iSysCtrlBit &= 0xFD;	}
#define		SdcBusyHigh()				{	iSysCtrlBit |= 0x02;	}
//------------------------------------------------------------------------------//
#define		LcdModeRead()				(	(iSysCtrlBit & 0x04) >> 2	)

#define		LcdModeLow()				{	iSysCtrlBit &= 0xFB;	}
#define		LcdModeHigh()				{	iSysCtrlBit |= 0x04;	}
//------------------------------------------------------------------------------//
#define		MsgDispRead()				(	(iSysCtrlBit & 0x08) >> 3	)

#define		MsgDispLow()				{	iSysCtrlBit &= 0xF7;	}
#define		MsgDispHigh()				{	iSysCtrlBit |= 0x08;	}
//------------------------------------------------------------------------------//
#define		LcdUpdtRead()				(	(iSysCtrlBit & 0x40) >> 6	)

#define		LcdUpdtLow()				{	iSysCtrlBit &= 0xBF;	}
#define		LcdUpdtHigh()				{	iSysCtrlBit |= 0x40;	}
//------------------------------------------------------------------------------//
#define		CtrlKeyRead()				(	(iSysCtrlBit & 0x80) >> 7	)

#define		CtrlKeyLow()				{	iSysCtrlBit &= 0x7F;	}
#define		CtrlKeyHigh()				{	iSysCtrlBit |= 0x80;	}
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static Sint08 iResetRequest;							//	リセット要求
//------------------------------------------------------------------------------//
static Uint08 iCpmBiosParamL;							//	BIOS引数下位
static Uint08 iCpmBiosParamH;							//	BIOS引数上位
//------------------------------------------------------------------------------//
static Uint08 iSysInfoBit;								//	システム情報ビット
static Uint08 iSysModeBit;								//	システム状態ビット
static Uint08 iSysCtrlBit;								//	システム制御ビット
static Uint08 iUartTxData;								//	シリアル送信データ
//------------------------------------------------------------------------------//
static Uint08 iAnyNumDt00;								//	ユーザー任意データ00
static Uint08 iAnyNumDt01;								//	ユーザー任意データ01
static Uint08 iAnyNumDt02;								//	ユーザー任意データ02
static Uint08 iAnyNumDt03;								//	ユーザー任意データ03
//------------------------------------------------------------------------------//
static Uint08 iCpuAdrsBus;								//	Z84C0020 Adrsバス
static Uint08 iCpuPortBus;								//	Z84C0020 Portバス
static Uint08 iCpuDataBus;								//	Z84C0020 Dataバス
static Uint08 iCpuCtrlBit;								//	Z84C0020 制御ビット
//------------------------------------------------------------------------------//
static Uint08 iPioPortBus;								//	Z84C0020 Portバス（PIO）
static Uint08 iPioDataBus;								//	Z84C0020 Dataバス（PIO）
//------------------------------------------------------------------------------//
static Uint08 iPioHistD01;								//	CPU入出力履歴データ01
static Uint08 iPioHistD02;								//	CPU入出力履歴データ02
static Uint08 iPioHistD03;								//	CPU入出力履歴データ03
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
//------------------------------------------------------------------------------//
static LGFX SpiLCD;										//	LGFX識別子（SPI2）
static LGFX_Sprite Canvas(&SpiLCD);						//	LGFX識別子（スプライト）
//==============================================================================//


//==============================================================================//
static void DataBusInput(void) {
	pinMode(GpioDatB00, INPUT_PULLDOWN);
	pinMode(GpioDatB01, INPUT_PULLDOWN);
	pinMode(GpioDatB02, INPUT_PULLDOWN);
	pinMode(GpioDatB03, INPUT_PULLDOWN);

	pinMode(GpioDatB04, INPUT_PULLDOWN);
	pinMode(GpioDatB05, INPUT_PULLDOWN);
	pinMode(GpioDatB06, INPUT_PULLDOWN);
	pinMode(GpioDatB07, INPUT_PULLDOWN);
}
//------------------------------------------------------------------------------//
static void DataBusOutput(void) {
	pinMode(GpioDatB00, OUTPUT);
	pinMode(GpioDatB01, OUTPUT);
	pinMode(GpioDatB02, OUTPUT);
	pinMode(GpioDatB03, OUTPUT);

	pinMode(GpioDatB04, OUTPUT);
	pinMode(GpioDatB05, OUTPUT);
	pinMode(GpioDatB06, OUTPUT);
	pinMode(GpioDatB07, OUTPUT);
}
//------------------------------------------------------------------------------//
static Uint08 DataBusRead(void) {
	Uint08 iData;

	iData =                digitalRead(GpioDatB07);
	iData = (iData << 1) | digitalRead(GpioDatB06);
	iData = (iData << 1) | digitalRead(GpioDatB05);
	iData = (iData << 1) | digitalRead(GpioDatB04);

	iData = (iData << 1) | digitalRead(GpioDatB03);
	iData = (iData << 1) | digitalRead(GpioDatB02);
	iData = (iData << 1) | digitalRead(GpioDatB01);
	iData = (iData << 1) | digitalRead(GpioDatB00);

	return(iData);
}
//------------------------------------------------------------------------------//
static void DataBusWrite(Uint08 iData) {
	digitalWrite(GpioDatB00,  iData        & 0x01);
	digitalWrite(GpioDatB01, (iData >>= 1) & 0x01);
	digitalWrite(GpioDatB02, (iData >>= 1) & 0x01);
	digitalWrite(GpioDatB03, (iData >>= 1) & 0x01);

	digitalWrite(GpioDatB04, (iData >>= 1) & 0x01);
	digitalWrite(GpioDatB05, (iData >>= 1) & 0x01);
	digitalWrite(GpioDatB06, (iData >>= 1) & 0x01);
	digitalWrite(GpioDatB07, (iData >>= 1) & 0x01);
}
//==============================================================================//


//==============================================================================//
static void CtrlBitInput(void) {
	pinMode(GpioPioRea, INPUT_PULLUP);
	pinMode(GpioPioWri, INPUT_PULLUP);
	pinMode(GpioPioMem, INPUT_PULLUP);
	pinMode(GpioPioReq, INPUT_PULLUP);

	pinMode(GpioPioFet, INPUT_PULLUP);
	pinMode(GpioPioRfs, INPUT_PULLUP);
	pinMode(GpioPioBus, INPUT_PULLUP);
	pinMode(GpioPioHal, INPUT_PULLUP);
}
//------------------------------------------------------------------------------//
static Uint08 CtrlBitRead(void) {
	Uint08 iCtrl;

	iCtrl =                digitalRead(GpioPioHal);
	iCtrl = (iCtrl << 1) | digitalRead(GpioPioBus);
	iCtrl = (iCtrl << 1) | digitalRead(GpioPioRfs);
	iCtrl = (iCtrl << 1) | digitalRead(GpioPioFet);

	iCtrl = (iCtrl << 1) | digitalRead(GpioPioReq);
	iCtrl = (iCtrl << 1) | digitalRead(GpioPioMem);
	iCtrl = (iCtrl << 1) | digitalRead(GpioPioWri);
	iCtrl = (iCtrl << 1) | digitalRead(GpioPioRea);

	return(iCtrl);
}
//------------------------------------------------------------------------------//
static void LowBnkOutput(void) {
	pinMode(GpioBnkP00, OUTPUT);
	pinMode(GpioBnkP01, OUTPUT);
	pinMode(GpioBnkP02, OUTPUT);
	pinMode(GpioBnkP03, OUTPUT);
}
//------------------------------------------------------------------------------//
static void LowBnkWrite(Uint08 iPage) {
	digitalWrite(GpioBnkP00,  iPage        & 0x01);
	digitalWrite(GpioBnkP01, (iPage >>= 1) & 0x01);
	digitalWrite(GpioBnkP02, (iPage >>= 1) & 0x01);
	digitalWrite(GpioBnkP03, (iPage >>= 1) & 0x01);
}
//==============================================================================//


//==============================================================================//
static void PortBusInput(void) {
	pinMode(GpioAdrB00, INPUT_PULLDOWN);
	pinMode(GpioAdrB01, INPUT_PULLDOWN);
	pinMode(GpioAdrB02, INPUT_PULLDOWN);
	pinMode(GpioAdrB03, INPUT_PULLDOWN);

	pinMode(GpioAdrB04, INPUT_PULLDOWN);
	pinMode(GpioAdrB05, INPUT_PULLDOWN);
	pinMode(GpioAdrB06, INPUT_PULLDOWN);
	pinMode(GpioAdrB07, INPUT_PULLDOWN);
}
//------------------------------------------------------------------------------//
static void PortBusOutput(void) {
	pinMode(GpioAdrB00, OUTPUT);
	pinMode(GpioAdrB01, OUTPUT);
	pinMode(GpioAdrB02, OUTPUT);
	pinMode(GpioAdrB03, OUTPUT);

	pinMode(GpioAdrB04, OUTPUT);
	pinMode(GpioAdrB05, OUTPUT);
	pinMode(GpioAdrB06, OUTPUT);
	pinMode(GpioAdrB07, OUTPUT);
}
//------------------------------------------------------------------------------//
static Uint08 PortBusRead(void) {
	Uint08 iPort;

	iPort =                digitalRead(GpioAdrB07);
	iPort = (iPort << 1) | digitalRead(GpioAdrB06);
	iPort = (iPort << 1) | digitalRead(GpioAdrB05);
	iPort = (iPort << 1) | digitalRead(GpioAdrB04);

	iPort = (iPort << 1) | digitalRead(GpioAdrB03);
	iPort = (iPort << 1) | digitalRead(GpioAdrB02);
	iPort = (iPort << 1) | digitalRead(GpioAdrB01);
	iPort = (iPort << 1) | digitalRead(GpioAdrB00);

	return(iPort);
}
//------------------------------------------------------------------------------//
static void PortBusWrite(Uint08 iPort) {
	digitalWrite(GpioAdrB00,  iPort        & 0x01);
	digitalWrite(GpioAdrB01, (iPort >>= 1) & 0x01);
	digitalWrite(GpioAdrB02, (iPort >>= 1) & 0x01);
	digitalWrite(GpioAdrB03, (iPort >>= 1) & 0x01);

	digitalWrite(GpioAdrB04, (iPort >>= 1) & 0x01);
	digitalWrite(GpioAdrB05, (iPort >>= 1) & 0x01);
	digitalWrite(GpioAdrB06, (iPort >>= 1) & 0x01);
	digitalWrite(GpioAdrB07, (iPort >>= 1) & 0x01);
}
//==============================================================================//


//==============================================================================//
static void AdrsBusInput(void) {
	pinMode(GpioAdrB08, INPUT_PULLDOWN);
	pinMode(GpioAdrB09, INPUT_PULLDOWN);
	pinMode(GpioAdrB10, INPUT_PULLDOWN);
	pinMode(GpioAdrB11, INPUT_PULLDOWN);

	pinMode(GpioAdrB12, INPUT_PULLDOWN);
	pinMode(GpioAdrB13, INPUT_PULLDOWN);
	pinMode(GpioAdrB14, INPUT_PULLDOWN);
	pinMode(GpioAdrB15, INPUT_PULLDOWN);
}
//------------------------------------------------------------------------------//
static void AdrsBusOutput(void) {
	pinMode(GpioAdrB08, OUTPUT);
	pinMode(GpioAdrB09, OUTPUT);
	pinMode(GpioAdrB10, OUTPUT);
	pinMode(GpioAdrB11, OUTPUT);

	pinMode(GpioAdrB12, OUTPUT);
	pinMode(GpioAdrB13, OUTPUT);
	pinMode(GpioAdrB14, OUTPUT);
	pinMode(GpioAdrB15, OUTPUT);
}
//------------------------------------------------------------------------------//
static Uint08 AdrsBusRead(void) {
	Uint08 iAdrs;

	iAdrs =                digitalRead(GpioAdrB15);
	iAdrs = (iAdrs << 1) | digitalRead(GpioAdrB14);
	iAdrs = (iAdrs << 1) | digitalRead(GpioAdrB13);
	iAdrs = (iAdrs << 1) | digitalRead(GpioAdrB12);

	iAdrs = (iAdrs << 1) | digitalRead(GpioAdrB11);
	iAdrs = (iAdrs << 1) | digitalRead(GpioAdrB10);
	iAdrs = (iAdrs << 1) | digitalRead(GpioAdrB09);
	iAdrs = (iAdrs << 1) | digitalRead(GpioAdrB08);

	return(iAdrs);
}
//------------------------------------------------------------------------------//
static void AdrsBusWrite(Uint08 iAdrs) {
	digitalWrite(GpioAdrB08,  iAdrs        & 0x01);
	digitalWrite(GpioAdrB09, (iAdrs >>= 1) & 0x01);
	digitalWrite(GpioAdrB10, (iAdrs >>= 1) & 0x01);
	digitalWrite(GpioAdrB11, (iAdrs >>= 1) & 0x01);

	digitalWrite(GpioAdrB12, (iAdrs >>= 1) & 0x01);
	digitalWrite(GpioAdrB13, (iAdrs >>= 1) & 0x01);
	digitalWrite(GpioAdrB14, (iAdrs >>= 1) & 0x01);
	digitalWrite(GpioAdrB15, (iAdrs >>= 1) & 0x01);
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

