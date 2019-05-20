//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Head.h"


#include <math.hpp>


#include "Sequence.h"
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "LotUnit.h"
#include "SVMaths.h"
#include "ManualMan.h"
#include "FormOperation.h"

//---------------------------------------------------------------------------
#include "Stage.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CHead HED;

CHead::CHead()
{
    Init();
}
CHead::~CHead (void)
{
    Close();
}

//기본적으로 세타 결과값을 사용 안할때 비젼결과값을 모터 포지션에 더해 주면.
//보정이 되게 비젼에서 값을 맞춘후 사용.
//--------------------------------------------------------------------------
bool CHead::GetAdjustPos(double  _CrtX , double  _CrtY , double  _CrtT , //엔코더 값들....//계원꺼.
                         double  _OfsX , double  _OfsY , double  _OfsT , //비젼 결과 오프셑값들.
                         double &_RltX , double &_RltY , double &_RltT , //결과값. <===이걸 가지고 때려주면 된다.
                         bool    _bUseTheta)                             //세타보정 사용 여부.
{
    //Local Var.
    double PosX = 0.0 ;
    double PosY = 0.0 ;
    double PosT = 0.0 ;

    if(_bUseTheta){ //쎄타보정 들어갈때.
        PosX = _OfsX + _CrtX - PM.GetValue(miHED_XHed , pvHED_XHedHWCntrPos) ;  // 나중에 확인 하고 홈 옵셋 X,Y 를 더해준다.     //sun
        PosY = _OfsY + _CrtY - PM.GetValue(miHED_YHed , pvHED_YHedHWCntrPos) ;
        PosT = DegToRad(_OfsT) ;

        //X,Y축 방향에 따라 기호가 반대로 바뀐다.
        //다이본더 210c용.
        //_RltX   = PosX * cos(PosT) - PosY * sin(PosT) + OM.MstOptn.dStgXCt;
        //_RltY   = PosX * sin(PosT) + PosY * cos(PosT) + OM.MstOptn.dStgYCt;
        _RltX   = PosX * cos(PosT) - PosY * sin(PosT) + PM.GetValue(miHED_XHed , pvHED_XHedHWCntrPos);  //반대.
        _RltY   = PosX * sin(PosT) + PosY * cos(PosT) + PM.GetValue(miHED_YHed , pvHED_YHedHWCntrPos);  //같음.

        _RltT   = _CrtT + _OfsT ;
    }

    else{ //쎄타보정 안들어 갈때.
        _RltX = _CrtX + _OfsX;  // 나중에 확인 하고 홈 옵셋 X,Y 를 더해준다.     //sun
        _RltY = _CrtY + _OfsY;
        _RltT = _CrtT     ;
    }
    return true ;
}

//------------------------------------------------------------------------

void CHead::Init()
{
    m_sPartName    = "Head " ;

    InitCycleName();

    m_iHomeTO  = (int)eiHED_HomeTO    ;
    m_iStartTO = (int)eiHED_ToStartTO ;
    m_iStopTO  = (int)eiHED_ToStopTO  ;
    m_iCycleTO = (int)eiHED_CycleTO   ;

    memset(&Rqst    , 0 , sizeof(SRqst ));

    memset(&Time    , 0 , sizeof(SCycleTime) * MAX_SEQ_CYCLE) ;

    Reset();
    Load(true);

    m_iMarkTraceCnt = 0 ;

    DM.ARAY[riSTG].CallBackReg(&SetClickMove);

    m_bNeedWideWfrChipInsp = false ;
    m_bNeedWideWfoChipInsp = false ;
}

void CHead::Close()
{
    Load(false);
}

void CHead::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));

    m_bNeedWideWfrChipInsp = false ; //광역검사.
    m_bNeedWideWfoChipInsp = false ;
}

void CHead::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();

    //Vision
    m_tmVisn    .Clear();
    m_tmVisnTemp.Clear();

}

void CHead::SetStartTime(EN_SEQ_CYCLE _iSeqCycle)
{
    static double dPreSttTime[MAX_SEQ_CYCLE] ;

    double dNow = GetTickTime();
    int iPitch;

    dPreSttTime[_iSeqCycle] = Time[_iSeqCycle].dStart ;
    Time[_iSeqCycle].dStart = dNow ;
    iPitch = Time[_iSeqCycle].dStart - dPreSttTime[_iSeqCycle] ;
    Time[_iSeqCycle].dPitch = iPitch;
    //Time[_iSeqCycle].dPitch = Time[_iSeqCycle].dStart - dPreSttTime[_iSeqCycle] ;
}

void CHead::SetEndTime  (EN_SEQ_CYCLE _iSeqCycle)
{
    double dNow = GetTickTime();
    int iWork;

    iWork = dNow - Time[_iSeqCycle].dStart ;
    Time[_iSeqCycle].dWork = iWork ;
    //Time[_iSeqCycle].dWork = dNow - Time[_iSeqCycle].dStart ;
}

int GetStgFrstChipRow()
{
    // 마스크에서 첫 시작하는 good fail 위치
    int iFrstGoodR , iFrstGoodC ;
    int iFrstFailR , iFrstFailC ;
    int r,c ;

    bool bRet1 = DM.ARAY[riSTG].FindFrstRowCol(csGood , iFrstGoodR , iFrstGoodC);
    bool bRet2 = DM.ARAY[riSTG].FindFrstRowCol(csFail , iFrstFailR , iFrstFailC);
    if(!bRet1){
        iFrstGoodR = DM.ARAY[riSTG].GetMaxRow() ;
        iFrstGoodC = DM.ARAY[riSTG].GetMaxCol() ;
    }
    if(!bRet2){
        iFrstFailR = DM.ARAY[riSTG].GetMaxRow() ;
        iFrstFailC = DM.ARAY[riSTG].GetMaxCol() ;
    }

    if(iFrstGoodR == iFrstFailR) { //첫열 혹은 같은 열에 같이 있을때
        if(iFrstGoodC > iFrstFailC) {r = iFrstFailR ; c = iFrstFailC ; }//페일이 더 앞행에 있음ㄴ 페일
        else                        {r = iFrstGoodR ; c = iFrstGoodC ; }//아님 굿.
    }
    else if(iFrstGoodR < iFrstFailR) {
        r = iFrstGoodR ; c = iFrstGoodC ;
    }
    else {
        r = iFrstFailR ; c = iFrstFailC ;
    }
    return r ;
}

int GetStgFrstChipCol()
{
    // 마스크에서 첫 시작하는 good fail 위치
    int iFrstGoodR , iFrstGoodC ;
    int iFrstFailR , iFrstFailC ;
    int r,c ;

    bool bRet1 = DM.ARAY[riSTG].FindFrstRowCol(csGood , iFrstGoodR , iFrstGoodC);
    bool bRet2 = DM.ARAY[riSTG].FindFrstRowCol(csFail , iFrstFailR , iFrstFailC);
    if(!bRet1){
        iFrstGoodR = DM.ARAY[riSTG].GetMaxRow() ;
        iFrstGoodC = DM.ARAY[riSTG].GetMaxCol() ;
    }
    if(!bRet2){
        iFrstFailR = DM.ARAY[riSTG].GetMaxRow() ;
        iFrstFailC = DM.ARAY[riSTG].GetMaxCol() ;
    }

    if(iFrstGoodR == iFrstFailR) { //첫열 혹은 같은 열에 같이 있을때
        if(iFrstGoodC > iFrstFailC) {r = iFrstFailR ; c = iFrstFailC ; }//페일이 더 앞행에 있음ㄴ 페일
        else                        {r = iFrstGoodR ; c = iFrstGoodC ; }//아님 굿.
    }
    else if(iFrstGoodR < iFrstFailR) {
        r = iFrstGoodR ; c = iFrstGoodC ;
    }
    else {
        r = iFrstFailR ; c = iFrstFailC ;
    }
    return c ;
}

bool CHead::FindChip(EN_ARAY_ID _riWhre , int &r , int &c , EN_CHIP_STAT _csStat)
{
    int r1,c1;
    bool bRet = false;

    int iErorAreaCol[2]; //웨이퍼를 총 4등분 하여 가운데 빼고 4군데 작업한다.
    int iErorAreaRow[2];

    if(_riWhre == riSTG) {
        if(_csStat == csFail) {  //맵파일에 나타나 있는 마킹 할 영역.

            r1 = DM.ARAY[_riWhre].FindFrstRow(_csStat) ;
            //마스크 영역이 3줄이던 2줄이던 맵파일 작업은 왼쪽 부터 시작 하게 함.
            if((r1-GetStgFrstChipRow())%2) DM.ARAY[_riWhre].FindFrstRowLastCol(_csStat , r1 , c1) ;
            else                                          DM.ARAY[_riWhre].FindFrstRowCol    (_csStat , r1 , c1) ;
            r = r1 ;
            c = c1 ;
            return (r > -1 && c > -1);
        }
        else if(_csStat == csEror) { //맵파일 상에 없는 웨이퍼 테두리 마스크 영역.
            //테두리 마스크 부터 선작업 하고 테두리 마스킹 하는 영역을 마킹 할때 웨이퍼 좌우로 와따갔다 하지 않게 손씀.
            //Eror영역을 총 4개의 영역으로 나눔. iFrstActivRow , iFrstActivCol , 웨이퍼 3등분 포인트 등을 이용.
            iErorAreaCol[0] = GetStgFrstChipCol() ; iErorAreaCol[1] = DM.ARAY[riSTG].GetMaxCol() * 2 / 3 ;
            iErorAreaRow[0] = DM.ARAY[riSTG].GetMaxRow() * 1 / 3 ; iErorAreaRow[1] = DM.ARAY[riSTG].GetMaxRow() * 2 / 3 ;

            //첫번째 영역 첫번째칩 위쪽.
            c1 = DM.ARAY[_riWhre].FindFrstCol(_csStat , 0 , iErorAreaRow[0] , iErorAreaCol[0] , iErorAreaCol[1]);
            if(c1%2) {
                if(DM.ARAY[_riWhre].FindFrstColLastRow(_csStat , r , c , 0 , iErorAreaRow[0] , iErorAreaCol[0] , iErorAreaCol[1])) return true ;
            }
            else     {
                if(DM.ARAY[_riWhre].FindFrstColRow    (_csStat , r , c , 0 , iErorAreaRow[0] , iErorAreaCol[0] , iErorAreaCol[1])) return true ;
            }

            //발견 안됐으면 웨이퍼 오른쪽.
            r1 = DM.ARAY[_riWhre].FindFrstRow(_csStat , 0 , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[1] , DM.ARAY[riSTG].GetMaxCol()-1);
            if(r1%2) {
                if(DM.ARAY[_riWhre].FindFrstRowLastCol(_csStat , r , c , 0 , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[1] , DM.ARAY[riSTG].GetMaxCol()-1)) return true ;
            }
            else     {
                if(DM.ARAY[_riWhre].FindFrstRowCol    (_csStat , r , c , 0 , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[1] , DM.ARAY[riSTG].GetMaxCol()-1)) return true ;
            }

            //발견 안됐으면 웨이퍼 아래쪽.
            c1 = DM.ARAY[_riWhre].FindLastCol(_csStat , iErorAreaRow[1] , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[0] , iErorAreaCol[1]);
            if(c1%2) {
                if(DM.ARAY[_riWhre].FindLastColFrstRow(_csStat , r , c , iErorAreaRow[1] , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[0] , iErorAreaCol[1])) return true ;
            }
            else     {
                if(DM.ARAY[_riWhre].FindLastColRow    (_csStat , r , c , iErorAreaRow[1] , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[0] , iErorAreaCol[1])) return true ;
            }

            //발견 안됐으면 웨이퍼 왼쪽.
            r1 = DM.ARAY[_riWhre].FindLastRow(_csStat , 0 , DM.ARAY[riSTG].GetMaxRow()-1 , 0 , iErorAreaCol[1] );
            if(r1%2) {
                if(DM.ARAY[_riWhre].FindLastRowFrstCol(_csStat , r , c , 0 , DM.ARAY[riSTG].GetMaxRow()-1 , 0 , iErorAreaCol[1])) return true ;
            }
            else     {
                if(DM.ARAY[_riWhre].FindLastRowCol    (_csStat , r , c , 0 , DM.ARAY[riSTG].GetMaxRow()-1 , 0 , iErorAreaCol[1])) return true ;
            }

            return false ;
        }
    }
    else if(_riWhre == riBRD) {

        DM.ARAY[_riWhre].FindFrstRowCol    (_csStat , r1 , c1) ;

        r = r1 ;
        c = c1 ;
        return (r > -1 && c > -1);
    }
    else if(_riWhre == riMSK) {
        DM.ARAY[_riWhre].FindFrstRowCol    (_csStat , r1 , c1) ;

        r = r1 ;
        c = c1 ;
        return (r > -1 && c > -1);
    }

    return bRet ;
}

double GetChipShortSide()
{
    if(HED.m_bFnshCycTAlgn){
        if(HED.m_dRevisePitchX < HED.m_dRevisePitchY) return HED.m_dRevisePitchX ;

        return HED.m_dRevisePitchY ;
    }
    else{
        if(OM.DevInfo.dWfPitchX < OM.DevInfo.dWfPitchY) return OM.DevInfo.dWfPitchX ;

        return OM.DevInfo.dWfPitchY ;
    }
}

//위치가 얼라인 데이터 영역 안인지 검사.
/*
bool CheckInConfirmArea(int _iR , int _iC) //짧은변 피치 *iAlignConfirmChip  미만 이면 데이터 영역안.
{
    double dRadius = OM.DevOptn.iAlignConfirmChip * GetChipShortSide() ;
    double dXGap ;
    double dYGap ;
    double dDist ;

    for(int r = 0 ; r < DM.ARAY[riSTG].GetMaxRow() ; r++) {
        for(int c = 0 ; c < DM.ARAY[riSTG].GetMaxCol() ; c++) {
            if(DM.ARAY[riSTG].GetTemp(r,c,0) || DM.ARAY[riSTG].GetTemp(r,c,1)){
                dXGap = (_iC - c) * OM.DevInfo.dWfPitchX  ;
                dYGap = (_iR - r) * OM.DevInfo.dWfPitchY ;
                dDist = sqrt(pow(dXGap , 2) + pow(dYGap , 2)) ;
                if(dDist < dRadius) return true ;
            }
        }
    }
    return false ;
}
*/

