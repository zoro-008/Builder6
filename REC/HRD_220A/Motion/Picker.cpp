//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Picker.h"
#include "Index.h"
#include "Dispensor.h"
#include "Stage.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "SVInterface.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "UserFile.h"
#include "Rs232Keyence.h"
#include "ManualMan.h"



//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CPicker PCK;

CPicker::CPicker(void)
{

}

CPicker::~CPicker (void)
{

}

void CPicker::Init()
{


    m_sPartName = "Picker " ;

    Load(true);
    Reset();                                                                    


    InitCycleName();
    InitCycleTime();

    m_sCheckSafeMsg = "" ;

    m_bRqstPMSave = false ;
    m_bOneSkipDefect = false;


    m_bVacErrDisp = false ;
    m_bVsnErrDisp = false ;
}

void CPicker::Close()
{
    Load(false);
}

void CPicker::ResetTimer()
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
bool CPicker::FindChip(int &c , EN_ARAY_ID &id)
{
    return IDX_F.FindChip(c,id);
}

double CPicker::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dWorkStt ; 

    if(_iMotr == miPCK_YPickr){
             if(_iPstnId ==  piPCK_YPickrWait     ) dPos = PM.GetValue (_iMotr , pvPCK_YPickrWait     );
        else if(_iPstnId ==  piPCK_YPickrStageCntr) dPos = PM.GetValue (_iMotr , pvPCK_YPickrStageCntr);
        else if(_iPstnId ==  piPCK_YPickrPick     ) dPos = PM.GetValue (_iMotr , pvPCK_YPickrPick     );
        else if(_iPstnId ==  piPCK_YPickrAlign    ) dPos = PM.GetValue (_iMotr , pvPCK_YPickrAlign    );
        else if(_iPstnId ==  piPCK_YPickrHeight   ) dPos = PM.GetValue (_iMotr , pvPCK_YPickrHeight   );
        else if(_iPstnId ==  piPCK_YPickrVision   ) dPos = PM.GetValue (_iMotr , pvPCK_YPickrVision   );
        else if(_iPstnId ==  piPCK_YPickrCheck    ) dPos = PM.GetValue (_iMotr , pvPCK_YPickrCheck    );
        else if(_iPstnId ==  piPCK_YPickrPlace    ) dPos = PM.GetValue (_iMotr , pvPCK_YPickrStageCntr)+OM.DevOptn.dPlaceYOfs                            ; //최종접합포지션.
        else if(_iPstnId ==  piPCK_YPickrPlaceOfs ) dPos = PM.GetValue (_iMotr , pvPCK_YPickrStageCntr)+OM.DevOptn.dPlaceYOfs+OM.DevOptn.dFosPlaceSttYOfs; //럔tldp
        else                                        dPos = MT_GetCmdPos(_iMotr                        );
    }
    else if(_iMotr == miPCK_ZPickr){ //요놈 높이측정기는 추후에 생각 하자.
             if(_iPstnId ==  piPCK_ZPickrWait    ) dPos = PM.GetValue (_iMotr , pvPCK_ZPickrWait    );
        else if(_iPstnId ==  piPCK_ZPickrMove    ) dPos = PM.GetValue (_iMotr , pvPCK_ZPickrMove    );
        else if(_iPstnId ==  piPCK_ZPickrPick    ) dPos = PM.GetValue (_iMotr , pvPCK_ZPickrPick    );
        else if(_iPstnId ==  piPCK_ZPickrAlign   ) dPos = PM.GetValue (_iMotr , pvPCK_ZPickrAlign   );
        else if(_iPstnId ==  piPCK_ZPickrCheck   ) dPos = PM.GetValue (_iMotr , pvPCK_ZPickrCheck   );
        else if(_iPstnId ==  piPCK_ZPickrPlace   ) dPos = PM.GetValue (_iMotr , pvPCK_ZPickrCheck   ) - m_dPkPkgThickness - OM.DevOptn.dPlaceZOfs1 - OM.DevOptn.dPlaceZOfs2;
        else if(_iPstnId ==  piPCK_ZPickrVsDefect) dPos = PM.GetValue (_iMotr , pvPCK_ZPickrCheck   ) - m_dPkPkgThickness + PM.GetValue (_iMotr , poPCK_ZPickrVsDefect);
        else if(_iPstnId ==  piPCK_ZPickrVsAlign ) dPos = PM.GetValue (_iMotr , pvPCK_ZPickrCheck   ) - m_dPkPkgThickness + PM.GetValue (_iMotr , poPCK_ZPickrVsAlign );
        else                                       dPos = MT_GetCmdPos(_iMotr                       );
    }
    else if(_iMotr == miPCK_XAlign){
             if(_iPstnId ==  piPCK_XAlignWait    ) dPos = PM.GetValue (_iMotr , pvPCK_XAlignWait    );
        else if(_iPstnId ==  piPCK_XAlignWork    ) dPos = PM.GetValue (_iMotr , pvPCK_XAlignWork    );
        else                                       dPos = MT_GetCmdPos(_iMotr                       );
    }
    else if(_iMotr == miPCK_YAlign){
             if(_iPstnId ==  piPCK_YAlignWait    ) dPos = PM.GetValue (_iMotr , pvPCK_YAlignWait );
        else if(_iPstnId ==  piPCK_YAlignWork    ) dPos = PM.GetValue (_iMotr , pvPCK_YAlignWork );
        else                                       dPos = MT_GetCmdPos(_iMotr                    );
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CPicker::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPicker::Update()
{

//<Temp>
//    static CDelayTimer tmVccmOn ;
//    if(tmVccmOn.OnDelay(IO_GetY(yPCK_VccmOn),1000)){
//        IO_SetY(yPCK_VccmOn,false);
//    }
//
//    static CDelayTimer tmVccmOff ;
//    if(tmVccmOff.OnDelay(IO_GetY(yPCK_VccmOff),1000)){
//        IO_SetY(yPCK_VccmOff,false);
//    }

}

bool CPicker::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    bool bNeedContWork = (OM.DevInfo.iWorkMode == wmFos && DM.ARAY[riSTG].GetStat(0,0) == csPush) ||
                         (OM.DevInfo.iWorkMode == wmDie && DM.ARAY[riSTG].GetStat(0,0) == csPush) ;
                         //두 모드다 푸쉬는 멈추면 안되네.....

    if(bNeedContWork) return false ;




    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CPicker::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPicker::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: Rs232_LoadCl.WeightCheck(); //출발 하기전에 현재 무게 확인 및 통신 테스트..
                 m_tmDelay.Clear();
                 Step.iToStart++;
                 return false ;

        case 11: if(m_tmDelay.OnDelay(true , 2000)) {
                     EM_SetErrMsg(eiPCK_LoadCell , "로드셀 통신이 지연되었습니다.");
                     Step.iToStart=0;
                     return true ;
                 }
                 if(!Rs232_LoadCl.GetMsgEnd      ()) return false;

                 MoveMotr(miPCK_XAlign , piPCK_XAlignWait);
                 MoveMotr(miPCK_YAlign , piPCK_YAlignWait);
                 Step.iToStart++;
                 return false ;

        case 12: if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWait)) return false ;
                 if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWait)) return false ;

                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iToStart ++ ;
                 return false ;

        case 13: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 //비젼검사 끝났어도 다시 시작 할때 다시 검사 한다.
                 if(DM.ARAY[riPCK].GetStat(0,0) == csPush)DM.ARAY[riPCK].SetStat(csVisn); 
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPicker::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: //얼라인할때 Z축을 내릴때 알람뜨면 급정지 하는데.. 그래서 확인.
                 if(!MT_GetStopInpos(miPCK_ZPickr )) return false ;
                 MoveMotr(miPCK_XAlign , piPCK_XAlignWait);
                 MoveMotr(miPCK_YAlign , piPCK_YAlignWait);
                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWait)) return false ;
                 if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWait)) return false ;

                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 //픽커가 디스펜서 존에 있으면 불편할까봐.
                 if(MT_GetCmdPos(miPCK_YPickr) > PM.GetValue(miPCK_YPickr , pvPCK_YPickrVision)) MoveMotr(miPCK_YPickr , piPCK_YPickrAlign);
                 Step.iToStop ++ ;
                 return false ;

        case 13: if(!MT_GetStop(miPCK_YPickr)) return false ;

                 //어테치 하다가 스탑 된경우.
                 if(DM.ARAY[riPCK].GetStat(0,0)==csWait)DM.ARAY[riPCK].SetStat(csAttach);
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CPicker::Autorun(void) //오토런닝시에 계속 타는 함수.
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
        bool bNeedContWork = (OM.DevInfo.iWorkMode == wmFos && DM.ARAY[riSTG].GetStat(0,0) == csPush) ||
                             (OM.DevInfo.iWorkMode == wmDie && DM.ARAY[riSTG].GetStat(0,0) == csPush) ;
                             //두 모드다 푸쉬는 멈추면 안되네.....

        if (Stat.bReqStop && !bNeedContWork)return false ;


        //LotEnd가 되지 않는다.....
        //강제 랏엔드 시킨다.
        if(IDX_R.GetWorkEnd()&&DM.ARAY[riPCK].GetStat(0,0)!=csNone){
            DM.ARAY[riPCK].SetStat(csSkip);
            m_bNeedLotEnd = true ;
        }



        //FOS 순서                                    //DIE순서
        //csVisn    얼라인검사                        //csVisn    얼라인 검사 해본다.

        //차이점.
        //csAttach  픽커 칩 붙인다.                   //csDisp    파란판에 패턴 디스펜싱 한다.
        //csDisp    디스펜싱 하고.                    //csAttach  칩을 붙인다.

        //csPush    일정 시간 꾹 누르고 픽커 빠진다.  //csPush
        //csEndVisn 작업 완료 비젼 확인.              //csEndVisn 작업 완료 비젼 확인.
        //csWorkEnd 작업 종료 하고 스테이지 내린다.   //csWorkEnd 작업 종료 하고 스테이지 내린다.

        bool isStgNeedAttach   =  DM.ARAY[riSTG].GetStat(0,0) == csAttach ;
        bool isDispPosStgUnder =  MT_GetCmdPos(miDSP_YDispr) <  PM.GetValue(miDSP_YDispr , pvDSP_YDisprVsStageCntr) ;
        bool isDispPosWait     =  MT_GetCmdPos(miDSP_YDispr) <= PM.GetValue(miDSP_YDispr , pvDSP_YDisprWait) ;



        bool isCyclePick     =  DM.ARAY[riPCK].CheckAllStat(csNone ) &&
                                DM.ARAY[riALN].CheckAllStat(csNone ) &&
                                DM.ARAY[riIDX_F].GetCntStat(csUnkwn) &&
                                //DM.ARAY[riIDX_R].GetCntStat(csUnkwn) && 
                                IDX_F.IsWorkPos()                    &&
                                IDX_F.GetSeqStep() == CIndex::scIdle  ; //MT_GetCmdPos(miSTG_ZStagR) == PM.GetValue(miSTG_ZStagR , pvSTG_ZStagWait);.
        bool isCycleAlign    =  DM.ARAY[riPCK].GetStat(0,0) == csUnkwn  &&
                                DM.ARAY[riALN].GetStat(0,0) == csNone   &&
                               (DM.ARAY[riSTG].GetStat(0,0) == csAttach || DM.ARAY[riSTG].GetStat(0,0) == csDisp) ; // && DM.ARAY[riSTG].GetStat(0,0) != csNone ; //스테이지 픽하기 전에 작업 하지 않고 대기.
                                //비전 검사 끝날때까지 진행 하면 안된다. 한번 얼라인 치면 에러시에 다시 보트에 넣어야 하는데 안착 불량 나서 최대한 지연 시켜서 얼라인 침.
        bool isCycleAlignPick=  DM.ARAY[riPCK].GetStat(0,0) == csNone   && //얼라인 하다가 놓친경우일때.
                                DM.ARAY[riALN].GetStat(0,0) != csNone  ;
        bool isCycleVisn     =  DM.ARAY[riPCK].GetStat(0,0) == csVisn   && isStgNeedAttach && isDispPosStgUnder; //
        bool isCycleAttach   =  DM.ARAY[riPCK].GetStat(0,0) == csAttach &&
                                DM.ARAY[riSTG].GetStat(0,0) == csAttach &&
                                /*isFinishStgVisn && */isDispPosWait    ;
        /*  잠시 주석..
        bool isCycleAttach   =  DM.ARAY[riPCK].GetStat(0,0) == csAttach &&
                                DM.ARAY[riSTG].GetStat(0,0) == csAttach &&
                                isFinishStgVisn && isDispPosWait    ;
        */

        bool isCyclePush     =  DM.ARAY[riSTG].GetStat(0,0) == csPush ;
        bool isCycleBoatPlace=  DM.ARAY[riPCK].GetStat(0,0) == csSkip ;
        bool isCycleVoid     = (DM.ARAY[riSTG].GetStat(0,0) == csVisn   || DM.ARAY[riSTG].GetStat(0,0) == csVisnAt ) &&
                                MT_GetTrgPos(miPCK_YPickr) > GetMotrPos(miPCK_YPickr , piPCK_YPickrAlign) ;


        bool isCycleEnd      =  IDX_F.GetStat().bWorkEnd && DM.ARAY[riPCK].CheckAllStat(csNone) && DM.ARAY[riALN].CheckAllStat(csNone) ;

        if(EM_IsErr()&& !bNeedContWork) return false ;
         //Normal Decide Step.
             if (isCyclePick      ) {Trace(m_sPartName.c_str(),"CyclePick      Stt"); Step.iSeq = scPick      ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleAlign     ) {Trace(m_sPartName.c_str(),"CycleAlign     Stt"); Step.iSeq = scAlign     ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleAlignPick ) {Trace(m_sPartName.c_str(),"CycleAlignPick Stt"); Step.iSeq = scAlignPick ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleVisn      ) {Trace(m_sPartName.c_str(),"CycleVisn      Stt"); Step.iSeq = scVisn      ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleAttach    ) {Trace(m_sPartName.c_str(),"CycleAttach    Stt"); Step.iSeq = scAttach    ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCyclePush      ) {Trace(m_sPartName.c_str(),"CyclePush      Stt"); Step.iSeq = scPush      ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleBoatPlace ) {Trace(m_sPartName.c_str(),"CycleBoatPlace Stt"); Step.iSeq = scBoatPlace ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleVoid      ) {Trace(m_sPartName.c_str(),"CycleVoid      Stt"); Step.iSeq = scVoid      ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd       ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    Step.iLastSeq = Step.iSeq ;
    switch (Step.iSeq) {
        default          :                        Trace(m_sPartName.c_str(),"default        End");                                                   Step.iSeq = scIdle ;  return false ;
        case scIdle      :                                                                                                                                                 return false ;
        case scPick      : if(CyclePick      ()){ Trace(m_sPartName.c_str(),"CyclePick      End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scAlign     : if(CycleAlign     ()){ Trace(m_sPartName.c_str(),"CycleAlign     End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scAlignPick : if(CycleAlignPick ()){ Trace(m_sPartName.c_str(),"CycleAlignPick End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scVisn      : if(CycleVisn      ()){ Trace(m_sPartName.c_str(),"CycleVisn      End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scAttach    : if(CycleAttach    ()){ Trace(m_sPartName.c_str(),"CycleAttach    End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scPush      : if(CyclePush      ()){ Trace(m_sPartName.c_str(),"CyclePush      End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scBoatPlace : if(CycleBoatPlace ()){ Trace(m_sPartName.c_str(),"CycleBoatPlace End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scVoid      : if(CycleVoid      ()){ Trace(m_sPartName.c_str(),"CycleVoid      End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

double CPicker::GetPkgThickness()
{
    return m_dPkPkgThickness;

}
double CPicker::GetEmptyToolHeight()
{
    return m_dEmptyToolHeight;
}
double CPicker::GetVisnDataX()
{
    return m_dVisnX;
}
double CPicker::GetVisnDataY()
{
    return m_dVisnY;
}
int CPicker::GetVisnDefectCnt()
{
    return m_iVisnDefCnt;
}



bool CPicker::CycleHome()     //sun DLL direct access.
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
                 if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: IO_SetY(yPCK_WeightOff , false); 
                 MT_DoHome(miPCK_ZPickr);
                 Step.iHome++;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miPCK_ZPickr))return false ;
                 MT_DoHome(miPCK_YPickr);
                 MT_DoHome(miPCK_XAlign);
                 MT_DoHome(miPCK_YAlign);
                 Step.iHome++;

                 return false ;

        case 12: if(!MT_GetHomeEnd(miPCK_YPickr)) return false ;
                 if(!MT_GetHomeEnd(miPCK_XAlign)) return false ;
                 if(!MT_GetHomeEnd(miPCK_YAlign)) return false ;

                 MT_GoAbsMan(miPCK_YPickr , PM.GetValue(miPCK_YPickr,pvPCK_YPickrWait));
                 MT_GoAbsMan(miPCK_ZPickr , PM.GetValue(miPCK_ZPickr,pvPCK_ZPickrMove));
                 MT_GoAbsMan(miPCK_XAlign , PM.GetValue(miPCK_XAlign,pvPCK_XAlignWait));
                 MT_GoAbsMan(miPCK_YAlign , PM.GetValue(miPCK_YAlign,pvPCK_YAlignWait));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miPCK_YPickr , PM.GetValue(miPCK_YPickr,pvPCK_YPickrWait))) return false ;
                 if(!MT_GoAbsMan(miPCK_ZPickr , PM.GetValue(miPCK_ZPickr,pvPCK_ZPickrMove))) return false ;
                 if(!MT_GoAbsMan(miPCK_XAlign , PM.GetValue(miPCK_XAlign,pvPCK_XAlignWait))) return false ;
                 if(!MT_GoAbsMan(miPCK_YAlign , PM.GetValue(miPCK_YAlign,pvPCK_YAlignWait))) return false ;


//여기 부터 rs232들 예외 처리.===========================================
                 MT_GoAbsMan(miPCK_YPickr , PM.GetValue(miPCK_YPickr,pvPCK_YPickrHeight));
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miPCK_YPickr , PM.GetValue(miPCK_YPickr,pvPCK_YPickrHeight))) return false ;
                 Rs232Keyence.CheckHeight(kcChA); //이상하게 첨 켜고 측정 하면 타임아웃이 한번 뜨는데 일단 한번 뜨고는 정상 작동 한다.
                 //땜빵으로 하고 나중에 원인 분석하자.
                 Step.iHome++;
                 return false;

        case 15: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChA)) {
                     Trace("PickerHome" , "키엔스 에러.");
                     FM_MsgOk("한라 개발자 확인용","픽커툴 높이측정기 리딩지연.");
                 }
                 Rs232_LoadCl.WeightCheck(); //출발 하기전에 현재 무게 확인 및 통신 테스트..
                 m_tmDelay.Clear();
                 Step.iHome++ ;
                 return false ;


        case 16: if(m_tmDelay.OnDelay(true , 2000)) {
                     Trace("PickerHome" , "무게센서 에러.");
                     FM_MsgOk("한라 개발자 확인용","픽커툴 무게센서  리딩지연.");
                     IO_SetY(yPCK_WeightOff , true); //가끔 무게 쎈서 통신 장애 생김.. 확인 하고
                     m_tmDelay.Clear();
                     Step.iHome++;
                     return false ;
                 }
                 if(!Rs232_LoadCl.GetMsgEnd ()) return false;
                 Step.iHome++;
                 return false ;

        case 17: if(!m_tmDelay.OnDelay(true , 1000)) return false ;
                 IO_SetY(yPCK_WeightOff , false); //가끔 무게 쎈서 통신 장애 생김.. 확인 하고

                 MT_GoAbsMan(miPCK_YPickr , PM.GetValue(miPCK_YPickr,pvPCK_YPickrWait));
                 Step.iHome++;
                 return false ;

        case 18: if(!MT_GoAbsMan(miPCK_YPickr , PM.GetValue(miPCK_YPickr,pvPCK_YPickrWait))) return false ;

                 Step.iHome = 0;
                 return true ;

    }
}

