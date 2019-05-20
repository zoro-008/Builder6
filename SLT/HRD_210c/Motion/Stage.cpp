//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <math.hpp>
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
#include "ManualMan.h"
#include "SAInterface.h"
//---------------------------------------------------------------------------

#include "Head.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone�����.

#define CST_OFFSET 5
//---------------------------------------------------------------------------
CStage STG;

CStage::CStage()
{

}
CStage::~CStage (void)
{
    //���������� �̵�.
    //Close();
}
void CStage::Init()
{
    InitCycleName();
    
    m_sPartName   = "Stage " ;
    m_dLastIdxPos = 0.0 ;

    m_iHomeTO  = (int)eiSTG_HomeTO    ;
    m_iStartTO = (int)eiSTG_ToStartTO ;
    m_iStopTO  = (int)eiSTG_ToStopTO  ;
    m_iCycleTO = (int)eiSTG_CycleTO   ;

    m_bWfrTAligned = false ;
    m_bStartSet    = false ;

    m_bDispStartForm = false ;

    Reset();
    Load(true);

    AT_MoveCyl(aiEJT_UpDn,ccBwd);
}


int    CStage::GetLastPickRow()
{
    return m_iLastPickRow ;
}
int    CStage::GetLastPickCol()
{
    return m_iLastPickCol ;

}
double CStage::GetLastPickPosX()
{
    return m_dLastWorkPosX ;

}
double CStage::GetLastPickPosY()
{
    return m_dLastWorkPosY ;
}


void CStage::SetLastPickRow(int _iRow)
{
    m_iLastPickRow = _iRow;
}
void CStage::SetLastPickCol(int _iCol)
{
    m_iLastPickCol = _iCol;

}
void CStage::SetLastPickPosX(double _dX)
{
    m_dLastWorkPosX = _dX;

}
void CStage::SetLastPickPosY(double _dY)
{
    m_dLastWorkPosY = _dY;
}



void CStage::Close()
{
    Load(false);
}
void CStage::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

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

void CStage::SetDirction(bool _bLeft)
{
    if(OM.CmnOptn.bUseMapFile) return ;

    if(_bLeft) {
        if(m_iLastPickRow%2 == 0) {
            m_bRvsDirction = true ;
        }
        else {
            m_bRvsDirction = false ;
        }
    }

    else {
        if(m_iLastPickRow%2 == 1) {
            m_bRvsDirction = true ;
        }
        else {
            m_bRvsDirction = false ;
        }
    }



}


//---------------------------------------------------------------------------
bool CStage::GetAdjustPos(double  _EncX , double  _EncY , double  _EncT , //���ڴ� ����....//�����.
                          double  _OfsX , double  _OfsY , double  _OfsT , //���� ��� �����V����.
                          double &_RltX , double &_RltY , double &_RltT , //�����. <===�̰� ������ �����ָ� �ȴ�.
                          bool    _bUseTheta)                             //��Ÿ���� ��� ����.
{
    //Local Var.
    double PosX = 0.0 ;
    double PosY = 0.0 ;
    double PosT = 0.0 ;

    double dTempX , dTempY ;

    if(_bUseTheta){ //��Ÿ���� ����.
        PosX = _OfsX + _EncX - OM.MstOptn.dStgXEjtCt ;  // ���߿� Ȯ�� �ϰ� Ȩ �ɼ� X,Y �� �����ش�.     //sun
        PosY = _OfsY + _EncY - OM.MstOptn.dStgYEjtCt ;
        PosT = DegToRad(_OfsT) ;
        dTempX = PosX * cos(PosT) ;
        dTempX = PosY * sin(PosT) ;
        dTempX = PosX * cos(PosT) - PosY * sin(PosT) ;
        dTempY = PosX * sin(PosT) ;
        dTempY = PosY * cos(PosT) ;
        dTempY = PosX * sin(PosT) + PosY * cos(PosT) ;


        //_RltX   = OM.MstOptn.dStgXEjtCt + PosX * cos(PosT) - PosY * sin(PosT)  ;
        //_RltY   = OM.MstOptn.dStgYEjtCt + PosX * sin(PosT) + PosY * cos(PosT)  ;

        //1��и�.
        _RltX   = OM.MstOptn.dStgXEjtCt + PosX * cos(PosT) + PosY * sin(PosT)  ;
        _RltY   = OM.MstOptn.dStgYEjtCt - PosX * sin(PosT) + PosY * cos(PosT)  ;

        _RltT   = _EncT + _OfsT ;
    }

    else{ //��Ÿ���� �ȵ�� ����.
        _RltX = _EncX + _OfsX;  // ���߿� Ȯ�� �ϰ� Ȩ �ɼ� X,Y �� �����ش�.     //sun
        _RltY = _EncY + _OfsY;
        _RltT = _EncT     ;
    }

    return true ;

}
bool CStage::VisnRsltLmtCheck(TRetResult * _tRsltbool)
{
    if( _tRsltbool->fRsltX > (OM.DevInfo.dWf_PitchX)/2 ||
        _tRsltbool->fRsltY > (OM.DevInfo.dWf_PitchY)/2 ||
        fabs(_tRsltbool->fRsltT) > OM.CmnOptn.dStageTLmtErr ) {
        return false ;
    }
    return true ;
}
//---------------------------------------------------------------------------
bool CStage::IsWaferZone(double _dPosX , double _dPosY)
{

    double dChipSize = hypot(OM.DevInfo.dWf_PitchX/2 , OM.DevInfo.dWf_PitchY/2);
    double dAreaPos  = OM.DevInfo.dWf_Diameter/2.0 ;//- dChipSize ;
    double dPos      = pow(dAreaPos,2);
    int    dX        ;
    int    dY        ;

    dX = pow(_dPosX - OM.MstOptn.dStgXEjtCt,2); //pow(_dPosX - OM.MstOptn.dStgXEjtCt ,2);
    dY = pow(_dPosY - OM.MstOptn.dStgYEjtCt,2); //pow(_dPosY - OM.MstOptn.dStgYEjtCt ,2);
    if(dPos < (dX + dY)) return false ;

    return true ;
}

bool CStage::IsSafeZone(double _dPosX , double _dPosY)
{

    double dChipSize = hypot(OM.DevInfo.dWf_PitchX/2 , OM.DevInfo.dWf_PitchY/2);
    double dAreaPos  = OM.MstOptn.dStgDia/2.0 ;//- dChipSize ;
    double dPos      = pow(dAreaPos,2);
    int    dX        ;
    int    dY        ;

    double dDistFromCtr ;

    dX = _dPosX - OM.MstOptn.dStgXEjtCt,2;
    dY = _dPosY - OM.MstOptn.dStgYEjtCt,2;

    dX = pow(dX,2); //pow(_dPosX - OM.MstOptn.dStgXEjtCt ,2);
    dY = pow(dY,2); //pow(_dPosY - OM.MstOptn.dStgYEjtCt ,2);

    dDistFromCtr = pow(dX + dY , 0.5);
    if(dAreaPos < dDistFromCtr) return false ;

    return true ;
}





//---------------------------------------------------------------------------
bool CStage::GetNextPos (double  _PckX , double  _PckY , double  _PckT , //Last Pick Position.
                         double &_RltX , double &_RltY , double &_RltT ) //Destination Value.
{

    const double dPitchX = OM.DevInfo.dWf_PitchX ;
    const double dPitchY = OM.DevInfo.dWf_PitchY ;


    if(OM.CmnOptn.bUseMapFile) {//������ ���
        int iRow , iCol ;
        int iRowMvCnt , iColMvCnt ;

        iRow = DM.ARAY[riSTG].FindFrstRow(csUnkwn);

        if(iRow%2) DM.ARAY[riSTG].FindFrstRowLastCol(csUnkwn, iRow , iCol) ;
        else       DM.ARAY[riSTG].FindFrstRowCol    (csUnkwn, iRow , iCol) ;

        if(iRow < 0 || iCol < 0) {return false ;}

        iRowMvCnt = iRow - m_iLastPickRow ; //m_iLastPickRow pick���� �־��ָ� �Ǵ� ����.
        iColMvCnt = iCol - m_iLastPickCol ;

        _RltX = _PckX + iColMvCnt * dPitchX ;
        _RltY = _PckY + iRowMvCnt * dPitchY ;
        _RltT = _PckT ;

        m_iLastPickRow = iRow ;
        m_iLastPickCol = iCol ;

        return true ;
    }

    else { //��� ���
        int iRowMvCnt = 0 ; //���� ��ġ���� ���� �������� �Ǵ� ��ġ.
        int iColMvCnt = 0 ;
        int iChkColCnt = OM.DevInfo.dWf_Diameter / (double)dPitchX ;
        int iChkRowCnt = OM.DevInfo.dWf_Diameter / (double)dPitchY ;

        if(!m_bRvsDirction) { //����������.
            if(!(m_iLastPickRow%2)){ //��ù�� ���� ¦�� ��. 0 ,2 ,4 ,6   ���� �̵�.
                if(IsWaferZone(_PckX + dPitchX , _PckY)){ //������ �������� �̵��� Ȯ��.
                     iRowMvCnt = 0 ;
                     iColMvCnt = 1 ;
                }
                else { //�� �ٲ�.
                     for(int c = iChkColCnt ; c >= -iChkColCnt ; c--) {   //���������� �̺��� ���� �˻��ϸ鼭 ��������
                         if(IsWaferZone(_PckX + c * dPitchX , _PckY + dPitchY)) {
                             iRowMvCnt = 1 ;
                             iColMvCnt = c  ;
                             break ;
                         }
                     }
                }
            }

            else { //Ȧ�� ��. 1 ,3 ,5 ,7   ���� �̵�.
                if(IsWaferZone(_PckX - dPitchX , _PckY)){
                     iRowMvCnt =  0 ;
                     iColMvCnt = -1 ;
                }
                else { //��ٲ�.
                     for(int c = -iChkColCnt ; c <= iChkColCnt ; c++) {
                         if(IsWaferZone(_PckX + c * dPitchX , _PckY + dPitchY)) {
                             iRowMvCnt = 1 ;
                             iColMvCnt = c ;
                             break;
                         }
                     }
                }
            }
        }
        else { //�ݴ��������.
            if((m_iLastPickRow%2)){ //��ù�� ���� ¦�� ��. 0 ,2 ,4 ,6   ���� �̵�.
                if(IsWaferZone(_PckX + dPitchX , _PckY)){ //������ �������� �̵��� Ȯ��.
                     iRowMvCnt = 0 ;
                     iColMvCnt = 1 ;
                }
                else { //�� �ٲ�.
                     for(int c = iChkColCnt ; c >= -iChkColCnt ; c--) {   //���������� �̺��� ���� �˻��ϸ鼭 ��������
                         if(IsWaferZone(_PckX + c * dPitchX , _PckY + dPitchY)) {
                             iRowMvCnt = 1 ;
                             iColMvCnt = c  ;
                             break ;
                         }
                     }
                }
            }

            else { //Ȧ�� ��. 1 ,3 ,5 ,7   ���� �̵�.
                if(IsWaferZone(_PckX - dPitchX , _PckY)){
                     iRowMvCnt =  0 ;
                     iColMvCnt = -1 ;
                }
                else { //��ٲ�.
                     for(int c = -iChkColCnt ; c <= iChkColCnt ; c++) {
                         if(IsWaferZone(_PckX + c * dPitchX , _PckY + dPitchY)) {
                             iRowMvCnt = 1 ;
                             iColMvCnt = c ;
                             break;
                         }
                     }
                }
            }


        }

        m_iLastPickRow +=iRowMvCnt ;
        m_iLastPickCol +=iColMvCnt ;

        _RltX = _PckX + (iColMvCnt * dPitchX) ;
        _RltY = _PckY + (iRowMvCnt * dPitchY) ;
        _RltT = _PckT ;

        return (iRowMvCnt != 0 || iColMvCnt != 0) ;
    }
}//äũ �Ϸ�

/*
bool CStage::MC_SetAjustPos(TRetResult * _tRsltbool , bool _bUseTheta , double &_dX , double &_dY ,double &_dT )
{
	double PosX = 0.0 ; double WorkSttX = 0.0 ; double dX = 0.0 ;
	double PosY = 0.0 ; double WorkSttY = 0.0 ; double dY = 0.0 ;
	double PosT = 0.0 ; double WorkSttT = 0.0 ; double dT = 0.0 ;

    WorkSttX = MT_GetCmdPos(miWRD_XStg);
    WorkSttY = MT_GetCmdPos(miWRD_YStg);
    WorkSttT = MT_GetCmdPos(miWRD_TStg);

      if(_bUseTheta)
      {
          PosT = DegToRad(_tRsltbool->fRsltT) ;

          PosX = WorkSttX - _tRsltbool->fRsltX ;
          PosY = WorkSttY + _tRsltbool->fRsltY ;
          PosT = WorkSttT + _tRsltbool->fRsltT ;

          dX   = PosX * cos(PosT) - PosY * sin(PosT);
          dY   = PosX * sin(PosT) + PosY * cos(PosT);
          dT   = WorkSttT ;

          PM.SetPos(miWRD_XStg, pvWRD_XStgVisnStt, dX) ;
          PM.SetPos(miWRD_YStg, pvWRD_YStgVisnStt, dY) ;
          PM.SetPos(miWRD_TStg, pvWRD_TStgVisnStt, dT) ;

      }

      else
      {
          PosX = WorkSttX - _tRsltbool->fRsltX ;
          PosY = WorkSttY + _tRsltbool->fRsltY ;
          PosT = WorkSttT ;//+ _tRsltbool->fRsltT ;

          dX = PosX;
          dY = PosY;
          dT = PosT;

          PM.SetPos(miWRD_XStg, pvWRD_XStgVisnStt, dX) ;
          PM.SetPos(miWRD_YStg, pvWRD_YStgVisnStt, dY) ;
          PM.SetPos(miWRD_TStg, pvWRD_TStgVisnStt, dT) ;


      }

      _dX = dX ;
      _dY = dY ;
      _dT = dT ;
      return true ;

} */

bool CStage::FindChip(EN_ARAY_ID _riWhre , int &r , int &c , EN_CHIP_STAT _csStats)
{
//�࿡�� ��������� ��ŸƮ ���������� �ϱ�� ����.
    int r1,c1;
    if(_riWhre == riSTG ) {
        c1 = DM.ARAY[_riWhre].FindFrstRow(_csStats) ;
        if(c1%2) DM.ARAY[_riWhre].FindFrstRowLastCol(_csStats , r1 , c1) ;
        else     DM.ARAY[_riWhre].FindFrstRowCol    (_csStats , r1 , c1) ;
        r = r1 ;
        c = c1;
        return (r > -1 && c > -1);
    }
    else if(_riWhre == riCST ) {
//        r = DM.ARAY[_riWhre].FindFrstRow(_csStats) ;
        r = DM.ARAY[_riWhre].FindLastRow(_csStats) ;
        return (r > -1 && r > -1);
    }

    return false ;
}

double CStage::GetWorkPos(EN_MOTR_ID _iMotr)
{
    double dWorkPos = MT_GetCmdPos(_iMotr);
    int    r  , c  ;

    double dWRowPitch = OM.DevInfo.dWf_PitchX ;
    double dWColPitch = OM.DevInfo.dWf_PitchY ;

    double WorkSttX = 0.0 ;
    double WorkSttY = 0.0 ;
    double WorkSttT = 0.0 ;

    //GetNextPos�Լ����� ó���Ѵ�. ���� Ŀ�ǵ� �� �ʿ� �ؼ� �ܺηλ�.
    //if(DM.ARAY[riSTG].GetStep() == 0 ) {
    //    WorkSttX = PM.GetValue(miWRD_XStg,pvWRD_XStgFindOri) + OM.DevOptn.dStgXOfs ;
    //    WorkSttY = PM.GetValue(miWRD_YStg,pvWRD_YStgFindOri) + OM.DevOptn.dStgYOfs ;
    //    WorkSttT = PM.GetValue(miWRD_TStg,pvWRD_TStgWorkStt) ;
    //}
    //else {
    //    WorkSttX = PM.GetValue(miWRD_XStg,pvWRD_XStgVisnStt) ;
    //    WorkSttY = PM.GetValue(miWRD_YStg,pvWRD_YStgVisnStt) ;
    //    WorkSttT = PM.GetValue(miWRD_TStg,pvWRD_TStgVisnStt) ;
    //}

    //if(_iMotr == miWRD_XStg){
    //    if(FindChip(riSTG,r,c)) {
    //        c = c - OM.DevInfo.iWFrstCol -1 ; //��� ���ݴµ�..
    //        dWorkPos = WorkSttX + ( dWColPitch * c ) ;
    //        return dWorkPos ;
    //    }
    //}
    //else if(_iMotr == miWRD_YStg){
    //    if(FindChip(riSTG,r,c)) {
    //        r = r - OM.DevInfo.iWFrstRow -1 ;//��� ���ݴµ�.
    //        dWorkPos = WorkSttY + ( dWRowPitch * r ) ;
    //        return dWorkPos ;
    //    }
    //}
    //else if(_iMotr == miWRD_TStg){
    //    if(FindChip(riSTG,r,c)) {
    //        dWorkPos = WorkSttT ;
    //        return dWorkPos ;
    //    }
    //}

    if(_iMotr == miWRD_ZCst){
        if(DM.ARAY[riCST].GetCntStat(csMask)) {
            FindChip(riCST,r,c,csMask);
//            r = OM.DevInfo.iCsSlotCnt - r - 1 ;
//            dWorkPos = PM.GetValue (_iMotr , pvWRD_ZCstWorkStt ) + ( OM.DevInfo.dCsSlotPitch * r ) ;
            r = OM.DevInfo.iCsSlotCnt - r - 1 ;
            dWorkPos = PM.GetValue (_iMotr , pvWRD_ZCstWorkStt ) - ( OM.DevInfo.dCsSlotPitch * r ) ;
        }
        else {
            FindChip(riCST,r,c,csUnkwn);
//            r = OM.DevInfo.iCsSlotCnt - r - 1 ;
//            dWorkPos = PM.GetValue (_iMotr , pvWRD_ZCstWorkStt ) + ( OM.DevInfo.dCsSlotPitch * r ) ;
            r = OM.DevInfo.iCsSlotCnt - r - 1 ;
            dWorkPos = PM.GetValue (_iMotr , pvWRD_ZCstWorkStt ) - ( OM.DevInfo.dCsSlotPitch * r ) ;

        }
    }

    return dWorkPos ;
}

