//----------------------------------    -----------------------------------------
#ifndef SEQUnitH
#define SEQUnitH
#include "Timer.h"

#include <Classes.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
//void Msg(AnsiString _sMsg)
class CSEQ
{
    public:
        CDelayTimer m_tmToStop     ;
        CDelayTimer m_tmToStrt     ;
        CDelayTimer m_tmCycle      ;
        CDelayTimer m_tmFlickOn    ;
        CDelayTimer m_tmFlickOff   ;
        CDelayTimer m_tmCloseDoor  ;
        CDelayTimer m_tmDrain      ;
        CDelayTimer m_tmWorkErr    ;
        CDelayTimer m_tmWorkEnd    ;
        CDelayTimer m_tmTemp       ;
        CDelayTimer m_tmTemp1      ;
        //준선이의 가라 놀이~~~! JS
        CDelayTimer m_tmGara       ;



        //Cycle Time
        CCycleTimer m_tmClean      ;
        CCycleTimer m_tmAir        ;
        CCycleTimer m_tmDry        ;


        enum EN_SEQ_STEP {
            scIdle       =  0 ,
            scToStartCon = 10 ,
            scToStart    = 20 ,
            scCycleClean = 30 ,
            scCycleAIR   = 40 ,
            scCycleDRY   = 50 ,
            scToStopCon  = 60 ,
            scToStop     = 70 ,

            MAX_SEQ_CYCLE
        };
        struct SStep {
            int          iHome    ;
            int          iToStart ;
            int          iCycle   ;
            int          iToStop  ;
        };


        bool        m_bBtnReset        ; //Button Input.
        bool        m_bBtnStart        ;
        bool        m_bBtnStop         ;
        bool        m_bBtnEmg          ;  

        bool        m_bRun             ; //Run Flag. (Latched)
        bool        m_bBtnResetDown    ; //Reset 동작 중인지 확인.

        bool __fastcall ToStart     (void);
        bool __fastcall ToStop      (void);
        bool __fastcall CycleClean  (void);
        bool __fastcall CycleAir    (void);
        bool __fastcall CycleDry    (void);

        EN_SEQ_STEP m_iStep;
        SStep       Step,PreStep;

        //Time Check
        //String sSeqStartTime , sSeqEndTime;
        int iForStart[100] ;
        int iForEnd  [100] ;
        int iForCnt  [100] ;

        int iForStartCnt ;
        int iForEndCnt   ;

        double dTotalTime  ; //double ;;; sorry

        void StartSeq  ();

        //bool IsRun (void) { return !Suspended; }
        bool ReqStop() {m_bReqStop = true ;}


    private:
        void __fastcall UpdateButton  (void);

        enum EN_FUNC {
            fcCHECKFINISH  = 0 ,
            fcMOVE0RPM         ,
            fcMOVEHOME         ,
            fcMOVEABS          ,
            fcMOVEINC          ,
            fcLASER            ,
            fcDELAY            ,
            fcCOMMENT          ,
            fcEMPTY            ,
            fcCAMERA           ,
            fcSERVO0           ,
            fcSHAKE0           ,
            fcOR_SETSPEED0     ,
            fcOR_SETTIME0      ,
            fcOR_SETSTART0     ,
            fcCOOLING          ,
            fcCAMLIVE          ,
            fcSETHEAT          ,
            fcFORSTART         ,
            fcFOREND           ,
            MAX_FUNC
        };

        int        m_dORSpeed[100] ;
        int        m_dORTime[100]  ;
        int        m_iMaxVal       ;
        int        m_iMaxTime      ;

        bool    m_bReqStop ;

        bool    m_bSeqInit ;

    public:
        __fastcall CSEQ(void);
        __fastcall ~CSEQ(void);

        void Init();
        void Close();
        void __fastcall Reset (void);
        void __fastcall Update(void);

        void __fastcall Flick();
        bool __fastcall FlickErr(void);
        bool __fastcall FlickWorkEnd(void);

        bool __fastcall MotrAlrmReset(void);

        bool m_bFlick   ;  //Buzz울리게 하는 놈...

        bool m_bErr       ;  //
        bool m_bWorkEnd   ;
        bool m_bWorkfinish;
        bool m_bFlickErr  ;

        int iTemp;
        void __fastcall ReConnect(void);

};
extern CSEQ *SEQ;
#endif
