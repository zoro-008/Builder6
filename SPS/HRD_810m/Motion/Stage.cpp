//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Stage.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
#include "MapUnit.h"
//#include "SVMaths.h"
#include "ManualMan.h"


//---------------------------------------------------------------------------
//#include <math.hpp>
//#include "math.h"
#include "Head.h"

#include "FormMain.h"//메세지 이용해서 접근.

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
CStage STG;

CStage::CStage()
{
    Init();

}

//---------------------------------------------------------------------------
CStage::~CStage (void)
{
    Close();
}

//---------------------------------------------------------------------------
void CStage::Init()
{
    m_sPartName   = "Stage " ;

    InitCycleName();

    m_iHomeTO  = (int)eiSTG_HomeTO    ;
    m_iStartTO = (int)eiSTG_ToStartTO ;
    m_iStopTO  = (int)eiSTG_ToStopTO  ;
    m_iCycleTO = (int)eiSTG_CycleTO   ;

    Reset();
    Load(true);

    memset(&Time    , 0 , sizeof(SCycleTime) * MAX_SEQ_CYCLE) ;

    Barcode = new TRS232C ();

    if(!Barcode->Open(1)) ShowMessage("Barcode Rs232 Port Open Fail") ;
    else                  Barcode->CallBackReg(OnReadBarcode);


}

//---------------------------------------------------------------------------
void CStage::Close()
{
    Barcode -> Close();
    delete Barcode ;
    Load(false);
}

void CStage::SendBarcodeMsg(AnsiString _sMsg)
{
    Barcode->sData = "";

    Barcode->SendData(_sMsg.Length(),_sMsg.c_str());
}

void CStage::OnReadBarcode(DWORD _cbInQue)
{


    static AnsiString sRcvMsg = "" ;
    String sTemp ;
    double dTemp = 0.0 ;

    BYTE RcvBuff[300];
    memset(&RcvBuff, 0 , sizeof(RcvBuff));
    STG.Barcode->ReadData(_cbInQue, RcvBuff);
    sRcvMsg += (char*)RcvBuff ;



    STG.Barcode->sData += sRcvMsg ;

    sRcvMsg  = "" ;

    Trace("Barcode",STG.Barcode->sData.c_str());

//    FrmMaster->mmRs232->Lines->Add("Resut" + Rs232_L->sData);

}

AnsiString CStage::GetWaferId(AnsiString _sBarcodeData)
{
    AnsiString sId1, sId2, sId3, sId;

    Trace("Barcode", _sBarcodeData.c_str());

    if(!OM.CmnOptn.bUse1stBarcode){
        //_sBarcodeData.Delete(1,_sBarcodeData.Pos(","));
        sId1 = _sBarcodeData.SubString(1, _sBarcodeData.Pos(",")-1);  // 20131227 바코드 불러 오기 실패해서 수정함...
        sId2 = _sBarcodeData.SubString(_sBarcodeData.Pos(",")  , 20); // 바코드 데이터 전부를 읽어 오도록 수정...
        sId3 = _sBarcodeData.SubString(_sBarcodeData.Pos(",0") , 10); // 기존에는 중간에 특정 부분만 읽어 옴...
        sId = sId1.Trim() + sId2.Trim() + sId3.Trim();                // 맵파일 이름에 형식이 아직 픽스 된게 아니라 추후에 수정 가능성 있음..,JH
    }
    else{
        sId1 = _sBarcodeData.SubString(1, _sBarcodeData.Pos(",")-1);  // 20131227 바코드 불러 오기 실패해서 수정함...
        sId2 = _sBarcodeData.SubString(_sBarcodeData.Pos(",")  , 20); // 바코드 데이터 전부를 읽어 오도록 수정...
        sId = sId1.Trim() + sId2.Trim();                              // 맵파일 이름에 형식이 아직 픽스 된게 아니라 추후에 수정 가능성 있음..,JH
    }


    return sId.Trim();

    //AnsiString sId = _sBarcodeData ;
    //return sId.Trim() ;
}

AnsiString CStage::GetWaferIdx(AnsiString _sBarcodeData)
{
    _sBarcodeData.Delete(1,_sBarcodeData.Pos(","));
    _sBarcodeData.Delete(1,_sBarcodeData.Pos(","));

    AnsiString sIdx = _sBarcodeData; //50번째 문자서 부터 50개의 문자가 웨이퍼 아이디.

    return sIdx.Trim();

}

#include <math.h>
double CStage::GetStageAbsMmFromAbsAngle(double _dAbsAngle)
{
    const double dDegreeToRadian = 3.14159265358979323846 / 180.0 ;

    const double dStageHomeAngle =  -15.0 ; //스테이지 볼스크류 홈 각도.
    const double dStagePLimAngle =   15.0 ; //스테이지 볼스크류 홈 각도.
    const double dLengthStgCntrToScrew = 58.0 ; //스테이지 센터에서 볼스크류 센터 까지 거리.

    double dRetPos = 0.0 ;

    if(_dAbsAngle < dStageHomeAngle) _dAbsAngle = dStageHomeAngle ;
    if(_dAbsAngle > dStagePLimAngle) _dAbsAngle = dStagePLimAngle ;

    dRetPos = dLengthStgCntrToScrew * tan(_dAbsAngle * dDegreeToRadian) ;

    return dRetPos ;
}

double CStage::GetStageAbsAngleFromAbsMm(double _dAbsMm)
{
    const double dRadianToDegree = 180.0 / 3.14159265358979323846 ;

    const double dStageHomeAngle =  -15.0 ; //스테이지 볼스크류 홈 각도.
    const double dStagePLimAngle =   15.0 ; //스테이지 볼스크류 홈 각도.
    const double dLengthStgCntrToScrew = 58.0 ; //스테이지 센터에서 볼스크류 센터 까지 거리.

    double dRetPos = 0.0 ;

    //if(_dAbsAngle < dStageHomeAngle) _dAbsAngle = dStageHomeAngle ;
    //if(_dAbsAngle > dStagePLimAngle) _dAbsAngle = dStagePLimAngle ;

    dRetPos = atan(_dAbsMm / dLengthStgCntrToScrew) ;

    dRetPos *= dRadianToDegree ;

    return dRetPos ;
}

//---------------------------------------------------------------------------
void CStage::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

//---------------------------------------------------------------------------
void CStage::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

}

void CStage::SetStartTime(EN_SEQ_CYCLE _iSeqCycle)
{
    static double dPreSttTime[MAX_SEQ_CYCLE] ;

    double dNow = GetTickTime();
    int iPitch;

    dPreSttTime[_iSeqCycle] = Time[_iSeqCycle].dStart ;
    Time[_iSeqCycle].dStart = dNow ;
    Time[_iSeqCycle].dPitch = Time[_iSeqCycle].dStart - dPreSttTime[_iSeqCycle] ;
    iPitch = Time[_iSeqCycle].dPitch;
    Time[_iSeqCycle].dPitch = iPitch;
}

void CStage::SetEndTime  (EN_SEQ_CYCLE _iSeqCycle)
{
    double dNow = GetTickTime();
    int iWork;

    iWork = dNow - Time[_iSeqCycle].dStart ;
    Time[_iSeqCycle].dWork = iWork ;

}

//---------------------------------------------------------------------------
bool CStage::FindChip(EN_ARAY_ID _riWhre , int &r , int &c , EN_CHIP_STAT _csStats)
{
//축에서 가까운쪽이 스타트 포지션으로 하기로 햇음.

    if(_riWhre == riCST ){
        r = DM.ARAY[riCST].FindLastRow(_csStats) ;
    }
    else return false ;
    return (r > -1 );
}

