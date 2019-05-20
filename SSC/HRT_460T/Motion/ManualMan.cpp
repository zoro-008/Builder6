//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuffer.h"
#include "WorkZone.h"
#include "PostBuff.h"
#include "PstnMan.h"
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "Rs232Man.h"
#include "ProbeComUnit.h"
//---------------------------------------------------------------------------
#include "FormDeviceSet.h"

#pragma package(smart_init)
int    iTrmManRow = -1;
int    iDevManRow = -1;
int    iTypeMan   = -1;
//----------------------------------------------------------------------------
CManualMan MM;

//---------------------------------------------------------------------------
__fastcall CManualMan::CManualMan(void)
{
    //Init. Var.
    Init();
}

//---------------------------------------------------------------------------
__fastcall CManualMan::~CManualMan(void)
{

}

//---------------------------------------------------------------------------
void CManualMan::Init()
{
    //Init. Var.
    m_iManStep    = 0     ;
    m_iPreManStep = 0     ;
    m_iManNo      = mcNoneCycle ;

    m_bManSetting = false ;

    m_tmCycle.Clear() ;
}
void CManualMan::Close()
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

    if(_iNo  == mcAllHome  && (IO_GetX(xPRB_PkgIn1) || (IO_GetX(xPSB_PkgOut2 ) && OM.EqpOptn.bExistUnLoader) )) {FM_MsgOk("ERROR","Check Rail In or Out Sensor!"); return false ;}
    if(_iNo  == mcAllHome  &&(!IO_GetX(xETC_MainAir) || !IO_GetY(yETC_MainSol))) {FM_MsgOk("ERROR","Check Main Air!"); return false ;}
    if(_iNo  == mcLDR_Home && (IO_GetX(xPRB_PkgIn1) /*|| IO_GetX(xPRB_2Pkg )*/)) {FM_MsgOk("ERROR","Check Rail In Sensor!"       ); return false ;}

    EM_SetDisp(true);

    double dTrimPitch ;
    dTrimPitch = (OM.DevInfo.dColPitch + OM.DevOptn.dTrimPtchOfs) ;

    int c,r ;
    int iMaxRow,iMaxCol;


    m_bManSetting = true ; //업데이트 함수에서 다른쓰레드로 들어와서 갱신하기에 플레그 걸어 막아둠.
    m_iManNo = _iNo ;

    bool bRet = true ;

    /********************/
    /* 프리프로세서     */
    /********************/
         if (m_iManNo == mcAllHome            ){     if(!IO_GetX(xETC_MainPower  )) {FM_MsgOk("ERR","Power On Plz"); bRet = false ;}
                                                else if(!IO_GetX(xETC_MainAir  ) || !IO_GetY(yETC_MainSol)) {FM_MsgOk("ERR","Air On Plz"); bRet = false ;}
                                                //여기서 굳이 하는건 아닌거 같네...시퀀스에서 에러 띄우는게... ?
                                               }
    else if (m_iManNo == mcLDR_Home           ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcLDR_Supply         ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;} }
    else if (m_iManNo == mcLDR_Pick           ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(!DM.ARAY[riLDR].CheckAllStat(csNone)       ){FM_MsgOk("ERR","Remove Mgz"          ); bRet = false ;} }
    else if (m_iManNo == mcLDR_Place          ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
                                                if(FM_MsgYesNo("Config" , "Remove Magazine?") != mrYes){
                                                    LDR._iMgzCnt -= 1;
                                                }
                                               }

    /*
    if(IO_GetX(xLDR_MgzDetect1) || IO_GetX(xLDR_MgzDetect2)) if(FM_MsgYesNo("Confirm","Mgz Detected, Are you Sure ?") != mrYes ) return ;
        bRet = LDR.CheckSafe((EN_ACTR_ID)iSel , ccFwd) ;

    */
    //else if (m_iManNo == mcLDR_Clamp          ){if(!OM.EqpOptn.bExistLoader                   ){FM_MsgOk("ERR","Loader doesn't Exist"); bRet = false ;}
    //                                            if(!LDR.CheckSafe(aiLDR_Cmp , ccFwd)) bRet = false ;
    //                                           }
    else if (m_iManNo == mcPRB_Home           ){}
    else if (m_iManNo == mcPRB_In             ){}
    else if (m_iManNo == mcPRB_Wait           ){}
    else if (m_iManNo == mcPRB_Out            ){}

    else if (m_iManNo == mcWRK_Home           ){}
    else if (m_iManNo == mcWRK_Work           ){if(!DM.ARAY[riWRK].GetCntStat(csUnkwn)) DM.ARAY[riWRK].SetStat(csUnkwn);}
    else if (m_iManNo == mcWRK_Out            ){}
    else if (m_iManNo == mcWRK_Visn           ){}
    else if (m_iManNo == mcWRK_ToolChng       ){}

    else if (m_iManNo == mcPSB_Home           ){}
    else if (m_iManNo == mcPSB_In             ){}
    else if (m_iManNo == mcPSB_Move           ){}
    else if (m_iManNo == mcPSB_Work           ){}
    else if (m_iManNo == mcPSB_Out            ){}
    else if (m_iManNo == mcSTL_YPchUp1Pitch   ){if( AT_Complete(aiSTL_Top   , ccFwd        )){FM_MsgOk("ERR","STL_Top is not BWD"     );bRet = false ;} }
    else if (m_iManNo == mcSTL_YPchDn1Pitch   ){if( AT_Complete(aiSTL_Top   , ccFwd        )){FM_MsgOk("ERR","STL_Top is not BWD"     );bRet = false ;} }

    else if (m_iManNo == mcETC_AllClampOff    ){}

    else if (m_iManNo == mcETC_PrbStt         ){}
    else if (m_iManNo == mcETC_LDRULDPlace    ){}

    else if (m_iManNo == mcULD_Home           ){}
    else if (m_iManNo == mcULD_Supply         ){}
    else if (m_iManNo == mcULD_Pick           ){if(!DM.ARAY[riULD].CheckAllStat(csNone))                                                bRet = false ;}
    else if (m_iManNo == mcULD_Place          ){}
    else if (m_iManNo == mcETC_PSBPlace       ){if(!DM.ARAY[riPSB].GetCntStat(csNone) && !DM.ARAY[riULD].GetCntStat(csEmpty))          bRet = false ;
                                                if(FM_MsgYesNo("Check", "Real Strip Remove?")==mrYes) {m_iManStep=10; m_iPreManStep = 0;}
                                               }

    //else if (m_iManNo == mcSTL_Pick           ){if( DM.ARAY[riSTL].GetCntStat  (csNone))                                                bRet = false ;}
    //else if (m_iManNo == mcSTL_Place          ){if( DM.ARAY[riSTL].GetCntStat  (csFail))                                                bRet = false ;}



    if(!bRet) Init();



    /********************/
    /* 처리..           */
    /********************/
    if      (m_iManNo == mcAllHome            ) { //IO_SetY(yETC_MainSol ,true); 이러면 빠져나가는 상황이 있어서 그냥 윗단에서
                                                  //IO_SetY(yETC_MainSol2,true); 에러를 잡는다.
                                                   if(OM.EqpOptn.bExistLoader) LDR.InitHomeStep ();
                                                                               PRB.InitHomeStep ();
                                                                               WRK.InitHomeStep ();
                                                                               PSB.InitHomeStep ();
                                                                               ULD.InitHomeStep (); }

    else if (m_iManNo == mcLDR_Home           ) { LDR.InitHomeStep (); }
    else if (m_iManNo == mcLDR_Supply         ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Pick           ) { LDR.InitCycleStep(); }
    else if (m_iManNo == mcLDR_Place          ) { DM.ARAY[riLDR].SetStat(csEmpty); LDR.InitCycleStep(); }

    else if (m_iManNo == mcPRB_Home           ) { PRB.InitHomeStep (); }
    else if (m_iManNo == mcPRB_In             ) { PRB.InitCycleStep(); }
    else if (m_iManNo == mcPRB_Wait           ) { PRB.InitCycleStep(); }
    else if (m_iManNo == mcPRB_Out            ) { PRB.InitCycleStep(); }

    else if (m_iManNo == mcWRK_Home           ) { WRK.InitHomeStep (); }
    else if (m_iManNo == mcWRK_Work           ) { WRK.InitCycleStep(); }
    else if (m_iManNo == mcWRK_Out            ) { WRK.InitCycleStep(); }
    else if (m_iManNo == mcWRK_Visn           ) { WRK.InitCycleStep(); }
    else if (m_iManNo == mcWRK_ToolChng       ) { WRK.InitCycleStep(); }

    else if (m_iManNo == mcPSB_Home           ) { PSB.InitHomeStep (); }
    else if (m_iManNo == mcPSB_In             ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_Move           ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_Work           ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcPSB_Out            ) { PSB.InitCycleStep(); }
    else if (m_iManNo == mcSTL_YPchUp1Pitch   ) {
        iTrmManRow++ ;
        iTypeMan = 0;
        if(iTrmManRow < 0 ) iTrmManRow = 0 ;
        if(iTrmManRow >= OM.DevInfo.iRowCnt - 1) iTrmManRow = OM.DevInfo.iRowCnt - 1;
        MT_GoAbsMan(miSTL_YTop , GetMotrPosTable(iTrmManRow));
    }
    else if (m_iManNo == mcSTL_YPchDn1Pitch   ) {
        iTrmManRow-- ;
        iTypeMan = 0;
        if(iTrmManRow < 0 ) iTrmManRow = 0 ;
        if(iTrmManRow >= OM.DevInfo.iRowCnt - 1) iTrmManRow = OM.DevInfo.iRowCnt - 1;
        MT_GoAbsMan(miSTL_YTop , GetMotrPosTable(iTrmManRow)) ;
    }

    else if (m_iManNo == mcULD_Home          ) { ULD.InitHomeStep (); }
    else if (m_iManNo == mcULD_Supply        ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_Pick          ) { ULD.InitCycleStep(); }
    else if (m_iManNo == mcULD_Place         ) { ULD.InitCycleStep(); }

    else if (m_iManNo == mcETC_PrbStt         ) {
        AnsiString sMsg;
        sMsg = "START#";
        RS232C_Pb1ch->SendData(sMsg.Length(),sMsg.c_str());
        RS232C_Pb2ch->SendData(sMsg.Length(),sMsg.c_str());
    }
    else if (m_iManNo == mcETC_LDRULDPlace    ) { if(!OM.EqpOptn.bExistLoader || !OM.EqpOptn.bExistUnLoader) {m_bManSetting = false ; return false ;  } //m_bManSetting 중요함 리턴전에 꼭 펄스 시켜야함. 쓰레드가 달라서. ::Update에서 m_iManNo=0이 되므로 주의.
                                                  LDR.InitCycleStep();
                                                  ULD.InitCycleStep();
                                                }
    else if (m_iManNo == mcETC_PSBPlace       ) { CyclePlaceStrp();}

    else                                        { m_iManNo = mcNoneCycle ; }

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

    if(!OM.CmnOptn.bIgnrHeatAlm ) SEQ.InspectTemp    ();
    if(!OM.CmnOptn.bIgnrDoor    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcLDR_Home &&
       m_iManNo != mcPRB_Home &&
       m_iManNo != mcWRK_Home &&
       m_iManNo != mcPSB_Home &&
       m_iManNo != mcULD_Home &&
       m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool r1,r2,r3,r4,r5,r6,r7 ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }
    //Cycle Step.
    if      (m_iManNo == mcAllHome         )  {if(OM.EqpOptn.bExistLoader)   r1 = LDR.CycleHome();
                                               else                          r1 = true ;
                                                                             r2 = PRB.CycleHome();
                                                                             r3 = WRK.CycleHome();
                                               if(r3)                        r4 = PSB.CycleHome();
                                               if(OM.EqpOptn.bExistUnLoader) r5 = ULD.CycleHome();
                                               else                          r5 = true ;
                                                   if(r1&&r2&&r3&&r4&&r5) {
                                                       m_iManNo = mcNoneCycle;
                                                       FM_MsgOk("Confirm","All Home Finished!");
                                                   }
                                               }
    else if (m_iManNo == mcETC_LDRULDPlace )  { r1 = LDR.CyclePlace();
                                                r5 = ULD.CyclePlace();
                                                if(r1&&r5) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcLDR_Home     && OM.EqpOptn.bExistLoader ) { if(LDR.CycleHome     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Supply   && OM.EqpOptn.bExistLoader ) { if(LDR.CycleSupply   ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Pick     && OM.EqpOptn.bExistLoader ) { if(LDR.CyclePick     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcLDR_Place    && OM.EqpOptn.bExistLoader ) { if(LDR.CyclePlace    ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcPRB_Home         ) { if(PRB.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPRB_In           ) { if(PRB.CycleIn         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPRB_Wait         ) { if(PRB.CycleWait       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPRB_Out          ) { if(PRB.CycleOut        ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcWRK_Home         ) { if(WRK.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_Work         ) { if(WRK.CycleWork       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_Out          ) { if(WRK.CycleOut        ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_Visn         ) { if(WRK.CycleVisn       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_ToolChng     ) { if(WRK.CycleToolChange ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcPSB_Home         ) { if(PSB.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_In           ) { if(PSB.CycleIn         ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Move         ) { if(PSB.CycleMove       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Work         ) { if(PSB.CycleWork       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSB_Out          ) { if(PSB.CycleOut        ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcULD_Home         ) { if(ULD.CycleHome       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Supply       ) { if(ULD.CycleSupply     ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Pick         ) { if(ULD.CyclePick       ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcULD_Place        ) { if(ULD.CyclePlace      ()) m_iManNo = mcNoneCycle; }

    else if (m_iManNo == mcETC_LDRULDPlace  ) { if(LDR.CyclePlace      ()) { if(ULD.CyclePlace     ()) m_iManNo = mcNoneCycle; } }
    else if (m_iManNo == mcETC_PSBPlace     ) { if(CyclePlaceStrp      ()) m_iManNo = mcNoneCycle; }


//    else if (m_iManNo == mcETC_RemoveStrip    ) { if(CycleStipRemove   () ){Trace("Manual Man","mcETC_RemoveStrip    End"); m_iManNo = mcNoneCycle; } }
//    else if (m_iManNo == mcETC_PreRemoveStrip ) { if(CyclePreStipRemove() ){Trace("Manual Man","mcETC_PreRemoveStrip End"); m_iManNo = mcNoneCycle; } }
//    else if (m_iManNo == mcETC_WrkRemoveStrip ) { if(CycleWrkStipRemove() ){Trace("Manual Man","mcETC_WrkRemoveStrip End"); m_iManNo = mcNoneCycle; } }
//    else if (m_iManNo == mcETC_PstRemoveStrip ) { if(CyclePstStipRemove() ){Trace("Manual Man","mcETC_PstRemoveStrip End"); m_iManNo = mcNoneCycle; } }

    else                                     {                          m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;
}

//---------------------------------------------------------------------------
double CManualMan::GetMotrPosTable(int _iRow)
{
    int    r;
    double dStartPos ;
    double dPos      ;

    r = _iRow ;//OM.DevOptn.iRowInspCnt ? _iRow/OM.DevOptn.iRowInspCnt * OM.DevOptn.iRowInspCnt : _iRow ;

    dStartPos = PM.GetValue(miSTL_YTop , pvSTL_YTWorkStart) ;

    //if(r >  OM.DevInfo.iRowGrCnt) dPos = dStartPos + (OM.DevInfo.dRowPitch * r);
    //else                          dPos = dStartPos + (OM.DevInfo.dRowPitch * r);
    dPos = dStartPos + (OM.DevInfo.dRowPitch * r) + (OM.DevOptn.dTrimPtchOfs * r);

    int iRowGrCnt = r / OM.DevInfo.iRowGrCnt ;
    int iRowSubGrCnt = r / OM.DevInfo.iRowSubGrCnt ;

    if(OM.DevInfo.iRowGrCnt > 1)
    {
        dPos += (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt;
    }

    if(OM.DevInfo.iRowSubGrCnt > 1)
    {
        dPos += (OM.DevInfo.dRowSubGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt ;
    }
    //if( OM.DevInfo.iRowGrCnt > 1 || OM.DevInfo.iRowSubGrCnt > 1) {
    //    //iRowGrCnt는 그룹의 갯수. OM.DevInfo.iRowGrCnt이거는 한그룹당 LED 갯수.
    //
    //    if(iRowGrCnt > 0 && iRowSubGrCnt > 1)  dPos += (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt -
    //                                                   (OM.DevInfo.dRowSubGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt ;
    //    if(iRowGrCnt > 0 && iRowSubGrCnt <= 1) dPos += (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt ;
    //    if(iRowSubGrCnt > 1)                   dPos += (OM.DevInfo.dRowSubGrGap - OM.DevInfo.dRowPitch) * iRowSubGrCnt ;
    //}

    return dPos;
}

bool CManualMan::CyclePlaceStrp()
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

        case  10: ULD.InitCycleStep();
                  PSB.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  11: if(!ULD.CycleWork())return false;
                  if(!PSB.CycleIn()) return false;
                  PSB.InitCycleStep();

                  m_iManStep++;
                  return false;

        case  12: if(!PSB.CycleOut())return false;
                  m_iManStep = 0;
                  return true;

        }
}
