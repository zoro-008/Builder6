//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "PostBuff.h"
#include "Loader.h"
#include "UnLoader.h"
#include "WorkZone.h"
//---------------------------------------------------------------------------
#include "Rs232Man.h"
//---------------------------------------------------------------------------
#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "UserIni.h"
#include "ProbeComUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

#define PSBBackPos 20
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CPostBuff PSB;
//---------------------------------------------------------------------------
CPostBuff::CPostBuff(void)
{

}
CPostBuff::~CPostBuff (void)
{

}

void CPostBuff::Init()
{
    m_sPartName = "PostBuff " ;
    Reset();
    Load(true);

    InitCycleName();
}
void CPostBuff::Close()
{
    Load(false);
}
//---------------------------------------------------------------------------
void CPostBuff::Update()
{
}
void CPostBuff::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}
//---------------------------------------------------------------------------
void CPostBuff::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmDelay  .Clear();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
bool CPostBuff::FindChip(int &r , int &c , EN_CHIP_STAT Stat)
{
    DM.ARAY[riPSB].FindLastColFrstRow(Stat, r, c) ;
    return (c > -1 && r > -1);
}
//---------------------------------------------------------------------------
bool CPostBuff::GetReadyPos()
{
//    double dv1,dv2,dv3;
//    AnsiString sv1, sv2;
////    dv1 = MT_GetCmdPos(miPSB_XCmp);
//    dv1 = MT_GetCmdPos(miPSB_XCmp);
//    dv2 = GetMotrPos(miPSB_XCmp , piPSB_XWork , Mode);
//    dv3 = MT_GetEncPos(miPSB_XCmp);
//
//    bool bRet = MT_CmprPos(miPSB_XCmp , dv2);
//
//    sv1.sprintf("%0.3f",dv1);
//    sv2.sprintf("%0.3f",dv2);
//
//    return bRet ;
//    이상하다 dXCmdPos == dFailPos 일때 같지 않다고 리턴됨.

    double dXCmdPos    = MT_GetCmdPos(miPSB_XIdx);                            //명령 포지션
    double dXEncPos    = MT_GetEncPos(miPSB_XIdx);                            //엔코더 포지션
    double dXInpos     = 0.1 ;                                                //넉넉하게 메뉴얼로 삽입한 인포지션.
    double dFailPos    = GetMotrPos(miPSB_XIdx , piPSB_XRWork) - PSBBackPos;          //페일자제가 있는 포지션
    bool   bXInposStop = (abs(dXCmdPos - dXEncPos) < dXInpos) ;               //메뉴얼로 삽입한 인포지션 적용한 인포지션 스탑 확인.

    //bool   bNeedMove   = bXInposStop && dXCmdPos != dFailPos ;                //인포지션스탑이고 최종명령했던 포지션과 페일자제 있는 포지션이 다를 경우 이동필요.
    bool   bNeedMove   = bXInposStop && abs(dXCmdPos - dFailPos) > dXInpos ;
    //bool   bReadyPos   = bXInposStop && abs(dXCmdPos - dFailPos)> dXInpos ;     //이상하다 dXCmdPos == dFailPos 일때 같지 않다고 리턴됨.

    return bNeedMove ;
    ////////////////////////////////////////////////////////////////////////
}

