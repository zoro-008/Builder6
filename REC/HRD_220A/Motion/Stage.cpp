//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Stage.h"
#include "Index.h"
#include "Picker.h"
#include "Dispensor.h"
//#include "RunThread.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "ManualMan.h"
//#include "LotUnit.h"
#include "UserIni.h"
#include "UserFile.h"
#include "Pattern.h"
#include "SPCUnit.h"





//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CStage STG;

CStage::CStage(void)
{

}

CStage::~CStage (void)
{

}

void CStage::Init()
{
    m_sPartName = "Stage " ;

    Reset();
    Load(true);



    InitCycleName();
    InitCycleTime();

    m_sCheckSafeMsg = "" ;


    m_bVacErrDisp = false ;
    m_bNeedLotEnd = false ;

    WorkData.LoadFromCsv(EXE_FOLDER + "SeqData\\WorkData.csv");
}

void CStage::Close()
{
    Load(false);

    WorkData.SaveToCsv(EXE_FOLDER + "SeqData\\WorkData.csv");
}

void CStage::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}


//마지막 워크를 리턴한다.
//첫자제면 OM.DevInfo.iBoatColCnt 리턴.
bool CStage::FindChip(int &c , EN_ARAY_ID &id)
{
    return IDX_R.FindChip(c,id);


    //id = riIDX_R ;
    //     if(DM.ARAY[riIDX_R].GetCntStat(csVisn   )) {c = DM.ARAY[riIDX_R].FindLastCol(csVisn   ); return true  ;}
    //else if(DM.ARAY[riIDX_R].GetCntStat(csAttach )) {c = DM.ARAY[riIDX_R].FindLastCol(csAttach ); return true  ;}
    //else if(DM.ARAY[riIDX_R].GetCntStat(csDisp   )) {c = DM.ARAY[riIDX_R].FindLastCol(csDisp   ); return true  ;}
    //else if(DM.ARAY[riIDX_R].GetCntStat(csPush   )) {c = DM.ARAY[riIDX_R].FindLastCol(csPush   ); return true  ;}
    //else if(DM.ARAY[riIDX_R].GetCntStat(csEndVisn)) {c = DM.ARAY[riIDX_R].FindLastCol(csEndVisn); return true  ;}
    //else if(DM.ARAY[riIDX_R].GetCntStat(csWorkEnd)) {c = DM.ARAY[riIDX_R].FindLastCol(csWorkEnd); return true  ;}
    //else                                            {c = -1 ;                                     return false ;}

}

double CStage::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    double dWorkStt ; //작업해야할 메거진의 맨 아래 슬롯 위치.

    if(_iMotr == miSTG_XStage){
             if(_iPstnId ==  piSTG_XStagWait   ) dPos = PM.GetValue (_iMotr , pvSTG_XStagWait     );
        else if(_iPstnId ==  piSTG_XStagCenter ) dPos = PM.GetValue (_iMotr , pvSTG_XStagCenter   );
        else                                     dPos = MT_GetCmdPos(_iMotr                       );
    }
    else if(_iMotr == miSTG_YStage){
             if(_iPstnId ==  piSTG_YStagWait   ) dPos = PM.GetValue (_iMotr , pvSTG_YStagWait     );
        else if(_iPstnId ==  piSTG_YStagCenter ) dPos = PM.GetValue (_iMotr , pvSTG_YStagCenter   );
        else                                     dPos = MT_GetCmdPos(_iMotr                       );
    }
    else if(_iMotr == miSTG_TStage){
             if(_iPstnId ==  piSTG_TStagWait   ) dPos = PM.GetValue (_iMotr , pvSTG_TStagWait     );
        else if(_iPstnId ==  piSTG_TStagWork   ) dPos = PM.GetValue (_iMotr , pvSTG_TStagWork     );
        else                                     dPos = MT_GetCmdPos(_iMotr                       );
    }
    else if(_iMotr == miSTG_ZStage){
             if(_iPstnId ==  piSTG_ZStagWait   ) dPos = PM.GetValue (_iMotr , pvSTG_ZStagWait     );
        else if(_iPstnId ==  piSTG_ZStagPick   ) dPos = PM.GetValue (_iMotr , pvSTG_ZStagPick     );
        else if(_iPstnId ==  piSTG_ZStagCheck  ) dPos = PM.GetValue (_iMotr , pvSTG_ZStagCheck    );
        else if(_iPstnId ==  piSTG_ZStagWork   ) dPos = PM.GetValue (_iMotr , pvSTG_ZStagCheck    ) - m_dPkPkgThickness ;
        else                                     dPos = MT_GetCmdPos(_iMotr                       );
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CStage::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

bool CStage::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CStage::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;   
    //Ok.
    return true;
}

