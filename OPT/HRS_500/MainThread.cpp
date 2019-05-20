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
�� ���ø� 1�ʰ� CPU �ӵ� ��ŭ�� ī��Ʈ �Ѵٴ°� �� �� ����
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
Category : VC++ �Ϲ�      
��        ��  Pentium CPU ���� ī���� �б�; RDTSC 
��   ��   ID  ����ȫ(neuron)   �� ��  �� ��  2001-01-01 ���� 12:58:19 
��   ȸ   ��  190 ÷ �� �� ��  


�ȳ��ϼ���. ��Ƽ������ CPU�� ������ϱ����� ����ī���Ͱ� �ִٴ°��� 

�������̴ϴ�. �ٷ� RDTSC��� �������Ͱ� �ڵ� �������� 0x0f31 �Դϴ�. 

������ ����ȭ ���� �ʾҰ�, ����(?)���� �ʴ´ٴ� ���༭�� 

�����ϴ� �����ڵ鿡�Ը� �˷��־��� ��������. ������ 

�ᱹ ������� �߰��� ��������. ����� ������ �̾߱��ε�, 

���� �̰��� �����󿡼� ����߾����ϴ�. 

RING 0 ������ ��ɾ�� ��������95 �󿡼� ũ������ 

����Ű�⵵ �߾��µ� ���� ��������(2000����)���� 

�ƹ��������� �� �ǳ׿�.  

����ϰ� �ִ� CPU�� Ŭ���ӵ��� �����ϴ� ī������ ���簪�� 

�о�� �Լ��Դϴ�.  ���� ������ ��Ȯ�� ī���Ͷ�� 

�� �� �ְ���. 



�Լ��� ������ �Ʒ��� ���� �����մϴ�. 



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


64��Ʈ �������� �����ϴ� �����Ϸ����� ����� �� �ֽ��ϴ�. 

_emit�� �������� ������ ����� ��ɾ��� db�� ����ؼ� 

����ϸ� �ɰ̴ϴ�. 

�����ִ� ���� CPU�� Ŭ���̹Ƿ� �����ִ� ���� CPU���ļ��� 

������ CPU�� ������ �������������� �ð�(��)�� �����Ե˴ϴ�. 

��) printf("%I64u\n", snapshot_rdtsc() / 466000000); // 466Mhz�ΰ�� 

466Mhz�ΰ�� 466*100000 ���� �־�����, ������ ��Ȯ�� 466�� �ƴϰ���.. 


��Ƽ������ RDTSC���� ����� ���� ����� ī���� �������͵��� �ֽ��ϴ�. 

�� code execution�� �Ͼ������ �����ϴ� ī����, ���ͷ�Ʈ�� �߻��Ҷ����� 

�����ϴ� ī���� ��� ���̿���. �� ������ 97���ΰ��� BYTE������ 

�Ǹ����� �ֽ��ϴ�. Terje Mathisen��� ���� Pentium Secrets��� �������� 

BYTE ARTICLES�� ����߾����ϴ�. 


��.. 

�׸��� CPU�� ���ļ��� �������� �˾Ƴ��÷��� 

������ priority class�� REALTIME_PRIORITY_CLASS�� 

thread priority�� THREAD_PRIORITY_TIME_CRITICAL�� 

�����Ͻð� (���� ���߿� ���󺹱��ؾ߰���) ���� Ŭ�� ���� 

Sleep(5000) ���� 5�ʰ� ��ٸ��� �ٽ� ���� Ŭ�� ���� 

�� �� ���� ���̸� 5000���� ������ ����� ��Ȯ�� ���ļ��� �� �� 

�����̴ϴ�. 
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


