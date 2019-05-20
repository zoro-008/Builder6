//---------------------------------------------------------------------------

#ifndef RightToolH
#define RightToolH
//---------------------------------------------------------------------------
#include "PartInterface.h"

#include "Timer.h"
#include "DataMan.h"
#include "Rs232LoadCl.h"
#include "CadPosition.h"

//---------------------------------------------------------------------------
class CRightTool : public CPartInterface
{
    public: //�ʱ�ȭ ���.
        //Constructor
        CRightTool ();
        ~CRightTool ();

        void Init();
        void Close();

    public:

        //Vars.
        struct SStat {
            bool bWorkEnd ;
            bool bReqStop ;

            bool bReqToolVoid ;

            bool bMsgChecked  ; //������������ �ѹ� ���߰� Ȯ������.
            bool bShowVisnMsg ; //������������ ���߰� �޼��� ������.
        };    //sun Clear When LOT Open. and every 30Sec in autorun()

        enum EN_SEQ_CYCLE {
            scIdle          = 0 ,
            scMidRHeight        ,
            scTrsfPick          ,
            scTrsfPlace         ,
            scCmsRHeight        ,
            scCmsRAlign         ,
            scCmsRZHeight       ,
            scCmsREndAlign      ,
            scCmsRFixEnd        ,
            scMidREndHeight     ,
            scWkoToPtl          ,
            scOut               ,
            scMidRCheck         ,

            scHtReZero          ,
            scToolVoid          ,
            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //������ ����Ŭ �ð�. ms
        void InitCycleName(){
            m_sCycleName[scIdle         ]=String("Idle         ").c_str()  ;
            m_sCycleName[scMidRHeight   ]=String("MidRHeight   ").c_str()  ;
            m_sCycleName[scTrsfPick     ]=String("TrsfPick     ").c_str()  ;
            m_sCycleName[scTrsfPlace    ]=String("TrsfPlace    ").c_str()  ;
            m_sCycleName[scCmsRHeight   ]=String("CmsRHeight   ").c_str()  ;
            m_sCycleName[scCmsRZHeight  ]=String("CmsRZHeight  ").c_str()  ;
            m_sCycleName[scCmsRAlign    ]=String("CmsRAlign    ").c_str()  ;
            m_sCycleName[scCmsREndAlign ]=String("CmsREndAlign ").c_str()  ;
            m_sCycleName[scCmsRFixEnd   ]=String("CmsRFixEnd   ").c_str()  ;
            m_sCycleName[scMidREndHeight]=String("MidREndHeight").c_str()  ;
            m_sCycleName[scWkoToPtl     ]=String("WkoToPtl     ").c_str()  ;
            m_sCycleName[scOut          ]=String("Out          ").c_str()  ;
            m_sCycleName[scMidRCheck    ]=String("MidRCheck    ").c_str()  ;

            m_sCycleName[scHtReZero     ]=String("HtReZero     ").c_str()  ;
            m_sCycleName[scToolVoid     ]=String("ToolVoid     ").c_str()  ;

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
            int iCol ;
            int iRow ;
            EN_CHIP_STAT eStat ;
            EN_CHIP_STAT ePreStat ;
        } WorkInfo ;//���䷱���� ������������ ������ ������ ��� ���� Cycle���� �̰��� ����....


        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );



        struct THeightInfo {//�ﰢ�� ���� ������.
            bool     bVertical          ;

            int      iLineHeightCnt     ;
            double * adLineHeightPos    ;
            double * adLineHeight       ;

            double   dLineHeightSubPosX ;
            double   dLineHeightSubPosY ;

            //������ �������� ������.
            double   dLineA             ;
            double   dLineB             ;
            double   dLineCntHeight     ;
            double   dLineTilt          ;

            //���� ����Ʈ�� �������� ������.
            double   dSubA              ;
            double   dSubB              ;
            double   dSubPntHeight      ;
            double   dSubTilt           ;
            double   dSubToLinePntHeight;


            THeightInfo(){
                bVertical = false ;

                iLineHeightCnt     = 0 ;
                adLineHeightPos    = NULL ;
                adLineHeight       = NULL ;

                dLineHeightSubPosX = 0 ;
                dLineHeightSubPosY = 0 ;

                dLineA             = 0 ;
                dLineB             = 0 ;
                dLineCntHeight     = 0 ;
                dLineTilt          = 0 ;

                dSubA              = 0 ; //Line ���Ϳ� SubPoint ���� ����.
                dSubB              = 0 ; //Line ���Ϳ� SubPoint ���� Y����.
                dSubPntHeight      = 0 ; //���� ����Ʈ��ġ�� ����.
                dSubTilt           = 0 ; //ƿƮ ����.
                dSubToLinePntHeight= 0 ; //��������Ʈ���� ���������� ������ ���� ���� ��ġ�� ����.

            }

