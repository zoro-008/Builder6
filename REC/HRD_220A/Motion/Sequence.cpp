//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//#include "FormMain.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "Index.h"
#include "Picker.h"
#include "Stage.h"
#include "Dispensor.h"
#include "Unloader.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Sequence.h"
//#include "UserFile.h"
#include "DataMan.h"
#include "SLogUnit.h"
#include "ManualMan.h"
#include "LotUnit.h"
#include "OptionMan.h"
#include "SPCUnit.h"

#include "Rs232Temp.h"




//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CSequence SEQ;

/***************************************************************************/
/* ������ & �Ҹ���. (Constructor & Destructor)                             */
/***************************************************************************/
//---------------------------------------------------------------------------
__fastcall CSequence::CSequence(void)
{
}

//---------------------------------------------------------------------------
__fastcall CSequence::~CSequence(void)
{
    //
}

/***************************************************************************/
/* Init.                                                                   */
/***************************************************************************/
//---------------------------------------------------------------------------
void __fastcall CSequence::Init(void)
{
    m_tmToStop   .Clear();
    m_tmToStrt   .Clear();
    m_tmFlickOn  .Clear();
    m_tmFlickOff .Clear();
    m_tmTemp     .Clear();
    m_tmCloseDoor.Clear();

    //Init. Var.
    m_bBtnReset    = false ;
    m_bBtnStart    = false ;
    m_bBtnStop     = false ;
    m_bBtnAir      = false ;

    m_bRun         = false ;
    m_bFlick       = false ;
    m_iStep        = scIdle;
    m_iSeqStat     = ssStop;

    m_pPart[piLDR_F] = &LDR_F ;
    m_pPart[piLDR_R] = &LDR_R ;
    m_pPart[piPRI_F] = &PRI_F ;
    m_pPart[piPRI_R] = &PRI_R ;
    m_pPart[piIDX_F] = &IDX_F ;
    m_pPart[piIDX_R] = &IDX_R ;
    m_pPart[piULD_F] = &ULD_F ;
    m_pPart[piULD_R] = &ULD_R ;
    m_pPart[piSTG  ] = &STG   ;
    m_pPart[piPCK  ] = &PCK   ;
    m_pPart[piDSP  ] = &DSP   ;

}

void __fastcall CSequence::Close(void)
{
}