//One Cycle.
bool CPicker::CyclePick()
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

    static int iPckDnCnt = 0;
    const  int iPckDnMaxCnt = 2;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: if(OM.MstOptn.bSkipPickrDispr) {
                     m_tmDelay.Clear();
                     Step.iCycle ++;
                     return false;
                 }
                 Step.iCycle = 12;
                 return false;

        case 11: if(!m_tmDelay.OnDelay(true , 1000)) return false;
                 int  iWorkingCol ;
                 EN_ARAY_ID riWork ;
                 FindChip(iWorkingCol ,riWork);
                 DM.ARAY[riIDX_F].SetStat(0,iWorkingCol,csEmpty);

                 Step.iCycle = 0;
                 return true;

        //위에서 씀.
        case 12: if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push
                 //메뉴얼 조작 잘 못 했을 경우를 위해서....
                 if(IO_GetX(xPCK_Vccm)){
                     EM_SetErrMsg(eiPCK_VaccSensor , "픽커 배큠센서가 On되어 있습니다.");
                     Step.iCycle = 0 ;
                     return true;
                 }
                 if(!IDX_F.IsWorkPos()){ //메뉴얼때문에...
                     EM_SetErrMsg(eiPRT_Crash , "프론트 인덱스가 작업 포지션이 아닙니다.");
                     Step.iCycle = 0 ;
                     return true;
                 }

                 //20160511 박창후 과장 요청.혹시 딸려서 픽커에 붙어 있을 경우를 위해 배큠을 켜서 확인도 해본다.
                 IO_SetY(yPCK_Ejct   , false);
                 IO_SetY(yPCK_VccmOn , true );

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 13: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 if(IO_GetX(xPCK_Vccm)){ //혹시 딸려서 픽커에 붙어 있을 경우를 위해 배큠을 켜서 확인도 해본다.
                     EM_SetErrMsg(eiPCK_VaccSensor , "픽커에 PKG가 붙어 있습니다. 픽커에 접착력이 생겼는지 확인하세요.");
                     Step.iCycle = 0 ;
                     return true;
                 }

                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);

                 IO_SetY(yPCK_Ejct    , false);
                 //20160511 히스토리 2점식 배큠이젝솔 -> 3점식 -> 2점식 적용하여 사용중.
                 //현재 2점식.
                 //<Temp> IO_SetY(yPCK_VccmOff , true );
                 IO_SetY(yPCK_VccmOn , false );

                 iPckDnCnt = 0;
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 IO_SetY(yPCK_VccmOff , false );

                 Step.iCycle++;
                 return false;

        case 15:
                 Step.iCycle++;
                 return false ;

        case 16: //if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 //Rs232Keyence.CheckHeight(kcChA);
                 Step.iCycle++;
                 return false ;

        case 17: //if(!Rs232Keyence.GetMsgEnd()) return false ;
                 //if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChA)) {
                 //    EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 측정포지션 및 배큠센서를 확인하세요.");
                 //    Step.iCycle = 0 ;
                 //    return true ;
                 //}
                 //m_dEmptyToolHeight = Rs232Keyence.GetHeightData(kcChA);
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false;

        case 18: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false;
                 MoveMotr(miPCK_YPickr , piPCK_YPickrPick);
                 Step.iCycle++;
                 return false ;

        case 19: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrPick)) return false ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrPick);
                 IO_SetY(yPCK_VccmOn , true);
                 Step.iCycle++;
                 return false ;

        case 20: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrPick)) return false ;
                 //<Temp>IO_SetY(yPCK_VccmOn , false);
                 Step.iCycle++;
                 return false;

        case 21: if(!MT_GetStop(miPCK_ZPickr)) return false;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 22: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iPkPickDelay))return false;
                 if(!IO_GetX(xPCK_Vccm)){
                     if(iPckDnCnt < iPckDnMaxCnt){
                         MT_GoInc(miPCK_ZPickr , 0.1 , 5.0); //천천히 내려 본다.
                         iPckDnCnt++;
                         Step.iCycle = 21;
                         return false ;
                     }
                     else{
                         MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                         Step.iCycle++;
                         return false ;
                     }
                  }
                  MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                  Step.iCycle++;
                  return false ;


        case 23: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 24: if(!m_tmDelay.OnDelay(true , 1000)) return false ;
                 if(!IO_GetX(xPCK_Vccm)){
                     //if(OM.CmnOptn.bVisnNgStop) {
                         EM_SetErrMsg(eiPCK_PickMiss , "Picker가 설정시간 동안 자제를 집지 못했습니다.");
                         if(MM.GetManNo() == mcNone)  m_bVacErrDisp = true;
                         Step.iCycle=0;
                         return true ;
                     //}
                     //else {
                     //    int  iLastEmptyCol ;
                     //    EN_ARAY_ID riWork ;
                     //    FindChip(iLastEmptyCol ,riWork);
                     //    DM.ARAY[riIDX_F].SetStat(0,iLastEmptyCol-1,csSkip);
                     //}

                 }
                 else {
                     int  iWorkingCol ;
                     EN_ARAY_ID riWork ;
                     FindChip(iWorkingCol ,riWork);
                     DM.ARAY[riIDX_F].SetStat(0,iWorkingCol,csWait);

                     DM.ARAY[riPCK].SetStat(csUnkwn);

                     //ID
                     String sChipID = DM.ARAY[riIDX_F].CHPS[0][iWorkingCol].GetID() ;
                     DM.ARAY[riPCK].CHPS[0][0].SetID(sChipID);
                 }
                 //MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);

                 Step.iCycle = 0;
                 return true ;
    }
}

