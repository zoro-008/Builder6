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
    public: //초기화 등등.
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
            scCmsLHeight      , //높이 측정모드에서만 씀.
            MAX_SEQ_CYCLE
        };

        int        m_iCycleSttTime             ;
        AnsiString m_sCycleName[MAX_SEQ_CYCLE] ;
        int        m_iCycleTime[MAX_SEQ_CYCLE] ; //마지막 사이클 시간. ms
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
        } WorkInfo ;//오토런에서 스테이지에서 정보를 가져다 담아 놓고 Cycle에서 이것을 쓴다....

        double GetMotrPos (EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId );



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

        bool m_bRqstPMSave  ; //스텐다드 캘등... 캘작업후 포지션 저장이 필요한경우. true시키고
                             //폼 타이머에서 확인 하고 UI에서 저장 한다.
        int  m_iCrntDispNode; //현재 수행중인 디스펜서 노드.
        int  m_iCrntUvNode  ; //현재 수행중인 UV 노드.

        int m_iDispCntF; //디스펜싱 카운트
        int m_iDispCntR; //디스펜싱 카운트


        double GetCrntStgCntVisnPosX(); //현재 스테이지가 있는 위치에서의 센터포지션에 대한 비젼포지션 계산.
        double GetCrntStgCntVisnPosY();

        //enum EN_CAD_TOOL {//캐드 데이터를 해당툴 모터의 좌표로 세팅한다.
        //    ctVisnHt =0,
        //    ctVisn     ,
        //    ctDispFt   ,
        //    ctDispRr   ,
        //    ctDispUV
        //};
        void   SetCadOfsGain(EN_TOOL_ID _eTool , EN_COORDINATION _eCood = cnStg);



        //경화 후 시간 디스플레이 되어야 한다.
        //20170202 오랫만에 자연경화 방식 구동하면 시간이 클리어가 되지 않는 경우가 있음.
        //아마도 데이터형 오버플로우 때문일까싶어서 일단 인트를 더블로 바꿈.
        //오버플로우 날경우 부호가 바뀌는 경우도 있어서.
        double m_iLastUVSttTime ;//int m_iLastUVSttTime ;
        int m_iLastUVRow     ;
        int m_iLastUVCol     ;



        int m_iLastTopUVSttTime ;


};




//---------------------------------------------------------------------------
extern CLeftTool LTL;
//---------------------------------------------------------------------------
#endif
