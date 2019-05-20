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
        void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;

        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle         = 0 ,
            scCstSupply    = 1 , //������ ����.
            scStgExpend    = 2 , //�ͽ����.
            scStgTAlign    = 3 , //������ T�����.
            scStgStartSetMc= 4 , //��ġ�� ù��ġ ã�ư��� Pick�Ҽ� �ְ� ����.
            scStgStartSetEg= 5 , //������ ���� Ȯ�� �Ͽ� ù��ġ ã�¤���.
            scStgNextMove  = 6 , //���� ���� ��ġ�� �̵�.
            scStgVision    = 7 , //�����˻� �� ����ġ�� �̵�.
            scCstPlace     = 8 , //������ ����.
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

//        //������ �ȿ� �ִ� ���������� Ȯ�� �ϴ� �Լ�. ���� ���� �ƴϰ� ������ ����.
//        bool IsWaferZone(double _dPosX , double _dPosY);
//        //������ �浹 ������.
//        bool IsSafeZone(double _dPosX , double _dPosY);

        //������� ģ�� �� ������ �� ���� �ִ� ���� ��ġ�� �˻�.
        double GetLeftEnd  (double _dCmdPosX,double _dCmdPosY);
        double GetRightEnd (double _dCmdPosX,double _dCmdPosY);
        double GetTopEnd   (double _dCmdPosX,double _dCmdPosY);
        double GetBottomEnd(double _dCmdPosX,double _dCmdPosY);





        bool VisnRsltLmtCheck(TRetResult * _tRsltbool);
        
        public:

        bool GetNextPos (double  _PckX , double  _PckY , double  _PckT , //Last Pick Position.
                         double &_RltX , double &_RltY , double &_RltT );//Destination Value.
        protected:


        //�Ǹ����� ������ ���������� ��,��.
        int m_iLastPickRow ;
        int m_iLastPickCol ;

        //�Ǹ����� ������ ���������� ��ġ. NextPos ���� �Ҷ� �ʿ�.
        double m_dLastWorkPosX ;
        double m_dLastWorkPosY ;
        double m_dLastWorkPosT ;

        //Ĩ����νÿ� ���� Ĩ ������.
        double m_dCntChipPosX  ;
        double m_dCntChipPosY  ;
        double m_dCntChipPosT  ;

        //��ŸƮ ������  �����V.
        //double m_dSttOffsetX ;
        //double m_dSttOffsetY ;

        bool m_bWfrTAligned ; //������ ��Ÿ �� XY���� �ߴ���.
        bool m_bStartSet    ; //�ʱ� ��ġ Ȯ�� �ߴ���.

        bool m_bRvsDirction ; //������ ���� ���� �ݴ�� ����.

    public:    /* Direct Accessable Var.  */
        //
        bool GetAdjustPos(double  _EncX , double  _EncY , double  _EncT , //���ڴ� ����....//�����.
                          double  _OfsX , double  _OfsY , double  _OfsT , //���� ��� �����V����.
                          double &_RltX , double &_RltY , double &_RltT , //�����. <===�̰� ������ �����ָ� �ȴ�.
                          bool    _bUseTheta);                            //��Ÿ���� ��� ����.

        bool   m_bDispStartForm ; //��ŸƮ ������ ����.

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

        //������ �ȿ� �ִ� ���������� Ȯ�� �ϴ� �Լ�. ���� ���� �ƴϰ� ������ ����.
        bool IsWaferZone(double _dPosX , double _dPosY);
        //������ �浹 ������.
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
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed = false); // ���͸� �����϶� ���� �Լ�.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //Running Functions.
        bool ToStopCon (void); //��ž�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStartCon(void); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStart   (void); //��ŸƮ�� �ϱ� ���� �Լ�.
        bool ToStop    (void); //��ž�� �ϱ� ���� �Լ�.
        bool Autorun   (void); //

        bool FindFrstChip(int &r , int &c);

        //One Cycle.
        bool CycleCstSupply      (void); //������ ����.
        bool CycleStgExpend      (void); //�ͽ����.
        bool CycleStgTAlign      (void); //�� ��� �϶� ����� Ȯ��.
        bool CycleStgStartSetMc  (void); //ù��ġ ã�ư�. ��ġ �̿�.
        bool CycleStgStartSetEg  (void); //ù��ġ ã�ư�. ������ �ܰ� ã��.  ������ ��ġ �ڵ� ���� ���.
        bool CycleStgNextMove    (void); //�������� ��ġ�� �̵�.
        bool CycleStgVision      (void); //�����˻� �� ����ġ�� �̵�.
        bool CycleCstPlace       (void); //������ ����.

        bool CycleStepMove (bool _bInit , int _iIncR , int _iIncC , int _iStep);
        bool CycleVisn     (EN_VISN_ID _Id , bool _bInit , bool _bDryRun) ;


        //�޴��� ����Ŭ.
        bool CycleCenterMove  (void); //Ĩ����� ó�� ����� ���°�.
        bool CycleUnExpend    (void); //������ Ǯ��.
        

        //ManCycle
        //bool CycleCsSupply (void);
        //bool CycleStgExpend(void);
        //bool CycleStgVsWork(void);
        //bool CycleStgWork  (void);
        //bool CycleCsPlace  (void);
        //bool MC_SttVsTest  (void);

        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool IsLoad);

        //������ ȣ���.
        double GetTOffset  (){return RsltWfr.fRsltT ;}

};

//---------------------------------------------------------------------------
extern CStage STG;
//---------------------------------------------------------------------------
#endif
