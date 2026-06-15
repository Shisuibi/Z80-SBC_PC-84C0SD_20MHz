
//==============================================================================//
//	Z80 Single Board Computer "PC-84C0SD 20MHz" I/O Sketch for ESP32-S3-MINI-1	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
enum {
	ClockMode000iHz,									//	手動φ／停止
	ClockMode004iHz,									//	発振φ（  4[ Hz]）
	ClockMode032iHz,									//	発振φ（ 32[ Hz]）
	ClockMode256iHz,									//	発振φ（256[ Hz]）
	ClockMode002KHz,									//	発振φ（  2[KHz]）
	ClockMode016KHz,									//	発振φ（ 16[KHz]）
	ClockMode128KHz,									//	発振φ（128[KHz]）
	ClockMode001MHz,									//	発振φ（  1[MHz]）
	ClockMode2p5MHz,									//	発振φ（2.5[MHz]）
	ClockMode004MHz,									//	発振φ（  4[MHz]）
	ClockMode006MHz,									//	発振φ（  6[MHz]）
	ClockMode008MHz,									//	発振φ（  8[MHz]）
	ClockMode010MHz,									//	発振φ（ 10[MHz]）
	ClockMode020MHz,									//	発振φ（ 20[MHz]）

	ClockModeMax,										//	発振φモード上限
};
//------------------------------------------------------------------------------//
#define		MelodyOctaveMax				9				//	旋律♪オクターブ上限
#define		MelodyScaleMax				12				//	旋律♪音階上限

#define		ClockChannel				4				//	発振φチャンネル
#define		MelodyChannel				5				//	旋律♪チャンネル
//------------------------------------------------------------------------------//
typedef struct tClockInfo {
	Uint08				iDisp;							//	発振φモード表示
	Cint08*				pMessage;						//	発振φメッセージ

	Uint08				iReso;							//	PWM解像度
	Uint32				iDuty;							//	PWMデューティ比
	Uint32				iFreq;							//	PWM周波数
} ClockInfo;											//	クロック情報
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static ClockInfo asClockInfo[ClockModeMax] = {					//	クロック情報
	{	0x00, "  0[_n_]", 0x00, 0x00000000, 0x00000000,		},	//	手動φ／停止
	{	0x10, "  4[ Hz]", 0x0E, 0x00002000, 0x00000004,		},	//	発振φ（  4[ Hz]）
	{	0x30, " 32[ Hz]", 0x0E, 0x00002000, 0x00000020,		},	//	発振φ（ 32[ Hz]）
	{	0x70, "256[ Hz]", 0x0E, 0x00002000, 0x00000100,		},	//	発振φ（256[ Hz]）
	{	0x18, "  2[KHz]", 0x0E, 0x00002000, 0x00000800,		},	//	発振φ（  2[KHz]）
	{	0x38, " 16[KHz]", 0x0B, 0x00000400, 0x00004000,		},	//	発振φ（ 16[KHz]）
	{	0x78, "128[KHz]", 0x08, 0x00000080, 0x00020000,		},	//	発振φ（128[KHz]）
	{	0x1C, "  1[MHz]", 0x05, 0x00000010, 0x00100000,		},	//	発振φ（  1[MHz]）
	{	0x2C, "2.5[MHz]", 0x03, 0x00000004, 0x00280000,		},	//	発振φ（2.5[MHz]）
	{	0x4C, "  4[MHz]", 0x03, 0x00000004, 0x00400000,		},	//	発振φ（  4[MHz]）
	{	0x6C, "  6[MHz]", 0x02, 0x00000002, 0x00600000,		},	//	発振φ（  6[MHz]）
	{	0x3C, "  8[MHz]", 0x02, 0x00000002, 0x00800000,		},	//	発振φ（  8[MHz]）
	{	0x5C, " 10[MHz]", 0x01, 0x00000001, 0x00A00000,		},	//	発振φ（ 10[MHz]）
	{	0x7C, " 20[MHz]", 0x01, 0x00000001, 0x01312D00,		},	//	発振φ（ 20[MHz]）
};
//------------------------------------------------------------------------------//
static Sint08 iCurrClkMode;								//	現行発振φモード
//==============================================================================//


//==============================================================================//
static Uint16 aiMelodyTone[MelodyOctaveMax][MelodyScaleMax] = {		//	旋律♪音階
//		C		C#		D		D#		E		F		F#		G		G#		A		A#		B			Octave
	{	   0,	   0,	   0,	   0,	   0,	   0,	   0,	   0,	   0,	  28,	  29,	  31,	},	//	0
	{	  33,	  35,	  37,	  39,	  41,	  44,	  46,	  49,	  52,	  55,	  58,	  62,	},	//	1
	{	  65,	  69,	  73,	  78,	  82,	  87,	  92,	  98,	 104,	 110,	 117,	 123,	},	//	2
	{	 131,	 139,	 147,	 156,	 165,	 175,	 185,	 196,	 208,	 220,	 233,	 247,	},	//	3
	{	 262,	 277,	 294,	 311,	 330,	 349,	 370,	 392,	 415,	 440,	 466,	 494,	},	//	4
	{	 523,	 554,	 587,	 622,	 659,	 698,	 740,	 784,	 831,	 880,	 932,	 988,	},	//	5
	{	1047,	1109,	1175,	1245,	1319,	1397,	1480,	1568,	1661,	1760,	1865,	1976,	},	//	6
	{	2093,	2217,	2349,	2489,	2637,	2794,	2960,	3136,	3322,	3520,	3729,	3951,	},	//	7
	{	4186,	   0,	   0,	   0,	   0,	   0,	   0,	   0,	   0,	   0,	   0,	   0,	},	//	8
};
//------------------------------------------------------------------------------//
static Uint08 iCurrMelNote;								//	現行旋律♪音符
static Uint08 iCurrMelVolume;							//	現行旋律♪音量
//------------------------------------------------------------------------------//
static Uint32 iTimerMillis;								//	システム時刻ミリ秒

