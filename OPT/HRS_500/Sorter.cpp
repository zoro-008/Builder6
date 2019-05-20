//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sorter.h"
#include "Supply.h"
#include "Picker.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "UserIni.h"
#include "DataMan.h"
#include "LotUnit.h"
#include "Supply.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CSorter  SRT;

int CSorter::GetEmptyBin(EN_CHIP_STAT _iStat)
{
    //빈번호 순서 Left:9,8,7,6,5,4,3,2,1,0   Right:10,11,12,13,14,15,16,17,18,19

    int iLBinCnt [MAX_BIN_CNT] ;
    int iRBinCnt [MAX_BIN_CNT] ;
    int iLBinNo  [MAX_BIN_CNT] ;
    int iRBinNo  [MAX_BIN_CNT] ;

    int iLBBinCnt[MAX_BIN_CNT] ;
    int iRBBinCnt[MAX_BIN_CNT] ;
    int iLBBinNo [MAX_BIN_CNT] ;
    int iRBBinNo [MAX_BIN_CNT] ;

    memset(iLBinCnt  , 0  , sizeof(int) * MAX_BIN_CNT) ;
    memset(iRBinCnt  , 0  , sizeof(int) * MAX_BIN_CNT) ;
    memset(iLBinNo   , -1 , sizeof(int) * MAX_BIN_CNT) ;
    memset(iRBinNo   , -1 , sizeof(int) * MAX_BIN_CNT) ;

    memset(iLBBinCnt , 0  , sizeof(int) * MAX_BIN_CNT) ;
    memset(iRBBinCnt , 0  , sizeof(int) * MAX_BIN_CNT) ;
    memset(iLBBinNo  , -1 , sizeof(int) * MAX_BIN_CNT) ;
    memset(iRBBinNo  , -1 , sizeof(int) * MAX_BIN_CNT) ;

    //GRADE BUFFER CNT
    for(int i=0; i<MAX_BIN_CNT; i++) {
        if(OM.DevOptn.iBnLIdx[i] == (int)_iStat                                           ) {iLBinCnt [i] = OM.DevOptn.iBinMaxCnt - SPL.GetPkgCnt(i); iLBinNo [i] = i ; }
        if(OM.DevOptn.iBnRIdx[i] == (int)_iStat                                           ) {iRBinCnt [i] = OM.DevOptn.iBinMaxCnt - SPR.GetPkgCnt(i); iRBinNo [i] = i ; }
        if(OM.DevOptn.iBnLIdx[i] == (int)gsBuffer && OM.DevOptn.iBnLSel[i] == (int)_iStat ) {iLBBinCnt[i] = OM.DevOptn.iBinMaxCnt - SPL.GetPkgCnt(i); iLBBinNo[i] = i ; }
        if(OM.DevOptn.iBnLIdx[i] == (int)gsBuffer && OM.DevOptn.iBnRSel[i] == (int)_iStat ) {iRBBinCnt[i] = OM.DevOptn.iBinMaxCnt - SPR.GetPkgCnt(i); iRBBinNo[i] = i ; }
    }
    //Cnt
    int iLMin        = OM.DevOptn.iBinMaxCnt + 1;
    int iRMin        = OM.DevOptn.iBinMaxCnt + 1;
    int iLBMin       = OM.DevOptn.iBinMaxCnt + 1;
    int iRBMin       = OM.DevOptn.iBinMaxCnt + 1;

    int iLastLBinNo  = -1 ;
    int iLastRBinNo  = -1 ;
    int iLastLBBinNo = -1 ;
    int iLastRBBinNo = -1 ;

    int iLBinTCnt  = 0 ;
    int iRBinTCnt  = 0 ;
    int iLBBinTCnt = 0 ;
    int iRBBinTCnt = 0 ;

    const int iLeftBinCnt = MAX_BIN_CNT ;

    for(int i=0; i<MAX_BIN_CNT; i++){
        iLBinTCnt  += iLBinCnt[i] ;
        iRBinTCnt  += iRBinCnt[i] ;
        iLBBinTCnt += iLBBinCnt[i] ;
        iRBBinTCnt += iRBBinCnt[i] ;
        if(iLBinCnt[i]  != 0 && iLBinCnt[i]  < iLMin  ) {
            iLMin       = iLBinCnt[i] ;
            iLastLBinNo = iLBinNo [i] ;
        }
        if(iRBinCnt[i]  != 0 && iRBinCnt[i]  < iRMin  ) {
            iRMin       = iRBinCnt[i] ;
            iLastRBinNo = iRBinNo [i] ;
        }
        if(iLBBinCnt[i] != 0 && iLBBinCnt[i] < iLBMin ) {
            iLBMin         = iLBBinCnt[i] ;
            iLastLBBinNo   = iLBBinNo [i] ;
        }
        if(iRBBinCnt[i] != 0 && iRBBinCnt[i] < iRBMin ) {
            iRBMin         = iRBBinCnt[i] ;
            iLastRBBinNo   = iRBBinNo [i] ;
        }
    }
    //Result
    if((iLBinTCnt + iRBinTCnt) > 0) {
        if(iLBinTCnt  > iRBinTCnt ) return iLastLBinNo ;
        else                        return iLastRBinNo + iLeftBinCnt;
    }
    else if((iLBBinTCnt + iRBBinTCnt) > 0) {
        if(iLBBinTCnt > iRBBinTCnt) return iLastLBBinNo ;
        else                        return iLastRBBinNo + iLeftBinCnt;
    }
    else return -1 ;

}