double CStage::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{

    double dPos  = MT_GetCmdPos(_iMotr);
    int r = -1 , c = -1 ;

    double dStgStartPosX ;
    double dStgStartPosY ;
    double dWfrPitchX = OM.DevInfo.dWf_PitchX ;
    double dWfrPitchY = OM.DevInfo.dWf_PitchY ;

    //double dNextXPos ;
    //double dNextYPos ;
    //double dNextTPos ;
    //GetNextPos(m_dLastWorkPosX , m_dLastWorkPosY , m_dLastWorkPosT , dNextXPos , dNextYPos , dNextTPos );


    //if(m_dCntChipPosX >= 0.0 && m_dCntChipPosY >= 0.0) {
    //    dStgStartPosY = GetTopEnd (m_dCntChipPosX , m_dCntChipPosY) ;
    //    dStgStartPosX = GetLeftEnd(m_dCntChipPosX , dStgStartPosY ) ;
    //}
    //else { //���� ó��.
    //    dStgStartPosX = MT_GetCmdPos(miWRD_XStg);
    //    dStgStartPosY = MT_GetCmdPos(miWRD_YStg);
    //}

    if(_iMotr == miWRD_XStg){
        switch(_iPstnId) {
            default                 : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWRD_XStgWait     : dPos = PM.GetValue (_iMotr , pvWRD_XStgWait    ); break ;
            case piWRD_XStgCassette : dPos = PM.GetValue (_iMotr , pvWRD_XStgCassette); break ;
            case piWRD_XStgAlign    : dPos = PM.GetValue (_iMotr , pvWRD_XStgAlign   ); break ;
            case piWRD_XStgFindOri  : dPos = PM.GetValue (_iMotr , pvWRD_XStgFindOri ); break ;
            //case piWRD_XStgFirstChip : dPos =
            //case piWRD_XStgWork     : dPos = dNextXPos     ;                            break ; �ٷΰ����� ������.
        }
    }

    else if(_iMotr == miWRD_YStg){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWRD_YStgWait      : dPos = PM.GetValue (_iMotr , pvWRD_YStgWait    ); break ;
            case piWRD_YStgCassette  : dPos = PM.GetValue (_iMotr , pvWRD_YStgCassette); break ;
            case piWRD_YStgAlign     : dPos = PM.GetValue (_iMotr , pvWRD_YStgAlign   ); break ;
            case piWRD_YStgFindOri   : dPos = PM.GetValue (_iMotr , pvWRD_YStgFindOri ); break ;
            //case piWRD_YStgFirstChip : dPos = 
            //case piWRD_YStgWork    : dPos = dNextYPos     ;                            break ;
        }
    }


    else if(_iMotr == miWRD_TStg){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWRD_TStgWait    : dPos = PM.GetValue (_iMotr , pvWRD_TStgWait    ); break ;
            case piWRD_TStgCassette: dPos = PM.GetValue (_iMotr , pvWRD_TStgCassette); break ;
            case piWRD_TStgAlign   : dPos = PM.GetValue (_iMotr , pvWRD_TStgAlign   ); break ;
            case piWRD_TStgWork    : dPos = m_dCntChipPosT ;                           break ;
        }
    }
    else if(_iMotr == miWRD_XTrs){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWRD_XTrsWait    : dPos = PM.GetValue (_iMotr , pvWRD_XTrsWait    ); break ;
            case piWRD_XTrsPick    : dPos = PM.GetValue (_iMotr , pvWRD_XTrsPick    ); break ;
            case piWRD_XTrsPickWait: dPos = PM.GetValue (_iMotr , pvWRD_XTrsPickWait); break ;
            case piWRD_XTrsPlace   : dPos = PM.GetValue (_iMotr , pvWRD_XTrsPlace   ); break ;
        }
    }
    else if(_iMotr == miWRD_ZCst){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWRD_ZCstWait    : dPos = PM.GetValue (_iMotr , pvWRD_ZCstWait    ); break ;
            case piWRD_ZCstWorkStt : dPos = PM.GetValue (_iMotr , pvWRD_ZCstWorkStt ); break ;
            case piWRD_ZCstWork    : dPos = GetWorkPos  (_iMotr                     ); break ;
//TODO ::            case piWRD_ZCstOut     : dPos = GetWorkPos  (_iMotr                     ) - CST_OFFSET; break ;
            case piWRD_ZCstOut     : dPos = GetWorkPos  (_iMotr                     ) - PM.GetValue(_iMotr , poWRD_ZCstPlaceOfs); break ;
        }
    }
    else if(_iMotr == miWRD_ZEjt){
        switch(_iPstnId) {
            default                    : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWRD_ZEjtWait        : dPos = PM.GetValue (_iMotr , pvWRD_ZEjtWait      ); break ;
            case piWRD_ZEjtWorkBefore  : dPos = PM.GetValue (_iMotr , pvWRD_ZEjtWorkBefore); break ;
            case piWRD_ZEjtWorkStt     : dPos = PM.GetValue (_iMotr , pvWRD_ZEjtWork      ); break ;
        }
    }
    //�� �־��ֱ�� �ϴµ� Stop Inposition ���� ����ϸ� �j�ȴ�.
    else if(_iMotr == miWRD_ZExd){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                     ); break ;
            case piWRD_ZExdWait    : dPos = PM.GetValue (_iMotr , pvWRD_ZExdWait    ); break ;
            case piWRD_ZExdGet     : dPos = PM.GetValue (_iMotr , pvWRD_ZExdGet     ); break ;
            case piWRD_ZExdExpend  : dPos = PM.GetValue (_iMotr , pvWRD_ZExdExpend  ); break ;
        }
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CStage::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isHedZWaitPs = MT_GetStopInpos(miWRD_ZHed) && (MT_GetCmdPos(miWRD_ZHed) <= PM.GetValue(miWRD_ZHed,pvWRD_ZHedMove)) ;
    bool isExpendPs   = MT_GetStopInpos(miWRD_ZExd) && MT_CmprPos(miWRD_ZExd,PM.GetValue(miWRD_ZExd,pvWRD_ZExdExpend)) ;
    bool isTranWaitPs = (MT_GetStopInpos(miWRD_XTrs) && MT_CmprPos(miWRD_XTrs,PM.GetValue(miWRD_XTrs,pvWRD_XTrsWait))) ||
                        (MT_GetStopInpos(miWRD_XTrs) && MT_CmprPos(miWRD_XTrs,PM.GetValue(miWRD_XTrs,pvWRD_XTrsPickWait))) ;
    bool isExpendPlus = MT_GetCmdPos(miWRD_ZExd) <= GetMotrPos(miWRD_ZExd,_iPstnId) ;

    bool isStgSWaitPs = MT_GetStopInpos(miWRD_TStg) && MT_CmprPos(miWRD_TStg,PM.GetValue(miWRD_TStg,pvWRD_TStgWait)) ;

    bool isEjtCylUp   = !AT_Complete(aiEJT_UpDn , ccBwd);
    bool isEjtPinUp   = MT_GetCmdPos(miWRD_ZEjt) > PM.GetValue(miWRD_ZEjt, pvWRD_ZEjtWorkBefore) + 0.1 ; //��ũ ���� �۾� ���� ������...���� 0.1

//    bool isStgSWaitPs = MT_GetStopInpos(miWRD_TStg) && MT_CmprPos(miWRD_TStg,0.0) ;
    //TODO :: Tracsfer X �� Wait ������ �϶���?

    /*  PTR
    if (Motr == miWS_X)
    {
        double dPowActDia = pow((FM.MstOptn.dExpLimDia - FM.MstOptn.dEjectorDia)/2 , 2 );
        double dPowPosX   = pow(GetMotrPos(Motr,mcPosition,PstnId),2);
        double dPowTgtY   = pow(MOTR.GetTrgPos(miWS_Y),2);  //
        double dPowEncY   = pow(MOTR.GetEncPos(miWS_Y),2);  //

        if( dPowActDia < dPowPosX + dPowTgtY || dPowActDia < dPowPosX + dPowEncY )
        {
            if (bDspMsg) UserMsg2("Error" , "Stage Limit ������ ����� �Ǿ� \n �����ϼ� �����ϴ�." ); return false;
        }

    }

    if (Motr == miWS_Y)
    {
        double dPowActDia = pow((FM.MstOptn.dExpLimDia - FM.MstOptn.dEjectorDia)/2 , 2 );
        double dPowPosY   = pow(GetMotrPos(Motr,mcPosition,PstnId),2);
        double dPowTgtX   = pow(MOTR.GetTrgPos(miWS_X),2);
        double dPowEncX   = pow(MOTR.GetEncPos(miWS_X),2);

        if((dPowActDia < dPowPosY + dPowTgtX) || (dPowActDia < dPowPosY + dPowEncX))
        {
            if (bDspMsg) UserMsg2("Error" , "Stage Limit ������ ����� �Ǿ� \n �����ϼ� �����ϴ�." ); return false;
        }
    }
    */
    if(_iMotr == miWRD_XStg){
        if(isEjtPinUp&&isEjtCylUp) {sMsg = "�������� �ö���־� �����ϼ� �����ϴ�.";  bRet = false ;}

        //ejector
        if(isEjtCylUp) {
            double dTrgPosX = GetMotrPos(_iMotr,_iPstnId) ;
            double dTrgPosY = MT_GetTrgPos(miWRD_YStg) ;
            double dCmdPosY = MT_GetCmdPos(miWRD_YStg) ;
            bool bR1 = !IsSafeZone(dTrgPosX ,dTrgPosY );
            bool bR2 = !IsSafeZone(dTrgPosX ,dCmdPosY );


            /*
            double dPowActDia = pow((OM.MstOptn.dStgDia )/2.0 , 2 );
            double dPowPosX   = pow(GetMotrPos(_iMotr,_iPstnId) - OM.MstOptn.dStgXEjtCt,2);
            double dPowTgtY   = pow(MT_GetTrgPos(miWRD_YStg) - OM.MstOptn.dStgYEjtCt ,2);  //
            double dPowEncY   = pow(MT_GetEncPos(miWRD_YStg) - OM.MstOptn.dStgYEjtCt ,2);  //
            */

            if( bR1 || bR2 ) {
                sMsg = "Stage Limit ������ ����� �Ǿ� �����ϼ� �����ϴ�.";  bRet = false ;;
            }
        }

        //head
        if(!isHedZWaitPs) { sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Hed_Z Axis is Not Wait Position)" ; bRet = false ;}
        if(!isTranWaitPs) { sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Stg_X_Transfer Axis is Not Wait Position)" ; bRet = false ;}
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_XStgWait    : if(!isExpendPs){ sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Expend_Z Axis is Not Expend Position)" ; bRet = false ;}  break ;
            case piWRD_XStgCassette: break ;
            case piWRD_XStgFindOri : if(!isExpendPs){ sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Expend_Z Axis is Not Expend Position)" ; bRet = false ;}  break ;
            case piWRD_XStgAlign   : if(!isExpendPs){ sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Expend_Z Axis is Not Expend Position)" ; bRet = false ;}  break ;
            case piWRD_XStgWork    : if(!isExpendPs){ sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Expend_Z Axis is Not Expend Position)" ; bRet = false ;}  break ;
        }
    }
    else if(_iMotr == miWRD_YStg){
        if(isEjtPinUp&&isEjtCylUp) {sMsg = "�������� �ö���־� �����ϼ� �����ϴ�.";  bRet = false ;}
        //ejector
        if(isEjtCylUp) {
            double dTrgPosY = GetMotrPos(_iMotr,_iPstnId) ;
            double dTrgPosX = MT_GetTrgPos(miWRD_XStg) ;
            double dCmdPosX = MT_GetCmdPos(miWRD_XStg) ;
            bool   bR1 = !IsSafeZone(dTrgPosX ,dTrgPosY );
            bool   bR2 = !IsSafeZone(dCmdPosX ,dTrgPosY );
            /*
            double dPowActDia = pow((OM.MstOptn.dStgDia - OM.MstOptn.dEjtDia)/2.0 , 2 );
            double dPowPosY   = pow(GetMotrPos(_iMotr,_iPstnId) - OM.MstOptn.dStgYEjtCt,2);
            double dPowTgtX   = pow(MT_GetTrgPos(miWRD_XStg) - OM.MstOptn.dStgXEjtCt ,2);  //
            double dPowEncX   = pow(MT_GetEncPos(miWRD_XStg) - OM.MstOptn.dStgXEjtCt ,2);  //
            */
            if( bR1 || bR2 ) {
                sMsg = "Stage Limit ������ ����� �Ǿ� �����ϼ� �����ϴ�." ;  bRet = false ;;
            }
        }

        if(!isHedZWaitPs) { sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Hed_Z Axis is Not Wait Position)" ; bRet = false ;}
        if(!isTranWaitPs) { sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Stg_X_Transfer Axis is Not Wait Position)" ; bRet = false ;}
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_YStgWait    : if(!isExpendPs){ sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Expend_Z Axis is Not Expend Position)" ; bRet = false ;}  break ;
            case piWRD_YStgCassette: break ;
            case piWRD_YStgFindOri : if(!isExpendPs){ sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Expend_Z Axis is Not Expend Position)" ; bRet = false ;}  break ;
            case piWRD_YStgAlign   : if(!isExpendPs){ sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Expend_Z Axis is Not Expend Position)" ; bRet = false ;}  break ;
            case piWRD_YStgWork    : if(!isExpendPs){ sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Expend_Z Axis is Not Expend Position)" ; bRet = false ;}  break ;
        }
    }
    else if(_iMotr == miWRD_TStg){
        if(!isHedZWaitPs) { sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Hed_Z Axis is Not Wait Position)" ; bRet = false ;}
        if(!isTranWaitPs) { sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Stg_X_Transfer Axis is Not Wait Position)" ; bRet = false ;}
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_TStgWait    : break ;
            case piWRD_TStgCassette: break ;
            case piWRD_TStgAlign   : break ;
            case piWRD_TStgWork    : break ;
//            case piWRD_TStgReverse : break ;
//            case piWRD_TStgWork    ; break ;
        }
    }
    else if(_iMotr == miWRD_XTrs){
        if(!(MT_GetStopInpos(miWRD_XStg) && MT_CmprPos(miWRD_XStg,PM.GetValue(miWRD_XStg,pvWRD_XStgWait))) ||
           !(MT_GetStopInpos(miWRD_YStg) && MT_CmprPos(miWRD_YStg,PM.GetValue(miWRD_YStg,pvWRD_YStgWait))) ||
           !(MT_GetStopInpos(miWRD_TStg) && MT_CmprPos(miWRD_TStg,PM.GetValue(miWRD_TStg,pvWRD_TStgWait))) ) {
               sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Stg_X,Y,S_Transfer Axis is Not Wait Position)" ; bRet = false ;
           }

        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_XTrsWait    : break ;
            case piWRD_XTrsPick    : break ;
            case piWRD_XTrsPickWait: break ;
            case piWRD_XTrsPlace   : break ;
        }
    }
    else if(_iMotr == miWRD_ZCst){
        if((MT_GetCmdPos(miWRD_XTrs) > PM.GetValue(miWRD_XTrs,pvWRD_XTrsPickWait) )) { sMsg = AnsiString(MT_GetName(miWRD_XTrs))+"(Transfer_X is Pick Position)" ; bRet = false ;}
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_ZCstWait    : break ;
            case piWRD_ZCstWorkStt : break ;
            case piWRD_ZCstWork    : break ;
            case piWRD_ZCstOut     : break ;
        }
    }
    else if(_iMotr == miWRD_ZEjt){
        switch(_iPstnId) {
            default                    : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_ZEjtWait        : break ;
            case piWRD_ZEjtWorkBefore  : break ;
            case piWRD_ZEjtWorkStt     : break ;
        }
    }
    else if(_iMotr == miWRD_ZExd){
        if(!isTranWaitPs && !isExpendPlus) { sMsg = AnsiString(MT_GetName(miWRD_XStg))+"(Stg_X_Transfer Axis is Not Wait Position)" ; bRet = false ;}
        if(!isStgSWaitPs && !isExpendPlus) { sMsg = AnsiString(MT_GetName(miWRD_TStg))+"(Stg_S Axis is Not Wait Position)" ; bRet = false ;}
        if(OM.MstOptn.dStgYMaxExdAble <= MT_GetCmdPos(miWRD_YStg) ) { sMsg = AnsiString(MT_GetName(miWRD_YStg))+"(Stg_Y Axis is Max Position , Extend Can't Move)" ; bRet = false ;}
        switch(_iPstnId) {
            default                : sMsg = "Is Not this Motr Position" ; bRet = false ; break ;
            case piWRD_ZExdWait    : break ;
            case piWRD_ZExdGet     : break ;
            case piWRD_ZExdExpend  : break ;
        }
    }
    else {
        sMsg = AnsiString(MT_GetName(_iMotr)) + " is Not this part Motr!" ;
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

    bool bWaferMoving = !MT_GetStop(miWRD_XStg) || !MT_GetStop(miWRD_YStg) || !MT_GetStop(miWRD_TStg) ;


    AnsiString sMsg ;
    bool       bRet = true ;
//
    if(aiEJT_UpDn == _iActr) {
        double dEncY   = MT_GetEncPos(miWRD_YStg);  //
        double dEncX   = MT_GetEncPos(miWRD_XStg);  //

        bool bWaferZone = IsSafeZone(dEncX , dEncY);

        if( !bWaferZone && _bFwd) {
             sMsg = "Stage Limit ������ ����� �Ǿ� �����ϼ� �����ϴ�." ;  bRet = false ;
        }

        if(bWaferMoving){
             sMsg = "Stage �� �̵����Դϴ�. " ;  bRet = false ;
        }
    }
    else if(aiTRS_Chuck) {


    }

    else if(aiTRS_UpDn) {



    }

    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr),sMsg.c_str());
    }

    return bRet ;
}

bool CStage::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlowSpeed) // ���͸� �����϶� ���� �Լ�.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) {
        if(_bSlowSpeed)return MT_GoAbsSlow(_iMotr , dPosition);
        else           return MT_GoAbsRun (_iMotr , dPosition);
    }
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CStage::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //�Ǹ����� �����϶� ���� �Լ�.
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
        case 10: IO_SetY(yEJT_Suction , false);
                 m_tmTemp.Clear();
                 Step.iHome++;
                 return false ;

        case 11: if(!m_tmTemp.OnDelay(true , 100)) return false ;

                 AT_MoveCyl(aiEJT_UpDn,ccBwd);
                 AT_MoveCyl(aiTRS_UpDn , ccBwd);
                 MT_SetHomeEnd(miWRD_XStg,false);
                 MT_SetHomeEnd(miWRD_YStg,false);
                 MT_SetHomeEnd(miWRD_TStg,false);
                 MT_SetHomeEnd(miWRD_ZCst,false);
                 MT_SetHomeEnd(miWRD_XTrs,false);
                 MT_SetHomeEnd(miWRD_ZExd,false);

                 Step.iHome++;
                 return false ;

        case 12: if(!AT_MoveCyl(aiEJT_UpDn,ccBwd)) return false ;
                 if(!AT_MoveCyl(aiTRS_UpDn , ccBwd)) return false ;

                 MT_DoHome(miWRD_XTrs);
                 Step.iHome++;
                 return false ;


        case 13: if(!MT_GetHomeEnd(miWRD_XTrs)) return false ;

                 MT_DoHome(miWRD_XStg);
                 MT_DoHome(miWRD_YStg);
                 MT_DoHome(miWRD_ZCst);
//
                 Step.iHome++;
                 return false ;

        case 14: if(!MT_GetHomeEnd(miWRD_XStg)) return false ;
                 if(!MT_GetHomeEnd(miWRD_YStg)) return false ;
                 if(!MT_GetHomeEnd(miWRD_ZCst)) return false ;
