//---------------------------------------------------------------------------
#ifndef HeadH
#define HeadH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
//Vision
//---------------------------------------------------------------------------
#include "SAInterface.h"
//---------------------------------------------------------------------------
//static void __stdcall HedZStripWorkWait(long _lAxis, DWORD _dwFlag);
//static void __stdcall HedZWaferWorkWait(long _lAxis, DWORD _dwFlag);
//static void __stdcall HedZWait         (long _lAxis, DWORD _dwFlag);
#define VOICE_COIL_ID     0

enum EN_ARAY_STEP {
    hsNotWork = 0 ,
    hsGrabStt     ,
    hsEmpty       ,
    hsWorkEnd     ,
    hsWorkFail    ,
};
class CHead
{
    public:
        //Constructor
         CHead ();
        ~CHead ();

        void Init      ();
        void Close     ();
        void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle     = 0 ,
            scPick     = 1 ,
            scPlace    = 2 ,

            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName [MAX_SEQ_CYCLE] ;
        void InitCycleName(){
            m_sCycleName[scIdle ]="Idle ";
            m_sCycleName[scPick ]="Pick ";
            m_sCycleName[scPlace]="Place";

        }

        struct SStep {
            int          iHome    ;
            int          iToStart ;
            EN_SEQ_CYCLE iSeq     ;
            int          iCycle   ;
            int          iToStop  ;
        };

    protected:
        AnsiString m_sPartName  ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmTemp    ;

        SStat Stat ;
        SStep Step , PreStep ;

        double m_dLastIdxPos ;

        int m_iHomeTO  ;
        int m_iStartTO ;
        int m_iStopTO  ;
        int m_iCycleTO ;



        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c , EN_CHIP_STAT _stats = csUnkwn );
        double GetWorkPos (EN_MOTR_ID _iMotr);
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

    private:
        static void __stdcall CallAxisZWaferPs (long _lAxis, DWORD _dwFlag);
        static void __stdcall CallAxisZWaferVc (long _lAxis, DWORD _dwFlag);
        static void __stdcall CallVisnAtGrib   (long _lAxis, DWORD _dwFlag);

        static void __stdcall CallAxisZStripPs (long _lAxis, DWORD _dwFlag);

        static void __stdcall CallAxisYWaferPs (long _lAxis, DWORD _dwFlag);


        static void __stdcall HedZStripWorkWait(long _lAxis, DWORD _dwFlag);
        static void __stdcall HedZWaferWorkWait(long _lAxis, DWORD _dwFlag);
        static void __stdcall HedZWaitPick     (long _lAxis, DWORD _dwFlag);
        static void __stdcall HedZWaitPlace    (long _lAxis, DWORD _dwFlag);

    public:    /* Direct Accessable Var.  */
        bool   m_bDispPickCal  ;
        bool   m_bDispPlaceCal ;
        double m_dPickCalPos   ;
        double m_dPlaceCalPos  ;



        double m_dVoiceCoilWafer   ;
        double m_dVoiceCoilWafer1  ;
        double m_dVoiceCoilStrip   ;
        double m_dVoiceCoilStrip1  ;

        double m_dPickCycleTime ;
        void   SetIntrptPos1();
        void   SetIntrptPos2();
        void   SetIntrptPos3();
        void   SetIntrptPos4();

        double GetVoiceCoilVolt(double _dGram);

        int    iStgCheck ;

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

  
        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ; m_iHomeSubStep = 10;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat        GetStat        (){return Stat          ;}
        AnsiString   GetCycleName   (){return m_sCycleName[Step.iSeq];}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Property.
//        __property int   _iMgzCnt      = { read = m_iMgzCnt    , write= m_iMgzCnt };
        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed = false); // 모터를 움직일때 쓰는 함수.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.
        bool CycleHomeSub(); //개객끼 삼메이를 조지기용.

        //Running Functions.
        bool ToStopCon (void); //스탑을 하기 위한 조건을 보는 함수.
        bool ToStartCon(void); //스타트를 하기 위한 조건을 보는 함수.
        bool ToStart   (void); //스타트를 하기 위한 함수.
        bool ToStop    (void); //스탑을 하기 위한 함수.
        bool Autorun   (void); //

        //One Cycle.
        bool CyclePick      (void); //메뉴얼일때 강제로 T축 안돌리기.
        bool CyclePickCurve (void);
        bool CyclePlace     (void);
        bool CyclePlaceCurve(void);

        //Manual Cycle.
        bool CyclePickCal   (void); //픽하는 높이 보정. 웨이퍼에 메뉴얼로 칩얼라인 까지 마친채로 수행 한다.
        bool CyclePlaceCal  (void); //플레이스 하는 높이 보정. 레일에 보트및 자제가 있는 채로 수행 한다.
        bool CycleWfrPlace  (void); //픽한것을 웨이퍼에 다시 내려 놓는 작업 픽 검증 할때 쓰인다.
        bool CycleRalPlace  (void); //픽한것을 플레이스 센터에 놓는것. 픽 검증 할때 쓰인다.


        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad);

        bool InspectToolSafe();


        int m_iHomeSubStep ;

};

//---------------------------------------------------------------------------
extern CHead HED;
//---------------------------------------------------------------------------
#endif
