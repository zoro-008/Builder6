//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "UnLoader.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//#include "LotUnit.h"
#include "LotUnit.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.
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
    InitCycleTime();


}

void CUnLoader::Close()
{
    Load(false);
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
    //r = DM.ARAY[riLDR].FindLastRow(csUnkwn) ;
    if(DM.ARAY[riULD].FindLastRowFrstCol(csEmpty , r,c)){
        return true ;
    }
    else {
        r=0 ;
        c=0 ;
        return false ;
    }

}

double CUnLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    int    iR,iC ;
    FindChip(iR ,iC);

    double dWorkPos ;
    dWorkPos = PM.GetValue(miULD_YIndx , pvULD_YIndxWorkStt) ;
    dWorkPos = dWorkPos + (OM.DevInfo.dULDRowPitch * (DM.ARAY[riULD].GetMaxRow() - iR -1)) ;

    if(_iMotr == miULD_YIndx){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piULD_YIndxWait     : dPos = PM.GetValue (_iMotr , pvULD_YIndxWait      ); break ;
            case piULD_YIndxGet      : dPos = PM.GetValue (_iMotr , pvULD_YIndxGet       ); break ;
            case piULD_YIndxWorkStt  : dPos = PM.GetValue (_iMotr , pvULD_YIndxWorkStt   ); break ;
            case piULD_YIndxOut      : dPos = PM.GetValue (_iMotr , pvULD_YIndxOut       ); break ;
            case piULD_YIndxWork     : dPos = dWorkPos                                    ; break ;

        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CUnLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CUnLoader::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CUnLoader::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CUnLoader::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
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

        case 10: if(!DM.ARAY[riULD].CheckAllStat(csNone)){ //Ư���� �� ������.'
                     Step.iToStart=20 ;
                 }
                 else {
                     Step.iToStart=30 ;
                 }
                 return false ;


        //Work���� ���� ������.
        case 20: MoveActr(aiULD_IndxUpDn , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 21: if(!MoveActr(aiULD_IndxUpDn , ccBwd)) return false ;
                 //MoveMotr(miULD_YIndx , piULD_YIndxWork );
                 MT_GoAbsMan(miULD_YIndx , m_dLastIdxPos);

                 Step.iToStart++;
                 return false ;

        case 22: if(!MT_GetStopInpos(miULD_YIndx)) return false;
                 MT_GoIncMan(miULD_YIndx , -1.0);
                 Step.iToStart++;
                 return false ;

        case 23: if(!MT_GetStopInpos(miULD_YIndx)) return false;
                 MoveActr(aiULD_IndxUpDn , ccFwd);
                 Step.iToStart++;
                 return false ;

        case 24: if(!MoveActr(aiULD_IndxUpDn , ccFwd)) return false ;
                 MT_GoIncMan(miULD_YIndx , 1.0);

                 Step.iToStart++;
                 return false ;

        case 25: if(!MT_GetStopInpos(miULD_YIndx)) return false;
                 Step.iToStart=0;
                 return true ;


        //��ũ ���� ���� ����.
        case 30: MoveActr(aiULD_IndxUpDn , ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 31: if(!MoveActr(aiULD_IndxUpDn , ccBwd)) return false;
                 MoveMotr(miULD_YIndx , piULD_YIndxWait);
                 Step.iToStart++;
                 return false ;

        case 32: if(!MoveMotr(miULD_YIndx , piULD_YIndxWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CUnLoader::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
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

        case 10: SetLastCmd();
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CUnLoader::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
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

        bool bExistWork  = !DM.ARAY[riSRT].CheckAllStat(csEmpty) ||
                           !DM.ARAY[riZIG].CheckAllStat(csEmpty) ||
                           !DM.ARAY[riSTF].CheckAllStat(csEmpty) ||
                           !DM.ARAY[riSTL].CheckAllStat(csEmpty) ||
                           !DM.ARAY[riSTB].CheckAllStat(csEmpty) ||
                           !DM.ARAY[riSTR].CheckAllStat(csEmpty) ||
                            DM.ARAY[riLDR].GetCntStat  (csUnkwn) ||
                            DM.ARAY[riPRL].GetCntStat  (csUnkwn) ||
                           (IO_GetX(xLSP_Tray5Sply) && !LOT.GetRqstLotEnd()) ;






        bool isCycleIn   = !DM.ARAY[riPRU].CheckAllStat(csNone ) && DM.ARAY[riULD].CheckAllStat(csNone );
        bool isCycleWork = !DM.ARAY[riULD].CheckAllStat(csNone ) && DM.ARAY[riULD].GetCntStat  (csEmpty)&& !GetInWorkPos();
        bool isCycleOut  =(!DM.ARAY[riULD].CheckAllStat(csNone ) &&!DM.ARAY[riULD].GetCntStat  (csEmpty)) ||
                          (!DM.ARAY[riULD].CheckAllStat(csNone ) && LOT.GetRqstLotEnd() &&  !bExistWork);
        bool isCycleEnd  =  DM.ARAY[riPRU].CheckAllStat(csNone ) && DM.ARAY[riULD].CheckAllStat(csNone );


        //�𸣴� ��Ʈ������.
        if(  DM.ARAY[riULD].CheckAllStat(csNone) && IO_GetX(xULD_Tray3Work)) {
            EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riULD].GetName().c_str());
        }
        //ī��Ʈ �����.
        if( !DM.ARAY[riULD].CheckAllStat(csNone) &&!IO_GetX(xULD_Tray3Work)) {
            EM_SetErrMsg(eiPKG_Dispr  , DM.ARAY[riULD].GetName().c_str());
        }
        //ī��Ʈ OUT �Ű��� Ǯ

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleIn    ) {Trace(m_sPartName.c_str(),"CycleIn     Stt"); Step.iSeq = scIn     ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleOut   ) {Trace(m_sPartName.c_str(),"CycleOut    Stt"); Step.iSeq = scOut    ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                      Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                             return false ;
        case scIn    : if(CycleIn    ()){ Trace(m_sPartName.c_str(),"CycleIn     End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scOut   : if(CycleOut   ()){ Trace(m_sPartName.c_str(),"CycleOut    End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CUnLoader::CycleHome()     //sun DLL direct access.
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

        case 10: MoveActr(aiULD_IndxUpDn, ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(aiULD_IndxUpDn, ccBwd)) return false ;
                 MT_DoHome(miULD_YIndx);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miULD_YIndx)) return false ;
                 MT_GoAbsMan(miULD_YIndx , PM.GetValue(miULD_YIndx , pvULD_YIndxWait ));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miULD_YIndx , PM.GetValue(miULD_YIndx , pvULD_YIndxWait ))) return false ;
                 SetLastCmd();
                 Step.iHome = 0;
                 return true ;

    }
}




