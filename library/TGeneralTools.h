//---------------------------------------------------------------------------
//***************************************************************************
//** Create Date : 2010.10.13              Ver.:1.00_A       by Cuiu_Handsome
//** Function : 共用 Function ,主要繼承自 TBasetools
//***************************************************************************
//---------------------------------------------------------------------------
#ifndef TGeneralToolsH
#define TGeneralToolsH

#include "TBasetools.h"
//---------------------------------------------------------------------------
class TGeneralTools : public TBasetools
{
	private:
		static  TGeneralTools* _instance;  // 宣告一私有指標
	protected:  //User declarations
		__fastcall  TGeneralTools(void);   // 將建構式及解構式移到私有區或保護區
		__fastcall  ~TGeneralTools(void);  // 禁止外部程式直接New物件產生
	public:
		static TGeneralTools*   __fastcall getInstance();       // 提供外部一個方法Get物件實體
		static void         	__fastcall delInstance();       // 提供外部一個方法Delete物件實體
};
//---------------------------------------------------------------------------
#endif
