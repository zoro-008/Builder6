//---------------------------------------------------------------------------

#ifndef WorkZoneH
#define WorkZoneH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------
#define  MAXBIN  5
enum EN_WRK_STEP {
    asWRK_ToWrkStt    =  0  ,
    asWRK_1IdxWork          ,
    asWRK_To2Idx            ,
    asWRK_2IdxWorkStt       ,
    asWRK_2IdxWork          ,
    asWRK_2IdxReWork        ,
    asWRK_2IdxPull          ,
    asWRK_2IdxOut           ,
    asWRK_None              ,

    MAX_WRK_STEP
};

class CPreWork
{
    public:
        //Constructor
        CPreWork (void);
        ~CPreWork (void);
	void Reset     (); //리셑 버튼 눌렀을때 타는 함수.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ; //파트가 할일이 없을경우 true ;
            bool bReqStop ; //에러가 나거나 혹은 스탑버튼을 눌렀을때 파트가 서기를 요청하는 프레그.
        };

        enum EN_SEQ_CYCLE {
            scIdle        =  0  ,
            scWrkStt      = 10  ,
            sc1IdxWork    = 20  ,
            scTo2Idx      = 30  ,
            sc2IdxWorkStt = 40  ,
            sc2IdxWork    = 50  ,
            sc2IdxReWork  = 60  ,
            sc2IdxOut     = 70  ,
            MAX_SEQ_CYCLE
        };

        struct SStep {
            int          iHome    ; //홈싸이클에서 쓰는 스텝.
            int          iToStart ; //투스타트 싸이클에서 씀.
            EN_SEQ_CYCLE iSeq     ; //장비의 할일 결정.
            int          iCycle   ; //장비의 할일에서 스텝.
            int          iToStop  ; //투스탑 싸이클에서 씀.

//            EN_SEQ_CYCLE iSubSeq  ; //Sub Autorun 하나더 돌릴라고 사용해봄.dd
//            int          iSubCycle; //Sub
        };

        struct SBinInfo {
            int  iBinCnt ;
            bool bBinFull;
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

        CDelayTimer m_tmSubCycle; //Sub타임아웃용.         

        SStat Stat ;              //
        SStep Step , PreStep ;
//        SStep SubStep , SubPreStep ;

        double     m_dLast1IdxPos  ;
        double     m_dLast2IdxPos  ;
        double     m_dLastXSrtPos  ;
        //        bool m_bWaitLotChange ;

        bool   FindChip   (int &r , int &c );
	double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //모터가 가야할 포지션을 리턴함. Double형.

    public:    /* Direct Accessable Var.  */

        SBinInfo Bin1 , Bin2 ,Bin3 , Bin4 , Bin5 ;

        bool           m_bLotEnded  ; //화면에 랏정보 폼 띄우기용.
        int            iGoodCnt ;

        unsigned long  m_lStrpCycleTime ; //한장 작업 시간.... 화면 디피용.
        unsigned long  m_lPreCycleTime  ;

        int m_iBinCnt ;
        String m_sLastStrip ;
        String m_sPreLotNo  ;
        double m_dFailChipPos ;

//        int iBin1Cnt , iBin2Cnt , iBin3Cnt , iBin4Cnt , iBin5Cnt ;

        CLot::SLotInfo EndedLot ;

        void Init ();
        void Close();
        
        double RejectMove  (void);
        bool   BinFullCheck(    );
        bool   BinInfoAdd  (    );

        bool   GetLastStrip();
        bool   LotInfoAdd  ();

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId ); //모터가 움직일때 충돌조건 감지.
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    ); //실린더가 움직일때 충돌조건 감지.

        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep    (){return PreStep.iHome       ;} void InitHomeStep (){Step.iHome  = 10;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep (){return PreStep.iToStart    ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep     (){return PreStep.iSeq        ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep   (){return PreStep.iCycle      ;} void InitCycleStep   (){Step.iCycle    = 10;}

        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep  (){return PreStep.iToStop     ;}
        SStat         GetStat        (){return Stat          ;}

        EN_SEQ_CYCLE  GetSubSeqStep  (){return Step.iSubSeq  ;} int GetPreSubSeqStep  (){return PreStep.iSubSeq     ;}
        int           GetSubCycleStep(){return Step.iSubCycle;} int GetPreSubCycleStep(){return PreStep.iSubCycle   ;} void InitSubCycleStep(){Step.iSubCycle = 10;}

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
        bool CycleToWrkStt   (void); //
        bool Cycle1IdxWork   (void); //
        bool CycleTo2Idx     (void); //
        bool Cycle2IdxWorkStt(void); //
        bool Cycle2IdxWork   (void); //
        bool Cycle2IdxReWork (void); //
        bool Cycle2IdxOut    (void); //
                
        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad);

};

//---------------------------------------------------------------------------
extern CPreWork WRP;

//---------------------------------------------------------------------------
#endif
