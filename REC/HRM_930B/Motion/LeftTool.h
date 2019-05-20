//---------------------------------------------------------------------------

#ifndef LeftToolH
#define LeftToolH
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Stage.h"

#include "Timer.h"
#include "DataMan.h"
#include "Rs232Dispr.h"
#include "Rs232UVCuring.h"
#include "CadPosition.h"
//#include "Rs232Keyence.h"


//---------------------------------------------------------------------------
class CLeftTool : public CPartInterface
{
    public: //�ʱ�ȭ ���.
        //Constructor
        CLeftTool ();
        ~CLeftTool ();

        void Init();
        void Close();

    public:

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;
            bool bReqToolVoid ;
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle        = 0 ,
            scMidSupply       ,
            scMidToWork       ,
            scClean           ,
            scMidLOri         ,
            scMidLAlign       ,
            scCmsLTopUV       ,
            scCmsLDisp        ,
            scCmsLAlign       ,
            scCmsLOri         ,
            scWorkOut         ,
            scToolVoid        ,
            scCmsLHeight      , //���� ������忡���� ��.
            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
        void InitCycleName(){
            m_sCycleName[scIdle      ]="Idle"      ;
            m_sCycleName[scMidSupply ]="MidSupply" ;
            m_sCycleName[scMidToWork ]="MidToWork" ;
            m_sCycleName[scClean     ]="Clean"     ;
            m_sCycleName[scMidLOri   ]="MidLOri"   ;
            m_sCycleName[scMidLAlign ]="MidLAlign" ;
            m_sCycleName[scCmsLTopUV ]="CmsLTopUV" ;
            m_sCycleName[scCmsLDisp  ]="CmsLDisp"  ;
            m_sCycleName[scCmsLAlign ]="CmsLAlign" ;
            m_sCycleName[scCmsLOri   ]="CmsLOri"   ;
            m_sCycleName[scWorkOut   ]="WorkOut"   ;
            m_sCycleName[scToolVoid  ]="ToolVoid"  ;
            m_sCycleName[scCmsLHeight]="CmsLHeight";

        }
        void InitCycleTime(){
            memset(&m_iCycleTime  , 0 , sizeof(int )*MAX_SEQ_CYCLE);
        }

        struct SStep {
            int          iHome    ;
            int          iToStart ;
            EN_SEQ_CYCLE iSeq     ;
            int          iCycle   ;
            int          iToStop  ;
            EN_SEQ_CYCLE iLastSeq ;
            SStep(){
                memset(this , 0 , sizeof(SStep));
            }
        };

    protected:
        AnsiString m_sPartName  ;

        CCycleTimer m_tmDispr   ;
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
        double GetLastCmd(EN_MOTR_ID _iMotr);
        void   ResetTimer();

        bool   FindChip   (int &r , int &c , EN_CHIP_STAT _iChip , EN_ARAY_ID _iId);
        struct TWorkInfo {
            int iCol     ;
            int iRow     ;
            EN_CHIP_STAT eStat ;
        } WorkInfo ;//���䷱���� ������������ ������ ������ ��� ���� Cycle���� �̰��� ����....

        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );



    public: //�������̽� ���.==================================================
	void Reset (); //���V ��ư �������� Ÿ�� �Լ�.

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
        bool CycleMidSupply();
        bool CycleMidToWork();
        bool CycleClean    ();
        bool CycleMidLOri  ();
        bool CycleMidLAlign();
        bool CycleTopLUV   ();
        bool CycleCmsLDisp ();
        bool CycleCmsLAlign();
        bool CycleCmsLOri  ();
        bool CycleWorkOut  ();
        bool CycleToolVoid ();

        bool CycleCmsLHeight();

        // Manual Cycle
        bool CycleHtReZero();
        bool CycleManNdlCheck();

        bool CycleMultiHeightCheck(int &_iStep  , double &_dHeight ,double _dPosX , double _dPosY);

        bool CycleManUVTest();

        bool ToolVoid(bool _bInit = false);

        //CRs232UV   Rs232_UV;

        bool CheckSafe  (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );
        bool CheckSafe  (EN_ACTR_ID _iActr , bool       _bFwd    );

        bool GetWorkEnd     (){return Stat.bWorkEnd ;}
        bool GetReqStop     (){return Stat.bReqStop ;}

        void  SetStat(SStat _sStat) { Stat = _sStat ; }
        SStat GetStat(            ) { return Stat;    }

        //Motor.
        bool MoveMotr    (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow = false);

        //�ù� ���ⱸ�� �Ϸ��� ����ũ�� ��ġ ���ƾ� ���� ;;
        bool MoveMotrSync(EN_MOTR_ID _iMotr1 , EN_MOTR_ID _iMotr2 , EN_MOTR_ID _iMotr3 ,
                          double dMotr1Pos   , double dMotr2Pos   , double dMotr3Pos   ,
                          double _dSpeed , double _dAcc , double _dDcc);

        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //�Ǹ����� �����϶� ���� �Լ�.

        //For Start Stop Checking.
        void SetLastCmd (); //��� �����ÿ� ������ Ŀ�ǵ带 ��� �ϰ� �ִٰ�.
        bool CheckMoved (); //�� �Լ��� �̿��� ���������� Ȯ�� �ϰ� ��� ��ŸƮ �ÿ� �׳� �۾� �� ���� �ƴϸ� ToStart�� �ϰ� �� ���� ����.
        bool CheckStop  (); //��� �����ÿ� ��� ���� �� �Ǹ��� �� ���� �Ǿ����� Ȯ�� �ϴ� �Լ�.

        //Loading Para.
        void Load(bool _bLoad);

        bool m_bRqstPMSave  ; //���ٴٵ� Ķ��... Ķ�۾��� ������ ������ �ʿ��Ѱ��. true��Ű��
                             //�� Ÿ�̸ӿ��� Ȯ�� �ϰ� UI���� ���� �Ѵ�.
        int  m_iCrntDispNode; //���� �������� ���漭 ���.
        int  m_iCrntUvNode  ; //���� �������� UV ���.

        int m_iDispCntF; //����� ī��Ʈ
        int m_iDispCntR; //����� ī��Ʈ


        double GetCrntStgCntVisnPosX(); //���� ���������� �ִ� ��ġ������ ���������ǿ� ���� ���������� ���.
        double GetCrntStgCntVisnPosY();

        //enum EN_CAD_TOOL {//ĳ�� �����͸� �ش��� ������ ��ǥ�� �����Ѵ�.
        //    ctVisnHt =0,
        //    ctVisn     ,
        //    ctDispFt   ,
        //    ctDispRr   ,
        //    ctDispUV
        //};
        void   SetCadOfsGain(EN_TOOL_ID _eTool , EN_COORDINATION _eCood = cnStg);



        //��ȭ �� �ð� ���÷��� �Ǿ�� �Ѵ�.
        //20170202 �������� �ڿ���ȭ ��� �����ϸ� �ð��� Ŭ��� ���� �ʴ� ��찡 ����.
        //�Ƹ��� �������� �����÷ο� �����ϱ�; �ϴ� ��Ʈ�� ����� �ٲ�.
        //�����÷ο� ����� ��ȣ�� �ٲ�� ��쵵 �־.
        double m_iLastUVSttTime ;//int m_iLastUVSttTime ;
        int m_iLastUVRow     ;
        int m_iLastUVCol     ;



        int m_iLastTopUVSttTime ;


};




//---------------------------------------------------------------------------
extern CLeftTool LTL;
//---------------------------------------------------------------------------
#endif