//---------------------------------------------------------------------------
double CStage::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{

    double dPos  = MT_GetCmdPos(_iMotr);
    int r = -1 , c = -1 ;

    FindChip(riCST , r, c , csUnkwn);
    double dCSTPickPs = PM.GetValue(miSTG_ZCst , pvSTG_ZCstPickStt) - (OM.DevInfo.iCsSlotCnt - r - 1) * OM.DevInfo.dCsSlotPitch ; //Loader Work Position

    FindChip(riCST , r, c , csMask );
    double dCSTPlcePs = PM.GetValue(miSTG_ZCst , pvSTG_ZCstPickStt) - (OM.DevInfo.iCsSlotCnt - r - 1) * OM.DevInfo.dCsSlotPitch + PM.GetValue(miSTG_ZCst , poSTG_ZCstPlaceOfs); //Loader Work Position



    if(_iMotr == miSTG_ZCst){
        switch(_iPstnId) {
            default                 : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piSTG_ZCstWait     : dPos = PM.GetValue (_iMotr , pvSTG_ZCstWait     ); break ;
            case piSTG_ZCstPickStt  : dPos = PM.GetValue (_iMotr , pvSTG_ZCstPickStt  ); break ;
            case piSTG_ZCstPick     : dPos = dCSTPickPs                                ; break ;
            case piSTG_ZCstPlace    : dPos = dCSTPlcePs                                ; break ;
        }
    }

    else if(_iMotr == miSTG_XTrs){
        switch(_iPstnId){
            default                 : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piSTG_XTrsWait     : dPos = PM.GetValue (_iMotr , pvSTG_XTrsWait     ); break ;
            case piSTG_XTrsPick     : dPos = PM.GetValue (_iMotr , pvSTG_XTrsPick     ); break ;
            case piSTG_XTrsPickWait : dPos = PM.GetValue (_iMotr , pvSTG_XTrsPickWait ); break ;
            case piSTG_XTrsBarcode  : dPos = PM.GetValue (_iMotr , pvSTG_XTrsBarcode  ); break ;
            case piSTG_XTrsPlace    : dPos = PM.GetValue (_iMotr , pvSTG_XTrsPlace    ); break ;
        }
    }

    else if(_iMotr == miSTG_TStg){
        switch(_iPstnId){
            default                 : dPos = MT_GetCmdPos(_iMotr                      ); break ;
            case piSTG_TStgWait     : dPos = PM.GetValue (_iMotr , pvSTG_TStgWait     ); break ;
            case piSTG_TStgTAlgn    : dPos = PM.GetValue (_iMotr , pvSTG_TStgTAlgn    ); break ;
        }
    }

    return dPos ;
}

void CStage::CallbackSTGRightClickAray(int _iCol , int _iRow , TShiftState Shift)
{



}


//---------------------------------------------------------------------------
bool CStage::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isTrnsWaitPs   =/*MT_GetStopInpos(miSTG_XTrs) &&*/ (MT_GetCmdPos(miSTG_XTrs) <= PM.GetValue(miSTG_XTrs , pvSTG_XTrsPickWait)) ;
    bool isCSTWorkPs    = MT_GetStopInpos(miSTG_ZCst) && (MT_GetCmdPos(miSTG_ZCst) <= PM.GetValue(miSTG_ZCst , pvSTG_ZCstWait    )) ;
    bool isCSTMoving    =!MT_GetStop     (miSTG_ZCst) ;
    bool isZPenBwd      = AT_Complete(aiHED_PenMrkDU , ccBwd);
    bool isSTG_ChkTblUp =!AT_Complete(aiSTG_ChkTblUD , ccBwd);
    bool isSTG_TrsCmpGr = AT_Complete(aiSTG_TrsCmpGR , ccFwd);
    bool isPenWaitPs    = MT_CmprPos(miHED_XHed , PM.GetValue(miHED_XHed,pvHED_XHedPCover)) &&
                          MT_CmprPos(miHED_YHed , PM.GetValue(miHED_YHed,pvHED_YHedPCover)) ;


    if(_iMotr == miSTG_ZCst){
        if(!isTrnsWaitPs) {sMsg = "트렌스퍼 모터가 전진해 있습니다.";  bRet = false ;}
        switch(_iPstnId){
            default                 : sMsg = "Is Not Motr Position" ; bRet = false ; break ;
            case piSTG_ZCstWait     : break ;
            case piSTG_ZCstPickStt  : break ;
            case piSTG_ZCstPlace    : break ;
            case piSTG_ZCstPick     : break ;
        }
    }

    else if(_iMotr == miSTG_XTrs) {
        if(isSTG_ChkTblUp) { sMsg = "척테이블이 올라와있습니다."; bRet = false ;}
        switch(_iPstnId){
            default                 : sMsg = "Is Not Motr Positon"; bRet = false; break ;
            case piSTG_XTrsWait     : break ;
            case piSTG_XTrsPick     : if( isCSTMoving   ) {sMsg = "카세트가 이동중 입니다."; bRet = false ; } break ;
            case piSTG_XTrsPickWait : break ;
            case piSTG_XTrsBarcode  : break ;
            case piSTG_XTrsPlace    : break ; //if( isSTG_TrsCmpGr) {sMsg = "그리퍼가 닫혀있습니다." ; bRet = false ; } break ;
        }
    }
    else if(_iMotr == miSTG_TStg) {
        if(!isZPenBwd && !isPenWaitPs) { sMsg = "펜이 내려와 있습니다."; bRet = false;}
        switch(_iPstnId){
            default                 : sMsg = "Is Not Motr Position"; bRet = false; break ;
            case piSTG_TStgWait     : break ;
            case piSTG_TStgTAlgn    : break ;
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
bool CStage::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    bool bTrsnWaitPs = MT_GetStopInpos(miSTG_XTrs) && MT_GetCmdPos(miSTG_XTrs) <= PM.GetValue(miSTG_XTrs , pvSTG_XTrsWait) + 5.0; //마진을 좀 준다.
    bool bZCovFwd    = !AT_Complete(aiHED_PenCoverUD , ccBwd);
    bool bHedMoving  = !MT_GetStopInpos(miHED_XHed) || !MT_GetStopInpos(miHED_YHed);

    AnsiString sMsg ;
    bool       bRet = true ;
//
    if(_iActr == aiSTG_ChkTblUD){
        if(!bTrsnWaitPs) { sMsg = "트렌스퍼가 Wait위치에 있지 않습니다." ; bRet = false ;}
    }
    else if(_iActr == aiSTG_TrsCmpGR){
    }
    else if(_iActr == aiHED_PenCoverUD){
        if(bHedMoving) { sMsg = "HED axis is Moving"                     ; bRet = false;}
    }
    return bRet ;
}

bool CStage::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);


    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CStage::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CStage::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmHome.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iHomeTO);
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

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;
                 //HED->STG

        case 10: IO_SetY(ySTG_WfrVac , false);

                 AT_MoveCyl(aiHED_PenMrkDU   , ccBwd);
                 AT_MoveCyl(aiHED_PenCoverUD , ccBwd);

                 //AT_MoveCyl(aiSTG_TrsCmpGR , ccBwd);
                 //AT_MoveCyl(aiSTG_ChkTblUD , ccBwd);
                 MT_SetHomeEnd(miSTG_ZCst,false);
                 MT_SetHomeEnd(miSTG_TStg,false);
                 MT_SetHomeEnd(miSTG_XTrs,false);

                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiHED_PenMrkDU   , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiHED_PenCoverUD , ccBwd)) return false;
                 //if(!AT_MoveCyl(aiSTG_TrsCmpGR , ccBwd)) return false ;
                 //if(!AT_MoveCyl(aiSTG_ChkTblUD , ccBwd)) return false ;
                 if(AT_Complete(aiSTG_ChkTblUD , ccFwd)&& OM.MstOptn.bVacuum) IO_SetY(ySTG_WfrVac , true);

                 MT_DoHome(miSTG_XTrs);

                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miSTG_XTrs)) return false ;
                 MT_DoHome(miSTG_TStg);
                 MT_DoHome(miSTG_ZCst);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GetHomeEnd(miSTG_ZCst)) return false ;
                 if(!MT_GetHomeEnd(miSTG_TStg)) return false ;
                 MT_GoAbsMan(miSTG_ZCst , PM.GetValue(miSTG_ZCst , pvSTG_ZCstWait));
                 MT_GoAbsMan(miSTG_XTrs , PM.GetValue(miSTG_XTrs , pvSTG_XTrsWait));
                 MT_GoAbsMan(miSTG_TStg , PM.GetValue(miSTG_TStg , pvSTG_TStgWait));
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GoAbsMan(miSTG_ZCst , PM.GetValue(miSTG_ZCst , pvSTG_ZCstWait))) return false ;
                 if(!MT_GoAbsMan(miSTG_XTrs , PM.GetValue(miSTG_XTrs , pvSTG_XTrsWait))) return false ;
                 if(!MT_GoAbsMan(miSTG_TStg , PM.GetValue(miSTG_TStg , pvSTG_TStgWait))) return false ;
                 Step.iHome = 0;
                 return true ;
    }
}