bool CPicker::CycleAlign(void)
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

    static double dStartWeightGram = 0.0;
    static double dCrntWeightGram = 0.0;
    static CDelayTimer WeightTimer ;
    static int    iWeightStep =  0 ;
    static int    iPreWeightStep = 0 ;
    iPreWeightStep = iWeightStep ;

    if(Step.iCycle == 10){
        iWeightStep    = 10 ;
        iPreWeightStep = 0  ;
    }



    switch (iWeightStep){
        default : sTemp = sTemp.sprintf("Weight Cycle Default Clear %s iWeightStep=%02d" , __FUNC__ , iWeightStep );
                  if(iWeightStep != iPreWeightStep)Trace(m_sPartName.c_str(), sTemp.c_str());
                  iWeightStep = 0 ;
                  return true ;


        case 10: Rs232_LoadCl.WeightCheck(); //Y축 출발 하기전에 현재 무게 확인 및 통신 테스트..
                 WeightTimer.Clear();
                 iWeightStep++;
                 break ;

        case 11: if(WeightTimer.OnDelay(true , 2000)) {
                     EM_SetErrMsg(eiPCK_LoadCell , "로드셀 통신이 지연되었습니다.");

                     Step.iCycle=0;
                     return true ;
                 }
                 if(!Rs232_LoadCl.GetMsgEnd())break;

                 dCrntWeightGram = Rs232_LoadCl.GetLoadCellData()* 1000;
                 Trace("Align LoadCell" , String(dCrntWeightGram).c_str());
                 iWeightStep = 10 ;
                 break ;
    }










    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: if(OM.DevOptn.bPkIgnrAlign) {
                     DM.ARAY[riPCK].SetStat(csVisn);
                     Step.iCycle=0;
                     return true ;
                 }

                 //if(IO_GetX(xPCK_Vccm)){
                 //    EM_SetErrMsg(eiPCK_VaccSensor , "픽커 배큠센서가 On되어 있습니다.");
                 //    Step.iCycle = 0 ;
                 //    return true;
                 //}

                 if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 MoveMotr(miPCK_YPickr , piPCK_YPickrAlign);
                 MoveMotr(miPCK_XAlign , piPCK_XAlignWait );
                 MoveMotr(miPCK_YAlign , piPCK_YAlignWait );
                 MoveActr(aiPCK_XAlgnFwBw , ccBwd);
                 MoveActr(aiPCK_YAlgnFwBw , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrAlign)) return false ;
                 if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWait )) return false ;
                 if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWait )) return false ;
                 if(!MoveActr(aiPCK_XAlgnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiPCK_YAlgnFwBw , ccBwd)) return false ;


                 //20170227 얼라이너 충돌 감지용 로드셀 프로그램 오성철과장요청


                 Step.iCycle++;
                 return false ;

        case 13: dStartWeightGram = dCrntWeightGram ;


                 MT_GoAbsRun(miPCK_ZPickr , PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrAlign)-0.5 );



                 Step.iCycle++;
                 return false ;

        case 14: if( dCrntWeightGram - dStartWeightGram > 40 ){ //40그람 이상 걸리면 처박은느낌.
                     EM_SetErrMsg(eiPCK_LoadCell , "얼라이너 상부 픽커와 로드셀 40그람 충돌 감지 ");
                     MT_EmgStop(miPCK_ZPickr);
                     Step.iCycle=0;
                     return true;
                 }

                 if(!MT_GetStopInpos(miPCK_ZPickr)) return false ;
                 //<Temp>                      IO_SetY(yPCK_VccmOff , true);
                 IO_SetY(yPCK_VccmOn , false );
                 IO_SetY(yPCK_Ejct , true );
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 15: if(!m_tmDelay.OnDelay(true , 100))return false ;
                 IO_SetY(yPCK_VccmOff , false);
                 if(DM.ARAY[riPCK].GetStat(0,0)!=csNone) {
                     DM.ARAY[riALN].CHPS[0][0].SetID(DM.ARAY[riPCK].CHPS[0][0].GetID());
                     DM.ARAY[riALN].SetStat(csUnkwn);
                 }
                 DM.ARAY[riPCK].SetStat(csNone );

                 //MT_GoIncRun(miPCK_ZPickr , -1.0);
                 MoveMotr(miPCK_XAlign , piPCK_XAlignWork );
                 MoveMotr(miPCK_YAlign , piPCK_YAlignWork );

                 Step.iCycle++;
                 return false ;

        case 16: //if(!MT_GetStopInpos(miPCK_ZPickr )) return false ;
                 if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWork )) return false ;
                 if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWork )) return false ;
                 MoveActr(aiPCK_XAlgnFwBw , ccFwd);
                 MoveActr(aiPCK_YAlgnFwBw , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 17: if(!MoveActr(aiPCK_XAlgnFwBw , ccFwd))return false ;
                 if(!MoveActr(aiPCK_YAlgnFwBw , ccFwd))return false ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrAlign);
                 Step.iCycle++;
                 return false;

        case 18: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrAlign)) return false ;
                 IO_SetY(yPCK_VccmOn , true );
                 IO_SetY(yPCK_Ejct , false);
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 19: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iPkPickDelay))return false;
                 MoveActr(aiPCK_XAlgnFwBw , ccBwd);
                 MoveActr(aiPCK_YAlgnFwBw , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 20: if(!MoveActr(aiPCK_XAlgnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiPCK_YAlgnFwBw , ccBwd)) return false ;
                 //<Temp>IO_SetY(yPCK_VccmOn , false );
                 MoveMotr(miPCK_XAlign , piPCK_XAlignWait );
                 MoveMotr(miPCK_YAlign , piPCK_YAlignWait );
                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWait )) return false ;
                 if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWait )) return false ;

                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 22: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 23: if(!m_tmDelay.OnDelay(true , 1000)) return false ;
                 if(!IO_GetX(xPCK_Vccm)){
                     EM_SetErrMsg(eiPCK_PickMiss , "Picker가 설정시간 동안 자제를 집지 못했습니다.");
                     Step.iCycle=0;
                     return true ;
                 }
                 if(DM.ARAY[riALN].GetStat(0,0)!=csNone) {
                     DM.ARAY[riPCK].CHPS[0][0].SetID(DM.ARAY[riALN].CHPS[0][0].GetID());
                     DM.ARAY[riPCK].SetStat(csVisn);
                     DM.ARAY[riALN].SetStat(csNone);
                 }

                 Step.iCycle = 0;
                 return true ;
        }
}

bool CPicker::CycleAlignPick(void)
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
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push
                 if(IO_GetX(xPCK_Vccm)){
                     EM_SetErrMsg(eiPCK_VaccSensor , "픽커 배큠센서가 On되어 있습니다.");
                     Step.iCycle = 0 ;
                     return true;
                 }

                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove );
                 MoveMotr(miPCK_XAlign , piPCK_XAlignWait );
                 MoveMotr(miPCK_YAlign , piPCK_YAlignWait );
                 MoveActr(aiPCK_XAlgnFwBw , ccBwd);
                 MoveActr(aiPCK_YAlgnFwBw , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove )) return false ;
                 if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWait )) return false ;
                 if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWait )) return false ;
                 if(!MoveActr(aiPCK_XAlgnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiPCK_YAlgnFwBw , ccBwd)) return false ;
                 IO_SetY(yPCK_Ejct , false);
                 //MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);
                 MoveMotr(miPCK_XAlign , piPCK_XAlignWork );
                 MoveMotr(miPCK_YAlign , piPCK_YAlignWork );

                 Step.iCycle++;
                 return false;

        case 12: if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWork )) return false ;
                 if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWork )) return false ;
                 //MoveActr(aiPCK_XAlgnFwBw , ccFwd);
                 //MoveActr(aiPCK_YAlgnFwBw , ccFwd);
                 //MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 13: //if(!MoveMotr(miPCK_YPickr , piPCK_YPickrHeight)) return false ;
                 //if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 //Rs232Keyence.CheckHeight(kcChA);
                 //if(!MoveActr(aiPCK_XAlgnFwBw , ccFwd)) return false ;
                 //if(!MoveActr(aiPCK_YAlgnFwBw , ccFwd)) return false ;
                 Step.iCycle++;
                 return false ;

        case 14: //if(!Rs232Keyence.GetMsgEnd()) return false; //가끔 껏따키면 지연 시간 5초 거림 확인 해야함.
                 //if(0 == Rs232Keyence.GetHeightData(kcChA)) {
                 //    EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 데이터가 이상합니다. 다시 수행해 주세요.");
                 //    Step.iCycle = 0 ;
                 //    return true ;
                 //}

                 //if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChA)) {
                 //    EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 측정포지션을 확인하세요.");
                 //    Step.iCycle = 0 ;
                 //    return true ;
                 //}
                 //m_dEmptyToolHeight = Rs232Keyence.GetHeightData(kcChA);
                 MoveMotr(miPCK_YPickr , piPCK_YPickrAlign);

                 Step.iCycle++;
                 return false ;

        case 15: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrAlign)) return false ;
                 MoveActr(aiPCK_XAlgnFwBw , ccFwd);
                 MoveActr(aiPCK_YAlgnFwBw , ccFwd);
                 Step.iCycle++;
                 return false;

        case 16: if(!MoveActr(aiPCK_XAlgnFwBw , ccFwd)) return false;
                 if(!MoveActr(aiPCK_YAlgnFwBw , ccFwd)) return false;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrAlign);
                 Step.iCycle++;
                 return false ;


        case 17: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrAlign)) return false ;
                 IO_SetY(yPCK_VccmOn , true );
                 IO_SetY(yPCK_Ejct , false);
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 18: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iPkPickDelay)) return false ;
                 MoveActr(aiPCK_XAlgnFwBw , ccBwd);
                 MoveActr(aiPCK_YAlgnFwBw , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 19: if(!MoveActr(aiPCK_XAlgnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiPCK_YAlgnFwBw , ccBwd)) return false ;
                 //<Temp>IO_SetY(yPCK_VccmOn , false );
                 MoveMotr(miPCK_XAlign , piPCK_XAlignWait );
                 MoveMotr(miPCK_YAlign , piPCK_YAlignWait );
                 Step.iCycle++;
                 return false;

        case 20: if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWait )) return false ;
                 if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWait )) return false ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false;

        case 21: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 22: if(!m_tmDelay.OnDelay(true , 1000)) return false ;
                 if(!IO_GetX(xPCK_Vccm)){
                     EM_SetErrMsg(eiPCK_PickMiss , "Picker가 설정시간 동안 자제를 집지 못했습니다.");
                     Step.iCycle=0;
                     return true ;
                 }

                 if(DM.ARAY[riALN].GetStat(0,0)!=csNone){
                     DM.ARAY[riPCK].SetStat(csVisn);
                     String sChipID = DM.ARAY[riALN].CHPS[0][0].GetID() ;
                     DM.ARAY[riPCK].CHPS[0][0].SetID(sChipID);
                 }

                 DM.ARAY[riALN].SetStat(csNone);

                 if(Step.iSeq == scNone){ //메뉴얼일때는 여기서 찝어서 비젼 세팅 하는 경우가 많아서  높이 측정을 한다.
                     Step.iCycle = 30;
                     return false ;
                 }

                 Step.iCycle = 0;
                 return true ;

        //흠...중복 동작시 삭제요청.
        //높이 측정.
        case 30: MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);
                 Step.iCycle++;
                 return false ;

        case 31: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrHeight)) return false ;
                 //MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck);
                 Step.iCycle++;
                 return false;

        case 32: //if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck)) return false;
                 //여기서 높이 측정 하고 높이 보정 한다.
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 33: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 Rs232Keyence.CheckHeight(kcChA);
                 Step.iCycle++;
                 return false ;

        case 34: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChA)) {
                     EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 측정포지션을 확인하세요.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 m_dPkPkgThickness = Rs232Keyence.GetHeightData(kcChA) - m_dEmptyToolHeight ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false;

        case 35: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false;
                 Step.iCycle = 0;
                 return true ;
    }

}