//현재 위치에서 가장 가까운 작업 위치를 찾아서 리턴.
bool GetHopJumpChipPos(int &_iTrgR , int &_iTrgC , int &_iDataR , int &_iDataC)
{
    int iMinC = -1 ;
    int iMinR = -1 ;
    double dMinDist = ~(1<<(sizeof(double)*8-1));

    double dXGap , dXMinGap ;
    double dYGap , dYMinGap ;
    double dDist ;

    for(int r = 0 ; r < DM.ARAY[riSTG].GetMaxRow() ; r++){
        for(int c = 0 ; c < DM.ARAY[riSTG].GetMaxCol() ; c++){
            if(DM.ARAY[riSTG].GetStat(r, c) == csFail){
                if(_iTrgC == 0 && _iTrgR == 0){
                    _iDataR = r ;
                    _iDataC = c ;
                    return true;
                }
                else{
                    if(HED.m_bFnshCycTAlgn){
                        dXGap = (_iTrgC - c) * HED.m_dRevisePitchX  ;
                        dYGap = (_iTrgR - r) * HED.m_dRevisePitchY ;
                    }
                    else{
                        dXGap = (_iTrgC - c) * OM.DevInfo.dWfPitchX  ;
                        dYGap = (_iTrgR - r) * OM.DevInfo.dWfPitchY ;
                    }


                    dDist = sqrt(pow(dXGap , 2) + pow(dYGap , 2)) ;
                    if(dDist < dMinDist) {
                        dMinDist  = dDist ;
                        iMinC = c ;
                        iMinR = r ;
                        dXMinGap = dXGap ;
                        dYMinGap = dYGap ;
                    }
                }
            }
        }
    }
    if(iMinC != -1 && iMinR != -1) {
        _iDataC = iMinC ;
        _iDataR = iMinR ;
        return true ;
    }

    return true;
}

//작업 하려는 위치를 주고 거기서 가장 가까운 데이터 참고 좌표를 리턴.
bool GetCloseDataPos(int _iTrgR , int _iTrgC , int &_iDataR , int &_iDataC)
{
    _iDataR = 0 ;
    _iDataC = 0 ;
    int iMinC = -1 ;
    int iMinR = -1 ;
    double dMinDist = ~(1<<(sizeof(double)*8-1));
    //double dRadius = OM.DevOptn.iAlignConfirmChip * GetChipShortSide() ;

    double dXGap , dXMinGap ;// , dMinXPos;
    double dYGap , dYMinGap ;// , dMinYPos;
    double dDist ;

    for(int r = 0 ; r < DM.ARAY[riSTG].GetMaxRow() ; r++) {
        for(int c = 0 ; c < DM.ARAY[riSTG].GetMaxCol() ; c++) {
            if(DM.ARAY[riSTG].GetTemp(r,c,0) || DM.ARAY[riSTG].GetTemp(r,c,1)){
                //todo 확인.
                if(HED.m_bFnshCycTAlgn){
                    dXGap = (_iTrgC - c) * HED.m_dRevisePitchX  ;
                    dYGap = (_iTrgR - r) * HED.m_dRevisePitchY ;
                }
                else{
                    dXGap = (_iTrgC - c) * OM.DevInfo.dWfPitchX  ;
                    dYGap = (_iTrgR - r) * OM.DevInfo.dWfPitchY ;
                }

                dDist = sqrt(pow(dXGap , 2) + pow(dYGap , 2)) ;
                if(dDist < dMinDist) {
                    dMinDist  = dDist ;
                    iMinC = c ;
                    iMinR = r ;
                    dXMinGap = dXGap ;
                    dYMinGap = dYGap ;
                }
            }
        }
    }

    if(iMinC != -1 && iMinR != -1) {
        _iDataC = iMinC ;
        _iDataR = iMinR ;
        return true ;
    }
    return false ;
}

//지금 작업 하려는 위치를 주고 중간에 얼라인.
//데이터 찍을 위치 , 데이터 기준점을 리턴.
/*
bool IsChipAlignZone(int _iR , int _iC) //마스크어레이 에서 칩 얼라인 존을 확인 함. 스테이지로 확인 하면 작업 상태 csEror => csWork로 바뀌어서 안됌.
{
    //공백 구간 혹은 외각테두리 마스크 영역이 아니면 Ok
    if(DM.ARAY[riMSK].GetStat(_iR , _iC) != csNone && DM.ARAY[riMSK].GetStat(_iR , _iC) != csEror)return true ;

    return false ;
}*/

bool IsChipAlignZone(int _iR , int _iC) //마스크어레이 에서 칩 얼라인 존을 확인 함. 스테이지로 확인 하면 작업 상태 csEror => csWork로 바뀌어서 안됌.
{
    //공백 구간 혹은 외각테두리 마스크 영역이 아니면 Ok
    if(_iR < 0 || _iC < 0) return false ;
    if(_iR >= DM.ARAY[riMSK].GetMaxRow() || _iC >= DM.ARAY[riMSK].GetMaxCol()) return false ;

    if(DM.ARAY[riMSK].GetStat(_iR , _iC) != csNone && DM.ARAY[riMSK].GetStat(_iR , _iC) != csEror)return true ;

    return false ;
}

//_iDataR,C 는 얼라인시 참고할 데이터 위치,.
//마지막 작업한 위치 기준으로 상,상우,우,하우,하,하좌,좌,좌상 검사해서 얼라인 할곳이 있으면 리턴
//없으면 전체 웨이퍼 다 검색하여... 빈곳으로 이동.

