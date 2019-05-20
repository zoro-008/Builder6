//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//----------------------------------------------------------------------------
#include "Timer.h"
#include "MainThread.h"
#include "Sequence.h"
#include "SMInterfaceUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
HANDLE ThreadHandle[MAX_TIME_PROC];
bool   ThreadEnable[MAX_TIME_PROC];
DWORD  dwStrtTime  [MAX_TIME_PROC];
DWORD  dwScanTime  [MAX_TIME_PROC];
DWORD  dwTimerId   [MAX_TIME_PROC];
int    iTimerId    [MAX_TIME_PROC];

/****************************************************************************/
/*  Get Time Interval in ms                                                 */
/****************************************************************************/
//----------------------------------------------------------------------------
double __fastcall GetTime(void)
{
    LARGE_INTEGER liEndCounter,liFrequency  ;
    QueryPerformanceCounter  (&liEndCounter);
    QueryPerformanceFrequency(&liFrequency );

    return (((double)liEndCounter.QuadPart / (double)liFrequency.QuadPart) * 1000.0 );
}

/****************************************************************************/
/*  Get Time Interval in us                                                 */
/****************************************************************************/
//----------------------------------------------------------------------------
double __fastcall GetTime_us(void)
{
    LARGE_INTEGER liEndCounter,liFrequency ;
    QueryPerformanceCounter  (&liEndCounter);
    QueryPerformanceFrequency(&liFrequency );

    return ( ((double)liEndCounter.QuadPart / (double)liFrequency.QuadPart) * 1000000.0);
}

/****************************************************************************/
/*  Get ScanTime                                                            */
/****************************************************************************/
//----------------------------------------------------------------------------
void __fastcall SetStrtTime(DWORD *StrtTime)
{
    *StrtTime = (DWORD)GetTime();
}

//----------------------------------------------------------------------------
void __fastcall GetScanTime(DWORD  StrtTime , DWORD *ScanTime)
{
    *ScanTime = (DWORD)GetTime() - StrtTime;
}

/****************************************************************************/
/* Thread Processing                                                        */
/****************************************************************************/
//----------------------------------------------------------------------------
void __fastcall StartThread(void)
{
    //Thread #1.
    ThreadEnable[TP_1] = true; ThreadHandle[TP_1] = CreateThread(NULL , 0 , (LPTHREAD_START_ROUTINE)TimeProc1 , NULL , 0 , &dwTimerId[TP_1]);
    SetThreadPriority(ThreadHandle[TP_1] , THREAD_PRIORITY_TIME_CRITICAL);

//    //Thread #2.
//    ThreadEnable[TP_2] = true; ThreadHandle[TP_2] = CreateThread(NULL , 0 , (LPTHREAD_START_ROUTINE)TimeProc2 , NULL , 0 , &dwTimerId[TP_2]);

    //Thread #3.
//    ThreadEnable[TP_3] = true; ThreadHandle[TP_3] = CreateThread(NULL , 0 , (LPTHREAD_START_ROUTINE)TimeProc3 , NULL , 0 , &dwTimerId[TP_3]);
    //SetThreadPriority(ThreadHandle[TP_3] , THREAD_PRIORITY_LOWEST);
}

//----------------------------------------------------------------------------
void __fastcall EndThread(void)
{
    //Thread #1.
    ThreadEnable[TP_1] = false;
    ::ResumeThread(ThreadHandle[TP_1]);
    while (::GetExitCodeThread(ThreadHandle[TP_1] , &dwTimerId[TP_1])) {
        if (dwTimerId[TP_1] == STILL_ACTIVE) continue;
        else break;
        }
    ::CloseHandle(ThreadHandle[TP_1]);

//    //Thread #2.
//    ThreadEnable[TP_2] = false;
//    ::ResumeThread(ThreadHandle[TP_2]);
//    while (::GetExitCodeThread(ThreadHandle[TP_2] , &dwTimerId[TP_2])) {
//        if (dwTimerId[TP_2] == STILL_ACTIVE) continue;
//        else break;
//        }
//    ::CloseHandle(ThreadHandle[TP_2]);

//    //Thread #3.
//    ThreadEnable[TP_3] = false;
//    ::ResumeThread(ThreadHandle[TP_3]);
//    while (::GetExitCodeThread(ThreadHandle[TP_3] , &dwTimerId[TP_3])) {
//        if (dwTimerId[TP_3] == STILL_ACTIVE) continue;
//        else break;
//        }
//    ::CloseHandle(ThreadHandle[TP_3]);
}