bool CStage::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

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
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
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

        case 10: if(DM.ARAY[riCST].CheckAllStat(csNone) && IO_GetX(xSTG_CastDetect)){
                     DM.ARAY[riCST].SetStat(csUnkwn);
                 }
                 Step.iToStart ++ ;
                 return false ;

        case 11:

                 Step.iToStart++;
                 return false;

        case 12:
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CStage::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 15000)) EM_SetErr((EN_ERR_ID)m_iStopTO);

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

        case 10: MoveActr(aiSTG_TrsCmpGR , ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(!MoveActr(aiSTG_TrsCmpGR , ccBwd)) return false ;
                 MT_Stop(miSTG_XTrs);
                 Step.iToStop++;
                 return false ;

        case 12: if(!MT_GetStop(miSTG_XTrs)) return false ;
                 MoveMotr(miSTG_XTrs , piSTG_XTrsWait);
                 Step.iToStop ++;
                 return false ;

        case 13: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsWait)) return false ;
                 Load(false);

                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CStage::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool bHeadPenPos   = MT_CmprPos(miHED_XHed , PM.GetValue(miHED_XHed ,pvHED_XHedPCover)) &&
                             MT_CmprPos(miHED_YHed , PM.GetValue(miHED_YHed ,pvHED_YHedPCover)) ;

        bool isCycleSupply = DM.ARAY[riCST].GetCntStat  (csUnkwn) && DM.ARAY[riSTG].CheckAllStat(csNone)  && bHeadPenPos;
        bool isCycleExpend = DM.ARAY[riSTG].GetCntStat  (csFail ) && !AT_Complete(aiSTG_ChkTblUD , ccFwd)  ;
        bool isCyclePlace  = DM.ARAY[riCST].GetCntStat  (csMask ) && //카세트에 꼽을때가 있고
                            !DM.ARAY[riSTG].GetCntStat  (csFail ) && //펜찍어야 되는 곳이 없고.
                            !DM.ARAY[riSTG].CheckAllStat(csNone ) && //웨이퍼는 있어야 되고.
                            !DM.ARAY[riSTG].GetCntStat  (csEror ) &&
                            !DM.ARAY[riSTG].GetCntStat  (csWait ) && bHeadPenPos;  //비전 기다리는 것이 없고.
        bool isConEnd      =!isCycleSupply && !isCycleExpend && !isCyclePlace ;


        if(!IO_GetX(xSTG_CastDetect)  &&  DM.ARAY[riCST].CheckAllStat(csNone )                                                                              ) EM_SetErr(eiCST_Need      ) ;
        if( IO_GetX(xSTG_CastDetect)  && !DM.ARAY[riCST].CheckAllStat(csNone )&& !DM.ARAY[riCST].GetCntStat  (csUnkwn) && !DM.ARAY[riCST].GetCntStat(csMask)) {
            EM_SetErr(eiCST_NeedChange) ;
        }

        //if( IO_GetX(xSTG_CastDetect)  &&  DM.ARAY[riCST].CheckAllStat(csNone )) EM_SetErr(eiCST_Unknwn    ); //바코드를 안쓰면 이에러가 뜨게 된다. 에러내용에 맵파일 로딩 하라고 뜸.
        if(!IO_GetX(xSTG_CastDetect)  && !DM.ARAY[riCST].CheckAllStat(csNone )) EM_SetErr(eiCST_Dispr     );

        if( IO_GetX(xSTG_SgWfDetect) &&  DM.ARAY[riSTG].CheckAllStat(csNone )) EM_SetErr(eiSTG_Unknwn    ); //바코드를 안쓰면 이에러가 뜨게 된다. 에러내용에 맵파일 로딩 하라고 뜸.
        if(!IO_GetX(xSTG_SgWfDetect) && !DM.ARAY[riSTG].CheckAllStat(csNone )) EM_SetErr(eiSTG_Dispr     );


        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleSupply) {Trace(m_sPartName.c_str(),"CycleSupply Stt"); Step.iSeq = scSupply ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleExpend) {Trace(m_sPartName.c_str(),"CycleExpend Stt"); Step.iSeq = scExpend ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCyclePlace ) {Trace(m_sPartName.c_str(),"CyclePlace  Stt"); Step.iSeq = scPlace  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;

        SetStartTime(Step.iSeq) ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default       :                      Trace(m_sPartName.c_str(),"default     End");                      Step.iSeq = scIdle ;  return false ;
        case scIdle   :                                                                   SetEndTime(Step.iSeq);                      return false ;
        case scSupply : if(CycleSupply  ()){ Trace(m_sPartName.c_str(),"CycleSupply End");SetEndTime(Step.iSeq);Step.iSeq = scIdle ;} return false ;
        case scExpend : if(CycleExpend  ()){ Trace(m_sPartName.c_str(),"CycleExpend End");SetEndTime(Step.iSeq);Step.iSeq = scIdle ;} return false ;
        case scPlace  : if(CyclePlace   ()){ Trace(m_sPartName.c_str(),"CyclePlace  End");SetEndTime(Step.iSeq);Step.iSeq = scIdle ;} return false ;
    }
}