bool GetAlignRowCol(int &_iAlgnR , int &_iAlgnC, int &_iDataR , int &_iDataC)
{
    //마지막 칩 주변에 있을 때...
    _iDataC = HED.m_iLastInspChipC; // HED.m_iLastInspChipC 서플라이시에 에 0이 되고 CycleOrigin , CycleChipAlign  시에 셑팅 된다.
    _iDataR = HED.m_iLastInspChipR;

    //IsOutSide는 칩이 csNone이 아닌 최외곽칩 + csNone이 아닌칩
    //상
    if(        IsChipAlignZone(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC)     &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC , 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC , 1) ){
        _iAlgnR = _iDataR - OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC                                ;
        return true;
    }
    //상우
    if(        IsChipAlignZone(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC + OM.DevOptn.iVisnChkChipColCnt    ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC + OM.DevOptn.iVisnChkChipColCnt , 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC + OM.DevOptn.iVisnChkChipColCnt , 1) ){
        _iAlgnR = _iDataR - OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC + OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //우
    if(        IsChipAlignZone(_iDataR , _iDataC + OM.DevOptn.iVisnChkChipColCnt   ) &&//!IsOutSide(_iDataR , _iDataC + OM.DevOptn.iVisnChkChipColCnt                ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR , _iDataC + OM.DevOptn.iVisnChkChipColCnt, 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR , _iDataC + OM.DevOptn.iVisnChkChipColCnt, 1)){
        _iAlgnR = _iDataR                                ;
        _iAlgnC = _iDataC + OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //하우
    if(        IsChipAlignZone(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC + OM.DevOptn.iVisnChkChipColCnt    ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC + OM.DevOptn.iVisnChkChipColCnt , 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC + OM.DevOptn.iVisnChkChipColCnt , 1) ){
        _iAlgnR = _iDataR + OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC + OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //하
    if(        IsChipAlignZone(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC    ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC , 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC , 1) ){
        _iAlgnR = _iDataR + OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC                                ;
        return true;
    }
    //하좌
    if(        IsChipAlignZone(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC - OM.DevOptn.iVisnChkChipColCnt    ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC - OM.DevOptn.iVisnChkChipColCnt , 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC - OM.DevOptn.iVisnChkChipColCnt , 1)){
        _iAlgnR = _iDataR + OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC - OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //좌.
    if(        IsChipAlignZone(_iDataR , _iDataC - OM.DevOptn.iVisnChkChipColCnt   ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR , _iDataC - OM.DevOptn.iVisnChkChipColCnt, 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR , _iDataC - OM.DevOptn.iVisnChkChipColCnt, 1)){
        _iAlgnR = _iDataR                                ;
        _iAlgnC = _iDataC - OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //좌상.
    if(        IsChipAlignZone(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC - OM.DevOptn.iVisnChkChipColCnt   ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC - OM.DevOptn.iVisnChkChipColCnt, 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC - OM.DevOptn.iVisnChkChipColCnt, 1)){
        _iAlgnR = _iDataR - OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC - OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //외통수 만났을때...
    //위로 이동.
    for(int r = 0 ; r < DM.ARAY[riSTG].GetMaxRow() ; r++) {
        for(int c = 0 ; c < DM.ARAY[riSTG].GetMaxCol() ; c++){
            if(IsChipAlignZone(r,c) &&
               DM.ARAY[riSTG].GetStat(r,c)!=csNone &&
               DM.ARAY[riSTG].GetStat(r,c)!=csEror ){
                if(DM.ARAY[riSTG].GetTemp(r, c, 0)==0.0 && DM.ARAY[riSTG].GetTemp(r, c, 1)==0.0){
                    if(DM.ARAY[riSTG].GetTemp(r + OM.DevOptn.iVisnChkChipRowCnt , c, 0)!=0.0 ||
                       DM.ARAY[riSTG].GetTemp(r + OM.DevOptn.iVisnChkChipRowCnt , c, 1)!=0.0 ){
                        _iDataR = r+OM.DevOptn.iVisnChkChipRowCnt;
                        _iDataC = c ;
                        _iAlgnR = r ;
                        _iAlgnC = c ;
                        return true;
                    }
                }
            }
        }
    }

    //오른쪽으로 이동.
    for(int r = 0 ; r < DM.ARAY[riSTG].GetMaxRow() ; r++) {
        for(int c = 0 ; c < DM.ARAY[riSTG].GetMaxCol() ; c++){
            if(IsChipAlignZone(r,c) &&
               DM.ARAY[riSTG].GetStat(r,c)!=csNone &&
               DM.ARAY[riSTG].GetStat(r,c)!=csEror ){
                if(DM.ARAY[riSTG].GetTemp(r, c, 0)==0.0 && DM.ARAY[riSTG].GetTemp(r, c, 1)==0.0){
                    if(DM.ARAY[riSTG].GetTemp(r , c- OM.DevOptn.iVisnChkChipColCnt, 0)!=0.0 ||
                       DM.ARAY[riSTG].GetTemp(r , c- OM.DevOptn.iVisnChkChipColCnt, 1)!=0.0 ){
                        _iDataR = r ;
                        _iDataC = c - OM.DevOptn.iVisnChkChipColCnt ;
                        _iAlgnR = r ;
                        _iAlgnC = c ;
                        return true;
                    }
                }
            }
        }
    }

    //아래로 이동.
    for(int r = 0 ; r < DM.ARAY[riSTG].GetMaxRow() ; r++) {
        for(int c = 0 ; c < DM.ARAY[riSTG].GetMaxCol() ; c++){
            if(IsChipAlignZone(r,c) &&
               DM.ARAY[riSTG].GetStat(r,c)!=csNone &&
               DM.ARAY[riSTG].GetStat(r,c)!=csEror ){
                if(DM.ARAY[riSTG].GetTemp(r, c, 0)==0.0 && DM.ARAY[riSTG].GetTemp(r, c, 1)==0.0){
                    if(DM.ARAY[riSTG].GetTemp(r - OM.DevOptn.iVisnChkChipRowCnt , c, 0)!=0.0 ||
                       DM.ARAY[riSTG].GetTemp(r - OM.DevOptn.iVisnChkChipRowCnt , c, 1)!=0.0 ){
                        _iDataR = r - OM.DevOptn.iVisnChkChipColCnt;
                        _iDataC = c ;
                        _iAlgnR = r ;
                        _iAlgnC = c ;
                        return true;
                    }
                }
            }
        }
    }

    //왼쪽이동.
    for(int r = 0 ; r < DM.ARAY[riSTG].GetMaxRow() ; r++) {
        for(int c = 0 ; c < DM.ARAY[riSTG].GetMaxCol() ; c++){
            if(IsChipAlignZone(r,c) &&
               DM.ARAY[riSTG].GetStat(r,c)!=csNone &&
               DM.ARAY[riSTG].GetStat(r,c)!=csEror ){
                if(DM.ARAY[riSTG].GetTemp(r, c, 0)==0.0 && DM.ARAY[riSTG].GetTemp(r, c, 1)==0.0){
                    if(DM.ARAY[riSTG].GetTemp(r , c + OM.DevOptn.iVisnChkChipColCnt, 0)!=0.0 ||
                       DM.ARAY[riSTG].GetTemp(r , c + OM.DevOptn.iVisnChkChipColCnt, 1)!=0.0 ){
                        _iDataR = r ;
                        _iDataC = c + OM.DevOptn.iVisnChkChipColCnt ;
                        _iAlgnR = r ;
                        _iAlgnC = c ;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/*
    int iMinC = -1 ;
    int iMinR = -1 ;
    double dMinDist = ~(1<<(sizeof(double)*8-1));
    double dRadius = OM.DevOptn.iAlignConfirmChip * GetChipShortSide() ;

    double dXGap , dXMinGap ;// , dMinXPos;
    double dYGap , dYMinGap ;// , dMinYPos;
    double dDist ;

    for(int r = 0 ; r < DM.ARAY[riSTG].GetMaxRow() ; r++) {
        for(int c = 0 ; c < DM.ARAY[riSTG].GetMaxCol() ; c++) {
            if(DM.ARAY[riSTG].GetTemp(r,c,0) || DM.ARAY[riSTG].GetTemp(r,c,1)){
                //todo 확인.
                dXGap = (_iTrgC - c) * OM.DevInfo.dWfPitchX  ;
                dYGap = (_iTrgR - r) * OM.DevInfo.dWfPitchY ;
                dDist = sqrt(pow(dXGap , 2) + pow(dYGap , 2)) ;
                if(dDist < dMinDist) {
                    dMinDist  = dDist ;
                    iMinC = c ;         //제일 작은  Colunm
                    iMinR = r ;         //제일 작은  Row
                    dXMinGap = dXGap ;
                    dYMinGap = dYGap ;
                }
            }
        }
    }

    //X,Y데이터 있는 칩이 없다.
    if(iMinC == -1 || iMinR == -1) return false ;

    //_iDataR = iMinR ;
    //_iDataC = iMinC ;

    if(dMinDist <= dRadius){ //타겟 포지션이 데이터 반경안에 있는경우는 타겟포지션이 얼라인 포지션.
        _iAlgnR = _iTrgR ;
        _iAlgnC = _iTrgC ;
        return true ;
    }

/*
//dXMinGap(_iTrgC - c)가 양수면 오른쪽으로
//dYMinGap(_iTrgR - r)가 양수면 밑으로
//한개의 선분(pX1, pX2, pY1, pY2)를 가지고 원과의 접점을 구하는 공식
//return 0 = 접점이 없는 경우
//       1 = 접점이 1점인 경우
//       2 = 접점이 2점인 경우
//원과 선분의 교점.
int __stdcall MATH_GetCircleSegmentIntersection(float _fX1      , float _fY1      , float _fX2    , float _fY2 ,
                                                float _fRadius  , float _fCntrX   , float _fCntrY ,
                                                TFPoint *_pPnt1 , TFPoint *_pPnt2 )
*/
    //포지션 계산 어레이순서 기준이기에 오른쪽,아래쪽이 플러스
/*
    TFPoint fPnt1 ;
    TFPoint fPnt2 ;
    int     iRet  ;
    iRet = MATH_GetCircleSegmentIntersection( 0.0      , 0.0      , dXMinGap    , dYMinGap ,
                                              dRadius  , 0.0      , 0.0         ,
                                             &fPnt1    , &fPnt2   );

    _iAlgnR = iMinR + fPnt1.y / OM.DevInfo.dWfPitchY ;
    _iAlgnC = iMinC + fPnt1.x / OM.DevInfo.dWfPitchX  ;

    _iDataR = iMinR ;
    _iDataC = iMinC ;

    return true ;
}
*/


//---------------------------------------------------------------------------

/*
bool GetNonOutSide(int _iR , int _iC , int & _iRetR , int & _iRetC)
{
    if(!IsOutSide(_iR,_iC)) {
        _iRetR = _iR ;
        _iRetC = _iC ;
        return true ;
    }

    if(!IsOutSide(_iR-1,_iC)&& !DM.ARAY[riSTG].GetTemp(_iR-1,_iC,0) && !DM.ARAY[riSTG].GetTemp(_iR-1,_iC,1)) { _iRetR = _iR-1 ; _iRetC = _iC   ; return true ;}
    if(!IsOutSide(_iR+1,_iC)&& !DM.ARAY[riSTG].GetTemp(_iR+1,_iC,0) && !DM.ARAY[riSTG].GetTemp(_iR+1,_iC,1)) { _iRetR = _iR+1 ; _iRetC = _iC   ; return true ;}
    if(!IsOutSide(_iR,_iC-1)&& !DM.ARAY[riSTG].GetTemp(_iR,_iC-1,0) && !DM.ARAY[riSTG].GetTemp(_iR,_iC-1,1)) { _iRetR = _iR   ; _iRetC = _iC-1 ; return true ;}
    if(!IsOutSide(_iR,_iC+1)&& !DM.ARAY[riSTG].GetTemp(_iR,_iC+1,0) && !DM.ARAY[riSTG].GetTemp(_iR,_iC+1,1)) { _iRetR = _iR   ; _iRetC = _iC+1 ; return true ;}

    return false ;
}
*/

bool IsWaferZone(double _dPosX , double _dPosY , double _dWfDia)
{
    double dAreaPos  = _dWfDia/2.0 ;
    double dPos      = pow(dAreaPos,2);
    int    dX        ;
    int    dY        ;

    dX = pow(_dPosX - PM.GetValue(miHED_XHed , pvHED_XHedHWCntrPos),2); //pow(_dPosX - OM.MstOptn.dStgXEjtCt ,2);
    dY = pow(_dPosY - PM.GetValue(miHED_YHed , pvHED_YHedHWCntrPos),2); //pow(_dPosY - OM.MstOptn.dStgYEjtCt ,2);
    if(dPos < (dX + dY)) return false ;

    return true ;
}

double GetLeftEnd  (double _dCmdPosX,double _dCmdPosY, double _dWfDia)
{
    double dXPitch ;
    if(HED.m_bFnshCycTAlgn){ dXPitch = HED.m_dRevisePitchX  ; }
    else                   { dXPitch = OM.DevInfo.dWfPitchX ; }

    int    iWfrMaxCol = _dWfDia / dXPitch ;
    double dRetPos    = _dCmdPosX ;

    //웨이퍼 피치로 이동하여 끝에 있는 자제를 찾는다.
    for(int x = 1 ; x <= iWfrMaxCol ; x++ ) {
        if(IsWaferZone(_dCmdPosX + x* dXPitch , _dCmdPosY,_dWfDia)) {
            dRetPos = _dCmdPosX + x* dXPitch ;
        }
        else return dRetPos ;
    }
    return dRetPos ;
}

double GetRightEnd (double _dCmdPosX,double _dCmdPosY, double _dWfDia)
{
    double dXPitch ;
    if(HED.m_bFnshCycTAlgn){ dXPitch = HED.m_dRevisePitchX  ; }
    else                   { dXPitch = OM.DevInfo.dWfPitchX ; }

    int    iWfrMaxCol = _dWfDia / dXPitch ;
    double dRetPos    = _dCmdPosX ;

    //웨이퍼 피치로 이동하여 끝에 있는 자제를 찾는다.
    for(int x = 1 ; x <= iWfrMaxCol ; x++ ) {
        if(IsWaferZone(_dCmdPosX - x * dXPitch , _dCmdPosY,_dWfDia)) {
            dRetPos = _dCmdPosX - x * dXPitch ;
        }
        else return dRetPos ;
    }
    return dRetPos ;
}

double GetTopEnd   (double _dCmdPosX,double _dCmdPosY, double _dWfDia)
{
    double dYPitch ;
    if(HED.m_bFnshCycTAlgn) { dYPitch = HED.m_dRevisePitchY  ; }
    else                    { dYPitch = OM.DevInfo.dWfPitchY ; }
    int    iWfrMaxRow = _dWfDia / dYPitch ;
    double dRetPos    = _dCmdPosY ;

    //웨이퍼 피치로 이동하여 끝에 있는 자제를 찾는다.
    for(int y = 1 ; y <= iWfrMaxRow ; y++) {
        if(IsWaferZone(_dCmdPosX , _dCmdPosY - y* dYPitch,_dWfDia)) {
            dRetPos = _dCmdPosY - y* dYPitch ;
        }
        else return dRetPos ;
    }
    return dRetPos ;
}

double GetBottomEnd(double _dCmdPosX,double _dCmdPosY, double _dWfDia)
{
    double dYPitch ;
    if(HED.m_bFnshCycTAlgn) { dYPitch = HED.m_dRevisePitchY  ; }
    else                    { dYPitch = OM.DevInfo.dWfPitchY ; }
    int    iWfrMaxRow = _dWfDia / dYPitch ;
    double dRetPos    = _dCmdPosY ;

    //웨이퍼 피치로 이동하여 끝에 있는 자제를 찾는다.
    for(int y = 1 ; y <= iWfrMaxRow ; y++ ) {
        if(IsWaferZone(_dCmdPosX , _dCmdPosY + y* dYPitch,_dWfDia)) {
            dRetPos = _dCmdPosY + y* dYPitch ;
        }
        else return dRetPos ;
    }
    return dRetPos ;
}

bool VisnRsltLmtCheck(CVisionMan::TRetResult * _tRsltbool)
{
    if(HED.m_bFnshCycTAlgn){
        if( _tRsltbool->fRsltX > (HED.m_dRevisePitchX)/2 ||
            _tRsltbool->fRsltY > (HED.m_dRevisePitchY)/2 ||
            _tRsltbool->fRsltT > OM.CmnOptn.dStageTLmtErr ) {
            return false ;
        }
    }
    else{
        if( _tRsltbool->fRsltX > (OM.DevInfo.dWfPitchX)/2 ||
            _tRsltbool->fRsltY > (OM.DevInfo.dWfPitchY)/2 ||
            _tRsltbool->fRsltT > OM.CmnOptn.dStageTLmtErr ) {
            return false ;
        }
    }

    return true ;
}

double CHead::GetWaferPitchX  (double _dOffsetX) //보정 X 피치 구하는 함수
{
    double dXPitch = OM.DevInfo.dWfPitchX ;
    double dReviseX ;

    dReviseX = dXPitch + _dOffsetX ;

    return dReviseX ;
}

double CHead::GetWaferPitchY  (double _dOffsetY) //보정 Y 피치 구하는 함수
{
    double dYPitch = OM.DevInfo.dWfPitchY ;
    double dReviseY ;

    dReviseY = dYPitch + _dOffsetY ;

    return dReviseY ;
}

void CHead::SetClickMove(int _iCol , int _iRow , TShiftState Shift)
{
    m_iClickMoveCol = _iCol ;
    m_iClickMoveRow = _iRow ;

    if(Shift.Contains(ssCtrl)) {
        int iTrgR  ,iTrgC ;
        int iDataR ,iDataC;
        if(!GetCloseDataPos(iTrgR,iTrgC,iDataR,iDataC)) {FM_MsgTime("ERROR","스테이지 스타트 셋 또는 얼라인 셋을 확인 해주세요!",2000); return ;}
        MM.SetManCycle(mcHED_CycleClickMove);
    }
    if(Shift.Contains(ssAlt)) {
        String sTemp = "Col:"  + String(_iCol)+
                      " Row:"  + String(_iRow)+
                      " DataX:"+ String(DM.ARAY[riSTG].GetTemp(_iRow,_iCol,0)) +
                      " DataY:"+ String(DM.ARAY[riSTG].GetTemp(_iRow,_iCol,1));

        ShowMessage(sTemp) ;

    }
}


double CHead::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos  = 0.0 ;
    double dTempPosX = 0.0 ;
    double dTempPosY = 0.0 ;
    int r = -1 , c = -1 ;

    //보드 워크 포지션.---------------------------------------------------------
    FindChip(riBRD , r , c , csUnkwn);
    double dXHedVBoardPos    = PM.GetValue(miHED_XHed , pvHED_XHedVBoardStt) - OM.DevInfo.dBdXPch * c ; //todo
    double dYHedVBoardPos    = PM.GetValue(miHED_YHed , pvHED_YHedVBoardStt) + OM.DevInfo.dBdYPch * r ;
    double dXHedPBoardPos    = dXHedVBoardPos + PM.GetValue(miHED_XHed , poHED_XHedVisnPenOfs) ; //펜의 보드 워크 포지션.
    double dYHedPBoardPos    = dYHedVBoardPos + PM.GetValue(miHED_YHed , poHED_YHedVisnPenOfs) ;
    double dXHedPBoardSttPos = PM.GetValue(miHED_XHed , pvHED_XHedVBoardStt)+PM.GetValue(miHED_XHed , poHED_XHedVisnPenOfs) ; //펜의 보드 스타트 포지션.
    double dYHedPBoardSttPos = PM.GetValue(miHED_YHed , pvHED_YHedVBoardStt)+PM.GetValue(miHED_YHed , poHED_YHedVisnPenOfs) ;

    //얼라인 포지션.------------------------------------------------------------
    double dXHedAlignPos  ;
    double dYHedAlignPos  ;
    int    iAlignC , iAlignR; //얼라인 칠 칩 오와열.
    int    iDataC  , iDataR ; //데이터 참고할 칩 오와열.
    if(DM.ARAY[riSTG].GetCntStat(csEror)) { //마스크 영역 작업 하러 가는 사이.
        FindChip(riSTG , r , c , csEror);
    }
    else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //맵파일 페일 영역 작업 하러 가는 사이.
        FindChip(riSTG , r , c , csFail);
    }
    if(GetAlignRowCol(iAlignR, iAlignC, iDataR, iDataC)){

        dTempPosX = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) ; //X  값
        dTempPosY = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) ; //Y 값.
        if(m_bFnshCycTAlgn){

            dXHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (iAlignC - iDataC) * m_dRevisePitchX ;
            dYHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (iAlignR - iDataR) * m_dRevisePitchY ;
        }
        else{
            dXHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (iAlignC - iDataC) * OM.DevInfo.dWfPitchX ;
            dYHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (iAlignR - iDataR) * OM.DevInfo.dWfPitchY ;
        }
    }
    //if(GetAlignRowCol(r , c , iAlignR , iAlignC , iDataR , iDataC) &&
    //   GetNonOutSide(iAlignR , iAlignC , iAlignR , iAlignC )){  //r,c작업타겟,iAlignR , iAlignC얼라인할 타겟,iDataR , iDataC위치데이터 참고할 타겟.
    //    dXHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (iAlignC - iDataC) * OM.DevInfo.dWfPitchX  ;//모터방향에 따라 -+부호 바뀜.
    //    dYHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (iAlignR - iDataR) * OM.DevInfo.dWfPitchY ;//모터방향에 따라 -+부호 바뀜.
    //}
    else {
        dXHedAlignPos = MT_GetCmdPos(miHED_XHed);
        dYHedAlignPos = MT_GetCmdPos(miHED_YHed);
    }

    //클릭 위치..............................................................
    double dXHedClickPos  ;
    double dYHedClickPos  ;
    if(DM.ARAY[riSTG].GetCntStat(csEror)) { //마스크 영역 작업 하러 가는 사이.
        FindChip(riSTG , r , c , csEror);
    }
    else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //맵파일 페일 영역 작업 하러 가는 사이.
        FindChip(riSTG , r , c , csFail);
    }
    if(GetAlignRowCol(iAlignR, iAlignC, iDataR, iDataC)){
        if(m_bFnshCycTAlgn){
            dXHedClickPos = DM.ARAY[riSTG].GetTemp(r, c , 0) + (iAlignC - iDataC) * m_dRevisePitchX ;
            dYHedClickPos = DM.ARAY[riSTG].GetTemp(r, c , 1) + (iAlignR - iDataR) * m_dRevisePitchY ;
        }
        else{
            dXHedClickPos = DM.ARAY[riSTG].GetTemp(r, c , 0) + (iAlignC - iDataC) * OM.DevInfo.dWfPitchX ;
            dYHedClickPos = DM.ARAY[riSTG].GetTemp(r, c , 1) + (iAlignR - iDataR) * OM.DevInfo.dWfPitchY ;
        }
    }
    //if(GetAlignRowCol(m_iClickMoveRow , m_iClickMoveCol , iAlignR , iAlignC , iDataR , iDataC)){  //r,c작업타겟,iAlignR , iAlignC얼라인할 타겟,iDataR , iDataC위치데이터 참고할 타겟.
    //    dXHedClickPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (m_iClickMoveCol - iDataC) * OM.DevInfo.dWfPitchX  ;//모터방향에 따라 -+부호 바뀜.
    //    dYHedClickPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (m_iClickMoveRow - iDataR) * OM.DevInfo.dWfPitchY ;//모터방향에 따라 -+부호 바뀜.
    //}
    else {                                      //20141022 sun 이상해서 바꿈.
        dXHedClickPos= MT_GetCmdPos(miHED_XHed);//dXHedAlignPos = MT_GetCmdPos(miHED_XHed);
        dYHedClickPos= MT_GetCmdPos(miHED_YHed);//dYHedAlignPos = MT_GetCmdPos(miHED_YHed);
    }

    //펜 , 펜 체크 , 펜커버 포지션.---------------------------------------------
    double dXHedPenPos ;
    double dYHedPenPos ;
    double dXHedPenChkPos ;
    double dYHedPenChkPos ;

    if(DM.ARAY[riSTG].GetCntStat(csEror)) { //마스크 영역 작업 하러 가는 사이.
        FindChip(riSTG , r , c , csEror);
    }
    else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //맵파일 페일 영역 작업 하러 가는 사이.
        FindChip(riSTG , r , c , csFail);
    }
    if(OM.MstOptn.bHopJumpChip && !DM.ARAY[riSTG].GetCntStat(csEror)){GetHopJumpChipPos(m_iLastMrkChipR , m_iLastMrkChipC , r, c);}
    if(GetCloseDataPos(r , c , iDataR , iDataC)) {   //r,c작업타겟,iDataR , iDataC위치데이터 참고할 타겟.
        if(m_bFnshCycTAlgn){
            dXHedPenChkPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (c - iDataC) * m_dRevisePitchX + PM.GetValue(miHED_XHed , poHED_XHedChipPenOfs);//모터방향에 따라 -+부호 바뀜.
            dYHedPenChkPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (r - iDataR) * m_dRevisePitchY + PM.GetValue(miHED_YHed , poHED_YHedChipPenOfs);//모터방향에 따라 -+부호 바뀜.
        }
        else{
            dXHedPenChkPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (c - iDataC) * OM.DevInfo.dWfPitchX + PM.GetValue(miHED_XHed , poHED_XHedChipPenOfs);//모터방향에 따라 -+부호 바뀜.
            dYHedPenChkPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (r - iDataR) * OM.DevInfo.dWfPitchY + PM.GetValue(miHED_YHed , poHED_YHedChipPenOfs);//모터방향에 따라 -+부호 바뀜.
        }
        dXHedPenPos    = dXHedPenChkPos + PM.GetValue(miHED_XHed , poHED_XHedVisnPenOfs);
        dYHedPenPos    = dYHedPenChkPos + PM.GetValue(miHED_YHed , poHED_YHedVisnPenOfs);
    }
    else {                                   //20141022 sun 이상해서 바꿈.
        dXHedPenPos =MT_GetCmdPos(miHED_XHed); //dXHedAlignPos =
        dYHedPenPos =MT_GetCmdPos(miHED_YHed); //dYHedAlignPos =
    }

    //마크 트레이스 포지션......................................................
    double dXHedVMarkTracePos ;
    double dYHedVMarkTracePos ;

    r = MarkData.GetDataR(m_iMarkTraceCnt);
    c = MarkData.GetDataC(m_iMarkTraceCnt);

    if(GetCloseDataPos(r , c , iDataR , iDataC)) {
        if(m_bFnshCycTAlgn){
            dXHedVMarkTracePos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (c - iDataC) * m_dRevisePitchX + PM.GetValue(miHED_XHed , poHED_XHedChipPenOfs);//모터방향에 따라 -+부호 바뀜.
            dYHedVMarkTracePos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (r - iDataR) * m_dRevisePitchY + PM.GetValue(miHED_YHed , poHED_YHedChipPenOfs);//모터방향에 따라 -+부호 바뀜.
        }
        else{
            dXHedVMarkTracePos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (c - iDataC) * OM.DevInfo.dWfPitchX + PM.GetValue(miHED_XHed , poHED_XHedChipPenOfs);//모터방향에 따라 -+부호 바뀜.
            dYHedVMarkTracePos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (r - iDataR) * OM.DevInfo.dWfPitchY + PM.GetValue(miHED_YHed , poHED_YHedChipPenOfs);//모터방향에 따라 -+부호 바뀜.
        }
    }

    //T 얼라인 포지션.----------------------------------------------------------
    //double dHedXHedVLftEnd , dHedYHedVLftEnd ;
    //double dHedXHedVRgtEnd , dHedYHedVRgtEnd ;
    //double dHedXHedVTopEnd , dHedYHedVTopEnd ;
    //double dHedXHedVBtmEnd , dHedYHedVBtmEnd ;
    //
    //dHedXHedVLftEnd = GetLeftEnd (MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed)) ;
    //dHedXHedVRgtEnd = GetRightEnd(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed)) ;
    //dHedXHedVTopEnd = MT_GetCmdPos(miHED_YHed) ;
    //dHedXHedVBtmEnd = MT_GetCmdPos(miHED_YHed) ;
    //
    //dHedYHedVLftEnd = MT_GetCmdPos(miHED_XHed) ;
    //dHedYHedVRgtEnd = MT_GetCmdPos(miHED_XHed) ;
    //dHedYHedVTopEnd = GetTopEnd (MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed)) ;
    //dHedYHedVBtmEnd = GetBtmEnd (MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed)) ;

    if(_iMotr == miHED_XHed){
        switch(_iPstnId) {
            default                   : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piHED_XHedWait       : dPos = PM.GetValue (_iMotr , pvHED_XHedWait      ); break ;
            case piHED_XHedVTAlgn     : dPos = PM.GetValue (_iMotr , pvHED_XHedVTAlgn    ); break ;
            case piHED_XHedVOri       : dPos = PM.GetValue (_iMotr , pvHED_XHedVOri      ); break ;
            case piHED_XHedVCalPad    : dPos = PM.GetValue (_iMotr , pvHED_XHedVCalPad   ); break ;
            case piHED_XHedVBoardStt  : dPos = PM.GetValue (_iMotr , pvHED_XHedVBoardStt ); break ;
            case piHED_XHedPCover     : dPos = PM.GetValue (_iMotr , pvHED_XHedPCover    ); break ;
            case piHED_XHedHWCntrPos  : dPos = PM.GetValue (_iMotr , pvHED_XHedHWCntrPos ); break ;
            case piHED_XHedVBoard     : dPos = dXHedVBoardPos                             ; break ;
            case piHED_XHedVChipAlign : dPos = dXHedAlignPos                              ; break ;
            case piHED_XHedPWrk       : dPos = dXHedPenPos                                ; break ;
            case piHED_XHedVWrk       : dPos = dXHedPenChkPos                             ; break ;
            case piHED_XHedPBoardStt  : dPos = dXHedPBoardSttPos                          ; break ;
            case piHED_XHedPBoard     : dPos = dXHedPBoardPos                             ; break ;
            case piHED_XHedVClick     : dPos = dXHedClickPos                              ; break ;
            case piHED_XHedVMarkTrace : dPos = dXHedVMarkTracePos                         ; break ;
        }
    }
    else if(_iMotr == miHED_YHed){
        switch(_iPstnId) {
            default                   : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piHED_YHedWait       : dPos = PM.GetValue (_iMotr , pvHED_YHedWait      ); break ;
            case piHED_YHedVTAlgn     : dPos = PM.GetValue (_iMotr , pvHED_YHedVTAlgn    ); break ;
            case piHED_YHedVOri       : dPos = PM.GetValue (_iMotr , pvHED_YHedVOri      ); break ;
            case piHED_YHedVCalPad    : dPos = PM.GetValue (_iMotr , pvHED_YHedVCalPad   ); break ;
            case piHED_YHedVBoardStt  : dPos = PM.GetValue (_iMotr , pvHED_YHedVBoardStt ); break ;
            case piHED_YHedPCover     : dPos = PM.GetValue (_iMotr , pvHED_YHedPCover    ); break ;
            case piHED_YHedHWCntrPos  : dPos = PM.GetValue (_iMotr , pvHED_YHedHWCntrPos ); break ;
            case piHED_YHedVBoard     : dPos = dYHedVBoardPos                             ; break ;
            case piHED_YHedVChipAlign : dPos = dYHedAlignPos                              ; break ;
            case piHED_YHedPWrk       : dPos = dYHedPenPos                                ; break ;
            case piHED_YHedVWrk       : dPos = dYHedPenChkPos                             ; break ;
            case piHED_YHedPBoardStt  : dPos = dYHedPBoardSttPos                          ; break ;
            case piHED_YHedPBoard     : dPos = dYHedPBoardPos                             ; break ;
            case piHED_YHedVClick     : dPos = dYHedClickPos                              ; break ;
            case piHED_YHedVMarkTrace : dPos = dYHedVMarkTracePos                         ; break ;
        }
    }
    else if(_iMotr == miHED_ZVsn){
        switch(_iPstnId){
            default                  : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piHED_ZVsnWait      : dPos = PM.GetValue (_iMotr , pvHED_ZVsnWait      ); break ;
            case piHED_ZVsnWork      : dPos = PM.GetValue (_iMotr , pvHED_ZVsnWork      ); break ;
        }
    }
    else if(_iMotr == miHED_TZom){
        switch(_iPstnId){
            default                  : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piHED_TZomWait      : dPos = PM.GetValue (_iMotr , pvHED_TZomWait      ); break ;
            case piHED_TZomWork      : dPos = PM.GetValue (_iMotr , pvHED_TZomWork      ); break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CHead::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool bZPenFwd = !AT_Complete(aiHED_PenMrkDU   , ccBwd);
    bool bZCovFwd = !AT_Complete(aiHED_PenCoverUD , ccBwd);

    if(_iMotr == miHED_XHed){
        if(bZPenFwd) { sMsg = "Pen Marking Cylinder is not Bwd" ; bRet = false ;}
        if(bZCovFwd) { sMsg = "Pen Cover Cylinder is not Bwd"   ; bRet = false ;}
        switch(_iPstnId) {
            default                   : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piHED_XHedWait       :                                                     break ;
            case piHED_XHedVTAlgn     :                                                     break ;
            case piHED_XHedVOri       :                                                     break ;
            case piHED_XHedVCalPad    :                                                     break ;
            case piHED_XHedVBoardStt  :                                                     break ;
            case piHED_XHedVBoard     :                                                     break ;
            case piHED_XHedVChipAlign :                                                     break ;
            case piHED_XHedPWrk       :                                                     break ;
            case piHED_XHedVWrk       :                                                     break ;
            case piHED_XHedPBoardStt  :                                                     break ;
            case piHED_XHedPBoard     :                                                     break ;
            case piHED_XHedVClick     :                                                     break ;
            case piHED_XHedPCover     :                                                     break ;
            case piHED_XHedHWCntrPos  :                                                     break ;
            case piHED_XHedVMarkTrace :                                                     break ;
        }
    }
    else if(_iMotr == miHED_YHed){
        if(bZPenFwd) { sMsg = "Pen Marking Cylinder is not Bwd" ; bRet = false ;}
        if(bZCovFwd) { sMsg = "Pen Cover Cylinder is not Bwd"   ; bRet = false ;}
        switch(_iPstnId) {
            default                   : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piHED_YHedWait       :                                                     break ;
            case piHED_YHedVTAlgn     :                                                     break ;
            case piHED_YHedVOri       :                                                     break ;
            case piHED_YHedVCalPad    :                                                     break ;
            case piHED_YHedVBoardStt  :                                                     break ;
            case piHED_YHedVBoard     :                                                     break ;
            case piHED_YHedVChipAlign :                                                     break ;
            case piHED_YHedPWrk       :                                                     break ;
            case piHED_YHedVWrk       :                                                     break ;
            case piHED_YHedPBoardStt  :                                                     break ;
            case piHED_YHedPBoard     :                                                     break ;
            case piHED_YHedVClick     :                                                     break ;
            case piHED_YHedPCover     :                                                     break ;
            case piHED_YHedHWCntrPos  :                                                     break ;
            case piHED_YHedVMarkTrace :                                                     break ;
        }
    }
    else if(_iMotr == miHED_ZVsn){
        switch(_iPstnId){
            default                  : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piHED_ZVsnWait      :                                                     break ;
            case piHED_ZVsnWork      :                                                     break ;
        }
    }
    else if(_iMotr == miHED_TZom){
        switch(_iPstnId){
            default                  : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piHED_TZomWait      :                                                     break ;
            case piHED_TZomWork      :                                                     break ;
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
bool CHead::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool bRet = true ;
    bool bHedMoving = !MT_GetStopInpos(miHED_XHed) || !MT_GetStopInpos(miHED_YHed);
   // bool bPenCover  = !AT_Complete(aiHED_PenCoverUD , ccFwd) && !AT_Complete(aiHED_PenMrkDU , ccBwd);
    bool bZPenFwd = !AT_Complete(aiHED_PenMrkDU   , ccBwd);
    bool bZCovFwd = !AT_Complete(aiHED_PenCoverUD , ccBwd);

    if(_iActr == aiHED_PenMrkDU  ){
        if(bHedMoving) { sMsg = "HED axis is Moving"           ; bRet = false;}
        if(bZCovFwd  ) { sMsg = "Pen Cover Cylinder is not Bwd"; bRet = false;}
    }
    else if(_iActr == aiHED_PenCoverUD){
        if(bHedMoving) { sMsg = "HED axis is Moving"           ; bRet = false;}
        if(bZPenFwd  ) { sMsg = "Pen Mark Cylinder is not Bwd" ; bRet = false;}
    }
    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CHead::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

//---------------------------------------------------------------------------
bool CHead::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

//---------------------------------------------------------------------------
bool CHead::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    bool bTest = CheckStop() ;
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

        case 10: AT_MoveCyl(aiHED_PenMrkDU   , ccBwd);
                 AT_MoveCyl(aiHED_PenCoverUD , ccBwd);
                 MT_SetHomeEnd(miHED_XHed,false);
                 MT_SetHomeEnd(miHED_YHed,false);
                 MT_SetHomeEnd(miHED_ZVsn,false);
                 MT_SetHomeEnd(miHED_TZom,false);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiHED_PenMrkDU   , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiHED_PenCoverUD , ccBwd)) return false ;
                 MT_DoHome(miHED_XHed);
                 MT_DoHome(miHED_YHed);
                 MT_DoHome(miHED_ZVsn);
                 MT_DoHome(miHED_TZom);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miHED_XHed))return false ;
                 if(!MT_GetHomeEnd(miHED_YHed))return false ;
                 if(!MT_GetHomeEnd(miHED_ZVsn))return false ;
                 if(!MT_GetHomeEnd(miHED_TZom))return false ;
                 MT_GoAbsMan(miHED_XHed , PM.GetValue(miHED_XHed , pvHED_XHedPCover));
                 MT_GoAbsMan(miHED_YHed , PM.GetValue(miHED_YHed , pvHED_YHedPCover));
                 MT_GoAbsMan(miHED_ZVsn , PM.GetValue(miHED_ZVsn , pvHED_ZVsnWork  ));
                 MT_GoAbsMan(miHED_TZom , PM.GetValue(miHED_TZom , pvHED_TZomWork  ));
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miHED_XHed , PM.GetValue(miHED_XHed , pvHED_XHedPCover))) return false ;
                 if(!MT_GoAbsMan(miHED_YHed , PM.GetValue(miHED_YHed , pvHED_YHedPCover))) return false ;
                 if(!MT_GoAbsMan(miHED_ZVsn , PM.GetValue(miHED_ZVsn , pvHED_ZVsnWork  ))) return false ;
                 if(!MT_GoAbsMan(miHED_TZom , PM.GetValue(miHED_TZom , pvHED_TZomWork  ))) return false ;
                 AT_MoveCyl(aiHED_PenCoverUD , ccFwd );
                 Step.iHome++;
                 return false ;

        case 14: if(!AT_MoveCyl(aiHED_PenCoverUD , ccFwd))return false;  // JH
                 //포지션 데이터를 다 지워야 한다.
                 DM.ARAY[riSTG].ClearTemp();

                 Rqst.bNeedCal    = true ;
                 Rqst.bNeedTAlign = true ;

                 Step.iHome = 0;
                 return true ;
    }
}

//---------------------------------------------------------------------------
bool CHead::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

//---------------------------------------------------------------------------
bool CHead::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool CHead::ToStart(void) //스타트를 하기 위한 함수.
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

        case 10:

                 MoveActr(aiHED_PenMrkDU   , ccBwd);
                 MoveActr(aiHED_PenCoverUD , ccBwd);
                 MoveMotr(miHED_TZom , piHED_TZomWork);
                 MoveMotr(miHED_ZVsn , piHED_ZVsnWork);

                 Step.iToStart++;
                 return false;

        case 11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                 if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;
                 if(!MoveMotr(miHED_TZom , piHED_TZomWork)) return false;
                 if(!MoveMotr(miHED_ZVsn , piHED_ZVsnWork)) return false;

                 Step.iToStart ++ ;
                 return false ;

        case 12: if(OM.CmnOptn.iPenDnCnt) Rqst.bNeedPenCheck = true ; //오래 서있었으면 펜이 굳었나 확인 작업.

                 Step.iToStart = 0 ;
                 return true ;
    }
}

