//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Dispensor.h"
#include "Index.h"
#include "Stage.h"
#include "Picker.h"
//---------------------------------------------------------------------------
#include "SVInterface.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "UserFile.h"
#include "Pattern.h"
#include "ManualMan.h"

#include "Rs232Keyence.h"



//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CDispensor DSP;

CDispensor::CDispensor(void)
{

}

CDispensor::~CDispensor (void)
{

}

void CDispensor::Init()
{
    m_sPartName = "Dispensor ";
    
    Reset();
    Load(true);



    InitCycleName();
    InitCycleTime();

    m_dPkgCntX = 0.0 ;
    m_dPkgCntY = 0.0 ; 
    m_dPkgCntT = 0.0 ;

    m_sCheckSafeMsg = "" ;

    m_bRqstPMSave = false;
    m_bOneSkipVisnAt = false;

    m_bVsnBfErrDisp = false ;
    m_bVsnAtErrDisp = false ;

}

void CDispensor::Close()
{
    Load(false);
}

void CDispensor::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}

//마지막 엠티를 리턴.
bool CDispensor::FindChip(int &c , EN_ARAY_ID &id)
{
    return IDX_R.FindChip(c,id);
}


double CDispensor::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dWorkStt ;


    //piDSP_XDisprWait
    //piDSP_XDisprVsStageCntr
    //piDSP_XDisprVsPad
    //piDSP_XDisprNdCheck
    //piDSP_XDisprNdClean
    //piDSP_XDisprHtStage
    //piDSP_XDisprHtCheck
    //
    //piDSP_XDisprNdStageCntr
    //piDSP_XDisprNdPad
    //
    //piDSP_XDisprHtStageCntr





    if(_iMotr == miDSP_XDispr){
             if(_iPstnId ==  piDSP_XDisprWait       ) dPos = PM.GetValue (_iMotr , pvDSP_XDisprWait       );
        else if(_iPstnId ==  piDSP_XDisprVsStageCntr) dPos = PM.GetValue (_iMotr , pvDSP_XDisprVsStageCntr);
        else if(_iPstnId ==  piDSP_XDisprVsPad      ) dPos = PM.GetValue (_iMotr , pvDSP_XDisprVsPad      );
        else if(_iPstnId ==  piDSP_XDisprNdCheck    ) dPos = PM.GetValue (_iMotr , pvDSP_XDisprNdCheck    );
        else if(_iPstnId ==  piDSP_XDisprNdClean    ) dPos = PM.GetValue (_iMotr , pvDSP_XDisprNdClean    );
        else if(_iPstnId ==  piDSP_XDisprHtStage    ) dPos = PM.GetValue (_iMotr , pvDSP_XDisprHtStage    );
        else if(_iPstnId ==  piDSP_XDisprHtCheck    ) dPos = PM.GetValue (_iMotr , pvDSP_XDisprHtCheck    );
        else if(_iPstnId ==  piDSP_XDisprStgFlatness1) dPos = PM.GetValue (_iMotr , pvDSP_XDisprStgFlatness1 ) ;
        else if(_iPstnId ==  piDSP_XDisprStgFlatness2) dPos = PM.GetValue (_iMotr , pvDSP_XDisprStgFlatness2 ) ;
        else if(_iPstnId ==  piDSP_XDisprStgFlatness3) dPos = PM.GetValue (_iMotr , pvDSP_XDisprStgFlatness3 ) ;
        else if(_iPstnId ==  piDSP_XDisprStgFlatness4) dPos = PM.GetValue (_iMotr , pvDSP_XDisprStgFlatness4 ) ;
        else if(_iPstnId ==  piDSP_XDisprNdStageCntr) dPos = PM.GetValue (_iMotr , pvDSP_XDisprVsStageCntr) + PM.GetValue (_iMotr , poDSP_XDisprVsNiddle);
        else if(_iPstnId ==  piDSP_XDisprNdPad      ) dPos = PM.GetValue (_iMotr , pvDSP_XDisprVsPad      ) + PM.GetValue (_iMotr , poDSP_XDisprVsNiddle);
        else if(_iPstnId ==  piDSP_XDisprHtStageCntr) dPos = PM.GetValue (_iMotr , pvDSP_XDisprVsStageCntr) + PM.GetValue (_iMotr , poDSP_XDisprVsHeight);

        else                                          dPos = MT_GetCmdPos(_iMotr                          );
    }
    else if(_iMotr == miDSP_YDispr){ //요놈 높이측정기는 추후에 생각 하자.
             if(_iPstnId ==  piDSP_YDisprWait       ) dPos = PM.GetValue (_iMotr , pvDSP_YDisprWait       );
        else if(_iPstnId ==  piDSP_YDisprVsStageCntr) dPos = PM.GetValue (_iMotr , pvDSP_YDisprVsStageCntr);
        else if(_iPstnId ==  piDSP_YDisprVsPad      ) dPos = PM.GetValue (_iMotr , pvDSP_YDisprVsPad      );
        else if(_iPstnId ==  piDSP_YDisprNdCheck    ) dPos = PM.GetValue (_iMotr , pvDSP_YDisprNdCheck    );
        else if(_iPstnId ==  piDSP_YDisprNdClean    ) dPos = PM.GetValue (_iMotr , pvDSP_YDisprNdClean    );
        else if(_iPstnId ==  piDSP_YDisprHtStage    ) dPos = PM.GetValue (_iMotr , pvDSP_YDisprHtStage    );
        else if(_iPstnId ==  piDSP_YDisprHtCheck    ) dPos = PM.GetValue (_iMotr , pvDSP_YDisprHtCheck    );
        else if(_iPstnId ==  piDSP_YDisprStgFlatness1) dPos = PM.GetValue (_iMotr , pvDSP_YDisprStgFlatness1 ) ;  //PV PI 카운팅 안맞는거.
        else if(_iPstnId ==  piDSP_YDisprStgFlatness2) dPos = PM.GetValue (_iMotr , pvDSP_YDisprStgFlatness2 ) ;
        else if(_iPstnId ==  piDSP_YDisprStgFlatness3) dPos = PM.GetValue (_iMotr , pvDSP_YDisprStgFlatness3 ) ;
        else if(_iPstnId ==  piDSP_YDisprStgFlatness4) dPos = PM.GetValue (_iMotr , pvDSP_YDisprStgFlatness4 ) ;
        else if(_iPstnId ==  piDSP_YDisprNdStageCntr) dPos = PM.GetValue (_iMotr , pvDSP_YDisprVsStageCntr) + PM.GetValue (_iMotr , poDSP_YDisprVsNiddle);
        else if(_iPstnId ==  piDSP_YDisprNdPad      ) dPos = PM.GetValue (_iMotr , pvDSP_YDisprVsPad      ) + PM.GetValue (_iMotr , poDSP_YDisprVsNiddle);

        else if(_iPstnId ==  piDSP_YDisprHtStageCntr) dPos = PM.GetValue (_iMotr , pvDSP_YDisprVsStageCntr) + PM.GetValue (_iMotr , poDSP_YDisprVsHeight);



        else                                          dPos = MT_GetCmdPos(_iMotr                          );
    }
    else if(_iMotr == miDSP_ZDispr){
             if(_iPstnId ==  piDSP_ZDisprWait    ) dPos = PM.GetValue (_iMotr , pvDSP_ZDisprWait    );
        else if(_iPstnId ==  piDSP_ZDisprCheck   ) dPos = PM.GetValue (_iMotr , pvDSP_ZDisprCheck   );
        else if(_iPstnId ==  piDSP_ZDisprMove    ) dPos = PM.GetValue (_iMotr , pvDSP_ZDisprMove    );
        else if(_iPstnId ==  piDSP_ZDisprClean   ) dPos = PM.GetValue (_iMotr , pvDSP_ZDisprClean   );
        else if(_iPstnId ==  piDSP_ZDisprWork    ) dPos = PM.GetValue (_iMotr , pvDSP_ZDisprCheck   ) - OM.DevOptn.dDispZOfs;
        else                                       dPos = MT_GetCmdPos(_iMotr                       );
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CDispensor::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CDispensor::Update()
{
    Rs232_Dispr.Update();
}

bool CDispensor::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CDispensor::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CDispensor::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: MoveMotr(miDSP_ZDispr , piDSP_ZDisprWait);
                 IO_SetY(yDSP_DispOn , false);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprWait)) return false ;
                 MoveActr(aiDSP_NdlCvFwBw , ccBwd);
                 MoveMotr(miDSP_XDispr , piDSP_XDisprWait);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprWait);
                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprWait)) return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprWait)) return false ;
                 if(!MoveActr(aiDSP_NdlCvFwBw , ccBwd)) return false ;
                 if(OM.CmnOptn.iDisprClnCnt != 0) m_iDisprBoatCnt = OM.CmnOptn.iDisprClnCnt ; //처음 시작 할때는 한번씩 씻고 하자.
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CDispensor::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 IO_SetY(yDSP_DispOn , false);
                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;
                 MoveMotr(miDSP_XDispr , piDSP_XDisprNdClean);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprNdClean);
                 MoveActr(aiDSP_NdlCvFwBw , ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprNdClean)) return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprNdClean)) return false ;
                 if(!MoveActr(aiDSP_NdlCvFwBw , ccBwd)) return false ;
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CDispensor::Autorun(void) //오토런닝시에 계속 타는 함수.
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
        //FOS 순서                                    //DIE순서
        //csVisn    얼라인검사                        //csVisn    얼라인 검사 해본다.

        //차이점.
        //csAttach  픽커 칩 붙인다.                   //csDisp    파란판에 패턴 디스펜싱 한다.
        //csDisp    디스펜싱 하고.                    //csAttach  칩을 붙인다.

        //csPush    일정 시간 꾹 누르고 픽커 빠진다.  //csPush
        //csEndVisn 작업 완료 비젼 확인.              //csEndVisn 작업 완료 비젼 확인.
        //csWorkEnd 작업 종료 하고 스테이지 내린다.   //csWorkEnd 작업 종료 하고 스테이지 내린다.
        bool isPckrPosAlnUnder =  MT_GetTrgPos(miPCK_YPickr) <= PM.GetValue(miPCK_YPickr , pvPCK_YPickrAlign      ) ;
        bool isDispPosVsnUnder =  MT_GetTrgPos(miDSP_YDispr) <= PM.GetValue(miDSP_YDispr , pvDSP_YDisprVsStageCntr) ;

        bool isCycleClean  = OM.CmnOptn.iDisprClnCnt != 0 && OM.CmnOptn.iDisprClnCnt <= m_iDisprBoatCnt;
        //DM.ARAY[riSTG].GetStat(0,0) == csWorkEnd || DM.ARAY[riSTG].GetStat(0,0) == csSkip || DM.ARAY[riSTG].GetStat(0,0) == csVisnNG;
        bool isCycleVisnBf = DM.ARAY[riSTG].GetStat(0,0) == csVisn   && //비전 에러 스킵 할때 이렇게 순서대로 해야지.. 편함....
                             isPckrPosAlnUnder ;
        bool isCycleDisp   = DM.ARAY[riSTG].GetStat(0,0) == csDisp   ;//&& isPckrPosAlnUnder ;
        bool isCycleVisnAt = DM.ARAY[riSTG].GetStat(0,0) == csVisnAt && isPckrPosAlnUnder ;
        bool isCycleEnd    = IDX_R.GetStat().bWorkEnd && DM.ARAY[riSTG].CheckAllStat(csNone);

        if(EM_IsErr()) return false ;
         //Normal Decide Step.
             if (isCycleVisnBf) {Trace(m_sPartName.c_str(),"CycleVisnBf Stt"); Step.iSeq = scVisnBf ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleDisp  ) {Trace(m_sPartName.c_str(),"CycleDisp   Stt"); Step.iSeq = scDisp   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleVisnAt) {Trace(m_sPartName.c_str(),"CycleVisnAt Stt"); Step.iSeq = scVisnAt ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleClean ) {Trace(m_sPartName.c_str(),"CycleClean  Stt"); Step.iSeq = scClean  ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    Step.iLastSeq = Step.iSeq ;
    switch (Step.iSeq) {
        default       :                     Trace(m_sPartName.c_str(),"default     End");                                                   Step.iSeq = scIdle ;  return false ;
        case scIdle   :                                                                                                                                           return false ;
        case scVisnBf : if(CycleVisnBf ()){ Trace(m_sPartName.c_str(),"CycleVisnBf End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scDisp   : if(CycleDisp   ()){ Trace(m_sPartName.c_str(),"CycleDisp   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scVisnAt : if(CycleVisnAt ()){ Trace(m_sPartName.c_str(),"CycleVisnAt End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scClean  : if(CycleClean  ()){ Trace(m_sPartName.c_str(),"CycleClean  End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}
int CDispensor::GetReptCnt()
{
    return m_iReptCnt;
}
double CDispensor::GetDisprTime()
{
    return m_dDisprTime;
}
void CDispensor::SetDisprTime(double _dDisprTime)
{
    m_dDisprTime = _dDisprTime;
}
void CDispensor::AddDisprBoatCnt()
{
    m_iDisprBoatCnt += 1;
}

bool CDispensor::CycleHome()     //sun DLL direct access.
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

        case 10: MT_DoHome(miDSP_ZDispr);
                 AT_MoveCyl(aiDSP_NdlCrFwBw , ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miDSP_ZDispr))return false ;
                 MT_DoHome(miDSP_YDispr);
                 MT_DoHome(miDSP_XDispr);
                 Step.iHome++;

                 return false ;

        case 12: if(!MT_GetHomeEnd(miDSP_YDispr)) return false ;
                 if(!MT_GetHomeEnd(miDSP_XDispr)) return false ;


                 MT_GoAbsMan(miDSP_XDispr , PM.GetValue(miDSP_XDispr,pvDSP_XDisprNdClean));
                 MT_GoAbsMan(miDSP_YDispr , PM.GetValue(miDSP_YDispr,pvDSP_YDisprNdClean));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miDSP_XDispr , PM.GetValue(miDSP_XDispr,pvDSP_XDisprNdClean))) return false ;
                 if(!MT_GoAbsMan(miDSP_YDispr , PM.GetValue(miDSP_YDispr,pvDSP_YDisprNdClean))) return false ;

                 MT_GoAbsMan(miDSP_ZDispr , PM.GetValue(miDSP_ZDispr,pvDSP_ZDisprMove));
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miDSP_ZDispr , PM.GetValue(miDSP_ZDispr,pvDSP_ZDisprMove))) return false ;
                 MoveActr(aiDSP_NdlCvFwBw , ccBwd);
                 Step.iHome++;
                 return false ;

        case 15: if(!MoveActr(aiDSP_NdlCvFwBw , ccBwd)) return false ;

                 Step.iHome = 0;
                 return true ;

    }
}