int CSorter::GetMaxBin(EN_CHIP_STAT _iStat)
{
    //빈번호 순서 Left:9,8,7,6,5,4,3,2,1,0   Right:10,11,12,13,14,15,16,17,18,19

    int iLBinCnt [MAX_BIN_CNT] ;
    int iRBinCnt [MAX_BIN_CNT] ;
    int iLBinNo  [MAX_BIN_CNT] ;
    int iRBinNo  [MAX_BIN_CNT] ;

    int iLBBinCnt[MAX_BIN_CNT] ;
    int iRBBinCnt[MAX_BIN_CNT] ;
    int iLBBinNo [MAX_BIN_CNT] ;
    int iRBBinNo [MAX_BIN_CNT] ;

    memset(iLBinCnt  , 0  , sizeof(int) * MAX_BIN_CNT) ;
    memset(iRBinCnt  , 0  , sizeof(int) * MAX_BIN_CNT) ;
    memset(iLBinNo   , -1 , sizeof(int) * MAX_BIN_CNT) ;
    memset(iRBinNo   , -1 , sizeof(int) * MAX_BIN_CNT) ;

    memset(iLBBinCnt , 0  , sizeof(int) * MAX_BIN_CNT) ;
    memset(iRBBinCnt , 0  , sizeof(int) * MAX_BIN_CNT) ;
    memset(iLBBinNo  , -1 , sizeof(int) * MAX_BIN_CNT) ;
    memset(iRBBinNo  , -1 , sizeof(int) * MAX_BIN_CNT) ;

    //GRADE BUFFER CNT
    for(int i=0; i<MAX_BIN_CNT; i++) {
        if(OM.DevOptn.iBnLIdx[i] == (int)_iStat                                           ) {iLBinCnt [i] = SPL.GetPkgCnt(i); iLBinNo [i] = i ; }
        if(OM.DevOptn.iBnRIdx[i] == (int)_iStat                                           ) {iRBinCnt [i] = SPR.GetPkgCnt(i); iRBinNo [i] = i ; }
        if(OM.DevOptn.iBnLIdx[i] == (int)gsBuffer && OM.DevOptn.iBnLSel[i] == (int)_iStat ) {iLBBinCnt[i] = SPL.GetPkgCnt(i); iLBBinNo[i] = i ; }
        if(OM.DevOptn.iBnLIdx[i] == (int)gsBuffer && OM.DevOptn.iBnRSel[i] == (int)_iStat ) {iRBBinCnt[i] = SPR.GetPkgCnt(i); iRBBinNo[i] = i ; }
    }
    //Cnt
    int iMaxLBinNo  = -1 ;
    int iMaxRBinNo  = -1 ;
    int iMaxLBBinNo = -1 ;
    int iMaxRBBinNo = -1 ;

    const int iLeftBinCnt = MAX_BIN_CNT ;

    for(int i=0; i<MAX_BIN_CNT; i++){
        if(iLBinCnt [i] == OM.DevOptn.iBinMaxCnt ) iMaxLBinNo  = iLBinNo  [i] ;
        if(iRBinCnt [i] == OM.DevOptn.iBinMaxCnt ) iMaxRBinNo  = iRBinNo  [i] ;
        if(iLBBinCnt[i] == OM.DevOptn.iBinMaxCnt ) iMaxLBBinNo = iLBBinNo [i] ;
        if(iRBBinCnt[i] == OM.DevOptn.iBinMaxCnt ) iMaxRBBinNo = iRBBinNo [i] ;


    }
    //Result
         if(iMaxLBBinNo > -1) return iMaxLBBinNo ;
    else if(iMaxRBBinNo > -1) return iMaxRBBinNo + MAX_BIN_CNT;

    else if(iMaxLBinNo  > -1) return iMaxLBinNo  ;
    else if(iMaxRBinNo  > -1) return iMaxRBinNo  + MAX_BIN_CNT;

    else return -1 ; // Buffer Not Have .

}