bool CStage::BarcodeLoadingTest()
{
                  Barcode->sData="5510KC6.002\r";

                  //if(!m_tmTemp.OnDelay(true , 1000 )) return false ;
 bool bRet ;
 String sWaferId ;
 String sWaferIdx;
 int iTemp ;


                  if(OM.CmnOptn.bUseBarcode){
                      sWaferId  = GetWaferId (Barcode->sData);
                      sWaferIdx = GetWaferIdx(Barcode->sData);

                      //일단 김진식씨 테스트 용으로 웨이퍼 바코드 다 쓰는 걸로 임시 수정. 2013.12.26
                      sWaferIdx = Barcode->sData ;
                      iTemp = sWaferIdx.Pos("\r") ;
                      sWaferIdx = sWaferIdx.Delete(iTemp,1);



                      sMainWfName  = sWaferId  ;


                      Trace("BarcodeIdx",sWaferIdx.c_str());

                      //sWaferId  = "KA85929XB-26.27.28.29.30" ;



                      if(OM.DevInfo.iMapType == 0) {
                           bRet = MAPU.ConvertCmn(OM.CmnOptn.sSourcePath +"\\" + sWaferId + "." + OM.DevInfo.sExtention , riMAP) ;
                          if(!bRet){
                              EM_SetErr(eiSTG_MapLoadingFail);
                              STG.Barcode->sData = "";
                              Trace("ConvertCmn","fail");
                              MoveActr(aiSTG_TrsCmpGR , ccBwd);
                              Step.iCycle = 40 ;
                              return false ;
                          }
                      }
                      else if(OM.DevInfo.iMapType == 1){

                          bRet = MAPU.ConvertCoordi1  (OM.CmnOptn.sSourcePath +"\\" + sWaferId + "." + OM.DevInfo.sExtention , riMAP);
                          if(!bRet){
                              EM_SetErr(eiSTG_MapLoadingFail);
                              STG.Barcode->sData = "";
                              MoveActr(aiSTG_TrsCmpGR , ccBwd);
                              Step.iCycle = 40 ;
                              return false ;
                          }
                      }
                      else if(OM.DevInfo.iMapType == 2){
                          bRet = MAPU.ConvertCoordi2  (OM.CmnOptn.sSourcePath +"\\" + sWaferId + "." + OM.DevInfo.sExtention , riMAP);
                          if(!bRet) {
                              EM_SetErr(eiSTG_MapLoadingFail);
                              STG.Barcode->sData = "";
                              MoveActr(aiSTG_TrsCmpGR , ccBwd);
                              Step.iCycle = 40 ;
                              return false ;
                          }
                      }
                      else {
                          bRet = MAPU.ConvertCoordi8in(OM.CmnOptn.sSourcePath +"\\" + sWaferId + "." + OM.DevInfo.sExtention , riMAP);
                          if(!bRet){
                              EM_SetErr(eiSTG_MapLoadingFail);
                              STG.Barcode->sData = "";
                              MoveActr(aiSTG_TrsCmpGR , ccBwd);
                              Step.iCycle = 40 ;
                              return false ;
                          }
                      }

          //            if(!MAPU.MergeArray (riMSK , riMAP  , riSTG)) {
          //                //g_bLocking = false ;
          //                DM.ARAY[riSTG].SetStat(csNone);
          //                EM_SetErr(eiSTG_MapLoadingFail);
          //                Step.iCycle = 40 ;
          //                return false ;
          //            }
          //            if(OM.DevInfo.bMskExist && OM.DevInfo.iMapType == 0){
          //                MAPU.ChangeMaskNone(OM.CmnOptn.sSourcePath +"\\" + sWaferId + "." + OM.DevInfo.sExtention , OM.DevInfo.iMapType , riSTG);
          //            }
          //            //g_bLocking = false ;
          //                 if(OM.DevInfo.iFlatAngle == 0) {                           }/*0  */
          //            else if(OM.DevInfo.iFlatAngle == 1) {DM.ARAY[riSTG].TurnCw90 ();}/*90 */
          //            else if(OM.DevInfo.iFlatAngle == 2) {DM.ARAY[riSTG].TurnCw180();}/*180*/
          //            else if(OM.DevInfo.iFlatAngle == 3) {DM.ARAY[riSTG].TurnCw270();}/*270*/
          //        }
          //        else {
          //            //???지울까?? ㅠ JS
          //        }


                    const int    iFlatAngle    = OM.DevInfo.iFlatAngle   ;
                    int    iColMax ;
                    int    iRowMax ;

                    iColMax = DM.ARAY[riMAP].GetMaxCol();
                    iRowMax = DM.ARAY[riMAP].GetMaxRow();

                         if(iFlatAngle == 0) {                           }/*0  */
                    else if(iFlatAngle == 1) {DM.ARAY[riMAP].TurnCw90 ();}/*90 */
                    else if(iFlatAngle == 2) {DM.ARAY[riMAP].TurnCw180();}/*180*/
                    else if(iFlatAngle == 3) {DM.ARAY[riMAP].TurnCw270();}/*270*/

                    iColMax = DM.ARAY[riMAP].GetMaxCol();
                    iRowMax = DM.ARAY[riMAP].GetMaxRow();


                    if(!MAPU.MergeArray(riMSK , riMAP  , riSTG)){ FM_MsgOk("ERROR","마스크영역에 기준점을 확인 해주세요"); DM.ARAY[riSTG].SetStat(csNone); }

                    if(OM.DevInfo.bMskExist && OM.DevInfo.iMapType == 0){ MAPU.ChangeMaskNone(OM.CmnOptn.sSourcePath +"\\" + sWaferId + "." + OM.DevInfo.sExtention , OM.DevInfo.iMapType , riSTG); }

                    iColMax = DM.ARAY[riSTG].GetMaxCol();
                    iRowMax = DM.ARAY[riSTG].GetMaxRow();

                    if(OM.DevInfo.bMskExist) {
                             if(iFlatAngle == 0) {                           }/*0  */
                        else if(iFlatAngle == 1) {DM.ARAY[riSTG].TurnCw90 ();}/*90 */
                        else if(iFlatAngle == 2) {DM.ARAY[riSTG].TurnCw180();}/*180*/
                        else if(iFlatAngle == 3) {DM.ARAY[riSTG].TurnCw270();}/*270*/
                    }
                    iColMax = DM.ARAY[riSTG].GetMaxCol();
                    iRowMax = DM.ARAY[riSTG].GetMaxRow();

                  }

                  //SP 김진식씨 요청 사항. 한가지 맵파일만 자동으로 계속 불러 오기...JH

                  if(!OM.CmnOptn.bUseBarcode && OM.CmnOptn.bLoadOnlyOneMapMarking){
                      //g_bLocking = true ;  //나중에  MAPU 안으로 넣을수 있도록 하자.
                      bRet = MAPU.ConvertCmn (OM.CmnOptn.sLoadOnlyOneMapMrkPath , riMAP);
                      //g_bLocking = false ;
                      if(!bRet){
                          EM_SetErr(eiSTG_MapLoadingFail);
                          Trace("ConvertCmn","fail");
                          Step.iCycle = 40 ;
                          MoveActr(aiSTG_TrsCmpGR , ccBwd);
                          return false ;
                      }
                      //g_bLocking = true ;
                      bRet = MAPU.MergeArray(riMSK , riMAP  , riSTG);
                      //g_bLocking = false ;

                      if(!bRet){
                          DM.ARAY[riSTG].SetStat(csNone);
                          EM_SetErr(eiSTG_MapLoadingFail);
                          STG.Barcode->sData = "";
                          Step.iCycle = 40 ;
                          MoveActr(aiSTG_TrsCmpGR , ccBwd);
                          return false ;
                      }
                      if(OM.DevInfo.bMskExist && OM.DevInfo.iMapType == 0){
                          //g_bLocking = true ;
                          MAPU.ChangeMaskNone(OM.CmnOptn.sLoadOnlyOneMapMrkPath , OM.DevInfo.iMapType , riSTG);
                          //g_bLocking = false ; //나중에  MAPU 안으로 넣을수 있도록 하자.
                      }

                           if(OM.DevInfo.iFlatAngle == 0) {                           }
                      else if(OM.DevInfo.iFlatAngle == 1) {DM.ARAY[riSTG].TurnCw90 ();}
                      else if(OM.DevInfo.iFlatAngle == 2) {DM.ARAY[riSTG].TurnCw180();}
                      else if(OM.DevInfo.iFlatAngle == 3) {DM.ARAY[riSTG].TurnCw270();}
                  }


                  sWaferIdx = Barcode->sData ;
                  iTemp = sWaferIdx.Pos("\r") ;
                  sWaferIdx = sWaferIdx.Delete(iTemp,1);
                  if(OM.CmnOptn.bUseBarcode) m_sWorkingWfrId = sWaferIdx ;
                  else                       m_sWorkingWfrId = Now().CurrentDateTime().FormatString("yyyymmdd_hhnnss") ;

                  DM.ARAY[riSTG].SetID   (sWaferId );
                  DM.ARAY[riSTG].SetLotNo(sWaferIdx);

                  LT.WriteArayData(riSTG);

                  //FindChip(riCST , r, c , csUnkwn);
                  //DM.ARAY[riCST].SetStat(r, 0, csMask);
}

