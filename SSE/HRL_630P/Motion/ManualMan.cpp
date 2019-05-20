//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "FrontFeed.h"
#include "LearFeed.h"
#include "FrontLaser.h"
#include "LearLaser.h"
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "PstnMan.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------------------------
CManualMan MM;

//---------------------------------------------------------------------------
__fastcall CManualMan::CManualMan(void)
{

}

//---------------------------------------------------------------------------
__fastcall CManualMan::~CManualMan(void)
{

}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Init(void)
{
    //Init. Var.
    m_iManStep    = 0     ;
    m_iPreManStep = 0     ;
    m_iManNo      = mcNoneCycle ;

    m_bManSetting = false ;

    m_tmCycle.Clear() ;
}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Close(void)
{
}

//---------------------------------------------------------------------------
bool __fastcall CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo     <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo     >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if(SEQ._bRun                     ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return false ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.

    //if(!IO_GetX(xETC_PowerOn)                          ) {FM_MsgOk("ERR","Power On Plz");      return false ;}
    //if(!IO_GetX(xETC_MainAir) || !IO_GetY(yETC_MainSol)) {FM_MsgOk("ERROR","Check Main Air!"); return false ;}

    EM_SetDisp(true);


    int c,r ;
    int iMaxRow,iMaxCol;


    m_bManSetting = true ; //업데이트 함수에서 다른쓰레드로 들어와서 갱신하기에 플레그 걸어 막아둠.
    m_iManNo = _iNo ;

    double dOffset       = OM.MstOptn.dOffset ;

    double dLearYCrntPos = MT_GetCmdPos(miFED_YLer) ;
    double dFrntYCrntPos = MT_GetCmdPos(miFED_YFrt) ;
    double dLearYFeedPos = PM.GetValue (miFED_YLer , pvFED_YLerFeed ) ;
    double dFrntYFeedPos = PM.GetValue (miFED_YFrt , pvFED_YFrtFeed ) ;
    double dLearYWorkPos = PM.GetValue (miFED_YLer , pvFED_YLerWork ) ;
    double dFrntYWorkPos = PM.GetValue (miFED_YFrt , pvFED_YFrtWork ) ;

    double dLearTCrntPos = MT_GetCmdPos(miLSR_TLer) ;
    double dLearTWaitPos = PM.GetValue (miLSR_TLer , pvLSR_TLerWait ) ;
    double dFrntTCrntPos = MT_GetCmdPos(miLSR_TFrt) ;
    double dFrntTWaitPos = PM.GetValue (miLSR_TFrt , pvLSR_TFrtWait ) ;

    bool bFrntYWorkPos = (dFrntYWorkPos - dOffset <= dFrntYCrntPos) && (dFrntYCrntPos <= dFrntYWorkPos + dOffset) ;
    bool bLearYWorkPos = (dLearYWorkPos - dOffset <= dLearYCrntPos) && (dLearYCrntPos <= dLearYWorkPos + dOffset) ;

    bool bFrntYFeedPos = (dFrntYFeedPos - dOffset <= dFrntYCrntPos) && (dFrntYCrntPos <= dFrntYFeedPos + dOffset) ;
    bool bLearYFeedPos = (dLearYFeedPos - dOffset <= dLearYCrntPos) && (dLearYCrntPos <= dLearYFeedPos + dOffset) ;

    bool bFrntTWaitPos = (dFrntTWaitPos - dOffset <= dFrntTCrntPos) && (dFrntTCrntPos <= dFrntTWaitPos + dOffset) ;
    bool bLearTWaitPos = (dLearTWaitPos - dOffset <= dLearTCrntPos) && (dLearTCrntPos <= dLearTWaitPos + dOffset) ;

    bool bCoverFwd     = !AT_Complete(aiFED_Covr_FwBw, ccBwd) ;

    bool bRet = true ;

    /********************/
    /* 프리프로세서     */
    /********************/

    bool bFrntStgFwd  = !AT_Complete(aiLSR_StgF_FwBw, ccBwd) ;
    bool bLearStgFwd  = !AT_Complete(aiLSR_StgL_FwBw, ccBwd) ;

         if(m_iManNo == mcAllHome     ) { if(!MT_GetServoAll()) {FM_MsgTime("ERR","Please All Motor is Servo On!" ,2000); bRet = false ;} }
    else if(m_iManNo == mcFFD_Home    ) { }
    else if(m_iManNo == mcLFD_Home    ) { }
    else if(m_iManNo == mcFLS_Home    ) { }
    else if(m_iManNo == mcLLS_Home    ) { }

    else if(m_iManNo == mcETC_Feed    ) { }
    else if(m_iManNo == mcETC_DstSck  ) { }
    else if(m_iManNo == mcETC_Airblow1) { }
    else if(m_iManNo == mcETC_Airblow2) { }
    else if(m_iManNo == mcETC_Vcc1    ) { }
    else if(m_iManNo == mcETC_Vcc2    ) { }
    else if(m_iManNo == mcETC_Covr    ) { }
    else if(m_iManNo == mcETC_Algn    ) { }

    else if(m_iManNo == mcFFD_Stage   ) { if(!bFrntYFeedPos) {FM_MsgTime("ERR","Front Y Motor is not Wait Pos!" ,2000); bRet = false ;}
                                          if(!bFrntTWaitPos) {FM_MsgTime("ERR","Front T Motor is not Wait Pos!" ,2000); bRet = false ;}
                                          if( bCoverFwd    ) {FM_MsgTime("ERR","Cover is Not BWD!"              ,2000); bRet = false ;} }

    else if(m_iManNo == mcFFD_Vcc     ) { if(IO_GetX(xLSR_VccSnsrF)){
                                              if(FM_MsgYesNo("WARNING","Package Exists. Are you want to switch off Vacuum?" )!= mrYes){
                                                  bRet = false ;
                                              }}}
    else if(m_iManNo == mcFFD_Ejct    ) { if(IO_GetX(xLSR_VccSnsrF)){
                                              if(FM_MsgYesNo("WARNING","Package Exists. Are you want to switch on Ejector?" )!= mrYes){
                                                  bRet = false ;
                                              }}}
    else if(m_iManNo == mcFFD_In      ) { if( bLearYFeedPos) {FM_MsgTime("ERR","Lear Y Motor is Feed Pos!" ,2000); bRet = false ;} }
    else if(m_iManNo == mcFFD_Out     ) { if( bLearYFeedPos) {FM_MsgTime("ERR","Lear Y Motor is Feed Pos!" ,2000); bRet = false ;} }
    else if(m_iManNo == mcFFD_Align   ) { if( bLearYFeedPos) {FM_MsgTime("ERR","Lear Y Motor is Feed Pos!" ,2000); bRet = false ;} }

    else if(m_iManNo == mcLFD_Stage   ) { if(!bLearYFeedPos) {FM_MsgTime("ERR","Lear Y Motor is not Feed Pos!" ,2000); bRet = false ;}
                                          if(!bLearTWaitPos) {FM_MsgTime("ERR","Lear T Motor is not Wait Pos!" ,2000); bRet = false ;}
                                          if( bCoverFwd    ) {FM_MsgTime("ERR","Cover is Not BWD!"             ,2000); bRet = false ;} }
    else if(m_iManNo == mcLFD_Vcc     ) { if(IO_GetX(xLSR_VccSnsrL)){
                                              if(FM_MsgYesNo("WARNING","Package Exists. Are you want to switch off Vacuum?" )!= mrYes){
                                                  bRet = false ;
                                              }}}
    else if(m_iManNo == mcLFD_Ejct    ) { if(IO_GetX(xLSR_VccSnsrL)){
                                              if(FM_MsgYesNo("WARNING","Package Exists. Are you want to switch on Ejector?" )!= mrYes){
                                                  bRet = false ;
                                              }}}

    else if(m_iManNo == mcLFD_In      ) { if( bFrntYFeedPos) {FM_MsgTime("ERR","Front Y Motor is Feed Pos!" ,2000); bRet = false ;} }
    else if(m_iManNo == mcLFD_Out     ) { if( bFrntYFeedPos) {FM_MsgTime("ERR","Front Y Motor is Feed Pos!" ,2000); bRet = false ;} }
    else if(m_iManNo == mcLFD_Align   ) { if( bFrntYFeedPos) {FM_MsgTime("ERR","Front Y Motor is Feed Pos!" ,2000); bRet = false ;} }

    else if(m_iManNo == mcFLS_Work    ) { if(!bFrntYWorkPos) {FM_MsgTime("ERR","Front Y Motor is not Work Pos!" ,2000); bRet = false ;}}
    else if(m_iManNo == mcFLS_Place   ) { if(!bFrntYWorkPos) {FM_MsgTime("ERR","Front Y Motor is not Work Pos!" ,2000); bRet = false ;}}

    else if(m_iManNo == mcLLS_Work    ) { if(!bLearYWorkPos) {FM_MsgTime("ERR","Lear Y Motor is not Work Pos!" ,2000); bRet = false ;}}
    else if(m_iManNo == mcLLS_Place   ) { if(!bLearYWorkPos) {FM_MsgTime("ERR","Lear Y Motor is not Work Pos!" ,2000); bRet = false ;}}

    if(!bRet) Init();



    /********************/
    /* 처리..           */
    /********************/

         if(m_iManNo == mcAllHome     ) { FFD.InitHomeStep();
                                          LFD.InitHomeStep();
                                          FLS.InitHomeStep();
                                          LLS.InitHomeStep(); }

    else if(m_iManNo == mcFFD_Home    ) { FFD.InitHomeStep(); }
    else if(m_iManNo == mcLFD_Home    ) { LFD.InitHomeStep(); }
    else if(m_iManNo == mcFLS_Home    ) { FLS.InitHomeStep(); }
    else if(m_iManNo == mcLLS_Home    ) { LLS.InitHomeStep(); }

    else if(m_iManNo == mcETC_Feed    ) {     IO_SetY (yETC_Feeder     , !IO_GetY  (yETC_Feeder     )); }
    else if(m_iManNo == mcETC_DstSck  ) {     IO_SetY (yETC_DustSuck   , !IO_GetY  (yETC_DustSuck   )); }
    else if(m_iManNo == mcETC_Airblow1) {     IO_SetY (yFED_AirBlow1   , !IO_GetY  (yFED_AirBlow1   )); }
    else if(m_iManNo == mcETC_Airblow2) {     IO_SetY (yFED_AirBlow2   , !IO_GetY  (yFED_AirBlow2   )); }
    else if(m_iManNo == mcETC_Vcc1    ) {     IO_SetY (yFED_Vcc1       , !IO_GetY  (yFED_Vcc1       )); }
    else if(m_iManNo == mcETC_Vcc2    ) {     IO_SetY (yFED_Vcc2       , !IO_GetY  (yFED_Vcc2       )); }
    else if(m_iManNo == mcETC_Covr    ) { FFD.MoveActr(aiFED_Covr_FwBw , !AT_GetCmd(aiFED_Covr_FwBw )); }
    else if(m_iManNo == mcETC_Algn    ) { FFD.MoveActr(aiFED_Algn_FwBw , !AT_GetCmd(aiFED_Algn_FwBw )); }

    else if(m_iManNo == mcFFD_Stage   ) { FFD.MoveActr(aiLSR_StgF_FwBw , !AT_GetCmd(aiLSR_StgF_FwBw )); }
    else if(m_iManNo == mcFFD_Vcc     ) {     IO_SetY (yLSR_VccF       , !IO_GetY  (yLSR_VccF       )); }
    else if(m_iManNo == mcFFD_Ejct    ) {     IO_SetY (yLSR_EjctF      , !IO_GetY  (yLSR_EjctF      )); }
    else if(m_iManNo == mcFFD_In      ) { FFD.InitCycleStep();                                          }
    else if(m_iManNo == mcFFD_Out     ) { FFD.InitCycleStep();                                          }
    else if(m_iManNo == mcFFD_Align   ) { m_iManStep = 10 ; m_iPreManStep = 0;                          }

    else if(m_iManNo == mcLFD_Stage   ) { LFD.MoveActr(aiLSR_StgL_FwBw , !AT_GetCmd(aiLSR_StgL_FwBw )); }
    else if(m_iManNo == mcLFD_Vcc     ) {     IO_SetY (yLSR_VccL       , !IO_GetY  (yLSR_VccL       )); }
    else if(m_iManNo == mcLFD_Ejct    ) {     IO_SetY (yLSR_EjctL      , !IO_GetY  (yLSR_EjctL      )); }
    else if(m_iManNo == mcLFD_In      ) { LFD.InitCycleStep();                                          }
    else if(m_iManNo == mcLFD_Out     ) { LFD.InitCycleStep();                                          }
    else if(m_iManNo == mcLFD_Align   ) { m_iManStep = 10 ; m_iPreManStep = 0;                          }

    else if(m_iManNo == mcFLS_Work    ) { FLS.InitCycleStep();                                          }
    else if(m_iManNo == mcFLS_Place   ) { FLS.InitCycleStep();                                          }

    else if(m_iManNo == mcLLS_Work    ) { LLS.InitCycleStep();                                          }
    else if(m_iManNo == mcLLS_Place   ) { LLS.InitCycleStep();                                          }


    m_bManSetting = false ; //m_bManSetting 중요함 리턴전에 꼭 펄스 시켜야함. 쓰레드가 달라서. ::Update에서 m_iManNo=0이 되므로 주의.
    return true ;
}

