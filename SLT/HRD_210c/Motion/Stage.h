//---------------------------------------------------------------------------
#ifndef StageH
#define StageH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
//Vision
//---------------------------------------------------------------------------
#include "SAInterface.h"
//---------------------------------------------------------------------------

class CStage
{
    public:
        //Constructor
         CStage ();
        ~CStage ();

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
            scIdle         = 0 ,
            scCstSupply    = 1 , //웨이퍼 공급.
            scStgExpend    = 2 , //익스펜딩.
            scStgTAlign    = 3 , //웨이퍼 T얼라인.
            scStgStartSetMc= 4 , //매치로 첫위치 찾아가고 Pick할수 있게 해줌.
            scStgStartSetEg= 5 , //웨이퍼 끝을 확인 하여 첫위치 찾는ㄷ다.
            scStgNextMove  = 6 , //다음 자제 위치로 이동.
            scStgVision    = 7 , //비전검사 및 정위치로 이동.
            scCstPlace     = 8 , //웨이퍼 배출.
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName [MAX_SEQ_CYCLE] ;
        void InitCycleName(){
            m_sCycleName[scIdle         ]="Idle         ";
            m_sCycleName[scCstSupply    ]="CstSupply    ";
            m_sCycleName[scStgExpend    ]="StgExpend    ";
            m_sCycleName[scStgTAlign    ]="StgTAlign    ";
            m_sCycleName[scStgStartSetMc]="StgStartSetMc";
            m_sCycleName[scStgStartSetEg]="StgStartSetEg";
            m_sCycleName[scStgNextMove  ]="StgNextMove  ";
            m_sCycleName[scStgVision    ]="StgVision    ";
            m_sCycleName[scCstPlace     ]="CstPlace     ";
        }

        struct SStep {
            int          iHome    ;
            int          iToStart ;
            EN_SEQ_CYCLE iSeq     ;
            int          iCycle   ;
            int          iToStop  ;

            int          iStepMove;
            //Vision.
            int          iVisn    ;
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

        //Vision.
        CDelayTimer m_tmVisn     ;
        CDelayTimer m_tmVisnTemp ;

        SStat Stat ;
        SStep Step , PreStep ;

        TRetResult RsltWfr  ;
        TRetResult RsltWfo  ;

        double m_dLastIdxPos ;

        int m_iHomeTO  ;
        int m_iStartTO ;
        int m_iStopTO  ;
        int m_iCycleTO ;

        double GetMotrPos    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

//        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c , EN_CHIP_STAT _csStats = csUnkwn);
        //bool SetAjustPos (TRetResult * _tRsltbool , bool _bUseTheta = false);
        //bool   MC_SetAjustPos(TRetResult * _tRsltbool , bool _bUseTheta , double &_dX , double &_dY ,double &_dT );

//        //웨이퍼 안에 있는 포지션인지 확인 하는 함수. 주의 림이 아니고 웨이퍼 기준.
//        bool IsWaferZone(double _dPosX , double _dPosY);
//        //웨이퍼 충돌 포지션.
//        bool IsSafeZone(double _dPosX , double _dPosY);

        //얼라인을 친후 각 포지션 맨 끝에 있는 자제 위치를 검색.
        double GetLeftEnd  (double _dCmdPosX,double _dCmdPosY);
        double GetRightEnd (double _dCmdPosX,double _dCmdPosY);
        double GetTopEnd   (double _dCmdPosX,double _dCmdPosY);
        double GetBottomEnd(double _dCmdPosX,double _dCmdPosY);





        bool VisnRsltLmtCheck(TRetResult * _tRsltbool);
        
        public:

        bool GetNextPos (double  _PckX , double  _PckY , double  _PckT , //Last Pick Position.
                         double &_RltX , double &_RltY , double &_RltT );//Destination Value.
        protected:


        //맨마지막 툴에서 픽했을때의 행,열.
        int m_iLastPickRow ;
        int m_iLastPickCol ;

        //맨마지막 툴에서 픽했을때의 위치. NextPos 결정 할때 필요.
        double m_dLastWorkPosX ;
        double m_dLastWorkPosY ;
        double m_dLastWorkPosT ;

        //칩얼라인시에 센터 칩 포지션.
        double m_dCntChipPosX  ;
        double m_dCntChipPosY  ;
        double m_dCntChipPosT  ;

        //스타트 포지션  오프셑.
        //double m_dSttOffsetX ;
        //double m_dSttOffsetY ;

