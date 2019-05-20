//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnLoader430t.h"
#include "PostBuff.h"
#include "Loader.h"

#include "SLogUnit.h"
#include "UserIni.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CUnLoader ULD;

CUnLoader::CUnLoader(void)
{
}

CUnLoader::~CUnLoader (void)
{
}

void CUnLoader::Init()
{
    m_sPartName = "UnLoader " ;
    Reset();
    Load(true);

    InitCycleName();
}

void CUnLoader::Close()
{
    Load(false);
}


void CUnLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CUnLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}



bool CUnLoader::FindChip(int &r , int &c )
{
    r = DM.ARAY[riULD].FindFrstRow(csEmpty) ;
    return (r > -1);
}

double CUnLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int  r  , c  ;
    bool bOddRow ;

    FindChip(r , c);

    //X , Y Pos Setting.
    double dZ1Pos = PM.GetValue(miULD_ZClmp , pvULD_ZClmpWorkStart) - r * OM.DevInfo.dCsSlPitch ;
    double dPos  = 0.0 ;

    if(_iMotr == miULD_ZClmp){
        switch(_iPstnId) {
            default                      : dPos = MT_GetCmdPos(_iMotr                           ); break ;
            case piULD_ZClmpWait         : dPos = PM.GetValue (_iMotr , pvULD_ZClmpWait         ); break ;
            case piULD_ZClmpPickFwd      : dPos = PM.GetValue (_iMotr , pvULD_ZClmpPickFwd      ); break ;
            case piULD_ZClmpClampOn      : dPos = PM.GetValue (_iMotr , pvULD_ZClmpClampOn      ); break ;
            case piULD_ZClmpPickBwd      : dPos = PM.GetValue (_iMotr , pvULD_ZClmpPickBwd      ); break ;
            case piULD_ZClmpWorkStart    : dPos = PM.GetValue (_iMotr , pvULD_ZClmpWorkStart    ); break ;
            case piULD_ZClmpPlaceFwd     : dPos = PM.GetValue (_iMotr , pvULD_ZClmpPlaceFwd     ); break ;
            case piULD_ZClmpClampOff     : dPos = PM.GetValue (_iMotr , pvULD_ZClmpClampOff     ); break ;
            case piULD_ZClmpPlaceBwd     : dPos = PM.GetValue (_iMotr , pvULD_ZClmpPlaceBwd     ); break ;
            case piULD_ZClmpWork         : dPos = dZ1Pos                                        ; break ;
        }
    }

    else if(_iMotr == miULD_YClmp){
        switch(_iPstnId) {
            default                      : dPos = MT_GetCmdPos  (_iMotr                         ); break ;
            case piULD_YClmpWait         : dPos = PM.GetValue   (_iMotr , pvULD_YClmpWait       ); break ;
            case piULD_YClmpPick         : dPos = PM.GetValue   (_iMotr , pvULD_YClmpPick       ); break ;
            case piULD_YClmpWork         : dPos = PM.GetValue   (_iMotr , pvULD_YClmpWork       ); break ;
            case piULD_YClmpPlace        : dPos = PM.GetValue   (_iMotr , pvULD_YClmpPlace      ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CUnLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isBwPusher        = AT_Complete(aiPSB_PshrFwBw , ccBwd) ;
    bool isRailGateCnsr    = IO_GetX(xPSB_PkgOut6) ; //IO_GetX(xPSB_PkgOut1) || IO_GetX(xPSB_PkgOut2) ;
    bool isClampYFwd       = MT_CmprPos(miULD_YClmp , GetMotrPos(miULD_YClmp , piULD_YClmpPick)) || MT_CmprPos(miULD_YClmp , GetMotrPos(miULD_YClmp , piULD_YClmpPlace    ));
    bool isClampZDnstair   = MT_CmprPos(miULD_ZClmp , piULD_ZClmpPickBwd)  || MT_CmprPos(miULD_ZClmp , piULD_ZClmpPickFwd  ) || MT_CmprPos(miULD_ZClmp , piULD_ZClmpClampOn   );
    bool isClampZCenter    = MT_CmprPos(miULD_ZClmp , piULD_ZClmpWait   )  || MT_CmprPos(miULD_ZClmp , piULD_ZClmpWork     ) || MT_CmprPos(miULD_ZClmp , piULD_ZClmpWorkStart );
    bool isClampZUpstair   = MT_CmprPos(miULD_ZClmp , piULD_ZClmpPlaceBwd) || MT_CmprPos(miULD_ZClmp , piULD_ZClmpPlaceFwd ) || MT_CmprPos(miULD_ZClmp , piULD_ZClmpClampOff  );

    if(_iMotr == miULD_ZClmp){
             if(!isBwPusher    ) { sMsg = "PSB Pusher is not Bwd" ; bRet = false ;}
        else if(isRailGateCnsr) { sMsg = "Strip is feeding now      " ; bRet = false ;}
            switch(_iPstnId) {
                default                   :                                                       { sMsg = "Not " +AnsiString(MT_GetName(miLDR_ZClmp))+"'s Postion" ; bRet = false ;} break ;                                                                           break ;
                //UpStair
                case piULD_ZClmpPickBwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = AnsiString(MT_GetName(miULD_YClmp))+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZClmpClampOn   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = AnsiString(MT_GetName(miULD_YClmp))+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZClmpPickFwd   : if(isClampYFwd &&                   isClampZDnstair)  { sMsg = AnsiString(MT_GetName(miULD_YClmp))+" is Fwd" ; bRet = false ;} break ;
                //Middle
                case piULD_ZClmpWait      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = AnsiString(MT_GetName(miULD_YClmp))+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZClmpWorkStart : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = AnsiString(MT_GetName(miULD_YClmp))+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZClmpWork      : if(isClampYFwd && (isClampZUpstair||isClampZDnstair)) { sMsg = AnsiString(MT_GetName(miULD_YClmp))+" is Fwd" ; bRet = false ;} break ;
                //DnStatir
                case piULD_ZClmpPlaceFwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = AnsiString(MT_GetName(miULD_YClmp))+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZClmpClampOff  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = AnsiString(MT_GetName(miULD_YClmp))+" is Fwd" ; bRet = false ;} break ;
                case piULD_ZClmpPlaceBwd  : if(isClampYFwd &&  isClampZUpstair)                   { sMsg = AnsiString(MT_GetName(miULD_YClmp))+" is Fwd" ; bRet = false ;} break ;

        }
    }

    else if(_iMotr == miULD_YClmp){
        if(!isBwPusher) return false ;
        else {
            switch(_iPstnId) {
                default                   :                    { sMsg = "Not " +AnsiString(MT_GetName(miLDR_ZClmp))+"'s Postion" ; bRet = false ;} break ;                                                                                      break ;
                case piULD_YClmpWait      :                                                                                                        break ;
                case piULD_YClmpWork      :                                                                                                        break ;
                case piULD_YClmpPick      : if(isClampZCenter) {sMsg = AnsiString(MT_GetName(miULD_ZClmp))+" is Middle Position" ; bRet = false ;} break ;
                case piULD_YClmpPlace     : if(isClampZCenter) {sMsg = AnsiString(MT_GetName(miULD_ZClmp))+" is Middle Position" ; bRet = false ;} break ;
            }
        }
    }
    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CUnLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool bULD_YClmpMoving = !MT_GetStop(miULD_YClmp) ;
    bool bULD_ZClmpMoving = !MT_GetStop(miULD_ZClmp) ;

    if(_iActr == aiULD_ClmpUpDn){
        if(bULD_YClmpMoving) {sMsg = AnsiString(MT_GetName(miULD_YClmp)) + "is Moving!"; bRet = false ;}
        if(bULD_ZClmpMoving) {sMsg = AnsiString(AT_GetName(miULD_ZClmp)) + "is Moving!"; bRet = false ;}
    }
    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }


    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }

    return bRet ;
}

