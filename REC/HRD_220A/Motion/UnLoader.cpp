//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "UnLoader.h"
//#include "RunThread.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
//#include "LotUnit.h"
#include "UserIni.h"
#include "UserFile.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CUnLoader ULD_F;
CUnLoader ULD_R;

CUnLoader::CUnLoader(void)
{

}

CUnLoader::~CUnLoader (void)
{

}

void CUnLoader::Init(bool _bFront )
{
    if(_bFront){
        m_sPartName                 = "Front UnLoader"         ;
        Add.iPartId                 = piULD_F                  ;
        Add.riULDTop                = riULD_FTop               ;
        Add.riULDBtm                = riULD_FBtm               ;
        Add.miULD_ZElev             = miULD_ZElevF             ;
        Add.pvULD_ZElevWait         = pvULD_ZElevFWait         ;
        Add.pvULD_ZElevWorkStartBtm = pvULD_ZElevFWorkStartBtm ;
        Add.pvULD_ZElevWorkStartTop = pvULD_ZElevFWorkStartTop ;
        Add.piULD_ZElevWait         = piULD_ZElevFWait         ;
        Add.piULD_ZElevWorkStartBtm = piULD_ZElevFWorkStartBtm ;
        Add.piULD_ZElevWorkStartTop = piULD_ZElevFWorkStartTop ;
        Add.piULD_ZElevWork         = piULD_ZElevFWork         ;
        Add.xULD_Detect5            = xULD_FDetect5            ;

        Add.riPSI                   = riPSI_F                  ;
        Add.aiPushLtRt              = aiULD_FPushLtRt          ;
        Add.aiPushUpDn              = aiULD_FPushUpDn          ;

        Add.IDX                     = &IDX_F                   ;


    }
    else {
        m_sPartName                 = "Rear UnLoader"          ;
        Add.iPartId                 = piULD_R                  ;
        Add.riULDTop                = riULD_RTop               ;
        Add.riULDBtm                = riULD_RBtm               ;
        Add.miULD_ZElev             = miULD_ZElevR             ;
        Add.pvULD_ZElevWait         = pvULD_ZElevRWait         ;
        Add.pvULD_ZElevWorkStartBtm = pvULD_ZElevRWorkStartBtm ;
        Add.pvULD_ZElevWorkStartTop = pvULD_ZElevRWorkStartTop ;
        Add.piULD_ZElevWait         = piULD_ZElevRWait         ;
        Add.piULD_ZElevWorkStartTop = piULD_ZElevRWorkStartTop ;
        Add.piULD_ZElevWorkStartBtm = piULD_ZElevRWorkStartBtm ;
        Add.piULD_ZElevWork         = piULD_ZElevRWork         ;
        Add.xULD_Detect5            = xULD_RDetect5            ;

        Add.riPSI                   = riPSI_R                  ;
        Add.aiPushLtRt              = aiULD_RPushLtRt          ;
        Add.aiPushUpDn              = aiULD_RPushUpDn          ;

        Add.IDX                     = &IDX_R                   ;
    }


    Reset();
    Load(true);

    InitCycleName();
    InitCycleTime();

    m_sCheckSafeMsg = "" ;
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


bool CUnLoader::FindChip(int &r , EN_ARAY_ID &id)
{
    if(-1 != DM.ARAY[Add.riULDBtm].FindLastRow(csEmpty)){
        id = Add.riULDBtm ;
        r  = DM.ARAY[Add.riULDBtm].FindLastRow(csEmpty) ;
        return true ;
    }
    else if(-1 != DM.ARAY[Add.riULDTop].FindLastRow(csEmpty)){
        id = Add.riULDTop ;
        r  = DM.ARAY[Add.riULDTop].FindLastRow(csEmpty) ;
        return true ;
    }

    //
    //if(-1 != DM.ARAY[Add.riULDTop].FindFrstRow(csEmpty)){
    //    id = Add.riULDTop ;
    //    r  = DM.ARAY[Add.riULDTop].FindFrstRow(csEmpty) ;
    //    return true ;
    //}
    //else if(-1 != DM.ARAY[Add.riULDBtm].FindFrstRow(csEmpty)){
    //    id = Add.riULDBtm ;
    //    r  = DM.ARAY[Add.riULDBtm].FindFrstRow(csEmpty) ;
    //    return true ;
    //}

    return false ;
}

double CUnLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dWorkStt ; //작업해야할 메거진의 맨 아래 슬롯 위치.
    double dWork    ; //현재 작업해야할 포지션.
    int r ;
    EN_ARAY_ID riWork ;

    if(FindChip(r,riWork)){
        //if(riWork == Add.riULDTop){
        //    dWorkStt = PM.GetValue(Add.miULD_ZElev , Add.pvULD_ZElevWorkStartTop) ;
        //    dWork    = dWorkStt - r * OM.DevInfo.dMgzSlotPitch ;
        //}
        //else {
        //    dWorkStt = PM.GetValue(Add.miULD_ZElev , Add.pvULD_ZElevWorkStartBtm) ;
        //    dWork    = dWorkStt - r * OM.DevInfo.dMgzSlotPitch ;
        //}

        if(riWork == Add.riULDBtm){
            dWorkStt = PM.GetValue(Add.miULD_ZElev , Add.pvULD_ZElevWorkStartBtm) ;
            dWork    = dWorkStt + (OM.DevInfo.iMgzSlotCnt - r - 1) * OM.DevInfo.dMgzSlotPitch ;
        }
        else {
            dWorkStt = PM.GetValue(Add.miULD_ZElev , Add.pvULD_ZElevWorkStartTop) ;
            dWork    = dWorkStt + (OM.DevInfo.iMgzSlotCnt - r - 1)  * OM.DevInfo.dMgzSlotPitch ;
        }



    }

    if(_iMotr == Add.miULD_ZElev){
             if(_iPstnId ==  Add.piULD_ZElevWait        ) dPos = PM.GetValue (_iMotr , Add.pvULD_ZElevWait         );
        else if(_iPstnId ==  Add.piULD_ZElevWorkStartBtm) dPos = PM.GetValue (_iMotr , Add.pvULD_ZElevWorkStartBtm );
        else if(_iPstnId ==  Add.piULD_ZElevWorkStartTop) dPos = PM.GetValue (_iMotr , Add.pvULD_ZElevWorkStartTop );
        else if(_iPstnId ==  Add.piULD_ZElevWork        ) dPos = dWork                                              ;
        else                                              dPos = MT_GetCmdPos(_iMotr                               );
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

bool CUnLoader::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
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
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiULD_ToStartTO);

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

        case 10: //if(IO_GetX(Add.xPRI_Detect1)) {
                 //    EM_SetErrMsg(eiPRT_Crash , (m_sPartName + "보트 충돌 방지 센서 감지중").c_str());
                 //    Step.iToStart = 0 ;
                 //    return true ;
                 //}

                 MoveActr(Add.aiPushLtRt , ccBwd);
                 MoveActr(Add.aiPushUpDn , ccBwd);

                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(Add.aiPushLtRt , ccBwd)) return false ;
                 if(!MoveActr(Add.aiPushUpDn , ccBwd)) return false ;

                 if(!DM.ARAY[Add.riPSI].CheckAllStat(csNone)) { //포스트 버퍼에 자제 있으면 움직이면 낑긴다.
                     Step.iToStart = 0 ;
                     return true ;
                 }

                 //오토런에서 되도록.
                 //if(DM.ARAY[Add.riULDBtm].GetCntStat(csUnkwn) && DM.ARAY[Add.riULDTop].GetCntStat(csUnkwn)) {
                 //    MoveMotr(Add.miULD_ZElev , Add.piULD_ZElevWait);
                 //}
                 //else {
                 //    MoveMotr(Add.miULD_ZElev , Add.piULD_ZElevWork);
                 //}
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MT_GetStopInpos(Add.miULD_ZElev)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CUnLoader::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiULD_ToStopTO);

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

        case 10:
                 MoveActr(Add.aiPushLtRt , ccBwd);
                 MoveActr(Add.aiPushUpDn , ccBwd);

                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveActr(Add.aiPushLtRt , ccBwd)) return false ;
                 if(!MoveActr(Add.aiPushUpDn , ccBwd)) return false ;
                 if(!DM.ARAY[Add.riPSI].CheckAllStat(csNone)) { //포스트 버퍼에 자제 있으면 움직이면 낑긴다.
                     Step.iToStop = 0 ;
                     return true ;
                 }

                 //if(!IO_GetX(Add.xULD_Detect5)) {//레일사이 감지 센서.
                 //    MoveMotr(Add.miULD_ZElev , Add.piULD_ZElevWait);
                 //}

                 Step.iToStop++;
                 return false;

        case 12: //if(!MT_GetStop(Add.miULD_ZElev)) return false;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CUnLoader::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        bool isZWorkPos = IsWorkPos();

        //이조건은 프리인덱스 조건이네....
        bool isCycleWork = (DM.ARAY[Add.riULDTop].GetCntStat (csEmpty) || DM.ARAY[Add.riULDBtm].GetCntStat (csEmpty)) &&
                           !isZWorkPos && !IO_GetX(Add.xULD_Detect5);

        bool isCyclePush = !DM.ARAY[Add.riPSI].GetCntStat(csNone) && IO_GetX(Add.xULD_Detect5) &&
                            isZWorkPos ;

        //bool isCycleWork = !DM.ARAY[Add.riPSI].CheckAllStat(csNone) &&
        //                   (DM.ARAY[Add.riULDTop].GetCntStat (csEmpty) || DM.ARAY[Add.riULDBtm].GetCntStat (csEmpty)) ;

        bool isCycleEnd  = Add.IDX -> GetWorkEnd() &&
                           DM.ARAY[Add.riPSI].CheckAllStat(csNone);
                           //DM.ARAY[Add.riULDTop].CheckAllStat(csNone) && //중간에 빼는 경우가 있어서 주석.
                           //DM.ARAY[Add.riULDBtm].CheckAllStat(csNone) ;

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCyclePush  ) {Trace(m_sPartName.c_str(),"CyclePush   Stt"); Step.iSeq = scPush   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                        Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                               return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPush  : if(CyclePush  ()){ Trace(m_sPartName.c_str(),"CyclePush   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CUnLoader::IsWorkPos()
{
    return MT_CmprPos(Add.miULD_ZElev ,GetMotrPos(Add.miULD_ZElev , Add.piULD_ZElevWork)) ;

}
bool CUnLoader::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        sTemp  = sTemp.sprintf(" %s TIMEOUT Step.iHome=%02d" , __FUNC__ , Step.iHome );
        sTemp  = m_sPartName + sTemp ;
        EM_SetErrMsg(eiPRT_HomeTo,sTemp.c_str());
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

        case 10:

                 if(IO_GetX(Add.xULD_Detect5)) {
                      EM_SetErrMsg(eiPRT_Crash , (m_sPartName + " 레일 배출부 센서 감지 상태로 보트 돌출 확인하세요").c_str());
                      Step.iHome = 0 ;
                      return true ;
                 }
                 MoveActr(Add.aiPushLtRt , ccBwd);
                 MoveActr(Add.aiPushUpDn , ccBwd);

                 Step.iHome++;
                 return false ;

        case 11: if(!MoveActr(Add.aiPushLtRt , ccBwd)) return false ;
                 if(!MoveActr(Add.aiPushUpDn , ccBwd)) return false ;

                 MT_DoHome(Add.miULD_ZElev);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(Add.miULD_ZElev)) return false ;
                 MT_GoAbsMan(Add.miULD_ZElev , PM.GetValue(Add.miULD_ZElev , Add.pvULD_ZElevWait ));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(Add.miULD_ZElev , PM.GetValue(Add.miULD_ZElev , Add.pvULD_ZElevWait ))) return false ;
                 SetLastCmd();
                 Step.iHome = 0;
                 return true ;

    }
}