//---------------------------------------------------------------------------
bool CHead::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 5000)) EM_SetErr((EN_ERR_ID)m_iStopTO);

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

        case 10: if(EM_GetErr((EN_ERR_ID)eiHED_PenMrkDUTO     )|| //펜마킹실린더 끼었을때 초기위치로 가면 뿌러짐.
                    EM_GetErr((EN_ERR_ID)eiHED_OriVisnInspFail)|| //오리진 검사 실패에러 일경우..기준칩 검사일때 매뉴얼로 설정하게
                    EM_GetErr((EN_ERR_ID)eiHED_OriVisnRlstOver)){ //얘는 일단 넣어봄.
                     Step.iToStop = 0;

                     return true;
                 }

                 MoveActr(aiHED_PenMrkDU   , ccBwd);
                 MoveActr(aiHED_PenCoverUD , ccBwd);
                 Step.iToStop ++;
                 return false ;

        case 11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd))return false;
                 if(!MoveActr(aiHED_PenCoverUD , ccBwd))return false;

                 //오리엔테이션 잡아야 하는데 도망감.
                 if(HED.m_bNeedOriDataSet) {
                     Step.iToStop = 0 ;
                     return true ;
                 }

                 MoveMotr(miHED_XHed , piHED_XHedPCover); //JH
                 MoveMotr(miHED_YHed , piHED_YHedPCover); //JH
                 Step.iToStop ++ ;
                 return false ;

        case 12: if(!MoveMotr(miHED_XHed , piHED_XHedPCover)) return false;
                 if(!MoveMotr(miHED_YHed , piHED_YHedPCover)) return false;
                 MoveActr(aiHED_PenCoverUD , ccFwd );
                 Step.iToStop ++ ;
                 return false ;

        case 13: if(!MoveActr(aiHED_PenCoverUD , ccFwd))return false;  // JH
                 Step.iToStop = 0;
                 return true;
    }
}