static Uint08 iTimerCentis;								//	システム時刻（厘）
static Uint08 iTimerSecond;								//	システム時刻（秒）

static Uint08 iTimerMinute;								//	システム時刻（分）
static Uint08 iTimerHour24;								//	システム時刻（時）
//==============================================================================//


//==============================================================================//
static void ClockManual(Uint32 iDelay) {
	CpuClkHigh();	delayMicroseconds(iDelay);
	CpuClkLow();	delayMicroseconds(iDelay);
}
//------------------------------------------------------------------------------//
static void ClockStart(Sint08 iClkMode) {
	ledcAttachChannel(GpioCpuClk, asClockInfo[iClkMode].iFreq, asClockInfo[iClkMode].iReso, ClockChannel);
	ledcWrite(GpioCpuClk, asClockInfo[iClkMode].iDuty);
}
//------------------------------------------------------------------------------//
static void ClockStop(void) {
	ledcWrite(GpioCpuClk, asClockInfo[ClockMode000iHz].iDuty);
	ledcDetach(GpioCpuClk);
}
//------------------------------------------------------------------------------//
static void ClockChange(Sint08 iClkMode) {
	if(Esp32Master) {
		if(iCurrClkMode != ClockMode000iHz) ClockStop();

		if(iClkMode == ClockMode000iHz)		{	CpuClkOutput();		CpuClkLow();	}
		else								ClockStart(iClkMode);
	}

	ClkModeWrite(iCurrClkMode = iClkMode);
}
//==============================================================================//


//==============================================================================//
static void ClockControl(Sint08 iClkMode) {
	if((iClkMode == ClockMode000iHz)||(iClkMode != iCurrClkMode)) {
		TransMessage(asClockInfo[iClkMode].pMessage);

		if((iClkMode == ClockMode000iHz)&&(iCurrClkMode == ClockMode000iHz)) {
			if(Esp32Master) ClockManual(100000);
		} else ClockChange(iClkMode);
	}
}
//------------------------------------------------------------------------------//
static void ClockCtrl000iHz(void) {
	ClockControl(ClockMode000iHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl004iHz(void) {
	ClockControl(ClockMode004iHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl032iHz(void) {
	ClockControl(ClockMode032iHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl256iHz(void) {
	ClockControl(ClockMode256iHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl002KHz(void) {
	ClockControl(ClockMode002KHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl016KHz(void) {
	ClockControl(ClockMode016KHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl128KHz(void) {
	ClockControl(ClockMode128KHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl001MHz(void) {
	ClockControl(ClockMode001MHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl2p5MHz(void) {
	ClockControl(ClockMode2p5MHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl004MHz(void) {
	ClockControl(ClockMode004MHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl006MHz(void) {
	ClockControl(ClockMode006MHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl008MHz(void) {
	ClockControl(ClockMode008MHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl010MHz(void) {
	ClockControl(ClockMode010MHz);
}
//------------------------------------------------------------------------------//
static void ClockCtrl020MHz(void) {
	ClockControl(ClockMode020MHz);
}
//==============================================================================//


//==============================================================================//
static void ClockMelody(Uint08 iNote, Uint08 iVolume) {
	Uint08 iPrevMelNote = iCurrMelNote;
	Uint08 iOctave, iScale;
	Uint16 iCurrTone, iPrevTone;

	if((iOctave = (iNote & 0xF0) >> 4) >= MelodyOctaveMax) iOctave = 0;
	if((iScale = iNote & 0x0F) >= MelodyScaleMax) iScale = 0;

	iCurrMelNote = (iOctave << 4) | iScale;
	iCurrMelVolume = iVolume;

	if(Esp32Slave) {
		iCurrTone = aiMelodyTone[(iCurrMelNote & 0xF0) >> 4][iCurrMelNote & 0x0F];
		iPrevTone = aiMelodyTone[(iPrevMelNote & 0xF0) >> 4][iPrevMelNote & 0x0F];

		if((iPrevTone > 0)&&(iCurrTone != iPrevTone)) {
			ledcWrite(GpioBuzTon, 0x0000);
			ledcDetach(GpioBuzTon);
		}

		if(iCurrTone > 0) {
			if(iCurrTone != iPrevTone) ledcAttachChannel(GpioBuzTon, iCurrTone, 0x0B, MelodyChannel);
			ledcWrite(GpioBuzTon, iCurrMelVolume);
		}
	}
}
//------------------------------------------------------------------------------//
static void ClockTimer(void) {
	Uint32 iMillis = millis() - iTimerMillis;

	while(iMillis > 999) {
		iMillis -= 1000;	iTimerMillis += 1000;

		if(++iTimerSecond > 59) {
			iTimerSecond = 0;

			if(++iTimerMinute > 59) {
				iTimerMinute = 0;

				if(++iTimerHour24 > 23) iTimerHour24 = 0;
			}
		}
	}

	iTimerCentis = (Uint08)(iMillis / 10);
}
//==============================================================================//


//==============================================================================//
static void ClockInit(void) {
	iCurrClkMode = ClockMode000iHz;

	iCurrMelNote = 0x00;
	iCurrMelVolume = 0x80;

	iTimerMillis = millis();

	iTimerCentis = iTimerSecond = 0;
	iTimerMinute = iTimerHour24 = 0;
}
//------------------------------------------------------------------------------//
static void ClockMove(void) {
	ClockTimer();
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