//One Cycle.
bool CUnLoader::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiULD_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //Step.iCycle = 0;
        //return true ;
    }

    int r ;
    EN_ARAY_ID riULD ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: if(IO_GetX(Add.xULD_Detect5)) {
                      EM_SetErrMsg(eiPRT_Crash , (m_sPartName + " 레일 배출부 센서 감지 상태로 보트 돌출 확인하세요").c_str());
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  MoveMotr(Add.miULD_ZElev , Add.piULD_ZElevWork);


                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(Add.miULD_ZElev , Add.piULD_ZElevWork))return false ;
                  Step.iCycle = 0;
                  return true;
    }
}

bool CUnLoader::CyclePush(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiULD_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //Step.iCycle = 0;
        //return true ;
    }

    int r ;
    EN_ARAY_ID riULD ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: MoveActr(Add.aiPushUpDn , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(Add.aiPushUpDn , ccFwd)) return false ;
                  MoveActr(Add.aiPushLtRt , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveActr(Add.aiPushLtRt , ccFwd)) return false ;

                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

        case  13: if(!m_tmDelay.OnDelay(true , 300)) return false ;


                  if(IO_GetX(Add.xULD_Detect5)){
                      EM_SetErrMsg(eiPRT_BoatMiss , (m_sPartName + " 에서 보트가 삽입완료 되지 않았습니다.").c_str()) ;
                      Step.iCycle=0;
                      return true ;
                  }


                  //데이터 이동.
                  if(FindChip(r , riULD)){
                      DM.ARAY[riULD].SetStat(r,0,csWork);
                      DM.ARAY[Add.riPSI].SetStat(csNone);
                      if(DM.ARAY[riULD].CheckAllStat(csEmpty)){
                          DM.ARAY[riULD].SetLotNo(DM.ARAY[Add.riPSI].GetLotNo()) ;
                      }
                      DM.ARAY[Add.riPSI].ClearMap();
                  }

                  MoveActr(Add.aiPushUpDn , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveActr(Add.aiPushUpDn , ccBwd)) return false ;
                  MoveActr(Add.aiPushLtRt , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(Add.aiPushLtRt , ccBwd)) return false ;
                  Step.iCycle=0;
                  return true;
    }

}