/****************************************************************************/
/* Thread Routine.                                                          */
/*    #1. To update sequence.                                               */
/****************************************************************************/
//----------------------------------------------------------------------------
WORD TimeProc1(void)
{
    //Local Var.
    CDelayTimer OnTimer;
//    bool  isUD      = false;
    DWORD Crnt      = (DWORD)GetTime();
//    DWORD dwUD_Strt = (DWORD)GetTime();
//    DWORD dwUP_Scan = (DWORD)GetTime();

    //Get No. of CPU.
    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);

    //Execute.
    while (ThreadEnable[TP_1]) {
        //To remove system's deadlock.
        Sleep(1);
        Application -> ProcessMessages();

        //Sequence Update
//        SEQ.Upde();
        SEQ.Update();
        //Cal. Scan Time.
        Crnt             = (DWORD)GetTime();
        dwScanTime[TP_1] = Crnt - dwStrtTime[TP_1];
        dwStrtTime[TP_1] = Crnt ;
        }
    return 0L;
/*
    while (ThreadEnable[TP_1]) {
        //Update.
        if (!isUD) {
            dwUD_Strt = (DWORD)GetTime();
            SEQ.Update();
            dwUP_Scan = (DWORD)GetTime() - dwUD_Strt;
            isUD      = true;
            OnTimer.Clear();
            }

        //To matched scan time.
        if (dwUP_Scan < TP1_INTERVAL) {
            if (!OnTimer.OnDelay(isUD , TP1_INTERVAL - dwUP_Scan)) {
                if (SystemInfo.dwNumberOfProcessors < 2) {
                    Sleep(10);
                    Application -> ProcessMessages();
                    }
                continue;
                }
            }

        isUD = false;

        //Cal. Scan Time.
        Crnt             = (DWORD)GetTime();
        dwScanTime[TP_1] = Crnt - dwStrtTime[TP_1];
        dwStrtTime[TP_1] = Crnt;
        }
    return 0L; */
}

/****************************************************************************/
/* Thread Routine.                                                          */
/*    #2. To update vision.                                                 */
/****************************************************************************/
//----------------------------------------------------------------------------
WORD TimeProc2(void)
{
    DWORD Crnt = (DWORD)GetTime();
    while (ThreadEnable[TP_2]) {
        //To remove system's deadlock.
        //Sleep(15);
        //BAL.Update();
        //Application -> ProcessMessages();
        //Cal. Scan Time.
        Crnt             = (DWORD)GetTime();
        dwScanTime[TP_2] = Crnt - dwStrtTime[TP_2];
        dwStrtTime[TP_2] = Crnt;
        }
    return 0L;
}

/****************************************************************************/
/* Thread Routine.                                                          */
/*    #3. To update vision.                                                 */
/****************************************************************************/
//----------------------------------------------------------------------------
WORD TimeProc3(void)
{
    DWORD Crnt = (DWORD)GetTime();
    while (ThreadEnable[TP_3]) {
        //To remove system's deadlock.
        Sleep(1);
        Application -> ProcessMessages();

        //Cal. Scan Time.
        Crnt             = (DWORD)GetTime();
        dwScanTime[TP_3] = Crnt - dwStrtTime[TP_3];
        dwStrtTime[TP_3] = Crnt;
        }
    return 0L;
}