//
                 MT_GoAbsMan(miWRD_XStg , PM.GetValue(miWRD_XStg , pvWRD_XStgWait  ));
                 Step.iHome++;
                 return false ;

        case 15: if(!MT_GoAbsMan(miWRD_XStg , PM.GetValue(miWRD_XStg , pvWRD_XStgWait  ))) return false ;
                 MT_DoHome(miWRD_ZExd);

                 Step.iHome++;
                 return false ;

        case 16: if(!MT_GetHomeEnd(miWRD_ZExd)) return false ;
                 MT_GoAbsMan(miWRD_ZExd , PM.GetValue(miWRD_ZExd , pvWRD_ZExdWait  ));
                 Step.iHome++;
                 return false ;

        case 17: if(!MT_GoAbsMan(miWRD_ZExd , PM.GetValue(miWRD_ZExd , pvWRD_ZExdWait  )))return false ;
                 MT_DoHome(miWRD_TStg);
                 Step.iHome++;
                 return false ;

        case 18: if(!MT_GetHomeEnd(miWRD_TStg)) return false ;
                 MT_GoAbsMan(miWRD_ZExd , PM.GetValue(miWRD_ZExd , pvWRD_ZExdWait  ));
                 MT_GoAbsMan(miWRD_XTrs , PM.GetValue(miWRD_XTrs , pvWRD_XTrsWait  ));
                 MT_GoAbsMan(miWRD_YStg , PM.GetValue(miWRD_YStg , pvWRD_YStgWait  ));
                 MT_GoAbsMan(miWRD_TStg , PM.GetValue(miWRD_TStg , pvWRD_TStgWait  ));
                 MT_GoAbsMan(miWRD_ZCst , PM.GetValue(miWRD_ZCst , pvWRD_ZCstWait  ));
                 Step.iHome++;
                 return false ;

        case 19: if(!MT_GoAbsMan(miWRD_ZExd , PM.GetValue(miWRD_ZExd , pvWRD_ZExdWait  ))) return false ;
                 if(!MT_GoAbsMan(miWRD_XTrs , PM.GetValue(miWRD_XTrs , pvWRD_XTrsWait  ))) return false ;
                 if(!MT_GoAbsMan(miWRD_YStg , PM.GetValue(miWRD_YStg , pvWRD_YStgWait  ))) return false ;
                 if(!MT_GoAbsMan(miWRD_TStg , PM.GetValue(miWRD_TStg , pvWRD_TStgWait  ))) return false ;
                 if(!MT_GoAbsMan(miWRD_ZCst , PM.GetValue(miWRD_ZCst , pvWRD_ZCstWait  ))) return false ;

                 //DM.ARAY[riSTG].ChangeStat(csGood , csUnkwn); //�̷��� ��� ��� �и�.
                 //DM.ARAY[riSTG].ChangeStat(csWait , csUnkwn);

                 DM.ARAY[riSTG].ChangeStat(csGood , csWait);



                 MoveActr(aiTRS_UpDn , ccBwd);
                 Step.iHome = 0;
                 return true ;
    }
}

bool CStage::ToStopCon(void) //��ž�� �ϱ� ���� ������ ���� �Լ�.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq ) return false;

    Step.iToStop = 10;
    //Ok.
    return true;

}

bool CStage::ToStartCon(void) //��ŸƮ�� �ϱ� ���� ������ ���� �Լ�.
{

    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CStage::ToStart(void) //��ŸƮ�� �ϱ� ���� �Լ�.
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

        case 10: IO_SetY(ySTG_Dryer,false);
                 IO_SetY(yEJT_Suction,false);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(IO_GetX(xEJT_Vaccum)) return false ;

              //�Ĵ��� �����Ҷ��� ������ �ȳ�����.
                 if(!IsSafeZone(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg)) ||
                    !IsSafeZone(m_dLastWorkPosX          , m_dLastWorkPosY         ) ||
                    !m_bStartSet                                                     ) MoveActr(aiEJT_UpDn,ccBwd );


                 Step.iToStart ++ ;
                 return false ;

        case 12: if(!AT_Done(aiEJT_UpDn)) return false ;
                 if(m_bStartSet  ) {
                     MoveMotr(miWRD_ZExd,piWRD_ZExdExpend);
                 }
                 Step.iToStart ++ ;
                 return false ;

        case 13: if(m_bStartSet  ) {
                     if(!MoveMotr(miWRD_ZExd,piWRD_ZExdExpend)) return false ;
                 }

                 if(m_bStartSet  ) {
                     if(m_bStartSet)MT_GoAbsMan(miWRD_XStg , m_dLastWorkPosX); //�Ǹ����� �۾��� ��ġ�� �̵�.
                     if(m_bStartSet)MT_GoAbsMan(miWRD_YStg , m_dLastWorkPosY);
                     if(m_bStartSet)MT_GoAbsMan(miWRD_TStg , m_dLastWorkPosT);
                 }
                 Step.iToStart++;
                 return false ;

        case 14: if(m_bStartSet&&!MT_GetStopInpos(miWRD_XStg)) return false ;
                 if(m_bStartSet&&!MT_GetStopInpos(miWRD_YStg)) return false ;
                 if(m_bStartSet&&!MT_GetStopInpos(miWRD_TStg)) return false ;

                 if(IsWaferZone(MT_GetEncPos(miWRD_XStg) , MT_GetEncPos(miWRD_YStg))) MoveActr(aiEJT_UpDn,ccFwd );

                 //MoveMotr(miWRD_ZCst,piWRD_ZCstWait);
                 Step.iToStart++;
                 return false ;

        case 15: if(!AT_Done(aiEJT_UpDn)) return false ;
                 //if(!MoveMotr(miWRD_ZCst,piWRD_ZCstWait)) return false ;
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CStage::ToStop(void) //��ž�� �ϱ� ���� �Լ�.
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

        case 10: IO_SetY(ySTG_Dryer  ,false);
                 IO_SetY(yEJT_Suction,false);
                 //IO_SetY(yEJT_Suction,false);
//                 IO_SetY(yEJT_Suction,false);

                 MoveActr(aiTRS_Chuck , ccBwd);
                 Step.iToStop ++ ;
                 return false ;

        case 11: if(IO_GetX(xEJT_Vaccum)) return false ;
                 if(!MoveActr(aiTRS_Chuck , ccBwd)) return false ;


                 MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait);
                 MoveMotr(miWRD_XTrs,piWRD_XTrsWait);

                 //�ϵ� ���� �ɷ��� �ѹ� ����.
                 //MoveActr(aiEJT_UpDn,ccBwd );

                 //if(!EM_GetErr(eiSTG_TransWaferFeedingFail))MoveMotr(miWRD_ZCst,piWRD_ZCstWait);

                 Step.iToStop ++;
                 return false ;

        case 12: if(!MoveMotr(miWRD_ZEjt,piWRD_ZEjtWait)) return false ;
                 if(!MoveMotr(miWRD_XTrs,piWRD_XTrsWait)) return false ;
                 //if(!MoveActr(aiEJT_UpDn,ccBwd )) return false ;

                 //if(!EM_GetErr(eiSTG_TransWaferFeedingFail) &&!MoveMotr(miWRD_ZCst,piWRD_ZCstWait)) return false ;



                 //�˻� �ѹ� �س��� ���� �ٽ� �˻� �ϵ���.
                 DM.ARAY[riSTG].ChangeStat(csGood , csWait);

                 //�Ⱦ��Ѱ��� UnKown���� �Ͽ� �ٽ� ����� ġ�� �˻��ϵ���.  ��ʸ�� �ؽ�Ʈ���� Ÿ�� ��ĭ �и�.
                 //DM.ARAY[riSTG].ChangeStat(csWait , csUnkwn );



                 //�̸����ýÿ� ������. �� ����Ŭ �������� ����.
                 //m_dLastWorkPosX = MT_GetCmdPos(miWRD_XStg);
                 //m_dLastWorkPosY = MT_GetCmdPos(miWRD_YStg);
                 //m_dLastWorkPosT = MT_GetCmdPos(miWRD_TStg);

                 Load(false);



                 Step.iToStop = 0   ;
                 return true ;
    }
}

bool CStage::Autorun(void) //���䷱�׽ÿ� ��� Ÿ�� �Լ�.
{
    PreStep.iSeq = Step.iSeq ;

    //if( IO_GetXUp(xCST_Detect) &&/*  IO_GetX(xCST_Detect) &&*/ DM.ARAY[riCST].CheckAllStat(csNone)) DM.ARAY[riCST].SetStat(csUnkwn);
    //if( IO_GetXDn(xCST_Detect) &&/* !IO_GetX(xCST_Detect) &&*/ DM.ARAY[riCST].CheckAllStat(csWork)) DM.ARAY[riCST].SetStat(csNone );

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        //Wait   �� �������� ��ٸ�.
        //Good   ������ ��.
        //Unkown �����̸� ��.

        bool isCstExstUnkn = DM.ARAY[riCST].GetCntStat  (csUnkwn);
        bool isCstExstMask = DM.ARAY[riCST].GetCntStat  (csMask );
        bool isAllNone     = DM.ARAY[riSTG].CheckAllStat(csNone );
        bool isExstUnkn    = DM.ARAY[riSTG].GetCntStat  (csUnkwn); //
        bool isExstWait    = DM.ARAY[riSTG].GetCntStat  (csWait ); //   bReqVisn
        bool isExstGood    = DM.ARAY[riSTG].GetCntStat  (csGood );
        bool isExpended    = MT_CmprPos(miWRD_ZExd , PM.GetValue(miWRD_ZExd , pvWRD_ZExdExpend)) ;
        bool isVisnZone    = PM.GetValue(miWRD_YHed , pvWRD_YHedWaferWait) >= MT_GetCmdPos(miWRD_YHed) ;

        bool isBfZoneNone  = DM.ARAY[riLDR].CheckAllStat(csNone) &&
                             DM.ARAY[riPRB].CheckAllStat(csNone) &&
                             DM.ARAY[riREF].CheckAllStat(csNone) &&
                             DM.ARAY[riRDF].CheckAllStat(csNone) ;

        /*
        ������ ���
        csUnkown = ����� ���� Good  ����
        csFail   = �ʻ��� ����
        csErr    = ���� ����.
        csGood   = ����� ģ �� ���� ���� �Ǵ� ����.
        csWait   = Ʈ���� ��Ʈ���� �ص���Y ������ �߸��� ����.
        */
        bool bUseStage       = !OM.CmnOptn.bStageStop ;

        bool isCstSupply     =  isAllNone    && isCstExstUnkn && bUseStage ;//��� �ּ�&& !isBfZoneNone ;
        bool isStgExpend     =( OM.CmnOptn.bUseMapFile &&  isExstUnkn  && !isExpended) ||
                              (!OM.CmnOptn.bUseMapFile && !isAllNone   && !isExpended) ;

        bool isStgAlign      = !m_bWfrTAligned                 && DM.ARAY[riSTG].GetCntStat(csUnkwn) && isExpended ;
        bool isStgStartSet   =  m_bWfrTAligned && !m_bStartSet && DM.ARAY[riSTG].GetCntStat(csUnkwn) && isExpended && OM.DevOptn.iOriMathod == omMatch ;
        bool isStgStartSetEg =  m_bWfrTAligned && !m_bStartSet && DM.ARAY[riSTG].GetCntStat(csUnkwn) && isExpended && OM.DevOptn.iOriMathod != omMatch ;
        bool isStgNextMove   = m_bStartSet && !isExstGood && !isExstWait && isExstUnkn && isExpended && !isBfZoneNone &&bUseStage;   /*bReqVisn �ص� Ȥ�� Tostart���� ������.*/
        bool isStgVision     = m_bStartSet &&  isExstWait &&  isVisnZone ;

        bool isCstPlace      = !isAllNone && isCstExstMask && !isExstUnkn && !isExstWait && !isExstGood && bUseStage;
        bool isConEnd        = true /*isAllNone  && !isCstExstUnkn*/ ;


        if(!IO_GetX(xCST_Detect) &&  DM.ARAY[riCST].CheckAllStat(csNone )&& bUseStage) EM_SetErr(eiCST_Need      ) ;
        if( IO_GetX(xCST_Detect) && !DM.ARAY[riCST].CheckAllStat(csNone )&& !DM.ARAY[riCST].GetCntStat  (csUnkwn) && !DM.ARAY[riCST].GetCntStat(csMask) && bUseStage) EM_SetErr(eiCST_NeedChange) ;

        if( IO_GetX(xCST_Detect) &&  DM.ARAY[riCST].CheckAllStat(csNone) && bUseStage) EM_SetErr(eiCST_Unknwn    );
        if(!IO_GetX(xCST_Detect) && !DM.ARAY[riCST].CheckAllStat(csNone) && bUseStage && !OM.MstOptn.bDryRun) EM_SetErr(eiCST_Dispr     );

        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCstSupply    ) {Trace(m_sPartName.c_str(),"CycleCstSupply     Stt"); Step.iSeq = scCstSupply     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isStgExpend    ) {Trace(m_sPartName.c_str(),"CycleStgExpend     Stt"); Step.iSeq = scStgExpend     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isStgAlign     ) {Trace(m_sPartName.c_str(),"CycleTStgAlign     Stt"); Step.iSeq = scStgTAlign     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isStgStartSet  ) {Trace(m_sPartName.c_str(),"CycleStgStartSetMc Stt"); Step.iSeq = scStgStartSetMc ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isStgStartSetEg) {Trace(m_sPartName.c_str(),"CycleStgStartSetEg Stt"); Step.iSeq = scStgStartSetEg ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isStgNextMove  ) {Trace(m_sPartName.c_str(),"CycleStgNextMove   Stt"); Step.iSeq = scStgNextMove   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isStgVision    ) {Trace(m_sPartName.c_str(),"CycleStgVision     Stt"); Step.iSeq = scStgVision     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCstPlace     ) {Trace(m_sPartName.c_str(),"CycleCstPlace      Stt"); Step.iSeq = scCstPlace      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd       ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
  }

    //Cycle.
    switch (Step.iSeq) {
        default              :                           Trace(m_sPartName.c_str(),"default            End");Step.iSeq = scIdle ;  return false ;
        case scIdle          :                                                                                                     return false ;
        case scCstSupply     : if(CycleCstSupply    ()){ Trace(m_sPartName.c_str(),"CycleCstSupply     End");Step.iSeq = scIdle ;} return false ;
        case scStgExpend     : if(CycleStgExpend    ()){ Trace(m_sPartName.c_str(),"CycleStgExpend     End");Step.iSeq = scIdle ;} return false ;
        case scStgTAlign     : if(CycleStgTAlign    ()){ Trace(m_sPartName.c_str(),"CycleStgTAlign     End");Step.iSeq = scIdle ;} return false ;
        case scStgStartSetMc : if(CycleStgStartSetMc()){ Trace(m_sPartName.c_str(),"CycleStgStartSetMc End");Step.iSeq = scIdle ;} return false ;
        case scStgStartSetEg : if(CycleStgStartSetEg()){ Trace(m_sPartName.c_str(),"CycleStgStartSetEg End");Step.iSeq = scIdle ;} return false ;
        case scStgNextMove   : if(CycleStgNextMove  ()){ Trace(m_sPartName.c_str(),"CycleStgNextMove   End");Step.iSeq = scIdle ;} return false ;
        case scStgVision     : if(CycleStgVision    ()){ Trace(m_sPartName.c_str(),"CycleStgVision     End");Step.iSeq = scIdle ;} return false ;
        case scCstPlace      : if(CycleCstPlace     ()){ Trace(m_sPartName.c_str(),"CycleCstPlace      End");Step.iSeq = scIdle ;} return false ;
    }
}

//One Cycle.

bool CStage::CycleCstSupply(void)
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

    if(Stat.bReqStop) {
        if(Step.iCycle <= 15){
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
                  //if(DM.ARAY[riCST].GetCntStat(csMask)) DM.ARAY[riCST].ChangeStat(csMask,csWork);
                  if(OM.CmnOptn.bUseMapFile && !MAPU.CheckConvert()){
                      EM_SetErr(eiSTG_MapConverFail) ;
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  m_bWfrTAligned = false ;
                  m_bStartSet = false;




                  MoveActr(aiEJT_UpDn,ccBwd);
                  //MoveMotr(miWRD_XTrs,piWRD_XTrsWait)
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiEJT_UpDn,ccBwd)) return false ;
                  MoveMotr(miWRD_XStg,piWRD_XStgCassette);
                  MoveMotr(miWRD_YStg,piWRD_YStgCassette);
                  MoveMotr(miWRD_TStg,piWRD_TStgCassette);
                  MoveMotr(miWRD_ZExd,piWRD_ZExdGet );
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miWRD_XStg,piWRD_XStgCassette)) return false ;
                  if(!MoveMotr(miWRD_YStg,piWRD_YStgCassette)) return false ;
                  if(!MoveMotr(miWRD_TStg,piWRD_TStgCassette)) return false ;
                  if(!MoveMotr(miWRD_ZExd,piWRD_ZExdGet )) return false ;
                  MoveActr(aiTRS_UpDn , ccFwd);
                  Step.iCycle++;

        case  13: if(!MoveActr(aiTRS_UpDn , ccFwd)) return false ;



                  MoveMotr(miWRD_ZCst,piWRD_ZCstWork     );

                  MoveMotr(miWRD_XTrs ,piWRD_XTrsPickWait); //�� �̷��� �ϴ� �ϰ� Pick ���� �ٲٴ���.
//                  MoveMotr(miWRD_XTrs ,piWRD_XTrsPick    ); //�� �̷��� �ϴ� �ϰ� Pick ���� �ٲٴ���.
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRD_ZCst,piWRD_ZCstWork     )) return false ;
                  if(!MoveActr(aiTRS_Chuck,ccBwd             )) return false ;
                  if(!MoveMotr(miWRD_XTrs ,piWRD_XTrsPickWait)) return false ;
                  //if(!MoveActr(aiTRS_UpDn , ccBwd)
//                  if(!MoveMotr(miWRD_XTrs ,piWRD_XTrsPick    )) return false ;
                  MoveMotr(miWRD_XTrs,piWRD_XTrsPick);

                  Step.iCycle++;
                  return false ;
