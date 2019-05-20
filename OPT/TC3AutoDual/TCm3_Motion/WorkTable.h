//---------------------------------------------------------------------------
#ifndef WorkTableH
#define WorkTableH
//---------------------------------------------------------------------------
#include "Timer.h"
#include "UtilEnums.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LogUnit.h"
#include "DpDataUnit.h"

enum EN_SEQ_STAT {
    ssInit     = 0 ,
    ssError    = 1 ,
    ssRunning  = 2 ,
    ssStop     = 3 ,

    MAX_SEQ_STAT
};
                                      //ó�� ��� ���� CycleRelease�� �޼��� â ����.
#define MsgOkNotRun if(!Stat.bRun && !Step.iToStart && !Step.iToStop)MsgOk

//enum EN_MOTION_VISION_CMD {
//    mvInsp    = 1 ,
//    mvAuto    = 2 ,
//    mvTest    = 3 ,
//    mvMsg     = 4
//};

//typedef void __fastcall (__closure *FuncAPCMsg )(AnsiString,AnsiString,AnsiString,bool);     // OnSendAPCMsg("S1","6"); //Off        ��.�ݹ�.. ������.
//typedef void __stdcall  (__closure *FuncVisnMsg)(EN_MOTION_VISION_CMD,char *);
//void __stdcall  OnSendVisnMsg(EN_MOTION_VISION_CMD cmd,  char * data = NULL);
//void __fastcall OnSendAPCMsg(AnsiString _sCmd , AnsiString _sData , AnsiString _sVal="" , bool _bInc = false );


//Cycle Retern.
#define ERR -1 //������ ���ϵ� ���.
#define CNT  0 //���� ��� ������ �ȳ��� ���.
#define END  1 //������ ���� �Ϸ� �Ȱ��.

#define MAX_PRE_POS 256


//---------------------------------------------------------------------------
class TWorkTable
{
    public:    /* Direct Accessable Var. */
        //Constructor
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        TWorkTable ();
        ~TWorkTable ();

        void __fastcall Init() ;


    public:   /* Member Var. */
        AnsiString     m_sPartName  ;
        EN_PART_ID     m_iPartId    ;

        EN_MOTR_ID     miZ          ;
        EN_MOTR_ID     miY          ;
        EN_MOTR_ID     miI          ;

        EN_ERR_ID      eiHomeTO     ;
        EN_ERR_ID      eiStartTO    ;
        EN_ERR_ID      eiStopTO     ;
        EN_ERR_ID      eiCkBtnTO    ;
        EN_ERR_ID      eiCycleTO    ;
        EN_ERR_ID      eiXAlarm     ;
        EN_ERR_ID      eiYAlarm     ;
        EN_ERR_ID      eiIAlarm     ;
        EN_ERR_ID      eiXNegLim    ;
        EN_ERR_ID      eiYNegLim    ;
        EN_ERR_ID      eiINegLim    ;
        EN_ERR_ID      eiXPosLim    ;
        EN_ERR_ID      eiYPosLim    ;
        EN_ERR_ID      eiIPosLim    ;
        EN_ERR_ID      eiMainTO     ;
        EN_ERR_ID      eiSubTO      ;
        EN_ERR_ID      eiLdCalTO    ;
        EN_ERR_ID      eiLdIOTO     ;
        EN_ERR_ID      eiGp1TO      ;
        EN_ERR_ID      eiSupportTO  ;
        EN_ERR_ID      eiPcbCmpTO   ;
        EN_ERR_ID      eiGtCalTO    ;
        EN_ERR_ID      eiUvFBTO     ;
        EN_ERR_ID      eiUvUDTO     ;
        EN_ERR_ID      eiDp1IOTO    ;
        EN_ERR_ID      eiDp2IOTO    ;
        EN_ERR_ID      eiDp1UDTO    ;
        EN_ERR_ID      eiDp2UDTO    ;
        EN_ERR_ID      eiPadUDTO    ;
        EN_ERR_ID      eiXHomeEnd   ;
        EN_ERR_ID      eiYHomeEnd   ;
        EN_ERR_ID      eiIHomeEnd   ;

        EN_OUTPUT_ID   yStartLp    ;
        EN_OUTPUT_ID   yPickupLp   ;
        EN_OUTPUT_ID   yUpDnLp     ;
        EN_OUTPUT_ID   yVacLp      ;
        EN_OUTPUT_ID   yChuckLp    ;
        EN_OUTPUT_ID   yStopLp     ;
        EN_OUTPUT_ID   yResetLp    ;
        EN_OUTPUT_ID   ySuckOn     ;