bool CUnLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CUnLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CUnLoader::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo,m_sPartName.c_str()); //EM_SetErr(eiULD_HomeTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iHome != PreStep.iHome) {
        sTemp = sTemp.sprintf("%s Step.iHome=%02d" , __FUNC__ , Step.iHome );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iHome = Step.iHome ;

    //if(Stat.bReqStop) {
    //    Step.iHome = 0;
    //    return true ;
    //}

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: //MT_Reset(miULD_ZClmp);
                 //MT_Reset(miULD_YClmp);

                 //MT_SetServo(miULD_ZClmp,true);
                 //MT_SetServo(miULD_YClmp,true);
                 //MoveActr(aiULD_Cmp,ccBwd);
                 PSB.MoveActr(aiPSB_PshrFwBw,ccBwd);
                 IO_SetY(yULD_MgzInAC , false);
                 IO_SetY(yULD_MgzOutAC, false);
                 Step.iHome++;
                 return false ;

        case 11: //if(!MoveActr(aiULD_Cmp,ccBwd)) return false ;
                 if(!PSB.MoveActr(aiPSB_PshrFwBw,ccBwd)) return false ;



                 MT_SetHomeEnd(miULD_ZClmp , true) ;
                 //MT_ClearPos(miULD_ZClmp);
                 MT_GoIncMan(miULD_ZClmp,-10); //12mm
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetStop(miULD_ZClmp))return false;
                 MT_DoHome(miULD_YClmp);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miULD_YClmp))return false ;
                 MT_DoHome(miULD_ZClmp);
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miULD_ZClmp))return false ;
                 MT_GoAbsMan(miULD_YClmp, GetMotrPos(miULD_YClmp,piULD_YClmpWait));
                 MT_GoAbsMan(miULD_ZClmp, GetMotrPos(miULD_ZClmp,piULD_ZClmpWait));
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miULD_YClmp, GetMotrPos(miULD_YClmp,piULD_YClmpWait)))return false ;
                 if(!MT_GoAbsMan(miULD_ZClmp, GetMotrPos(miULD_ZClmp,piULD_ZClmpWait)))return false ;
                 Step.iHome = 0;
                 return true ;

    }
}

