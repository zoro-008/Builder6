//---------------------------------------------------------------------------

#ifndef PickerH
#define PickerH
//---------------------------------------------------------------------------
#include "PartInterface.h"
#include "Rs232LoadCl.h"

#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------
class CPicker : public CPartInterface
{
    public: //�ʱ�ȭ ���.
        //Constructor
        CPicker ();
        ~CPicker ();

        void Init();
        void Close();


    public:

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scPick            ,
            scAlign           ,
            scAlignPick       ,
            scVisn            ,
            scAttach          ,
            scPush            ,
            scBoatPlace       ,
            scVoid            , //ȸ��..

            scManStdCal       ,
            scManVsDefectPos  ,
            scManVsAlignPos   ,
            scManAlgnPlce     ,
            scManAlignerCal   ,

            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
        void   InitCycleName(){
            m_sCycleName[scIdle          ]="Idle"       ;
            m_sCycleName[scPick          ]="Pick"       ;
            m_sCycleName[scAlign         ]="Align"      ;
            m_sCycleName[scAlignPick     ]="AlignPick"  ;
            m_sCycleName[scVisn          ]="Visn"       ;
            m_sCycleName[scAttach        ]="Attach"     ;
            m_sCycleName[scPush          ]="Push"       ;
            m_sCycleName[scBoatPlace     ]="BoatPlace"  ;
            m_sCycleName[scVoid          ]="Void"       ;

            m_sCycleName[scManStdCal     ]="ManStdCal"  ;
            m_sCycleName[scManVsDefectPos]="ManVsDefectPos";
            m_sCycleName[scManVsAlignPos ]="ManVsAlignPos ";
            m_sCycleName[scManAlgnPlce   ]="ManAlgnPlce ";
            m_sCycleName[scManAlignerCal ]="ManAlignerCal";  

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
            EN_SEQ_CYCLE iLastSeq ;
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

        SStat  Stat ;
        SStep  Step , PreStep ;

        double m_dLastIdxPos ;

        String m_sCheckSafeMsg ;

        bool   m_bVisn  ;
        double m_dVisnX , m_dVisnY , m_dVisnT ;
        int    m_iVisnDefCnt ;

        double GetLastCmd(EN_MOTR_ID _iMotr);

        void   ResetTimer();

        bool   FindChip   (int &c , EN_ARAY_ID &id);

        bool   m_bOneSkipDefect;

        
        double m_dEmptyToolHeight; //���� ������ ����.
        double m_dPkPkgThickness ; //Ű���� ������� ������ pkg �β�.

    public: //�������̽� ���.==================================================
	void   Reset (); //���V ��ư �������� Ÿ�� �Լ�.

        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        
        double GetPkgThickness   () ; //Ű���� ������� ������ pkg �β�.
        double GetEmptyToolHeight() ;
        double GetVisnDataX();
        double GetVisnDataY();
        int    GetVisnDefectCnt();

        //Running Functions.
        void Update    ();
        bool ToStopCon (); //��ž�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStartCon(); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStart   (); //��ŸƮ�� �ϱ� ���� �Լ�.
        bool ToStop    (); //��ž�� �ϱ� ���� �Լ�.

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome  = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int          GetSeqStep     (){return (int)Step.iSeq;} int GetPreSeqStep    (){return (int)PreStep.iSeq;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        SStep        GetStep        (){return Step          ;}

        AnsiString   GetCycleName   (int _iSeq){return m_sCycleName[_iSeq];}
        int          GetCycleTime   (int _iSeq){return m_iCycleTime[_iSeq];}
        AnsiString   GetPartName    (){return m_sPartName   ;}

        int          GetCycleMaxCnt (){return MAX_SEQ_CYCLE ;}

        bool         Autorun        (); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
        //======================================================================

    public:
    
        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //One Cycle.
        bool CyclePick      ();
        bool CycleAlign     ();
        bool CycleAlignPick ();
        bool CycleVisn      ();
        bool CycleAttach    ();
        bool CyclePush      ();
        bool CycleBoatPlace ();
        bool CycleVoid      ();

        bool CycleManStdCal ();
        bool CycleManVsDefectPos();
        bool CycleManVsAlignPos ();
        bool CycleManAlgnPlce() ;
        bool CycleManAlignerCal() ;

        bool ToolVoid       ();


        //�޴���. ���߿� ������.
        //bool CycleManAlignPlace(); //��Ŀ�� �ִ°��� ����̳ʿ� ���� ���´�.
        //bool CycleManPickVisnPlace(); //����ο��� �� �����˻��ϰ� �ٽ� ���� ���´�.
        //bool CycleManPickerCheck  (); //�ƹ��͵� ��� ���� ���� ���� ��ġ���� ���� ��û.



        bool GetVisnRslt(double * _dVisnX , double * _dVisnY , double * _dVisnT);

        //Rs232Height Sensor.
        CRs232LoadCl  Rs232_LoadCl;



        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void  SetStat(SStat _sStat) { Stat = _sStat ; }
        SStat GetStat(            ) { return Stat;    }

        //Motor.
        //bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // ���͸� �����϶� ���� �Լ�.

        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow = false);

        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //For Start Stop Checking.
        void SetLastCmd (); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad);

        bool m_bRqstPMSave ;

        bool m_bVacErrDisp; //���� �� ����
        bool m_bVsnErrDisp; //���� �� ����

        bool m_bNeedLotEnd ;
};

//---------------------------------------------------------------------------
extern CPicker PCK;
//---------------------------------------------------------------------------
#endif
