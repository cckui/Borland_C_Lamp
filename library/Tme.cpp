//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Tme.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall Tme::Tme(TComponent *Owner): TComponent(Owner)
{
	FShapeLamp=new TShape(this);
	FShapeLamp->Parent = NULL ;
	FComboBox=new TComboBox(this);

	//--- 顯示執行緒宣告 ---------------------
	hMainCntShowHandle = CreateEvent(NULL,true,false,NULL);
	MainCntShowThread = new TModuleThread(hMainCntShowHandle,5,true);
	MainCntShowThread->OnSignal = DoMainCntShowEvent;
	MainCntShowThread->bContinueRun = false ;
	bMainCntShowThreadRunSet = true ;

	bMainShowClose = false ;
	hMainShowHandle = CreateEvent(NULL,true,false,NULL);
	MainShowThread = new TModuleThread(hMainShowHandle,5,false);
	MainShowThread->OnSignal = DoMainShowEvent;
	MainShowThread->bContinueRun = false ;

}
//---------------------------------------------------------------------------
__fastcall Tme::~Tme(void)
{
	//---- 關閉 顯示執行緒------------------------
	bMainShowClose = true ;
	while(!MainCntShowThread->GetThreadEnd()){
		Application->ProcessMessages();
	}
	while(!MainShowThread->GetThreadEnd()){
		Application->ProcessMessages();
	}
	MainShowThread->Terminate();
	SetEvent(hMainShowHandle);
}
//---------------------------------------------------------------------------



//***************************************************************************
//
//  				   Shape Component
//
//***************************************************************************
void __fastcall Tme::set_ShapeProperty(TWinControl *PanelParent,TShapeType Type,TColor PenColor,
									   TColor BrushColor,int PenWidth,int TShapeHeight,int TShapeWidth)
{
		FShapeLamp->Parent = PanelParent;
		FShapeLamp->Shape = Type;
		FShapeLamp->Pen->Color = PenColor;
		FShapeLamp->Brush->Color = BrushColor;
		FShapeLamp->Pen->Width = PenWidth;
		FShapeLamp->Height = TShapeHeight;
		FShapeLamp->Width = TShapeWidth;

}
//---------------------------------------------------------------------------
void __fastcall Tme::set_ShapePosition(int Top,int Left)
{
		FShapeLamp->Top = Top;
		FShapeLamp->Left = Left;
}
//---------------------------------------------------------------------------
void __fastcall Tme::set_ShapeColor(TColor Color)
{
		FShapeLamp->Brush->Color=Color;
}
//---------------------------------------------------------------------------



//***************************************************************************
//
//  				   ComboBox Component
//
//***************************************************************************
void __fastcall Tme::set_ComboBoxProperty(TWinControl *PanelParent,UnicodeString Text,int FontSize,int Left,int Top)
{
		FComboBox->Parent = PanelParent;
		FComboBox->Text = Text;
		FComboBox->Font->Size = FontSize;
		FComboBox->Left = Left;
		FComboBox->Top = Top;
}
//---------------------------------------------------------------------------
void __fastcall Tme::set_ComboBoxItem(TStrings *Item)
{
		FComboBox->Items = Item;
}
//---------------------------------------------------------------------------



//***************************************************************************
//
//  				   Thread Start and Stop
//
//***************************************************************************
//---------------------------------------------------------------------------
void __fastcall Tme::set_ThreadStasrt(void)
{
	MainShowThread->bContinueRun = true ;
	SetEvent(hMainShowHandle);
}
//---------------------------------------------------------------------------
void __fastcall Tme::set_ThreadStop(void)
{
	MainShowThread->bContinueRun = false ;
}
//---------------------------------------
//	DoMainCntShowEvent 控制畫面
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
//---------------------------------------------------------------------------
void __fastcall Tme::DoMainCntShowEvent(TObject* Sender)
{
	if ( Now_Lamp_Color==clGray ){
		this->set_ShapeColor(clGray);
		this->set_ThreadStop();
	}
	else if (Now_Lamp_Color==Lamp_Color)
		this->set_ShapeColor(Lamp_Color);
	else
		this->set_ShapeColor(clGray);

	//-----------------------------------------------------
	Application->ProcessMessages();
	bMainCntShowThreadRunSet = true ;
}
//---------------------------------------------------------------------------
void __fastcall Tme::DoMainShowEvent(TObject* Sender)
{

	int Now_Time = GetTickCount()- FSystemTimeBase;
	Now_Time=Now_Time/1000;

	if(Now_Time >= 0 && Now_Time < Green_Time)
		Now_Lamp_Color=clGreen;
	else if (Now_Time >= Green_Time && Now_Time < (Green_Time+Yellow_Time) )
		Now_Lamp_Color=clYellow;
	else if (Now_Time >= (Green_Time+Yellow_Time) && Now_Time < (Green_Time+Yellow_Time+Red_Time) )
		Now_Lamp_Color=clRed;
	else if (Now_Time>=(Green_Time+Yellow_Time+Red_Time))
		Now_Lamp_Color=clGray;

	//--------------------------------------
	if(MainCntShowThread->GetThreadEnd() && bMainCntShowThreadRunSet){
		bMainCntShowThreadRunSet = false ;
		SetEvent(hMainCntShowHandle);
	}
	else Sleep(0);
	//----------------------
	if(bMainShowClose){
	   MainShowThread->bContinueRun = false ;
	   bMainShowClose = false ;
	}
	else Sleep(0);
}
//---------------------------------------------------------------------------



//***************************************************************************
//
//  				   Lamp Start.Stop
//
//***************************************************************************
void __fastcall Tme::lamp_start(TColor color,int Red,int Yellow,int Green)
{
	Red_Time=Red;
	Yellow_Time=Yellow;
	Green_Time=Green;
	Lamp_Color=color;
	FSystemTimeBase = GetTickCount() ;
	this->set_ThreadStasrt();
}
//---------------------------------------------------------------------------
void __fastcall Tme::lamp_stop(void)
{
	this->set_ThreadStop();
}
//---------------------------------------------------------------------------
