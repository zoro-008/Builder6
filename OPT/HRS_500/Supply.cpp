//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Supply.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "OptionMan.h"
#include "PstnMan.h"
#include "UserIni.h"
#include "DataMan.h"
#include "Rs232Man.h"
#include "SPCUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
CSupply SPL;
CSupply SPR;

int CSupply::GetOpenBin(double _dVal)
{
    //오퍼가 측정한 값으로 범위 확인하여 빈결정
    int iBinSel ;
         if(OM.DevOptn.dMinBnS[0] <= _dVal && _dVal <= OM.DevOptn.dMaxBnS[0]) iBinSel = (int)gsGradeA ;
    else if(OM.DevOptn.dMinBnS[1] <= _dVal && _dVal <= OM.DevOptn.dMaxBnS[1]) iBinSel = (int)gsGradeB ;
    else if(OM.DevOptn.dMinBnS[2] <= _dVal && _dVal <= OM.DevOptn.dMaxBnS[2]) iBinSel = (int)gsGradeC ;
    else if(OM.DevOptn.dMinBnS[3] <= _dVal && _dVal <= OM.DevOptn.dMaxBnS[3]) iBinSel = (int)gsGradeD ;
    else if(OM.DevOptn.dMinBnS[4] <= _dVal && _dVal <= OM.DevOptn.dMaxBnS[4]) iBinSel = (int)gsGradeE ;
    else if(OM.DevOptn.dMinBnS[5] <= _dVal && _dVal <= OM.DevOptn.dMaxBnS[5]) iBinSel = (int)gsGradeF ;
    else if(OM.DevOptn.dMinBnS[6] <= _dVal && _dVal <= OM.DevOptn.dMaxBnS[6]) iBinSel = (int)gsGradeG ;
    else if(OM.DevOptn.dMinBnS[7] <= _dVal && _dVal <= OM.DevOptn.dMaxBnS[7]) iBinSel = (int)gsGradeH ;
    else if(OM.DevOptn.dMinBnS[8] <= _dVal && _dVal <= OM.DevOptn.dMaxBnS[8]) iBinSel = (int)gsGradeI ;
    else if(OM.DevOptn.dMinBnS[9] <= _dVal && _dVal <= OM.DevOptn.dMaxBnS[9]) iBinSel = (int)gsGradeJ ;
    else                                                                      iBinSel = -1 ; //Fail 오퍼 측정 페일일경우 그냥씹어야 하나?

    int iBinCnt [MAX_BIN_CNT] ;
    int iBinNo  [MAX_BIN_CNT] ;
    int iBBinCnt[MAX_BIN_CNT] ;
    int iBBinNo [MAX_BIN_CNT] ;

    memset(iBinCnt  , 0  , sizeof(int) * MAX_BIN_CNT) ;
    memset(iBinNo   , -1 , sizeof(int) * MAX_BIN_CNT) ;
    memset(iBBinCnt , 0  , sizeof(int) * MAX_BIN_CNT) ;
    memset(iBBinNo  , -1 , sizeof(int) * MAX_BIN_CNT) ;

    //GRADE BUFFER CNT
    if(m_sPartName == lrLeftBin) {
        for(int i=0;i<MAX_BIN_CNT;i++) {
            if(OM.DevOptn.iBnLIdx[i] == (int)iBinSel                                           ) {iBinCnt[i]  = GetPkgCnt(i); iBinNo[i]  = i ; }
            if(OM.DevOptn.iBnLIdx[i] == (int)gsBuffer && OM.DevOptn.iBnLSel[i] == (int)iBinSel ) {iBBinCnt[i] = GetPkgCnt(i); iBBinNo[i] = i ; }
        }
    }
    else {
        for(int i=0;i<MAX_BIN_CNT;i++) {
            if(OM.DevOptn.iBnRIdx[i] == (int)iBinSel                                           ) {iBinCnt[i]  = GetPkgCnt(i); iBinNo[i]  = i ; }
            if(OM.DevOptn.iBnRIdx[i] == (int)gsBuffer && OM.DevOptn.iBnRSel[i] == (int)iBinSel ) {iBBinCnt[i] = GetPkgCnt(i); iBBinNo[i] = i ; }
        }
    }

    int iBinTCnt  = 0 ;
    int iBBinTCnt = 0 ;

    int iMin        = OM.DevOptn.iBinMaxCnt + 1;
    int iBMin       = OM.DevOptn.iBinMaxCnt + 1;
    int iLastBinNo  = -1 ;
    int iLastBBinNo = -1 ;

    for(int i =0; i<MAX_BIN_CNT; i++) {
        iBinTCnt  += iBinCnt [i] ;
        iBBinTCnt += iBBinCnt[i] ;
        if(iBinCnt [i] != 0 && iBinCnt [i] < iMin  ) { iLastBinNo  = iBinNo [i] ; iMin  = iBinCnt [i] ; }
        if(iBBinCnt[i] != 0 && iBBinCnt[i] < iBMin ) { iLastBBinNo = iBBinNo[i] ; iBMin = iBBinCnt[i] ; }
    }

         if(iBinSel == -1                  ) return -1          ; //자재 측정범위에 없음.
    else if(iBinTCnt  <=0 && iBBinTCnt <= 0) return -2          ; //남아있는 자재가 없음.
    else if(iBBinTCnt  >0                  ) return iLastBBinNo ; //제일 조금 남아 있는 버퍼를 내보내고
    else if(iBinTCnt   >0                  ) return iLastBinNo  ; //그담에 GRADE
    else                                     return -1          ;

}

