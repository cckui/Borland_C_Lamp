//---------------------------------------------------------------------------
#include <vcl.h>
#include "TBasetools.h"
//---------------------------------------------------------------------------
#pragma hdrstop
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TBasetools::TBasetools(void)
{

}
//---------------------------------------------------------------------------
__fastcall TBasetools::~TBasetools(void)
{

}
//---------------------------------------------------------------------------
double __fastcall TBasetools::mod45(double modvalue, int bitnum)
{
	double z = IntPower(10.0, (double)bitnum);
	double rtvalue = 0;
	modf(modvalue*z+0.5, &rtvalue);
	rtvalue = rtvalue/z;
	return rtvalue;
}
//---------------------------------------------------------------------------
bool __fastcall TBasetools::SafeCreateSystem(AnsiString AppName,HWND hHandle)
{
/*	bool bRet = true ;
	HANDLE Mutex = OpenMutex(SYNCHRONIZE, false, AppName.c_str());
	if(Mutex != NULL){
	   HWND PrevInstHandle;
	   AnsiString AppSysName = AnsiString("T")+ AppName ;
	   AnsiString tmpName = AnsiString("NewApp");
	   SetWindowText(hHandle, NULL);
	   PrevInstHandle = FindWindow(AppSysName.c_str(), AppName.c_str());
	   if(PrevInstHandle != 0){
		  if(IsIconic(PrevInstHandle) != 0){
			 ShowWindow(PrevInstHandle, SW_RESTORE);
		  }
		  else{
			 BringWindowToTop(PrevInstHandle);
          }
	   }
	   SetForegroundWindow(PrevInstHandle);
	   bRet = false ;
	   //Application->ShowMainForm = false;
	   //Application->Terminate();
	}
	else{
	   CreateMutex(NULL, false, AppName.c_str());
	}
	return bRet ;  */
}
//---------------------------------------------------------------------------
void __fastcall TBasetools::SetSystemWorkArea(TForm* ActForm,int Width,int Height)
{
	ActForm->Width = Width ;
	ActForm->Height = Height ;
	if ((Screen->WorkAreaHeight == Width) && (Screen->WorkAreaHeight == Height)){
		ActForm->WindowState = wsMaximized;
	}
	else{
		ActForm->Left = Screen->WorkAreaWidth/2 - Width/2 ;
		ActForm->Top = Screen->WorkAreaHeight/2 - Height/2 ;
		ActForm->Align = alNone;
		ActForm->WindowState = wsNormal;
	}
	ActForm->Scaled = false ;            //-- 避免字型改變
	ActForm->BorderStyle =  bsSingle ;   //-- 避免系統視窗大小被改變
	//-- 放大/縮小/關閉全不見，縮小鍵系統自行處理 ---
	TBorderIcons tempBI = ActForm->BorderIcons;
	tempBI >> biSystemMenu	;
	ActForm->BorderIcons = tempBI;
}
//---------------------------------------------------------------------------
String __fastcall TBasetools::FindToLastDirectory(String FileName)
{
	String sStr = String("") ;
	int tmpPos ;
	while(FileName.Pos("\\")){
	   tmpPos = FileName.Pos("\\") ;
	   sStr = sStr + FileName.SubString(1,tmpPos) ;
	   FileName = FileName.SubString(tmpPos+1,FileName.Length()-tmpPos) ;
	}
	return sStr ;
}
//----------------------------------------------------------------------------
String __fastcall TBasetools::FindToLastFileName(String FileName)
{
    int tmpPos ;
    while(FileName.Pos("\\")){
       tmpPos = FileName.Pos("\\") ;
       FileName = FileName.SubString(tmpPos+1,FileName.Length()-tmpPos) ;
    }
    return FileName ;
}
//---------------------------------------------------------------------------
String __fastcall TBasetools::DirGetFileName(TSearchRec FileName,eDirActType ActType,String CondStr)
{
   String sRet = String("");
   switch(ActType) {
      case eDirNone   : { sRet = FileName.Name ;
                        } break ;
	  case eDirTimeEQLE :  case eDirTimeEQLG :  case eDirTimeLG :  case eDirTimeLE :
	  case eDirTimeEQ : { Word Year,Month,Day;
						  TDateTime dtPresent ;
						  #ifdef Application_Ver_BCB2010
						  dtPresent= FileDateToDateTime(FileName.Time);//    .Time);
						  #else
						  dtPresent= FileDateToDateTime(FileName.TimeStamp);
						  #endif
						  DecodeDate(dtPresent,Year,Month,Day);
                          String cmpstr = IntToStr(Year+10000).SubString(2,4)+
                                          IntToStr(Month+100).SubString(2,2) +
                                          IntToStr(Day+100).SubString(2,2)   ;
                          bool bAdd = false ;
                          if(ActType == eDirTimeEQ && cmpstr == CondStr)
                             bAdd = true ;
                          else if(ActType == eDirTimeLG && cmpstr > CondStr)
                             bAdd = true ;
                          else if(ActType == eDirTimeLE && cmpstr < CondStr)
                             bAdd = true ;
                          else if(ActType == eDirTimeEQLG && cmpstr >= CondStr)
                             bAdd = true ;
                          else if(ActType == eDirTimeEQLE && cmpstr <= CondStr)
                             bAdd = true ;
                          if(bAdd)  sRet = FileName.Name ;
                        } break ;
      case eDirExtName: { CondStr = String(".")+ CondStr.UpperCase() ;
                          if(FileName.Name.UpperCase().Pos(CondStr))
                             sRet = FileName.Name ;
                        } break ;
      case eDirIncd   : { CondStr = CondStr.UpperCase() ;
                          if(FileName.Name.UpperCase().Pos(CondStr))
                             sRet = FileName.Name ;
                        } break ;
   }
   return sRet;
}
//---------------------------------------------------------------------------
void __fastcall TBasetools::CreatingFileList(TStringList* &P, AnsiString SubDir,
							 eDirActType ActType,String CondStr,bool bSubDirInclude)
{
	TSearchRec F;
	if(FindFirst(SubDir + "\\*.*", faAnyFile, F)==0){
	   do{
		  if(F.Name!="." && F.Name!=".."){
			 if((F.Attr & faDirectory)==faDirectory)
				if(bSubDirInclude)
					CreatingFileList(P, SubDir + F.Name +"\\" ,ActType,CondStr);
				else Sleep(0);
			 else{
				String tmpfilename = DirGetFileName(F,ActType,CondStr);
				if(!tmpfilename.Trim().IsEmpty()){
				   //tmpfilename = SubDir + "\\" + tmpfilename ;
				   tmpfilename = SubDir + tmpfilename ;
				   P->Add(tmpfilename);
				}
			 }
          }
          Application->ProcessMessages();
       } while(FindNext(F)==0);
       FindClose(F);
    }
}
//---------------------------------------------------------------------------
void __fastcall TBasetools::CreateFileList(AnsiString Dir,eDirActType ActType,
												   String CondStr,TStringList* SList,
												   bool bSubDirInclude)
{
	String ActSubDir = Dir ;
	int length = ActSubDir.Length();
    if(ActSubDir.SubString(length,1)=="\\")
	   ActSubDir = ActSubDir.SubString(1,length-1);
	SList->Clear() ;
	CreatingFileList(SList,Dir,ActType,CondStr,bSubDirInclude);
	//return P;
}
//---------------------------------------------------------------------------
bool __fastcall TBasetools::CheckDirectory(String DirName)
{
	bool bRet = true;
	if(!ForceDirectories(DirName)){
	   bRet = false;
	}
	return bRet;
}
//----------------------------------------------------------------------------
void __fastcall TBasetools::CreateUniCodeFile(String FileName)
{
	String LastPath = FindToLastDirectory(FileName);
	CheckDirectory(LastPath) ;
	TStringList* tmpSList = new TStringList();
	tmpSList->Clear();
	tmpSList->SaveToFile(FileName,TEncoding::UTF8) ;
	delete tmpSList ;
}
//----------------------------------------------------------------------------
void __fastcall TBasetools::TMemIniFile_DataRead(TMemIniFile* ActMemFile, AnsiString FileName)
{
	//-- ！！ 此函式僅適用於在每一 Section 區內的 TagName 資料欄位均相同
	TStringList* tmpSList = new TStringList();
	tmpSList->Clear() ;
	tmpSList->LoadFromFile(FileName) ;
	// 把 TStringList 的內容塞到 TMemIniFile (會自動清掉 IniMemory 的 data)
	ActMemFile->SetStrings(tmpSList);
	tmpSList->Clear() ;
	delete tmpSList ;
}
//----------------------------------------------------------------------------
void __fastcall TBasetools::SaveUniCodeFile(TMemIniFile* MemIniFile,String FileName)
{
	String LastPath = FindToLastDirectory(FileName);
	if(LastPath.Trim().IsEmpty())
	   LastPath = ExtractFilePath(Application->ExeName)+ "DefineData\\";
	CheckDirectory(LastPath) ;
	TStringList* tmpSList = new TStringList();
	tmpSList->Clear();
	MemIniFile->GetStrings(tmpSList);
	tmpSList->SaveToFile(FileName,TEncoding::UTF8);
	tmpSList->Clear() ;
	delete tmpSList ;
}
//----------------------------------------------------------------------------
void __fastcall TBasetools::VCLTMenuItemsProcess(String LevelSer,TMenuItem* ActMenuItem,TMemIniFile* ActMemIniFile)
{
	int ItemsCnt = ActMenuItem->Count ;
	ActMemIniFile->WriteInteger(LevelSer,"Count",ItemsCnt) ;
    int length = LevelSer.Length() ;
	String SectName = LevelSer.SubString(1,length-4) ;
	String TagName = LevelSer.SubString(length-3,4) ;
	ActMemIniFile->WriteString(SectName,TagName,ActMenuItem->Name) ;
	if(ItemsCnt > 0){
		for(int i=0;i<ItemsCnt;i++){
			TMenuItem* tmpMenuItem = (TMenuItem*)ActMenuItem->Items[i] ;
			String tmpLevelSer = LevelSer + IntToStr(i+10000).SubString(2,4);
			VCLTMenuItemsProcess(tmpLevelSer,tmpMenuItem,ActMemIniFile) ;   // 遞迴函式
		}
	}
}
//----------------------------------------------------------------------------
void __fastcall TBasetools::GetVCLTMenuItemsData(TMenu* ActMenu,TMemIniFile* ActMemIniFile)
{
	ActMemIniFile->Clear() ;
	TMenuItem *FMainItems = static_cast<TMenuItem*>( GetObjectProp(ActMenu, "Items") );
	String Level = String("0000");
	ActMemIniFile->WriteInteger(Level,"Count",FMainItems->Count) ;
	for(int i=0;i<FMainItems->Count;i++){
		TMenuItem* ActMenuItem = (TMenuItem*)FMainItems->Items[i] ;
		Level = String("0000")+IntToStr(i+10000).SubString(2,4);
		VCLTMenuItemsProcess(Level,ActMenuItem,ActMemIniFile);
	}
}
//---------------------------------------------------------------------------
String __fastcall TBasetools::GetUniqueKey(void)
{  String retval;
   Word Year,Month,Day,Hour,Min,Sec,MSec;

   TDateTime dtPresent=Now();
   DecodeDate(dtPresent,Year,Month,Day);
   DecodeTime(dtPresent,Hour,Min,Sec,MSec) ;
   retval =IntToStr(Year+10000).SubString(2,4)+
           IntToStr(Month+100).SubString(2,2) +
           IntToStr(Day+100).SubString(2,2)   +
           IntToStr(Hour+100).SubString(2,2)  +
           IntToStr(Min+100).SubString(2,2)   +
           IntToStr(Sec+100).SubString(2,2)   +
           IntToStr(MSec+1000).SubString(2,3) ;
   return retval;
}
//---------------------------------------------------------------------------
bool __fastcall TBasetools::GetArcCenter_3Pos(sPos_2D* CenterResult,sPos_2D* CoPos1,
											  sPos_2D* CoPos2, sPos_2D* CoPos3)
{
	bool bRet = true ;
	double x1 = CoPos1->_Pos1 ;   	double y1 = CoPos1->_Pos2 ;
	double x2 = CoPos2->_Pos1 ;     double y2 = CoPos2->_Pos2 ;
	double x3 = CoPos3->_Pos1 ;     double y3 = CoPos3->_Pos2 ;

	//-- 二條直線的長度 -------------------------------
	double dx1 = x2-x1 ;                 double dy1 = y2-y1 ;
	double dx2 = x3-x2 ;                 double dy2 = y3-y2 ;
	double L1 = sqrt(dx1*dx1+dy1*dy1);
	double L2 = sqrt(dx2*dx2+dy2*dy2);
	double X1c = (x1+x2)/2 ;     double Y1c = (y1+y2)/2 ;
	double X2c = (x3+x2)/2 ;     double Y2c = (y3+y2)/2 ;
	double Xc ,Yc ; //-- 圓心座標 ---
	if(L1==0 || L2==0)  bRet = false ;
	else{
		if(dy1!=0 && dy2!=0){
			double L1m = -(dx1/dy1);     double L2m = -(dx2/dy2); // 二條直線之中垂線斜率
			//-- 中垂線方程式：(1) Y=L1m*(X-X1c)+Y1c   (2) Y=L2m*(X-X2c)+Y2c
			Xc = (L1m*X1c - Y1c + Y2c -L2m*X2c)/(L1m-L2m) ;  // L1m 三點不可能相等
			Yc = L1m*(Xc-X1c) + Y1c ;
		}
		else if(dy1==0 && dy2!=0){  //-- 第一條為水平線
			Xc = X1c ;
			double L2m = -(dx2/dy2);
			Yc = Y2c+ L2m*(Xc-X2c);
		}
		else if(dy1!=0 && dy2==0){  //-- 第二條為水平線
			Xc = X2c ;
			double L1m = -(dx1/dy1);
			Yc = Y1c+ L1m*(Xc-X2c);
		}
		else bRet = false ;
	}
	if(bRet){
		CenterResult->_Pos1 = Xc ;
        CenterResult->_Pos2 = Yc ;
	}
	return bRet ;
}
//---------------------------------------------------------------------------
String __fastcall TBasetools::Int_Hex(int TransValue,int ByteNum)
{
	char temp[16] ;
	sprintf(temp,"%X",TransValue);
	String sRet = temp;
	String tempStr ;
	for(int i=0;i<ByteNum*2;i++)
		tempStr = tempStr.Trim()+AnsiString("0");
	sRet = tempStr + sRet ;
	int length = sRet.Length() ;
	return(sRet.SubString(length-ByteNum*2+1,ByteNum*2));
}
//---------------------------------------------------------------------------
String __fastcall TBasetools::Int_Hex_16(int TransValue)
{
	char temp[4] ;
	sprintf(temp,"%X",TransValue);
	String sRet = temp;
	sRet = String("0000")+sRet.Trim();
	int length = sRet.Length() ;
	return(sRet.SubString(length-3,4));
}
//---------------------------------------------------------------------------
String __fastcall TBasetools::Int_Hex_32(int TransValue)
{
	char temp[8] ;
	sprintf(temp,"%X",TransValue);
	String sRet = temp;
	sRet = String("00000000")+sRet.Trim();
	int length = sRet.Length() ;
	return(sRet.SubString(length-7,8));
}
//---------------------------------------------------------------------------
String __fastcall TBasetools::Int_Hex_64(int TransValue)
{
	char temp[16] ;
	sprintf(temp,"%X",TransValue);
	String sRet = temp;
	sRet = String("0000000000000000")+sRet.Trim();
	int length = sRet.Length() ;
	return(sRet.SubString(length-15,16));
}
//---------------------------------------------------------------------------
String __fastcall TBasetools::Int_Bin(int TransValue)
{
	String sRet = String("");
	while(true){
		if((TransValue/2)>=2){
			sRet = IntToStr((TransValue%2))+ sRet ;
            TransValue = TransValue/2 ;
		}
		else{
			sRet = IntToStr((TransValue/2))+ IntToStr((TransValue%2))+
				   sRet ;
			break ;
		}
	}
	return sRet ;
}
//---------------------------------------------------------------------------
int __fastcall TBasetools::Bin_Int(String TransValue)
{
	int iRet = 0 ;
	int length = TransValue.Length();
	for(int i=0;i<length;i++){
		int ActValue = TransValue.SubString(length-i,1).ToInt();
		//ActValue = ActValue * IntPower(2,i);
		ActValue = (ActValue << i) ;
		iRet = iRet + ActValue ;
	}
	return iRet ;
}
//---------------------------------------------------------------------------
bool __fastcall TBasetools::String_Int(String TransStr,int &RetValue)
{
	if(TransStr.Trim().IsEmpty())  return false ;
	else Sleep(0);
	//-------------
	char* tmpChar = new char[1];
	int length = TransStr.Length() ;
	int iDotCount = 0 ;
	bool bRet = true ;
	//-- 第一碼正負判斷(-/數字) ----
	String actStr = TransStr.SubString(1,1) ;
	sprintf(tmpChar,"%s", actStr);
	int JustASCII = (int)tmpChar[0] ;
	bool bTrue = (JustASCII == 45) ||
				 (JustASCII>=48 && JustASCII<=57) ;
	if(!bTrue) return false ;
	else Sleep(0);
	if(length == 1){
		if(JustASCII < 48 || JustASCII> 57)  //單一碼不是數字
			return false ;
		else Sleep(0);
	}
	else Sleep(0);
	//-- Loop 判斷 ---------
	for(int i=1;i<length;i++){
		actStr = TransStr.SubString((i+1),1) ;
		sprintf(tmpChar,"%s", actStr);
		JustASCII = (int)tmpChar[0] ;
		if(JustASCII<48 || JustASCII>57){   //-- 不是數字
			bRet = false ;
		}
		else Sleep(0);

		if(!bRet)  break;
		else Sleep(0);
	}
	if(bRet){
		RetValue = TransStr.ToDouble() ;
	}
	else Sleep(0);
	delete [] tmpChar ;
	return bRet ;
}
//---------------------------------------------------------------------------
bool __fastcall TBasetools::String_Double(String TransStr,double &RetValue)
{
	if(TransStr.Trim().IsEmpty())  return false ;
	else Sleep(0);
	//-------------
	char* tmpChar = new char[1];
	int length = TransStr.Length() ;
	int iDotCount = 0 ;
	bool bRet = true ;
	//-- 第一碼正負判斷(-/./數字) ----
	String actStr = TransStr.SubString(1,1) ;
	sprintf(tmpChar,"%s", actStr);
	int JustASCII = (int)tmpChar[0] ;
	bool bTrue = (JustASCII == 45) || (JustASCII == 46) ||
				 (JustASCII>=48 && JustASCII<=57) ;
	if(!bTrue) return false ;
	else Sleep(0);
	if(length == 1){
		if(JustASCII < 48 || JustASCII> 57)  //單一碼不是數字
			return false ;
		else Sleep(0);
	}
	else Sleep(0);
	if(JustASCII == 46) iDotCount++ ;
	else Sleep(0);
	//-- Loop 判斷 ---------
	for(int i=1;i<length;i++){
		actStr = TransStr.SubString((i+1),1) ;
		sprintf(tmpChar,"%s", actStr);
		JustASCII = (int)tmpChar[0] ;
		if(JustASCII<48 || JustASCII>57){   //-- 不是數字
			if(JustASCII == 46){            //-- 不是 " . "
				iDotCount++ ;
				if(iDotCount>=2)
					bRet = false ;
				else Sleep(0);
			}
			else bRet = false ;
		}
		else Sleep(0);

		if(!bRet)  break;
		else Sleep(0);
	}
	if(bRet){
		RetValue = TransStr.ToDouble() ;
	}
	else Sleep(0);
	delete [] tmpChar ;
	return bRet ;
}
//---------------------------------------------------------------------------
void __fastcall TBasetools::GetMainFormBasePos(TControl* ActControl,String BaseFormName,int &Left,int &Top)
{
	int BaseTop	 = 0 ;
	int BaseLeft = 0 ;
	TControl* ParentControl = ActControl->Parent ;
	while(ParentControl->Name != BaseFormName){
		BaseTop += ParentControl->Top ;
		BaseLeft += ParentControl->Left ;
		ParentControl = ParentControl->Parent ;
	}
	Top = BaseTop + ParentControl->Top ;
	Left = BaseLeft + ParentControl->Left ;
}
//----------------------------------------------------------------------------
int __fastcall TBasetools::GetCheckSum_XOR(AnsiString CheckData)
{
	int iRet = 0;
	long Datalength = CheckData.Length();
	char* CaculateData = new char [Datalength+2];
	strcpy(CaculateData, CheckData.c_str());
	iRet = CaculateData[0];
	for(int i=1;i<Datalength;i++){
		iRet = iRet ^ CaculateData[i];
	}

	delete[] CaculateData;
	return iRet ;
}
//----------------------------------------------------------------------------
bool __fastcall TBasetools::DataTypeCheck_Numeral(String Context, String &MSG , int LanguageType , bool bIntCheck)
{
	bool bRet = true ;
	if(!bIntCheck){    //-- 浮點數判斷 ---
		int iPos = Context.Pos(".") ;
		int Length = Context.Length();
		if(iPos>0){
			Context = Context.SubString(1,iPos-1) + Context.SubString(iPos+1,Length-iPos);
		}
	}
	else Sleep(0);
	if(Context.Trim().ToIntDef(-1) < 0){
		if(LanguageType == 0)
			MSG = String(L"控制序號需為浮點數格式，\r\n請重新輸入！");
		else if(LanguageType == 1)
			MSG = String(L"控制序号需为浮点数格式，\r\n请重新输入！");
		else if(LanguageType == 2)
			MSG = String("Ser No. should be positive float type,\r\n Kindly Retry again !");
		bRet =  false ;
	}
	else Sleep(0);

	return bRet ;
}
//---------------------------------------------------------------------------
String __fastcall TBasetools::LongTypeToTimeType(long lTime)
{
	String sRet ;
	int mSec = lTime %1000 ;
	int TolSec =  lTime /1000 ;
	int Sec = TolSec % 60 ;
	int TolMin = TolSec / 60 ;
	int Min = TolMin % 60 ;
	int Hour = TolMin / 60 ;
	sRet = IntToStr(Hour)+String(":")+
		   IntToStr(Min+100).SubString(2,2)+String(":")+
		   IntToStr(Sec+100).SubString(2,2)	;
	return sRet ;
}
//---------------------------------------------------------------------------
bool __fastcall TBasetools::FloatDatatypeCheck(String ChkValue)
{
	int pos = ChkValue.Pos(".");
	if(pos>0){
		int length = ChkValue.Length();
		if(pos == 1) ChkValue = ChkValue.SubString(2,length-1);
		else{
			ChkValue = ChkValue.SubString(1,pos-1) + ChkValue.SubString(pos+1,length-pos);
		}
	}
	else Sleep(0);

	return (ChkValue.Trim().ToIntDef(-99999)!=-99999);
}
//---------------------------------------------------------------------------
double __fastcall TBasetools::GetLineSlopAngle(sPos_2D* Line_Point1,sPos_2D* Line_Point2)
{
    //--- Pos_2D 為基準點
	double dRet ;
	double dVector_X = Line_Point1->_Pos1 - Line_Point2->_Pos1 ;
	double dVector_Y = Line_Point1->_Pos2 - Line_Point2->_Pos2 ;

	//-- 判斷無意義的值，角度為 90 ----
	if(dVector_X == 0){
		dRet = (dVector_Y > 0) ? 90 :  -90 ;
	}
	if(dVector_Y == 0){
		dRet = (dVector_X > 0) ? 0 :  180 ; //0 ;
	}
	else{
		//double dDistance = sqrt((dVector_X * dVector_X)+ (dVector_Y * dVector_Y));
		dRet = atan(dVector_Y/dVector_X) * (180 / Math_Pi);
		//--- 象限判斷 --
		if((dVector_X<0 && dVector_Y>0) || (dVector_X<0 && dVector_Y<0)){  //- 第二象限角度為負 , 第三象限角度為正
			dRet = dRet + 180 ;
		}
		//else if(dVector_X<0 && dVector_Y<0){
		//	dRet = dRet + 180 ;
		//}
		else Sleep(0);

	}
	return dRet ;
}
//---------------------------------------------------------------------------
double __fastcall TBasetools::GetCrossAngle_3Points(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
													sPos_2D* Point_Base3,bool &bCCW)
{
	//-- 以 Point_Base1 為基準找出兩向量的 X 及 Y 分量 -----
	double dVector1_X = Point_Base2->_Pos1 - Point_Base1->_Pos1 ;
	double dVector1_Y = Point_Base2->_Pos2 - Point_Base1->_Pos2 ;
	double dVector2_X = Point_Base3->_Pos1 - Point_Base1->_Pos1 ;
	double dVector2_Y = Point_Base3->_Pos2 - Point_Base1->_Pos2 ;

	double dDistance_1 = sqrt((dVector1_X * dVector1_X)+ (dVector1_Y * dVector1_Y));
	double dDistance_2 = sqrt((dVector2_X * dVector2_X)+ (dVector2_Y * dVector2_Y));

	dVector1_X = dVector1_X/dDistance_1 ; //單位向量
	dVector1_Y = dVector1_Y/dDistance_1 ;
	dVector2_X = dVector2_X/dDistance_2 ;
	dVector2_Y = dVector2_Y/dDistance_2 ;

	double dDotVectorValue = dVector1_X * dVector2_X + dVector1_Y * dVector2_Y ;
	double dAngle  = acos(dDotVectorValue) ;
	double dCrossVectorValue = dVector1_X * dVector2_Y - dVector2_X * dVector1_Y ;
	dAngle = (dCrossVectorValue<0) ? Math_Pi-dAngle : dAngle ;
	bCCW = (dCrossVectorValue>=0);
	return dAngle * (180 / Math_Pi) ;  //-- 角度由弳度換算為角度 --------

	/*double dDotVectorValue = dVector1_X * dVector2_X + dVector1_Y * dVector2_Y ;
	double dAngle = acos(dDotVectorValue/(dDistance_Base*dDistance_Ref)) ;
	if(fabs(dDotVectorValue-1.0))
	return dCrossCita * (180 / Math_Pi) ; */ //-- 角度由弳度換算為角度 -------- */
}
//---------------------------------------------------------------------------
double __fastcall TBasetools::GetCrossAngle_4Points(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
												 sPos_2D* Point_Base3,sPos_2D* Point_Base4,
												 bool &bCCW)
{
	//-- 以 Point_Base1 為基準找出兩向量的 X 及 Y 分量 -----
	double dVector1_X = Point_Base2->_Pos1 - Point_Base1->_Pos1 ;
	double dVector1_Y = Point_Base2->_Pos2 - Point_Base1->_Pos2 ;
	double dVector2_X = Point_Base3->_Pos1 - Point_Base1->_Pos1 ;
	double dVector2_Y = Point_Base3->_Pos2 - Point_Base1->_Pos2 ;
	double dVector3_X = Point_Base4->_Pos1 - Point_Base1->_Pos1 ;
	double dVector3_Y = Point_Base4->_Pos2 - Point_Base1->_Pos2 ;

	double dDistance_1 = sqrt((dVector1_X * dVector1_X)+ (dVector1_Y * dVector1_Y));
	double dDistance_2 = sqrt((dVector2_X * dVector2_X)+ (dVector2_Y * dVector2_Y));
	double dDistance_3 = sqrt((dVector3_X * dVector3_X)+ (dVector3_Y * dVector3_Y));

	dVector1_X = dVector1_X/dDistance_1 ; //單位向量
	dVector1_Y = dVector1_Y/dDistance_1 ;
	dVector2_X = dVector2_X/dDistance_2 ;
	dVector2_Y = dVector2_Y/dDistance_2 ;
	dVector3_X = dVector3_X/dDistance_3 ;
	dVector3_Y = dVector3_Y/dDistance_3 ;

	double dDotVectorValue = dVector1_X * dVector3_X + dVector1_Y * dVector3_Y ;
	double dAngle  = acos(dDotVectorValue) ;

	double dCrossVectorValue1 = dVector1_X * dVector2_Y - dVector2_X * dVector1_Y ;
	double dCrossVectorValue2 = dVector1_X * dVector3_Y - dVector3_X * dVector1_Y ;
	double bJustValue = dCrossVectorValue1* dCrossVectorValue2 ;

	if(bJustValue>=0){  //-- 同方向
		dAngle = (dCrossVectorValue2<0) ? Math_Pi-dAngle : dAngle ;
		bCCW = (dCrossVectorValue2>=0);
	}
	else{
		dAngle = 2* Math_Pi-dAngle ;
		bCCW = (dCrossVectorValue2<=0);
	}
	return dAngle * (180 / Math_Pi) ;  //-- 角度由弳度換算為角度 --------
}
//---------------------------------------------------------------------------
double __fastcall TBasetools::GetCrossAngle_2Lines(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
													sPos_2D* Point_Base3,sPos_2D* Point_Base4)
{
	//-- 以 Point_Base1 為基準找出兩向量的 X 及 Y 分量 -----
	double dVector1_X = Point_Base2->_Pos1 - Point_Base1->_Pos1 ;
	double dVector1_Y = Point_Base2->_Pos2 - Point_Base1->_Pos2 ;
	double dVector2_X = Point_Base4->_Pos1 - Point_Base3->_Pos1 ;
	double dVector2_Y = Point_Base4->_Pos2 - Point_Base3->_Pos2 ;

	double dDistance_Base = sqrt((dVector1_X * dVector1_X)+ (dVector1_Y * dVector1_Y));
	double dDistance_Ref = sqrt((dVector2_X * dVector2_X)+ (dVector2_Y * dVector2_Y));

	double dDotVectorValue = dVector1_X * dVector2_X + dVector1_Y * dVector2_Y ;
	double dCrossCita = acos(dDotVectorValue/(dDistance_Base*dDistance_Ref)) ;
	dCrossCita = dCrossCita * (180 / Math_Pi) ;
	return dCrossCita ;  //-- 角度由弳度換算為角度 --------
}
//---------------------------------------------------------------------------
bool __fastcall TBasetools::GetLineSegmentPoint(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
												double dDistance,sPos_2D* Point_Result)
{
	//-- BASE 線不可為點 -----
	double dVector1_X = Point_Base2->_Pos1 - Point_Base1->_Pos1 ;
	double dVector1_Y = Point_Base2->_Pos2 - Point_Base1->_Pos2 ;
	double dDistance_Base = sqrt((dVector1_X * dVector1_X)+ (dVector1_Y * dVector1_Y));
	bool bRet = (dDistance_Base > 0) ;
	if(!bRet) return bRet  ;
	else Sleep(0);
	//--- 判斷使用內分點或外分點公式 ---
	double m , n ;
	if(dDistance_Base > dDistance){  //-- 內分點公式
		// X = (m x X1 + n x X2) / (m+n)  , Y = (m x Y2 + n x Y1) / (m+n)
		m = dDistance ;
		n = dDistance_Base - dDistance ;
		Point_Result->_Pos1 =  (m * Point_Base2->_Pos1 + n * Point_Base1->_Pos1)/(m+n) ;
		Point_Result->_Pos2 =  (m * Point_Base2->_Pos2 + n * Point_Base1->_Pos2)/(m+n) ;
	}
	else{  //-- 外分點公式
		// X = (m x X2 - n x X1) / (m-n)  , Y = (m x Y2 - n x Y1) / (m-n)
		m = dDistance ;
		n = dDistance - dDistance_Base  ;
		Point_Result->_Pos1 =  (m * Point_Base2->_Pos1 - n * Point_Base1->_Pos1)/(m-n) ;
		Point_Result->_Pos2 =  (m * Point_Base2->_Pos2 - n * Point_Base1->_Pos2)/(m-n) ;
	}
	return bRet ;
}
//---------------------------------------------------------------------------
bool __fastcall TBasetools::GetCrossAnglePoint(sPos_2D* Point_Base1,sPos_2D* Point_Base2,
						    double dDistanceBase,double dCrossAngle,
						    sPos_2D* Point_Result)
{
	//--- 取得兩點斜率的角度 -----
	double dSlopeAngleBase = GetLineSlopAngle(Point_Base1, Point_Base2);
	double RealCita = dSlopeAngleBase - dCrossAngle ; //-- 扣掉夾角的角度
    //-- 計算實際點座標
	Point_Result->_Pos1 =  Point_Base1->_Pos1 + dDistanceBase * cos(RealCita) ;
	Point_Result->_Pos2 =  Point_Base1->_Pos2 + dDistanceBase * sin(RealCita) ;
}
//--- 2014.09.30 以後新增的函式  --
//---------------------------------------------------------------------------
bool __fastcall TBasetools::AnalogScaleCalculation(double In_Zero,double In_Span,   // 等比率 Analog 計算
												   double Out_Zero,double Out_Span,
												   double In_Value,double &dResult)
{
	bool bRet = ((In_Span - In_Zero) != 0.0) ;
	if(bRet){
		double dSlope = (Out_Span - Out_Zero)/(In_Span - In_Zero) ;
		dResult = Out_Zero + dSlope*(In_Value - In_Zero) ;
	}
	else Sleep(0);
	return bRet ;
}
//----------------------------------------------------------------------------