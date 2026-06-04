
//==============================================================================//
//	Z80 Single Board Computer "PC-84C0SD 20MHz" I/O Sketch for ESP32-S3-MINI-1	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
enum {
	Z3dProcStandBy  ,									//	Z3Dモード（待機）

	Z3dProcInit     ,									//	Z3Dモード（初期化）
	Z3dProcFlushScrn,									//	Z3Dモード（画面更新）
	Z3dProcAmbiLight,									//	Z3Dモード（環境反射係数）
	Z3dProcDiffLight,									//	Z3Dモード（拡散反射係数）
	Z3dProcMatPush  ,									//	Z3Dモード（行列積重）
	Z3dProcMatPop   ,									//	Z3Dモード（行列取除）
	Z3dProcMatUnit  ,									//	Z3Dモード（単位行列）
	Z3dProcMatCopy  ,									//	Z3Dモード（行列複写）

	Z3dProcMatRotX  ,									//	Z3Dモード（回転X軸）
	Z3dProcMatRotY  ,									//	Z3Dモード（回転Y軸）
	Z3dProcMatRotZ  ,									//	Z3Dモード（回転Z軸）
	Z3dProcMatMulti ,									//	Z3Dモード（行列乗算）
	Z3dProcMatTrans ,									//	Z3Dモード（平行移動）
	Z3dProcMatScale ,									//	Z3Dモード（拡大縮小）
	Z3dProcMatDevice,									//	Z3Dモード（表示機器行列）
	Z3dProcMatPers  ,									//	Z3Dモード（透視変換行列）

	Z3dProcMdlClear ,									//	Z3Dモード（模型消去）
	Z3dProcMdlBuild ,									//	Z3Dモード（模型追加・変更）
	Z3dProcMdlVertex,									//	Z3Dモード（頂点追加・変更）
	Z3dProcMdlPoly  ,									//	Z3Dモード（多角追加・変更）
	Z3dProcMdlEntry ,									//	Z3Dモード（模型登録）
	Z3dProcMdlShade ,									//	Z3Dモード（模型陰影）

	Z3dProcVectorXL ,									//	Z3Dモード（座標X下位）
	Z3dProcVectorXH ,									//	Z3Dモード（座標X上位）
	Z3dProcVectorYL ,									//	Z3Dモード（座標Y下位）
	Z3dProcVectorYH ,									//	Z3Dモード（座標Y上位）
	Z3dProcVectorZL ,									//	Z3Dモード（座標Z下位）
	Z3dProcVectorZH ,									//	Z3Dモード（座標Z上位）
	Z3dProcVectorWL ,									//	Z3Dモード（座標W下位）
	Z3dProcVectorWH ,									//	Z3Dモード（座標W上位）

	Z3dProcMax,											//	Z3Dモード上限
};
//------------------------------------------------------------------------------//
#define		Z3dMatrixMax				0x10			//	行列スタック上限

#define		ModelStorageMax				0x9000			//	模型格納領域上限
#define		Z3dModelMax					0x0100			//	模型追加上限 ※
														//	※【注意】上限検査省略
