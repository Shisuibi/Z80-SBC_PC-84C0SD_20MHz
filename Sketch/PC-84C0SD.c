
//==============================================================================//
//	Z80 Single Board Computer "PC-84C0SD 20MHz" I/O Sketch for ESP32-S3-MINI-1	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
//==============================================================================//


//==============================================================================//
#include	"Global.h"									//	定義ファイル（広域）
//==============================================================================//


//==============================================================================//
void setup(void) {
	MultiInit();
	ClockInit();
	TransInit();
	CpmEmuInit(False);

	MatrixInit(False);
	VportInit();
	ResetInit();
	SpiLcdInit(False);
}
//------------------------------------------------------------------------------//
void loop(void) {
	MultiMove();
	ClockMove();
	TransMove();
	CpmEmuMove();

	MatrixMove();
	VportMove();
	ResetMove();
	SpiLcdMove();
}
//==============================================================================//