CSorter::CSorter(void)
{
    Init();
}

CSorter::~CSorter (void)
{
    Close();
}

void CSorter::Init()
{
    m_sPartName = "Sorter " ;
    isRunWarn   = false ;
    isMaxBinNo  = -1 ;
    isBinNo     = -1 ;
    Reset();
    Load(true);
}

void CSorter::Close()
{
    Load(false);
}

void CSorter::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CSorter::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}



bool CSorter::FindChip(int &r , int &c )
{
    return false;
}

double CSorter::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int r  , c  ;
    double dPos = 0.0 ;
    //빈번호 순서 Left:9,8,7,6,5,4,3,2,1,0   Right:10,11,12,13,14,15,16,17,18,19
    double dLBN0Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 9  * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dLBN1Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 8  * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dLBN2Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 7  * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dLBN3Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 6  * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dLBN4Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 5  * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dLBN5Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 4  * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dLBN6Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 3  * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dLBN7Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 2  * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dLBN8Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 1  * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dLBN9Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 0  * PM.GetValue(miSRT_X , poSRT_XPtch);




    double dRBN0Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 10 * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dRBN1Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 11 * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dRBN2Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 12 * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dRBN3Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 13 * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dRBN4Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 14 * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dRBN5Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 15 * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dRBN6Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 16 * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dRBN7Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 17 * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dRBN8Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 18 * PM.GetValue(miSRT_X , poSRT_XPtch);
    double dRBN9Pos = PM.GetValue (miSRT_X , pvSRT_XLBn9) + 19 * PM.GetValue(miSRT_X , poSRT_XPtch);



    if(_iMotr == miSRT_X){
        switch(_iPstnId) {
            default          : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piSRT_XWait : dPos = PM.GetValue (_iMotr , pvSRT_XWait        ); break ;
            case piSRT_XPick : dPos = PM.GetValue (_iMotr , pvSRT_XPick        ); break ;
            case piSRT_XLBn0 : dPos = dLBN0Pos                                  ; break ;
            case piSRT_XLBn1 : dPos = dLBN1Pos                                  ; break ;
            case piSRT_XLBn2 : dPos = dLBN2Pos                                  ; break ;
            case piSRT_XLBn3 : dPos = dLBN3Pos                                  ; break ;
            case piSRT_XLBn4 : dPos = dLBN4Pos                                  ; break ;
            case piSRT_XLBn5 : dPos = dLBN5Pos                                  ; break ;
            case piSRT_XLBn6 : dPos = dLBN6Pos                                  ; break ;
            case piSRT_XLBn7 : dPos = dLBN7Pos                                  ; break ;
            case piSRT_XLBn8 : dPos = dLBN8Pos                                  ; break ;
            case piSRT_XLBn9 : dPos = dLBN9Pos                                  ; break ;

            case piSRT_XRBn0 : dPos = dRBN0Pos                                  ; break ;
            case piSRT_XRBn1 : dPos = dRBN1Pos                                  ; break ;
            case piSRT_XRBn2 : dPos = dRBN2Pos                                  ; break ;
            case piSRT_XRBn3 : dPos = dRBN3Pos                                  ; break ;
            case piSRT_XRBn4 : dPos = dRBN4Pos                                  ; break ;
            case piSRT_XRBn5 : dPos = dRBN5Pos                                  ; break ;
            case piSRT_XRBn6 : dPos = dRBN6Pos                                  ; break ;
            case piSRT_XRBn7 : dPos = dRBN7Pos                                  ; break ;
            case piSRT_XRBn8 : dPos = dRBN8Pos                                  ; break ;
            case piSRT_XRBn9 : dPos = dRBN9Pos                                  ; break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CSorter::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;


    if(_iMotr == miSRT_X){
        if(!AT_Complete(aiSRT_Flip , ccBwd)) { sMsg = "Flipper is not Bwd" ; bRet = false ; }
        switch(_iPstnId) {
            default               :  break ;
            case piSRT_XWait      :  break ;
            case piSRT_XPick      :  break ;
            case piSRT_XLBn0      :  break ;
            case piSRT_XLBn1      :  break ;
            case piSRT_XLBn2      :  break ;
            case piSRT_XLBn3      :  break ;
            case piSRT_XLBn4      :  break ;
            case piSRT_XLBn5      :  break ;
            case piSRT_XLBn6      :  break ;
            case piSRT_XLBn7      :  break ;
            case piSRT_XLBn8      :  break ;
            case piSRT_XLBn9      :  break ;

            case piSRT_XRBn0      :  break ;
            case piSRT_XRBn1      :  break ;
            case piSRT_XRBn2      :  break ;
            case piSRT_XRBn3      :  break ;
            case piSRT_XRBn4      :  break ;
            case piSRT_XRBn5      :  break ;
            case piSRT_XRBn6      :  break ;
            case piSRT_XRBn7      :  break ;
            case piSRT_XRBn8      :  break ;
            case piSRT_XRBn9      :  break ;
        }
    }
    else {
        sMsg = MT_GetName(_iMotr) + " is Not this part Motr!" ;
        bRet = false ;
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool  CSorter::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiSRT_Flip){
        if(MT_GetCmdPos(miPCK_Y) > PM.GetValue(miPCK_Y,pvPCK_YPickStart) && _bFwd == ccFwd) {
            sMsg = AT_GetName(aiSRT_Flip) + " is Check Safe (Pick_Y_Axis)" ; bRet = false ;
        }

        if(!MT_GetStop(miSRT_X)){ sMsg = MT_GetName(miSRT_X) + " is Moving" ; bRet = false ; }
    }
    else if(_iActr == aiSRT_Grip){
        if(!MT_GetStop(miSRT_X)){ sMsg = MT_GetName(miSRT_X) + " is Moving" ; bRet = false ; }
    }

    else {
        sMsg = AT_GetName(_iActr) + " is Not this part Cylinder!" ;
        bRet =  false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

bool CSorter::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CSorter::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CSorter::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiSRT_HomeTO);
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

    switch (Step.iHome) {
        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10: if(DM.ARAY[riSRT].CheckAllStat(csNone)) AT_MoveCyl(aiSRT_Grip , ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(DM.ARAY[riSRT].CheckAllStat(csNone) && !AT_MoveCyl(aiSRT_Grip , ccBwd)) return false ;
                 AT_MoveCyl(aiSRT_Flip , ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 12: if(!AT_MoveCyl(aiSRT_Flip , ccBwd)) return false ;
                 MT_DoHome(miSRT_X);
                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miSRT_X))return false ;
                 MT_GoAbsMan(miSRT_X , PM.GetValue(miSRT_X , pvSRT_XWait  ));
                 Step.iHome++;

        case 14: if(!MT_GoAbsMan(miSRT_X , PM.GetValue(miSRT_X , pvSRT_XWait  )))return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CSorter::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;

    if (Step.iSeq ) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CSorter::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    return true ;
}

bool CSorter::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiSRT_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: if(DM.ARAY[riSRT].CheckAllStat(csNone)) AT_MoveCyl(aiSRT_Grip , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(DM.ARAY[riSRT].CheckAllStat(csNone) && !AT_MoveCyl(aiSRT_Grip , ccBwd)) return false ;
                 AT_MoveCyl(aiSRT_Flip , ccBwd);
                 Step.iToStart++ ;
                 return false ;

        case 12: if(!AT_MoveCyl(aiSRT_Flip , ccBwd)) return false ;
                 Step.iToStart = 0;
                 return true ;
    }
}

bool CSorter::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiSRT_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: if(Step.iToStop) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10:
                 Step.iToStop ++ ;
                 return false ;

        case 11: 
                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CSorter::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop) return false ;

        int  iBinNo    = GetEmptyBin(DM.ARAY[riSRT].GetStat(0,0)) ;
        int  iMaxBinNo = GetMaxBin  (DM.ARAY[riSRT].GetStat(0,0)) ;
        bool isCyclePick = !DM.ARAY[riSTN].CheckAllStat(csNone) && DM.ARAY[riSRT].CheckAllStat(csNone);
        bool isCycleSort = !DM.ARAY[riSRT].CheckAllStat(csNone) && iBinNo >= 0 ;//&&!IO_GetX(xSPL_FullDetect)  ; 싸이클 안으로빼기.
        if(!DM.ARAY[riSRT].CheckAllStat(csNone) && iBinNo < 0) {
            isRunWarn   = true      ;
            isBinNo     = iBinNo    ;
            isMaxBinNo  = iMaxBinNo ;
        }
        else {
            isRunWarn   = false ;
            isBinNo     = -1    ;
            isMaxBinNo  = -1    ;
        }

        bool isConEnd    =  DM.ARAY[riSTN].CheckAllStat(csNone) && DM.ARAY[riSRT].CheckAllStat(csNone);

        bool temp1 = !DM.ARAY[riSRT].CheckAllStat(csNone) ;
        bool temp2 = !IO_GetX(xSPL_FullDetect) ;

        //모르는 스트립에러.
        if(!DM.ARAY[riSTN ].GetCntExist() &&  IO_GetX(xSRT_StnDetect) && PCK.GetSeqStep() != CPicker::scPlace
                                                                      && !OM.MstOptn.bCheckCycle
                                                                      && PCK.GetSeqStep() != CPicker::scRezero) EM_SetErr(eiSTN_Unknwn ) ;

        //스트립 사라짐 에러.
        if( DM.ARAY[riSTN ].GetCntExist() && !IO_GetX(xSRT_StnDetect)) EM_SetErr(eiSTN_Dispr  ) ;

        //빈 만땅 에러.
        //if(IO_GetX(xSPL_FullDetect)) EM_SetErr(eiSTR_PKGFeed) ;


        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCyclePick) {Trace(m_sPartName.c_str(),"CyclePick Stt"); Step.iSeq = scPick ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //스트립을 PreBuff로 이송.
        else if (isCycleSort) {Trace(m_sPartName.c_str(),"CycleSort Stt"); Step.iSeq = scSort ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd   ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :                   Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;  return false ;
        case  scIdle :                                                                                    return false ;
        case  scPick : if(CyclePick ()){ Trace(m_sPartName.c_str(),"CyclePick End");Step.iSeq = scIdle ;} return false ;
        case  scSort : if(CycleSort ()){ Trace(m_sPartName.c_str(),"CycleSort End");Step.iSeq = scIdle ;} return false ;
    }
}