        EN_OUTPUT_ID   ySol1       ;
        EN_OUTPUT_ID   ySol2       ;
        EN_OUTPUT_ID   ySol3       ;
        EN_OUTPUT_ID   ySol4       ;
        EN_OUTPUT_ID   ySol5       ;
        EN_OUTPUT_ID   ySol6       ;
//        EN_OUTPUT_ID   yVac1       ;
//        EN_OUTPUT_ID   yVac2       ;
//        EN_OUTPUT_ID   yVac3       ;
//        EN_OUTPUT_ID   yVac4       ;
//        EN_OUTPUT_ID   yVac5       ;
//        EN_OUTPUT_ID   yVac6       ;
        EN_OUTPUT_ID   yHdStart    ;

        EN_INPUT_ID    xStartSw    ;
        EN_INPUT_ID    xPickupSw   ;
        EN_INPUT_ID    xUpDnSw     ;
        EN_INPUT_ID    xVacSw      ;
        EN_INPUT_ID    xChuckSw    ;
        EN_INPUT_ID    xStopSw     ;
        EN_INPUT_ID    xResetSw    ;
        EN_INPUT_ID    xHdEnd      ;


        EN_ACTR_ID     aiMain      ;
        EN_ACTR_ID     aiSub       ;
        EN_ACTR_ID     aiLdCal     ;
        EN_ACTR_ID     aiGp1       ;
        EN_ACTR_ID     aiSupport   ;
        EN_ACTR_ID     aiPcbCmp    ;
        EN_ACTR_ID     aiGtCal     ;
        EN_ACTR_ID     aiUvFB      ;
        EN_ACTR_ID     aiUvUD      ;
        EN_ACTR_ID     aiDp1IO     ;
        EN_ACTR_ID     aiDp2IO     ;
        EN_ACTR_ID     aiDp1UD     ;
        EN_ACTR_ID     aiDp2UD     ;
        EN_ACTR_ID     aiPadUD     ;
        EN_ACTR_ID     aiLdIO      ;

        EN_PSTN_Z     pi_Z_Wait  ;        //sun
        EN_PSTN_Z     pi_Z_Insp  ;
        EN_PSTN_Y     pi_Y_Wait  ;
        EN_PSTN_Y     pi_Y_Insp  ;
        EN_PSTN_I     pi_I_Wait  ;
        EN_PSTN_I     pi_I_Insp  ;

        int           iOptnDisp[MAX_DISP_NOZZLE][MAX_DISP_CNT] ;




    protected:
        //Timer.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        CDelayTimer m_CycleTimer   ; //One  Cycle Timer.
        CDelayTimer m_HomeTimer    ; //Home       Timer.
        CDelayTimer m_ToStopTimer  ; //ToStop     Timer.
        CDelayTimer m_ToStartTimer ; //ToStart    Timer.
        CDelayTimer m_MainTimer    ; //Main       Timer.

        //Seq Timer
        CDelayTimer m_TempTimer    ;
        CDelayTimer m_TempTimer2   ;

        CDelayTimer m_DispTimer1   ;
        CDelayTimer m_DispTimer2   ;
        CDelayTimer m_DispTimer3   ;
        CDelayTimer m_DispTimer4   ;
        CDelayTimer m_DispTimer5   ;
        CDelayTimer m_DispTimer6   ;

        CDelayTimer m_PickUpBtTimer ;
        CDelayTimer m_UpDownBtTimer ;
        CDelayTimer m_VaccumBtTimer ;  
        CDelayTimer m_ChuckBtTimer  ;  
        CDelayTimer m_StartBtTimer  ;
        CDelayTimer m_StopBtTimer   ;




        //Fick Timer.
        CDelayTimer m_FlickOnTimer    ;
        CDelayTimer m_FlickOffTimer   ;

        //Vars.
        bool         m_bFlick   ;

        TDateTime    m_InspStartTime ;
        TDateTime    m_InspTime      ;

        double       m_dInspStartTimems ;
        double       m_dInspTimems      ;

        double       m_dCycleTime    ;
        double       m_dPreCycleTime ;

        bool         m_bStartRev ; //��ŸƮ ����.

        bool         m_bPreBarcode ; //���ڵ� ������ ��� ��ġ ����.

        double       m_dLastIPos ;
        bool         m_bInspMaxErr ; // ������ �˻翡 �˻� Ƚ�� ���� �����ÿ� ���½�ŸƮ �ϸ� ���ڸ����� �۾� �ٽý��۵ǰ�..//20120726 ���->�̻���->����� ��û.




        //FuncAPCMsg  * APCMsg  ;
        //FuncVisnMsg * VisnMsg ;

