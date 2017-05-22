//---------------------------------------------------------------------------
#ifndef TModuleThreadH
#define TModuleThreadH
const long TickResetCount = 4294967295;
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TModuleEvent)(TObject* Sender);
//---------------------------------------------------------------------------
class TModuleThread : public TThread
{
__published://User declarations
private://User declarations
    TModuleEvent FOnSignal;
    HANDLE FhModuleHandle;

    bool FbSafeSetEventFlag;
    int  FSleepTime ;
    bool FbSynchronize ;
protected:
	void __fastcall Execute(void);
    void __fastcall DoOnSignal(void);
public:
	bool bContinueRun;
	bool FbRunFlag;
    //-- 2007/10/29�W�[�B�zVCL is not thread-safe comopnent
    __fastcall TModuleThread(HANDLE hModuleHandle,int SleepTime,bool bSynchronize);
	__property TModuleEvent OnSignal = {read = FOnSignal, write = FOnSignal};
	void __fastcall WaitThreadFinish(void);
    //----- �w����SetEvent��ܡA�@�w�bResetEvent��~�i�A�����~���USetEvent���O
    bool __fastcall SafeSetEvent(void);  // ���w TimeOut 1000 ms
    bool __fastcall GetThreadEnd(void);
    void __fastcall ThreadTerminate(void);
};
//---------------------------------------------------------------------------
class TAccuracyTimeThread : public TThread
{
private:
	TModuleEvent FOnSignal;
	int  FAccTimeMs ;
	bool FbSynchronize ;
	HANDLE hTimerEvent ;
	HANDLE FhModuleHandle;
	bool FbSafeSetEventFlag;
	bool bTerminatedCountTimer  ;
protected:
	void __fastcall Execute();
	void __fastcall DoOnSignal(void);
public:
	__fastcall TAccuracyTimeThread(HANDLE hModuleHandle, int accTimeMs,bool bSynchronize);
	__property TModuleEvent OnSignal = {read = FOnSignal, write = FOnSignal};
	void __fastcall WaitThreadFinish(void);
	//----- �w����SetEvent��ܡA�@�w�bResetEvent��~�i�A�����~���USetEvent���O
	bool __fastcall GetThreadEnd(void);
	void __fastcall ThreadTerminate(void);
	void __fastcall ThreadTerminateCountTimer(void);
	bool FbRunFlag;
	bool bContinueRun;

};

//---------------------------------------------------------------------------
#endif


