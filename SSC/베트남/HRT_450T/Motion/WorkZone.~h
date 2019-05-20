//---------------------------------------------------------------------------
#ifndef WorkZoneH
#define WorkZoneH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "uRs232C.h"
//---------------------------------------------------------------------------
#define MAX_PROB_COL 4
extern int iPinFailCnt[MAX_ARRAY_ROW][MAX_PROB_COL]  ;
class CWorkZone
{
    public:
        //Constructor
        CWorkZone (void);
        ~CWorkZone (void);

        void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        void Close(void);

        //Vars.
        struct SStat {
            bool bWorkEnd  ;
            bool bReqStop  ;
            bool bRepeat   ; //메뉴얼 동작중에 리피트 동작용.
        };

        enum EN_SEQ_CYCLE {
            scIdle          =  0 ,
            scIn            =  1 ,
            scWork          =  2 ,
            scOut           =  3 ,
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        void   InitCycleName(){
            m_sCycleName[scIdle   ]="Idle   ";
            m_sCycleName[scIn     ]="In     ";
            m_sCycleName[scWork   ]="Work   ";
            m_sCycleName[scOut    ]="Out    ";
        }


        struct SStep {
            int          iHome    ; //홈싸이클에서 쓰는 스텝.
            int          iToStart ; //투스타트 싸이클에서 씀.
            EN_SEQ_CYCLE iSeq     ; //장비의 할일 결정.
            int          iCycle   ; //장비의 할일에서 스텝.
            int          iToStop  ; //투스탑 싸이클에서 씀.
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

        int    m_iReGripCnt  ;
        double m_dLastIndPos ;
        int    m_iLastStep   ;

        bool   FindChip   (int &r , int &c   );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

    public:    /* Direct Accessable Var.  */
        AnsiString m_sLastStrip ;

        bool           m_bDispLotEnded ; //화면에 랏정보 폼 띄우기용.
        int            m_iDataErrCnt   ; //데이터 수신 이상에러... 테스트용..
        int            m_iDataErrCntRe ;
        CLot::SLotInfo EndedLot        ;

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        AnsiString    GetCycleName   (){return m_sCycleName[Step.iSeq];}
        SStat         GetStat        (){return Stat          ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // 모터를 움직일때 쓰는 함수.

        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //Running Functions.
        bool ToStopCon (void); //스탑을 하기 위한 조건을 보는 함수.
        bool ToStartCon(void); //스타트를 하기 위한 조건을 보는 함수.
        bool ToStart   (void); //스타트를 하기 위한 함수.
        bool ToStop    (void); //스탑을 하기 위한 함수.
        bool Autorun   (void); //오토런닝시에 계속 타는 함수.

        //One Cycle.
        bool CycleIn      ();
        bool CycleWork    ();
        bool CyclePSBWork ();
        bool CycleOut     ();

        //Manual Cycle.
        bool CycleToolChange ();
        bool CycleToolWait   ();
        bool CycleNextMove   ();
        bool CycleRptCntct   ();


        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad );

        bool bOverFail;
        bool bOverErr ;
        int  iOverFail;

        double dLotYield;
        bool   bYieldErr;


        //CallBack 방식 Rs232
        static void HeatProbe1Ch(DWORD _cbInQue);
        static void HeatProbe2Ch(DWORD _cbInQue);

        bool bPb1Rslt ;
        bool bPb2Rslt ;
        AnsiString sPb1Msg ;
        AnsiString sPb2Msg ;

        String sPb1Bin[2] ;
        String sPb2Bin[2] ;
        String sPb1Vf [2] ;
        String sPb2Vf [2] ;

};

//---------------------------------------------------------------------------
extern CWorkZone WRK;

extern TRS232C *RS232C_Pb1ch  ;
extern TRS232C *RS232C_Pb2ch  ;

//---------------------------------------------------------------------------
#endif