bool CPicker::CycleVisn(void)
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
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: m_dVisnX = 0.0 ;
                 m_dVisnY = 0.0 ;
                 m_dVisnT = 0.0 ;
                 m_iVisnDefCnt = 0.0 ;
                 m_bVisn  = true ;

                 if(OM.DevOptn.bPkIgnrVisnAlign && (OM.DevOptn.bPkIgnrVisnDefect || m_bOneSkipDefect)){
                     if(OM.DevInfo.iWorkMode == wmFos)DM.ARAY[riPCK].SetStat(csAttach);
                     else                             DM.ARAY[riPCK].SetStat(csDisp  );
                     Step.iCycle=0;
                     return true ;
                 }
                 else if(OM.DevOptn.bPkIgnrVisnAlign && !m_bOneSkipDefect){
                     //JS
                     if(!SKIP_VISN)SV_Ready(viPickerDefect); //미리 카메라 세팅 바꿔 놓는다.
                 }
                 else { //모두 검사.
                     //JS
                     if(!SKIP_VISN)SV_Ready(viPickerAlign ); //미리 카메라 세팅 바꿔 놓는다.
                 }

                 //메뉴얼시에 충돌 위치에 있으면 치운다.
                 if(MM.GetManNo() != scNone && !DSP.ToolVoid()) return false ;
                 //if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push

                 //시간이 오래 걸림.
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);

                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrHeight)) return false ;
                 //MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck);
                 Step.iCycle++;
                 return false;

        case 13: //if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck)) return false;
                 //여기서 높이 측정 하고 높이 보정 한다.
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 14: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 Rs232Keyence.CheckHeight(kcChA);
                 Step.iCycle++;
                 return false ;

        case 15: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChA)) {
                     EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 측정포지션을 확인하세요.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 m_dPkPkgThickness = Rs232Keyence.GetHeightData(kcChA) - m_dEmptyToolHeight ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false;

        case 16: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false;
                 MoveMotr(miPCK_YPickr , piPCK_YPickrVision);
                 Step.iCycle++;
                 return false ;

        case 17: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrVision)) return false ;

                 if(OM.DevOptn.bPkIgnrVisnAlign && !m_bOneSkipDefect){
                     Step.iCycle=30;
                     return false ;
                 }

                 Step.iCycle=20;
                 return false ;

        //Vision 검사 준비완료....


        //Align Vision===========================================================
        case 20: MoveMotr(miPCK_ZPickr , piPCK_ZPickrVsAlign);
                 Step.iCycle++;
                 return false ;

        case 21: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrVsAlign)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 22: if(!m_tmDelay.OnDelay(true , 100)) return false ;
                 //JS
                 if(!SKIP_VISN){

                     SV_SetPkgID(viPickerAlign , DM.ARAY[riPCK].CHPS[0][0].GetID().c_str());
                     SV_GrabInsp(viPickerAlign);
                 }
                 Step.iCycle++;
                 return false ;

        case 23: //JS
                 //if(!SKIP_VISN && !SV_GetInspEnd(viPickerAlign)) return false ;
                 if(!SKIP_VISN &&!SV_GetGrabEnd(viPickerAlign)) return false ;
                 if(!SKIP_VISN &&!SV_GetInspEnd(viPickerAlign)) return false ;
                 //JS
                 if(!SKIP_VISN)SV_GetRslt(viPickerAlign , VisnRslt);
                 sVisnRslt = VisnRslt ;

                 if(sVisnRslt.Pos("OK")){                                 //"OK-0.23145,0.12344,0.32123
                     sVisnRslt.Delete(1,3);                               //0.23145,0.12344,0.32123
                     //X
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.23145
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));              //0.12344,0.32123
                     sItem.Delete(1,2);
                     m_dVisnX = StrToFloatDef(sItem , 0.0);

                     //Y
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.12344
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));
                     sItem.Delete(1,2);
                     m_dVisnY = StrToFloatDef(sItem , 0.0);

                     //T
                     sItem = sVisnRslt ;
                     sItem.Delete(1,2);
                     m_dVisnT = StrToFloatDef(sItem , 0.0);               //0.32123

                     m_bVisn = true ;
                 }
                 else {//"NG"
                     m_dVisnX = 0.0 ;
                     m_dVisnY = 0.0 ;
                     m_dVisnT = 0.0 ;

                     m_bVisn = false ;

                     if(OM.CmnOptn.bVisnNgStop) {
                         EM_SetErrMsg(eiVSN_InspNG , "Picker Align Vision Inspection NG");
                         m_bVsnErrDisp = true ;
                         MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                         Step.iCycle=50;
                         return false ;
                     }
                     else {
                         MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                         if(DM.ARAY[riPCK].GetStat(0,0)!=csNone) DM.ARAY[riPCK].SetStat(csSkip);
                         //if(DM.ARAY[riSTG].GetStat(0,0)!=csNone)DM.ARAY[riSTG].SetStat(csSkip);
                         Step.iCycle=50;
                         return false  ;
                     }
                 }

                 if(OM.CmnOptn.bVisnNgStop) {
                     if(m_dVisnX >  OM.DevOptn.dPickerTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 X보정값이 설정범위를 넘었습니다."); m_bVsnErrDisp = true ; Step.iCycle = 0 ; return true ;}
                     if(m_dVisnY >  OM.DevOptn.dPickerTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 Y보정값이 설정범위를 넘었습니다."); m_bVsnErrDisp = true ; Step.iCycle = 0 ; return true ;}
                     if(m_dVisnT >  OM.DevOptn.dPickerTolAng ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 T보정값이 설정범위를 넘었습니다."); m_bVsnErrDisp = true ; Step.iCycle = 0 ; return true ;}

                     if(m_dVisnX < -OM.DevOptn.dPickerTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 X보정값이 설정범위를 넘었습니다."); m_bVsnErrDisp = true ; Step.iCycle = 0 ; return true ;}
                     if(m_dVisnY < -OM.DevOptn.dPickerTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 Y보정값이 설정범위를 넘었습니다."); m_bVsnErrDisp = true ; Step.iCycle = 0 ; return true ;}
                     if(m_dVisnT < -OM.DevOptn.dPickerTolAng ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 T보정값이 설정범위를 넘었습니다."); m_bVsnErrDisp = true ; Step.iCycle = 0 ; return true ;}
                 }
                 else {
                     if(m_dVisnX >  OM.DevOptn.dPickerTolXY  ){if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riPCK].SetStat(csSkip); MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);Step.iCycle = 50 ; return false  ;}
                     if(m_dVisnY >  OM.DevOptn.dPickerTolXY  ){if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riPCK].SetStat(csSkip); MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);Step.iCycle = 50 ; return false  ;}
                     if(m_dVisnT >  OM.DevOptn.dPickerTolAng ){if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riPCK].SetStat(csSkip); MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);Step.iCycle = 50 ; return false  ;}

                     if(m_dVisnX < -OM.DevOptn.dPickerTolXY  ){if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riPCK].SetStat(csSkip); MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);Step.iCycle = 50 ; return false  ;}
                     if(m_dVisnY < -OM.DevOptn.dPickerTolXY  ){if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riPCK].SetStat(csSkip); MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);Step.iCycle = 50 ; return false  ;}
                     if(m_dVisnT < -OM.DevOptn.dPickerTolAng ){if(DM.ARAY[riPCK].GetStat(0,0)!=csNone)DM.ARAY[riPCK].SetStat(csSkip); MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);Step.iCycle = 50 ; return false  ;}





                     //DM.ARAY[riPCK].SetStat(csSkip);
                     //if(DM.ARAY[riSTG].GetStat(0,0)!=csNone)DM.ARAY[riSTG].SetStat(csSkip);
                     //Step.iCycle=50;
                     //return false  ;
                 }

                 if(MM.GetManNo() == mcVisnCal) { //비젼 캘 메뉴얼 일땐 얼라인까지만 한다.
                     Step.iCycle ++;
                     return false ;
                 }

                 if(!OM.DevOptn.bPkIgnrVisnDefect && !m_bOneSkipDefect) {
                     //JS
                     if(!SKIP_VISN)SV_Ready(viPickerDefect);
                     MoveMotr(miPCK_ZPickr , piPCK_ZPickrVsDefect);
                     Step.iCycle = 30;
                     return false ;
                 }
                 else {
                     MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                     Step.iCycle ++;
                     return false ;
                 }

        //얼라인 까지만 할때.
        case 24: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 DM.ARAY[riPCK].SetStat(csAttach);
                 m_bOneSkipDefect = false;
                 Step.iCycle=0;
                 return true ;
        //========얼라인만 검사할때 끝.



        case 30: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrVsDefect)) return false ;

                 //JS
                 if(!SKIP_VISN){
                     SV_SetPkgID(viPickerDefect , DM.ARAY[riPCK].CHPS[0][0].GetID().c_str());
                     SV_GrabInsp(viPickerDefect);
                 }
                 Step.iCycle++;
                 return false ;

        case 31: //JS
                 if(!SKIP_VISN &&!SV_GetGrabEnd(viPickerDefect)) return false ;
                 if(!SKIP_VISN &&!SV_GetInspEnd(viPickerDefect)) return false ;

                 //JS
                 if(!SKIP_VISN)SV_GetRslt(viPickerDefect , VisnRslt);
                 sVisnRslt = VisnRslt ;

                 if(sVisnRslt.Pos("OK")){
                     //"OK-12
                     sVisnRslt.Delete(1,3);                               //12
                     m_iVisnDefCnt = StrToIntDef(sVisnRslt , 0);
                     Step.iCycle = 40 ;
                 }
                 else {//"NG"
                     if(OM.CmnOptn.bVisnNgStop) {
                         EM_SetErrMsg(eiVSN_InspNG , "Picker Defect Vision Inspection NG");
                         m_bVsnErrDisp = true ;
                         sVisnRslt.Delete(1,3);                               //12
                         m_iVisnDefCnt = StrToIntDef(sVisnRslt , 0);
                         MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                         Step.iCycle=50;
                         return false ;
                     }
                     else {
                         MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                         sVisnRslt.Delete(1,3);                               //12
                         m_iVisnDefCnt = StrToIntDef(sVisnRslt , 0);
                         if(DM.ARAY[riPCK].GetStat(0,0)!=csNone) DM.ARAY[riPCK].SetStat(csSkip);
                         Step.iCycle=50;
                         return false  ;
                     }
                 }

                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 return false ;

        //OK조건
        case 40: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 //DM.ARAY[riPCK].SetStat(csAttach);
                 DM.ARAY[riPCK].SetStat(csAttach);
                 //DM.ARAY[riPCK].SetStat(csDisp  );

                 Step.iCycle=0;
                 return true ;

        //NG조건
        case 50: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;

                 Step.iCycle=0;
                 return true ;
    }

}