double  CPostBuff::RejectMove()
{
    if(!OM.MstOptn.bUsedBinSorting){
        if((DM.ARAY[riPSB].GetLotNo() != m_sLastStrip))
        {
            m_iBinCnt++;
            if(m_iBinCnt >= MAXBIN)   m_iBinCnt = 0;
            m_sLastStrip = DM.ARAY[riPSB].GetLotNo();
        }

        m_dFailChipPos = PM.GetValue(miREJ_XBtm , pvREJ_XBWorkStt) + (OM.DevOptn.dREJPitch * m_iBinCnt) ;

        return m_dFailChipPos;
    }
    else{
        m_iBinCnt = 10;
        int Row, Row1, Row2 = 0;
        int Col, Col1, Col2 = 0;
        double dRlstVf = 0.0;
        double dRlstVr = 0.0;

        FindChip(Row1 , Col1 , csFail);

        dRlstVf = DM.ARAY[riPSB].GetVf(0,0);
        dRlstVf = DM.ARAY[riPSB].GetVf(Row1, Col1);
        dRlstVr = DM.ARAY[riPSB].GetVr(Row1, Col1);

        if(OM.DevOptn.bUseZener){
            if(OM.CmnOptn.bUsedBin1 ){      if((OM.CmnOptn.dMinVf1 <= 0.0     && OM.CmnOptn.dMaxVf1 <= 0.0     ) && (OM.CmnOptn.dMinVr1  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr1 )) m_iBinCnt = 0;
                                       else if((OM.CmnOptn.dMinVf1 <= 0.0     && OM.CmnOptn.dMaxVf1 <= 0.0     ) && (OM.CmnOptn.dMinVr1  <= 0.0     && 0.0     <= OM.CmnOptn.dMaxVr1 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf1 <= dRlstVf && OM.CmnOptn.dMaxVf1 >= dRlstVf ) m_iBinCnt = 0; }
            if(OM.CmnOptn.bUsedBin2 ){      if((OM.CmnOptn.dMinVf2 <= 0.0     && OM.CmnOptn.dMaxVf2 <= 0.0     ) && (OM.CmnOptn.dMinVr2  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr2 )) m_iBinCnt = 1;
                                       else if((OM.CmnOptn.dMinVf2 <= 0.0     && OM.CmnOptn.dMaxVf2 <= 0.0     ) && (OM.CmnOptn.dMinVr2  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr2 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf2 <= dRlstVf && OM.CmnOptn.dMaxVf2 >= dRlstVf ) m_iBinCnt = 1; }
            if(OM.CmnOptn.bUsedBin3 ){      if((OM.CmnOptn.dMinVf3 <= 0.0     && OM.CmnOptn.dMaxVf3 <= 0.0     ) && (OM.CmnOptn.dMinVr3  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr3 )) m_iBinCnt = 2;
                                       else if((OM.CmnOptn.dMinVf3 <= 0.0     && OM.CmnOptn.dMaxVf3 <= 0.0     ) && (OM.CmnOptn.dMinVr3  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr3 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf3 <= dRlstVf && OM.CmnOptn.dMaxVf3 >= dRlstVf ) m_iBinCnt = 2; }
            if(OM.CmnOptn.bUsedBin4 ){      if((OM.CmnOptn.dMinVf4 <= 0.0     && OM.CmnOptn.dMaxVf4 <= 0.0     ) && (OM.CmnOptn.dMinVr4  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr4 )) m_iBinCnt = 3;
                                       else if((OM.CmnOptn.dMinVf4 <= 0.0     && OM.CmnOptn.dMaxVf4 <= 0.0     ) && (OM.CmnOptn.dMinVr4  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr4 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf4 <= dRlstVf && OM.CmnOptn.dMaxVf4 >= dRlstVf ) m_iBinCnt = 3; }
            if(OM.CmnOptn.bUsedBin5 ){      if((OM.CmnOptn.dMinVf5 <= 0.0     && OM.CmnOptn.dMaxVf5 <= 0.0     ) && (OM.CmnOptn.dMinVr5  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr5 )) m_iBinCnt = 4;
                                       else if((OM.CmnOptn.dMinVf5 <= 0.0     && OM.CmnOptn.dMaxVf5 <= 0.0     ) && (OM.CmnOptn.dMinVr5  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr5 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf5 <= dRlstVf && OM.CmnOptn.dMaxVf5 >= dRlstVf ) m_iBinCnt = 4; }
            if(OM.CmnOptn.bUsedBin6 ){      if((OM.CmnOptn.dMinVf6 <= 0.0     && OM.CmnOptn.dMaxVf6 <= 0.0     ) && (OM.CmnOptn.dMinVr6  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr6 )) m_iBinCnt = 5;
                                       else if((OM.CmnOptn.dMinVf6 <= 0.0     && OM.CmnOptn.dMaxVf6 <= 0.0     ) && (OM.CmnOptn.dMinVr6  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr6 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf6 <= dRlstVf && OM.CmnOptn.dMaxVf6 >= dRlstVf ) m_iBinCnt = 5; }
            if(OM.CmnOptn.bUsedBin7 ){      if((OM.CmnOptn.dMinVf7 <= 0.0     && OM.CmnOptn.dMaxVf7 <= 0.0     ) && (OM.CmnOptn.dMinVr7  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr7 )) m_iBinCnt = 6;
                                       else if((OM.CmnOptn.dMinVf7 <= 0.0     && OM.CmnOptn.dMaxVf7 <= 0.0     ) && (OM.CmnOptn.dMinVr7  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr7 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf7 <= dRlstVf && OM.CmnOptn.dMaxVf7 >= dRlstVf ) m_iBinCnt = 6; }
            if(OM.CmnOptn.bUsedBin8 ){      if((OM.CmnOptn.dMinVf8 <= 0.0     && OM.CmnOptn.dMaxVf8 <= 0.0     ) && (OM.CmnOptn.dMinVr8  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr8 )) m_iBinCnt = 7;
                                       else if((OM.CmnOptn.dMinVf8 <= 0.0     && OM.CmnOptn.dMaxVf8 <= 0.0     ) && (OM.CmnOptn.dMinVr8  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr8 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf8 <= dRlstVf && OM.CmnOptn.dMaxVf8 >= dRlstVf ) m_iBinCnt = 7; }
            if(OM.CmnOptn.bUsedBin9 ){      if((OM.CmnOptn.dMinVf9 <= 0.0     && OM.CmnOptn.dMaxVf9 <= 0.0     ) && (OM.CmnOptn.dMinVr9  <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr9 )) m_iBinCnt = 8;
                                       else if((OM.CmnOptn.dMinVf9 <= 0.0     && OM.CmnOptn.dMaxVf9 <= 0.0     ) && (OM.CmnOptn.dMinVr9  <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr9 )) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf9 <= dRlstVf && OM.CmnOptn.dMaxVf9 >= dRlstVf ) m_iBinCnt = 8; }
            if(OM.CmnOptn.bUsedBin10){      if((OM.CmnOptn.dMinVf10<= 0.0     && OM.CmnOptn.dMaxVf10<= 0.0     ) && (OM.CmnOptn.dMinVr10 <= dRlstVr && dRlstVr <= OM.CmnOptn.dMaxVr10)) m_iBinCnt = 9;
                                       else if((OM.CmnOptn.dMinVf10<= 0.0     && OM.CmnOptn.dMaxVf10<= 0.0     ) && (OM.CmnOptn.dMinVr10 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVr10)) m_iBinCnt = OM.CmnOptn.iBinDefaultNo;
                                       else if( OM.CmnOptn.dMinVf10<= dRlstVf && OM.CmnOptn.dMaxVf10>= dRlstVf ) m_iBinCnt = 9; }
        }

        else{
            if(OM.CmnOptn.bUsedBin1 ){      if(OM.CmnOptn.dMinVf1 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf1 ) m_iBinCnt = 0;
                                       else if(OM.CmnOptn.dMinVf1 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf1 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin2 ){      if(OM.CmnOptn.dMinVf2 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf2 ) m_iBinCnt = 1;
                                       else if(OM.CmnOptn.dMinVf2 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf2 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin3 ){      if(OM.CmnOptn.dMinVf3 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf3 ) m_iBinCnt = 2;
                                       else if(OM.CmnOptn.dMinVf3 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf3 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin4 ){      if(OM.CmnOptn.dMinVf4 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf4 ) m_iBinCnt = 3;
                                       else if(OM.CmnOptn.dMinVf4 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf4 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin5 ){      if(OM.CmnOptn.dMinVf5 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf5 ) m_iBinCnt = 4;
                                       else if(OM.CmnOptn.dMinVf5 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf5 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin6 ){      if(OM.CmnOptn.dMinVf6 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf6 ) m_iBinCnt = 5;
                                       else if(OM.CmnOptn.dMinVf6 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf6 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin7 ){      if(OM.CmnOptn.dMinVf7 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf7 ) m_iBinCnt = 6;
                                       else if(OM.CmnOptn.dMinVf7 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf7 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin8 ){      if(OM.CmnOptn.dMinVf8 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf8 ) m_iBinCnt = 7;
                                       else if(OM.CmnOptn.dMinVf8 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf8 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin9 ){      if(OM.CmnOptn.dMinVf9 <= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf9 ) m_iBinCnt = 8;
                                       else if(OM.CmnOptn.dMinVf9 <= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf9 ) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
            if(OM.CmnOptn.bUsedBin10){      if(OM.CmnOptn.dMinVf10<= dRlstVf && dRlstVf <= OM.CmnOptn.dMaxVf10) m_iBinCnt = 9;
                                       else if(OM.CmnOptn.dMinVf10<= 0.0     && 0.0     >= OM.CmnOptn.dMaxVf10) m_iBinCnt = OM.CmnOptn.iBinDefaultNo; }
        }
        if(!OM.CmnOptn.bUsedBin1 && !OM.CmnOptn.bUsedBin2 &&
           !OM.CmnOptn.bUsedBin3 && !OM.CmnOptn.bUsedBin4 &&
           !OM.CmnOptn.bUsedBin5 && !OM.CmnOptn.bUsedBin6 &&
           !OM.CmnOptn.bUsedBin7 && !OM.CmnOptn.bUsedBin8 &&
           !OM.CmnOptn.bUsedBin9 && !OM.CmnOptn.bUsedBin10)
        m_iBinCnt = OM.CmnOptn.iBinDefaultNo;

        if(m_iBinCnt == 10){
            m_iBinCnt = 0;
        }


        m_dFailChipPos = PM.GetValue(miREJ_XBtm , pvREJ_XBWorkStt) + (OM.DevOptn.dREJPitch * m_iBinCnt);

        return m_dFailChipPos;
    }
}
//---------------------------------------------------------------------------
double CPostBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    int  r ,c , iColGrCnt , iMaxCol, iMaxRow ;
    int fc, fr;
    int    iGrCnt ;
    bool   bGrGrip;
    iMaxCol = DM.ARAY[riPSB].GetMaxCol();
    iMaxRow = DM.ARAY[riPSB].GetMaxRow();

    FindChip(r , c , csFail);

    fc = (iMaxCol - c) -1 ; //횡으로 플립된 값.
    fr = iMaxRow - r - 1 ;  // 맨밑이 워크 스타트 포지션으로 바꿈.

    double dTrimColptch = OM.DevInfo.dColPitch + OM.DevOptn.dTrimPtchOfs ;

    double dY1Pos, dX1Pos;
    //X , Y Pos Setting.
    //dY1Pos  = PM.GetValue(miSTL_YTop , pvSTL_YTWorkStart) + (OM.DevInfo.dRowPitch * fr) + (OM.DevOptn.dTrimPtchOfs * fr);
    dY1Pos = PM.GetValue(miSTL_YTop , pvSTL_YTWorkStart) + (OM.DevInfo.dRowPitch * fr) + (OM.DevOptn.dTrimPtchOfs * r);

    int iRowGrCnt = (fr) / OM.DevInfo.iRowGrCnt ;
    int iRowSubGrCnt = (fr) / OM.DevInfo.iRowSubGrCnt ;

    if(OM.DevInfo.iRowGrCnt > 1)
    {
        dY1Pos += (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt;
    }

    if(OM.DevInfo.iRowSubGrCnt > 1)
    {
        dY1Pos += (OM.DevInfo.dRowSubGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt ;
    }
    //if( OM.DevInfo.iRowGrCnt > 1 || OM.DevInfo.iRowSubGrCnt > 1) {
    //    //iRowGrCnt는 그룹의 갯수. OM.DevInfo.iRowGrCnt이거는 한그룹당 LED 갯수.
    //    int iRowGrCnt = r / OM.DevInfo.iRowGrCnt ;
    //    int iRowSubGrCnt = r / OM.DevInfo.iRowSubGrCnt ;
    //    if(iRowGrCnt > 0 && iRowSubGrCnt > 1)  dY1Pos += (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt -
    //                                                     (OM.DevInfo.dRowSubGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt ;
    //    if(iRowGrCnt > 0 && iRowSubGrCnt <= 1) dY1Pos += (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * iRowGrCnt ;
    //    if(iRowSubGrCnt > 1)                   dY1Pos += (OM.DevInfo.dRowSubGrGap - OM.DevInfo.dRowPitch) * iRowSubGrCnt ;
    //}

    double dPos       = 0.0 ;
    if(OM.DevInfo.iColGrCnt != 0) dPos = PM.GetValue(miPSB_XIdx , pvPSB_XRWorkStt) + (dTrimColptch * fc) + (fc / OM.DevInfo.iColGrCnt * OM.DevInfo.dColGrGap);
    else                          dPos = PM.GetValue(miPSB_XIdx , pvPSB_XRWorkStt) + (dTrimColptch * fc);

    if(_iMotr == miPSB_XIdx){
        switch(_iPstnId) {   //Pull은 첫번째 그립에서 Drag 할때 가지고 가는 포지션 Drag는 두번째 그립 하고 가져 가는 포지션.
            default               : dPos = MT_GetCmdPos(_iMotr                   ); break ;
            case piPSB_XWait      : dPos = PM.GetValue (_iMotr , pvPSB_XWait     ); break ;
            case piPSB_XLIn       : dPos = PM.GetValue (_iMotr , pvPSB_XLIn      ); break ;
            case piPSB_XLPush     : dPos = PM.GetValue (_iMotr , pvPSB_XLPush    ); break ; //CycleIn에서 처음 자제를 끌고 가는 위치.
            case piPSB_XRGet      : dPos = PM.GetValue (_iMotr , pvPSB_XRGet     ); break ;
            case piPSB_XRWorkStt  : dPos = PM.GetValue (_iMotr , pvPSB_XRWorkStt ); break ;
            case piPSB_XRWork     : dPos = dPos ;                                   break ;
            case piPSB_XROut      : dPos = PM.GetValue (_iMotr , pvPSB_XROut     ); break ;//WorkZone에서 사용하는 포지션.
        }
    }
    else if(_iMotr == miREJ_XBtm){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                           ); break ;
            case piREJ_XBWait        : dPos = PM.GetValue (_iMotr , pvREJ_XBWait            ); break ;
            case piREJ_XBWorkStt     : dPos = PM.GetValue (_iMotr , pvREJ_XBWorkStt         ); break ;
            case piREJ_XBWork        : dPos = RejectMove()                                   ; break ;
        }
    }
    else if(_iMotr == miSTL_YTop){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                           ); break ;
            case piSTL_YTWait        : dPos = PM.GetValue (_iMotr , pvSTL_YTWait            ); break ;
            case piSTL_YTWorkStart   : dPos = PM.GetValue (_iMotr , pvSTL_YTWorkStart       ); break ;
            case piSTL_YTWork        : dPos = dY1Pos                                         ; break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}
//---------------------------------------------------------------------------
bool CPostBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    bool bRet = true ;
    AnsiString sMsg ;

    bool isDrngPunch  =  AT_Complete(aiSTL_Top , ccFwd) ;

    bool isXCmpMove   = !MT_GetStopInpos(miPSB_XIdx);
    bool isYBtmFwd    = !AT_Complete(aiSTL_Btm , ccBwd);

    if(_iMotr == miPSB_XIdx){
        if(isDrngPunch) {sMsg = "During Punching"; bRet = false ;}
        if(isYBtmFwd  ) {sMsg = (AnsiString)AT_GetName(aiSTL_Btm) + " is Fwd"; bRet = false ; }
        switch(_iPstnId) {
            default               :                      break ;
            case piPSB_XWait      :                      break ;
            case piPSB_XLIn       :                      break ;
            case piPSB_XLPush     :                      break ;
            case piPSB_XRGet      :                      break ;
            case piPSB_XRWorkStt  :                      break ;
            case piPSB_XRWork     :                      break ;
            case piPSB_XROut      :                      break ;

        }
    }
    else if(_iMotr == miREJ_XBtm){
        switch(_iPstnId) {
            default                  :                   break ;
            case piREJ_XBWait        :                   break ;
            case piREJ_XBWorkStt     :                   break ;
            case piREJ_XBWork        :                   break ;
        }
    }
    else if(_iMotr == miSTL_YTop){
        if(isDrngPunch) {sMsg = "During Punching"; bRet = false ;}
        switch(_iPstnId) {
            default                  :                   break ;
            case piSTL_YTWait        :                   break ;
            case piSTL_YTWorkStart   :                   break ;
            case piSTL_YTWork        :                   break ;
        }
    }

    else { bRet = false; }

    if(!bRet){
        Trace(AT_GetName(_iMotr), sMsg.c_str());
        if(!Step.iCycle && !Step.iToStart)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}
//---------------------------------------------------------------------------
bool  CPostBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isXMoving = MT_GetStopInpos(miPSB_XIdx);

    if(_iActr == aiPSB_Lft) {
        if(!_bFwd) {
            if(!isXMoving) { sMsg = (AnsiString)MT_GetName(miPSB_XIdx)+" is Moving" ; bRet = false ; }
        }
    }
    else if(_iActr == aiPSB_Rgt ) {
        if(!_bFwd) {
            if(!isXMoving) { sMsg = (AnsiString)MT_GetName(miPSB_XIdx)+" is Moving" ; bRet = false ; }
        }
    }
    else if(_iActr == aiPSB_Rolr) {

    }


    if(!bRet){
        Trace(AT_GetName(_iActr), sMsg.c_str());
        if(!Step.iCycle && !Step.iToStart)FM_MsgOk("CheckSafe",sMsg.c_str());
    }

    return bRet ;
}
//---------------------------------------------------------------------------
bool CPostBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId, bool _bSlow) // 모터를 움직일때 쓰는 함수.
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
//---------------------------------------------------------------------------
bool CPostBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}
//---------------------------------------------------------------------------
bool CPostBuff::CycleHome()     //sun DLL direct access.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() &&!OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErrMsg(eiPRT_HomeTo, m_sPartName.c_str());
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

        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 Step.iHome = 0 ;
                 return true ;

        case 10: IO_SetY(yPSB_FeedingAC,false);
                 AT_MoveCyl(aiPSB_Lft , ccBwd);
                 AT_MoveCyl(aiPSB_Rgt , ccBwd);
                 AT_MoveCyl(aiPSB_Rolr, ccBwd);
                 AT_MoveCyl(aiSTL_Top , ccBwd);
                 AT_MoveCyl(aiSTL_Btm , ccBwd);
                 Step.iHome++;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_Lft , ccBwd))return false ;
                 if(!AT_MoveCyl(aiPSB_Rgt , ccBwd))return false ;
                 if(!AT_MoveCyl(aiPSB_Rolr, ccBwd))return false ;
                 if(!AT_MoveCyl(aiSTL_Top , ccBwd))return false ;
                 if(!AT_MoveCyl(aiSTL_Btm , ccBwd))return false ;
                 MT_DoHome(miSTL_YTop);
                 MT_DoHome(miREJ_XBtm);
                 Step.iHome++;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miSTL_YTop))return false;
                 if(!MT_GetHomeEnd(miREJ_XBtm))return false;
                 MT_GoAbsMan(miSTL_YTop, GetMotrPos(miSTL_YTop, piSTL_YTWait));
                 MT_GoAbsMan(miREJ_XBtm, GetMotrPos(miREJ_XBtm, piREJ_XBWait));
                 MT_DoHome(miPSB_XIdx);
                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miSTL_YTop, GetMotrPos(miSTL_YTop,piSTL_YTWait)))return false;
                 if(!MT_GoAbsMan(miREJ_XBtm, GetMotrPos(miREJ_XBtm,piREJ_XBWait)))return false;
                 if(!MT_GetHomeEnd(miPSB_XIdx))return false;
                 MT_GoAbsMan(miPSB_XIdx, GetMotrPos(miPSB_XIdx,piPSB_XWait));
                 Step.iHome++;
                 return false;

        case 14: if(!MT_GoAbsMan(miPSB_XIdx, GetMotrPos(miPSB_XIdx,piPSB_XWait)))return false;
                 Step.iHome = 0;
                 return true ;
    }
}
//---------------------------------------------------------------------------
bool CPostBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    if ( IO_GetX(xPSB_FstOvrld) || IO_GetX(xPSB_SndOvrld )){
        EM_SetErrMsg(eiPRT_OverLoad , m_sPartName.c_str());
        MT_Stop(miWRK_XBtm);
        Step.iCycle = 0;
    }

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        //sun kye won.
        //Cycle동작에 관련 되어 있으면 SetSubStep으로 사용.
        //Cycle동작말고 자재와 관련 있으면 SetStep으로 사용함. JS 2013. 9. 11.
        if (Stat.bReqStop)return false ;

        int r1, c1;

        bool isCycleIn      =!DM.ARAY[riPSB].GetCntStat(csNone) && DM.ARAY[riPSB].GetStep() == 0 && WRK.GetCycleStep() != CWorkZone::scOut;
        bool isCycleMove    =!DM.ARAY[riPSB].GetCntStat(csNone) && DM.ARAY[riPSB].GetStep() == 1 &&
                              DM.ARAY[riPSB].GetCntStat(csFail) && GetReadyPos() && !OM.CmnOptn.bIgnrTrim;

        bool isCycleWork    =!DM.ARAY[riPSB].GetCntStat(csNone) && DM.ARAY[riPSB].GetStep() == 1 &&
                              DM.ARAY[riPSB].GetCntStat(csFail) && !GetReadyPos() && !OM.CmnOptn.bIgnrTrim;

        bool bTemp1 = DM.ARAY[riPSB].GetCntStat(csFail);

        bool isCycleOut     = !DM.ARAY[riPSB].GetCntStat(csNone )  &&
                             (!DM.ARAY[riPSB].GetCntStat(csFail )  ||
                             ( DM.ARAY[riPSB].GetCntStat(csFail )  || OM.CmnOptn.bIgnrTrim)) &&
                               DM.ARAY[riULD].GetCntStat(csEmpty)  &&
                               DM.ARAY[riPSB].GetStep() == 1 &&
                               ULD.GetSeqStep() == CUnLoader :: scIdle ;

        bool isCycleEnd     =  DM.ARAY[riPSB].GetCntStat(csNone) ;




        //모르는 스트립에러.
        if(isCycleWork && DM.ARAY[riPSB].CheckAllStat(csNone) && !OM.CmnOptn.bDryRun) {
            if(IO_GetX(xSTL_Detect1) || IO_GetX(xSTL_Detect2) || IO_GetX(xPSB_PkgOut1) || IO_GetX(xPSB_PkgOut2) ) EM_SetErrMsg(eiPKG_Unknwn, m_sPartName.c_str()) ;
        }

        //strip 사라짐.
        if(!isCycleIn && DM.ARAY[riPSB].GetCntExist() && !OM.CmnOptn.bDryRun) {
            if(!IO_GetX(xSTL_Detect1) && !IO_GetX(xSTL_Detect2) && !IO_GetX(xPSB_PkgOut1) && !IO_GetX(xPSB_PkgOut2))EM_SetErrMsg(eiPKG_Dispr, m_sPartName.c_str()) ;
        }
        if(EM_IsErr()) return false ;

         //Normal Decide Step.
             if (isCycleIn      ) {Trace(m_sPartName.c_str(),"CycleIn       Stt" ); Step.iSeq = scIn       ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleMove    ) {Trace(m_sPartName.c_str(),"CycleMove     Stt" ); Step.iSeq = scMove     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork    ) {Trace(m_sPartName.c_str(),"CycleWork     Stt" ); Step.iSeq = scWork     ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut     ) {Trace(m_sPartName.c_str(),"CycleOut      Stt" ); Step.iSeq = scOut      ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleEnd     ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default         :                     Trace(m_sPartName.c_str(),"default        End");Step.iSeq = scIdle ;  return false ;
        case scIdle     :                                                                                           return false ;
        case scIn       : if(CycleIn      ()){ Trace(m_sPartName.c_str(),"CycleIn       End");Step.iSeq = scIdle ;} return false ;
        case scMove     : if(CycleMove    ()){ Trace(m_sPartName.c_str(),"CycleMove     End");Step.iSeq = scIdle ;} return false ;
        case scWork     : if(CycleWork    ()){ Trace(m_sPartName.c_str(),"CycleWork     End");Step.iSeq = scIdle ;} return false ;
        case scOut      : if(CycleOut     ()){ Trace(m_sPartName.c_str(),"CycleOut      End");Step.iSeq = scIdle ;} return false ;
    }

  return false ;
}


//---------------------------------------------------------------------------
bool CPostBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10;
    //Ok.
    return true;
}
//---------------------------------------------------------------------------
bool CPostBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}
//---------------------------------------------------------------------------
bool CPostBuff::ToStart(void) //스타트를 하기 위한 함수.
{
    
    AnsiString sTemp ;
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() , 5000)) {
        EM_SetErrMsg(eiPRT_ToStartTO, m_sPartName.c_str());
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iToStart = 0 ;
        return true;
    }
    if(Step.iToStart != PreStep.iToStart) {
        sTemp = sTemp.sprintf("%s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: IO_SetY(yPSB_FeedingAC,false);
                 MoveActr(aiPSB_Lft  , ccBwd);
                 MoveActr(aiPSB_Rgt  , ccBwd);
                 MoveActr(aiPSB_Rolr , ccFwd);
                 MoveActr(aiSTL_Btm  , ccBwd);
                 MoveActr(aiSTL_Top  , ccBwd);
                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_Lft  , ccBwd)) return false ;
                 if(!MoveActr(aiPSB_Rgt  , ccBwd)) return false ;
                 if(!MoveActr(aiPSB_Rolr , ccFwd)) return false ;
                 if(!MoveActr(aiSTL_Btm  , ccBwd)) return false ;
                 if(!MoveActr(aiSTL_Top  , ccBwd)) return false ;
                 MoveMotr(miSTL_YTop , piSTL_YTWait);
                 Step.iToStart++;
                 return false;

        case 12: if(!MoveMotr(miSTL_YTop , piSTL_YTWait))return false;
                 if(DM.ARAY[riPSB].GetCntStat(csNone)){
                     Step.iToStart++;
                     return false;
                 }

                 Step.iToStart = 0;
                 return true;

        case 13: MoveMotr(miPSB_XIdx , piPSB_XWait);
                 MoveActr(aiPSB_Hld  , ccBwd);
                 Step.iToStart++;
                 return false ;

        case 14: if(!MoveMotr(miPSB_XIdx , piPSB_XWait)) return false ;
                 if(!MoveActr(aiPSB_Hld , ccBwd)) return false;
                 Step.iToStart = 0 ;
                 return true ;
    }
}
//---------------------------------------------------------------------------
bool CPostBuff::ToStop(void) //스탑을 하기 위한 함수.
{
    
    AnsiString sTemp ;
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop(), 10000)) {
        EM_SetErrMsg(eiPRT_ToStopTO, m_sPartName.c_str());
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iToStart = 0 ;
        return true;
    }

    if(Step.iToStop != PreStep.iToStop) {
        sTemp = sTemp.sprintf("%s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }


    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: IO_SetY(yPSB_FeedingAC, false);
                 Step.iToStop ++ ;
                 return false ;

        case 11: IO_SetY(yPSB_FeedingAC, false);
                 if(IO_GetY(yPSB_FeedingAC)) return false;
                 if(!MT_GetAlarm   (miPSB_XIdx) &&  //껏다켰을때 마지막 포지션 기억 하기위해서 인데 리및을 치고 있거나 알람중이면 리셑하도록 .
                    !MT_GetNLimSnsr(miPSB_XIdx) &&
                    !MT_GetPLimSnsr(miPSB_XIdx) ) m_dLastIndPos = MT_GetCmdPos(miPSB_XIdx) ;
                 else                             m_dLastIndPos = GetMotrPos  (miPSB_XIdx , piPSB_XWait) ;

                 Step.iToStop = 0   ;
                 return true ;
    }
}
//---------------------------------------------------------------------------