//---------------------------------------------------------------------------
bool CHead::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;


        int er , ec ; //Error칩.
        int fr , fc ; //Fail 칩.
        int wr , wc ; //작업 해야 할 칩.
        int iAlignR, iAlignC ;
        int iDataR , iDataC  ;
        bool bExistEror = FindChip(riSTG , er , ec , csEror) ;
        bool bExistFail = FindChip(riSTG , fr , fc , csFail) ;
        bool bExistWork = bExistEror || bExistFail ;
        //bool bExistData = false ;
        bool bExistVisn = GetAlignRowCol(iAlignR, iAlignC, iDataR, iDataC);
        //for(int r = 0 ; r < DM.ARAY[riSTG].GetMaxRow() ; r++) {
        //    for(int c = 0 ; c < DM.ARAY[riSTG].GetMaxCol() ; c++) {
        //        if(DM.ARAY[riSTG].GetTemp(r,c,0) || DM.ARAY[riSTG].GetTemp(r,c,1)) bExistData = true ;
        //    }
        //}
        //작업 해야 하는 칩을 우선순위로 설정. 1.Eror 2.Fail.
        /*
        if(DM.ARAY[riSTG].GetCntStat(csEror)) { //마스크 영역 작업 하러 가는 사이.
            wr = er ; wc = ec ;
        }
        else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //맵파일 페일 영역 작업 하러 가는 사이.
            wr = fr ; wc = fc ;
        */

        bool bNeedCycleSupply = DM.ARAY[riCST].GetCntStat  (csUnkwn) && DM.ARAY[riSTG].CheckAllStat(csNone) ;
        bool bNeedCyclePlace  = DM.ARAY[riCST].GetCntStat  (csMask ) && //카세트에 꼽을때가 있고
                               !DM.ARAY[riSTG].GetCntStat  (csFail ) && //펜찍어야 되는 곳이 없고.
                               !DM.ARAY[riSTG].CheckAllStat(csNone ) && //웨이퍼는 있어야 되고.
                               !DM.ARAY[riSTG].GetCntStat  (csWait ) ;  //비전 기다리는 것이 없고.
        bool bHedPenPos      = MT_CmprPos(miHED_XHed , PM.GetValue(miHED_XHed , pvHED_XHedPCover)) &&
                               MT_CmprPos(miHED_YHed , PM.GetValue(miHED_YHed , pvHED_YHedPCover)) ;

        bool bTrsfWaitPos     = MT_CmprPos(miSTG_XTrs , PM.GetValue(miSTG_XTrs , pvSTG_XTrsWait)) ;
        bool bStageReady      = bTrsfWaitPos && AT_Complete(aiSTG_ChkTblUD , ccFwd);

        /*
        isCycleCal       //비젼 켈리브레이션.                              홈/잡컨버젼
        isCyclePenCheck  //펜 나오는지 화이트 보드 가서 찍어보고 체크.     홈/새웨이퍼
        isCycleTAlign    //비전 사각형으로 웨이퍼 세타 얼라인 잡는것.      홈/새웨이퍼
        isCycleOrigin    //비젼 매치로 기준 칩 찾기.                       홈/새웨이퍼/장비 스타트시.
        isCycleMark      //비전 사각형으로 칩 얼라인 위치 보정.
        isCycleChipAlign //펜마킹 & 경우에 따라 찍고 비젼 확인 까지.
        */

        /*
        bool isCycleCal       = false;
        bool isCyclePenCheck  = false;
        bool isCycleTAlign    = false;
        bool isCycleOrigin    = false;
        bool isCycleMark      = false;
        bool isCycleChipAlign = false;
        bool isCycleWaitPos   = false;
        */

        bool isCycleCal       =  Rqst.bNeedCal      ;
        bool isCyclePenCheck  =  bExistWork && Rqst.bNeedPenCheck && STG.GetSeqStep()!=CStage::scSupply && STG.GetSeqStep()!=CStage::scPlace;
        bool isCycleTAlign    =  bExistWork && bStageReady && Rqst.bNeedTAlign  ;
        bool isCycleOrigin    =  bExistWork && bStageReady && Rqst.bNeedOrigin  ;
        bool isCycleMarkTrace =  Rqst.bNeedMarkTrace ;
        bool isCycleMark      =  bExistWork && bStageReady ;
        bool isCycleChipAlign =  bExistWork && bStageReady && bExistVisn;//&& //!CheckInConfirmArea(wr , wc) && OM.DevOptn.iAlignConfirmChip ;
        bool isCycleWaitPos   = (bNeedCycleSupply || bNeedCyclePlace) && !bHedPenPos ;

        bool isConEnd         = !isCycleCal       &&
                                !isCyclePenCheck  &&
                                !isCycleTAlign    &&
                                !isCycleOrigin    &&
                                !isCycleMark      &&
                                !isCycleChipAlign &&
                                !isCycleWaitPos   ;

        //if( DM.ARAY[riSTG].CheckAllStat(csNone ) &&  IO_GetX(xSTG_SgWfDetect)             ) EM_SetErr(eiSTG_Unknwn);      // JH
        //if(!DM.ARAY[riSTG].CheckAllStat(csNone ) && !IO_GetX(xSTG_SgWfDetect) && isConEnd ) EM_SetErr(eiSTG_Dispr );

        if(!DM.ARAY[riBRD].GetCntStat  (csUnkwn)) {
            //자동으로 지워 준다.
            DM.ARAY[riBRD].SetStat(csUnkwn);
            EM_SetErr(eiHED_WhiteFull);
        }

        if(EM_IsErr()) return false ;

             if (isCycleCal      ) {Trace(m_sPartName.c_str(),"CycleCal       Stt"); Step.iSeq = scCal       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ; } //
        else if (isCyclePenCheck ) {Trace(m_sPartName.c_str(),"CyclePenCheck  Stt"); Step.iSeq = scPenCheck  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ; } //
        else if (isCycleTAlign   ) {Trace(m_sPartName.c_str(),"CycleTAlign    Stt"); Step.iSeq = scTAlign    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ; } //
        else if (isCycleOrigin   ) {Trace(m_sPartName.c_str(),"CycleOrigin    Stt"); Step.iSeq = scOrigin    ; Step.iCycle = 10 ; PreStep.iCycle = 0 ; } //
        else if (isCycleChipAlign) {Trace(m_sPartName.c_str(),"CycleChipAlign Stt"); Step.iSeq = scChipAlign ; Step.iCycle = 10 ; PreStep.iCycle = 0 ; } //
        else if (isCycleMarkTrace) {Trace(m_sPartName.c_str(),"CycleMarkTrace Stt"); Step.iSeq = scMarkTrace ; Step.iCycle = 10 ; PreStep.iCycle = 0 ; } //
        else if (isCycleMark     ) {Trace(m_sPartName.c_str(),"CycleMark      Stt"); Step.iSeq = scMark      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ; } //
        else if (isCycleWaitPos  ) {Trace(m_sPartName.c_str(),"CycleWaitPos   Stt"); Step.iSeq = scWaitPos   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ; } //
        else if (isConEnd        ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
        SetStartTime(Step.iSeq) ;
   }

    //Cycle.
    switch (Step.iSeq) {
        default         :                        Trace(m_sPartName.c_str(),"default        End");                       Step.iSeq = scIdle ;   return false ;
        case scIdle     :                                                                        SetEndTime(Step.iSeq);                        return false ;
        case scCal      : if(CycleCal       ()){ Trace(m_sPartName.c_str(),"CycleCal       End");SetEndTime(Step.iSeq); Step.iSeq = scIdle ; } return false ;
        case scPenCheck : if(CyclePenCheck  ()){ Trace(m_sPartName.c_str(),"CyclePenCheck  End");SetEndTime(Step.iSeq); Step.iSeq = scIdle ; } return false ;
        case scTAlign   : if(CycleTAlign    ()){ Trace(m_sPartName.c_str(),"CycleTAlign    End");SetEndTime(Step.iSeq); Step.iSeq = scIdle ; } return false ;
        case scOrigin   : if(CycleOrigin    ()){ Trace(m_sPartName.c_str(),"CycleOrigin    End");SetEndTime(Step.iSeq); Step.iSeq = scIdle ; } return false ;
        case scChipAlign: if(CycleChipAlign ()){ Trace(m_sPartName.c_str(),"CycleChipAlign End");SetEndTime(Step.iSeq); Step.iSeq = scIdle ; } return false ;
        case scMarkTrace: if(CycleMarkTrace ()){ Trace(m_sPartName.c_str(),"CycleMarkTrace End");SetEndTime(Step.iSeq); Step.iSeq = scIdle ; } return false ;
        case scMark     : if(CycleMark      ()){ Trace(m_sPartName.c_str(),"CycleMark      End");SetEndTime(Step.iSeq); Step.iSeq = scIdle ; } return false ;
        case scWaitPos  : if(CycleWaitPos   ()){ Trace(m_sPartName.c_str(),"CycleWaitPos   End");SetEndTime(Step.iSeq); Step.iSeq = scIdle ; } return false ;
    }
}

//비젼 싸이클
bool CHead::CycleVisn(EN_VISN_ID _iId , bool _bInit)
{
    if(_bInit) Step.iVisn = 10 ;

    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmVisn.OnDelay(Step.iVisn && Step.iVisn == PreStep.iVisn && CheckStop() && !OM.MstOptn.bDebugMode, 4000 )) {
        EM_SetErr(eiHED_VisnTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iVisn=%02d" , __FUNC__ , Step.iVisn );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iVisn = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iVisn=%02d" , __FUNC__ , Step.iVisn );
    if(Step.iVisn != PreStep.iVisn) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iVisn = Step.iVisn ;

    //Cycle.
    switch (Step.iVisn) {

        default : if(Step.iVisn) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iVisn=%02d" , __FUNC__ , Step.iVisn );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iVisn = 0 ;
                  return true ;

        case  10: VSM.SetLight(_iId); //조명 세팅. rs-232라 느림. 미리 보내 놓아야함.

                  m_tmVisnTemp.Clear();
                  Step.iVisn++ ;
                  return false ;

        case  11: if(!m_tmVisnTemp.OnDelay(true , OM.CmnOptn.iVsBfGrabDly)) return false ;

                  VSM.Grab(_iId);
                  m_tmVisnTemp.Clear() ;
                  Step.iVisn++;
                  return false;

                  //밑에서 씀 번호 주의.
        case  12: if(m_tmVisnTemp.OnDelay(!VSM.GetGrabEnd(_iId),2000)) {
                      EM_SetErr(eiHED_VisnGrabFail);
                      Step.iVisn = 0 ;
                      return true ;
                  }
                  if(!VSM.GetGrabEnd(_iId)) return false ;

                  //FOV넓게 검사한다.
                  if(m_bNeedWideWfrChipInsp) {VSM.SetWfrOri1WideInspection(true); m_bNeedWideWfrChipInsp = false ; }
                  if(m_bNeedWideWfoChipInsp) {VSM.SetWfoOri1WideInspection(true); m_bNeedWideWfoChipInsp = false ; }

                  VSM.Inspect(_iId,false);
                  VSM.SetWfrOri1WideInspection(false); //바로 해지 한다.
                  VSM.SetWfoOri1WideInspection(false); //바로 해지 한다.

                  m_tmVisnTemp.Clear() ;
                  Step.iVisn++;
                  return false ;

        case  13: if(m_tmVisnTemp.OnDelay(!VSM.GetInspEnd(_iId),5000 )) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iVisn = 0 ;
                      return true ;
                  }
                  if(!VSM.GetInspEnd(_iId)) return false ;

                  Step.iVisn=0;
                  return true ;
    }
}


