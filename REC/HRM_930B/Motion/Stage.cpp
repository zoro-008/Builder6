//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Stage.h"
//---------------------------------------------------------------------------
#include <math.h>
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "HexaPot.h"
#include "LeftTool.h"
#include "CadPosition.h"
//#include "LotUnit.h"
#include "UserIni.h"
#include "UserFile.h"
#include "SPCUnit.h"
#include "Rs232UVCuring.h"                              
#include "LotUnit.h"

#include "DispLogUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------

TAlignPosOffset g_tMidPosOfs ;

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

    m_bNeedStgHome = false ;

    //m_bVacErrDisp = false ;
    //m_bNeedLotEnd = false ;

    WorkData.LoadFromCsv(EXE_FOLDER + "SeqData\\WorkData.csv");

    //배큠솔들 메인솔.
    IO_SetY(ySTG_VcumSol , true);
}

void CStage::Close()
{
    Load(false);

    WorkData.SaveToCsv(EXE_FOLDER + "SeqData\\WorkData.csv");
}

void CStage::GetRotPntFromCntrPnt(double _dX     , double _dY     , double _dAng ,  //입력 x 왼쪽이-, 입력 y위쪽이- , 돌릴각도. , +가 반시계방향.
                                  double _dCntrX , double _dCntrY ,                //돌리는 기준점 x , y
                                  double *_pRetX , double *_pRetY )                //결과값 x,y
{

    double dRadT =  _dAng*3.14159265358979323846/180.0;
    double x = _dX - _dCntrX;
    double y = _dY - _dCntrY;

    *_pRetX = x*cos(dRadT) - y*sin(dRadT) ;
    *_pRetY = x*sin(dRadT) + y*cos(dRadT) ;

    *_pRetX += _dCntrX ;
    *_pRetY += _dCntrY ;


    /*오리지날
    *_pRetY = cos(dRadT)*(_dY-_dCntrY) - sin(dRadT)*(_dX-_dCntrX)+ _dCntrY;//+ dCY + dCY);
    *_pRetX = sin(dRadT)*(_dY-_dCntrY) + cos(dRadT)*(_dX-_dCntrX)+ _dCntrX;//+ dCX + dCX);
    */
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
bool CStage::FindChip(int &r , int &c , EN_CHIP_STAT _iChip)
{
    return DM.ARAY[riSTG].FindFrstRowCol(_iChip ,r,c);
}

double CStage::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    int r,c ;

    double dWorkX = PM.GetValue (_iMotr , pvSTG_XStageCntr) ;
    double dWorkY = PM.GetValue (_iMotr , pvSTG_YStageCntr) ;

    if(FindChip(r,c,csCmsRNeed)){
        dWorkX += CAD.CM[c][r].CM_Rect -> GetCntrPosX();
        dWorkY += CAD.CM[c][r].CM_Rect -> GetCntrPosY();
    }
    if(_iMotr == miSTG_XStage){
             if(_iPstnId ==  piSTG_XStageWait ) dPos = PM.GetValue (_iMotr , pvSTG_XStageWait    );
        else if(_iPstnId ==  piSTG_XStageCntr ) dPos = PM.GetValue (_iMotr , pvSTG_XStageCntr    );
        else if(_iPstnId ==  piSTG_XStageWork ) dPos = dWorkX ;                                    //STG_XStageWork없애고 그냥 쓰자.
        else                                    dPos = MT_GetCmdPos(_iMotr                       );
    }
    else if(_iMotr == miSTG_YStage){
             if(_iPstnId ==  piSTG_YStageWait ) dPos = PM.GetValue (_iMotr , pvSTG_YStageWait    );
        else if(_iPstnId ==  piSTG_YStageCntr ) dPos = PM.GetValue (_iMotr , pvSTG_YStageCntr    );
        else if(_iPstnId ==  piSTG_YStageWork ) dPos = dWorkY ;
        else                                    dPos = MT_GetCmdPos(_iMotr                       );
    }
    else if(_iMotr == miSTG_YConvs){
             if(_iPstnId ==  piSTG_YConvsWait ) dPos = PM.GetValue (_iMotr , pvSTG_YConvsWait    );
        else if(_iPstnId ==  piSTG_YConvsWork ) dPos = PM.GetValue (_iMotr , pvSTG_YConvsWork    );
        else                                    dPos = MT_GetCmdPos(_iMotr                       );
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

        case 10: Step.iToStart++;
                 return false;

        case 11: Step.iToStart++;
                 return false;

        case 12: Step.iToStart ++ ;
                 return false ;

        case 13: Step.iToStart = 0 ;
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

        case 10: if(DM.ARAY[riSTG].CheckAllStat(csNone) && !IO_GetX(xSTG_StgVcum1)&& !IO_GetX(xSTG_StgVcum2)&&!IO_GetX(xSTG_StgVcum3)){
                     HexaPot.MoveAbsRes(paX,HEX_X_WAIT);
                     HexaPot.MoveAbsRes(paY,HEX_Y_WAIT);
                     HexaPot.MoveAbsRes(paZ,HEX_Z_WAIT);
                     HexaPot.MoveAbsRes(paU,HEX_U_WAIT);
                     HexaPot.MoveAbsRes(paV,HEX_V_WAIT);
                     HexaPot.MoveAbsRes(paW,HEX_W_WAIT);
                     HexaPot.Move();

                 }

                 Step.iToStop++;
                 return false ;

        case 11: if(!HexaPot.GetStopInpos())return false ;

                 Step.iToStop ++ ;
                 return false ;

        case 12: SetLastCmd();
                 Step.iToStop = 0 ;
                 return true ;
    }
}

