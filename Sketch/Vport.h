
//==============================================================================//
//	Z80 Single Board Computer "PC-84C0SD 20MHz" I/O Sketch for ESP32-S3-MINI-1	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
enum {
	PioModeStandBy  ,									//	PIOモード（待機）
	PioModeInput    ,									//	PIOモード（入力）
	PioModeOutput   ,									//	PIOモード（出力）
	PioModeInterrupt,									//	PIOモード（割込）
	PioModeContinue ,									//	PIOモード（続行）
	PioModeMulti    ,									//	PIOモード（相互）
	PioModeSlave    ,									//	PIOモード（隷属）

	PioModeMax,											//	PIOモード上限
};
//------------------------------------------------------------------------------//
#define		PortStandBySlave			0x0F			//	PIOポート（隷属待機）

#define		PortUartCtrl				0x30			//	PIOポート（UART制御）
#define		PortBiosConSt				0x81			//	PIOポート（UART制御）
//------------------------------------------------------------------------------//
#define		PioInput	(iCurrPioMode == PioModeInput )		//	PIOモード（入力）
#define		PioOutput	(iCurrPioMode == PioModeOutput)		//	PIOモード（出力）
//------------------------------------------------------------------------------//
static Sint08 IsPioStandBy(void);
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static Sint08 iCurrPioMode;								//	現行PIOモード
static Sint08 iReleaseCpuBus;							//	CPUバス開放
//------------------------------------------------------------------------------//
static Uint16 iRomImageAdrs;							//	ROM読込アドレス
//==============================================================================//


//==============================================================================//
static void VportMakeDisk(void) {
	if(PioInput) {
				if((iRomImageAdrs >= 0x0000)&&(iRomImageAdrs < 0x0100))
					iPioDataBus = aiRomBootImage[0x0000 + iRomImageAdrs];
		else	if((iRomImageAdrs >= 0x0100)&&(iRomImageAdrs < CpmBiosBaseAdrs))
					iPioDataBus = 0xFF;
		else	if((iRomImageAdrs >= CpmBiosBaseAdrs)&&(iRomImageAdrs <= 0xFFFF))
					iPioDataBus = aiRomBootImage[0x0100 + iRomImageAdrs - CpmBiosBaseAdrs];
		iRomImageAdrs++;
	} else {
		if((iCpmBiosParamH == '1')||(iCpmBiosParamH == '2')) {
			if(Esp32Master) iReleaseCpuBus = False;
			iCpmBiosParamL = iPioDataBus;	iCurrCpmMode = CpmModeMakeDisk;
			LcdModeLow();	CtrlKeyLow();
		}
	}
}
//------------------------------------------------------------------------------//
static void VportSysReset(void) {
	if(PioInput) {	iPioDataBus = 0x00;		MultiCpuOutput();	}
	TransSysReset();
}
//------------------------------------------------------------------------------//
static void VportBiosParamL(void) {
	if(PioInput) iPioDataBus = iCpmBiosParamL;
	else {
		iCpmBiosParamL = iPioDataBus;
		iRomImageAdrs = (iRomImageAdrs & 0xFF00) | (((Uint16)iPioDataBus) << 0);
	}
}
//------------------------------------------------------------------------------//
static void VportBiosParamH(void) {
	if(PioInput) iPioDataBus = iCpmBiosParamH;
	else {
		iCpmBiosParamH = iPioDataBus;
		iRomImageAdrs = (iRomImageAdrs & 0x00FF) | (((Uint16)iPioDataBus) << 8);
	}
}
//------------------------------------------------------------------------------//
static void VportHelpText(void) {
	if(PioInput) {	iPioDataBus = 0x00;		MultiCpuOutput();	}
	TransHelpText();
}
//------------------------------------------------------------------------------//
static void VportClrScrn(void) {
	if(PioInput) {	iPioDataBus = 0x00;		MultiCpuOutput();	}
	TransClrScrn();
}
//------------------------------------------------------------------------------//
static void VportMsgDisp(void) {
	if(PioInput) iPioDataBus = MsgDispRead();
	else {		if((iPioDataBus == False)&&(MsgDispRead() != False))
			{	TransMsgDisp(pTransMsgDispOff);		MsgDispLow();	}
		else	if((iPioDataBus != False)&&(MsgDispRead() == False))
			{	MsgDispHigh();	TransMsgDisp(pTransMsgDispOn);	}
	}
}
//------------------------------------------------------------------------------//
static void VportLcdStarLogo(void) {
	if(PioInput) {	iPioDataBus = 0x00;		MultiCpuOutput();	}
	TransLcdStarLogo();
}
//------------------------------------------------------------------------------//
static void VportLcdScrnShot(void) {
	if(PioInput) {	iPioDataBus = 0x00;		MultiCpuOutput();	}
	TransLcdScrnShot();
}
//------------------------------------------------------------------------------//
static void VportCtrlKey(void) {
	if(PioInput) iPioDataBus = CtrlKeyRead();
	else {		if((iPioDataBus == False)&&(CtrlKeyRead() != False))
			{	TransMsgDisp(pTransCtrlKeyOff);		CtrlKeyLow();	}
		else	if((iPioDataBus != False)&&(CtrlKeyRead() == False))
			{	CtrlKeyHigh();	TransMsgDisp(pTransCtrlKeyOn);	}
	}
}
//==============================================================================//