//---------------------------------------------------------------------------
bool CUnLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == Add.miULD_ZElev){
        if(IO_GetX(Add.xULD_Detect5)) {
            sMsg = m_sPartName + " 레일 배출부 센서 감지 상태로 보트 돌출 확인하세요" ;
            bRet = false ;
        }
        //if(MT_GetCmdPos(Add.miPRI_XIndx) > PM.GetValue(Add.miPRI_XIndx , Add.pvPRI_XWait)) {
        //    EM_SetErrMsg(eiPRT_Crash , (m_sPartName + "보트 충돌 방지 센서 감지중").c_str());
        //    Step.iToStart = 0 ;
        //    return true ;
        //}
    }
    else {
        sMsg = "Motor " + AnsiString(MT_GetName(_iMotr)) + " is Not this parts." ;
        bRet = false ;
    }

    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(MT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iSeq)FM_MsgOk(MT_GetName(_iMotr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CUnLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool bPushUpDnFwd = AT_GetCmd(Add.aiPushUpDn) == ccFwd;
    
    if(_iActr == Add.aiPushLtRt){
        if(_bFwd == ccFwd) {
            if(!bPushUpDnFwd){sMsg = "Cylinder PushUpDn is not Fwd.";bRet = false ;}
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else if(_iActr == Add.aiPushUpDn){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    }


    if(!bRet){
        m_sCheckSafeMsg = sMsg ;
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }
    else {
        m_sCheckSafeMsg = "" ;
    }

    return bRet ;
}

bool CUnLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(_bSlow) {
        return MT_GoAbsSlow(_iMotr , dPosition);
    }
    else {
        if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
        else            return MT_GoAbsMan(_iMotr , dPosition);
    }

    return false ;
}







void CUnLoader::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CUnLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CUnLoader::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(Add. miULD_ZElev);
    return ; //이파트는 필요 없다.

}

bool CUnLoader::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(Add.miULD_ZElev)) return true  ; //이파트는 필요 없다.
    else                                               return false ;

}

bool CUnLoader::CheckStop()
{
    if(!MT_GetStop(Add.miULD_ZElev)) return false ;

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