String CUnLoader::GetNextStrpLotNo()
{
    if(!DM.ARAY[riPSB].CheckAllStat(csNone)) return  DM.ARAY[riPSB].GetLotNo() ;
    if(!DM.ARAY[riVSN].CheckAllStat(csNone)) return  DM.ARAY[riVSN].GetLotNo() ;
    if(!DM.ARAY[riLSR].CheckAllStat(csNone)) return  DM.ARAY[riLSR].GetLotNo() ;
    if(!DM.ARAY[riPRB].CheckAllStat(csNone)) return  DM.ARAY[riPRB].GetLotNo() ;

    return DM.ARAY[riULD].GetLotNo() ;
}

String CUnLoader::GetNextStrpID()
{
    if(!DM.ARAY[riPSB].CheckAllStat(csNone)) return  DM.ARAY[riPSB].GetID() ;
    if(!DM.ARAY[riVSN].CheckAllStat(csNone)) return  DM.ARAY[riVSN].GetID() ;
    if(!DM.ARAY[riLSR].CheckAllStat(csNone)) return  DM.ARAY[riLSR].GetID() ;
    if(!DM.ARAY[riPRB].CheckAllStat(csNone)) return  DM.ARAY[riPRB].GetID() ;

    return DM.ARAY[riULD].GetID() ;
}


