
//---------------------------------------------------------------------------
#ifndef SequenceH
#define SequenceH

//---------------------------------------------------------------------------
#include "Timer.h"
#include "SMInterfaceUnit.h"
//#include <Grids.hpp>

class CSequence
{
    public :
        enum EN_SEQ_STEP {
            scIdle       =  0 ,
            scToStartCon = 10 ,
            scToStart    = 11 ,
            scRun        = 12 ,
            scToStopCon  = 13 ,
            scToStop     = 14 ,

            MAX_SEQ_CYCLE
        };

    private:   /* Member Var. */
        //Timer.                        m_tmMain
        CDelayTimer m_tmToStop     ;
        CDelayTimer m_tmToStrt     ;
        CDelayTimer m_tmFlickOn    ;
        CDelayTimer m_tmFlickOff   ;
        CDelayTimer m_tmCloseDoor  ;
        CDelayTimer m_tmTemp       ;



        //Vars.
        bool        m_bBtnReset        ; //Button Input.
        bool        m_bBtnStart        ;
        bool        m_bBtnHome         ;
        bool        m_bBtnRotate       ; //회전 메뉴얼 버튼.

        bool        m_bRun             ; //Run Flag. (Latched)
        bool        m_bAutoMode        ; //AutoMode / ManualMode .
        bool        m_bReqStop         ;
        bool        m_bFlick           ; //Flicking Flags.

        EN_SEQ_STEP m_iStep            ; //Sequence Step.


        bool r1 , r2 , r3 ,r4 ,r5;

    protected:
        //Check Button.
        void __fastcall UpdateButton  (void);

    public:
        //Var.
        DWORD dwUD_Strt[20]; //Scan Time
        DWORD dwUP_Scan[20];

        //Property.
        __property bool         _bBtnReset  = {                    write = m_bBtnReset  };
        __property bool         _bBtnStart  = {                    write = m_bBtnStart  };
        __property bool         _bBtnHome   = {                    write = m_bBtnHome   };
        __property bool         _bBtnRotate = {                    write = m_bBtnRotate };



        __property bool         _bRun      = { read = m_bRun                           };
        __property bool         _bAutoMode = { read = m_bAutoMode  ,write = m_bAutoMode};
        __property bool         _bReqStop  = { read = m_bReqStop                       };
        __property bool         _bFlick    = { read = m_bFlick                         };

        __property EN_SEQ_STEP  _iStep     = { read = m_iStep                          };
        __property EN_SEQ_STAT  _iSeqStat  = { read = m_iSeqStat                       };


    public:
        __fastcall  CSequence(void);
        __fastcall ~CSequence(void);

        //Init.
        void __fastcall Init (void);
        void __fastcall Reset(bool _Option = true );

        //Functions.
            //Inspection Machine Status.
        bool  __fastcall InspectMainAir     (void);
        bool  __fastcall InspectTemp        (void);
        bool  __fastcall InspectEmergency   (void);
        bool  __fastcall InspectLightGrid   (void);
        bool  __fastcall InspectDoor        (void);
        bool  __fastcall InspectActuator    (void);
        bool  __fastcall InspectMotor       (void);
        bool  __fastcall InspectHomeEnd     (void);
        bool  __fastcall InspectCrash       (void);
        bool  __fastcall InspectStripDispr  (void);
        bool  __fastcall InspectStripUnknown(void);

            //Running acions.
        bool  __fastcall ToStartCon      (void);        //Clear data to start.
        bool  __fastcall ToStopCon       (void);        //Check condition to stop the cycle. (Check iStepIndex number)
        bool  __fastcall ToStart         (void);        //Functions when start cycle running.
        bool  __fastcall ToStop          (void);        //Functions when stop the cycle.
        bool  __fastcall AutoRun         (void);        //Auto Run.

            //Real Time Update.
        void  __fastcall UpdateSeqState  (void);
        void  __fastcall Update          (void);

        EN_SEQ_STAT m_iSeqStat         ; //Current Sequence Status.9

        TDateTime m_dLastTime ;

};

//---------------------------------------------------------------------------
extern CSequence SEQ;

//---------------------------------------------------------------------------
#endif