bool CStage::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: if(!DM.ARAY[riSTG].CheckAllStat(csNone)){
                     Step.iToStart = 0 ;
                     return true ;
                 }
                 IO_SetY(ySTG_Ejct , false);
                 IO_SetY(ySTG_Vccm , true );
                 m_tmDelay.Clear();
                 Step.iToStart++;
                 return false;

        case 11: if(!m_tmDelay.OnDelay(true , 300)) return false;
                 if(!IO_GetX(xSTG_Vccm)){
                     STG.MoveMotr(miSTG_ZStage , piSTG_ZStagWait);
                 }
                 Step.iToStart++;
                 return false;

        case 12: if(!IO_GetX(xSTG_Vccm) && !STG.MoveMotr(miSTG_ZStage , piSTG_ZStagWait)) return false;
                 IO_SetY(ySTG_Vccm , false);
                 MoveMotr(miSTG_XStage,piSTG_XStagCenter);
                 MoveMotr(miSTG_YStage,piSTG_YStagCenter);
                 MoveMotr(miSTG_TStage,piSTG_TStagWork  );
                 Step.iToStart ++ ;
                 return false ;

        case 13: if(!MoveMotr(miSTG_XStage,piSTG_XStagCenter))return false ;
                 if(!MoveMotr(miSTG_YStage,piSTG_YStagCenter))return false ;
                 if(!MoveMotr(miSTG_TStage,piSTG_TStagWork  ))return false ;


                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CStage::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: Step.iToStop++;
                 return false ;

        case 11:

                 Step.iToStop ++ ;
                 return false ;

        case 12: 
                 Step.iToStop = 0 ;
                 return true ;
    }
}