//---------------------------------------------------------------------------
bool CHead::CycleCal(void)
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

    if(Stat.bReqStop) { //검사하면서 에러 나면 멈추고 Rqst.bNeedCal 플레그 살아 있게.
        MT_Stop(miHED_XHed);
        MT_Stop(miHED_YHed);

        Step.iCycle = 0;
        return true;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);
                  MoveMotr(miHED_TZom , piHED_TZomWork);
                  MoveMotr(miHED_ZVsn , piHED_ZVsnWork);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;
                  if(!MoveMotr(miHED_TZom , piHED_TZomWork)) return false;
                  if(!MoveMotr(miHED_ZVsn , piHED_ZVsnWork)) return false;

                  MoveMotr(miHED_XHed , piHED_XHedVCalPad);
                  MoveMotr(miHED_YHed , piHED_YHedVCalPad);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miHED_XHed , piHED_XHedVCalPad)) return false ;
                  if(!MoveMotr(miHED_YHed , piHED_YHedVCalPad)) return false ;

                  CycleVisn(viCal , true);
                  Step.iCycle++;
                  return false ;

        case  13: if(!CycleVisn(viCal)) return false ;
                  VSM.GetRslt(viCal , &VsRslt) ;
                  if(VsRslt.iRet != veCalOk) { //비젼 검사 실패.
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  Rqst.bNeedCal = false ;
                  Step.iCycle=0;
                  return true ;
    }
}


//-------------------------------------------------------------------------
bool CHead::CyclePenCheck(void)
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

    static int iPenDnCnt = 0 ;
    int r,c ;

    double dTempX , dTempY ;

    if(Stat.bReqStop) { //검사하면서 에러 나면 멈추고 Rqst. 플레그 살아 있게.
        MT_Stop(miHED_XHed);
        MT_Stop(miHED_YHed);

        Step.iCycle = 0;
        return true;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);
                  iPenDnCnt = 0 ;
                  Step.iCycle = 15;
                  return false;

        /*  // 펜 일직선으로 긋는 사이클
        case  11: if(!MoveActr(aiHED_PenMrkDU , ccBwd)) return false;
                  MT_GoAbsRun(miHED_XHed , PM.GetValue(miHED_XHed , pvHED_XHedVBoardStt ) +
                                           PM.GetValue(miHED_XHed , poHED_XHedVisnPenOfs) +
                                           OM.DevInfo.dBdXPch );

                  MT_GoAbsRun(miHED_YHed , PM.GetValue(miHED_YHed , pvHED_YHedVBoardStt ) +
                                           PM.GetValue(miHED_YHed , poHED_YHedVisnPenOfs));

                  Step.iCycle++;
                  return false ;

        case  12: if(!MT_GetStop(miHED_XHed)) return false ;
                  if(!MT_GetStop(miHED_YHed)) return false ;
                  MoveActr(aiHED_PenMrkDU , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveActr(aiHED_PenMrkDU , ccFwd)) return false ;
                  MT_GoAbsRun(miHED_YHed , PM.GetValue(miHED_YHed , pvHED_YHedVBoardStt ) +
                                           PM.GetValue(miHED_YHed , poHED_YHedVisnPenOfs) +
                                           25.0 );

                  Step.iCycle++;
                  return false ;

        case  14: if(!MT_GetStop(miHED_YHed)) return false ;
                  MoveActr(aiHED_PenMrkDU , ccBwd);
                  Step.iCycle++;
                  return false ;

        */

        //밑에서 씀.
        case  15: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;
                  MoveMotr(miHED_XHed , piHED_XHedPBoard);
                  MoveMotr(miHED_YHed , piHED_YHedPBoard);
                  Step.iCycle++;
                  return false ;

        case  16: if(!MoveMotr(miHED_XHed , piHED_XHedPBoard)) return false ;
                  if(!MoveMotr(miHED_YHed , piHED_YHedPBoard)) return false ;
                  MoveActr(aiHED_PenMrkDU , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiHED_PenMrkDU , ccFwd)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  18: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPenAtDnDly)) return false ;
                  MoveActr(aiHED_PenMrkDU , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  19: if(!MoveActr(aiHED_PenMrkDU , ccBwd)) return false ;

                  iPenDnCnt++;
                  if(OM.CmnOptn.iPenDnCnt > iPenDnCnt) { //3으로 설정되어 있으면 제자리에서 3회 찍는다.
                      if(FindChip(riBRD,r,c,csUnkwn)){
                          DM.ARAY[riBRD].SetStat(r,c,csWork);
                      }
                      Step.iCycle = 15;
                      return false ;
                  }

                  MoveMotr(miHED_XHed , piHED_XHedVBoard);
                  MoveMotr(miHED_YHed , piHED_YHedVBoard);

                  Step.iCycle++;
                  return false ;

        case  20: if(!MoveMotr(miHED_XHed , piHED_XHedVBoard)) return false ;
                  if(!MoveMotr(miHED_YHed , piHED_YHedVBoard)) return false ;

                  CycleVisn(viWfp , true);

                  Step.iCycle++;
                  return false ;

        case  21: if(!CycleVisn(viWfp)) return false ;
                  VSM.GetRslt(viWfp , &VsRslt) ;

                  if(VsRslt.iRet != veWfpOk) { //비젼 검사 실패.
                      if(MM.GetManNo() == mcNone)EM_SetErr(eiHED_PenFail);
                      Step.iCycle = 30;
                      return false;
                  }

                  //메뉴얼 펜얼라인  일때만 비전 잘 됐으면 보정.
                  if(MM.GetManNo() == mcHED_CyclePenAlign) {
                      dTempX = PM.GetValue(miHED_XHed , poHED_XHedVisnPenOfs) - VsRslt.fRsltX ;
                      dTempY = PM.GetValue(miHED_YHed , poHED_YHedVisnPenOfs) - VsRslt.fRsltY ;

                      PM.SetValue(miHED_XHed , poHED_XHedVisnPenOfs , dTempX);
                      PM.SetValue(miHED_YHed , poHED_YHedVisnPenOfs , dTempY);

                      PM.Save(OM.m_sCrntDev);
                  }

                  if(FindChip(riBRD,r,c,csUnkwn)){
                      DM.ARAY[riBRD].SetStat(r,c,csWork);
                  }

                  //처음 작업 하는 칩은 무조건 검사 하게.
                  m_iPenDnCnt = OM.DevOptn.iPenCheckCnt ;

                  if(MM.GetManNo() != mcNone){
                      Step.iCycle=30;
                      return false;
                  }
                  Rqst.bNeedPenCheck = false ;
                  Step.iCycle = 0 ;
                  return true ;

        case  30: MoveMotr(miHED_XHed , piHED_XHedPCover);
                  MoveMotr(miHED_YHed , piHED_YHedPCover);

                  Step.iCycle++;
                  return false;

        case  31: if(!MoveMotr(miHED_XHed , piHED_XHedPCover)) return false;
                  if(!MoveMotr(miHED_YHed , piHED_YHedPCover)) return false;
                  MoveActr(aiHED_PenCoverUD , ccFwd);
                  Step.iCycle++;
                  return false;

        case  32: if(!MoveActr(aiHED_PenCoverUD , ccFwd)) return false;
                  Rqst.bNeedPenCheck = false ;
                  Step.iCycle = 0;
                  return true;
    }
}


//---------------------------------------------------------------------------
bool CHead::CycleTAlign(void)
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

    if(Stat.bReqStop) { //검사하면서 에러 나면 멈추고 Rqst. 플레그 살아 있게.
        MT_Stop(miHED_XHed);
        MT_Stop(miHED_YHed);

        Step.iCycle = 0;
        return true;
    }

    double dXPos  ;
    double dYPos  ;
    double dTPos  ;
    double dAngle ;
    double dAngle2;
    const POINT Neib[8] = { {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} ,{1, 0}, {1,-1} }; // 위치로 이동 했을 때 일치하는 칩이 없으면 주위 8방향 검사...
    static int iNextDir = 0 ; // 위치로 이동 했을 때 일치하는 칩이 없으면 주위 8방향 검사...


    //쎈터 칩 위치.
    static double dCntChipPosX = 0.0;
    static double dCntChipPosY = 0.0;


    //좌우 검사 결과값.
    static double dLeftChipPosX  = 0.0 ;
    static double dLeftChipPosY  = 0.0 ;
    static double dRightChipPosX = 0.0 ;
    static double dRightChipPosY = 0.0 ;

    static double dOfsetChipPosX = 0.0 ;
    static double dOfsetChipPosY = 0.0 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);

                  iNextDir = 0 ;
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;
                  MoveMotr(miHED_XHed , piHED_XHedVTAlgn);
                  MoveMotr(miHED_YHed , piHED_YHedVTAlgn);
                  Step.iCycle++;
                  return false ;

        //밑에서씀. 웨이퍼 세타 기준 센터 칩 찾기.
        case  12: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;

                  m_bNeedWideWfrChipInsp = true ; //광역검사.
                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

//============================센터칩 1차 보정===================================

        case  13: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      dXPos = PM.GetValue(miHED_XHed , pvHED_XHedVTAlgn) + (OM.DevInfo.dWfPitchX  / 4.0) * Neib[iNextDir].x ;
                      dYPos = PM.GetValue(miHED_YHed , pvHED_YHedVTAlgn) + (OM.DevInfo.dWfPitchY  / 4.0) * Neib[iNextDir].y ;

                      MT_GoAbsRun(miHED_XHed , dXPos ) ;
                      MT_GoAbsRun(miHED_YHed , dYPos ) ;

                      iNextDir++;

                      if(iNextDir == 8){
                          EM_SetErr(eiHED_VisnInspFail);
                          Step.iCycle=0;
                          return true ;
                      }
                      //iNextDir=((iNextDir+1) & 7);

                      Step.iCycle=12;
                      return false ;
                  }

                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               true );  //세타보정 씀. 스테이지 센터에서는 보정써도 크게 안벋어 난다.

                  MT_GoAbsRun(miHED_XHed , dXPos);
                  MT_GoAbsRun(miHED_YHed , dYPos);
                  MT_GoAbsRun(miSTG_TStg , STG.GetStageAbsMmFromAbsAngle(dTPos));

                  Step.iCycle++;
                  return false ;

        case  14: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

//========================센터칩 1차 보정 완료==================================
//========================센터칩 2차 보정 ======================================

                  //정밀도 때문에 한번더 검사.
                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  15: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               true );  //세타보정 씀. 스테이지 센터에서는 보정써도 크게 안벋어 난다.

                  MT_GoAbsRun(miHED_XHed , dXPos);
                  MT_GoAbsRun(miHED_YHed , dYPos);
                  MT_GoAbsRun(miSTG_TStg , STG.GetStageAbsMmFromAbsAngle(dTPos));

                  dCntChipPosX = dXPos ;
                  dCntChipPosY = dYPos ;

                  Step.iCycle++;
                  return false ;

        case  16: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

//========================센터칩 2차 보정 완료==================================
//===================센터 칩 왼쪽으로 1피치 이동 후 검사========================

                  MT_GoIncRun(miHED_XHed , OM.DevInfo.dWfPitchX);

                  Step.iCycle++;
                  return false ;

        case  17: if(!MT_GetStopInpos(miHED_XHed)) return false ;

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  18: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //세타보정 안씀.


                  dLeftChipPosX = dXPos ;
                  dLeftChipPosY = dYPos ;

                  MT_GoIncRun(miHED_XHed , -OM.DevInfo.dWfPitchX*2);

                  Step.iCycle++;
                  return false ;

        case  19: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;

//===================센터 칩 왼쪽으로 1피치 이동 후 검사 완료===================
//===================센터 칩 오른쪽으로 1피치 이동 후 검사======================

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  20: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //세타보정 안씀.


                  dRightChipPosX = dXPos ;
                  dRightChipPosY = dYPos ;

                  dAngle = MATH_GetLineAngle(dLeftChipPosX , dLeftChipPosY , dRightChipPosX , dRightChipPosY ) ;

                  //역방향.
                  dAngle =  dAngle - 180.0;

                  dAngle2 = STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ;

                  MT_GoAbsRun(miHED_XHed , dCntChipPosX);
                  MT_GoAbsRun(miHED_YHed , dCntChipPosY);
                  MT_GoAbsRun(miSTG_TStg , STG.GetStageAbsMmFromAbsAngle(dAngle2)+dAngle);

//============센터 칩 오른쪽으로 1피치 이동 후 검사 후 세타 보정 완료===========
//===========================보정 피치 값 구하기================================

                  Step.iCycle++;
                  return false;

        case  21: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

                  MT_GoIncRun(miHED_XHed , OM.DevInfo.dWfPitchX ); //한피치씩만 움직여서 확인.
                  MT_GoIncRun(miHED_YHed , OM.DevInfo.dWfPitchY );

                  Step.iCycle++;
                  return false ;

        case  22: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  23: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //세타보정 안씀.

                  //가세팅.
                  //OM.DevInfo.dWfPitchX = (dXPos - dCntChipPosX) ;
                  //OM.DevInfo.dWfPitchY = (dYPos - dCntChipPosY) ;

//============================가보정 패턴 종료==================================
//===========================T얼라인 왼쪽 검사==================================
                  dOfsetChipPosX = VsRslt.fRsltX;
                  dOfsetChipPosY = VsRslt.fRsltY;
                  MT_GoAbsRun(miHED_XHed , dCntChipPosX + (GetWaferPitchX(dOfsetChipPosX) * OM.DevOptn.iVisnChkChipColCnt));
                  MT_GoAbsRun(miHED_YHed , dCntChipPosY );



                  //MT_GoAbsRun(miHED_XHed , dCntChipPosX + (GetWaferPitchX(VsRslt.fRsltX) * OM.DevOptn.iVisnChkChipColCnt));
                  //MT_GoAbsRun(miHED_YHed , dCntChipPosY );

                  Step.iCycle++;
                  return false ;

        case  24: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;

                  //왼쪽에 가서 검사.
                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  25: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  //값이 범위에서 벋어나면 에러.
                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //왼쪽칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //세타보정 안씀.

                  dLeftChipPosX = dXPos ;
                  dLeftChipPosY = dYPos ;