//One Cycle.
//--------------------------------------------------------------------------
bool CStage::CycleSupply(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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

    if(Stat.bReqStop) {
        if(Step.iCycle <= 14){
            MT_Stop(miSTG_XTrs);
            Step.iCycle = 0;
            return true;
        }
    }

    int r,c ;
    AnsiString sSendMsg ;
    AnsiString sWaferId , sWaferIdx;
    AnsiString sMapPath ;
    bool bRet = false ;
    int iTemp ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //LotInfo = Wafer 1장...JS
                  LT.LotInfo.dStartTime = Now();
                  HED.m_iLastInspChipC = 0;
                  HED.m_iLastInspChipR = 0;
                  DM.ARAY[riSTG].ClearTemp();
                  //LotInfo = 1 Casstte 시.....JS
                  //if(DM.ARAY[riCST].CheckAllStat(csUnkwn)) LT.LotInfo.dStartTime = Now();

                  MoveMotr(miSTG_TStg , piSTG_TStgWait);
                  MoveActr(aiSTG_WfrAlgnFB , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miSTG_TStg , piSTG_TStgWait)) return false;
                  if(!MoveActr(aiSTG_WfrAlgnFB , ccBwd    )) return false;
                  MoveActr(aiSTG_ChkTblUD , ccBwd);
                  MoveActr(aiSTG_TrsCmpGR , ccBwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiSTG_ChkTblUD , ccBwd)) return false;
                  if(!MoveActr(aiSTG_TrsCmpGR , ccBwd)) return false;
                  MoveMotr(miSTG_ZCst , piSTG_ZCstPick    );
                  MoveMotr(miSTG_XTrs , piSTG_XTrsPickWait);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miSTG_ZCst , piSTG_ZCstPick    )) return false;
                  if(!MoveMotr(miSTG_XTrs , piSTG_XTrsPickWait)) return false;
                  IO_SetY(ySTG_Ionizer , true);
                  MoveMotr(miSTG_XTrs , piSTG_XTrsPick);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsPick)) return false;
                  MoveActr(aiSTG_TrsCmpGR , ccFwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiSTG_TrsCmpGR , ccFwd)) return false;
                  if(!IO_GetX(xSTG_TfWfDetect)){ //웨이퍼 없는 경우.
                      MoveActr(aiSTG_TrsCmpGR , ccBwd);
                      Step.iCycle++;
                      return false;
                  }
                  else { //웨이퍼 있는 경우.
                      MoveMotr(miSTG_XTrs , piSTG_XTrsBarcode);
                      Step.iCycle = 20;
                      return false;
                  }

        case  16: if(!MoveActr(aiSTG_TrsCmpGR , ccBwd))return false;

                  if(MM.GetManNo() == mcSTG_CycleSupply)MoveMotr(miSTG_XTrs , piSTG_XTrsWait    );
                  else                                  MoveMotr(miSTG_XTrs , piSTG_XTrsPickWait);
                  Step.iCycle++;
                  return false;

        case  17: if(!MT_GetStop(miSTG_XTrs))   return false;
                  FindChip(riCST , r, c , csUnkwn);
                  DM.ARAY[riCST].SetStat(r, 0, csEmpty);

                  //Autorun에서 에러 띄움.
                  //if(!DM.ARAY[riCST].GetCntStat(csUnkwn)){
                  //    EM_SetErr(eiCST_NeedChange);
                  //}
                  Step.iCycle = 0;
                  return true ;


        // 웨이퍼 픽 하고 다시 틀어짐을 잡아주는 사이클
        case  20: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsBarcode)) return false;
                  MoveActr(aiSTG_TrsCmpGR , ccBwd);
                  Step.iCycle++;
                  return false;

        case  21: if(!MoveActr(aiSTG_TrsCmpGR , ccBwd)) return false;
                  //MT_GoIncAcDc(miSTG_XTrs , 3.0 , 300 , 5000, 5000);
                  Step.iCycle++;
                  return false;

        case  22: if(!MT_GetStop(miSTG_XTrs)) return false;
                  MoveMotr(miSTG_XTrs , piSTG_XTrsPickWait);
                  Step.iCycle++;
                  return false;

        case  23: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsPickWait)) return false;
                  MoveActr(aiSTG_TrsCmpGR , ccFwd);
                  Step.iCycle++;
                  return false;

        case  24: if(!MoveActr(aiSTG_TrsCmpGR , ccFwd)) return false;
                  MoveMotr(miSTG_XTrs , piSTG_XTrsBarcode);
                  Step.iCycle++;
                  return false;

        case  25: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsBarcode)) return false;

                  //아직 스테이지 데이터가 없어 에러 내용 통일 때문에 그냥 주석.
                  //그냥 바코드 에러로 뜨게.
                  //if(!IO_GetX(xSTG_TfWfDetect)){
                  //    MoveActr(aiSTG_TrsCmpGR , ccBwd);
                  //    MoveMotr(miSTG_XTrs , piSTG_XTrsPickWait);
                  //    EM_SetErr(eiSTG_TransWaferFeedingFail);
                  //    Step.iCycle = 0;
                  //    return true;



                  //}


                  if(OM.CmnOptn.bUseBarcode){
                      SendBarcodeMsg("LON\r");
                      STG.Barcode->sData = "";
                  }

                  m_tmTemp.Clear();
                  Step.iCycle ++;
                  return false;

        case  26:
                  if(m_tmTemp.OnDelay(true , 1000 )) {
                      EM_SetErr(eiSTG_BarcodeFail);
                      SendBarcodeMsg("LOFF\r");
                      MoveActr(aiSTG_TrsCmpGR , ccBwd);
                      STG.Barcode->sData = "";
                      Step.iCycle = 40 ;
                      return false ;
                      //Bar Code 확인 하여서 m_bBarCodeRlst값 변환.JS
                  }
                  if(OM.CmnOptn.bUseBarcode && !Barcode->sData.Pos("\r") ) return false;

                  //if(!m_tmTemp.OnDelay(true , 1000 )) return false ;



                  if(OM.CmnOptn.bUseBarcode){
                      //쓸데 없어서 뺌. 안에 보면 ","를 기준으로 뽑는데 현재 "."을 쓰는듯. 20190729
                      //sWaferId  = GetWaferId (Barcode->sData);
                      //sWaferIdx = GetWaferIdx(Barcode->sData);

                      //일단 김진식씨 테스트 용으로 웨이퍼 바코드 다 쓰는 걸로 임시 수정. 2013.12.26
                      sWaferId  = Barcode->sData ;
                      sWaferIdx = Barcode->sData ;
                      iTemp = sWaferIdx.Pos("\r") ;
                      sWaferIdx = sWaferIdx.Delete(iTemp,1);
                      sMainWfName  = sWaferIdx  ;

                      Trace("BarcodeIdx",sWaferIdx.c_str());

                      //sWaferId  = "KA85929XB-26.27.28.29.30" ;
                      //20190729
                      //C90879-13C0 <= 웨이퍼바코드가 이런게 들어옴.
                      //하는 요령은 밑에 쩜이 없어서C90879-13C0.map 으로 이름을 바꾸고 OM.DevInfo.sExtention에 map을 입력.
                      if(sWaferId.Pos(".")&&OM.DevInfo.iMapType == 0){
                          sMapPath = OM.CmnOptn.sSourcePath +"\\" + sWaferId  ;
                      }
                      else {
                          sMapPath = OM.CmnOptn.sSourcePath +"\\" + sWaferId + "." + OM.DevInfo.sExtention  ;
                      }

                      if(OM.DevInfo.iMapType == 0) {
                          Trace("ConvertCmn Path", sMapPath.c_str());
                          bRet = MAPU.ConvertCmn(sMapPath , riMAP) ;

                          if(!bRet){
                              EM_SetErr(eiSTG_MapLoadingFail);
                              STG.Barcode->sData = "";
                              Trace("ConvertCmn","fail");
                              MoveActr(aiSTG_TrsCmpGR , ccBwd);
                              Step.iCycle = 40 ;
                              return false ;
                          }
                      }
                      else if(OM.DevInfo.iMapType == 1){

                          bRet = MAPU.ConvertCoordi1  (sMapPath , riMAP);
                          if(!bRet){
                              EM_SetErr(eiSTG_MapLoadingFail);
                              STG.Barcode->sData = "";
                              MoveActr(aiSTG_TrsCmpGR , ccBwd);
                              Step.iCycle = 40 ;
                              return false ;
                          }
                      }
                      else if(OM.DevInfo.iMapType == 2){
                          bRet = MAPU.ConvertCoordi2  (sMapPath , riMAP);
                          if(!bRet) {
                              EM_SetErr(eiSTG_MapLoadingFail);
                              STG.Barcode->sData = "";
                              MoveActr(aiSTG_TrsCmpGR , ccBwd);
                              Step.iCycle = 40 ;
                              return false ;
                          }
                      }
                      else {
                          bRet = MAPU.ConvertCoordi8in(sMapPath , riMAP);
                          if(!bRet){
                              EM_SetErr(eiSTG_MapLoadingFail);
                              STG.Barcode->sData = "";
                              MoveActr(aiSTG_TrsCmpGR , ccBwd);
                              Step.iCycle = 40 ;
                              return false ;
                          }
                      }


                    const int    iFlatAngle    = OM.DevInfo.iFlatAngle   ;
                    int    iColMax ;
                    int    iRowMax ;

                    iColMax = DM.ARAY[riMAP].GetMaxCol();
                    iRowMax = DM.ARAY[riMAP].GetMaxRow();

                         if(iFlatAngle == 0) {                           }/*0  */
                    else if(iFlatAngle == 1) {DM.ARAY[riMAP].TurnCw90 ();}/*90 */
                    else if(iFlatAngle == 2) {DM.ARAY[riMAP].TurnCw180();}/*180*/
                    else if(iFlatAngle == 3) {DM.ARAY[riMAP].TurnCw270();}/*270*/

                    iColMax = DM.ARAY[riMAP].GetMaxCol();
                    iRowMax = DM.ARAY[riMAP].GetMaxRow();


                    if(!MAPU.MergeArray(riMSK , riMAP  , riSTG)){ FM_MsgOk("ERROR","마스크영역에 기준점을 확인 해주세요"); DM.ARAY[riSTG].SetStat(csNone); }

                    if(OM.DevInfo.bMskExist && OM.DevInfo.iMapType == 0){ MAPU.ChangeMaskNone(OM.CmnOptn.sSourcePath +"\\" + sWaferId + "." + OM.DevInfo.sExtention , OM.DevInfo.iMapType , riSTG); }

                    iColMax = DM.ARAY[riSTG].GetMaxCol();
                    iRowMax = DM.ARAY[riSTG].GetMaxRow();

                    if(OM.DevInfo.bMskExist) {
                             if(iFlatAngle == 0) {                           }/*0  */
                        else if(iFlatAngle == 1) {DM.ARAY[riSTG].TurnCw90 ();}/*90 */
                        else if(iFlatAngle == 2) {DM.ARAY[riSTG].TurnCw180();}/*180*/
                        else if(iFlatAngle == 3) {DM.ARAY[riSTG].TurnCw270();}/*270*/
                    }
                    iColMax = DM.ARAY[riSTG].GetMaxCol();
                    iRowMax = DM.ARAY[riSTG].GetMaxRow();

                  }

                  //SP 김진식씨 요청 사항. 한가지 맵파일만 자동으로 계속 불러 오기...JH

                  if(!OM.CmnOptn.bUseBarcode && OM.CmnOptn.bLoadOnlyOneMapMarking){
                      //g_bLocking = true ;  //나중에  MAPU 안으로 넣을수 있도록 하자.
                      bRet = MAPU.ConvertCmn (OM.CmnOptn.sLoadOnlyOneMapMrkPath , riMAP);
                      //g_bLocking = false ;
                      if(!bRet){
                          EM_SetErr(eiSTG_MapLoadingFail);
                          Trace("ConvertCmn","fail");
                          Step.iCycle = 40 ;
                          MoveActr(aiSTG_TrsCmpGR , ccBwd);
                          return false ;
                      }
                      //g_bLocking = true ;
                      bRet = MAPU.MergeArray(riMSK , riMAP  , riSTG);
                      //g_bLocking = false ;

                      if(!bRet){
                          DM.ARAY[riSTG].SetStat(csNone);
                          EM_SetErr(eiSTG_MapLoadingFail);
                          STG.Barcode->sData = "";
                          Step.iCycle = 40 ;
                          MoveActr(aiSTG_TrsCmpGR , ccBwd);
                          return false ;
                      }
                      if(OM.DevInfo.bMskExist && OM.DevInfo.iMapType == 0){
                          //g_bLocking = true ;
                          MAPU.ChangeMaskNone(OM.CmnOptn.sLoadOnlyOneMapMrkPath , OM.DevInfo.iMapType , riSTG);
                          //g_bLocking = false ; //나중에  MAPU 안으로 넣을수 있도록 하자.
                      }

                           if(OM.DevInfo.iFlatAngle == 0) {                           }
                      else if(OM.DevInfo.iFlatAngle == 1) {DM.ARAY[riSTG].TurnCw90 ();}
                      else if(OM.DevInfo.iFlatAngle == 2) {DM.ARAY[riSTG].TurnCw180();}
                      else if(OM.DevInfo.iFlatAngle == 3) {DM.ARAY[riSTG].TurnCw270();}
                  }


                  sWaferIdx = Barcode->sData ;
                  iTemp = sWaferIdx.Pos("\r") ;
                  sWaferIdx = sWaferIdx.Delete(iTemp,1);
                  if(OM.CmnOptn.bUseBarcode) m_sWorkingWfrId = sWaferIdx ;
                  else                       m_sWorkingWfrId = Now().CurrentDateTime().FormatString("yyyymmdd_hhnnss") ;

                  DM.ARAY[riSTG].SetID   (sWaferId );
                  DM.ARAY[riSTG].SetLotNo(sWaferIdx);

                  LT.WriteArayData(riSTG);

                  FindChip(riCST , r, c , csUnkwn);
                  DM.ARAY[riCST].SetStat(r, 0, csMask);
                  Step.iCycle ++ ;
                  return false;

        case  27: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsBarcode)) return false ;
                  MoveMotr(miSTG_XTrs , piSTG_XTrsPlace);
                  Step.iCycle ++;
                  return false;

        case  28: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsPlace)) return false ;
                  if(!IO_GetX(xSTG_TfWfDetect)) EM_SetErr(eiSTG_TransWaferFeedingFail);
                  MoveActr(aiSTG_WfrAlgnFB , ccFwd);
                  Step.iCycle++;
                  return false;

        case  29: if(!MoveActr(aiSTG_WfrAlgnFB , ccFwd)) return false;
                  MoveActr(aiSTG_TrsCmpGR , ccBwd);
                  Step.iCycle ++;
                  return false;

        case  30: if(!MoveActr(aiSTG_TrsCmpGR , ccBwd)) return false;
                  MT_GoIncRun( miSTG_XTrs , 2.0);     //트렌스퍼 실린더가 힘이 없어서 Wafer Align 실린더가 밀어주면 당기지 못해서 짧게 감..JS
                  Step.iCycle++;
                  return false;

        case  31: if(!MT_GetStopInpos(miSTG_XTrs)) return false;
                  MoveActr(aiSTG_TrsCmpGR , ccFwd);
                  Step.iCycle++;
                  return false;

        case  32: if(!MoveActr(aiSTG_TrsCmpGR , ccFwd)) return false;
                  MoveMotr(miSTG_XTrs , piSTG_XTrsPlace);
                  Step.iCycle++;
                  return false;

        case  33: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsPlace)) return false;
                  if(!IO_GetX(xSTG_TfWfDetect)) EM_SetErr(eiSTG_TransWaferFeedingFail);
                  MoveActr(aiSTG_TrsCmpGR , ccBwd);

                  Step.iCycle ++ ;
                  return false;

        case  34: if(!MoveActr(aiSTG_TrsCmpGR , ccBwd)) return false ;

                  MoveMotr(miSTG_XTrs , piSTG_XTrsWait);
                  IO_SetY(ySTG_Ionizer , false);

                  //eiSTG_TransWaferFeedingFail 이거 떴으면 혹시 웨이퍼 껴 있을 경우 있어서.
                  if(!EM_IsErr()) MoveMotr(miSTG_ZCst , piSTG_ZCstWait) ;


                  Step.iCycle++;
                  return false ;


        case  35: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsWait)) return false ;
                  //if(!MoveActr(aiSTG_ChkTblUD , ccFwd     )) return false ;
                  DM.ARAY[riSTG].ClearTemp();
                  MoveActr(aiSTG_ChkTblUD , ccFwd);
                  Step.iCycle++;
                  return false;

        case  36: if(!MoveActr(aiSTG_ChkTblUD , ccFwd)) return false ;
                  MoveMotr(miSTG_TStg , piSTG_TStgTAlgn);
                  Step.iCycle++;
                  return false;

        case  37: if(!MoveMotr(miSTG_TStg , piSTG_TStgTAlgn)) return false;

                  if(OM.MstOptn.bVacuum) IO_SetY(ySTG_WfrVac , true); //마스터옵션 체크 박스 체크 시 Vacuum On JH

                  HED.Rqst.bNeedTAlign = true ;
                  Step.iCycle = 0 ;
                  return true;



        //Barcode Data Read Fail 시. 다시 밀어넣고 뒤로 빠진다.
        case  40: MoveMotr(miSTG_XTrs , piSTG_XTrsPick);
                  Step.iCycle++;
                  return false;

        case  41: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsPick)) return false;
                  MoveActr(aiSTG_TrsCmpGR , ccBwd);
                  Step.iCycle++;
                  return false;

        case  42: if(!MoveActr(aiSTG_TrsCmpGR , ccBwd)) return false;
                  //뒤로 빠지는 것은 투스탑에 있음.
                  IO_SetY(ySTG_Ionizer , false);
                  Step.iCycle = 0;
                  return false;

    }
}