            ~THeightInfo(){
                if(adLineHeightPos) delete [] adLineHeightPos;
                if(adLineHeight   ) delete [] adLineHeight   ;
            }

            void SetLineHeightCnt(int _iCnt){
                if(adLineHeightPos) delete [] adLineHeightPos ;
                if(adLineHeight   ) delete [] adLineHeight    ;
                iLineHeightCnt = _iCnt ;
                adLineHeightPos = new double [_iCnt];
                adLineHeight    = new double [_iCnt];
            }
        } StandHeightInfo    , WorkHeightInfo    ,
          SubStandHeightInfo , SubWorkHeightInfo ; //4�� ���϶� 2���� 4���� ���ո�.




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
        bool CycleMidRHeight   (); //�̵���� ��������.
        bool CycleTrsfPick     (); //Ʈ�����۰� �������Ϳ��� ���.
        bool CycleTrsfPlace    (); //Ʈ�����۰� ���������� ���� ����.
        bool CycleCmsRHeight   (); //�����ʳ��� �����Ⱑ ������������ �ø� ���� �����ϱ�.
        bool CycleCmsRAlign    (); //������ ������ ������������ �����. (������ �ִ� ����)
        bool CycleCmsRZHeight  (); //��ȭ�� ������ ���� �Ͽ� ��հ����� ���� �ϱ�.
        bool CycleCmsREndAlign (); //��ȭ�� ���� �˻� �� ��ȭ�� Ʋ���� �˻�.
//        bool CycleCmsRFixEnd   ();
        bool CycleMidREndHeight();
        bool CycleWkoToPtl     ();
        bool CycleOut          ();
        bool CycleMidRCheck    (); //�̵���� ���������� �ϰ� ��δ��� �̼� �ϴ� ���.

        bool CycleHtReZero     ();

        bool CycleToolVoid     ();



        //���� ���� ������.
        bool CycleVisnHtCal    (int &_iStep  ,  CCadRect * _pCmsRect , CCadVisnPoint * _pVisnPos , int _iOfsX , int _iOfsY);

        //���� ����.
        bool CycleMultiHeightCheck  (int &_iStep  , double &_dHeight ,double _dPosX , double _dPosY ,bool _bUseConfocal = true);
        bool CycleSigleHeightCheck  (int &_iStep  , double &_dHeight ,double _dPosX , double _dPosY ,bool _bUseConfocal = true);


        //Rs232Height Sensor.
        CRs232LoadCl  Rs232_LoadCl;


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

        double GetTouchToStgCntX();
        double GetTouchToStgCntY();
        double GetCrntStgCntVisnPosX(); //���� ���������� �ִ� ��ġ������ ���������ǿ� ���� ���������� ���.
        double GetCrntStgCntVisnPosY();

        bool ToolVoid(bool _bInit = false);

        //enum EN_CAD_TOOL {//ĳ�� �����͸� �ش��� ������ ��ǥ�� �����Ѵ�.
        //    ctVisnHt =0,
        //    ctVisn     ,
        //    ctTrnsf    ,
        //    ctStg      ,
        //    ctConfocal ,
        //};

        /*
        tlLVisn = 0,
        tlLHght ,
        tlLUV   ,
        tlLDspR ,
        tlLDspF ,
        tlRTrns ,
        tlRVisn ,
        tlRHght ,
        tlStage ,
        */

        struct TVisnRslt {  //���� �����ÿ� ������ �������� ������� ������ �ִ°�.
           double dMasterX ;
           double dMasterY ;
           double dSlaveX  ; //�����̺��� ��� ������ ���ؿ��� Ʋ���� ���� �������� �Ŷ� �����Ͷ� �����̺�� ���Ѱ���
           double dSlaveY  ; //���� ����� �ٸ����̴�.
           TVisnRslt(){
              Clear();
           }
           void Clear(){
               dMasterX = 0.0 ;
               dMasterY = 0.0 ;
               dSlaveX  = 0.0 ;
               dSlaveY  = 0.0 ;
           }
        }VisnRslt; //VisnRslt0�� 2�����ո��� �������ո�.

        bool m_bNeedHexaJog ;

        int m_iLastBtmUVSttTime ;

        void SetCadOfsGain(EN_TOOL_ID _eTool);
};


//---------------------------------------------------------------------------
extern CRightTool RTL;
//---------------------------------------------------------------------------
#endif