bool CStage::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        //
        //int  iWorkingCol ;
        //EN_ARAY_ID riWork ;
        //FindChip(iWorkingCol ,riWork);
        //bool isWorking = iWorkingCol >= 0 ;


        //일단 픽커만 해보자....
        //랏엔드 상황에 뻘줌하게 들고 있는 경우가 있다.
        //DSP.GetSeqStep()==CDispensor::scIdle이것은 나중에보니 비젼검사해야 하는데 같이 스테이지가
        //스킵되서 뻘짓함.
        if(PCK.GetWorkEnd()&&DM.ARAY[riSTG].GetStat(0,0)!=csNone&&DSP.GetSeqStep()==CDispensor::scIdle){
            DM.ARAY[riSTG].SetStat(csSkip);
            m_bNeedLotEnd = true ;
        }

        //첫자제 옵션은 다 넣었다.
        //시퀜스만 넣으면 된다.




        bool isIdxWorkPos = IDX_R.IsWorkPos() ;

        //FOS 순서                                    //DIE순서
        //csVisn    얼라인검사                        //csVisn    얼라인 검사 해본다.

        //차이점.
        //csAttach  픽커 칩 붙인다.                   //csDisp    파란판에 패턴 디스펜싱 한다.
        //csDisp    디스펜싱 하고.                    //csAttach  칩을 붙인다.

        //csPush    일정 시간 꾹 누르고 픽커 빠진다.  //csPush
        //csEndVisn 작업 완료 비젼 확인.              //csEndVisn 작업 완료 비젼 확인.
        //csWorkEnd 작업 종료 하고 스테이지 내린다.   //csWorkEnd 작업 종료 하고 스테이지 내린다.
        //bool bFrontWorkEnd = PCK.GetWorkEnd();

        bool isCycleUp   =  DM.ARAY[riIDX_R].GetCntStat(csUnkwn )&&
                            isIdxWorkPos                         &&
                            DM.ARAY[riSTG].CheckAllStat(csNone)  &&
                            IDX_R.GetSeqStep() == CIndex::scIdle ; //MT_GetCmdPos(miSTG_ZStagR) == PM.GetValue(miSTG_ZStagR , pvSTG_ZStagWait);
        bool isCycleDown =  DM.ARAY[riSTG].GetStat(0,0) == csWorkEnd || DM.ARAY[riSTG].GetStat(0,0) == csSkip || DM.ARAY[riSTG].GetStat(0,0) == csVisnNG;

        bool isCycleEnd  =  IDX_R.GetStat().bWorkEnd && DM.ARAY[riIDX_R].CheckAllStat(csNone) && DM.ARAY[riSTG].CheckAllStat(csNone) ;

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleUp    ) {Trace(m_sPartName.c_str(),"CycleUp   Stt"); Step.iSeq = scUp   ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleDown  ) {Trace(m_sPartName.c_str(),"CycleDown Stt"); Step.iSeq = scDown ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                   Trace(m_sPartName.c_str(),"default     End");                                                   Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                         return false ;
        case scUp    : if(CycleUp   ()){ Trace(m_sPartName.c_str(),"CycleUp     End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
        case scDown  : if(CycleDown ()){ Trace(m_sPartName.c_str(),"CycleDown   End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

double CStage::GetPkgThickness()
{
    return m_dPkPkgThickness;
}

void CStage::SetPkgThickness(double _dThickness)
{
    m_dPkPkgThickness = _dThickness ;
}


double CStage::GetFinishThickness()
{
    return m_dFinishThickness;
}
void CStage::SetFinishThickness(double _dThickness)
{
    m_dFinishThickness = _dThickness;
}

double CStage::GetEpoxyThickness()
{
    return m_dEpoxyThickness;
}
void CStage::SetEpoxyThickness(double _dThickness)
{
    int iThickness = _dThickness * 10000 ;
    m_dEpoxyThickness = iThickness / 10000.0;
}

double CStage::GetTickTime()
{
    return m_dTickTime;
}

void CStage::SetTickTime(double _dTickTime)
{
    m_dTickTime = _dTickTime;
}

bool CStage::CycleHome()     //sun DLL direct access.
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

    static EN_CHIP_STAT csSTGStat ;




    switch (Step.iHome) {

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: IO_SetY(ySTG_Vccm,true);
                 m_tmDelay.Clear();
                 Step.iHome++;
                 return false ;

        case 11: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                 //작업 하다가 홈잡을때...
                 //자제배큠빨아보니 있고 장비껐다가 켠상태가 아닐때.
                 //이거 디스펜싱 하고 그럴때 정지 하고 홈잡으면 대책 없음...
                 //오퍼레이션에서 일단 막자..

                 //이거 오퍼레이션으로빼야 되나???? =>> 아니다.. 메뉴얼에 에러 걸렸으면 메뉴얼 구동 초기화됌.
                 //if(IO_GetX(xSTG_Vccm)){
                 //    EM_SetErrMsg(eiSTG_RemovePkg , "작업 Stage에 자제를 제거하여 주십시오");
                 //
                 //    Step.iHome = 0 ;
                 //    return true ;
                 //}



                 //if(!DM.ARAY[riSTG].CheckAllStat(csNone)) {
                 //    EM_SetErrMsg(eiSTG_RemovePkg , "작업 Stage 화면에 데이터를 제거하여 주십시오");
                 //    Step.iHome = 0 ;
                 //    return true ;
                 //}
                 //if(IO_GetX(xSTG_Vccm) && MT_GetHomeEnd(miSTG_XStage)&&
                 //                         MT_GetHomeEnd(miSTG_YStage)&&
                 //                         MT_GetHomeEnd(miSTG_TStage)){
                 //csSTGStat = DM.ARAY[riSTG].GetStat(0,0);
                 //
                 //if(IO_GetX(xSTG_Vccm)){
                 //    if(MT_GetHomeEnd(miSTG_XStage)&& MT_GetHomeEnd(miSTG_YStage)&& MT_GetHomeEnd(miSTG_TStage)){
                 //        if(csSTGStat == csVisn   )
                 //        if(csSTGStat == csDisp   )
                 //        if(csSTGStat == csAttach )
                 //        if(csSTGStat == csPush   )
                 //        if(csSTGStat == csVisnAt )
                 //        if(csSTGStat == csWorkEnd)
                 //        if(csSTGStat == csSkip   )
                 //
                 //        MT_GoAbsMan(miSTG_XStage , PM.GetValue(miSTG_XStage , pvSTG_XStagCenter));
                 //        MT_GoAbsMan(miSTG_YStage , PM.GetValue(miSTG_YStage , pvSTG_YStagCenter));
                 //        MT_GoAbsMan(miSTG_TStage , PM.GetValue(miSTG_TStage , pvSTG_TStagWork  ));
                 //
                 //        Step.iHome++ ;
                 //        return false ;
                 //    }
                 //}

                 Step.iHome=13 ;
                 return false ;

        case 12: if(!MT_GoAbsMan(miSTG_XStage , PM.GetValue(miSTG_XStage , pvSTG_XStagCenter))) return false ;
                 if(!MT_GoAbsMan(miSTG_YStage , PM.GetValue(miSTG_YStage , pvSTG_YStagCenter))) return false ;
                 if(!MT_GoAbsMan(miSTG_TStage , PM.GetValue(miSTG_TStage , pvSTG_TStagWork  ))) return false ;


                 Step.iHome++;
                 return false ;

        case 13: IO_SetY(ySTG_Vccm,false);
                 MT_DoHome(miSTG_ZStage);
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miSTG_ZStage)) return false ;
                 MT_DoHome(miSTG_XStage);
                 MT_DoHome(miSTG_YStage);
                 MT_DoHome(miSTG_TStage);
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GetHomeEnd(miSTG_XStage)) return false ;
                 if(!MT_GetHomeEnd(miSTG_YStage)) return false ;
                 if(!MT_GetHomeEnd(miSTG_TStage)) return false ;

                 MT_GoAbsMan(miSTG_XStage , PM.GetValue(miSTG_XStage , pvSTG_XStagWait));
                 MT_GoAbsMan(miSTG_YStage , PM.GetValue(miSTG_YStage , pvSTG_YStagWait));
                 MT_GoAbsMan(miSTG_TStage , PM.GetValue(miSTG_TStage , pvSTG_TStagWait));
                 MT_GoAbsMan(miSTG_ZStage , PM.GetValue(miSTG_ZStage , pvSTG_ZStagWait));
                 //if(!DM.ARAY[riSTG].CheckAllStat(csNone)) {
                 //    EM_SetErrMsg(eiSTG_RemovePkg , "작업 Stage 확인 및 화면에 데이터를 제거하여 주십시오");
                 //}
                 if(DM.ARAY[riSTG].GetStat(0,0)!=csNone) {

                     int  iWorkingCol ;
                     EN_ARAY_ID riWork ;
                     if(FindChip(iWorkingCol ,riWork) >= 0) { //findchip은 마지막 csWork 리턴.
                         csSTGStat = DM.ARAY[riSTG].GetStat(0,0) ;
                              if(csSTGStat ==csDisp    ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csUnkwn   );
                         else if(csSTGStat ==csVisn    ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csUnkwn   );
                         else if(csSTGStat ==csWorkEnd ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csWork    );
                         else if(csSTGStat ==csVisnNG  ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csVisnNG  );
                         else if(csSTGStat ==csSkip    ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );

                         if(OM.DevInfo.iWorkMode==wmDie) {
                                  if(csSTGStat ==csAttach) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );
                             else if(csSTGStat ==csPush  ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );
                             else if(csSTGStat ==csVisnAt) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );
                         }
                         else {
                                  if(csSTGStat ==csAttach) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csUnkwn   );
                             else if(csSTGStat ==csPush  ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );
                             else if(csSTGStat ==csVisnAt) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );
                         }
                     }
                     DM.ARAY[riSTG].SetStat(csNone);
                          if(csSTGStat ==csDisp    ) {}
                     else if(csSTGStat ==csVisn    ) {}
                     else if(csSTGStat ==csAttach  ) {if(OM.DevInfo.iWorkMode==wmDie)FM_MsgOk("Confirm","리어인덱스 워크존에 있는 자제를 점검해주세요!");
                                                      else                           FM_MsgOk("Confirm","리어인덱스 워크존에 있는 자제의 안착을 확인해주세요!"); }
                     else if(csSTGStat ==csPush    ) {FM_MsgOk("Confirm","리어인덱스 워크존에 있는 자제를 점검해주세요!");}
                     else if(csSTGStat ==csVisnAt  ) {FM_MsgOk("Confirm","리어인덱스 워크존에 있는 자제를 점검해주세요!");}
                     else if(csSTGStat ==csWorkEnd ) {FM_MsgOk("Confirm","리어인덱스 워크존에 있는 자제의 안착을 확인해주세요!");}
                     else if(csSTGStat ==csSkip    ) {}
                 }







                 Step.iHome = 0;
                 return true ;





        case 20: MT_GoAbsMan(miSTG_ZStage , PM.GetValue(miSTG_ZStage , pvSTG_ZStagPick));
                 Step.iHome++;
                 return false;
        
        case 21: if(!MT_GoAbsMan(miSTG_ZStage , PM.GetValue(miSTG_ZStage , pvSTG_ZStagPick))) return false ;
                 IO_SetY(ySTG_Ejct , false);
                 IO_SetY(ySTG_Vccm , false);
                 Step.iCycle++;
                 return false;
        
        case 22: MT_GoAbsMan(miSTG_ZStage , PM.GetValue(miSTG_ZStage , pvSTG_ZStagWait));
                 Step.iCycle++;
                 return false;
        
        case 23: if(!MT_GoAbsMan(miSTG_ZStage , PM.GetValue(miSTG_ZStage , pvSTG_ZStagWait))) return false;
                 DM.ARAY[riSTG].SetStat(csNone);
                 Step.iCycle= 0;
                 return true;


    }
}