//---------------------------------------------------------------------------
void CSequence::Reset(void)
{
    //Check running flag.
    if (m_bRun                   ) return;
    if (m_iSeqStat == ssInit     ) return;
    if (m_iStep    == scToStopCon) return;
    if (m_iStep    == scToStop   ) return;

    LOT.Reset();



    Trace("Seq","Reset");

    m_tmToStop   .Clear();
    m_tmToStrt   .Clear();
    m_tmFlickOn  .Clear();
    m_tmFlickOff .Clear();
    m_tmTemp     .Clear();
    m_tmCloseDoor.Clear();

    //Init. Var.
    m_bBtnReset  = false ;
    m_bBtnStart  = false ;
    m_bBtnStop   = false ;

    m_bRun       = false ;
    m_iStep      = scIdle;




    //Error.
    EM_Clear();

    //Manual.
    MM.Reset();

    //Part.
    for(int i=0; i < MAX_PART; i++){
        m_pPart[i] -> Reset();
    }

    //Lot End Flag Reset.
    LOT.Reset();
    FM_CloseMsgOk();

    m_iSeqStat = ssStop;

    AT_Reset   ();

    //PostMessage(FrmSubErr -> Handle , WM_CLOSE , 0 , 0 );

    //System.
    if(!EM_IsErr()) return; //20150801 ����� Ȩ������ ���V ������ Ȩ������ ���󰡼� ó�ھ���.
    MT_ResetAll();
    MT_SetServoAll(true);

}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateButton(void) //�ܺ� ��ư�� �� �������� ��ư���� Ŭ���� �Ǿ����� �����ؼ� �������� ��Ű�� �Լ�.
{
    //Check Inspect.
    if(m_iStep != scIdle) {
        if(!OM.CmnOptn.bIgnrDoor) InspectDoor    () ;
        InspectHomeEnd () ;
        InspectLightGrid() ;
    }

    //Local Var.
    bool isErr     = EM_IsErr() ;
    bool isHomeEnd = MT_GetHomeEndAll();

    //vision manual button.
    static CDelayTimer tmVisnCycle ;
    if(IO_GetX(xETC_StopSw)&& m_iSeqStat == ssStop ){
        //if(tmVisnCycle.OnDelay(true , 1000)) {
        //    tmVisnCycle.Clear();
        //    if(MM.GetManNo() == mcNoneCycle) {
        //        MM.SetManCycle(mcVSN_CycleWork);
        //    }
        //}
    }
    else {
        //tmVisnCycle.Clear();
    }







    bool bStartSw   = IO_GetXUp(xETC_StartSw) || m_bBtnStart ;//|| ( EC.GetPstEqpReady() && IO_GetXUp(xETC_PstEqpStart) && OM.CmnOptn.bAutoOperation);
    bool bStopSw    = IO_GetXUp(xETC_StopSw ) || m_bBtnStop  ;//|| IO_GetXUp(xETC_PstEqpStop) ;
    bool bResetSw   = IO_GetXUp(xETC_ResetSw) || m_bBtnReset ;//|| IO_GetXUp(xETC_PstEqpReset);
    bool bAirSw     = IO_GetXUp(xETC_AirSw  ) || m_bBtnAir   ;
    bool bInitSw    = IO_GetXUp(xETC_InitSw )                ;

    if(IO_GetXUp(xETC_StartSw)){
        Trace("IO_GetXUp(xETC_StartSw)","true");
    }
    if(m_bBtnStart){
        Trace("m_bBtnStart","true");

    }

    if(bStartSw){
        Trace("bStartSw","Started");

        if(m_iSeqStat == ssInit   ) {Trace("SeqStat","ssInit   ");}
        if(m_iSeqStat == ssWorkEnd) {Trace("SeqStat","ssWorkEnd");}
        if(m_iSeqStat == ssError  ) {Trace("SeqStat","ssError  ");}
        if(m_iSeqStat == ssRunning) {Trace("SeqStat","ssRunning");}
        if(m_iSeqStat == ssStop   ) {Trace("SeqStat","ssStop   ");}

        if(MM.GetManNo() != mcNoneCycle) Trace("ManCycle",String(MM.GetManNo()).c_str());

    }


    //Update Switch's Lamp
    bool bStopBtnFlick = (m_iStep == scToStopCon || m_iStep == scToStop) && m_bFlick ;


    //��ư Ŭ���ó�
    IO_SetY(yETC_StartLp  , IO_GetX(xETC_StartSw ) ||  m_bRun);
    IO_SetY(yETC_StopLp   , IO_GetX(xETC_StopSw  ) || !m_bRun || bStopBtnFlick);
    IO_SetY(yETC_ResetLp  , IO_GetX(xETC_ResetSw ) || (m_bFlick && isErr)     );
    IO_SetY(yETC_AirLp    , IO_GetX(xETC_AirSw   ) || IO_GetY(yETC_MainAir)   );
    IO_SetY(yETC_InitLp   , IO_GetX(xETC_InitSw  ) );

    //Center Man Button
    m_bBtnStart = false ;
    m_bBtnStop  = false ;
    m_bBtnReset = false ;
    m_bBtnAir   = false ;

    //Door Sensor.
    bool isAllCloseDoor = IO_GetX(xETC_DoorFt) &&
                          IO_GetX(xETC_DoorLt) &&
                          IO_GetX(xETC_DoorRt) &&
                          IO_GetX(xETC_DoorRr) ;
    static bool bPreAllCloseDoor = isAllCloseDoor ;
    if(bPreAllCloseDoor != isAllCloseDoor) {
        //IO_SetY(yETC_Light , !isAllCloseDoor);
        bPreAllCloseDoor = isAllCloseDoor ;
    }

    //if(isAllCloseDoor && m_bRun){
    //    OM.CmnOptn.bIgnrDoor = false ;
    //
    //}
    if(m_bRun && !OM.CmnOptn.bIgnrDoor) {
        if(!IO_GetX(xETC_DoorFt)){EM_SetErrMsg(eiETC_Door , "Front");}
        if(!IO_GetX(xETC_DoorLt)){EM_SetErrMsg(eiETC_Door , "Left ");}
        if(!IO_GetX(xETC_DoorRt)){EM_SetErrMsg(eiETC_Door , "Right");}
        if(!IO_GetX(xETC_DoorRr)){EM_SetErrMsg(eiETC_Door , "Rear ");}
    }
    if(bStartSw){
        Trace("bStartSw","1");
    }

    //Init. Button Flags.
    if (bStartSw) {
        //bool bAllArayNone = DM.ARAY[riLSP].CheckAllStat(csNone) && DM.ARAY[riLDR].CheckAllStat(csNone) && DM.ARAY[riLST].CheckAllStat(csNone) &&
        //                    DM.ARAY[riPSB].CheckAllStat(csNone) && DM.ARAY[riULD].CheckAllStat(csNone) && DM.ARAY[riVSN].CheckAllStat(csNone)  ;

        if(!OM.CmnOptn.bTempIgnrErrorChk) {
            int iTemp[3] ;
            iTemp[0] = Rs232_TempCh.GetCrntTemp(1); //Rail Bottom
            iTemp[1] = Rs232_TempCh.GetCrntTemp(2); //Rail Top
            iTemp[2] = Rs232_TempCh.GetCrntTemp(3); //Stage

            if(iTemp[0] > OM.DevOptn.iRailBtmTemp + OM.CmnOptn.dTempErrorTol) FM_MsgOk("ERROR" , (String("Rail Bottom�µ��� ���� �µ� ���� ")+ OM.CmnOptn.dTempErrorTol + "�� �̻� �����ϴ�.").c_str());
            if(iTemp[1] > OM.DevOptn.iRailTopTemp + OM.CmnOptn.dTempErrorTol) FM_MsgOk("ERROR" , (String("Rail Top�µ��� ���� �µ� ���� "   )+ OM.CmnOptn.dTempErrorTol + "�� �̻� �����ϴ�.").c_str());
            if(iTemp[2] > OM.DevOptn.iStageTemp   + OM.CmnOptn.dTempErrorTol) FM_MsgOk("ERROR" , (String("Stage �µ��� ���� �µ� ���� "     )+ OM.CmnOptn.dTempErrorTol + "�� �̻� �����ϴ�.").c_str());

            if(iTemp[0] < OM.DevOptn.iRailBtmTemp - OM.CmnOptn.dTempErrorTol) FM_MsgOk("ERROR" , (String("Rail Bottom�µ��� ���� �µ� ���� ")+ OM.CmnOptn.dTempErrorTol + "�� �̻� �����ϴ�.").c_str());
            if(iTemp[1] < OM.DevOptn.iRailTopTemp - OM.CmnOptn.dTempErrorTol) FM_MsgOk("ERROR" , (String("Rail Top�µ��� ���� �µ� ���� "   )+ OM.CmnOptn.dTempErrorTol + "�� �̻� �����ϴ�.").c_str());
            if(iTemp[2] < OM.DevOptn.iStageTemp   - OM.CmnOptn.dTempErrorTol) FM_MsgOk("ERROR" , (String("Stage �µ��� ���� �µ� ���� "     )+ OM.CmnOptn.dTempErrorTol + "�� �̻� �����ϴ�.").c_str());
        }

        if(!isHomeEnd           ){FM_MsgOk("WARNING" , "��� Ȩ�� ����ּ���."); bStartSw = false ; }
        if(!DSP.m_bNiddleChecked){FM_MsgOk("WARNING" , "�ϵ� Z�� ������ �����ϼ���."); bStartSw = false ; }; //
        //if(LTQ.GetLotCnt() == 0 && bAllArayNone) {FM_MsgOk("WARNING" , "Please Lot Open"); bStartSw = false ; }
        if(!InspectStripDispr  ()) { m_bInspDispr = true ; bStartSw = false ; }
        if(!InspectStripUnknown()) { m_bInspUnkwn = true ; bStartSw = false ; }
    }
    if(bStartSw){
        Trace("bStartSw","2");
    }

    if (bInitSw) {
        //sun sun InspectMainAir(void) �̰� �����ؼ� ��������.
        MM.SetManCycle(mcAllHome);
    }

    //Air Switch.
    if(bAirSw && !m_bRun && m_iSeqStat != ssInit) {
        IO_SetY(yETC_MainAir , !IO_GetY(yETC_MainAir )) ;
    }

    //Buzzer Off.
    if (isErr && bStopSw) TL_MuteSnd(true);

    //Set Condition Flags.
    if( bStartSw) { //��ŸƮ��ư �ȴ����°� ������ �׽�Ʈ.
        Trace("isErr" , isErr ? "true":"false");
        Trace("MM.GetManNo()",String(MM.GetManNo()).c_str());
    }

    bool isStopCon  = bStopSw  || (isErr  && !m_bReqStop &&  m_bRun) ;
    bool isRunCon   = bStartSw && !isErr  && MM.GetManNo() == mcNone ;
    bool isResetCon = bResetSw && !m_bRun ;

    //Run.
    if (isRunCon && !m_iStep) {
        m_iStep = scToStartCon ;
        EM_SetDisp(true );
        TL_MuteSnd(false);
    }
    if( isRunCon && m_iStep) { //��ŸƮ��ư �ȴ����°� ������ �׽�Ʈ.
        Trace("isRunCon && m_iStep" , String(m_iStep).c_str());
    }
    if( isStopCon  &&  m_iStep) { //��ŸƮ��ư �ȴ����°� ������ �׽�Ʈ.
        Trace("isStopCon  &&  m_iStep" , String(m_iStep).c_str());
        Trace("bStopSw"                , bStopSw    ? "True" : "False");
        Trace("isErr"                  , isErr      ? "True" : "False");
        Trace("m_bReqStop"             , m_bReqStop ? "True" : "False");
        Trace("m_bRun"                 , m_bRun     ? "True" : "False");
    }
    if (isStopCon  &&  m_iStep) m_bReqStop = true ;
    if (isResetCon && !m_iStep) Reset()           ;

    if (m_tmToStrt.OnDelay(m_iStep == scToStartCon || m_iStep == scToStart , 10000)) {
        //Trace Log.
        AnsiString Msg ;
        Msg.sprintf("ToStrtTimeOut : m_iStep=%d" ,m_iStep );
        Trace  ("SEQ",Msg.c_str() );
        EM_SetErr (eiETC_ToStartTO);
        m_iStep = scIdle;
        m_bRun  = false;
    }


    static int iToStopTimeOut = 20000;
    enum EN_CYCLE {
        ccEtc  = 0 ,
        ccDisp     ,
        ccAttach   ,
        ccPush
    };
    static EN_CYCLE iPreSeqStep = ccEtc ;
    EN_CYCLE iSeqStep ;
    if(DSP.GetSeqStep() == CDispensor::scDisp) {
        iSeqStep = ccDisp ;
    }
    else if(PCK.GetSeqStep() == CPicker::scAttach) {
        iSeqStep = ccAttach ;
    }
    else if(PCK.GetSeqStep() == CPicker::scPush){
        iSeqStep = ccPush ;
    }
    else {
        iSeqStep = ccEtc ;
    }

    if(iPreSeqStep != iSeqStep) {
        if(DSP.GetSeqStep() == CDispensor::scDisp) {
            m_tmToStop.Clear();
            iToStopTimeOut = 60000 * 3 ; //3�� ���� �˳��ϰ� ����.
            iPreSeqStep = iSeqStep ;
        }
        else if(PCK.GetSeqStep() == CPicker::scAttach) {
            m_tmToStop.Clear();
            iToStopTimeOut = 30000 ;
            iPreSeqStep = iSeqStep ;
        }
        else if(PCK.GetSeqStep() == CPicker::scPush){
            m_tmToStop.Clear();
            iToStopTimeOut = OM.DevOptn.iPkPushDelay + 30000 ;
            if(OM.DevInfo.iWorkMode == wmFos) iToStopTimeOut +=  OM.DevOptn.iFosPlaceBfDelay ;
            iPreSeqStep = iSeqStep ;
        }
        else {
            m_tmToStop.Clear();
            iToStopTimeOut = 20000 ;
            iPreSeqStep = iSeqStep ;
        }
    }

    //m_tmToStop.Clear();
    //if( )m_tmToStop.Clear();
    //if( )m_tmToStop.Clear();

    if (m_tmToStop.OnDelay(m_iStep == scToStopCon || m_iStep == scToStop , iToStopTimeOut)){//  20000)) {
        //Trace Log.
        AnsiString Msg;
        Msg.sprintf("ToStopTimeOut : m_iStep=%d", m_iStep  );
        Trace("SEQ",Msg.c_str());
        m_bRun = false ;
        //ToStop();
        EM_SetErr(eiETC_ToStopTO);
        m_iStep    = scIdle;
        m_bReqStop = false;
    }

    static EN_SEQ_STEP iPreStep = m_iStep ;
    if(iPreStep != m_iStep) {
        String sMsg = "" ;
        sMsg = "Step Changed" + String(iPreStep) + " -> " + String(m_iStep) ;
        Trace("SEQ",sMsg.c_str());
    }
    iPreStep = m_iStep ;


    //�̻��ϰ� �߰��� �����尡 �Ǵ� ���� �߰��ؼ� �־��.
    static bool bPreLotEnd = LOT.GetLotEnd() ;
    if(LOT.GetLotEnd() != bPreLotEnd) {
        Trace("SEQ",LOT.GetLotEnd() ? "LotEnd True" : "LotEnd False");
    }
    bPreLotEnd = LOT.GetLotEnd() ;



    //Running Step.
    switch (m_iStep) {
        case scIdle       : return;

        case scToStartCon : if(!ToStartCon()) return ;
                            m_iStep=scToStart;
                            Trace("SEQ","scToStartCon END");
                            return ;

        case scToStart    : if(!ToStart()) return ;
                            m_bRun = true ;
                            m_iStep=scRun ;
                            Trace("SEQ","scToStart END");
                            return ;

        case scRun        : if(!m_bReqStop) {
                                if(Autorun()) {
                                    //LSR.m_bAutoRun = true;
                                    //������ ��Ȳ.
                                    LOT.LotEnd();
                                    Trace("SEQ","scRun LotEnd");
                                    m_iStep=scToStopCon ;
                                }
                                return ;
                            }
                            m_bReqStop = false ;
                            m_iStep=scToStopCon ;
                            Trace("SEQ","scRun END");
                            return ;

        case scToStopCon :  if(!ToStopCon()) {
                                if(Autorun()) {
                                    //������ ��Ȳ.
                                    LOT.LotEnd();
                                    Trace("SEQ","scToStopCon LotEnd");
                                }
                                return ;
                            }
                            m_bRun = false ;
                            m_iStep=scToStop;
                            Trace("SEQ","scToStopCon END");
                            return ;

        case scToStop    :  if (!ToStop()) return ;
                            m_iStep = scIdle ;
                            m_bReqStop = false ;

                            DM.SaveMap();
                            Trace("SEQ","scToStop END");

                            return;
        }
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMainAir(void)
{                                                                                         
    bool isOk = true ;

    if(!IO_GetX(xETC_MainAir) || !IO_GetY(yETC_MainAir)) isOk = false ;

    if(!isOk) EM_SetErr(eiETC_MainAir) ;


    return isOk ;                                                                         
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectTemp(void)
{
    bool isOk = true ;

    //if(IO_GetX(xPSB_TempAlrm)){EM_SetErr(eiPRB_TempZ1Alrm); isOk = false ;}

    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectEmergency(void)
{
    bool isOk = true ;

    //Check Emergency
    if (IO_GetX(xETC_EmgSw)) {
        EM_SetErr(eiETC_Emergency); MT_SetServoAll(false); isOk = false ;
    }

    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectLightGrid(void)
{
    bool isOk = true ;
    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectDoor(void)
{
    //Local Var.
    bool isOk = true;

    //Check Door
    //if (!IO_GetX(xETC_FDoor) ){EM_SetErr(eiETC_FDoor); isOk = false;} //Front Door Sensor
    //if (!IO_GetX(xETC_BDoor) ){EM_SetErr(eiETC_BDoor); isOk = false;} //Front Door Sensor

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectActuator(void)
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    for(int i = 0 ; i < MAX_ACTR ; i++) {
        if(AT_Err(i)) {EM_SetErrMsg(eiATR_TimeOut ,AT_GetName(i)) ; isOk = false ; }
    }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectMotor(void)
{
    //Local Var.
    bool isOk  = true;

    for(int i = 0 ; i < MAX_MOTR ; i++) {
        if (MT_GetAlarm   (i)                  ){EM_SetErrMsg(eiMTR_Alarm  , MT_GetName(i)); isOk = false; }
        if (MT_GetNLimSnsr(i)&&MT_GetHomeEnd(i)){EM_SetErrMsg(eiMTR_NegLim , MT_GetName(i)); isOk = false; }
        if (MT_GetPLimSnsr(i)&&MT_GetHomeEnd(i)){EM_SetErrMsg(eiMTR_PosLim , MT_GetName(i)); isOk = false; }
    }

    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectHomeEnd()
{
    //Local Var.
    bool isOk = true;

    //Inspect.
    for(int i = 0 ; i < MAX_MOTR ; i++) {
        if (!MT_GetHomeEnd(i)){EM_SetErrMsg(eiMTR_HomeEnd , MT_GetName(i)); isOk = false; }
    }


    //Ok.
    return isOk;
}

//---------------------------------------------------------------------------
bool  __fastcall CSequence::InspectCrash(void)
{
    if(IO_GetX(xDSP_HeadDamege)) {
        if(MM.GetManNo() != mcAllHome){ //�ϵ���� �浹 �˶��ÿ� ��Ȩ���� Ǯ�� �ִ�.
            if(!MT_GetStop(miDSP_XDispr))MT_EmgStop(miDSP_XDispr);
            if(!MT_GetStop(miPCK_YPickr))MT_EmgStop(miPCK_YPickr);
            EM_SetErrMsg(eiPRT_Crash , "�� �ϵ���� �浹 ������ ���� �Ǿ����ϴ�.");
            return false ;
        }
    }
    return true ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectStripDispr(void)
{
    bool isOk  = true;

//    if(DM.ARAY[riWRK].GetCntExist() && !IO_GetX(xWRK_Detect)  && !OM.MstOptn.bDryRun ) isOk = false ;
//    if(DM.ARAY[riPSB].GetCntExist() &&  !OM.MstOptn.bDryRun) {
//        if(!IO_GetX(xPSB_LDetect) && !IO_GetX(xPSB_RDetect) && !IO_GetX(xPSB_Pkg1) && !IO_GetX(xPSB_Pkg2) && !IO_GetX(xPSB_PkgOut1) && !IO_GetX(xPSB_PkgOut2)) isOk = false ;
//    }
//
//    if (OM.EqpOptn.bExistLoader  ) { if(!DM.ARAY[riLDR].CheckAllStat(csNone) && ( !IO_GetX(xLDR_MgzDetect1) && !IO_GetX(xLDR_MgzDetect2) ) ) isOk = false ; }
//    if (OM.EqpOptn.bExistUnLoader) { if(!DM.ARAY[riULD].CheckAllStat(csNone) && ( !IO_GetX(xULD_MgzDetect1) && !IO_GetX(xULD_MgzDetect2) ) ) isOk = false ; }
//
    return isOk ;
}

//---------------------------------------------------------------------------
bool __fastcall CSequence::InspectStripUnknown(void)
{
//    if (DM.ARAY[riPR2].CheckAllStat(csNone) && DM.ARAY[riPR1].CheckAllStat(csNone) && IO_GetX(xPRB_3Pkg) ) return false ;
    return true ;
}

//---------------------------------------------------------------------------
bool CSequence::ToStartCon(void)
{
    bool bRet[MAX_PART];
    //Call ToStop.

    for(int i=0; i < MAX_PART; i++){
        bRet[i] = m_pPart[i] -> ToStartCon();
    }

    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!bRet[i]) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool CSequence::ToStopCon(void)
{
    bool bRet[MAX_PART];
    //Call ToStop.


    for(int i=0; i < MAX_PART; i++){
        bRet[i] = m_pPart[i] -> ToStopCon();
    }
    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!bRet[i]) return false ;
    }

    return true ;

}

//---------------------------------------------------------------------------
bool CSequence::ToStart(void)
{
    bool bRet[MAX_PART];
    //Call ToStop.
    for(int i=0; i < MAX_PART; i++){
        bRet[i] = m_pPart[i] -> ToStart();
    }


    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!bRet[i]) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool CSequence::ToStop(void)
{
    bool bRet[MAX_PART];
    //Call ToStop.
    for(int i=0; i < MAX_PART; i++){
        bRet[i] = m_pPart[i] -> ToStop();
    }

    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!bRet[i]) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
bool CSequence::Autorun(void)
{
    bool bRet[MAX_PART];
    //Call ToStop.
    for(int i=0; i < MAX_PART; i++){
        bRet[i] = m_pPart[i] -> Autorun();
    }

    for(int i = 0 ; i < MAX_PART ; i++) {
        if(!bRet[i]) return false ;
    }

    return true ;
}

//---------------------------------------------------------------------------
void __fastcall CSequence::UpdateSeqState(void)
{
    //Local Var.
    bool isInit    =  MM.GetManNo() == mcAllHome ;
    bool isError   =  EM_IsErr() ;                 
    bool isRunning =  m_bRun     ;                 
    bool isStop    = !m_bRun     ;                 
    bool isLotEnd  =  LOT.GetLotEnd() ;

    //Flicking Timer.
    if (m_bFlick) { m_tmFlickOn .Clear(); if (m_tmFlickOff.OnDelay( m_bFlick , 500)) m_bFlick = false; }
    else          { m_tmFlickOff.Clear(); if (m_tmFlickOn .OnDelay(!m_bFlick , 500)) m_bFlick = true ; }

    //Set Sequence State. Tower Lamp
         if (isInit   ) { m_iSeqStat = ssInit       ;}
    else if (isLotEnd ) { m_iSeqStat = ssWorkEnd    ;}
    else if (isError  ) { m_iSeqStat = ssError      ;}
    else if (isRunning) { m_iSeqStat = ssRunning    ;}
    else if (isStop   ) { m_iSeqStat = ssStop       ;}
    else                { m_iSeqStat = m_iSeqStat   ;}

}

//---------------------------------------------------------------------------
void __fastcall CSequence::Update(void)
{
    //Update DeviceNet (I/O).
    SM_Update(m_iSeqStat);

    //Part.
    for(int i=0; i < MAX_PART; i++){
        m_pPart[i] -> Update();
    }

    //Inspection.
    InspectMainAir  () ;
    InspectEmergency() ;
    InspectActuator () ;
    InspectMotor    () ;
    InspectCrash    () ;

    //Update ErrorProc.
    UpdateButton();

    //Check Button.
    UpdateSeqState();

    //Update Motor State (Input).
    MM.Update();

    //SPC
    SPC.Update(m_iSeqStat);

    DSP.Rs232_Dispr.Update();

}

