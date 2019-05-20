//---------------------------------------------------------------------------

#ifndef SortingToolH
#define SortingToolH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"


#define  MAXBIN  10
//---------------------------------------------------------------------------
class CSortingTool
{
    public:
        //Constructor
        CSortingTool (void);
        ~CSortingTool (void);

        void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd  ;
            bool bReqStop  ;
            bool bRepeat   ; //메뉴얼 동작중에 리피트 동작용.
        };

        enum EN_SEQ_CYCLE {
            scIdle          =  0 ,
            scPick          = 10 ,
            scPlace         = 20 ,
            scDevChk        = 30 ,
            scTrmChk        = 40 ,
            MAX_SEQ_CYCLE
        };

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

        SStat Stat              ;
        SStep Step , PreStep    ;

        bool       m_bLtDir       ;
        int        m_iPreCol      ;
        bool       m_bDir         ;
        int        m_iBinCnt      ;
        double     m_dFailChipPos ;
        double     m_dLastIndPos  ;
        AnsiString m_sLastID      ;

        AnsiString  m_sCrentID;

        //bool   FindChip   (int &r , int &c , EN_CHIP_STAT Stat  );
        void   GetCrntStep(void              );
        
        //void   PickDir    (int Cnt           );
        //double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

    public:    /* Direct Accessable Var.  */
        AnsiString m_sLastStrip ;
        int TopRowCnt;
        int BtmRowCnt;

        __property AnsiString   _sLastID   = {read = m_sLastID};
        __property int          _iBinCnt   = {read = m_iBinCnt , write = m_iBinCnt};
        double RejectMove (void              );

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
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
        bool CyclePick     ();
        bool CyclePlce     ();
        bool CycleDevCheck ();

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad );
        bool   FindChip   (int &r , int &c , EN_CHIP_STAT Stat  );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

};

//---------------------------------------------------------------------------
extern CSortingTool STL;

#endif