#define		Z3dVertexMax				0xFF			//	頂点追加上限 ※
#define		Z3dPolygonMax				0xFF			//	多角追加上限 ※
//------------------------------------------------------------------------------//
#define		MdlAttrBackJudge			0x01			//	模型描画属性（裏面判定）
#define		MdlAttrWireFrame			0x02			//	模型描画属性（針金枠組）
#define		MdlAttrDepthSort			0x80			//	模型描画属性（奥行整列）
//------------------------------------------------------------------------------//
typedef struct tPolyInfo {
	Uint08				aiVertex[XYZ];					//	多角頂点
	Uint08				iReservedV;						//	予備

	Uint08				aiMaterial[XYZ];				//	多角材質
	Uint08				iReservedM;						//	予備
} PolyInfo;
//------------------------------------------------------------------------------//
typedef struct tModelInfo {
	union tModelHead {
		struct tInternal {
			Uint08				iVertexCount;			//	頂点数
			Uint08				iPolygonCount;			//	多角数

			Uint08				iAttribute;				//	描画属性
			Uint08				iAllocate;				//	領域割当
		} Internal;										//	内部形式

		Uint32				iExternal;					//	外部形式
	} ModelHead;										//	先頭データ

	Sflt32				(* pVertex)[XYZW];				//	格納領域（模型頂点）
	Sflt32				(* pNormal)[XYZW];				//	格納領域（模型法線）

	PolyInfo*			pPolyInfo;						//	格納領域（模型多角）
} ModelInfo;											//	模型情報
//------------------------------------------------------------------------------//
typedef struct tMaterialInfo {
	Uint08				aiMaterial[XYZ];				//	多角材質
	Uint08				iReservedM;						//	予備

	Sflt32				fCenter;						//	多角重心
} MaterialInfo;
//------------------------------------------------------------------------------//
typedef struct tTriangleInfo {
	Sint16				aiPos[XYZ][XY];					//	表示座標

	Uint32				iColor;							//	表示色彩
} TriangleInfo;											//	三角情報
//------------------------------------------------------------------------------//
static void MatrixFlushScreen(void);
static void MatrixTriangle(void);
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static Sint08 iCurrZ3dProc;								//	現行Z3Dモード
static Sint08 iCurrZ3dMatrix;							//	現行行列

static Uint08 iCurrZ3dModel;							//	現行模型
static Uint16 iCurrZ3dStorage;							//	現行格納領域
//------------------------------------------------------------------------------//
static Uint16 iModelEntry;								//	模型登録
static Uint16 iModelShade;								//	模型陰影

static Sfix88 aiCoordinate[XYZW];						//	演算座標（固定小数点数）
static Sflt32 afCoordinate[XYZW];						//	演算座標（浮動小数点数）
//------------------------------------------------------------------------------//
static Sflt32 fAmbiLight;								//	環境反射係数
static Sflt32 fDiffLight;								//	拡散反射係数

static Sflt32 afParaLight[XYZW];						//	平行光源ベクトル
//------------------------------------------------------------------------------//
#ifdef		BuildMaster
static Sflt32 afMatrixStack[Z3dMatrixMax][XYZW][XYZW];	//	行列スタック

static ModelInfo asModelInfo[Z3dModelMax];				//	模型情報
static Uint32 aiModelStorage[ModelStorageMax];			//	模型格納領域

static Uint08 aiCalcDepthSort[Z3dPolygonMax];			//	演算領域（奥行整列）
static Sflt32 afCalcVertex[Z3dVertexMax][XYZW];			//	演算領域（模型頂点）

static MaterialInfo asCalcMaterial[Z3dPolygonMax];		//	演算領域（多角材質）
static TriangleInfo asCalcTriangle[Z3dPolygonMax];		//	演算領域（三角情報）
#endif
//------------------------------------------------------------------------------//
#ifdef		BuildSlave
static Sflt32 afMatrixStack[1][XYZW][XYZW];				//	行列スタック

static ModelInfo asModelInfo[1];						//	模型情報
static Uint32 aiModelStorage[1];						//	模型格納領域

static Uint08 aiCalcDepthSort[1];						//	演算領域（奥行整列）
static Sflt32 afCalcVertex[1][XYZW];					//	演算領域（模型頂点）

static MaterialInfo asCalcMaterial[1];					//	演算領域（多角材質）
static TriangleInfo asCalcTriangle[1];					//	演算領域（三角情報）
#endif
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
	for(i = X;i < XYZ;i++) pVec[i] *= fInr;
}
//------------------------------------------------------------------------------//
static void MatrixPosCross(Sflt32* pCrs, Sflt32* apPos[]) {
	Sint08 i;
	Sflt32 afVA[XYZ], afVB[XYZ];

	for(i = X;i < XYZ;i++) {	afVA[i] = apPos[Y][i] - apPos[X][i];	afVB[i] = apPos[Z][i] - apPos[X][i];	}
	MatrixVecCross(pCrs, afVA, afVB);
}
//==============================================================================//


