
//==============================================================================//
//	Z80 Single Board Computer "PC-84C0SD 20MHz" I/O Sketch for ESP32-S3-MINI-1	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
enum {
	Z3dModeStandBy  ,									//	Z3Dモード（待機）

	Z3dModeInit     ,									//	Z3Dモード（初期化）
	Z3dModeFlushScrn,									//	Z3Dモード（画面更新）
	Z3dModeAmbiLight,									//	Z3Dモード（環境光源係数）
	Z3dModeDiffLight,									//	Z3Dモード（拡散反射係数）
	Z3dModeMatPush  ,									//	Z3Dモード（行列積重）
	Z3dModeMatPop   ,									//	Z3Dモード（行列取除）
	Z3dModeMatUnit  ,									//	Z3Dモード（単位行列）
	Z3dModeMatCopy  ,									//	Z3Dモード（行列複写）

	Z3dModeMatRotX  ,									//	Z3Dモード（回転X軸）
	Z3dModeMatRotY  ,									//	Z3Dモード（回転Y軸）
	Z3dModeMatRotZ  ,									//	Z3Dモード（回転Z軸）
	Z3dModeMatMulti ,									//	Z3Dモード（行列乗算）
	Z3dModeMatTrans ,									//	Z3Dモード（平行移動）
	Z3dModeMatScale ,									//	Z3Dモード（拡大縮小）
	Z3dModeMatDevice,									//	Z3Dモード（表示機器行列）
	Z3dModeMatPers  ,									//	Z3Dモード（透視変換行列）

	Z3dModeMdlClear ,									//	Z3Dモード（模型消去）
	Z3dModeMdlBuild ,									//	Z3Dモード（模型追加・変更）
	Z3dModeMdlVertex,									//	Z3Dモード（頂点追加・変更）
	Z3dModeMdlPoly  ,									//	Z3Dモード（多角追加・変更）
	Z3dModeMdlEntry ,									//	Z3Dモード（模型登録）
	Z3dModeMdlShade ,									//	Z3Dモード（模型陰影）

	Z3dModeVectorXL ,									//	Z3Dモード（座標X下位）
	Z3dModeVectorXH ,									//	Z3Dモード（座標X上位）
	Z3dModeVectorYL ,									//	Z3Dモード（座標Y下位）
	Z3dModeVectorYH ,									//	Z3Dモード（座標Y上位）
	Z3dModeVectorZL ,									//	Z3Dモード（座標Z下位）
	Z3dModeVectorZH ,									//	Z3Dモード（座標Z上位）
	Z3dModeVectorWL ,									//	Z3Dモード（座標W下位）
	Z3dModeVectorWH ,									//	Z3Dモード（座標W上位）

	Z3dModeMax,											//	Z3Dモード上限
};
//------------------------------------------------------------------------------//
#define		MatrixStackMax				0x10			//	行列スタック上限
//------------------------------------------------------------------------------//
#define		FixedDecimalSize			8				//	小数ビット長

#define		FloatExponentBias			127				//	指数バイアス
#define		FloatFractionSize			23				//	仮数ビット長
//------------------------------------------------------------------------------//
typedef union tCoordinate {
	struct tInternal {
		Uint32				iFraction : 23;				//	内部ビット（仮数）
		Uint32				iExponent : 8;				//	内部ビット（指数）
		Uint32				iSign : 1;					//	内部ビット（符号）
	} Internal;											//	内部形式

	Sflt32				fFloat;							//	浮動小数点数
} Coordinate;											//	演算座標
//------------------------------------------------------------------------------//
typedef struct tTriInfo {
	Sint16				aiPos[3][XY];					//	表示座標
	Uint32				iColor;							//	表示色彩
} TriInfo;												//	三角情報
//------------------------------------------------------------------------------//
static void MatrixFlushScreen(void);
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static Sint08 iCurrZ3dMode;								//	現行Z3Dモード
static Sint08 iCurrMatrix;								//	現行行列