bool CPostBuff::CycleIn() //WorkZone 에서 자제 받자 마자 무조건  한번 타서 작업 시작 위치로 가져 간다.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str());
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    int fr, fc;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPSB_Lft, ccBwd);
                  MoveActr(aiPSB_Rgt, ccBwd);
                  if(OM.CmnOptn.bUsedInsOptn){
                      for(int r = 0; r < OM.DevInfo.iRowCnt ; r++){
                          for(int c = 0; c < OM.DevInfo.iColCnt; c++){
                              if(DM.ARAY[riOPT].GetStat(r, c) != csNone){
                                  DM.ARAY[riPSB].SetStat(r, c, DM.ARAY[riOPT].GetStat(r, c));
                              }
                          }
                      }
                  }
                  if(m_sLastLotNo != DM.ARAY[riPSB].GetLotNo()){
                      m_sLastLotNo = DM.ARAY[riPSB].GetLotNo();
                      m_iTrimCnt = 0;
                      m_iBin0Cnt = 0;
                      m_iBin1Cnt = 0;
                      m_iBin2Cnt = 0;
                      m_iBin3Cnt = 0;
                      m_iBin4Cnt = 0;
                      m_iBin5Cnt = 0;
                      m_iBin6Cnt = 0;
                      m_iBin7Cnt = 0;
                      m_iBin8Cnt = 0;
                      m_iBin9Cnt = 0;

                  }
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Lft, ccBwd)) return false;
                  if(!MoveActr(aiPSB_Rgt, ccBwd)) return false;
                  //Lot이름으로 확인.JS
                  MoveMotr(miPSB_XIdx, piPSB_XLIn);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miPSB_XIdx, piPSB_XLIn)) return false ;
                  MoveActr(aiPSB_Lft, ccFwd);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveActr(aiPSB_Lft, ccFwd)) return false;
                  MoveMotr(miPSB_XIdx, piPSB_XLPush);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveMotr(miPSB_XIdx, piPSB_XLPush)) return false ;
                  //DM.ShiftArrayData(riPS1,riPS2);

                  MoveActr(aiPSB_Lft , ccBwd);
                  Step.iCycle++;
                  return false ;

        case  15: if(!MoveActr(aiPSB_Lft , ccBwd)) return false ;
                  MoveMotr(miPSB_XIdx , piPSB_XRGet);
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveMotr(miPSB_XIdx, piPSB_XRGet))return false;
                  MoveActr(aiPSB_Rgt , ccFwd);
                  Step.iCycle++;
                  return false ;

        case  17: if(!MoveActr(aiPSB_Rgt , ccFwd)) return false ;
                  MoveMotr(miPSB_XIdx, piPSB_XRWorkStt);
                  //if(DM.ARAY[riPSB].GetCntStat(csFail))
                  DM.ARAY[riPSB].SetStep(1);
                  //SortBinOption 때문에 주석. JS
                  //STL.RejectMove(); //페일이 하나도 안나는 메거진은

                  FindChip(fr , fc , csFail);
                  m_iCrntCol = fc;
                  Step.iCycle = 0;
                  return true ;
        }
}