//One Cycle.
bool CDispensor::CycleVisnBf()
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

    if(Stat.bReqStop) { //스탑시에 멈추면 자제 못쓰게됌.
        //Step.iCycle = 0;
        //return true ;
    }

    char VisnRslt[64];
    String sVisnRslt ;
    String sItem ;

    double dX     ;
    double dY     ;
    double dT     ;
    double dRadT  ;

    double dMoveX ;
    double dMoveY ;
    double dMoveT ;

    static bool bNeedVoidPCK = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

                 
        case 10: m_dBfVisnX = 0.0 ;
                 m_dBfVisnY = 0.0 ;
                 m_dBfVisnT = 0.0 ;
                 m_bBfVisn  = true;

                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);

                 Step.iCycle++;
                 return false ;

        case 11: //메뉴얼시에 충돌 위치에 있으면 치운다.
                 if(MM.GetManNo() != scNone && !PCK.ToolVoid()) return false ;
                 if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;

                 MoveMotr(miDSP_XDispr , piDSP_XDisprHtStage);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprHtStage);
                 MoveActr(aiDSP_NdlCvFwBw , ccBwd);

                 Step.iCycle++;
                 return false;

        case 12: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprHtStage)) return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprHtStage)) return false ;
                 if(!MoveActr(aiDSP_NdlCvFwBw , ccBwd)) return false ;
                 if(MM.GetManNo() != mcNoneCycle && MT_GetTrgPos(miSTG_ZStage) <= PM.GetValue(miSTG_ZStage , pvSTG_ZStagPick) /*!IO_GetX(xSTG_Vccm)*/){
                     STG.MoveMotr(miSTG_ZStage , piSTG_ZStagPick);
                     Step.iCycle++;
                     return false;
                 }

                 STG.MoveMotr(miSTG_ZStage , piSTG_ZStagCheck);
                 Step.iCycle = 15;
                 return false ;

        case 13: if(!STG.MoveMotr(miSTG_ZStage , piSTG_ZStagPick)) return false;
                 IO_SetY(ySTG_Vccm , true );
                 IO_SetY(ySTG_Ejct , false);
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 14: if(!m_tmDelay.OnDelay(true , 500)) return false;
                 if(!IO_GetX(xSTG_Vccm)){
                     EM_SetErrMsg(eiSTG_PickMiss , "스테이지에 보트나 자제의 안착 상태를 확인하세요");
                     Step.iCycle = 0;
                     return true;
                 }
                 STG.MoveMotr(miSTG_ZStage , piSTG_ZStagCheck);
                 Step.iCycle++;
                 return false;

        //위에서 사용. 메뉴얼 아닐 시.
        case 15: if(!STG.MoveMotr(miSTG_ZStage , piSTG_ZStagCheck)) return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 16: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 Rs232Keyence.CheckHeight(kcChB);
                 Step.iCycle++;
                 return false ;

        case 17: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChB)) {
                     EM_SetErrMsg(eiETC_RS232 , "디스펜서 높이 측정기 측정포지션을 확인하세요.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 STG.SetPkgThickness(Rs232Keyence.GetHeightData(kcChB));
                 STG.SetFinishThickness(0.0);
                 STG.SetEpoxyThickness(0.0);
                 STG.MoveMotr(miSTG_ZStage , piSTG_ZStagWork);

                 if(OM.DevOptn.bStIgnrVisnBefore){
                     m_dBfVisnX = 0.0 ;
                     m_dBfVisnY = 0.0 ;
                     m_dBfVisnT = 0.0 ;
                     m_bBfVisn  = true;

                     m_dPkgCntX = MT_GetTrgPos(miSTG_XStage) ;
                     m_dPkgCntY = MT_GetTrgPos(miSTG_YStage) ;
                     m_dPkgCntT = MT_GetTrgPos(miSTG_TStage) ;

                     if(OM.DevInfo.iWorkMode == wmFos)DM.ARAY[riSTG].SetStat(csAttach);
                     else                             DM.ARAY[riSTG].SetStat(csDisp  );
                     Step.iCycle=0;
                     return true ;
                 }
                 else {
                     //JS
                     if(!SKIP_VISN)SV_Ready(viStageAlignBf);
                 }

                 Step.iCycle++;
                 return false ;

        case 18: if(!STG.MoveMotr(miSTG_ZStage , piSTG_ZStagWork)) return false ;
                 //MoveMotr(miDSP_XDispr , piDSP_XDisprVsStageCntr);
                 //MoveMotr(miDSP_YDispr , piDSP_YDisprVsStageCntr);

                 //백레쉬 때문에 ㅜㅠ
                 MT_GoAbsRun(miDSP_XDispr , GetMotrPos(miDSP_XDispr , piDSP_XDisprVsStageCntr)-0.2);
                 MT_GoAbsRun(miDSP_YDispr , GetMotrPos(miDSP_YDispr , piDSP_YDisprVsStageCntr)-0.2);

                 Step.iCycle++;
                 return false ;

        case 19: if(!MT_GetStop(miDSP_XDispr)) return false ;
                 if(!MT_GetStop(miDSP_YDispr)) return false ;
                 MoveMotr(miDSP_XDispr , piDSP_XDisprVsStageCntr);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprVsStageCntr);


                 Step.iCycle++;
                 return false ;

        case 20: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprVsStageCntr))return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprVsStageCntr))return false ;
                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false ;



        case 21: if(!m_tmDelay.OnDelay(true , 100)) return false ;
                 //JS
                 if(!SKIP_VISN){

                     SV_SetPkgID(viStageAlignBf , DM.ARAY[riSTG].CHPS[0][0].GetID().c_str());
                     SV_GrabInsp(viStageAlignBf);
                 }
                 Step.iCycle++;
                 return false ;

        case 22: //JS
                 if(!SKIP_VISN &&!SV_GetGrabEnd(viStageAlignBf)) return false ;
                 if(!SKIP_VISN &&!SV_GetInspEnd(viStageAlignBf)) return false ;

                 //JS
                 if(!SKIP_VISN)SV_GetRslt(viStageAlignBf , VisnRslt);
                 sVisnRslt = VisnRslt ;

                 if(sVisnRslt.Pos("OK")){                                 //"OK-0.23145,0.12344,0.32123
                     sVisnRslt.Delete(1,3);                               //0.23145,0.12344,0.32123
                     //X
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.23145
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));              //0.12344,0.32123
                     sItem.Delete(1,2);
                     m_dBfVisnX = StrToFloatDef(sItem , 0.0);

                     //Y
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.12344
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));
                     sItem.Delete(1,2);
                     m_dBfVisnY = StrToFloatDef(sItem , 0.0);

                     //T
                     sItem = sVisnRslt ;
                     sItem.Delete(1,2);                        //스테이지 세타오프셑.
                     m_dBfVisnT = StrToFloatDef(sItem , 0.0) + OM.DevOptn.dAttachStageOfsT;               //0.32123

                     m_bBfVisn = true ;
                 }
                 else {//"NG"
                     m_dBfVisnX = MT_GetTrgPos(miSTG_XStage) ;
                     m_dBfVisnY = MT_GetTrgPos(miSTG_YStage) ;
                     m_dBfVisnT = MT_GetTrgPos(miSTG_TStage) ;

                     m_bBfVisn = false ;

                     if(OM.CmnOptn.bVisnNgStop) {
                         EM_SetErrMsg(eiVSN_InspNG , "Before Attach Vision Inspection NG");
                         if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp = true ;
                         Step.iCycle=0;
                         return true ;
                     }
                     else {
                         DM.ARAY[riSTG].SetStat(csSkip);
                         //if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riPCK].SetStat(csSkip);
                         Step.iCycle=0;
                         return true  ;
                     }
                 }

                 if(OM.CmnOptn.bVisnNgStop) {
                     if(m_dBfVisnX > OM.DevOptn.dStageTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 X보정값이 설정범위를 넘었습니다.");if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ; Step.iCycle = 0 ; return true ;}
                     if(m_dBfVisnY > OM.DevOptn.dStageTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 Y보정값이 설정범위를 넘었습니다.");if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ; Step.iCycle = 0 ; return true ;}
                     if(m_dBfVisnT > OM.DevOptn.dStageTolAng ){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 T보정값이 설정범위를 넘었습니다.");if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ; Step.iCycle = 0 ; return true ;}

                     if(m_dBfVisnX < -OM.DevOptn.dStageTolXY ){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 X보정값이 설정범위를 넘었습니다.");if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ; Step.iCycle = 0 ; return true ;}
                     if(m_dBfVisnY < -OM.DevOptn.dStageTolXY ){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 Y보정값이 설정범위를 넘었습니다.");if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ; Step.iCycle = 0 ; return true ;}
                     if(m_dBfVisnT < -OM.DevOptn.dStageTolAng){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 T보정값이 설정범위를 넘었습니다.");if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ; Step.iCycle = 0 ; return true ;}
                 }
                 else {
                     if(m_dBfVisnX > OM.DevOptn.dStageTolXY  ){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}
                     if(m_dBfVisnY > OM.DevOptn.dStageTolXY  ){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}
                     if(m_dBfVisnT > OM.DevOptn.dStageTolAng ){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}

                     if(m_dBfVisnX < -OM.DevOptn.dStageTolXY ){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}
                     if(m_dBfVisnY < -OM.DevOptn.dStageTolXY ){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}
                     if(m_dBfVisnT < -OM.DevOptn.dStageTolAng){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}
                 }

                 dX    =  m_dBfVisnX;
                 dY    =  m_dBfVisnY;
                 dT    =  m_dBfVisnT;
                 dRadT =  dT*3.14159265358979323846/180.0;
                 //요기는 정규 공식.==================
                 dMoveX = dX*cos(dRadT) - dY*sin(dRadT) ;
                 dMoveY = dX*sin(dRadT) + dY*cos(dRadT) ;
                 dMoveT = dT ;
                 //======================================

                 dMoveT += OM.MstOptn.dPckToStgAngleOfs ;

                 MT_GoIncRun(miSTG_TStage , dMoveT );
                 MT_GoIncRun(miSTG_XStage , -dMoveY );
                 MT_GoIncRun(miSTG_YStage , -dMoveX );

                 Step.iCycle++;
                 return false ;

        case 23: if(!MT_GetStopInpos(miSTG_TStage)) return false ;
                 if(!MT_GetStopInpos(miSTG_XStage)) return false ;
                 if(!MT_GetStopInpos(miSTG_YStage)) return false ;
                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false ;

        case 24: if(!m_tmDelay.OnDelay(true , 100)) return false ;
                 //JS
                 if(!SKIP_VISN)SV_GrabInsp(viStageAlignBf);
                 //Step.iCycle=0;
                 //return true ;


                 Step.iCycle++;
                 return false ;

        case 25: //JS
                 if(!SKIP_VISN &&!SV_GetGrabEnd(viStageAlignBf)) return false ;
                 if(!SKIP_VISN &&!SV_GetInspEnd(viStageAlignBf)) return false ;

                 //JS
                 if(!SKIP_VISN)SV_GetRslt(viStageAlignBf , VisnRslt);
                 sVisnRslt = VisnRslt ;

                 if(sVisnRslt.Pos("OK")){                                 //"OK-0.23145,0.12344,0.32123
                     sVisnRslt.Delete(1,3);                               //0.23145,0.12344,0.32123
                     //X
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.23145
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));              //0.12344,0.32123
                     sItem.Delete(1,2);
                     m_dBfVisnX = StrToFloatDef(sItem , 0.0);

                     //Y
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.12344
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));
                     sItem.Delete(1,2);
                     m_dBfVisnY = StrToFloatDef(sItem , 0.0);

                     //T
                     sItem = sVisnRslt ;
                     sItem.Delete(1,2);
                     m_dBfVisnT = StrToFloatDef(sItem , 0.0);               //0.32123

                     m_bBfVisn = true ;
                 }
                 else {//"NG"
                     m_dBfVisnX = MT_GetTrgPos(miSTG_XStage) ;
                     m_dBfVisnY = MT_GetTrgPos(miSTG_YStage) ;
                     m_dBfVisnT = MT_GetTrgPos(miSTG_TStage) ;

                     m_bBfVisn = false ;

                     if(OM.CmnOptn.bVisnNgStop) {
                         EM_SetErrMsg(eiVSN_InspNG , "Before Attach Vision Inspection NG");
                         Step.iCycle=0;
                         return true ;
                     }
                     else {
                         DM.ARAY[riSTG].SetStat(csSkip);
                         //if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riSTG].SetStat(csSkip);
                         Step.iCycle=0;
                         return false  ;
                     }
                 }
                 if(OM.CmnOptn.bVisnNgStop) {
                     if(m_dBfVisnX > OM.DevOptn.dStageTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 X보정값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ;Step.iCycle = 0 ; return true ;}
                     if(m_dBfVisnY > OM.DevOptn.dStageTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 Y보정값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ;Step.iCycle = 0 ; return true ;}
                   //if(m_dBfVisnT > OM.DevOptn.dStageTolAng ){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 T보정값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ;Step.iCycle = 0 ; return true ;}

                     if(m_dBfVisnX < -OM.DevOptn.dStageTolXY ){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 X보정값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ;Step.iCycle = 0 ; return true ;}
                     if(m_dBfVisnY < -OM.DevOptn.dStageTolXY ){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 Y보정값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ;Step.iCycle = 0 ; return true ;}
                   //if(m_dBfVisnT < -OM.DevOptn.dStageTolAng){EM_SetErrMsg(eiVSN_InspNG , "Before Attach 비젼의 T보정값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnBfErrDisp=true ;Step.iCycle = 0 ; return true ;}
                 }
                 else {
                     if(m_dBfVisnX > OM.DevOptn.dStageTolXY  ){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}
                     if(m_dBfVisnY > OM.DevOptn.dStageTolXY  ){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}
                     //if(m_dBfVisnT > OM.DevOptn.dStageTolAng ){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}

                     if(m_dBfVisnX < -OM.DevOptn.dStageTolXY ){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}
                     if(m_dBfVisnY < -OM.DevOptn.dStageTolXY ){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}
                     //if(m_dBfVisnT < -OM.DevOptn.dStageTolAng){DM.ARAY[riSTG].SetStat(csSkip); Step.iCycle = 0 ; return true  ;}




                     //DM.ARAY[riSTG].SetStat(csSkip);
                     //if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riPCK].SetStat(csSkip);
                     //return false  ;
                 }

                 //MT_GoIncRun(miSTG_TStage ,  m_dBfVisnT );
                 MT_GoIncRun(miSTG_YStage , -m_dBfVisnX );
                 MT_GoIncRun(miSTG_XStage , -m_dBfVisnY );
                 Step.iCycle++;
                 return false ;

        case 26: //if(!MT_GetStopInpos(miSTG_TStage)) return false ;
                 if(!MT_GetStopInpos(miSTG_YStage)) return false ;
                 if(!MT_GetStopInpos(miSTG_XStage)) return false ;

                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false ;

        case 27: if(!m_tmDelay.OnDelay(true , 100)) return false ;
                 //JS
                 //비포 에프터 비교 위해 사진만 찍어 놓는다.
                 if(!SKIP_VISN)SV_GrabInsp(viStageAlignBf);
                 //Step.iCycle=0;
                 //return true ;


                 Step.iCycle++;
                 return false ;

        case 28: //JS
                 if(!SKIP_VISN &&!SV_GetGrabEnd(viStageAlignBf)) return false ;
                 if(!SKIP_VISN &&!SV_GetInspEnd(viStageAlignBf)) return false ;

                 //JS
                 if(!SKIP_VISN)SV_GetRslt(viStageAlignBf , VisnRslt);
                 sVisnRslt = VisnRslt ;

                 if(sVisnRslt.Pos("OK")){                                 //"OK-0.23145,0.12344,0.32123
                     sVisnRslt.Delete(1,3);                               //0.23145,0.12344,0.32123
                     //X
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.23145
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));              //0.12344,0.32123
                     sItem.Delete(1,2);
                     m_dBfVisnX = StrToFloatDef(sItem , 0.0);

                     //Y
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.12344
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));
                     sItem.Delete(1,2);
                     m_dBfVisnY = StrToFloatDef(sItem , 0.0);

                     //T
                     sItem = sVisnRslt ;
                     sItem.Delete(1,2);
                     m_dBfVisnT = StrToFloatDef(sItem , 0.0);               //0.32123

                     m_bBfVisn = true ;
                 }
                 else {//"NG"
                     m_dBfVisnX = 0 ;
                     m_dBfVisnY = 0 ;
                     m_dBfVisnT = 0 ;

                     m_bBfVisn = false ;
                     //여기는 그냥 비교만 한다.
                     //if(OM.CmnOptn.bVisnNgStop) {
                     //    EM_SetErrMsg(eiVSN_InspNG , "Before Attach Vision Inspection NG");
                     //    Step.iCycle=0;
                     //    return true ;
                     //}
                     //else {
                     //    DM.ARAY[riSTG].SetStat(csSkip);
                     //    //if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riSTG].SetStat(csSkip);
                     //    return false  ;
                     //}
                 }

                 //비포 에프터 포지션을 비교 하기 위해 비포에서
                 //포지션을 담아놨다가 에프터 검사할때 이리로 옮긴다.
                 m_dPkgCntX = MT_GetTrgPos(miSTG_XStage) ;
                 m_dPkgCntY = MT_GetTrgPos(miSTG_YStage) ;
                 m_dPkgCntT = MT_GetTrgPos(miSTG_TStage) ;


                 if(OM.DevInfo.iWorkMode == wmFos) DM.ARAY[riSTG].SetStat(csAttach);
                 else                              DM.ARAY[riSTG].SetStat(csDisp  );

                 //DIE 모드 일때는 비젼 끝나고 바로 패턴을 그리기 때문에.. 안간다.
                 if(MM.GetManNo() != mcDSPCycleVisnBf && OM.DevInfo.iWorkMode == wmFos){ //메뉴얼 아닐때에는 뒤로 빠져야 피커가 와서 작업 한다.
                     MoveMotr(miDSP_XDispr , piDSP_XDisprWait);
                     MoveMotr(miDSP_YDispr , piDSP_YDisprWait);
                 }

                 Step.iCycle++;
                 return false ;

        case 29: if(!MT_GetStopInpos(miDSP_XDispr)) return false ;
                 if(!MT_GetStopInpos(miDSP_YDispr)) return false ;

                 Step.iCycle = 0;
                 return true ;


    }
}