static Sfix88 aiCoordinate[XYZW];						//	演算座標（固定小数点数）
static Sflt32 afCoordinate[XYZW];						//	演算座標（浮動小数点数）
//------------------------------------------------------------------------------//
static Sflt32 fAmbiLight;								//	環境光源係数
static Sflt32 fDiffLight;								//	拡散反射係数

static Sflt32 afParaLight[XYZW];						//	平行光源ベクトル
static Sflt32 afMatrixStack[MatrixStackMax][XYZW][XYZW];	//	行列スタック
//==============================================================================//


//==============================================================================//
static Sflt32 FixToFlt(Sfix88 iFix) {
	Coordinate uCoordinate;
	Sint08 iDigit, iShift;

	if(uCoordinate.Internal.iSign = iFix < 0) iFix = -iFix;
	iDigit = 31 - __builtin_clz(iFix);

	if(iFix == 0)	uCoordinate.Internal.iExponent = 0;
	else			uCoordinate.Internal.iExponent = iDigit - FixedDecimalSize + FloatExponentBias;

	iShift = iDigit - FloatFractionSize;

	if(iShift < 0)	uCoordinate.Internal.iFraction = iFix << -iShift;
	else			uCoordinate.Internal.iFraction = iFix >>  iShift;

	return(uCoordinate.fFloat);
}
//==============================================================================//


//==============================================================================//
static void MatrixVecInner(Sflt32* pInr, Sflt32* pVA, Sflt32* pVB) {
	*pInr = pVA[X] * pVB[X] + pVA[Y] * pVB[Y] + pVA[Z] * pVB[Z];
}
//------------------------------------------------------------------------------//
static void MatrixVecCross(Sflt32* pCrs, Sflt32* pVA, Sflt32* pVB) {
	pCrs[X] = pVA[Y] * pVB[Z] - pVA[Z] * pVB[Y];
	pCrs[Y] = pVA[Z] * pVB[X] - pVA[X] * pVB[Z];
	pCrs[Z] = pVA[X] * pVB[Y] - pVA[Y] * pVB[X];
	pCrs[W] = 1.0;
}
//------------------------------------------------------------------------------//
static void MatrixVecUnit(Sflt32* pVec) {
	Sint08 i;
	Sflt32 fInr;

	MatrixVecInner(&fInr, pVec, pVec);

	fInr = 1.0 / sqrt(fInr);	pVec[W] = 1.0;
	for(i = 0;i < XYZ;i++) pVec[i] *= fInr;
}
//------------------------------------------------------------------------------//
static void MatrixCrsVertex(Sflt32* pCrs, Sflt32* pV0, Sflt32* pV1, Sflt32* pV2) {
	Sflt32 afVA[XYZ], afVB[XYZ];

	afVA[X] = pV1[X] - pV0[X];	afVA[Y] = pV1[Y] - pV0[Y];	afVA[Z] = pV1[Z] - pV0[Z];
	afVB[X] = pV2[X] - pV0[X];	afVB[Y] = pV2[Y] - pV0[Y];	afVB[Z] = pV2[Z] - pV0[Z];

	MatrixVecCross(pCrs, afVA, afVB);
}
//==============================================================================//


//==============================================================================//
static void MatrixReset(void) {
	Sint08 i;

	iCurrMatrix = 0;
	fAmbiLight = 1.0;	fDiffLight = 0.0;

	for(i = 0;i < XYZW;i++) {
		if(i != W) {
			aiCoordinate[i] = 0x0000;	afCoordinate[i] = 0.0;
			afParaLight[i] = 0.0;
		} else {
			aiCoordinate[i] = 0x0100;	afCoordinate[i] = 1.0;
			afParaLight[i] = 1.0;
		}
	}
}
//------------------------------------------------------------------------------//
static void MatrixFlushScreen(void) {
	if(LcdModeGraphic) {
		SpiLCD.startWrite();
		Canvas.pushSprite((LcdScrnPixelX - LcdCnvsPixelX) >> 1, (LcdScrnPixelY - LcdCnvsPixelY) >> 1);
		SpiLCD.endWrite();
	}

	Canvas.fillScreen(iLcdRgbC1);
}
//------------------------------------------------------------------------------//
static void MatrixAmbiLight(void) {
	if((fAmbiLight + fDiffLight) > 1.0) fDiffLight = 1.0 - fAmbiLight;
}
//------------------------------------------------------------------------------//
static void MatrixDiffLight(void) {
	if((fAmbiLight + fDiffLight) > 1.0) fAmbiLight = 1.0 - fDiffLight;
}
//------------------------------------------------------------------------------//
static void MatrixParaLight(Sflt32* pVec) {
	Sint08 i;

	for(i = 0;i < XYZW;i++) afParaLight[i] = pVec[i];
	MatrixVecUnit(afParaLight);
}
//==============================================================================//