bool CStage::CycleExpend(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true;
    //}

    int r,c ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miSTG_XTrs , piSTG_XTrsWait);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsWait)) return false ;
                  MoveActr(aiSTG_ChkTblUD , ccFwd);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveActr(aiSTG_ChkTblUD , ccFwd)) return false ;
                  MoveMotr(miSTG_TStg , piSTG_TStgTAlgn);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miSTG_TStg , piSTG_TStgTAlgn)) return false;

                  if(OM.MstOptn.bVacuum) IO_SetY(ySTG_WfrVac , true); //마스터옵션 체크 박스 체크 시 Vacuum On JH

                  HED.Rqst.bNeedTAlign = true ;
                  Step.iCycle = 0 ;
                  return true ;

    }
}


bool CStage::CyclePlace(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(IO_GetX(xSTG_OvrLdDetect)){
        AT_MoveCyl(aiSTG_TrsCmpGR , ccBwd);

        if(!MT_GetStop(miSTG_XTrs)){ //20150326 엔드카메라 조건수정중에 혹시하해서 추가함.  멈추는지 테스트 해보자...
            MT_Stop(miSTG_XTrs);
        }

        MT_GoAbsRun(miSTG_XTrs , PM.GetValue(miSTG_TStg , pvSTG_XTrsWait)); //어차피 투스탑에서 보낸다. 메뉴얼조건때문에 추가..
        EM_SetErr(eiSTG_TrsOverloadDetect);
        Step.iCycle = 0;
        return true;
    }    //오버로드 센서 추가 확인....JH

    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iCycle != PreStep.iCycle) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true;
    //}

    if(Stat.bReqStop) {
        if(Step.iCycle <= 14){
            Step.iCycle = 0;
            return true;
        }
    }

    int r,c ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: 

                  
                  


                  if(OM.CmnOptn.bUseEndCam ){// 20150326 메뉴얼 상태 및 오버로드 센서 알람있을때 스크린샷 오버라이트 되어서...

                      Cam[ciEnd] -> SetTrgMode(tmSw); //자꾸 트리거 모드가 풀려서
                      //원인을 알수 없어... 일단 야매로 처리...

                      if(!DM.ARAY[riSTG].GetCntStat  (csFail ) && //펜찍어야 되는 곳이 없고.
                         !DM.ARAY[riSTG].CheckAllStat(csNone ) && //웨이퍼는 있어야 되고.
                         !DM.ARAY[riSTG].GetCntStat  (csEror ) && //테두리 영역도 다작업되고.
                         !DM.ARAY[riSTG].GetCntStat  (csWait ) ){ //비젼검사 기다리는것도 없어야...
                          if (!IO_GetX(xETC_FrntDoorL) || !IO_GetX(xETC_FrntDoorR) ){ //문짝 열린채로 사진 찍으면 안되서.
                              EM_SetErr(eiETC_FDoor);
                              Step.iCycle = 0 ;
                              return true ;
                          }
                          //엔드비젼 그랩 명령. 메세지 처리 해서 여기서 써도 안전.
                          GrabEndVisn();
                      }
                  }
                  Step.iCycle++;
                  return false ;



        case  11:
                  if(OM.CmnOptn.bUseEndCam ){
                      if(!DM.ARAY[riSTG].GetCntStat  (csFail ) &&
                         !DM.ARAY[riSTG].CheckAllStat(csNone ) &&
                         !DM.ARAY[riSTG].GetCntStat  (csEror ) &&
                         !DM.ARAY[riSTG].GetCntStat  (csWait ) ){
                          if(!GetGrabEndEndVisn()) return false ; //엔드비젼 그랩 종료 됐는지 확인..
                          SendCmdEndVisn(mcCapAndShowSec); //스샷 및 이미지 세이브.
                      }
                  }
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 1000)) return false ; //메세지 날라가서 스샷 및 이미지 저장 될때까지 딜레이좀 넣음.

                  IO_SetY(ySTG_WfrVac , false);
                  MoveActr(aiSTG_ChkTblUD  , ccBwd);
                  MoveActr(aiSTG_TrsCmpGR  , ccBwd);
                  MoveActr(aiSTG_WfrAlgnFB , ccBwd);
                  if(MM.GetManNo() == mcSTG_CyclePlace)MoveActr(aiHED_PenCoverUD , ccBwd); //메뉴얼 배출중에 펜이랑 트렌스퍼랑 박을수 있음.JS
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiSTG_ChkTblUD   , ccBwd)) return false ;
                  if(!MoveActr(aiSTG_TrsCmpGR   , ccBwd)) return false ;
                  if(!MoveActr(aiSTG_WfrAlgnFB  , ccBwd)) return false ;
                  if(MM.GetManNo() == mcSTG_CyclePlace && !MoveActr(aiHED_PenCoverUD , ccBwd)) return false ; //메뉴얼 배출중에 펜이랑 트렌스퍼랑 박을수 있음.JS
                  if(MM.GetManNo() == mcSTG_CyclePlace)MT_GoAbsRun(miHED_XHed , PM.GetValue(miHED_XHed , pvHED_XHedPCover)); //메뉴얼 배출중에 펜이랑 트렌스퍼랑 박을수 있음.JS
                  if(MM.GetManNo() == mcSTG_CyclePlace)MT_GoAbsRun(miHED_YHed , PM.GetValue(miHED_YHed , pvHED_YHedPCover)); //메뉴얼 배출중에 펜이랑 트렌스퍼랑 박을수 있음.JS
                  Step.iCycle++;
                  return false;

        case  14: if(MM.GetManNo() == mcSTG_CyclePlace &&!MT_GoAbsRun(miHED_XHed , PM.GetValue(miHED_XHed , pvHED_XHedPCover))) return false; //메뉴얼 배출중에 펜이랑 트렌스퍼랑 박을수 있음.JS
                  if(MM.GetManNo() == mcSTG_CyclePlace &&!MT_GoAbsRun(miHED_YHed , PM.GetValue(miHED_YHed , pvHED_YHedPCover))) return false; //메뉴얼 배출중에 펜이랑 트렌스퍼랑 박을수 있음.JS
                  MoveMotr(miSTG_TStg , piSTG_TStgWait);
                  MoveMotr(miSTG_ZCst , piSTG_ZCstPlace);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miSTG_TStg , piSTG_TStgWait )) return false ;
                  if(!MoveMotr(miSTG_ZCst , piSTG_ZCstPlace)) return false ;
                  //MT_GoIncMan(miSTG_ZCst , PM.GetValue(miSTG_ZCst , poSTG_ZCstPlaceOfs));
                  IO_SetY(ySTG_Ionizer , true);
                  Step.iCycle++;
                  return false;

        case  16: if(!MT_GetStopInpos(miSTG_ZCst)) return false;
                  MoveMotr(miSTG_XTrs , piSTG_XTrsPlace);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsPlace)) return false ;

                  //if() 여기에 추가 한다...오버로드 센서 에러 JH

                  //LotInfo = Wafer 1장...JS
                  LotInfoAdd();

                  //LotInfo = 1 Casstte 시.....JS
                  //if(DM.ARAY[riCST].GetCntStat(csUnkwn)==0) LotInfoAdd();
                  //이미 타기 전에 웨이퍼 감지 센서에서 확인해서 에러 띄움.
                  //if(!IO_GetX(xSTG_TfWfDetect)){
                  //    EM_SetErr(eiSTG_WaferNotDetected);
                  //    MoveMotr(miSTG_XTrs , piSTG_XTrsWait);
                  //    Step.iCycle = 0;
                  //    return true;
                  //}
                  MoveMotr(miSTG_XTrs , piSTG_XTrsPick);
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveMotr(miSTG_XTrs , piSTG_XTrsPick)) return false;
                  FindChip(riCST , r , c , csMask);
                  //g_bLocking = true ; //나중에  MAPU 안으로 넣을수 있도록 하자.
                  DM.ARAY[riCST].SetStat(r , 0 , csWork);
                  DM.ARAY[riSTG].SetStat(csNone);
                  DM.ARAY[riMAP].SetStat(csNone);
                  //g_bLocking = false ; //나중에  MAPU 안으로 넣을수 있도록 하자.
                  HED.MarkData.ClearData();
                  if(MM.GetManNo() == mcSTG_CyclePlace || OM.CmnOptn.bLoadingStop) MoveMotr(miSTG_XTrs , piSTG_XTrsWait    ) ;
                  else                                                             MoveMotr(miSTG_XTrs , piSTG_XTrsPickWait) ;
                  Step.iCycle++;
                  return false;

        case  19: if(MM.GetManNo() == mcSTG_CyclePlace || OM.CmnOptn.bLoadingStop) {if(!MoveMotr(miSTG_XTrs , piSTG_XTrsWait    )) return false; }
                  else                                                             {if(!MoveMotr(miSTG_XTrs , piSTG_XTrsPickWait)) return false; }
                  IO_SetY(ySTG_Ionizer , false);
                  HED.Rqst.bNeedTAlign = true;
                  Step.iCycle = 0;
                  return true;
    }
}

