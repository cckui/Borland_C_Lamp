//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <mmsystem.h>
#pragma hdrstop
#include "TModuleThread.h"
//#include "TGenTools.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//----- Module作動Threads!!    //----95.01.02 增加SLEEP的時間參數
//----- Module作動Threads!!    //----96.10.29 增加Synchronize參數
//----- 2008.02.13 增加!Terminated 判斷，在外部停執行緒之後需加 Terminated()及 SetEvent
//---------------------------------------------------------------------------
__fastcall TModuleThread::TModuleThread(HANDLE hModuleHandle,int SleepTime,bool bSynchronize):
           TThread(true)
{
    FSleepTime = SleepTime ;
	FreeOnTerminate = true;
	FhModuleHandle = hModuleHandle;
	bContinueRun = false;
	FbSafeSetEventFlag = false;
	FbSynchronize = bSynchronize;
	//Resume();  C++Builder2009/2010以后的版本中, TThread类的Resume和Suspend函数已不建议使用
	Suspended = false;
}
//---------------------------------------------------------------------------

void __fastcall TModuleThread::Execute(void)
{
    DWORD bWaitEventResult;
	while (!Terminated){
		bWaitEventResult = WaitForSingleObject(FhModuleHandle, INFINITE);
		if (bWaitEventResult == WAIT_OBJECT_0 && !Terminated){
			FbRunFlag = true;
            FbSafeSetEventFlag = true;
			if(!FbSynchronize){
			   DoOnSignal() ;
			}
			else {
			   Synchronize(DoOnSignal);
			   Application->ProcessMessages();
			}
			ResetEvent(FhModuleHandle);
			FbSafeSetEventFlag = false;
			Application->ProcessMessages();
			Sleep(FSleepTime) ;

		}
		else Sleep(0);
		if (bContinueRun) {
			SetEvent(FhModuleHandle);
		}
		else
			FbRunFlag = false;

		Application->ProcessMessages();
	}
	CloseHandle(FhModuleHandle);
}
//---------------------------------------------------------------------------

void __fastcall TModuleThread::DoOnSignal(void)
{
    if (FOnSignal)  FOnSignal(this);
}
//---------------------------------------------------------------------------

void __fastcall TModuleThread::WaitThreadFinish(void)
{
   bContinueRun = false ;
    while (FbRunFlag){
       Application->ProcessMessages();//  Sleep(0);
    }
}
//---------------------------------------------------------------------------

bool __fastcall TModuleThread::SafeSetEvent(void)  // 內定 TimeOut 1000 ms
{
    int TickCount, ElapsedTime;
    TickCount = GetTickCount();
    ElapsedTime = 0;
    while (ElapsedTime < 1000){
        if (!FbSafeSetEventFlag){
            SetEvent(FhModuleHandle);
            return true;
        }
        else {
            ElapsedTime = GetTickCount()-TickCount;
            ElapsedTime = ElapsedTime<0?ElapsedTime+TickResetCount:ElapsedTime;
        }
        Sleep(0);
    }
    return false;
}
//---------------------------------------------------------------------------

bool __fastcall TModuleThread::GetThreadEnd(void)  // 內定 TimeOut 1000 ms
{
    return !FbRunFlag ;
}
//---------------------------------------------------------------------------

void __fastcall TModuleThread::ThreadTerminate(void)
{
    Terminate() ;
    WaitFor();
}
//---------------------------------------------------------------------------
__fastcall TAccuracyTimeThread::TAccuracyTimeThread(HANDLE hModuleHandle, int accTimeMs,bool bSynchronize)
	 :TThread(false)
{
	FAccTimeMs =  accTimeMs ;
	FreeOnTerminate = true;
	FhModuleHandle = hModuleHandle;
	bContinueRun = false;
	FbSafeSetEventFlag = false;
    FbSynchronize = bSynchronize;
}

//---------------------------------------------------------------------------
void __fastcall TAccuracyTimeThread::Execute()
{

	DWORD bWaitTimerEventResult ;
	DWORD bWaitEventResult;

	hTimerEvent = CreateEvent(NULL,false,false,NULL);
	int TimerID = timeSetEvent(FAccTimeMs,100,(LPTIMECALLBACK)hTimerEvent,0,
	  TIME_PERIODIC|TIME_CALLBACK_EVENT_SET);

	while (!Terminated){
		bWaitEventResult = WaitForSingleObject(FhModuleHandle, INFINITE);
		if (bWaitEventResult == WAIT_OBJECT_0 && !Terminated){
			FbRunFlag = true;
			FbSafeSetEventFlag = true;
			bTerminatedCountTimer = false ;

		while(!bTerminatedCountTimer){
			bWaitTimerEventResult = WaitForSingleObject(hTimerEvent,INFINITE);
			if((bWaitTimerEventResult == WAIT_OBJECT_0 )&&(!bTerminatedCountTimer))
			{
				if(!FbSynchronize){
					DoOnSignal() ;
				}
				else {
					Synchronize(DoOnSignal);
					Application->ProcessMessages();
				}
			}
			else Sleep(0);
		}

		ResetEvent(FhModuleHandle);
		FbSafeSetEventFlag = false;
		Application->ProcessMessages();

		if (bContinueRun) {
			SetEvent(FhModuleHandle);
		}
		else
			FbRunFlag = false;

		Application->ProcessMessages();
	  }
	  else Sleep(0);
	}

	timeKillEvent(TimerID);
	CloseHandle(hTimerEvent);
	CloseHandle(FhModuleHandle);
}
//---------------------------------------------------------------------------
void __fastcall TAccuracyTimeThread::DoOnSignal(void)
{
	if (FOnSignal)  FOnSignal(this);
}
//---------------------------------------------------------------------------
bool __fastcall TAccuracyTimeThread::GetThreadEnd(void)  // 內定 TimeOut 1000 ms
{
    return !FbRunFlag ;
}
//---------------------------------------------------------------------------

void __fastcall TAccuracyTimeThread::ThreadTerminate(void)
{
	Terminate() ;
	WaitFor();
}
//---------------------------------------------------------------------------
void __fastcall TAccuracyTimeThread::ThreadTerminateCountTimer(void)
{
    bTerminatedCountTimer = true ;
}