//=============================왼쪽 검사 패턴 종료==============================
//===============================오른쪽 검사 패턴===============================
                  MT_GoAbsRun(miHED_XHed , dCntChipPosX - (GetWaferPitchX(dOfsetChipPosX) * OM.DevOptn.iVisnChkChipColCnt));
                  MT_GoAbsRun(miHED_YHed , dCntChipPosY );



                  //MT_GoAbsRun(miHED_XHed , dCntChipPosX - (GetWaferPitchX(VsRslt.fRsltX) * OM.DevOptn.iVisnChkChipColCnt)); //검사간격 2배 만큼 오른쪽 이동 제거.
                  //MT_GoAbsRun(miHED_YHed , dCntChipPosY);
                  Step.iCycle++;
                  return false ;

        case  26: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;

                  //왼쪽 끝에 가서 검사.
                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  27: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  //값이 범위에서 벋어나면 에러.
                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //오른쪽칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //세타보정 안씀.

                  dRightChipPosX = dXPos ;
                  dRightChipPosY = dYPos ;

                  dAngle = MATH_GetLineAngle(dLeftChipPosX , dLeftChipPosY , dRightChipPosX , dRightChipPosY ) ;

                  //역방향.
                  dAngle =  dAngle - 180.0;

                  dAngle2 = STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ;

//============================오른쪽 검사 패턴 종료=============================
//=============================피치 보정 검사 패턴==============================

                  MT_GoAbsRun(miHED_XHed , dCntChipPosX);
                  MT_GoAbsRun(miHED_YHed , dCntChipPosY);
                  MT_GoAbsRun(miSTG_TStg , STG.GetStageAbsMmFromAbsAngle(dAngle2) + dAngle);

                  Step.iCycle++;
                  return false ;

        case  28: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

                  //T보정후 센터칩에서 검사.
                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  29: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  //값이 범위에서 벋어나면 에러.
                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //세타보정 안씀.

                  MT_GoAbsRun(miHED_XHed , dXPos);
                  MT_GoAbsRun(miHED_YHed , dYPos);
                  Step.iCycle++;
                  return false ;

        case  30: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

//===========================센터칩 1차 보정 검사===============================
//==============================센터칩 2차 보정=================================

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  31: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  //값이 범위에서 벋어나면 에러.
                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //오른쪽칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //세타보정 안씀.

                  MT_GoAbsRun(miHED_XHed , dXPos); //검사 간격 이동 후 피치 보정.
                  MT_GoAbsRun(miHED_YHed , dYPos);

                  dCntChipPosX = dXPos ;
                  dCntChipPosY = dYPos ;

                  Step.iCycle++;
                  return false ;

        case  32: if(!MT_GetStopInpos(miHED_XHed)) return false ; //없어도 됌.
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

//===========================2차 센터칩 찾기 완료===============================
//==============================피치 보정 패턴==================================

                  MT_GoIncRun(miHED_XHed , GetWaferPitchX(VsRslt.fRsltX) * OM.DevOptn.iVisnChkChipColCnt); //검사 간격 이동 후 피치 보정.
                  MT_GoIncRun(miHED_YHed , GetWaferPitchY(VsRslt.fRsltY) * OM.DevOptn.iVisnChkChipRowCnt);

                  Step.iCycle++;
                  return false ;

        case  33: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  34: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //세타보정 안씀.

                  MT_GoAbsRun(miHED_XHed , dXPos); //검사 간격 이동 후 피치 보정.
                  MT_GoAbsRun(miHED_YHed , dYPos);

                  Step.iCycle++;
                  return false ;

        case  35: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  36: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //세타보정 안씀.

                  //피치 세팅.
                  //OM.DevInfo.dWfPitchX = (dXPos - dCntChipPosX)/ OM.DevOptn.iVisnChkChipColCnt;
                  //OM.DevInfo.dWfPitchY = (dYPos - dCntChipPosY)/ OM.DevOptn.iVisnChkChipRowCnt;

                  //if(MM.GetManNo() != mcHED_CycleTAlign){
                      Rqst.bNeedTAlign = false ;
                      Rqst.bNeedOrigin = true  ;
                  //}

                  m_dRevisePitchX = GetWaferPitchX(VsRslt.fRsltX);
                  m_dRevisePitchY = GetWaferPitchY(VsRslt.fRsltY);
                  m_bFnshCycTAlgn = true;

                  //쎄타를 바꿔서 포지션 데이터 클리어.
                  DM.ARAY[riSTG].ClearTemp();

                  Step.iCycle=0;
                  return false ;
    }
}


bool CHead::CycleOrigin(void)
{
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

    double dXPos ;
    double dYPos ;
    double dTPos ;
    const POINT Neib[8] = { {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} ,{1, 0}, {1,-1} };
    static int iNextDir = 0 ;
    static int iVisnCnt = 0 ;

    double dTempPosX ;
    double dTempPosY ;
    int    c,r ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);
                  iNextDir = 0 ;
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;
                  MoveMotr(miHED_XHed , piHED_XHedVOri);
                  MoveMotr(miHED_YHed , piHED_YHedVOri);
                  Step.iCycle++;
                  return false ;

        //밑에서씀. 웨이퍼 세타 기준 센터 칩 찾기.
        case  12: if(!MoveMotr(miHED_XHed , piHED_XHedVOri)) return false;
                  if(!MoveMotr(miHED_YHed , piHED_YHedVOri)) return false;
                  Step.iCycle++;
                  return false ;

        case  13: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;

                  if(OM.DevOptn.iOriMathod == omFiducial ) {m_bNeedWideWfoChipInsp = true ; CycleVisn(viWfo , true); }
                  if(OM.DevOptn.iOriMathod == omChip     ) {m_bNeedWideWfrChipInsp = true ; CycleVisn(viWfr , true); }

                  Step.iCycle++;
                  return false ;

        case  14: if(OM.DevOptn.iOriMathod == omFiducial  && !CycleVisn(viWfo)) return false ;
                  if(OM.DevOptn.iOriMathod == omChip      && !CycleVisn(viWfr)) return false ;

                  if(OM.DevOptn.iOriMathod == omFiducial  ) VSM.GetRslt(viWfo , &VsRslt) ;
                  if(OM.DevOptn.iOriMathod == omChip      ) VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != 0) {  //veWfrOk veWfoOk
                      if(m_bFnshCycTAlgn){
                          dXPos = PM.GetValue(miHED_XHed , pvHED_XHedVOri) + (m_dRevisePitchX  / 4.0) * Neib[iNextDir].x ;
                          dYPos = PM.GetValue(miHED_YHed , pvHED_YHedVOri) + (m_dRevisePitchY  / 4.0) * Neib[iNextDir].y ;
                      }
                      else{
                          dXPos = PM.GetValue(miHED_XHed , pvHED_XHedVOri) + (OM.DevInfo.dWfPitchX  / 4.0) * Neib[iNextDir].x ;
                          dYPos = PM.GetValue(miHED_YHed , pvHED_YHedVOri) + (OM.DevInfo.dWfPitchY  / 4.0) * Neib[iNextDir].y ;
                      }

                      MT_GoAbsRun(miHED_XHed , dXPos ) ;
                      MT_GoAbsRun(miHED_YHed , dYPos ) ;

                      iNextDir++;

                      if(iNextDir == 8){
                          EM_SetErr(eiHED_VisnInspFail);
                          Step.iCycle=0;
                          return true ;
                      }
                      //iNextDir=((iNextDir+1) & 7);

                      Step.iCycle=13;
                      return false ;
                  }

                  //센터칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );

                  MT_GoAbsRun(miHED_XHed , dXPos);
                  MT_GoAbsRun(miHED_YHed , dYPos);
                  MT_GoAbsRun(miSTG_TStg , STG.GetStageAbsMmFromAbsAngle(dTPos));

                  Step.iCycle++;
                  return false ;

        case  15: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

                  //칩 한번더 검사.
                  if(OM.DevOptn.iOriMathod == omFiducial  ) {m_bNeedWideWfoChipInsp = true ; CycleVisn(viWfo , true); }
                  if(OM.DevOptn.iOriMathod == omChip      ) {m_bNeedWideWfrChipInsp = true ; CycleVisn(viWfr , true); }
                  //if(OM.DevOptn.iOriMathod == omManual) {m_bNeedWideWfrChipInsp = true ; CycleVisn(viWfr , true); }

                  Step.iCycle++;
                  return false ;

        case  16: if(OM.DevOptn.iOriMathod == omFiducial  && !CycleVisn(viWfo)) return false ;
                  if(OM.DevOptn.iOriMathod == omChip      && !CycleVisn(viWfr)) return false ;
                  //if(OM.DevOptn.iOriMathod == omManual && !CycleVisn(viWfr)) return false ;

                  if(OM.DevOptn.iOriMathod == omFiducial  ) VSM.GetRslt(viWfo , &VsRslt) ;
                  if(OM.DevOptn.iOriMathod == omChip      ) VSM.GetRslt(viWfr , &VsRslt) ;
                  //if(OM.DevOptn.iOriMathod == omManual) VSM.GetRslt(viWfr , &VsRslt) ;



                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_OriVisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_OriVisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //세타보정 씀. 스테이지 센터에서는 보정써도 크게 안벋어 난다.

                  MT_GoAbsRun(miHED_XHed , dXPos);
                  MT_GoAbsRun(miHED_YHed , dYPos);
                  MT_GoAbsRun(miSTG_TStg , STG.GetStageAbsMmFromAbsAngle(dTPos));

                  Step.iCycle++;
                  return false ;

        case  17: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

                  //마스크에 기준칩이 없으면 에러....
                  if(!FindChip(riMSK,r,c,csOrig)) {
                      EM_SetErr(eiSTG_NoOrig);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                        if(MM.GetManNo() == mcHED_CycleOrigin){ //메뉴얼 동작시에...
                            //자동오리진일때만 수행하고 수동일때는 다시 하게끔.
                            if(OM.DevOptn.bAutoOrigin) { Rqst.bNeedOrigin = false;}
                            else                       { Rqst.bNeedOrigin = true ;}
                            FrmOperation -> btManOri -> Enabled = Rqst.bNeedOrigin ;
                        }

                  //메뉴얼 오리엔테이션 일때는 장비 세우고 오퍼가 잡도록.
                  if(!OM.DevOptn.bAutoOrigin) {
                      m_bNeedOriDataSet = true ;
                      SEQ._bBtnStop = true ;
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  dTempPosX = MT_GetCmdPos(miHED_XHed);
                  dTempPosY = MT_GetCmdPos(miHED_YHed);

                  DM.ARAY[riSTG].SetTemp(r,c,0,dTempPosX);
                  DM.ARAY[riSTG].SetTemp(r,c,1,dTempPosY);

                  dTempPosX = DM.ARAY[riSTG].GetTemp(r,c,0) ; //X  값
                  dTempPosY = DM.ARAY[riSTG].GetTemp(r,c,1) ; //Y 값.

                  m_iLastInspChipC = c;
                  m_iLastInspChipR = r;
                  Rqst.bNeedOrigin = false ;

                  Step.iCycle=0;
                  return false ;

    }
}


bool CHead::CycleChipAlign(void)
{
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

    double dXPos ;
    double dYPos ;
    double dTPos ;
    int r,c ;
    int iAlignC , iAlignR; //얼라인 칠 칩 오와열.
    int iDataC  , iDataR ; //데이터 참고할 칩 오와열.

    const int iVisnErrMaxCnt = 5 ; //칩검사시에 5번까지 에러는 그냥 모터 포지션으로 세팅 하고 넘어가면 에러 띄운다.
    static int iVisnErrCnt = 0 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;
                  MoveMotr(miHED_XHed , piHED_XHedVChipAlign);
                  MoveMotr(miHED_YHed , piHED_YHedVChipAlign);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miHED_XHed , piHED_XHedVChipAlign)) return false ;
                  if(!MoveMotr(miHED_YHed , piHED_YHedVChipAlign)) return false ;

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  13: if(!CycleVisn(viWfr)) return false ;

                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      iVisnErrCnt++;
                      if(iVisnErrCnt >= iVisnErrMaxCnt) {
                          iVisnErrCnt = 0 ;
                          EM_SetErr(eiHED_VisnInspFail);
                          Step.iCycle=0;
                          return true ;
                      }
                  }
                  else { //굿일땐 카운터 클리어.
                      iVisnErrCnt = 0 ;
                  }

                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  if(VsRslt.iRet == veWfrOk) { //굿일때만 보정.
                      GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                                   VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                                   dXPos                    , dYPos                    , dTPos                    ,
                                   false );  //세타보정 씀. 스테이지 센터에서는 보정써도 크게 안벋어 난다.
                  }
                  else {
                      dXPos = MT_GetCmdPos(miHED_XHed) ;
                      dYPos = MT_GetCmdPos(miHED_YHed) ;
                  }

                  if(DM.ARAY[riSTG].GetCntStat(csEror)) { //마스크 영역 작업 하러 가는 사이.
                      FindChip(riSTG , r , c , csEror);
                  }
                  else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //맵파일 페일 영역 작업 하러 가는 사이.
                      FindChip(riSTG , r , c , csFail);
                  }
                  //얼라인 위치는 맵에 표시가 안되서 FindChip을 쓸수 없음.
                  GetAlignRowCol(iAlignR , iAlignC, iDataR , iDataC);
                  //GetAlignRowCol(r , c , iAlignR , iAlignC , iDataR , iDataC) ;  //r,c작업타겟,iAlignR , iAlignC얼라인할 타겟,iDataR , iDataC위치데이터 참고할 타겟.
                  //GetNonOutSide (iAlignR , iAlignC , iAlignR , iAlignC ) ;

                  /*
                  if(GetAlignRowCol(r , c , iAlignR , iAlignC , iDataR , iDataC) && ){  //r,c작업타겟,iAlignR , iAlignC얼라인할 타겟,iDataR , iDataC위치데이터 참고할 타겟.
                      dXHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (iAlignC - iDataC) * OM.DevInfo.dWfPitchX  ;//모터방향에 따라 -+부호 바뀜.
                      dYHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (iAlignR - iDataR) * OM.DevInfo.dWfPitchY ;//모터방향에 따라 -+부호 바뀜.
                  }
                  */

                  DM.ARAY[riSTG].SetTemp(iAlignR,iAlignC,0,dXPos);
                  DM.ARAY[riSTG].SetTemp(iAlignR,iAlignC,1,dYPos);

                  Trace("X----------",String(dXPos).c_str());
                  Trace("Y----------",String(dYPos).c_str());

                  m_iLastInspChipC = iAlignC;
                  m_iLastInspChipR = iAlignR;

                  Trace("XC---------",String(m_iLastInspChipC).c_str());
                  Trace("YR---------",String(m_iLastInspChipR).c_str());

                  Step.iCycle=0;
                  return true ;
    }
}