        bool m_bWfrTAligned ; //웨이퍼 쎄타 및 XY보정 했는지.
        bool m_bStartSet    ; //초기 위치 확인 했는지.

        bool m_bRvsDirction ; //웨이퍼 진행 방향 반대로 설정.

    public:    /* Direct Accessable Var.  */
        //
        bool GetAdjustPos(double  _EncX , double  _EncY , double  _EncT , //엔코더 값들....//계원꺼.
                          double  _OfsX , double  _OfsY , double  _OfsT , //비젼 결과 오프셑값들.
                          double &_RltX , double &_RltY , double &_RltT , //결과값. <===이걸 가지고 때려주면 된다.
                          bool    _bUseTheta);                            //세타보정 사용 여부.

        bool   m_bDispStartForm ; //스타트 포지션 세팅.

        int    GetLastPickRow();
        int    GetLastPickCol();
        double GetLastPickPosX();
        double GetLastPickPosY();

        void SetLastPickRow(int _iRow);
        void SetLastPickCol(int _iCol);
        void SetLastPickPosX(double _dX);
        void SetLastPickPosY(double _dY);


        void   SetStartSet   (bool _bOn  ) { m_bStartSet    = _bOn;}
        void   SetWfrTAligned(bool _bOn  ) { m_bWfrTAligned = _bOn;}

        bool   GetStartSet   (           ) { return m_bStartSet   ;}
        bool   GetWfrTAligned(           ) { return m_bWfrTAligned;}

        void   SetDirction  (bool _bLeft) ;

        //웨이퍼 안에 있는 포지션인지 확인 하는 함수. 주의 림이 아니고 웨이퍼 기준.
        bool IsWaferZone(double _dPosX , double _dPosY);
        //웨이퍼 충돌 포지션.
        bool IsSafeZone(double _dPosX , double _dPosY);

        double GetWorkPos (EN_MOTR_ID _iMotr) ;
        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c , EN_CHIP_STAT _csStats = csUnkwn);

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStat        GetStat        (){return Stat          ;}
        AnsiString   GetCycleName   (){return m_sCycleName[Step.iSeq];}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Property.
//        __property int   _iMgzCnt      = { read = m_iMgzCnt    , write= m_iMgzCnt };
        __property double _dLastWorkPosX    = { read = m_dLastWorkPosX    , write= m_dLastWorkPosX };
        __property double _dLastWorkPosY    = { read = m_dLastWorkPosY    , write= m_dLastWorkPosY };
        __property double _dLastWorkPosT    = { read = m_dLastWorkPosT    , write= m_dLastWorkPosT };


        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed = false); // 모터를 움직일때 쓰는 함수.
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

        bool FindFrstChip(int &r , int &c);

        //One Cycle.
        bool CycleCstSupply      (void); //웨이퍼 공급.
        bool CycleStgExpend      (void); //익스펜딩.
        bool CycleStgTAlign      (void); //맵 모드 일때 얼라인 확인.
        bool CycleStgStartSetMc  (void); //첫위치 찾아감. 매치 이용.
        bool CycleStgStartSetEg  (void); //첫위치 찾아감. 웨이퍼 외각 찾음.  웨이퍼 피치 자동 보정 기능.
        bool CycleStgNextMove    (void); //다음자제 위치로 이동.
        bool CycleStgVision      (void); //비전검사 및 정위치로 이동.
        bool CycleCstPlace       (void); //웨이퍼 배출.

        bool CycleStepMove (bool _bInit , int _iIncR , int _iIncC , int _iStep);
        bool CycleVisn     (EN_VISN_ID _Id , bool _bInit , bool _bDryRun) ;


        //메뉴얼 싸이클.
        bool CycleCenterMove  (void); //칩얼라인 처서 가운데로 오는것.
        bool CycleUnExpend    (void); //웨이퍼 풀기.
        

        //ManCycle
        //bool CycleCsSupply (void);
        //bool CycleStgExpend(void);
        //bool CycleStgVsWork(void);
        //bool CycleStgWork  (void);
        //bool CycleCsPlace  (void);
        //bool MC_SttVsTest  (void);

        //For Start Stop Checking.
        void SetLastCmd (void); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (void); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (void); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool IsLoad);

        //툴에서 호출용.
        double GetTOffset  (){return RsltWfr.fRsltT ;}

};

//---------------------------------------------------------------------------
extern CStage STG;
//---------------------------------------------------------------------------
#endif