bool CDispensor::CycleDisp(void)
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s %s Step.iCycle=%03d %s",m_sPartName.c_str() , __FUNC__ , Step.iCycle , m_sCheckSafeMsg);
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiLDR_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        IO_SetY(yDSP_DispOn , false);
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) { //스탑시에 그냥 멈춤.
        //IO_SetY(yDSP_DispOn , false);
        //Step.iCycle = 0;
        //return true ;
    }

    //dVel, dAcc, dDec
    //값은 설정 할때 다시 해서 추가 하기..
    //20150629.  JS

    CSetInterpolation Interpol            ; // 보간 운동 셋팅
    Interpol.iMotorAixs[0] = miDSP_XDispr ;
    Interpol.iMotorAixs[1] = miDSP_YDispr ;  // 보간 운동할 모터 순서 설정
    Interpol.dEndPos[0]    = 0            ; // 마지막 포지션
    Interpol.dEndPos[1]    = 0            ; // 마지막 포지션
    Interpol.iCoord        = 0            ; // 좌표계 설정
    Interpol.iAxisCnt      = 2            ; // 모터 축 개수 설정
    Interpol.dwAbsRelMode  = 0            ; // 절대 모드 = 0, 상대 모드 = 1
    Interpol.dwProfileset  = 1            ; // 가감속 모드 설정, 일반 = 1, 노드 가감속 모드 = 2, 자동 가감속 모드 = 3
    Interpol.dVel          = PTT.GetSpeed(); // 모터 기본 속도
    Interpol.dAcc          = PTT.GetAcc() ; // 모터 가속도
    Interpol.dDec          = PTT.GetDec() ; // 모터 가감속도
    Interpol.iAngle        = 0            ;

    double dPosX ;
    double dPosY ;


    double dPckVisnX ;
    double dPckVisnY ;
    double dPckVisnT ;

    int    iTemp ;

    static int iRepeatCnt = 0;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: Rs232_Dispr.GetSylVolm(OM.DevOptn.iDspCh+1);
                 Step.iCycle++;
                 return false ;

        case 11: if(!Rs232_Dispr.GetMsgEnd()) return false ;
                 if(Rs232_Dispr.GetSylFill()< OM.DevOptn.iDspMinAmount){
                     EM_SetErrMsg(eiDSP_Amount , String(Rs232_Dispr.GetSylFill()).c_str());
                     Step.iCycle=0 ;
                     return true ;
                 }

                 m_iCrntDispNode = 0 ;
                 if(MM.GetManNo() == mcNoneCycle){
                     Step.iCycle = 30; //오토런이 면 높이 측정 하지 않는다.
                     return false;
                 }

                 STG.MoveMotr(miSTG_XStage , piSTG_XStagCenter);
                 STG.MoveMotr(miSTG_YStage , piSTG_YStagCenter);
                 STG.MoveMotr(miSTG_TStage , piSTG_TStagWork  );
                 Step.iCycle++;
                 return false;

        case 12: if(!STG.MoveMotr(miSTG_XStage , piSTG_XStagCenter)) return false;
                 if(!STG.MoveMotr(miSTG_YStage , piSTG_YStagCenter)) return false;
                 if(!STG.MoveMotr(miSTG_TStage , piSTG_TStagWork  )) return false;
                 STG.MoveMotr(miSTG_ZStage , piSTG_ZStagPick);

                 Step.iCycle++;
                 return false;

        case 13: if(!STG.MoveMotr(miSTG_ZStage , piSTG_ZStagPick)) return false;
                 //if(!MoveActr(aiDSP_NdlCvFwBw , ccFwd)) return false ;
                 IO_SetY(ySTG_Ejct , false);
                 IO_SetY(ySTG_Vccm , true );
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 14: if(m_tmDelay.OnDelay(true , 500)) return false ;
                 if(!IO_GetX(xSTG_Vccm)){
                     EM_SetErrMsg(eiSTG_PickMiss , "스테이지에 보트나 자제의 안착 상태를 확인하세요");
                     Step.iCycle = 0;
                     return true;
                 }
                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 Step.iCycle++ ;
                 return false ;

        case 15: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;
                 MoveMotr(miDSP_XDispr , piDSP_XDisprHtStage);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprHtStage);
                 STG.MoveMotr(miSTG_ZStage , piSTG_ZStagCheck);
                 Step.iCycle++ ;
                 return false ;

        case 16: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprHtStage)) return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprHtStage)) return false ;
                 if(!STG.MoveMotr(miSTG_ZStage , piSTG_ZStagCheck)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++ ;
                 return false ;

        case 17: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 Rs232Keyence.CheckHeight(kcChB);
                 Step.iCycle++ ;
                 return false ;

        case 18: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChB)) {
                     EM_SetErrMsg(eiETC_RS232 , "디스펜서 높이 측정기 측정포지션을 확인하세요.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 STG.SetPkgThickness(Rs232Keyence.GetHeightData(kcChB));
                 STG.SetFinishThickness(0.0);
                 STG.SetEpoxyThickness(0.0);


                 STG.MoveMotr(miSTG_ZStage , piSTG_ZStagWork);
                 Step.iCycle++ ;
                 return false ;

        case 19: if(!STG.MoveMotr(miSTG_ZStage , piSTG_ZStagWork)) return false;
                 Step.iCycle=30;
                 return false;

        //Manual 아닐때 시작부분.
        //위에서 사용.JS
        case 30: if(OM.DevOptn.bPkIgnrAlign) {
                     DM.ARAY[riSTG].SetStat(csVisn);
                     Step.iCycle=0;
                     return true ;
                 }

                 if(!MT_GetStop(miDSP_YDispr)) return false ; //마지막에 Push
                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 Step.iCycle++;
                 return false ;

        case 31: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;
                 //MoveMotr(miDSP_XDispr , piDSP_XDisprNdStageCntr);
                 //MoveMotr(miDSP_YDispr , piDSP_YDisprNdStageCntr);
                 Step.iCycle++;
                 return false;

        case 32: //if(!MoveMotr(miDSP_XDispr , piDSP_XDisprNdStageCntr))return false;
                 //if(!MoveMotr(miDSP_YDispr , piDSP_YDisprNdStageCntr))return false;
                 //패턴 오프셑 적용된 포지션으로 이동.
                 //MT_GoIncRun(miDSP_XDispr, PTT.GetSttOffsetX());
                 //MT_GoIncRun(miDSP_YDispr, PTT.GetSttOffsetY());
                 //그냥 첫번째 적용된 포지션으로 이동.
                 //MT_GoAbsRun(miDSP_XDispr , GetMotrPos(miDSP_XDispr , piDSP_XDisprNdStageCntr)+PTT.GetScalePosX(0));//+STG. + );
                 //MT_GoAbsRun(miDSP_YDispr , GetMotrPos(miDSP_YDispr , piDSP_YDisprNdStageCntr)+PTT.GetScalePosY(0));// + );
                 if(OM.DevInfo.iWorkMode == wmFos) {
                     MT_GoAbsRun(miDSP_XDispr , GetMotrPos(miDSP_XDispr , piDSP_XDisprNdStageCntr)+PTT.GetScalePosX(0)- PCK.GetVisnDataY());
                     MT_GoAbsRun(miDSP_YDispr , GetMotrPos(miDSP_YDispr , piDSP_YDisprNdStageCntr)+PTT.GetScalePosY(0)+ PCK.GetVisnDataX());
                 }
                 else { //다이 모드는 아직 픽커 의 결과값이 안나와있다.
                     MT_GoAbsRun(miDSP_XDispr , GetMotrPos(miDSP_XDispr , piDSP_XDisprNdStageCntr)+PTT.GetScalePosX(0)); //- PCK.GetVisnDataY());
                     MT_GoAbsRun(miDSP_YDispr , GetMotrPos(miDSP_YDispr , piDSP_YDisprNdStageCntr)+PTT.GetScalePosY(0)); //+ PCK.GetVisnDataX());
                     //Interpol.dEndPos[0] = GetMotrPos(miDSP_XDispr , piDSP_XDisprNdStageCntr) + dPosX ; //- PCK.GetVisnDataY();//+ PCK.GetVisnDataY();
                     //Interpol.dEndPos[1] = GetMotrPos(miDSP_YDispr , piDSP_YDisprNdStageCntr) + dPosY ; //+ PCK.GetVisnDataX();//+ PCK.GetVisnDataX();
                 }

                 MoveActr(aiDSP_NdlCvFwBw , ccFwd);


                 Step.iCycle++;
                 return false ;

        case 33: if(!MT_GetStopInpos(miDSP_XDispr)) return false;
                 if(!MT_GetStopInpos(miDSP_YDispr)) return false;
                 if(!MoveActr(aiDSP_NdlCvFwBw , ccFwd)) return false ;
                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprWork);
                 Step.iCycle++;
                 return false ;

        case 34: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprWork)) return  false ; // Work -> Move

                 m_tmDispr.Clear();

                 //m_tmDelay.Clear(); //첨에 토출 딜레이가 있어야함.

                 //Rs232_Dispr.SetDisprOnOff(true); // RS232 전용 디스펜서 On/Off

                 Step.iCycle++;
                 return false;

        case 35:
                 //if(!m_tmDelay.OnDelay(true,500)) return false ;

                 //패턴그리고.
                 MT_ContiSetAxisMap(Interpol.iCoord, Interpol.iAxisCnt, Interpol.iMotorAixs) ;
                 MT_ContiSetAbsRelMode(Interpol.iCoord, Interpol.dwAbsRelMode) ;
                 MT_ContiBeginNode(Interpol.iCoord) ;
                 for(int i = 0 ; i < PTT.GetPosCnt() ; i++) {
                     dPosX = PTT.GetScalePosX(i);
                     dPosY = PTT.GetScalePosY(i);

                     if(OM.DevInfo.iWorkMode == wmFos) {
                         Interpol.dEndPos[0] = GetMotrPos(miDSP_XDispr , piDSP_XDisprNdStageCntr) + dPosX - PCK.GetVisnDataY();//+ PCK.GetVisnDataY();
                         Interpol.dEndPos[1] = GetMotrPos(miDSP_YDispr , piDSP_YDisprNdStageCntr) + dPosY + PCK.GetVisnDataX();//+ PCK.GetVisnDataX();
                     }
                     else { //다이 모드는 아직 픽커 의 결과값이 안나와있다.
                         Interpol.dEndPos[0] = GetMotrPos(miDSP_XDispr , piDSP_XDisprNdStageCntr) + dPosX ; //- PCK.GetVisnDataY();//+ PCK.GetVisnDataY();
                         Interpol.dEndPos[1] = GetMotrPos(miDSP_YDispr , piDSP_YDisprNdStageCntr) + dPosY ; //+ PCK.GetVisnDataX();//+ PCK.GetVisnDataX();
                     }
                     MT_LineMove(Interpol.iCoord, Interpol.iMotorAixs, Interpol.dEndPos, Interpol.dVel, Interpol.dAcc, Interpol.dDec , Interpol.iAxisCnt) ;
                 }
                 MT_ContiEndNode(Interpol.iCoord);

                 MT_ContiSetAbsRelMode(Interpol.iCoord, Interpol.dwAbsRelMode) ; // 보간 구동 절대 모드 설정
                 MT_ContiStart(Interpol.iCoord, Interpol.dwProfileset, Interpol.iAngle) ; // 큐에 등록된 작업들을 시작하는 함수

                 IO_SetY(yDSP_DispOn , true ); // IO 전용 디스펜서 On/Off

                 //총 8개 넣으면 1~8까지 리턴이 되는듯 하다.
                 m_iCrntDispNode = MT_GetContCrntIdx(Interpol.iCoord)-1 ;
                 if(IO_GetY(yDSP_DispOn) != PTT.GetDispOn(m_iCrntDispNode)){
                     IO_SetY(yDSP_DispOn , PTT.GetDispOn(m_iCrntDispNode));
                     Trace("ContCrntIdx" , String(m_iCrntDispNode).c_str());
                     Trace("PTT.GetDispOn(ContCrntIdx)" , PTT.GetDispOn(m_iCrntDispNode) ? "Disp On":"Disp Off");
                 }



                 Step.iCycle++;
                 return false;

        case 36:
                 m_iCrntDispNode = MT_GetContCrntIdx(Interpol.iCoord)-1 ;
                 if(IO_GetY(yDSP_DispOn) != PTT.GetDispOn(m_iCrntDispNode)){
                     IO_SetY(yDSP_DispOn , PTT.GetDispOn(m_iCrntDispNode));
                     Trace("ContCrntIdx" , String(m_iCrntDispNode).c_str());
                     Trace("PTT.GetDispOn(ContCrntIdx)" , PTT.GetDispOn(m_iCrntDispNode) ? "Disp On":"Disp Off");
                 }


                 if(!MT_GetStopInpos(miDSP_XDispr)) return false;
                 if(!MT_GetStopInpos(miDSP_YDispr)) return false;
                 //Rs232_Dispr.SetDisprOnOff(false); // RS232 전용 디스펜서 On/Off
                 SetDisprTime(m_tmDispr.OnCheckCycle());
                 IO_SetY(yDSP_DispOn , false ); // IO 전용 디스펜서 On/Off
                 iRepeatCnt = 0;
                 m_iReptCnt = iRepeatCnt;
                 Step.iCycle++;
                 return false;

        case 37: if(OM.DevOptn.iReptCnt < 0 || OM.DevInfo.iWorkMode == wmDie){
                     Step.iCycle = 39;
                     return false;
                 }
                 MT_GoAbs(miDSP_XDispr , GetMotrPos(miDSP_XDispr , piDSP_XDisprNdStageCntr) + OM.DevOptn.dReptSttXPos - PCK.GetVisnDataY(), OM.DevOptn.dReptSpd);
                 MT_GoAbs(miDSP_YDispr , GetMotrPos(miDSP_YDispr , piDSP_YDisprNdStageCntr) + OM.DevOptn.dReptSttYPos + PCK.GetVisnDataX(), OM.DevOptn.dReptSpd);
                 Step.iCycle++;
                 return false;

        case 38: if(!MT_GetStopInpos(miDSP_XDispr)) return false;
                 if(!MT_GetStopInpos(miDSP_YDispr)) return false;
                 MT_GoAbs(miDSP_XDispr , GetMotrPos(miDSP_XDispr , piDSP_XDisprNdStageCntr) + OM.DevOptn.dReptEndXPos - PCK.GetVisnDataY(), OM.DevOptn.dReptSpd);
                 MT_GoAbs(miDSP_YDispr , GetMotrPos(miDSP_YDispr , piDSP_YDisprNdStageCntr) + OM.DevOptn.dReptEndYPos + PCK.GetVisnDataX(), OM.DevOptn.dReptSpd);
                 Step.iCycle++;
                 return false;

        case 39: if(!MT_GetStopInpos(miDSP_XDispr)) return false;
                 if(!MT_GetStopInpos(miDSP_YDispr)) return false;
                 if(iRepeatCnt < OM.DevOptn.iReptCnt){
                     iRepeatCnt++;
                     m_iReptCnt = iRepeatCnt;
                     Step.iCycle = 37;
                     return false;
                 }
                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 Step.iCycle++;
                 return false ;

        case 40: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;

                 if(OM.DevInfo.iWorkMode == wmFos)  DM.ARAY[riSTG].SetStat(csPush  );
                 else                               DM.ARAY[riSTG].SetStat(csAttach);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprWait);
                 MoveActr(aiDSP_NdlCvFwBw , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 41: if(!MoveMotr(miDSP_YDispr , piDSP_YDisprWait)) return false ;
                 if(!MoveActr(aiDSP_NdlCvFwBw , ccBwd)) return false ;
                 Step.iCycle = 0;
                 return true ;
    }
}