//==============================================================================//
static void MatrixReset(void) {
	Sint08 i;

	iCurrZ3dMatrix = 0;
	fAmbiLight = 1.0;	fDiffLight = 0.0;

	for(i = X;i < XYZW;i++) {
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

	for(i = X;i < XYZW;i++) afParaLight[i] = pVec[i];
	MatrixVecUnit(afParaLight);
}
//==============================================================================//


//==============================================================================//
static void MatrixPush(void) {
	if(iCurrZ3dMatrix < (Z3dMatrixMax - 1)) iCurrZ3dMatrix++;
}
//------------------------------------------------------------------------------//
static void MatrixPop(void) {
	if(iCurrZ3dMatrix > 0) iCurrZ3dMatrix--;
}
//------------------------------------------------------------------------------//
static void MatrixUnit(void) {
	Sint08 i, j;
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrZ3dMatrix];

	for(i = X;i < XYZW;i++) {
		for(j = X;j < XYZW;j++) {
			if(i == j)	pMat[i][j] = 1.0;
			else		pMat[i][j] = 0.0;
		}
	}
}
//------------------------------------------------------------------------------//
static void MatrixCopy(void) {
	Sint08 i, j;
	Sflt32 (* pSrc)[XYZW], (* pDst)[XYZW];

	if(iCurrZ3dMatrix < 1) return;

	pSrc = afMatrixStack[iCurrZ3dMatrix - 1];
	pDst = afMatrixStack[iCurrZ3dMatrix    ];

	for(i = X;i < XYZW;i++) {
		for(j = X;j < XYZW;j++) {
			pDst[i][j] = pSrc[i][j];
		}
	}
}
//------------------------------------------------------------------------------//
static void MatrixRotateX(Sflt32 fRad) {
	Sint08 i;
	Sflt32 fTmp, fSin = sin(fRad), fCos = cos(fRad);
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrZ3dMatrix];

	for(i = X;i < XYZW;i++) {
		fTmp = pMat[Y][i];	pMat[Y][i] = fCos * fTmp - fSin * pMat[Z][i];
							pMat[Z][i] = fSin * fTmp + fCos * pMat[Z][i];
	}
}
//------------------------------------------------------------------------------//
static void MatrixRotateY(Sflt32 fRad) {
	Sint08 i;
	Sflt32 fTmp, fSin = sin(fRad), fCos = cos(fRad);
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrZ3dMatrix];

	for(i = X;i < XYZW;i++) {
		fTmp = pMat[Z][i];	pMat[Z][i] = fCos * fTmp - fSin * pMat[X][i];
							pMat[X][i] = fSin * fTmp + fCos * pMat[X][i];
	}
}
//------------------------------------------------------------------------------//
static void MatrixRotateZ(Sflt32 fRad) {
	Sint08 i;
	Sflt32 fTmp, fSin = sin(fRad), fCos = cos(fRad);
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrZ3dMatrix];

	for(i = X;i < XYZW;i++) {
		fTmp = pMat[X][i];	pMat[X][i] = fCos * fTmp - fSin * pMat[Y][i];
							pMat[Y][i] = fSin * fTmp + fCos * pMat[Y][i];
	}
}
//------------------------------------------------------------------------------//
static void MatrixMultiply(Sint08 iLevel) {
	Sint08 i, j;
	Sflt32 (* pSrc)[XYZW], (* pDst)[XYZW];
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrZ3dMatrix];

	if((iCurrZ3dMatrix < 1)||(iLevel >= iCurrZ3dMatrix)) return;

	pSrc = afMatrixStack[iLevel];
	pDst = afMatrixStack[iCurrZ3dMatrix - 1];

	for(i = X;i < XYZW;i++) {
		for(j = X;j < XYZW;j++) {
			pMat[i][j] =	pDst[i][X] * pSrc[X][j] + pDst[i][Y] * pSrc[Y][j] +
							pDst[i][Z] * pSrc[Z][j] + pDst[i][W] * pSrc[W][j];
		}
	}
}
//------------------------------------------------------------------------------//
static void MatrixTrans(Sflt32* pVec) {
	Sint08 i;
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrZ3dMatrix];

	for(i = X;i < XYZW;i++) {
		pMat[W][i] =	pVec[X] * pMat[X][i] + pVec[Y] * pMat[Y][i] +
						pVec[Z] * pMat[Z][i] + pVec[W] * pMat[W][i];
	}
}
//------------------------------------------------------------------------------//
static void MatrixScale(Sflt32* pVec) {
	Sint08 i, j;
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrZ3dMatrix];

	for(i = X;i < XYZW;i++) {
		for(j = X;j < XYZW;j++) {
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
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrZ3dMatrix];

	pMat[X][X] = fPer;	pMat[X][Y] =		pMat[X][Z] =								pMat[X][W] = 0.0;
	pMat[Y][X] = 0.0;	pMat[Y][Y] = fPer;	pMat[Y][Z] =								pMat[Y][W] = 0.0;
	pMat[Z][X] =		pMat[Z][Y] = 0.0;	pMat[Z][Z] =  (      fFar + fNea) / fDis;	pMat[Z][W] = 1.0;
	pMat[W][X] =		pMat[W][Y] = 0.0;	pMat[W][Z] = -(2.0 * fFar * fNea) / fDis;	pMat[W][W] = 0.0;
}
//==============================================================================//