/*
해 보시면 1초간 CPU 속도 만큼을 카운트 한다는걸 알 수 있죠
#define getTick(count){                 \
    __asm rdtsc;                        \
    __asm lea ebx, count;               \
    __asm mov dword ptr [ebx], eax;     \
    __asm mov dword ptr [ebx + 4], edx; \
}
unsigned __int64 snapshot_rdtsc()
{
    static unsigned __int64 tick;
    static unsigned __int32 lo, hi;

    _asm _emit 0x0f
    _asm _emit 0x31
    _asm mov lo, eax
    _asm mov hi, edx

    tick = hi;
    tick <<= 32;
    tick |= lo;

    return tick;
}
Category : VC++ 일반      
제        목  Pentium CPU 내부 카운터 읽기; RDTSC 
올   린   ID  김진홍(neuron)   작 성  시 각  2001-01-01 오전 12:58:19 
조   회   수  190 첨 부 파 일  


안녕하세요. 펜티엄에는 CPU를 디버깅하기위한 내부카운터가 있다는것을 

들어보셨을겁니다. 바로 RDTSC라는 레지스터고 코드 시퀸스는 0x0f31 입니다. 

원래는 문서화 되지 않았고, 폭로(?)하지 않는다는 서약서에 

서명하는 개발자들에게만 알려주었던 내용이죠. 하지만 

결국 사람들이 발견해 내었었죠. 상당히 오래된 이야기인데, 

저는 이것을 도스상에서 사용했었습니다. 

RING 0 레벨의 명령어라서 윈도우즈95 상에서 크래쉬를 

일으키기도 했었는데 지금 윈도우즈(2000포함)에서 

아무문제없이 잘 되네요.  

사용하고 있는 CPU의 클럭속도로 증가하는 카운터의 현재값을 

읽어내는 함수입니다.  가장 빠르고 정확한 카운터라고 

할 수 있겠죠. 



함수의 원형은 아래와 같이 간단합니다. 



unsigned __int64 snapshot_rdtsc() 
{ 
    static unsigned __int64 tick; 
    static unsigned __int32 lo, hi; 

    _asm _emit 0x0f 
    _asm _emit 0x31 
    _asm mov lo, eax 
    _asm mov hi, edx 

    tick = hi; 
    tick <<= 32; 
    tick |= lo; 

    return tick; 
} 


64비트 정수형을 지원하는 컴파일러에서 사용할 수 있습니다. 

_emit을 지원하지 않으면 어셈블리 명령어인 db를 대신해서 

사용하면 될겁니다. 

돌려주는 값이 CPU의 클럭이므로 돌려주는 값을 CPU주파수로 

나누면 CPU에 전원이 들어오고나서부터의 시간(초)이 나오게됩니다. 

예) printf("%I64u\n", snapshot_rdtsc() / 466000000); // 466Mhz인경우 

466Mhz인경우 466*100000 값을 넣었지만, 실제로 정확히 466은 아니겠죠.. 


펜티엄에는 RDTSC말고도 상당히 많은 디버깅 카운터 레지스터들이 있습니다. 

뭐 code execution이 일어날때마다 증가하는 카운터, 인터럽트가 발생할때마다 

증가하는 카운터 등등 말이에요. 이 내용은 97년인가에 BYTE잡지에 

실린적이 있습니다. Terje Mathisen라는 분이 Pentium Secrets라는 제목으로 

BYTE ARTICLES에 기고했었습니다. 


음.. 

그리고 CPU의 주파수를 동적으로 알아내시려면 

현재의 priority class를 REALTIME_PRIORITY_CLASS로 

thread priority를 THREAD_PRIORITY_TIME_CRITICAL로 

설정하시고 (물론 나중에 원상복구해야겠죠) 현재 클럭 얻어내고 

Sleep(5000) 으로 5초간 기다리고 다시 현재 클럭 얻어내어 

그 두 값의 차이를 5000으로 나누면 상당히 정확한 주파수를 알 수 

있을겁니다. 
    //Local Var.
    unsigned __int64 iCount;

    //Get Count.
    asm {
        rdtsc
        lea   ebx                 , iCount
        mov   dword ptr [ebx    ] , eax
        mov   dword ptr [ebx + 4] , edx
        }

    //Time.
    LARGE_INTEGER liFrequency ;
    QueryPerformanceFrequency(&liFrequency );
    return ( ((double)iCount / (double)liFrequency.QuadPart) * 1000.0   );
    //return iCount;

*/


