//---------------------------------------------------------------------------

#ifndef frmMainH
#define frmMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class Tme;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelTimer;
	TGroupBox *GroupTimer;
	TGroupBox *GroupThread;
	TPanel *PanelThread;
	TTimer *Timer1;
	TShape *TShapeTimerRed;
	TShape *TShapeTimerYellow;
	TShape *TShapeTimerGreen;
	TEdit *TEditYellow;
	TEdit *TEditGreen;
	TEdit *TEditRed;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TButton *Btn_Timer_Start;
	TButton *Btn_Timer_Stop;
	TButton *Btn_Thread_Start;
	TButton *Btn_Thread_Stop;
	TEdit *Thread_Edit_Red;
	TEdit *Thread_Edit_Yellow;
	TEdit *Thread_Edit_Green;
	TLabel *Label1;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Btn_Timer_StartClick(TObject *Sender);
	void __fastcall Btn_Thread_StartClick(TObject *Sender);
	void __fastcall Btn_Thread_StopClick(TObject *Sender);
	void __fastcall Btn_Timer_StopClick(TObject *Sender);

private:	// User declarations
	Tme	*FLampRed;
	Tme	*FLampGreen;
	Tme	*FLampYellow;

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
