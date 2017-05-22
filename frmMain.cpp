//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmMain.h"
#include "Tme.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
int flag=0;
int count_Red=0 ;
int count_Yellow=0;
int count_Green=0;
//---------------------------------------------------------------------------



//***************************************************************************
//
//  				   Init
//
//***************************************************************************
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	FLampRed = new Tme(this);
	FLampGreen = new Tme(this);
	FLampYellow = new Tme(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormActivate(TObject *Sender)
{
	FLampRed->set_ShapeProperty(PanelThread,stCircle,clBlue,clRed,5,130,130);
	FLampRed->set_ShapePosition(15,30);

	FLampYellow->set_ShapeProperty(PanelThread,stCircle,clBlue,clYellow,5,130,130);
	FLampYellow->set_ShapePosition(15,30+160);

	FLampGreen->set_ShapeProperty(PanelThread,stCircle,clBlue,clGreen,5,130,130);
	FLampGreen->set_ShapePosition(15,30+320);

}
//---------------------------------------------------------------------------



//***************************************************************************
//
//  				   Timer
//
//***************************************************************************
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{

	if (count_Red + count_Yellow + count_Green > 0) {
		
		if (flag==1 && count_Green!=0) {
			 count_Green -=1;
			 this->TShapeTimerRed->Brush->Color= clGray;
			 this->TShapeTimerYellow->Brush->Color= clGray;
			 this->TShapeTimerGreen->Brush->Color= clGreen;
			 if (count_Green==0) {
				flag +=1;	 
			 }
		}else if (flag==2 && count_Yellow!=0) {
			count_Yellow -=1;
			this->TShapeTimerRed->Brush->Color= clGray;
			this->TShapeTimerYellow->Brush->Color= clYellow;
			this->TShapeTimerGreen->Brush->Color= clGray;
			 if (count_Yellow==0) {
				flag +=1;	 
			 }
		}else if(flag==3 && count_Red!=0) {
			count_Red -=1;
			this->TShapeTimerRed->Brush->Color= clRed;
			this->TShapeTimerYellow->Brush->Color= clGray;
			this->TShapeTimerGreen->Brush->Color= clGray;
			 if (count_Red==0) {
				flag =0;	 
			 }
		}
	}else{
		this->Timer1->Enabled=false;
		this->TShapeTimerRed->Brush->Color=clGray;
		this->Btn_Timer_Start->Enabled=true;
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Btn_Timer_StartClick(TObject *Sender)
{
	flag=1;
	count_Red = this->TEditRed->Text.ToInt();
	count_Yellow=this->TEditYellow->Text.ToInt();
	count_Green=(this->TEditGreen->Text.ToInt())-1;
	
	this->Btn_Timer_Start->Enabled=false;
	
	this->TShapeTimerRed->Brush->Color= clGray;
	this->TShapeTimerYellow->Brush->Color= clGray;
	this->TShapeTimerGreen->Brush->Color= clGreen;
	
	this->Timer1->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Btn_Timer_StopClick(TObject *Sender)
{
	this->Timer1->Enabled=false;
}
//---------------------------------------------------------------------------



//***************************************************************************
//
//  				   Thread + Component
//
//***************************************************************************
//---------------------------------------------------------------------------

void __fastcall TForm1::Btn_Thread_StartClick(TObject *Sender)
{

	FLampRed->lamp_start(clRed,Thread_Edit_Red->Text.ToInt(),Thread_Edit_Yellow->Text.ToInt(),Thread_Edit_Green->Text.ToInt());
	FLampGreen ->lamp_start(clGreen,Thread_Edit_Red->Text.ToInt(),Thread_Edit_Yellow->Text.ToInt(),Thread_Edit_Green->Text.ToInt());
	FLampYellow ->lamp_start(clYellow,Thread_Edit_Red->Text.ToInt(),Thread_Edit_Yellow->Text.ToInt(),Thread_Edit_Green->Text.ToInt());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Btn_Thread_StopClick(TObject *Sender)
{

	FLampRed->lamp_stop();
	FLampGreen->lamp_stop();
	FLampYellow->lamp_stop();

}
//---------------------------------------------------------------------------