bool CDispensor::CycleVisnAt(void)
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

    char VisnRslt[64];
    String sVisnRslt ;
    String sItem ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: m_dAtVisnX = 0 ;
                 m_dAtVisnY = 0 ;
                 m_dAtVisnT = 0 ;
                 m_bAtVisn  = true ;


                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 Step.iCycle++;
                 return false ;

        case 11: //메뉴얼시에 충돌 위치에 있으면 치운다.
                 if(MM.GetManNo() != scNone && !PCK.ToolVoid()) return false ;
                 if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;

                 //MT_GetTrgPos(miSTG_ZStage) <=


                 MoveMotr(miDSP_XDispr , piDSP_XDisprHtStageCntr);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprHtStageCntr);
                 STG.MoveMotr(miSTG_ZStage , piSTG_ZStagCheck);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprHtStageCntr)) return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprHtStageCntr)) return false ;
                 if(!STG.MoveMotr(miSTG_ZStage , piSTG_ZStagCheck)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 13: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 Rs232Keyence.CheckHeight(kcChB);
                 Step.iCycle++;
                 return false ;

        case 14: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChB)) {
                     EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 측정포지션을 확인하세요.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 STG.SetFinishThickness(Rs232Keyence.GetHeightData(kcChB));
                 STG.SetEpoxyThickness(Rs232Keyence.GetHeightData(kcChB) - STG.GetPkgThickness() - PCK.GetPkgThickness());
                 STG.MoveMotr(miSTG_ZStage , piSTG_ZStagWork);
                 Step.iCycle++;
                 return false ;

        case 15: if(OM.DevOptn.bStIgnrVisnAfter || m_bOneSkipVisnAt){
                     m_dAtVisnX = 0 ;
                     m_dAtVisnY = 0 ;
                     m_dAtVisnT = 0 ;
                     m_bAtVisn  = true ;

                     DM.ARAY[riSTG].SetStat(csWorkEnd);
                     m_bOneSkipVisnAt = false;
                     Step.iCycle=0;
                     return true ;
                 }
                 //JS
                 if(!SKIP_VISN)SV_Ready(viStageAlignAt);
                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);

                 if(MM.GetManNo() == mcVisnCal){
                     STG.MoveMotr(miSTG_XStage , piSTG_XStagCenter);
                     STG.MoveMotr(miSTG_YStage , piSTG_YStagCenter);
                     STG.MoveMotr(miSTG_TStage , piSTG_TStagWork  );
                 }
                 else{
                     MT_GoAbsRun(miSTG_XStage , m_dPkgCntX);//+OM.DevOptn.dPlaceXOfs);
                     MT_GoAbsRun(miSTG_YStage , m_dPkgCntY);//+OM.DevOptn.dPlaceYOfs);
                     MT_GoAbsRun(miSTG_TStage , m_dPkgCntT);
                 }

                 Step.iCycle++;
                 return false ;

        case 16: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;
                 if(!MT_GetStopInpos(miSTG_XStage)) return false ;
                 if(!MT_GetStopInpos(miSTG_YStage)) return false ;
                 if(!MT_GetStopInpos(miSTG_TStage)) return false ;





                 if(MM.GetManNo() == mcVisnCal){
                     //백레쉬 때문에 ㅜㅠ
                     MT_GoAbsRun(miDSP_XDispr , GetMotrPos(miDSP_XDispr , piDSP_XDisprVsStageCntr)-0.2);
                     MT_GoAbsRun(miDSP_YDispr , GetMotrPos(miDSP_YDispr , piDSP_YDisprVsStageCntr)-0.2);
                 }
                 else{
                     MT_GoAbsRun(miDSP_XDispr , GetMotrPos(miDSP_XDispr , piDSP_XDisprVsStageCntr) - OM.DevOptn.dPlaceXOfs -0.2);
                     MT_GoAbsRun(miDSP_YDispr , GetMotrPos(miDSP_YDispr , piDSP_YDisprVsStageCntr) - OM.DevOptn.dPlaceYOfs -0.2);
                 }

                 Step.iCycle++;
                 return false ;

        case 17: if(!MT_GetStop(miDSP_XDispr))return false ;
                 if(!MT_GetStop(miDSP_YDispr))return false ;




                 //MoveMotr(miDSP_XDispr , piDSP_XDisprVsStageCntr);
                 //MoveMotr(miDSP_YDispr , piDSP_YDisprVsStageCntr);
                 if(MM.GetManNo() == mcVisnCal){
                     MoveMotr(miDSP_XDispr , piDSP_XDisprVsStageCntr);
                     MoveMotr(miDSP_YDispr , piDSP_YDisprVsStageCntr);
                 }
                 else{
                     MT_GoAbsRun(miDSP_XDispr , GetMotrPos(miDSP_XDispr , piDSP_XDisprVsStageCntr) - OM.DevOptn.dPlaceXOfs);
                     MT_GoAbsRun(miDSP_YDispr , GetMotrPos(miDSP_YDispr , piDSP_YDisprVsStageCntr) - OM.DevOptn.dPlaceYOfs);
                 }

                 Step.iCycle++;
                 return false ;

        case 18: //if(!MoveMotr(miDSP_XDispr , piDSP_XDisprVsStageCntr)) return false ;
                 //if(!MoveMotr(miDSP_YDispr , piDSP_YDisprVsStageCntr)) return false ;
                 if(!MT_GetStopInpos(miDSP_XDispr)) return false;
                 if(!MT_GetStopInpos(miDSP_YDispr)) return false;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 19: if(!m_tmDelay.OnDelay(true , 100)) return false ;
                 //JS
                 if(!SKIP_VISN){
                     SV_SetPkgID(viStageAlignAt , DM.ARAY[riSTG].CHPS[0][0].GetID().c_str());
                     SV_GrabInsp(viStageAlignAt);

                 }
                 Step.iCycle++;
                 return false ;

        case 20: //JS
                 //if(!SKIP_VISN && !SV_GetInspEnd(viStageAlignAt)) return false ;

                 if(!SKIP_VISN &&!SV_GetGrabEnd(viStageAlignAt)) return false ;
                 if(!SKIP_VISN &&!SV_GetInspEnd(viStageAlignAt)) return false ;
                 //JS
                 if(!SKIP_VISN)SV_GetRslt(viStageAlignAt , VisnRslt);
                 sVisnRslt = VisnRslt ;
                 if(sVisnRslt.Pos("NG")){
                     if(OM.CmnOptn.bVisnNgStop) {
                         EM_SetErrMsg(eiVSN_InspNG , "After Attach Vision Inspection Failed");
                         if(MM.GetManNo() == mcNone) m_bVsnAtErrDisp = true ;
                         Step.iCycle=0;
                         return true ;
                     }
                     else {
                         DM.ARAY[riSTG].SetStat(csVisnNG);
                         //if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riPCK].SetStat(csWorkEnd);
                         Step.iCycle=0; //50번으로 가서 피커 놀고 있는지 확인 하고 마스킹 해야함.
                         return true  ;
                     }
                 }
                 else if(sVisnRslt.Pos("OK")){                                 //"OK-0.23145,0.12344,0.32123
                     sVisnRslt.Delete(1,3);                               //0.23145,0.12344,0.32123
                     //X
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.23145
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));              //0.12344,0.32123
                     sItem.Delete(1,2);
                     m_dAtVisnX = StrToFloatDef(sItem , 0.0);

                     //Y
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.12344
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));
                     sItem.Delete(1,2);
                     m_dAtVisnY = StrToFloatDef(sItem , 0.0);

                     //T
                     sItem = sVisnRslt ;
                     sItem.Delete(1,2);
                     m_dAtVisnT = StrToFloatDef(sItem , 0.0);               //0.32123
                     sItem.Delete(1,2);

                     m_bAtVisn = true ;
                 }
                 else {//"NG"
                     EM_SetErrMsg(eiVSN_InspNG , "After Attach Vision Inspection NG");
                     if(MM.GetManNo() == mcNone) m_bVsnAtErrDisp=true ;
                     Step.iCycle = 0;
                     return true ;
                 }

                 if(OM.CmnOptn.bVisnNgStop) {
                     if(m_dAtVisnX > OM.DevOptn.dPlaceTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "After Attach 비젼의 X값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnAtErrDisp=true ;Step.iCycle = 0 ; return true ;}
                     if(m_dAtVisnY > OM.DevOptn.dPlaceTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "After Attach 비젼의 Y값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnAtErrDisp=true ;Step.iCycle = 0 ; return true ;}
                     if(m_dAtVisnT > OM.DevOptn.dPlaceTolAng ){EM_SetErrMsg(eiVSN_InspNG , "After Attach 비젼의 T값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnAtErrDisp=true ;Step.iCycle = 0 ; return true ;}

                     if(m_dAtVisnX < -OM.DevOptn.dPlaceTolXY ){EM_SetErrMsg(eiVSN_InspNG , "After Attach 비젼의 X값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnAtErrDisp=true ;Step.iCycle = 0 ; return true ;}
                     if(m_dAtVisnY < -OM.DevOptn.dPlaceTolXY ){EM_SetErrMsg(eiVSN_InspNG , "After Attach 비젼의 Y값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnAtErrDisp=true ;Step.iCycle = 0 ; return true ;}
                     if(m_dAtVisnT < -OM.DevOptn.dPlaceTolAng){EM_SetErrMsg(eiVSN_InspNG , "After Attach 비젼의 T값이 설정범위를 넘었습니다."); if(MM.GetManNo() == mcNone) m_bVsnAtErrDisp=true ;Step.iCycle = 0 ; return true ;}
                 }
                 else {
                     if(m_dAtVisnX > OM.DevOptn.dPlaceTolXY  ){DM.ARAY[riSTG].SetStat(csVisnNG);Step.iCycle=0;return true  ;}
                     if(m_dAtVisnY > OM.DevOptn.dPlaceTolXY  ){DM.ARAY[riSTG].SetStat(csVisnNG);Step.iCycle=0;return true  ;}
                     if(m_dAtVisnT > OM.DevOptn.dPlaceTolAng ){DM.ARAY[riSTG].SetStat(csVisnNG);Step.iCycle=0;return true  ;}

                     if(m_dAtVisnX < -OM.DevOptn.dPlaceTolXY ){DM.ARAY[riSTG].SetStat(csVisnNG);Step.iCycle=0;return true  ;}
                     if(m_dAtVisnY < -OM.DevOptn.dPlaceTolXY ){DM.ARAY[riSTG].SetStat(csVisnNG);Step.iCycle=0;return true  ;}
                     if(m_dAtVisnT < -OM.DevOptn.dPlaceTolAng){DM.ARAY[riSTG].SetStat(csVisnNG);Step.iCycle=0;return true  ;}


                     //if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riPCK].SetStat(csWorkEnd);
                      //50번으로 가서 피커 놀고 있는지 확인 하고 마스킹 해야함.

                 }

                 MoveMotr(miDSP_XDispr , piDSP_XDisprWait);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprWait);
                 DM.ARAY[riSTG].SetStat(csWorkEnd);
                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprWait)) return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprWait)) return false ;

                 //DM.ARAY[riSTG].SetData(0, 0, diXData     , m_dAtVisnX             );
                 //DM.ARAY[riSTG].SetData(0, 0, diYData     , m_dAtVisnY             );
                 //DM.ARAY[riSTG].SetData(0, 0, diEpxThckns , STG.GetEpoxyThickness());
                 Step.iCycle = 0;
                 return true ;
    }

}

bool CDispensor::CycleClean(void)
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

    char VisnRslt[64];
    String sVisnRslt ;
    String sItem ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: if(!MT_GetStop(miDSP_YDispr)) return false ; //마지막에 Push
                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;
                 MoveMotr(miDSP_XDispr , piDSP_XDisprNdClean);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprNdClean);
                 MoveActr(aiDSP_NdlCvFwBw , ccFwd);
                 MoveActr(aiDSP_NdlCrFwBw , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprNdClean)) return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprNdClean)) return false ;
                 if(!MoveActr(aiDSP_NdlCvFwBw , ccFwd)) return false ;
                 if(!MoveActr(aiDSP_NdlCrFwBw , ccBwd)) return false ;
                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprClean);

                 //시그마 모드에서는 사용 못함.
                 //Rs232_Dispr.SetPTV(OM.DevOptn.dDspPrsPres, OM.DevOptn.dDspVacPres , 10);
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprClean)) return false ;
                 if(!m_tmDelay.OnDelay(true , 500)) return false ;
                 m_tmDelay.Clear();
                 IO_SetY(yDSP_DispOn , true ); // IO 전용 디스펜서 On/Off
                 Step.iCycle++;
                 return false;

        case 14: if(!m_tmDelay.OnDelay(true , 3000)) return false;
                 IO_SetY(yDSP_DispOn , false ); // IO 전용 디스펜서 On/Off
                 //Rs232_Dispr.SetPTV(OM.DevOptn.dDspPrsPres, OM.DevOptn.dDspVacPres , 10);
                 MoveActr(aiDSP_NdlCrFwBw , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 15: if(!MoveActr(aiDSP_NdlCrFwBw , ccFwd)) return false;
                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 Step.iCycle++;
                 return false ;

        case 16: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false;
                 MoveActr(aiDSP_NdlCrFwBw , ccBwd);
                 MoveActr(aiDSP_NdlCvFwBw , ccBwd);
                 Step.iCycle++;
                 return false;

        case 17: if(!MoveActr(aiDSP_NdlCrFwBw , ccBwd))return false ;
                 if(!MoveActr(aiDSP_NdlCvFwBw , ccBwd)) return false ;
                 m_iDisprBoatCnt = 0;
                 Step.iCycle = 0;
                 return true ;
    }
}

