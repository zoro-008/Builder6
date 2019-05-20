//---------------------------------------------------------------------------

#ifndef StageVTH
#define StageVTH
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Timer.h"
#include "DataMan.h"


//---------------------------------------------------------------------------
class CStageVT : public CPartInterface
{
    public: //�ʱ�ȭ ���.
        //Constructor
        CStageVT ();
        ~CStageVT ();

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
            scWork       =  1 ,
            scTest       =  2 ,
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
        void   InitCycleName(){
            m_sCycleName[scIdle]="Idle";
            m_sCycleName[scWork]="Work";
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

        bool   FindChip   (int &r , int &c   );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );



    public: //�������̽� ���.==================================================
	void Reset (); //���V ��ư �������� Ÿ�� �Լ�.

        //Running Functions.
        bool ToStopCon (); //��ž�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStartCon(); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStart   (); //��ŸƮ�� �ϱ� ���� �Լ�.
        bool ToStop    (); //��ž�� �ϱ� ���� �Լ�.

        int          GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;}void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome  = 0 ;}
        int          GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        int          GetSeqStep     (){return (int)Step.iSeq;} int GetPreSeqStep    (){return (int)PreStep.iSeq;}
        int          GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;}void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int          GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}

        AnsiString   GetCycleName   (int _iSeq){return m_sCycleName[_iSeq];}
        int          GetCycleTime   (int _iSeq){return m_iCycleTime[_iSeq];}
        AnsiString   GetPartName    (){return m_sPartName   ;}

        int          GetCycleMaxCnt (){return MAX_SEQ_CYCLE ;}

        bool Autorun   (); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
        //======================================================================
    public:

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //One Cycle.
        bool CycleWork  ();
        bool CycleBind  (); //�˻����.
        bool CycleUnBind();

        bool   CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool   CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );


        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // ���͸� �����϶� ���� �Լ�.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //For Start Stop Checking.
        void SetLastCmd (); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad);

        //���߿� ���ͷ� ������. SaveCrntCmdPos()
        double m_dLastIdxPos ;

        bool VTJobChange(); // ����̽� ü����
        bool SaveVTDevice(AnsiString _sDevice);
        bool GetVTRslt(int _iCol , EN_CHIP_STAT &_iStat);


        int m_iVT1Step ;
        int m_iVT2Step ;
        int m_iVT3Step ;
        int m_iVT4Step ;

        bool CycleVT1() ;
        bool CycleVT2() ;
        bool CycleVT3() ;
        bool CycleVT4() ;

        //���� BLK ���� ī��Ʈ.
        int m_iBLKCntPock1 ;
        int m_iBLKCntPock2 ;
        int m_iBLKCntPock3 ;
        int m_iBLKCntPock4 ;

        //���� Defect ���� ī��Ʈ.
        int m_iDFTCntPock1 ;
        int m_iDFTCntPock2 ;
        int m_iDFTCntPock3 ;
        int m_iDFTCntPock4 ;

        //���� Defect ���� ī��Ʈ.
        int m_iDFT2CntPock1 ;
        int m_iDFT2CntPock2 ;
        int m_iDFT2CntPock3 ;
        int m_iDFT2CntPock4 ;


        //���� VH���� ī��Ʈ/'
        int m_iVHCntPock1 ;
        int m_iVHCntPock2 ;
        int m_iVHCntPock3 ;
        int m_iVHCntPock4 ;
};

//---------------------------------------------------------------------------
extern CStageVT VTI;
//---------------------------------------------------------------------------
#endif