//One Cycle.
bool CStage::CycleUp()
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


        case 10:if(OM.MstOptn.bSkipPickrDispr) {
                     m_tmDelay.Clear();
                     Step.iCycle ++;
                     return false;
                 }
                 Spc.Clear();
                 Spc.dWorkSttTime = Now().Val ;
                 m_tmTick.Clear();


                 m_bNeedLotEnd = false ;

                 Step.iCycle = 12;
                 return false;

        case 11: if(!m_tmDelay.OnDelay(true , 1000)) return false;
                 int  iWorkingCol ;
                 EN_ARAY_ID riWork ;
                 if(FindChip(iWorkingCol ,riWork) >= 0) { //findchip은 마지막 csWork 리턴.
                     DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csWork);
                 }
                 Step.iCycle = 0;
                 return true;

        case 12: if(!IDX_R.IsWorkPos()){ //메뉴얼때문에...
                     EM_SetErrMsg(eiPRT_Crash , "리어 인덱스가 작업 포지션이 아닙니다.");
                     Step.iCycle = 0 ;
                     return true;
                 }
                 MoveMotr(miSTG_XStage , piSTG_XStagCenter);
                 MoveMotr(miSTG_YStage , piSTG_YStagCenter);
                 MoveMotr(miSTG_TStage , piSTG_TStagWork  );
                 IO_SetY(ySTG_Ejct , false);
                 IO_SetY(ySTG_Vccm , false);
                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveMotr(miSTG_XStage , piSTG_XStagCenter)) return false ;
                 if(!MoveMotr(miSTG_YStage , piSTG_YStagCenter)) return false ;
                 if(!MoveMotr(miSTG_TStage , piSTG_TStagWork  )) return false ;

                 MoveMotr(miSTG_ZStage , piSTG_ZStagPick);
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(miSTG_ZStage , piSTG_ZStagPick)) return false ;
                 IO_SetY(ySTG_Vccm , true);
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 15: if(IO_GetX(xSTG_Vccm)){
                     MoveMotr(miSTG_ZStage , piSTG_ZStagCheck);
                     Step.iCycle=20;
                     return false ;
                 }


                 if(!m_tmDelay.OnDelay(true , 1000)) return false ;

                 //if(OM.CmnOptn.bVisnNgStop) {
                     EM_SetErrMsg(eiSTG_PickMiss , "스테이지에 보트나 자제의 안착 상태를 확인하세요");
                     if(MM.GetManNo() == mcNone)  m_bVacErrDisp = true;
                 //}
                 //else {
                 //    MoveMotr(miPCK_ZPickr , piPCK_ZPickrMove);
                 //    int iLastWorkedCol ;
                 //    EN_ARAY_ID riWork2 ;
                 //    FindChip(iLastWorkedCol ,riWork2);
                 //    EN_CHIP_STAT csStat = DM.ARAY[riIDX_R].GetStat(0,iLastWorkedCol-1);
                 //    DM.ARAY[riIDX_R].SetStat(0,iLastWorkedCol-1,csSkip);
                 //}

                 MoveMotr(miSTG_ZStage , piSTG_ZStagWait);
                 Step.iCycle++;
                 return false ;

        case 16: if(!MoveMotr(miSTG_ZStage , piSTG_ZStagWait)) return false ;
                 Step.iCycle=0;
                 return true ;

        case 20: if(!MoveMotr(miSTG_ZStage , piSTG_ZStagCheck)) return false ;

                 EN_ARAY_ID riWork2 ;
                 FindChip(iWorkingCol ,riWork2);
                 EN_CHIP_STAT csStat = DM.ARAY[riIDX_R].GetStat(0,iWorkingCol);
                 if(csStat == csUnkwn ) {
                     DM.ARAY[riSTG].SetStat(csVisn  );
                     String sChipID = DM.ARAY[riIDX_R].CHPS[0][iWorkingCol].GetID() ;
                     DM.ARAY[riSTG].CHPS[0][0].SetID(sChipID);

                 }
                 else if(csStat == csVisnAt) DM.ARAY[riSTG].SetStat(csVisnAt);
                 DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csWait);
                 Step.iCycle = 0;
                 return true ;
    }




}