//
        //���� ReqStop�ÿ� �� ��ȣ ����.
        case  15: if(!MoveMotr(miWRD_XTrs,piWRD_XTrsPick)) return false ;
                  MoveActr(aiTRS_Chuck,ccFwd);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiTRS_Chuck,ccFwd)) return false ;
                  if(!IO_GetX(xTRS_Wafer) && !OM.MstOptn.bDryRun){  // ������ ���� ���.
                      MoveActr(aiTRS_Chuck,ccBwd);
                      //MoveMotr(miWRD_XTrs,piWRD_XTrsPickWait);
                      Step.iCycle++;
                      return false;
                  }
                  else {  //������ �ִ°��.
                      //MoveMotr(miWRD_XTrs,piWRD_XTrsPickWait , true); ���� ���� ȿ���� ������ ���Ƽ�.
                      MT_GoAbsSlow(miWRD_XTrs , PM.GetValue(miWRD_XTrs , pvWRD_XTrsPickWait) - 30.0);


                      Step.iCycle = 20;
                      return false;
                  }

        case  17: if(!MoveActr(aiTRS_Chuck,ccBwd)) return false ;
                  MoveMotr(miWRD_XTrs,piWRD_XTrsPickWait);
                  Step.iCycle++;
                  return false;

        case  18: if(!MoveMotr(miWRD_XTrs,piWRD_XTrsPickWait)) return false ;
                  FindChip(riCST,r,c,csUnkwn);
                  DM.ARAY[riCST].SetStat(r,0,csEmpty);
                  if(!DM.ARAY[riCST].GetCntStat(csUnkwn)) {
                      EM_SetErr(eiCST_NeedChange) ;
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  //Step.iCycle = 10;
                  //return false;
                  Step.iCycle = 0 ;
                  return true ;


        //case 13 ->
        case  20: //if(!MoveMotr(miWRD_XTrs,piWRD_XTrsPickWait , true)) return false ; //���� ����� �ѹ� ó�ֱ� ����.
                  if(!MT_GetStop(miWRD_XTrs))return false ;

                  MoveActr(aiTRS_Chuck,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  21: if(!MoveActr(aiTRS_Chuck,ccBwd)) return false ;
                  MoveMotr(miWRD_XTrs,piWRD_XTrsPick);
                  Step.iCycle++;
                  return false;

        case  22: if(!MoveMotr(miWRD_XTrs,piWRD_XTrsPick)) return false ;
                  MoveActr(aiTRS_Chuck,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  23: if(!MoveActr(aiTRS_Chuck,ccFwd)) return false ;
                  MoveMotr(miWRD_XTrs,piWRD_XTrsPlace , true);


                  Step.iCycle++;
                  return false ;

        case  24: if(!MoveMotr(miWRD_XTrs,piWRD_XTrsPlace , true)) return false ;

                  if(!OM.MstOptn.bDryRun && !IO_GetX(xTRS_Wafer)){ // �߰��� �����ų� ������ ������� �����ʹ� �����ۿ�
                      MoveActr(aiTRS_Chuck,ccBwd);
                      MoveMotr(miWRD_XTrs,piWRD_XTrsWait);
                      EM_SetErr(eiSTG_TransWaferFeedingFail) ;
                  }

                  MoveActr(aiTRS_Chuck,ccBwd);
                  Step.iCycle++;
                  return false;

        case  25: if(!MoveActr(aiTRS_Chuck,ccBwd)) return false ;
                  MT_GoIncRun(miWRD_XTrs , 5.0); //��ƴ ���ֱ�.
                  Step.iCycle++;
                  return false ;

        case  26: if(!MT_GetStopInpos(miWRD_XTrs)) return false ;
                  MoveActr(aiTRS_Chuck , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  27: if(!MoveActr(aiTRS_Chuck , ccFwd)) return false ;
                  MoveMotr(miWRD_XTrs,piWRD_XTrsPlace,true);
                  Step.iCycle++;
                  return false;

        case  28: if(!MoveMotr(miWRD_XTrs,piWRD_XTrsPlace,true)) return false ;
                  if(!OM.MstOptn.bDryRun && !IO_GetX(xTRS_Wafer)){
                      EM_SetErr(eiSTG_WaferNotDetected);
                  }

                  MoveActr(aiTRS_Chuck , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  29: if(!MoveActr(aiTRS_Chuck , ccBwd)) return false ;
                  FindChip(riCST,r,c,csUnkwn);
                  DM.ARAY[riCST].SetStat(r,0,csMask);
                  DM.ARAY[riSTG].SetStat(csUnkwn);

                                                //�����Ͽ��� ���� ������.
                  if(OM.CmnOptn.bUseMapFile) {
                           if(OM.DevInfo.iMapType == 0 && !MAPU.ConvertCmn(riSTG)){ EM_SetErr(eiSTG_MapConverFail); }
                      else if(OM.DevInfo.iMapType == 1 && !MAPU.ConvertNxt(riSTG)){ EM_SetErr(eiSTG_MapConverFail); }
                      //���߿� ��ȭ�� Ȯ�� �ϸ鼭 ����.
                      //DM.ARAY[riSTG].FindFrstRowCol    (csUnkwn, iRow , iCol) ;
                      //if(iRow < 0 || iCol < 0) {return false ;}
                      //�ϴ� �ӽ÷�.
                      m_iLastPickCol = !DM.ARAY[riSTG].FindFrstCol(csNone) - 1 ;
                      m_iLastPickRow = !DM.ARAY[riSTG].FindFrstRow(csNone) - 1 ;

                  }
                  else {
                      DM.ARAY[riSTG].SetMaxColRow(1,1);
                  }

                  MoveMotr(miWRD_XTrs,piWRD_XTrsWait);
                  Step.iCycle++;
                  return false ;


        case  30: if(!MoveMotr(miWRD_XTrs,piWRD_XTrsWait)) return false ;




                  //������ �Ⱦ���ġ �ʱ�ȭ.
                  m_dLastWorkPosX = MT_GetCmdPos(miWRD_XStg);
                  m_dLastWorkPosY = MT_GetCmdPos(miWRD_YStg);
                  m_dLastWorkPosT = MT_GetCmdPos(miWRD_TStg);

                  if(!EM_GetErr(eiSTG_WaferNotDetected) && EM_GetErr(eiSTG_TransWaferFeedingFail)){
                      MoveMotr(miWRD_ZCst ,piWRD_ZCstWait);
                  }


                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CStage::CycleStgExpend(void)
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

        case  10: IO_SetY(ySTG_Dryer,true);

                  MoveActr(aiEJT_UpDn,ccBwd);
                  MoveMotr(miWRD_XTrs,piWRD_XTrsWait);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiEJT_UpDn,ccBwd)) return false ;
                  if(!MoveMotr(miWRD_XTrs,piWRD_XTrsWait)) return false ;
                  MoveMotr(miWRD_XStg,piWRD_XStgCassette);
                  MoveMotr(miWRD_YStg,piWRD_YStgCassette);
                  MoveMotr(miWRD_TStg,piWRD_TStgCassette);
                  //MoveMotr(miWRD_ZExd,piWRD_ZExdWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRD_XStg,piWRD_XStgCassette)) return false ;
                  if(!MoveMotr(miWRD_YStg,piWRD_YStgCassette)) return false ;
                  if(!MoveMotr(miWRD_TStg,piWRD_TStgCassette)) return false ;
                  //if(!MoveMotr(miWRD_ZExd,piWRD_ZExdWait)) return false ;
                  MoveMotr(miWRD_ZExd,piWRD_ZExdExpend );
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRD_ZExd,piWRD_ZExdExpend )) return false ;
                  MoveMotr(miWRD_XStg,piWRD_XStgAlign);
                  MoveMotr(miWRD_YStg,piWRD_YStgAlign);
                  MoveMotr(miWRD_TStg,piWRD_TStgAlign);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRD_XStg,piWRD_XStgAlign)) return false ;
                  if(!MoveMotr(miWRD_YStg,piWRD_YStgAlign)) return false ;
                  if(!MoveMotr(miWRD_TStg,piWRD_TStgAlign)) return false ;
                  IO_SetY(ySTG_Dryer,false);

                  MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait)) return false ;

                  if(!OM.CmnOptn.bUseMapFile) {
                      DM.ARAY[riSTG].SetMaxColRow(1,1);
                      DM.ARAY[riSTG].SetStat(csUnkwn);
                  }

                  Step.iCycle = 0 ;
                  return true ;

    }
}
bool CStage::CycleStepMove (bool _bInit , int _iIncR , int _iIncC , int _iStep)
{

    static int iCrntR = 0 ;  //���� ��ǥ
    static int iCrntC = 0 ;  //���� ��ǥ

    static bool bRowFirst      = true ; //
    static bool bStraightFirst = true ; //���� �̵��� �ƴҶ� ���� ���� ������ �밢�� ���� ������.

    double dPosRFrstX , dPosRFrstY , dDistToCtrRFrst;
    double dPosCFrstX , dPosCFrstY , dDistToCtrCFrst;

    if(_bInit) {
        Step.iStepMove = 10 ;
        iCrntR = 0 ;
        iCrntC = 0 ;

        dPosCFrstX = MT_GetCmdPos(miWRD_XStg) + _iIncC * (OM.DevInfo.dWf_PitchX) ;
        dPosCFrstY = MT_GetCmdPos(miWRD_YStg) ;

        dPosRFrstX = MT_GetCmdPos(miWRD_XStg) ;
        dPosRFrstY = MT_GetCmdPos(miWRD_YStg) + _iIncR * (OM.DevInfo.dWf_PitchY) ;

        dDistToCtrRFrst = pow(dPosRFrstX - OM.MstOptn.dStgXEjtCt , 2) + pow(dPosRFrstY - OM.MstOptn.dStgYEjtCt , 2);
        dDistToCtrCFrst = pow(dPosCFrstX - OM.MstOptn.dStgXEjtCt , 2) + pow(dPosCFrstY - OM.MstOptn.dStgYEjtCt , 2);

        bRowFirst = dDistToCtrCFrst > dDistToCtrRFrst ; //�ܰ��� Ÿ�� ���� �ʰ� ���� �̵��ؾ� �� �� ����.

        if(bRowFirst) {
            bStraightFirst = abs(_iIncR) > abs(_iIncC) ;
        }
        else {
            bStraightFirst = abs(_iIncR) < abs(_iIncC) ;
        }


    }
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmVisn.OnDelay(Step.iStepMove && Step.iStepMove == PreStep.iStepMove && CheckStop() && !OM.MstOptn.bDebugMode, 4000 )) {
        EM_SetErr(eiHED_VisnTO);
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iStepMove=%02d" , __FUNC__ , Step.iStepMove );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iStepMove = 0 ;
        return true;
    }

    sTemp = sTemp.sprintf("%s Step.iStepMove=%02d" , __FUNC__ , Step.iStepMove );
    if(Step.iStepMove != PreStep.iStepMove) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iStepMove = Step.iStepMove ;

    int iJumpC = 0 ;
    int iJumpR = 0 ;

    double dMoveX , dMoveY , dMoveT ;
    double dTempX , dTempY ;

    //Cycle.
    switch (Step.iStepMove) {

        default : if(Step.iStepMove) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iStepMove=%02d" , __FUNC__ , Step.iStepMove );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iStepMove = 0 ;
                  return true ;

        case  10: iJumpC = 0 ;
                  iJumpR = 0 ;

                  /*
                  if(_iIncR == 0 || _iIncC == 0) { //������ ���.
                      for(int i = 0; i < _iStep ; i++){
                          if((_iIncC - iCrntC - iJumpC) == 0 &&
                             (_iIncR - iCrntR - iJumpR) == 0 ){
                             break ;
                          }
                          else if(abs(_iIncC - iCrntC - iJumpC) > abs(_iIncR - iCrntR - iJumpR)){ //X�� ������ X������ �������� ��.
                              if(_iIncC < 0) iJumpC-- ;
                              else           iJumpC++ ;

                          }
                          else if(abs(_iIncC - iCrntC - iJumpC) < abs(_iIncR - iCrntR - iJumpR)){ //Y�� ������ Y������ �������� ��.
                              if(_iIncR < 0) iJumpR-- ;
                              else           iJumpR++ ;
                          }
                          else { //���� ���. �밢������ �������� ��.
                              if(_iIncC < 0) iJumpC-- ;
                              else           iJumpC++ ;
                              if(_iIncR < 0) iJumpR-- ;
                              else           iJumpR++ ;
                          }
                      }
                  }*/

                  //if(_iIncR == 0 || _iIncC == 0) {
                      for(int i = 0; i < _iStep ; i++){
                          if((_iIncC - iCrntC - iJumpC) == 0 &&
                             (_iIncR - iCrntR - iJumpR) == 0 ){
                             break ;
                          }

                          if(bStraightFirst) {
                              if(abs(_iIncC - iCrntC - iJumpC) > abs(_iIncR - iCrntR - iJumpR)){ //X�� ������ X������ �������� ��.
                                  if(_iIncC < 0) iJumpC-- ;
                                  else           iJumpC++ ;

                              }
                              else if(abs(_iIncC - iCrntC - iJumpC) < abs(_iIncR - iCrntR - iJumpR)){ //Y�� ������ Y������ �������� ��.
                                  if(_iIncR < 0) iJumpR-- ;
                                  else           iJumpR++ ;
                              }
                              else { //���� ���. �밢������ �������� ��.
                                  if(_iIncC < 0) iJumpC-- ;
                                  else           iJumpC++ ;
                                  if(_iIncR < 0) iJumpR-- ;
                                  else           iJumpR++ ;
                              }
                          }
                          else {
                              if(abs(_iIncC - iCrntC - iJumpC) != 0 && abs(_iIncR - iCrntR - iJumpR) != 0){ //X�� ������ X������ �������� ��.
                                  if(_iIncC < 0) iJumpC-- ;
                                  else           iJumpC++ ;
                                  if(_iIncR < 0) iJumpR-- ;
                                  else           iJumpR++ ;

                              }
                              else if(abs(_iIncC - iCrntC - iJumpC) < abs(_iIncR - iCrntR - iJumpR)){ //Y�� ������ Y������ �������� ��.
                                  if(_iIncR < 0) iJumpR-- ;
                                  else           iJumpR++ ;
                              }
                              else { //���� ���. �밢������ �������� ��.
                                  if(_iIncC < 0) iJumpC-- ;
                                  else           iJumpC++ ;
                              }
                          }
                      }
                  //}


                  dTempX = MT_GetCmdPos(miWRD_XStg) + (OM.DevInfo.dWf_PitchX) * iJumpC ;
                  dTempY = MT_GetCmdPos(miWRD_YStg) + (OM.DevInfo.dWf_PitchY) * iJumpR ;
                  //if(!IsWaferZone(dTempX,dTempY)) {
                  //    EM_SetErr(eiSTG_EjectorSoftLimit);
                  //    return true ;
                  //}
                  dTempX = (OM.DevInfo.dWf_PitchX) * iJumpC ;
                  dTempY = (OM.DevInfo.dWf_PitchY) * iJumpR ;

                  MT_GoIncRun(miWRD_XStg , dTempX );
                  MT_GoIncRun(miWRD_YStg , dTempY );
                  iCrntC += iJumpC ;
                  iCrntR += iJumpR ;
                  Step.iStepMove++ ;
                  return false ;

        case  11: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;

                  if(iCrntC == _iIncC && iCrntR == _iIncR) {
                      Step.iStepMove = 0 ;
                      return true ;
                  }

                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);

                  Step.iStepMove++ ;
                  return false;

        case  12: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr, &RsltWfr);
                  /*
                      veWfrOk       = 0 ,
                      veWfrRctFail  = 1 ,
                      veWfrRct      = 2 ,
                      veWfrAngle    = 3 ,
                      veWfrCrack    = 4 ,
                      veWfrTsdFail  = 5 ,
                      veWfrDot      = 6 ,
                  */
                  if(RsltWfr.iRet == veWfrRctFail || RsltWfr.iRet == veWfrRct) { //�簢�� ��ã������ �׳� ����
                      Step.iStepMove = 10 ;
                      return false ;
                  }

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );  //��Ÿ���� ��.

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iStepMove ++;
                  return false ;


        case  13: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;
                  Step.iStepMove = 10 ;
                  return false ;
    }
}


bool CStage::CycleVisn(EN_VISN_ID _Id, bool _bInit, bool _bDryRun)
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

        case  10: SV_SetLight(_Id); //���� ����. rs-232�� ����. �̸� ���� ���ƾ���.
                  m_tmVisnTemp.Clear();
                  Step.iVisn++ ;
                  return false ;

        case  11: if(!m_tmVisnTemp.OnDelay(true , OM.DevOptn.iWfrVsBfDly)) return false ;

                  SV_Grab(_Id);
                  m_tmVisnTemp.Clear() ;
                  Step.iVisn++;
                  return false;

                  //�ؿ��� �� ��ȣ ����.
        case  12: if(m_tmVisnTemp.OnDelay(!OM.MstOptn.bDebugMode , OM.DevOptn.iWfrVsGrabTO)) {
                      EM_SetErr(eiSTG_WfrVsGrabFail);
                      Step.iVisn = 0 ;
                      return true ;
                  }
                  if(!SV_GetGrabEnd(_Id)) return false ;

                  if(!_bDryRun) SV_Inspect(_Id,false);
                  m_tmVisnTemp.Clear() ;
                  Step.iVisn++;
                  return false ;

        case  13: if(m_tmVisnTemp.OnDelay(!OM.MstOptn.bDebugMode,OM.DevOptn.iWfrVsInspTO )) {
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iVisn = 0 ;
                      return true ;
                  }
                  if(!_bDryRun && !SV_GetInspEnd(_Id)) return false ;

                  Step.iVisn=0;
                  return true ;

    }
}

double CStage::GetLeftEnd  (double _dCmdPosX,double _dCmdPosY)
{
    double dXPitch    = OM.DevInfo.dWf_PitchX ;
    int    iWfrMaxCol = OM.DevInfo.dWf_Diameter / dXPitch ;
    double dRetPos    = _dCmdPosX ;

    //������ ��ġ�� �̵��Ͽ� ���� �ִ� ������ ã�´�.
    for(int x = 1 ; x <= iWfrMaxCol ; x++ ) {
        if(IsWaferZone(_dCmdPosX - x* dXPitch , _dCmdPosY)) {
            dRetPos = _dCmdPosX - x* dXPitch ;
        }
        else return dRetPos ;
    }
    return dRetPos ;
}

double CStage::GetRightEnd (double _dCmdPosX,double _dCmdPosY)
{
    double dXPitch    = OM.DevInfo.dWf_PitchX ;
    int    iWfrMaxCol = OM.DevInfo.dWf_Diameter / dXPitch ;
    double dRetPos    = _dCmdPosX ;

    //������ ��ġ�� �̵��Ͽ� ���� �ִ� ������ ã�´�.
    for(int x = 1 ; x <= iWfrMaxCol ; x++ ) {
        if(IsWaferZone(_dCmdPosX + x* dXPitch , _dCmdPosY)) {
            dRetPos = _dCmdPosX + x* dXPitch ;
        }
        else return dRetPos ;
    }
    return dRetPos ;
}

double CStage::GetTopEnd   (double _dCmdPosX,double _dCmdPosY)
{
    double dYPitch    = OM.DevInfo.dWf_PitchY ;
    int    iWfrMaxRow = OM.DevInfo.dWf_Diameter / dYPitch ;
    double dRetPos    = _dCmdPosY ;

    //������ ��ġ�� �̵��Ͽ� ���� �ִ� ������ ã�´�.
    for(int y = 1 ; y <= iWfrMaxRow ; y++) {
        if(IsWaferZone(_dCmdPosX , _dCmdPosY - y* dYPitch)) {
            dRetPos = _dCmdPosY - y* dYPitch ;
        }
        else return dRetPos ;
    }
    return dRetPos ;
}

double CStage::GetBottomEnd(double _dCmdPosX,double _dCmdPosY)
{
    double dYPitch    = OM.DevInfo.dWf_PitchY ;
    int    iWfrMaxRow = OM.DevInfo.dWf_Diameter / dYPitch ;
    double dRetPos    = _dCmdPosY ;

    //������ ��ġ�� �̵��Ͽ� ���� �ִ� ������ ã�´�.
    for(int y = 1 ; y <= iWfrMaxRow ; y++ ) {
        if(IsWaferZone(_dCmdPosX , _dCmdPosY + y* dYPitch)) {
            dRetPos = _dCmdPosY + y* dYPitch ;
        }
        else return dRetPos ;
    }
    return dRetPos ;
}

float __stdcall MATH_GetLineAngle(float x1, float y1, float x2, float y2)
{
    float rad, deg;
    float cntval ;
    float dx, dy, dl;

    dx = x2 - x1;
    dy = y2 - y1;
    dl = sqrt(pow(dx, 2) + pow(dy, 2));

    if (dl==0) return 0.0; //������ ���̰� 0�̶� �Ǻ� ����.

    rad = asin(fabs(dy/dl));
                                    
    if (dx >= 0 && dy >= 0) rad = 3.14159265358979323846*2 - rad;
    if (dx  < 0 && dy >= 0) rad = 3.14159265358979323846   + rad;
    if (dx  < 0 && dy  < 0) rad = 3.14159265358979323846   - rad;
    if (dx >= 0 && dy  < 0) rad =                            rad;

    deg = (rad*180)/3.14159265358979323846;

    return deg;
}


