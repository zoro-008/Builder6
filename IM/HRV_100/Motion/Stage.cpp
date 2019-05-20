//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Stage.h"
#include "Vision.h"
#include "Image.h"
#include "UtilDefine.h"
#include "SVMaths.h"

//#include <math.h>

//---------------------------------------------------------------------------
#include "DataMan.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
//#include "LotUnit.h"
#include "UserIni.h"
#include "PstnMan.h"
#include "UtilDefine.h"
#include "Sequence.h"

//#include "FormGraph.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
int convertASCII2Number(char ascii)
{
    if(isdigit(ascii)){ return ascii - '0'; }
    if(islower(ascii)){ return ascii - 'a' + 10; }
    return ascii - 'A' + 10;
}
String convertHex2Bin(char* hex)
{
    String result;
    for(int i = 0; hex[i]; i++)
    {
        if(hex[i] == '.'){result += ".";continue;}
        int num = convertASCII2Number(hex[i]);
        for(int bit = 3; bit >= 0; bit--)
        {
            result += num&(1<<bit)? "1" : "0";
        }
    }

    return result;
}

double GetMinFromPx(double _dPx)
{
    return _dPx * OM.MstOptn.dVisnYMaxMin / g_pImg[iiSTGCrnt]->GetHeight();

}




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

    m_pVcmCon = new TRS232C;
    m_pHgtSnr = new TRS232C;

    if(!m_pVcmCon -> Open(0)) ShowMessage("VCM Con Rs232 Port Open Fail") ;
    else                      m_pVcmCon->CallBackReg(ReceiveVcmConMsg);

    if(!m_pHgtSnr -> Open(1)) ShowMessage("Height Sensor Rs232 Port Open Fail") ;
    else                      m_pHgtSnr->CallBackReg(ReceiveHgtSnrMsg);

    m_iActiveStage = 0 ;
    m_eRepeatType  = rtNone ;
    m_iRepeatCnt   = 0 ;
}

void CStage::Close()
{

    Load(false);

    m_pVcmCon -> Close() ;
    m_pHgtSnr -> Close() ;

    delete m_pVcmCon ;
    delete m_pHgtSnr ;
}
/*
void ReceiveVcmConMsg(DWORD _cbInQue);
void ReceiveHgtSnrMsg(DWORD _cbInQue);*/

void CStage::ReceiveVcmConMsg(DWORD _cbInQue)
{
    //Ex : GA,0,CheckSum
    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    double dTemp = 0.0 ;
    BYTE RcvBuff[256];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
    STG.m_pVcmCon -> ReadData(_cbInQue, RcvBuff);
    sRcvMsg += (char*)RcvBuff ;

    Trace(__FUNC__,RcvBuff);

    if(!sRcvMsg.Pos((char)0x03)) return ;
    //Trace("LaserCom",sRcvMsg.c_str());
    STG.m_pVcmCon->sData = sRcvMsg;
    sRcvMsg  = "" ;
}

void CStage::ReceiveHgtSnrMsg(DWORD _cbInQue)
{
    if(_cbInQue == 0) return ;
    //Ex : GA,0,CheckSum
//    static bool bEnter = false ;
//
//    int iTemp ;
//    if(bEnter) {
//        iTemp++;
//    }
//    bEnter = true ;


    static String sRcvMsg = "" ;
    double dTemp = 0.0 ;
    BYTE RcvBuff[256];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
    STG.m_pHgtSnr -> ReadData(_cbInQue, RcvBuff);
    sRcvMsg += (char*)RcvBuff ;

    Trace(__FUNC__,RcvBuff);

    if(sRcvMsg == NULL) return ;
    if(!sRcvMsg.Pos((char)0x0D)) {return ;}
    //Trace("LaserCom",sRcvMsg.c_str());
    STG.m_pHgtSnr->sData = sRcvMsg;
    sRcvMsg  = "" ;
//    bEnter = false ;
}

double CStage::GetHeight(String _sRcvMsg)
{
    //MS,01,+000.073\r

    bool   bPos  = _sRcvMsg.SubString(7,1) == "+" ;
    String sTemp = _sRcvMsg.SubString(8,7) ;

    double dRet  = StrToFloatDef(sTemp , 0.0);

    if(!bPos) dRet = -1 * dRet ;


    //준선이 TEST

    dRet = dRet * OM.CmnOptn.dHghtGain + OM.CmnOptn.dHghtOfst;

    return dRet ;
}

bool CStage::GetAsciiFromStringNo(String _sStringNo , int _iDigit , String & _sAsciiNo)
{
    const int iLength = _sStringNo.Length() ;
    int iGap = _iDigit - iLength ;
    for(int i = 0 ; i < iGap ; i++) {
        _sStringNo = "0" +_sStringNo ;
    }

    _sAsciiNo = "";

    String sChar = "";

    for(int i = 0 ; i < _iDigit ; i++) {
        sChar = _sStringNo[i+1] ;
             if(sChar == "0") _sAsciiNo += (char)0x30;
        else if(sChar == "1") _sAsciiNo += (char)0x31;
        else if(sChar == "2") _sAsciiNo += (char)0x32;
        else if(sChar == "3") _sAsciiNo += (char)0x33;
        else if(sChar == "4") _sAsciiNo += (char)0x34;
        else if(sChar == "5") _sAsciiNo += (char)0x35;
        else if(sChar == "6") _sAsciiNo += (char)0x36;
        else if(sChar == "7") _sAsciiNo += (char)0x37;
        else if(sChar == "8") _sAsciiNo += (char)0x38;
        else if(sChar == "9") _sAsciiNo += (char)0x39;
        else                  _sAsciiNo += (char)0x30;
    }

    return true ;


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


bool CStage::FindChip(int &r , int &c )
{
    r = 0;
    c = DM.ARAY[riSTG].FindFrstCol(csUnkwn);

    if(c ==-1) return false ;
    else       return true  ;

}

double CStage::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;

    int    iR,iC ;
    FindChip(iR ,iC);

    double dWorkPos ;
    dWorkPos = PM.GetValue(miSTG_XIndx , pvSTG_XIndxWorkStt) ;
    dWorkPos = dWorkPos + (PM.GetValue(miSTG_XIndx , poSTG_XIndxPtch) * iC) ;

    if(_iMotr == miSTG_XIndx){
        switch(_iPstnId) {
            default                 : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piSTG_XIndxWait    : dPos = PM.GetValue (_iMotr , pvSTG_XIndxWait      ); break ;
            case piSTG_XIndxWorkStt : dPos = PM.GetValue (_iMotr , pvSTG_XIndxWorkStt   ); break ;
            case piSTG_XIndxWork    : dPos = dWorkPos                                    ; break ;

        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

void CStage::Reset()
{
    ResetTimer();

    STG.m_iRepeatCnt = 0 ;

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
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErrMsg(eiPRT_ToStartTO,m_sPartName.c_str()); //EM_SetErr(eiSTG_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    int iActualStepCnt ;
    int iActualMaxCrnt ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;


        case 10: if(!OM.CmnOptn.bUseStage1) DM.ARAY[riSTG].SetStat(0,0,csWork);
                 if(!OM.CmnOptn.bUseStage2) DM.ARAY[riSTG].SetStat(0,1,csWork);
                 if(!OM.CmnOptn.bUseStage3) DM.ARAY[riSTG].SetStat(0,2,csWork);
                 if(!OM.CmnOptn.bUseStage4) DM.ARAY[riSTG].SetStat(0,3,csWork);
                 if(!OM.CmnOptn.bUseStage5) DM.ARAY[riSTG].SetStat(0,4,csWork);

                 if(!DM.ARAY[riSTG].GetCntStat(csUnkwn)) {
                     if(OM.CmnOptn.bUseStage1) DM.ARAY[riSTG].SetStat(0,0,csUnkwn); else DM.ARAY[riSTG].SetStat(0,0,csWork);
                     if(OM.CmnOptn.bUseStage2) DM.ARAY[riSTG].SetStat(0,1,csUnkwn); else DM.ARAY[riSTG].SetStat(0,1,csWork);
                     if(OM.CmnOptn.bUseStage3) DM.ARAY[riSTG].SetStat(0,2,csUnkwn); else DM.ARAY[riSTG].SetStat(0,2,csWork);
                     if(OM.CmnOptn.bUseStage4) DM.ARAY[riSTG].SetStat(0,3,csUnkwn); else DM.ARAY[riSTG].SetStat(0,3,csWork);
                     if(OM.CmnOptn.bUseStage5) DM.ARAY[riSTG].SetStat(0,4,csUnkwn); else DM.ARAY[riSTG].SetStat(0,4,csWork);


                     iActualStepCnt = (OM.DevOptn.iTestCrntMax / OM.DevOptn.iStepIncrement) ;
                     iActualMaxCrnt = iActualStepCnt * OM.DevOptn.iStepIncrement ;
                     //처음1번스테이지 작업할때 모든 스테이지 클리어 하고.
                     for(int i = 0 ; i < MAX_STAGE_CNT ; i++){
                         g_VisnRslt  [i].Clear();
                         g_DriveData [i].Clear();
                         g_TiltData  [i].Clear();
                         g_VisnRslt  [i].Clear();
                         g_HeightRslt[i].SetMaxData(iActualMaxCrnt+1 , iActualMaxCrnt+1); //0~80 이면 81개임.
                         g_VisnRsltPnt[i].SetMaxData((iActualMaxCrnt+1) * 2);
                         g_bInspEnd  [i]= false;
                     }

                     //DM.ARAY[riSTG].SetStat(csUnkwn);
                     for(int i=0 ; i < MAX_IMG_ID ; i++){
                         g_pImg[i]->ClrImage();
                     }
                     for(int i=0 ; i < MAX_STAGE_CNT ; i++) {
                         g_VisnRslt[i].Clear();

                     }
                 }
                 if(m_eRepeatType != rtNone) {
                     Step.iToStart = 0 ;
                     return true ;
                 }






                 MoveActr(aiSTG_ProbFwBw , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(!MoveActr(aiSTG_ProbFwBw , ccBwd)) return false ;
                 MoveActr(aiSTG_ChckDnUp , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 12: if(!MoveActr(aiSTG_ChckDnUp , ccBwd)) return false ;
                 Step.iToStart=0;
                 return true ;
    }
}

bool CStage::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) EM_SetErrMsg(eiPRT_ToStopTO , m_sPartName.c_str()); //EM_SetErr(eiSTG_ToStopTO);

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

        case 10: if(m_eRepeatType != rtNone) {
                     if(STG.m_iRepeatCnt>0){
                         Step.iToStop = 0 ;
                         return true ;
                     }
                 }
                 MoveActr(aiSTG_ProbFwBw , ccBwd);
                 Step.iToStop++;
                 return false ;

        case 11: if(!MoveActr(aiSTG_ProbFwBw , ccBwd)) return false ;
                 MoveActr(aiSTG_ChckDnUp , ccBwd);
                 Step.iToStop++;
                 return false ;

        case 12: if(!MoveActr(aiSTG_ChckDnUp , ccBwd)) return false ;
                 SetLastCmd();
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

    static int iCycleSttTime = 0;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isCycleWork =  DM.ARAY[riSTG].GetCntStat(csUnkwn) ;
        bool isCycleEnd  = !DM.ARAY[riSTG].GetCntStat(csUnkwn) ;


        ////모르는 스트립에러.
        //if(  DM.ARAY[riSTG].CheckAllStat(csNone) && IO_GetX(xSTG_Tray3Work))
        //    EM_SetErrMsg(eiPKG_Unknwn , DM.ARAY[riSTG].GetName().c_str());
        ////카세트 사라짐.
        //if( !DM.ARAY[riSTG].CheckAllStat(csNone) &&!IO_GetX(xSTG_Tray3Work))
        //    EM_SetErrMsg(eiPKG_Dispr  , DM.ARAY[riSTG].GetName().c_str());
        ////카세트 OUT 매거진 풀

        if(EM_IsErr()) return false ;


         //Normal Decide Step.
             if (isCycleWork  ) {Trace(m_sPartName.c_str(),"CycleWork   Stt"); Step.iSeq = scWork   ; InitCycleStep(); iCycleSttTime=GetTickCount();} //
        else if (isCycleEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default      :                    Trace(m_sPartName.c_str(),"default     End");                                                      Step.iSeq = scIdle ;  return false ;
        case scIdle  :                                                                                                                                             return false ;
        case scWork  : if(CycleWork  ()){ Trace(m_sPartName.c_str(),"CycleWork   End");m_iCycleTime[Step.iSeq]=GetTickCount()-iCycleSttTime; Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}

bool CStage::CycleHome()     //sun DLL direct access.
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

        case 10: AT_MoveCyl(aiSTG_ProbFwBw, ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiSTG_ProbFwBw, ccBwd)) return false ;
                 AT_MoveCyl(aiSTG_ChckDnUp, ccBwd);
                 Step.iHome++;
                 return false ;

        case 12: if(!AT_MoveCyl(aiSTG_ChckDnUp, ccBwd)) return false ;

                 AT_MoveCyl(aiSTG_TbleInOt, ccBwd);
                 MT_DoHome(miSTG_XIndx);
                 Step.iHome++;
                 return false ;

        case 13: if(!AT_MoveCyl   (aiSTG_ProbFwBw, ccBwd)) return false ;
                 if(!AT_MoveCyl   (aiSTG_TbleInOt, ccBwd)) return false ;
                 if(!MT_GetHomeEnd(miSTG_XIndx          )) return false ;

                 MT_GoAbsMan(miSTG_XIndx , PM.GetValue(miSTG_XIndx , pvSTG_XIndxWait ));
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miSTG_XIndx , PM.GetValue(miSTG_XIndx , pvSTG_XIndxWait ))) return false ;
                 SetLastCmd();
                 Step.iHome = 0;
                 return true ;

    }
}

