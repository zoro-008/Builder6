//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "ManualMan.h"
#include "SMInterfaceUnit.h"
#include "SLogUnit.h"
#include "OptionMan.h"
#include "Sequence.h"
#include "PstnMan.h"
#include "VisnComUnit.h"
//---------------------------------------------------------------------------
#include "FrmDeviceSet.h"
#include "FrmMain.h"
#include "FrmManual.h"
//---------------------------------------------------------------------------
//Part Refernce
//---------------------------------------------------------------------------
#include "PreBuff.h"
#include "PreIdx.h"
#include "Work.h"
#include "PstIdx.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
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
void __fastcall CManualMan::Init(void)
{
    //Init. Var.
    m_iManStep    = 0     ;
    m_iPreManStep = 0     ;
    m_iManNo      = mcNoneCycle ;

//    m_bManSetting = false ;

    m_tmCycle.Clear() ;
}

//---------------------------------------------------------------------------
bool __fastcall CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo     <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo     >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if(SEQ._bRun                     ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}

//    if(IO_GetX(xETC_DoorBack) || IO_GetX(xETC_DoorFront) || IO_GetX(xETC_DoorRear)) {FM_MsgOk("Warring","Door Open" ); return false ;}
//    if(IO_GetX(xETC_AreaFront) ) {FM_MsgOk("Warring","Area Sensor Detected" ); return false ;}

    //Check Alarm
    if ( EM_IsErr()      ) { Init(); return false  ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.

    if(_iNo  == mcAllHome  ) {
        if((!IO_GetX(xETC_MainSol) || !IO_GetY(yETC_MainSol))) {FM_MsgOk("ERROR","Check Main Air!"); return false ;}
        for(int i=0; i<MAX_MOTR; i++) MT_SetHomeEnd(i,false);
    }

    EM_SetDisp(true);

    m_bManSetting = true ;
    m_iManNo = _iNo ;

    bool bRet = true ;

//****************************************************************************************************//
// 프리프로세서                                                                                       //
//****************************************************************************************************//
    if (m_iManNo == mcAllHome            ){
              if(!IO_GetX(xETC_PowerOn )) {FM_MsgOk("ERR","Power On Plz"); bRet = false ;}
         else if(!IO_GetY(yETC_MainSol )) {FM_MsgOk("ERR","Air On Plz"  ); bRet = false ;}
         else if( IO_GetX(xPRB_Strip2   ) || IO_GetX(xPRI_Strip3      )) {FM_MsgOk("ERR","Flipper In Out Sensor Check Plz"  ); bRet = false ;}
    }
    else if (m_iManNo == mcPRB_Home                ){
        Trace("Manual","WRK_Home Clicked");
        if(IO_GetX(xETC_DoorBack) || IO_GetX(xETC_DoorFront) || IO_GetX(xETC_DoorRear)) {FM_MsgOk("Warring","Door Open" ); bRet = false ;}
        if(IO_GetX(xETC_AreaFront) ) {FM_MsgOk("Warring","Area Sensor Detected" ); bRet = false ;}
    }
    else if (m_iManNo == mcPRB_FlpNormal           ){  }
    else if (m_iManNo == mcPRB_FlpInverse          ){  }
    else if (m_iManNo == mcPRB_IdxUp               ){  }
    else if (m_iManNo == mcPRB_IdxDn               ){  }
    else if (m_iManNo == mcPRB_IdxWait             ){  }
    else if (m_iManNo == mcPRB_IdxStrip            ){  }
    else if (m_iManNo == mcPRB_IdxOut              ){  }

    else if (m_iManNo == mcPRI_Home                ){
        Trace("Manual","WRK_Home Clicked");
        if(IO_GetX(xETC_DoorBack) || IO_GetX(xETC_DoorFront) || IO_GetX(xETC_DoorRear)) {FM_MsgOk("Warring","Door Open" ); bRet = false ;}
        if(IO_GetX(xETC_AreaFront) ) {FM_MsgOk("Warring","Area Sensor Detected" ); bRet = false ;}
    }
    else if (m_iManNo == mcPRI_XIdxWait            ){  }
    else if (m_iManNo == mcPRI_XIdxWork            ){  }
    else if (m_iManNo == mcPRI_XIdxOut             ){  }
    else if (m_iManNo == mcPRI_IdxUp               ){  }
    else if (m_iManNo == mcPRI_IdxDn               ){  }
    else if (m_iManNo == mcPRI_IdxStripReMv        ){  m_iManStep = 10 ; m_iPreManStep = 0; }

    else if (m_iManNo == mcWRK_Home                ){
        Trace("Manual","WRK_Home Clicked");
        if(IO_GetX(xETC_DoorBack) || IO_GetX(xETC_DoorFront) || IO_GetX(xETC_DoorRear)) {FM_MsgOk("Warring","Door Open" ); bRet = false ;}
        if(IO_GetX(xETC_AreaFront) ) {FM_MsgOk("Warring","Area Sensor Detected" ); bRet = false ;}
    }
    else if (m_iManNo == mcWRK_XSrtBin1            ){  }
    else if (m_iManNo == mcWRK_XSrtBin2            ){  }
    else if (m_iManNo == mcWRK_XSrtBin3            ){  }
    else if (m_iManNo == mcWRK_XSrtBin4            ){  }
    else if (m_iManNo == mcWRK_XSrtBin5            ){  }
    else if (m_iManNo == mcWRK_PunchUp             ){
        Trace("Manual","WRK_PunchUp Clicked");
        if(IO_GetX(xETC_DoorBack) || IO_GetX(xETC_DoorFront) || IO_GetX(xETC_DoorRear)) {FM_MsgOk("Warring","Door Open" ); bRet = false ;}
        if(IO_GetX(xETC_AreaFront) ) {FM_MsgOk("Warring","Area Sensor Detected" ); bRet = false ;}

        if (FM_MsgYesNo("Confirm" , "펀치를 Wait 포지션으로 이동하시겠습니까?") == mrNo) bRet = false ;
        else Trace("Manual","WRK_PunchUp");
    }

    else if (m_iManNo == mcPSI_Home                ){
        Trace("Manual","WRK_Home Clicked");
        if(IO_GetX(xETC_DoorBack) || IO_GetX(xETC_DoorFront) || IO_GetX(xETC_DoorRear)) {FM_MsgOk("Warring","Door Open" ); bRet = false ;}
        if(IO_GetX(xETC_AreaFront) ) {FM_MsgOk("Warring","Area Sensor Detected" ); bRet = false ;}
    }
    else if (m_iManNo == mcPSI_XIdxWait            ){  }
    else if (m_iManNo == mcPSI_XIdxPull            ){  }
    else if (m_iManNo == mcPSI_XIdxBack            ){  }
    else if (m_iManNo == mcPSI_XIdxOut             ){  }
    else if (m_iManNo == mcPSI_IdxUp               ){  }
    else if (m_iManNo == mcPSI_IdxDn               ){  }
    if(!bRet) Init();

//****************************************************************************************************//
// 처리..                                                                                             //
//****************************************************************************************************//

    if      (m_iManNo == mcAllHome            ) { //IO_SetY(yETC_MainSol ,true); 이러면 빠져나가는 상황이 있어서 그냥 윗단에서
                                                  //IO_SetY(yETC_MainSol2,true); 에러를 잡는다.
                                                  IO_SetY(yETC_InitLp, !IO_GetY(yETC_InitLp));
//                                                  IO_SetY(yETC_RInitLp, !IO_GetY(yETC_RInitLp));

                                                  PRB.InitHomeStep();
                                                  PRI.InitHomeStep();
                                                  WRK.InitHomeStep();
                                                  PSI.InitHomeStep();

                                                  }


    else if (m_iManNo == mcPRB_Home                ){ PRB.InitHomeStep();}
    else if (m_iManNo == mcPRB_FlpNormal           ){ PRB.MoveMotr(miPRB_TFlp , piPRB_TFlpNormal ); }
    else if (m_iManNo == mcPRB_FlpInverse          ){ PRB.MoveMotr(miPRB_TFlp , piPRB_TFlpInverse); }
    else if (m_iManNo == mcPRB_IdxUp               ){ PRB.MoveActr(aiPRB_Idx  , ccBwd            ); }
    else if (m_iManNo == mcPRB_IdxDn               ){ PRB.MoveActr(aiPRB_Idx  , ccFwd            ); }
    else if (m_iManNo == mcPRB_IdxWait             ){ PRB.MoveMotr(miPRB_XIdx , piPRB_XIdxWait   ); }
    else if (m_iManNo == mcPRB_IdxStrip            ){ PRB.MoveMotr(miPRB_XIdx , piPRB_XIdxTurn   ); }
    else if (m_iManNo == mcPRB_IdxOut              ){ PRB.MoveMotr(miPRB_XIdx , piPRB_XIdxOut    ); }

    else if (m_iManNo == mcPRI_Home                ){ PRI.InitHomeStep();}
    else if (m_iManNo == mcPRI_XIdxWait            ){ PRI.MoveMotr(miPRI_XIdx , piPRI_XIdxWait   ); }
    else if (m_iManNo == mcPRI_XIdxWork            ){ PRI.MoveMotr(miPRI_XIdx , piPRI_XIdxWorkStt); }
    else if (m_iManNo == mcPRI_XIdxOut             ){ PRI.MoveMotr(miPRI_XIdx , piPRI_XIdxOut    ); }
    else if (m_iManNo == mcPRI_IdxUp               ){ PRI.MoveActr(aiPRI_Idx  , ccBwd            ); }
    else if (m_iManNo == mcPRI_IdxDn               ){ PRI.MoveActr(aiPRI_Idx  , ccFwd            ); }
    else if (m_iManNo == mcPRI_IdxStripReMv        ){}

    else if (m_iManNo == mcWRK_Home                ){ WRK.InitHomeStep(); }
    else if (m_iManNo == mcWRK_XSrtBin1            ){ WRK.MoveMotr(miWRK_XSrt , piWRK_XSrtBin1   ); }
    else if (m_iManNo == mcWRK_XSrtBin2            ){ WRK.MoveMotr(miWRK_XSrt , piWRK_XSrtBin2   ); }
    else if (m_iManNo == mcWRK_XSrtBin3            ){ WRK.MoveMotr(miWRK_XSrt , piWRK_XSrtBin3   ); }
    else if (m_iManNo == mcWRK_XSrtBin4            ){ WRK.MoveMotr(miWRK_XSrt , piWRK_XSrtBin4   ); }
    else if (m_iManNo == mcWRK_XSrtBin5            ){ WRK.MoveMotr(miWRK_XSrt , piWRK_XSrtBin5   ); }
    else if (m_iManNo == mcWRK_PunchUp             ){ WRK.MoveMotr(miWRK_ZPch , piWRK_ZPchWait   ); }

    else if (m_iManNo == mcPSI_Home                ){ PSI.MoveMotr(miPSI_XIdx , piPSI_XIdxWait   ); }
    else if (m_iManNo == mcPSI_XIdxWait            ){ PSI.MoveMotr(miPSI_XIdx , piPSI_XIdxWait   ); }
    else if (m_iManNo == mcPSI_XIdxPull            ){ PSI.MoveMotr(miPSI_XIdx , piPSI_XIdxPull   ); }
    else if (m_iManNo == mcPSI_XIdxBack            ){ PSI.MoveMotr(miPSI_XIdx , piPSI_XIdxBack   ); }
    else if (m_iManNo == mcPSI_XIdxOut             ){ PSI.MoveMotr(miPSI_XIdx , piPSI_XIdxOut    ); }
    else if (m_iManNo == mcPSI_IdxUp               ){ PSI.MoveActr(aiPSI_Idx  , ccBwd            ); }
    else if (m_iManNo == mcPSI_IdxDn               ){ PSI.MoveActr(aiPSI_Idx  , ccFwd            ); }
    else                                            { m_iManNo = mcNoneCycle ; }

    m_bManSetting = false ;
    return true ;
}

