//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
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
CLoader LDR_F;
CLoader LDR_R;

CGridData g_LotData ;

CLoader::CLoader(void)
{

}

CLoader::~CLoader (void)
{

}

void CLoader::Init(bool _bFront )
{
    if(_bFront){
        m_sPartName                 = "Front Loader"           ;
        Add.iPartId                 = piLDR_F                  ;
        Add.riLDRTop                = riLDR_FTop               ;
        Add.riLDRBtm                = riLDR_FBtm               ;
        Add.miLDR_ZElev             = miLDR_ZElevF             ;
        Add.pvLDR_ZElevWait         = pvLDR_ZElevFWait         ;
        Add.pvLDR_ZElevWorkStartBtm = pvLDR_ZElevFWorkStartBtm ;
        Add.pvLDR_ZElevWorkStartTop = pvLDR_ZElevFWorkStartTop ;
        Add.piLDR_ZElevWait         = piLDR_ZElevFWait         ;
        Add.piLDR_ZElevWorkStartBtm = piLDR_ZElevFWorkStartBtm ;
        Add.piLDR_ZElevWorkStartTop = piLDR_ZElevFWorkStartTop ;
        Add.piLDR_ZElevWork         = piLDR_ZElevFWork         ;

        Add.riPRI                   = riPRI_F                  ;
        Add.xPRI_Detect1            = xPRI_FDetect1            ;
        Add.xPRI_Detect2            = xPRI_FDetect2            ;
        Add.miPRI_XIndx             = miPRI_XIndxF             ;
        Add.pvPRI_XWait             = pvPRI_XIndxFWait         ;

        Add.riIDX                   = riIDX_F                  ;

        g_LotData.LoadFromCsv(EXE_FOLDER + "SeqData\\LotData.csv");

    }
    else {
        m_sPartName                 = "Rear Loader"            ;
        Add.iPartId                 = piLDR_R                  ;
        Add.riLDRTop                = riLDR_RTop               ;
        Add.riLDRBtm                = riLDR_RBtm               ;
        Add.miLDR_ZElev             = miLDR_ZElevR             ;
        Add.pvLDR_ZElevWait         = pvLDR_ZElevRWait         ;
        Add.pvLDR_ZElevWorkStartBtm = pvLDR_ZElevRWorkStartBtm ;
        Add.pvLDR_ZElevWorkStartTop = pvLDR_ZElevRWorkStartTop ;
        Add.piLDR_ZElevWait         = piLDR_ZElevRWait         ;
        Add.piLDR_ZElevWorkStartTop = piLDR_ZElevRWorkStartTop ;
        Add.piLDR_ZElevWorkStartBtm = piLDR_ZElevRWorkStartBtm ;
        Add.piLDR_ZElevWork         = piLDR_ZElevRWork         ;

        Add.riPRI                   = riPRI_R                  ;
        Add.xPRI_Detect1            = xPRI_RDetect1            ;
        Add.xPRI_Detect2            = xPRI_RDetect2            ;
        Add.miPRI_XIndx             = miPRI_XIndxR             ;
        Add.pvPRI_XWait             = pvPRI_XIndxRWait         ;

        Add.riIDX                   = riIDX_R                  ;
    }

    m_sCheckSafeMsg = "" ;

    Reset();
    Load(true);


    InitCycleName();
    InitCycleTime();

}

void CLoader::Close()
{
    Load(false);
    g_LotData.SaveToCsv(EXE_FOLDER + "SeqData\\LotData.csv");
}

void CLoader::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


bool CLoader::FindChip(int &r , EN_ARAY_ID &id)
{
    if(-1 != DM.ARAY[Add.riLDRBtm].FindLastRow(csUnkwn)){
        id = Add.riLDRBtm ;
        r  = DM.ARAY[Add.riLDRBtm].FindLastRow(csUnkwn) ;
        return true ;
    }
    else if(-1 != DM.ARAY[Add.riLDRTop].FindLastRow(csUnkwn)){
        id = Add.riLDRTop ;
        r  = DM.ARAY[Add.riLDRTop].FindLastRow(csUnkwn) ;
        return true ;
    }

    return false ;
}

