
//==============================================================================//
//	Z80 Single Board Computer "PC-84C0SD 20MHz" I/O Sketch for ESP32-S3-MINI-1	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
enum {
	CodeTelSynchWait,									//	電文コード（システム再起動）
	CodeTelAutoExec ,									//	電文コード（自動実行有無）
	CodeTelDmaInit  ,									//	電文コード（DMA転送開始）
	CodeTelDmaRecord,									//	電文コード（DMA転送記録）
	CodeTelDmaMove  ,									//	電文コード（DMA転送動作）
	CodeTelDmaExit  ,									//	電文コード（DMA転送終了）
	CodeTelSerCtrl  ,									//	電文コード（通信端末制御）
	CodeTelSerData  ,									//	電文コード（通信端末データ）
	CodeTelBleUart  ,									//	電文コード（BLE通信状態）
	CodeTelSdcBusy  ,									//	電文コード（SDカード状態）
	CodeTelZ3dFlush ,									//	電文コード（Z3D画面更新）
	CodeTelZ3dPoly  ,									//	電文コード（Z3D多角描画）
	CodeTelPioOutput,									//	電文コード（CPU出力命令）
	CodeTelBnkPage  ,									//	電文コード（RAM下位バンク）
	CodeTelPioHist  ,									//	電文コード（CPU入出力履歴）
	CodeTelLcdUpdt  ,									//	電文コード（LCD表示更新）

	CodeTelMax,											//	電文コード上限
};
//------------------------------------------------------------------------------//
#define		Esp32Master		(iMasterSlave == False)		//	ESP32識別（主機）
#define		Esp32Slave		(iMasterSlave != False)		//	ESP32識別（補機）
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static Sint08 iMasterSlave;								//	主機・補機フラグ
static Sint08 iSynchWait;								//	同期待機フラグ
//------------------------------------------------------------------------------//
static Uint08 iSerialCountMx;							//	シリアル回数（転送）
static Uint08 iSerialCountDx;							//	シリアル回数（電文）
//------------------------------------------------------------------------------//
static Uint08 aiSerialBufMx[SerialSegSizeTx];			//	シリアル緩衝（転送）
static Uint08 aiSerialBufDx[SerialSegSizeTx];			//	シリアル緩衝（電文）
//==============================================================================//


//==============================================================================//
static void MultiFlush(void) {
	Serial .flush();
	Serial1.flush();

	while(Serial .available() > 0) Serial .read();
	while(Serial1.available() > 0) Serial1.read();
}
//------------------------------------------------------------------------------//
static void MultiData(Uint08 iData) {
	Serial1.write(iData);
}
//------------------------------------------------------------------------------//
static Uint08 MultiRecep(void) {
	while(Serial1.available() == 0);
	return(Serial1.read());
}
//------------------------------------------------------------------------------//
static void MultiSector(void) {
	if(iSerialCountMx == 0) return;
	MultiData(CodeTelDmaRecord);	MultiData(iSerialCountMx);

	Serial1.write(aiSerialBufMx, iSerialCountMx);
	iSerialCountMx = 0;
}
//------------------------------------------------------------------------------//
static void MultiKeep(Uint08 iData) {
	aiSerialBufDx[iSerialCountDx++] = iData;
}
//------------------------------------------------------------------------------//
static void MultiSend(void) {
	if(iSerialCountDx == 0) return;
	MultiData(CodeTelSerData);	MultiData(iSerialCountDx);

	Serial1.write(aiSerialBufDx, iSerialCountDx);
	iSerialCountDx = 0;
}
//------------------------------------------------------------------------------//
static void MultiCpuOutput(void) {
	MultiData(CodeTelPioOutput);
	MultiData(iPioPortBus);		MultiData(iPioDataBus);
}
//==============================================================================//