//==============================================================================//
static void VportMelNote(void) {
	if(PioInput)	iPioDataBus = iCurrMelNote;
	else			ClockMelody(iPioDataBus, iCurrMelVolume);
}
//------------------------------------------------------------------------------//
static void VportMelVolume(void) {
	if(PioInput)	iPioDataBus = iCurrMelVolume;
	else			ClockMelody(iCurrMelNote, iPioDataBus);
}
//------------------------------------------------------------------------------//
static void VportBnkPage(void) {
	if(PioInput) iPioDataBus = BnkPageRead();
	else {	BnkPageWrite(iPioDataBus);	if(Esp32Slave) LowBnkWrite(BnkPageRead());	}
	if(Esp32Master)		iReleaseCpuBus = False;
	else				MultiData(CodeTelBnkPage);
}
//------------------------------------------------------------------------------//
static void VportIntRqst(void) {
	if(PioInput) iPioDataBus = IntRqstRead();
	else {
		if(iPioDataBus == False)	{	IntRqstLow();	if(Esp32Master) CpuIntHigh();	}
		else						{	IntRqstHigh();	if(Esp32Master) CpuIntLow();	}
	}
}
//------------------------------------------------------------------------------//
static void VportNmiRqst(void) {
	if(PioInput) iPioDataBus = NmiRqstRead();
	else {
		if(iPioDataBus == False)	{	NmiRqstLow();	if(Esp32Master) CpuNmiHigh();	}
		else						{	NmiRqstHigh();	if(Esp32Master) CpuNmiLow();	}
	}
}
//------------------------------------------------------------------------------//
static void VportWitEnbl(void) {
	if(PioInput) iPioDataBus = WitEnblRead();
	else {
		if(iPioDataBus == False)	{	WitEnblLow();	if(Esp32Master) PioWitHigh();	}
		else						{	WitEnblHigh();	if(Esp32Master) PioWitLow();	}
	}
}
//------------------------------------------------------------------------------//
static void VportChpEnbl(void) {
	if(PioInput) iPioDataBus = ChpEnblRead();
	else {
		if(iPioDataBus == False)	{	ChpEnblLow();	if(Esp32Master) RamChpHigh();	}
		else						{	ChpEnblHigh();	if(Esp32Master) RamChpLow();	}
	}
}
//------------------------------------------------------------------------------//
static void VportTimerCentis(void) {
	if(PioInput) iPioDataBus = iTimerCentis;
	else {	iTimerCentis = iPioDataBus;		iTimerMillis = millis() - iTimerCentis * 10;	}
}
//------------------------------------------------------------------------------//
static void VportTimerSecond(void) {
	if(PioInput)	iPioDataBus = iTimerSecond;
	else			iTimerSecond = iPioDataBus;
}
//------------------------------------------------------------------------------//
static void VportTimerMinute(void) {
	if(PioInput)	iPioDataBus = iTimerMinute;
	else			iTimerMinute = iPioDataBus;
}
//------------------------------------------------------------------------------//
static void VportTimerHour24(void) {
	if(PioInput)	iPioDataBus = iTimerHour24;
	else			iTimerHour24 = iPioDataBus;
}
//==============================================================================//


//==============================================================================//
static void VportAnyNum00(void) {
	if(PioInput)	iPioDataBus = iAnyNumDt00;
	else			iAnyNumDt00 = iPioDataBus;
}
//------------------------------------------------------------------------------//
static void VportAnyNum01(void) {
	if(PioInput)	iPioDataBus = iAnyNumDt01;
	else			iAnyNumDt01 = iPioDataBus;
	if(PioHistRead() == 0) iPioHistD01 = iAnyNumDt01;
}
//------------------------------------------------------------------------------//
static void VportAnyNum02(void) {
	if(PioInput)	iPioDataBus = iAnyNumDt02;
	else			iAnyNumDt02 = iPioDataBus;
	if(PioHistRead() == 0) iPioHistD02 = iAnyNumDt02;
}
//------------------------------------------------------------------------------//
static void VportAnyNum03(void) {
	if(PioInput)	iPioDataBus = iAnyNumDt03;
	else			iAnyNumDt03 = iPioDataBus & 0x3F;
	if(PioHistRead() == 0)	iPioHistD03 = iAnyNumDt03;
	else					iPioHistD03 = (iAnyNumDt03 & 0x30) | (iPioHistD03 & 0x0F);
	if(((iPioDataBus & 0x40) != 0x00)&&(Esp32Master)) NeoPixWrite(iAnyNumDt00, iAnyNumDt01, iAnyNumDt02);
	if(((iPioDataBus & 0x80) != 0x00)&&(Esp32Slave )) NeoPixWrite(iAnyNumDt00, iAnyNumDt01, iAnyNumDt02);
}
//------------------------------------------------------------------------------//
static void VportPioHist(void) {
	if(PioInput)	iPioDataBus = PioHistRead();
	else			TransChangeHist(iPioDataBus);
}
//------------------------------------------------------------------------------//
static void VportExeCycl(void) {
	if(PioInput) iPioDataBus = ExeCyclRead();
	else {		if((iPioDataBus == False)&&(ExeCyclRead() != False))
			{	TransMsgDisp(pTransExeCyclOff);		ExeCyclLow();	}
		else	if((iPioDataBus != False)&&(ExeCyclRead() == False))
			{	ExeCyclHigh();	TransMsgDisp(pTransExeCyclOn);	}
	}
}
//------------------------------------------------------------------------------//
static void VportClkMode(void) {
	if(PioInput) iPioDataBus = (Uint08)iCurrClkMode;
	else {	if(iPioDataBus < ClockModeMax) ClockControl(iPioDataBus);	}
}
//------------------------------------------------------------------------------//
static void VportClockReset(void) {
	if(PioInput) {	iPioDataBus = 0x00;		MultiCpuOutput();	}
	ClockReset();
}
//------------------------------------------------------------------------------//
static void VportClockSec(void) {
	if(PioInput) iPioDataBus = iClockSec;
}
//------------------------------------------------------------------------------//
static void VportClockMin(void) {
	if(PioInput) iPioDataBus = iClockMin;
}
//------------------------------------------------------------------------------//
static void VportClockHour(void) {
	if(PioInput) iPioDataBus = iClockHour;
}
//------------------------------------------------------------------------------//
static void VportClockDay(void) {
	if(PioInput) iPioDataBus = iClockDay;
}
//------------------------------------------------------------------------------//
static void VportClockMon(void) {
	if(PioInput) iPioDataBus = iClockMon;
}
//------------------------------------------------------------------------------//
static void VportClockYear(void) {
	if(PioInput) iPioDataBus = iClockYear;
}
//------------------------------------------------------------------------------//
static void VportClockWeek(void) {
	if(PioInput) iPioDataBus = iClockWeek;
}
//==============================================================================//