//==============================================================================//
static void MatrixPush(void) {
	if(iCurrMatrix < (MatrixStackMax - 1)) iCurrMatrix++;
}
//------------------------------------------------------------------------------//
static void MatrixPop(void) {
	if(iCurrMatrix > 0) iCurrMatrix--;
}
//------------------------------------------------------------------------------//
static void MatrixUnit(void) {
	Sint08 i, j;
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrMatrix];

	for(i = 0;i < XYZW;i++) {
		for(j = 0;j < XYZW;j++) {
			if(i == j)	pMat[i][j] = 1.0;
			else		pMat[i][j] = 0.0;
		}
	}
}
//------------------------------------------------------------------------------//
static void MatrixCopy(void) {
	Sint08 i, j;
	Sflt32 (* pSrc)[XYZW], (* pDst)[XYZW];

	if(iCurrMatrix < 1) return;

	pSrc = afMatrixStack[iCurrMatrix - 1];
	pDst = afMatrixStack[iCurrMatrix    ];

	for(i = 0;i < XYZW;i++) {
		for(j = 0;j < XYZW;j++) {
			pDst[i][j] = pSrc[i][j];
		}
	}
}
//------------------------------------------------------------------------------//
static void MatrixRotateX(Sflt32 fRad) {
	Sint08 i;
	Sflt32 fTmp, fSin = sin(fRad), fCos = cos(fRad);
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrMatrix];

	for(i = 0;i < XYZW;i++) {
		fTmp = pMat[Y][i];	pMat[Y][i] = fCos * fTmp - fSin * pMat[Z][i];
							pMat[Z][i] = fSin * fTmp + fCos * pMat[Z][i];
	}
}
//------------------------------------------------------------------------------//
static void MatrixRotateY(Sflt32 fRad) {
	Sint08 i;
	Sflt32 fTmp, fSin = sin(fRad), fCos = cos(fRad);
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrMatrix];

	for(i = 0;i < XYZW;i++) {
		fTmp = pMat[Z][i];	pMat[Z][i] = fCos * fTmp - fSin * pMat[X][i];
							pMat[X][i] = fSin * fTmp + fCos * pMat[X][i];
	}
}
//------------------------------------------------------------------------------//
static void MatrixRotateZ(Sflt32 fRad) {
	Sint08 i;
	Sflt32 fTmp, fSin = sin(fRad), fCos = cos(fRad);
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrMatrix];

	for(i = 0;i < XYZW;i++) {
		fTmp = pMat[X][i];	pMat[X][i] = fCos * fTmp - fSin * pMat[Y][i];
							pMat[Y][i] = fSin * fTmp + fCos * pMat[Y][i];
	}
}
//------------------------------------------------------------------------------//
static void MatrixMultiply(Sint08 iLevel) {
	Sint08 i, j;
	Sflt32 (* pSrc)[XYZW], (* pDst)[XYZW];
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrMatrix];

	if((iCurrMatrix < 1)||(iLevel >= iCurrMatrix)) return;

	pSrc = afMatrixStack[iLevel];
	pDst = afMatrixStack[iCurrMatrix - 1];

	for(i = 0;i < XYZW;i++) {
		for(j = 0;j < XYZW;j++) {
			pMat[i][j] =	pDst[i][X] * pSrc[X][j] + pDst[i][Y] * pSrc[Y][j] +
							pDst[i][Z] * pSrc[Z][j] + pDst[i][W] * pSrc[W][j];
		}
	}
}
//------------------------------------------------------------------------------//
static void MatrixTrans(Sflt32* pVec) {
	Sint08 i;
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrMatrix];

	for(i = 0;i < XYZW;i++) {
		pMat[W][i] =	pVec[X] * pMat[X][i] + pVec[Y] * pMat[Y][i] +
						pVec[Z] * pMat[Z][i] + pVec[W] * pMat[W][i];
	}
}
//------------------------------------------------------------------------------//
static void MatrixScale(Sflt32* pVec) {
	Sint08 i, j;
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrMatrix];

	for(i = 0;i < XYZW;i++) {
		for(j = 0;j < XYZW;j++) {
			pMat[i][j] =  pVec[i] * pMat[i][j];
		}
	}
}
//------------------------------------------------------------------------------//
static void MatrixDevice(void) {
	Sflt32 afVec[XYZW];
	MatrixUnit();

	afVec[X] = (Sflt32)(LcdCnvsPixelX >> 1);
	afVec[Y] = (Sflt32)(LcdCnvsPixelY >> 1);

	afVec[Z] = afVec[W] = 1.0;	MatrixTrans(afVec);
	afVec[Y] = -(afVec[X]);		MatrixScale(afVec);
}
//------------------------------------------------------------------------------//
static void MatrixPers(Sflt32 fRad, Sflt32 fNea, Sflt32 fFar) {
	Sflt32 fPer = 1.0 / tan(fRad / 2.0);
	Sflt32 fDis = fFar - fNea;
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrMatrix];

	pMat[X][X] = fPer;	pMat[X][Y] =		pMat[X][Z] =								pMat[X][W] = 0.0;
	pMat[Y][X] = 0.0;	pMat[Y][Y] = fPer;	pMat[Y][Z] =								pMat[Y][W] = 0.0;
	pMat[Z][X] =		pMat[Z][Y] = 0.0;	pMat[Z][Z] =  (      fFar + fNea) / fDis;	pMat[Z][W] = 1.0;
	pMat[W][X] =		pMat[W][Y] = 0.0;	pMat[W][Z] = -(2.0 * fFar * fNea) / fDis;	pMat[W][W] = 0.0;
}
//==============================================================================//