bool CStage::CycleStgTAlign(void)
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

    if(Stat.bReqStop) {
        Step.iCycle = 0;
        return true;
    }

    int r,c ;
    double dTemp ;
    bool bVsnRet ;
    static int    iInspStep = 0 ;
    const  double dInspGap  = 1.5 ;
    const  int    iInspCnt  = 9 ; //3X3 = 9
    double dMoveX , dMoveY , dMoveT ;

    //����Ĩ ��Ÿ���� ����.
    const  int    iCntChipTInspCnt  = 3 ;
    static int    iCntChipTInspStep = 0 ;

    //������ ���� ����.
    static double dLeftChipCntX ;
    static double dLeftChipCntY ;

    static double dRightChipCntX ;
    static double dRightChipCntY ;

    //����,������Ĩ ����.
    static int    iInspCntLtRtChip ;
    const  int    iMaxInspCntLtRtChip = 20;
    double        dTriWidth        ;
    double        dTriHeight       ;
    double        dAngle           ;


    //���� Ĩ ã�� �� ����.
    static double dCntChipXLmt ; //����Ĩ ã�µ� �������� ���� X 1��ġ��.
    static double dCntChipYLmt ; //����Ĩ ã�µ� �������� ���� Y 1��ġ��.
    static int    iCntChipRow  ; //����Ĩ ã�µ� �ο� �� %2 �� 0�̸� �������� 1�̸� ����������.

    const bool bUseLeftRight = true; //�¿� ġ�� ��

    static int iTrgRow = 0;
    static int iTrgCol = 0;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;


        case  10: if(OM.MstOptn.bDryRun) {
                      m_dCntChipPosX = MT_GetCmdPos(miWRD_XStg);
                      m_dCntChipPosY = MT_GetCmdPos(miWRD_YStg);
                      m_dCntChipPosT = MT_GetCmdPos(miWRD_TStg);

                      m_bWfrTAligned = true ;

                      Step.iCycle = 0 ;
                      return true ;
                  }


                  iInspStep = 0 ;
                  iCntChipTInspStep = 0 ;

                  dLeftChipCntX  = 0 ;
                  dLeftChipCntY  = 0 ;

                  dRightChipCntX = 0 ;
                  dRightChipCntY = 0 ;

                  iInspCntLtRtChip = 0 ;

                  //����Ĩ ����.
                  dCntChipXLmt = GetMotrPos(miWRD_XStg , piWRD_XStgAlign) - (OM.DevInfo.dWf_PitchX) * 2;
                  dCntChipYLmt = GetMotrPos(miWRD_YStg , piWRD_YStgAlign) - (OM.DevInfo.dWf_PitchY) * 5;
                  iCntChipRow  = 0 ;

                  MoveMotr(miWRD_ZExd,piWRD_ZExdExpend);
                  MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait);
                  MoveActr(aiEJT_UpDn , ccBwd);


                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveMotr(miWRD_ZExd,piWRD_ZExdExpend)) return false ;
                  if(!MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait)) return false ;
                  if(!MoveActr(aiEJT_UpDn , ccBwd         )) return false ;

                  MoveMotr(miWRD_XStg,piWRD_XStgAlign);
                  MoveMotr(miWRD_YStg,piWRD_YStgAlign);
                  MoveMotr(miWRD_TStg,piWRD_TStgAlign);
                  Step.iCycle++;
                  return false;

                  //�ؿ��� �� ��ȣ ����.
        case  12: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;


                  Step.iCycle++;
                  return false;

        case  13: CycleVisn(viWfr, true , OM.MstOptn.bDryRun);

                  Step.iCycle++;
                  return false ;

        case  14: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false;
                  SV_GetRslt(viWfr, &RsltWfr);


                  /*
                      veWfrOk       = 0 ,
                      veWfrRctFail  = 1 ,
                      veWfrRct      = 2 ,
                      veWfrAngle    = 3 ,
                      veWfrCrack    = 4 ,
                      veWfrTsdFail  = 5 ,
                      veWfrDot      = 6 ,
                  */
                  if(RsltWfr.iRet == veWfrRctFail || RsltWfr.iRet == veWfrRct) { //���� �̸�. ������׷� ���� ���� �̵��غ���. ���ݾ�.
                      if(iCntChipRow%2 == 0){ //¦�� ���϶� ������ ->>>
                          if(dCntChipXLmt < MT_GetCmdPos(miWRD_XStg)){ //���� ���� �ȳѾ����� ������.
                              MT_GoAbsRun(miWRD_XStg , MT_GetCmdPos(miWRD_XStg) - dInspGap);
                              Step.iCycle = 12 ;
                              return false ;
                          }
                          else { //���� ������ �Ѿ����� �ѿ� �̵�.
                              if(dCntChipYLmt < MT_GetCmdPos(miWRD_YStg) - dInspGap){
                                  MT_GoAbsRun(miWRD_YStg , MT_GetCmdPos(miWRD_YStg) - dInspGap);
                                  iCntChipRow++;
                                  Step.iCycle = 12 ;
                                  return false ;
                              }
                              else { //���Ա��� ���µ� ��ã��.
                                  EM_SetErr(eiSTG_WfrVsInspFail);
                                  Step.iCycle=0;
                                  return true ;
                              }
                          }
                      }
                      else { //Ȧ�� ���϶� ������ <<----
                          if(GetMotrPos(miWRD_XStg , piWRD_XStgAlign) > MT_GetCmdPos(miWRD_XStg)){ //���� ���� �ȳѾ����� ������.
                              MT_GoAbsRun(miWRD_XStg , MT_GetCmdPos(miWRD_XStg) + dInspGap);
                              Step.iCycle = 12 ;
                              return false ;

                          }
                          else { //���� ������ �Ѿ����� �ѿ� �̵�.
                              MT_GoAbsRun(miWRD_YStg , MT_GetCmdPos(miWRD_YStg) - dInspGap);
                              iCntChipRow++;
                              Step.iCycle = 12 ;
                              return false ;
                          }

                      }
                  }

                  if(!VisnRsltLmtCheck(&RsltWfr)) {
                      EM_SetErr(eiSTG_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }


                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               true );  //��Ÿ���� ��.

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle++;
                  return false ;


        //����Ĩ ��Ÿ ���� ����. �ؿ��� ���ܹ�ȣ �� ����.
        case  15: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false ;

        case  16: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false;
                  SV_GetRslt(viWfr, &RsltWfr);

                  /*
                      veWfrOk       = 0 ,
                      veWfrRctFail  = 1 ,
                      veWfrRct      = 2 ,
                      veWfrAngle    = 3 ,
                      veWfrCrack    = 4 ,
                      veWfrTsdFail  = 5 ,
                      veWfrDot      = 6 ,
                  */
                                                                //RectFail
                  if(RsltWfr.iRet == veWfrRctFail || RsltWfr.iRet == veWfrRct) { //���� �̸�.����.
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(!VisnRsltLmtCheck(&RsltWfr)) {
                      EM_SetErr(eiSTG_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               true );  //��Ÿ���� ��. �������� ���Ϳ����� �����ᵵ ũ�� �ȹ��� ����.

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  iCntChipTInspStep++;

                  //3�� �� �Ѵ�.
                  if(iCntChipTInspStep < iCntChipTInspCnt) {
                      Step.iCycle = 17 ;
                      return false ;
                  }

                  Step.iCycle++;
                  return false ;

        case  17: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  m_dCntChipPosX = MT_GetCmdPos(miWRD_XStg);
                  m_dCntChipPosY = MT_GetCmdPos(miWRD_YStg);
                  m_dCntChipPosT = MT_GetCmdPos(miWRD_TStg);

                  if(!bUseLeftRight) {
                      m_dCntChipPosX = MT_GetCmdPos(miWRD_XStg);
                      m_dCntChipPosY = MT_GetCmdPos(miWRD_YStg);
                      m_dCntChipPosT = MT_GetCmdPos(miWRD_TStg);

                      m_bWfrTAligned = true ;

                      Step.iCycle = 0 ;
                      return true ;
                  }

                  Step.iCycle++ ;
                  return false ;



        //�������� �������� ���� Ȯ��.
        case  18: iTrgRow = 0 ;
                  iTrgCol = (OM.DevInfo.iWColCnt / 2) - 2  ;

                  CycleStepMove(true , iTrgRow , -iTrgCol , 3);


                  //MT_GoAbsRun(miWRD_XStg , dTemp);

                  Step.iCycle++;
                  return false ;

        case  19: if(!CycleStepMove(false , iTrgRow , -iTrgCol , 3))return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  20: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false;
                  SV_GetRslt(viWfr, &RsltWfr);

                  //float fRsltT ;  //���� T��.
                  if(RsltWfr.iRet == veWfrRctFail || RsltWfr.iRet == veWfrRct) { //���� �̸�.����.
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(!VisnRsltLmtCheck(&RsltWfr)) {
                      EM_SetErr(eiSTG_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false ); //��Ÿ���� �Ⱦ��� XY �糡�ܿ��� ������ ��ü�� T�� ã��.

                  dLeftChipCntX = dMoveX ;
                  dLeftChipCntY = dMoveY ;

                  MT_GoAbsRun(miWRD_XStg , m_dCntChipPosX);
                  MT_GoAbsRun(miWRD_YStg , m_dCntChipPosY);
                  MT_GoAbsRun(miWRD_TStg , m_dCntChipPosT);

                  Step.iCycle++;
                  return false;

        //�������� ���������� ���� Ȯ��.
        case  21: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  CycleStepMove(true , iTrgRow , iTrgCol , 3);

                  //dTemp = GetRightEnd(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg));
                  //MT_GoAbsRun(miWRD_XStg , dTemp);
                  iInspCntLtRtChip = 0 ;
                  Step.iCycle++;
                  return false ;

        //�ؿ��� ���� ���� ����.
        case  22: if(!CycleStepMove(false , iTrgRow , iTrgCol , 3))return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case  23: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false;
                  SV_GetRslt(viWfr, &RsltWfr);

                  //float fRsltT ;  //���� T��.
                  if(RsltWfr.iRet == veWfrRctFail || RsltWfr.iRet == veWfrRct) { //���� �̸�.����.
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(!VisnRsltLmtCheck(&RsltWfr)) {
                      EM_SetErr(eiSTG_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false ); //��Ÿ���� �Ⱦ��� XY �糡�ܿ��� ������ ��ü�� T�� ã��.

                  dRightChipCntX = dMoveX ;
                  dRightChipCntY = dMoveY ;

                  dTriWidth   = dRightChipCntX - dLeftChipCntX ;
                  dTriHeight  = dRightChipCntY - dLeftChipCntY ;

                  //dAngle = MATH_GetLineAngle(dLeftChipCntX , dLeftChipCntY , dRightChipCntX , dRightChipCntY ) ;
                  dAngle = MATH_GetLineAngle(dRightChipCntX , dRightChipCntY , dLeftChipCntX , dLeftChipCntY ) ;

                  //������.
                  dAngle = -(dAngle - 180) ;

                  MT_GoIncRun(miWRD_TStg , dAngle);
                  MT_GoAbsRun(miWRD_XStg , m_dCntChipPosX);
                  MT_GoAbsRun(miWRD_YStg , m_dCntChipPosY);

                  Step.iCycle++;
                  return false ;

        case  24: if(!MT_GetStopInpos(miWRD_TStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false ;



        //������ ����Ĩ �����.
        case  25: if(!CycleVisn(viWfr , false, OM.MstOptn.bDryRun)) return false;
                  SV_GetRslt(viWfr, &RsltWfr);
                  iInspCntLtRtChip++;

                  if(RsltWfr.iRet == veWfrRctFail || RsltWfr.iRet == veWfrRct) { //���� �̸�.����.
                      if(iInspCntLtRtChip >= iMaxInspCntLtRtChip) {
                          EM_SetErr(eiSTG_WfrVsInspFail);
                          Step.iCycle = 0 ;
                          return true ;
                      }
                      else {
                          MT_GoIncRun(miWRD_XStg , dInspGap);
                          Step.iCycle = 22 ;
                          return false ;


                      }
                  }
                  if(!VisnRsltLmtCheck(&RsltWfr)) {
                      EM_SetErr(eiSTG_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  //����Ĩ ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );  //��Ÿ���� �Ⱦ��� XY �糡�ܿ��� ������ ��ü�� T�� ã��.

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle++;
                  return false ;

        case  26: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  m_dCntChipPosX = MT_GetCmdPos(miWRD_XStg);
                  m_dCntChipPosY = MT_GetCmdPos(miWRD_YStg);
                  m_dCntChipPosT = MT_GetCmdPos(miWRD_TStg);

                  m_bWfrTAligned = true ;

                  Step.iCycle = 0 ;
                  return true ;
    }
}

//��ġ�� �̿��� ��ŸƮ �V.
bool CStage::CycleStgStartSetMc(void)
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

    if(Stat.bReqStop) {
        Step.iCycle = 0;
        return true;
    }

    int r,c ;
    static double dMoveX ;
    static double dMoveY ;
    static double dMoveT ;
//


    const  double dInspGap  = 1.5 ;
    static double dOriMatchXLmt ; //���� ã�µ� ���� �������� ���� X 1��ġ��.
    static double dOriMatchYLmt ; //���� ã�µ� ���� �������� ���� Y 1��ġ��.
    static int    iOriMatchRow  ; //���� ã�µ� �ο� �� %2 �� 0�̸� �������� 1�̸� ����������.


    static int    iFindOriMatchRow ; //���� ��Ī ã�� �ο� ���� ī��Ʈ    ���⼭ �ο� ��ġ�� dInspGap
    static int    iFindOriMatchCol ; //���� ��Ī ã�� �÷� ���� ī��Ʈ ��ġ�� dInspGap

    static int    iFstChipInspCnt ; //�۾��ϴ� ������ �ٽ� �־����� ù �۾� Ĩ ��ġ ���� ã��  �˻� Ƚ��.

    bool bInspRet ;




    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(OM.MstOptn.bDryRun) {

                      m_dLastWorkPosX = MT_GetCmdPos(miWRD_XStg) ;
                      m_dLastWorkPosY = MT_GetCmdPos(miWRD_YStg) ;
                      m_dLastWorkPosT = MT_GetCmdPos(miWRD_TStg) ;

                      m_bStartSet = true ;
                      Step.iCycle = 0 ;
                      return true ;

                  }




                  //���� ã��  ����.
                  dOriMatchXLmt = GetMotrPos(miWRD_XStg , piWRD_XStgFindOri) + (OM.DevInfo.dWf_PitchX) * 1 ;
                  dOriMatchYLmt = GetMotrPos(miWRD_YStg , piWRD_YStgFindOri) + (OM.DevInfo.dWf_PitchY) * 1 ;
                  iOriMatchRow  = 0 ;

                  iFstChipInspCnt = 0 ;


                  MoveMotr(miWRD_ZExd,piWRD_ZExdExpend);
                  MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait);
                  MoveActr(aiEJT_UpDn , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveMotr(miWRD_ZExd,piWRD_ZExdExpend)) return false ;
                  if(!MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait)) return false ;
                  if(!MoveActr(aiEJT_UpDn , ccBwd         )) return false ;
                  MoveMotr(miWRD_XStg,piWRD_XStgFindOri);
                  MoveMotr(miWRD_YStg,piWRD_YStgFindOri);
                  MoveMotr(miWRD_TStg,piWRD_TStgWork   );
                  Step.iCycle++;
                  return false;

                  //�ؿ��� �� ��ȣ ����.
        case  12: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  Step.iCycle++;
                  return false;

        case  13: CycleVisn(viWfo , true , OM.MstOptn.bDryRun);

                  Step.iCycle++;
                  return false ;

        case  14: if(!CycleVisn(viWfo , false , OM.MstOptn.bDryRun)) return false;
                  SV_GetRslt(viWfo , &RsltWfo) ;


                  bInspRet = RsltWfo.iRet == veWfoOk;


                  if(!bInspRet) { //���� �̸�. ������׷� ������ �Ʒ��� �̵��غ���. ���ݾ�.
                      if(iOriMatchRow%2 == 0){ //¦�� ���϶� ������ ->>>
                          if(dOriMatchXLmt > MT_GetCmdPos(miWRD_XStg)){ //���� ���� �ȳѾ����� ������.
                              MT_GoAbsRun(miWRD_XStg , MT_GetCmdPos(miWRD_XStg) + dInspGap);
                              Step.iCycle = 12 ;
                              return false ;
                          }
                          else { //���� ������ �Ѿ����� �ѿ� ������ �̵�.
                              if(dOriMatchYLmt > MT_GetCmdPos(miWRD_YStg) + dInspGap){
                                  MT_GoAbsRun(miWRD_YStg , MT_GetCmdPos(miWRD_YStg) + dInspGap);
                                  iOriMatchRow++;
                                  Step.iCycle = 12 ;
                                  return false ;
                              }
                              else { //���Ա��� ���µ� ��ã��.
                                  EM_SetErr(eiSTG_WfrVsInspFail);
                                  Step.iCycle=0;
                                  return true ;
                              }
                          }
                      }
                      else { //Ȧ�� ���϶� ������ <<----
                          if(GetMotrPos(miWRD_XStg , piWRD_XStgFindOri) < MT_GetCmdPos(miWRD_XStg)){ //���� ���� �ȳѾ����� ������.
                              MT_GoAbsRun(miWRD_XStg , MT_GetCmdPos(miWRD_XStg) - dInspGap);
                              Step.iCycle = 12 ;
                              return false ;

                          }
                          else { //���� ������ �Ѿ����� �ѿ� ������ �̵�.
                              MT_GoAbsRun(miWRD_YStg , MT_GetCmdPos(miWRD_YStg) + dInspGap);
                              iOriMatchRow++;
                              Step.iCycle = 12 ;
                              return false ;
                          }

                      }
                  }

                  //���� ��Ī  ã������ ��ġ�� �̵�.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfo.fRsltX           , RsltWfo.fRsltY           , RsltWfo.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );  //��Ÿ���� �Ⱦ�.

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle++;
                  return false ;


        //����Ĩ �ѹ��� �˻� (ȭ�� ������ ���ؼ�)
        case  15: if(!MT_GoAbsRun(miWRD_XStg , dMoveX)) return false ;
                  if(!MT_GoAbsRun(miWRD_YStg , dMoveY)) return false ;
                  if(!MT_GoAbsRun(miWRD_TStg , dMoveT)) return false ;
                  CycleVisn(viWfo , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false ;

        case  16: if(!CycleVisn(viWfo , false , OM.MstOptn.bDryRun)) return false;
                  dMoveX = PM.GetValue(miWRD_XStg,poWRD_XStgFirstChip) ;
                  dMoveY = PM.GetValue(miWRD_YStg,poWRD_YStgFirstChip) ;

                  dMoveX = MT_GetCmdPos(miWRD_XStg) ;
                  dMoveY = MT_GetCmdPos(miWRD_YStg) ;

                  dMoveX = MT_GetCmdPos(miWRD_XStg) + PM.GetValue(miWRD_XStg,poWRD_XStgFirstChip) ;
                  dMoveY = MT_GetCmdPos(miWRD_YStg) + PM.GetValue(miWRD_YStg,poWRD_YStgFirstChip) ;

                  if(!IsSafeZone(dMoveX,dMoveY)) {
                      EM_SetErr(eiSTG_EjectorSoftLimit);
                      Step.iCycle= 0 ;
                      return true ;
                  }

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);

                  Step.iCycle++;
                  return false ;

        case  17: if(!MT_GoAbsRun(miWRD_XStg , dMoveX)) return false ;
                  if(!MT_GoAbsRun(miWRD_YStg , dMoveY)) return false ;

                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false ;

        //ù���� �����ǿ��� ���� ã�� ����.
        case  18: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false;
                  SV_GetRslt(viWfr , &RsltWfr);

                  if(RsltWfr.iRet != veWfrOk){ //����� �˻� �ȉ�.
                      Step.iCycle++;
                      return false ;
                  }

                  Trace("WAFER VISION",("X=" + AnsiString(RsltWfr.fRsltX)).c_str() );
                  Trace("WAFER VISION",("Y=" + AnsiString(RsltWfr.fRsltY)).c_str() );
                  Trace("WAFER VISION",("T=" + AnsiString(RsltWfr.fRsltT)).c_str() );

                  //if(!VisnRsltLmtCheck(&RsltWfr)) {
                  //    EM_SetErr(eiSTG_VisnRlstOver);
                  //    Step.iCycle = 0 ;
                  //    return true ;
                  //}
                  //������.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );  //��Ÿ���� �Ⱦ�.

//                  if( fabs(RsltWfr.fRsltX) > (OM.DevInfo.dWf_PitchX)/2 ||
//                      fabs(RsltWfr.fRsltY) > (OM.DevInfo.dWf_PitchY)/2 ){
//                      EM_SetErr(eiSTG_WfrVsThchFail);
//                  }
//                  if( fabs(RsltWfr.fRsltT) > OM.CmnOptn.dStageTLmtErr) {
//                      EM_SetErr(eiSTG_TLmtErr);
//                  }

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle++;
                  return false ;

        case  19: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  m_dLastWorkPosX = MT_GetCmdPos(miWRD_XStg) ;
                  m_dLastWorkPosY = MT_GetCmdPos(miWRD_YStg) ;
                  m_dLastWorkPosT = MT_GetCmdPos(miWRD_TStg) ;


                  m_bStartSet = true ;


                  if(OM.CmnOptn.bUseMapFile) {
                      FindFrstChip(r,c);
                  }
                  else {
                      r = OM.DevInfo.iWYOffset ;
                      c = OM.DevInfo.iWXOffset ;
                  }






                  m_iLastPickRow = r ;  //OM.DevInfo.iWFrstRow - 1 ;
                  m_iLastPickCol = c ;  //OM.DevInfo.iWFrstCol - 1 ;

                  m_bRvsDirction = false ;

                  //ù°Ĩ �۾� �����۴� �׳� ���� �ϰ�

                  if(!DM.ARAY[riSTG].GetStat(r,c) == csUnkwn) {
                      if(OM.DevOptn.bOperChckAtMatch) {
                          m_bDispStartForm = true ;
                          //�޴����϶� �ϸ� bReqStop �̰� ���V�� �ȵ�.
                          if(MM.GetManNo() == mcNoneCycle) Stat.bReqStop = true ;
                      }
                      else {
                          m_bDispStartForm = false ;
                      }
                  }

                  //�۾��ߴ� �����۵�
                  //�۾� �ߴ� �����۵��� �ٽ� ���� ù��ġ�� ã�ư���.
                  iFstChipInspCnt = 0 ;
                  if(!GetNextPos(MT_GetCmdPos(miWRD_XStg),MT_GetCmdPos(miWRD_YStg),MT_GetCmdPos(miWRD_TStg) , dMoveX , dMoveY , dMoveT )){
                      //������ ���̻� ����.
                      //�ؿ� ���� ��� �ɵ�.
                      //DM.ARAY[riSTG].SetStat(csEmpty); //���䷱�� �÷��̽� ���� ���󰡰�.
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(IsSafeZone (dMoveX , dMoveY )) {
                      MT_GoAbsRun(miWRD_XStg , dMoveX);
                      MT_GoAbsRun(miWRD_YStg , dMoveY);
                      MT_GoAbsRun(miWRD_TStg , dMoveT);
                  }
                  else {
                      EM_SetErr(eiSTG_EjectorSoftLimit);
                      Step.iCycle = 0 ;
                      return false ;
                  }

                  Step.iCycle++;
                  return false ;

        //�ؿ��� �� ��ȣ ����
        case  20: if(!MT_GetStopInpos(miWRD_XStg )) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg )) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg )) return false ;

                  iFstChipInspCnt++;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);

                  Step.iCycle++;
                  return false ;

        case  21: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun))return false;
                  SV_GetRslt(viWfr, &RsltWfr);

                  //�̱׳�� �ֱ�.
                  //if(OM.CmnOptn.bign

                  //bool  bInspEnd ; //�˻� ���� ����.
                  //
                  //bool  bRslt  ;  //�˰��� �Լ� ����...
                  //int   iRet   ;  //���� ����ġ ����....�׿� �ּ� ����. bRslt == true && iRet == 0 �̸� Pass.
                  //float fRsltX ;  //���� X��.
                  //float fRsltY ;  //���� Y��.
                  //float fRsltT ;  //���� T��.
                                                                //RectFail
                  if(RsltWfr.iRet == veWfrRctFail || RsltWfr.iRet == veWfrRct) { //���� �̸�.����.
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //if(!VisnRsltLmtCheck(&RsltWfr)) {
                  //    EM_SetErr(eiSTG_VisnRlstOver);
                  //    Step.iCycle = 0 ;
                  //    return true ;
                  //}

                  //�۾� Ĩ  ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT        ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  if(iFstChipInspCnt < 2) {
                      Step.iCycle = 20 ;
                      return false ;

                  }

                  Step.iCycle++;
                  return false ;

        case  22: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  m_dLastWorkPosX = MT_GetCmdPos(miWRD_XStg) ;
                  m_dLastWorkPosY = MT_GetCmdPos(miWRD_YStg) ;
                  m_dLastWorkPosT = MT_GetCmdPos(miWRD_TStg) ;

                  //IO_SetY(yEJT_Suction,true);

                  if(OM.DevOptn.bOperChckAtMatch) {
                      m_bDispStartForm = true ;

                      //�޴����϶� �ϸ� bReqStop �̰� ���V�� �ȵ�.
                      if(MM.GetManNo() == mcNoneCycle) Stat.bReqStop = true ;
                  }
                  else {
                      m_bDispStartForm = false ;
                  }


                  Step.iCycle = 0 ;
                  return true ;
        }
}

