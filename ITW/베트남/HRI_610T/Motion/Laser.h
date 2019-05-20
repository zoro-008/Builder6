//---------------------------------------------------------------------------

#ifndef LaserH
#define LaserH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Timer.h"
#include "DataMan.h"
//---------------------------------------------------------------------------

struct TAlignResult {
    double dMainX ;
    double dMainY ;
    double dTheta ;
    double dSubX  ;
    double dSubY  ;
};

class CLaser
{
    public:
        //Constructor
        CLaser (void);
        ~CLaser (void);

        void Init();
        void Close();
    
	void Reset     (); //���V ��ư �������� Ÿ�� �Լ�.
        void ResetTimer();

        //Vars.
        struct SStat {
            bool bWorkEnd     ;
            bool bReqStop     ;
        };

        enum EN_SEQ_CYCLE {
            scIdle       =  0 , //���.
            scVisn       =  1 , //�޴��� ������.
            scLaser      =  2 , //�޴��� ������ ���縸.
            scWork       =  3 , //����& ������.
            scDataChange =  4 , //������ Ȥ��
            MAX_SEQ_CYCLE
        };

        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        void InitCycleName(){
            m_sCycleName[scIdle ]="Idle "  ;
            m_sCycleName[scVisn ]="Visn "  ; //����� ����.
            m_sCycleName[scLaser]="Laser"  ;
            m_sCycleName[scWork ]="Work "  ;
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
        CDelayTimer m_tmTemp    ;

        SStat Stat ;
        SStep Step , PreStep ;

        bool   FindChip   (int &r , int &c   );
        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );

        String m_sLaserWorkLotNo ;


    public:    /* Direct Accessable Var.  */
        bool CheckSafe (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        int           GetHomeStep    (){return Step.iHome    ;} int GetPreHomeStep   (){return PreStep.iHome    ;} void InitHomeStep (){ Step.iHome  = 10 ; PreStep.iHome = 0 ;}
        int           GetToStartStep (){return Step.iToStart ;} int GetPreToStartStep(){return PreStep.iToStart ;}
        EN_SEQ_CYCLE  GetSeqStep     (){return Step.iSeq     ;} int GetPreSeqStep    (){return PreStep.iSeq     ;}
        int           GetCycleStep   (){return Step.iCycle   ;} int GetPreCycleStep  (){return PreStep.iCycle   ;} void InitCycleStep(){ Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        int           GetToStopStep  (){return Step.iToStop  ;} int GetPreToStopStep (){return PreStep.iToStop  ;}
        AnsiString    GetCycleName   (){return m_sCycleName[Step.iSeq];}
        SStat         GetStat        (){return Stat          ;}
        AnsiString    GetPartName    (){return m_sPartName   ;}

        void SetStat(SStat _sStat) { Stat = _sStat ; }

        //�������� �������� LotNo ;
        __property String _sLaserWorkLotNo  = { read = m_sLaserWorkLotNo   };

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId); // ���͸� �����϶� ���� �Լ�.
        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //Running Functions.
        bool ToStopCon (void); //��ž�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStartCon(void); //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
        bool ToStart   (void); //��ŸƮ�� �ϱ� ���� �Լ�.
        bool ToStop    (void); //��ž�� �ϱ� ���� �Լ�.
        bool Autorun   (void); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.

        //One Cycle.
        bool CycleVisn (); //�޴���
        bool CycleLaser(); //�޴���

        bool CycleWork (); //���䷱������ �̰͸� ����.


        //For Start Stop Checking.
        void SetLastCmd (void); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (void); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (void); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad );

        //Laser ���� Data
        bool       CheckFormat(AnsiString _sFormat);
        AnsiString GetLaserDataFromLotNot(AnsiString _sFormat , AnsiString _sLotNo);

        bool       LaserJobChange(AnsiString _sDevName);
        //void       SaveLaserLotNo(AnsiString _sLotNo, bool _bDeviceChage = false);
        bool       SaveLaserLotNo(AnsiString _sLotNo);

        bool       SaveLaserDevice(AnsiString _sDevice);

        //���� ��� ������ ��������.
        bool       LoadAlignRslt(TAlignResult *_tAlign);
        
        bool       m_bChangeLotNo ;


        //�޴��� ��ŷ ����.
        bool       m_bManMark ;
        String     m_sManMark ;

        bool       m_bAutoRun  ;

        TAlignResult m_tAlign ;
        void       GetRotPntFromCntrPnt(double _dX     , double _dY     , double _dAng , //�Է� x , �Է� y , ��������. , +�� �ݽð����.
                                        double _dCntrX , double _dCntrY ,                //������ ������ x , y
                                        double *_pRetX , double *_pRetY );                //����� x,y
};

//---------------------------------------------------------------------------
extern CLaser LSR;
//---------------------------------------------------------------------------
#endif