//==============================================================================//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
static Sflt32 afModelVertex[8][XYZW] = {
	{	-1.0,	-1.0,	-1.0,	 1.0,	},
	{	 1.0,	-1.0,	-1.0,	 1.0,	},
	{	-1.0,	 1.0,	-1.0,	 1.0,	},
	{	 1.0,	 1.0,	-1.0,	 1.0,	},
	{	-1.0,	-1.0,	 1.0,	 1.0,	},
	{	 1.0,	-1.0,	 1.0,	 1.0,	},
	{	-1.0,	 1.0,	 1.0,	 1.0,	},
	{	 1.0,	 1.0,	 1.0,	 1.0,	},
};
//------------------------------------------------------------------------------//
static Uint08 aiModelPolygon[12][8] = {
	{	0,	3,	1,	NUL,	NUL,	0xFF,	0xFF,	0x00,	},
	{	0,	2,	3,	NUL,	NUL,	0xFF,	0xFF,	0x00,	},
	{	1,	7,	5,	NUL,	NUL,	0xFF,	0x00,	0x00,	},
	{	1,	3,	7,	NUL,	NUL,	0xFF,	0x00,	0x00,	},
	{	2,	4,	6,	NUL,	NUL,	0xFF,	0x80,	0x00,	},
	{	2,	0,	4,	NUL,	NUL,	0xFF,	0x80,	0x00,	},
	{	3,	6,	7,	NUL,	NUL,	0x00,	0x00,	0xFF,	},
	{	3,	2,	6,	NUL,	NUL,	0x00,	0x00,	0xFF,	},
	{	4,	1,	5,	NUL,	NUL,	0x00,	0xFF,	0x00,	},
	{	4,	0,	1,	NUL,	NUL,	0x00,	0xFF,	0x00,	},
	{	5,	6,	4,	NUL,	NUL,	0xFF,	0xFF,	0xFF,	},
	{	5,	7,	6,	NUL,	NUL,	0xFF,	0xFF,	0xFF,	},
};
//------------------------------------------------------------------------------//
//			6----7
//		   /|   /|
//		  / |  / |
//		 /  4-/--5
//		2----3  /		Y  Z
//		| /  | /		| /
//		|/   |/			|/__X
//		0----1
//------------------------------------------------------------------------------//
static Sflt32 afModelNormal[12][XYZW];

