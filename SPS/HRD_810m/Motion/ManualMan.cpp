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
#include "LotUnit.h"
#include "Rs232Man.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Head.h"
#include "Stage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

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
    if( _iNo      <  0                ) {FM_MsgOk("ERROR","잘못된 메뉴얼 동작입니다." ); return false ;}
    if( _iNo      >= MAX_MANUAL_CYCLE ) {FM_MsgOk("ERROR","잘못된 메뉴얼 동작입니다." ); return false ;}
    if( m_iManNo  != mcNoneCycle      ) {FM_MsgOk("ERROR","아직 메뉴얼 동작 중입니다."); return false ;}
    if(SEQ._bRun                      ) {FM_MsgOk("ERROR","장비가 동작 중입니다."     ); return false ;}
    if(SEQ._iStep != CSequence::scIdle) {FM_MsgOk("ERROR","장비가 동작 중입니다."     ); return false ;}

    //Check Alar
    if ( EM_IsErr()      ) { Init(); return false  ; } //아 밑에 처리 하는 애 때문에 잠시 이렇게 함.

    EM_SetDisp(true);

    m_bManSetting = true ;
    m_iManNo = _iNo ;

    bool bRet = true ;

//***********************************************************************************************
// 프리프로세서
//***********************************************************************************************

    if(m_iManNo == mcAllHome ){
        //if(!IO_GetX(xETC_PowerOn  )    ) {FM_MsgTime("ERR","Power On Plz        ",2000); bRet = false ;}
        if(!IO_GetX(xETC_MainAir   )) {FM_MsgTime("ERR","메인공압 센서를 확인 해주세요!",2000); bRet = false ;}
        if(!IO_GetY(yETC_MainAirSol)) {FM_MsgTime("ERR","메인 에어 SOL을 확인 해주세요!",2000); bRet = false ;}
        if(!MT_GetServoAll()        ) {FM_MsgTime("ERR","모터들을 Servo On 해주세요!"   ,2000); bRet = false ;}
    }
    else if(m_iManNo == mcHED_CycleTAlign    ){
        //if(DM.ARAY[riSTG].CheckAllStat(csNone)) {FM_MsgTime("ERROR","Check Stage Map Aray!",2000); bRet = false ;}
    }

    else if(m_iManNo == mcETC_MainAirOnOff     ){}
    else if(m_iManNo == mcSTG_CycleHome        ){}
    else if(m_iManNo == mcSTG_CycleSupply      ){
        if(!DM.ARAY[riCST].GetCntStat(csUnkwn))  {FM_MsgTime("ERROR","OPREATIOM 화면에 카세트에서 작업전 영역이 있는지 확인해 주세요.!",2000); bRet = false ;}
        if(!IO_GetX(xSTG_CastDetect          ))  {FM_MsgTime("ERROR","카세트 유무를 확인 해 주세요!"             ,2000); bRet = false ;}
        if(!DM.ARAY[riSTG].CheckAllStat(csNone)) {FM_MsgTime("ERROR","스테이지에 웨이퍼데이터를 제거하세요"      ,2000); bRet = false ;}
        if( IO_GetX(xSTG_SgWfDetect          ))  {FM_MsgTime("ERROR","스테이지에 웨이퍼를 제거하세요"            ,2000); bRet = false ;}
        if((MT_GetCmdPos(miHED_XHed) != PM.GetValue(miHED_XHed , pvHED_XHedPCover)) ||
           (MT_GetCmdPos(miHED_YHed) != PM.GetValue(miHED_YHed , pvHED_YHedPCover))) {
            FM_MsgTime("ERROR","헤드 모터들을 펜 커버 포지션으로 이동해 주세요.",2000); bRet = false ;
        }
    }
    else if(m_iManNo == mcSTG_CycleExpend      ){}

    else if(m_iManNo == mcSTG_CyclePlace       ){
        if(!DM.ARAY[riCST].GetCntStat(csMask))   {FM_MsgTime("ERROR","OPREATIOM 화면에 카세트에서 작업중 영역이 있는지 확인해 주세요.!",2000); bRet = false ;}
        if(!IO_GetX(xSTG_CastDetect         ))   {FM_MsgTime("ERROR","카세트 유무를 확인 해 주세요!"  ,2000); bRet = false ;}
        if((MT_GetCmdPos(miHED_XHed) != PM.GetValue(miHED_XHed , pvHED_XHedPCover)) ||
           (MT_GetCmdPos(miHED_YHed) != PM.GetValue(miHED_YHed , pvHED_YHedPCover))) {
            FM_MsgTime("ERROR","헤드 모터들을 펜 커버 포지션으로 이동해 주세요.",2000); bRet = false ;
        }
    }
    else if(m_iManNo == mcSTG_CycleUnExpend    ){}
    else if(m_iManNo == mcSTG_VaccumOnOff      ){}
    else if(m_iManNo == mcSTG_WfrAlgn          ){}

    else if(m_iManNo == mcHED_CycleHome        ){}
    else if(m_iManNo == mcHED_CycleOrigin      ){}
    else if(m_iManNo == mcHED_CycleChipAlign   ){}
    else if(m_iManNo == mcHED_CycleMark        ){}
    else if(m_iManNo == mcHED_CycleClickMove   ){}
    else if(m_iManNo == mcHED_CycleCntrMove    ){}
    else if(m_iManNo == mcHED_CycleCntrMoveT   ){}

    else if(m_iManNo == mcHED_CycleCal         ){}
    else if(m_iManNo == mcHED_CyclePenClearPs  ){}
    else if(m_iManNo == mcHED_CyclePenChangPs  ){}
    else if(m_iManNo == mcHED_CyclePenCheck    ){}
    else if(m_iManNo == mcHED_CyclePenAlign    ){}
    else if(m_iManNo == mcHED_CycleMarkTraceBw ){}
    else if(m_iManNo == mcHED_CycleMarkTraceFw ){}
    else if(m_iManNo == mcHED_CyclePenCoverPs  ){}
    else if(m_iManNo == mcETC_LampOnOff        ){}

    else if(m_iManNo == mcHED_YHedHWCntrPos    ){}
    else if(m_iManNo == mcHED_HedVTAlgn        ){}
    else if(m_iManNo == mcHED_HedVOri          ){}
    else if(m_iManNo == mcHED_HedVCalPad       ){}
    else if(m_iManNo == mcHED_HedVBoardStt     ){}

    if(!bRet) Init();