bool CPicker::CycleAttach(void)
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
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push
                 if(!IO_GetX(xPCK_Vccm)){
                     EM_SetErrMsg(eiPCK_VaccSensor , "픽커 배큠센서가 Off되어 있습니다.");
                     Step.iCycle = 0 ;
                     return true;
                 }

                 //메뉴얼시에 충돌 위치에 있으면 치운다.
                 if(MM.GetManNo() != scNone && !DSP.ToolVoid()) return false ;

                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 if(OM.DevInfo.iWorkMode == wmFos) MoveMotr(miPCK_YPickr , piPCK_YPickrPlaceOfs);
                 else                              MoveMotr(miPCK_YPickr , piPCK_YPickrPlace   );


                 //픽커에 관한 오프셑 + 평탄도 오프셑.
                 MT_GoIncRun(miSTG_XStage , -m_dVisnY + OM.DevOptn.dAttachStageOfsX);
                 MT_GoIncRun(miSTG_YStage ,  m_dVisnX + OM.DevOptn.dAttachStageOfsY);
                 MT_GoIncRun(miSTG_TStage ,  m_dVisnT + OM.DevOptn.dAttachStageOfsT);

                 //세타는 여기서 돌리면 안되고 Stage얼라인 할때 돌린다.
                 //20160816기존에 얼라이너를 믿고 픽커쪽 세타 보정적용을 하지 않았는데
                 //FOS모드와 DIE모드 일때 얼라이너가 좀 틀어진다.
                 //어쩔수 없이 보정을 하는데 사실 위의 X,Y 이동량을 세타랑 같이 계산해야 하는데.
                 //너무 검증시간이 오래걸릴것 같아서 일단 그냥 세타만 보정하고 쎄타에 따른 X,Y이동량 보정은 스킵.
                 //MT_GoIncRun(miSTG_TStage , 


                 //무게 센서 에러 미리 감지 할려고 함.
                 Rs232_LoadCl.WeightCheck(); //출발 하기전에 현재 무게 확인 및 통신 테스트..
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 12: if(m_tmDelay.OnDelay(true , 1000)) {
                     EM_SetErrMsg(eiPCK_LoadCell , "로드셀 통신이 지연되었습니다.");
                     Step.iCycle=0;
                     return true ;
                 }
                 if(!Rs232_LoadCl.GetMsgEnd()) return false;
                 //세타 보정은 하지 않는다..
                 //얼라이너 믿고 가자..

                 Step.iCycle++;
                 return false ;

        case 13: if(OM.DevInfo.iWorkMode == wmFos && !MoveMotr(miPCK_YPickr , piPCK_YPickrPlaceOfs)) return false ;
                 if(OM.DevInfo.iWorkMode != wmFos && !MoveMotr(miPCK_YPickr , piPCK_YPickrPlace   )) return false ;
                 if(!MT_GetStopInpos(miSTG_XStage)) return false ;
                 if(!MT_GetStopInpos(miSTG_YStage)) return false ;

                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrPlace);
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrPlace)) return false ;
                 DM.ARAY[riPCK].SetStat(csWait);
                 if(OM.DevInfo.iWorkMode == wmFos){ DM.ARAY[riSTG].SetStat(csDisp);}
                 else                             { DM.ARAY[riSTG].SetStat(csPush);}


                 Step.iCycle=0;
                 return true ;
    }
}

bool CPicker::CyclePush()
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 + OM.DevOptn.iPkPushDelay)) {
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

    double dVel1   = 0.0 ;
    double dVel2   = 0.0 ;
    double dVel3   = 0.0 ;
    double dVel4   = 0.0 ;
    double dVelMax = 0.0 ;

    double dTemp1 , dTemp2 , dTemp3 , dTemp4 ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10:
                 //if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push
                 //메뉴얼시에 충돌 위치에 있으면 치운다.
                 if(MM.GetManNo() != scNone && !DSP.ToolVoid()) return false ;
                 if(!IO_GetX(xPCK_Vccm)&& MM.GetManNo() != scNone){
                     EM_SetErrMsg(eiPCK_VaccSensor , "픽커 배큠센서가 Off되어 있습니다.");
                     Step.iCycle = 0 ;
                     return true;
                 }


                 dTemp1 = MT_GetTrgPos(miPCK_YPickr) ;
                 dTemp2 = GetMotrPos(miPCK_YPickr , piPCK_YPickrPlaceOfs) ;
                 dTemp3 = MT_GetTrgPos(miPCK_ZPickr) ;
                 dTemp4 = GetMotrPos(miPCK_ZPickr , piPCK_ZPickrPlace   ) ;
                 if((MT_GetTrgPos(miPCK_YPickr) == GetMotrPos(miPCK_YPickr , piPCK_YPickrPlaceOfs) ||
                     MT_GetTrgPos(miPCK_YPickr) == GetMotrPos(miPCK_YPickr , piPCK_YPickrPlace   )) &&
                     MT_GetTrgPos(miPCK_ZPickr) == GetMotrPos(miPCK_ZPickr , piPCK_ZPickrPlace   ) ){
                     //MT_GoAbsSlow(miPCK_YPickr , GetMotrPos(miPCK_YPickr , piPCK_YPickrPlace));
                     Step.iCycle=12;
                     return false ;
                 }


                 //시간이 오래 걸림.
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 MoveMotr(miPCK_YPickr , piPCK_YPickrPlace);

                 Step.iCycle++;
                 return false ;

        //위에서 씀
        case 12: if(!MT_GetStopInpos(miPCK_YPickr) ) return false ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrPlace);
                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrPlace)) return false ;
                 Rs232_LoadCl.WeightCheck(); //Y축 출발 하기전에 현재 무게 확인 및 통신 테스트..
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 14: if(m_tmDelay.OnDelay(true , 2000)) {
                     EM_SetErrMsg(eiPCK_LoadCell , "로드셀 통신이 지연되었습니다.");
                     //일단 붙이고 보자..
                     //Step.iCycle=0;
                     //return true ;
                 }
                 if(!Rs232_LoadCl.GetMsgEnd      ()) return false;
                 if( Rs232_LoadCl.GetLoadCellData() * 1000 > 50) {
                     EM_SetErrMsg(eiPCK_LoadCell , "로드셀의 무게가 Y축 이동 전에 50g 이상입니다.");


                     //일단 붙이고 보자..
                     Step.iCycle=21;
                     return false ;
                 }








                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 15: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iFosPlaceBfDelay)) return false ;

                 //MoveMotr(miPCK_YAlign , piPCK_YPickrPlace);
                 //MT_GoAbsSlow(miPCK_YPickr , GetMotrPos(miPCK_YPickr , piPCK_YPickrPlace));
                 MT_GoAbsAcDc(miPCK_YPickr , GetMotrPos(miPCK_YPickr , piPCK_YPickrPlace) , OM.DevOptn.dFosPlaceSttYSpd , OM.DevOptn.dFosPlaceSttYSpd * 10 , OM.DevOptn.dFosPlaceSttYSpd * 10 );
                 Step.iCycle++;
                 return false ;

        case 16: if(!MT_GetStopInpos(miPCK_YPickr) ) return false ;
                 Rs232_LoadCl.WeightCheck(); //출발 하기전에 현재 무게 확인 및 통신 테스트..
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 17: if(m_tmDelay.OnDelay(true , 2000)) {
                     EM_SetErrMsg(eiPCK_LoadCell , "로드셀 통신이 지연되었습니다.");
                     //일단 붙이고 보자..
                     Step.iCycle=21;
                     return false ;
                 }
                 if(!Rs232_LoadCl.GetMsgEnd      ()) return false;
                 if( Rs232_LoadCl.GetLoadCellData() * 1000 > OM.DevOptn.dAttachForce + OM.DevOptn.dAttachForceOfs) {
                     EM_SetErrMsg(eiPCK_LoadCell , "로드셀의 무게가 누르기 전에 설정값 이상입니다.");


                     //일단 붙이고 보자..
                     Step.iCycle=21;
                     return false ;
                 }

                 //누르는것 속도 2단계로
                 dVel1 = OM.DevOptn.dAttachSpeed1 > MT_GetRunVel(miPCK_ZPickr) ? MT_GetRunVel(miPCK_ZPickr) : OM.DevOptn.dAttachSpeed1 ;
                 dVel2 = OM.DevOptn.dAttachSpeed2 > MT_GetRunVel(miPCK_ZPickr) ? MT_GetRunVel(miPCK_ZPickr) : OM.DevOptn.dAttachSpeed2 ;
                 dVelMax = dVel1 > dVel2 ? dVel1 : dVel2 ;
                 MT_SetOverrideMaxSpeed(miPCK_ZPickr , dVelMax) ;
                 MT_SetOverrideVelAtPos(miPCK_ZPickr ,
                                        GetMotrPos(miPCK_ZPickr , piPCK_ZPickrCheck) - m_dPkPkgThickness + 1.0, //로드셀파손...+ 0.5 , // 약1.5mm이동하면 5Kg 나온다.
                                        OM.DevOptn.dAttachSpeed1 ,
                                        GetMotrPos(miPCK_ZPickr , piPCK_ZPickrCheck) - m_dPkPkgThickness - OM.DevOptn.dPlaceZOfs2 ,
                                        OM.DevOptn.dAttachSpeed2,
                                        true);

                 //MT_GoInc(miPCK_ZPickr , OM.DevOptn.dPlaceZOfs1 - OM.DevOptn.dPlaceZOfs2 , OM.DevInfo.dAttachSpeed1);

                 Step.iCycle++;
                 return false ;

        //밑에서 씀.
        case 18: Rs232_LoadCl.WeightCheck();
                 Step.iCycle++;
                 return false ;

        case 19: if(!Rs232_LoadCl.GetMsgEnd      ()) return false;
                 if( Rs232_LoadCl.GetLoadCellData() * 1000 > OM.DevOptn.dAttachForce+ OM.DevOptn.dAttachForceOfs) {
                     MT_Stop(miPCK_ZPickr);  //급 정지시 포지션 틀어질 위험... 있음.
                 }

                 if(!MT_GetStop(miPCK_ZPickr)) {
                     Step.iCycle = 18 ;
                     return false ;
                 }


                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false ;


        case 20: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iPkPushDelay)) return false ;
                 Step.iCycle++;
                 return false ;

        //위에서 씀.
        case 21: //<Temp>                      IO_SetY(yPCK_VccmOff , true);
                 IO_SetY(yPCK_VccmOn , false );
                 Rs232_LoadCl.WeightCheck();
                 //IO_SetY(yPCK_Ejct , true  );
                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false ;

        case 22: if(!m_tmDelay.OnDelay(true , 2000)) return false ;  //여기서 포지션 틀어질수도 있음.
                 if(!Rs232_LoadCl.GetMsgEnd      ()) return false; //일단 제껴보고 나중에 문제 되면 확인할까???
                 IO_SetY(yPCK_VccmOff , false);
                 STG.Spc.dPushWeight = Rs232_LoadCl.GetLoadCellData();
                 MT_GoInc(miPCK_ZPickr , -2.0 , 1.0 );

                 Step.iCycle++;
                 return false ;

        case 23: if(!MT_GetStop(miPCK_ZPickr))return false ;
                 IO_SetY(yPCK_Ejct , true  );
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 24: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 IO_SetY(yPCK_Ejct , false  );
                 MoveMotr(miPCK_YPickr , piPCK_YPickrAlign) ;

                 Step.iCycle++;
                 return false ;

        case 25: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrAlign)) return false ;
                 DM.ARAY[riSTG].SetStat(csVisnAt);
                 DM.ARAY[riPCK].SetStat(csNone  );

                 int  iWorkingCol ;
                 EN_ARAY_ID riWork ;
                 FindChip(iWorkingCol ,riWork);

                 //보트 맨마지막 자제 작업 하면 혹시 빠져 나갈까봐 여기서 세팅 해주고 Stage에서 저장 한다.
                 STG.Spc.sWorkFrntBoatId   = DM.ARAY[riIDX_F].GetID();
                 STG.Spc.iWorkFrntBoatSlot = OM.DevInfo.iBoatColCnt - iWorkingCol;
                 STG.Spc.sWorkFrntChipId   = DM.ARAY[riPCK].CHPS[0][0].GetID() ;

                 DM.ARAY[riIDX_F].SetStat(0,iWorkingCol,csEmpty);

                 Step.iCycle=0;
                 return true ;

    }
}

