//---------------------------------------------------------------------------
//***************************************************************************
//** Create Date : 2010.10.13              Ver.:1.00_A       by Cuiu_Handsome
//** Function : �@�� Function ,�D�n�~�Ӧ� TBasetools
//***************************************************************************
//---------------------------------------------------------------------------
#ifndef TGeneralToolsH
#define TGeneralToolsH

#include "TBasetools.h"
//---------------------------------------------------------------------------
class TGeneralTools : public TBasetools
{
	private:
		static  TGeneralTools* _instance;  // �ŧi�@�p������
	protected:  //User declarations
		__fastcall  TGeneralTools(void);   // �N�غc���θѺc������p���ϩΫO�@��
		__fastcall  ~TGeneralTools(void);  // �T��~���{������New���󲣥�
	public:
		static TGeneralTools*   __fastcall getInstance();       // ���ѥ~���@�Ӥ�kGet�������
		static void         	__fastcall delInstance();       // ���ѥ~���@�Ӥ�kDelete�������
};
//---------------------------------------------------------------------------
#endif
