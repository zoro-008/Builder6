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
//sun AT.AllDone�����.
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

//�⺻������ ��Ÿ ������� ��� ���Ҷ� ����������� ���� �����ǿ� ���� �ָ�.
//������ �ǰ� �������� ���� ������ ���.
//--------------------------------------------------------------------------
bool CHead::GetAdjustPos(double  _CrtX , double  _CrtY , double  _CrtT , //���ڴ� ����....//�����.
                         double  _OfsX , double  _OfsY , double  _OfsT , //���� ��� �����V����.
                         double &_RltX , double &_RltY , double &_RltT , //�����. <===�̰� ������ �����ָ� �ȴ�.
                         bool    _bUseTheta)                             //��Ÿ���� ��� ����.
{
    //Local Var.
    double PosX = 0.0 ;
    double PosY = 0.0 ;
    double PosT = 0.0 ;

    if(_bUseTheta){ //��Ÿ���� ����.
        PosX = _OfsX + _CrtX - PM.GetValue(miHED_XHed , pvHED_XHedHWCntrPos) ;  // ���߿� Ȯ�� �ϰ� Ȩ �ɼ� X,Y �� �����ش�.     //sun
        PosY = _OfsY + _CrtY - PM.GetValue(miHED_YHed , pvHED_YHedHWCntrPos) ;
        PosT = DegToRad(_OfsT) ;

        //X,Y�� ���⿡ ���� ��ȣ�� �ݴ�� �ٲ��.
        //���̺��� 210c��.
        //_RltX   = PosX * cos(PosT) - PosY * sin(PosT) + OM.MstOptn.dStgXCt;
        //_RltY   = PosX * sin(PosT) + PosY * cos(PosT) + OM.MstOptn.dStgYCt;
        _RltX   = PosX * cos(PosT) - PosY * sin(PosT) + PM.GetValue(miHED_XHed , pvHED_XHedHWCntrPos);  //�ݴ�.
        _RltY   = PosX * sin(PosT) + PosY * cos(PosT) + PM.GetValue(miHED_YHed , pvHED_YHedHWCntrPos);  //����.

        _RltT   = _CrtT + _OfsT ;
    }

    else{ //��Ÿ���� �ȵ�� ����.
        _RltX = _CrtX + _OfsX;  // ���߿� Ȯ�� �ϰ� Ȩ �ɼ� X,Y �� �����ش�.     //sun
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

    m_bNeedWideWfrChipInsp = false ; //�����˻�.
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
    // ����ũ���� ù �����ϴ� good fail ��ġ
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

    if(iFrstGoodR == iFrstFailR) { //ù�� Ȥ�� ���� ���� ���� ������
        if(iFrstGoodC > iFrstFailC) {r = iFrstFailR ; c = iFrstFailC ; }//������ �� ���࿡ ������ ����
        else                        {r = iFrstGoodR ; c = iFrstGoodC ; }//�ƴ� ��.
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
    // ����ũ���� ù �����ϴ� good fail ��ġ
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

    if(iFrstGoodR == iFrstFailR) { //ù�� Ȥ�� ���� ���� ���� ������
        if(iFrstGoodC > iFrstFailC) {r = iFrstFailR ; c = iFrstFailC ; }//������ �� ���࿡ ������ ����
        else                        {r = iFrstGoodR ; c = iFrstGoodC ; }//�ƴ� ��.
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

    int iErorAreaCol[2]; //�����۸� �� 4��� �Ͽ� ��� ���� 4���� �۾��Ѵ�.
    int iErorAreaRow[2];

    if(_riWhre == riSTG) {
        if(_csStat == csFail) {  //�����Ͽ� ��Ÿ�� �ִ� ��ŷ �� ����.

            r1 = DM.ARAY[_riWhre].FindFrstRow(_csStat) ;
            //����ũ ������ 3���̴� 2���̴� ������ �۾��� ���� ���� ���� �ϰ� ��.
            if((r1-GetStgFrstChipRow())%2) DM.ARAY[_riWhre].FindFrstRowLastCol(_csStat , r1 , c1) ;
            else                                          DM.ARAY[_riWhre].FindFrstRowCol    (_csStat , r1 , c1) ;
            r = r1 ;
            c = c1 ;
            return (r > -1 && c > -1);
        }
        else if(_csStat == csEror) { //������ �� ���� ������ �׵θ� ����ũ ����.
            //�׵θ� ����ũ ���� ���۾� �ϰ� �׵θ� ����ŷ �ϴ� ������ ��ŷ �Ҷ� ������ �¿�� �͵����� ���� �ʰ� �վ�.
            //Eror������ �� 4���� �������� ����. iFrstActivRow , iFrstActivCol , ������ 3��� ����Ʈ ���� �̿�.
            iErorAreaCol[0] = GetStgFrstChipCol() ; iErorAreaCol[1] = DM.ARAY[riSTG].GetMaxCol() * 2 / 3 ;
            iErorAreaRow[0] = DM.ARAY[riSTG].GetMaxRow() * 1 / 3 ; iErorAreaRow[1] = DM.ARAY[riSTG].GetMaxRow() * 2 / 3 ;

            //ù��° ���� ù��°Ĩ ����.
            c1 = DM.ARAY[_riWhre].FindFrstCol(_csStat , 0 , iErorAreaRow[0] , iErorAreaCol[0] , iErorAreaCol[1]);
            if(c1%2) {
                if(DM.ARAY[_riWhre].FindFrstColLastRow(_csStat , r , c , 0 , iErorAreaRow[0] , iErorAreaCol[0] , iErorAreaCol[1])) return true ;
            }
            else     {
                if(DM.ARAY[_riWhre].FindFrstColRow    (_csStat , r , c , 0 , iErorAreaRow[0] , iErorAreaCol[0] , iErorAreaCol[1])) return true ;
            }

            //�߰� �ȵ����� ������ ������.
            r1 = DM.ARAY[_riWhre].FindFrstRow(_csStat , 0 , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[1] , DM.ARAY[riSTG].GetMaxCol()-1);
            if(r1%2) {
                if(DM.ARAY[_riWhre].FindFrstRowLastCol(_csStat , r , c , 0 , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[1] , DM.ARAY[riSTG].GetMaxCol()-1)) return true ;
            }
            else     {
                if(DM.ARAY[_riWhre].FindFrstRowCol    (_csStat , r , c , 0 , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[1] , DM.ARAY[riSTG].GetMaxCol()-1)) return true ;
            }

            //�߰� �ȵ����� ������ �Ʒ���.
            c1 = DM.ARAY[_riWhre].FindLastCol(_csStat , iErorAreaRow[1] , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[0] , iErorAreaCol[1]);
            if(c1%2) {
                if(DM.ARAY[_riWhre].FindLastColFrstRow(_csStat , r , c , iErorAreaRow[1] , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[0] , iErorAreaCol[1])) return true ;
            }
            else     {
                if(DM.ARAY[_riWhre].FindLastColRow    (_csStat , r , c , iErorAreaRow[1] , DM.ARAY[riSTG].GetMaxRow()-1 , iErorAreaCol[0] , iErorAreaCol[1])) return true ;
            }

            //�߰� �ȵ����� ������ ����.
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

//��ġ�� ����� ������ ���� ������ �˻�.
/*
bool CheckInConfirmArea(int _iR , int _iC) //ª���� ��ġ *iAlignConfirmChip  �̸� �̸� ������ ������.
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

//���� ��ġ���� ���� ����� �۾� ��ġ�� ã�Ƽ� ����.
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

//�۾� �Ϸ��� ��ġ�� �ְ� �ű⼭ ���� ����� ������ ���� ��ǥ�� ����.
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
                //todo Ȯ��.
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

//���� �۾� �Ϸ��� ��ġ�� �ְ� �߰��� �����.
//������ ���� ��ġ , ������ �������� ����.
/*
bool IsChipAlignZone(int _iR , int _iC) //����ũ��� ���� Ĩ ����� ���� Ȯ�� ��. ���������� Ȯ�� �ϸ� �۾� ���� csEror => csWork�� �ٲ� �ȉ�.
{
    //���� ���� Ȥ�� �ܰ��׵θ� ����ũ ������ �ƴϸ� Ok
    if(DM.ARAY[riMSK].GetStat(_iR , _iC) != csNone && DM.ARAY[riMSK].GetStat(_iR , _iC) != csEror)return true ;

    return false ;
}*/

bool IsChipAlignZone(int _iR , int _iC) //����ũ��� ���� Ĩ ����� ���� Ȯ�� ��. ���������� Ȯ�� �ϸ� �۾� ���� csEror => csWork�� �ٲ� �ȉ�.
{
    //���� ���� Ȥ�� �ܰ��׵θ� ����ũ ������ �ƴϸ� Ok
    if(_iR < 0 || _iC < 0) return false ;
    if(_iR >= DM.ARAY[riMSK].GetMaxRow() || _iC >= DM.ARAY[riMSK].GetMaxCol()) return false ;

    if(DM.ARAY[riMSK].GetStat(_iR , _iC) != csNone && DM.ARAY[riMSK].GetStat(_iR , _iC) != csEror)return true ;

    return false ;
}

//_iDataR,C �� ����ν� ������ ������ ��ġ,.
//������ �۾��� ��ġ �������� ��,���,��,�Ͽ�,��,����,��,�»� �˻��ؼ� ����� �Ұ��� ������ ����
//������ ��ü ������ �� �˻��Ͽ�... ������� �̵�.

bool GetAlignRowCol(int &_iAlgnR , int &_iAlgnC, int &_iDataR , int &_iDataC)
{
    //������ Ĩ �ֺ��� ���� ��...
    _iDataC = HED.m_iLastInspChipC; // HED.m_iLastInspChipC ���ö��̽ÿ� �� 0�� �ǰ� CycleOrigin , CycleChipAlign  �ÿ� �V�� �ȴ�.
    _iDataR = HED.m_iLastInspChipR;

    //IsOutSide�� Ĩ�� csNone�� �ƴ� �ֿܰ�Ĩ + csNone�� �ƴ�Ĩ
    //��
    if(        IsChipAlignZone(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC)     &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC , 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC , 1) ){
        _iAlgnR = _iDataR - OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC                                ;
        return true;
    }
    //���
    if(        IsChipAlignZone(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC + OM.DevOptn.iVisnChkChipColCnt    ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC + OM.DevOptn.iVisnChkChipColCnt , 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC + OM.DevOptn.iVisnChkChipColCnt , 1) ){
        _iAlgnR = _iDataR - OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC + OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //��
    if(        IsChipAlignZone(_iDataR , _iDataC + OM.DevOptn.iVisnChkChipColCnt   ) &&//!IsOutSide(_iDataR , _iDataC + OM.DevOptn.iVisnChkChipColCnt                ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR , _iDataC + OM.DevOptn.iVisnChkChipColCnt, 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR , _iDataC + OM.DevOptn.iVisnChkChipColCnt, 1)){
        _iAlgnR = _iDataR                                ;
        _iAlgnC = _iDataC + OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //�Ͽ�
    if(        IsChipAlignZone(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC + OM.DevOptn.iVisnChkChipColCnt    ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC + OM.DevOptn.iVisnChkChipColCnt , 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC + OM.DevOptn.iVisnChkChipColCnt , 1) ){
        _iAlgnR = _iDataR + OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC + OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //��
    if(        IsChipAlignZone(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC    ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC , 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC , 1) ){
        _iAlgnR = _iDataR + OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC                                ;
        return true;
    }
    //����
    if(        IsChipAlignZone(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC - OM.DevOptn.iVisnChkChipColCnt    ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC - OM.DevOptn.iVisnChkChipColCnt , 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR + OM.DevOptn.iVisnChkChipRowCnt, _iDataC - OM.DevOptn.iVisnChkChipColCnt , 1)){
        _iAlgnR = _iDataR + OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC - OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //��.
    if(        IsChipAlignZone(_iDataR , _iDataC - OM.DevOptn.iVisnChkChipColCnt   ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR , _iDataC - OM.DevOptn.iVisnChkChipColCnt, 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR , _iDataC - OM.DevOptn.iVisnChkChipColCnt, 1)){
        _iAlgnR = _iDataR                                ;
        _iAlgnC = _iDataC - OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //�»�.
    if(        IsChipAlignZone(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC - OM.DevOptn.iVisnChkChipColCnt   ) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC - OM.DevOptn.iVisnChkChipColCnt, 0) &&
       !DM.ARAY[riSTG].GetTemp(_iDataR - OM.DevOptn.iVisnChkChipRowCnt , _iDataC - OM.DevOptn.iVisnChkChipColCnt, 1)){
        _iAlgnR = _iDataR - OM.DevOptn.iVisnChkChipRowCnt;
        _iAlgnC = _iDataC - OM.DevOptn.iVisnChkChipColCnt;
        return true;
    }
    //����� ��������...
    //���� �̵�.
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

    //���������� �̵�.
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

    //�Ʒ��� �̵�.
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

    //�����̵�.
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
                //todo Ȯ��.
                dXGap = (_iTrgC - c) * OM.DevInfo.dWfPitchX  ;
                dYGap = (_iTrgR - r) * OM.DevInfo.dWfPitchY ;
                dDist = sqrt(pow(dXGap , 2) + pow(dYGap , 2)) ;
                if(dDist < dMinDist) {
                    dMinDist  = dDist ;
                    iMinC = c ;         //���� ����  Colunm
                    iMinR = r ;         //���� ����  Row
                    dXMinGap = dXGap ;
                    dYMinGap = dYGap ;
                }
            }
        }
    }

    //X,Y������ �ִ� Ĩ�� ����.
    if(iMinC == -1 || iMinR == -1) return false ;

    //_iDataR = iMinR ;
    //_iDataC = iMinC ;

    if(dMinDist <= dRadius){ //Ÿ�� �������� ������ �ݰ�ȿ� �ִ°��� Ÿ���������� ����� ������.
        _iAlgnR = _iTrgR ;
        _iAlgnC = _iTrgC ;
        return true ;
    }

/*
//dXMinGap(_iTrgC - c)�� ����� ����������
//dYMinGap(_iTrgR - r)�� ����� ������
//�Ѱ��� ����(pX1, pX2, pY1, pY2)�� ������ ������ ������ ���ϴ� ����
//return 0 = ������ ���� ���
//       1 = ������ 1���� ���
//       2 = ������ 2���� ���
//���� ������ ����.
int __stdcall MATH_GetCircleSegmentIntersection(float _fX1      , float _fY1      , float _fX2    , float _fY2 ,
                                                float _fRadius  , float _fCntrX   , float _fCntrY ,
                                                TFPoint *_pPnt1 , TFPoint *_pPnt2 )
*/
    //������ ��� ��̼��� �����̱⿡ ������,�Ʒ����� �÷���
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

    //������ ��ġ�� �̵��Ͽ� ���� �ִ� ������ ã�´�.
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

    //������ ��ġ�� �̵��Ͽ� ���� �ִ� ������ ã�´�.
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

    //������ ��ġ�� �̵��Ͽ� ���� �ִ� ������ ã�´�.
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

    //������ ��ġ�� �̵��Ͽ� ���� �ִ� ������ ã�´�.
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

double CHead::GetWaferPitchX  (double _dOffsetX) //���� X ��ġ ���ϴ� �Լ�
{
    double dXPitch = OM.DevInfo.dWfPitchX ;
    double dReviseX ;

    dReviseX = dXPitch + _dOffsetX ;

    return dReviseX ;
}

double CHead::GetWaferPitchY  (double _dOffsetY) //���� Y ��ġ ���ϴ� �Լ�
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
        if(!GetCloseDataPos(iTrgR,iTrgC,iDataR,iDataC)) {FM_MsgTime("ERROR","�������� ��ŸƮ �� �Ǵ� ����� ���� Ȯ�� ���ּ���!",2000); return ;}
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

    //���� ��ũ ������.---------------------------------------------------------
    FindChip(riBRD , r , c , csUnkwn);
    double dXHedVBoardPos    = PM.GetValue(miHED_XHed , pvHED_XHedVBoardStt) - OM.DevInfo.dBdXPch * c ; //todo
    double dYHedVBoardPos    = PM.GetValue(miHED_YHed , pvHED_YHedVBoardStt) + OM.DevInfo.dBdYPch * r ;
    double dXHedPBoardPos    = dXHedVBoardPos + PM.GetValue(miHED_XHed , poHED_XHedVisnPenOfs) ; //���� ���� ��ũ ������.
    double dYHedPBoardPos    = dYHedVBoardPos + PM.GetValue(miHED_YHed , poHED_YHedVisnPenOfs) ;
    double dXHedPBoardSttPos = PM.GetValue(miHED_XHed , pvHED_XHedVBoardStt)+PM.GetValue(miHED_XHed , poHED_XHedVisnPenOfs) ; //���� ���� ��ŸƮ ������.
    double dYHedPBoardSttPos = PM.GetValue(miHED_YHed , pvHED_YHedVBoardStt)+PM.GetValue(miHED_YHed , poHED_YHedVisnPenOfs) ;

    //����� ������.------------------------------------------------------------
    double dXHedAlignPos  ;
    double dYHedAlignPos  ;
    int    iAlignC , iAlignR; //����� ĥ Ĩ ���Ϳ�.
    int    iDataC  , iDataR ; //������ ������ Ĩ ���Ϳ�.
    if(DM.ARAY[riSTG].GetCntStat(csEror)) { //����ũ ���� �۾� �Ϸ� ���� ����.
        FindChip(riSTG , r , c , csEror);
    }
    else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //������ ���� ���� �۾� �Ϸ� ���� ����.
        FindChip(riSTG , r , c , csFail);
    }
    if(GetAlignRowCol(iAlignR, iAlignC, iDataR, iDataC)){

        dTempPosX = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) ; //X  ��
        dTempPosY = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) ; //Y ��.
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
    //   GetNonOutSide(iAlignR , iAlignC , iAlignR , iAlignC )){  //r,c�۾�Ÿ��,iAlignR , iAlignC������� Ÿ��,iDataR , iDataC��ġ������ ������ Ÿ��.
    //    dXHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (iAlignC - iDataC) * OM.DevInfo.dWfPitchX  ;//���͹��⿡ ���� -+��ȣ �ٲ�.
    //    dYHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (iAlignR - iDataR) * OM.DevInfo.dWfPitchY ;//���͹��⿡ ���� -+��ȣ �ٲ�.
    //}
    else {
        dXHedAlignPos = MT_GetCmdPos(miHED_XHed);
        dYHedAlignPos = MT_GetCmdPos(miHED_YHed);
    }

    //Ŭ�� ��ġ..............................................................
    double dXHedClickPos  ;
    double dYHedClickPos  ;
    if(DM.ARAY[riSTG].GetCntStat(csEror)) { //����ũ ���� �۾� �Ϸ� ���� ����.
        FindChip(riSTG , r , c , csEror);
    }
    else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //������ ���� ���� �۾� �Ϸ� ���� ����.
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
    //if(GetAlignRowCol(m_iClickMoveRow , m_iClickMoveCol , iAlignR , iAlignC , iDataR , iDataC)){  //r,c�۾�Ÿ��,iAlignR , iAlignC������� Ÿ��,iDataR , iDataC��ġ������ ������ Ÿ��.
    //    dXHedClickPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (m_iClickMoveCol - iDataC) * OM.DevInfo.dWfPitchX  ;//���͹��⿡ ���� -+��ȣ �ٲ�.
    //    dYHedClickPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (m_iClickMoveRow - iDataR) * OM.DevInfo.dWfPitchY ;//���͹��⿡ ���� -+��ȣ �ٲ�.
    //}
    else {                                      //20141022 sun �̻��ؼ� �ٲ�.
        dXHedClickPos= MT_GetCmdPos(miHED_XHed);//dXHedAlignPos = MT_GetCmdPos(miHED_XHed);
        dYHedClickPos= MT_GetCmdPos(miHED_YHed);//dYHedAlignPos = MT_GetCmdPos(miHED_YHed);
    }

    //�� , �� üũ , ��Ŀ�� ������.---------------------------------------------
    double dXHedPenPos ;
    double dYHedPenPos ;
    double dXHedPenChkPos ;
    double dYHedPenChkPos ;

    if(DM.ARAY[riSTG].GetCntStat(csEror)) { //����ũ ���� �۾� �Ϸ� ���� ����.
        FindChip(riSTG , r , c , csEror);
    }
    else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //������ ���� ���� �۾� �Ϸ� ���� ����.
        FindChip(riSTG , r , c , csFail);
    }
    if(OM.MstOptn.bHopJumpChip && !DM.ARAY[riSTG].GetCntStat(csEror)){GetHopJumpChipPos(m_iLastMrkChipR , m_iLastMrkChipC , r, c);}
    if(GetCloseDataPos(r , c , iDataR , iDataC)) {   //r,c�۾�Ÿ��,iDataR , iDataC��ġ������ ������ Ÿ��.
        if(m_bFnshCycTAlgn){
            dXHedPenChkPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (c - iDataC) * m_dRevisePitchX + PM.GetValue(miHED_XHed , poHED_XHedChipPenOfs);//���͹��⿡ ���� -+��ȣ �ٲ�.
            dYHedPenChkPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (r - iDataR) * m_dRevisePitchY + PM.GetValue(miHED_YHed , poHED_YHedChipPenOfs);//���͹��⿡ ���� -+��ȣ �ٲ�.
        }
        else{
            dXHedPenChkPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (c - iDataC) * OM.DevInfo.dWfPitchX + PM.GetValue(miHED_XHed , poHED_XHedChipPenOfs);//���͹��⿡ ���� -+��ȣ �ٲ�.
            dYHedPenChkPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (r - iDataR) * OM.DevInfo.dWfPitchY + PM.GetValue(miHED_YHed , poHED_YHedChipPenOfs);//���͹��⿡ ���� -+��ȣ �ٲ�.
        }
        dXHedPenPos    = dXHedPenChkPos + PM.GetValue(miHED_XHed , poHED_XHedVisnPenOfs);
        dYHedPenPos    = dYHedPenChkPos + PM.GetValue(miHED_YHed , poHED_YHedVisnPenOfs);
    }
    else {                                   //20141022 sun �̻��ؼ� �ٲ�.
        dXHedPenPos =MT_GetCmdPos(miHED_XHed); //dXHedAlignPos =
        dYHedPenPos =MT_GetCmdPos(miHED_YHed); //dYHedAlignPos =
    }

    //��ũ Ʈ���̽� ������......................................................
    double dXHedVMarkTracePos ;
    double dYHedVMarkTracePos ;

    r = MarkData.GetDataR(m_iMarkTraceCnt);
    c = MarkData.GetDataC(m_iMarkTraceCnt);

    if(GetCloseDataPos(r , c , iDataR , iDataC)) {
        if(m_bFnshCycTAlgn){
            dXHedVMarkTracePos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (c - iDataC) * m_dRevisePitchX + PM.GetValue(miHED_XHed , poHED_XHedChipPenOfs);//���͹��⿡ ���� -+��ȣ �ٲ�.
            dYHedVMarkTracePos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (r - iDataR) * m_dRevisePitchY + PM.GetValue(miHED_YHed , poHED_YHedChipPenOfs);//���͹��⿡ ���� -+��ȣ �ٲ�.
        }
        else{
            dXHedVMarkTracePos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (c - iDataC) * OM.DevInfo.dWfPitchX + PM.GetValue(miHED_XHed , poHED_XHedChipPenOfs);//���͹��⿡ ���� -+��ȣ �ٲ�.
            dYHedVMarkTracePos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (r - iDataR) * OM.DevInfo.dWfPitchY + PM.GetValue(miHED_YHed , poHED_YHedChipPenOfs);//���͹��⿡ ���� -+��ȣ �ٲ�.
        }
    }

    //T ����� ������.----------------------------------------------------------
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
bool CHead::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // ���͸� �����϶� ���� �Լ�.
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
bool CHead::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
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
                 //������ �����͸� �� ������ �Ѵ�.
                 DM.ARAY[riSTG].ClearTemp();

                 Rqst.bNeedCal    = true ;
                 Rqst.bNeedTAlign = true ;

                 Step.iHome = 0;
                 return true ;
    }
}

//---------------------------------------------------------------------------
bool CHead::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

//---------------------------------------------------------------------------
bool CHead::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

//---------------------------------------------------------------------------
bool CHead::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
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

        case 12: if(OM.CmnOptn.iPenDnCnt) Rqst.bNeedPenCheck = true ; //���� ���־����� ���� ������ Ȯ�� �۾�.

                 Step.iToStart = 0 ;
                 return true ;
    }
}