bool CStage::CycleUnExpend(void)
{
//Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //이런거 에러명 바꿀수 있게 해서 사이클 이름으로 넣어주면 좋겟네...
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

    //f(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true;
    //}

    int r,c ;

    static int    iInspStep = 0 ;
    const  double dInspGap  = 2 ;
    const  int    iInspCnt  = 9 ; //3X3 = 9      ''
    double dMoveX , dMoveY , dMoveT ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(ySTG_WfrVac , false);
                  MoveActr(aiSTG_ChkTblUD , ccBwd);
                  MoveActr(aiSTG_TrsCmpGR , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiSTG_ChkTblUD , ccBwd)) return false ;
                  if(!MoveActr(aiSTG_TrsCmpGR , ccBwd)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
        }
}




void CStage::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CStage::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CStage::CheckStop()
{
    if(!MT_GetStop(miSTG_TStg)) return false ;
    if(!MT_GetStop(miSTG_XTrs)) return false ;
    if(!MT_GetStop(miSTG_ZCst)) return false ;

    if(!AT_Done(aiSTG_ChkTblUD )) return false ;
    if(!AT_Done(aiSTG_TrsCmpGR )) return false ;


    return true ;
}

void CStage::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_sWorkingWfrId  " , m_sWorkingWfrId   );

    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_sWorkingWfrId  " , m_sWorkingWfrId   );
    }
    //m_sWorkingWfrId = "sunkyewon1" ;
}
//---------------------------------------------------------------------------

bool CStage::LotInfoAdd()
{
    LT.AddDayInfoWorkStrp(1);
    LT.AddLotInfoWrkWafer(1);

    LT.AddDayInfoFailChip(DM.ARAY[riMAP].GetCntStat(csFail));
    LT.AddLotInfoFailChip(DM.ARAY[riMAP].GetCntStat(csFail));

    LT.AddDayInfoMarkChip(DM.ARAY[riSTG].GetCntStat(csWork));
    LT.AddLotInfoMarkChip(DM.ARAY[riSTG].GetCntStat(csWork));

    LT.WriteLotLog();
    LT.WriteLotDayLog();
    LT.LotInfo.dEndTime = Now();
}