//One Cycle.
bool CPicker::CycleBoatPlace()
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
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: Step.iCycle ++;
                 return false;


        //위에서 씀.
        case 11: if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push
                 if(!MT_GetStop(miPCK_ZPickr)) return false ; //Skip 마지막에 확인 위해.
                 //if(IO_GetX(xPCK_Vccm)){
                 //    EM_SetErrMsg(eiPCK_VaccSensor , "픽커 배큠센서가 On되어 있습니다.");
                 //    Step.iCycle = 0 ;
                 //    return true;
                 //}
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;

                 MoveMotr(miPCK_YPickr , piPCK_YPickrPick);
                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrPick)) return false ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrPick);
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrPick)) return false ;
                 MT_GoAbsRun(miPCK_ZPickr , PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrPick) - 1.0) ;
                 Step.iCycle++;
                 return false ;

        case 15: if(!MT_GetStopInpos(miPCK_ZPickr)) return false ;
                 //<Temp>                      IO_SetY(yPCK_VccmOff , true);
                 IO_SetY(yPCK_VccmOn , false );
                 IO_SetY(yPCK_Ejct , true );

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 16: if(!m_tmDelay.OnDelay(true , 400)) return false ;
                 IO_SetY(yPCK_VccmOff , false);
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 17: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 int  iWorkingCol ;
                 EN_ARAY_ID riWork ;
                 FindChip(iWorkingCol ,riWork);
                 DM.ARAY[riIDX_F].SetStat(0,iWorkingCol,csSkip);

                 if(m_bNeedLotEnd){
                     DM.ARAY[riIDX_F].ChangeStat(csUnkwn,csSkip);
                     if(!DM.ARAY[riPRI_F].CheckAllStat(csNone))DM.ARAY[riPRI_F].SetStat(csSkip);
                     m_bNeedLotEnd = false ;
                 }


                 DM.ARAY[riPCK].SetStat(csNone);

                 IO_SetY(yPCK_Ejct , false);

                 Step.iCycle = 0;
                 return true ;
    }
}

//One Cycle.
bool CPicker::CycleVoid()
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
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10: Step.iCycle ++;
                 return false;


        //위에서 씀.
        case 11: if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push
                 if(!MT_GetStop(miPCK_ZPickr)) return false ; //Skip 마지막에 확인 위해.
                 //if(IO_GetX(xPCK_Vccm)){
                 //    EM_SetErrMsg(eiPCK_VaccSensor , "픽커 배큠센서가 On되어 있습니다.");
                 //    Step.iCycle = 0 ;
                 //    return true;
                 //}
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;

                 MoveMotr(miPCK_YPickr , piPCK_YPickrAlign);
                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrAlign)) return false ;
                 Step.iCycle = 0;
                 return true ;
    }
}

bool CPicker::CycleManStdCal()
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

    double dRezeroOfs ;

    //Cycle.
    switch (Step.iCycle) {
        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push

                 //메뉴얼시에 충돌 위치에 있으면 치운다.
                 if(MM.GetManNo() != scNone && !DSP.ToolVoid()) return false ;


                 if(IO_GetX(xPCK_Vccm)){
                     EM_SetErrMsg(eiPCK_VaccSensor , "픽커 배큠센서가 감지 되어 있습니다.");
                     Step.iCycle = 0 ;
                     return true;
                 }
                     MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 DSP.MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                 Step.iCycle++;
                 return false ;

        case 11: if(!    MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 if(!DSP.MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;

                 DSP.MoveMotr(miDSP_YDispr , piDSP_YDisprWait);
                 DSP.MoveMotr(miDSP_XDispr , piDSP_XDisprWait);
                 Step.iCycle++;
                 return false ;

        case 12: if(!DSP.MoveMotr(miDSP_YDispr , piDSP_YDisprWait)) return false ;
                 if(!DSP.MoveMotr(miDSP_XDispr , piDSP_XDisprWait)) return false ;

                 MoveMotr(miPCK_YPickr , piPCK_YPickrCheck);

                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrCheck)) return false ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck , true);

                 Step.iCycle++;
                 return false ;

        case 14: if(!IO_GetX(xDSP_NdleNotCheck)) {//기존체크위치 가다가 체크되면 스탑.
                     MT_EmgStop(miPCK_ZPickr);
                 }
                 if(!MT_GetStop(miPCK_ZPickr)) return false ;
                 if(IO_GetX(xDSP_NdleNotCheck)){//체크센서 감지 안되면 다시 3미리 보내 본다.
                     MT_GoInc(miPCK_ZPickr , 3.0 , 1.0);
                 }
                 Step.iCycle++ ;
                 return false ;

        case 15: if(!IO_GetX(xDSP_NdleNotCheck)) {//기존체크위치 가다가 체크되면 스탑.
                     MT_EmgStop(miPCK_ZPickr);
                 }
                 if(!MT_GetStop(miPCK_ZPickr)) return false ;
                 if(IO_GetX(xDSP_NdleNotCheck)){
                     Step.iCycle = 30;
                     return false;
                 }

                 //체크 완료 되면 뒤로 빼면서...
                 //체크 해제
                 MT_GoInc(miPCK_ZPickr , -10.0 , 0.1);
                 Step.iCycle++;
                 return false ;

        case 16: if(IO_GetX(xDSP_NdleNotCheck)) {//기존체크위치 가다가 체크되면 스탑.
                     MT_EmgStop(miPCK_ZPickr);
                 }
                 if(!MT_GetStop(miPCK_ZPickr)) return false ;
                 dRezeroOfs = PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrCheck) - MT_GetCmdPos(miPCK_ZPickr);
                 PM.SetValue(miPCK_ZPickr , pvPCK_ZPickrCheck , MT_GetCmdPos(miPCK_ZPickr));
                 PM.SetValue(miPCK_ZPickr , pvPCK_ZPickrMove  , PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrMove ) - dRezeroOfs);
                 PM.SetValue(miPCK_ZPickr , pvPCK_ZPickrPick  , PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrPick ) - dRezeroOfs);
                 PM.SetValue(miPCK_ZPickr , pvPCK_ZPickrAlign , PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrAlign) - dRezeroOfs);
                 PM.SetValue(miPCK_ZPickr , pvPCK_ZPickrWait  , PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrWait ) - dRezeroOfs);
                 m_bRqstPMSave = true ;

                 //Z축 올리고 대기위치로 이동하는 것 만들기.
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrWait);
                 Step.iCycle++;
                 return false ;

        case 17: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);
                 Step.iCycle++;
                 return false ;

        case 18: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrHeight)) return false ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck);
                 Step.iCycle++;
                 return false;

        case 19: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck)) return false;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 20: if(!m_tmDelay.OnDelay(true, 300)) return false;
                 Step.iCycle++;
                 return false;

        case 21: Rs232Keyence.SetZero(kcChA); //실제 피커가 터치센서 위치에서 0점을 잡는다.
                 Step.iCycle++;
                 return false;

        case 22: if(!Rs232Keyence.GetMsgEnd())return false;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false;

        case 23: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false;
                 Step.iCycle++;
                 return false;

        case 24: m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 25: if(!m_tmDelay.OnDelay(500)) return false ;
                 Rs232Keyence.CheckHeight(kcChA);
                 Step.iCycle++;
                 return false ;

        case 26: if(!Rs232Keyence.GetMsgEnd()) return false; //가끔 껏따키면 지연 시간 5초 거림 확인 해야함.
                 if(0 == Rs232Keyence.GetHeightData(kcChA)) {
                     EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 데이터가 이상합니다. 다시 수행해 주세요.");
                     Step.iCycle = 0 ;
                     return true ;
                 }

                 if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChA)) {
                     EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 측정포지션을 확인하세요.");
                     Step.iCycle = 0 ;
                     return true ;
                 }

                 m_dEmptyToolHeight = Rs232Keyence.GetHeightData(kcChA);
                 MoveMotr(miPCK_YPickr , piPCK_YPickrWait);
                 Step.iCycle++;
                 return false ;

        case 27: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrWait)) return false;
                 Step.iCycle=0;
                 return true ;

        //Error일때 Z축 위로 올려놓고 스탑.
        case 30: MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false;

        case 31: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false;
                 EM_SetErrMsg(eiPRT_CheckErr, "Picker가 높이 세팅을 하지 못하였습니다.");
                 Step.iCycle = 0;
                 return true;
    }
}


bool CPicker::CycleManVsDefectPos()
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
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //JS
                 if(!SKIP_VISN)SV_Ready(viPickerDefect); //미리 카메라 세팅 바꿔 놓는다.

                 //시간이 오래 걸림.
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 11: //메뉴얼시에 충돌 위치에 있으면 치운다.
                 if(MM.GetManNo() != scNone && !DSP.ToolVoid()) return false ;
                 if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrHeight)) return false ;
                 //MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck);
                 Step.iCycle++;
                 return false;

        case 13: //if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck)) return false;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 14: if(!m_tmDelay.OnDelay(true , 300)) return false;
                 Rs232Keyence.CheckHeight(kcChA);
                 Step.iCycle++;
                 return false ;

        case 15: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChA)) {
                     EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 측정포지션을 확인하세요.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 m_dPkPkgThickness = Rs232Keyence.GetHeightData(kcChA) - m_dEmptyToolHeight;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false;

        case 16: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false;
                 MoveMotr(miPCK_YPickr , piPCK_YPickrVision);
                 Step.iCycle++;
                 return false ;

        case 17: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrVision)) return false ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrVsDefect);
                 Step.iCycle++;
                 return false ;

        case 18: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrVsDefect)) return false ;
                 Step.iCycle=0;
                 return true ;
    }
}