bool CDispensor::CycleManStdCal()
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

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push
                 //IO_SetY(ySTG_Ejct , false);
                 //IO_SetY(ySTG_Vccm , true );
                 //m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 11: //if(m_tmDelay.OnDelay(true , 500)) return false;
                 //if(IO_GetX(xSTG_Vccm)){
                 //    EM_SetErrMsg(eiSTG_RemovePkg , "작업 Stage에 자제와 화면에 데이터를 제거하여 주십시오");
                 //    Step.iCycle = 0;
                 //    return true;
                 //}
                 //else{
                 //    IO_SetY(ySTG_Ejct , false);
                 //    IO_SetY(ySTG_Vccm , false);
                 //}
                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;
                 if(!PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;

                 PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrAlign);
                 Step.iCycle++;
                 return false ;

        case 13: if(!PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrAlign)) return false ;
                 MoveMotr(miDSP_XDispr , piDSP_XDisprHtCheck);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprHtCheck);

                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprHtCheck)) return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprHtCheck)) return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 15: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                 Rs232Keyence.SetZero(kcChB);
                 Step.iCycle++;
                 return false ;

        case 16: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 MoveMotr(miDSP_XDispr , piDSP_XDisprNdCheck);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprNdCheck);
                 MoveActr(aiDSP_NdlCvFwBw , ccFwd);

                 Step.iCycle++;
                 return false ;

        case 17: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprNdCheck)) return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprNdCheck)) return false ;
                 if(!MoveActr(aiDSP_NdlCvFwBw , ccFwd)) return false ;

                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprCheck);

                 Step.iCycle++ ;
                 return false ;

        case 18: if(!IO_GetX(xDSP_NdleNotCheck)) {//기존체크위치 가다가 체크되면 스탑.
                     MT_EmgStop(miDSP_ZDispr);
                 }
                 if(!MT_GetStop(miDSP_ZDispr)) return false ;
                 if(IO_GetX(xDSP_NdleNotCheck)){//체크센서 감지 안되면 다시 3미리 보내 본다.
                     MT_GoInc(miDSP_ZDispr , 3.0 , 1.0);
                 }
                 Step.iCycle++ ;
                 return false ;

        case 19: if(!IO_GetX(xDSP_NdleNotCheck)) {//기존체크위치 가다가 체크되면 스탑.
                     MT_EmgStop(miDSP_ZDispr);
                 }
                 if(!MT_GetStop(miDSP_ZDispr)) return false ;
                 if(IO_GetX(xDSP_NdleNotCheck)){
                     Step.iCycle = 40;
                     return false;
                 }

                 //체크 완료 되면 뒤로 빼면서...
                 //체크 해제
                 MT_GoInc(miDSP_ZDispr , -10.0 , 0.1);
                 Step.iCycle++;
                 return false ;

        case 20: if(IO_GetX(xDSP_NdleNotCheck)) {//기존체크위치 가다가 체크되면 스탑.
                     MT_EmgStop(miDSP_ZDispr);
                 }
                 if(!MT_GetStop(miDSP_ZDispr)) return false ;

                 PM.SetValue(miDSP_ZDispr , pvDSP_ZDisprCheck , MT_GetCmdPos(miDSP_ZDispr));
                 m_bRqstPMSave = true ;

                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprWait);
                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprWait)) return false ;
                 MoveMotr(miDSP_XDispr , piDSP_XDisprHtStage);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprHtStage);

                 STG.MoveMotr(miSTG_XStage , piSTG_XStagCenter);
                 STG.MoveMotr(miSTG_YStage , piSTG_YStagCenter);
                 STG.MoveMotr(miSTG_TStage , piSTG_TStagWork  );
                 MoveActr(aiDSP_NdlCvFwBw , ccBwd);


                 Step.iCycle++;
                 return false ;

        case 22: if(!STG.MoveMotr(miSTG_XStage , piSTG_XStagCenter)) return false ;
                 if(!STG.MoveMotr(miSTG_YStage , piSTG_YStagCenter)) return false ;
                 if(!STG.MoveMotr(miSTG_TStage , piSTG_TStagWork  )) return false ;
                 if(!MoveActr(aiDSP_NdlCvFwBw , ccBwd)) return false ;

                 STG.MoveMotr(miSTG_ZStage , piSTG_ZStagPick);

                 Step.iCycle++;
                 return false;

        case 23: if(!STG.MoveMotr(miSTG_ZStage , piSTG_ZStagPick)) return false ;
                 m_tmDelay.Clear();
                 IO_SetY(ySTG_Ejct , false);
                 IO_SetY(ySTG_Vccm , true );
                 Step.iCycle++;
                 return false ;

        case 24: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 if(IO_GetX(xSTG_Vccm)){
                     EM_SetErrMsg(eiSTG_RemovePkg , "작업 Stage에 자제를 제거하여 주십시오");
                     Step.iCycle = 0;
                     return true;
                 }
                 else{
                     IO_SetY(ySTG_Ejct , false);
                     IO_SetY(ySTG_Vccm , false);
                 }

                 STG.MoveMotr(miSTG_ZStage , piSTG_ZStagCheck);


                 Step.iCycle++;
                 return false ;

        case 25: if(!STG.MoveMotr(miSTG_ZStage , piSTG_ZStagCheck)) return false ;
                 if(!MoveMotr(miDSP_XDispr , piDSP_XDisprHtStage)) return false;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprHtStage)) return false;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 26: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                 Rs232Keyence.CheckHeight(kcChB);
                 Step.iCycle++;
                 return false ;

        case 27: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 MT_GoIncMan(miSTG_ZStage , -Rs232Keyence.GetHeightData(kcChB)) ;
                 Step.iCycle++;
                 return false ;

        case 28: if(!MT_GetStopInpos(miSTG_ZStage)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 29: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                 PM.SetValue(miSTG_ZStage , pvSTG_ZStagCheck , MT_GetCmdPos(miSTG_ZStage));
                 MoveMotr(miDSP_XDispr , piDSP_XDisprNdClean);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprNdClean);
                 Step.iCycle++;
                 return false;

        case 30: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprNdClean)) return false;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprNdClean)) return false;
                 STG.MoveMotr(miSTG_ZStage , piSTG_ZStagWait);
                 Step.iCycle++;
                 return false;

        case 31: if(!STG.MoveMotr(miSTG_ZStage , piSTG_ZStagWait)) return false;
                 FM_MsgOk("Check","니들 터치 센서를 닦아주세요");
                 m_bNiddleChecked = true ;
                 Step.iCycle=0;
                 return true ;

        //Error시에 대기 위치로 올라거서 Stop.
        case 40: MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 Step.iCycle++;
                 return false;

        case 41: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false;
                 MoveActr(aiDSP_NdlCvFwBw , ccBwd);
                 EM_SetErrMsg(eiPRT_CheckErr, "Dispensor가 높이 세팅을 하지 못하였습니다.");
                 Step.iCycle++;
                 return false ;

        case 42: if(!MoveActr(aiDSP_NdlCvFwBw , ccFwd)) return false ;
                 Step.iCycle = 0;
                 return true;
    }
}