static Sflt32 afCalcVertex[8][XYZW];
static Uint32 aiCalcMaterial[12];
//------------------------------------------------------------------------------//
static void MatrixNormal(void) {
	Sint08 i;
	Sflt32* pV0, * pV1, * pV2;

	for(i = 0;i < 12;i++) {
		pV0 = afModelVertex[aiModelPolygon[i][0]];
		pV1 = afModelVertex[aiModelPolygon[i][1]];
		pV2 = afModelVertex[aiModelPolygon[i][2]];

		MatrixCrsVertex(afModelNormal[i], pV0, pV1, pV2);
		MatrixVecUnit(afModelNormal[i]);
	}
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//------------------------------------------------------------------------------//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//==============================================================================//


//==============================================================================//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
static void MatrixEntry(void) {
	Sint08 i, j;
	Sflt32 fTmp;
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrMatrix];

	for(i = 0;i < 8;i++) {
		for(j = 0;j < XYZW;j++) {
			afCalcVertex[i][j] =	afModelVertex[i][X] * pMat[X][j] + afModelVertex[i][Y] * pMat[Y][j] +
									afModelVertex[i][Z] * pMat[Z][j] + afModelVertex[i][W] * pMat[W][j];
		}

		fTmp = 1.0 / afCalcVertex[i][W];	afCalcVertex[i][W] = 1.0;
		for(j = 0;j < XYZ;j++) afCalcVertex[i][j] *= fTmp;
	}
}
//------------------------------------------------------------------------------//
static void MatrixShade(void) {
	Sint08 i, j;
	Sflt32 fInr, afVec[XYZW];
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrMatrix];

	for(i = 0;i < 12;i++) {
		for(j = 0;j < XYZW;j++) {
			afVec[j] =	afModelNormal[i][X] * pMat[X][j] + afModelNormal[i][Y] * pMat[Y][j] +
						afModelNormal[i][Z] * pMat[Z][j];
		}

		MatrixVecInner(&fInr, afVec, afParaLight);

		if(fInr < 0)	fInr = fAmbiLight;
		else			fInr = fAmbiLight + fDiffLight * fInr;

		for(j = 0;j < XYZW;j++) {
			((Uint08*)(aiCalcMaterial + i))[j] = (Uint08)((Sflt32)aiModelPolygon[i][7 - j] * fInr);
		}
	}
}
//------------------------------------------------------------------------------//
static void MatrixDraw(void) {
	Uint08 iCount;
	Sint08 i;
	Sflt32 afCrs[XYZW], * pV0, * pV1, * pV2;
	static TriInfo asTriangle[12];

	for(iCount = i = 0;i < 12;i++) {
		pV0 = afCalcVertex[aiModelPolygon[i][0]];
		pV1 = afCalcVertex[aiModelPolygon[i][1]];
		pV2 = afCalcVertex[aiModelPolygon[i][2]];

		MatrixCrsVertex(afCrs, pV0, pV2, pV1);			//	Y軸反転済（デバイス座標系）

		if(afCrs[Z] < 0) {
			asTriangle[iCount].aiPos[0][X] = (Sint16)pV0[X];	asTriangle[iCount].aiPos[0][Y] = (Sint16)pV0[Y];
			asTriangle[iCount].aiPos[1][X] = (Sint16)pV1[X];	asTriangle[iCount].aiPos[1][Y] = (Sint16)pV1[Y];
			asTriangle[iCount].aiPos[2][X] = (Sint16)pV2[X];	asTriangle[iCount].aiPos[2][Y] = (Sint16)pV2[Y];
			asTriangle[iCount++].iColor = aiCalcMaterial[i];
		}
	}

	if(iCount > 0) {
		MultiData(CodeTelZ3dPoly);	MultiData(iCount);
		Serial1.write((Uint08*)asTriangle, sizeof(TriInfo) * iCount);
	}
}
//------------------------------------------------------------------------------//
static void MatrixPrint(void) {
	Sint08 i, j;
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrMatrix];

	for(i = 0;i < XYZW;i++) {
		for(j = 0;j < XYZW;j++) {
			Serial.printf("%f | ", pMat[i][j]);
		}

		Serial.println();
	}

	Serial.println();
}
//------------------------------------------------------------------------------//
static void VertexPrint(void) {
	Sint08 i, j;

	for(i = 0;i < 8;i++) {
		for(j = 0;j < XYZW;j++) {
			Serial.printf("%f | ", afCalcVertex[i][j]);
		}

		Serial.println();
	}

	Serial.println();
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//------------------------------------------------------------------------------//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//==============================================================================//


//==============================================================================//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
static void MatrixFrameRate(void) {
	static Uint32 iCount = 0;
	static Uint32 iFrame = 0;
	static Uint32 iMatrixMicros = 0;
	Uint32 iMicros = micros();

	iFrame += 1000000 / (iMicros - iMatrixMicros);
	iMatrixMicros = iMicros;

	if(++iCount >= 100) {
		Serial.printf("[FPS %02X]", iFrame / 100);
		iCount = iFrame = 0;
	}
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//------------------------------------------------------------------------------//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//==============================================================================//


//==============================================================================//
static void Z3dApiInit(void) {
	MatrixReset();	MatrixUnit();
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiFlushScrn(void) {
	MultiData(CodeTelZ3dFlush);
	MultiWait(CodeTelZ3dFlush);

	iCurrZ3dMode = Z3dModeStandBy;
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/*@@@@
	MatrixFrameRate();
@@@@*/
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
}
//------------------------------------------------------------------------------//
static void Z3dApiAmbiLight(void) {
	fAmbiLight = (Sflt32)iPioDataBus / 255.0;
	MatrixAmbiLight();

	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiDiffLight(void) {
	fDiffLight = (Sflt32)iPioDataBus / 255.0;
	MatrixDiffLight();

	MatrixParaLight(afCoordinate);
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatPush(void) {
	MatrixPush();
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatPop(void) {
	MatrixPop();
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatUnit(void) {
	MatrixUnit();
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatCopy(void) {
	MatrixCopy();
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatRotX(void) {
	MatrixRotateX(PI * (Sflt32)(Sint08)iPioDataBus / 128.0);
	iCurrZ3dMode = Z3dModeStandBy;						//	【注意】±境界条件
}
//------------------------------------------------------------------------------//
static void Z3dApiMatRotY(void) {
	MatrixRotateY(PI * (Sflt32)(Sint08)iPioDataBus / 128.0);
	iCurrZ3dMode = Z3dModeStandBy;						//	【注意】±境界条件
}
//------------------------------------------------------------------------------//
static void Z3dApiMatRotZ(void) {
	MatrixRotateZ(PI * (Sflt32)(Sint08)iPioDataBus / 128.0);
	iCurrZ3dMode = Z3dModeStandBy;						//	【注意】±境界条件
}
//------------------------------------------------------------------------------//
static void Z3dApiMatMulti(void) {
	MatrixMultiply((Sint08)iPioDataBus);
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatTrans(void) {
	MatrixTrans(afCoordinate);
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatScale(void) {
	MatrixScale(afCoordinate);
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatDevice(void) {
	MatrixDevice();
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatPers(void) {
	MatrixPers(PI * (Sflt32)iPioDataBus / 256.0, afCoordinate[X], afCoordinate[Y]);
	iCurrZ3dMode = Z3dModeStandBy;						//	【注意】角度の縮尺をRotateXYZに合わせる
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlClear(void) {
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlBuild(void) {
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlVertex(void) {
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlPoly(void) {
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlEntry(void) {
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlShade(void) {
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorXL(void) {
	afCoordinate[X] = FixToFlt(aiCoordinate[X] = (aiCoordinate[X] & 0xFF00) | ((Sfix88)iPioDataBus));
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorXH(void) {
	afCoordinate[X] = FixToFlt(aiCoordinate[X] = (aiCoordinate[X] & 0x00FF) | (((Sfix88)iPioDataBus) << 8));
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorYL(void) {
	afCoordinate[Y] = FixToFlt(aiCoordinate[Y] = (aiCoordinate[Y] & 0xFF00) | ((Sfix88)iPioDataBus));
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorYH(void) {
	afCoordinate[Y] = FixToFlt(aiCoordinate[Y] = (aiCoordinate[Y] & 0x00FF) | (((Sfix88)iPioDataBus) << 8));
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorZL(void) {
	afCoordinate[Z] = FixToFlt(aiCoordinate[Z] = (aiCoordinate[Z] & 0xFF00) | ((Sfix88)iPioDataBus));
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorZH(void) {
	afCoordinate[Z] = FixToFlt(aiCoordinate[Z] = (aiCoordinate[Z] & 0x00FF) | (((Sfix88)iPioDataBus) << 8));
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorWL(void) {
	afCoordinate[W] = FixToFlt(aiCoordinate[W] = (aiCoordinate[W] & 0xFF00) | ((Sfix88)iPioDataBus));
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorWH(void) {
	afCoordinate[W] = FixToFlt(aiCoordinate[W] = (aiCoordinate[W] & 0x00FF) | (((Sfix88)iPioDataBus) << 8));
	iCurrZ3dMode = Z3dModeStandBy;
}
//==============================================================================//


//==============================================================================//
static void Z3dApiStandBy(void) {
	iCurrZ3dMode = Z3dModeStandBy;
}
//------------------------------------------------------------------------------//
static void (* apZ3dApiMode[Z3dModeMax])(void) = {
	Z3dApiStandBy  ,

	Z3dApiInit     ,	Z3dApiFlushScrn,	Z3dApiAmbiLight,	Z3dApiDiffLight,
	Z3dApiMatPush  ,	Z3dApiMatPop   ,	Z3dApiMatUnit  ,	Z3dApiMatCopy  ,

	Z3dApiMatRotX  ,	Z3dApiMatRotY  ,	Z3dApiMatRotZ  ,	Z3dApiMatMulti ,
	Z3dApiMatTrans ,	Z3dApiMatScale ,	Z3dApiMatDevice,	Z3dApiMatPers  ,

	Z3dApiMdlClear ,	Z3dApiMdlBuild ,	Z3dApiMdlVertex,	Z3dApiMdlPoly  ,
	Z3dApiMdlEntry ,	Z3dApiMdlShade ,

	Z3dApiVectorXL ,	Z3dApiVectorXH ,	Z3dApiVectorYL ,	Z3dApiVectorYH ,
	Z3dApiVectorZL ,	Z3dApiVectorZH ,	Z3dApiVectorWL ,	Z3dApiVectorWH ,
};
//==============================================================================//


//==============================================================================//
static void MatrixInit(Sint08 iReset) {
	if(Esp32Slave) {
		if(iReset != False) Canvas.fillScreen(iLcdRgbC1);
		return;
	}

	if(iReset == False) {
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
		MatrixNormal();
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
	}

	iCurrZ3dMode = Z3dModeStandBy;
	MatrixReset();	MatrixUnit();
}
//------------------------------------------------------------------------------//
static void MatrixMove(void) {
	if(Esp32Slave) return;
	(* apZ3dApiMode[iCurrZ3dMode])();
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

