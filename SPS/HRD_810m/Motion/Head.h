//---------------------------------------------------------------------------
#ifndef HeadH
#define HeadH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
#include "QuedataUnit.h"
//---------------------------------------------------------------------------
//Vision
//---------------------------------------------------------------------------
#include "VisionMan.h"
//---------------------------------------------------------------------------

class CHead
{
    public:
        //Constructor
         CHead ();
        ~CHead ();

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
            scIdle      = 0 ,
            scCal       = 1 , //���� �̸��극�̼�.
            scPenCheck  = 2 , //�� �������� ȭ��Ʈ ���� ���� ���� üũ.
            scTAlign    = 3 , //���� �簢������ ������ ��Ÿ ����� ��°�.
            scOrigin    = 4 , //���� ��ġ�� ���� Ĩ ã��.
            scChipAlign = 5 , //���� �簢������ Ĩ ����� ��ġ ����.
            scMark      = 6 , //�渶ŷ & ��쿡 ���� ��� ���� Ȯ�� ����.
            scWaitPos   = 7 , //������ ���ų� ������ �浹 ���� �ʱ� ���� ���� ��� ��ġ�� ���ִ´�.
            scMarkTrace = 8 , //�渶ũ �˻� ���Ͻÿ� ������..
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName [MAX_SEQ_CYCLE] ;
        void InitCycleName(){                                            
            m_sCycleName[scIdle     ]="Idle     ";                       
            m_sCycleName[scCal      ]="Cal      ";                       
            m_sCycleName[scPenCheck ]="PenCheck ";                       
            m_sCycleName[scTAlign   ]="TAlign   ";                       
            m_sCycleName[scOrigin   ]="Origin   ";                       
            m_sCycleName[scChipAlign]="ChipAlign";
            m_sCycleName[scMark     ]="Mark     ";
            m_sCycleName[scWaitPos  ]="WaitPos  ";
            m_sCycleName[scMarkTrace]="MarkTrace";
        }

        struct SCycleTime {
            double dStart ; //����Ŭ ��ŸƮ.
            double dPitch ; //����Ŭ ���� ����.
            double dWork  ; //����Ŭ �ҿ�ð�.
        };



        struct SStep {
            int          iHome    ;
            int          iToStart ;
            EN_SEQ_CYCLE iSeq     ;
            int          iCycle   ;
            int          iToStop  ;

            //Vision.
            int          iVisn    ;
        };

        struct SRqst { //�۾���û.
            bool bNeedTAlign    ; //����� �۾� �ʿ�.                          
            bool bNeedOrigin    ; //������ �۾� �ʿ�.                          
            bool bNeedCal       ; //�̸��극�̼� �ʿ�.                         
            bool bNeedPenCheck  ; //ȭ��Ʈ ���忡�� ���������� üũ �۾� �ʿ�.
            bool bNeedMarkTrace ; //�� ��ŷ �ȵǾ� �������� �� ���� �� ������ ã�ư�.
        }Rqst;
        int m_iMarkTraceCnt ; //��ŷ �ڱ� ������ �ϴ� Ƚ��.

        int m_iPenDnCnt ; //���� �۾� Ƚ��. //��üũ �ϸ� Ŭ���� �ȴ�.
        int m_iTotalPenDnCnt ; //���� ���� ���۾���... �汳ü�� ���� ����.


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

        SCycleTime Time[MAX_SEQ_CYCLE] ;

        CVisionMan::TRetResult VsRslt ;

        int m_iHomeTO  ;
        int m_iStartTO ;
        int m_iStopTO  ;
        int m_iCycleTO ;

        void SetStartTime(EN_SEQ_CYCLE _iSeqCycle);
        void SetEndTime  (EN_SEQ_CYCLE _iSeqCycle);

        bool GetAdjustPos(double  _CrtX , double  _CrtY , double  _CrtT , //���ڴ� ����....//�����.
                          double  _OfsX , double  _OfsY , double  _OfsT , //���� ��� �����V����.
                          double &_RltX , double &_RltY , double &_RltT , //�����. <===�̰� ������ �����ָ� �ȴ�.
                          bool    _bUseTheta);                            //��Ÿ���� ��� ����.


        bool   FindChip   (EN_ARAY_ID _riWhre , int &r , int &c , EN_CHIP_STAT _csStat);
        //double GetWorkPos (EN_MOTR_ID _iMotr);
    public:     double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );




    public:    /* Direct Accessable Var.  */
        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


        bool GetWorkEnd (){return Stat.bWorkEnd ;}
        bool GetReqStop (){return Stat.bReqStop ;}

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ; }
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        AnsiString   GetCycleName   (){return m_sCycleName[Step.iSeq];}
        SStat        GetStat        (){return Stat          ;}

        
        void SetStat(SStat _sStat) { Stat = _sStat ; }

        double GetTimePitch (EN_SEQ_CYCLE _iSeqCycle){return Time[_iSeqCycle].dPitch ; }
        double GetTimeWork  (EN_SEQ_CYCLE _iSeqCycle){return Time[_iSeqCycle].dWork  ; }


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

        //One Cycle.
        bool CycleCal       (void); //���� �̸��극�̼�.
        bool CyclePenCheck  (void); //�� �������� ȭ��Ʈ ���� ���� ���� üũ.
        bool CycleTAlign    (void); //���� �簢������ ������ ��Ÿ ����� ��°�.
        bool CycleOrigin    (void); //���� ��ġ�� ���� Ĩ ã��.
        bool CycleChipAlign (void); //���� �簢������ Ĩ ����� ��ġ ����.
        bool CycleMark      (void); //�渶ŷ & ��쿡 ���� ��� ���� Ȯ�� ����.
        bool CycleWaitPos   (void); //��� ��ġ�� ����.
        bool CyclePenChange (void); //�� ��ü ��ġ�� ����.
        bool CycleMarkTrace (void); //��ũ �ϰ� ���� ��������� ���������� ã�ư�.
        bool CyclePenCoverPs(void); //�� Ŀ�� ��ġ�� �̵��� �Ǹ��� ������.

        //Vision Cycle
        bool CycleVisn     (EN_VISN_ID _iId , bool _bInit = false);
        //viWfr        = 0 , //Ĩ
        //viWfo            , //�Ǵϼȸ�ũ
        //viWfp            , //�� �˻�.
        //viCal            , //Ķ���극�̼�.

        //Manual Cycle.
        //....
        bool CycleClickMove();
        bool CycleCntrMove ();


        //Click CallBackMove

        void SetClickMove(int _iCol ,int _iRow , TShiftState Shift);
        int  m_iClickMoveCol;
        int  m_iClickMoveRow;



        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        double GetWaferPitchX  (double _dOffsetX);
        double GetWaferPitchY  (double _dOffsetY);

        //Loading Para.
        void Load(bool IsLoad);

        int  m_iLastInspChipR;
        int  m_iLastInspChipC;

        int  m_iLastMrkChipR ;
        int  m_iLastMrkChipC ;

        bool m_bNeedOriDataSet ; //���������̼�Ĩ �޴��� ���� �ʿ�...

        CQueData MarkData ;

        bool m_bNeedWideWfrChipInsp ;//Ĩ �˻縦 �������� �Ҷ� �̿�.
        bool m_bNeedWideWfoChipInsp ; //

        double m_dRevisePitchX ;
        double m_dRevisePitchY ;
        bool   m_bFnshCycTAlgn ;


        /*���� �׽�Ʈ��.
        double m_dTempPosX ;
        double m_dTempPosY ;
        */

};

//---------------------------------------------------------------------------
extern CHead HED;
//---------------------------------------------------------------------------
#endif