//==============================================================================//
static void MultiSynchWait(void) {
	iSynchWait = False;
}
//------------------------------------------------------------------------------//
static void MultiAutoExec(void) {
	iCpmBiosParamL = MultiRecep();
}
//------------------------------------------------------------------------------//
static void MultiDmaInit(void) {
	AdrsBusOutput();	AdrsBusWrite(0x00);
	MultiData(CodeTelDmaInit);
}
//------------------------------------------------------------------------------//
static void MultiDmaRecord(void) {
	iSerialCountMx = MultiRecep();
	while(Serial1.available() < iSerialCountMx);

	Serial1.read(aiSerialBufMx, iSerialCountMx);
}
//------------------------------------------------------------------------------//
static void MultiDmaMove(void) {
	AdrsBusWrite(iCpuAdrsBus = MultiRecep());
	MultiData(CodeTelDmaMove);
}
//------------------------------------------------------------------------------//
static void MultiDmaExit(void) {
	AdrsBusInput();
	MultiData(CodeTelDmaExit);
}
//------------------------------------------------------------------------------//
static void MultiSerCtrl(void) {
	TransControl(MultiRecep());
}
//------------------------------------------------------------------------------//
static void MultiSerData(void) {
	Sint16 i;

	iSerialCountDx = MultiRecep();
	while(Serial1.available() < iSerialCountDx);

	for(i = 0;i < iSerialCountDx;i++) {
		RecepData(Serial1.read());
	}

	iSerialCountDx = 0;
}
//------------------------------------------------------------------------------//
static void MultiBleUart(void) {
	if(BleUartRead() != False)	{	BleUartLow();	}
	else						{	BleUartHigh();	}
}
//------------------------------------------------------------------------------//
static void MultiSdcBusy(void) {
	iSynchWait = False;									//	BASICファイル読込Master側Wait解除（変更禁止）
	SdcBusyLow();
}
//------------------------------------------------------------------------------//
static void MultiZ3dFlush(void) {
	MultiData(CodeTelZ3dFlush);
	MatrixFlushScreen();
}
//------------------------------------------------------------------------------//
static void MultiZ3dPoly(void) {
	MatrixTriangle();
}
//------------------------------------------------------------------------------//
static void MultiPioOutput(void) {
	iPioPortBus = MultiRecep();
	iPioDataBus = MultiRecep();
}
//------------------------------------------------------------------------------//
static void MultiBnkPage(void) {
	BusReqHigh();
}
//------------------------------------------------------------------------------//
static void MultiPioHist(void) {
	iPioHistD01 = MultiRecep();
	iPioHistD02 = MultiRecep();
	iPioHistD03 = MultiRecep();
}
//------------------------------------------------------------------------------//
static void MultiLcdUpdt(void) {
	iCpuPortBus = MultiRecep();
	iCpuDataBus = MultiRecep();
	LcdUpdtHigh();
}
//------------------------------------------------------------------------------//
static void (* apMultiTel[CodeTelMax])(void) = {
	MultiSynchWait,		MultiAutoExec ,
	MultiDmaInit  ,		MultiDmaRecord,		MultiDmaMove  ,		MultiDmaExit  ,
	MultiSerCtrl  ,		MultiSerData  ,		MultiBleUart  ,		MultiSdcBusy  ,
	MultiZ3dFlush ,		MultiZ3dPoly  ,
	MultiPioOutput,		MultiBnkPage  ,		MultiPioHist  ,		MultiLcdUpdt  ,
};
//------------------------------------------------------------------------------//
static void MultiWait(Uint08 iCode) {
	Uint08 iData;

	while((iData = Serial1.read()) != iCode) {
		if(iData < CodeTelMax) (* apMultiTel[iData])();
		if(iCode == 0xFF) break;
	}
}
//==============================================================================//


//==============================================================================//
static void MultiInit(void) {
	Serial1.end();
#ifdef		BuildMaster
	Serial1.setRxBufferSize(SerialBufSizeRx);
	Serial1.setTxBufferSize(SerialBufSizeDx);
#endif
#ifdef		BuildSlave
	Serial1.setRxBufferSize(SerialBufSizeDx);
	Serial1.setTxBufferSize(SerialBufSizeTx);
#endif
	Serial1.begin(SerialBaudRateDx, SERIAL_8N1, GpioUa1Rxd, GpioUa1Txd);

	NeoPixInput();	iMasterSlave = NeoPixRead();	iSynchWait = False;
	iSerialCountMx = iSerialCountDx = 0;
}
//------------------------------------------------------------------------------//
static void MultiMove(void) {
	if(iResetRequest == ResetModeSynchWait) {
		MultiData(CodeTelSynchWait);
		MultiWait(CodeTelSynchWait);

		MultiFlush();	delay(100);
		iResetRequest = ResetModeBootExec;
	}

	MultiWait(0xFF);
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