bool CStage::FindFrstChip(int &r , int &c)  //���� ���� ���� �Ҷ� ���� ������ �����ؾ���.
{
    int iFstR = DM.ARAY[riSTG].GetMaxRow() ;
    int iFstC = DM.ARAY[riSTG].GetMaxCol() ;
    int R , C ;

    /*
    if(DM.ARAY[riSTG].FindFrstRowCol(csUnkwn ,R,C)){
        if(R <= iFstR) {
            if(C < iFstC) {
                iFstC = C ;
                iFstR = R ;
            }
        }
    }
    //�࿡�� ��������� ��ŸƮ ���������� �ϱ�� ����.
    int r1,c1;
    if(_riWhre == riSTG ) {
        c1 = DM.ARAY[_riWhre].FindFrstRow(_csStats) ;
        if(c1%2) DM.ARAY[_riWhre].FindFrstRowLastCol(_csStats , r1 , c1) ;
        else     DM.ARAY[_riWhre].FindFrstRowCol    (_csStats , r1 , c1) ;
        r = r1 ;
        c = c1;
        return (r > -1 && c > -1);
    }
    else if(_riWhre == riCST ) {
//        r = DM.ARAY[_riWhre].FindFrstRow(_csStats) ;
        r = DM.ARAY[_riWhre].FindLastRow(_csStats) ;
        return (r > -1 && r > -1);
    }




    */

    if(DM.ARAY[riSTG].FindFrstRowCol(csUnkwn ,R,C)){if(R <= iFstR) {if(R < iFstR || C < iFstC){iFstC = C ; iFstR = R ;}}}
    if(DM.ARAY[riSTG].FindFrstRowCol(csWait  ,R,C)){if(R <= iFstR) {if(R < iFstR || C < iFstC){iFstC = C ; iFstR = R ;}}}
    if(DM.ARAY[riSTG].FindFrstRowCol(csEmpty ,R,C)){if(R <= iFstR) {if(R < iFstR || C < iFstC){iFstC = C ; iFstR = R ;}}}
    if(DM.ARAY[riSTG].FindFrstRowCol(csGood  ,R,C)){if(R <= iFstR) {if(R < iFstR || C < iFstC){iFstC = C ; iFstR = R ;}}}
    if(DM.ARAY[riSTG].FindFrstRowCol(csFail  ,R,C)){if(R <= iFstR) {if(R < iFstR || C < iFstC){iFstC = C ; iFstR = R ;}}}
    if(DM.ARAY[riSTG].FindFrstRowCol(csEror  ,R,C)){if(R <= iFstR) {if(R < iFstR || C < iFstC){iFstC = C ; iFstR = R ;}}}

    if(iFstR == DM.ARAY[riSTG].GetMaxRow() && iFstC == DM.ARAY[riSTG].GetMaxCol()) return false ;

    r=iFstR ;
    c=iFstC ;

    return false;
}