void CStage::Update()
{
    HexaPot.Update();
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

    static CDelayTimer tmFix ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool bNeedDown = false;
        if(DM.ARAY[riSTG].GetCntStat(csCmsFix)) {
            if(LTL.m_iLastUVSttTime + OM.DevOptn.iFixDelay < GetTime()){
                bNeedDown = true ;
            }
        }

        //if(!DM.ARAY[riSTG].GetCntStat(csCmsFix)) {
        //    tmFix.Clear();
        //}

        bool isCycleDown   =  bNeedDown ;
        bool isCycleEnd    =  DM.ARAY[riSTG].CheckAllStat(csNone)  ;

        if(EM_IsErr()) return false ;



         //Normal Decide Step.
             if (isCycleDown       ) {Trace(m_sPartName.c_str(),"CycleDown Stt"); Step.iSeq = scDown ; InitCycleStep(); m_iCycleSttTime=GetTime();} //
        else if (isCycleEnd        ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;

        if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}

        Stat.bWorkEnd = false ;
    }

    //싸이클 작업이 없음.
    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                   Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                          return false ;
        case scDown  : if(CycleDown()  ){ Trace(m_sPartName.c_str(),"CycleAttach End");m_iCycleTime[Step.iSeq]=GetTime()-m_iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

    return false ;
}