//---------------------------------------------------------------------------

bool CPostBuff::CycleMove()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str());
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPSB_Rgt, ccFwd);
                  MoveActr(aiPSB_Hld, ccBwd);
                  MoveActr(aiSTL_Btm, ccBwd);
                  MoveActr(aiSTL_Top, ccBwd);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveActr(aiPSB_Rgt, ccFwd)) return false;
                  if(!MoveActr(aiPSB_Hld, ccBwd)) return false;
                  if(!MoveActr(aiSTL_Btm, ccBwd)) return false;
                  if(!MoveActr(aiSTL_Top, ccBwd)) return false;
                  MoveMotr(miSTL_YTop , piSTL_YTWait);
                  Step.iCycle++;
                  return false;

        case  12: if(!MoveMotr(miSTL_YTop , piSTL_YTWait))return false;
                  MoveMotr(miPSB_XIdx , piPSB_XRWork);
                  Step.iCycle++;
                  return false;

        case  13: if(!MoveMotr(miPSB_XIdx , piPSB_XRWork)) return false;
                  MoveActr(aiPSB_Hld, ccFwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiPSB_Hld, ccFwd)) return false ;
                  MT_GoAbsRun(miPSB_XIdx , GetMotrPos(miPSB_XIdx , piPSB_XRWork) - PSBBackPos);
                  Step.iCycle++;
                  return false;

        case  15: if(!MT_GoAbsRun(miPSB_XIdx , GetMotrPos(miPSB_XIdx , piPSB_XRWork) - PSBBackPos))return false;
                  Step.iCycle = 0;
                  return true ;
        }
}

