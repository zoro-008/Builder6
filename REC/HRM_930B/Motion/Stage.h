//---------------------------------------------------------------------------

#ifndef StageH
#define StageH
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Timer.h"
#include "DataMan.h"
#include "GridData.h"
//#include "HexaPot.h"

//---------------------------------------------------------------------------
struct TAlignPosOffset{
    double dX  ;
    double dY  ;
    double dT  ;

    TAlignPosOffset(){
        memset(this , 0 , sizeof(TAlignPosOffset));
    }
};

extern TAlignPosOffset g_tMidPosOfs ;




//struct TWorkInfo {
//    int iCol ;
//    int iRow ;
//    EN_CHIP_STAT eStat ;
//} WorkInfo ;



//struct TCmsRAlign {
//    double dOffsetX ;
//    double dOffsetY ;
//    double dAngle   ;
//};
//


class CStage : public CPartInterface
{
    public: //�ʱ�ȭ ���.
        //Constructor
        CStage ();
        ~CStage ();

        void Init();
        void Close();

        void GetRotPntFromCntrPnt(double _dX     , double _dY     , double _dAng ,  //�Է� x , �Է� y , ��������. , +�� �ݽð����.
                                  double _dCntrX , double _dCntrY ,                //������ ������ x , y
                                  double *_pRetX , double *_pRetY );                //����� x,y


    public:

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle       =  0 ,
            scDown       =  1 ,
            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
        void   InitCycleName(){
            m_sCycleName[scIdle  ]="Idle" ;
            m_sCycleName[scDown  ]="Down" ;
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

        SStat  Stat ;
        SStep  Step , PreStep ;

        double m_dLastIdxPosX ;
        double m_dLastIdxPosY ;
        String m_sCheckSafeMsg ;

        void   ResetTimer();


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

        bool         Autorun        (); //���䷱�׽ÿ� ��� Ÿ�� �Լ�.

        void         Update         ();
        //======================================================================
    public:
        bool   FindChip   (int &r , int &c , EN_CHIP_STAT _iChip);


        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        
        //HomeCycle
        bool CycleHome(); //��Ʈ Ȩ�� ������ ���� �Լ�.

        //One Cycle.
        bool CycleDown ();

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void  SetStat(SStat _sStat) { Stat = _sStat ; }
        SStat GetStat(            ) { return Stat;    }

        //Motor.
        void MoveIncMotr (EN_MOTR_ID _iMotr , double _iPstn); // ���͸� �����϶� ���� �Լ�.

        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow = false);

        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //For Start Stop Checking.
        void SetLastCmd (); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //bool GetWorkStep(TWorkInfo &_tWorkInfo, bool _bLeft);

        bool m_bNeedStgHome ; //�̸����� �������� �������� Ȩ�� �� �ٽ� ��� ��� �Ѵ�.

        struct TPkgStat{
            double dMidTopHeight  ;
            double dHexaTopHeight ;

            TPkgStat(){
                memset(this , 0 , sizeof(TPkgStat));
            }
        }PkgStat;





        //Loading Para.
        void Load(bool _bLoad);

        //SPC Data��.
        CGridData WorkData ;
        void WriteSpc();

        bool GetWorkingRC(int &_iR , int &_iC);

        /*
        struct TStageSpc {
            double    dWorkSttTime      ;
            int       iWorkFrntBoatSlot ;
            String    sWorkFrntBoatId   ;
            String    sWorkFrntChipId   ;
            double    dPushWeight       ;
            double    dCrntStgTemp      ;
            int       iDefectCnt        ;
            TStageSpc(){
                Clear();
            }
            ~TStageSpc(){
            }
            void Clear(){
                dWorkSttTime      = 0 ;
                iWorkFrntBoatSlot = 0 ;
                sWorkFrntBoatId   = "";
                sWorkFrntChipId   = "";
                dPushWeight       = 0 ;
                dCrntStgTemp      = 0 ;
                iDefectCnt        = 0 ;
            }
        }*/

        struct TStageSpc {
            double dWorkSttTime  ;
            double dWorkEndTime  ;
            String sModuleID00    ;
            String sModuleID10    ;
            String sModuleID01    ;
            String sModuleID11    ;
            String sBfAlign00    ;
            String sAtAlign00    ;
            String sBfHeight00   ;
            String sAtHeight00   ;
            String sBfHeight10   ;
            String sAtHeight10   ;
            String sBfHeight01   ;
            String sAtHeight01   ;
            String sBfHeight11   ;
            String sAtHeight11   ;
            String sBfAlign00_10 ;
            String sAtAlign00_10 ;
            String sBfAlign00_01 ;
            String sAtAlign00_01 ;
            String sBfAlign10_11 ;
            String sAtAlign10_11 ;
            String sBfAlign01_11 ;
            String sAtAlign01_11 ;
            String sEndHeight00  ;
            String sEndHeight10  ;
            String sEndHeight01  ;
            String sEndHeight11  ;
            String sDispPres     ;
            String sDispSpeed    ;
            String sUvCycle      ;
            String sUvPower      ;
            String sUvHeight     ;
            String sUvSpeed      ;
            String sUvFix        ;
            String s1stZGap      ;
            String sSensor       ;
            String sBottomUv     ;

            TStageSpc(){
                Clear();
            }
            ~TStageSpc(){
            }
            void Clear(){
                dWorkSttTime  = 0 ;
                dWorkEndTime  = 0 ;
                sModuleID00   = "";
                sModuleID10   = "";
                sModuleID01   = "";
                sModuleID11   = "";
                sBfAlign00    = "";
                sAtAlign00    = "";
                sBfHeight00   = "";
                sAtHeight00   = "";
                sBfHeight10   = "";
                sAtHeight10   = "";
                sBfHeight01   = "";
                sAtHeight01   = "";
                sBfHeight11   = "";
                sAtHeight11   = "";
                sBfAlign00_10 = "";
                sAtAlign00_10 = "";
                sBfAlign00_01 = "";
                sAtAlign00_01 = "";
                sBfAlign10_11 = "";
                sAtAlign10_11 = "";
                sBfAlign01_11 = "";
                sAtAlign01_11 = "";
                sEndHeight00  = "";
                sEndHeight10  = "";
                sEndHeight01  = "";
                sEndHeight11  = "";
                sDispPres     = "";
                sDispSpeed    = "";
                sUvCycle      = "";
                sUvPower      = "";
                sUvHeight     = "";
                sUvSpeed      = "";
                sUvFix        = "";
                s1stZGap      = "";
                sSensor       = "";
                sBottomUv     = "";
            }
        }Spc;




};

//---------------------------------------------------------------------------
extern CStage STG;
//---------------------------------------------------------------------------
#endif