bool CStage::CycleDown(void)
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


        case 10: MoveMotr(miSTG_XStage , piSTG_XStagCenter);
                 MoveMotr(miSTG_YStage , piSTG_YStagCenter);
                 MoveMotr(miSTG_TStage , piSTG_TStagWork  );
                 Step.iCycle++;
                 return false ;

        case 11: if(!MoveMotr(miSTG_XStage , piSTG_XStagCenter)) return false ;
                 if(!MoveMotr(miSTG_YStage , piSTG_YStagCenter)) return false ;
                 if(!MoveMotr(miSTG_TStage , piSTG_TStagWork  )) return false ;

                 MoveMotr(miSTG_ZStage , piSTG_ZStagPick);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveMotr(miSTG_ZStage , piSTG_ZStagPick)) return false ;
                 if(MM.GetManNo() == mcNoneCycle){
                     if(!IO_GetX(xSTG_Vccm)){
                         EM_SetErrMsg(eiSTG_PickMiss , "스테이지에 보트나 자제의 안착 상태를 확인하세요");
                     }
                 }
                 IO_SetY(ySTG_Vccm , false);
                 IO_SetY(ySTG_Ejct , true );
                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 13: if(!m_tmDelay.OnDelay(true , 200)) return false ;
                 IO_SetY(ySTG_Ejct , false);
                 MoveMotr(miSTG_ZStage , piSTG_ZStagWait);
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveMotr(miSTG_ZStage , piSTG_ZStagWait)) return false ;

                 SetTickTime(m_tmTick.OnCheckCycle());
                 //메뉴얼로 내리는 경우가 골치...
                 if(DM.ARAY[riSTG].CheckAllStat(csWorkEnd)||
                    DM.ARAY[riSTG].CheckAllStat(csVisnNG )||
                    DM.ARAY[riSTG].CheckAllStat(csVisnAt )){//비젼검사 하지 않았지만 일단 붙어 있는 상태므로 남긴다.
                     WriteSpc();
                 }



                 EN_CHIP_STAT csSTGStat ;
                 csSTGStat = DM.ARAY[riSTG].GetStat(0,0) ;
                 if(csSTGStat!=csNone) {
                     int  iWorkingCol ;
                     EN_ARAY_ID riWork ;
                     if(FindChip(iWorkingCol ,riWork) >= 0) { //findchip은 마지막 csWork 리턴.
                              if(csSTGStat ==csDisp    ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csUnkwn   );
                         else if(csSTGStat ==csVisn    ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csUnkwn   );
                         else if(csSTGStat ==csWorkEnd ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csWork    );
                         else if(csSTGStat ==csSkip    ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );
                         else if(csSTGStat ==csVisnNG  ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csVisnNG  );

                         if(OM.DevInfo.iWorkMode==wmDie) {
                                  if(csSTGStat ==csAttach) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );
                             else if(csSTGStat ==csPush  ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );
                             else if(csSTGStat ==csVisnAt) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );
                         }
                         else {
                                  if(csSTGStat ==csAttach) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csUnkwn   );
                             else if(csSTGStat ==csPush  ) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );
                             else if(csSTGStat ==csVisnAt) DM.ARAY[riIDX_R].SetStat(0,iWorkingCol,csSkip    );
                         }
                     }
                     DM.ARAY[riSTG].SetStat(csNone);
                 }

                 if(m_bNeedLotEnd){
                     DM.ARAY[riIDX_R].ChangeStat(csUnkwn,csSkip);
                     if(!DM.ARAY[riPRI_R].CheckAllStat(csNone))DM.ARAY[riPRI_R].ChangeStat(csUnkwn,csSkip);
                     m_bNeedLotEnd = false ;
                 }

                 DM.ARAY[riSTG  ].SetStat( csNone);

                 Step.iCycle = 0;
                 return true ;
    }

}

