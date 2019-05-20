//---------------------------------------------------------------------------

#ifndef WorkH
#define WorkH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
#define  MAX_BIN  5
enum EN_PRI_STEP {
    asNotReady   =  0 ,
    asNeedTrim        ,
    MAX_PRI_STEP
};

class CWork
{
    public:
        //Constructor
        CWork (void);
        ~CWork (void);
	void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ; //파트가 할일이 없을경우 true ;
            bool bReqStop ; //에러가 나거나 혹은 스탑버튼을 눌렀을때 파트가 서기를 요청하는 프레그.
        };

        enum EN_SEQ_CYCLE {
            scIdle   =  0  ,
            scWork   = 10  ,
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
        AnsiString  m_sPartName ;
        //Timer.
        CDelayTimer m_tmMain    ; //타임아웃용.
        CDelayTimer m_tmCycle   ; //타임아웃용.
        CDelayTimer m_tmHome    ; //타임아웃용.
        CDelayTimer m_tmToStop  ; //타임아웃용.
        CDelayTimer m_tmToStart ; //타임아웃용.
        CDelayTimer m_tmTemp    ; //싸이클 함수에서 막대려다 쓰는애.

        SStat Stat ;              //
        SStep Step , PreStep ;

        bool   FindChip   (int &r , int &c );
	    double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //모터가 가야할 포지션을 리턴함. Double형.

    public:    /* Direct Accessable Var.  */

        void ClearBin    (int _iBinNo );
        bool CheckBin    (int _iBinNo );
        bool CheckBinFull(int _iBinNo );


        unsigned long  m_lStrpCycleTime ; //한장 작업 시간.... 화면 디피용.
        unsigned long  m_lPreCycleTime  ;

        //빈통 작업 칩 갯수.
        int    m_iBinCnt  ;
        int    m_iBin1Cnt ;
        int    m_iBin2Cnt ;
        int    m_iBin3Cnt ;
        int    m_iBin4Cnt ;
        int    m_iBin5Cnt ;

        int    m_iBinNo         ;
        String m_sPreLotNo      ;
        int    m_iTotalChip     ; //이상혁 사장님 UI 에 맞추다 보니 생김. dd
        int    m_iTotalFailChip ;
        int    m_iTotalFrameCnt ;
        int    m_iTotalToolCnt  ;
        bool   m_bLotEnded      ;
        bool   m_bLotEndCheck   ;//다음 데이터 받기 위해JS

        CLot::SLotInfo EndedLot ;

        void Init ();
        void Close();



        bool   GetLastStrip();
        bool   LotInfoAdd  ();

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //모터가 움직일때 충돌조건 감지.
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //실린더가 움직일때 충돌조건 감지.

        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep    (){return PreStep.iHome       ;} void InitHomeStep (){Step.iHome  = 10;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep (){return PreStep.iToStart    ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep     (){return PreStep.iSeq        ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep   (){return PreStep.iCycle      ;} void InitCycleStep(){Step.iCycle    = 10;}

        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep  (){return PreStep.iToStop     ;}
        SStat         GetStat        (){return Stat          ;}


        void SetStat   (SStat _sStat) { Stat = _sStat ; }

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
        bool CycleWork   (void); //트림 한방 찍기.

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad);

};

//---------------------------------------------------------------------------
extern CWork WRK;

//---------------------------------------------------------------------------
#endif