EN_MANUAL_CYCLE __fastcall CManualMan::GetManNo()
{
    return m_iManNo ;
}

//---------------------------------------------------------------------------
void __fastcall CManualMan::Update(void)
{
    if( m_iManNo == mcNoneCycle ) return  ;
    if( m_bManSetting           ) return  ;

    if(!OM.CmnOptn.bDoorSkip    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcPRB_Home     &&
       m_iManNo != mcWRK_Home     &&
       m_iManNo != mcAllHome    ) SEQ.InspectHomeEnd ();

    bool r1,r2,r3,r4,r5 ;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }

    //Cycle Step.
    if (m_iManNo == mcAllHome )
    {
        if(IO_GetX(xETC_AreaFront) ) {
            EM_SetErr(eiETC_AreaSsrDectected);
            MT_SetServoAll(false);
            return ;
        }

        r1 = PRB.CycleHome();
        r2 = PRI.CycleHome();
        r3 = WRK.CycleHome();
        r4 = PSI.CycleHome();

        if(r1&&r2&&r3&&r4){
            m_iManNo = mcNoneCycle;
            FM_MsgOk("Confirm","All Home Finished!");
        }
    }

    else if (m_iManNo == mcPRB_Home         ) { if(PRB.CycleHome  ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPRI_Home         ) { if(PRI.CycleHome  ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcWRK_Home         ) { if(WRK.CycleHome  ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPSI_Home         ) { if(PSI.CycleHome  ()) m_iManNo = mcNoneCycle; }
    else if (m_iManNo == mcPRI_IdxStripReMv ) { if(CycleStipRemove()) m_iManNo = mcNoneCycle; }
    else                              {                     m_iManNo = mcNoneCycle; }

    //Ok.
    return ;
}


bool CManualMan::CycleStipRemove()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 20000 )) {
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

        default : if(m_iManStep) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s m_iManStep=%02d" , __FUNC__ , m_iManStep );
                      Trace("Manual Man", sTemp.c_str());
                  }
                  m_iManStep = 0 ;
                  return true ;

        case  10: AT_MoveCyl(aiPRI_Idx , ccBwd);

                  m_iManStep++;
                  return false ;

        case  11: if(!AT_MoveCyl(aiPRI_Idx , ccBwd))return false;
                  MT_GoAbsMan(miPRI_XIdx , PM.GetValue(miPRI_XIdx , pvPRI_XIdxOut));

                  m_iManStep = 0 ;
                  return true;

    }
}