bool CDispensor::CycleManNdlCheck()
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

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push
                 //메뉴얼시에 충돌 위치에 있으면 치운다.
                 if(MM.GetManNo() != scNone && !PCK.ToolVoid()) return false ;
                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);

                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;
                 if(!PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;

                 PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrAlign);
                 Step.iCycle++;
                 return false ;

        case 12: if(!PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrAlign)) return false ;
                 MoveMotr(miDSP_XDispr , piDSP_XDisprNdCheck);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprNdCheck);
                 MoveActr(aiDSP_NdlCvFwBw , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprNdCheck)) return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprNdCheck)) return false ;
                 if(!MoveActr(aiDSP_NdlCvFwBw , ccFwd)) return false ;
                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprCheck, true);
                 Step.iCycle++ ;
                 return false ;

        case 14: if(!IO_GetX(xDSP_NdleNotCheck)) {//기존체크위치 가다가 체크되면 스탑.
                     MT_EmgStop(miDSP_ZDispr);
                 }
                 if(!MT_GetStop(miDSP_ZDispr)) return false ;
                 if(IO_GetX(xDSP_NdleNotCheck)){//체크센서 감지 안되면 다시 3미리 보내 본다.
                     MT_GoInc(miDSP_ZDispr , 3.0 , 1.0);
                 }
                 Step.iCycle++ ;
                 return false ;

        case 15: if(!IO_GetX(xDSP_NdleNotCheck)) {//기존체크위치 가다가 체크되면 스탑.
                     MT_EmgStop(miDSP_ZDispr);
                 }
                 if(!MT_GetStop(miDSP_ZDispr)) return false ;

                 if(IO_GetX(xDSP_NdleNotCheck)){
                     Step.iCycle = 30;
                     return false;
                 }

                 //체크 완료 되면 뒤로 빼면서...
                 //체크 해제
                 MT_GoInc(miDSP_ZDispr , -10.0 , 0.1);
                 Step.iCycle++;
                 return false ;

        case 16: if(IO_GetX(xDSP_NdleNotCheck)) {//기존체크위치 가다가 체크되면 스탑.
                     MT_EmgStop(miDSP_ZDispr);
                 }
                 if(!MT_GetStop(miDSP_ZDispr)) return false ;

                 PM.SetValue(miDSP_ZDispr , pvDSP_ZDisprCheck , MT_GetCmdPos(miDSP_ZDispr));

                 MoveMotr(miDSP_ZDispr , piDSP_ZDisprWait);
                 Step.iCycle++;
                 return false ;

        case 17: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprWait)) return false ;
                 MoveMotr(miDSP_XDispr , piDSP_XDisprWait);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprWait);
                 MoveActr(aiDSP_NdlCvFwBw , ccBwd);
                 Step.iCycle++;
                 return false;

        case 18: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprWait)) return false;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprWait)) return false;
                 if(!MoveActr(aiDSP_NdlCvFwBw , ccBwd))return false ;


                 FM_MsgOk("Check","니들 터치 센서를 꼭 닦아주세요");
                 m_bNiddleChecked = true ;
                 Step.iCycle=0;
                 return true;

        //Error시에 대기 위치로 올라거서 Stop.
        case 30: MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 Step.iCycle++;
                 return false;

        case 31: if(!MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false;
                 EM_SetErrMsg(eiPRT_CheckErr, "Dispensor가 높이 세팅을 하지 못하였습니다.");
                 MoveActr(aiDSP_NdlCvFwBw , ccBwd);
                 Step.iCycle++;
                 return false;

        case 32: if(!MoveActr(aiDSP_NdlCvFwBw , ccBwd)) return false;
                 Step.iCycle = 0;
                 return true;
    }
}