bool CPicker::CycleManVsAlignPos()
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
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: if(!SKIP_VISN)SV_Ready(viPickerDefect); //미리 카메라 세팅 바꿔 놓는다.

                 //시간이 오래 걸림.
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false ;

        case 11: //메뉴얼시에 충돌 위치에 있으면 치운다.
                 if(MM.GetManNo() != scNone && !DSP.ToolVoid()) return false ;
                 if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrHeight)) return false ;
                 //MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck);
                 Step.iCycle++;
                 return false;

        case 13: //if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck))return false;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 14: if(!m_tmDelay.OnDelay(true, 300)) return false;
                 Rs232Keyence.CheckHeight(kcChA);
                 Step.iCycle++;
                 return false ;

        case 15: if(!Rs232Keyence.GetMsgEnd()) return false ;  //센서에서 멀어지면 - 이고 센서에서 가까워지면 +
                 if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChA)) {
                     EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 측정포지션을 확인하세요.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 m_dPkPkgThickness = Rs232Keyence.GetHeightData(kcChA)- m_dEmptyToolHeight ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);

                 Step.iCycle++;
                 return false ;

        case 16: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false;
                 MoveMotr(miPCK_YPickr , piPCK_YPickrVision);
                 Step.iCycle++;
                 return false ;

        case 17: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrVision)) return false ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrVsAlign);
                 Step.iCycle++;
                 return false ;

        case 18: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrVsAlign)) return false ;
                 Step.iCycle=0;
                 return true ;
    }
}

bool CPicker::CycleManAlgnPlce()
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
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                  DSP.MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove);
                  Step.iCycle++;
                  return false;

        case  11: if(!PCK.MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                  if(!DSP.MoveMotr(miDSP_ZDispr , piDSP_ZDisprMove)) return false ;
                  MoveMotr(miPCK_YPickr , piPCK_YPickrAlign);
                  MoveMotr(miPCK_XAlign , piPCK_XAlignWait );
                  MoveMotr(miPCK_YAlign , piPCK_YAlignWait );
                  MoveActr(aiPCK_XAlgnFwBw , ccBwd);
                  MoveActr(aiPCK_YAlgnFwBw , ccBwd);

                  DSP.MoveMotr(miDSP_YDispr , piDSP_YDisprWait );

                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrAlign)) return false ;
                  if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWait )) return false ;
                  if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWait  )) return false ;
                  if(!MoveActr(aiPCK_XAlgnFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiPCK_YAlgnFwBw , ccBwd)) return false ;

                  if(!DSP.MoveMotr(miDSP_YDispr , piDSP_YDisprWait )) return false ;

                  MT_GoAbsRun(miPCK_ZPickr , PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrAlign)-0.5 );
                  Step.iCycle++;
                  return false ;

        case  13: if(!MT_GetStopInpos(miPCK_ZPickr)) return false ;
                  //<Temp>                      IO_SetY(yPCK_VccmOff , true);
                  IO_SetY(yPCK_VccmOn , false );
                  IO_SetY(yPCK_Ejct , true );
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

        case  14: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                  IO_SetY(yPCK_VccmOff , false);
                  MoveMotr(miPCK_ZPickr , pvPCK_ZPickrMove);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveMotr(miPCK_ZPickr , pvPCK_ZPickrMove)) return false;
                  if(!DM.ARAY[riPCK].GetStat(0,0)==csNone )DM.ARAY[riALN].SetStat(csUnkwn);
                  DM.ARAY[riPCK].SetStat(csNone );
                  IO_SetY(yPCK_Ejct , false );
                  Step.iCycle=0;
                  return true ;
    }
}

bool CPicker::CycleManAlignerCal(void)
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
                  if(Step.iCycle != PreStep.iCycle)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: //if(!MT_GetStop(miPCK_YAlign)) return false ; //마지막에 Push
                 //메뉴얼시에 충돌 위치에 있으면 치운다.
                 if(MM.GetManNo() != scNone && !DSP.ToolVoid()) return false ;
                 if(IO_GetX(xPCK_Vccm)){
                     EM_SetErrMsg(eiPCK_VaccSensor , "픽커 배큠센서가 On되어 있습니다.");
                     Step.iCycle = 0 ;
                     return true;
                 }

                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove );
                 MoveMotr(miPCK_XAlign , piPCK_XAlignWait );
                 MoveMotr(miPCK_YAlign , piPCK_YAlignWait );
                 MoveActr(aiPCK_XAlgnFwBw , ccBwd);
                 MoveActr(aiPCK_YAlgnFwBw , ccBwd);

                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove )) return false ;
                 if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWait )) return false ;
                 if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWait )) return false ;
                 if(!MoveActr(aiPCK_XAlgnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiPCK_YAlgnFwBw , ccBwd)) return false ;
                 IO_SetY(yPCK_Ejct , false);
                 //<Temp>                      IO_SetY(yPCK_VccmOff , true);
                 IO_SetY(yPCK_VccmOn , false );
                 MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);
                 MoveMotr(miPCK_XAlign , piPCK_XAlignWork );
                 MoveMotr(miPCK_YAlign , piPCK_YAlignWork );

                 Step.iCycle++;
                 return false;

        case 12: if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWork )) return false ;
                 if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWork )) return false ;
                 IO_SetY(yPCK_VccmOff , false);
                 //MoveActr(aiPCK_XAlgnFwBw , ccFwd);
                 //MoveActr(aiPCK_YAlgnFwBw , ccFwd);
                 MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 13: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrHeight)) return false ;
                 if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 Rs232Keyence.CheckHeight(kcChA);
                 //if(!MoveActr(aiPCK_XAlgnFwBw , ccFwd)) return false ;
                 //if(!MoveActr(aiPCK_YAlgnFwBw , ccFwd)) return false ;
                 Step.iCycle++;
                 return false ;

        case 14: if(!Rs232Keyence.GetMsgEnd()) return false;
                 if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChA)) {
                     EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 측정포지션을 확인하세요.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 m_dEmptyToolHeight = Rs232Keyence.GetHeightData(kcChA);
                 MoveMotr(miPCK_YPickr , piPCK_YPickrAlign);

                 Step.iCycle++;
                 return false ;

        case 15: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrAlign)) return false ;
                 MoveActr(aiPCK_XAlgnFwBw , ccFwd);
                 MoveActr(aiPCK_YAlgnFwBw , ccFwd);
                 Step.iCycle++;
                 return false;

        case 16: if(!MoveActr(aiPCK_XAlgnFwBw , ccFwd)) return false;
                 if(!MoveActr(aiPCK_YAlgnFwBw , ccFwd)) return false;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrAlign);
                 Step.iCycle++;
                 return false ;


        case 17: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrAlign)) return false ;
                 IO_SetY(yPCK_VccmOn , true );
                 IO_SetY(yPCK_Ejct , false);
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false;

        case 18: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iPkPickDelay)) return false ;
                 MoveActr(aiPCK_XAlgnFwBw , ccBwd);
                 MoveActr(aiPCK_YAlgnFwBw , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 19: if(!MoveActr(aiPCK_XAlgnFwBw , ccBwd)) return false ;
                 if(!MoveActr(aiPCK_YAlgnFwBw , ccBwd)) return false ;
                 //<Temp>IO_SetY(yPCK_VccmOn , false);
                 MoveMotr(miPCK_XAlign , piPCK_XAlignWait );
                 MoveMotr(miPCK_YAlign , piPCK_YAlignWait );
                 Step.iCycle++;
                 return false;

        case 20: if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWait )) return false ;
                 if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWait )) return false ;
                 MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 Step.iCycle++;
                 return false;

        case 21: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 22: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                 if(!IO_GetX(xPCK_Vccm)){
                     EM_SetErrMsg(eiPCK_PickMiss , "Picker가 설정시간 동안 자제를 집지 못했습니다.");
                     Step.iCycle=0;
                     return true ;
                 }

                 //DM.ARAY[riALN].SetStat(csNone);
                 //DM.ARAY[riPCK].SetStat(csVisn);
                 Step.iCycle = 30 ;
                 return false ;

        //흠...중복 동작시 삭제요청.
        //높이 측정.
        case 30: MoveMotr(miPCK_YPickr , piPCK_YPickrHeight);
                 Step.iCycle++;
                 return false ;

        case 31: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrHeight)) return false ;
                 //MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck);
                 Step.iCycle++;
                 return false;

        case 32: //if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrCheck)) return false;
                 //여기서 높이 측정 하고 높이 보정 한다.
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 33: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                 Rs232Keyence.CheckHeight(kcChA);
                 Step.iCycle++;
                 return false ;

        case 34: if(!Rs232Keyence.GetMsgEnd()) return false ;
                 if(KEYENCE_ERR == Rs232Keyence.GetHeightData(kcChA)) {
                     EM_SetErrMsg(eiETC_RS232 , "픽커언더 높이 측정기 측정포지션을 확인하세요.");
                     Step.iCycle = 0 ;
                     return true ;
                 }
                 m_dPkPkgThickness = Rs232Keyence.GetHeightData(kcChA) - m_dEmptyToolHeight ;

                 MoveMotr(miPCK_YPickr , piPCK_YPickrVision);
                 Step.iCycle=40;
                 return false ;

        //Vision 검사 준비완료....
        //Align Vision===========================================================
        case 40: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrVision)) return false ;
                 Step.iCycle++;
                 return false ;


        case 41: MoveMotr(miPCK_ZPickr , piPCK_ZPickrVsAlign);
                 Step.iCycle++;
                 return false ;

        case 42: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrVsAlign)) return false ;
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 43: if(!m_tmDelay.OnDelay(true , 100)) return false ;
                 //JS
                 if(!SKIP_VISN)SV_GrabInsp(viPickerAlign);
                 Step.iCycle++;
                 return false ;

        case 44: //JS
                 //if(!SKIP_VISN && !SV_GetInspEnd(viPickerAlign)) return false ;
                 if(!SKIP_VISN &&!SV_GetGrabEnd(viPickerAlign)) return false ;
                 if(!SKIP_VISN &&!SV_GetInspEnd(viPickerAlign)) return false ;
                 //JS
                 if(!SKIP_VISN)SV_GetRslt(viPickerAlign , VisnRslt);
                 sVisnRslt = VisnRslt ;

                 if(sVisnRslt.Pos("OK")){                                 //"OK-0.23145,0.12344,0.32123
                     sVisnRslt.Delete(1,3);                               //0.23145,0.12344,0.32123
                     //X
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.23145
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));              //0.12344,0.32123
                     sItem.Delete(1,2);
                     m_dVisnX = StrToFloatDef(sItem , 0.0);

                     //Y
                     sItem = sVisnRslt.SubString(1,sVisnRslt.Pos(",")-1); //0.12344
                     sVisnRslt.Delete(1,sVisnRslt.Pos(","));
                     sItem.Delete(1,2);
                     m_dVisnY = StrToFloatDef(sItem , 0.0);

                     //T
                     sItem = sVisnRslt ;
                     sItem.Delete(1,2);
                     m_dVisnT = StrToFloatDef(sItem , 0.0);               //0.32123

                     m_bVisn = true ;
                 }
                 else {//"NG"
                     m_dVisnX = 0.0 ;
                     m_dVisnY = 0.0 ;
                     m_dVisnT = 0.0 ;

                     m_bVisn = false ;

                     EM_SetErrMsg(eiVSN_InspNG , "Picker Align Vision Inspection NG");
                     MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                     Step.iCycle=0;
                     return true ;
                 }

                 if(m_dVisnX >  OM.DevOptn.dPickerTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 X보정값이 설정범위를 넘었습니다."); Step.iCycle = 0 ; return true ;}
                 if(m_dVisnY >  OM.DevOptn.dPickerTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 Y보정값이 설정범위를 넘었습니다."); Step.iCycle = 0 ; return true ;}
                 if(m_dVisnT >  OM.DevOptn.dPickerTolAng ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 T보정값이 설정범위를 넘었습니다."); Step.iCycle = 0 ; return true ;}

                 if(m_dVisnX < -OM.DevOptn.dPickerTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 X보정값이 설정범위를 넘었습니다."); Step.iCycle = 0 ; return true ;}
                 if(m_dVisnY < -OM.DevOptn.dPickerTolXY  ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 Y보정값이 설정범위를 넘었습니다."); Step.iCycle = 0 ; return true ;}
                 if(m_dVisnT < -OM.DevOptn.dPickerTolAng ){EM_SetErrMsg(eiVSN_InspNG , "Picker Align 비젼의 T보정값이 설정범위를 넘었습니다."); Step.iCycle = 0 ; return true ;}

                 PM.SetValue(miPCK_XAlign , pvPCK_XAlignWork , PM.GetValue(miPCK_XAlign , pvPCK_XAlignWork) + m_dVisnY);
                 PM.SetValue(miPCK_YAlign , pvPCK_YAlignWork , PM.GetValue(miPCK_YAlign , pvPCK_YAlignWork) + m_dVisnX);
                 m_bRqstPMSave = true ;
                 Step.iCycle=50;
                 return false ;


        //플레이스 페턴.
        case  50: MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                  Step.iCycle++;
                  return false;

        case  51: if(!MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove)) return false ;
                  MoveMotr(miPCK_YPickr , piPCK_YPickrAlign);
                  MoveMotr(miPCK_XAlign , piPCK_XAlignWait );
                  MoveMotr(miPCK_YAlign , piPCK_YAlignWait );
                  MoveActr(aiPCK_XAlgnFwBw , ccBwd);
                  MoveActr(aiPCK_YAlgnFwBw , ccBwd);

                  Step.iCycle++;
                  return false;

        case  52: if(!MoveMotr(miPCK_YPickr , piPCK_YPickrAlign)) return false ;
                  if(!MoveMotr(miPCK_XAlign , piPCK_XAlignWait )) return false ;
                  if(!MoveMotr(miPCK_YAlign , piPCK_YAlignWait )) return false ;
                  if(!MoveActr(aiPCK_XAlgnFwBw , ccBwd)) return false ;
                  if(!MoveActr(aiPCK_YAlgnFwBw , ccBwd)) return false ;

                  MT_GoAbsRun(miPCK_ZPickr , PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrAlign)-0.5 );
                  Step.iCycle++;
                  return false ;

        case  53: if(!MT_GetStopInpos(miPCK_ZPickr)) return false ;
                  //<Temp>                      IO_SetY(yPCK_VccmOff , true);
                  IO_SetY(yPCK_VccmOn , false );
                  IO_SetY(yPCK_Ejct , true );
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false ;

        case  54: if(!m_tmDelay.OnDelay(true , 300)) return false ;
                  IO_SetY(yPCK_VccmOff , false);
                  MoveMotr(miPCK_ZPickr , pvPCK_ZPickrMove);
                  Step.iCycle++;
                  return false ;

        case  55: if(!MoveMotr(miPCK_ZPickr , pvPCK_ZPickrMove)) return false;
                  IO_SetY(yPCK_Ejct , false );
                  Step.iCycle=0;
                  return true ;
    }

}

bool CPicker::ToolVoid() //디스펜서 일할때 피해주기.
{
    bool bZPosOk = MT_GetTrgPos(miPCK_ZPickr) <= PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrMove ) && MT_GetStopInpos(miPCK_ZPickr) ;
    bool bYPosOk = MT_GetTrgPos(miPCK_YPickr) <= PM.GetValue(miPCK_YPickr , pvPCK_YPickrAlign) && MT_GetStopInpos(miPCK_YPickr) ;

    bool bRetZ = true ;
    bool bRetY = true ;

    if(!bZPosOk) {
        bRetZ = MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
    }
    else {
        bRetZ = true ;
    }
    if(!bRetZ) return false ;

    if(!bYPosOk) {
        bRetY = MoveMotr(miPCK_YPickr , piPCK_YPickrAlign);
    }
    else {
        bRetY = true ;
    }
    return bRetY ;
}



