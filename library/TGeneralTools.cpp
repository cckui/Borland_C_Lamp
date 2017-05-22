//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TGeneralTools.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TGeneralTools* TGeneralTools::_instance = 0;  // Instanceªì©l¤Æ
//---------------------------------------------------------------------------
__fastcall TGeneralTools::TGeneralTools(void)
{

}
//---------------------------------------------------------------------------
__fastcall TGeneralTools::~TGeneralTools(void)
{

}
//----------------------------------------------------------------------------
TGeneralTools* __fastcall TGeneralTools::getInstance()
{
    if (_instance == 0){
        _instance = new TGeneralTools();
	}
    return _instance ;
}
//----------------------------------------------------------------------------
void __fastcall TGeneralTools::delInstance()
{
    if (_instance != 0){
       delete _instance;
       _instance = 0;
    }
}
//----------------------------------------------------------------------------