bool CHead::CycleMark(void)
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

    int r,c ;
    int iTemp1,iTemp2 ;


    static double dPreTime = GetTickTime();

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: /*비전 테스트용.
                  DM.ARAY[riSTG].ClearTemp();

                  DM.ARAY[riMSK].FindFrstColRow(csOrig,r,c);
                  DM.ARAY[riSTG].SetTemp(r,c,0,m_dTempPosX);
                  DM.ARAY[riSTG].SetTemp(r,c,1,m_dTempPosY);

                  HED.m_iLastInspChipC = c;
                  HED.m_iLastInspChipR = r;

                  Step.iCycle = 0 ;
                  return true ;
                  */
                  //
                  MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;

                  if(m_iTotalPenDnCnt > OM.CmnOptn.iPenDnOverCnt) {
                      EM_SetErr(eiHED_PenCntOver);
                      Step.iCycle=0;
                      return false ;
                  }


                  MoveMotr(miHED_XHed , piHED_XHedPWrk);
                  MoveMotr(miHED_YHed , piHED_YHedPWrk);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miHED_XHed , piHED_XHedPWrk)) return false;
                  if(!MoveMotr(miHED_YHed , piHED_YHedPWrk)) return false;
                  MoveActr(aiHED_PenMrkDU , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(EM_GetErr((EN_ERR_ID)eiHED_PenMrkDUTO)) {
                      MoveActr(aiHED_PenMrkDU , ccBwd);
                      Step.iCycle = 0 ;
                      return true;
                  }
                  if(!MoveActr(aiHED_PenMrkDU , ccFwd)) return false;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  14: if(!m_tmTemp.OnDelay(true , OM.CmnOptn.iPenAtDnDly)) return false ;
                  MoveActr(aiHED_PenMrkDU , ccBwd);
                  Step.iCycle++;
                  return false;

        case  15: if(EM_GetErr((EN_ERR_ID)eiHED_PenMrkDUTO)) {
                      Step.iCycle = 0 ;
                      return true;
                  }

                  if(!MoveActr(aiHED_PenMrkDU , ccBwd)) return false;

                  m_iPenDnCnt++;
                  m_iTotalPenDnCnt++;

                  iTemp1 = DM.ARAY[riSTG].GetCntStat(csWork) ; //첫 작업 자제  확인 한다.
                  iTemp2 = DM.ARAY[riSTG].GetCntStat(csFail) ; //마지막 자제 확인.

                  //첫자제이면 클리어 시킨다.
                  if(iTemp1 == 0) MarkData.ClearData();

                  if(OM.DevOptn.iPenCheckCnt != 0 ) {
                      if(iTemp1 == 0 || iTemp2 == 1 || OM.DevOptn.iPenCheckCnt <= m_iPenDnCnt) {
                          MoveMotr(miHED_XHed , piHED_XHedVWrk);
                          MoveMotr(miHED_YHed , piHED_YHedVWrk);
                          m_iPenDnCnt = 0 ;

                          Step.iCycle = 20;
                          return false ;
                      }
                  }

                  if(DM.ARAY[riSTG].GetCntStat(csEror)) { //마스크 영역 작업 하러 가는 사이.
                      FindChip(riSTG , r , c , csEror);
                  }
                  else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //맵파일 페일 영역 작업 하러 가는 사이.
                      FindChip(riSTG , r , c , csFail);
                  }
                  if(!DM.ARAY[riSTG].GetCntStat(csEror) && OM.MstOptn.bHopJumpChip){
                      GetHopJumpChipPos(m_iLastMrkChipR , m_iLastMrkChipC , r, c);
                  }
                  MarkData.PushData(r,c,DM.ARAY[riSTG].GetStat(r,c)==csFail) ;
                  DM.ARAY[riSTG].SetStat(r,c,csWork);
                  m_iLastMrkChipR = r ;
                  m_iLastMrkChipC = c ;

                  if(!DM.ARAY[riSTG].GetCntStat(csEror) && !DM.ARAY[riSTG].GetCntStat(csFail)) {
                      m_iLastMrkChipC = 0;
                      m_iLastMrkChipR = 0;
                  }

                  Step.iCycle = 0;
                  return true;

        //펜 찍은거 비젼으로 확인 하는 것.
        case  20: if(!MoveMotr(miHED_XHed , piHED_XHedVWrk)) return false ;
                  if(!MoveMotr(miHED_YHed , piHED_YHedVWrk)) return false ;

                  CycleVisn(viWfp , true);
                  Step.iCycle++;
                  return false ;

        case  21: if(!CycleVisn(viWfp)) return false ;
                  VSM.GetRslt(viWfp , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfpOk) {

                      //웨이퍼 첫자제 일경우.
                      if(MarkData.GetDataCnt() == 0) {
                          EM_SetErr(eiHED_PenFail);
                          Step.iCycle=0;
                          return true ;
                      }

                      Rqst.bNeedMarkTrace = true ;

                      //아... 병신같다...  CycleMarkTrace에서 ++하고 시작 해서
                      //이거
                      m_iMarkTraceCnt = -1 ; //오퍼가 화면에서 건드릴수가 있으니 마크 할때 마다 클리어 시킨다.
                      Step.iCycle=0;
                      return true ;
                  }

                  if(DM.ARAY[riSTG].GetCntStat(csEror)) { //마스크 영역 작업 하러 가는 사이.
                      FindChip(riSTG , r , c , csEror);
                  }
                  else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //맵파일 페일 영역 작업 하러 가는 사이.
                      FindChip(riSTG , r , c , csFail);
                  }

                  if(!DM.ARAY[riSTG].GetCntStat(csEror) && OM.MstOptn.bHopJumpChip){
                      GetHopJumpChipPos(m_iLastMrkChipR, m_iLastMrkChipC, r, c);
                  }

                  MarkData.PushData(r,c,DM.ARAY[riSTG].GetStat(r,c)==csFail) ;
                  DM.ARAY[riSTG].SetStat(r,c,csWork);
                  m_iLastMrkChipR = r ;
                  m_iLastMrkChipC = c ;

                  if(!DM.ARAY[riSTG].GetCntStat(csEror) && !DM.ARAY[riSTG].GetCntStat(csFail)) {
                      m_iLastMrkChipC = 0;
                      m_iLastMrkChipR = 0;
                  }
                  Step.iCycle=0;
                  return true ;
    }
}


bool CHead::CycleClickMove()
{
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

    double dXPos ;
    double dYPos ;
    double dTPos ;
    int r,c ;
    int iAlignC , iAlignR; //얼라인 칠 칩 오와열.
    int iDataC  , iDataR ; //데이터 참고할 칩 오와열.

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;
                  MoveMotr(miHED_XHed , piHED_XHedVClick);
                  MoveMotr(miHED_YHed , piHED_YHedVClick);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miHED_XHed , piHED_XHedVClick)) return false ;
                  if(!MoveMotr(miHED_YHed , piHED_YHedVClick)) return false ;

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  13: if(!CycleVisn(viWfr)) return false ;
                  Step.iCycle=0;
                  return true ;
    }

}


bool CHead::CycleCntrMove()
{
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

    double dXPos ;
    double dYPos ;
    double dTPos ;
    int r,c ;
    int iAlignC , iAlignR; //얼라인 칠 칩 오와열.
    int iDataC  , iDataR ; //데이터 참고할 칩 오와열.

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  12: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //비젼 검사 못찾음.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //센터칩 찾았으면 보정.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               MM.GetManNo() == mcHED_CycleCntrMoveT );  //세타보정 씀. 스테이지 센터에서는 보정써도 크게 안벋어 난다.
                               //m_iManNo == mcHED_CycleCntrMove

                  MT_GoAbsMan(miHED_XHed , dXPos);
                  MT_GoAbsMan(miHED_YHed , dYPos);
                  MT_GoAbsMan(miSTG_TStg , STG.GetStageAbsMmFromAbsAngle(dTPos));

                  Step.iCycle++;
                  return false ;

        case  13: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

                  CycleVisn(viWfr , true);

                  Step.iCycle++;
                  return false ;

        case  14: if(!CycleVisn(viWfr)) return false ;

                  Step.iCycle=0;
                  return true ;
    }
}


bool CHead::CyclePenChange()
{
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
    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;
                  MT_GoAbsRun(miHED_XHed ,   0);
                  MT_GoAbsRun(miHED_YHed , 200);

                  Step.iCycle++;
                  return false ;

        case  12: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_XHed)) return false ;

                  Step.iCycle=0;
                  return true ;
    }

}


bool CHead::CycleWaitPos()
{
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
    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;

                  MoveMotr(miHED_XHed ,piHED_XHedPCover) ;
                  MoveMotr(miHED_YHed ,piHED_YHedPCover) ;

                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miHED_XHed ,piHED_XHedPCover)) return false ;
                  if(!MoveMotr(miHED_YHed ,piHED_YHedPCover)) return false ;

                  Step.iCycle=0;
                  return true ;
    }
}


bool CHead::CycleMarkTrace()
{
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
    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(MM.GetManNo() != mcHED_CycleMarkTraceFw) { //메뉴얼 백워드거나 오토런에서 찾으러 다닐때.
                      if(m_iMarkTraceCnt >= MarkData.GetDataCnt() -1) {
                          EM_SetErr(eiHED_PenFailOper);
                          Step.iCycle = 0 ;
                          return true ;
                      }

                      m_iMarkTraceCnt ++ ;
                  }
                  else {
                      if(m_iMarkTraceCnt <= 0) { // 메뉴얼 포워드
                          EM_SetErr(eiHED_PenFailOper);
                          Step.iCycle = 0 ;
                          return true ;
                      }
                      m_iMarkTraceCnt -- ;
                  }

                  MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd)) return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd)) return false;

                  MoveMotr(miHED_XHed ,piHED_XHedVMarkTrace) ;
                  MoveMotr(miHED_YHed ,piHED_YHedVMarkTrace) ;

                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miHED_XHed ,piHED_XHedVMarkTrace)) return false ;
                  if(!MoveMotr(miHED_YHed ,piHED_YHedVMarkTrace)) return false ;

                  CycleVisn(viWfp , true);
                  Step.iCycle++;
                  return false ;

        case  13: if(!CycleVisn(viWfp)) return false ;
                  VSM.GetRslt(viWfp , &VsRslt) ;


                  if(MM.GetManNo() == mcNoneCycle) {//메뉴얼로 오퍼가 움직일때는 검사 에러 안띄움.
                      if(VsRslt.iRet == veWfpOk ) {//굿인 경우 멈춰서 에러 띄워서 작업자가 판단하게 한다.
                          EM_SetErr(eiHED_PenFailOper);
                  //        Step.iCycle = 20 ;
                  //        return false ;

                      }
                  }

                  Step.iCycle=0;
                  return true ;

        //오토런시에만 ....
        case  20: //최종 굿을 찾았으면 한칸 앞으로 가서 페일 상태에서 에러.;..
                  if(m_iMarkTraceCnt >=1) m_iMarkTraceCnt -- ;

                  MoveMotr(miHED_XHed ,piHED_XHedVMarkTrace) ;
                  MoveMotr(miHED_YHed ,piHED_YHedVMarkTrace) ;

                  Step.iCycle++;
                  return false ;

        case  21: if(!MoveMotr(miHED_XHed ,piHED_XHedVMarkTrace)) return false ;
                  if(!MoveMotr(miHED_YHed ,piHED_YHedVMarkTrace)) return false ;

                  CycleVisn(viWfp , true);
                  Step.iCycle++;
                  return false ;

        case  22: if(!CycleVisn(viWfp)) return false ;
                  VSM.GetRslt(viWfp , &VsRslt) ;

                  EM_SetErr(eiHED_PenFailOper);

                  Step.iCycle=0;
                  return true ;
    }
}


bool CHead::CyclePenCoverPs()
{
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
    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiHED_PenMrkDU   , ccBwd);
                  MoveActr(aiHED_PenCoverUD , ccBwd);

                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd))return false;
                  if(!MoveActr(aiHED_PenCoverUD , ccBwd))return false;
                  MoveMotr(miHED_XHed , piHED_XHedPCover);
                  MoveMotr(miHED_YHed , piHED_YHedPCover);

                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miHED_XHed , piHED_XHedPCover))return false;
                  if(!MoveMotr(miHED_YHed , piHED_YHedPCover))return false;
                  MoveActr(aiHED_PenCoverUD , ccFwd);

                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiHED_PenCoverUD , ccFwd)) return false;

                  Step.iCycle = 0;
                  return true;
    }
}


void CHead::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CHead::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CHead::CheckStop()
{
    if(!MT_GetStop(miHED_XHed)) return false ;
    if(!MT_GetStop(miHED_YHed)) return false ;
    if(!MT_GetStop(miHED_ZVsn)) return false ;
    if(!MT_GetStop(miHED_TZom)) return false ;

    return true ;
}

void CHead::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_iLastInspChipC" , m_iLastInspChipC );
        UserINI.Load(sPath, "Member" , "m_iLastInspChipR" , m_iLastInspChipR );
        UserINI.Load(sPath, "Member" , "m_iLastMrkChipR " , m_iLastMrkChipR  );
        UserINI.Load(sPath, "Member" , "m_iLastMrkChipC " , m_iLastMrkChipC  );
        UserINI.Load(sPath, "Member" , "m_iTotalPenDnCnt" , m_iTotalPenDnCnt );
    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_iLastInspChipC" , m_iLastInspChipC );
        UserINI.Save(sPath, "Member" , "m_iLastInspChipR" , m_iLastInspChipR );
        UserINI.Save(sPath, "Member" , "m_iLastMrkChipR " , m_iLastMrkChipR  );
        UserINI.Save(sPath, "Member" , "m_iLastMrkChipC " , m_iLastMrkChipC  );
        UserINI.Save(sPath, "Member" , "m_iTotalPenDnCnt" , m_iTotalPenDnCnt ); 

    }
}
//---------------------------------------------------------------------------