void CStage::WriteSpc()
{
    int  iWorkingCol ;
    EN_ARAY_ID riWork ;

    FindChip(iWorkingCol ,riWork);


    TDateTime SttTime ;
    SttTime.Val = Spc.dWorkSttTime ;
    String sFilePath = WRK_FOLDER+SttTime.FormatString("yyyy_mm_dd")+".csv";
    if (!DirectoryExists(WRK_FOLDER)) {
        ForceDirectories(WRK_FOLDER);
    }

    if(!FileExists(sFilePath)){
        STG.WorkData.SetMaxColRow(30,1);
        STG.WorkData.SetCell( 0 ,0 ,"PkgNo"             );
        STG.WorkData.SetCell( 1 ,0 ,"FrontBoatNo"       );
        STG.WorkData.SetCell( 2 ,0 ,"FrontSlotNo"       );
        STG.WorkData.SetCell( 3 ,0 ,"FrontSerialNo"     );
        STG.WorkData.SetCell( 4 ,0 ,"RearBoatNo"        );
        STG.WorkData.SetCell( 5 ,0 ,"RearSlotNo"        );
        STG.WorkData.SetCell( 6 ,0 ,"RearSerialNo"      );
        STG.WorkData.SetCell( 7 ,0 ,"Recipe"            );
        STG.WorkData.SetCell( 8 ,0 ,"Pressure(kPa)"     );
        STG.WorkData.SetCell( 9 ,0 ,"Vaccum(kPa)"       );
        STG.WorkData.SetCell(10 ,0 ,"NodeCount"         );
        STG.WorkData.SetCell(11 ,0 ,"SetWeight(g)"      );
        STG.WorkData.SetCell(12 ,0 ,"PushTime(sec)"     );
        STG.WorkData.SetCell(13 ,0 ,"SetStageTemp(˚)"  );
        STG.WorkData.SetCell(14 ,0 ,"RealStageTemp(˚)" );
        STG.WorkData.SetCell(15 ,0 ,"PushWeight(g)"     );
        STG.WorkData.SetCell(16 ,0 ,"PickerAlignX(mm)"  );
        STG.WorkData.SetCell(17 ,0 ,"PickerAlignY(mm)"  );
        STG.WorkData.SetCell(18 ,0 ,"PickerAlignT(˚)"  );
        STG.WorkData.SetCell(19 ,0 ,"StageAlignX(mm)"   );
        STG.WorkData.SetCell(20 ,0 ,"StageAlignY(mm)"   );
        STG.WorkData.SetCell(21 ,0 ,"StageAlignT(˚)"   );
        STG.WorkData.SetCell(22 ,0 ,"AttachAlignX(mm)"  );
        STG.WorkData.SetCell(23 ,0 ,"AttachAlignY(mm)"  );
        STG.WorkData.SetCell(24 ,0 ,"AttachAlignT(˚)"  );
        STG.WorkData.SetCell(25 ,0 ,"EpoxyThickness(mm)");
        STG.WorkData.SetCell(26 ,0 ,"DefectCount"       );
        STG.WorkData.SetCell(27 ,0 ,"WorkStartTime"     );
        STG.WorkData.SetCell(28 ,0 ,"WorkEndTime"       );
        STG.WorkData.SetCell(29 ,0 ,"WorkTime"          );

        WorkData.SaveToCsv(sFilePath);
    }

    int iDataRowCnt = WorkData.GetMaxRow();
    WorkData.SetMaxColRow(30,iDataRowCnt+1);
    WorkData.SetCell( 0 ,iDataRowCnt ,iDataRowCnt                      );//"PkgNo"         );
    WorkData.SetCell( 1 ,iDataRowCnt ,Spc.sWorkFrntBoatId              );//"FrontBoatNo"   );
    WorkData.SetCell( 2 ,iDataRowCnt ,Spc.iWorkFrntBoatSlot            );//"FrontSlotNo"   );
    WorkData.SetCell( 3 ,iDataRowCnt ,Spc.sWorkFrntChipId              );//"FrontSerialNo" );
    WorkData.SetCell( 4 ,iDataRowCnt ,DM.ARAY[riIDX_R].GetID()         );//"RearBoatNo"    );


    WorkData.SetCell( 5 ,iDataRowCnt ,OM.DevInfo.iBoatColCnt - iWorkingCol) ;//"RearSlotNo"    );
    WorkData.SetCell( 6 ,iDataRowCnt ,DM.ARAY[riSTG].CHPS[0][0].GetID());//"RearSerialNo"  );
    WorkData.SetCell( 7 ,iDataRowCnt ,OM.GetCrntDev()                  );//"Recipe"        );
    WorkData.SetCell( 8 ,iDataRowCnt ,OM.DevOptn.dDspPrsPres           );//"Pressure"      );
    WorkData.SetCell( 9 ,iDataRowCnt ,OM.DevOptn.dDspVacPres           );//"Vaccum"        );
    WorkData.SetCell(10 ,iDataRowCnt ,PTT.GetPosCnt()                  );//"NodeCount"     );
    WorkData.SetCell(11 ,iDataRowCnt ,OM.DevOptn.dAttachForce          );//
    WorkData.SetCell(12 ,iDataRowCnt ,OM.DevOptn.iPkPushDelay/1000.0   );//"PushTime"      );
    WorkData.SetCell(13 ,iDataRowCnt ,OM.DevOptn.iStageTemp            );//"WorkTime"      );
    WorkData.SetCell(14 ,iDataRowCnt ,Spc.dCrntStgTemp                 );//"WorkTime"      );
    WorkData.SetCell(15 ,iDataRowCnt ,Spc.dPushWeight                  );//"PushWeight"    );

    double dVisnRsltX , dVisnRsltY , dVisnRsltT ;
    PCK.GetVisnRslt(&dVisnRsltX , &dVisnRsltY , &dVisnRsltT);
    WorkData.SetCell(16 ,iDataRowCnt ,dVisnRsltX                       );//"PickerAlignX"  );
    WorkData.SetCell(17 ,iDataRowCnt ,dVisnRsltY                       );//"PickerAlignY"  );
    WorkData.SetCell(18 ,iDataRowCnt ,dVisnRsltT                       );//"PickerAlignT"  );

    DSP.GetVisnRslt(&dVisnRsltX , &dVisnRsltY , &dVisnRsltT , false);
    WorkData.SetCell(19 ,iDataRowCnt ,dVisnRsltX                       );//"StageAlignX"   );
    WorkData.SetCell(20 ,iDataRowCnt ,dVisnRsltY                       );//"StageAlignY"   );
    WorkData.SetCell(21 ,iDataRowCnt ,dVisnRsltT                       );//"StageAlignT"   );

    DSP.GetVisnRslt(&dVisnRsltX , &dVisnRsltY , &dVisnRsltT , true);
    WorkData.SetCell(22 ,iDataRowCnt ,dVisnRsltX                       );//"AttachAlignX"  );
    WorkData.SetCell(23 ,iDataRowCnt ,dVisnRsltY                       );//"AttachAlignY"  );
    WorkData.SetCell(24 ,iDataRowCnt ,dVisnRsltT                       );//"AttachAlignT"  );
    WorkData.SetCell(25 ,iDataRowCnt ,GetEpoxyThickness()              );//"EpoxyThickness");
    WorkData.SetCell(26 ,iDataRowCnt ,PCK.GetVisnDefectCnt());//"WorkTime"      );



    WorkData.SetCell(27 ,iDataRowCnt ,SttTime.FormatString("hh:mm:ss"));//"WorkStartTime" );

    TDateTime EndTime ;
    EndTime = Now();
    WorkData.SetCell(28 ,iDataRowCnt ,EndTime.FormatString("hh:mm:ss"));//"WorkEndTime"   );

    TDateTime WrkTime = EndTime - SttTime ;
    WorkData.SetCell(29 ,iDataRowCnt ,WrkTime.FormatString("hh:mm:ss"));//"WorkTime"      );




    //기존에 있던것들 지우기.
    UserFile.ClearDirDate(WRK_FOLDER , Now() - 30 * 12); //1년
    WorkData.SaveToCsv(sFilePath);
}
//---------------------------------------------------------------------------
bool CStage::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    if(!MT_GetStop(miIDX_XIndxR)){
        sMsg = "Motor " + AnsiString(MT_GetName(miIDX_XIndxR)) + " is Moving." ;
        bRet = false ;
    }

    if(_iMotr == miSTG_XStage){

    }
    else if(_iMotr == miSTG_YStage){

    }
    else if(_iMotr == miSTG_TStage){

    }
    else if(_iMotr == miSTG_ZStage){

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
bool CStage::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
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

bool CStage::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , bool _bSlow )
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