bool CStage::CycleStgStartSetEg(void)
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

    if(Stat.bReqStop) {
        Step.iCycle = 0;
        return true;
    }

    const  bool bFindLeft         = OM.DevOptn.iOriMathod == omEdgeLeftAndTop ;
    const  int  iWfColCnt         = OM.DevInfo.iWColCnt ; //30
    const  int  iWfRowCnt         = OM.DevInfo.iWRowCnt ; //30

    const  int  iMinWidRange      = OM.DevInfo.iWColCnt / 2 - 2 ;  //������ �����̱⿡ �̹��� �ȿ� �ִ°��� ���� �ʴ´�.
    const  int  iMinHeiRange      = OM.DevInfo.iWRowCnt / 2 - 2 ;  // -2�� ����Ĩ�� �з�����쿡 ���� ���.

    const  int  iWfFullRctCntX    = OM.DevOptn.iWfFullRctCntX ; //  30 ; //�����ۿ� ���� �ƴ� Ĩ�� ����.
    const  int  iWfFullRctCntY    = OM.DevOptn.iWfFullRctCntY ; //32 ; //Y���� 2�� �� ����.

    const  int  iWfFullRctMapOfsX = OM.DevOptn.iWfFullRctMapOfsX ;//0  ; //�Ⱥ��� �簢���� �ִ°����� ���� ������ �ٸ��Ƿ�
    const  int  iWfFullRctMapOfsY = OM.DevOptn.iWfFullRctMapOfsY ;//1  ; //

    const  int  iStepFind         = 3  ;  //�̵��� ���� ����.


    int  iScndFindPos = OM.DevOptn.iSecondFindPos; //15 ; //���࿡�� ã���� 15��° ���� ã�´�.

    //Static
    static int    iTempCrntPos    = 0  ;  //ó���� ���� ������ ��� �̵�Ĩ��. ����ã���� ���밪���� �̵�.

    static int    iCtrChipX       = 0  ;  //����Ĩ�� ��ǥ�� �˾Ƴ���.
    static int    iCtrChipY       = 0  ;

    static double dTopX           = 0.0 ; //������ ���ʰ˻��ÿ� ó��Ĩ��ġ
    static double dTopY           = 0.0 ; //������ ���ʰ˻��ÿ� ó��Ĩ��ġ

    static int    iTopChipX       = 0  ; //���������� žĨ�� �ʻ��� ��ǥ
    static int    iTopChipY       = 0  ;

    int r,c ;

    double dMoveX , dMoveY , dMoveT ;
    double dTemp ;

    
    static double dLtEndPos ;
    static double dRtEndPos ;
    static double dTpEndPos ;
    static double dBtEndPos ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: if(OM.MstOptn.bDryRun) {

                      m_dLastWorkPosX = MT_GetCmdPos(miWRD_XStg) ;
                      m_dLastWorkPosY = MT_GetCmdPos(miWRD_YStg) ;
                      m_dLastWorkPosT = MT_GetCmdPos(miWRD_TStg) ;

                      m_bStartSet = true ;
                      Step.iCycle = 0 ;
                      return true ;

                  }

                  iTempCrntPos = 0 ;

                  dTopX     = 0.0 ;
                  dTopY     = 0.0 ;

                  iCtrChipX = 0 ;
                  iCtrChipY = 0 ;

                  //�ڵ� ��ġ ����...
                  dLtEndPos = 0.0 ;
                  dRtEndPos = 0.0 ;
                  dTpEndPos = 0.0 ;
                  dBtEndPos = 0.0 ;

                  MoveMotr(miWRD_ZExd,piWRD_ZExdExpend);
                  MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait);
                  MoveActr(aiEJT_UpDn , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveMotr(miWRD_ZExd,piWRD_ZExdExpend)) return false ;
                  if(!MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait)) return false ;
                  if(!MoveActr(aiEJT_UpDn , ccBwd         )) return false ;

                  MT_GoAbsRun(miWRD_XStg , m_dCntChipPosX );
                  MT_GoAbsRun(miWRD_YStg , m_dCntChipPosY );
                  MT_GoAbsRun(miWRD_TStg , m_dCntChipPosT );
                  Step.iCycle++;
                  return false ;

        case  12: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  if(bFindLeft){Step.iCycle = 100 ; return false ;}
                  else         {Step.iCycle = 200 ; return false ;}

                  Step.iCycle++;
                  return false;


        //--------Left----------------------------------------------------------
                  //�ؿ��� �� ��ȣ ����.
        case 100: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  if(iTempCrntPos + iStepFind < iMinWidRange) {//�η����� �Ѿ�� 1ĭ�� ����.
                      MT_GoIncRun(miWRD_XStg , -(OM.DevInfo.dWf_PitchX) * iStepFind );
                      iTempCrntPos += iStepFind ; //iTempCrntPos �ϴ� �������� ��� ��밪���� ������ ����.
                  }
                  else {
                      MT_GoIncRun(miWRD_XStg , -(OM.DevInfo.dWf_PitchX));
                      iTempCrntPos ++ ;
                  }

                  Step.iCycle++;
                  return false ;

        case 101: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;

                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 102: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);

                  if(RsltWfr.iRet != veWfrOk    &&  //�簢�� �˻� �Ϸ� �ȳ�� �����
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      if(iTempCrntPos < iMinWidRange) { //�ΰ� ���Ͽ����� ������ �߸� ã�ƺ��� �����̹Ƿ� �׳� �佺
                          Step.iCycle = 100 ;
                          return false ;
                      }
                      else { //���� ���̵� ���� ���� Ĩ �߰�!!!
                          MT_GoIncRun(miWRD_XStg , (OM.DevInfo.dWf_PitchX)); //������ �Ϻ��� �簢������ �̵�.
                          iTempCrntPos-- ;
                          Step.iCycle++;
                          return false ;
                      }
                  }

                  //�簢 Ĩ  ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle = 100 ;
                  return false ;

        //���� ù �Ϻ��� �簢�� �³� Ȯ��
        case 103: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 104: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet != veWfrOk    &&  //�簢�� �˻� �Ϸ� �ȳ�� �����
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  //�簢Ĩ  ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle++;
                  return false ;

        case 105: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  iCtrChipX = iTempCrntPos ;  //�������� ����Ĩ�� ��ǥ ���.
                  iCtrChipY = 0 ; //������.
                  //iTempCrntPos = iCtrChipX ;  //������ ã�� ���� ����Ĩ �ִ´�. ������ ã�Ƽ� ���밪���� �ٲ�.

                  //�ڵ� ��ġ ���.
                  dLtEndPos = MT_GetCmdPos(miWRD_XStg) ;

                  //���� �������� ������ ��Ĩ Ȯ�� ã�� ��.
                  MT_GoAbsRun(miWRD_XStg , m_dCntChipPosX);
                  MT_GoAbsRun(miWRD_YStg , m_dCntChipPosY);
                  MT_GoAbsRun(miWRD_TStg , m_dCntChipPosT);
                  Step.iCycle++;
                  return false ;

        //�׿��� ���ܾ�. ����.
        case 106: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;  //����Ĩ �̵� �Ϸ�.

                  CycleStepMove(true , 0 , iWfFullRctCntX - iTempCrntPos , iStepFind);
                  Step.iCycle++;
                  return false ;

        case 107: if(!CycleStepMove(false , 0 , iWfFullRctCntX - iTempCrntPos , iStepFind)) return false ;
                  iTempCrntPos += iWfFullRctCntX - iTempCrntPos ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 108: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet == veWfrOk    ||  //�������̹Ƿ� �簢�˻簡 �ȵǾ���.
                     RsltWfr.iRet == veWfrNoPkg ||
                     RsltWfr.iRet == veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  MT_GoIncRun(miWRD_XStg , -(OM.DevInfo.dWf_PitchX)); //������ �Ϻ��� �簢������ �̵�.
                  iTempCrntPos--;
                  Step.iCycle++;
                  return false ;

        case 109: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 110: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet != veWfrOk    &&  //�簢 �˻簡 �Ǿ�� ��.
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }

                  //�ڵ� ��ġ ���.
                  dRtEndPos = MT_GetCmdPos(miWRD_XStg) + RsltWfr.fRsltX;

                  //iTempCrntPos �ϴ� �������� ��� ��밪���� ������ ����.
                  //���� �������� ã��Col�� �̵� iScndFindPos
                  MT_GoAbsRun(miWRD_XStg , m_dCntChipPosX + (iScndFindPos - iCtrChipX)*(OM.DevInfo.dWf_PitchX));
                  MT_GoAbsRun(miWRD_YStg , m_dCntChipPosY );
                  MT_GoAbsRun(miWRD_TStg , m_dCntChipPosT );
                  iTempCrntPos = 0 ;
                  Step.iCycle=120;
                  return false ;


        //Y���� �� ��ĵ.-----------------------------------------------------------
        //�ؿ��� ��.
        case 120: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;


                  if(iTempCrntPos + iStepFind < iMinHeiRange) {//�η����� �Ѿ�� 1ĭ�� ����.
                      MT_GoIncRun(miWRD_YStg , -(OM.DevInfo.dWf_PitchY) * iStepFind );
                      iTempCrntPos += iStepFind ; //iTempCrntPos �ϴ� �������� ��� ��밪���� ������ ����.
                  }
                  else {
                      MT_GoIncRun(miWRD_YStg , -(OM.DevInfo.dWf_PitchY));
                      iTempCrntPos ++ ;
                  }

                  Step.iCycle++;
                  return false ;

        case 121: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;

                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 122: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);

                  if(RsltWfr.iRet != veWfrOk    &&  //�簢�� �˻� �Ϸ� �ȳ�� �����
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      if(iTempCrntPos < iMinHeiRange) { //�ΰ� ���Ͽ����� ������ �߸� ã�ƺ��� �����̹Ƿ� �׳� �佺
                          Step.iCycle = 120 ;
                          return false ;
                      }
                      else { //���� ���̵� ���� ���� Ĩ �߰�!!!
                          MT_GoIncRun(miWRD_YStg , (OM.DevInfo.dWf_PitchY)); //������ �Ϻ��� �簢������ �̵�.
                          iTempCrntPos-- ;
                          Step.iCycle++;
                          return false ;
                      }
                  }

                  //�簢 Ĩ  ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle = 120 ;
                  return false ;

        //���� �³� Ȯ��
        case 123: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 124: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet != veWfrOk    &&  //�簢�� �˻� �Ϸ� �ȳ�� �����
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  //�簢Ĩ  ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle++;
                  return false ;

        case 125: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  dTopY     = MT_GetCmdPos(miWRD_YStg) ;
                  dTopX     = MT_GetCmdPos(miWRD_XStg) ;

                  iTopChipX = iScndFindPos - iWfFullRctMapOfsX ; //iScndFindPos <=���� ���Ϳ��� �������ǿ� ���缭 �ö���ִ� ��Ȳ.
                  iTopChipY =              - iWfFullRctMapOfsY ; //FullRct��ǥ�迡�� Map��ǥ��� ��ȯ �ϴ� ��.

                  iCtrChipY = iTempCrntPos ;  //�������� ����Ĩ�� ��ǥ ���.

                  //�ڵ� ��ġ ���.
                  dTpEndPos = MT_GetCmdPos(miWRD_YStg) ;




                  //���� �������� �Ʒ��� Ȯ��.
                  MT_GoAbsRun(miWRD_XStg , m_dCntChipPosX + (iScndFindPos - iCtrChipX)*(OM.DevInfo.dWf_PitchX));
                  MT_GoAbsRun(miWRD_YStg , m_dCntChipPosY );
                  MT_GoAbsRun(miWRD_TStg , m_dCntChipPosT );
                  Step.iCycle=130;
                  return false ;


        //Y���� �Ʒ��� ��ĵ.-----------------------------------------------------------
        //�ؿ��� ��.
        case 130: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ; //����Ĩ ����

                  CycleStepMove(true , iWfFullRctCntY - iTempCrntPos , 0 , iStepFind);

                  Step.iCycle++;
                  return false ;

        case 131: if(!CycleStepMove(false , iWfFullRctCntY - iTempCrntPos , 0 , iStepFind)) return false ;
                  iTempCrntPos += iWfFullRctCntY - iTempCrntPos ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 132: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet == veWfrOk    ||  //�������̹Ƿ� �簢�˻簡 �ȵǾ���.
                     RsltWfr.iRet == veWfrNoPkg ||
                     RsltWfr.iRet == veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  MT_GoIncRun(miWRD_YStg , -(OM.DevInfo.dWf_PitchX)); //������ �Ϻ��� �簢������ �̵�.
                  iTempCrntPos--;
                  Step.iCycle++;
                  return false ;

        case 133: if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 134: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet != veWfrOk    &&  //�簢 �˻簡 �Ǿ�� ��.
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }

                  //�ڵ� ��ġ ���.
                  dBtEndPos = MT_GetCmdPos(miWRD_YStg) + RsltWfr.fRsltY ;

                  OM.DevInfo.dWf_PitchX = (dRtEndPos - dLtEndPos) / (OM.DevOptn.iWfFullRctCntX - 1) ;
                  OM.DevInfo.dWf_PitchY = (dBtEndPos - dTpEndPos) / (OM.DevOptn.iWfFullRctCntY - 1) ;


                  if(OM.CmnOptn.bUseMapFile) {
                      FindFrstChip(r,c);
                  }
                  else {
                      r = OM.DevInfo.iWYOffset ;
                      c = OM.DevInfo.iWXOffset ;
                  }

                  MT_GoAbsRun(miWRD_YStg , dTopY + (r - iTopChipY ) * (OM.DevInfo.dWf_PitchY));
                  MT_GoAbsRun(miWRD_XStg , dTopX + (c - iTopChipX ) * (OM.DevInfo.dWf_PitchX));

                  Step.iCycle++;
                  return false ;

        case 135: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 136: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  if(!OM.MstOptn.bDryRun) {
                      SV_GetRslt(viWfr , &RsltWfr);

                      //GetInspEnd == 0 //�˰��� �˻�
                      //Rslt.bRslt == 1 //�˻� ����
                      //Rslt.bRslt == 0 //�˻� ����
                      //Rslt.iRet  == 0:Pass , 1:RCT Fail , 2: Nodevice or Crack 3: FailDevice 4: Std Device
                  }
                  else {
                      //RsltWfr.bRslt  = true ;
                      RsltWfr.iRet   = 0    ;
                      RsltWfr.fRsltX = 0.0  ;
                      RsltWfr.fRsltY = 0.0  ;
                      RsltWfr.fRsltT = 0.0  ;

                  }
                  if(RsltWfr.iRet != veWfrOk    &&  //�簢�� �˻� �Ϸ� ���� ������ ����.
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      //EM_SetErr(eiSTG_WfrVsInspFail);
                      //Step.iCycle = 0 ;
                      //return false ;
                  }

                  //�簢 Ĩ  ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);
                  Step.iCycle++;
                  return false;

        case 137: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  m_dLastWorkPosX = MT_GetCmdPos(miWRD_XStg) ;
                  m_dLastWorkPosY = MT_GetCmdPos(miWRD_YStg) ;
                  m_dLastWorkPosT = MT_GetCmdPos(miWRD_TStg) ;

                  m_bStartSet = true ;

                  if(OM.CmnOptn.bUseMapFile) {
                      FindFrstChip(r,c);
                      m_iLastPickRow = r ;
                      m_iLastPickCol = c ;
                      DM.ARAY[riSTG].ChangeStat(csWait , csUnkwn);
                      //���� ������ ����ŷ �����൵ NextMoveŸ�� �˾Ƽ� ��ĭ�� �ȿ�����
                      //��� ����Ʈ�� ������ �������� �����ָ� ù����  ��ġ���� �ϳ� �˻��ϰ� ������.
                  }
                  else {
                      m_iLastPickRow = OM.DevInfo.iWYOffset ;
                      m_iLastPickCol = OM.DevInfo.iWXOffset ;
                      //
                      FindChip(riSTG,r,c,csUnkwn);
                      DM.ARAY[riSTG].SetStat(r,c,csWait); //���� ��ġ ���� �۾����� �ؾ���.
                  }

                  m_bRvsDirction = false ;

                  if(OM.DevOptn.bOperChckAtMatch) {
                      m_bDispStartForm = true ;
                      //�޴����϶� �ϸ� bReqStop �̰� ���V�� �ȵ�.
                      if(MM.GetManNo() == mcNoneCycle) Stat.bReqStop = true ;
                  }
                  else {
                      m_bDispStartForm = false ;
                  }

                  Step.iCycle = 0 ;
                  return true ;









        //--------Top----------------------------------------------------------
                  //�ؿ��� �� ��ȣ ����.
        case 200: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  if(iTempCrntPos + iStepFind < iMinHeiRange) {//�η����� �Ѿ�� 1ĭ�� ����.
                      MT_GoIncRun(miWRD_YStg , -(OM.DevInfo.dWf_PitchY) * iStepFind );
                      iTempCrntPos += iStepFind ; //iTempCrntPos �ϴ� �������� ��� ��밪���� ������ ����.
                  }
                  else {
                      MT_GoIncRun(miWRD_YStg , -(OM.DevInfo.dWf_PitchY));
                      iTempCrntPos ++ ;
                  }

                  Step.iCycle++;
                  return false ;

        case 201: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;

                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 202: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);

                  if(RsltWfr.iRet != veWfrOk    &&  //�簢�� �˻� �Ϸ� �ȳ�� �����
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      if(iTempCrntPos < iMinHeiRange) { //�ΰ� ���Ͽ����� ������ �߸� ã�ƺ��� �����̹Ƿ� �׳� �佺
                          Step.iCycle = 200 ;
                          return false ;
                      }
                      else { //���� ���̵� ���� ���� Ĩ �߰�!!!
                          MT_GoIncRun(miWRD_YStg , (OM.DevInfo.dWf_PitchY)); //������ �Ϻ��� �簢������ �̵�.
                          iTempCrntPos-- ;
                          Step.iCycle++;
                          return false ;
                      }
                  }

                  //�簢 Ĩ  ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle = 200 ;
                  return false ;

        //���� ù �Ϻ��� �簢�� �³� Ȯ��
        case 203: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 204: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet != veWfrOk    &&  //�簢�� �˻� �Ϸ� �ȳ�� �����
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  //�簢Ĩ  ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle++;
                  return false ;

        case 205: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  dTopY     = MT_GetCmdPos(miWRD_YStg) ;
                  dTopX     = MT_GetCmdPos(miWRD_XStg) ;

                  iCtrChipY = iTempCrntPos ;  //�������� ����Ĩ�� ��ǥ ���.
                  iCtrChipX = 0 ; //������.
                  //iTempCrntPos = iCtrChipX ;  //�Ʒ��� ã�� ���� ����Ĩ �ִ´�. ������ ã�Ƽ� ���밪���� �ٲ�.

                  //�ڵ� ��ġ ���.
                  dTpEndPos = MT_GetCmdPos(miWRD_YStg) ;

                  //���� �������� �Ʒ� �� ���� �ִ� �� Ȯ��.
                  MT_GoAbsRun(miWRD_XStg , m_dCntChipPosX);
                  MT_GoAbsRun(miWRD_YStg , m_dCntChipPosY);
                  MT_GoAbsRun(miWRD_TStg , m_dCntChipPosT);
                  Step.iCycle++;
                  return false ;

        //�׿��� ���ܾ�. ����.
        case 206: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;  //����Ĩ �̵� �Ϸ�.

                  CycleStepMove(true , iWfFullRctCntY - iTempCrntPos , 0 , iStepFind);
                  Step.iCycle++;
                  return false ;

        case 207: if(!CycleStepMove(false , iWfFullRctCntY - iTempCrntPos, 0  , iStepFind)) return false ;
                  iTempCrntPos += iWfFullRctCntY - iTempCrntPos ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 208: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet == veWfrOk    ||  //�������̹Ƿ� �簢�˻簡 �ȵǾ���.
                     RsltWfr.iRet == veWfrNoPkg ||
                     RsltWfr.iRet == veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  MT_GoIncRun(miWRD_YStg , -(OM.DevInfo.dWf_PitchY)); //������ �Ϻ��� �簢������ �̵�.
                  iTempCrntPos--;
                  Step.iCycle++;
                  return false ;

        case 209: if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 210: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet != veWfrOk    &&  //�簢 �˻簡 �Ǿ�� ��.
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }

                  //�ڵ� ��ġ ���.
                  dBtEndPos = MT_GetCmdPos(miWRD_YStg) + RsltWfr.fRsltY ;

                  //iTempCrntPos �ϴ� �������� ��� ��밪���� ������ ����.
                  //���� �������� ã��Row�� �̵� iScndFindPos
                  MT_GoAbsRun(miWRD_XStg , m_dCntChipPosX );
                  MT_GoAbsRun(miWRD_YStg , m_dCntChipPosY + (iScndFindPos - iCtrChipY)*(OM.DevInfo.dWf_PitchY));
                  MT_GoAbsRun(miWRD_TStg , m_dCntChipPosT );
                  iTempCrntPos = 0 ;
                  Step.iCycle=220;
                  return false ;


        //X���� ���� ��ĵ.-----------------------------------------------------------
        //�ؿ��� ��.
        case 220: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;


                  if(iTempCrntPos + iStepFind < iMinWidRange) {//�η����� �Ѿ�� 1ĭ�� ����.
                      MT_GoIncRun(miWRD_XStg , -(OM.DevInfo.dWf_PitchX) * iStepFind );
                      iTempCrntPos += iStepFind ; //iTempCrntPos �ϴ� �������� ��� ��밪���� ������ ����.
                  }
                  else {
                      MT_GoIncRun(miWRD_XStg , -(OM.DevInfo.dWf_PitchX));
                      iTempCrntPos ++ ;
                  }

                  Step.iCycle++;
                  return false ;

        case 221: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;

                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 222: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);

                  if(RsltWfr.iRet != veWfrOk    &&  //�簢�� �˻� �Ϸ� �ȳ�� �����
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      if(iTempCrntPos < iMinWidRange) { //�ΰ� ���Ͽ����� ������ �߸� ã�ƺ��� �����̹Ƿ� �׳� �佺
                          Step.iCycle = 220 ;
                          return false ;
                      }
                      else { //���� ���̵� ���� ���� Ĩ �߰�!!!
                          MT_GoIncRun(miWRD_XStg , (OM.DevInfo.dWf_PitchX)); //������ �Ϻ��� �簢������ �̵�.
                          iTempCrntPos-- ;
                          Step.iCycle++;
                          return false ;
                      }
                  }

                  //�簢 Ĩ  ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle = 220 ;
                  return false ;

        //���� �³� Ȯ��
        case 223: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 224: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet != veWfrOk    &&  //�簢�� �˻� �Ϸ� �ȳ�� �����
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  //�簢Ĩ  ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle++;
                  return false ;

        case 225: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;


                  iTopChipX = iTempCrntPos - iWfFullRctMapOfsX ; //iScndFindPos <=���� ���Ϳ��� �������ǿ� ���缭 �ö���ִ� ��Ȳ.
                  iTopChipY =              - iWfFullRctMapOfsY ; //FullRct��ǥ�迡�� Map��ǥ��� ��ȯ �ϴ� ��.

                  iCtrChipX = iTempCrntPos ;  //�������� ����Ĩ�� ��ǥ ���.

                  //�ڵ� ��ġ ���.
                  dLtEndPos = MT_GetCmdPos(miWRD_XStg)  ;

                  //���� �������� ������ Ȯ��.
                  MT_GoAbsRun(miWRD_XStg , m_dCntChipPosX );
                  MT_GoAbsRun(miWRD_YStg , m_dCntChipPosY + (iScndFindPos - iCtrChipY)*(OM.DevInfo.dWf_PitchY));
                  MT_GoAbsRun(miWRD_TStg , m_dCntChipPosT );
                  Step.iCycle=230;
                  return false ;


        //X���� ������ ��ĵ.-----------------------------------------------------------
        //�ؿ��� ��.
        case 230: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ; //����Ĩ ����

                  CycleStepMove(true , 0 ,iWfFullRctCntX - iTempCrntPos ,  iStepFind);

                  Step.iCycle++;
                  return false ;

        case 231: if(!CycleStepMove(false  ,0 ,iWfFullRctCntX - iTempCrntPos ,  iStepFind)) return false ;
                  iTempCrntPos += iWfFullRctCntX - iTempCrntPos ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 232: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet == veWfrOk    ||  //�������̹Ƿ� �簢�˻簡 �ȵǾ���.
                     RsltWfr.iRet == veWfrNoPkg ||
                     RsltWfr.iRet == veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }
                  MT_GoIncRun(miWRD_XStg , -(OM.DevInfo.dWf_PitchX)); //������ �Ϻ��� �簢������ �̵�.
                  iTempCrntPos--;
                  Step.iCycle++;
                  return false ;

        case 233: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 234: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;
                  SV_GetRslt(viWfr,&RsltWfr);
                  if(RsltWfr.iRet != veWfrOk    &&  //�簢 �˻簡 �Ǿ�� ��.
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      EM_SetErr(eiSTG_WfrVsInspFail);
                      Step.iCycle = 0 ;
                      return false ;
                  }

                  //�ڵ� ��ġ ���.
                  dRtEndPos = MT_GetCmdPos(miWRD_XStg) + RsltWfr.fRsltX ;

                  OM.DevInfo.dWf_PitchX = (dRtEndPos - dLtEndPos) / (OM.DevOptn.iWfFullRctCntX - 1) ;
                  OM.DevInfo.dWf_PitchY = (dBtEndPos - dTpEndPos) / (OM.DevOptn.iWfFullRctCntY - 1) ;


                  if(OM.CmnOptn.bUseMapFile) {
                      FindFrstChip(r,c);
                  }
                  else {
                      r = OM.DevInfo.iWYOffset ;
                      c = OM.DevInfo.iWXOffset ;
                  }

                  MT_GoAbsRun(miWRD_YStg , dTopY + (r - iTopChipY ) * (OM.DevInfo.dWf_PitchY));
                  MT_GoAbsRun(miWRD_XStg , dTopX + (c - iTopChipX ) * (OM.DevInfo.dWf_PitchX));

                  Step.iCycle++;
                  return false ;

        case 235: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case 236: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false ;

                  if(!OM.MstOptn.bDryRun) {
                      SV_GetRslt(viWfr , &RsltWfr);

                      //GetInspEnd == 0 //�˰��� �˻�
                      //Rslt.bRslt == 1 //�˻� ����
                      //Rslt.bRslt == 0 //�˻� ����
                      //Rslt.iRet  == 0:Pass , 1:RCT Fail , 2: Nodevice or Crack 3: FailDevice 4: Std Device
                  }
                  else {
                      //RsltWfr.bRslt  = true ;
                      RsltWfr.iRet   = 0    ;
                      RsltWfr.fRsltX = 0.0  ;
                      RsltWfr.fRsltY = 0.0  ;
                      RsltWfr.fRsltT = 0.0  ;
                  }

                  if(RsltWfr.iRet != veWfrOk    &&  //�簢�� �˻� �Ϸ� ���� ������ ����.
                     RsltWfr.iRet != veWfrNoPkg &&
                     RsltWfr.iRet != veWfrDot   ){
                      //�ؔf�� ��������.
                      //EM_SetErr(eiSTG_WfrVsInspFail);
                      //Step.iCycle = 0 ;
                      //return false ;
                  }

                  dTemp = MT_GetCmdPos(miWRD_XStg) ;

                  //�簢 Ĩ  ã������ ����.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               false );

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);
                  Step.iCycle++;
                  return false;

        case 237: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  m_dLastWorkPosX = MT_GetCmdPos(miWRD_XStg) ;
                  m_dLastWorkPosY = MT_GetCmdPos(miWRD_YStg) ;
                  m_dLastWorkPosT = MT_GetCmdPos(miWRD_TStg) ;

                  m_bStartSet = true ;

                  if(OM.CmnOptn.bUseMapFile) {
                      FindFrstChip(r,c);
                      m_iLastPickRow = r ;
                      m_iLastPickCol = c ;
                      DM.ARAY[riSTG].ChangeStat(csWait , csUnkwn);
                      //���� ������ ����ŷ �����൵ NextMoveŸ�� �˾Ƽ� ��ĭ�� �ȿ�����
                      //��� ����Ʈ�� ������ �������� �����ָ� ù����  ��ġ���� �ϳ� �˻��ϰ� ������.
                  }
                  else {
                      m_iLastPickRow = OM.DevInfo.iWYOffset ;
                      m_iLastPickCol = OM.DevInfo.iWXOffset ;
                      //
                      FindChip(riSTG,r,c,csUnkwn);
                      DM.ARAY[riSTG].SetStat(r,c,csWait); //���� ��ġ ���� �۾����� �ؾ���.
                  }


                  m_bRvsDirction = false ;

                  if(OM.DevOptn.bOperChckAtMatch) {
                      m_bDispStartForm = true ;
                      //�޴����϶� �ϸ� bReqStop �̰� ���V�� �ȵ�.
                      if(MM.GetManNo() == mcNoneCycle) Stat.bReqStop = true ;
                  }
                  else {
                      m_bDispStartForm = false ;
                  }

                  Step.iCycle = 0 ;
                  return true ;

        }
}

bool CStage::CycleStgNextMove(void)
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true;
    //}

    int r,c ;

    static int    iInspStep = 0 ;
    const  double dInspGap  = 2 ;
    const  int    iInspCnt  = 9 ; //3X3 = 9      ''
    static double dMoveX = 0.0;
    static double dMoveY = 0.0;
    static double dMoveT = 0.0;


    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: IO_SetY(yEJT_Suction,false);
                  MoveMotr(miWRD_ZExd , piWRD_ZExdExpend);

                  m_tmTemp.Clear();

                  Step.iCycle++;
                  return false ;

        case  11: if(!m_tmTemp.OnDelay(true , 30)) return false ; //�������� ������ ���� �Ȳ����� ������ �� �ణ �ְ� �ȴ�.
                  if(!MoveMotr(miWRD_ZExd , piWRD_ZExdExpend)) return false ;
                  if(!GetNextPos(MT_GetCmdPos(miWRD_XStg),MT_GetCmdPos(miWRD_YStg),MT_GetCmdPos(miWRD_TStg) , dMoveX , dMoveY , dMoveT )){
                      //������ ���̻� ����.
                      /*if(OM.CmnOptn.bUseMapFile)*/
                      DM.ARAY[riSTG].SetStat(csEmpty); //���䷱�� �÷��̽� ���� ���󰡰�.
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  if(IsSafeZone (dMoveX , dMoveY )) {
                      MT_GoAbsRun(miWRD_XStg , dMoveX);
                      MT_GoAbsRun(miWRD_YStg , dMoveY);
                      MT_GoAbsRun(miWRD_TStg , dMoveT);
                  }
                  else {
                      EM_SetErr(eiSTG_EjectorSoftLimit);
                      Step.iCycle = 0 ;
                      return false ;
                  }

                  Step.iCycle++;
                  return false ;

        case  12: if(!MT_GoAbsRun(miWRD_XStg , dMoveX)) return false ;
                  if(!MT_GoAbsRun(miWRD_YStg , dMoveY)) return false ;
                  if(!MT_GoAbsRun(miWRD_TStg , dMoveT)) return false ;

                  FindChip(riSTG,r,c,csUnkwn);
                  DM.ARAY[riSTG].SetStat(r,c,csWait);

                  m_dLastWorkPosX = MT_GetCmdPos(miWRD_XStg) ;
                  m_dLastWorkPosY = MT_GetCmdPos(miWRD_YStg) ;
                  m_dLastWorkPosT = MT_GetCmdPos(miWRD_TStg) ;

                  //IO_SetY(yEJT_Suction,true);

                  Step.iCycle = 0 ;
                  return true ;
        }
}