EN_MANUAL_CYCLE __fastcall CManualMan::GetManNo()
{
    return m_iManNo ;
}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Update(void)
{
    if( m_iManNo == mcNoneCycle ) return ;
    if(m_bManSetting            ) return ;

    if(!OM.CmnOptn.bIgnrDoorOpen ) SEQ.InspectDoor    ();
    if(m_iManNo != mcFFD_Home &&
       m_iManNo != mcLFD_Home &&
       m_iManNo != mcFLS_Home &&
       m_iManNo != mcLLS_Home &&
       m_iManNo != mcAllHome    ) {
           SEQ.InspectHomeEnd ();
       }
    SEQ.InspectLightGrid();

    bool r1,r2,r3,r4 ;
    r1=r2=r3=r4 = false ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }
    //Cycle Step.

    if      (m_iManNo == mcAllHome  ) {  r3  = FFD.CycleHome();
                                        r4  = LFD.CycleHome();
                                        if(r3 && r4){
                                            r1  = FLS.CycleHome();
                                            r2  = LLS.CycleHome();
                                        }
                                        if(r1&&r2&&r3&&r4) {
                                            m_iManNo = mcNoneCycle;
                                            FM_MsgOk("Confirm","All Home Finished!");
                                        }
                                      }

    else if(m_iManNo == mcFFD_Home  ) { if(FFD.CycleHome  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcFFD_In    ) { if(FFD.CycleIn    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcFFD_Out   ) { if(FFD.CycleOut   ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcFFD_Align ) { if(CycleFrntAlign ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcLFD_Home  ) { if(LFD.CycleHome  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLFD_In    ) { if(LFD.CycleIn    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLFD_Out   ) { if(LFD.CycleOut   ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLFD_Align ) { if(CycleLearAlign ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcFLS_Home  ) { if(FLS.CycleHome  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcFLS_Work  ) { if(FLS.CycleWork  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcFLS_Place ) { if(FLS.CyclePlace ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcLLS_Home  ) { if(LLS.CycleHome  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLLS_Work  ) { if(LLS.CycleWork  ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLLS_Place ) { if(LLS.CyclePlace ()) m_iManNo = mcNoneCycle; }

    else                              {                       m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;
}
//---------------------------------------------------------------------------
bool CManualMan::CycleFrntAlign()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: FFD.MoveActr(aiFED_Algn_FwBw , ccBwd);
                  FFD.MoveActr(aiFED_Covr_FwBw , ccBwd);

                  m_iManStep++;
                  return false;

        case  11: if(!FFD.MoveActr(aiFED_Algn_FwBw , ccBwd)) return false;
                  if(!FFD.MoveActr(aiFED_Covr_FwBw , ccBwd)) return false;

                  FFD.MoveActr(aiLSR_StgF_FwBw , ccBwd);

                  m_iManStep++;
                  return false;

        case  12: if(!FFD.MoveActr(aiLSR_StgF_FwBw , ccBwd)) return false;

                  FLS.MoveMotr(miLSR_TFrt , piLSR_TFrtWait);

                  m_iManStep++;
                  return false;

        case  13: if(!FLS.MoveMotr(miLSR_TFrt , piLSR_TFrtWait))return false;

                  FFD.MoveMotr(miFED_YFrt , piFED_YFrtFeed);

                  m_iManStep++;
                  return false;

        case  14: if(!FFD.MoveMotr(miFED_YFrt , piFED_YFrtFeed)) return false;

                  FFD.MoveActr(aiLSR_StgF_FwBw , ccFwd);

                  m_iManStep++;
                  return false;

        case  15: if(!FFD.MoveActr(aiLSR_StgF_FwBw , ccFwd))return false;

                  FFD.MoveActr(aiFED_Covr_FwBw , ccFwd);

                  m_iManStep++;
                  return false;

        case  16: if(!FFD.MoveActr(aiFED_Covr_FwBw , ccFwd))return false;

                  FFD.MoveActr(aiFED_Algn_FwBw , ccFwd);
                  m_tmDelay.Clear();

                  m_iManStep++;
                  return false;

        case  17: if(!FFD.MoveActr(aiFED_Algn_FwBw , ccFwd))return false;
                  if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iAlignTm))return false;

                  FFD.MoveActr(aiFED_Algn_FwBw , ccBwd);
                  DM.ARAY[riFFD].SetStat(csUnkwn);

                  m_iManStep = 0;
                  return true;

        }
}
//---------------------------------------------------------------------------
bool CManualMan::CycleLearAlign()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiETC_ManCycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
        m_iManStep = 0 ;
        return true;
    }

    if(m_iManStep != m_iPreManStep) {
        sTemp = sTemp.sprintf("%s m_iManStep=%02d" , __FUNC__ , m_iManStep );
        Trace("Manual Man",sTemp.c_str());
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: LFD.MoveActr(aiFED_Algn_FwBw , ccBwd);
                  LFD.MoveActr(aiFED_Covr_FwBw , ccBwd);

                  m_iManStep++;
                  return false;

        case  11: if(!LFD.MoveActr(aiFED_Algn_FwBw , ccBwd)) return false;
                  if(!LFD.MoveActr(aiFED_Covr_FwBw , ccBwd)) return false;

                  LFD.MoveActr(aiLSR_StgL_FwBw , ccBwd);

                  m_iManStep++;
                  return false;

        case  12: if(!LFD.MoveActr(aiLSR_StgL_FwBw , ccBwd)) return false;

                  LLS.MoveMotr(miLSR_TLer , piLSR_TLerWait);

                  m_iManStep++;
                  return false;

        case  13: if(!LLS.MoveMotr(miLSR_TLer , piLSR_TLerWait))return false;

                  LFD.MoveMotr(miFED_YLer , piLSR_TLerWait);

                  m_iManStep++;
                  return false;

        case  14: if(!LFD.MoveMotr(miFED_YLer , piLSR_TLerWait))return false;

                  LFD.MoveActr(aiLSR_StgL_FwBw , ccFwd);

                  m_iManStep++;
                  return false;

        case  15: if(!LFD.MoveActr(aiLSR_StgL_FwBw , ccFwd))return false;

                  LFD.MoveActr(aiFED_Covr_FwBw , ccFwd);

                  m_iManStep++;
                  return false;

        case  16: if(!LFD.MoveActr(aiFED_Covr_FwBw , ccFwd))return false;

                  LFD.MoveActr(aiFED_Algn_FwBw , ccFwd);
                  m_tmDelay.Clear();

                  m_iManStep++;
                  return false;

        case  17: if(!LFD.MoveActr(aiFED_Algn_FwBw , ccFwd))return false;
                  if(!m_tmDelay.OnDelay(true , OM.CmnOptn.iAlignTm))return false;

                  LFD.MoveActr(aiFED_Algn_FwBw , ccBwd);
                  DM.ARAY[riLFD].SetStat(csUnkwn);

                  m_iManStep = 0;
                  return true;

        }
}