//One Cycle.
bool CStage::CycleWork(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 6000 )) {
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS :%s Step.iCycle=%02d",m_sPartName.c_str() , __FUNC__ , Step.iCycle );
        EM_SetErrMsg(eiPRT_CycleTO , sTemp.c_str()); //EM_SetErr(eiSTG_CycleTO);
        Trace(m_sPartName.c_str(),sTemp.c_str());
        VSN.Live(false,m_iActiveStage);
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
    int r,c;
    String sCrnt ;
    String sMsg ;

    EN_IMG_ID iDstImg ;
    EN_IMG_ID iSrcImg ;


    static int iCrnt    =  0 ;
    static int iPreCrnt = -1 ;

    static int iHeight    = 0 ;
    static int iPreHeight = 0 ;

    static int iInspSttTime = 0 ;

    static int iActualMaxCrnt = 0 ;
    static int iActualMinCrnt = 0 ;
    static int iActualStepCnt = 0 ;

    static double dBiggestGap = 0 ;

    static TPointData SttPnt ;

    static TPointData SensiPntStt ;
    static TPointData SensiPntEnd ;

    static TPointData LinearityStt ;
    static TPointData LinearityEnd ;


    static int iPeakStroke = 0 ;

    static TLiveRslt LiveRslt ;
    static TLiveRslt LiveRsltOfs ; //bUseBfCttOfs옵션이켜져 있을경우.  BfCtt  와  SttPnt 와의 차를 적용한 값.

    double dTemp1 , dTemp2 ,dTemp3, dTemp4 ,dTemp5,dTemp6;

    double dOriRegi ;

    TPointData PntData ;

    //쇼트검사.
    if(OM.DevOptn.bUseCaseShort && Step.iCycle > 10 ){
        if(IO_GetX(xSTG_CaseShort)){
            g_DriveData[m_iActiveStage].bCaseShort = true ;

        }
    }





    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10:
                 FindChip(r,c);
                 m_iActiveStage = c ;

                 if(OM.DevOptn.bUseDuplex) {
                     iActualMinCrnt =  OM.DevOptn.iTestCrntMin ; //민값을 기준으로 잡고
                     iActualStepCnt = (OM.DevOptn.iTestCrntMax - OM.DevOptn.iTestCrntMin) / OM.DevOptn.iStepIncrement ; //맥스값까지 스텝으로 등분하여 카운트를 잡고,
                     iActualMaxCrnt =  iActualMinCrnt + iActualStepCnt * OM.DevOptn.iStepIncrement ; //민값서부터 등분스텝을 계상하여 맥스값을 잡는다.
                 }
                 else {
                     iActualMinCrnt = 0 ;
                     iActualStepCnt = OM.DevOptn.iTestCrntMax / OM.DevOptn.iStepIncrement ;
                     iActualMaxCrnt = iActualStepCnt * OM.DevOptn.iStepIncrement ;

                 }
                 //메뉴얼로 한스테이지만 계속 작업할때 클리어 안되서 히스테리시스가
                 //가장 큰놈인상태로 되어 있어서 갱신이 안되서 클리어 하고 시작.
                 g_VisnRslt   [m_iActiveStage].Clear();
                 g_HeightRslt [m_iActiveStage].SetMaxData(iActualStepCnt+1 , iActualStepCnt+1); //0~80 이면 81개임.
                 g_VisnRsltPnt[m_iActiveStage].SetMaxData((iActualStepCnt+1) * 2 ); //0~80 이면 81개 * 2 임.
                 g_DriveData  [m_iActiveStage].Clear();
                 g_TiltData   [m_iActiveStage].Clear();

                 //커런트 기준이였다가 높이 기준으로 바뀌면서 높이가 해당 DriveTilt높이 까지 않올라가면.
                 //그냥 0.0으로 초기값으로 결과값이 나와서 OK로 인식되는 경우가 있음.
                 g_TiltData  [m_iActiveStage].dTiltInitR    = 999 ;
                 g_TiltData  [m_iActiveStage].dTiltInitT    = 999 ;
                 g_TiltData  [m_iActiveStage].dTiltInit     = 999 ;

                 g_TiltData  [m_iActiveStage].dTiltFull     = 999 ;
                 g_TiltData  [m_iActiveStage].dTiltDrive    = 999 ;
                 g_TiltData  [m_iActiveStage].dTiltDriveAbs = 999 ;
                 g_bInspEnd   [m_iActiveStage]= false;

                 //20151120 남차장 요청.. 컨텍한채로 계속 측정.
                 if(m_iRepeatCnt>0)m_iRepeatCnt-- ;
                 if(m_eRepeatType == rtCurrentRepeat){
                     if(AT_Complete(aiSTG_TbleInOt , ccFwd) &&
                        AT_Complete(aiSTG_ChckDnUp , ccFwd) &&
                        AT_Complete(aiSTG_ProbFwBw , ccFwd) ){
                         Step.iCycle = 17 ;
                         return false ;
                     }
                 }
                 else if(m_eRepeatType == rtPinRepeat){
                     if(AT_Complete(aiSTG_TbleInOt , ccFwd) &&
                        AT_Complete(aiSTG_ChckDnUp , ccFwd) ){
                         Step.iCycle = 17 ;
                         return false ;
                     }
                 }
                 else if(m_eRepeatType == rtChuckPinRepeat){
                     if(AT_Complete(aiSTG_TbleInOt , ccFwd) ){
                         Step.iCycle = 17 ;
                         return false ;
                     }
                 }

                 Step.iCycle++;
                 return false ;

        case 11: MoveActr(aiSTG_ProbFwBw , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 12: if(!MoveActr(aiSTG_ProbFwBw , ccBwd)) return false ;
                 MoveActr(aiSTG_ChckDnUp , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 13: if(!MoveActr(aiSTG_ChckDnUp , ccBwd)) return false ;
                 MoveActr(aiSTG_TbleInOt , ccFwd);
                 Trace("Motor","12 Step");
                 MoveMotr(miSTG_XIndx , piSTG_XIndxWork);
                 Step.iCycle++;
                 return false ;

        case 14: if(!MoveActr(aiSTG_TbleInOt , ccFwd)) return false ;
                 if(!MoveMotr(miSTG_XIndx , piSTG_XIndxWork)) return false ;

                 if(!OM.CmnOptn.bDoubleHold) {
                     Step.iCycle = 17 ; ///
                     return false ;
                 }

                 MoveActr(aiSTG_ChckDnUp , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 15: if(!MoveActr(aiSTG_ChckDnUp , ccFwd)) return false ;
                 MoveActr(aiSTG_ChckDnUp , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 16: if(!MoveActr(aiSTG_ChckDnUp , ccBwd)) return false ;
                 Step.iCycle++;
                 return false ;

        //위에서 씀.
        case 17:

                 //비젼 라이브 온. IM 요청으로 컨텍 하기 전에 라이브 키는 걸로 변경.
                 VSN.Live(true,m_iActiveStage);

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 18: if(!m_tmDelay.OnDelay(true , 200)) return false ;

                 //비젼 결과값 세팅.
                 LiveRslt = VSN.GetLiveRslt() ;
                 g_VisnRslt[m_iActiveStage].dBfCttX   = LiveRslt.Rslt.x;
                 g_VisnRslt[m_iActiveStage].dBfCttY   = LiveRslt.Rslt.y;
                 g_VisnRslt[m_iActiveStage].BfCttRect = LiveRslt.RsltRect;

                 //비포 컨텍 이미지를 리절트 이미지에 저장.
                 iSrcImg = iiSTGCrnt ;
                 iDstImg = iiSTG1Rslt + m_iActiveStage;
                 VSN.ImgCopy(iSrcImg , iDstImg);


                 MoveActr(aiSTG_ChckDnUp , ccFwd);
                 Step.iCycle++;
                 return false ;

        case 19: if(!MoveActr(aiSTG_ChckDnUp , ccFwd)) return false ;
                 MoveActr(aiSTG_ProbFwBw , ccFwd);
                 Step.iCycle++;
                 return false ;

        //컨텍 완료.
        case 20: if(!MoveActr(aiSTG_ProbFwBw , ccFwd)) return false ;

//컨텍 완료 시점.
//================================================================================
//전류 전압 측정.

                 //초기화
                 iInspSttTime = GetTickCount() ;
                 iHeight    = -999 ;
                 iPreHeight = -999 ;
                 dBiggestGap = 0 ;
                 iPeakStroke = 0 ;
                 SttPnt.Clear();
                 SensiPntStt.Clear();
                 SensiPntEnd.Clear();


                 


                 Step.iCycle = 30;
                 return false ;
//곽채훈 대리 요청 다시 예전처럼 최고값에서 전류 측정 하도록.  20141202
//================================================================================

                 //VCM 저항 측정 전류로 세팅.
                 sMsg = GetSendVcmConMsg(OM.DevOptn.iImpdCheckCrnt) ;
                 m_pVcmCon -> sData = "" ;
                 m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;

                 Step.iCycle++;
                 return false ;

                 //일단 Vcm 리턴 받지 않는다.
        case 21: //if(m_pVcmCon -> sData == "")return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 22: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iStableDelay))return false ;

                 //전류 전압  측정.
                 sMsg  = (char)0x02 ;
                 sMsg += "DATA";
                 sMsg += (char)0x03 ;
                 //+ (char)0x03 ;
                 STG.m_pVcmCon->sData = "" ;                           //??
                 STG.m_pVcmCon->SendData(sMsg.Length(),sMsg.c_str());  //??

                 Step.iCycle++;
                 return false ;

        case 23: if(m_pVcmCon -> sData == "")return false ;

                 //오픈
                 if(GetCurrent(m_pVcmCon -> sData) < 0.001 &&
                    GetVolt   (m_pVcmCon -> sData) > 3     ){
                     g_DriveData[m_iActiveStage].bOpenOk = false ;
                 }
                 else {
                     g_DriveData[m_iActiveStage].bOpenOk = true ;
                 }

                 //쇼트
                 if(//GetCurrent(m_pVcmCon -> sData) < 0.001 &&
                    GetVolt   (m_pVcmCon -> sData) < 0.1   ){
                     g_DriveData[m_iActiveStage].bShortOk = false ;
                 }
                 else {
                     g_DriveData[m_iActiveStage].bShortOk = true ;
                 }

                 dOriRegi = GetRegistance(GetCurrent(m_pVcmCon -> sData),GetVolt(m_pVcmCon -> sData));
                 g_DriveData[m_iActiveStage].dImpedance = dOriRegi * OM.CmnOptn.dRegiGain + OM.CmnOptn.dRegiofset ;

                 //오픈쇼트 검사 페일일 경우. 32번으로 가서 그냥 다음 스테이지로 이동.
                 if(!g_DriveData[m_iActiveStage].bOpenOk ||!g_DriveData[m_iActiveStage].bShortOk) { //오픈쇼트를 항목 없애달라고 하고 그냥 임피던스에서만 확인 하게 함.
                     if(OM.DevOptn.dMinImpedance > g_DriveData[m_iActiveStage].dImpedance || //오픈쇼트 페일일 경우 이후 검사 생략.
                        OM.DevOptn.dMaxImpedance < g_DriveData[m_iActiveStage].dImpedance ){

                         //VCM 초기화
                         sMsg = GetSendVcmConMsg(0) ;
                         m_pVcmCon -> sData = "" ;
                         m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;
                         MoveActr(aiSTG_ProbFwBw , ccBwd);
                         Step.iCycle = 60 ;
                         return false ;

                     }
                 }

                 Step.iCycle=30;
                 return false ;


//전압 전류 측정 완료 및 문제 없을시에 전류 초기화
//문제 있을시엔 다음 스테이지로 점프.
//================================================================================
//그래프 측정및 틸트 측정 초기화.


        //위에서씀.
        case 30: //if(!MoveActr(aiSTG_ProbFwBw , ccFwd)) return false ;
                 //iInspSttTime = GetTickCount() ;

                 iHeight    = -999 ;
                 iPreHeight = -999 ;

                 dBiggestGap = 0 ;

                 iPeakStroke = 0 ;

                 SttPnt.Clear();

                 SensiPntStt.Clear();
                 SensiPntEnd.Clear();

                 //VCM 초기화
                 iCrnt = 0 ; //높이 측정기의 리제로 세팅을 위해.
                 iPreCrnt = OM.DevOptn.bUseDuplex ? OM.DevOptn.iTestCrntMin - OM.DevOptn.iStepIncrement : -1;
                 sMsg = GetSendVcmConMsg(iCrnt) ;
                 m_pVcmCon -> sData = "" ;
                 m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;

                 m_tmDelay.Clear();

                 Step.iCycle++;
                 return false ;



        case 31: //if(m_pVcmCon -> sData == "")return false ;   //일단 Vcm 리턴 받지 않는다.
                 if(!m_tmDelay.OnDelay(true , OM.DevOptn.iStableDelay))return false ;

                 //높이 측정기 리제로.
                 sMsg = "VS,01" ; //ZERO~~~~
                 sMsg = sMsg + (char)0x0D ;
                 m_pHgtSnr -> sData = "" ;
                 m_pHgtSnr -> SendData(sMsg.Length() , sMsg.c_str());

                 //비젼 라이브 온. IM 요청으로 컨텍 하기 전에 라이브 키는 걸로 변경.
                 //if(!OM.DevOptn.bUseInitBfCtt){
                 //    VSN.Live(true);
                 //}

                 Step.iCycle++;
                 return false ;

        case 32: if(m_pHgtSnr -> sData == "")return false ;
                 if(!OM.DevOptn.bUseDuplex) Step.iCycle = 40 ; //단방향은 그냥 0부터 시작이니깐 점프.



                 //듀플렉스의 경우... 초기값까지 스텝이동. 한방에 가면 미러가 튄다.
                 //검사 시작값으로 세팅.
                 iCrnt = 0 ;
                 Step.iCycle++;
                 return false ;

        case 33: //iCrnt = OM.DevOptn.bUseDuplex ? OM.DevOptn.iTestCrntMin : 0 ; //검사 초기값으로 세팅.
                 iCrnt-- ;
                 sMsg = GetSendVcmConMsg(iCrnt) ;
                 m_pVcmCon -> sData = "" ;
                 m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 34: if(!m_tmDelay.OnDelay(true , 5 ))return false ;

                 if(iCrnt > OM.DevOptn.iTestCrntMin) {
                     Step.iCycle=33 ;
                     return false ;
                 }
                 Step.iCycle = 40 ;
                 return false ;


//검사민값 넣고.
//검사초기값 넣고 검사 시작.
//듀플렉스의 경우...
//민값까지 스텝으로 이동한다.
//==================================================================================================================
        case 40: //검사 시작값으로 세팅.
                 iCrnt = OM.DevOptn.bUseDuplex ? OM.DevOptn.iTestCrntMin : 0 ; //검사 초기값으로 세팅.
                 sMsg = GetSendVcmConMsg(iCrnt) ;
                 m_pVcmCon -> sData = "" ;
                 m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 41: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iStableDelay ))return false ;

                 //높이 측정.
                 sMsg = "MS,01" ; //측정.
                 sMsg = sMsg + (char)0x0D ;
                 m_pHgtSnr -> sData = "" ;
                 m_pHgtSnr -> SendData(sMsg.Length() , sMsg.c_str());
                                                                      


                     //비젼 결과값 세팅.
                     LiveRslt = VSN.GetLiveRslt() ;
                     iDstImg = iiSTG1Stt + m_iActiveStage;

                     if(OM.DevOptn.bUseBfCttOfs){  //이옵션 사용할때만 오프셑 적용 하고 나머지는 그대로..
                         g_VisnRslt[m_iActiveStage].dBfSttOfsX = g_VisnRslt[m_iActiveStage].dBfCttX - LiveRslt.Rslt.x ;
                         g_VisnRslt[m_iActiveStage].dBfSttOfsY = g_VisnRslt[m_iActiveStage].dBfCttY - LiveRslt.Rslt.y ;

                     }
                     else {
                         g_VisnRslt[m_iActiveStage].dBfSttOfsX = 0.0;
                         g_VisnRslt[m_iActiveStage].dBfSttOfsY = 0.0;
                     }

                     //스타트와 비포컨텍값의 오프셑먹인값.
                     LiveRsltOfs.Rslt.x          = LiveRslt.Rslt.x          + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                     LiveRsltOfs.Rslt.y          = LiveRslt.Rslt.y          + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                     LiveRsltOfs.RsltRect.left   = LiveRslt.RsltRect.left   + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                     LiveRsltOfs.RsltRect.top    = LiveRslt.RsltRect.top    + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                     LiveRsltOfs.RsltRect.right  = LiveRslt.RsltRect.right  + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                     LiveRsltOfs.RsltRect.bottom = LiveRslt.RsltRect.bottom + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;

                     g_pImg[iiSTGCrnt]->SetRect(&LiveRsltOfs.RsltRect);
                     g_pImg[iiSTGCrnt]->SetX   (&LiveRsltOfs.Rslt.x  );
                     g_pImg[iiSTGCrnt]->SetY   (&LiveRsltOfs.Rslt.y  );

                     g_VisnRslt[m_iActiveStage].dSttCntX = LiveRsltOfs.Rslt.x ;
                     g_VisnRslt[m_iActiveStage].dSttCntY = LiveRsltOfs.Rslt.y ;
                     g_VisnRslt[m_iActiveStage].SttRect  = LiveRsltOfs.RsltRect;







                     //스타트 이미지 카피.
                     VSN.ImgCopy(iiSTGCrnt , iDstImg); //라이브이미지를 스타트 이미지에 저장.

                     //스타트 이미지를 리절트 이미지에 저장. 이것은 위로 올라갔음.
                     //iSrcImg = iDstImg ;
                     //iDstImg = iiSTG1Rslt + m_iActiveStage;
                     //VSN.ImgCopy(iSrcImg , iDstImg);



                     //매번 라이브 이미지를 머지 하여 리졀트에 합성해 놓는다.
                     iDstImg = iiSTG1Rslt + m_iActiveStage ;


                     VSN.ImgRectMerge(iiSTGCrnt , iDstImg , LiveRslt.RsltRect , g_VisnRslt[m_iActiveStage].dBfSttOfsX ,
                                                                                g_VisnRslt[m_iActiveStage].dBfSttOfsY );

                     //처음 라이브 결과값 저장.
                     g_VisnRsltPnt[m_iActiveStage].PushData(LiveRsltOfs.Rslt);




                 Step.iCycle++;
                 return false ;

        case 42: if(m_pHgtSnr -> sData == "")return false ;

                 //높이 측정 엣지 구하기 위해...
                 iPreHeight = iHeight ;
                 iHeight = GetHeight(m_pHgtSnr -> sData)*1000 ;

                 PntData.x = iCrnt   ;
                 PntData.y = iHeight ;
                 g_HeightRslt[m_iActiveStage].PushIncData(PntData);



                 //프리로드 시점은 6um뜰때 전류값. 혹시나 첨부터 뜰까해서 확인.
                 if(iHeight >= OM.DevOptn.iPreLoadCheckHt && iPreHeight < OM.DevOptn.iPreLoadCheckHt){
                     g_DriveData[m_iActiveStage].dPreLoad = iCrnt ;
                     SttPnt.x = iPreCrnt   ;
                     SttPnt.y = iPreHeight ;
                 }

                 //피크 스트록.
                 if(iPeakStroke < iHeight) {
                     iPeakStroke = iHeight ;
                 }

                 //커스톰 스트록이 0으로 되어 있을경우를 위해서.
                 if(iCrnt >= OM.DevOptn.iCustomStroke && iPreCrnt < OM.DevOptn.iCustomStroke){
                     g_DriveData[m_iActiveStage].dCustomStroke = iHeight ;
                 }


                 //드라이브 스타트  틸트 확인.
                 if(( OM.MstOptn.bSetUnitHeight && iPreHeight < OM.DevOptn.iDrvTtStrtCrnt && iHeight >= OM.DevOptn.iDrvTtStrtCrnt)|| //높이 세팅일때.
                    (!OM.MstOptn.bSetUnitHeight && iPreCrnt   < OM.DevOptn.iDrvTtStrtCrnt && iCrnt   >= OM.DevOptn.iDrvTtStrtCrnt)){ //ma 세팅일때.
                     //스타트와 비포컨텍값의 오프셑먹인값.
                     LiveRsltOfs.Rslt.x          = LiveRslt.Rslt.x          + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                     LiveRsltOfs.Rslt.y          = LiveRslt.Rslt.y          + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                     LiveRsltOfs.RsltRect.left   = LiveRslt.RsltRect.left   + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                     LiveRsltOfs.RsltRect.top    = LiveRslt.RsltRect.top    + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                     LiveRsltOfs.RsltRect.right  = LiveRslt.RsltRect.right  + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                     LiveRsltOfs.RsltRect.bottom = LiveRslt.RsltRect.bottom + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;

                     g_pImg[iiSTGCrnt]->SetRect(&LiveRsltOfs.RsltRect);
                     g_pImg[iiSTGCrnt]->SetX   (&LiveRsltOfs.Rslt.x  );
                     g_pImg[iiSTGCrnt]->SetY   (&LiveRsltOfs.Rslt.y  );

                     g_VisnRslt[m_iActiveStage].dDriveSttX   = LiveRsltOfs.Rslt.x ;
                     g_VisnRslt[m_iActiveStage].dDriveSttY   = LiveRsltOfs.Rslt.y ;
                     g_VisnRslt[m_iActiveStage].DriveSttRect = LiveRsltOfs.RsltRect;

                 }

                 //센서티비티 스타트 확인.
                 if(OM.MstOptn.bSetUnitHeight) { //센서티비티 측정기준을 높이로 할때
                     if(iHeight >= OM.DevOptn.iSensiSttCrnt && iPreHeight < OM.DevOptn.iSensiSttCrnt){
                         SensiPntStt.x = iCrnt   ;
                         SensiPntStt.y = iHeight ;
                     }

                     //센서티비티 엔드 확인.
                     if(iHeight >= OM.DevOptn.iSensiEndCrnt && iPreHeight < OM.DevOptn.iSensiEndCrnt){
                         SensiPntEnd.x = iCrnt   ;
                         SensiPntEnd.y = iHeight ;
                     }
                 }
                 else { //센서티비티 측정기준을 전류로 할때
                     if(iCrnt >= OM.DevOptn.iSensiSttCrnt && iPreCrnt < OM.DevOptn.iSensiSttCrnt){
                         SensiPntStt.x = iCrnt   ;
                         SensiPntStt.y = iHeight ;
                     }

                     //센서티비티 엔드 확인.
                     if(iCrnt >= OM.DevOptn.iSensiEndCrnt && iPreCrnt < OM.DevOptn.iSensiEndCrnt){
                         SensiPntEnd.x = iCrnt   ;
                         SensiPntEnd.y = iHeight ;
                     }
                 }

                 Step.iCycle=50;
                 return false ;

//그래프 측정 및 틸트 측정 초기화 완료.
//============================================================================================================================================================
//그래프 측정및 틸트 측정 루프 돌리기...


        //증가 스텝.
        //스텝바꿀때 주의 밑에서 씀.
        case 50: //VCM 초기화
                 iPreCrnt = iCrnt ;
                 iCrnt += OM.DevOptn.iStepIncrement;  //+1ma
                 sMsg = GetSendVcmConMsg(iCrnt) ;
                 m_pVcmCon -> sData = "" ;
                 m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;

                 Step.iCycle++;
                 return false ;

        case 51: //if(m_pVcmCon -> sData == "")return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 52: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iStepDelay))return false ;

                 //높이 측정.
                 sMsg = "MS,01" ;
                 sMsg = sMsg + (char)0x0D ;
                 m_pHgtSnr -> sData = "" ;
                 m_pHgtSnr -> SendData(sMsg.Length() , sMsg.c_str());

                 Step.iCycle++;
                 return false ;

        case 53: if(m_pHgtSnr -> sData == "")return false ;

                 //높이 데이터 값. 저장.
                 iPreHeight = iHeight ;
                 iHeight = GetHeight(m_pHgtSnr -> sData)*1000 ;
                 PntData.x = iCrnt ;
                 PntData.y = iHeight ;
                 g_HeightRslt[m_iActiveStage].PushIncData(PntData);

                 //==============================================================위의 스텝에 있던것 드라이브 한스텝 밀리는 현상 있어서 뒤로 옴김.
                 //라이브 결과 데이터 저장.
                 iDstImg = iiSTG1Rslt + m_iActiveStage ;
                 LiveRslt = VSN.GetLiveRslt() ;

                 //스타트와 비포컨텍값의 오프셑먹인값.
                 LiveRsltOfs.Rslt.x          = LiveRslt.Rslt.x          + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                 LiveRsltOfs.Rslt.y          = LiveRslt.Rslt.y          + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                 LiveRsltOfs.RsltRect.left   = LiveRslt.RsltRect.left   + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                 LiveRsltOfs.RsltRect.top    = LiveRslt.RsltRect.top    + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                 LiveRsltOfs.RsltRect.right  = LiveRslt.RsltRect.right  + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                 LiveRsltOfs.RsltRect.bottom = LiveRslt.RsltRect.bottom + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;

                 g_pImg[iiSTGCrnt]->SetRect(&LiveRsltOfs.RsltRect);
                 g_pImg[iiSTGCrnt]->SetX   (&LiveRsltOfs.Rslt.x  );
                 g_pImg[iiSTGCrnt]->SetY   (&LiveRsltOfs.Rslt.y  );


                 //매번 라이브 이미지를 머지 하여 리졀트에 합성해 놓는다.
                 VSN.ImgRectMerge(iiSTGCrnt , iDstImg , LiveRslt.RsltRect , g_VisnRslt[m_iActiveStage].dBfSttOfsX ,
                                                                            g_VisnRslt[m_iActiveStage].dBfSttOfsY );
                 g_VisnRsltPnt[m_iActiveStage].PushData(LiveRsltOfs.Rslt);

                 //가장 큰위치차이 비교.
                 dTemp1 = LiveRsltOfs.Rslt.x - g_VisnRslt[m_iActiveStage].dSttCntX ;
                 dTemp2 = LiveRsltOfs.Rslt.y - g_VisnRslt[m_iActiveStage].dSttCntY ;
                 Trace("case 53 1_1" , String(dTemp1).c_str());
                 Trace("case 53 1_2" , String(dTemp2).c_str());
                 dTemp3 = MATH_Sqrt(dTemp1*dTemp1 + dTemp2*dTemp2) ;
                 if(dTemp3 > dBiggestGap) {
                     dBiggestGap = dTemp3 ;
                     iDstImg = iiSTG1Max + m_iActiveStage;
                     VSN.ImgCopy(iiSTGCrnt , iDstImg);

                     //비젼 결과값 세팅... 맥스값.
                     g_VisnRslt[m_iActiveStage].dMaxCntX = LiveRsltOfs.Rslt.x ;
                     g_VisnRslt[m_iActiveStage].dMaxCntY = LiveRsltOfs.Rslt.y ;
                     g_VisnRslt[m_iActiveStage].MaxRect  = LiveRsltOfs.RsltRect ;

                 }

                 //드라이브 스타트  틸트 확인.
                 if(( OM.MstOptn.bSetUnitHeight && iPreHeight < OM.DevOptn.iDrvTtStrtCrnt && iHeight >= OM.DevOptn.iDrvTtStrtCrnt)|| //높이 세팅일때.
                    (!OM.MstOptn.bSetUnitHeight && iPreCrnt   < OM.DevOptn.iDrvTtStrtCrnt && iCrnt   >= OM.DevOptn.iDrvTtStrtCrnt)){ //ma 세팅일때.
                     //스타트와 비포컨텍값의 오프셑먹인값.
                     LiveRsltOfs.Rslt.x          = LiveRslt.Rslt.x          + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                     LiveRsltOfs.Rslt.y          = LiveRslt.Rslt.y          + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                     LiveRsltOfs.RsltRect.left   = LiveRslt.RsltRect.left   + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                     LiveRsltOfs.RsltRect.top    = LiveRslt.RsltRect.top    + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                     LiveRsltOfs.RsltRect.right  = LiveRslt.RsltRect.right  + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                     LiveRsltOfs.RsltRect.bottom = LiveRslt.RsltRect.bottom + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;

                     g_pImg[iiSTGCrnt]->SetRect(&LiveRsltOfs.RsltRect);
                     g_pImg[iiSTGCrnt]->SetX   (&LiveRsltOfs.Rslt.x  );
                     g_pImg[iiSTGCrnt]->SetY   (&LiveRsltOfs.Rslt.y  );

                     g_VisnRslt[m_iActiveStage].dDriveSttX   = LiveRsltOfs.Rslt.x ;
                     g_VisnRslt[m_iActiveStage].dDriveSttY   = LiveRsltOfs.Rslt.y ;
                     g_VisnRslt[m_iActiveStage].DriveSttRect = LiveRsltOfs.RsltRect;

                 }

                 //드라이브 틸트 확인.
                 if(( OM.MstOptn.bSetUnitHeight && iPreHeight < OM.DevOptn.iDrvTtTestCrnt && iHeight >= OM.DevOptn.iDrvTtTestCrnt)|| //높이 세팅일때.
                    (!OM.MstOptn.bSetUnitHeight && iPreCrnt   < OM.DevOptn.iDrvTtTestCrnt && iCrnt   >= OM.DevOptn.iDrvTtTestCrnt)){ //ma 세팅일때.

                      iDstImg = iiSTG1Drive + m_iActiveStage;
                      VSN.ImgCopy(iiSTGCrnt , iDstImg);

                      dTemp1 = LiveRsltOfs.Rslt.x - g_VisnRslt[m_iActiveStage].dSttCntX ;
                      dTemp2 = LiveRsltOfs.Rslt.y - g_VisnRslt[m_iActiveStage].dSttCntY ;

                      //왜 이걸로 안하고 있지 모르겠다.....
                      //g_VisnRslt[m_iActiveStage].dDriveSttX
                      //g_VisnRslt[m_iActiveStage].dDriveSttY


                      Trace("case 53 2_1" , String(dTemp1).c_str());
                      Trace("case 53 2_2" , String(dTemp2).c_str());
                      dTemp3 = MATH_Sqrt(dTemp1*dTemp1 + dTemp2*dTemp2) ;

                      dTemp4 = LiveRsltOfs.Rslt.x - g_pImg[iiSTGCrnt]->GetWidth ()/2 ;
                      dTemp5 = LiveRsltOfs.Rslt.y - g_pImg[iiSTGCrnt]->GetHeight()/2 ;
                      Trace("case 53 3_1" , String(dTemp4).c_str());
                      Trace("case 53 3_2" , String(dTemp5).c_str());
                      dTemp6 = MATH_Sqrt(dTemp4*dTemp4 + dTemp5*dTemp5) ;

                      if(LiveRsltOfs.Rslt.x == 0 &&
                         g_VisnRslt[m_iActiveStage].dSttCntX == 0 &&
                         LiveRsltOfs.Rslt.y == 0 &&
                         g_VisnRslt[m_iActiveStage].dSttCntY ){
                          g_TiltData[m_iActiveStage].dTiltDrive    = OM.MstOptn.dVisnYMaxMin ; //g_TiltData[m_iActiveStage].dTilt2 + g_TiltData[m_iActiveStage].dTilt1 ;
                          g_TiltData[m_iActiveStage].dTiltDriveAbs = OM.MstOptn.dVisnYMaxMin  ; //g_TiltData[m_iActiveStage].dTilt2 + g_TiltData[m_iActiveStage].dTilt1 ;
                      }
                      else {
                          g_TiltData[m_iActiveStage].dTiltDrive    = GetMinFromPx(dTemp3);//dTemp3 * OM.MstOptn.dVisnYMaxMin / g_pImg[iiSTGCrnt]->GetHeight() ; //g_TiltData[m_iActiveStage].dTilt2 + g_TiltData[m_iActiveStage].dTilt1 ;
                          g_TiltData[m_iActiveStage].dTiltDriveAbs = GetMinFromPx(dTemp6);//dTemp6 * OM.MstOptn.dVisnYMaxMin / g_pImg[iiSTGCrnt]->GetHeight() ;
                      }

                      //드라이브 틸트 값 세팅.
                      g_VisnRslt[m_iActiveStage].dDriveCntX = LiveRsltOfs.Rslt.x ;
                      g_VisnRslt[m_iActiveStage].dDriveCntY = LiveRsltOfs.Rslt.y ;
                      g_VisnRslt[m_iActiveStage].DriveRect  = LiveRsltOfs.RsltRect ;
                 }
                 //==============================================================위의 스텝에 있던것 드라이브 한스텝 밀리는 현상 있어서 뒤로 옴김.

                 //프리로드 시점은 6um뜰때 전류값.
                 if(iHeight >= OM.DevOptn.iPreLoadCheckHt && iPreHeight < OM.DevOptn.iPreLoadCheckHt){
                     g_DriveData[m_iActiveStage].dPreLoad = iCrnt ;
                     SttPnt.x = iPreCrnt   ;
                     SttPnt.y = iPreHeight ;
                 }

                 //피크 스트록.
                 if(iPeakStroke < iHeight) {
                     iPeakStroke = iHeight ;
                 }

                 if(iCrnt >= OM.DevOptn.iCustomStroke && iPreCrnt < OM.DevOptn.iCustomStroke){
                     g_DriveData[m_iActiveStage].dCustomStroke = iHeight ;
                 }

                 //센서티비티 스타트 확인.
                 if(OM.MstOptn.bSetUnitHeight) { //센서티비티 측정기준을 높이로 할때
                     if(iHeight >= OM.DevOptn.iSensiSttCrnt && iPreHeight < OM.DevOptn.iSensiSttCrnt){
                         SensiPntStt.x = iCrnt   ;
                         SensiPntStt.y = iHeight ;
                     }

                     //센서티비티 엔드 확인.
                     if(iHeight >= OM.DevOptn.iSensiEndCrnt && iPreHeight < OM.DevOptn.iSensiEndCrnt){
                         SensiPntEnd.x = iCrnt   ;
                         SensiPntEnd.y = iHeight ;
                     }
                 }
                 else { //센서티비티 측정기준을 전류로 할때
                     if(iCrnt >= OM.DevOptn.iSensiSttCrnt && iPreCrnt < OM.DevOptn.iSensiSttCrnt){
                         SensiPntStt.x = iCrnt   ;
                         SensiPntStt.y = iHeight ;
                     }

                     //센서티비티 엔드 확인.
                     if(iCrnt >= OM.DevOptn.iSensiEndCrnt && iPreCrnt < OM.DevOptn.iSensiEndCrnt){
                         SensiPntEnd.x = iCrnt   ;
                         SensiPntEnd.y = iHeight ;
                     }
                 }






//상승루프 엔드=====================================================================================================================
                 if(iCrnt < iActualMaxCrnt) {
                     Step.iCycle = 50 ;
                     return false ;

                 }


                 //풀 틸트 이미지,데이터 저장.
                 //라이브 결과 데이터 저장.
                 iDstImg = iiSTG1Rslt + m_iActiveStage ;
                 LiveRslt = VSN.GetLiveRslt() ;
                 //스타트와 비포컨텍값의 오프셑먹인값.
                 LiveRsltOfs.Rslt.x          = LiveRslt.Rslt.x          + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                 LiveRsltOfs.Rslt.y          = LiveRslt.Rslt.y          + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                 LiveRsltOfs.RsltRect.left   = LiveRslt.RsltRect.left   + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                 LiveRsltOfs.RsltRect.top    = LiveRslt.RsltRect.top    + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                 LiveRsltOfs.RsltRect.right  = LiveRslt.RsltRect.right  + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                 LiveRsltOfs.RsltRect.bottom = LiveRslt.RsltRect.bottom + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;

                 g_pImg[iiSTGCrnt]->SetRect(&LiveRsltOfs.RsltRect);
                 g_pImg[iiSTGCrnt]->SetX   (&LiveRsltOfs.Rslt.x  );
                 g_pImg[iiSTGCrnt]->SetY   (&LiveRsltOfs.Rslt.y  );

                 iDstImg = iiSTG1Full + m_iActiveStage;
                 VSN.ImgCopy(iiSTGCrnt , iDstImg);
                 dTemp1 = LiveRsltOfs.Rslt.x - g_VisnRslt[m_iActiveStage].dSttCntX ;
                 dTemp2 = LiveRsltOfs.Rslt.y - g_VisnRslt[m_iActiveStage].dSttCntY ;
                 Trace("case 53 3_1" , String(dTemp1).c_str());
                 Trace("case 53 3_2" , String(dTemp2).c_str());
                 dTemp3 = MATH_Sqrt(dTemp1*dTemp1 + dTemp2*dTemp2) ;
                 if(LiveRsltOfs.Rslt.x == 0 && g_VisnRslt[m_iActiveStage].dSttCntX == 0 &&
                    LiveRsltOfs.Rslt.y == 0 && g_VisnRslt[m_iActiveStage].dSttCntY == 0 ){
                     g_TiltData[m_iActiveStage].dTiltFull = 999.0 ;

                 }
                 else {//빔이 dSttCntX에서도 안보이고 현재도 안보이면.
                     g_TiltData[m_iActiveStage].dTiltFull = GetMinFromPx(dTemp3); //dTemp3 * OM.MstOptn.dVisnYMaxMin / g_pImg[iiSTGCrnt]->GetHeight() ; //g_TiltData[m_iActiveStage].dTilt2 + g_TiltData[m_iActiveStage].dTilt1 ;

                 }


                 //풀 틸트 값 세팅.
                 g_VisnRslt[m_iActiveStage].dFullTiltX = LiveRsltOfs.Rslt.x ;
                 g_VisnRslt[m_iActiveStage].dFullTiltY = LiveRsltOfs.Rslt.y ;
                 g_VisnRslt[m_iActiveStage].FullRect   = LiveRsltOfs.RsltRect ;

                 
                 //센서티비티
                 //g_DriveData[m_iActiveStage].dSensitivity = (iCrnt - SttPnt.x) ? (iHeight - SttPnt.y)/(iCrnt - SttPnt.x) : 0.0 ; //2014.10.20 아이엠 전기원 사원 요청 조절 가능하게.
                 g_DriveData[m_iActiveStage].dSensitivity = (SensiPntEnd.x - SensiPntStt.x) ? (SensiPntEnd.y - SensiPntStt.y)/(SensiPntEnd.x - SensiPntStt.x) : 0.0 ; //2014.10.20 아이엠 전기원 사원 요청 조절 가능하게.


                 //리니어리티
                 dTemp3 = -999 ; //프리커런트
                 dTemp4 = -999 ; //프리 헤이트
                 LinearityStt.Clear();
                 LinearityEnd.Clear();
                 for(int i = 0 ; i < g_HeightRslt[m_iActiveStage].iMaxIncDataCnt ; i++) {
                     dTemp1 = g_HeightRslt[m_iActiveStage].tIncData[i].x ; //mA 전류값.
                     dTemp2 = g_HeightRslt[m_iActiveStage].tIncData[i].y ; //um 높이값.

                     //iCrnt >= OM.DevOptn.iSensiSttCrnt &&
                     if(dTemp3 < OM.DevOptn.iLinearSttCrnt && OM.DevOptn.iLinearSttCrnt<=dTemp1 ){ //스타트 점 확인.
                         LinearityStt.x = dTemp1 ;
                         LinearityStt.y = dTemp2 ;
                     }

                     if(dTemp3 < OM.DevOptn.iLinearEndCrnt && OM.DevOptn.iLinearEndCrnt<=dTemp1 ){ //엔드 점 확인.
                         LinearityEnd.x = dTemp1 ;
                         LinearityEnd.y = dTemp2 ;
                     }

                     dTemp3 = dTemp1 ; //전류값
                     dTemp4 = dTemp2 ; //높이값.
                 }
                 dTemp5 = 0 ; //직선방정식에서 가장 멀리 있는 점과 수직 거리. 최단거리 방식 아니고 수직 거리....
                 for(int i = 0 ; i < g_HeightRslt[m_iActiveStage].iMaxIncDataCnt ; i++) {
                     dTemp1 = g_HeightRslt[m_iActiveStage].tIncData[i].x ; //mA 전류값.
                     dTemp2 = g_HeightRslt[m_iActiveStage].tIncData[i].y ; //um 높이값.
                     dTemp3 = MATH_GetLineA(LinearityStt.x,LinearityStt.y,LinearityEnd.x,LinearityEnd.y);  //직선방정식 기울기.
                     dTemp4 = MATH_GetLineB(LinearityStt.x,LinearityStt.y,LinearityEnd.x,LinearityEnd.y);  //직선방정식 Y절편.

                     //iCrnt >= OM.DevOptn.iSensiSttCrnt &&
                     if(OM.DevOptn.iLinearSttCrnt<=dTemp1 && dTemp1<=OM.DevOptn.iLinearEndCrnt){ //스타트 점 확인.
                         //y=ax+b
                         dTemp6 = dTemp3 * dTemp1 + dTemp4 ; //직선방정식에서 현재 포인트 X에 대한 Y값.
                         if(abs(dTemp2 - dTemp6) > abs(dTemp5)){ //현재 포인트의 Y값과 직선방정식간의 직선 거리를 구해서 최대값 보다 크면 최대값으로 갱신.
                             dTemp5 = dTemp2 - dTemp6 ;
                         }
                     }
                 }
                 g_DriveData[m_iActiveStage].dLinearity = dTemp5 ;



                 //내려가는 첫번째 높이값 저장.
                 PntData.x = iCrnt ;
                 PntData.y = GetHeight(m_pHgtSnr -> sData)*1000 ;
                 g_HeightRslt[m_iActiveStage].PushDecData(PntData);





                 //매번 라이브 이미지를 머지 하여 리졀트에 합성해 놓는다.
                 iDstImg = iiSTG1Rslt + m_iActiveStage ;

                 VSN.ImgRectMerge(iiSTGCrnt , iDstImg , LiveRslt.RsltRect , g_VisnRslt[m_iActiveStage].dBfSttOfsX ,
                                                                            g_VisnRslt[m_iActiveStage].dBfSttOfsY );
                 //내려가는 첫번째 틸트값 저장.
                 //처음 라이브 결과값 저장.
                 g_VisnRsltPnt[m_iActiveStage].PushData(LiveRsltOfs.Rslt);




                 Step.iCycle++;
                 return false ;








        case 54:
                 //전류 전압  측정.
                 sMsg  = (char)0x02 ;
                 sMsg += "DATA";
                 sMsg += (char)0x03 ;
                 //+ (char)0x03 ;
                 STG.m_pVcmCon->sData = "" ;
                 STG.m_pVcmCon->SendData(sMsg.Length(),sMsg.c_str());

                 Step.iCycle++;
                 return false ;

        case 55: if(m_pVcmCon -> sData == "")return false ;

                 //오픈
                 if(GetCurrent(m_pVcmCon -> sData) < 0.001 &&
                    GetVolt   (m_pVcmCon -> sData) > 3     ){
                     g_DriveData[m_iActiveStage].bOpenOk = false ;
                 }
                 else {
                     g_DriveData[m_iActiveStage].bOpenOk = true ;
                 }

                 //쇼트
                 if(//GetCurrent(m_pVcmCon -> sData) < 0.001 &&
                     GetVolt    (m_pVcmCon -> sData) < 0.1   ){
                     g_DriveData[m_iActiveStage].bShortOk = false ;
                 }
                 else {
                     g_DriveData[m_iActiveStage].bShortOk = true ;
                 }

                 dOriRegi = GetRegistance(GetCurrent(m_pVcmCon -> sData),GetVolt(m_pVcmCon -> sData));

                 g_DriveData[m_iActiveStage].dImpedance = dOriRegi * OM.CmnOptn.dRegiGain + OM.CmnOptn.dRegiofset ;

                 if(!g_DriveData[m_iActiveStage].bOpenOk ||!g_DriveData[m_iActiveStage].bShortOk) { //오픈쇼트를 항목 없애달라고 하고 그냥 임피던스에서만 확인 하게 함.
                     if(OM.DevOptn.dMinImpedance > g_DriveData[m_iActiveStage].dImpedance || //오픈쇼트 페일일 경우 이후 검사 생략.
                        OM.DevOptn.dMaxImpedance < g_DriveData[m_iActiveStage].dImpedance ){

                         //VCM 초기화
                         sMsg = GetSendVcmConMsg(0) ;
                         m_pVcmCon -> sData = "" ;
                         m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;
                         VSN.Live(false,m_iActiveStage);
                         MoveActr(aiSTG_ProbFwBw , ccBwd);

                         Step.iCycle = 70 ;
                         return false ;

                     }
                 }

                 //케이스 쇼트 검사.
                 if(g_DriveData[m_iActiveStage].bCaseShort ) { //케이스 쇼트시에 그냥 검사 종료... 2015.0716 전기원,곽채훈 책임 요청.

                     //VCM 초기화
                     sMsg = GetSendVcmConMsg(0) ;
                     m_pVcmCon -> sData = "" ;
                     m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;
                     VSN.Live(false,m_iActiveStage);
                     MoveActr(aiSTG_ProbFwBw , ccBwd);

                     Step.iCycle = 70 ;
                     return false ;
                 }

                 Step.iCycle=60;
                 return false ;

        //감소  스텝.
        //스텝바꿀때 주의 밑,위 에서 씀.
        case 60:


                 iCrnt -= OM.DevOptn.iStepIncrement;  //+1ma

                 sMsg = GetSendVcmConMsg(iCrnt) ;
                 m_pVcmCon -> sData = "" ;
                 m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;

                 Step.iCycle++;
                 return false ;

        case 61: //if(m_pVcmCon -> sData == "")return false ;

                 m_tmDelay.Clear();
                 Step.iCycle++;
                 return false ;

        case 62: if(!m_tmDelay.OnDelay(true , OM.DevOptn.iStepDelay))return false ;

                 sMsg = "MS,01" ; //측정.
                 sMsg = sMsg + (char)0x0D ;
                 m_pHgtSnr -> sData = "" ;
                 m_pHgtSnr -> SendData(sMsg.Length() , sMsg.c_str());

                 Step.iCycle++;
                 return false ;

        case 63: if(m_pHgtSnr -> sData == "")return false ;
                 iHeight   = GetHeight(m_pHgtSnr -> sData)*1000 ;
                 PntData.x = iCrnt ;
                 PntData.y = iHeight ;
                 g_HeightRslt[m_iActiveStage].PushDecData(PntData);

                 //==============================================================
                 //라이브 결과 데이터 저장.
                 iDstImg = iiSTG1Rslt + m_iActiveStage ;
                 LiveRslt = VSN.GetLiveRslt() ;
                 //스타트와 비포컨텍값의 오프셑먹인값.
                 LiveRsltOfs.Rslt.x          = LiveRslt.Rslt.x          + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                 LiveRsltOfs.Rslt.y          = LiveRslt.Rslt.y          + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                 LiveRsltOfs.RsltRect.left   = LiveRslt.RsltRect.left   + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                 LiveRsltOfs.RsltRect.top    = LiveRslt.RsltRect.top    + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;
                 LiveRsltOfs.RsltRect.right  = LiveRslt.RsltRect.right  + g_VisnRslt[m_iActiveStage].dBfSttOfsX ;
                 LiveRsltOfs.RsltRect.bottom = LiveRslt.RsltRect.bottom + g_VisnRslt[m_iActiveStage].dBfSttOfsY ;

                 g_pImg[iiSTGCrnt]->SetRect(&LiveRsltOfs.RsltRect);
                 g_pImg[iiSTGCrnt]->SetX   (&LiveRsltOfs.Rslt.x  );
                 g_pImg[iiSTGCrnt]->SetY   (&LiveRsltOfs.Rslt.y  );


                 //매번 라이브 이미지를 머지 하여 리졀트에 합성해 놓는다.
                 VSN.ImgRectMerge(iiSTGCrnt , iDstImg , LiveRslt.RsltRect , g_VisnRslt[m_iActiveStage].dBfSttOfsX ,
                                                                            g_VisnRslt[m_iActiveStage].dBfSttOfsY );
                 g_VisnRsltPnt[m_iActiveStage].PushData(LiveRsltOfs.Rslt);

                 //가장큰 차이 나는 값 찾기위한 값.
                 dTemp1 = LiveRsltOfs.Rslt.x - g_VisnRslt[m_iActiveStage].dSttCntX ;
                 dTemp2 = LiveRsltOfs.Rslt.y - g_VisnRslt[m_iActiveStage].dSttCntY ;
                 Trace("case 63 1_1" , String(dTemp1).c_str());
                 Trace("case 63 1_2" , String(dTemp2).c_str());
                 dTemp3 = MATH_Sqrt(dTemp1*dTemp1 + dTemp2*dTemp2) ;
                 if(dTemp3 > dBiggestGap) {
                     dBiggestGap = dTemp3 ;
                     iDstImg = iiSTG1Max + m_iActiveStage;
                     VSN.ImgCopy(iiSTGCrnt , iDstImg);

                     //비젼 결과값 세팅... 맥스값.
                     g_VisnRslt[m_iActiveStage].dMaxCntX = LiveRsltOfs.Rslt.x ;
                     g_VisnRslt[m_iActiveStage].dMaxCntY = LiveRsltOfs.Rslt.y ;
                     g_VisnRslt[m_iActiveStage].MaxRect  = LiveRsltOfs.RsltRect ;

                 }
                 //==============================================================




                 //피크 스트록.
                 if(iPeakStroke < iHeight) {
                     iPeakStroke = iHeight ;
                 }

                 //0이 아직 안되었으면 계속 한다.
                 if(iCrnt > iActualMinCrnt ) {
                     Step.iCycle = 60 ;
                     return false ;

                 }


                 VSN.Live(false,m_iActiveStage);

                 Step.iCycle++;
                 return false ;


        case 64: if(!VSN.GetGrabEnd()) return false ;

                 //비젼 검사랑 같이 VSN.Live상태이면 화면에 뿌려주면서 결과값이 바뀌므로 뻑날수 있음.
                 //그래서 라이브 끄고 그랩 엔드 확인 하고 검사 한다.
                 //VSN.Inspection(iiSTG1Stt+m_iActiveStage,iiSTG1Max+m_iActiveStage,iiSTG1Rslt+m_iActiveStage,&g_VisnRslt[m_iActiveStage]);
                 //VSN.Inspection(iiSTG1Stt  +m_iActiveStage,
                 //               iiSTG1Drive+m_iActiveStage,
                 //               iiSTG1Full +m_iActiveStage,
                 //               iiSTG1Max  +m_iActiveStage,&g_VisnRslt[m_iActiveStage]);

                 //절대값 측정 영역 (MasterOption에 스테이지별 오프셑이 적용되어야 한다.)==================================================================
                 dTemp1 = g_VisnRslt[m_iActiveStage].dSttCntX - g_pImg[iiSTGCrnt]->GetWidth ()/2 ;
                 dTemp2 = g_VisnRslt[m_iActiveStage].dSttCntY - g_pImg[iiSTGCrnt]->GetHeight()/2 ;
                 if(g_VisnRslt[m_iActiveStage].dSttCntX != 0 || g_VisnRslt[m_iActiveStage].dSttCntY != 0){
                     g_TiltData[m_iActiveStage].dTiltInitR = GetMinFromPx(dTemp1);//dTemp1 * OM.MstOptn.dVisnYMaxMin / g_pImg[iiSTGCrnt]->GetHeight();
                     g_TiltData[m_iActiveStage].dTiltInitT = GetMinFromPx(dTemp2);//dTemp2 * OM.MstOptn.dVisnYMaxMin / g_pImg[iiSTGCrnt]->GetHeight();
                     //g_TiltData[m_iActiveStage].dTiltInitT *= -1 ; //틸트데이터는 Y축이 위로 가면 +로 된다.   20150615 품질쪽 요청사항.
                     Trace("case 64 1_1" , String(dTemp1).c_str());
                     Trace("case 64 1_2" , String(dTemp2).c_str());
                     g_TiltData[m_iActiveStage].dTiltInit = GetMinFromPx(MATH_Sqrt(dTemp1 * dTemp1 + dTemp2 * dTemp2));// * OM.MstOptn.dVisnYMaxMin / g_pImg[iiSTGCrnt]->GetHeight() ;
                 }
                 //========================================================================================================================================

                 //상대값 측정 영역 g_VisnRslt[m_iActiveStage].dSttCnt 값으로 부터의 차이를 구한다.==========================================================
                 dTemp1 = (g_VisnRslt[m_iActiveStage].dMaxCntX - g_VisnRslt[m_iActiveStage].dSttCntX) ;
                 dTemp2 = (g_VisnRslt[m_iActiveStage].dMaxCntY - g_VisnRslt[m_iActiveStage].dSttCntY) ;
                 if(g_VisnRslt[m_iActiveStage].dMaxCntX == 0 && g_VisnRslt[m_iActiveStage].dSttCntX == 0 && //빔이 안보였을경우.
                    g_VisnRslt[m_iActiveStage].dMaxCntY == 0 && g_VisnRslt[m_iActiveStage].dSttCntY == 0 ){
                     g_TiltData[m_iActiveStage].dTiltMax = 999.0 ;
                 }
                 else { //정상.
                     Trace("case 64 2_1" , String(dTemp1).c_str());
                     Trace("case 64 2_2" , String(dTemp2).c_str());
                     g_TiltData[m_iActiveStage].dTiltMax = GetMinFromPx(MATH_Sqrt(dTemp1 * dTemp1 + dTemp2 * dTemp2));// * OM.MstOptn.dVisnYMaxMin / g_pImg[iiSTGCrnt]->GetHeight() ;
                 }
                 //==========================================================================================================================================

                 //총 이동거리..
                 double dPreX ;
                 double dPreY ;
                 double dLengthSum ;
                 double dLength ;
                 dPreX = g_VisnRsltPnt[m_iActiveStage].tData[0].x;
                 dPreY = g_VisnRsltPnt[m_iActiveStage].tData[0].y;
                 dLengthSum = 0.0;           
                 for(int i = 0 ; i < (iActualStepCnt+1) * 2 ; i++) {
                     dTemp1 = g_VisnRsltPnt[m_iActiveStage].tData[i].x - dPreX ;
                     dTemp2 = g_VisnRsltPnt[m_iActiveStage].tData[i].y - dPreY ;
                     Trace((String("case 64 3_1_")+i).c_str() , String(dTemp1).c_str());
                     Trace((String("case 64 3_2_")+i).c_str() , String(dTemp2).c_str());
                     dTemp3 = MATH_Sqrt(dTemp1*dTemp1 + dTemp2*dTemp2) ;
                     dLengthSum += dTemp3 ;
                     dPreX  = g_VisnRsltPnt[m_iActiveStage].tData[i].x ;
                     dPreY  = g_VisnRsltPnt[m_iActiveStage].tData[i].y ;
                 }
                 g_TiltData[m_iActiveStage].dTiltLength = GetMinFromPx(dLengthSum);


                 //Hysterecis 1차.  스펙맥스값에서 오프셑 더해서 1차로 추린후 OK된 놈들만. 2차로 구간에서만 검색해서.. 수율을 높인다. 20150826남우석차장요청. 애메.
                 for(int i = 0 ; i < iActualStepCnt+1 ; i++) {
                     //dTemp1 = g_HeightRslt[m_iActiveStage].tIncData[                 i ].y ;
                     //dTemp2 = g_HeightRslt[m_iActiveStage].tDecData[iActualStepCnt - i ].y ;
                     //
                     //dTemp3 = abs(dTemp1 - dTemp2) ;
                     //
                     //if(dTemp3 > g_DriveData[m_iActiveStage].dHysteresis) {
                     //    g_DriveData[m_iActiveStage].dHysteresis = dTemp3 ;
                     //}

                     //남우석 차장 요청.. 20150826 언더 쪽에서는 2차처럼 값을 가공 한다... .ㅠ  산으로 가는구나 ;;;
                     dTemp1 = g_HeightRslt[m_iActiveStage].tIncData[                 i ].y ;
                     dTemp2 = g_HeightRslt[m_iActiveStage].tDecData[iActualStepCnt - i ].y ;

                     dTemp3 = abs(dTemp1 - dTemp2) ;

                     //둘중에 하나만 해당 높이 구간에 들어오면 구간 외의 놈은 민,혹은 맥스 값으로 세팅하여 구한다.
                     if(dTemp1 >= OM.DevOptn.iHysteSttCrnt &&
                        dTemp2 >= OM.DevOptn.iHysteSttCrnt ){ //둘다 들어온 경우.

                     }
                     else if(dTemp1 >= OM.DevOptn.iHysteSttCrnt ){ //증가만 들어온경우.
                         if(dTemp2 < OM.DevOptn.iHysteSttCrnt)dTemp2 = OM.DevOptn.iHysteSttCrnt ; //감소가 언더인경우.
                     }
                     else if(dTemp2 >= OM.DevOptn.iHysteSttCrnt ){ //감소만 들어온경우.
                         if(dTemp1 < OM.DevOptn.iHysteSttCrnt)dTemp1 = OM.DevOptn.iHysteSttCrnt ; //감소가 언더인경우.
                     }
                     else { //둘다 안들어온 경우
                         continue ;

                     }
                     dTemp3 = abs(dTemp1 - dTemp2) ;
                     if(dTemp3 > g_DriveData[m_iActiveStage].dHysteresis) {
                         g_DriveData[m_iActiveStage].dHysteresis = dTemp3 ;
                     }
                 }
                 //Hysterecis 2차. 아.. 너무 야매다...
                 if(OM.DevOptn.dMinHysteresis                               <= g_DriveData[m_iActiveStage].dHysteresis &&
                    OM.DevOptn.dMaxHysteresis + OM.DevOptn.dPreHysteMaxOfst >= g_DriveData[m_iActiveStage].dHysteresis ){
                     g_DriveData[m_iActiveStage].dHysteresis = 0.0;
                     if(OM.MstOptn.bSetUnitHeight){
                         dTemp1 = 0 ;
                         dTemp2 = 0 ;
                         for(int i = 0 ; i < iActualStepCnt+1 ; i++) {
                             dTemp1 = g_HeightRslt[m_iActiveStage].tIncData[                 i ].y ;
                             dTemp2 = g_HeightRslt[m_iActiveStage].tDecData[iActualStepCnt - i ].y ;

                             dTemp3 = abs(dTemp1 - dTemp2) ;

                             //둘중에 하나만 해당 높이 구간에 들어오면 구간 외의 놈은 민,혹은 맥스 값으로 세팅하여 구한다.
                             if(dTemp1 >= OM.DevOptn.iHysteSttCrnt && dTemp1 <= OM.DevOptn.iHysteEndCrnt &&
                                dTemp2 >= OM.DevOptn.iHysteSttCrnt && dTemp2 <= OM.DevOptn.iHysteEndCrnt ){ //둘다 들어온 경우.

                             }
                             else if(dTemp1 >= OM.DevOptn.iHysteSttCrnt && dTemp1 <= OM.DevOptn.iHysteEndCrnt){ //증가만 들어온경우.
                                 if(dTemp2 < OM.DevOptn.iHysteSttCrnt)dTemp2 = OM.DevOptn.iHysteSttCrnt ; //감소가 언더인경우.
                                 if(dTemp2 > OM.DevOptn.iHysteEndCrnt)dTemp2 = OM.DevOptn.iHysteEndCrnt ; //감소가 오버인경우.
                             }
                             else if(dTemp2 >= OM.DevOptn.iHysteSttCrnt && dTemp2 <= OM.DevOptn.iHysteEndCrnt){ //감소만 들어온경우.
                                 if(dTemp1 < OM.DevOptn.iHysteSttCrnt)dTemp1 = OM.DevOptn.iHysteSttCrnt ; //감소가 언더인경우.
                                 if(dTemp1 > OM.DevOptn.iHysteEndCrnt)dTemp1 = OM.DevOptn.iHysteEndCrnt ; //감소가 오버인경우.
                             }
                             else { //둘다 안들어온 경우
                                 continue ;

                             }
                             dTemp3 = abs(dTemp1 - dTemp2) ;
                             if(dTemp3 > g_DriveData[m_iActiveStage].dHysteresis) {
                                 g_DriveData[m_iActiveStage].dHysteresis = dTemp3 ;
                             }
                         }
                     }
                     else {
                         dTemp1 = 0 ;
                         dTemp2 = 0 ;
                         for(int i = 0 ; i < iActualStepCnt+1 ; i++) {
                             dTemp1 = g_HeightRslt[m_iActiveStage].tIncData[                 i ].y ;
                             dTemp2 = g_HeightRslt[m_iActiveStage].tDecData[iActualStepCnt - i ].y ;
                             dTemp3 = abs(dTemp1 - dTemp2) ;
                             if(i >= OM.DevOptn.iHysteSttCrnt && i <= OM.DevOptn.iHysteEndCrnt){
                                 if(dTemp3 > g_DriveData[m_iActiveStage].dHysteresis) {
                                     g_DriveData[m_iActiveStage].dHysteresis = dTemp3 ;
                                 }
                             }
                         }
                     }
                 }

                 //Peak Stroke
                 g_DriveData[m_iActiveStage].dPeakStroke = iPeakStroke ;

                 m_iInspTime = GetTickCount() - iInspSttTime ;

                 sMsg = GetSendVcmConMsg(0) ; //컨트롤러 초기화.
                 m_pVcmCon -> sData = "" ;
                 m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;

                 DM.ARAY[riSTG].SetStat(0,m_iActiveStage,csWork);

                 //20151120 남차장 요청... 컨텍한체로 데이터 뽑게.
                 //타이머에서 한다. 폼리피트/
                 if(m_eRepeatType == rtCurrentRepeat){
                     if(m_iRepeatCnt>0){
                         SEQ._bBtnStop = true ;
                         Step.iCycle = 0 ;
                         return true     ;
                     }
                 }
                 else if(m_eRepeatType == rtPinRepeat){
                     if(m_iRepeatCnt>0){
                         if(!MoveActr(aiSTG_ProbFwBw , ccBwd)) return false ;
                         SEQ._bBtnStop = true ;
                         Step.iCycle = 0 ;
                         return true     ;
                     }
                 }
                 else if(m_eRepeatType == rtChuckPinRepeat){
                     if(m_iRepeatCnt>0){
                         if(!MoveActr(aiSTG_ProbFwBw , ccBwd)) return false ;
                         if(!MoveActr(aiSTG_ChckDnUp , ccBwd)) return false ;
                         SEQ._bBtnStop = true ;
                         Step.iCycle = 0 ;
                         return false ;
                     }
                 }


                 Step.iCycle=70;
                 return false ;

        //위에서 씀
        case 70: DM.ARAY[riSTG].SetStat(0,m_iActiveStage,csWork);
                 g_bInspEnd[m_iActiveStage]= true ;


                 MoveActr(aiSTG_ProbFwBw , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 71: if(!MoveActr(aiSTG_ProbFwBw , ccBwd)) return false ;
                 MoveActr(aiSTG_ChckDnUp , ccBwd);
                 Step.iCycle++;
                 return false ;

        case 72: if(!MoveActr(aiSTG_ChckDnUp , ccBwd)) return false ;

                 if(!DM.ARAY[riSTG].GetCntStat(csUnkwn)) {
                     MoveActr(aiSTG_TbleInOt , ccBwd);
                     Trace("Motor","39 Step");
                     MoveMotr(miSTG_XIndx , piSTG_XIndxWait);
                 }
                 Step.iCycle++;
                 return false ;

        case 73: if(!AT_Done(aiSTG_TbleInOt)) return false ;
                 if(!MT_GetStop(miSTG_XIndx)) return false ;


                 Step.iCycle = 0 ;
                 return true ;
    }
}

//---------------------------------------------------------------------------
bool CStage::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bChuckDn = AT_Complete(aiSTG_ChckDnUp , ccFwd);
    bool bProbeFw = AT_Complete(aiSTG_ProbFwBw , ccFwd);


    if(_iMotr == miSTG_XIndx){
        if(bChuckDn){ sMsg = " Chuck is Down"   ; bRet = false ;}
        if(bProbeFw){ sMsg = " Probe is Foward" ; bRet = false ;}
        switch(_iPstnId) {
            default                   :                                  { sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion" ; bRet = false ;} break ;
            case piSTG_XIndxWait      :                                                                                                                 break ;
            case piSTG_XIndxWorkStt   :                                                                                                                 break ;
            case piSTG_XIndxWork      :                                                                                                                 break ;

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
bool CStage::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool       bSTG_XMoving = !MT_GetStop(miSTG_XIndx);
    bool       bChuckDn     =  AT_Complete(aiSTG_ChckDnUp , ccFwd);


    if(_iActr == aiSTG_ChckDnUp){
        if(_bFwd == ccFwd) {
            if(bSTG_XMoving) {sMsg = AnsiString("X-Axis Moving!"); bRet = false ;}
        }
    }
    else if(_iActr == aiSTG_ProbFwBw){
    }
    else if(_iActr == aiSTG_TbleInOt){
        //if(_bFwd == ccFwd) {
        if(bChuckDn) {sMsg = AnsiString("Chuck is Down!"); bRet = false ;}
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

bool CStage::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_VALUE _iPstnVal) //Position Form Callback.
{
    if(MT_CmprPos(_iMotr , PM.GetValue(_iMotr , _iPstnVal))) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bChuckDn = AT_Complete(aiSTG_ChckDnUp , ccFwd);
    bool bProbeFw = AT_Complete(aiSTG_ProbFwBw , ccFwd);


    if(_iMotr == miSTG_XIndx){
        if(bChuckDn){ sMsg = " Chuck is Down"   ; bRet = false ;}
        if(bProbeFw){ sMsg = " Probe is Foward" ; bRet = false ;}
        switch(_iPstnVal) {
            default                   :                                  { sMsg = "Not " +AnsiString(MT_GetName(_iMotr))+"'s Postion" ; bRet = false ;} break ;
            case pvSTG_XIndxWait      :                                                                                                                 break ;
            case pvSTG_XIndxWorkStt   :                                                                                                                 break ;

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

bool CStage::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
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
    m_dLastIdxPos = MT_GetCmdPos(miSTG_XIndx);
    return ; //이파트는 필요 없다.

}

bool CStage::CheckMoved()
{
    if(m_dLastIdxPos != MT_GetCmdPos(miSTG_XIndx)) return true  ; //이파트는 필요 없다.
    else                                           return false ;

}

bool CStage::CheckStop()
{
    if(!MT_GetStop(miSTG_XIndx)) return false ;

    if(!AT_Done(aiSTG_ChckDnUp)) return false ;
    if(!AT_Done(aiSTG_ProbFwBw)) return false ;
    if(!AT_Done(aiSTG_TbleInOt)) return false ;

    return true ;
}

void CStage::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName +".INI";

    if(m_dLastIdxPos > MT_GetMaxPos(miSTG_XIndx)) m_dLastIdxPos = MT_GetMaxPos(miSTG_XIndx) ;
    if ( _bLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastIdxPos" , m_dLastIdxPos );
    }
}

String CStage::GetSendVcmConMsg(int _iCrnt)
{
    if(OM.DevOptn.bUseDuplex) return GetSendVcmConMsgDuplex ( _iCrnt);
    else                      return GetSendVcmConMsgSimplex( _iCrnt);
}

String CStage::GetSendVcmConMsgSimplex(int _iCrnt)
{
    const double dMaxCrnt   = 120  ; //백상 컨트롤러에서 출력할 수 있는 최대 암페어. 120ma
    const int    iMaxSet    = 1024 ; //10비트 = 1024
    const double dCrntResol = dMaxCrnt / iMaxSet ; //한눈금. 0.1171875ma


    double dCalCrnt    = _iCrnt * OM.CmnOptn.dCrntGain + OM.CmnOptn.dCrntOfst; //게인과 오프셑 적용된 보정 출력값.
    double dRndOffCrnt = dCalCrnt + dCrntResol / 2 ; //dCalCrnt 값 + 반올림 값. dCrntResol / 2

    int    iSendCrnt = dRndOffCrnt / dCrntResol ; //dCalCrnt 값 + 반올림 값. dCrntResol / 2
    if(iSendCrnt <  0      ) iSendCrnt = 0         ;
    if(iSendCrnt >= iMaxSet) iSendCrnt = iMaxSet-1 ;


    //백상예전 방식 실제 ma값을 넘겨 준다.
    //String sTemp = _iCrnt * OM.CmnOptn.dCrntGain + OM.CmnOptn.dCrntOfst;
    String sTemp = iSendCrnt ;
    String sMsg  = ""     ;
    String sNo  ;

    //아스키 코드로 변환.
    GetAsciiFromStringNo(sTemp , 4 , sNo);

    sMsg += (char)0x02 ;
    sMsg +=  sNo ;
    sMsg += (char)0x03 ;

    return sMsg ;

    //STG.m_pVcmCon->sData = "" ;
    //STG.m_pVcmCon->SendData(sMsg.Length(),sMsg.c_str());
}

String CStage::GetSendVcmConMsgDuplex(int _iCrnt)//20150325 역방향 추가.
{
    const double dMinCrnt   =-100  ;
    const double dMaxCrnt   = 100  ; //백상 컨트롤러에서 출력할 수 있는 최대 암페어. 100ma
    const int    iMaxSet    = 1024 ; //10비트 = 1024
    const int    iZeroSet   = 512  ; //-100ma ~ 0ma ~ 100ma 일때 0ma의 세팅값.
    const double dCrntResol = (dMaxCrnt - dMinCrnt) / iMaxSet ; //한눈금. 0.1953125ma


    double dCalCrnt    = _iCrnt * OM.CmnOptn.dCrntGain + OM.CmnOptn.dCrntOfst; //게인과 오프셑 적용된 보정 출력값.
    double dRndOffCrnt ;

    if(_iCrnt >=0) { //양수일 경우.
        dRndOffCrnt = dCalCrnt + dCrntResol / 2 ; //dCalCrnt 값 + 반올림 값. dCrntResol / 2
    }
    else { //음수일 경우.
        dRndOffCrnt = dCalCrnt - dCrntResol / 2 ; //dCalCrnt 값 + 반올림 값. dCrntResol / 2
    }


    int    iSendCrnt = dRndOffCrnt / dCrntResol + iZeroSet ; //dCalCrnt 값 + 반올림 값. dCrntResol / 2
    if(iSendCrnt <  0      ) iSendCrnt = 0         ;
    if(iSendCrnt >= iMaxSet) iSendCrnt = iMaxSet-1 ;


    //백상예전 방식 실제 ma값을 넘겨 준다.
    //String sTemp = _iCrnt * OM.CmnOptn.dCrntGain + OM.CmnOptn.dCrntOfst;
    String sTemp = iSendCrnt ;
    String sMsg  = ""     ;
    String sNo  ;

    //아스키 코드로 변환.
    GetAsciiFromStringNo(sTemp , 4 , sNo);

    sMsg += (char)0x02 ;
    sMsg +=  sNo ;
    sMsg += (char)0x03 ;

    return sMsg ;

    //STG.m_pVcmCon->sData = "" ;
    //STG.m_pVcmCon->SendData(sMsg.Length(),sMsg.c_str());
}



bool CStage::SetVCMCrnt(int _iCrnt)
{
    String sMsg = "";
    String sCrnt ;

    sCrnt = sCrnt.sprintf("%03d",_iCrnt);

    sMsg += (char)0x02 ;
    sMsg += sCrnt ;
    sMsg += (char)0x03 ;

    m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str());

}

/*
void CStage::SendVCMCrnt(int _iCrnt)
{
    //VCM 저항 측정 전류로 세팅.
    sMsg = GetSendVcmConMsg(OM.DevOptn.iImpdCheckCrnt) ;
    m_pVcmCon -> sData = "" ;
    m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str()); //if(!camera) return false ;



}
*/
bool CStage::RqstVCMCrntVolt()
{
    String sMsg = "";

    sMsg += (char)0x02 ;
    sMsg += "DAT" ;
    sMsg += (char)0x03 ;

    m_pVcmCon -> SendData(sMsg.Length() , sMsg.c_str());
}

double CStage::GetRegistance(double _dCrnt , double _dVolt)
{
    double dRet ;

    if(_dCrnt != 0.0) {
        dRet =  _dVolt / _dCrnt ;
    }
    else {
        dRet = 9999 ;
    }

    return dRet ;
}

double CStage::GetCurrent(String _sMsg)
{
    String sVal = _sMsg.SubString(2,5);
    double dVal = StrToFloatDef(sVal , 0.0);

    dVal = dVal / 100000 ; //0.01 ma단위를 1a단위로 바꿈.

    return dVal ;
}

double CStage::GetVolt   (String _sMsg)
{
    String sVal = _sMsg.SubString(7,5);
    double dVal = StrToFloatDef(sVal , 0.0);

    dVal = dVal / 10000 ;

    return dVal ;

}