        double       dPrePstnY[MAX_PRE_POS] ;
        double       dPrePstnZ[MAX_PRE_POS] ;

        void         PushData(double dDataY , double dDataZ , int iCnt);
        double GetAvrPosY(int iCnt);
        double GetAvrPosZ(int iCnt);
        void   LoadAvrPos(bool bLeft);

        void   SaveAvrPos(bool bLeft);


        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool __fastcall CheckSafe(EN_MOTR_ID _iMotr , int _iPstn , bool _bDispMsg = false );
        bool __fastcall CheckSafe(EN_ACTR_ID _iActr , bool _bFwd , bool _bDispMsg = false );

    public:    /* Direct Accessable Var.  */
        CLogUnit LOG ;
        CDpData  DSP ;

        EN_SEQ_STAT  m_iSeqStat ;

        int (__closure *Cycle)();

        bool       m_bBarcodeScan ;
        AnsiString m_sCrntBarcode ;

        //Insp.
        int    m_iCcInspInspCnt  ;
        int    m_iCcInspTMInspCnt;
        double m_dCcInspGTOriPos ;
        int    m_iCcInspFindCnt  ;

        //DpIp
        int    m_iCcDpIpDispCnt  ; //���� ī��Ʈ ���� ������ 5���ΰ� �ϸ� �װ� ������� ���� �ϱ� ���� ����.
        int    m_iCcDpIpTMInspCnt; //���� �˻� �޼��� ��ġ�� ��찡 �־ 3�� ������ �ٽ� ���� ����.
        int    m_iCcDpIpInspCnt  ;
        int    m_iCcDpIpGTCnt    ; //�Ǹ������˻�, �˻�Ϸ��Ŀ� GT�ø��� �ٽ� �˻��ߴµ� Fail�̸� �ٽ� �˻��ϴ� ī��Ʈ

        //Disp
        int    m_iCcDispDispCnt  ;

        //ReInsp
        int    m_iCcReIpInspCnt  ;
        int    m_dCcReIpGTOriPos ;


        //DispOnly
        int    m_iCcDpOlDispCnt    ;
        int    m_iCcDpOlSetDispCnt ; //����â���� Ƚ�� ������ ��.
        int    m_iCcDpOlTtDispCnt  ; //���� ������ ���� ����.++

        //Dry Run
        int    m_iCcDrRnDispCnt  ;


        struct TRqst {
            bool bDisp      ; int iDispNzl ;
            bool bUVCycle   ;
            bool bDispCycle ;
            bool bInspCycle ;
        }Rqst ;

        struct TDone {
            bool bPickUp ;
            bool bUpDown ;
            bool bVaccum ;
            bool bChuck  ;
            bool bInsp   ;
            bool bDisp   ;
            bool bReInsp ;
            bool bUV     ;