bool CStage::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 15000 )) {
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

        case 10: if(IO_GetX(xSTG_Detect3)||
                    IO_GetX(xSTG_Detect4)||
                    IO_GetX(xWKO_Detect5)){

                        if(!HexaPot.GetHomeDone() || m_bNeedStgHome){
                            EM_SetErrMsg(eiSTG_HexaPot , "헥사포트 홈을 잡아야 하는데 미들블럭이 있습니다.");
                            Step.iHome=0;
                            return true ;
                        }


                        MT_SetHomeEnd(miSTG_XStage , true);
                        MT_SetHomeEnd(miSTG_YStage , true);

                        MT_SetPos(miSTG_XStage , m_dLastIdxPosX);
                        MT_SetPos(miSTG_YStage , m_dLastIdxPosY);

                        if(OM.DevOptn.bUseSTG_Vac1)IO_SetY(ySTG_VcumPump1On,true);
                        if(OM.DevOptn.bUseSTG_Vac2)IO_SetY(ySTG_VcumPump2On,true);
                        if(OM.DevOptn.bUseSTG_Vac3)IO_SetY(ySTG_VcumPump3On,true);


                        MT_SetHomeEnd(miSTG_YConvs , true);

                        MT_SetPos(miSTG_YConvs , PM.GetValue(miSTG_YConvs , pvSTG_YConvsWork));



                        //EM_SetErrMsg(eiPRT_RemovePkg , "미들블럭을 스테이지 레일에서 제거해주세요.");
                        Step.iHome = 0 ;
                        return true ;
                 }



                 IO_SetY(ySTG_VcumPump1On,true);
                 IO_SetY(ySTG_VcumPump2On,true);
                 IO_SetY(ySTG_VcumPump3On,true);


                 m_tmDelay.Clear();
                 Step.iHome++;
                 return false ;

        case 11: if(!m_tmDelay.OnDelay(true , 500)) return false ;
                 if(IO_GetX(xSTG_StgVcum1)||IO_GetX(xSTG_StgVcum2)||IO_GetX(xSTG_StgVcum3)){
                     EM_SetErrMsg(eiPRT_RemovePkg , "스테이지 배큠센서가 On 입니다. 자제를 제거해주세요.");

                     Step.iHome = 0 ;
                     return true ;
                 }


        case 12: IO_SetY(ySTG_VcumPump1On,false);
                 IO_SetY(ySTG_VcumPump2On,false);
                 IO_SetY(ySTG_VcumPump3On,false);

                 MT_DoHome(miSTG_XStage); //스테이지 왼쪽 전면이 홈.
                 MT_DoHome(miSTG_YStage);
                 Step.iHome++;
                 return false ;


        case 13: if(!MT_GetHomeEnd(miSTG_XStage)) return false ;
                 if(!MT_GetHomeEnd(miSTG_YStage)) return false ;

                 MT_GoAbsMan(miSTG_XStage , PM.GetValue(miSTG_XStage , pvSTG_XStageWait));
                 MT_GoAbsMan(miSTG_YStage , PM.GetValue(miSTG_YStage , pvSTG_YStageWait));
                 Step.iHome++;
                 return false ;


        case 14: if(!MT_GoAbsMan(miSTG_XStage , PM.GetValue(miSTG_XStage , pvSTG_XStageWait)))return false ;
                 if(!MT_GoAbsMan(miSTG_YStage , PM.GetValue(miSTG_YStage , pvSTG_YStageWait)))return false ;


                 if(!HexaPot.DoHome()){
                     EM_SetErrMsg(eiSTG_HexaPot , "헥사포트 홈동작 에러 입니다.");
                     Trace("HexaPotErr",HexaPot.GetLastErrMsg().c_str());
                     Step.iHome = 0 ;
                     return true ;
                 }
                 Step.iHome++;
                 return false ;

        case 15: if(!HexaPot.GetStopInpos()) return false ;
                 HexaPot.SetPivotPnt(0.0,0.0,HEX_Z_PIVOT_PLACE_ZIG_TOP);
                 HexaPot.MoveAbsRes(paZ , HEX_Z_WAIT); //MIN -12.5
                 HexaPot.Move();
                 Step.iHome++;
                 return false ;

        case 16: if(!HexaPot.GetStopInpos()) return false ;

                 MT_SetHomeEnd(miSTG_YConvs , true);

                 MT_SetPos(miSTG_YConvs , PM.GetValue(miSTG_YConvs , pvSTG_YConvsWork));

                 m_bNeedStgHome = false ;


                 SetLastCmd();
                 Step.iHome = 0;
                 return true ;
    }
}

