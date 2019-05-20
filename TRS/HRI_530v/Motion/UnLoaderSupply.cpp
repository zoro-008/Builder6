//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "UnLoader.h"
#include "UnLoaderSupply.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "LotUnit.h"
//#include "LotUnit.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CUnLoaderSupply USP;

CUnLoaderSupply::CUnLoaderSupply(void)
{

}

CUnLoaderSupply::~CUnLoaderSupply (void)
{

}

void CUnLoaderSupply::Init()
{
    m_sPartName = "UnLoaderSupply" ;
    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

    m_iTrayCnt    = 0     ;


}

void CUnLoaderSupply::Close()
{
    Load(false);
}

void CUnLoaderSupply::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CUnLoaderSupply::FindChip(int &r , int &c )
{
    //r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
    r=0 ; c=0;
    return false ;

}

double CUnLoaderSupply::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    if(_iMotr == miUSP_ZElev){
        switch(_iPstnId) {
            default                   : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piUSP_ZElevWait      : dPos = PM.GetValue (_iMotr , pvUSP_ZElevWait      ); break ;
            case piUSP_ZElevTop       : dPos = PM.GetValue (_iMotr , pvUSP_ZElevTop       ); break ;
            case piUSP_ZElevSperate   : dPos = PM.GetValue (_iMotr , pvUSP_ZElevSperate   ); break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CUnLoaderSupply::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CUnLoaderSupply::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CUnLoaderSupply::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CUnLoaderSupply::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: MoveActr(aiUSP_GuidFwBw, ccFwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiUSP_GuidFwBw, ccFwd)) return false;
                 MoveMotr(miUSP_ZElev , piUSP_ZElevWait);
                 Step.iToStart++;
                 return false ;

        case 12: if(!MoveMotr(miUSP_ZElev , piUSP_ZElevWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CUnLoaderSupply::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: MoveActr(aiUSP_GuidFwBw, ccFwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiUSP_GuidFwBw, ccFwd)) return false;
                 MoveMotr(miUSP_ZElev , piUSP_ZElevWait);
                 Step.iToStop++;
                 return false ;

        case 12: if(!MoveMotr(miUSP_ZElev , piUSP_ZElevWait)) return false ;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CUnLoaderSupply::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iSeq=%02d" , __FUNC__ , Step.iSeq );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    static int iCycleSttTime = 0;                                                          
                                                                                           
    //Check Error & Decide Step.                                                        //riPRU   프리언로더
    if (Step.iSeq == 0) {                                                               //riULD       언로더
        if (Stat.bReqStop)return false ;                                                //riPSU 포스트언로더

                                                                                   //bExistChip
        bool bExistChip = DM.ARAY[riSRT].GetCntStat(csGood) ;
        bool isCycleSupply =  DM.ARAY[riPRU].CheckAllStat(csNone) &&
                              DM.ARAY[riULD].CheckAllStat(csNone) &&
                              IO_GetX(xUSP_Tray5Sply)             &&
                             !IO_GetX(xULD_Tray3Work)             && //!IO_GetX(xULD_Tray3Work) 없으면 PreZoneTray 아작남.
                            (!LOT.GetRqstLotEnd()||bExistChip)    ;  //랏엔드시에는 소트에 칩이 있을때에만 공급.


        bool isCycleEnd    =  DM.ARAY[riPRU].CheckAllStat(csNone ) && LOT.GetRqstLotEnd();


        //모르는 스트립에러.
        if(  DM.ARAY[riPRU].CheckAllStat(csNone) && IO_GetX(xUSP_Tray4)){
            EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riPRU].GetName().c_str());
        }
        //카세트 사라짐.
        if(ULD.GetSeqStep() != CUnLoader::scIn){
            if( !DM.ARAY[riPRU].CheckAllStat(csNone) &&!IO_GetX(xUSP_Tray4))
                EM_SetErrMsg(eiPKG_Dispr  , DM.ARAY[riPRU].GetName().c_str());
        }
        //카세트 OUT 매거진 풀

                                   //sunsun 랏엔드시에 ULD가 않차져 있을때 배출 되는 패턴.
                                   //배출 되더라도 트래이공급 에러 안뜨게.

        if (!IO_GetX(xUSP_Tray5Sply)&&
             DM.ARAY[riULD].CheckAllStat(csNone) &&
             DM.ARAY[riPRU].CheckAllStat(csNone) &&
              (!LOT.GetRqstLotEnd()||bExistChip)){

            EM_SetErr(eiUSP_NeedTray) ;
        }

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleSupply) {Trace(m_sPartName.c_str(),"CycleSupply Stt"); Step.iSeq = scSupply ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                      Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                             return false ;
        case scSupply: if(CycleSupply()){ Trace(m_sPartName.c_str(),"CycleSupply End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CUnLoaderSupply::CycleHome()     //sun DLL direct access.
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

        case 10: MoveActr(aiUSP_GuidFwBw, ccFwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(aiUSP_GuidFwBw, ccFwd)) return false ;
                 MT_DoHome(miUSP_ZElev);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miUSP_ZElev)) return false ;
                 MT_GoAbsMan(miUSP_ZElev , PM.GetValue(miUSP_ZElev , pvUSP_ZElevWait ));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miUSP_ZElev , PM.GetValue(miUSP_ZElev , pvUSP_ZElevWait ))) return false ;
                 Step.iHome = 0;
                 return true ;

    }
}




