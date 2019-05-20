//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Sorter.h"
#include "Picker.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "UserIni.h"
#include "DataMan.h"
#include "LotUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CSorter  SRT;

int CSorter::GetEmptyBin(EN_CHIP_STAT _iStat)
{
    //GRADE BUFFER CNT
    EN_CHIP_STAT iFailStat = -1;

    for(int i=0; i<MAX_BIN_CNT; i++) {
        if(OM.DevOptn.iBnLSel[i] == (int)gsFail  ) iFailStat = i ;
        if(OM.DevOptn.iBnLSel[i] == (int)_iStat  ) return i ;
    }

    if(_iStat == gsFail) return iFailStat ;

    return -1 ;

}

//현제 위치에서 가장 조금 움직일수 있는 그리퍼를 리턴함.
int  CSorter::GetWorkGripper()
{
    double dMinDist    = 9999.0 ; //일단 이빠이 세팅.
    double dDist       = 0.0    ;
    double dPos        = 0.0    ;
    double dCrntPos    = MT_GetCmdPos(miSRT_X) ;

    int    iMinDistGrp = -1 ;
    int    iBin        = -1 ;


    for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
        if(DM.ARAY[riSRT].GetStat(0,i) == csNone) continue ;
        iBin = GetEmptyBin(DM.ARAY[riSRT].GetStat(0,i)) ;

        dPos = PM.GetValue (miSRT_X , pvSRT_XBin0) + iBin  * PM.GetValue(miSRT_X , poSRT_XBinPtch) ;
        dPos = dPos - i * PM.GetValue(miSRT_X , poSRT_XSrtPtch) ;

        dDist = abs(dCrntPos - dPos) ;

        if(dMinDist > dDist) {
            dMinDist = dDist ;
            iMinDistGrp = i  ;
        }
    }

    return iMinDistGrp ;
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
    Reset();

    ClearBinCnt();

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