            bool bDataCheck ;
        }Done ;
        //Vars.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        struct TStat {
            bool bWorkEnd    ; //Work End Flag.
            bool bToStart    ; //To... Flag.
            bool bToStop     ;
            bool bReqStop    ;
            bool bReqHome    ;
            bool bHomming    ;
            bool bRun        ;
            bool bStoped     ;
            bool bPosReset   ;
            bool bDispensing ;
        } Stat ;

        struct TStep {
            int iSeq     ; //Step.
            int iCycle   ; //����Ŭ������ ����.
            int iHome    ; //Ȩ ��� ����.
            int iToStart ; //�����غ� ����.
            int iToStop  ; //�����غ� ����.
        } Step , PreStep ;

       //void SetFuncAPCMsg  (FuncAPCMsg  * Func){APCMsg  = Func ;}
       // void SetFuncVisnMsg (FuncVisnMsg * Func){VisnMsg = Func ;}

        bool GetStatWorkEnd (){return Stat.bWorkEnd ;}
        bool GetStatToStart (){return Stat.bToStart ;}
        bool GetStatToStop  (){return Stat.bToStop  ;}
        bool GetStatReqStop (){return Stat.bReqStop ;}

        int  GetStepSeq     (){return Step.iSeq     ;}
        int  GetStepCycle   (){return Step.iCycle   ;}
        int  GetStepHome    (){return Step.iHome    ;}
        int  GetStepToStart (){return Step.iToStart ;}
        int  GetStepToStop  (){return Step.iToStop  ;}

        int  GetPreStepSeq     (){return PreStep.iSeq     ;}
        int  GetPreStepCycle   (){return PreStep.iCycle   ;}
        int  GetPreStepHome    (){return PreStep.iHome    ;}
        int  GetPreStepToStart (){return PreStep.iToStart ;}
        int  GetPreStepToStop  (){return PreStep.iToStop  ;}

        void ReqHome();

        //Reset.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        void __fastcall Reset (void);

        //Motor.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool __fastcall MoveMotr (EN_MOTR_ID _iMotr , int _iPstn , bool _bDispMsg = false );
        bool __fastcall StopMotr (EN_MOTR_ID _iMotr , bool _bEmgStop = false);

        //Actuator.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        bool __fastcall MoveActr    (EN_ACTR_ID _iActr , bool _bFwd , bool _bDispMsg = false );

        //One Cycle.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        int CycleHome  (void);

        int CyclePickup   (void); int CycleUnPickup(void);
        int CycleUpDn     (void); int CycleUnUpDn  (void);
        int CycleVac      (void); int CycleUnVac   (void);
        int CycleChuck    (void); int CycleUnChuck (void);
        int CycleInsp     (void); //����
        int CycleDisp     (void); //����
        int CycleDpIp     (void); //���������
        int CycleReInsp   (void); //������. �Ⱦ�.
        int CycleUv       (void); //UV
        int CycleRelease  (void);
        int CycleDispOnly (void);
        int CycleDataCheck(void); int CycleUnDataCheck(void);



        int CycleDryRun   (void);


        //Running Functions.
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //bool __fastcall AutoRun   (void);
        bool __fastcall ToStopCon (void);
        bool __fastcall ToStartCon(void);
        bool __fastcall ToStart   (void);
        bool __fastcall ToStop    (void);
        bool __fastcall IsWorkEnd (void);

        AnsiString GetPartName(){return m_sPartName;}

        void __fastcall Update         (void) ;
        void __fastcall UpdateSeqState (void) ;

        void __fastcall SetEtcLog();


        enum EN_INSP_MODE {
            imAll ,    //������Ƽ��
            imAnGt,    //������Ƽ.
            imAn  ,    //���۸�.

            MAX_INSP_MODE
        };
        void __fastcall SetServerData    (EN_INSP_MODE ipMode      ,
                                          AnsiString   sJudge      ,
                                          AnsiString   sFirst_NG   ,
                                          AnsiString   sLD_Motor_Y ,
                                          AnsiString   sLD_MOtor_Z ,
                                          AnsiString   sGTX_Motor_X);
        void __fastcall SetServerDataSerial(AnsiString sSerial );


        bool __fastcall InspectActuator(void) ;
        bool __fastcall InspectMotor   (void) ;
        bool __fastcall InspectHomeEnd (void) ;

        //TDateTime GetInspTime() {return m_InspTime ;)
};

//Vision Insp Rslt.        ����ȭ�� �������� X,Y��. ��񿡼��� Y,Z��.
extern double dAngleX      ,dRowAngleX     ;
extern double dAngleY      ,dRowAngleY     ;
extern int    iAngleNG                     ;   //0:OK , 1:AngleNG , 2:NotFound ,
extern bool   bSizeNG                      ;

extern double dShift       ,dRowShift      ;
extern bool   bShiftNG                     ;

extern bool   bVerNG                       ;
extern double dVerGap      ,dRowVerGap     ;

extern bool   bHorNG                       ;
extern double dHorGap      ,dRowHorGap     ;

extern double               dRowBeamX      ;
extern double               dRowBeamY      ;
extern bool   bBeamNG                      ; //�̰͵��� ���� ���� �ʰ� ������. ����.



//Server Log.
//[DVD]
extern AnsiString sAngle             ;
extern AnsiString sFA_R              ;
extern AnsiString sFA_T              ;
extern AnsiString sThree_Beam_Tilt   ;
extern AnsiString sBeam_Size         ;
extern AnsiString sGT_Shift          ;
extern AnsiString sArea              ;
extern AnsiString sPeak_Ratio        ;
extern AnsiString sParallelism       ;
extern AnsiString sAbsolute_Center_X ;
extern AnsiString sAbsolute_Center_Y ;
extern AnsiString sBeam_Center_X     ;
extern AnsiString sBeam_Center_Y     ;
extern AnsiString sPeak_Center_X     ;
extern AnsiString sPeak_Center_Y     ;
extern AnsiString sBeam_Shift        ;
extern AnsiString sBeam_Shift_R      ;
extern AnsiString sBeam_Shift_T      ;
//[COMMON]
extern AnsiString sEvalTime          ;

//Vision Insp Rslt Received.
extern bool   bRcvdRslt;


//Use APC
extern bool   bUseAPC  ;

//APC Last Output Data.
extern double dLastAPCOut ;
//

//---------------------------------------------------------------------------
#endif