//==============================================================================//
static void VportUartCtrl(void) {
	if(PioInput)	iPioDataBus = SliceData(False);
	else			TransCheck(iPioDataBus);
}
//------------------------------------------------------------------------------//
static void VportUartData(void) {
	if(PioInput)	iPioDataBus = SliceData(True);
	else			TransData(iPioDataBus);
}
//------------------------------------------------------------------------------//
static void VportBleUart(void) {
	if(PioInput) iPioDataBus = BleUartRead();
}
//------------------------------------------------------------------------------//
static void VportSdcBusy(void) {
	if(PioInput) iPioDataBus = SdcBusyRead();
}
//------------------------------------------------------------------------------//
static void VportLcdMode(void) {
	if(PioInput) iPioDataBus = LcdModeRead();
	else {		if((iPioDataBus == False)&&(LcdModeRead() != False))
			{	TransMsgDisp(pTransLcdModeOff);		LcdModeLow();	}
		else	if((iPioDataBus != False)&&(LcdModeRead() == False))
			{	LcdModeHigh();	TransMsgDisp(pTransLcdModeOn);	}
	}
}
//------------------------------------------------------------------------------//
static void VportLcdLedBright(void) {
	if(PioInput) iPioDataBus = iLcdBrightness;
	else {	iLcdBrightness = iPioDataBus;	if(Esp32Slave) SpiLCD.setBrightness(iPioDataBus);	}
}
//------------------------------------------------------------------------------//
static void VportBasicFile(void) {
	if(PioInput) iPioDataBus = BasicFileMax;
	else {	if(iPioDataBus < BasicFileMax) BasicControl(iPioDataBus);	}
}
//==============================================================================//


