//---------------------------------------------------------------------------
//***************************************************************************
//** Create Date : 2010.10.13              Ver.:1.00_A       by Cuiu_Handsome
//** Function : �@�� Function �b���i��A�� TGentools ���~�Ӫ�
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
//-- �y�Ц�m�w�q Structure -------------------------------------------------
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
		void __fastcall VCLTMenuItemsProcess(String LevelSer,TMenuItem* ActMenuItem,TMemIniFile* ActMemIniFile); // �^�� TMenuItem ������W��
       	void   __fastcall CreatingFileList(TStringList* &P, AnsiString SubDir,
										 eDirActType ActType,String CondStr,
										 bool bSubDirInclude = false);
		String __fastcall DirGetFileName(TSearchRec FileName,eDirActType ActType,String CondStr);
	 protected:  //User declarations
		__fastcall  TBasetools(void);   // �N�غc���θѺc������p���ϩΫO�@��
		__fastcall  ~TBasetools(void);  // �T��~���{������New���󲣥�
	 public:
		//-- �ƾǹB��禡 ---------------------
		double __fastcall mod45(double modvalue, int bitnum);
		//--  �t�Τ�API�@�Ψ禡  -------------------------------------
		bool __fastcall SafeCreateSystem(AnsiString AppName,HWND hHandle);   // �קK�t�έ��ж}��
		void __fastcall SetSystemWorkArea(TForm* ActForm,int Width = 1024,int Height = 738);  // �t�ζ}����ܦ�m
		//-- �ɮ׳B�z�禡 --------------------------------------------
		String __fastcall FindToLastDirectory(String FileName);    // ���o�̩��h�����|�W��
		String __fastcall FindToLastFileName(String FileName);     // ���o���t���|���ɮצW��
		bool __fastcall CheckDirectory(String DirName);            // �ˬd���|�A���s�b�h�s�W
		void __fastcall CreateUniCodeFile(String FileName);        // ���� UniCode �榡���ɮ�
		void __fastcall TMemIniFile_DataRead(TMemIniFile* ActMemFile, AnsiString FileName); // �N�ɮ�Ū�JIMemIniFile
		void __fastcall SaveUniCodeFile(TMemIniFile* MemIniFile,String FileName);           // �N TMemIniFile ��Ʀs�J UniCode �����
		void __fastcall CreateFileList(AnsiString Dir,eDirActType ActType,String CondStr,TStringList* SList,
										 bool bSubDirInclude = false);
		//-- �y�t�B�z�禡 ---------------------------------------------
		void __fastcall GetVCLTMenuItemsData(TMenu* ActMenu,TMemIniFile* ActMemIniFile); // �^�� TMenu �U Menuitem������W��
		//---- ��Ʈw�B�zFUNCTION�]�t�����Ʈw�B��r�ɡBTSTRINGGRID�C�C�C���^------
		String __fastcall GetUniqueKey(void);
		//-- �y�Эp�� --------------------------------------------------------------
		bool __fastcall GetArcCenter_3Pos(sPos_2D* CenterResult,sPos_2D* CoPos1,
										  sPos_2D* CoPos2, sPos_2D* CoPos3);
		//-- �Ʀr�榡�ഫ / �ˬd -----------------------------------------------------------
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
		//-- �U�� CheckSum���p��覡 ----
		int  __fastcall GetCheckSum_XOR(AnsiString CheckData);
		//--- ��Ʈ榡�ഫ --------------
		String  __fastcall LongTypeToTimeType(long lTime);
		//--- �B�I���ˬd -----------------
		bool  __fastcall FloatDatatypeCheck(String ChkValue);
		//-- �v���y�ЩM�b�y�Шt���v��h �]�Ҧ��B�⧡�H ���y�Ь���h�^------------
		//-- 1. �v���y�Шt�û����� IV �H��
		//-- 2. �b�y�Шt���� I �H���� : X-> -X , Y -> Y , �c -> �c
		//-- 3. �b�y�Шt���� II �H���� : X-> X , Y -> Y , �c -> -�c
		//-- 4. �b�y�Шt���� III �H���� : X-> X , Y -> -Y , �c -> �c
		//-- 5. �b�y�Шt���� IV �H���� : X-> -X , Y -> -Y , �c -> -�c
		//-- �����y�Шt��m/���׭p��禡 -----
		double  __fastcall GetLineSlopAngle(sPos_2D* Line_Point1,sPos_2D* Line_Point2); //-- �ײv/����
		double  __fastcall GetCrossAngle_3Points(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
												 sPos_2D* Point_Base3,bool &bCCW); //-- �Q�Τ��n�����䧨��
		double  __fastcall GetCrossAngle_2Lines(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
												 sPos_2D* Point_Base3,sPos_2D* Point_Base4); //-- �Q�Τ��n�����䧨��
		bool  __fastcall GetLineSegmentPoint(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
							double dDistance,sPos_2D* Point_Result); //-- �Q�Τ��I������X���ݶZ�����y�Ц�m
		bool  __fastcall GetCrossAnglePoint(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
											double dDistanceBase,double dCrossAngle,
											sPos_2D* Point_Result); //-- ���o�۹﨤�ת����৹�y��
		//double  __fastcall GetCrossAngle_Direct_3Points(sPos_2D* Point_Base,sPos_2D* Point_Start,
		//										 sPos_2D* Point_Mid,sPos_2D* Point_End); //-- ��X���קt�����V
		double  __fastcall GetCrossAngle_4Points(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
												 sPos_2D* Point_Base3,sPos_2D* Point_Base4,
												 bool &bCCW); //-- �Q�Τ��n�����䧨��
		//--- 2014.09.30 �H��s�W���禡  --
		bool __fastcall AnalogScaleCalculation(double In_Zero,double In_Span,   // ����v Analog �p��
											   double Out_Zero,double Out_Span,
											   double In_Value,double &dResult) ;
};
//---------------------------------------------------------------------------
#endif
