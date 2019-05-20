/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Usage  :                                                                */
/* Date   : 2004.05.26                                                     */
/* Modify : 2004.06.24                                                     */
/* Programmed by Primer(YongSik, Lee).                                     */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//---------------------------------------------------------------------------
#ifndef MainThreadH
#define MainThreadH

//----------------------------------------------------------------------------
    //Timer ID.
#define MAX_TIME_PROC      3
#define TP_1               0
#define TP_2               1
#define TP_3               2
    //Time Interval.
#define TP1_INTERVAL       2
#define TP2_INTERVAL       10

//----------------------------------------------------------------------------
extern DWORD dwStrtTime[MAX_TIME_PROC];
extern DWORD dwScanTime[MAX_TIME_PROC];

//----------------------------------------------------------------------------
extern double __fastcall GetTime    (void                             );
extern double __fastcall GetTime_us (void                             );
extern void   __fastcall SetStrtTime(DWORD *StrtTime                  );
extern void   __fastcall GetScanTime(DWORD  StrtTime , DWORD *ScanTime);

//----------------------------------------------------------------------------
extern void __fastcall StartThread(void);
extern void __fastcall EndThread  (void);

//----------------------------------------------------------------------------
extern WORD TimeProc1(void);
extern WORD TimeProc2(void);
extern WORD TimeProc3(void);
extern WORD TimeProc4(void);



//---------------------------------------------------------------------------
#endif