double CLoader::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dWorkStt ; //작업해야할 메거진의 맨 아래 슬롯 위치.
    double dWork    ; //현재 작업해야할 포지션.
    int r ;
    EN_ARAY_ID riWork ;

    if(FindChip(r,riWork)){
        if(riWork == Add.riLDRBtm){
            dWorkStt = PM.GetValue(Add.miLDR_ZElev , Add.pvLDR_ZElevWorkStartBtm) ;
            dWork    = dWorkStt + (OM.DevInfo.iMgzSlotCnt - r - 1) * OM.DevInfo.dMgzSlotPitch ;
        }
        else {
            dWorkStt = PM.GetValue(Add.miLDR_ZElev , Add.pvLDR_ZElevWorkStartTop) ;
            dWork    = dWorkStt + (OM.DevInfo.iMgzSlotCnt - r - 1)  * OM.DevInfo.dMgzSlotPitch ;
        }
    }

    if(_iMotr == Add.miLDR_ZElev){
             if(_iPstnId ==  Add.piLDR_ZElevWait        ) dPos = PM.GetValue (_iMotr , Add.pvLDR_ZElevWait         );
        else if(_iPstnId ==  Add.piLDR_ZElevWorkStartBtm) dPos = PM.GetValue (_iMotr , Add.pvLDR_ZElevWorkStartBtm );
        else if(_iPstnId ==  Add.piLDR_ZElevWorkStartTop) dPos = PM.GetValue (_iMotr , Add.pvLDR_ZElevWorkStartTop );
        else if(_iPstnId ==  Add.piLDR_ZElevWork        ) dPos = dWork                                              ;
        else                                              dPos = MT_GetCmdPos(_iMotr                               );
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CLoader::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CLoader::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CLoader::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CLoader::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: if(IO_GetX(Add.xPRI_Detect1)) {
                     EM_SetErrMsg(eiPRT_Crash , (m_sPartName + "보트 충돌 방지 센서 감지중").c_str());
                     Step.iToStart = 0 ;
                     return true ;
                 }

                 Step.iToStart ++ ;
                 return false ;

        case 11: //if(!MT_GetStopInpos(Add.miLDR_ZElev)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CLoader::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: if(IO_GetX(Add.xPRI_Detect1)){
                     Step.iCycle = 0;
                     return true;
                 }
                 //MoveMotr(Add.miLDR_ZElev , Add.piLDR_ZElevWait);
                 Step.iToStop++;
                 return false;

        case 11: //if(!MoveMotr(Add.miLDR_ZElev , Add.piLDR_ZElevWait))return false;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CLoader::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        //이조건은 프리인덱스 조건이네....                                                                  ㄴ
        bool isCycleWork = (DM.ARAY[Add.riLDRTop].GetCntStat (csUnkwn) || DM.ARAY[Add.riLDRBtm].GetCntStat (csUnkwn)) &&
                           !isZWorkPos && !IO_GetX(Add.xPRI_Detect1);

                           // DM.ARAY[Add.riPRI   ].CheckAllStat(csNone) &&
                           //(DM.ARAY[Add.riIDX   ].FindFrstCol(csWork  ) <= 1 || DM.ARAY[Add.riIDX   ].CheckAllStat(csEmpty));
        bool isCycleEnd  = DM.ARAY[Add.riLDRTop].GetCntStat(csUnkwn)==0&& DM.ARAY[Add.riLDRBtm].GetCntStat(csUnkwn)==0 ;

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                        Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                               return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}
bool CLoader::IsWorkPos()
{
    return MT_CmprPos(Add.miLDR_ZElev ,GetMotrPos(Add.miLDR_ZElev , Add.piLDR_ZElevWork)) ;

}

bool CLoader::CycleHome()     //sun DLL direct access.
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

        case 10: if(IO_GetX(Add.xPRI_Detect1)) {
                      EM_SetErrMsg(eiPRT_Crash , (m_sPartName + " 레일 진입부 센서 감지 상태로 보트 돌출 확인하세요").c_str());
                      Step.iHome = 0 ;
                      return true ;
                 }
                 MT_DoHome(Add.miLDR_ZElev);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(Add.miLDR_ZElev)) return false ;
                 MT_GoAbsMan(Add.miLDR_ZElev , PM.GetValue(Add.miLDR_ZElev , Add.pvLDR_ZElevWait ));
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GoAbsMan(Add.miLDR_ZElev , PM.GetValue(Add.miLDR_ZElev , Add.pvLDR_ZElevWait ))) return false ;
                 SetLastCmd();
                 Step.iHome = 0;
                 return true ;

    }
}

//One Cycle.
bool CLoader::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
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

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //Step.iCycle = 0;
        //return true ;
    }

    int r ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case  10: if(IO_GetX(Add.xPRI_Detect1)) {
                      EM_SetErrMsg(eiPRT_Crash , (m_sPartName + " 레일 진입부 센서 감지 상태로 보트 돌출 확인하세요").c_str());
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  MoveMotr(Add.miLDR_ZElev , Add.piLDR_ZElevWork);


                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(Add.miLDR_ZElev , Add.piLDR_ZElevWork))return false ;
                  Step.iCycle=0;
                  return true;
    }

}

//---------------------------------------------------------------------------
bool CLoader::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == Add.miLDR_ZElev){
        if(IO_GetX(Add.xPRI_Detect1)) {
            sMsg = m_sPartName + " 레일 진입부 센서 감지 상태로 보트 돌출 확인하세요" ;
            bRet = false;
        }
        if(MT_GetCmdPos(Add.miPRI_XIndx) > PM.GetValue(Add.miPRI_XIndx , Add.pvPRI_XWait)) {
            sMsg = m_sPartName + "프리 인덱스의 위치가 Wait위치 보다 높습니다.";
            bRet = false;
        }
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
bool CLoader::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    //if(_iActr == aiLDR_IndxUpUp){
    //    if(_bFwd == ccFwd) {
    //        //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
    //    }
    //}
    //else {
        sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
        bRet = false ;
    //}


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

bool CLoader::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
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







void CLoader::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CLoader::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CLoader::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(Add. miLDR_ZElev);
    return ; //이파트는 필요 없다.

}

bool CLoader::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(Add.miLDR_ZElev)) return true  ; //이파트는 필요 없다.
    else                                               return false ;

}

bool CLoader::CheckStop()
{
    if(!MT_GetStop(Add.miLDR_ZElev)) return false ;

    return true ;
}

void CLoader::Load(bool _bLoad)
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