//==============================================================================//
static void MatrixClear(void) {
	Sint16 i;

	iCurrZ3dModel = 0;	iCurrZ3dStorage = 0;
	iModelEntry = iModelShade = Z3dModelMax;

	if(Esp32Master) for(i = 0;i < Z3dModelMax;i++)	asModelInfo[i].ModelHead.iExternal = 0;
	else											asModelInfo[0].ModelHead.iExternal = 0;
}
//------------------------------------------------------------------------------//
static void MatrixBuild(Uint08 iModel) {
	Uint08 iVertex, iPolygon;
	Uint16 iStorage = iCurrZ3dStorage;
	ModelInfo* pModel = &(asModelInfo[iCurrZ3dModel = iModel]);

	pModel->ModelHead.Internal.iVertexCount  = iVertex  = (Uint08)(aiCoordinate[X] & 0x00FF);
	pModel->ModelHead.Internal.iPolygonCount = iPolygon = (Uint08)(aiCoordinate[Y] & 0x00FF);
	pModel->ModelHead.Internal.iAttribute               = (Uint08)(aiCoordinate[Z] & 0x00FF);

	pModel->pVertex   = (Sflt32(*)[XYZW])(&(aiModelStorage[iStorage]));
	iStorage += (Uint16)iVertex  * XYZW;

	pModel->pNormal   = (Sflt32(*)[XYZW])(&(aiModelStorage[iStorage]));
	iStorage += (Uint16)iPolygon * XYZW;

	pModel->pPolyInfo =       (PolyInfo*)(&(aiModelStorage[iStorage]));
	iStorage += (Uint16)iPolygon * (sizeof(PolyInfo) >> 2);

	if((iVertex == 0)||(iPolygon == 0)||(iStorage > ModelStorageMax)) {		pModel->ModelHead.iExternal = 0;	}
	else {	iCurrZ3dStorage = iStorage;		pModel->ModelHead.Internal.iAllocate = True;	}
}
//------------------------------------------------------------------------------//
static void MatrixVertex(Uint08 iVertex) {
	Sint08 i;
	ModelInfo* pModel = &(asModelInfo[iCurrZ3dModel]);

	if(iVertex < pModel->ModelHead.Internal.iVertexCount) {
		for(i = X;i < XYZW;i++) pModel->pVertex[iVertex][i] = afCoordinate[i];
	}
}
//------------------------------------------------------------------------------//
static void MatrixPolygon(Uint08 iPolygon) {
	Sint08 i, j;
	ModelInfo* pModel = &(asModelInfo[iCurrZ3dModel]);
	Sflt32* apPos[XYZ];

	if(iPolygon < pModel->ModelHead.Internal.iPolygonCount) {
		for(i = X;i < XYZW;i++) ((Sfix88*)(pModel->pPolyInfo + iPolygon))[i] = aiCoordinate[i];
		for(j = X;j < XYZ;j++) apPos[j] = pModel->pVertex[pModel->pPolyInfo[iPolygon].aiVertex[j]];

		MatrixPosCross(pModel->pNormal[iPolygon], apPos);
		MatrixVecUnit(pModel->pNormal[iPolygon]);
	}
}
//------------------------------------------------------------------------------//
static void MatrixEntry(Uint08 iModel) {
	Sint16 i, j;
	ModelInfo* pModel = &(asModelInfo[iCurrZ3dModel = iModel]);
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrZ3dMatrix];
	Sflt32 fTmp;

	for(i = 0;i < pModel->ModelHead.Internal.iVertexCount;i++) {
		for(j = X;j < XYZW;j++) {
			afCalcVertex[i][j] =	pModel->pVertex[i][X] * pMat[X][j] + pModel->pVertex[i][Y] * pMat[Y][j] +
									pModel->pVertex[i][Z] * pMat[Z][j] + pModel->pVertex[i][W] * pMat[W][j];
		}

		fTmp = 1.0 / fabs(afCalcVertex[i][W]);
		for(j = X;j < XYZ;j++) afCalcVertex[i][j] *= fTmp;
	}
}
//------------------------------------------------------------------------------//
static void MatrixShade(Uint08 iModel) {
	Sint16 i, j;
	ModelInfo* pModel = &(asModelInfo[iCurrZ3dModel = iModel]);
	Sflt32 (* pMat)[XYZW] = afMatrixStack[iCurrZ3dMatrix];
	Sflt32 fInr, afVec[XYZW];

	for(i = 0;i < pModel->ModelHead.Internal.iPolygonCount;i++) {
		for(j = X;j < XYZW;j++) {
			afVec[j] =	pModel->pNormal[i][X] * pMat[X][j] + pModel->pNormal[i][Y] * pMat[Y][j] +
						pModel->pNormal[i][Z] * pMat[Z][j];
		}

		MatrixVecInner(&fInr, afVec, afParaLight);

		if(fInr < 0)	fInr = fAmbiLight;
		else			fInr = fAmbiLight + fDiffLight * fInr;

		for(j = X;j < XYZ;j++) {
			asCalcMaterial[i].aiMaterial[j] = (Uint08)((Sflt32)(pModel->pPolyInfo[i].aiMaterial[j]) * fInr);
		}
	}
}
//------------------------------------------------------------------------------//
static int MatrixCompare(const void* pDSL, const void* pDSR) {
	Sflt32 fPCL = asCalcMaterial[*(Uint08*)pDSL].fCenter;
	Sflt32 fPCR = asCalcMaterial[*(Uint08*)pDSR].fCenter;

	return((fPCL < fPCR) - (fPCL > fPCR));
}
//------------------------------------------------------------------------------//
static void MatrixDraw(void) {
	Sint16 i, j, iCount;
	ModelInfo* pModel = &(asModelInfo[iCurrZ3dModel]);
	Uint08 iBackJudge = pModel->ModelHead.Internal.iAttribute & MdlAttrBackJudge;
	Uint08 iDepthSort = pModel->ModelHead.Internal.iAttribute & MdlAttrDepthSort;
	Sflt32 afCrs[XYZW], * apPos[XYZ];

	for(i = 0;i < pModel->ModelHead.Internal.iPolygonCount;i++) {
		aiCalcDepthSort[i] = i;

		if(iDepthSort != False) {
			asCalcMaterial[i].fCenter  = afCalcVertex[pModel->pPolyInfo[i].aiVertex[X]][Z];
			asCalcMaterial[i].fCenter += afCalcVertex[pModel->pPolyInfo[i].aiVertex[Y]][Z];
			asCalcMaterial[i].fCenter += afCalcVertex[pModel->pPolyInfo[i].aiVertex[Z]][Z];
		}
	}

	if(iDepthSort != False) {
		qsort(aiCalcDepthSort, pModel->ModelHead.Internal.iPolygonCount, sizeof(Uint08), MatrixCompare);
	}

	for(iCount = i = 0;i < pModel->ModelHead.Internal.iPolygonCount;i++) {
		apPos[X] = afCalcVertex[pModel->pPolyInfo[aiCalcDepthSort[i]].aiVertex[X]];
		apPos[Y] = afCalcVertex[pModel->pPolyInfo[aiCalcDepthSort[i]].aiVertex[Z]];		//	Y軸反転済（デバイス座標系）
		apPos[Z] = afCalcVertex[pModel->pPolyInfo[aiCalcDepthSort[i]].aiVertex[Y]];		//	Y軸反転済（デバイス座標系）

		if((apPos[X][Z] < 0.0)||(apPos[X][Z] > 2.0)) continue;
		if((apPos[Y][Z] < 0.0)||(apPos[X][Z] > 2.0)) continue;
		if((apPos[Z][Z] < 0.0)||(apPos[X][Z] > 2.0)) continue;

		if(iBackJudge != False) {
			MatrixPosCross(afCrs, apPos);
			if(afCrs[Z] > 0.0) continue;
		}

		for(j = X;j < XYZ;j++) {
			asCalcTriangle[iCount].aiPos[j][X] = (Sint16)apPos[j][X];
			asCalcTriangle[iCount].aiPos[j][Y] = (Sint16)apPos[j][Y];
		}

		asCalcTriangle[iCount++].iColor = *((Uint32*)(asCalcMaterial[aiCalcDepthSort[i]].aiMaterial));
	}

	if(iCount > 0) {
		MultiData(CodeTelZ3dPoly);	MultiData(pModel->ModelHead.Internal.iAttribute);
		MultiData((Uint08)iCount);	Serial1.write((Uint08*)asCalcTriangle, sizeof(TriangleInfo) * iCount);
	}
}
//------------------------------------------------------------------------------//
static void MatrixTriangle(void) {
	Uint08 iAttribute = MultiRecep();
	Uint08 iBackJudge = iAttribute & MdlAttrBackJudge;
	Uint08 iWireFrame = iAttribute & MdlAttrWireFrame;
	Sint16 i, iCount = (Sint16)MultiRecep();
	TriangleInfo sTriangle;

	i = sizeof(TriangleInfo) * iCount;
	while(Serial1.available() < i);

	for(i = 0;i < iCount;i++) {
		Serial1.read((Uint08*)(&sTriangle), sizeof(TriangleInfo));

		if(iWireFrame != False) {
			if(iBackJudge != False) {
				Canvas.fillTriangle(	sTriangle.aiPos[0][X], sTriangle.aiPos[0][Y],
										sTriangle.aiPos[1][X], sTriangle.aiPos[1][Y],
										sTriangle.aiPos[2][X], sTriangle.aiPos[2][Y], iLcdRgbC1		);
			}

			Canvas.drawTriangle(	sTriangle.aiPos[0][X], sTriangle.aiPos[0][Y],
									sTriangle.aiPos[1][X], sTriangle.aiPos[1][Y],
									sTriangle.aiPos[2][X], sTriangle.aiPos[2][Y], sTriangle.iColor	);
		} else {
			Canvas.fillTriangle(	sTriangle.aiPos[0][X], sTriangle.aiPos[0][Y],
									sTriangle.aiPos[1][X], sTriangle.aiPos[1][Y],
									sTriangle.aiPos[2][X], sTriangle.aiPos[2][Y], sTriangle.iColor	);
		}
	}
}
//==============================================================================//