bool CDispensor::CycleManVisnPos(void)
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

     switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //메뉴얼시에 충돌 위치에 있으면 치운다.
                 if(MM.GetManNo() != scNone && !PCK.ToolVoid()) return false ;

                 MoveMotr(miDSP_ZDispr, piDSP_ZDisprMove);
                 //PCK.MoveMotr(miPCK_ZPickr, piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miDSP_ZDispr, piDSP_ZDisprMove)) return false ;
                 //if(!PCK.MoveMotr(miPCK_ZPickr, piPCK_ZPickrMove)) return false ;
                 //PCK.MoveMotr(miPCK_YPickr, piPCK_YPickrAlign);
                 Step.iCycle++;
                 return false ;

        case 12:
                 //if(!PCK.MoveMotr(miPCK_YPickr, piPCK_YPickrAlign)) return false ;
                 //MoveMotr(miDSP_YDispr, piDSP_YDisprHtCheck);
                 Step.iCycle++;
                 return false ;

        case 13: //if(!MoveMotr(miDSP_YDispr, piDSP_YDisprHtCheck)) return false ;
                 //m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 14: //if(!m_tmDelay.OnDelay(true , 300)) return false;
                 //Rs232Keyence.SetZero(kcChB);
                 Step.iCycle++;
                 return false ;

        case 15: MoveMotr(miDSP_YDispr, piDSP_YDisprHtStage);
                 MoveMotr(miDSP_XDispr, piDSP_XDisprHtStage);

                 STG.MoveMotr(miSTG_ZStage, piSTG_ZStagCheck );
                 STG.MoveMotr(miSTG_XStage, piSTG_XStagCenter);
                 STG.MoveMotr(miSTG_YStage, piSTG_YStagCenter);
                 STG.MoveMotr(miSTG_TStage, piSTG_TStagWork  );
                 Step.iCycle++;
                 return false ;

        case 16: if(!MoveMotr(miDSP_YDispr, piDSP_YDisprHtStage  )) return false ;
                 if(!MoveMotr(miDSP_XDispr, piDSP_XDisprHtStage  )) return false ;
                 if(!STG.MoveMotr(miSTG_ZStage, piSTG_ZStagCheck )) return false ;
                 if(!STG.MoveMotr(miSTG_XStage, piSTG_XStagCenter)) return false ;
                 if(!STG.MoveMotr(miSTG_YStage, piSTG_YStagCenter)) return false ;
                 if(!STG.MoveMotr(miSTG_TStage, piSTG_TStagWork  )) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 17: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 Rs232Keyence.CheckHeight(kcChB);
                 Step.iCycle++;
                 return false ;

        case 18: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 MT_GoIncMan(miSTG_ZStage , -Rs232Keyence.GetHeightData(kcChB)) ;
                 Step.iCycle++;
                 return false ;

        case 19: if(!MT_GetStopInpos(miSTG_ZStage)) return false ;
                 //PM.SetValue(miSTG_ZStage , pvSTG_ZStagCheck , MT_GetCmdPos(miSTG_ZStage));
                 //백레쉬 때문에 ㅜㅠ
                 MT_GoAbsRun(miDSP_XDispr , GetMotrPos(miDSP_XDispr , piDSP_XDisprVsStageCntr)-0.2);
                 MT_GoAbsRun(miDSP_YDispr , GetMotrPos(miDSP_YDispr , piDSP_YDisprVsStageCntr)-0.2);
                 Step.iCycle++;
                 return false ;

        case 20: if(!MT_GetStop(miDSP_XDispr)) return false ;
                 if(!MT_GetStop(miDSP_YDispr)) return false ;

                 MoveMotr(miDSP_XDispr , piDSP_XDisprVsStageCntr);
                 MoveMotr(miDSP_YDispr , piDSP_YDisprVsStageCntr);
                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveMotr(miDSP_XDispr , piDSP_XDisprVsStageCntr)) return false ;
                 if(!MoveMotr(miDSP_YDispr , piDSP_YDisprVsStageCntr)) return false ;
                 Step.iCycle = 0;
                 return true;
    }
}
bool CDispensor::CycleManSlnChange(void)
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

     switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: MoveMotr(miDSP_ZDispr, piDSP_ZDisprMove);
                 PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miDSP_ZDispr, piDSP_ZDisprMove)) return false;
                 if(!PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false;
                 PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrPick);
                 Step.iCycle++;
                 return false;

        case 12: if(!PCK.MoveMotr(miPCK_YPickr , piPCK_YPickrPick)) return false;

                 // 디스펜서 X, Y가 모두 400, 35 값이면 Wait 포지션으로
                 // 그렇지 않으면 400, 35 포지션 위치로 이동
                 if(MT_GetCmdPos(miDSP_YDispr) == 400 && MT_GetCmdPos(miDSP_XDispr) == 35) {
                     MoveMotr(miDSP_YDispr, piDSP_YDisprWait);
                     MoveMotr(miDSP_XDispr, piDSP_XDisprWait);
                 }
                 else {
                     MT_GoAbsMan(miDSP_YDispr , 400);
                     MT_GoAbsMan(miDSP_XDispr , 35 );
                 }
                 Step.iCycle++;
                 return false;

        case 13: if(!MT_GetStopInpos(miDSP_YDispr)) return false;
                 if(!MT_GetStopInpos(miDSP_XDispr)) return false;
                 Step.iCycle = 0;
                 return true;
    }
}

