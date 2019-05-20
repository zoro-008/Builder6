//---------------------------------------------------------------------------

#ifndef SupplyH
#define SupplyH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "UtilDefine.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------


enum EN_LEFT_RIGHT {
    lrLeftBin  ,
    lrRightBin

};



class CSupply
{
    public:
        //Constructor
        CSupply (void);
        ~CSupply (void);

        void Init(EN_LEFT_RIGHT _iPartId );

        void Close();

        void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ; //파트가 할일이 없을경우 true ;
            bool bReqStop ; //에러가 나거나 혹은 스탑버튼을 눌렀을때 파트가 서기를 요청하는 프레그.
        };

        enum EN_SEQ_CYCLE {
            scIdle      =  0  ,
            scSupply    = 10  , //공급을 해야 할때.
            scSupplyAll = 20  , //모든 자제 제거 할때.
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

        SStat Stat ;
        SStep Step , PreStep ;

//        int  m_iPkgCnt[MAX_BIN_CNT];
        int  m_iSetBin ;  // 투하할것을 미리 세팅 함. -1이면 아무것도 아니고 0~7까지 빈넘버.
        bool m_bSetAll ;  // 모두 투하 하고 싶을때 세팅함.

        bool m_bOpenBin[MAX_BIN_CNT]  ;



        EN_ACTR_ID  aiSPL_Bn0 ; EN_INPUT_ID xSPL_Detect0 ;
        EN_ACTR_ID  aiSPL_Bn1 ; EN_INPUT_ID xSPL_Detect1 ;
        EN_ACTR_ID  aiSPL_Bn2 ; EN_INPUT_ID xSPL_Detect2 ;
        EN_ACTR_ID  aiSPL_Bn3 ; EN_INPUT_ID xSPL_Detect3 ;
        EN_ACTR_ID  aiSPL_Bn4 ; EN_INPUT_ID xSPL_Detect4 ;
        EN_ACTR_ID  aiSPL_Bn5 ; EN_INPUT_ID xSPL_Detect5 ;
        EN_ACTR_ID  aiSPL_Bn6 ; EN_INPUT_ID xSPL_Detect6 ;
        EN_ACTR_ID  aiSPL_Bn7 ; EN_INPUT_ID xSPL_Detect7 ;
        EN_ACTR_ID  aiSPL_Bn8 ; EN_INPUT_ID xSPL_Detect8 ;
        EN_ACTR_ID  aiSPL_Bn9 ; EN_INPUT_ID xSPL_Detect9 ;

        int   GetOpenBin(double _dVal);

        bool   FindChip   (int &r , int &c );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //모터가 가야할 포지션을 리턴함. Double형.

        bool   GetLastStrip(); //아직 파트에 어레이 데이터가 존재 할때만 유용. 즉 보내기전에 체크해야함.

    public:    /* Direct Accessable Var.  */

        int  m_iPkgCnt[MAX_BIN_CNT];
        bool m_bSetSupplyAll ;

        double dIndicator ;
//        double dRIndicator ;

        void SetBin   (int _iBin , bool _bAll) { m_iSetBin = _iBin ; m_bSetAll = _bAll; } //떨어뜨릴때 세팅해주는 함수.
        int  GetPkgCnt(int _iBin)              { return m_iPkgCnt[_iBin] ;              }

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //모터가 움직일때 충돌조건 감지.
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //실린더가 움직일때 충돌조건 감지.

        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){Step.iHome  = 10;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){Step.iCycle = 10;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat         GetStat        (){return Stat          ;}

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
        bool Update      (void); //업데이트 시에 타고 에러시에 상관없이 동작 하기 위해 넣음.

	    //One Cycle.
        bool CycleSupply    (void); //오퍼가 측정기로 찍으면 자제 공급.
        bool CycleSupplyAll (void); //오퍼가 측정기로 찍으면 자제 공급.

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad);

        static void procCom2(DWORD _cbInQue);
        static void procCom3(DWORD _cbInQue);
};

//---------------------------------------------------------------------------
extern CSupply SPL; //왼쪽빈
extern CSupply SPR; //오른쪽빈

//---------------------------------------------------------------------------
#endif