void CStage::MoveIncMotr(EN_MOTR_ID _iMotr , double iPstn) // 모터를 움직일때 쓰는 함수.
{
    if(Step.iCycle) MT_GoIncRun(_iMotr , iPstn);
    else            MT_GoIncMan(_iMotr , iPstn);
}

bool CStage::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

void CStage::SetLastCmd()
{
    m_dLastIdxPos = MT_GetCmdPos(miSTG_ZStage);
    return ;

}

double CStage::GetLastCmd(EN_MOTR_ID _iMotr)
{
    double dLastIdxPos = 0.0 ;
    if(!MT_GetAlarm(_iMotr) && !MT_GetNLimSnsr(_iMotr) && !MT_GetPLimSnsr(_iMotr) ) dLastIdxPos = MT_GetCmdPos(_iMotr) ;
    else                                                                            dLastIdxPos = GetMotrPos  (_iMotr , (EN_PSTN_ID)0) ;

    return dLastIdxPos ;
}

bool CStage::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miSTG_ZStage)) return true  ; //이파트는 필요 없다.
    else                                            return false ;

}

bool CStage::CheckStop()
{
    if(!MT_GetStop(miSTG_XStage)) return false ;
    if(!MT_GetStop(miSTG_YStage)) return false ;
    if(!MT_GetStop(miSTG_TStage)) return false ;
    if(!MT_GetStop(miSTG_ZStage)) return false ;


    //if(!AT_Done(aiIDX_IndxUpDn)) return false ;

    return true ;
}