//One Cycle.
bool CStage::CycleDown()
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

    static double dHexaX ;
    static double dHexaY ;
    static double dHexaZ ;
    static double dHexaU ;
    static double dHexaV ;
    static double dHexaW ;

    static double dStgX  ;
    static double dStgY  ;


    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;


        case 10: dHexaX = HexaPot.GetEncPos(paX);
                 dHexaY = HexaPot.GetEncPos(paY);
                 dHexaZ = HexaPot.GetEncPos(paZ);
                 dHexaU = HexaPot.GetEncPos(paU);
                 dHexaV = HexaPot.GetEncPos(paV);
                 dHexaW = HexaPot.GetEncPos(paW);

                 dStgX  = MT_GetEncPos(miSTG_XStage);
                 dStgY  = MT_GetEncPos(miSTG_YStage);

                 IO_SetY(ySTG_VcumPump1On, false);
                 IO_SetY(ySTG_VcumPump2On, false);
                 IO_SetY(ySTG_VcumPump3On, false);
                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false;

        case 11: if(!m_tmDelay.OnDelay(true, OM.CmnOptn.iHexaPotRelDelay)) return false ;

                 dHexaX = HexaPot.GetEncPos(paX)-dHexaX;
                 dHexaY = HexaPot.GetEncPos(paY)-dHexaY;
                 dHexaZ = HexaPot.GetEncPos(paZ)-dHexaZ;
                 dHexaU = HexaPot.GetEncPos(paU)-dHexaU;
                 dHexaV = HexaPot.GetEncPos(paV)-dHexaV;
                 dHexaW = HexaPot.GetEncPos(paW)-dHexaW;

                 dStgX  = MT_GetEncPos(miSTG_XStage)-dStgX;
                 dStgY  = MT_GetEncPos(miSTG_YStage)-dStgY;

                 SendLog(String("HexaX:")+dHexaX);
                 SendLog(String("HexaY:")+dHexaY);
                 SendLog(String("HexaZ:")+dHexaZ);
                 SendLog(String("HexaU:")+dHexaU);
                 SendLog(String("HexaV:")+dHexaV);
                 SendLog(String("HexaW:")+dHexaW);

                 SendLog(String("StagX:")+dStgX );
                 SendLog(String("StagY:")+dStgY );


                 HexaPot.MoveIncRes(paZ, -1.0);
                 HexaPot.Move();






                 Step.iCycle++;
                 return false ;

        case 12: if(!HexaPot.GetStopInpos()) return false ;
                 HexaPot.MoveAbsRes(paX, HEX_X_WAIT);
                 HexaPot.MoveAbsRes(paY, HEX_Y_WAIT);
                 HexaPot.MoveAbsRes(paZ, HEX_Z_WAIT);
                 HexaPot.MoveAbsRes(paU, HEX_U_WAIT);
                 HexaPot.MoveAbsRes(paV, HEX_V_WAIT);
                 HexaPot.MoveAbsRes(paW, HEX_W_WAIT);
                 HexaPot.Move();

                 Rs232UV.SetCuringOnOff(uvCh1, false);
                 Rs232UV.SetCuringOnOff(uvCh2, false);
                 Rs232UV.SetCuringOnOff(uvCh3, false);
                 Rs232UV.SetCuringOnOff(uvCh4, false);

                 Step.iCycle ++;
                 return false;

        case 13: if(!HexaPot.GetStopInpos()) return false ;
                 DM.ARAY[riSTG].ChangeStat(csCmsFix,csCmsRFixAlign);
                 //DM.ARAY[riSTG].ChangeStat(csCmsFix,csWorkEnd);
                 Step.iCycle = 0;
                 return true ;
    }
}