//==============================================================================//
static void VportLcdRotation(void) {
	if(PioInput) iPioDataBus = iLcdRotation;
	else {
		if((Esp32Master)||(LcdModeSegment)) return;
		SpiLCD.setRotation(iLcdRotation = iPioDataBus);
	}
}
//------------------------------------------------------------------------------//
static void VportLcdClrScrn(void) {
	if(PioInput) iPioDataBus = (Uint08)iCurrLcdMode;
	else {
		if((Esp32Master)||(LcdModeSegment)) return;
		if(iPioDataBus == False)	SpiLCD.fillScreen(iLcdRgbC0);
		else						SpiLCD.fillScreen(iLcdRgbC1);
	}
}
//------------------------------------------------------------------------------//
static void VportLcdTextSize(void) {
	if(PioInput) iPioDataBus = iLcdTextSize;
	else {	if(Esp32Slave) SpiLCD.setTextSize(iLcdTextSize = iPioDataBus);	}
}
//------------------------------------------------------------------------------//
static void VportLcdDrawChar(void) {
	if(PioInput) iPioDataBus = (Uint08)iCurrLcdMode;
	else {
		if(LcdModeSegment) return;
		if(Esp32Slave) {
			SpiLCD.setTextColor(iLcdRgbC0, iLcdRgbC1);
			SpiLCD.drawChar(iPioDataBus, iLcdRadX3, iLcdRadY3, iLcdTextSize);
		}
		iLcdRadX3 += iLcdDisX2;		iLcdRadY3 += iLcdDisY2;
	}
}
//------------------------------------------------------------------------------//
static void VportLcdRgbC0L(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdRgbC0 & 0x00FF);
	else			iLcdRgbC0 = (iLcdRgbC0 & 0xFF00) | (((Uint16)iPioDataBus) << 0);
}
//------------------------------------------------------------------------------//
static void VportLcdRgbC0H(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdRgbC0 >> 8);
	else			iLcdRgbC0 = (iLcdRgbC0 & 0x00FF) | (((Uint16)iPioDataBus) << 8);
}
//------------------------------------------------------------------------------//
static void VportLcdRgbC1L(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdRgbC1 & 0x00FF);
	else			iLcdRgbC1 = (iLcdRgbC1 & 0xFF00) | (((Uint16)iPioDataBus) << 0);
}
//------------------------------------------------------------------------------//
static void VportLcdRgbC1H(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdRgbC1 >> 8);
	else			iLcdRgbC1 = (iLcdRgbC1 & 0x00FF) | (((Uint16)iPioDataBus) << 8);
}
//------------------------------------------------------------------------------//
static void VportLcdDrawPixel(void) {
	if(PioInput) iPioDataBus = (Uint08)iCurrLcdMode;
	else {
		if((Esp32Master)||(LcdModeSegment)) return;
		if(iPioDataBus == False)	SpiLCD.drawPixel(iLcdPosX0, iLcdPosY0, iLcdRgbC0);
		else						SpiLCD.drawPixel(iLcdPosX1, iLcdPosY1, iLcdRgbC1);
	}
}
//------------------------------------------------------------------------------//
static void VportLcdDrawLine(void) {
	if(PioInput) iPioDataBus = (Uint08)iCurrLcdMode;
	else {
		if((Esp32Master)||(LcdModeSegment)) return;
		if(iPioDataBus == False)	SpiLCD.drawLine(iLcdPosX0, iLcdPosY0, iLcdPosX1, iLcdPosY1, iLcdRgbC0);
		else						SpiLCD.drawLine(iLcdPosX0, iLcdPosY0, iLcdPosX1, iLcdPosY1, iLcdRgbC1);
	}
}
//------------------------------------------------------------------------------//
static void VportLcdDrawTrgl(void) {
	if(PioInput) iPioDataBus = (Uint08)iCurrLcdMode;
	else {
		if((Esp32Master)||(LcdModeSegment)) return;
		if(iPioDataBus == False)	SpiLCD.drawTriangle(iLcdPosX0, iLcdPosY0, iLcdPosX1, iLcdPosY1, iLcdDisX2, iLcdDisY2, iLcdRgbC0);
		else						SpiLCD.drawTriangle(iLcdPosX0, iLcdPosY0, iLcdPosX1, iLcdPosY1, iLcdDisX2, iLcdDisY2, iLcdRgbC1);
	}
}
//------------------------------------------------------------------------------//
static void VportLcdFillTrgl(void) {
	if(PioInput) iPioDataBus = (Uint08)iCurrLcdMode;
	else {
		if((Esp32Master)||(LcdModeSegment)) return;
		if(iPioDataBus == False)	SpiLCD.fillTriangle(iLcdPosX0, iLcdPosY0, iLcdPosX1, iLcdPosY1, iLcdDisX2, iLcdDisY2, iLcdRgbC0);
		else						SpiLCD.fillTriangle(iLcdPosX0, iLcdPosY0, iLcdPosX1, iLcdPosY1, iLcdDisX2, iLcdDisY2, iLcdRgbC1);
	}
}
//------------------------------------------------------------------------------//
static void VportLcdDrawRect(void) {
	if(PioInput) iPioDataBus = (Uint08)iCurrLcdMode;
	else {
		if((Esp32Master)||(LcdModeSegment)) return;
		if(iPioDataBus == False)	SpiLCD.drawRect(iLcdPosX0, iLcdPosY0, iLcdDisX2, iLcdDisY2, iLcdRgbC0);
		else						SpiLCD.drawRect(iLcdPosX1, iLcdPosY1, iLcdDisX2, iLcdDisY2, iLcdRgbC1);
	}
}
//------------------------------------------------------------------------------//
static void VportLcdFillRect(void) {
	if(PioInput) iPioDataBus = (Uint08)iCurrLcdMode;
	else {
		if((Esp32Master)||(LcdModeSegment)) return;
		if(iPioDataBus == False)	SpiLCD.fillRect(iLcdPosX0, iLcdPosY0, iLcdDisX2, iLcdDisY2, iLcdRgbC0);
		else						SpiLCD.fillRect(iLcdPosX1, iLcdPosY1, iLcdDisX2, iLcdDisY2, iLcdRgbC1);
	}
}
//------------------------------------------------------------------------------//
static void VportLcdDrawCrcl(void) {
	if(PioInput) iPioDataBus = (Uint08)iCurrLcdMode;
	else {
		if((Esp32Master)||(LcdModeSegment)) return;
		if(iLcdRadX3 == iLcdRadY3) {
			if(iPioDataBus == False)	SpiLCD.drawCircle(iLcdPosX0, iLcdPosY0, iLcdRadX3, iLcdRgbC0);
			else						SpiLCD.drawCircle(iLcdPosX1, iLcdPosY1, iLcdRadX3, iLcdRgbC1);
		} else {
			if(iPioDataBus == False)	SpiLCD.drawEllipse(iLcdPosX0, iLcdPosY0, iLcdRadX3, iLcdRadY3, iLcdRgbC0);
			else						SpiLCD.drawEllipse(iLcdPosX1, iLcdPosY1, iLcdRadX3, iLcdRadY3, iLcdRgbC1);
		}
	}
}
//------------------------------------------------------------------------------//
static void VportLcdFillCrcl(void) {
	if(PioInput) iPioDataBus = (Uint08)iCurrLcdMode;
	else {
		if((Esp32Master)||(LcdModeSegment)) return;
		if(iLcdRadX3 == iLcdRadY3) {
			if(iPioDataBus == False)	SpiLCD.fillCircle(iLcdPosX0, iLcdPosY0, iLcdRadX3, iLcdRgbC0);
			else						SpiLCD.fillCircle(iLcdPosX1, iLcdPosY1, iLcdRadX3, iLcdRgbC1);
		} else {
			if(iPioDataBus == False)	SpiLCD.fillEllipse(iLcdPosX0, iLcdPosY0, iLcdRadX3, iLcdRadY3, iLcdRgbC0);
			else						SpiLCD.fillEllipse(iLcdPosX1, iLcdPosY1, iLcdRadX3, iLcdRadY3, iLcdRgbC1);
		}
	}
}
//==============================================================================//