double CSorter::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , int _iPickerId)
{
    int r  , c  ;
    double dPos = 0.0 ;
    //빈번호 순서 0,1,2,3,4,5,6,7...
    double dBin0Pos = PM.GetValue (miSRT_X , pvSRT_XBin0) + 0  * PM.GetValue(miSRT_X , poSRT_XBinPtch);
    double dBin1Pos = PM.GetValue (miSRT_X , pvSRT_XBin0) + 1  * PM.GetValue(miSRT_X , poSRT_XBinPtch);
    double dBin2Pos = PM.GetValue (miSRT_X , pvSRT_XBin0) + 2  * PM.GetValue(miSRT_X , poSRT_XBinPtch);
    double dBin3Pos = PM.GetValue (miSRT_X , pvSRT_XBin0) + 3  * PM.GetValue(miSRT_X , poSRT_XBinPtch);
    double dBin4Pos = PM.GetValue (miSRT_X , pvSRT_XBin0) + 4  * PM.GetValue(miSRT_X , poSRT_XBinPtch);
    double dBin5Pos = PM.GetValue (miSRT_X , pvSRT_XBin0) + 5  * PM.GetValue(miSRT_X , poSRT_XBinPtch);
    double dBin6Pos = PM.GetValue (miSRT_X , pvSRT_XBin0) + 6  * PM.GetValue(miSRT_X , poSRT_XBinPtch);
    double dBin7Pos = PM.GetValue (miSRT_X , pvSRT_XBin0) + 7  * PM.GetValue(miSRT_X , poSRT_XBinPtch);
    double dBin8Pos = PM.GetValue (miSRT_X , pvSRT_XBin0) + 8  * PM.GetValue(miSRT_X , poSRT_XBinPtch);
    double dBin9Pos = PM.GetValue (miSRT_X , pvSRT_XBin0) + 9  * PM.GetValue(miSRT_X , poSRT_XBinPtch);

    //Picker의 인덱스를 찾는다.
    int    iPickerIdx = _iPickerId ;

    dBin0Pos = dBin0Pos - iPickerIdx * PM.GetValue(miSRT_X , poSRT_XSrtPtch) ;
    dBin1Pos = dBin1Pos - iPickerIdx * PM.GetValue(miSRT_X , poSRT_XSrtPtch) ;
    dBin2Pos = dBin2Pos - iPickerIdx * PM.GetValue(miSRT_X , poSRT_XSrtPtch) ;
    dBin3Pos = dBin3Pos - iPickerIdx * PM.GetValue(miSRT_X , poSRT_XSrtPtch) ;
    dBin4Pos = dBin4Pos - iPickerIdx * PM.GetValue(miSRT_X , poSRT_XSrtPtch) ;
    dBin5Pos = dBin5Pos - iPickerIdx * PM.GetValue(miSRT_X , poSRT_XSrtPtch) ;
    dBin6Pos = dBin6Pos - iPickerIdx * PM.GetValue(miSRT_X , poSRT_XSrtPtch) ;
    dBin7Pos = dBin7Pos - iPickerIdx * PM.GetValue(miSRT_X , poSRT_XSrtPtch) ;
    dBin8Pos = dBin8Pos - iPickerIdx * PM.GetValue(miSRT_X , poSRT_XSrtPtch) ;
    dBin9Pos = dBin9Pos - iPickerIdx * PM.GetValue(miSRT_X , poSRT_XSrtPtch) ;

    if(_iMotr == miSRT_X){
        switch(_iPstnId) {
            default          : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piSRT_XWait : dPos = PM.GetValue (_iMotr , pvSRT_XWait        ); break ;
            case piSRT_XPick : dPos = PM.GetValue (_iMotr , pvSRT_XPick        ); break ;
            case piSRT_XBin0 : dPos = dBin0Pos                                  ; break ;
            case piSRT_XBin1 : dPos = dBin1Pos                                  ; break ;
            case piSRT_XBin2 : dPos = dBin2Pos                                  ; break ;
            case piSRT_XBin3 : dPos = dBin3Pos                                  ; break ;
            case piSRT_XBin4 : dPos = dBin4Pos                                  ; break ;
            case piSRT_XBin5 : dPos = dBin5Pos                                  ; break ;
            case piSRT_XBin6 : dPos = dBin6Pos                                  ; break ;
            case piSRT_XBin7 : dPos = dBin7Pos                                  ; break ;
            case piSRT_XBin8 : dPos = dBin8Pos                                  ; break ;
            case piSRT_XBin9 : dPos = dBin9Pos                                  ; break ;

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
        if(!AT_Complete(aiSRT_FlipFB , ccBwd)) { sMsg = "Flipper is not Bwd" ; bRet = false ; }
        switch(_iPstnId) {
            default               :  break ;
            case piSRT_XWait      :  break ;
            case piSRT_XPick      :  break ;
            case piSRT_XBin0      :  break ;
            case piSRT_XBin1      :  break ;
            case piSRT_XBin2      :  break ;
            case piSRT_XBin3      :  break ;
            case piSRT_XBin4      :  break ;
            case piSRT_XBin5      :  break ;
            case piSRT_XBin6      :  break ;
            case piSRT_XBin7      :  break ;
            case piSRT_XBin8      :  break ;
            case piSRT_XBin9      :  break ;
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

    bool       bPCKPlacePos = MT_CmprPos(miPCK_Y , PM.GetValue(miPCK_Y , pvPCK_YPlce)) ;
    bool       bPickrDn     = AT_Complete(aiPCK_PckrDU , ccFwd);
    bool       bSRTPickPos  = MT_CmprPos(miSRT_X , PM.GetValue(miSRT_X , pvSRT_XPick)) ;
    bool       bSRTFlpFwd   = AT_Complete(aiSRT_FlipFB , ccFwd);

    if(_iActr == aiSRT_FlipFB){
        if(_bFwd == ccFwd){
            if(MT_GetCmdPos(miPCK_Y) < PM.GetValue(miPCK_Y,pvPCK_YZig) ) {
                sMsg = MT_GetName(miPCK_Y) + " is not in safety zone" ; bRet = false ;
            }

            if(!AT_Complete(aiSTN_GuidFB,ccFwd)) {
                sMsg = AT_GetName(aiSTN_GuidFB) + " is not Fwd" ; bRet = false ;
            }

            
            if(!bSRTPickPos){
                sMsg = MT_GetName(miSRT_X) + " is not Pick Position" ; bRet = false;
            }

        }


        if(!MT_GetStop(miSRT_X)){ sMsg = MT_GetName(miSRT_X) + " is Moving" ; bRet = false ; }
    }
    else if(_iActr == aiSRT_Grip1FB || _iActr == aiSRT_Grip2FB || _iActr == aiSRT_Grip3FB){
        if(!MT_GetStop(miSRT_X)){ sMsg = MT_GetName(miSRT_X) + " is Moving" ; bRet = false ; }
    }
    else if(_iActr == aiSTN_GuidFB) {
        if(bPCKPlacePos && bPickrDn) { sMsg = AT_GetName(aiPCK_PckrDU) + " is Fwd" ; bRet = false ; }
        if(bSRTFlpFwd              ) { sMsg = AT_GetName(aiSRT_FlipFB) + " is Fwd" ; bRet = false ; }
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

bool CSorter::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId , int _iPickerId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId , _iPickerId);

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

        case 10: if(DM.ARAY[riSRT].GetStat(0,0) == csNone ) AT_MoveCyl(aiSRT_Grip1FB , ccBwd);
                 if(DM.ARAY[riSRT].GetStat(0,1) == csNone ) AT_MoveCyl(aiSRT_Grip2FB , ccBwd);
                 if(DM.ARAY[riSRT].GetStat(0,2) == csNone ) AT_MoveCyl(aiSRT_Grip3FB , ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(DM.ARAY[riSRT].GetStat(0,0) == csNone && !AT_MoveCyl(aiSRT_Grip1FB , ccBwd)) return false ;
                 if(DM.ARAY[riSRT].GetStat(0,1) == csNone && !AT_MoveCyl(aiSRT_Grip2FB , ccBwd)) return false ;
                 if(DM.ARAY[riSRT].GetStat(0,2) == csNone && !AT_MoveCyl(aiSRT_Grip3FB , ccBwd)) return false ;



                 AT_MoveCyl(aiSRT_FlipFB , ccBwd);
                 Step.iHome++ ;
                 return false ;

        case 12: if(!AT_MoveCyl(aiSRT_FlipFB , ccBwd)) return false ;
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

        case 10: if(DM.ARAY[riSRT].GetStat(0,0) == csNone ) AT_MoveCyl(aiSRT_Grip1FB , ccBwd);
                 if(DM.ARAY[riSRT].GetStat(0,1) == csNone ) AT_MoveCyl(aiSRT_Grip2FB , ccBwd);
                 if(DM.ARAY[riSRT].GetStat(0,2) == csNone ) AT_MoveCyl(aiSRT_Grip3FB , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 11: if(DM.ARAY[riSRT].GetStat(0,0) == csNone && !AT_MoveCyl(aiSRT_Grip1FB , ccBwd)) return false ;
                 if(DM.ARAY[riSRT].GetStat(0,1) == csNone && !AT_MoveCyl(aiSRT_Grip2FB , ccBwd)) return false ;
                 if(DM.ARAY[riSRT].GetStat(0,2) == csNone && !AT_MoveCyl(aiSRT_Grip3FB , ccBwd)) return false ;
                 AT_MoveCyl(aiSRT_FlipFB , ccBwd);
                 Step.iToStart++ ;
                 return false ;

        case 12: if(!AT_MoveCyl(aiSRT_FlipFB , ccBwd)) return false ;
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

        case 10: IO_SetY(ySRT_StnAir , false );
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

        bool isCyclePick = !DM.ARAY[riSTN].CheckAllStat(csNone) && DM.ARAY[riSRT].CheckAllStat(csNone);
        bool isCycleSort = !DM.ARAY[riSRT].CheckAllStat(csNone) ;//&& iBinNo >= 0 ;//&&!IO_GetX(xSPL_FullDetect)  ; 싸이클 안으로빼기.
        bool isConEnd    =  DM.ARAY[riSTN].CheckAllStat(csNone) && DM.ARAY[riSRT].CheckAllStat(csNone);

        bool temp1 = !DM.ARAY[riSRT].CheckAllStat(csNone) ;
        bool temp2 = !IO_GetX(xSRT_DropDetect) ;



        //sun                     //rezero시에 에러 나서 주석처리.
        //bool bPckrWorkOnStation = /*!AT_Complete(aiPCK_PckrDU , ccBwd) &&*/ //MT_CmprPos(miPCK_Y , PM.GetValue(miPCK_Y , pvPCK_YPlce));
        bool bPckrWorkOnStation = MT_GetCmdPos(miPCK_Y) < PM.GetValue(miPCK_Y , pvPCK_YZig); //자꾸 Y축 움직일때 살짝 감지 되어서 문제.


        for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
            //모르는 스트립에러.
            if( DM.ARAY[riSTN ].GetStat(0,i) == csNone &&  IO_GetX(xSRT_StnDetect1 + i) && !bPckrWorkOnStation) EM_SetErr(eiSTN_Unknwn ) ;

            //스트립 사라짐 에러.
            if( DM.ARAY[riSTN ].GetStat(0,i) != csNone && !IO_GetX(xSRT_StnDetect1 + i)) EM_SetErr(eiSTN_Dispr  ) ;

        }


        //
        //
        //


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

        case 10 : MoveActr(aiSRT_Grip1FB  , ccBwd);
                  MoveActr(aiSRT_Grip2FB  , ccBwd);
                  MoveActr(aiSRT_Grip3FB  , ccBwd);
                  MoveActr(aiSRT_FlipFB  , ccBwd);
                  MoveActr(aiSTN_GuidFB , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiSRT_Grip1FB  , ccBwd)) return false ;
                  if(!MoveActr(aiSRT_Grip2FB  , ccBwd)) return false ;
                  if(!MoveActr(aiSRT_Grip3FB  , ccBwd)) return false ;
                  if(!MoveActr(aiSRT_FlipFB  , ccBwd)) return false ;
                  MoveMotr(miSRT_X , piSRT_XPick);

                  Step.iCycle++;
                  return false ;

        case 12 : //UPH때문에 미리 보내 놓는다. 위험함..~~~~~~~~~
                  //if(MT_GetCmdPos(miSRT_X) < PM.GetValue(miSRT_X , pvSRT_XBin0)) MoveActr(aiSRT_FlipFB , ccFwd);


                  if(!MoveMotr(miSRT_X , piSRT_XPick)) return false ;
                  
                  


                  Step.iCycle++;
                  return false;

        case 13 : if(!MoveActr(aiSTN_GuidFB , ccFwd)) return false ;
                  MoveActr(aiSRT_FlipFB  , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 14:  if(!MoveActr(aiSRT_FlipFB  , ccFwd)) return false ;
                  MoveActr(aiSRT_Grip1FB  , ccFwd);
                  MoveActr(aiSRT_Grip2FB  , ccFwd);
                  MoveActr(aiSRT_Grip3FB  , ccFwd);

                  Step.iCycle++;
                  return false ;

        case 15:  if(!MoveActr(aiSRT_Grip1FB  , ccFwd)) return false ;
                  if(!MoveActr(aiSRT_Grip2FB  , ccFwd)) return false ;
                  if(!MoveActr(aiSRT_Grip3FB  , ccFwd)) return false ;


                  MoveActr(aiSRT_FlipFB  , ccBwd);
                  IO_SetY(ySRT_StnAir , true);
                  Step.iCycle++;
                  return false ;

        case 16:  if(!MoveActr(aiSRT_FlipFB  , ccBwd)) return false ;
                   //eiSRT_PickFail
                  IO_SetY(ySRT_StnAir , false);

                  if(DM.ARAY[riSTN].GetStat(0,0) != csNone && IO_GetX(xSRT_StnDetect1)) EM_SetErr(eiSRT_PickFail);
                  if(DM.ARAY[riSTN].GetStat(0,1) != csNone && IO_GetX(xSRT_StnDetect2)) EM_SetErr(eiSRT_PickFail);
                  if(DM.ARAY[riSTN].GetStat(0,2) != csNone && IO_GetX(xSRT_StnDetect3)) EM_SetErr(eiSRT_PickFail);

                  if(DM.ARAY[riSTN].GetStat(0,0) != csNone && !IO_GetX(xSRT_StnDetect1)) {DM.ARAY[riSRT].SetStat(0,0,DM.ARAY[riSTN].GetStat(0,0)) ; DM.ARAY[riSTN].SetStat(0,0,csNone) ; }
                  if(DM.ARAY[riSTN].GetStat(0,1) != csNone && !IO_GetX(xSRT_StnDetect2)) {DM.ARAY[riSRT].SetStat(0,1,DM.ARAY[riSTN].GetStat(0,1)) ; DM.ARAY[riSTN].SetStat(0,1,csNone) ; }
                  if(DM.ARAY[riSTN].GetStat(0,2) != csNone && !IO_GetX(xSRT_StnDetect3)) {DM.ARAY[riSRT].SetStat(0,2,DM.ARAY[riSTN].GetStat(0,2)) ; DM.ARAY[riSTN].SetStat(0,2,csNone) ; }

                  //DM.ShiftArrayData(riSTN , riSRT);

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
    static int iSorterIdx = 0 ;

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

        case 10 : //if(!MoveActr(aiSRT_Grip1FB  , ccFwd)) return false ;
                  //if(!MoveActr(aiSRT_Grip2FB  , ccFwd)) return false ;
                  //if(!MoveActr(aiSRT_Grip3FB  , ccFwd)) return false ;


                  MoveActr(aiSRT_FlipFB  , ccBwd);

                  Step.iCycle++;
                  return false ;

        case 11 : //if(!MoveActr(aiSRT_Grip  , ccFwd)) return false ;
                  if(!MoveActr(aiSRT_FlipFB  , ccBwd)) return false ;

                  //if(IO_GetX(xSRT_DropDetect)) {
                  //    EM_SetErr(eiSRT_DropCheck) ;
                  //    Step.iCycle=0;
                  //    return true ;
                  //}



                  /*
                  for(int i = 0 ; i < MAX_PICKER_CNT ; i++) {
                      if(DM.ARAY[riSRT].GetStat(0,i) != csNone) {
                          iSorterIdx = i ;
                          iBinNo = GetEmptyBin(DM.ARAY[riSRT].GetStat(0,i)) ;
                          break ;
                      }
                  }*/

                  //가장 가까운 작업거리 그리퍼를 받아서 작업.
                  iSorterIdx = GetWorkGripper() ;
                  iBinNo = GetEmptyBin(DM.ARAY[riSRT].GetStat(0,iSorterIdx)) ;

                  if(iSorterIdx < 0) {
                      EM_SetErr(eiSRT_NoBin) ;
                      Step.iCycle=0;
                      return true ;
                  }


                  //디바이스 옵션창에 선택되지 않은 빈임... 확인하고 세팅 해야 함.
                  if(iBinNo <0) {
                      EM_SetErr(eiSRT_NoBin) ;
                      Step.iCycle=0;
                      return true ;
                  }

                  if(iBinNo == 0 ) MoveMotr(miSRT_X , piSRT_XBin0 , iSorterIdx) ;
                  if(iBinNo == 1 ) MoveMotr(miSRT_X , piSRT_XBin1 , iSorterIdx) ;
                  if(iBinNo == 2 ) MoveMotr(miSRT_X , piSRT_XBin2 , iSorterIdx) ;
                  if(iBinNo == 3 ) MoveMotr(miSRT_X , piSRT_XBin3 , iSorterIdx) ;
                  if(iBinNo == 4 ) MoveMotr(miSRT_X , piSRT_XBin4 , iSorterIdx) ;
                  if(iBinNo == 5 ) MoveMotr(miSRT_X , piSRT_XBin5 , iSorterIdx) ;
                  if(iBinNo == 6 ) MoveMotr(miSRT_X , piSRT_XBin6 , iSorterIdx) ;
                  if(iBinNo == 7 ) MoveMotr(miSRT_X , piSRT_XBin7 , iSorterIdx) ;
                  if(iBinNo == 8 ) MoveMotr(miSRT_X , piSRT_XBin8 , iSorterIdx) ;
                  if(iBinNo == 9 ) MoveMotr(miSRT_X , piSRT_XBin9 , iSorterIdx) ;


                  //MoveMotr(miSRT_X , EN_PSTN_ID(piSRT_XLBn0 + iBinNo)) ;
                  Step.iCycle++;
                  return false ;


        case 12 : if(!MT_GetStop(miSRT_X)) return false ;

                  //MoveActr(aiSRT_GripFB  , ccBwd);
                  if(iSorterIdx == 0 )MoveActr(aiSRT_Grip1FB  , ccBwd);
                  if(iSorterIdx == 1 )MoveActr(aiSRT_Grip2FB  , ccBwd);
                  if(iSorterIdx == 2 )MoveActr(aiSRT_Grip3FB  , ccBwd);

                  if(iBinNo == 0) { m_iBinCnt[0] += 1;}
                  if(iBinNo == 1) { m_iBinCnt[1] += 1;}
                  if(iBinNo == 2) { m_iBinCnt[2] += 1;}
                  if(iBinNo == 3) { m_iBinCnt[3] += 1;}
                  if(iBinNo == 4) { m_iBinCnt[4] += 1;}
                  if(iBinNo == 5) { m_iBinCnt[5] += 1;}
                  if(iBinNo == 6) { m_iBinCnt[6] += 1;}
                  if(iBinNo == 7) { m_iBinCnt[7] += 1;}
                  if(iBinNo == 8) { m_iBinCnt[8] += 1;}
                  if(iBinNo == 9) { m_iBinCnt[9] += 1;}






                  /*
                  gsGradeA = csRslt0  , //  0
                  gsGradeB = csRslt1  , //  1
                  gsGradeC = csRslt2  , //  2
                  gsGradeD = csRslt3  , //  3
                  gsGradeE = csRslt4  , //  4
                  gsGradeF = csRslt5  , //  5
                  gsGradeG = csRslt6  , //  6
                  gsGradeH = csRslt7  , //  7
                  gsGradeI = csRslt8  , //  8
                  gsGradeJ = csRslt9  , //  9
                  MAX_OK_GRADE  ,

                  gsFail   = csRsltA ,  //  10 로터 설정 범위내에 없음.
                  gsCFail  = csRsltB ,  //  11 로터 안착 페일.
                  gsNotUse = csRsltC ,  //  12 사용안함.
                  */

                  //Lot Day Info Add
                  LT.AddLotInfoWorkChip(1);
                  LT.AddLotInfoFailChip(((DM.ARAY[riSRT].GetStat(0,iSorterIdx)==gsFail)  || (DM.ARAY[riSRT].GetStat(0,iSorterIdx)==gsCFail)) ? 1 : 0);  // 0 -> Fail
                  LT.AddLotInfoChips   (DM.ARAY[riSRT].GetStat(0,iSorterIdx)==gsGradeA ? 1 : 0 , //a Lot Unit 에서 저장 할때 0번을 안쓰네.
                                        DM.ARAY[riSRT].GetStat(0,iSorterIdx)==gsGradeB ? 1 : 0 , //b
                                        DM.ARAY[riSRT].GetStat(0,iSorterIdx)==gsGradeC ? 1 : 0 , //c
                                        DM.ARAY[riSRT].GetStat(0,iSorterIdx)==gsGradeD ? 1 : 0 , //d
                                        DM.ARAY[riSRT].GetStat(0,iSorterIdx)==gsGradeE ? 1 : 0 , //e
                                        DM.ARAY[riSRT].GetStat(0,iSorterIdx)==gsGradeF ? 1 : 0 );//f

                  LT.AddEqpInfoWorkChip(1);
                  LT.AddEqpInfoFailChip(DM.ARAY[riSRT].GetStat(0,iSorterIdx)==csRslt0 ? 1 : 0);

                  DM.ARAY[riSRT].SetStat(0,iSorterIdx, csNone) ;

                  Step.iCycle++;
                  return false ;

        case 13 : if(iSorterIdx == 0 && !MoveActr(aiSRT_Grip1FB  , ccBwd)) return false ;
                  if(iSorterIdx == 1 && !MoveActr(aiSRT_Grip2FB  , ccBwd)) return false ;
                  if(iSorterIdx == 2 && !MoveActr(aiSRT_Grip3FB  , ccBwd)) return false ;

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case 14 : if(!m_tmTemp.OnDelay(true , 100)) return false ; //자제떨어지는 동안 기다림.

                  if(DM.ARAY[riSRT].CheckAllStat(csNone)) MoveMotr(miSRT_X , piSRT_XPick);
                  m_tmTemp.Clear() ;
                  Step.iCycle++;
                  return false ;

        case 15 : //if(m_tmTemp.OnDelay(!OM.MstOptn.bDebugMode , 3000))EM_SetErr(eiSRT_DropFail) ;
                  //if(IO_GetX(xSRT_DropDetect)) return false ;
                  //피딩 페일인지 풀 페일인지 알수가 없네...dd

                  Step.iCycle++;
                  return false ;

        case 16 : if(!MT_GetStopInpos(miSRT_X)) return false ;
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

    if(!AT_Done(aiSRT_FlipFB  )) return false ;
    if(!AT_Done(aiSRT_Grip1FB )) return false ;
    if(!AT_Done(aiSRT_Grip2FB )) return false ;
    if(!AT_Done(aiSRT_Grip3FB )) return false ;


    return true ;
}

void CSorter::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    if(IsLoad) {
        for(int i = 0 ; i < MAX_BIN_CNT ; i++) {
            sTemp = "m_iBinCnt" + AnsiString(i) ;
            UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , sTemp.c_str() , m_iBinCnt[i]   );
        }
    }

    else {
        for(int i = 0 ; i < MAX_BIN_CNT ; i++) {
            sTemp = "m_iBinCnt" + AnsiString(i) ;
            UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , sTemp.c_str() , m_iBinCnt[i]   );
        }
    }
}

bool CSorter::GetLastStrip()
{
    return false ;
}

void CSorter::ClearBinCnt()
{
    memset(m_iBinCnt , 0 , sizeof(int) * MAX_BIN_CNT) ;

}