//***********************************************************************************************
// 처리..
//***********************************************************************************************
         if (m_iManNo == mcAllHome ) {
             STG.InitHomeStep();
             HED.InitHomeStep();
             for(int i=0; i<MAX_MOTR; i++) MT_SetHomeEnd(i,false);
         }
    else if(m_iManNo == mcSTG_CycleHome       ){STG.InitHomeStep ();}
    else if(m_iManNo == mcSTG_CycleSupply     ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CycleExpend     ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CyclePlace      ){STG.InitCycleStep();}
    else if(m_iManNo == mcSTG_CycleUnExpend   ){STG.InitCycleStep();}

    else if(m_iManNo == mcHED_CycleHome       ){HED.InitHomeStep ();}
    else if(m_iManNo == mcHED_CycleCal        ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CyclePenCheck   ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CyclePenChangPs ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleTAlign     ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleOrigin     ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleChipAlign  ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleMark       ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleClickMove  ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleCntrMove   ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleCntrMoveT  ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CyclePenClearPs ){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleMarkTraceBw){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CycleMarkTraceFw){HED.InitCycleStep();}
    else if(m_iManNo == mcHED_CyclePenCoverPs ){HED.InitCycleStep();}

    else if(m_iManNo == mcHED_CyclePenAlign   ){HED.InitCycleStep();}

    else if(m_iManNo == mcSTG_VaccumOnOff     ){if(IO_GetY(ySTG_WfrVac    )) IO_SetY(ySTG_WfrVac     , false);
                                                else                         IO_SetY(ySTG_WfrVac     , true ); }
    else if(m_iManNo == mcETC_MainAirOnOff    ){if(IO_GetY(ySTG_Ionizer   )) IO_SetY(ySTG_Ionizer    , false);
                                                else                         IO_SetY(ySTG_Ionizer    , true ); }

    else if(m_iManNo == mcETC_LampOnOff       ){if(IO_GetY(yETC_TopLight  ))  IO_SetY(yETC_TopLight   , false);
                                                else                          IO_SetY(yETC_TopLight   , true ); }
    else if(m_iManNo == mcSTG_WfrAlgn         ){if(IO_GetY(ySTG_WfrAlignBw)) {IO_SetY(ySTG_WfrAlignBw , false); IO_SetY(ySTG_WfrAlignFw, true);}
                                                else                         {IO_SetY(ySTG_WfrAlignFw , false); IO_SetY(ySTG_WfrAlignBw, true);}}


    else if(m_iManNo == mcHED_YHedHWCntrPos    ){m_iManStep = 10 ; m_iPreManStep = 0 ; }
    else if(m_iManNo == mcHED_HedVTAlgn        ){m_iManStep = 10 ; m_iPreManStep = 0 ; }
    else if(m_iManNo == mcHED_HedVOri          ){m_iManStep = 10 ; m_iPreManStep = 0 ; }
    else if(m_iManNo == mcHED_HedVCalPad       ){m_iManStep = 10 ; m_iPreManStep = 0 ; }
    else if(m_iManNo == mcHED_HedVBoardStt     ){m_iManStep = 10 ; m_iPreManStep = 0 ; }
    else { m_iManNo = mcNoneCycle ; }

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
    if( m_bStop                 ) { m_iManNo = mcNoneCycle; m_bStop = false ; return ; } // Stop 추가 dd 0523 . PM 1h

    if(!OM.CmnOptn.bDoorSkip    ) SEQ.InspectDoor    ();
    if(m_iManNo != mcSTG_CycleHome &&
       m_iManNo != mcHED_CycleHome &&
       m_iManNo != mcAllHome       &&
       !OM.MstOptn.bDebugMode  ) SEQ.InspectHomeEnd ();
    SEQ.InspectLightGrid();

    bool r1,r2;

    //Check Alarm.
    if ( EM_IsErr()      ) { Init(); return ; }

    if (!IO_GetX(xETC_MainPwrOn) && m_iManNo == mcAllHome){
        m_iManNo = mcNoneCycle;
        EM_SetErr(eiETC_MainPwr);
        return;
    }

    //Cycle Step.
    if(m_iManNo == mcAllHome   ) {


        r1 = STG.CycleHome();
        r2 = HED.CycleHome();
        if(!r1 || !r2) return ;
        m_iManNo = mcNoneCycle;
        FM_MsgOk("Confirm","모든 홈 동작이 끝났습니다.!");
    }
    else if(m_iManNo == mcSTG_CycleHome       ){if(STG.CycleHome      ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcSTG_CycleSupply     ){if(STG.CycleSupply    ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcSTG_CycleExpend     ){if(STG.CycleExpend    ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcSTG_CyclePlace      ){if(STG.CyclePlace     ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcSTG_CycleUnExpend   ){if(STG.CycleUnExpend  ()) m_iManNo = mcNoneCycle ; }

    else if(m_iManNo == mcHED_CycleHome       ){if(HED.CycleHome      ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CycleCal        ){if(HED.CycleCal       ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CyclePenCheck   ){if(HED.CyclePenCheck  ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CycleTAlign     ){if(HED.CycleTAlign    ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CycleOrigin     ){if(HED.CycleOrigin    ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CycleChipAlign  ){if(HED.CycleChipAlign ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CycleMark       ){if(HED.CycleMark      ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CycleClickMove  ){if(HED.CycleClickMove ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CycleCntrMove   ){if(HED.CycleCntrMove  ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CycleCntrMoveT  ){if(HED.CycleCntrMove  ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CyclePenClearPs ){if(HED.CyclePenChange ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CyclePenChangPs ){if(HED.CyclePenChange ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CyclePenCoverPs ){if(HED.CyclePenCoverPs()) m_iManNo = mcNoneCycle ; }

    else if(m_iManNo == mcHED_CycleMarkTraceBw){if(HED.CycleMarkTrace ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_CycleMarkTraceFw){if(HED.CycleMarkTrace ()) m_iManNo = mcNoneCycle ; }


    else if(m_iManNo == mcHED_CyclePenAlign   ){if(HED.CyclePenCheck  ()) m_iManNo = mcNoneCycle ; }

    else if(m_iManNo == mcHED_YHedHWCntrPos   ){if(CycleHedVisnMove   ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_HedVTAlgn       ){if(CycleHedVisnMove   ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_HedVOri         ){if(CycleHedVisnMove   ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_HedVCalPad      ){if(CycleHedVisnMove   ()) m_iManNo = mcNoneCycle ; }
    else if(m_iManNo == mcHED_HedVBoardStt    ){if(CycleHedVisnMove   ()) m_iManNo = mcNoneCycle ; }

    else                                       {                          m_iManNo = mcNoneCycle ; }




    //Ok.
    return ;
}


bool CManualMan::CycleHedVisnMove()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(m_iManStep && m_iManStep == m_iPreManStep &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr(eiHED_CycleTO);
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s m_iPreManStep=%02d" , __FUNC__ , m_iPreManStep );
                 m_iManStep = 0 ;
                 return true ;

        case 10: STG.MoveMotr(miSTG_XTrs , piSTG_XTrsWait);

                 m_iManStep++;
                 return false ;

        case 11: if(!STG.MoveMotr(miSTG_XTrs , piSTG_XTrsWait)) return false ;

                 HED.MoveActr(aiHED_PenCoverUD , ccBwd);
                 m_iManStep++;
                 return false ;

        case 12: if(!HED.MoveActr(aiHED_PenCoverUD , ccBwd)) return false ;
                 if(m_iManNo == mcHED_YHedHWCntrPos) {HED.MoveMotr(miHED_XHed , piHED_XHedHWCntrPos); HED.MoveMotr(miHED_YHed , piHED_YHedHWCntrPos); }
                 if(m_iManNo == mcHED_HedVTAlgn    ) {HED.MoveMotr(miHED_XHed , piHED_XHedVTAlgn   ); HED.MoveMotr(miHED_YHed , piHED_YHedVTAlgn   ); }
                 if(m_iManNo == mcHED_HedVOri      ) {HED.MoveMotr(miHED_XHed , piHED_XHedVOri     ); HED.MoveMotr(miHED_YHed , piHED_YHedVOri     ); }
                 if(m_iManNo == mcHED_HedVCalPad   ) {HED.MoveMotr(miHED_XHed , piHED_XHedVCalPad  ); HED.MoveMotr(miHED_YHed , piHED_YHedVCalPad  ); }
                 if(m_iManNo == mcHED_HedVBoardStt ) {HED.MoveMotr(miHED_XHed , piHED_XHedVBoardStt); HED.MoveMotr(miHED_YHed , piHED_YHedVBoardStt); }

                 m_iManStep++;
                 return false ;

        case 13: if(!MT_GetStop(miHED_XHed)) return false ;
                 if(!MT_GetStop(miHED_YHed)) return false ;

                 m_iManStep = 0 ;
                 return true ;
    }
}


