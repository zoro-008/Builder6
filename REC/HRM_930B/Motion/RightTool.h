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
    public: //초기화 등등.
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
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //마지막 사이클 시간. ms
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
        } WorkInfo ;//오토런에서 스테이지에서 정보를 가져다 담아 놓고 Cycle에서 이것을 쓴다....


        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );



        struct THeightInfo {//삼각형 높이 측정용.
            bool     bVertical          ;

            int      iLineHeightCnt     ;
            double * adLineHeightPos    ;
            double * adLineHeight       ;

            double   dLineHeightSubPosX ;
            double   dLineHeightSubPosY ;

            //라인쪽 높이측정 데이터.
            double   dLineA             ;
            double   dLineB             ;
            double   dLineCntHeight     ;
            double   dLineTilt          ;

            //서브 포인트쪽 높이측정 데이터.
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

                dSubA              = 0 ; //Line 센터와 SubPoint 간의 기울기.
                dSubB              = 0 ; //Line 센터와 SubPoint 간의 Y절편.
                dSubPntHeight      = 0 ; //서브 포인트위치의 높이.
                dSubTilt           = 0 ; //틸트 각도.
                dSubToLinePntHeight= 0 ; //서브포인트에서 라인으로의 수선의 발을 내린 위치의 높이.

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
          SubStandHeightInfo , SubWorkHeightInfo ; //4장 붙일때 2번과 4번의 접합면.




    public: //인터페이스 상속.==================================================
	void Reset (); //리셑 버튼 눌렀을때 타는 함수.

        //Running Functions.
        void Update    ();
        bool ToStopCon (); //스탑을 하기 위한 조건을 보는 함수.
        bool ToStartCon(); //스타트를 하기 위한 조건을 보는 함수.
        bool ToStart   (); //스타트를 하기 위한 함수.
        bool ToStop    (); //스탑을 하기 위한 함수.

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

        bool         Autorun        (); //오토런닝시에 계속 타는 함수.
        //======================================================================
    public:
        //HomeCycle
        bool CycleHome(); //파트 홈을 잡을때 쓰는 함수.

        //One Cycle.
        bool CycleMidRHeight   (); //미들블럭 높이측정.
        bool CycleTrsfPick     (); //트렌스퍼가 로테이터에서 찝기.
        bool CycleTrsfPlace    (); //트렌스퍼가 스테이지에 내려 놓기.
        bool CycleCmsRHeight   (); //오른쪽높이 측정기가 스테이지에서 시모스 높이 측정하기.
        bool CycleCmsRAlign    (); //오른쪽 비전이 스테이지에서 얼라인. (벌려져 있는 상태)
        bool CycleCmsRZHeight  (); //경화전 단차를 측정 하여 평균값으로 보정 하기.
        bool CycleCmsREndAlign (); //경화전 최종 검사 및 경화후 틀어짐 검사.
//        bool CycleCmsRFixEnd   ();
        bool CycleMidREndHeight();
        bool CycleWkoToPtl     ();
        bool CycleOut          ();
        bool CycleMidRCheck    (); //미들블럭 높이측정만 하고 언로더로 이송 하는 모드.

        bool CycleHtReZero     ();

        bool CycleToolVoid     ();



        //비젼 높이 보정용.
        bool CycleVisnHtCal    (int &_iStep  ,  CCadRect * _pCmsRect , CCadVisnPoint * _pVisnPos , int _iOfsX , int _iOfsY);

        //높이 측정.
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

        //시바 동기구동 하려면 볼스크류 피치 같아야 쓸듯 ;;
        bool MoveMotrSync(EN_MOTR_ID _iMotr1 , EN_MOTR_ID _iMotr2 , EN_MOTR_ID _iMotr3 ,
                          double dMotr1Pos   , double dMotr2Pos   , double dMotr3Pos   ,
                          double _dSpeed , double _dAcc , double _dDcc);

        //Actuator.
        bool MoveActr    (EN_ACTR_ID _iActr , bool _bFwd); //실린더를 움직일때 쓰는 함수.

        //For Start Stop Checking.
        void SetLastCmd (); //장비 정지시에 마지막 커맨드를 기억 하고 있다가.
        bool CheckMoved (); //이 함수를 이용해 움직였는지 확인 하고 장비 스타트 시에 그냥 작업 할 껀지 아니면 ToStart를 하고 할 건지 결정.
        bool CheckStop  (); //장비 정지시에 모든 모터 와 실린더 가 정지 되었는지 확인 하는 함수.

        //Loading Para.
        void Load(bool _bLoad);

        double GetTouchToStgCntX();
        double GetTouchToStgCntY();
        double GetCrntStgCntVisnPosX(); //현재 스테이지가 있는 위치에서의 센터포지션에 대한 비젼포지션 계산.
        double GetCrntStgCntVisnPosY();

        bool ToolVoid(bool _bInit = false);

        //enum EN_CAD_TOOL {//캐드 데이터를 해당툴 모터의 좌표로 세팅한다.
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

        struct TVisnRslt {  //높이 측정시에 보정한 포지션을 찍기위해 가지고 있는값.
           double dMasterX ;
           double dMasterY ;
           double dSlaveX  ; //슬레이브의 경우 마스터 기준에서 틀어진 값이 더해지는 거라서 마스터랑 슬레이브랑 더한값이
           double dSlaveY  ; //실제 도면과 다른값이다.
           TVisnRslt(){
              Clear();
           }
           void Clear(){
               dMasterX = 0.0 ;
               dMasterY = 0.0 ;
               dSlaveX  = 0.0 ;
               dSlaveY  = 0.0 ;
           }
        }VisnRslt; //VisnRslt0은 2중접합면중 참조접합면.

        bool m_bNeedHexaJog ;

        int m_iLastBtmUVSttTime ;

        void SetCadOfsGain(EN_TOOL_ID _eTool);
};


//---------------------------------------------------------------------------
extern CRightTool RTL;
//---------------------------------------------------------------------------
#endif