bool CUnLoader::Autorun (void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isExstStrip   = !DM.ARAY[riLDR].CheckAllStat(csNone) || !DM.ARAY[riPRB].CheckAllStat(csNone) || //LDR의 메거진 카운트는 일부러 넣지 않는다. 확정이 있어야 ULD MGZ 집게끔.
                             !DM.ARAY[riLSR].CheckAllStat(csNone) || !DM.ARAY[riVSN].CheckAllStat(csNone) ||
                             !DM.ARAY[riPSB].CheckAllStat(csNone) ;

        bool isLotChanged =  DM.ARAY[riULD].GetLotNo()!="" &&
                             DM.ARAY[riULD].GetLotNo() != GetNextStrpLotNo() &&
                            !DM.ARAY[riULD].CheckAllStat(csEmpty);
        bool isMgzChanged =  DM.ARAY[riULD].GetID().ToIntDef(0)/100 != GetNextStrpID().ToIntDef(0)/100 &&
                            !DM.ARAY[riULD].CheckAllStat(csEmpty);

        bool isZWorkPos    = MT_CmprPos(miULD_ZClmp ,GetMotrPos(miULD_ZClmp , piULD_ZClmpWork)) ;

        bool isCycleSupply = DM.ARAY[riULD].CheckAllStat(csNone ) && isExstStrip && !IO_GetX(xULD_MgzIn) && !Stat.bChckSplyEmpty ;
        bool isCyclePick   = DM.ARAY[riULD].CheckAllStat(csNone ) && (IO_GetX(xULD_MgzIn) ||  OM.MstOptn.bDryRun ) &&
                             isExstStrip ;
        bool isCycleWork   = DM.ARAY[riULD].GetCntStat  (csEmpty) && !isZWorkPos ;

        bool isCyclePlace  = !DM.ARAY[riULD].CheckAllStat(csNone ) &&
                             (DM.ARAY[riULD].CheckAllStat(csWork ) ||
                              isLotChanged ||
                              isMgzChanged ||
                             (!isExstStrip && LDR.GetStat().bWorkEnd));



        bool isCycleEnd    = DM.ARAY[riULD].CheckAllStat(csNone ) && !isExstStrip && LDR.GetStat().bWorkEnd;//  isPreEqpEnd  ;

        Stat.bReadyMgz = isZWorkPos ;

        //모르는 스트립에러.
        if(  DM.ARAY[riULD].CheckAllStat(csNone) && (  IO_GetX(xULD_MgzDetect1 ) ||  IO_GetX(xULD_MgzDetect2 ))                       )
            EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riULD].GetName().c_str()) ;//EM_SetErr(eiULD_MgzUnknown) ;
        //카세트 사라짐.
        if( !DM.ARAY[riULD].CheckAllStat(csNone) && ( !IO_GetX(xULD_MgzDetect1 ) && !IO_GetX(xULD_MgzDetect2 ))&& !OM.MstOptn.bDryRun )
            EM_SetErrMsg(eiPKG_Dispr  , DM.ARAY[riULD].GetName().c_str()) ;//EM_SetErr(eiULD_MgzDispr  ) ; //sun bDryRun 옵션 추가.
        //카세트 OUT 매거진 풀

        if(  DM.ARAY[riULD].CheckAllStat(csNone) && !IO_GetX(xULD_MgzIn) && Stat.bChckSplyEmpty && (DM.ARAY[riPSB].GetCntExist()) && !OM.MstOptn.bDryRun ) EM_SetErr(eiULD_NeedMgz  ) ;


        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSupply) {Trace(m_sPartName.c_str(),"CycleSupply Stt"); Step.iSeq = scSupply ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePick  ) {Trace(m_sPartName.c_str(),"CyclePick   Stt"); Step.iSeq = scPick   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePlace ) {Trace(m_sPartName.c_str(),"CyclePlace  Stt"); Step.iSeq = scPlace  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                       return false ;
        case scSupply: if(CycleSupply()){ Trace(m_sPartName.c_str(),"CycleSupply End");Step.iSeq = scIdle ;} return false ;
        case scPick  : if(CyclePick  ()){ Trace(m_sPartName.c_str(),"CyclePick   End");Step.iSeq = scIdle ;} return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");Step.iSeq = scIdle ;} return false ;
        case scPlace : if(CyclePlace ()){ Trace(m_sPartName.c_str(),"CyclePlace  End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CUnLoader::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CUnLoader::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CUnLoader::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO , m_sPartName.c_str()); //EM_SetErr(eiULD_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {                                                   
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: PSB.MoveActr(aiPSB_PshrFwBw,ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!PSB.MoveActr(aiPSB_PshrFwBw,ccBwd)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CUnLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiULD_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: IO_SetY(yULD_MgzInAC ,false);
                 IO_SetY(yULD_MgzOutAC,false);
                 PSB.MoveActr(aiPSB_PshrFwBw,ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!PSB.MoveActr(aiPSB_PshrFwBw,ccBwd)) return false ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}

//One Cycle.
bool CUnLoader::CycleSupply(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 7000 )) {
        //EM_SetErr(eiULD_CycleTO);
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()) ;
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    bool r1,r2 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yULD_MgzInAC,true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  11: r1 = m_tmDelay.OnDelay(true , 6000) ;
                  r2 = IO_GetX(xULD_MgzIn)           ;
                  if(!r1&&!r2) return false ;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  12: if(!m_tmDelay.OnDelay(true ,  200)) return false ;
                  IO_SetY(yULD_MgzInAC,false);
                  if(!IO_GetX(xULD_MgzIn))Stat.bChckSplyEmpty = true ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CUnLoader::CyclePick()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        //EM_SetErr(eiULD_CycleTO);
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()) ;
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiULD_ClmpUpDn     , ccFwd );
                  MoveMotr(miULD_YClmp   , piULD_YClmpWork) ;
                  IO_SetY(yULD_MgzInAC,true);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiULD_ClmpUpDn    , ccFwd  )) return false ;
                  if(!MoveMotr(miULD_YClmp   , piULD_YClmpWork)) return false ;
                  MoveMotr(miULD_ZClmp , piULD_ZClmpPickFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_ZClmp , piULD_ZClmpPickFwd))return false ;
                  MoveMotr(miULD_YClmp , piULD_YClmpPick);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miULD_YClmp , piULD_YClmpPick))return false ;
                  if(!IO_GetX(xULD_MgzIn) && !OM.MstOptn.bDryRun)return false ;
                  IO_SetY(yULD_MgzInAC,false);
                  MoveMotr(miULD_ZClmp , piULD_ZClmpClampOn) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miULD_ZClmp , piULD_ZClmpClampOn)) return false ;
                  MoveActr(aiULD_ClmpUpDn , ccBwd);
                  DM.ARAY[riULD].SetStat(csEmpty) ;
                  DM.ARAY[riULD].SetLotNo("");
                  Stat.bChckSplyEmpty = false ;
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiULD_ClmpUpDn , ccBwd)) return false ;
                  //IO_SetY(yULD_MgzInAC,true);
                  MoveMotr(miULD_ZClmp , piULD_ZClmpPickBwd) ;
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miULD_ZClmp , piULD_ZClmpPickBwd)) return false ;
                  MoveMotr(miULD_YClmp , piULD_YClmpWork) ;
                  IO_SetY(yULD_MgzInAC,true);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miULD_YClmp , piULD_YClmpWork)) return false ;
                  //IO_SetY(yULD_MgzInAC,false);
                  MoveMotr(miULD_ZClmp , piULD_ZClmpWorkStart);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveMotr(miULD_ZClmp , piULD_ZClmpWorkStart)) return false ;
                  IO_SetY(yULD_MgzInAC,false);
                  Step.iCycle++;
                  return false;

        case  19: if(IO_GetY(yULD_MgzInAC)) return false;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CUnLoader::CycleWork ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        //EM_SetErr(eiULD_CycleTO);
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()) ;
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miULD_YClmp , piULD_YClmpWork) ;
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_YClmp , piULD_YClmpWork)) return false ;
                  MoveMotr(miULD_ZClmp , piULD_ZClmpWork);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_ZClmp , piULD_ZClmpWork)) return false ;
                  Step.iCycle = 0;
                  return true ;
    }

}

