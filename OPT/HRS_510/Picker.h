//---------------------------------------------------------------------------

#ifndef PickerH
#define PickerH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "uRs232c.h"
#include "OptionMan.h"

//---------------------------------------------------------------------------
#define MAX_VALUE_GRAPH_CNT 40 //5미크론 씩 구분 한다.
#define MAX_BIN_GRAPH_CNT   10 //bin 0,1,2,3,4,5


class CPicker
{
    public:
        //Constructor
        CPicker (void);
        ~CPicker (void);

        void Init();
        void Close();
        void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd    ;
            bool bReqStop    ;
        };

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scRezero     = 10 ,
            scPick       = 20 ,
            scPlace      = 30 ,
            scCheck      = 40 ,
            scRezeroCheck= 50 ,
            MAX_SEQ_CYCLE
        };

        struct SStep {
            int          iHome    ;
            int          iToStart ;
            EN_SEQ_CYCLE iSeq     ;
            int          iCycle   ;
            int          iToStop  ;
        };

    protected:
        AnsiString  m_sPartName  ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmTemp    ;
        CDelayTimer m_tmTemp2   ;

        SStat Stat ;
        SStep Step , PreStep ;

        bool FindChip(EN_CHIP_STAT _iStat , int &r , int &c ) ;


	double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );



    public:    /* Direct Accessable Var.  */
        double m_dWorkCycleTime                 ;
        double m_dZeroPoint     [MAX_PICKER_CNT]; //리제로시에 무부하 상태의 값을 기억 했다가 바뀌면 적용 한다.
        double m_dZeroPointCheck[MAX_PICKER_CNT]; //리제로시에 무부하 상태의 값을 기억 했다가 바뀌면 적용 한다.
        //int    m_iPickCnt                       ; //픽 카운트 올라 가면서 리제로 수행
        int    m_iFindRow                       ;
        int    m_iFindCol                       ;


        EN_GRADE_STAT GetBin     (double _dVal);
        int           GetGraphBin(double _dVal);


        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //모터가 움직일때 충돌조건 감지.
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //실린더가 움직일때 충돌조건 감지.

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} int InitHomeStep (){ return Step.iHome  = 10 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} int InitCycleStep(){ return Step.iCycle = 10 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat        GetStat        (){return Stat          ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

	    //Motor.
	    bool MoveMotr (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // 모터를 움직일때 쓰는 함수.

	    //Actuator.
	    bool MoveActr (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //Running Functions.
	    bool ToStopCon   (void); //스탑을 하기 위한 조건을 보는 함수.
	    bool ToStartCon  (void); //스타트를 하기 위한 조건을 보는 함수.
	    bool ToStart     (void); //스타트를 하기 위한 함수.
	    bool ToStop      (void); //스탑을 하기 위한 함수.
        bool Autorun     (void); //오토런닝시에 계속 타는 함수.

        //One Cycle.
        bool CyclePick       (void); //자제 집는 사이클.
        bool CyclePlace      (void); //자제 내려놓고 검사하는 사이클.
        bool CycleRezero     (void); //장비 높이 게이지 리제로 하는 사이클. 메뉴얼.
        bool CycleRezeroCheck(void); //
        bool CycleCheck      (void); //자제 검증 모드 찝어서 검사하고 다시 가져다 놓음.
        bool CycleStation    (void); //자제를 스테이션에 놓고 높이만 측정하는 메뉴얼 동작. 한번 찝었다가 놓으면서 측정.

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad );

        static void procCom1(DWORD _cbInQue);


        void ClearGraphData(){memset(m_iValGraphCnt , 0 , sizeof(int)*MAX_VALUE_GRAPH_CNT);
                              memset(m_iBinGraphCnt , 0 , sizeof(int)*MAX_BIN_GRAPH_CNT  );}
        int m_iValGraphCnt[MAX_VALUE_GRAPH_CNT] ; //화면에 분포를 나타내주려는 데이터.
        int m_iBinGraphCnt[MAX_BIN_GRAPH_CNT  ] ; //화면에 뿌려주려고 만든 빈 카운트 데이터 .
        AnsiString m_sPreDate ;


        TRS232C * Rs232_H ;
        double m_dPrbData[MAX_PICKER_CNT] ;
        static void procComH(DWORD _cbInQue);





};

//---------------------------------------------------------------------------
extern CPicker PCK;

//---------------------------------------------------------------------------
#endif
