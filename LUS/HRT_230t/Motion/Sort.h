//---------------------------------------------------------------------------
#ifndef SortH
#define SortH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
class CSort
{
    public:
        //Constructor
         CSort ();
        ~CSort ();

        void Init      ();
        void Close     ();
        void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bSortMove   ;//Sort모터 움직이기 위한...JS
            bool bSortDetect ;
            bool bWorkEnd    ;
            bool bReqStop    ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scSort       = 10 ,
            scDetect     = 20 ,
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
        AnsiString  m_sPartName ;
        //Timer.
        CDelayTimer m_tmMain    ;
        CDelayTimer m_tmCycle   ;
        CDelayTimer m_tmHome    ;
        CDelayTimer m_tmToStop  ;
        CDelayTimer m_tmToStart ;
        CDelayTimer m_tmTemp    ;

        SStat Stat ;
        SStep Step , PreStep ;

        int m_iHomeTO  ;
        int m_iStartTO ;
        int m_iStopTO  ;
        int m_iCycleTO ;

        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c     );
//        double GetMotrPos (EN_MOTR_ID _iMotr  , EN_PSTN_ID _iPstnId );

        int  m_iCrntBin    ; //현재 빈        0 = WaitPos 1 = 1번빈.....
        int  m_iSetBin     ; //소팅할 빈 넘버. 1,2,3,4,5    Work에서 세팅함.
        bool m_bSetSort    ; //소팅할 빈 넘버. 1,2,3,4,5    Work에서 세팅함.
        bool m_bSetSortSub ; //소팅할 빈 넘버. 1,2,3,4,5    Work에서 세팅함.
        bool m_bUseDetect  ;

        int m_iBin1Cnt ;
        int m_iBin2Cnt ;
        int m_iBin3Cnt ;
        int m_iBin4Cnt ;
        int m_iBin5Cnt ;


    public:    /* Direct Accessable Var.  */
        double GetMotrPos (EN_MOTR_ID _iMotr  , EN_PSTN_ID _iPstnId );    
        bool   bFirstPkg ;
        bool   bSucDetect;
        bool   bNeedDetect;
        bool   bSortEnd   ;

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


        bool GetSortEnd () {return bSortEnd ;}



        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat        GetStat        (){return Stat          ;}

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
        bool Autorun   (void); //

        //One Cycle.
        bool CycleSort  ();
        bool CycleDetect();

        //For Start Stop Checking.
//        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        double GetLastCmd(EN_MOTR_ID _iMotr);
        bool   CheckMoved(void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool   CheckStop (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad);

        void SetBin     (int _iBinNo , bool _bWriteData = false);// , bool _bUseDetect = false );
        void SetSort    (bool _bRet );
        int  GetBin     (           );
        int  GetCntBin  (           );
        int  GetBinCnt  (int _iBinNo);
        void AddBinCnt  (int _iBinNo , int _iCnt);
        void ClearBinCnt(int _iBinNo);

};

//---------------------------------------------------------------------------
extern CSort SRT;
//---------------------------------------------------------------------------
#endif