bool CUnLoader::CyclePlace ()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        //EM_SetErr(eiULD_CycleTO);
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str()) ;
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        IO_SetY(yULD_MgzOutAC,false);
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miULD_YClmp , piULD_YClmpWork) ;
                  IO_SetY(yULD_MgzOutAC,true);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miULD_YClmp , piULD_YClmpWork)) return false ;
                  MoveMotr(miULD_ZClmp , piULD_ZClmpPlaceFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_ZClmp , piULD_ZClmpPlaceFwd))return false ;

                  IO_SetY(yULD_MgzOutAC,false);

                  MoveMotr(miULD_YClmp , piULD_YClmpPlace) ;
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miULD_YClmp , piULD_YClmpPlace))return false ;
                  MoveMotr(miULD_ZClmp , piULD_ZClmpClampOff) ;
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miULD_ZClmp , piULD_ZClmpClampOff)) return false ;
                  MoveActr(aiULD_ClmpUpDn , ccFwd);
                  DM.ARAY[riULD].ClearMap();
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiULD_ClmpUpDn , ccFwd)) return false ;
                  IO_SetY(yULD_MgzOutAC,true);
                  MoveMotr(miULD_ZClmp , piULD_ZClmpPlaceBwd) ;

                  //DM.ARAY[riULD].SetLotNo("");
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miULD_ZClmp , piULD_ZClmpPlaceBwd)) return false ;
                  MoveMotr(miULD_YClmp , piULD_YClmpWork) ;
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveMotr(miULD_YClmp , piULD_YClmpWork)) return false ;
                  MoveMotr(miULD_ZClmp , piULD_ZClmpPickFwd);
                  Step.iCycle++;
                  return false ;

        case  18: if(!MoveMotr(miULD_ZClmp , piULD_ZClmpPickFwd)) return false ;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

                  /////////////////
        case  19: if(m_tmDelay.OnDelay(true , 2000)){
                      if(IO_GetX(xULD_MgzOutFull)) {
                          IO_SetY(yULD_MgzOutAC,false);
                          EM_SetErr(eiULD_FullMgz);
                          Step.iCycle = 0 ;
                          return true ;
                      }
                  }
                  if(IO_GetX(xULD_MgzOutFull)) return false ;

                  m_tmDelay.Clear();

                  Step.iCycle ++;
                  return false;

        case  20: if(!m_tmDelay.OnDelay(true,3000)) return false;
                  IO_SetY(yULD_MgzOutAC,false);
                  if(IO_GetY(yULD_MgzOutAC)) return false;
                  Step.iCycle = 0;
                  return true ;
    }
}

void CUnLoader::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CUnLoader::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CUnLoader::CheckStop()
{
    if(!MT_GetStop(miULD_YClmp)) return false ;
    if(!MT_GetStop(miULD_ZClmp)) return false ;

    if(!AT_Done(aiULD_ClmpUpDn   )) return false ;

    return true ;
}

void CUnLoader::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\Unloader.INI";

    if ( _bLoad ) {
      //  UserINI.Load(sPath, "Member" , "m_iMgzCnt" , m_iMgzCnt );
    }
    else {
      //  UserINI.ClearFile(sPath) ;
      //  UserINI.Save(sPath, "Member" , "m_iMgzCnt" , m_iMgzCnt );
    }
}
//---------------------------------------------------------------------------