//---------------------------------------------------------------------------
bool CHead::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
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

        case 10: if(EM_GetErr((EN_ERR_ID)eiHED_PenMrkDUTO     )|| //�渶ŷ�Ǹ��� �������� �ʱ���ġ�� ���� �ѷ���.
                    EM_GetErr((EN_ERR_ID)eiHED_OriVisnInspFail)|| //������ �˻� ���п��� �ϰ��..����Ĩ �˻��϶� �Ŵ���� �����ϰ�
                    EM_GetErr((EN_ERR_ID)eiHED_OriVisnRlstOver)){ //��� �ϴ� �־.
                     Step.iToStop = 0;

                     return true;
                 }

                 MoveActr(aiHED_PenMrkDU   , ccBwd);
                 MoveActr(aiHED_PenCoverUD , ccBwd);
                 Step.iToStop ++;
                 return false ;

        case 11: if(!MoveActr(aiHED_PenMrkDU   , ccBwd))return false;
                 if(!MoveActr(aiHED_PenCoverUD , ccBwd))return false;

                 //���������̼� ��ƾ� �ϴµ� ������.
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
bool CHead::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;


        int er , ec ; //ErrorĨ.
        int fr , fc ; //Fail Ĩ.
        int wr , wc ; //�۾� �ؾ� �� Ĩ.
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
        //�۾� �ؾ� �ϴ� Ĩ�� �켱������ ����. 1.Eror 2.Fail.
        /*
        if(DM.ARAY[riSTG].GetCntStat(csEror)) { //����ũ ���� �۾� �Ϸ� ���� ����.
            wr = er ; wc = ec ;
        }
        else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //������ ���� ���� �۾� �Ϸ� ���� ����.
            wr = fr ; wc = fc ;
        */

        bool bNeedCycleSupply = DM.ARAY[riCST].GetCntStat  (csUnkwn) && DM.ARAY[riSTG].CheckAllStat(csNone) ;
        bool bNeedCyclePlace  = DM.ARAY[riCST].GetCntStat  (csMask ) && //ī��Ʈ�� �������� �ְ�
                               !DM.ARAY[riSTG].GetCntStat  (csFail ) && //������ �Ǵ� ���� ����.
                               !DM.ARAY[riSTG].CheckAllStat(csNone ) && //�����۴� �־�� �ǰ�.
                               !DM.ARAY[riSTG].GetCntStat  (csWait ) ;  //���� ��ٸ��� ���� ����.
        bool bHedPenPos      = MT_CmprPos(miHED_XHed , PM.GetValue(miHED_XHed , pvHED_XHedPCover)) &&
                               MT_CmprPos(miHED_YHed , PM.GetValue(miHED_YHed , pvHED_YHedPCover)) ;

        bool bTrsfWaitPos     = MT_CmprPos(miSTG_XTrs , PM.GetValue(miSTG_XTrs , pvSTG_XTrsWait)) ;
        bool bStageReady      = bTrsfWaitPos && AT_Complete(aiSTG_ChkTblUD , ccFwd);

        /*
        isCycleCal       //���� �̸��극�̼�.                              Ȩ/��������
        isCyclePenCheck  //�� �������� ȭ��Ʈ ���� ���� ���� üũ.     Ȩ/��������
        isCycleTAlign    //���� �簢������ ������ ��Ÿ ����� ��°�.      Ȩ/��������
        isCycleOrigin    //���� ��ġ�� ���� Ĩ ã��.                       Ȩ/��������/��� ��ŸƮ��.
        isCycleMark      //���� �簢������ Ĩ ����� ��ġ ����.
        isCycleChipAlign //�渶ŷ & ��쿡 ���� ��� ���� Ȯ�� ����.
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
            //�ڵ����� ���� �ش�.
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

//���� ����Ŭ
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

        case  10: VSM.SetLight(_iId); //���� ����. rs-232�� ����. �̸� ���� ���ƾ���.

                  m_tmVisnTemp.Clear();
                  Step.iVisn++ ;
                  return false ;

        case  11: if(!m_tmVisnTemp.OnDelay(true , OM.CmnOptn.iVsBfGrabDly)) return false ;

                  VSM.Grab(_iId);
                  m_tmVisnTemp.Clear() ;
                  Step.iVisn++;
                  return false;

                  //�ؿ��� �� ��ȣ ����.
        case  12: if(m_tmVisnTemp.OnDelay(!VSM.GetGrabEnd(_iId),2000)) {
                      EM_SetErr(eiHED_VisnGrabFail);
                      Step.iVisn = 0 ;
                      return true ;
                  }
                  if(!VSM.GetGrabEnd(_iId)) return false ;

                  //FOV�а� �˻��Ѵ�.
                  if(m_bNeedWideWfrChipInsp) {VSM.SetWfrOri1WideInspection(true); m_bNeedWideWfrChipInsp = false ; }
                  if(m_bNeedWideWfoChipInsp) {VSM.SetWfoOri1WideInspection(true); m_bNeedWideWfoChipInsp = false ; }

                  VSM.Inspect(_iId,false);
                  VSM.SetWfrOri1WideInspection(false); //�ٷ� ���� �Ѵ�.
                  VSM.SetWfoOri1WideInspection(false); //�ٷ� ���� �Ѵ�.

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
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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

    if(Stat.bReqStop) { //�˻��ϸ鼭 ���� ���� ���߰� Rqst.bNeedCal �÷��� ��� �ְ�.
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
                  if(VsRslt.iRet != veCalOk) { //���� �˻� ����.
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
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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

    if(Stat.bReqStop) { //�˻��ϸ鼭 ���� ���� ���߰� Rqst. �÷��� ��� �ְ�.
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

        /*  // �� ���������� �ߴ� ����Ŭ
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

        //�ؿ��� ��.
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
                  if(OM.CmnOptn.iPenDnCnt > iPenDnCnt) { //3���� �����Ǿ� ������ ���ڸ����� 3ȸ ��´�.
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

                  if(VsRslt.iRet != veWfpOk) { //���� �˻� ����.
                      if(MM.GetManNo() == mcNone)EM_SetErr(eiHED_PenFail);
                      Step.iCycle = 30;
                      return false;
                  }

                  //�޴��� ������  �϶��� ���� �� ������ ����.
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

                  //ó�� �۾� �ϴ� Ĩ�� ������ �˻� �ϰ�.
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
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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

    if(Stat.bReqStop) { //�˻��ϸ鼭 ���� ���� ���߰� Rqst. �÷��� ��� �ְ�.
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
    const POINT Neib[8] = { {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1} ,{1, 0}, {1,-1} }; // ��ġ�� �̵� ���� �� ��ġ�ϴ� Ĩ�� ������ ���� 8���� �˻�...
    static int iNextDir = 0 ; // ��ġ�� �̵� ���� �� ��ġ�ϴ� Ĩ�� ������ ���� 8���� �˻�...


    //���� Ĩ ��ġ.
    static double dCntChipPosX = 0.0;
    static double dCntChipPosY = 0.0;


    //�¿� �˻� �����.
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

        //�ؿ�����. ������ ��Ÿ ���� ���� Ĩ ã��.
        case  12: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;

                  m_bNeedWideWfrChipInsp = true ; //�����˻�.
                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

//============================����Ĩ 1�� ����===================================

        case  13: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //���� �˻� ��ã��.
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

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               true );  //��Ÿ���� ��. �������� ���Ϳ����� �����ᵵ ũ�� �ȹ��� ����.

                  MT_GoAbsRun(miHED_XHed , dXPos);
                  MT_GoAbsRun(miHED_YHed , dYPos);
                  MT_GoAbsRun(miSTG_TStg , STG.GetStageAbsMmFromAbsAngle(dTPos));

                  Step.iCycle++;
                  return false ;

        case  14: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

//========================����Ĩ 1�� ���� �Ϸ�==================================
//========================����Ĩ 2�� ���� ======================================

                  //���е� ������ �ѹ��� �˻�.
                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  15: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //���� �˻� ��ã��.
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

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               true );  //��Ÿ���� ��. �������� ���Ϳ����� �����ᵵ ũ�� �ȹ��� ����.

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

//========================����Ĩ 2�� ���� �Ϸ�==================================
//===================���� Ĩ �������� 1��ġ �̵� �� �˻�========================

                  MT_GoIncRun(miHED_XHed , OM.DevInfo.dWfPitchX);

                  Step.iCycle++;
                  return false ;

        case  17: if(!MT_GetStopInpos(miHED_XHed)) return false ;

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  18: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //���� �˻� ��ã��.
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

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //��Ÿ���� �Ⱦ�.


                  dLeftChipPosX = dXPos ;
                  dLeftChipPosY = dYPos ;

                  MT_GoIncRun(miHED_XHed , -OM.DevInfo.dWfPitchX*2);

                  Step.iCycle++;
                  return false ;

        case  19: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;

//===================���� Ĩ �������� 1��ġ �̵� �� �˻� �Ϸ�===================
//===================���� Ĩ ���������� 1��ġ �̵� �� �˻�======================

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  20: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //���� �˻� ��ã��.
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

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //��Ÿ���� �Ⱦ�.


                  dRightChipPosX = dXPos ;
                  dRightChipPosY = dYPos ;

                  dAngle = MATH_GetLineAngle(dLeftChipPosX , dLeftChipPosY , dRightChipPosX , dRightChipPosY ) ;

                  //������.
                  dAngle =  dAngle - 180.0;

                  dAngle2 = STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ;

                  MT_GoAbsRun(miHED_XHed , dCntChipPosX);
                  MT_GoAbsRun(miHED_YHed , dCntChipPosY);
                  MT_GoAbsRun(miSTG_TStg , STG.GetStageAbsMmFromAbsAngle(dAngle2)+dAngle);

//============���� Ĩ ���������� 1��ġ �̵� �� �˻� �� ��Ÿ ���� �Ϸ�===========
//===========================���� ��ġ �� ���ϱ�================================

                  Step.iCycle++;
                  return false;

        case  21: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

                  MT_GoIncRun(miHED_XHed , OM.DevInfo.dWfPitchX ); //����ġ���� �������� Ȯ��.
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

                  //���� �˻� ��ã��.
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

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //��Ÿ���� �Ⱦ�.

                  //������.
                  //OM.DevInfo.dWfPitchX = (dXPos - dCntChipPosX) ;
                  //OM.DevInfo.dWfPitchY = (dYPos - dCntChipPosY) ;

//============================������ ���� ����==================================
//===========================T����� ���� �˻�==================================
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

                  //���ʿ� ���� �˻�.
                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  25: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //���� �˻� ��ã��.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  //���� �������� ����� ����.
                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //��Ÿ���� �Ⱦ�.

                  dLeftChipPosX = dXPos ;
                  dLeftChipPosY = dYPos ;

//=============================���� �˻� ���� ����==============================
//===============================������ �˻� ����===============================
                  MT_GoAbsRun(miHED_XHed , dCntChipPosX - (GetWaferPitchX(dOfsetChipPosX) * OM.DevOptn.iVisnChkChipColCnt));
                  MT_GoAbsRun(miHED_YHed , dCntChipPosY );



                  //MT_GoAbsRun(miHED_XHed , dCntChipPosX - (GetWaferPitchX(VsRslt.fRsltX) * OM.DevOptn.iVisnChkChipColCnt)); //�˻簣�� 2�� ��ŭ ������ �̵� ����.
                  //MT_GoAbsRun(miHED_YHed , dCntChipPosY);
                  Step.iCycle++;
                  return false ;

        case  26: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;

                  //���� ���� ���� �˻�.
                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  27: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //���� �˻� ��ã��.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  //���� �������� ����� ����.
                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //������Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //��Ÿ���� �Ⱦ�.

                  dRightChipPosX = dXPos ;
                  dRightChipPosY = dYPos ;

                  dAngle = MATH_GetLineAngle(dLeftChipPosX , dLeftChipPosY , dRightChipPosX , dRightChipPosY ) ;

                  //������.
                  dAngle =  dAngle - 180.0;

                  dAngle2 = STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ;

//============================������ �˻� ���� ����=============================
//=============================��ġ ���� �˻� ����==============================

                  MT_GoAbsRun(miHED_XHed , dCntChipPosX);
                  MT_GoAbsRun(miHED_YHed , dCntChipPosY);
                  MT_GoAbsRun(miSTG_TStg , STG.GetStageAbsMmFromAbsAngle(dAngle2) + dAngle);

                  Step.iCycle++;
                  return false ;

        case  28: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

                  //T������ ����Ĩ���� �˻�.
                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  29: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //���� �˻� ��ã��.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  //���� �������� ����� ����.
                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //��Ÿ���� �Ⱦ�.

                  MT_GoAbsRun(miHED_XHed , dXPos);
                  MT_GoAbsRun(miHED_YHed , dYPos);
                  Step.iCycle++;
                  return false ;

        case  30: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

//===========================����Ĩ 1�� ���� �˻�===============================
//==============================����Ĩ 2�� ����=================================

                  CycleVisn(viWfr , true);
                  Step.iCycle++;
                  return false ;

        case  31: if(!CycleVisn(viWfr)) return false ;
                  VSM.GetRslt(viWfr , &VsRslt) ;

                  //���� �˻� ��ã��.
                  if(VsRslt.iRet != veWfrOk) {
                      EM_SetErr(eiHED_VisnInspFail);
                      Step.iCycle=0;
                      return true ;
                  }

                  //���� �������� ����� ����.
                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //������Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //��Ÿ���� �Ⱦ�.

                  MT_GoAbsRun(miHED_XHed , dXPos); //�˻� ���� �̵� �� ��ġ ����.
                  MT_GoAbsRun(miHED_YHed , dYPos);

                  dCntChipPosX = dXPos ;
                  dCntChipPosY = dYPos ;

                  Step.iCycle++;
                  return false ;

        case  32: if(!MT_GetStopInpos(miHED_XHed)) return false ; //��� ��.
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

//===========================2�� ����Ĩ ã�� �Ϸ�===============================
//==============================��ġ ���� ����==================================

                  MT_GoIncRun(miHED_XHed , GetWaferPitchX(VsRslt.fRsltX) * OM.DevOptn.iVisnChkChipColCnt); //�˻� ���� �̵� �� ��ġ ����.
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

                  //���� �˻� ��ã��.
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

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //��Ÿ���� �Ⱦ�.

                  MT_GoAbsRun(miHED_XHed , dXPos); //�˻� ���� �̵� �� ��ġ ����.
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

                  //���� �˻� ��ã��.
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

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm( MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //��Ÿ���� �Ⱦ�.

                  //��ġ ����.
                  //OM.DevInfo.dWfPitchX = (dXPos - dCntChipPosX)/ OM.DevOptn.iVisnChkChipColCnt;
                  //OM.DevInfo.dWfPitchY = (dYPos - dCntChipPosY)/ OM.DevOptn.iVisnChkChipRowCnt;

                  //if(MM.GetManNo() != mcHED_CycleTAlign){
                      Rqst.bNeedTAlign = false ;
                      Rqst.bNeedOrigin = true  ;
                  //}

                  m_dRevisePitchX = GetWaferPitchX(VsRslt.fRsltX);
                  m_dRevisePitchY = GetWaferPitchY(VsRslt.fRsltY);
                  m_bFnshCycTAlgn = true;

                  //��Ÿ�� �ٲ㼭 ������ ������ Ŭ����.
                  DM.ARAY[riSTG].ClearTemp();

                  Step.iCycle=0;
                  return false ;
    }
}


bool CHead::CycleOrigin(void)
{
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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

        //�ؿ�����. ������ ��Ÿ ���� ���� Ĩ ã��.
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

                  //���� �˻� ��ã��.
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

                  //����Ĩ ã������ ����.
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

                  //Ĩ �ѹ��� �˻�.
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

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               false );  //��Ÿ���� ��. �������� ���Ϳ����� �����ᵵ ũ�� �ȹ��� ����.

                  MT_GoAbsRun(miHED_XHed , dXPos);
                  MT_GoAbsRun(miHED_YHed , dYPos);
                  MT_GoAbsRun(miSTG_TStg , STG.GetStageAbsMmFromAbsAngle(dTPos));

                  Step.iCycle++;
                  return false ;

        case  17: if(!MT_GetStopInpos(miHED_XHed)) return false ;
                  if(!MT_GetStopInpos(miHED_YHed)) return false ;
                  if(!MT_GetStopInpos(miSTG_TStg)) return false ;

                  //����ũ�� ����Ĩ�� ������ ����....
                  if(!FindChip(riMSK,r,c,csOrig)) {
                      EM_SetErr(eiSTG_NoOrig);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                        if(MM.GetManNo() == mcHED_CycleOrigin){ //�޴��� ���۽ÿ�...
                            //�ڵ��������϶��� �����ϰ� �����϶��� �ٽ� �ϰԲ�.
                            if(OM.DevOptn.bAutoOrigin) { Rqst.bNeedOrigin = false;}
                            else                       { Rqst.bNeedOrigin = true ;}
                            FrmOperation -> btManOri -> Enabled = Rqst.bNeedOrigin ;
                        }

                  //�޴��� ���������̼� �϶��� ��� ����� ���۰� �⵵��.
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

                  dTempPosX = DM.ARAY[riSTG].GetTemp(r,c,0) ; //X  ��
                  dTempPosY = DM.ARAY[riSTG].GetTemp(r,c,1) ; //Y ��.

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
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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
    int iAlignC , iAlignR; //����� ĥ Ĩ ���Ϳ�.
    int iDataC  , iDataR ; //������ ������ Ĩ ���Ϳ�.

    const int iVisnErrMaxCnt = 5 ; //Ĩ�˻�ÿ� 5������ ������ �׳� ���� ���������� ���� �ϰ� �Ѿ�� ���� ����.
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

                  //���� �˻� ��ã��.
                  if(VsRslt.iRet != veWfrOk) {
                      iVisnErrCnt++;
                      if(iVisnErrCnt >= iVisnErrMaxCnt) {
                          iVisnErrCnt = 0 ;
                          EM_SetErr(eiHED_VisnInspFail);
                          Step.iCycle=0;
                          return true ;
                      }
                  }
                  else { //���϶� ī���� Ŭ����.
                      iVisnErrCnt = 0 ;
                  }

                  if(!VisnRsltLmtCheck(&VsRslt)) {
                      EM_SetErr(eiHED_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //����Ĩ ã������ ����.
                  if(VsRslt.iRet == veWfrOk) { //���϶��� ����.
                      GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                                   VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                                   dXPos                    , dYPos                    , dTPos                    ,
                                   false );  //��Ÿ���� ��. �������� ���Ϳ����� �����ᵵ ũ�� �ȹ��� ����.
                  }
                  else {
                      dXPos = MT_GetCmdPos(miHED_XHed) ;
                      dYPos = MT_GetCmdPos(miHED_YHed) ;
                  }

                  if(DM.ARAY[riSTG].GetCntStat(csEror)) { //����ũ ���� �۾� �Ϸ� ���� ����.
                      FindChip(riSTG , r , c , csEror);
                  }
                  else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //������ ���� ���� �۾� �Ϸ� ���� ����.
                      FindChip(riSTG , r , c , csFail);
                  }
                  //����� ��ġ�� �ʿ� ǥ�ð� �ȵǼ� FindChip�� ���� ����.
                  GetAlignRowCol(iAlignR , iAlignC, iDataR , iDataC);
                  //GetAlignRowCol(r , c , iAlignR , iAlignC , iDataR , iDataC) ;  //r,c�۾�Ÿ��,iAlignR , iAlignC������� Ÿ��,iDataR , iDataC��ġ������ ������ Ÿ��.
                  //GetNonOutSide (iAlignR , iAlignC , iAlignR , iAlignC ) ;

                  /*
                  if(GetAlignRowCol(r , c , iAlignR , iAlignC , iDataR , iDataC) && ){  //r,c�۾�Ÿ��,iAlignR , iAlignC������� Ÿ��,iDataR , iDataC��ġ������ ������ Ÿ��.
                      dXHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 0) - (iAlignC - iDataC) * OM.DevInfo.dWfPitchX  ;//���͹��⿡ ���� -+��ȣ �ٲ�.
                      dYHedAlignPos = DM.ARAY[riSTG].GetTemp(iDataR , iDataC , 1) + (iAlignR - iDataR) * OM.DevInfo.dWfPitchY ;//���͹��⿡ ���� -+��ȣ �ٲ�.
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
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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

        case  10: /*���� �׽�Ʈ��.
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

                  iTemp1 = DM.ARAY[riSTG].GetCntStat(csWork) ; //ù �۾� ����  Ȯ�� �Ѵ�.
                  iTemp2 = DM.ARAY[riSTG].GetCntStat(csFail) ; //������ ���� Ȯ��.

                  //ù�����̸� Ŭ���� ��Ų��.
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

                  if(DM.ARAY[riSTG].GetCntStat(csEror)) { //����ũ ���� �۾� �Ϸ� ���� ����.
                      FindChip(riSTG , r , c , csEror);
                  }
                  else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //������ ���� ���� �۾� �Ϸ� ���� ����.
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

        //�� ������ �������� Ȯ�� �ϴ� ��.
        case  20: if(!MoveMotr(miHED_XHed , piHED_XHedVWrk)) return false ;
                  if(!MoveMotr(miHED_YHed , piHED_YHedVWrk)) return false ;

                  CycleVisn(viWfp , true);
                  Step.iCycle++;
                  return false ;

        case  21: if(!CycleVisn(viWfp)) return false ;
                  VSM.GetRslt(viWfp , &VsRslt) ;

                  //���� �˻� ��ã��.
                  if(VsRslt.iRet != veWfpOk) {

                      //������ ù���� �ϰ��.
                      if(MarkData.GetDataCnt() == 0) {
                          EM_SetErr(eiHED_PenFail);
                          Step.iCycle=0;
                          return true ;
                      }

                      Rqst.bNeedMarkTrace = true ;

                      //��... ���Ű���...  CycleMarkTrace���� ++�ϰ� ���� �ؼ�
                      //�̰�
                      m_iMarkTraceCnt = -1 ; //���۰� ȭ�鿡�� �ǵ帱���� ������ ��ũ �Ҷ� ���� Ŭ���� ��Ų��.
                      Step.iCycle=0;
                      return true ;
                  }

                  if(DM.ARAY[riSTG].GetCntStat(csEror)) { //����ũ ���� �۾� �Ϸ� ���� ����.
                      FindChip(riSTG , r , c , csEror);
                  }
                  else if(DM.ARAY[riSTG].GetCntStat(csFail)) { //������ ���� ���� �۾� �Ϸ� ���� ����.
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
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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
    int iAlignC , iAlignR; //����� ĥ Ĩ ���Ϳ�.
    int iDataC  , iDataR ; //������ ������ Ĩ ���Ϳ�.

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
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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
    int iAlignC , iAlignR; //����� ĥ Ĩ ���Ϳ�.
    int iDataC  , iDataR ; //������ ������ Ĩ ���Ϳ�.

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

                  //���� �˻� ��ã��.
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

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miHED_XHed) , MT_GetCmdPos(miHED_YHed) , STG.GetStageAbsAngleFromAbsMm(MT_GetCmdPos(miSTG_TStg)) ,
                               VsRslt.fRsltX            , VsRslt.fRsltY            , VsRslt.fRsltT            ,
                               dXPos                    , dYPos                    , dTPos                    ,
                               MM.GetManNo() == mcHED_CycleCntrMoveT );  //��Ÿ���� ��. �������� ���Ϳ����� �����ᵵ ũ�� �ȹ��� ����.
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
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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

        case  10: if(MM.GetManNo() != mcHED_CycleMarkTraceFw) { //�޴��� �����ų� ���䷱���� ã���� �ٴҶ�.
                      if(m_iMarkTraceCnt >= MarkData.GetDataCnt() -1) {
                          EM_SetErr(eiHED_PenFailOper);
                          Step.iCycle = 0 ;
                          return true ;
                      }

                      m_iMarkTraceCnt ++ ;
                  }
                  else {
                      if(m_iMarkTraceCnt <= 0) { // �޴��� ������
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


                  if(MM.GetManNo() == mcNoneCycle) {//�޴���� ���۰� �����϶��� �˻� ���� �ȶ��.
                      if(VsRslt.iRet == veWfpOk ) {//���� ��� ���缭 ���� ����� �۾��ڰ� �Ǵ��ϰ� �Ѵ�.
                          EM_SetErr(eiHED_PenFailOper);
                  //        Step.iCycle = 20 ;
                  //        return false ;

                      }
                  }

                  Step.iCycle=0;
                  return true ;

        //���䷱�ÿ��� ....
        case  20: //���� ���� ã������ ��ĭ ������ ���� ���� ���¿��� ����.;..
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
        EM_SetErr((EN_ERR_ID)m_iCycleTO); //�̷��� ������ �ٲܼ� �ְ� �ؼ� ����Ŭ �̸����� �־��ָ� ���ٳ�...
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
    return ; //����Ʈ�� �ʿ� ����.

}

bool CHead::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
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