CSupply::CSupply(void)
{
    //SEQ에서 하게 함.... 아이디 세팅 때문에 그럼.
    //Init();
    m_iSetBin = -1 ;

    dIndicator = 0.0 ;
//    dRIndicator = 0.0 ;

/*
    Rs232_L = new TRS232C;
    Rs232_R = new TRS232C;

    if(!Rs232_L->Open(2)) ShowMessage("Second Rs232 Port Open Fail") ;
    else                  Rs232_L->CallBackReg(procCom2);
    if(!Rs232_R->Open(3)) ShowMessage("Third  Rs232 Port Open Fail") ;
    else                  Rs232_R->CallBackReg(procCom3);
*/
//ANA .....................dd

}

CSupply::~CSupply (void)
{

    Close();
}

void CSupply::Init(EN_LEFT_RIGHT _iPartId )
{
    if(_iPartId == lrLeftBin) {
        aiSPL_Bn0 = aiSPL_LBn0 ;  xSPL_Detect0 = xSPL_LDetect0 ;
        aiSPL_Bn1 = aiSPL_LBn1 ;  xSPL_Detect1 = xSPL_LDetect1 ;
        aiSPL_Bn2 = aiSPL_LBn2 ;  xSPL_Detect2 = xSPL_LDetect2 ;
        aiSPL_Bn3 = aiSPL_LBn3 ;  xSPL_Detect3 = xSPL_LDetect3 ;
        aiSPL_Bn4 = aiSPL_LBn4 ;  xSPL_Detect4 = xSPL_LDetect4 ;
        aiSPL_Bn5 = aiSPL_LBn5 ;  xSPL_Detect5 = xSPL_LDetect5 ;
        aiSPL_Bn6 = aiSPL_LBn6 ;  xSPL_Detect6 = xSPL_LDetect6 ;
        aiSPL_Bn7 = aiSPL_LBn7 ;  xSPL_Detect7 = xSPL_LDetect7 ;
        aiSPL_Bn8 = aiSPL_LBn8 ;  xSPL_Detect8 = xSPL_LDetect8 ;
        aiSPL_Bn9 = aiSPL_LBn9 ;  xSPL_Detect9 = xSPL_LDetect9 ;

        m_sPartName = "Left Bin " ;
    }

    else {
        aiSPL_Bn0 = aiSPL_RBn0 ;  xSPL_Detect0 = xSPL_RDetect0 ;
        aiSPL_Bn1 = aiSPL_RBn1 ;  xSPL_Detect1 = xSPL_RDetect1 ;
        aiSPL_Bn2 = aiSPL_RBn2 ;  xSPL_Detect2 = xSPL_RDetect2 ;
        aiSPL_Bn3 = aiSPL_RBn3 ;  xSPL_Detect3 = xSPL_RDetect3 ;
        aiSPL_Bn4 = aiSPL_RBn4 ;  xSPL_Detect4 = xSPL_RDetect4 ;
        aiSPL_Bn5 = aiSPL_RBn5 ;  xSPL_Detect5 = xSPL_RDetect5 ;
        aiSPL_Bn6 = aiSPL_RBn6 ;  xSPL_Detect6 = xSPL_RDetect6 ;
        aiSPL_Bn7 = aiSPL_RBn7 ;  xSPL_Detect7 = xSPL_RDetect7 ;
        aiSPL_Bn8 = aiSPL_RBn8 ;  xSPL_Detect8 = xSPL_RDetect8 ;
        aiSPL_Bn9 = aiSPL_RBn9 ;  xSPL_Detect9 = xSPL_RDetect9 ;

        m_sPartName = "Right Bin " ;
    }

    Reset();
    Load(true);
}

void CSupply::Close()
{
    Load(false);
}

void CSupply::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CSupply::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}



bool CSupply::FindChip(int &r , int &c )
{
    return false;
}

double CSupply::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    sMsg = MT_GetName(_iMotr) + " is Not this part Motr!" ;
    bRet = false ;

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk("CheckSafe",sMsg);
    }


    return false ;
}

//---------------------------------------------------------------------------
bool CSupply::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;


    sMsg = MT_GetName(_iMotr) + " is Not this part Motr!" ;
    bRet = false ;

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CSupply::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

         if(_iActr == aiSPL_Bn0){}
    else if(_iActr == aiSPL_Bn1){}
    else if(_iActr == aiSPL_Bn2){}
    else if(_iActr == aiSPL_Bn3){}
    else if(_iActr == aiSPL_Bn4){}
    else if(_iActr == aiSPL_Bn5){}
    else if(_iActr == aiSPL_Bn6){}
    else if(_iActr == aiSPL_Bn7){}
    else if(_iActr == aiSPL_Bn8){}
    else if(_iActr == aiSPL_Bn9){}
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

bool CSupply::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CSupply::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CSupply::CycleHome()
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

        case 10: AT_MoveCyl(aiSPL_Bn0 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn1 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn2 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn3 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn4 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn5 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn6 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn7 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn8 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn9 , ccBwd);

                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiSPL_Bn0 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn1 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn2 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn3 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn4 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn5 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn6 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn7 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn8 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn9 , ccBwd)) return false ;

                 Step.iHome = 0;
                 return true ;
    }
}

bool CSupply::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    //Clear Timer.
    Stat.bReqStop = true ;

    if (Step.iSeq ) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CSupply::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CSupply::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10: AT_MoveCyl(aiSPL_Bn0 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn1 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn2 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn3 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn4 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn5 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn6 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn7 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn8 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn9 , ccBwd);

                 Step.iToStart++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiSPL_Bn0 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn1 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn2 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn3 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn4 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn5 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn6 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn7 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn8 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn9 , ccBwd)) return false ;

                 Step.iToStart = 0;
                 return true ;
    }
}