//==============================================================================//
static void VportLcdPosX0L(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdPosX0 & 0x00FF);
	else			iLcdPosX0 = (iLcdPosX0 & 0xFF00) | (((Sint16)iPioDataBus) << 0);
}
//------------------------------------------------------------------------------//
static void VportLcdPosX0H(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdPosX0 >> 8);
	else			iLcdPosX0 = (iLcdPosX0 & 0x00FF) | (((Sint16)iPioDataBus) << 8);
}
//------------------------------------------------------------------------------//
static void VportLcdPosY0L(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdPosY0 & 0x00FF);
	else			iLcdPosY0 = (iLcdPosY0 & 0xFF00) | (((Sint16)iPioDataBus) << 0);
}
//------------------------------------------------------------------------------//
static void VportLcdPosY0H(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdPosY0 >> 8);
	else			iLcdPosY0 = (iLcdPosY0 & 0x00FF) | (((Sint16)iPioDataBus) << 8);
}
//------------------------------------------------------------------------------//
static void VportLcdPosX1L(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdPosX1 & 0x00FF);
	else			iLcdPosX1 = (iLcdPosX1 & 0xFF00) | (((Sint16)iPioDataBus) << 0);
}
//------------------------------------------------------------------------------//
static void VportLcdPosX1H(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdPosX1 >> 8);
	else			iLcdPosX1 = (iLcdPosX1 & 0x00FF) | (((Sint16)iPioDataBus) << 8);
}
//------------------------------------------------------------------------------//
static void VportLcdPosY1L(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdPosY1 & 0x00FF);
	else			iLcdPosY1 = (iLcdPosY1 & 0xFF00) | (((Sint16)iPioDataBus) << 0);
}
//------------------------------------------------------------------------------//
static void VportLcdPosY1H(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdPosY1 >> 8);
	else			iLcdPosY1 = (iLcdPosY1 & 0x00FF) | (((Sint16)iPioDataBus) << 8);
}
//------------------------------------------------------------------------------//
static void VportLcdDisX2L(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdDisX2 & 0x00FF);
	else			iLcdDisX2 = (iLcdDisX2 & 0xFF00) | (((Sint16)iPioDataBus) << 0);
}
//------------------------------------------------------------------------------//
static void VportLcdDisX2H(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdDisX2 >> 8);
	else			iLcdDisX2 = (iLcdDisX2 & 0x00FF) | (((Sint16)iPioDataBus) << 8);
}
//------------------------------------------------------------------------------//
static void VportLcdDisY2L(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdDisY2 & 0x00FF);
	else			iLcdDisY2 = (iLcdDisY2 & 0xFF00) | (((Sint16)iPioDataBus) << 0);
}
//------------------------------------------------------------------------------//
static void VportLcdDisY2H(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdDisY2 >> 8);
	else			iLcdDisY2 = (iLcdDisY2 & 0x00FF) | (((Sint16)iPioDataBus) << 8);
}
//------------------------------------------------------------------------------//
static void VportLcdRadX3L(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdRadX3 & 0x00FF);
	else			iLcdRadX3 = (iLcdRadX3 & 0xFF00) | (((Sint16)iPioDataBus) << 0);
}
//------------------------------------------------------------------------------//
static void VportLcdRadX3H(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdRadX3 >> 8);
	else			iLcdRadX3 = (iLcdRadX3 & 0x00FF) | (((Sint16)iPioDataBus) << 8);
}
//------------------------------------------------------------------------------//
static void VportLcdRadY3L(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdRadY3 & 0x00FF);
	else			iLcdRadY3 = (iLcdRadY3 & 0xFF00) | (((Sint16)iPioDataBus) << 0);
}
//------------------------------------------------------------------------------//
static void VportLcdRadY3H(void) {
	if(PioInput)	iPioDataBus = (Uint08)(iLcdRadY3 >> 8);
	else			iLcdRadY3 = (iLcdRadY3 & 0x00FF) | (((Sint16)iPioDataBus) << 8);
}
//==============================================================================//