bool CPicker::GetVisnRslt(double * _dVisnX , double * _dVisnY , double * _dVisnT)
{
    * _dVisnX = m_dVisnX ;
    * _dVisnY = m_dVisnY ;
    * _dVisnT = m_dVisnT ;

    return m_bVisn ;
}


//---------------------------------------------------------------------------
bool CPicker::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(_iMotr == miPCK_ZPickr){

        if(!MT_GetStop(miPCK_YPickr)){
            sMsg = MT_GetName(miPCK_YPickr) + String(" 모터가 이동중 입니다.") ;
            bRet = false ;
        }

        if(_iPstnId == piPCK_ZPickrWait    ){
        }
        if(_iPstnId == piPCK_ZPickrMove    ){
        }
        if(_iPstnId == piPCK_ZPickrPick    ){
            if(GetMotrPos(miPCK_YPickr , piPCK_YPickrPick) != MT_GetTrgPos(miPCK_YPickr)){
                sMsg = MT_GetName(miPCK_YPickr) + String(" 모터가 Pick 포지션이 아닙니다.") ;
                bRet = false ;
            }
        }
        if(_iPstnId == piPCK_ZPickrAlign   ){
            if(GetMotrPos(miPCK_YPickr , piPCK_YPickrAlign) != MT_GetTrgPos(miPCK_YPickr)){
                sMsg = MT_GetName(miPCK_YPickr) + String(" 모터가 Align 포지션이 아닙니다.") ;
                bRet = false ;
            }
        }
        if(_iPstnId == piPCK_ZPickrCheck   ){
            if(GetMotrPos(miPCK_YPickr , piPCK_YPickrCheck ) != MT_GetTrgPos(miPCK_YPickr) &&
               GetMotrPos(miPCK_YPickr , piPCK_YPickrHeight) != MT_GetTrgPos(miPCK_YPickr) ){
                sMsg = MT_GetName(miPCK_YPickr) + String(" 모터가 Check 포지션이 아닙니다.") ;
                bRet = false ;
            }
        }
        if(_iPstnId == piPCK_ZPickrVsDefect){
            if(GetMotrPos(miPCK_YPickr , piPCK_YPickrVision) != MT_GetTrgPos(miPCK_YPickr)){
                sMsg = MT_GetName(miPCK_YPickr) + String(" 모터가 Vision 포지션이 아닙니다.") ;
                bRet = false ;
            }
        }
        if(_iPstnId == piPCK_ZPickrVsAlign ){
            if(GetMotrPos(miPCK_YPickr , piPCK_YPickrVision) != MT_GetTrgPos(miPCK_YPickr)){
                sMsg = MT_GetName(miPCK_YPickr) + String(" 모터가 Vision 포지션이 아닙니다.") ;
                bRet = false ;
            }
        }
        if(_iPstnId == piPCK_ZPickrPlace   ){
            if(GetMotrPos(miPCK_YPickr , piPCK_YPickrPlace   ) != MT_GetTrgPos(miPCK_YPickr)&&
               GetMotrPos(miPCK_YPickr , piPCK_YPickrPlaceOfs) != MT_GetTrgPos(miPCK_YPickr)){
                sMsg = MT_GetName(miPCK_YPickr) + String(" 모터가 Place 포지션이 아닙니다.") ;
                bRet = false ;
            }

            if(STG.GetMotrPos(miSTG_ZStage , piSTG_ZStagCheck) <= MT_GetTrgPos(miSTG_ZStage)){
                sMsg = MT_GetName(miSTG_ZStage) + String(" 모터가 Check포지션이라 너무 높아 충돌 위험이 있습니다.") ;
                bRet = false ;
            }
        }









    }
    else if(_iMotr == miPCK_YPickr){
        if(MT_GetCmdPos(miPCK_ZPickr) > PM.GetValue(miPCK_ZPickr , pvPCK_ZPickrMove)+1.0){ //Z축 이동포지션에서 1mm 마진주고 그거보다 낮으면
            sMsg = MT_GetName(miPCK_ZPickr) + String(" 모터의 높이가 이동포지션 보다 낮습니다.") ;
            bRet = false ;
        }
        else if(MT_GetTrgPos(miDSP_YDispr) + GetMotrPos(_iMotr,_iPstnId) > OM.MstOptn.dPckDspStroke ||
            MT_GetCmdPos(miDSP_YDispr) + GetMotrPos(_iMotr,_iPstnId) > OM.MstOptn.dPckDspStroke ){
            sMsg = MT_GetName(miDSP_YDispr) + String(" 모터가 충돌 위치에 있습니다.") ;
            bRet = false ;
        }
    }
    else if(_iMotr == miPCK_XAlign){

    }
    else if(_iMotr == miPCK_YAlign){

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
bool CPicker::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiPCK_XAlgnFwBw){
        if(_bFwd == ccFwd) {
            //if(!bExistSply /*|| !bSRT_ZTop*/) {sMsg = AnsiString("Tray 센서 감지중!"); bRet = false ;}
        }
    }
    else if(_iActr == aiPCK_YAlgnFwBw){
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

bool CPicker::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
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







void CPicker::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CPicker::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CPicker::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(miPCK_ZPickr);
    return ;

}

double CPicker::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return dLastIdxPos ;
}

bool CPicker::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miPCK_ZPickr)) return true  ; //이파트는 필요 없다.
    else                                            return false ;

}

bool CPicker::CheckStop()
{
    if(!MT_GetStop(miPCK_XAlign)) return false ;
    if(!MT_GetStop(miPCK_YAlign)) return false ;
    if(!MT_GetStop(miPCK_YPickr)) return false ;
    if(!MT_GetStop(miPCK_ZPickr)) return false ;

    if(!AT_Done(aiPCK_XAlgnFwBw)) return false ;
    if(!AT_Done(aiPCK_YAlgnFwBw)) return false ;

    return true ;
}

void CPicker::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos     " , m_dLastIdxPos      );
        UserINI.Load(sPath, "Member" , "m_dPkPkgThickness " , m_dPkPkgThickness  );
        UserINI.Load(sPath, "Member" , "m_dEmptyToolHeight" , m_dEmptyToolHeight );
        UserINI.Load(sPath, "Member" , "m_bNeedLotEnd     " , m_bNeedLotEnd      );
        UserINI.Load(sPath, "Member" , "m_bVisn           " , m_bVisn            );
        UserINI.Load(sPath, "Member" , "m_dVisnX          " , m_dVisnX           );
        UserINI.Load(sPath, "Member" , "m_dVisnY          " , m_dVisnY           );
        UserINI.Load(sPath, "Member" , "m_dVisnT          " , m_dVisnT           );
        UserINI.Load(sPath, "Member" , "m_iVisnDefCnt     " , m_iVisnDefCnt      );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos     " , m_dLastIdxPos     );
        UserINI.Save(sPath, "Member" , "m_dPkPkgThickness " , m_dPkPkgThickness );
        UserINI.Save(sPath, "Member" , "m_dEmptyToolHeight" , m_dEmptyToolHeight);
        UserINI.Save(sPath, "Member" , "m_bNeedLotEnd     " , m_bNeedLotEnd     );
        UserINI.Save(sPath, "Member" , "m_bVisn           " , m_bVisn           );
        UserINI.Save(sPath, "Member" , "m_dVisnX          " , m_dVisnX          );
        UserINI.Save(sPath, "Member" , "m_dVisnY          " , m_dVisnY          );
        UserINI.Save(sPath, "Member" , "m_dVisnT          " , m_dVisnT          );
        UserINI.Save(sPath, "Member" , "m_iVisnDefCnt     " , m_iVisnDefCnt     );
    }


}




                                                     
