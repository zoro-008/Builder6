//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "ManualMan.h"
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
#include "Sequence.h"
#include "Rs232Keyence.h"

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
CManualMan::CManualMan(void)
{

}

//---------------------------------------------------------------------------
CManualMan::~CManualMan(void)
{

}

//---------------------------------------------------------------------------
void CManualMan::Init(void)
{
    //Init. Var.
    m_iManStep    = 0     ;
    m_iPreManStep = 0     ;
    m_iManNo      = mcNoneCycle ;

    m_bManSetting  = false ;

    m_tmCycle.Clear() ;
    m_tmDelay.Clear() ;
}

//---------------------------------------------------------------------------
void CManualMan::Close(void)
{
}

//---------------------------------------------------------------------------
bool CManualMan::SetManCycle(EN_MANUAL_CYCLE _iNo)
{
    if( _iNo     <  0                ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( _iNo     >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","Wrong Manual No"      ); return false ;}
    if( m_iManNo != mcNoneCycle      ) {FM_MsgOk("ERROR","Doing Manual Cycle"   ); return false ;}
    if(SEQ._bRun                     ) {FM_MsgOk("ERROR","Autorunning"          ); return false ;}

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return false ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.          //test

    if(!IO_GetX(xETC_MainPower) ) {FM_MsgOk("ERR","Power On Plz");      return false ;} //test
    if(!IO_GetX(xETC_MainAir  ) ) {FM_MsgOk("ERROR","Check Main Air!"); return false ;} //test

    EM_SetDisp(true);

    m_bStdCalPick = false;

    m_bManSetting = true ; //SetManCycle함수는 화면 쓰레드. 업데이트 함수에서 다른쓰레드로 들어와서 갱신하기에 플레그 걸어 막아둠.
    m_iManNo = _iNo ;

    bool bRet = true ;

    /********************/
    /* 프리프로세서     */
    /********************/
         if(m_iManNo == mcNoneCycle             ){bRet = false ;}
    else if(m_iManNo == mcAllHome               ){}
    else if(m_iManNo == mcLDR_FHome             ){if(!MT_GetHomeEnd(miPRI_XIndxF)) {FM_MsgOk("ERROR","Front Pre Index 홈을 먼저 잡아주세요"); bRet = false;}
                                                  if(MT_GetCmdPos(miPRI_XIndxF)>PM.GetValue(miPRI_XIndxF,pvPRI_XIndxFWait)) {FM_MsgOk("ERROR","Front Pre Index Postion이 Wait포지션 보다 높습니다.");bRet = false; }}
    else if(m_iManNo == mcLDR_RHome             ){if(!MT_GetHomeEnd(miPRI_XIndxR)) {FM_MsgOk("ERROR","Rear Pre Index 홈을 먼저 잡아주세요"); bRet = false;}
                                                  if(MT_GetCmdPos(miPRI_XIndxR)>PM.GetValue(miPRI_XIndxR,pvPRI_XIndxRWait)) {FM_MsgOk("ERROR","Rear  Pre Index Postion이 Wait포지션 보다 높습니다.");bRet = false; }}
    else if(m_iManNo == mcPRI_FHome             ){}
    else if(m_iManNo == mcPRI_RHome             ){}
    else if(m_iManNo == mcIDX_FHome             ){if(!MT_GetHomeEnd(miPCK_ZPickr)) {FM_MsgOk("ERROR","Picker 홈을 먼저 잡아주세요"); bRet = false;}
                                                  if(!MT_GetCmdPos(miPCK_ZPickr)>PM.GetValue(miPCK_ZPickr,pvPCK_ZPickrWait)) {FM_MsgOk("ERROR","Picker Z축이 Wait포지션 보다 낮습니다."); bRet = false;}}
    else if(m_iManNo == mcIDX_RHome             ){if(!MT_GetHomeEnd(miPCK_ZPickr)) {FM_MsgOk("ERROR","Picker 홈을 먼저 잡아주세요"); bRet = false;}
                                                  if(!MT_GetHomeEnd(miDSP_ZDispr)) {FM_MsgOk("ERROR","Dispensor 홈을 먼저 잡아주세요"); bRet = false;}
                                                  if(!MT_GetCmdPos(miPCK_ZPickr)>PM.GetValue(miPCK_ZPickr,pvPCK_ZPickrWait)) {FM_MsgOk("ERROR","Picker Z축이 Wait포지션 보다 낮습니다."); bRet = false;}
                                                  if(!MT_GetCmdPos(miDSP_ZDispr)>PM.GetValue(miDSP_ZDispr,pvDSP_ZDisprWait)) {FM_MsgOk("ERROR","Dispensor Z축이 Wait포지션 보다 낮습니다."); bRet = false;}}
    else if(m_iManNo == mcULD_FHome             ){if(!MT_GetHomeEnd(miIDX_XIndxF)) {FM_MsgOk("ERROR","Front Index 홈을 먼저 잡아주세요"); bRet = false;}
                                                  if(MT_GetCmdPos(miIDX_XIndxF)>PM.GetValue(miIDX_XIndxF,pvIDX_XIndxFOut)) {FM_MsgOk("ERROR","Front Index Postion이 Out포지션 보다 높습니다."); bRet = false;}}
    else if(m_iManNo == mcULD_RHome             ){if(!MT_GetHomeEnd(miIDX_XIndxR)) {FM_MsgOk("ERROR","Rear  Index 홈을 먼저 잡아주세요"); bRet = false;}
                                                  if(MT_GetCmdPos(miIDX_XIndxR)>PM.GetValue(miIDX_XIndxR,pvIDX_XIndxROut)) {FM_MsgOk("ERROR","Rear  Index Postion이 Out포지션 보다 높습니다."); bRet = false;}}
    else if(m_iManNo == mcSTGHome               ){}
    else if(m_iManNo == mcPCKHome               ){}
    else if(m_iManNo == mcDSPHome               ){} //다이모드는 After 검사가 안되서 할 수 없다.
    else if(m_iManNo == mcVisnCal               ){if(OM.DevInfo.iWorkMode!=wmFos){FM_MsgOk("ERROR","FOS 레서피가 아니면 수행 할 수 없습니다."); bRet = false;}}
    else if(m_iManNo == mcLdrUldMoveWait        ){if(IO_GetX(xPRI_FDetect1) || IO_GetX(xPRI_RDetect1)){FM_MsgOk("ERROR","프리인덱스 진입 센서가 감지중 입니다."   ); bRet = false;}}
    else if(m_iManNo == mcStgFlatness           ){if(IO_GetX(xULD_FDetect5) || IO_GetX(xULD_RDetect5)){FM_MsgOk("ERROR","포스트인덱스 배출 센서가 감지중 입니다." ); bRet = false;}}

    else if(m_iManNo == mcLDR_FCycleWork        ){}
    else if(m_iManNo == mcLDR_FWorkSttBtm       ){if(!LDR_F.CheckSafe(miLDR_ZElevF,piLDR_ZElevFWorkStartBtm))bRet=false;}
    else if(m_iManNo == mcLDR_FWorkSttTop       ){if(!LDR_F.CheckSafe(miLDR_ZElevF,piLDR_ZElevFWorkStartTop))bRet=false;}
    else if(m_iManNo == mcLDR_FPitchUp          ){if(!LDR_F.CheckSafe(miLDR_ZElevF,piLDR_ZElevFWorkStartBtm))bRet=false;}
    else if(m_iManNo == mcLDR_FPitchDn          ){if(!LDR_F.CheckSafe(miLDR_ZElevF,piLDR_ZElevFWorkStartTop))bRet=false;}

    else if(m_iManNo == mcLDR_RCycleWork        ){}
    else if(m_iManNo == mcLDR_RWorkSttBtm       ){if(!LDR_R.CheckSafe(miLDR_ZElevR,piLDR_ZElevRWorkStartBtm))bRet=false;}
    else if(m_iManNo == mcLDR_RWorkSttTop       ){if(!LDR_R.CheckSafe(miLDR_ZElevR,piLDR_ZElevRWorkStartTop))bRet=false;}
    else if(m_iManNo == mcLDR_RPitchUp          ){if(!LDR_R.CheckSafe(miLDR_ZElevR,piLDR_ZElevRWorkStartBtm))bRet=false;}
    else if(m_iManNo == mcLDR_RPitchDn          ){if(!LDR_R.CheckSafe(miLDR_ZElevR,piLDR_ZElevRWorkStartTop))bRet=false;}

    else if(m_iManNo == mcPRI_FCycleWork        ){}
    else if(m_iManNo == mcPRI_FIndxUpDn         ){if(!PRI_F.CheckSafe(aiPRI_FIndxUpDn , !AT_GetCmd(aiPRI_FIndxUpDn)))bRet=false;}

    else if(m_iManNo == mcPRI_RCycleWork        ){}
    else if(m_iManNo == mcPRI_RIndxUpDn         ){if(!PRI_R.CheckSafe(aiPRI_RIndxUpDn , !AT_GetCmd(aiPRI_RIndxUpDn)))bRet=false;}

    else if(m_iManNo == mcIDX_FCycleGet         ){}
    else if(m_iManNo == mcIDX_FCycleWork        ){}
    else if(m_iManNo == mcIDX_FCycleOut         ){}
    else if(m_iManNo == mcIDX_FIndxUpDn         ){if(!IDX_F.CheckSafe(aiIDX_FIndxUpDn , !AT_GetCmd(aiIDX_FIndxUpDn)))bRet=false;}
    else if(m_iManNo == mcIDX_FPitchRt          ){if(!IDX_F.CheckSafe(miIDX_XIndxF    ,  piIDX_XIndxFWork          ))bRet=false;}
    else if(m_iManNo == mcIDX_FPitchLt          ){if(!IDX_F.CheckSafe(miIDX_XIndxF    ,  piIDX_XIndxFWork          ))bRet=false;}

    else if(m_iManNo == mcIDX_RCycleGet         ){}
    else if(m_iManNo == mcIDX_RCycleWork        ){}
    else if(m_iManNo == mcIDX_RCycleOut         ){}
    else if(m_iManNo == mcIDX_RIndxUpDn         ){if(!IDX_R.CheckSafe(aiIDX_RIndxUpDn , !AT_GetCmd(aiIDX_RIndxUpDn)))bRet=false;}
    else if(m_iManNo == mcIDX_RPitchRt          ){if(!IDX_R.CheckSafe(miIDX_XIndxR    ,  piIDX_XIndxRWork          ))bRet=false;}
    else if(m_iManNo == mcIDX_RPitchLt          ){if(!IDX_R.CheckSafe(miIDX_XIndxR    ,  piIDX_XIndxRWork          ))bRet=false;}

    else if(m_iManNo == mcULD_FCycleWork        ){}
    else if(m_iManNo == mcULD_FWorkSttTop       ){if(!ULD_F.CheckSafe(miULD_ZElevF,piULD_ZElevFWorkStartTop))bRet=false;}
    else if(m_iManNo == mcULD_FWorkSttBtm       ){if(!ULD_F.CheckSafe(miULD_ZElevF,piULD_ZElevFWorkStartBtm))bRet=false;}
    else if(m_iManNo == mcULD_FPitchUp          ){if(!ULD_F.CheckSafe(miULD_ZElevF,piULD_ZElevFWorkStartTop))bRet=false;}
    else if(m_iManNo == mcULD_FPitchDn          ){if(!ULD_F.CheckSafe(miULD_ZElevF,piULD_ZElevFWorkStartBtm))bRet=false;}

    else if(m_iManNo == mcULD_RCycleWork        ){}
    else if(m_iManNo == mcULD_RWorkSttTop       ){if(!ULD_R.CheckSafe(miULD_ZElevR,piULD_ZElevRWorkStartTop))bRet=false;}
    else if(m_iManNo == mcULD_RWorkSttBtm       ){if(!ULD_R.CheckSafe(miULD_ZElevR,piULD_ZElevRWorkStartBtm))bRet=false;}
    else if(m_iManNo == mcULD_RPitchUp          ){if(!ULD_R.CheckSafe(miULD_ZElevR,piULD_ZElevRWorkStartTop))bRet=false;}
    else if(m_iManNo == mcULD_RPitchDn          ){if(!ULD_R.CheckSafe(miULD_ZElevR,piULD_ZElevRWorkStartBtm))bRet=false;}

    else if(m_iManNo == mcSTGCycleUp            ){}
    else if(m_iManNo == mcSTGCycleDn            ){}

    //이밑에 놈들은 다들 변수가 너무 많아.. 일단 메뉴얼에서는 못쓰게 하고...
    //쓸놈만 꼼꼼히 따지자.
    else if(m_iManNo == mcPCKCyclePick          ){}
    else if(m_iManNo == mcPCKCycleAlign         ){}
    else if(m_iManNo == mcPCKCycleAlignPick     ){}
    else if(m_iManNo == mcPCKCycleVisn          ){}
    else if(m_iManNo == mcPCKCycleAttach        ){}
    else if(m_iManNo == mcPCKCyclePush          ){}

    //이놈들은 OK
    else if(m_iManNo == mcPCKCycleManVsDefectPos){}
    else if(m_iManNo == mcPCKCycleManVsAlignPos ){}
    else if(m_iManNo == mcPCKCycleManStdCal     ){}
    else if(m_iManNo == mcPCKCycleManAlgnPlce   ){}
    else if(m_iManNo == mcPCKCycleManAlignerCal ){}

    else if(m_iManNo == mcDSPCycleVisnBf        ){}
    else if(m_iManNo == mcDSPCycleDisp          ){}
    else if(m_iManNo == mcDSPCycleVisnAt        ){}
    else if(m_iManNo == mcDSPCycleClean         ){}

    //이놈들은 OK
    else if(m_iManNo == mcDSPCycleManNdlCheck   ){}
    else if(m_iManNo == mcDSPCycleManStdCal     ){}
    else if(m_iManNo == mcDSPCycleVisnPos       ){}
    else if(m_iManNo == mcDSPCycleNdlClean      ){}
    else if(m_iManNo == mcDSPCycleManSlnChange  ){}


    if(!bRet)Init();

    /********************/
    /* 처리..           */
    /********************/

         if(m_iManNo == mcNoneCycle             ){}
    else if(m_iManNo == mcAllHome               ){MT_SetServoAll(true);
                                                  LDR_F.InitHomeStep ();
                                                  LDR_R.InitHomeStep ();
                                                  PRI_F.InitHomeStep ();
                                                  PRI_R.InitHomeStep ();
                                                  IDX_F.InitHomeStep ();
                                                  IDX_R.InitHomeStep ();
                                                  PCK  .InitHomeStep ();
                                                  STG  .InitHomeStep ();
                                                  DSP  .InitHomeStep ();
                                                  ULD_F.InitHomeStep ();
                                                  ULD_R.InitHomeStep ();
                                                 }
    else if(m_iManNo == mcLDR_FHome             ){LDR_F.InitHomeStep ();}
    else if(m_iManNo == mcLDR_RHome             ){LDR_R.InitHomeStep ();}
    else if(m_iManNo == mcPRI_FHome             ){PRI_F.InitHomeStep ();}
    else if(m_iManNo == mcPRI_RHome             ){PRI_R.InitHomeStep ();}
    else if(m_iManNo == mcIDX_FHome             ){IDX_F.InitHomeStep ();}
    else if(m_iManNo == mcIDX_RHome             ){IDX_R.InitHomeStep ();}
    else if(m_iManNo == mcULD_FHome             ){ULD_F.InitHomeStep ();}
    else if(m_iManNo == mcULD_RHome             ){ULD_R.InitHomeStep ();}
    else if(m_iManNo == mcSTGHome               ){STG  .InitHomeStep ();}
    else if(m_iManNo == mcPCKHome               ){PCK  .InitHomeStep ();}
    else if(m_iManNo == mcDSPHome               ){DSP  .InitHomeStep ();}

    else if(m_iManNo == mcVisnCal               ){m_iPreManStep = 0 ; m_iManStep = 10; }
    else if(m_iManNo == mcLdrUldMoveWait        ){LDR_F.MoveMotr(miLDR_ZElevF, piLDR_ZElevFWait);
                                                  LDR_R.MoveMotr(miLDR_ZElevR, piLDR_ZElevRWait);
                                                  ULD_F.MoveMotr(miULD_ZElevF, piULD_ZElevFWait);
                                                  ULD_R.MoveMotr(miULD_ZElevR, piULD_ZElevRWait);}
    else if(m_iManNo == mcStgFlatness           ){m_iPreManStep = 0 ; m_iManStep = 10; }

    else if(m_iManNo == mcLDR_FCycleWork        ){LDR_F.InitCycleStep();}
    else if(m_iManNo == mcLDR_FWorkSttBtm       ){MT_GoAbsMan(miLDR_ZElevF , PM.GetValue(miLDR_ZElevF , pvLDR_ZElevFWorkStartBtm));}
    else if(m_iManNo == mcLDR_FWorkSttTop       ){MT_GoAbsMan(miLDR_ZElevF , PM.GetValue(miLDR_ZElevF , pvLDR_ZElevFWorkStartTop));}
    else if(m_iManNo == mcLDR_FPitchUp          ){MT_GoIncMan(miLDR_ZElevF ,-OM.DevInfo.dMgzSlotPitch);}
    else if(m_iManNo == mcLDR_FPitchDn          ){MT_GoIncMan(miLDR_ZElevF , OM.DevInfo.dMgzSlotPitch);}

    else if(m_iManNo == mcLDR_RCycleWork        ){LDR_R.InitCycleStep();}
    else if(m_iManNo == mcLDR_RWorkSttBtm       ){MT_GoAbsMan(miLDR_ZElevR , PM.GetValue(miLDR_ZElevR , pvLDR_ZElevRWorkStartBtm));}
    else if(m_iManNo == mcLDR_RWorkSttTop       ){MT_GoAbsMan(miLDR_ZElevR , PM.GetValue(miLDR_ZElevR , pvLDR_ZElevRWorkStartTop));}
    else if(m_iManNo == mcLDR_RPitchUp          ){MT_GoIncMan(miLDR_ZElevR ,-OM.DevInfo.dMgzSlotPitch);}
    else if(m_iManNo == mcLDR_RPitchDn          ){MT_GoIncMan(miLDR_ZElevR , OM.DevInfo.dMgzSlotPitch);}

    else if(m_iManNo == mcPRI_FCycleWork        ){PRI_F.InitCycleStep();}
    else if(m_iManNo == mcPRI_FIndxUpDn         ){AT_Complete(aiPRI_FIndxUpDn , !AT_GetCmd(aiPRI_FIndxUpDn));}

    else if(m_iManNo == mcPRI_RCycleWork        ){PRI_R.InitCycleStep();}
    else if(m_iManNo == mcPRI_RIndxUpDn         ){AT_Complete(aiPRI_RIndxUpDn , !AT_GetCmd(aiPRI_RIndxUpDn));}

    else if(m_iManNo == mcIDX_FCycleGet         ){IDX_F.InitCycleStep();}
    else if(m_iManNo == mcIDX_FCycleWork        ){IDX_F.InitCycleStep();}
    else if(m_iManNo == mcIDX_FCycleOut         ){IDX_F.InitCycleStep();}
    else if(m_iManNo == mcIDX_FIndxUpDn         ){AT_Complete(aiIDX_FIndxUpDn , !AT_GetCmd(aiIDX_FIndxUpDn));}
    else if(m_iManNo == mcIDX_FPitchRt          ){MT_GoIncMan(miIDX_XIndxF , OM.DevInfo.dBoatColPitch);}
    else if(m_iManNo == mcIDX_FPitchLt          ){MT_GoIncMan(miIDX_XIndxF ,-OM.DevInfo.dBoatColPitch);}

    else if(m_iManNo == mcIDX_RCycleGet         ){IDX_R.InitCycleStep();}
    else if(m_iManNo == mcIDX_RCycleWork        ){IDX_R.InitCycleStep();}
    else if(m_iManNo == mcIDX_RCycleOut         ){IDX_R.InitCycleStep();}
    else if(m_iManNo == mcIDX_RIndxUpDn         ){AT_Complete(aiIDX_RIndxUpDn , !AT_GetCmd(aiIDX_RIndxUpDn));}
    else if(m_iManNo == mcIDX_RPitchRt          ){MT_GoIncMan(miIDX_XIndxR , OM.DevInfo.dBoatColPitch);}
    else if(m_iManNo == mcIDX_RPitchLt          ){MT_GoIncMan(miIDX_XIndxR ,-OM.DevInfo.dBoatColPitch);}

    else if(m_iManNo == mcULD_FCycleWork        ){ULD_F.InitCycleStep();}
    else if(m_iManNo == mcULD_FWorkSttTop       ){MT_GoAbsMan(miULD_ZElevF , PM.GetValue(miULD_ZElevF , pvULD_ZElevFWorkStartBtm));}
    else if(m_iManNo == mcULD_FWorkSttBtm       ){MT_GoAbsMan(miULD_ZElevF , PM.GetValue(miULD_ZElevF , pvULD_ZElevFWorkStartTop));}
    else if(m_iManNo == mcULD_FPitchUp          ){MT_GoIncMan(miULD_ZElevF ,-OM.DevInfo.dMgzSlotPitch);}
    else if(m_iManNo == mcULD_FPitchDn          ){MT_GoIncMan(miULD_ZElevF , OM.DevInfo.dMgzSlotPitch);}

    else if(m_iManNo == mcULD_RCycleWork        ){ULD_R.InitCycleStep();}
    else if(m_iManNo == mcULD_RWorkSttTop       ){MT_GoAbsMan(miULD_ZElevR , PM.GetValue(miULD_ZElevR , pvULD_ZElevRWorkStartBtm));}
    else if(m_iManNo == mcULD_RWorkSttBtm       ){MT_GoAbsMan(miULD_ZElevR , PM.GetValue(miULD_ZElevR , pvULD_ZElevRWorkStartTop));}
    else if(m_iManNo == mcULD_RPitchUp          ){MT_GoIncMan(miULD_ZElevR ,-OM.DevInfo.dMgzSlotPitch);}
    else if(m_iManNo == mcULD_RPitchDn          ){MT_GoIncMan(miULD_ZElevR , OM.DevInfo.dMgzSlotPitch);}

    else if(m_iManNo == mcSTGCycleUp            ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTGCycleDn            ){STG.InitCycleStep();}


    else if(m_iManNo == mcPCKCyclePick          ){PCK.InitCycleStep();}
    else if(m_iManNo == mcPCKCycleAlign         ){PCK.InitCycleStep();}
    else if(m_iManNo == mcPCKCycleAlignPick     ){PCK.InitCycleStep();}
    else if(m_iManNo == mcPCKCycleVisn          ){PCK.InitCycleStep();}
    else if(m_iManNo == mcPCKCycleAttach        ){PCK.InitCycleStep();}
    else if(m_iManNo == mcPCKCyclePush          ){PCK.InitCycleStep();}
    else if(m_iManNo == mcPCKCycleManStdCal     ){PCK.InitCycleStep();}
    else if(m_iManNo == mcPCKCycleManVsDefectPos){PCK.InitCycleStep();}
    else if(m_iManNo == mcPCKCycleManVsAlignPos ){PCK.InitCycleStep();}
    else if(m_iManNo == mcPCKCycleManAlgnPlce   ){PCK.InitCycleStep();}
    else if(m_iManNo == mcPCKCycleManAlignerCal ){PCK.InitCycleStep();}

    else if(m_iManNo == mcDSPCycleVisnBf        ){DSP.InitCycleStep();}
    else if(m_iManNo == mcDSPCycleDisp          ){DSP.InitCycleStep();}
    else if(m_iManNo == mcDSPCycleVisnAt        ){DSP.InitCycleStep();}
    else if(m_iManNo == mcDSPCycleClean         ){DSP.InitCycleStep();}
    else if(m_iManNo == mcDSPCycleManNdlCheck   ){DSP.InitCycleStep();}
    else if(m_iManNo == mcDSPCycleManStdCal     ){DSP.InitCycleStep();}
    else if(m_iManNo == mcDSPCycleVisnPos       ){DSP.InitCycleStep();}
    else if(m_iManNo == mcDSPCycleNdlClean      ){DSP.InitCycleStep();}
    else if(m_iManNo == mcDSPCycleManSlnChange  ){DSP.InitCycleStep();}
    m_bManSetting = false ; //m_bManSetting 중요함 리턴전에 꼭 펄스 시켜야함. 쓰레드가 달라서. ::Update에서 m_iManNo=0이 되므로 주의.
    return true ;
}

EN_MANUAL_CYCLE CManualMan::GetManNo()
{
    return m_iManNo ;
}

int CManualMan::GetManStep()
{
    return m_iManStep ;
}

//---------------------------------------------------------------------------
void CManualMan::Update(void)
{

    if( m_iManNo == mcNoneCycle ) return ;
    if(m_bManSetting            ) return ;

      if(!OM.CmnOptn.bIgnrDoor   ) SEQ.InspectDoor    ();    
      if(m_iManNo != mcLDR_FHome &&
         m_iManNo != mcLDR_RHome &&
         m_iManNo != mcPRI_FHome &&
         m_iManNo != mcPRI_RHome &&
         m_iManNo != mcIDX_FHome &&
         m_iManNo != mcIDX_RHome &&
         m_iManNo != mcULD_FHome &&
         m_iManNo != mcULD_RHome &&
         m_iManNo != mcSTGHome   &&
         m_iManNo != mcPCKHome   &&
         m_iManNo != mcDSPHome   &&
         m_iManNo != mcAllHome   ) SEQ.InspectHomeEnd ();

      SEQ.InspectLightGrid();

      bool r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11 ;
      r1=r2=r3=r4=r5=r6=r7=r8=r9=r10=r11 = false ;

      //Check Alarm.
      if ( EM_IsErr()      ) { Init(); return ; }             //test
    //Cycle Step.
    if      (m_iManNo == mcAllHome      ) { r1 = PRI_F.CycleHome();
                                            r2 = PRI_R.CycleHome();
                                            r3 = STG  .CycleHome();
                                            r4 = DSP  .CycleHome();

                                            //피커와 DISP같이 홈잡으면 둘이 모여있을때 자제를 떨어 트린다.
                                            if(MT_GetHomeEnd(miDSP_ZDispr)) {r5 = PCK.CycleHome();}
                                            else                            {r5 = false;          }
                                            if(!r1||!r2||!r3||!r4||!r5) {
                                                return ;
                                            }

                                            r6  = LDR_F.CycleHome();
                                            r7  = LDR_R.CycleHome();
                                            r8  = IDX_F.CycleHome();
                                            r9  = IDX_R.CycleHome();
                                            r10 = ULD_F.CycleHome();
                                            r11 = ULD_R.CycleHome();

                                            if(r6&&r7&&r8&&r9&&r10&&r11) {
                                                m_iManNo = mcNoneCycle;
                                                FM_MsgOk("Confirm","All Home Finished!");
                                            }
                                          }





    else if(m_iManNo == mcLDR_FHome             ){if(LDR_F.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_RHome             ){if(LDR_R.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPRI_FHome             ){if(PRI_F.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPRI_RHome             ){if(PRI_R.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcIDX_FHome             ){if(IDX_F.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcIDX_RHome             ){if(IDX_R.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_FHome             ){if(ULD_F.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_RHome             ){if(ULD_R.CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTGHome               ){if(STG  .CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPCKHome               ){if(PCK  .CycleHome        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcDSPHome               ){if(DSP  .CycleHome        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcVisnCal               ){if(CycleVisnCal           ()) m_iManNo = mcNoneCycle; }
    //else if(m_iManNo == mcLdrUldMoveWait        ){if(CycleLdrUldMoveWait    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcStgFlatness           ){if(CycleStgFlatness       ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcLDR_FCycleWork        ){if(LDR_F.CycleWork        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcLDR_RCycleWork        ){if(LDR_R.CycleWork        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcPRI_FCycleWork        ){if(PRI_F.CycleWork        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPRI_RCycleWork        ){if(PRI_R.CycleWork        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcIDX_FCycleGet         ){if(IDX_F.CycleGet         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcIDX_FCycleWork        ){if(IDX_F.CycleWork        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcIDX_FCycleOut         ){if(IDX_F.CycleOut         ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcIDX_RCycleGet         ){if(IDX_R.CycleGet         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcIDX_RCycleWork        ){if(IDX_R.CycleWork        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcIDX_RCycleOut         ){if(IDX_R.CycleOut         ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcULD_FCycleWork        ){if(ULD_F.CycleWork        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcULD_RCycleWork        ){if(ULD_R.CycleWork        ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcSTGCycleUp            ){if(STG.CycleUp            ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcSTGCycleDn            ){if(STG.CycleDown          ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcPCKCyclePick          ){if(PCK.CyclePick          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPCKCycleAlign         ){if(PCK.CycleAlign         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPCKCycleAlignPick     ){if(PCK.CycleAlignPick     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPCKCycleVisn          ){if(PCK.CycleVisn          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPCKCycleAttach        ){if(PCK.CycleAttach        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPCKCyclePush          ){if(PCK.CyclePush          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPCKCycleManStdCal     ){if(PCK.CycleManStdCal     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPCKCycleManVsDefectPos){if(PCK.CycleManVsDefectPos()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPCKCycleManVsAlignPos ){if(PCK.CycleManVsAlignPos ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPCKCycleManAlgnPlce   ){if(PCK.CycleManAlgnPlce   ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcPCKCycleManAlignerCal ){if(PCK.CycleManAlignerCal ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcDSPCycleVisnBf        ){if(DSP.CycleVisnBf        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcDSPCycleDisp          ){if(DSP.CycleDisp          ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcDSPCycleVisnAt        ){if(DSP.CycleVisnAt        ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcDSPCycleManNdlCheck   ){if(DSP.CycleManNdlCheck   ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcDSPCycleClean         ){if(DSP.CycleClean         ()) m_iManNo = mcNoneCycle; }

    else if(m_iManNo == mcDSPCycleManStdCal     ){if(DSP.CycleManStdCal     ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcDSPCycleVisnPos       ){if(DSP.CycleManVisnPos    ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcDSPCycleNdlClean      ){if(DSP.CycleClean         ()) m_iManNo = mcNoneCycle; }
    else if(m_iManNo == mcDSPCycleManSlnChange  ){if(DSP.CycleManSlnChange  ()) m_iManNo = mcNoneCycle; }
    else                                         {                              m_iManNo = mcNoneCycle; } //여기서 갱신됌.

    //Ok.
    return ;

}


bool CManualMan::CycleVisnCal()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;

    static int iPckDnCnt = 0 ;
    const int iPckDnMaxCnt = 2 ;

    double dPckVisnRsltX , dPckVisnRsltY , dPckVisnRsltT ;
    double dDspVisnRsltX , dDspVisnRsltY , dDspVisnRsltT ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: m_dVisnRsltGapX = 999 ;
                  m_dVisnRsltGapY = 999 ;
                  m_dVisnRsltGapT = 999 ;
                  iPckDnCnt = 0 ;
                  PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                  DSP.MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                  m_iManStep++;
                  return false;

        case  11: if(!PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                  if(!DSP.MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;
                  PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);
                  DSP.MoveMotr(miDSP_YDispr , piDSP_YDisprWait  );
                  //DSP.InitCycleStep();

                  m_iManStep++;
                  return false;

        case  12: //if(!DSP.CycleManVisnPos()) return false ;
                  if(!PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrHeight)) return false ;
                  if(!DSP.MoveMotr(miDSP_YDispr , piDSP_YDisprWait  )) return false ;
                  PCK.InitCycleStep();
                  m_iManStep++;
                  return false ;


        case  13: if(!PCK.CycleAlignPick())return false;
                  PCK.InitCycleStep();

                  m_iManStep++;
                  return false;

        case  14: if(!PCK.CycleVisn())return false;
                  IO_SetY(ySTG_Vccm , false);
                  PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrStageCntr);
                  STG.MoveMotr(miSTG_XStage , piSTG_XStagCenter);
                  STG.MoveMotr(miSTG_YStage , piSTG_YStagCenter);
                  STG.MoveMotr(miSTG_TStage , piSTG_TStagWork  );

                  m_iManStep++;
                  return false ;

        case  15: if(!PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrStageCntr)) return false ;
                  if(!STG.MoveMotr(miSTG_XStage , piSTG_XStagCenter)) return false ;
                  if(!STG.MoveMotr(miSTG_YStage , piSTG_YStagCenter)) return false ;
                  if(!STG.MoveMotr(miSTG_TStage , piSTG_TStagWork  )) return false ;
                  STG.MoveMotr(miSTG_ZStage , piSTG_ZStagCheck);
                  m_iManStep++;
                  return false ;

        case  16: if(!STG.MoveMotr(miSTG_ZStage , piSTG_ZStagCheck)) return false ;                    //떨어트리는 느낌이라 0.1 더 내림.
                  MT_GoAbs(miPCK_ZPickr , PM.GetValue (miPCK_ZPickr , pvPCK_ZPickrCheck ) - PCK.GetPkgThickness()+0.1 , 5.0); //천천히 내려 본다.
                  m_iManStep++;
                  return false;

        case  17: if(!MT_GetStop(miPCK_ZPickr)) return false ;
                  m_tmDelay.Clear();
                  m_iManStep++;
                  return false ;

        case  18: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                  //<Temp>                      IO_SetY(yPCK_VccmOff , true);
                  IO_SetY(yPCK_VccmOn , false );
                  m_tmDelay.Clear();
                  m_iManStep++;
                  return false ;

        case  19: if(!m_tmDelay.OnDelay(true , 2000)) return false ;
                  IO_SetY(yPCK_VccmOff , false);
                  MT_GoInc(miPCK_ZPickr , - 1.0, 1.0); //천천히 올려본다.
                  m_iManStep++;
                  return false ;

        case  20: if(!MT_GetStop(miPCK_ZPickr)) return false ;
                  PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                  m_iManStep++;
                  return false ;

        case  21: if(!PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                  PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrAlign);
                  m_iManStep++;
                  return false ;

        case  22: if(!PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrAlign)) return false ;
                  DSP.InitCycleStep();
                  m_iManStep++;
                  return false;

        case  23: if(!DSP.CycleVisnAt()) return false ;
                  DM.ARAY[riSTG].SetStat(csNone);

                  //두개 비젼의 결과값을 차를 알아서 디스플레이 해준다.
                  //셑업자는 이를 보고 Picker와 Stage의 센터를 맞춘다.
                  PCK.GetVisnRslt(&dPckVisnRsltX ,&dPckVisnRsltY ,&dPckVisnRsltT       );
                  DSP.GetVisnRslt(&dDspVisnRsltX ,&dDspVisnRsltY ,&dDspVisnRsltT , true);

                  

                  m_dVisnRsltGapX =(-dPckVisnRsltX + dDspVisnRsltX) ;
                  m_dVisnRsltGapY =( dPckVisnRsltY + dDspVisnRsltY) ; //위비젼 아래 비전이라 Y결과값과 T결과값은 반대이다.
                  //m_dVisnRsltGapT =(-dPckVisnRsltT + dDspVisnRsltT) ;

                  Trace("Visn Cal",(String("ORI DSP X :")+PM.GetValue(miDSP_XDispr , pvDSP_XDisprVsStageCntr)).c_str());
                  Trace("Visn Cal",(String("ORI DSP Y :")+PM.GetValue(miDSP_YDispr , pvDSP_YDisprVsStageCntr)).c_str());
                  PM.SetValue(miDSP_XDispr , pvDSP_XDisprVsStageCntr , PM.GetValue(miDSP_XDispr , pvDSP_XDisprVsStageCntr) + m_dVisnRsltGapY);
                  //픽커 쪽을 바꾸면 얼라이너 바뀜에 따라 레일 센터를 보던 Disp의 포지션이 바뀐다.
                  //그래서 Y축은 Picker에서 바뀌어야 한다.
                  //PM.SetValue(miDSP_YDispr , pvDSP_YDisprVsStageCntr , PM.GetValue(miDSP_YDispr , pvDSP_YDisprVsStageCntr) + m_dVisnRsltGapX);
                  PM.SetValue(miPCK_YPickr , pvPCK_YPickrStageCntr , PM.GetValue(miPCK_YPickr , pvPCK_YPickrStageCntr) + m_dVisnRsltGapX);
                  DSP.m_bRqstPMSave = true ;
                  Trace("Visn Cal",(String("CAL DSP X :")+PM.GetValue(miDSP_XDispr , pvDSP_XDisprVsStageCntr)).c_str());
                  Trace("Visn Cal",(String("CAL DSP Y :")+PM.GetValue(miDSP_YDispr , pvDSP_YDisprVsStageCntr)).c_str());



                  m_iManStep++;
                  return false;

        case  24:


                  DSP.MoveMotr(miDSP_YDispr , piDSP_YDisprWait);
                  m_iManStep++;
                  return false ;

        case  25: if(!DSP.MoveMotr(miDSP_YDispr , piDSP_YDisprWait)) return false ;
                  PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrStageCntr);
                  m_iManStep++;
                  return false ;

        case  26: if(!PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrStageCntr)) return false ; //위에서 비젼 때리면서 높이 Check면으로 맞춘다.
                  MT_GoAbs(miPCK_ZPickr , PM.GetValue (miPCK_ZPickr , pvPCK_ZPickrCheck )/* - PCK.GetPkgThickness()*/ , 5.0); //천천히 내려 본다.
                  m_iManStep++;
                  return false ;
        //밑에서 씀.
        case  27: if(!MT_GetStop(miPCK_ZPickr)) return false ;
                  IO_SetY(yPCK_VccmOn,true);
                  m_tmDelay.Clear();
                  m_iManStep++;
                  return false ;

        case  28: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                  //<Temp>IO_SetY(yPCK_VccmOn,false);
                  if(!IO_GetX(xPCK_Vccm)){
                      if(iPckDnCnt < iPckDnMaxCnt){
                          MT_GoInc(miPCK_ZPickr , 0.1 , 5.0); //천천히 내려 본다.
                          iPckDnCnt++;
                          m_iManStep = 27;
                          return false ;
                      }
                      else{
                          PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                          m_iManStep++;
                          return false ;
                      }
                  }
                  PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                  m_iManStep++;
                  return false ;

        case  29: if(!PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                  PCK.InitCycleStep();

                  m_iManStep++;
                  return false;

        case  30: if(!PCK.CycleManAlgnPlce())return false;

                  m_iManStep = 0;
                  return true;

        }
}

bool CManualMan::GetVisnGap(double * _dVisnX , double * _dVisnY , double * _dVisnT)
{
    if(m_dVisnRsltGapX == 999)return false ;
    if(m_dVisnRsltGapY == 999)return false ;
    if(m_dVisnRsltGapT == 999)return false ;


    * _dVisnX = m_dVisnRsltGapX ;
    * _dVisnX = m_dVisnRsltGapY ;
    * _dVisnX = m_dVisnRsltGapT ;

    return true ;
}

bool CManualMan::CycleStgFlatness()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 30000 )) {
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

    if(EM_IsErr()){ //에러면 중지시킨다.
        m_iManStep = 0 ;
        return true ;
    }

    m_iPreManStep = m_iManStep ;

    switch (m_iManStep) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                  //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  m_iManStep = 0 ;
                  return true ;

        case  10: DSP.MoveMotr(miDSP_ZDispr, piDSP_ZDisprWait);
                  STG.MoveMotr(miSTG_ZStage, piSTG_ZStagWait);
                  m_iManStep++;
                  return false;

        case  11: if(!DSP.MoveMotr(miDSP_ZDispr, piDSP_ZDisprWait)) return false ;
                  if(!STG.MoveMotr(miSTG_ZStage, piSTG_ZStagWait )) return false ;
                  STG.MoveMotr(miSTG_ZStage, piSTG_ZStagWork);

                  m_iManStep++;
                  return false;

        case  12: if(!STG.MoveMotr(miSTG_ZStage, piSTG_ZStagWork )) return false ;
                  DSP.MoveMotr(miDSP_XDispr, piDSP_XDisprStgFlatness1);
                  DSP.MoveMotr(miDSP_YDispr, piDSP_YDisprStgFlatness1);

                  m_iManStep++;
                  return false ;


        case  13: if(!DSP.MoveMotr(miDSP_XDispr, piDSP_XDisprStgFlatness1)) return false;
                  if(!DSP.MoveMotr(miDSP_YDispr, piDSP_YDisprStgFlatness1)) return false;
                  m_tmDelay.Clear();
                  m_iManStep++;
                  return false;

        case  14: if(!m_tmDelay.OnDelay(true, 100)) return false;
                  Rs232Keyence.CheckHeight(kcChB);
                  m_iManStep++;
                  return false;

        case  15: if(!Rs232Keyence.GetMsgEnd()) return false ;
                  if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChB)) {
                      EM_SetErrMsg(eiETC_RS232 , "높이 측정기 측정포지션을 확인하세요.");
                      m_iManStep = 0 ;
                      return true ;
                  }
                  m_dStgFlatness1 = Rs232Keyence.GetHeightData(kcChB);
                  m_iManStep++;
                  return false ;

        case  16: DSP.MoveMotr(miDSP_XDispr, piDSP_XDisprStgFlatness2);
                  DSP.MoveMotr(miDSP_YDispr, piDSP_YDisprStgFlatness2);

                  m_iManStep++;
                  return false ;


        case  17: if(!DSP.MoveMotr(miDSP_XDispr, piDSP_XDisprStgFlatness2)) return false;
                  if(!DSP.MoveMotr(miDSP_YDispr, piDSP_YDisprStgFlatness2)) return false;
                  m_tmDelay.Clear();
                  m_iManStep++;
                  return false;

        case  18: if(!m_tmDelay.OnDelay(true, 100))return false;
                  Rs232Keyence.CheckHeight(kcChB);
                  m_iManStep++;
                  return false;

        case  19: if(!Rs232Keyence.GetMsgEnd()) return false ;
                  if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChB)) {
                      EM_SetErrMsg(eiETC_RS232 , "높이 측정기 측정포지션을 확인하세요.");
                      m_iManStep = 0 ;
                      return true ;
                  }
                  m_dStgFlatness2 = Rs232Keyence.GetHeightData(kcChB);
                  m_iManStep++;
                  return false ;

        case  20: DSP.MoveMotr(miDSP_XDispr, piDSP_XDisprStgFlatness3);
                  DSP.MoveMotr(miDSP_YDispr, piDSP_YDisprStgFlatness3);

                  m_iManStep++;
                  return false ;


        case  21: if(!DSP.MoveMotr(miDSP_XDispr, piDSP_XDisprStgFlatness3)) return false;
                  if(!DSP.MoveMotr(miDSP_YDispr, piDSP_YDisprStgFlatness3)) return false;
                  m_tmDelay.Clear();
                  m_iManStep++;
                  return false;

        case  22: if(!m_tmDelay.OnDelay(true, 100)) return false;
                  Rs232Keyence.CheckHeight(kcChB);
                  m_iManStep++;
                  return false;

        case  23: if(!Rs232Keyence.GetMsgEnd()) return false ;
                  if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChB)) {
                      EM_SetErrMsg(eiETC_RS232 , "높이 측정기 측정포지션을 확인하세요.");
                      m_iManStep = 0 ;
                      return true ;
                  }
                  m_dStgFlatness3 = Rs232Keyence.GetHeightData(kcChB);
                  m_iManStep++;
                  return false ;

        case  24: DSP.MoveMotr(miDSP_XDispr, piDSP_XDisprStgFlatness4);
                  DSP.MoveMotr(miDSP_YDispr, piDSP_YDisprStgFlatness4);

                  m_iManStep++;
                  return false ;


        case  25: if(!DSP.MoveMotr(miDSP_XDispr, piDSP_XDisprStgFlatness4)) return false;
                  if(!DSP.MoveMotr(miDSP_YDispr, piDSP_YDisprStgFlatness4)) return false;
                  m_tmDelay.Clear();
                  m_iManStep++;
                  return false;

        case  26: if(!m_tmDelay.OnDelay(true, 100))return false;
                  Rs232Keyence.CheckHeight(kcChB);
                  m_iManStep++;
                  return false;

        case  27: if(!Rs232Keyence.GetMsgEnd()) return false ;
                  if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChB)) {
                      EM_SetErrMsg(eiETC_RS232 , "높이 측정기 측정포지션을 확인하세요.");
                      m_iManStep = 0 ;
                      return true ;
                  }
                  m_dStgFlatness4 = Rs232Keyence.GetHeightData(kcChB);
                  STG.MoveMotr(miSTG_ZStage, piSTG_ZStagWait);
                  m_iManStep++;
                  return false ;

        case  28: if(!STG.MoveMotr(miSTG_ZStage, piSTG_ZStagWait)) return false;
                  FM_MsgOk("Check Height", String("1 : " + String(m_dStgFlatness1) + ", 2 : " + String(m_dStgFlatness2) + ", 3 : " + String(m_dStgFlatness3) + ", 4 : " + String(m_dStgFlatness4)).c_str());

                  m_iManStep = 0;
                  return true ;

        }
}







































