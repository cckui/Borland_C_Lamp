//---------------------------------------------------------------------------
//***************************************************************************
//** Create Date : 2010.10.13              Ver.:1.00_A       by Cuiu_Handsome
//** Function : 共用 Function 在此進行，為 TGentools 之繼承者
//***************************************************************************
//---------------------------------------------------------------------------
#ifndef TBasetoolsH
#define TBasetoolsH
#include "inifiles.hpp"
//#include "dout.h"
#include <map>
#include <vector>
#include <list>
#include <math.hpp>
#include <math.h>
#include <stdio.h>
#include <vcl.h>
//#include "TControlDefine.h"
#include "TModuleThread.h"
//----------------------------------------------------------------------------
const double Math_Pi = 3.14159265;
const double Math_Epsion =  1.0e-6;
//----------------------------------------------------------------------------
enum eDirActType{eDirNone,eDirTimeEQ,eDirTimeLG,eDirTimeLE,
				 eDirTimeEQLG,eDirTimeEQLE,eDirExtName,eDirIncd};
//-- 座標位置定義 Structure -------------------------------------------------
struct sPos_2D{
	double _Pos1;
	double _Pos2 ;
};
struct sPos_3D{
	double _Pos1;
	double _Pos2 ;
	double _Pos3 ;
};
struct sPos_4D{
	double _Pos1;
	double _Pos2 ;
	double _Pos3 ;
    double _Pos4 ;
};
//---------------------------------------------------------------------------
class TBasetools : public TObject
{
	 private:
		void __fastcall VCLTMenuItemsProcess(String LevelSer,TMenuItem* ActMenuItem,TMemIniFile* ActMemIniFile); // 擷取 TMenuItem 之元件名稱
       	void   __fastcall CreatingFileList(TStringList* &P, AnsiString SubDir,
										 eDirActType ActType,String CondStr,
										 bool bSubDirInclude = false);
		String __fastcall DirGetFileName(TSearchRec FileName,eDirActType ActType,String CondStr);
	 protected:  //User declarations
		__fastcall  TBasetools(void);   // 將建構式及解構式移到私有區或保護區
		__fastcall  ~TBasetools(void);  // 禁止外部程式直接New物件產生
	 public:
		//-- 數學運算函式 ---------------------
		double __fastcall mod45(double modvalue, int bitnum);
		//--  系統及API共用函式  -------------------------------------
		bool __fastcall SafeCreateSystem(AnsiString AppName,HWND hHandle);   // 避免系統重覆開啟
		void __fastcall SetSystemWorkArea(TForm* ActForm,int Width = 1024,int Height = 738);  // 系統開啟顯示位置
		//-- 檔案處理函式 --------------------------------------------
		String __fastcall FindToLastDirectory(String FileName);    // 取得最底層之路徑名稱
		String __fastcall FindToLastFileName(String FileName);     // 取得不含路徑的檔案名稱
		bool __fastcall CheckDirectory(String DirName);            // 檢查路徑，不存在則新增
		void __fastcall CreateUniCodeFile(String FileName);        // 產生 UniCode 格式之檔案
		void __fastcall TMemIniFile_DataRead(TMemIniFile* ActMemFile, AnsiString FileName); // 將檔案讀入IMemIniFile
		void __fastcall SaveUniCodeFile(TMemIniFile* MemIniFile,String FileName);           // 將 TMemIniFile 資料存入 UniCode 資料檔
		void __fastcall CreateFileList(AnsiString Dir,eDirActType ActType,String CondStr,TStringList* SList,
										 bool bSubDirInclude = false);
		//-- 語系處理函式 ---------------------------------------------
		void __fastcall GetVCLTMenuItemsData(TMenu* ActMenu,TMemIniFile* ActMemIniFile); // 擷取 TMenu 下 Menuitem之元件名稱
		//---- 資料庫處理FUNCTION（含實體資料庫、文字檔、TSTRINGGRID。。。等）------
		String __fastcall GetUniqueKey(void);
		//-- 座標計算 --------------------------------------------------------------
		bool __fastcall GetArcCenter_3Pos(sPos_2D* CenterResult,sPos_2D* CoPos1,
										  sPos_2D* CoPos2, sPos_2D* CoPos3);
		//-- 數字格式轉換 / 檢查 -----------------------------------------------------------
		String __fastcall Int_Hex(int TransValue,int ByteNum);
		String __fastcall Int_Hex_16(int TransValue);
		String __fastcall Int_Hex_32(int TransValue);
		String __fastcall Int_Hex_64(int TransValue);
		String __fastcall Int_Bin(int TransValue);
		int    __fastcall Bin_Int(String TransValue);
		bool   __fastcall String_Int(String TransStr,int &RetValue);
		bool   __fastcall String_Double(String TransStr,double &RetValue);
		void __fastcall GetMainFormBasePos(TControl* ActControl,String BaseFormName,int &Top,int &Left);

		bool __fastcall DataTypeCheck_Numeral(String Context, String &MSG , int LanguageType , bool bIntCheck = false);
		//-- 各種 CheckSum的計算方式 ----
		int  __fastcall GetCheckSum_XOR(AnsiString CheckData);
		//--- 資料格式轉換 --------------
		String  __fastcall LongTypeToTimeType(long lTime);
		//--- 浮點數檢查 -----------------
		bool  __fastcall FloatDatatypeCheck(String ChkValue);
		//-- 影像座標和軸座標系補償原則 （所有運算均以 極座標為原則）------------
		//-- 1. 影像座標系永遠為第 IV 象限
		//-- 2. 軸座標系為第 I 象限時 : X-> -X , Y -> Y , θ -> θ
		//-- 3. 軸座標系為第 II 象限時 : X-> X , Y -> Y , θ -> -θ
		//-- 4. 軸座標系為第 III 象限時 : X-> X , Y -> -Y , θ -> θ
		//-- 5. 軸座標系為第 IV 象限時 : X-> -X , Y -> -Y , θ -> -θ
		//-- 平面座標系位置/角度計算函式 -----
		double  __fastcall GetLineSlopAngle(sPos_2D* Line_Point1,sPos_2D* Line_Point2); //-- 斜率/角度
		double  __fastcall GetCrossAngle_3Points(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
												 sPos_2D* Point_Base3,bool &bCCW); //-- 利用內積公式找夾角
		double  __fastcall GetCrossAngle_2Lines(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
												 sPos_2D* Point_Base3,sPos_2D* Point_Base4); //-- 利用內積公式找夾角
		bool  __fastcall GetLineSegmentPoint(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
							double dDistance,sPos_2D* Point_Result); //-- 利用分點公式找出所屬距離的座標位置
		bool  __fastcall GetCrossAnglePoint(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
											double dDistanceBase,double dCrossAngle,
											sPos_2D* Point_Result); //-- 取得相對角度的旋轉完座標
		//double  __fastcall GetCrossAngle_Direct_3Points(sPos_2D* Point_Base,sPos_2D* Point_Start,
		//										 sPos_2D* Point_Mid,sPos_2D* Point_End); //-- 找出角度含旋轉方向
		double  __fastcall GetCrossAngle_4Points(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
												 sPos_2D* Point_Base3,sPos_2D* Point_Base4,
												 bool &bCCW); //-- 利用內積公式找夾角
		//--- 2014.09.30 以後新增的函式  --
		bool __fastcall AnalogScaleCalculation(double In_Zero,double In_Span,   // 等比率 Analog 計算
											   double Out_Zero,double Out_Span,
											   double In_Value,double &dResult) ;
};
//---------------------------------------------------------------------------
#endif