bool CDispensor::ToolVoid() //디스펜서 일할때 피해주기.
{
    bool bZPosOk = MT_GetTrgPos(miDSP_ZDispr) <= PM.GetValue(miDSP_ZDispr , pvDSP_ZDisprMove ) && MT_GetStopInpos(miDSP_ZDispr) ;
    bool bYPosOk = MT_GetTrgPos(miDSP_YDispr) <= PM.GetValue(miDSP_YDispr , pvDSP_YDisprWait ) && MT_GetStopInpos(miDSP_YDispr) ;

    bool bRetZ = true ;
    bool bRetY = true ;

    if(!bZPosOk) {
        bRetZ = MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
    }
    else {
        bRetZ = true ;
    }
    if(!bRetZ) return false ;

    if(!bYPosOk) {
        bRetY = MoveMotr(miDSP_YDispr , piDSP_YDisprWait);
    }
    else {
        bRetY = true ;
    }
    return bRetY ;
}




//---------------------------------------------------------------------------
bool CDispensor::GetVisnRslt(double * _dVisnX , double * _dVisnY , double * _dVisnT , bool _bAt)
{
    if(!_bAt){
        * _dVisnX = m_dBfVisnX ;
        * _dVisnY = m_dBfVisnY ;
        * _dVisnT = m_dBfVisnT ;
    }
    else{
        * _dVisnX = m_dAtVisnX ;
        * _dVisnY = m_dAtVisnY ;
        * _dVisnT = m_dAtVisnT ;
    }

    return m_bBfVisn ;
}


//---------------------------------------------------------------------------
bool CDispensor::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miDSP_ZDispr){
        if(!MT_GetStop(miDSP_YDispr)){
            sMsg = MT_GetName(miDSP_YDispr) + String(" 모터가 이동중 입니다.") ;
            bRet = false ;
        }
        if(AT_GetCmd(aiDSP_NdlCvFwBw) == ccBwd){
            sMsg = AT_GetName(aiDSP_NdlCvFwBw) + String(" 실린더가 BWD입니다.") ;
            bRet = false ;
        }
    }
    else if(_iMotr == miDSP_YDispr){
        if(MT_GetCmdPos(miDSP_ZDispr) > PM.GetValue(miDSP_ZDispr , pvDSP_ZDisprMove)+1.0){ //Z축 이동포지션에서 1mm 마진주고 그거보다 낮으면
            sMsg = MT_GetName(miDSP_ZDispr) + String(" 모터의 높이가 이동포지션 보다 낮습니다.") ;
            bRet = false ;
        }
        else if(MT_GetTrgPos(miPCK_YPickr) + GetMotrPos(_iMotr,_iPstnId) > OM.MstOptn.dPckDspStroke ||
                MT_GetCmdPos(miPCK_YPickr) + GetMotrPos(_iMotr,_iPstnId) > OM.MstOptn.dPckDspStroke ){
            sMsg = MT_GetName(miPCK_YPickr) + String(" 모터가 충돌 위치에 있습니다.") ;
            bRet = false ;
        }
    }
    else if(_iMotr == miDSP_XDispr){
        if(MT_GetCmdPos(miDSP_ZDispr) > PM.GetValue(miDSP_ZDispr , pvDSP_ZDisprMove)+1.0){ //Z축 이동포지션에서 1mm 마진주고 그거보다 낮으면
            sMsg = MT_GetName(miDSP_ZDispr) + String(" 모터의 높이가 이동포지션 보다 낮습니다.") ;
            bRet = false ;
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
bool CDispensor::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiDSP_NdlCrFwBw){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    if(_iActr == aiDSP_NdlCvFwBw){
        if(_bFwd == ccFwd) {
            if(MT_GetCmdPos(miDSP_ZDispr) > PM.GetValue(miDSP_ZDispr , pvDSP_ZDisprWait)){
               sMsg = MT_GetName(miDSP_ZDispr) + String(" 모터가 Wait 포지션이 아닙니다.") ;
               bRet = false ;
            }
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

bool CDispensor::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
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
//Z축 보간 추가 되면 사용 하자...
bool CDispensor::MoveMotrSync(EN_MOTR_ID _iMotr1 , EN_MOTR_ID _iMotr2 , double dMotr1Pos , double dMotr2Pos , double _dSpeed , double _dAcc , double _dDcc)
{
    double dMotr1CrntPos = MT_GetCmdPos(_iMotr1);  // 0
    double dMotr2CrntPos = MT_GetCmdPos(_iMotr2);  // 0

    double dMotr1Dist    = dMotr1Pos - dMotr1CrntPos ; // 100
    double dMotr2Dist    = dMotr2Pos - dMotr2CrntPos ; // 100
    double dMotrDist     = sqrt(dMotr1Dist * dMotr1Dist + dMotr2Dist * dMotr2Dist); //141.421
    double dTakeTimeSec  = dMotrDist / _dSpeed ;                                    //mm/ (mm/sec)   141.421 / 100 = 1.41421

    double dMotr1Speed   = dMotr1Dist / dTakeTimeSec ;                              //100 / 1.41421  70.7
    double dMotr2Speed   = dMotr2Dist / dTakeTimeSec ;                              //70.7

    double dMotr1Acc     = _dAcc * dMotr1Speed/_dSpeed  ;                           //707
    double dMotr2Acc     = _dAcc * dMotr2Speed/_dSpeed  ;                           //707

    double dMotr1Dcc     = _dDcc * dMotr1Speed/_dSpeed  ;                           //707
    double dMotr2Dcc     = _dDcc * dMotr2Speed/_dSpeed  ;                           //707


                //          100        70.7          707         707
    MT_GoAbsAcDc(_iMotr1 , dMotr1Pos , dMotr1Speed , dMotr1Acc , dMotr1Dcc);
                //          100        70.7          707         707
    MT_GoAbsAcDc(_iMotr2 , dMotr2Pos , dMotr2Speed , dMotr2Acc , dMotr2Dcc);




}

void CDispensor::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CDispensor::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CDispensor::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(miDSP_ZDispr);
    return ;

}

double CDispensor::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return dLastIdxPos ;
}

bool CDispensor::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miDSP_ZDispr)) return true  ; //이파트는 필요 없다.
    else                                            return false ;

}

bool CDispensor::CheckStop()
{
    if(!MT_GetStop(miDSP_XDispr)) return false ;
    if(!MT_GetStop(miDSP_YDispr)) return false ;
    if(!MT_GetStop(miDSP_ZDispr)) return false ;

    if(!AT_Done(aiDSP_NdlCrFwBw)) return false ;

    return true ;
}

void CDispensor::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos    " , m_dLastIdxPos   );
        UserINI.Load(sPath, "Member" , "m_dPkgCntX       " , m_dPkgCntX      );
        UserINI.Load(sPath, "Member" , "m_dPkgCntY       " , m_dPkgCntY      );
        UserINI.Load(sPath, "Member" , "m_dPkgCntT       " , m_dPkgCntT      );
        UserINI.Load(sPath, "Member" , "m_bBfVisn        " , m_bBfVisn       );
        UserINI.Load(sPath, "Member" , "m_dBfVisnX       " , m_dBfVisnX      );
        UserINI.Load(sPath, "Member" , "m_dBfVisnY       " , m_dBfVisnY      );
        UserINI.Load(sPath, "Member" , "m_dBfVisnT       " , m_dBfVisnT      );
        UserINI.Load(sPath, "Member" , "m_bAtVisn        " , m_bAtVisn       );
        UserINI.Load(sPath, "Member" , "m_dAtVisnX       " , m_dAtVisnX      );
        UserINI.Load(sPath, "Member" , "m_dAtVisnY       " , m_dAtVisnY      );
        UserINI.Load(sPath, "Member" , "m_dAtVisnT       " , m_dAtVisnT      );
        UserINI.Load(sPath, "Member" , "m_bNiddleChecked " , m_bNiddleChecked);
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos    " , m_dLastIdxPos   );
        UserINI.Save(sPath, "Member" , "m_dPkgCntX       " , m_dPkgCntX      );
        UserINI.Save(sPath, "Member" , "m_dPkgCntY       " , m_dPkgCntY      );
        UserINI.Save(sPath, "Member" , "m_dPkgCntT       " , m_dPkgCntT      );
        UserINI.Save(sPath, "Member" , "m_bBfVisn        " , m_bBfVisn       );
        UserINI.Save(sPath, "Member" , "m_dBfVisnX       " , m_dBfVisnX      );
        UserINI.Save(sPath, "Member" , "m_dBfVisnY       " , m_dBfVisnY      );
        UserINI.Save(sPath, "Member" , "m_dBfVisnT       " , m_dBfVisnT      );
        UserINI.Save(sPath, "Member" , "m_bAtVisn        " , m_bAtVisn       );
        UserINI.Save(sPath, "Member" , "m_dAtVisnX       " , m_dAtVisnX      );
        UserINI.Save(sPath, "Member" , "m_dAtVisnY       " , m_dAtVisnY      );
        UserINI.Save(sPath, "Member" , "m_dAtVisnT       " , m_dAtVisnT      );
        UserINI.Save(sPath, "Member" , "m_bNiddleChecked " , m_bNiddleChecked);
    }
}









