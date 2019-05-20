//---------------------------------------------------------------------------

#ifndef SortH
#define SortH
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Timer.h"
#include "DataMan.h"

enum EN_VAC_ERR_STAT { //현재 랏 런 하고 있는 상태.
    vsNomal            = 0 , //에러났던 곳을 다시 픽하고 안찝히면 다시 에러.
    vsErrSkipPick      = 1 , //픽했던데 다시 픽하고 없으면 그냥 다음 픽으로 넘어감 -> vsNomal.
    vsErrSkipRow       = 2 , //픽했던데 다시 픽하고 해당 에러열에서는 에러 스킵 -> vsNomal.
    vsErrSkipTray      = 3 , //픽했던데 다시 픽하고 해당 트레이에서는 에러 스킵 -> vsNomal.
    vsErrSkipRowOut    = 4 , //픽했던열 스킵으로 확인 하고 트레이 아웃.
    vsErrSkipRowOutLE  = 5 , //픽했던열 스킵으로 확인하고 트레이 아웃후 랏엔드요청.

    //이것은 클릭시에 바로 처리 되서 필요 없다.  vsTrayOut          = 6 , //현재 트레이 모두 Empty 시키기.
    //이것은 클릭시에 바로 처리 되서 필요 없다.  vsTrayOutLE        = 7 , //트레이 아웃 시키고 랏앤드 요청.

};


//---------------------------------------------------------------------------
class CSort : public CPartInterface
{
    public: //초기화 등등.
        //Constructor
        CSort ();
        ~CSort ();

        void Init();
        void Close();


    public:

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle    =  0 ,
            scPickSTG =  1 ,
            scPlceRJT =  2 ,
            scPlceULD =  3 ,
            scPickZIG =  4 ,
            scPlceSTG =  5 ,
            scPickLDR =  6 ,
            scPlceZIG =  7 ,
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //마지막 사이클 시간. ms
        void   InitCycleName(){
            m_sCycleName[scIdle   ]="Idle   ";
            m_sCycleName[scPickSTG]="PickSTG";
            m_sCycleName[scPlceRJT]="PlceRJT";
            m_sCycleName[scPlceULD]="PlceULD";
            m_sCycleName[scPickZIG]="PickZIG";
            m_sCycleName[scPlceSTG]="PlceSTG";
            m_sCycleName[scPickLDR]="PickLDR";
            m_sCycleName[scPlceZIG]="PlceZIG";
        }
        void   InitCycleTime(){
            memset(&m_iCycleTime  , 0 , sizeof(int )*MAX_SEQ_CYCLE);
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
        CDelayTimer m_tmDelay   ;

        SStat Stat ;
        SStep Step , PreStep ;

        SStat GetStat(){return Stat;}

        void ResetTimer();

        bool   FindChip   (EN_ARAY_ID _iArray , EN_CHIP_STAT _iStat , int &r , int &c   );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );



    public: //인터페이스 상속.==================================================
	void Reset (); //리셑 버튼 눌렀을때 타는 함수.

        //Running Functions.
        bool ToStopCon (); //스탑을 하기 위한 조건을 보는 함수.
        bool ToStartCon(); //스타트를 하기 위한 조건을 보는 함수.
        bool ToStart   (); //스타트를 하기 위한 함수.
        bool ToStop    (); //스탑을 하기 위한 함수.

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome  = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int          GetSeqStep     (){return (int)Step.iSeq;} int GetPreSeqStep    (){return (int)PreStep.iSeq;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}

        AnsiString   GetCycleName   (int _iSeq){return m_sCycleName[_iSeq];}
        int          GetCycleTime   (int _iSeq){return m_iCycleTime[_iSeq];}
        AnsiString   GetPartName    (){return m_sPartName   ;}

        int          GetCycleMaxCnt (){return MAX_SEQ_CYCLE ;}

        bool Autorun   (); //오토런닝시에 계속 타는 함수.
        //======================================================================
    public:

        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //One Cycle.
        bool CyclePickSTG();
        bool CyclePlceRJT();
        bool CyclePlceULD();
        bool CyclePickZIG();
        bool CyclePlceSTG();
        bool CyclePickLDR();
        bool CyclePlceZIG();


        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // 모터를 움직일때 쓰는 함수.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // 모터를 움직일때 쓰는 함수.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.



        //For Start Stop Checking.
        void SetLastCmd (); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool _bLoad);










        //소팅 인포=============================================================
        struct TSortInfo{
            bool         bFindOk     ; //작업가능여부.
            bool         isPick      ; //픽용인지 플레이스 용인지.
            int          iCol        ; //iARAY의 작업할 영역의 왼쪽
            int          iRow        ; //iARAY의 작업할 영역의 제일위쪽.
            EN_CHIP_STAT iTargetStat ; //목적 칩 상태.
            EN_ARAY_ID   iAray       ; //작업할 어레이
            EN_ARAY_ID   iTool       ; //피커툴 어레이.
            int          iToolShft   ; //툴의 첫번째 컬럼 부터 작업이 안될경우 쉬프트 된다.
            int          iDnCnt      ; //찝거나 내려놓는 칩 갯수.
            bool         bDn[MAX_TOOL_SOCK_COL] ;
        };
        TSortInfo SortInfo ;
        //트레이에서 찝거나 트레이에 놓을때 사용 하는 함수.
        bool GetTraySortInfo  (bool _bPick , EN_ARAY_ID _iTool , EN_ARAY_ID _iAray , TSortInfo & _tInfo);
        bool ShiftTraySortInfo(TSortInfo & _tInfo , bool _bVacErr1 = false , bool _bVacErr2 = false ,bool _bVacErr3 = false ,bool _bVacErr4 = false );

        //지그나 소켓에서 찝거나 놓을때 사용 하는 함수.
        bool GetSockSortInfo  (bool _bPick , EN_ARAY_ID _iTool , EN_ARAY_ID _iAray , TSortInfo & _tInfo);
        bool ShiftSockSortInfo(TSortInfo & _tInfo , bool _bVacErr1 = false , bool _bVacErr2 = false ,bool _bVacErr3 = false ,bool _bVacErr4 = false );

        //========================================================================

        //베큠에러
        EN_VAC_ERR_STAT m_iVacErrStat ;
        bool            m_bVacErr[MAX_TOOL_SOCK_COL] ;

        bool CheckRetryStgPick();
        bool CheckRetryZigPick();


        bool m_bRetryZigPick ; //픽 미스 에러 났을때 리트라이 선택시..
        bool m_bRetryStgPick ;

        bool m_bLdrPickErrDisp ; //로더에서 픽 미스 났을때 선택창 디스플레이
        bool m_bZigPickErrDisp ; //지그에서 픽 미스 났을때 선택창 디스플레이
        bool m_bStgPickErrDisp ; //스테이지에서 픽 미스 났을때 선택창 디스플레이

        double m_dTickTime;


        void Test();
};

//---------------------------------------------------------------------------
extern CSort SRT;
//---------------------------------------------------------------------------
#endif