void CStage::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos     " , m_dLastIdxPos     );
        UserINI.Load(sPath, "Member" , "m_dPkPkgThickness " , m_dPkPkgThickness );
        UserINI.Load(sPath, "Member" , "m_dStageHeight    " , m_dStageHeight    );
        UserINI.Load(sPath, "Member" , "m_dFinishThickness" , m_dFinishThickness);
        UserINI.Load(sPath, "Member" , "m_dEpoxyThickness " , m_dEpoxyThickness );
        UserINI.Load(sPath, "Member" , "m_bNeedLotEnd     " , m_bNeedLotEnd     );

        UserINI.Load(sPath, "Spc"    , "dWorkSttTime      " , Spc.dWorkSttTime      );
        UserINI.Load(sPath, "Spc"    , "iWorkFrntBoatSlot " , Spc.iWorkFrntBoatSlot );
        UserINI.Load(sPath, "Spc"    , "sWorkFrntBoatId   " , Spc.sWorkFrntBoatId   );
        UserINI.Load(sPath, "Spc"    , "sWorkFrntChipId   " , Spc.sWorkFrntChipId   );
        UserINI.Load(sPath, "Spc"    , "dPushWeight       " , Spc.dPushWeight       );
        UserINI.Load(sPath, "Spc"    , "dCrntStgTemp      " , Spc.dCrntStgTemp       );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos     " , m_dLastIdxPos     );
        UserINI.Save(sPath, "Member" , "m_dPkPkgThickness " , m_dPkPkgThickness );
        UserINI.Save(sPath, "Member" , "m_dStageHeight    " , m_dStageHeight    );
        UserINI.Save(sPath, "Member" , "m_dFinishThickness" , m_dFinishThickness);
        UserINI.Save(sPath, "Member" , "m_dEpoxyThickness " , m_dEpoxyThickness );
        UserINI.Save(sPath, "Member" , "m_bNeedLotEnd     " , m_bNeedLotEnd     );
        //UserINI.Save(sPath, "Member" , "m_dWorkSttTime    " , m_dWorkSttTime    );

        UserINI.Save(sPath, "Spc"    , "dWorkSttTime      " , Spc.dWorkSttTime     );
        UserINI.Save(sPath, "Spc"    , "iWorkFrntBoatSlot " , Spc.iWorkFrntBoatSlot);
        UserINI.Save(sPath, "Spc"    , "sWorkFrntBoatId   " , Spc.sWorkFrntBoatId  );
        UserINI.Save(sPath, "Spc"    , "sWorkFrntChipId   " , Spc.sWorkFrntChipId  );
        UserINI.Save(sPath, "Spc"    , "dPushWeight       " , Spc.dPushWeight      );
        UserINI.Save(sPath, "Spc"    , "dCrntStgTemp      " , Spc.dCrntStgTemp     );

    }
}