bool CPostBuff::CycleWork()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 5000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str());
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    int r1, c1;
    FindChip(r1 , c1 , csFail);

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveMotr(miREJ_XBtm , piREJ_XBWork);
                  Step.iCycle++;
                  return false;

        case  11: if(!MoveMotr(miREJ_XBtm , piREJ_XBWork)) return false;
                  if(m_iBinCnt == 0 && !IO_GetX(xREJ_Box1     )) {EM_SetErrMsg(eiREJ_Box , "1"  ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 1 && !IO_GetX(xREJ_Box2     )) {EM_SetErrMsg(eiREJ_Box , "2"  ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 2 && !IO_GetX(xREJ_Box3     )) {EM_SetErrMsg(eiREJ_Box , "3"  ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 3 && !IO_GetX(xREJ_Box4     )) {EM_SetErrMsg(eiREJ_Box , "4"  ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 4 && !IO_GetX(xREJ_Box5     )) {EM_SetErrMsg(eiREJ_Box , "5"  ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 5 && !IO_GetX(xREJ_Box6     )) {EM_SetErrMsg(eiREJ_Box , "6"  ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 6 && !IO_GetX(xREJ_Box7     )) {EM_SetErrMsg(eiREJ_Box , "7"  ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 7 && !IO_GetX(xREJ_Box8     )) {EM_SetErrMsg(eiREJ_Box , "8"  ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 8 && !IO_GetX(xREJ_Box9     )) {EM_SetErrMsg(eiREJ_Box , "9"  ); Step.iCycle = 0 ; return true ; }
                  if(m_iBinCnt == 9 && !IO_GetX(xREJ_Box10    )) {EM_SetErrMsg(eiREJ_Box , "10" ); Step.iCycle = 0 ; return true ; }
                  MoveActr(aiSTL_Top , ccBwd);
                  MoveActr(aiPSB_Hld , ccFwd);
                  Step.iCycle++;                                                           
                  return false;

        case  12: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  if(!MoveActr(aiPSB_Hld , ccFwd)) return false;
                  Step.iCycle++;
                  return false;

        case  13: MoveActr(aiSTL_Btm , ccFwd);
                  Step.iCycle++;
                  return false;

        case  14: if(!MoveActr(aiSTL_Btm , ccFwd)) return false;
                  MoveMotr(miSTL_YTop , piSTL_YTWork);
                  Step.iCycle++;
                  return false;

        case  15: if(!MoveMotr(miSTL_YTop , piSTL_YTWork)) return false ;
                  MoveActr(aiSTL_Top , ccFwd);
                  IO_SetY(ySTL_Blower , true);
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  16: if(!MoveActr(aiSTL_Top , ccFwd)) return false;
                  if(!m_tmDelay.OnDelay(true ,  OM.DevOptn.iTrimRjtDly)) return false ;
                  IO_SetY(ySTL_Blower , false);
                  MoveActr(aiSTL_Top , ccBwd);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(aiSTL_Top , ccBwd)) return false;
                  m_tmDelay.Clear();
                  Step.iCycle++;
                  return false;

        case  18: if(!m_tmDelay.OnDelay(true, OM.DevOptn.iRejectMoveDeley)) return false;
                  DM.ARAY[riPSB].SetStat(r1, c1,csEmpty);
                  if(OM.MstOptn.bUsedBinSorting){
                      if(m_iBinCnt == 0) m_iBin0Cnt++;
                      if(m_iBinCnt == 1) m_iBin1Cnt++;
                      if(m_iBinCnt == 2) m_iBin2Cnt++;
                      if(m_iBinCnt == 3) m_iBin3Cnt++;
                      if(m_iBinCnt == 4) m_iBin4Cnt++;
                      if(m_iBinCnt == 5) m_iBin5Cnt++;
                      if(m_iBinCnt == 6) m_iBin6Cnt++;
                      if(m_iBinCnt == 7) m_iBin7Cnt++;
                      if(m_iBinCnt == 8) m_iBin8Cnt++;
                      if(m_iBinCnt == 9) m_iBin9Cnt++;
                  }

                  //MoveActr(aiSTL_Btm , ccBwd);
                  //MoveMotr(miSTL_YTop , piSTL_YTWait);
                  Step.iCycle++;
                  return false;

        case  19: //if(!MoveActr(aiSTL_Btm , ccBwd)) return false;
                  //if(!MoveMotr(miSTL_YTop , piSTL_YTWait)) return false;
                  Step.iCycle = 0 ;
                  return true ;

    }
}
//---------------------------------------------------------------------------
bool CPostBuff::CycleOut()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode, 10000 )) {
        EM_SetErrMsg(eiPRT_CycleTO , m_sPartName.c_str());
        sTemp = sTemp.sprintf( "%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    static bool bLastStrip ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iCycle != PreStep.iCycle) Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: LOT.WriteTrimInfo(DM.ARAY[riPSB].GetLotNo(), m_iTrimCnt, _iBinCnt);
                  if(OM.MstOptn.bUsedBinSorting) {
                      LOT.WriteBinInfo (DM.ARAY[riPSB].GetLotNo(), m_iBin0Cnt, m_iBin1Cnt, m_iBin2Cnt, m_iBin3Cnt, m_iBin4Cnt,
                      m_iBin5Cnt, m_iBin6Cnt, m_iBin7Cnt, m_iBin8Cnt, m_iBin9Cnt);
                  }
                  IO_SetY(yPSB_FeedingAC,true);
                  MoveActr(aiPSB_Rolr, ccFwd);
                  MoveActr(aiPSB_Rgt , ccFwd);
                  MoveActr(aiPSB_Hld , ccBwd);
                  MoveActr(aiSTL_Btm , ccBwd);
                  MoveMotr(miSTL_YTop, piSTL_YTWait);
                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPSB_Rgt , ccFwd)) return false;
                  if(!MoveActr(aiPSB_Rolr, ccFwd)) return false;
                  if(!MoveActr(aiPSB_Hld , ccBwd)) return false;
                  if(!MoveActr(aiSTL_Btm , ccBwd)) return false;
                  if(!MoveMotr(miSTL_YTop, piSTL_YTWait)) return false;
                  if(ULD.GetSeqStep() != CUnLoader :: scIdle) return false  ; //졸라짜증나는 1싸이클 사이 에 들어 올수 있어서 막아놓음.
                  MoveMotr(miPSB_XIdx,piPSB_XROut);
                  Step.iCycle++;
                  return false ;

        case  12: if(!MoveMotr(miPSB_XIdx,piPSB_XROut)) return false ;
                  MoveActr(aiPSB_Rgt  , ccBwd);
                  MoveActr(aiPSB_Rolr , ccBwd);
                  Step.iCycle++;
                  return false ;


        case  13: if(!MoveActr(aiPSB_Rgt  , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_Rolr , ccBwd)) return false ;
                  Step.iCycle++;
                  return false;

        case  14: MoveMotr(miPSB_XIdx,piPSB_XWait);
                  Step.iCycle++;
                  return false;

        case  15: IO_SetY(yPSB_FeedingAC,false);
                  Step.iCycle++;
                  return false;

        case  16: if(IO_GetX(xPSB_PkgOut2)){EM_SetErr(eiPSB_Out2Detected); return false; }
                  MoveActr(aiPSB_Rolr , ccFwd);
                  DM.ARAY[riPSB].SetStat(csNone) ;
                  if(DM.ARAY[riULD].CheckAllStat(csEmpty)) DM.ARAY[riULD].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                  DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);
                  Step.iCycle++;
                  return false;

        case  17: if(!MoveActr(aiPSB_Rolr , ccFwd      )) return false ;
                  if(!MoveMotr(miPSB_XIdx , piPSB_XWait)) return false ;
                  DM.ARAY[riPSB].SetSubStep(0);
                  DM.ARAY[riPSB].SetStep(0);
                  Step.iCycle = 0;
                  return true;
    }
}
//---------------------------------------------------------------------------
void CPostBuff::SetLastCmd()
{
    return ; //이파트는 필요 없다.
}
//---------------------------------------------------------------------------
bool CPostBuff::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}
//---------------------------------------------------------------------------
bool CPostBuff::CheckStop()
{
    if(!MT_GetStop(miPSB_XIdx)) return false ;
    if(!MT_GetStop(miSTL_YTop)) return false ;
    if(!MT_GetStop(miREJ_XBtm)) return false ;

    if(!AT_Done(aiPSB_Hld    )) return false ;
    if(!AT_Done(aiPSB_Lft    )) return false ;
    if(!AT_Done(aiPSB_Rgt    )) return false ;
    if(!AT_Done(aiPSB_Rolr   )) return false ;
    if(!AT_Done(aiSTL_Top    )) return false ;
    if(!AT_Done(aiSTL_Btm    )) return false ;


    return true ;
}
//---------------------------------------------------------------------------
void CPostBuff::Load(bool IsLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\PstBuff.INI";

    if ( IsLoad ) {
//        UserINI.Load(sPath, "Member" , "m_dFailChipPos" , m_dFailChipPos );
        UserINI.Load(sPath, "Member" , "m_dLastIndPos  " , m_dLastIndPos  );
        UserINI.Load(sPath, "Member" , "m_iCrntCol     " , m_iCrntCol     );
        UserINI.Load(sPath, "Member" , "m_sWorkedLotNo " , m_sWorkedLotNo );


    }
    else {
        UserINI.ClearFile(sPath) ;
//        UserINI.Save(sPath, "Member" , "m_dFailChipPos" , m_dFailChipPos );
        UserINI.Save(sPath, "Member" , "m_dLastIndPos  " , m_dLastIndPos  );
        UserINI.Save(sPath, "Member" , "m_iCrntCol     " , m_iCrntCol     );
        UserINI.Save(sPath, "Member" , "m_sWorkedLotNo " , m_sWorkedLotNo );


    }
}
//---------------------------------------------------------------------------

bool CPostBuff::GetLastStrip()
{
    AnsiString sLastLotNo = DM.ARAY[riPSB].GetCntExist() ? DM.ARAY[riPSB].GetLotNo() : m_sWorkedLotNo ;

    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWRKExist = !DM.ARAY[riWRK].CheckAllStat(csNone) ;

    bool bLDRDiff  = sLastLotNo != DM.ARAY[riLDR].GetLotNo() ;
    bool bPRBDiff  = sLastLotNo != DM.ARAY[riPRB].GetLotNo() ;
    bool bWRKDiff  = sLastLotNo != DM.ARAY[riWRK].GetLotNo() ;

    bool bLastMgz  = LDR._iMgzCnt > OM.DevOptn.iLotEndMgzCnt ;

         if( bWRKExist) return bWRKDiff ;
    else if( bPRBExist) return bPRBDiff ;
    else if( bLDRExist) return bLDRDiff ;
    else if(!bLastMgz ) return false    ;
    else                return true     ;
}