bool CSupply::ToStop(void) //스탑을 하기 위한 함수.
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

        case 10: AT_MoveCyl(aiSPL_Bn0 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn1 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn2 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn3 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn4 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn5 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn6 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn7 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn8 , ccBwd);
                 AT_MoveCyl(aiSPL_Bn9 , ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiSPL_Bn0 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn1 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn2 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn3 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn4 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn5 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn6 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn7 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn8 , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiSPL_Bn9 , ccBwd)) return false ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CSupply::Autorun(void) //오토런닝시에 계속 타는 함수.
{
//    AnsiString sMsg = "PkgCnt Added , iPkgCnt is ";

    return true ;
/*
    if(IO_GetXUp(xSPL_Detect0)) {m_iPkgCnt[0]++; sMsg ="0 " + sMsg + AnsiString(m_iPkgCnt[0]) ;}
    if(IO_GetXUp(xSPL_Detect1)) {m_iPkgCnt[1]++; sMsg ="1 " + sMsg + AnsiString(m_iPkgCnt[1]) ;}
    if(IO_GetXUp(xSPL_Detect2)) {m_iPkgCnt[2]++; sMsg ="2 " + sMsg + AnsiString(m_iPkgCnt[2]) ;}
    if(IO_GetXUp(xSPL_Detect3)) {m_iPkgCnt[3]++; sMsg ="3 " + sMsg + AnsiString(m_iPkgCnt[3]) ;}
    if(IO_GetXUp(xSPL_Detect4)) {m_iPkgCnt[4]++; sMsg ="4 " + sMsg + AnsiString(m_iPkgCnt[4]) ;}
    if(IO_GetXUp(xSPL_Detect5)) {m_iPkgCnt[5]++; sMsg ="5 " + sMsg + AnsiString(m_iPkgCnt[5]) ;}
    if(IO_GetXUp(xSPL_Detect6)) {m_iPkgCnt[6]++; sMsg ="6 " + sMsg + AnsiString(m_iPkgCnt[6]) ;}
    if(IO_GetXUp(xSPL_Detect7)) {m_iPkgCnt[7]++; sMsg ="7 " + sMsg + AnsiString(m_iPkgCnt[7]) ;}
    if(IO_GetXUp(xSPL_Detect8)) {m_iPkgCnt[8]++; sMsg ="8 " + sMsg + AnsiString(m_iPkgCnt[8]) ;}
    if(IO_GetXUp(xSPL_Detect9)) {m_iPkgCnt[9]++; sMsg ="9 " + sMsg + AnsiString(m_iPkgCnt[9]) ;}

    Trace(m_sPartName.c_str(), sMsg.c_str());
    return true ;
*/
}

bool CSupply::Update(void) //평소에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        //if (Stat.bReqStop) return false ;
        bool isRcvValL = false ; //
        bool isRcvValR = false ; //
        int  iRcvBin ;
        String sMgz ;

        if(m_sPartName == "Left Bin "  && Rs232_L->sData != "" /*&& Rs232_L->IsOpened()*/) isRcvValL = true ;
        if(m_sPartName == "Right Bin " && Rs232_R->sData != "" /*&& Rs232_R->IsOpened()*/) isRcvValR = true ;

        if(isRcvValL) { //Option Mst Add
            dIndicator = StrToFloatDef(Rs232_L->sData.ToDouble(),0.0) ;

            if(GetOpenBin(dIndicator) == -1) {
                sMgz = "Left Side Not A Set Of Grades(" + String(dIndicator) + ")" ;
                FM_MsgTime ("Notification" , sMgz , OM.MstOptn.iMgzShowTime);
            }
            else if(GetOpenBin(dIndicator) == -2) {
                sMgz = "Left Side Not Have Rotors(" + String(dIndicator) + ")" ;
                FM_MsgTime ("Notification" , sMgz , OM.MstOptn.iMgzShowTime);

                m_iSetBin = GetOpenBin(dIndicator);
            }
            else {
                m_iSetBin = GetOpenBin(dIndicator);
                SPC.WriteLeftStatorData(dIndicator);
            }
            Rs232_L->sData = "" ;
        }
        if(isRcvValR) { //Option Mst Add
            dIndicator = StrToFloatDef(Rs232_R->sData.ToDouble(),0.0) ;
            if(GetOpenBin(dIndicator) == -1) {
                sMgz = "Right Side Not A Set Of Grades(" + String(dIndicator) + ")" ;
                FM_MsgTime ("Notification" , sMgz , OM.MstOptn.iMgzShowTime);
            }
            else if(GetOpenBin(dIndicator) == -2) {
                sMgz = "Right Side Not Have Rotors(" + String(dIndicator) + ")" ;
                FM_MsgTime ("Notification" , sMgz , OM.MstOptn.iMgzShowTime);
            }
            else {
                m_iSetBin = GetOpenBin(dIndicator);
                SPC.WriteRightStatorData(dIndicator);                
            }
            Rs232_R->sData = "" ;
        }


        bool isCycleSupply    = m_iSetBin > -1 && m_iSetBin < MAX_BIN_CNT;
        bool isCycleSupplyAll = m_bSetSupplyAll ;
        bool isConEnd         = !isCycleSupply ;

        if(isConEnd)m_iSetBin = -1;

        //모르는 스트립에러.
        //스트립 사라짐 에러.

        //if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleSupply   ) {Trace(m_sPartName.c_str(),"CycleSupply    Stt"); Step.iSeq = scSupply    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} 
        else if (isCycleSupplyAll) {Trace(m_sPartName.c_str(),"CycleSupplyAll Stt"); Step.iSeq = scSupplyAll ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;}
        else if (isConEnd        ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default        :                    Trace(m_sPartName.c_str(),"default     End");Step.iSeq = scIdle ;  return false ;
        case  scIdle   :                                                                                       return false ;

        case  scSupply    : if(CycleSupply   ()){ Trace(m_sPartName.c_str(),"CycleSupply End");Step.iSeq = scIdle ;} return false ;
        case  scSupplyAll : if(CycleSupplyAll()){ Trace(m_sPartName.c_str(),"CycleSupply End");Step.iSeq = scIdle ;} return false ;
    }
}