bool CSorter::CyclePick(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiSRT_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiSRT_Grip  , ccBwd);
                  MoveActr(aiSRT_Flip  , ccBwd);

                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiSRT_Grip  , ccBwd)) return false ;
                  if(!MoveActr(aiSRT_Flip  , ccBwd)) return false ;
                  MoveMotr(miSRT_X , piSRT_XPick);

                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miSRT_X , piSRT_XPick)) return false ;
                  MoveActr(aiSRT_Flip  , ccFwd);


                  Step.iCycle++;
                  return false ;

        case 13:  if(!MoveActr(aiSRT_Flip  , ccFwd)) return false ;
                  MoveActr(aiSRT_Grip  , ccFwd);

                  Step.iCycle++;
                  return false ;

        case 14:  if(!MoveActr(aiSRT_Grip  , ccFwd)) return false ;
                  MoveActr(aiSRT_Flip  , ccBwd);
                  IO_SetY(ySRT_StnAir , true);
                  Step.iCycle++;
                  return false ;

        case 15:  if(!MoveActr(aiSRT_Flip  , ccBwd)) return false ;
                  IO_SetY(ySRT_StnAir , false);
                  DM.ARAY[riSRT].SetStat(DM.ARAY[riSTN].GetStat(0,0));
                  DM.ARAY[riSTN].SetStat(csNone);
                  Step.iCycle = 0;
                  return true ;
    }
}

