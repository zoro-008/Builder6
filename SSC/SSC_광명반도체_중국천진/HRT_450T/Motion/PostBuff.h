//---------------------------------------------------------------------------

#ifndef PostBuffH
#define PostBuffH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
class CPostBuff
{
    public:
        //Constructor
        CPostBuff (void);
        ~CPostBuff (void);
	void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        void Close(void);

        //Vars.
        struct SStat {
            bool bWorkEnd   ;
            bool bReqStop   ;
            bool bInWorkEnd ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scIn         =  1 ,
            scDrag       =  2 ,
            scWork       =  3 ,
            scReGrip     =  4 ,
            scOut        =  5 ,
            scContact    =  6 ,
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        void   InitCycleName(){
            m_sCycleName[scIdle  ]="Idle   ";
            m_sCycleName[scIn    ]="In     ";
            m_sCycleName[scDrag  ]="Drag   ";
            m_sCycleName[scWork  ]="Work   ";
            m_sCycleName[scReGrip]="ReGrip ";
            m_sCycleName[scOut   ]="Out    ";
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

        double m_dLastIndPos  ;
        double m_dFailChipPos ;
        int    m_iCrntCol     ;

        //bool   FindChip   (int &r , int &c , EN_CHIP_STAT Stat  );
//        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , EN_CHIP_STAT Mode = csFail);

    public:    /* Direct Accessable Var.  */
        //WorkZone 마지막 부분 PostBuff에서 처리 때문.. 추가.JS
        int    m_iDataErrCnt   ; //데이터 수신 이상에러... 테스트용..
        int    m_iReGripCnt    ; //WorkZone에서 봐서 public으로 뺌. 

        CLot::SLotInfo EndedLot        ;

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , EN_CHIP_STAT Mode = csFail);        
        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        bool   GetRegribCnt (){return m_iReGripCnt;}

        bool   GetReadyPos(EN_CHIP_STAT Mode = csFail);

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        AnsiString    GetCycleName   (){return m_sCycleName[Step.iSeq];}
        SStat         GetStat        (){return Stat          ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }
        bool   FindChip   (int &r , int &c , EN_CHIP_STAT Stat  );
        bool   FindChipWRK(int &r , int &c) ;
        //Property.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        __property int   _iReGripCnt   = { read = m_iReGripCnt                 };
        __property int   _iReGripCnt   = {                   write = m_iReGripCnt};
        __property int   _iCrntCol     = {read = m_iCrntCol                      };

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
        bool CycleContact ();
        bool CycleIn      ();
        bool CycleDrag    ();
        bool CycleWork    ();
        bool CycleReGrip  ();
        bool CycleOut     ();
        bool CycleNextMove();

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad );

        //LotLogWrite.
        bool GetLastStrip();
        AnsiString m_sWorkedLotNo;

};

//---------------------------------------------------------------------------
extern CPostBuff PSB;
//---------------------------------------------------------------------------
#endif