//==============================================================================//
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
//==============================================================================//


//==============================================================================//
static void Z3dApiInit(void) {
	MatrixReset();	MatrixUnit();	MatrixClear();
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiFlushScrn(void) {
	MultiData(CodeTelZ3dFlush);
	MultiWait(CodeTelZ3dFlush);

//	MatrixFrameRate();
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiAmbiLight(void) {
	fAmbiLight = (Sflt32)iPioDataBus / 255.0;
	MatrixAmbiLight();

	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiDiffLight(void) {
	fDiffLight = (Sflt32)iPioDataBus / 255.0;
	MatrixDiffLight();

	MatrixParaLight(afCoordinate);
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatPush(void) {
	MatrixPush();
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatPop(void) {
	MatrixPop();
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatUnit(void) {
	MatrixUnit();
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatCopy(void) {
	MatrixCopy();
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatRotX(void) {
	MatrixRotateX(PI * (Sflt32)(Sint08)iPioDataBus / 128.0);
	iCurrZ3dProc = Z3dProcStandBy;						//	【注意】±境界条件
}
//------------------------------------------------------------------------------//
static void Z3dApiMatRotY(void) {
	MatrixRotateY(PI * (Sflt32)(Sint08)iPioDataBus / 128.0);
	iCurrZ3dProc = Z3dProcStandBy;						//	【注意】±境界条件
}
//------------------------------------------------------------------------------//
static void Z3dApiMatRotZ(void) {
	MatrixRotateZ(PI * (Sflt32)(Sint08)iPioDataBus / 128.0);
	iCurrZ3dProc = Z3dProcStandBy;						//	【注意】±境界条件
}
//------------------------------------------------------------------------------//
static void Z3dApiMatMulti(void) {
	MatrixMultiply((Sint08)iPioDataBus);
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatTrans(void) {
	MatrixTrans(afCoordinate);
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatScale(void) {
	MatrixScale(afCoordinate);
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatDevice(void) {
	MatrixDevice();
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMatPers(void) {
	MatrixPers(PI * (Sflt32)iPioDataBus / 256.0, afCoordinate[X], afCoordinate[Y]);
	iCurrZ3dProc = Z3dProcStandBy;						//	【注意】角度の縮尺をRotateXYZに合わせる
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlClear(void) {
	MatrixClear();
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlBuild(void) {
	MatrixBuild(iPioDataBus);
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlVertex(void) {
	MatrixVertex(iPioDataBus);
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlPoly(void) {
	MatrixPolygon(iPioDataBus);
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlEntry(void) {
	MatrixEntry(iPioDataBus);

	if((iModelEntry = (Uint16)iPioDataBus) == iModelShade) {
		MatrixDraw();	iModelEntry = iModelShade = Z3dModelMax;
	}

	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiMdlShade(void) {
	MatrixShade(iPioDataBus);

	if((iModelShade = (Uint16)iPioDataBus) == iModelEntry) {
		MatrixDraw();	iModelEntry = iModelShade = Z3dModelMax;
	}

	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorXL(void) {
	afCoordinate[X] = (Sflt32)(aiCoordinate[X] = (aiCoordinate[X] & 0xFF00) | (((Sfix88)iPioDataBus) << 0)) / 256.0;
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorXH(void) {
	afCoordinate[X] = (Sflt32)(aiCoordinate[X] = (aiCoordinate[X] & 0x00FF) | (((Sfix88)iPioDataBus) << 8)) / 256.0;
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorYL(void) {
	afCoordinate[Y] = (Sflt32)(aiCoordinate[Y] = (aiCoordinate[Y] & 0xFF00) | (((Sfix88)iPioDataBus) << 0)) / 256.0;
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorYH(void) {
	afCoordinate[Y] = (Sflt32)(aiCoordinate[Y] = (aiCoordinate[Y] & 0x00FF) | (((Sfix88)iPioDataBus) << 8)) / 256.0;
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorZL(void) {
	afCoordinate[Z] = (Sflt32)(aiCoordinate[Z] = (aiCoordinate[Z] & 0xFF00) | (((Sfix88)iPioDataBus) << 0)) / 256.0;
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorZH(void) {
	afCoordinate[Z] = (Sflt32)(aiCoordinate[Z] = (aiCoordinate[Z] & 0x00FF) | (((Sfix88)iPioDataBus) << 8)) / 256.0;
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorWL(void) {
	afCoordinate[W] = (Sflt32)(aiCoordinate[W] = (aiCoordinate[W] & 0xFF00) | (((Sfix88)iPioDataBus) << 0)) / 256.0;
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void Z3dApiVectorWH(void) {
	afCoordinate[W] = (Sflt32)(aiCoordinate[W] = (aiCoordinate[W] & 0x00FF) | (((Sfix88)iPioDataBus) << 8)) / 256.0;
	iCurrZ3dProc = Z3dProcStandBy;
}
//==============================================================================//


//==============================================================================//
static void Z3dApiStandBy(void) {
	iCurrZ3dProc = Z3dProcStandBy;
}
//------------------------------------------------------------------------------//
static void (* apZ3dApiProc[Z3dProcMax])(void) = {
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
	iCurrZ3dProc = Z3dProcStandBy;
	MatrixReset();	MatrixUnit();	MatrixClear();

	if(Esp32Slave) {
		if(iReset == False)		Canvas.createSprite(LcdCnvsPixelX, LcdCnvsPixelY);
		else					Canvas.fillScreen(iLcdRgbC1);
	}
}
//------------------------------------------------------------------------------//
static void MatrixMove(void) {
	if(Esp32Slave) return;
	(* apZ3dApiProc[iCurrZ3dProc])();
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