//One Cycle.
bool CUnLoader::CycleIn(void)
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

        case  10: MoveActr(aiULD_IndxUpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiULD_IndxUpDn , ccBwd)) return false ;
                  MoveMotr(miULD_YIndx , piULD_YIndxGet);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miULD_YIndx , piULD_YIndxGet)) return false ;
                  MoveActr(aiULD_IndxUpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiULD_IndxUpDn , ccFwd)) return false ;
                  MoveMotr(miULD_YIndx,piULD_YIndxWorkStt);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miULD_YIndx,piULD_YIndxWorkStt)) return false ;
                  DM.ShiftArrayData(riPRU , riULD);
                  Step.iCycle = 0 ;
                  return true ;
    }
}

//One Cycle.
bool CUnLoader::CycleWork(void)
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

        case  10: MoveActr(aiULD_IndxUpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiULD_IndxUpDn , ccFwd)) return false ;
                  MoveMotr(miULD_YIndx , piULD_YIndxWork);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miULD_YIndx , piULD_YIndxWork)) return false ;
                  Step.iCycle = 0 ;
                  return true ;

    }
}

bool CUnLoader::CycleOut(void)
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

        case  10: MoveActr(aiULD_IndxUpDn , ccFwd);
                  MoveActr(aiUST_StckUpDn , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiULD_IndxUpDn , ccFwd)) return false ;
                  if(!MoveActr(aiUST_StckUpDn , ccBwd)) return false ;
                  MoveMotr(miULD_YIndx , piULD_YIndxOut);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miULD_YIndx , piULD_YIndxOut)) return false ;
                  DM.ShiftArrayData(riULD , riPSU);
                  MoveActr(aiULD_IndxUpDn , ccBwd);
                  MoveMotr(miULD_YIndx , piULD_YIndxGet);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiULD_IndxUpDn , ccBwd)) return false ;
                  if(!MoveMotr(miULD_YIndx , piULD_YIndxGet)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

//---------------------------------------------------------------------------
bool CUnLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bExistPRL   = DM.ARAY[riPRU].GetCntExist() ;
    bool bExistPSL   = DM.ARAY[riPSU].GetCntExist() ;
    bool bExistULD   = DM.ARAY[riULD].GetCntExist() ;
    bool bIndxFwd    = !AT_Complete(aiULD_IndxUpDn , ccBwd);

    bool bSRT_ZDn    = MT_GetTrgPos(miSRT_ZPckr) > PM.GetValue(miSRT_ZPckr,pvSRT_ZPckrMove) + 1.0 ||
                       MT_GetCmdPos(miSRT_ZPckr) > PM.GetValue(miSRT_ZPckr,pvSRT_ZPckrMove) + 1.0 ;


    bool bUnderWorkPos = PM.GetValue(miULD_YIndx , pvULD_YIndxWorkStt)+10.0 >= MT_GetCmdPos(miULD_YIndx) ;

    if(_iMotr == miULD_YIndx){
        if(bSRT_ZDn){ sMsg = " Tool is Down"    ; bRet = false ;}
        if(bIndxFwd) {
            switch(_iPstnId) {
                default                   :                                  { sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion" ; bRet = false ;} break ;
                case piULD_YIndxWait      : if(bUnderWorkPos && bExistPRL )  { sMsg = AnsiString(DM.ARAY[riPRU].GetName())+" is Exist"    ; bRet = false ;} break ;
                case piULD_YIndxGet       : if(bUnderWorkPos && bExistPRL )  { sMsg = AnsiString(DM.ARAY[riPRU].GetName())+" is Exist"    ; bRet = false ;} break ;
                case piULD_YIndxWorkStt   :                                                                                                                 break ;
                case piULD_YIndxOut       : if(bExistPSL     && bExistULD )  { sMsg = AnsiString(DM.ARAY[riPSU].GetName())+" is Exist"    ; bRet = false ;} break ;
                case piULD_YIndxWork      :                                                                                                                 break ;

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
bool CUnLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiULD_IndxUpDn){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bLSP_ZTop*/) {sMsg = AnsiString("Tray ���� ������!"); bRet = false ;}
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
                                        //sunsun SORT_Y ���׹��� �ݴ�. ��δ��ε��� ���ٿ� �Ǹ��� ���� �ݴ�
                                        //��δ� ���۷����� �Ǹ��� ��ư ����.
bool CUnLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

void CUnLoader::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // ���͸� �����϶� ���� �Լ�.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CUnLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CUnLoader::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(miULD_YIndx);
    return ; //����Ʈ�� �ʿ� ����.

}

bool CUnLoader::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miULD_YIndx)) return true  ; //����Ʈ�� �ʿ� ����.
    else                                           return false ;

}

bool CUnLoader::CheckStop()
{
    if(!MT_GetStop(miULD_YIndx)) return false ;

    if(!AT_Done(aiULD_IndxUpDn)) return false ;

    return true ;
}

void CUnLoader::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";

    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
}

bool CUnLoader::GetInWorkPos()
{
    //return GetMotrPos(miULD_YIndx , piULD_YIndxWork) == MT_GetCmdPos(miULD_YIndx) && MT_GetStopInpos(miULD_YIndx);
    //���� �̻��� dMotrPos == dCmdPos ���� ���ϸ� ���� ���ε� ���� ���� false�� �ִ�.

    double dMotrPos   = GetMotrPos(miULD_YIndx , piULD_YIndxWork) ;
    double dCmdPos    = MT_GetCmdPos(miULD_YIndx);
    bool   bStopInpos = MT_GetStopInpos(miULD_YIndx);
    return dMotrPos-0.1 <= dCmdPos && dCmdPos <= dMotrPos+0.1 && bStopInpos ;

}