bool CSorter::CycleSort(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiSRT_CycleTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    static int iBinNo = 0 ;

    //const bool

    //if(Stat.bReqStop) {
    //    //Step.iCycle = 0;
    //    //return true ;
    //}

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : if(!MoveActr(aiSRT_Grip  , ccFwd)) return false ;
                  MoveActr(aiSRT_Flip  , ccBwd);

                  Step.iCycle++;
                  return false ;

        case 11 : //if(!MoveActr(aiSRT_Grip  , ccFwd)) return false ;
                  if(!MoveActr(aiSRT_Flip  , ccBwd)) return false ;

                  if(IO_GetX(xSPL_FullDetect)) {
                      EM_SetErr(eiSPL_LimitFull) ;
                      Step.iCycle=0;
                      return true ;
                  }
                  iBinNo = GetEmptyBin(DM.ARAY[riSRT].GetStat(0,0)) ;

                  if(iBinNo <0) {
                      EM_SetErr(eiSPL_BinFull) ;
                      Step.iCycle=0;
                      return true ;
                  }

                  if(iBinNo == 0 ) MoveMotr(miSRT_X , piSRT_XLBn0 ) ;
                  if(iBinNo == 1 ) MoveMotr(miSRT_X , piSRT_XLBn1 ) ;
                  if(iBinNo == 2 ) MoveMotr(miSRT_X , piSRT_XLBn2 ) ;
                  if(iBinNo == 3 ) MoveMotr(miSRT_X , piSRT_XLBn3 ) ;
                  if(iBinNo == 4 ) MoveMotr(miSRT_X , piSRT_XLBn4 ) ;
                  if(iBinNo == 5 ) MoveMotr(miSRT_X , piSRT_XLBn5 ) ;
                  if(iBinNo == 6 ) MoveMotr(miSRT_X , piSRT_XLBn6 ) ;
                  if(iBinNo == 7 ) MoveMotr(miSRT_X , piSRT_XLBn7 ) ;
                  if(iBinNo == 8 ) MoveMotr(miSRT_X , piSRT_XLBn8 ) ;
                  if(iBinNo == 9 ) MoveMotr(miSRT_X , piSRT_XLBn9 ) ;
                  if(iBinNo == 10) MoveMotr(miSRT_X , piSRT_XRBn0 ) ;
                  if(iBinNo == 11) MoveMotr(miSRT_X , piSRT_XRBn1 ) ;
                  if(iBinNo == 12) MoveMotr(miSRT_X , piSRT_XRBn2 ) ;
                  if(iBinNo == 13) MoveMotr(miSRT_X , piSRT_XRBn3 ) ;
                  if(iBinNo == 14) MoveMotr(miSRT_X , piSRT_XRBn4 ) ;
                  if(iBinNo == 15) MoveMotr(miSRT_X , piSRT_XRBn5 ) ;
                  if(iBinNo == 16) MoveMotr(miSRT_X , piSRT_XRBn6 ) ;
                  if(iBinNo == 17) MoveMotr(miSRT_X , piSRT_XRBn7 ) ;
                  if(iBinNo == 18) MoveMotr(miSRT_X , piSRT_XRBn8 ) ;
                  if(iBinNo == 19) MoveMotr(miSRT_X , piSRT_XRBn9 ) ;

                  //MoveMotr(miSRT_X , EN_PSTN_ID(piSRT_XLBn0 + iBinNo)) ;
                  Step.iCycle++;
                  return false ;


        case 12 : if(!MT_GetStop(miSRT_X)) return false ;

                  MoveActr(aiSRT_Grip  , ccBwd);

                  //Lot Day Info Add
                  LT.AddLotInfoWorkChip(1);
                  LT.AddLotInfoFailChip(DM.ARAY[riSRT].GetCntStat(csRslt0));  // 0 -> Fail
                  LT.AddLotInfoChips   (DM.ARAY[riSRT].GetCntStat(csRslt0 ) , // Lot Unit 에서 저장 할때 0번을 안쓰네.
                                        DM.ARAY[riSRT].GetCntStat(csRslt1 ) , //
                                        DM.ARAY[riSRT].GetCntStat(csRslt2 ) , //
                                        DM.ARAY[riSRT].GetCntStat(csRslt3 ) , //
                                        DM.ARAY[riSRT].GetCntStat(csRslt4 ) , //
                                        DM.ARAY[riSRT].GetCntStat(csRslt5 ));

                  LT.AddEqpInfoWorkChip(1);
                  LT.AddEqpInfoFailChip(DM.ARAY[riSRT].GetCntStat(csRslt0));

                  DM.ARAY[riSRT].SetStat(csNone) ;
                                    
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiSRT_Grip  , ccBwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case 14 : if(!m_tmTemp.OnDelay(true , 100)) return false ; //자제떨어지는 동안 기다림.

                  MoveMotr(miSRT_X , piSRT_XPick);
                  //Supply Bin Cnt ++
                  //빈번호 순서 Left:7,6,5,4,3,2,1,0   Right:8,9,10,11,12,13,14,15
                  if(iBinNo < MAX_BIN_CNT) {
                      SPL.m_iPkgCnt[iBinNo]++;
                      sTemp = (String)iBinNo + "PkgCnt Added , iPkgCnt is" + AnsiString(SPL.m_iPkgCnt[iBinNo]) ;
                      Trace(m_sPartName.c_str(),sTemp.c_str());
                  }
                  else {
                      SPR.m_iPkgCnt[iBinNo-MAX_BIN_CNT]++;
                      sTemp = (String)iBinNo + "PkgCnt Added , iPkgCnt is" + AnsiString(SPR.m_iPkgCnt[iBinNo]) ;
                      Trace(m_sPartName.c_str(),sTemp.c_str());
                  }



                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case 15 : if(m_tmTemp.OnDelay(!OM.MstOptn.bDebugMode , 3000))EM_SetErr(eiSTR_PKGFeed) ;
                  if(IO_GetX(xSPL_FullDetect)) return false ;
                  //피딩 페일인지 풀 페일인지 알수가 없네...dd

                  Step.iCycle++;
                  return false ;

        case 16 : if(!MoveMotr(miSRT_X , piSRT_XPick)) return false ;
                  //if(IO_GetX(xSPL_FullDetect))EM_SetErr(eiSTR_PKGFeed) ;
                  Step.iCycle = 0;
                  return true ;
    }
}

void CSorter::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CSorter::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CSorter::CheckStop()
{
    if(!MT_GetStop(miSRT_X)) return false ;

    if(!AT_Done(aiSRT_Flip )) return false ;
    if(!AT_Done(aiSRT_Grip )) return false ;
    return true ;
}

void CSorter::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    if(IsLoad) {
        //UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " , m_sWorkedId   );
    }

    else {
        //UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_sWorkedId   " , m_sWorkedId   );
    }
}

bool CSorter::GetLastStrip()
{
    return false ;
}