//One Cycle.
bool CUnLoaderSupply::CycleSupply(void)
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

        case  10: MoveMotr(miUSP_ZElev , piUSP_ZElevTop);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveMotr(miUSP_ZElev , piUSP_ZElevTop)) return false ;
                  MoveActr(aiUSP_GuidFwBw , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveActr(aiUSP_GuidFwBw , ccBwd)) return false ;
                  MoveMotr(miUSP_ZElev,piUSP_ZElevSperate);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miUSP_ZElev,piUSP_ZElevSperate)) return false ;
                  MoveActr(aiUSP_GuidFwBw , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveActr(aiUSP_GuidFwBw , ccFwd)) return false ;
                  MoveMotr(miUSP_ZElev,piUSP_ZElevWait);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miUSP_ZElev,piUSP_ZElevWait)) return false ;
                  if(!IO_GetX(xUSP_Tray4)) return false ;
                  m_iTrayCnt++;
                  DM.ARAY[riPRU].SetStat(csEmpty);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

//---------------------------------------------------------------------------
bool CUnLoaderSupply::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bExistPRU = !DM.ARAY[riPRU].CheckAllStat(csNone) ;

    if(_iMotr == miUSP_ZElev){
        switch(_iPstnId) {
            default                   :                 { sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion" ; bRet = false ;} break ;
            case piUSP_ZElevWait      :                                                                                                break ;
            case piUSP_ZElevTop       : if(bExistPRU )  { sMsg = AnsiString(DM.ARAY[riPRU].GetName())+" is Exist"    ; bRet = false ;} break ;
            case piUSP_ZElevSperate   :                                                                                                break ;

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
bool CUnLoaderSupply::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool bExistSply = IO_GetX(xUSP_Tray5Sply) ;
    bool bUSP_ZTop  = PM.GetValue(miUSP_ZElev , pvUSP_ZElevTop);


    if(_iActr == aiUSP_GuidFwBw){
        if(_bFwd == ccBwd) {
            if(!bExistSply /*|| !bLSP_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
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

bool CUnLoaderSupply::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CUnLoaderSupply::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CUnLoaderSupply::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CUnLoaderSupply::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CUnLoaderSupply::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CUnLoaderSupply::CheckStop()
{
    if(!MT_GetStop(miUSP_ZElev)) return false ;

    if(!AT_Done(aiUSP_GuidFwBw)) return false ;

    return true ;
}

void CUnLoaderSupply::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_iTrayCnt" , m_iTrayCnt );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iTrayCnt" , m_iTrayCnt );
    }
}