bool CStage::CycleStgVision(void)
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

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true;
    //}

    int r,c ;

    static int    iInspCnt  = 0 ; //3X3 = 9      ''
    const  int    iMaxInspCnt = 2 ;
    double dMoveX , dMoveY , dMoveT ;

    static int iVisnFailCnt = 0 ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;
        case  10: MoveMotr(miWRD_ZExd , piWRD_ZExdExpend);
                  MoveActr(aiEJT_UpDn , ccFwd);

                  iInspCnt = 0 ;


                  Step.iCycle++;
                  return false ;

        case  11:
                  if(!MoveMotr(miWRD_ZExd , piWRD_ZExdExpend)) return false ;
                  if(!MoveActr(aiEJT_UpDn , ccFwd)) return false ;
                  MoveActr(aiEJT_UpDn,ccFwd);
                  Step.iCycle++;
                  return false ;

        //�ؿ��� ���.
        case  12: if(!MoveActr(aiEJT_UpDn,ccFwd)) return false ;
                  //IO_SetY(yEJT_Suction , true); ������ ������ Ʋ����.
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false ;


        case  13: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun)) return false;
                  if(!OM.MstOptn.bDryRun) {
                      SV_GetRslt(viWfr , &RsltWfr);

                      //GetInspEnd == 0 //�˰��� �˻�
                      //Rslt.bRslt == 1 //�˻� ����
                      //Rslt.bRslt == 0 //�˻� ����
                      //Rslt.iRet  == 0:Pass , 1:RCT Fail , 2: Nodevice or Crack 3: FailDevice 4: Std Device
                  }
                  else {
                      //RsltWfr.bRslt  = true ;
                      RsltWfr.iRet   = 0    ;
                      RsltWfr.fRsltX = 0.0  ;
                      RsltWfr.fRsltY = 0.0  ;
                      RsltWfr.fRsltT = 0.0  ;

                  }

                  if(RsltWfr.iRet == veWfrRctFail || RsltWfr.iRet == veWfrRct){ //����� �˻� �ȉ�.
                      Step.iCycle++;
                      return false ;
                  }

                  Trace("WAFER VISION",("X=" + AnsiString(RsltWfr.fRsltX)).c_str() );
                  Trace("WAFER VISION",("Y=" + AnsiString(RsltWfr.fRsltY)).c_str() );
                  Trace("WAFER VISION",("T=" + AnsiString(RsltWfr.fRsltT)).c_str() );

                  if(!VisnRsltLmtCheck(&RsltWfr)) {
                      EM_SetErr(eiSTG_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }
                  //������.
                  GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                               RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                               dMoveX                   , dMoveY                   , dMoveT                   ,
                               OM.CmnOptn.bUseStgTheta );  //��Ÿ���� �Ⱦ�.

//                  if( fabs(RsltWfr.fRsltX) > (OM.DevInfo.dWf_PitchX)/2 ||
//                      fabs(RsltWfr.fRsltY) > (OM.DevInfo.dWf_PitchY)/2 ){
//                      EM_SetErr(eiSTG_WfrVsThchFail);
//                  }
//                  if( fabs(RsltWfr.fRsltT) > OM.CmnOptn.dStageTLmtErr) {
//                      EM_SetErr(eiSTG_TLmtErr);
//                  }

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  iInspCnt++;

                  //���׽�Ʈ���� �ѹ� �� ����.
                  if(!OM.MstOptn.bDryRun && OM.CmnOptn.bWfrVisnRetest && iInspCnt < iMaxInspCnt && RsltWfr.iRet  == 0){
                      Step.iCycle = 12 ;
                      return false ;

                  }

                  Step.iCycle++;
                  return false ;

        case  14: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;

                  m_dLastWorkPosX = MT_GetCmdPos(miWRD_XStg) ;
                  m_dLastWorkPosY = MT_GetCmdPos(miWRD_YStg) ;
                  m_dLastWorkPosT = MT_GetCmdPos(miWRD_TStg) ;

                  //veWfrOk       = 0 ,
                  //veWfrRctFail  = 1 ,
                  //veWfrRct      = 2 ,
                  //veWfrAngle    = 3 ,
                  //veWfrCrack    = 4 ,
                  //veWfrTsdFail  = 5 ,
                  //veWfrDot      = 6 ,
                  //veWfrNoPkg    = 7 ,
                  if(RsltWfr.iRet == veWfrOk ) { //��...
                      iVisnFailCnt = 0 ; //�簢�� ã�°� �˻� ����ī��Ʈ ���� ����.
                      FindChip(riSTG,r,c,csWait);
                      //DM.ARAY[riSTG].SetStat(r,c,csGood);
                      if(OM.CmnOptn.bUseMapFile) {
                          DM.ARAY[riSTG].SetStat(r,c,csGood);
                      }
                      else{
                          if(!OM.CmnOptn.bInkDieWork) {
                              DM.ARAY[riSTG].SetStat(r,c,csGood);
                          }
                          else {  //��ũ���� �۾� ��� �϶�.
                              DM.ARAY[riSTG].SetStat(r,c,csUnkwn );
                          }
                      }
                      IO_SetY(yEJT_Suction,true);
                  }
                  else if(RsltWfr.iRet == veWfrRct || RsltWfr.iRet == veWfrRctFail || RsltWfr.iRet == veWfrAngle) { //��Ʈ ���� ����.
                      iVisnFailCnt++; //�簢�� �� ���� ���� �˻�ȵȰ�.
                      FindChip(riSTG,r,c,csWait);
                      if(OM.CmnOptn.bUseMapFile) DM.ARAY[riSTG].SetStat(r,c,csEror  ); //�ʿ� ǥ��.
                      else                       DM.ARAY[riSTG].SetStat(r,c,csUnkwn ); //�׳� ������������ �̵�.
                  }
                  else if(RsltWfr.iRet == veWfrNoPkg) {  //�������� or Crack90���� �̻�����.
                      iVisnFailCnt = 0 ; //�簢�� ���� ���� �˻�ȵȰ��� �ƴ�.
                      FindChip(riSTG,r,c,csWait);
                      if(OM.CmnOptn.bUseMapFile) {
                          DM.ARAY[riSTG].SetStat(r,c,csEmpty );
                          //DM.ARAY[riSTG].SetStat(r,c,csGood  );
                      }
                      else  DM.ARAY[riSTG].SetStat(r,c,csUnkwn );
                  }
                  else if(RsltWfr.iRet == veWfrCrack) {  //Crack����.
                      iVisnFailCnt = 0 ;
                      FindChip(riSTG,r,c,csWait);
                      if(OM.CmnOptn.bUseMapFile) {
                          //DM.ARAY[riSTG].SetStat(r,c,csEmpty );
                          DM.ARAY[riSTG].SetStat(r,c,csGood  ); //ũ���� �ϴ� �۾��ϴ°�����.
                      }
                      else  DM.ARAY[riSTG].SetStat(r,c,csUnkwn );
                  }
                  else if(RsltWfr.iRet == veWfrTsdFail || RsltWfr.iRet == veWfrDot) { //��Ʈ �˻� ���� ����.
                      iVisnFailCnt = 0 ;
                      FindChip(riSTG,r,c,csWait);
                      if(OM.CmnOptn.bUseMapFile) {
                          //DM.ARAY[riSTG].SetStat(r,c,csGood ); //�ʸ���϶��� �Ű� �Ⱦ��� ��´�.
                          DM.ARAY[riSTG].SetStat(r,c,csFail );   //�׶�������� ����� �ش޶���.
                      }
                      else {
                          if(!OM.CmnOptn.bInkDieWork) {
                              DM.ARAY[riSTG].SetStat(r,c,csUnkwn );
                          }
                          else {
                              DM.ARAY[riSTG].SetStat(r,c,csGood );
                          }

                      }
                  }


                  if(OM.DevOptn.iWfrVsFailCnt && iVisnFailCnt >= OM.DevOptn.iWfrVsFailCnt ) {
                      EM_SetErr(eiSTG_WfrVsFailCnt);
                      iVisnFailCnt = 0 ;
                      Step.iCycle = 0 ;
                      return true ;
                  }



                  //IO_SetY(yEJT_Suction,true);
                  Step.iCycle = 0 ;
                  return true ;
        }
}
/*
bool CStage::CycleStgWork(void)
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

        case  10: IO_SetY(yEJT_Suction,false);
                  MoveMotr(miWRD_XTrs,piWRD_XTrsWait  );
                  MoveMotr(miWRD_ZExd,piWRD_ZExdExpend);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miWRD_XTrs,piWRD_XTrsWait  )) return false ;
                  if(!MoveMotr(miWRD_ZExd,piWRD_ZExdExpend)) return false ;
                  MoveMotr(miWRD_XStg,piWRD_XStgWork);
                  MoveMotr(miWRD_YStg,piWRD_YStgWork);
                  MoveMotr(miWRD_TStg,piWRD_TStgWork);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miWRD_XStg,piWRD_XStgWork)) return false ;
                  if(!MoveMotr(miWRD_YStg,piWRD_YStgWork)) return false ;
                  if(!MoveMotr(miWRD_TStg,piWRD_TStgWork)) return false ;
                  MoveActr(aiEJT_UpDn,ccFwd);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveActr(aiEJT_UpDn,ccFwd)) return false ;
                  IO_SetY(yEJT_Suction,true);
                  Step.iCycle = 0 ;
                  return true ;
    }
}*/

bool CStage::CycleCstPlace(void)
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

        case  10: m_bWfrTAligned = false ;
                  m_bStartSet = false;

                  IO_SetY(yEJT_Suction,false);
                  MoveActr(aiEJT_UpDn,ccBwd);
                  //MoveActr(aiTRS_UpDn , ccFwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiEJT_UpDn,ccBwd)) return false ;
                  //if(!MoveActr(aiTRS_UpDn , ccFwd))return false ;
                  MoveMotr(miWRD_XStg,piWRD_XStgCassette);
                  MoveMotr(miWRD_YStg,piWRD_YStgCassette);
                  MoveMotr(miWRD_TStg,piWRD_TStgCassette);
                  MoveMotr(miWRD_ZExd,piWRD_ZExdGet );






                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miWRD_XStg,piWRD_XStgCassette)) return false ;
                  if(!MoveMotr(miWRD_YStg,piWRD_YStgCassette)) return false ;
                  if(!MoveMotr(miWRD_TStg,piWRD_TStgCassette)) return false ;
                  if(!MoveMotr(miWRD_ZExd,piWRD_ZExdGet )) return false ;
                  MoveMotr(miWRD_ZCst,piWRD_ZCstOut      );
                  MoveActr(aiTRS_Chuck,ccBwd             );
                  MoveActr(aiTRS_UpDn , ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miWRD_ZCst,piWRD_ZCstOut      )) return false ;
                  if(!MoveActr(aiTRS_Chuck,ccBwd             )) return false ;
                  if(!MoveActr(aiTRS_UpDn , ccFwd))             return false ;
                  MoveMotr(miWRD_XTrs ,piWRD_XTrsPlace   ); //�� �̷��� �ϴ� �ϰ� Pick ���� �ٲٴ���.
                  Step.iCycle++;
                  return false;

        //���� ReqStop�ÿ� �� ��ȣ ����.
        case  14: if(!MoveMotr(miWRD_XTrs,piWRD_XTrsPlace    )) return false ;
//                  MoveActr(aiTRS_Chuck,ccFwd); //������븮 ��û. �Ĺ�
                  Step.iCycle++;
                  return false;

        case  15: //if(!MoveActr(aiTRS_Chuck,ccFwd)) return false ;
                  if(!OM.MstOptn.bDryRun && !IO_GetX(xTRS_Wafer)){
                      EM_SetErr(eiSTG_WaferNotDetected);
                      MoveMotr(miWRD_XTrs,piWRD_XTrsWait);
                      Step.iCycle = 0 ;
                      return true;
                  }
                  MoveMotr(miWRD_XTrs,piWRD_XTrsPick);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miWRD_XTrs,piWRD_XTrsPick)) return false ;
                  //äũ���������� �ɸ�.
                  //MoveMotr(miWRD_ZCst,piWRD_ZCstWork);
                  MT_GoIncRun(miWRD_ZCst , CST_OFFSET);

                  //MoveActr(aiTRS_Chuck,ccBwd);
                  Step.iCycle++;
                  return false;

        case  17: //if(!MoveActr(aiTRS_Chuck,ccBwd)) return false ;
                  //if(!MoveMotr(miWRD_ZCst,piWRD_ZCstWork)) return false ;
                  if(!MT_GetStopInpos(miWRD_ZCst)) return false ;
                  FindChip(riCST,r,c,csMask);
                  DM.ARAY[riCST].SetStat(r,0,csWork);
                  DM.ARAY[riSTG].SetStat(csNone);

                  if(MM.GetManNo() == mcSTG_CycleCsPlace || OM.CmnOptn.bStageStop) MoveMotr(miWRD_XTrs,piWRD_XTrsWait    );
                  else                                    MoveMotr(miWRD_XTrs,piWRD_XTrsPickWait);
                  //                                      MoveMotr(miWRD_XTrs,piWRD_XTrsPickWait);
                  Step.iCycle++;
                  return false;

        case  18: //if(!MoveMotr(miWRD_XTrs,piWRD_XTrsPickWait)) return false ;
                  if(MM.GetManNo() == mcSTG_CycleCsPlace || OM.CmnOptn.bStageStop) {if(!MoveMotr(miWRD_XTrs,piWRD_XTrsWait    )) return false ;}
                  else                                    {if(!MoveMotr(miWRD_XTrs,piWRD_XTrsPickWait)) return false ;}


                  if(!MT_CmprPos(miWRD_XTrs , PM.GetValue(miWRD_XTrs , pvWRD_XTrsWait))) {
                      Step.iCycle = 0 ;
                      return true ;

                  }


                  if(DM.ARAY[riCST].GetCntStat(csUnkwn) == 0) MoveMotr(miWRD_ZCst ,piWRD_ZCstWait);

        case  19: if(!MT_GetStop(miWRD_ZCst)) return false ;
                  Step.iCycle = 0 ;
                  return true ;
    }
}

bool CStage::CycleCenterMove(void) //Manual
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

    //if(Stat.bReqStop) {
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

        case  10: IO_SetY(yEJT_Suction , false);
                  MoveMotr(miWRD_ZExd , piWRD_ZExdExpend);
                  MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait  );
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveMotr(miWRD_ZExd , piWRD_ZExdExpend)) return false ;
                  if(!MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait  )) return false ;
                  //MoveActr(aiEJT_UpDn,ccFwd);
                  Step.iCycle++;
                  return false ;

        case  12: //if(!MoveActr(aiEJT_UpDn,ccFwd)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false ;

        case  13: if(!CycleVisn(viWfr , false , OM.MstOptn.bDryRun))return false;
                  SV_GetRslt(viWfr , &RsltWfr);

                  if(RsltWfr.iRet == veWfrRctFail || RsltWfr.iRet == veWfrRct){ //����� �˻� �ȉ�.
                      Step.iCycle++;
                      return false ;
                  }

                  if(!VisnRsltLmtCheck(&RsltWfr)) {
                      EM_SetErr(eiSTG_VisnRlstOver);
                      Step.iCycle = 0 ;
                      return true ;
                  }

                  //������.
                  if(MM.GetManNo() == mcSTG_CycleCenterMoveT) {
                      GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                                   RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                                   dMoveX                   , dMoveY                   , dMoveT                   ,
                                   true );  //

                  }
                  else {
                      GetAdjustPos(MT_GetCmdPos(miWRD_XStg) , MT_GetCmdPos(miWRD_YStg) , MT_GetCmdPos(miWRD_TStg) ,
                                   RsltWfr.fRsltX           , RsltWfr.fRsltY           , RsltWfr.fRsltT           ,
                                   dMoveX                   , dMoveY                   , dMoveT                   ,
                                   false );  //��Ÿ���� �Ⱦ�.

                  }

                  MT_GoAbsRun(miWRD_XStg , dMoveX);
                  MT_GoAbsRun(miWRD_YStg , dMoveY);
                  MT_GoAbsRun(miWRD_TStg , dMoveT);

                  Step.iCycle++;
                  return false ;

        case  14: if(!MT_GetStopInpos(miWRD_XStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_YStg)) return false ;
                  if(!MT_GetStopInpos(miWRD_TStg)) return false ;
                  CycleVisn(viWfr , true , OM.MstOptn.bDryRun);
                  Step.iCycle++;
                  return false;

        case  15: if(!CycleVisn(viWfr , false, OM.MstOptn.bDryRun)) return false;

                  Step.iCycle = 0 ;
                  return true ;
        }
}



bool CStage::CycleUnExpend(void)
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

    //if(Stat.bReqStop) {
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

        case  10: MoveMotr(miWRD_ZExd , piWRD_ZExdExpend);
                  MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait  );
                  MoveActr(aiTRS_UpDn , ccBwd );
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveMotr(miWRD_ZExd , piWRD_ZExdExpend)) return false ;
                  if(!MoveMotr(miWRD_ZEjt , piWRD_ZEjtWait  )) return false ;
                  if(!MoveActr(aiTRS_UpDn , ccBwd )) return false ;
                  MoveActr(aiEJT_UpDn,ccBwd);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveActr(aiEJT_UpDn,ccBwd)) return false ;
                  MoveMotr(miWRD_XStg , piWRD_XStgCassette);
                  MoveMotr(miWRD_YStg , piWRD_YStgCassette);
                  MoveMotr(miWRD_TStg , piWRD_TStgCassette);
                  Step.iCycle++;
                  return false ;

        case  13: if(!MoveMotr(miWRD_XStg , piWRD_XStgCassette)) return false ;
                  if(!MoveMotr(miWRD_YStg , piWRD_YStgCassette)) return false ;
                  if(!MoveMotr(miWRD_TStg , piWRD_TStgCassette)) return false ;
                  MoveMotr(miWRD_ZExd , piWRD_ZExdGet);
                  Step.iCycle++;
                  return false ;

        case  14: if(!MoveMotr(miWRD_ZExd , piWRD_ZExdGet)) return false ;
                  //m_bStartSet   = false ;
                  //m_bWfrTAligned = false ;

                  Step.iCycle = 0 ;
                  return true ;
        }




}







void CStage::SetLastCmd()
{
    return ; //����Ʈ�� �ʿ� ����.

}

bool CStage::CheckMoved()
{
    return true ; //����Ʈ�� �ʿ� ����.
}

bool CStage::CheckStop()
{
    if(!MT_GetStop(miWRD_XStg)) return false ;
    if(!MT_GetStop(miWRD_YStg)) return false ;
    if(!MT_GetStop(miWRD_TStg)) return false ;
    if(!MT_GetStop(miWRD_XTrs)) return false ;
    if(!MT_GetStop(miWRD_ZCst)) return false ;
    if(!MT_GetStop(miWRD_ZExd)) return false ;

    if(!AT_Done(aiEJT_UpDn )) return false ;

    return true ;
}

void CStage::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() + ".INI";

    if ( IsLoad ) {
        UserINI.Load(sPath, "Member" , "m_dLastWorkPosX" , m_dLastWorkPosX );
        UserINI.Load(sPath, "Member" , "m_dLastWorkPosY" , m_dLastWorkPosY );
        UserINI.Load(sPath, "Member" , "m_dLastWorkPosT" , m_dLastWorkPosT );
        UserINI.Load(sPath, "Member" , "m_iLastPickRow " , m_iLastPickRow  );
        UserINI.Load(sPath, "Member" , "m_iLastPickCol " , m_iLastPickCol  );
        UserINI.Load(sPath, "Member" , "m_bStartSet    " , m_bStartSet     );
        UserINI.Load(sPath, "Member" , "m_bWfrTAligned " , m_bWfrTAligned  );

    }
    else {
        UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath, "Member" , "m_dLastWorkPosX" , m_dLastWorkPosX );
        UserINI.Save(sPath, "Member" , "m_dLastWorkPosY" , m_dLastWorkPosY );
        UserINI.Save(sPath, "Member" , "m_dLastWorkPosT" , m_dLastWorkPosT );
        UserINI.Save(sPath, "Member" , "m_iLastPickRow " , m_iLastPickRow  );
        UserINI.Save(sPath, "Member" , "m_iLastPickCol " , m_iLastPickCol  );
        UserINI.Save(sPath, "Member" , "m_bStartSet    " , m_bStartSet     );
        UserINI.Save(sPath, "Member" , "m_bWfrTAligned " , m_bWfrTAligned  );
    }
}
//---------------------------------------------------------------------------