void CStage::WriteSpc()
{
    int  iWorkingCol ;
    EN_ARAY_ID riWork ;

    //FindChip(iWorkingCol ,riWork);


    TDateTime SttTime ;
    SttTime.Val = Spc.dWorkSttTime ;

    TDateTime EndTime ;
    EndTime.Val = Spc.dWorkEndTime ;


    String sFilePath = WRK_FOLDER+SttTime.FormatString("yyyy_mm_dd")+".csv";
    if (!DirectoryExists(WRK_FOLDER)) {
        ForceDirectories(WRK_FOLDER);
    }

    if(!FileExists(sFilePath)){
        WorkData.SetMaxColRow(41,1);

        WorkData.SetCell( 0 ,0 ,"No"          );
        WorkData.SetCell( 1 ,0 ,"Recipe"      );
        WorkData.SetCell( 2 ,0 ,"EpoxyLotNo"  );
        WorkData.SetCell( 3 ,0 ,"WorkSttTime" );
        WorkData.SetCell( 4 ,0 ,"WorkEndTime" );
        WorkData.SetCell( 5 ,0 ,"ModuleID00"  );
        WorkData.SetCell( 6 ,0 ,"ModuleID10"  );
        WorkData.SetCell( 7 ,0 ,"ModuleID01"  );
        WorkData.SetCell( 8 ,0 ,"ModuleID11"  );
        WorkData.SetCell( 9 ,0 ,"BfAlign00"   );
        WorkData.SetCell(10 ,0 ,"AtAlign00"   );
        WorkData.SetCell(11 ,0 ,"BfHeight00"  );
        WorkData.SetCell(12 ,0 ,"AtHeight00"  );
        WorkData.SetCell(13 ,0 ,"BfHeight10"  );
        WorkData.SetCell(14 ,0 ,"AtHeight10"  );
        WorkData.SetCell(15 ,0 ,"BfHeight01"  );
        WorkData.SetCell(16 ,0 ,"AtHeight01"  );
        WorkData.SetCell(17 ,0 ,"BfHeight11"  );
        WorkData.SetCell(18 ,0 ,"AtHeight11"  );
        WorkData.SetCell(19 ,0 ,"BfAlign00_10");
        WorkData.SetCell(20 ,0 ,"AtAlign00_10");
        WorkData.SetCell(21 ,0 ,"BfAlign00_01");
        WorkData.SetCell(22 ,0 ,"AtAlign00_01");
        WorkData.SetCell(23 ,0 ,"BfAlign10_11");
        WorkData.SetCell(24 ,0 ,"AtAlign10_11");
        WorkData.SetCell(25 ,0 ,"BfAlign01_11");
        WorkData.SetCell(26 ,0 ,"AtAlign01_11");
        WorkData.SetCell(27 ,0 ,"EndHeight00" );
        WorkData.SetCell(28 ,0 ,"EndHeight10" );
        WorkData.SetCell(29 ,0 ,"EndHeight01" );
        WorkData.SetCell(30 ,0 ,"EndHeight11" );
        WorkData.SetCell(31 ,0 ,"DispPres"    );
        WorkData.SetCell(32 ,0 ,"DispSpeed"   );
        WorkData.SetCell(33 ,0 ,"UvCycle"     );
        WorkData.SetCell(34 ,0 ,"UvPower"     );
        WorkData.SetCell(35 ,0 ,"UvHeight"    );
        WorkData.SetCell(36 ,0 ,"UvSpeed"     );
        WorkData.SetCell(37 ,0 ,"UvFix"       );
        WorkData.SetCell(38 ,0 ,"1stZGap"     );
        WorkData.SetCell(39 ,0 ,"Sensor"      );
        WorkData.SetCell(40 ,0 ,"BottomUv"    );
        WorkData.SaveToCsv(sFilePath);
    }

    int iDataRowCnt = WorkData.GetMaxRow();
    WorkData.SetMaxColRow(WorkData.GetMaxCol() ,iDataRowCnt+1);

    WorkData.SetCell(0  ,iDataRowCnt ,iDataRowCnt                     );
    WorkData.SetCell(1  ,iDataRowCnt ,OM.GetCrntDev()                 );
    WorkData.SetCell(2  ,iDataRowCnt ,LOT.GetEpoxyLotNo()             );
    WorkData.SetCell(3  ,iDataRowCnt ,SttTime.FormatString("hh:mm:ss"));
    WorkData.SetCell(4  ,iDataRowCnt ,EndTime.FormatString("hh:mm:ss"));
    WorkData.SetCell(5  ,iDataRowCnt ,Spc.sModuleID00                 );
    WorkData.SetCell(6  ,iDataRowCnt ,Spc.sModuleID10                 );
    WorkData.SetCell(7  ,iDataRowCnt ,Spc.sModuleID01                 );
    WorkData.SetCell(8  ,iDataRowCnt ,Spc.sModuleID11                 );
    WorkData.SetCell(9  ,iDataRowCnt ,Spc.sBfAlign00                  );
    WorkData.SetCell(10 ,iDataRowCnt ,Spc.sAtAlign00                  );
    WorkData.SetCell(11 ,iDataRowCnt ,Spc.sBfHeight00                 );
    WorkData.SetCell(12 ,iDataRowCnt ,Spc.sAtHeight00                 );
    WorkData.SetCell(13 ,iDataRowCnt ,Spc.sBfHeight10                 );
    WorkData.SetCell(14 ,iDataRowCnt ,Spc.sAtHeight10                 );
    WorkData.SetCell(15 ,iDataRowCnt ,Spc.sBfHeight01                 );
    WorkData.SetCell(16 ,iDataRowCnt ,Spc.sAtHeight01                 );
    WorkData.SetCell(17 ,iDataRowCnt ,Spc.sBfHeight11                 );
    WorkData.SetCell(18 ,iDataRowCnt ,Spc.sAtHeight11                 );
    WorkData.SetCell(19 ,iDataRowCnt ,Spc.sBfAlign00_10               );
    WorkData.SetCell(20 ,iDataRowCnt ,Spc.sAtAlign00_10               );
    WorkData.SetCell(21 ,iDataRowCnt ,Spc.sBfAlign00_01               );
    WorkData.SetCell(22 ,iDataRowCnt ,Spc.sAtAlign00_01               );
    WorkData.SetCell(23 ,iDataRowCnt ,Spc.sBfAlign10_11               );
    WorkData.SetCell(24 ,iDataRowCnt ,Spc.sAtAlign10_11               );
    WorkData.SetCell(25 ,iDataRowCnt ,Spc.sBfAlign01_11               );
    WorkData.SetCell(26 ,iDataRowCnt ,Spc.sAtAlign01_11               );
    WorkData.SetCell(27 ,iDataRowCnt ,Spc.sEndHeight00                );
    WorkData.SetCell(28 ,iDataRowCnt ,Spc.sEndHeight10                );
    WorkData.SetCell(29 ,iDataRowCnt ,Spc.sEndHeight01                );
    WorkData.SetCell(30 ,iDataRowCnt ,Spc.sEndHeight11                );
    WorkData.SetCell(31 ,iDataRowCnt ,Spc.sDispPres                   );
    WorkData.SetCell(32 ,iDataRowCnt ,Spc.sDispSpeed                  );
    WorkData.SetCell(33 ,iDataRowCnt ,Spc.sUvCycle                    );
    WorkData.SetCell(34 ,iDataRowCnt ,Spc.sUvPower                    );
    WorkData.SetCell(35 ,iDataRowCnt ,Spc.sUvHeight                   );
    WorkData.SetCell(36 ,iDataRowCnt ,Spc.sUvSpeed                    );
    WorkData.SetCell(37 ,iDataRowCnt ,Spc.sUvFix                      );
    WorkData.SetCell(38 ,iDataRowCnt ,Spc.s1stZGap                    );
    WorkData.SetCell(39 ,iDataRowCnt ,Spc.sSensor                     );
    WorkData.SetCell(40 ,iDataRowCnt ,Spc.sBottomUv                   );

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

    bool bHexZWaitPot = HEX_Z_WAIT == HexaPot.GetTrgPos(paZ) && HexaPot.GetStopInpos(); //MIN -12.5
    bool bStgYWaitPos = MT_GetCmdPos(miSTG_YStage) == PM.GetValue(miSTG_YStage , pvSTG_YStageWait)  ||
                        MT_GetCmdPos(miSTG_YStage) == PM.GetValue(miSTG_YStage , pvSTG_YStageCntr)  ;


    if(!bHexZWaitPot) {
        sMsg = "HexaPot가 대기 위치가 아닙니다." ;
        bRet = false ;

    }

    if(_iMotr == miSTG_XStage){

    }
    else if(_iMotr == miSTG_YStage){


    }
    else if(_iMotr == miSTG_YConvs){
        if(!bStgYWaitPos){
            sMsg = "Stage Y축이 센터 포지션이 아닙니다." ;
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
    //    sMsg = "Cylinder " + AnsiString(AT_GetName(_iActr)) + " is Not this parts." ;
    //    bRet = false ;
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
    m_dLastIdxPosX = MT_GetCmdPos(miSTG_XStage);
    m_dLastIdxPosY = MT_GetCmdPos(miSTG_YStage);
    return ;

}

bool CStage::CheckMoved()
{
    if(m_dLastIdxPosX != MT_GetCmdPos(miSTG_XStage)) return true  ; //이파트는 필요 없다.
    if(m_dLastIdxPosY != MT_GetCmdPos(miSTG_YStage)) return true  ; //이파트는 필요 없다.
    else                                            return false ;

}

bool CStage::CheckStop()
{
    if(!MT_GetStop(miSTG_XStage)) return false ;
    if(!MT_GetStop(miSTG_YStage)) return false ;

    return true ;
}



void CStage::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";




    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPosX    " , m_dLastIdxPosX     );
        UserINI.Load(sPath, "Member" , "m_dLastIdxPosY    " , m_dLastIdxPosY     );
        UserINI.Load(sPath, "Member" , "g_tMidPosOfs_dX   " , g_tMidPosOfs.dX    );
        UserINI.Load(sPath, "Member" , "g_tMidPosOfs_dY   " , g_tMidPosOfs.dY    );
        UserINI.Load(sPath, "Member" , "g_tMidPosOfs_dT   " , g_tMidPosOfs.dT    );

        UserINI.Load(sPath, "Member" , "PkgStat.dMidTopHeight " , PkgStat.dMidTopHeight  );
        UserINI.Load(sPath, "Member" , "PkgStat.dHexaTopHeight" , PkgStat.dHexaTopHeight );

        UserINI.Load(sPath, "Spc" , "dWorkSttTime " , Spc.dWorkSttTime  );
        UserINI.Load(sPath, "Spc" , "dWorkEndTime " , Spc.dWorkEndTime  );
        UserINI.Load(sPath, "Spc" , "sModuleID00  " , Spc.sModuleID00   );
        UserINI.Load(sPath, "Spc" , "sModuleID10  " , Spc.sModuleID10   );
        UserINI.Load(sPath, "Spc" , "sModuleID01  " , Spc.sModuleID01   );
        UserINI.Load(sPath, "Spc" , "sModuleID11  " , Spc.sModuleID11   );
        UserINI.Load(sPath, "Spc" , "sBfAlign00   " , Spc.sBfAlign00    );
        UserINI.Load(sPath, "Spc" , "sAtAlign00   " , Spc.sAtAlign00    );
        UserINI.Load(sPath, "Spc" , "sBfHeight00  " , Spc.sBfHeight00   );
        UserINI.Load(sPath, "Spc" , "sAtHeight00  " , Spc.sAtHeight00   );
        UserINI.Load(sPath, "Spc" , "sBfHeight10  " , Spc.sBfHeight10   );
        UserINI.Load(sPath, "Spc" , "sAtHeight10  " , Spc.sAtHeight10   );
        UserINI.Load(sPath, "Spc" , "sBfHeight01  " , Spc.sBfHeight01   );
        UserINI.Load(sPath, "Spc" , "sAtHeight01  " , Spc.sAtHeight01   );
        UserINI.Load(sPath, "Spc" , "sBfHeight11  " , Spc.sBfHeight11   );
        UserINI.Load(sPath, "Spc" , "sAtHeight11  " , Spc.sAtHeight11   );
        UserINI.Load(sPath, "Spc" , "sBfAlign00_10" , Spc.sBfAlign00_10 );
        UserINI.Load(sPath, "Spc" , "sAtAlign00_10" , Spc.sAtAlign00_10 );
        UserINI.Load(sPath, "Spc" , "sBfAlign00_01" , Spc.sBfAlign00_01 );
        UserINI.Load(sPath, "Spc" , "sAtAlign00_01" , Spc.sAtAlign00_01 );
        UserINI.Load(sPath, "Spc" , "sBfAlign10_11" , Spc.sBfAlign10_11 );
        UserINI.Load(sPath, "Spc" , "sAtAlign10_11" , Spc.sAtAlign10_11 );
        UserINI.Load(sPath, "Spc" , "sBfAlign01_11" , Spc.sBfAlign01_11 );
        UserINI.Load(sPath, "Spc" , "sAtAlign01_11" , Spc.sAtAlign01_11 );
        UserINI.Load(sPath, "Spc" , "sEndHeight00 " , Spc.sEndHeight00  );
        UserINI.Load(sPath, "Spc" , "sEndHeight10 " , Spc.sEndHeight10  );
        UserINI.Load(sPath, "Spc" , "sEndHeight01 " , Spc.sEndHeight01  );
        UserINI.Load(sPath, "Spc" , "sEndHeight11 " , Spc.sEndHeight11  );




    }
    else {
        UserINI.ClearFile(sPath) ;
        m_dLastIdxPosX = MT_GetCmdPos(miSTG_XStage);
        m_dLastIdxPosY = MT_GetCmdPos(miSTG_YStage);
        UserINI.Save(sPath, "Member" , "m_dLastIdxPosX    " , m_dLastIdxPosX     );
        UserINI.Save(sPath, "Member" , "m_dLastIdxPosY    " , m_dLastIdxPosY     );
        UserINI.Save(sPath, "Member" , "g_tMidPosOfs_dX   " , g_tMidPosOfs.dX    );
        UserINI.Save(sPath, "Member" , "g_tMidPosOfs_dY   " , g_tMidPosOfs.dY    );
        UserINI.Save(sPath, "Member" , "g_tMidPosOfs_dT   " , g_tMidPosOfs.dT    );

        UserINI.Save(sPath, "Member" , "PkgStat.dMidTopHeight " , PkgStat.dMidTopHeight  );
        UserINI.Save(sPath, "Member" , "PkgStat.dHexaTopHeight" , PkgStat.dHexaTopHeight );

        UserINI.Save(sPath, "Spc" , "dWorkSttTime " , Spc.dWorkSttTime  );
        UserINI.Save(sPath, "Spc" , "dWorkEndTime " , Spc.dWorkEndTime  );
        UserINI.Save(sPath, "Spc" , "sModuleID00  " , Spc.sModuleID00   );
        UserINI.Save(sPath, "Spc" , "sModuleID10  " , Spc.sModuleID10   );
        UserINI.Save(sPath, "Spc" , "sModuleID01  " , Spc.sModuleID01   );
        UserINI.Save(sPath, "Spc" , "sModuleID11  " , Spc.sModuleID11   );
        UserINI.Save(sPath, "Spc" , "sBfAlign00   " , Spc.sBfAlign00    );
        UserINI.Save(sPath, "Spc" , "sAtAlign00   " , Spc.sAtAlign00    );
        UserINI.Save(sPath, "Spc" , "sBfHeight00  " , Spc.sBfHeight00   );
        UserINI.Save(sPath, "Spc" , "sAtHeight00  " , Spc.sAtHeight00   );
        UserINI.Save(sPath, "Spc" , "sBfHeight10  " , Spc.sBfHeight10   );
        UserINI.Save(sPath, "Spc" , "sAtHeight10  " , Spc.sAtHeight10   );
        UserINI.Save(sPath, "Spc" , "sBfHeight01  " , Spc.sBfHeight01   );
        UserINI.Save(sPath, "Spc" , "sAtHeight01  " , Spc.sAtHeight01   );
        UserINI.Save(sPath, "Spc" , "sBfHeight11  " , Spc.sBfHeight11   );
        UserINI.Save(sPath, "Spc" , "sAtHeight11  " , Spc.sAtHeight11   );
        UserINI.Save(sPath, "Spc" , "sBfAlign00_10" , Spc.sBfAlign00_10 );
        UserINI.Save(sPath, "Spc" , "sAtAlign00_10" , Spc.sAtAlign00_10 );
        UserINI.Save(sPath, "Spc" , "sBfAlign00_01" , Spc.sBfAlign00_01 );
        UserINI.Save(sPath, "Spc" , "sAtAlign00_01" , Spc.sAtAlign00_01 );
        UserINI.Save(sPath, "Spc" , "sBfAlign10_11" , Spc.sBfAlign10_11 );
        UserINI.Save(sPath, "Spc" , "sAtAlign10_11" , Spc.sAtAlign10_11 );
        UserINI.Save(sPath, "Spc" , "sBfAlign01_11" , Spc.sBfAlign01_11 );
        UserINI.Save(sPath, "Spc" , "sAtAlign01_11" , Spc.sAtAlign01_11 );
        UserINI.Save(sPath, "Spc" , "sEndHeight00 " , Spc.sEndHeight00  );
        UserINI.Save(sPath, "Spc" , "sEndHeight10 " , Spc.sEndHeight10  );
        UserINI.Save(sPath, "Spc" , "sEndHeight01 " , Spc.sEndHeight01  );
        UserINI.Save(sPath, "Spc" , "sEndHeight11 " , Spc.sEndHeight11  );
    }
}