//==============================================================================//
static void VportZ3dInit(void) {
	if(PioInput) iPioDataBus = 0x00;
	if(Esp32Master) iCurrZ3dProc = Z3dProcInit;
}
//------------------------------------------------------------------------------//
static void VportZ3dFlushScrn(void) {
	if(PioInput) iPioDataBus = 0x00;
	if(Esp32Master) iCurrZ3dProc = Z3dProcFlushScrn;
}
//------------------------------------------------------------------------------//
static void VportZ3dAmbiLight(void) {
	if(PioInput) iPioDataBus = (Uint08)(255.0 * fAmbiLight);
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcAmbiLight;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dDiffLight(void) {
	if(PioInput) iPioDataBus = (Uint08)(255.0 * fDiffLight);
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcDiffLight;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatPush(void) {
	if(PioInput)	iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatPush;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatPop(void) {
	if(PioInput)	iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatPop;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatUnit(void) {
	if(PioInput) iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatUnit;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatCopy(void) {
	if(PioInput) iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatCopy;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatRotX(void) {
	if(PioInput) iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatRotX;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatRotY(void) {
	if(PioInput) iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatRotY;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatRotZ(void) {
	if(PioInput) iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatRotZ;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatMulti(void) {
	if(PioInput) iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatMulti;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatTrans(void) {
	if(PioInput) iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatTrans;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatScale(void) {
	if(PioInput) iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatScale;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatDevice(void) {
	if(PioInput) iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatDevice;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMatPers(void) {
	if(PioInput) iPioDataBus = iCurrZ3dMatrix;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMatPers;	}
}
//==============================================================================//


//==============================================================================//
static void VportZ3dMdlClear(void) {
	if(PioInput) iPioDataBus = 0x00;
	if(Esp32Master) iCurrZ3dProc = Z3dProcMdlClear;
}
//------------------------------------------------------------------------------//
static void VportZ3dMdlBuild(void) {
	if(PioInput) iPioDataBus = asModelInfo[iCurrZ3dModel].ModelHead.Internal.iMaterialCount;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMdlBuild;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dMdlChange(void) {
	if(PioInput) iPioDataBus = asModelInfo[iCurrZ3dModel].ModelHead.Internal.iMaterialCount;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMdlChange;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMdlMate(void) {
	if(PioInput) iPioDataBus = asModelInfo[iCurrZ3dModel].ModelHead.Internal.iMaterialCount;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMdlMate;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMdlVertex(void) {
	if(PioInput) iPioDataBus = asModelInfo[iCurrZ3dModel].ModelHead.Internal.iVertexCount;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMdlVertex;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMdlPoly(void) {
	if(PioInput) iPioDataBus = asModelInfo[iCurrZ3dModel].ModelHead.Internal.iPolygonCount;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMdlPoly;	}
}
//------------------------------------------------------------------------------//
static void VportZ3dMdlEntry(void) {
	if(PioInput) iPioDataBus = asModelInfo[iCurrZ3dModel].ModelHead.Internal.iVertexCount;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMdlEntry;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dMdlShade(void) {
	if(PioInput) iPioDataBus = asModelInfo[iCurrZ3dModel].ModelHead.Internal.iPolygonCount;
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcMdlShade;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dVectorXL(void) {
	if(PioInput)	iPioDataBus = (Uint08)(aiCoordinate[X] & 0x00FF);
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcVectorXL;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dVectorXH(void) {
	if(PioInput)	iPioDataBus = (Uint08)(aiCoordinate[X] >> 8);
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcVectorXH;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dVectorYL(void) {
	if(PioInput)	iPioDataBus = (Uint08)(aiCoordinate[Y] & 0x00FF);
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcVectorYL;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dVectorYH(void) {
	if(PioInput)	iPioDataBus = (Uint08)(aiCoordinate[Y] >> 8);
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcVectorYH;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dVectorZL(void) {
	if(PioInput)	iPioDataBus = (Uint08)(aiCoordinate[Z] & 0x00FF);
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcVectorZL;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dVectorZH(void) {
	if(PioInput)	iPioDataBus = (Uint08)(aiCoordinate[Z] >> 8);
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcVectorZH;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dVectorWL(void) {
	if(PioInput)	iPioDataBus = (Uint08)(aiCoordinate[W] & 0x00FF);
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcVectorWL;		}
}
//------------------------------------------------------------------------------//
static void VportZ3dVectorWH(void) {
	if(PioInput)	iPioDataBus = (Uint08)(aiCoordinate[W] >> 8);
	else {	if(Esp32Master) iCurrZ3dProc = Z3dProcVectorWH;		}
}
//==============================================================================//


//==============================================================================//
static void VportBiosWBoot(void) {
	if(PioInput) iPioDataBus = 0x00;
	else {
		if(Esp32Master) iReleaseCpuBus = False;
		iCpmBiosParamL = iPioDataBus;	iCurrCpmMode = CpmModeCcprInit;
	}
}
//------------------------------------------------------------------------------//
static void VportBiosConSt(void) {
	if(PioInput) {
		if(SliceData(False) == NUL)		iPioDataBus = 0x00;
		else							iPioDataBus = 0xFF;
	}
}
//------------------------------------------------------------------------------//
static void VportBiosConIn(void) {
	if(PioInput) iPioDataBus = SliceData(True);
}
//------------------------------------------------------------------------------//
static void VportBiosConOut(void) {
	if(PioInput)	iPioDataBus = 0x00;
	else			TransData(iPioDataBus);
}
//------------------------------------------------------------------------------//
static void VportBiosList(void) {
	if(PioInput) iPioDataBus = 0x00;
}
//------------------------------------------------------------------------------//
static void VportBiosPunch(void) {
	if(PioInput) iPioDataBus = 0x00;
}
//------------------------------------------------------------------------------//
static void VportBiosReader(void) {
	if(PioInput) iPioDataBus = 0x1A;
}
//------------------------------------------------------------------------------//
static void VportBiosHome(void) {
	if(PioInput)	iPioDataBus = 0x00;
	else			iCpmDiskTrack = iCpmDiskSector = 0x0000;
}
//------------------------------------------------------------------------------//
static void VportBiosSelDsk(void) {
	if(PioInput) iPioDataBus = 0x00;
	else {
		if(iPioDataBus < CpmDiskDriveMax) {
			iCpmCurrDrive = iPioDataBus;
			iCpmBaseDrive = (Uint32)CpmDiskBlockMax * (Uint32)CpmDiskBlockSize * iCpmCurrDrive;
			iCpmBiosParamL = iCpmCurrDrive << 4;
		} else iCpmBiosParamL = iCpmBiosParamH = 0x00;
	}
}
//------------------------------------------------------------------------------//
static void VportBiosSetTrk(void) {
	if(PioInput)	iPioDataBus = 0x00;
	else			iCpmDiskTrack = (iCpmBiosParamH << 8) | iPioDataBus;
}
//------------------------------------------------------------------------------//
static void VportBiosSetSec(void) {
	if(PioInput)	iPioDataBus = 0x00;
	else			iCpmDiskSector = (iCpmBiosParamH << 8) | iPioDataBus;
}
//------------------------------------------------------------------------------//
static void VportBiosSetDma(void) {
	if(PioInput)	iPioDataBus = 0x00;
	else			iCpmDiskDmaAdrs = (iCpmBiosParamH << 8) | iPioDataBus;
}
//------------------------------------------------------------------------------//
static void VportBiosRead(void) {
	if(PioInput) iPioDataBus = 0x00;
	else {
		if(Esp32Master) iReleaseCpuBus = False;
		iCpmBiosParamL = 0x00;	iCurrCpmMode = CpmModeSecRead;
	}
}
//------------------------------------------------------------------------------//
static void VportBiosWrite(void) {
	if(PioInput) iPioDataBus = 0x00;
	else {
		if(Esp32Master) iReleaseCpuBus = False;
		iCpmBiosParamL = 0x00;	iCurrCpmMode = CpmModeSecWrite;
	}
}
//------------------------------------------------------------------------------//
static void VportBiosListSt(void) {
	if(PioInput) iPioDataBus = 0xFF;
}
//------------------------------------------------------------------------------//
static void VportBiosSecTran(void) {
	if(PioInput)	iPioDataBus = 0x00;
	else			iCpmBiosParamL = iPioDataBus;
}
//==============================================================================//


//==============================================================================//
static void VportInOutNop(void) {
	if(PioInput) iPioDataBus = 0x00;
}
//------------------------------------------------------------------------------//
static void (* apVportInOut[0x0100])(void) = {
	VportMakeDisk    ,	VportSysReset    ,	VportBiosParamL  ,	VportBiosParamH  ,
	VportHelpText    ,	VportClrScrn     ,	VportMsgDisp     ,	VportInOutNop    ,
	VportLcdStarLogo ,	VportLcdScrnShot ,	VportCtrlKey     ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,

	VportMelNote     ,	VportMelVolume   ,	VportBnkPage     ,	VportIntRqst     ,
	VportNmiRqst     ,	VportWitEnbl     ,	VportChpEnbl     ,	VportInOutNop    ,
	VportTimerCentis ,	VportTimerSecond ,	VportTimerMinute ,	VportTimerHour24 ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,

	VportAnyNum00    ,	VportAnyNum01    ,	VportAnyNum02    ,	VportAnyNum03    ,
	VportPioHist     ,	VportExeCycl     ,	VportClkMode     ,	VportInOutNop    ,
	VportClockReset  ,	VportClockSec    ,	VportClockMin    ,	VportClockHour   ,
	VportClockDay    ,	VportClockMon    ,	VportClockYear   ,	VportClockWeek   ,

	VportUartCtrl    ,	VportUartData    ,	VportBleUart     ,	VportSdcBusy     ,
	VportLcdMode     ,	VportLcdLedBright,	VportBasicFile   ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,

	VportLcdRotation ,	VportLcdClrScrn  ,	VportLcdTextSize ,	VportLcdDrawChar ,
	VportLcdRgbC0L   ,	VportLcdRgbC0H   ,	VportLcdRgbC1L   ,	VportLcdRgbC1H   ,
	VportLcdDrawPixel,	VportLcdDrawLine ,	VportLcdDrawTrgl ,	VportLcdFillTrgl ,
	VportLcdDrawRect ,	VportLcdFillRect ,	VportLcdDrawCrcl ,	VportLcdFillCrcl ,

	VportLcdPosX0L   ,	VportLcdPosX0H   ,	VportLcdPosY0L   ,	VportLcdPosY0H   ,
	VportLcdPosX1L   ,	VportLcdPosX1H   ,	VportLcdPosY1L   ,	VportLcdPosY1H   ,
	VportLcdDisX2L   ,	VportLcdDisX2H   ,	VportLcdDisY2L   ,	VportLcdDisY2H   ,
	VportLcdRadX3L   ,	VportLcdRadX3H   ,	VportLcdRadY3L   ,	VportLcdRadY3H   ,

	VportZ3dInit     ,	VportZ3dFlushScrn,	VportZ3dAmbiLight,	VportZ3dDiffLight,
	VportZ3dMatPush  ,	VportZ3dMatPop   ,	VportZ3dMatUnit  ,	VportZ3dMatCopy  ,
	VportZ3dMatRotX  ,	VportZ3dMatRotY  ,	VportZ3dMatRotZ  ,	VportZ3dMatMulti ,
	VportZ3dMatTrans ,	VportZ3dMatScale ,	VportZ3dMatDevice,	VportZ3dMatPers  ,

	VportZ3dMdlClear ,	VportZ3dMdlBuild ,	VportZ3dMdlChange,	VportZ3dMdlMate  ,
	VportZ3dMdlVertex,	VportZ3dMdlPoly  ,	VportZ3dMdlEntry ,	VportZ3dMdlShade ,
	VportZ3dVectorXL ,	VportZ3dVectorXH ,	VportZ3dVectorYL ,	VportZ3dVectorYH ,
	VportZ3dVectorZL ,	VportZ3dVectorZH ,	VportZ3dVectorWL ,	VportZ3dVectorWH ,

	VportBiosWBoot   ,	VportBiosConSt   ,	VportBiosConIn   ,	VportBiosConOut  ,
	VportBiosList    ,	VportBiosPunch   ,	VportBiosReader  ,	VportBiosHome    ,
	VportBiosSelDsk  ,	VportBiosSetTrk  ,	VportBiosSetSec  ,	VportBiosSetDma  ,
	VportBiosRead    ,	VportBiosWrite   ,	VportBiosListSt  ,	VportBiosSecTran ,

	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,

	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,

	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,

	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,

	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,

	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,

	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,	VportInOutNop    ,
};
//==============================================================================//


//==============================================================================//
static Sint08 VportPioStandBy(void) {
	if(PioReqRead() != False) return(False);
	iPioPortBus = PortBusRead();

	if(PioFetRead() != False) {
		if(PioReaRead() == False)	iCurrPioMode = PioModeInput ;
		else						iCurrPioMode = PioModeOutput;

		if(PioOutput) iPioDataBus = DataBusRead();
		(* apVportInOut[iPioPortBus])();
		if(PioInput) {	DataBusOutput();	DataBusWrite(iPioDataBus);	}

		if(ExeCyclRead() != False) {	iCpuPortBus = iPioPortBus;	iCpuDataBus = iPioDataBus;	}
	} else iCurrPioMode = PioModeInterrupt;

	BusReqLow();	CpuWitHigh();
	return(True);
}
//------------------------------------------------------------------------------//
static Sint08 VportPioInput(void) {
	if(PioReqRead() == False) return(False);
	DataBusInput();

	iCurrPioMode = PioModeContinue;
	return(True);
}
//------------------------------------------------------------------------------//
static Sint08 VportPioOutput(void) {
	if(PioReqRead() == False) return(False);

	iCurrPioMode = PioModeMulti;
	return(True);
}
//------------------------------------------------------------------------------//
static Sint08 VportPioInterrupt(void) {
	if(PioReqRead() == False) return(False);

	iCurrPioMode = PioModeContinue;
	return(True);
}
//------------------------------------------------------------------------------//
static Sint08 VportPioContinue(void) {
	if(PioBusRead() != False) return(False);
	CpuWitLow();	BusReqHigh();

	iCurrPioMode = PioModeStandBy;
	return(False);
}
//------------------------------------------------------------------------------//
static Sint08 VportPioMulti(void) {
	if(PioBusRead() != False) return(False);
	MultiCpuOutput();	CpuWitLow();

	if(iReleaseCpuBus == False)		iReleaseCpuBus = True;
	else							BusReqHigh();

	iCurrPioMode = PioModeStandBy;
	return(False);
}
//------------------------------------------------------------------------------//
static Sint08 VportPioSlave(void) {
	if(LcdUpdtRead() != False) {
		if(ExeCyclRead() != False) {
			iCpuAdrsBus = AdrsBusRead();
			iCpuCtrlBit = CtrlBitRead() ^ 0xFF;
			iSysCtrlBit = (iSysCtrlBit & 0xCF) | ((iCpuCtrlBit & 0xC0) >> 2);
		} else {
			iCpuAdrsBus = 0xFF;
			iCpuCtrlBit = CpuCtrlRFSH | CpuCtrlM1 | CpuCtrlIORQ | CpuCtrlMREQ | CpuCtrlWR | CpuCtrlRD;
			iSysCtrlBit = (iSysCtrlBit & 0xCF) | (CpuCtrlBSAK >> 2);
		}
	}

	iCurrPioMode = PioModeOutput;
	(* apVportInOut[iPioPortBus])();
	iPioPortBus = PortStandBySlave;

	iCurrPioMode = PioModeSlave;
	return(False);
}
//------------------------------------------------------------------------------//
static Sint08 (* apVportPioMode[PioModeMax])(void) = {
	VportPioStandBy  ,	VportPioInput    ,	VportPioOutput   ,	VportPioInterrupt,
	VportPioContinue ,	VportPioMulti    ,	VportPioSlave    ,
};
//==============================================================================//


//==============================================================================//
static Sint08 IsPioStandBy(void) {
	return(iCurrPioMode == PioModeStandBy);
}
//------------------------------------------------------------------------------//
static void VportShift(void) {
	if(LcdUpdtRead() != False) {
		if(ExeCyclRead() != False) {
			iCpuPortBus = PortBusRead();
			iCpuDataBus = DataBusRead();
		} else iCpuPortBus = iCpuDataBus = 0xFF;
	}

	if((ExeCyclRead() != False)&&(PioHistRead() != 0)) {
		iCpuCtrlBit = CtrlBitRead() ^ 0xFF;
	}
}
//------------------------------------------------------------------------------//
static void VportHistory(void) {
	if((ExeCyclRead() != False)&&(PioHistRead() != 0)) {
		if(		((iCpuCtrlBit & (CpuCtrlRFSH | CpuCtrlM1)) == 0)&&
				((iCpuCtrlBit & (CpuCtrlWR   | CpuCtrlRD)) != 0)	) {
			if(		((PioHistRead() & 0x01)&&((iCpuCtrlBit & CpuCtrlMREQ) != 0))||
					((PioHistRead() & 0x02)&&((iCpuCtrlBit & CpuCtrlIORQ) != 0))	) {
				if(		(PioHistRead() & 0x01)||
						((iCpuPortBus != PortUartCtrl)&&(iCpuPortBus != PortBiosConSt))		) {
					MultiData(CodeTelPioHist);
					MultiData(iPioHistD01 = iCpuPortBus);	MultiData(iPioHistD02 = iCpuDataBus);
					MultiData(iPioHistD03 = (iPioHistD03 & 0x30) | (iCpuCtrlBit & 0x0F));
				}
			}
		}
	}
}
//==============================================================================//


//==============================================================================//
static void VportInit(void) {
	if(Esp32Master)		iCurrPioMode = PioModeStandBy;
	else				iCurrPioMode = PioModeSlave;

	iPioPortBus = PortStandBySlave;
	iPioDataBus = 0x00;

	iReleaseCpuBus = True;	iRomImageAdrs = 0x0000;
	iPioHistD01 = iPioHistD02 = iPioHistD03 = 0x00;
}
//------------------------------------------------------------------------------//
static void VportMove(void) {
	if(iResetRequest != ResetModeStandBy) return;

	if(Esp32Master) VportShift();
	while((* apVportPioMode[iCurrPioMode])());
	if(Esp32Master) VportHistory();
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

