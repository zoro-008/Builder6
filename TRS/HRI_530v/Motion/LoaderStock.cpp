//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "LoaderStock.h"
#include "Loader.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CLoaderStock LST;

CLoaderStock::CLoaderStock(void)
{

}

CLoaderStock::~CLoaderStock (void)
{

}

void CLoaderStock::Init()
{
    m_sPartName = "LoaderStock " ;
    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();


}

void CLoaderStock::Close()
{
    Load(false);
}

void CLoaderStock::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CLoaderStock::FindChip(int &r , int &c )
{
    //r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
    r=0 ; c=0;
    return false ;

}

double CLoaderStock::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    /*
    if(_iMotr == miLSP_ZElev){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piLDR_YIndxWait     : dPos = PM.GetValue (_iMotr , pvLDR_YIndxWait      ); break ;
            case piLDR_YIndxGet      : dPos = PM.GetValue (_iMotr , pvLDR_YIndxGet       ); break ;
            case piLDR_YIndxWorkStt  : dPos = PM.GetValue (_iMotr , pvLDR_YIndxWorkStt   ); break ;
            case piLDR_YIndxOut      : dPos = PM.GetValue (_iMotr , pvLDR_YIndxOut       ); break ;
            case piLDR_YIndxWork     : dPos = dWorkPos                                    ; break ;

        }
    }

    else {*/
        dPos = MT_GetCmdPos(_iMotr);
    //}

    return dPos ;
}

void CLoaderStock::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CLoaderStock::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CLoaderStock::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLoaderStock::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStartTO);

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

        case 10: MoveActr(aiLST_StckUpDn , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiLST_StckUpDn , ccBwd)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoaderStock::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiLDR_ToStopTO);

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

        case 10: MoveActr(aiLST_StckUpDn , ccBwd);
                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveActr(aiLST_StckUpDn , ccBwd)) return false ;


                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CLoaderStock::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iSeq=%02d" , __FUNC__ , Step.iSeq );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    static int iCycleSttTime = 0;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isCycleStock = !DM.ARAY[riPSL].CheckAllStat(csNone ) ;
        bool isCycleEnd   =  DM.ARAY[riPSL].CheckAllStat(csNone ) ;


        //모르는 스트립에러.
        if(LDR.GetSeqStep() != CLoader::scOut){
            if(  DM.ARAY[riPSL].CheckAllStat(csNone) && IO_GetX(xLST_Tray1)) EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riPSL].GetName().c_str());
        }
        //카세트 사라짐.
        if( !DM.ARAY[riPSL].CheckAllStat(csNone) &&!IO_GetX(xLST_Tray1)) EM_SetErrMsg(eiPKG_Dispr  , DM.ARAY[riPSL].GetName().c_str());
        //카세트 OUT 매거진 풀

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleStock ) {Trace(m_sPartName.c_str(),"CycleStock  Stt"); Step.iSeq = scStock  ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                      Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                             return false ;
        case scStock : if(CycleStock ()){ Trace(m_sPartName.c_str(),"CycleStock  End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CLoaderStock::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_HomeTo,m_sPartName.c_str());
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

    if(Stat.bReqStop) {
        //Step.iHome = 0;
        //return true ;
    }

    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MoveActr(aiLST_StckUpDn , ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(aiLST_StckUpDn , ccBwd)) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}




//One Cycle.
bool CLoaderStock::CycleStock(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
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

        case  10: MoveActr(aiLST_StckUpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiLST_StckUpDn , ccFwd)) return false ;
                  MoveActr(aiLST_StckUpDn , ccBwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiLST_StckUpDn , ccBwd)) return false ;
                  MoveActr(aiLST_StckUpDn , ccBwd);
                  DM.ARAY[riPSL].SetStat(csNone);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

//---------------------------------------------------------------------------
bool CLoaderStock::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    /*
    if(_iMotr == miLDR_YIndx){
        if(bIndxFwd) {
            switch(_iPstnId) {
                default                 :                                  { sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion" ; bRet = false ;} break ;
                case piLDR_YIndxWait    : if(bUnderWorkPos && bExistPRL )  { sMsg = AnsiString(DM.ARAY[riPRL].GetName())+" is Exist"    ; bRet = false ;} break ;
                case piLDR_YIndxGet     : if(bUnderWorkPos && bExistPRL )  { sMsg = AnsiString(DM.ARAY[riPRL].GetName())+" is Exist"    ; bRet = false ;} break ;
                case piLDR_YIndxWorkStt :                                                                                                                 break ;
                case piLDR_YIndxOut     : if(bExistPSL     && bExistLDR )  { sMsg = AnsiString(DM.ARAY[riPSL].GetName())+" is Exist"    ; bRet = false ;} break ;
                case piLDR_YIndxWork    :                                                                                                                 break ;

            }
        }
    }

    else {*/
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    //}

    if(!bRet){
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CLoaderStock::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiLST_StckUpDn){
        //if(_bFwd == ccFwd) {
        //    //if(!bExistSply /*|| !bLSP_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        //}
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

bool CLoaderStock::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CLoaderStock::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CLoaderStock::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CLoaderStock::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CLoaderStock::CheckMoved()
{
    //if(m_dLastIdxPos != MT_GetCmdPos(miLDR_YIndx)) return true  ; //이파트는 필요 없다.
    //else                                           return false ;
    return false ;

}

bool CLoaderStock::CheckStop()
{
  //  if(!MT_GetStop(miLDR_YIndx)) return false ;

 //   if(!AT_Done(aiLDR_IndxUpUp)) return false ;

    return true ;
}

void CLoaderStock::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";

    if ( _bLoad ) {
        //UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        //UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
}