bool CSupply::CycleSupply(void) //
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

        case 10 :
                  if(m_iSetBin == 0) MoveActr(aiSPL_Bn0  , ccBwd);
                  if(m_iSetBin == 1) MoveActr(aiSPL_Bn1  , ccBwd);
                  if(m_iSetBin == 2) MoveActr(aiSPL_Bn2  , ccBwd);
                  if(m_iSetBin == 3) MoveActr(aiSPL_Bn3  , ccBwd);
                  if(m_iSetBin == 4) MoveActr(aiSPL_Bn4  , ccBwd);
                  if(m_iSetBin == 5) MoveActr(aiSPL_Bn5  , ccBwd);
                  if(m_iSetBin == 6) MoveActr(aiSPL_Bn6  , ccBwd);
                  if(m_iSetBin == 7) MoveActr(aiSPL_Bn7  , ccBwd);
                  if(m_iSetBin == 8) MoveActr(aiSPL_Bn8  , ccBwd);
                  if(m_iSetBin == 9) MoveActr(aiSPL_Bn9  , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 11 : if(m_iSetBin == 0 && !MoveActr(aiSPL_Bn0  , ccBwd))return false ;
                  if(m_iSetBin == 1 && !MoveActr(aiSPL_Bn1  , ccBwd))return false ;
                  if(m_iSetBin == 2 && !MoveActr(aiSPL_Bn2  , ccBwd))return false ;
                  if(m_iSetBin == 3 && !MoveActr(aiSPL_Bn3  , ccBwd))return false ;
                  if(m_iSetBin == 4 && !MoveActr(aiSPL_Bn4  , ccBwd))return false ;
                  if(m_iSetBin == 5 && !MoveActr(aiSPL_Bn5  , ccBwd))return false ;
                  if(m_iSetBin == 6 && !MoveActr(aiSPL_Bn6  , ccBwd))return false ;
                  if(m_iSetBin == 7 && !MoveActr(aiSPL_Bn7  , ccBwd))return false ;
                  if(m_iSetBin == 8 && !MoveActr(aiSPL_Bn8  , ccBwd))return false ;
                  if(m_iSetBin == 9 && !MoveActr(aiSPL_Bn9  , ccBwd))return false ;

                  Step.iCycle++;
                  return false ;

        case 12 : //if(m_iSetBin == 0 && !IO_GetX(xSPL_Detect0) ) {FM_MsgOk("ERROR","0 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 1 && !IO_GetX(xSPL_Detect1) ) {FM_MsgOk("ERROR","1 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 2 && !IO_GetX(xSPL_Detect2) ) {FM_MsgOk("ERROR","2 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 3 && !IO_GetX(xSPL_Detect3) ) {FM_MsgOk("ERROR","3 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 4 && !IO_GetX(xSPL_Detect4) ) {FM_MsgOk("ERROR","4 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 5 && !IO_GetX(xSPL_Detect5) ) {FM_MsgOk("ERROR","5 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 6 && !IO_GetX(xSPL_Detect6) ) {FM_MsgOk("ERROR","6 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 7 && !IO_GetX(xSPL_Detect7) ) {FM_MsgOk("ERROR","7 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 8 && !IO_GetX(xSPL_Detect8) ) {FM_MsgOk("ERROR","8 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 9 && !IO_GetX(xSPL_Detect9) ) {FM_MsgOk("ERROR","9 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  Step.iCycle++;
                  return false ;

        case 13:  if(m_iSetBin == 0) MoveActr(aiSPL_Bn0  , ccFwd);
                  if(m_iSetBin == 1) MoveActr(aiSPL_Bn1  , ccFwd);
                  if(m_iSetBin == 2) MoveActr(aiSPL_Bn2  , ccFwd);
                  if(m_iSetBin == 3) MoveActr(aiSPL_Bn3  , ccFwd);
                  if(m_iSetBin == 4) MoveActr(aiSPL_Bn4  , ccFwd);
                  if(m_iSetBin == 5) MoveActr(aiSPL_Bn5  , ccFwd);
                  if(m_iSetBin == 6) MoveActr(aiSPL_Bn6  , ccFwd);
                  if(m_iSetBin == 7) MoveActr(aiSPL_Bn7  , ccFwd);
                  if(m_iSetBin == 8) MoveActr(aiSPL_Bn8  , ccFwd);
                  if(m_iSetBin == 9) MoveActr(aiSPL_Bn9  , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 14:  if(m_iSetBin == 0 && !MoveActr(aiSPL_Bn0  , ccFwd))return false ;
                  if(m_iSetBin == 1 && !MoveActr(aiSPL_Bn1  , ccFwd))return false ;
                  if(m_iSetBin == 2 && !MoveActr(aiSPL_Bn2  , ccFwd))return false ;
                  if(m_iSetBin == 3 && !MoveActr(aiSPL_Bn3  , ccFwd))return false ;
                  if(m_iSetBin == 4 && !MoveActr(aiSPL_Bn4  , ccFwd))return false ;
                  if(m_iSetBin == 5 && !MoveActr(aiSPL_Bn5  , ccFwd))return false ;
                  if(m_iSetBin == 6 && !MoveActr(aiSPL_Bn6  , ccFwd))return false ;
                  if(m_iSetBin == 7 && !MoveActr(aiSPL_Bn7  , ccFwd))return false ;
                  if(m_iSetBin == 8 && !MoveActr(aiSPL_Bn8  , ccFwd))return false ;
                  if(m_iSetBin == 9 && !MoveActr(aiSPL_Bn9  , ccFwd))return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case 15:  if(!m_tmTemp.OnDelay(true,300)) return false ;
                  //if(m_iSetBin == 0 && IO_GetX(xSPL_Detect0)) {FM_MsgOk("ERROR","0 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn0  , ccBwd);return true ;}
                  //if(m_iSetBin == 1 && IO_GetX(xSPL_Detect1)) {FM_MsgOk("ERROR","1 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn1  , ccBwd);return true ;}
                  //if(m_iSetBin == 2 && IO_GetX(xSPL_Detect2)) {FM_MsgOk("ERROR","2 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn2  , ccBwd);return true ;}
                  //if(m_iSetBin == 3 && IO_GetX(xSPL_Detect3)) {FM_MsgOk("ERROR","3 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn3  , ccBwd);return true ;}
                  //if(m_iSetBin == 4 && IO_GetX(xSPL_Detect4)) {FM_MsgOk("ERROR","4 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn4  , ccBwd);return true ;}
                  //if(m_iSetBin == 5 && IO_GetX(xSPL_Detect5)) {FM_MsgOk("ERROR","5 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn5  , ccBwd);return true ;}
                  //if(m_iSetBin == 6 && IO_GetX(xSPL_Detect6)) {FM_MsgOk("ERROR","6 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn6  , ccBwd);return true ;}
                  //if(m_iSetBin == 7 && IO_GetX(xSPL_Detect7)) {FM_MsgOk("ERROR","7 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn7  , ccBwd);return true ;}
                  //if(m_iSetBin == 8 && IO_GetX(xSPL_Detect8)) {FM_MsgOk("ERROR","8 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn8  , ccBwd);return true ;}
                  //if(m_iSetBin == 9 && IO_GetX(xSPL_Detect9)) {FM_MsgOk("ERROR","9 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn9  , ccBwd);return true ;}
                  Step.iCycle++;
                  return false ;

        case 16:  if(m_iSetBin == 0) MoveActr(aiSPL_Bn0  , ccBwd);
                  if(m_iSetBin == 1) MoveActr(aiSPL_Bn1  , ccBwd);
                  if(m_iSetBin == 2) MoveActr(aiSPL_Bn2  , ccBwd);
                  if(m_iSetBin == 3) MoveActr(aiSPL_Bn3  , ccBwd);
                  if(m_iSetBin == 4) MoveActr(aiSPL_Bn4  , ccBwd);
                  if(m_iSetBin == 5) MoveActr(aiSPL_Bn5  , ccBwd);
                  if(m_iSetBin == 6) MoveActr(aiSPL_Bn6  , ccBwd);
                  if(m_iSetBin == 7) MoveActr(aiSPL_Bn7  , ccBwd);
                  if(m_iSetBin == 8) MoveActr(aiSPL_Bn8  , ccBwd);
                  if(m_iSetBin == 9) MoveActr(aiSPL_Bn9  , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 17 : if(m_iSetBin == 0 && !MoveActr(aiSPL_Bn0  , ccBwd))return false ;
                  if(m_iSetBin == 1 && !MoveActr(aiSPL_Bn1  , ccBwd))return false ;
                  if(m_iSetBin == 2 && !MoveActr(aiSPL_Bn2  , ccBwd))return false ;
                  if(m_iSetBin == 3 && !MoveActr(aiSPL_Bn3  , ccBwd))return false ;
                  if(m_iSetBin == 4 && !MoveActr(aiSPL_Bn4  , ccBwd))return false ;
                  if(m_iSetBin == 5 && !MoveActr(aiSPL_Bn5  , ccBwd))return false ;
                  if(m_iSetBin == 6 && !MoveActr(aiSPL_Bn6  , ccBwd))return false ;
                  if(m_iSetBin == 7 && !MoveActr(aiSPL_Bn7  , ccBwd))return false ;
                  if(m_iSetBin == 8 && !MoveActr(aiSPL_Bn8  , ccBwd))return false ;
                  if(m_iSetBin == 9 && !MoveActr(aiSPL_Bn9  , ccBwd))return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case 18 : if(!m_tmTemp.OnDelay(true,300)) return false ; //TODO : 다음 자제가 내려와서 센싱이 되어야 한다. 에러처리는 좀더 확인하고 넣자.


                  if(m_bSetAll) {
                      //모두 배출시에 마지막 자제 이면 공급 중단 .. 하지만 상황을 봐봐야 암.
                      if(m_iSetBin == 0){
                          if(!IO_GetX(xSPL_Detect0)) {m_bSetAll = false ; m_iPkgCnt[m_iSetBin] = 0 ; m_iSetBin = -1 ;}
                          else                       {if(m_iPkgCnt[m_iSetBin]>0)m_iPkgCnt[m_iSetBin]-- ;      }
                      }
                      if(m_iSetBin == 1){
                          if(!IO_GetX(xSPL_Detect1)) {m_bSetAll = false ; m_iPkgCnt[m_iSetBin] = 0 ; m_iSetBin = -1 ;}
                          else                       {if(m_iPkgCnt[m_iSetBin]>0)m_iPkgCnt[m_iSetBin]-- ;      }
                      }
                      if(m_iSetBin == 2){
                          if(!IO_GetX(xSPL_Detect2)) {m_bSetAll = false ; m_iPkgCnt[m_iSetBin] = 0 ; m_iSetBin = -1 ;}
                          else                       {if(m_iPkgCnt[m_iSetBin]>0)m_iPkgCnt[m_iSetBin]-- ;      }
                      }
                      if(m_iSetBin == 3){
                          if(!IO_GetX(xSPL_Detect3)) {m_bSetAll = false ; m_iPkgCnt[m_iSetBin] = 0 ; m_iSetBin = -1 ;}
                          else                       {if(m_iPkgCnt[m_iSetBin]>0)m_iPkgCnt[m_iSetBin]-- ;      }
                      }
                      if(m_iSetBin == 4){
                          if(!IO_GetX(xSPL_Detect4)) {m_bSetAll = false ; m_iPkgCnt[m_iSetBin] = 0 ; m_iSetBin = -1 ;}
                          else                       {if(m_iPkgCnt[m_iSetBin]>0)m_iPkgCnt[m_iSetBin]-- ;      }
                      }
                      if(m_iSetBin == 5){
                          if(!IO_GetX(xSPL_Detect5)) {m_bSetAll = false ; m_iPkgCnt[m_iSetBin] = 0 ; m_iSetBin = -1 ;}
                          else                       {if(m_iPkgCnt[m_iSetBin]>0)m_iPkgCnt[m_iSetBin]-- ;      }
                      }
                      if(m_iSetBin == 6){
                          if(!IO_GetX(xSPL_Detect6)) {m_bSetAll = false ; m_iPkgCnt[m_iSetBin] = 0 ; m_iSetBin = -1 ;}
                          else                       {if(m_iPkgCnt[m_iSetBin]>0)m_iPkgCnt[m_iSetBin]-- ;      }
                      }
                      if(m_iSetBin == 7){
                          if(!IO_GetX(xSPL_Detect7)) {m_bSetAll = false ; m_iPkgCnt[m_iSetBin] = 0 ; m_iSetBin = -1 ;}
                          else                       {if(m_iPkgCnt[m_iSetBin]>0)m_iPkgCnt[m_iSetBin]-- ;      }
                      }
                      if(m_iSetBin == 8){
                          if(!IO_GetX(xSPL_Detect8)) {m_bSetAll = false ; m_iPkgCnt[m_iSetBin] = 0 ; m_iSetBin = -1 ;}
                          else                       {if(m_iPkgCnt[m_iSetBin]>0)m_iPkgCnt[m_iSetBin]-- ;      }
                      }
                      if(m_iSetBin == 9){
                          if(!IO_GetX(xSPL_Detect9)) {m_bSetAll = false ; m_iPkgCnt[m_iSetBin] = 0 ; m_iSetBin = -1 ;}
                          else                       {if(m_iPkgCnt[m_iSetBin]>0)m_iPkgCnt[m_iSetBin]-- ;      }
                      }
                      //if(m_iSetBin == 1 && !IO_GetX(xSPL_Detect1)) {m_bSetAll = false ; m_iSetBin = -1 ; m_iPkgCnt[1] = 0 ; }
                      //if(m_iSetBin == 2 && !IO_GetX(xSPL_Detect2)) {m_bSetAll = false ; m_iSetBin = -1 ; m_iPkgCnt[2] = 0 ; }
                      //if(m_iSetBin == 3 && !IO_GetX(xSPL_Detect3)) {m_bSetAll = false ; m_iSetBin = -1 ; m_iPkgCnt[3] = 0 ; }
                      //if(m_iSetBin == 4 && !IO_GetX(xSPL_Detect4)) {m_bSetAll = false ; m_iSetBin = -1 ; m_iPkgCnt[4] = 0 ; }
                      //if(m_iSetBin == 5 && !IO_GetX(xSPL_Detect5)) {m_bSetAll = false ; m_iSetBin = -1 ; m_iPkgCnt[5] = 0 ; }
                      //if(m_iSetBin == 6 && !IO_GetX(xSPL_Detect6)) {m_bSetAll = false ; m_iSetBin = -1 ; m_iPkgCnt[6] = 0 ; }
                      //if(m_iSetBin == 7 && !IO_GetX(xSPL_Detect7)) {m_bSetAll = false ; m_iSetBin = -1 ; m_iPkgCnt[7] = 0 ; }
                      //if(m_iSetBin == 8 && !IO_GetX(xSPL_Detect8)) {m_bSetAll = false ; m_iSetBin = -1 ; m_iPkgCnt[8] = 0 ; }
                      //if(m_iSetBin == 9 && !IO_GetX(xSPL_Detect9)) {m_bSetAll = false ; m_iSetBin = -1 ; m_iPkgCnt[9] = 0 ; }
                  }
                  else {
                      if(m_iPkgCnt[m_iSetBin]>0)m_iPkgCnt[m_iSetBin]-- ;
                      m_iSetBin = -1 ;
                  }

                  //모두 배출중이면.
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CSupply::CycleSupplyAll(void) //
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

        case 10 : m_bSetSupplyAll = false ;
                  m_bOpenBin[0] = IO_GetX(xSPL_Detect0) ;
                  m_bOpenBin[1] = IO_GetX(xSPL_Detect1) ;
                  m_bOpenBin[2] = IO_GetX(xSPL_Detect2) ;
                  m_bOpenBin[3] = IO_GetX(xSPL_Detect3) ;
                  m_bOpenBin[4] = IO_GetX(xSPL_Detect4) ;
                  m_bOpenBin[5] = IO_GetX(xSPL_Detect5) ;
                  m_bOpenBin[6] = IO_GetX(xSPL_Detect6) ;
                  m_bOpenBin[7] = IO_GetX(xSPL_Detect7) ;
                  m_bOpenBin[8] = IO_GetX(xSPL_Detect8) ;
                  m_bOpenBin[9] = IO_GetX(xSPL_Detect9) ;

                  if(m_bOpenBin[0]) MoveActr(aiSPL_Bn0  , ccBwd);
                  if(m_bOpenBin[1]) MoveActr(aiSPL_Bn1  , ccBwd);
                  if(m_bOpenBin[2]) MoveActr(aiSPL_Bn2  , ccBwd);
                  if(m_bOpenBin[3]) MoveActr(aiSPL_Bn3  , ccBwd);
                  if(m_bOpenBin[4]) MoveActr(aiSPL_Bn4  , ccBwd);
                  if(m_bOpenBin[5]) MoveActr(aiSPL_Bn5  , ccBwd);
                  if(m_bOpenBin[6]) MoveActr(aiSPL_Bn6  , ccBwd);
                  if(m_bOpenBin[7]) MoveActr(aiSPL_Bn7  , ccBwd);
                  if(m_bOpenBin[8]) MoveActr(aiSPL_Bn8  , ccBwd);
                  if(m_bOpenBin[9]) MoveActr(aiSPL_Bn9  , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 11 : if(m_bOpenBin[0] && !MoveActr(aiSPL_Bn0  , ccBwd))return false ;
                  if(m_bOpenBin[1] && !MoveActr(aiSPL_Bn1  , ccBwd))return false ;
                  if(m_bOpenBin[2] && !MoveActr(aiSPL_Bn2  , ccBwd))return false ;
                  if(m_bOpenBin[3] && !MoveActr(aiSPL_Bn3  , ccBwd))return false ;
                  if(m_bOpenBin[4] && !MoveActr(aiSPL_Bn4  , ccBwd))return false ;
                  if(m_bOpenBin[5] && !MoveActr(aiSPL_Bn5  , ccBwd))return false ;
                  if(m_bOpenBin[6] && !MoveActr(aiSPL_Bn6  , ccBwd))return false ;
                  if(m_bOpenBin[7] && !MoveActr(aiSPL_Bn7  , ccBwd))return false ;
                  if(m_bOpenBin[8] && !MoveActr(aiSPL_Bn8  , ccBwd))return false ;
                  if(m_bOpenBin[9] && !MoveActr(aiSPL_Bn9  , ccBwd))return false ;

                  Step.iCycle++;
                  return false ;

        case 12 : //if(m_iSetBin == 0 && !IO_GetX(xSPL_Detect0) ) {FM_MsgOk("ERROR","0 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 1 && !IO_GetX(xSPL_Detect1) ) {FM_MsgOk("ERROR","1 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 2 && !IO_GetX(xSPL_Detect2) ) {FM_MsgOk("ERROR","2 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 3 && !IO_GetX(xSPL_Detect3) ) {FM_MsgOk("ERROR","3 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 4 && !IO_GetX(xSPL_Detect4) ) {FM_MsgOk("ERROR","4 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 5 && !IO_GetX(xSPL_Detect5) ) {FM_MsgOk("ERROR","5 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 6 && !IO_GetX(xSPL_Detect6) ) {FM_MsgOk("ERROR","6 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 7 && !IO_GetX(xSPL_Detect7) ) {FM_MsgOk("ERROR","7 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 8 && !IO_GetX(xSPL_Detect8) ) {FM_MsgOk("ERROR","8 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  //if(m_iSetBin == 9 && !IO_GetX(xSPL_Detect9) ) {FM_MsgOk("ERROR","9 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;return true ;}
                  Step.iCycle++;
                  return false ;

        case 13:  if(m_bOpenBin[0]) MoveActr(aiSPL_Bn0  , ccFwd);
                  if(m_bOpenBin[1]) MoveActr(aiSPL_Bn1  , ccFwd);
                  if(m_bOpenBin[2]) MoveActr(aiSPL_Bn2  , ccFwd);
                  if(m_bOpenBin[3]) MoveActr(aiSPL_Bn3  , ccFwd);
                  if(m_bOpenBin[4]) MoveActr(aiSPL_Bn4  , ccFwd);
                  if(m_bOpenBin[5]) MoveActr(aiSPL_Bn5  , ccFwd);
                  if(m_bOpenBin[6]) MoveActr(aiSPL_Bn6  , ccFwd);
                  if(m_bOpenBin[7]) MoveActr(aiSPL_Bn7  , ccFwd);
                  if(m_bOpenBin[8]) MoveActr(aiSPL_Bn8  , ccFwd);
                  if(m_bOpenBin[9]) MoveActr(aiSPL_Bn9  , ccFwd);
                  Step.iCycle++;
                  return false ;

        case 14:  if(m_bOpenBin[0] && !MoveActr(aiSPL_Bn0  , ccFwd))return false ;
                  if(m_bOpenBin[1] && !MoveActr(aiSPL_Bn1  , ccFwd))return false ;
                  if(m_bOpenBin[2] && !MoveActr(aiSPL_Bn2  , ccFwd))return false ;
                  if(m_bOpenBin[3] && !MoveActr(aiSPL_Bn3  , ccFwd))return false ;
                  if(m_bOpenBin[4] && !MoveActr(aiSPL_Bn4  , ccFwd))return false ;
                  if(m_bOpenBin[5] && !MoveActr(aiSPL_Bn5  , ccFwd))return false ;
                  if(m_bOpenBin[6] && !MoveActr(aiSPL_Bn6  , ccFwd))return false ;
                  if(m_bOpenBin[7] && !MoveActr(aiSPL_Bn7  , ccFwd))return false ;
                  if(m_bOpenBin[8] && !MoveActr(aiSPL_Bn8  , ccFwd))return false ;
                  if(m_bOpenBin[9] && !MoveActr(aiSPL_Bn9  , ccFwd))return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case 15:  if(!m_tmTemp.OnDelay(true,100)) return false ;
                  //if(m_iSetBin == 0 && IO_GetX(xSPL_Detect0)) {FM_MsgOk("ERROR","0 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn0  , ccBwd);return true ;}
                  //if(m_iSetBin == 1 && IO_GetX(xSPL_Detect1)) {FM_MsgOk("ERROR","1 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn1  , ccBwd);return true ;}
                  //if(m_iSetBin == 2 && IO_GetX(xSPL_Detect2)) {FM_MsgOk("ERROR","2 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn2  , ccBwd);return true ;}
                  //if(m_iSetBin == 3 && IO_GetX(xSPL_Detect3)) {FM_MsgOk("ERROR","3 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn3  , ccBwd);return true ;}
                  //if(m_iSetBin == 4 && IO_GetX(xSPL_Detect4)) {FM_MsgOk("ERROR","4 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn4  , ccBwd);return true ;}
                  //if(m_iSetBin == 5 && IO_GetX(xSPL_Detect5)) {FM_MsgOk("ERROR","5 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn5  , ccBwd);return true ;}
                  //if(m_iSetBin == 6 && IO_GetX(xSPL_Detect6)) {FM_MsgOk("ERROR","6 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn6  , ccBwd);return true ;}
                  //if(m_iSetBin == 7 && IO_GetX(xSPL_Detect7)) {FM_MsgOk("ERROR","7 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn7  , ccBwd);return true ;}
                  //if(m_iSetBin == 8 && IO_GetX(xSPL_Detect8)) {FM_MsgOk("ERROR","8 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn8  , ccBwd);return true ;}
                  //if(m_iSetBin == 9 && IO_GetX(xSPL_Detect9)) {FM_MsgOk("ERROR","9 Supplyer Feeding Error!"); Step.iCycle=0; m_iSetBin = -1 ;MoveActr(aiSPL_Bn9  , ccBwd);return true ;}
                  Step.iCycle++;
                  return false ;

        case 16:  if(m_bOpenBin[0]) MoveActr(aiSPL_Bn0  , ccBwd);
                  if(m_bOpenBin[1]) MoveActr(aiSPL_Bn1  , ccBwd);
                  if(m_bOpenBin[2]) MoveActr(aiSPL_Bn2  , ccBwd);
                  if(m_bOpenBin[3]) MoveActr(aiSPL_Bn3  , ccBwd);
                  if(m_bOpenBin[4]) MoveActr(aiSPL_Bn4  , ccBwd);
                  if(m_bOpenBin[5]) MoveActr(aiSPL_Bn5  , ccBwd);
                  if(m_bOpenBin[6]) MoveActr(aiSPL_Bn6  , ccBwd);
                  if(m_bOpenBin[7]) MoveActr(aiSPL_Bn7  , ccBwd);
                  if(m_bOpenBin[8]) MoveActr(aiSPL_Bn8  , ccBwd);
                  if(m_bOpenBin[9]) MoveActr(aiSPL_Bn9  , ccBwd);
                  Step.iCycle++;
                  return false ;

        case 17 : if(m_bOpenBin[0] && !MoveActr(aiSPL_Bn0  , ccBwd))return false ;
                  if(m_bOpenBin[1] && !MoveActr(aiSPL_Bn1  , ccBwd))return false ;
                  if(m_bOpenBin[2] && !MoveActr(aiSPL_Bn2  , ccBwd))return false ;
                  if(m_bOpenBin[3] && !MoveActr(aiSPL_Bn3  , ccBwd))return false ;
                  if(m_bOpenBin[4] && !MoveActr(aiSPL_Bn4  , ccBwd))return false ;
                  if(m_bOpenBin[5] && !MoveActr(aiSPL_Bn5  , ccBwd))return false ;
                  if(m_bOpenBin[6] && !MoveActr(aiSPL_Bn6  , ccBwd))return false ;
                  if(m_bOpenBin[7] && !MoveActr(aiSPL_Bn7  , ccBwd))return false ;
                  if(m_bOpenBin[8] && !MoveActr(aiSPL_Bn8  , ccBwd))return false ;
                  if(m_bOpenBin[9] && !MoveActr(aiSPL_Bn9  , ccBwd))return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case 18 : if(!m_tmTemp.OnDelay(true,300)) return false ; //TODO : 다음 자제가 내려와서 센싱이 되어야 한다. 에러처리는 좀더 확인하고 넣자.
                  if(IO_GetX(xSPL_Detect0)) {Step.iCycle = 10 ; return false ;}
                  if(IO_GetX(xSPL_Detect1)) {Step.iCycle = 10 ; return false ;}
                  if(IO_GetX(xSPL_Detect2)) {Step.iCycle = 10 ; return false ;}
                  if(IO_GetX(xSPL_Detect3)) {Step.iCycle = 10 ; return false ;}
                  if(IO_GetX(xSPL_Detect4)) {Step.iCycle = 10 ; return false ;}
                  if(IO_GetX(xSPL_Detect5)) {Step.iCycle = 10 ; return false ;}
                  if(IO_GetX(xSPL_Detect6)) {Step.iCycle = 10 ; return false ;}
                  if(IO_GetX(xSPL_Detect7)) {Step.iCycle = 10 ; return false ;}
                  if(IO_GetX(xSPL_Detect8)) {Step.iCycle = 10 ; return false ;}
                  if(IO_GetX(xSPL_Detect9)) {Step.iCycle = 10 ; return false ;}

                  for(int i = 0 ; i < MAX_BIN_CNT ; i++) {
                      m_iPkgCnt[i]=0;
                  }
                  Step.iCycle = 0 ;
                  return true ;
    }
}

void CSupply::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CSupply::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CSupply::CheckStop()
{
    if(!AT_Done(aiSPL_Bn0 )) return false ;
    if(!AT_Done(aiSPL_Bn1 )) return false ;
    if(!AT_Done(aiSPL_Bn2 )) return false ;
    if(!AT_Done(aiSPL_Bn3 )) return false ;
    if(!AT_Done(aiSPL_Bn4 )) return false ;
    if(!AT_Done(aiSPL_Bn5 )) return false ;
    if(!AT_Done(aiSPL_Bn6 )) return false ;
    if(!AT_Done(aiSPL_Bn7 )) return false ;
    if(!AT_Done(aiSPL_Bn8 )) return false ;
    if(!AT_Done(aiSPL_Bn9 )) return false ;

    return true ;
}

void CSupply::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    if(IsLoad) {
        for(int i = 0 ; i < MAX_BIN_CNT ; i++) {
            UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_iPkgCnt"+AnsiString(i) , m_iPkgCnt[i]);
        }
    }

    else {
        for(int i = 0 ; i < MAX_BIN_CNT ; i++) {
            UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_iPkgCnt"+AnsiString(i) , m_iPkgCnt[i]);
        }
    }
}

bool CSupply::GetLastStrip()
{
    return false ;
}