bool CStage::GetWorkingRC(int &_iR , int &_iC)
{
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLAlign    , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRPreAlign , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRHeight   , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRAlign    , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRZHeight  , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsREndAlign , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsREndHeight, _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsFix       , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLTopUV2   , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLTopUV1   , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRFixAlign , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRFixHeight, _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csMidREndHeight, _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRNeed     , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindLastRowCol(csWorkEnd      , _iR , _iC)) return true ;
    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLDisp2    , _iR , _iC)) return true ;

    if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLDisp1    , _iR , _iC)) return true ;

/*
    if(OM.DevOptn.GetDispMode() == dmBoth) {
    }
    else if(OM.DevOptn.GetDispMode() == dmFrst){
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLAlign    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRPreAlign , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRHeight   , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRAlign    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRZHeight  , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsREndAlign , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsREndHeight, _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsFix       , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLTopUV    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRFixAlign , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRFixHeight, _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csMidREndHeight, _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRNeed     , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindLastRowCol(csWorkEnd      , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLDisp2    , _iR , _iC)) return true ;

        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLDisp1    , _iR , _iC)) return true ;

    }
    else if(OM.DevOptn.bUseFstDisp && ) {
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLAlign    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRPreAlign , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRHeight   , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRAlign    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRZHeight  , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsREndAlign , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsREndHeight, _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsFix       , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLTopUV    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRFixAlign , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRFixHeight, _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csMidREndHeight, _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRNeed     , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindLastRowCol(csWorkEnd      , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLDisp1    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLDisp2    , _iR , _iC)) return true ;
    }
    else if(OM.DevOptn.bUseSecDisp) {
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLDisp1    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLDisp2    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLAlign    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRPreAlign , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRHeight   , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRAlign    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRZHeight  , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsREndAlign , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsREndHeight, _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsFix       , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsLTopUV    , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRFixAlign , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRFixHeight, _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csMidREndHeight, _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindLastRowCol(csWorkEnd      , _iR , _iC)) return true ;
        if(DM.ARAY[riSTG].FindFrstRowCol(csCmsRNeed     , _iR , _iC)) return true ;
    }
*/
    _iR = -1 ;
    _iC = -1 ;
    return false ;
}




