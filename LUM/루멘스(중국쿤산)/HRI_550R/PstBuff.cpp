//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "PstBuff.h"
#include "UnLoader.h"
#include "Rail.h"
#include "Loader.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "SPCUnit.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
CPstBuff  PSB;

enum {PSB_Insp  = 0 , PSB_Mark  = 1 };
enum EN_INSP_DIRC{idLeftTop = 0 , idLeftBtm = 1 };
EN_INSP_DIRC PSB_INSP_DIRECTION = idLeftBtm ;

int PsbGetVtlColCnt ()
{
    int iRet = 0 ;
    if(OM.DevInfo.iColInspMrkCnt) {
        iRet = (OM.DevInfo.iColCnt % OM.DevInfo.iColInspMrkCnt) ?
              ((OM.DevInfo.iColCnt / OM.DevInfo.iColInspMrkCnt) * OM.DevInfo.iColInspMrkCnt + OM.DevInfo.iColInspMrkCnt) :
              ((OM.DevInfo.iColCnt / OM.DevInfo.iColInspMrkCnt) * OM.DevInfo.iColInspMrkCnt) ;
    }
    return iRet ;
}
int PsbGetVtlRowCnt ()
{
    int iRet = 0 ;
    if(OM.DevInfo.iRowInspMrkCnt) {
        iRet = (OM.DevInfo.iRowCnt % OM.DevInfo.iRowInspMrkCnt) ?
              ((OM.DevInfo.iRowCnt / OM.DevInfo.iRowInspMrkCnt) * OM.DevInfo.iRowInspMrkCnt + OM.DevInfo.iRowInspMrkCnt) :
              ((OM.DevInfo.iRowCnt / OM.DevInfo.iRowInspMrkCnt) * OM.DevInfo.iRowInspMrkCnt) ;
    }
    return iRet ;
}

double PsbGetLeftPos ()
{
    double dRet = (PM.GetValue(miPSB_XMrk , pvPSB_XMrkVisnPs) - (OM.DevInfo.iColInspMrkCnt - 1) * OM.DevInfo.dColPitch / 2.0) ;
    return dRet ;
}
double PsbGetRightPos ()
{
    int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;
    double dRet = PM.GetValue(miPSB_XMrk , pvPSB_XMrkVisnPs) -
                  OM.DevInfo.dColPitch * PsbGetVtlColCnt() -
                  (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) *        (iColOneGrCnt ? (PsbGetVtlColCnt()/iColOneGrCnt) : 0) +
                  (OM.DevInfo.iColInspMrkCnt - 1) * OM.DevInfo.dColPitch / 2.0 ;
    return dRet ;
}

double PsbGetLeftPosCt()
{
    double dRet = PM.GetValue(miPSB_XMrk , pvPSB_XMrkVisnPs) ;
    return dRet ;
}

double PsbGetRightPosCt()
{
    int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;

    double dRet = PM.GetValue(miPSB_XMrk , pvPSB_XMrkVisnPs) + OM.DevInfo.dColPitch * PsbGetVtlColCnt() +
                  (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * (iColOneGrCnt ? (PsbGetVtlColCnt()/iColOneGrCnt) : 0) -
                  OM.DevInfo.iColInspMrkCnt * OM.DevInfo.dColPitch ;
                  //(OM.DevOptn.iColInspMrkCnt - 1) * OM.DevInfo.dColPitch / 2.0 ;
    return dRet ;
}

bool PsbGetLtToRt(int r)
{
//    if(PSB_INSP_DIRECTION == idLeftTop) return !((OM.DevInfo.iRowInspMrkCnt ? r/OM.DevInfo.iRowInspMrkCnt : r) %2);
    if(PSB_INSP_DIRECTION == idLeftBtm) return !((OM.DevInfo.iRowInspMrkCnt ? r/OM.DevInfo.iRowInspMrkCnt : r) %2);
    else                            return  ((OM.DevInfo.iRowInspMrkCnt ? r/OM.DevInfo.iRowInspMrkCnt : r) %2);
}

CPstBuff::CPstBuff(void)
{
    m_sPartName = "Post Buff " ;
    m_iTrimErr  = 0 ;
    Reset();
    Load(true);
}

void CPstBuff::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
}

void CPstBuff::ResetTimer()
{
    //Clear Timer.
    m_tmMain   .Clear();
    m_tmCycle  .Clear();
    m_tmHome   .Clear();
    m_tmToStop .Clear();
    m_tmToStart.Clear();
    m_tmTemp   .Clear();
}

CPstBuff::~CPstBuff (void)
{
 //FormMain으로     Load(false);
//    Trace("","");
}

bool CPstBuff::FindChipMrk( int &r , int &c ) //안씀.
{
    int cSel  = OM.DevInfo.iColCnt ;
    int rTemp = 0 ;
    int cTemp = 0 ;


//    c = DM.ARAY[riPSB].FindFrstCol(csRslt0 ); if(c != -1){if(cSel > c) cSel = c ; }
//    if(!OM.CmnOptn.bEmptyNoMrk) c = DM.ARAY[riPSB].FindFrstCol(csRslt1 ); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR1FailSkip ) c = DM.ARAY[riPSB].FindFrstCol(csRslt1 ); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR2FailSkip ) c = DM.ARAY[riPSB].FindFrstCol(csRslt2 ); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR3FailSkip ) c = DM.ARAY[riPSB].FindFrstCol(csRslt3 ); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR4FailSkip ) c = DM.ARAY[riPSB].FindFrstCol(csRslt4 ); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR5FailSkip ) c = DM.ARAY[riPSB].FindFrstCol(csRslt5 ); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR6FailSkip ) c = DM.ARAY[riPSB].FindFrstCol(csRslt6 ); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR7FailSkip ) c = DM.ARAY[riPSB].FindFrstCol(csRslt7 ); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR8FailSkip ) c = DM.ARAY[riPSB].FindFrstCol(csRslt8 ); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR9FailSkip ) c = DM.ARAY[riPSB].FindFrstCol(csRslt9 ); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR10FailSkip) c = DM.ARAY[riPSB].FindFrstCol(csRslt10); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR11FailSkip) c = DM.ARAY[riPSB].FindFrstCol(csRslt11); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR12FailSkip) c = DM.ARAY[riPSB].FindFrstCol(csRslt12); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR13FailSkip) c = DM.ARAY[riPSB].FindFrstCol(csRslt13); if(c != -1){if(cSel > c) cSel = c ; }
    if(!OM.CmnOptn.bR14FailSkip) c = DM.ARAY[riPSB].FindFrstCol(csRslt14); if(c != -1){if(cSel > c) cSel = c ; }
//    c = DM.ARAY[riPSB].FindFrstCol(csRslt15); if(c != -1){if(cSel > c) cSel = c ; }
//    c = DM.ARAY[riPSB].FindFrstCol(csRslt16); if(c != -1){if(cSel > c) cSel = c ; }
    c = DM.ARAY[riPSB].FindFrstCol(csFail  ); if(c != -1){if(cSel > c) cSel = c ; }

    if(cSel % 2){

        rTemp = -1 ;
        if(!OM.CmnOptn.bR1FailSkip ) DM.ARAY[riPSB].FindFrstColLastRow(csRslt1  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR2FailSkip ) DM.ARAY[riPSB].FindFrstColLastRow(csRslt2  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR3FailSkip ) DM.ARAY[riPSB].FindFrstColLastRow(csRslt3  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR4FailSkip ) DM.ARAY[riPSB].FindFrstColLastRow(csRslt4  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR5FailSkip ) DM.ARAY[riPSB].FindFrstColLastRow(csRslt5  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR6FailSkip ) DM.ARAY[riPSB].FindFrstColLastRow(csRslt6  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR7FailSkip ) DM.ARAY[riPSB].FindFrstColLastRow(csRslt7  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR8FailSkip ) DM.ARAY[riPSB].FindFrstColLastRow(csRslt8  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR9FailSkip ) DM.ARAY[riPSB].FindFrstColLastRow(csRslt9  , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR10FailSkip) DM.ARAY[riPSB].FindFrstColLastRow(csRslt10 , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR11FailSkip) DM.ARAY[riPSB].FindFrstColLastRow(csRslt11 , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR12FailSkip) DM.ARAY[riPSB].FindFrstColLastRow(csRslt12 , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR13FailSkip) DM.ARAY[riPSB].FindFrstColLastRow(csRslt13 , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        if(!OM.CmnOptn.bR14FailSkip) DM.ARAY[riPSB].FindFrstColLastRow(csRslt14 , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColLastRow(csFail   , r , c) ; if(r != -1 && c == cSel)if(rTemp<r) rTemp = r ;

    }
    else     { //삐구다.
        rTemp = OM.DevInfo.iRowCnt ;
        if(!OM.CmnOptn.bR1FailSkip ) DM.ARAY[riPSB].FindFrstColRow    (csRslt1  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR2FailSkip ) DM.ARAY[riPSB].FindFrstColRow    (csRslt2  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR3FailSkip ) DM.ARAY[riPSB].FindFrstColRow    (csRslt3  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR4FailSkip ) DM.ARAY[riPSB].FindFrstColRow    (csRslt4  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR5FailSkip ) DM.ARAY[riPSB].FindFrstColRow    (csRslt5  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR6FailSkip ) DM.ARAY[riPSB].FindFrstColRow    (csRslt6  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR7FailSkip ) DM.ARAY[riPSB].FindFrstColRow    (csRslt7  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR8FailSkip ) DM.ARAY[riPSB].FindFrstColRow    (csRslt8  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR9FailSkip ) DM.ARAY[riPSB].FindFrstColRow    (csRslt9  , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR10FailSkip) DM.ARAY[riPSB].FindFrstColRow    (csRslt10 , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR11FailSkip) DM.ARAY[riPSB].FindFrstColRow    (csRslt11 , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR12FailSkip) DM.ARAY[riPSB].FindFrstColRow    (csRslt12 , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR13FailSkip) DM.ARAY[riPSB].FindFrstColRow    (csRslt13 , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        if(!OM.CmnOptn.bR14FailSkip) DM.ARAY[riPSB].FindFrstColRow    (csRslt14 , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;
        DM.ARAY[riPSB].FindFrstColRow    (csFail   , r , c) ; if(r != -1 && c == cSel)if(rTemp>r) rTemp = r ;

    }
    r= rTemp ; c= cSel ;
    return (r > -1 && c > -1 );

}

bool CPstBuff::FindChipVsn(int &r , int &c )
{
    int r1 , c1 ;

    if(PSB_INSP_DIRECTION == idLeftTop) {
//        if(DM.ARAY[riVS4].CheckAllExist()) {
            DM.ARAY[riVS4].FindFrstRowCol(csUnkwn  , r1 , c1);
            r = r1 ; c = c1 ;
//        }
    }

    if(PSB_INSP_DIRECTION == idLeftBtm) {
//        if(DM.ARAY[riVS4].CheckAllExist()) {
            DM.ARAY[riVS4].FindLastRowFrstCol(csUnkwn  , r1 , c1);
            r = r1 ; c = c1 ;
//        }
        r = DM.ARAY[riVS4].GetMaxRow() -r -1 ;
        c = DM.ARAY[riVS4].GetMaxCol() -c -1 ;
    }

    if(r == -1 && c == -1) return false ;

    return true ;
}


double CPstBuff::GetMotrPosTable(EN_MOTR_ID _iMotr , int _iRow , int _iCol)
{
    int    r , c ;
    double dStartYPos ;
    double dStartXPos ;
    double dYPos      ;
    double dXPos      ;

    r = _iRow ;//OM.DevOptn.iRowInspMrkCnt ? _iRow/OM.DevOptn.iRowInspMrkCnt * OM.DevOptn.iRowInspMrkCnt : _iRow ;
    c = _iCol ;//OM.DevOptn.iColInspMrkCnt ? _iCol/OM.DevOptn.iColInspMrkCnt * OM.DevOptn.iColInspMrkCnt : _iCol ;

    dStartYPos = PM.GetValue(_iMotr , pvPSB_YMrkWorkStartPs ) ;
    dStartXPos = PM.GetValue(_iMotr , pvPSB_XMrkWorkStartPs ) ;

    if(_iMotr != miPSB_YMrk && _iMotr != miPSB_XMrk) return MT_GetCmdPos(_iMotr) ;

    dXPos = dStartXPos - c * OM.DevInfo.dColPitch ;
    dYPos = dStartYPos + r * OM.DevInfo.dRowPitch ;

    if(OM.DevInfo.dColGrGap && OM.DevInfo.iColGrCnt) {
        dXPos = dXPos - (c / OM.DevInfo.iColGrCnt) * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) ;
    }


    if(OM.DevInfo.dRowGrGap && OM.DevInfo.iRowGrCnt) {
        dYPos = dYPos + (r / OM.DevInfo.iRowGrCnt) * (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) ;
    }

    if     (_iMotr == miPSB_YMrk) return dYPos ;
    else if(_iMotr == miPSB_XMrk) return dXPos ;
}

double CPstBuff::GetMotrVsPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{

    int r  , c  ;
    const double dOfsforTrg = 5.0 ; //아진 인터럽트 트리거를 위한 옵셑.
    double dStartYPos ;
    const bool bPkgCntr = false ;
    //int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    //int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;

    //요놈. 플라잉 방식이라 맵핑 못씀.
    FindChipVsn( r , c ) ;

    r = OM.DevInfo.iRowInspMrkCnt ? r/OM.DevInfo.iRowInspMrkCnt  * OM.DevInfo.iRowInspMrkCnt  :  r ;

    bool bLeftToRight = PsbGetLtToRt(r);

//    if     (_iMotr == miWK1_YVsn) dStartYPos = PM.GetValue(miWK1_YVsn , pvWK1_YVsnWorkSttPs ) ;
//    else if(_iMotr == miWK2_YVsn) dStartYPos = PM.GetValue(miWK2_YVsn , pvWK2_YVsnWorkSttPs ) ;
    dStartYPos = PM.GetValue(miPSB_YMrk , pvPSB_YMrkVisnPs ) ;

    //스타트 포지션과 엔드 포지션 구분.
    double dXStartPos ;
    double dXEndPos   ;
    double dYPos      ;
//세트
/**/int iVtlColCnt = PsbGetVtlColCnt();
/**/int iVtlRowCnt = PsbGetVtlRowCnt();
/**/
/**/double dLeftPos  = PsbGetLeftPos ();
/**/double dRightPos = PsbGetRightPos();
/**/
/**/double dLeftPosCt  = PsbGetLeftPosCt ();
/**/double dRightPosCt = PsbGetRightPosCt();

    if(OM.DevInfo.iInsMrkVsRowCnt == 1) {  //한줄만 검사 할때는 왼쪽에서 오른쪽으로만 가게 함.
        dXEndPos   = dLeftPosCt + dOfsforTrg ;
        dXStartPos     = dXEndPos - dOfsforTrg - OM.DevInfo.iInsMrkVsColCnt * OM.DevInfo.iColInspMrkCnt * OM.DevInfo.dColPitch ;
   
    }
    else if(bLeftToRight ) {
        dXStartPos     = dLeftPosCt  + dOfsforTrg ;
//        dXEndPos   = dRightPosCt + dOfsforTrg ;
        dXEndPos   = dXStartPos    - dOfsforTrg - OM.DevInfo.iInsMrkVsColCnt * OM.DevInfo.iColInspMrkCnt * OM.DevInfo.dColPitch ;
    }
    else {
        dXEndPos   = dLeftPosCt + dOfsforTrg ;
        dXStartPos     = dXEndPos - dOfsforTrg - OM.DevInfo.iInsMrkVsColCnt * OM.DevInfo.iColInspMrkCnt * OM.DevInfo.dColPitch ;
    }

    if(PSB_INSP_DIRECTION == idLeftTop) {
        dYPos = dStartYPos +                                           //첫포지션 더하고.
                r * OM.DevInfo.dRowPitch +    //검사열을 감안해서 해당 로우 까지 거리계산해서 더하고.
               //(OM.DevOptn.iRowInspMrkCnt - 1) * OM.DevInfo.dRowPitch / 2.0 +  // 요거는
               (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * (OM.DevInfo.iRowGrCnt?(r+1)/OM.DevInfo.iRowGrCnt : 0 ) ; //그룹 갯수 개산 해서 더함.
    }

    if(PSB_INSP_DIRECTION == idLeftBtm) {
        dYPos = dStartYPos -                                           //첫포지션 더하고.
                r * OM.DevInfo.dRowPitch -    //검사열을 감안해서 해당 로우 까지 거리계산해서 더하고.
               //(OM.DevOptn.iRowInspMrkCnt - 1) * OM.DevInfo.dRowPitch / 2.0 +  // 요거는
               (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * (OM.DevInfo.iRowGrCnt?(r+1)/OM.DevInfo.iRowGrCnt : 0 ) ; //그룹 갯수 개산 해서 더함.
    }


    double dPos = 0.0 ;

    if(_iMotr == miPSB_XMrk){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piPSB_XMrkVisn      : dPos = PM.GetValue (_iMotr , pvPSB_XMrkVisnPs      ); break ;
            case piPSB_XVsnTrgStt    : dPos = dXStartPos                                   ; break ;
            case piPSB_XVsnTrgEnd    : dPos = dXEndPos                                     ; break ;
        }
    }

    else if(_iMotr == miPSB_YMrk){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piPSB_YMrkVisn      : dPos = PM.GetValue (_iMotr , pvPSB_YMrkVisnPs      ); break ;
            case piPSB_YMrkVisnWork  : dPos = dYPos                                        ; break ;
        }
    }

    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}
double CPstBuff::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    double dPos = 0.0 ;

    if(_iMotr ==  miPSB_XMrk)
    {
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos (_iMotr                         ); break ;
            case piPSB_XMrkWait      : dPos = PM.GetValue  (_iMotr , pvPSB_XMrkWaitPs      ); break ;
            case piPSB_XMrkWorkStart : dPos = PM.GetValue  (_iMotr , pvPSB_XMrkWorkStartPs ); break ;
            case piPSB_XMrkVisn      : dPos = PM.GetValue  (_iMotr , pvPSB_XMrkVisnPs      ); break ;
            case piPSB_XVsnTrgStt    : dPos = GetMotrVsPos (_iMotr , _iPstnId              ); break ;
            case piPSB_XVsnTrgEnd    : dPos = GetMotrVsPos (_iMotr , _iPstnId              ); break ;
            case piPSB_XMrkWork      : dPos = GetMotrMrkPos(_iMotr , _iPstnId              ); break ;
        }
    }
    else if(_iMotr ==  miPSB_YMrk)
    {
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos (_iMotr                         ); break ;
            case piPSB_YMrkWait      : dPos = PM.GetValue  (_iMotr , pvPSB_YMrkWaitPs      ); break ;
            case piPSB_YMrkWorkStart : dPos = PM.GetValue  (_iMotr , pvPSB_YMrkWorkStartPs ); break ;
            case piPSB_YMrkVisn      : dPos = PM.GetValue  (_iMotr , pvPSB_YMrkVisnPs      ); break ;
            case piPSB_YMrkVisnWork  : dPos = GetMotrVsPos (_iMotr , _iPstnId              ); break ;
            case piPSB_YMrkWork      : dPos = GetMotrMrkPos(_iMotr , _iPstnId              ); break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

double CPstBuff::GetMotrMrkPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int    r  , c  ;
    int    gr , gc ;
    double dPos ;

    FindChipMrk( r , c);

    //X , Y Pos Setting.
    double dXPos =  PM.GetValue(miPSB_XMrk , pvPSB_XMrkWorkStartPs) -
                    c * OM.DevInfo.dColPitch ;

    double dYPos =  PM.GetValue(miPSB_YMrk , pvPSB_YMrkWorkStartPs) +
                    r * OM.DevInfo.dRowPitch ;


    int    dColGroupValue  = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    double dGrPitch        = OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ;
    int    iSubCnt         = 0;

    double dGrValue;
    int    iGrValueCnt;


    if( OM.DevInfo.iColGrCnt) {
        //dGrValue = dGrPitch * iGrValueCnt;
        gc = c / dColGroupValue ;

//        dXPos = dXPos + (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * c / OM.DevInfo.iColGrCnt ;
        dXPos = dXPos - (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * gc ;

    }

    if( OM.DevInfo.iRowGrCnt) {
        gr = r / OM.DevInfo.iRowGrCnt ;
//        dYPos = dYPos + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * r / OM.DevInfo.iRowGrCnt;
        dYPos = dYPos + (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * gr;

    }

    if(_iMotr == miPSB_XMrk){
        switch(_iPstnId) {
            default                    : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piPSB_XMrkWait        : dPos = PM.GetValue (_iMotr , pvPSB_XMrkWaitPs     ); break ;
            case piPSB_XMrkWorkStart   : dPos = PM.GetValue (_iMotr , pvPSB_XMrkWorkStartPs); break ;
            case piPSB_XMrkWork        : dPos = dXPos ;                                       break ;
        }
    }
    else if(_iMotr == miPSB_YMrk){
        switch(_iPstnId) {
            default                    : dPos = MT_GetCmdPos(_iMotr                        ); break ;
            case piPSB_YMrkWait        : dPos = PM.GetValue (_iMotr , pvPSB_YMrkWaitPs     ); break ;
            case piPSB_YMrkWorkStart   : dPos = PM.GetValue (_iMotr , pvPSB_YMrkWorkStartPs); break ;
            case piPSB_YMrkWork        : dPos = dYPos ;                                       break ;
        }
    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CPstBuff::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId) )) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isMarkingDn = !AT_MoveCyl(aiPSB_Marking , ccBwd ) ;

    if(_iMotr == miPSB_XMrk){
        if(isMarkingDn) { sMsg = "isMarkingDn" ; bRet = false ;}
        switch(_iPstnId) {
            default                   : break ;
            case piPSB_XMrkWait       : break ;
            case piPSB_XMrkWorkStart  : break ;
            case piPSB_XMrkVisn       : break ;
            case piPSB_XVsnTrgStt     : break ;
            case piPSB_XVsnTrgEnd     : break ;
            case piPSB_XMrkWork       : break ;
        }
    }
    else if(_iMotr == miPSB_YMrk){
        if(isMarkingDn) { sMsg = "isMarkingDn" ; bRet = false ;}
        switch(_iPstnId) {
            default                   : break ;
            case piPSB_YMrkWait       : break ;
            case piPSB_YMrkWorkStart  : break ;
            case piPSB_YMrkVisn       : break ;
            case piPSB_YMrkVisnWork   : break ;
            case piPSB_YMrkWork       : break ;
        }
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;
}

//---------------------------------------------------------------------------
bool CPstBuff::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

//    bool isMarkingDn = AT_Complete(aiPSB_Marking , ccFwd ) ;
    bool isXMoving   = MT_GetStopInpos(miPSB_XMrk);
    bool isYMoving   = MT_GetStopInpos(miPSB_YMrk);

    if(_iActr == aiPSB_Marking){
        if(!isXMoving && !isYMoving) { sMsg = "isXMrkMoving || isYMrkMoving" ; bRet = false ;}
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}
void CPstBuff::SetTrgPos()
{
    int iTrgCnt      = OM.DevInfo.iInsMrkVsColCnt; //OM.DevInfo.iColInspMrkCnt ? GetVtlColCnt() / OM.DevInfo.iColInspMrkCnt : 0;
    //int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    //int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;
    double dTemp    ;
    int r,c ;

    AnsiString sTemp ;

    double * dTrgPos = new double[iTrgCnt] ;



    FindChipVsn(r,c );

    bool bLeftToRight = PsbGetLtToRt(r);

    r = r/OM.DevInfo.iRowInspMrkCnt * OM.DevInfo.iRowInspMrkCnt ;

    for(int i = 0 ; i < iTrgCnt ; i++) {
        dTemp      = PsbGetLeftPosCt() -                                                                                                                              //첫포지션
                      OM.DevInfo.dColPitch * OM.DevInfo.iColInspMrkCnt * i -

                     (OM.DevInfo.iColGrCnt ? (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch)* OM.DevInfo.iColInspMrkCnt * (i + 1) / OM.DevInfo.iColGrCnt : 0 ) ;

                                                                                                      //빼기 검사 그룹별로
                     //(OM.DevOptn.iColInspMrkCnt * (i + 1) /
                     //(iColOneGrCnt ? OM.DevOptn.iColInspMrkCnt/2 / iColOneGrCnt : 0) * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch)) ;//그룹갭 계산.
        if(OM.DevInfo.iInsMrkVsRowCnt == 1 ) dTrgPos[iTrgCnt - 1 - i] = dTemp ;
        else if(bLeftToRight)                dTrgPos[i] = dTemp ;
        else                                 dTrgPos[iTrgCnt - 1 - i] = dTemp ;

        sTemp += AnsiString(dTemp) + AnsiString(" ") ;
    }

    Trace("PsbTrigger" , sTemp.c_str());


    MT_SetAxtTrgPos(miPSB_XMrk , iTrgCnt , dTrgPos , 1000 , true , true) ;

    delete [] dTrgPos ;
}

void CPstBuff::ResetTrgPos()
{
    MT_ResetAxtTrgPos(miPSB_XMrk) ;
}

bool CPstBuff::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CPstBuff::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CPstBuff::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 10000 )) {
        EM_SetErr(eiPSB_HomeTO);
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

    //if(Stat.bReqStop) {
    //    Step.iHome = 0;
    //    return true ;
    //}

    switch (Step.iHome) {
        default: sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                 //if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                 Step.iHome = 0 ;
                 return true ;

        case 10: MT_Reset(miPSB_XMrk);
                 MT_Reset(miPSB_YMrk);

                 MT_SetServo(miPSB_XMrk,true);
                 MT_SetServo(miPSB_YMrk,true);

                 AT_MoveCyl(aiPSB_Align    , ccBwd);
                 AT_MoveCyl(aiPSB_Lift     , ccBwd);
                 AT_MoveCyl(aiPSB_Marking  , ccBwd);
                 AT_MoveCyl(aiPSB_Pusher   , ccBwd);
                 AT_MoveCyl(aiPSB_Stopper  , ccFwd);

                 IO_SetY(yWR2_FeedingAC , false ) ;

                 Step.iHome++ ;
                 return false ;

        case 11: if(!AT_MoveCyl(aiPSB_Align    , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_Lift     , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_Marking  , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_Pusher   , ccBwd)) return false ;
                 if(!AT_MoveCyl(aiPSB_Stopper  , ccFwd)) return false ;

                 SetTrgPos  ();
                 ResetTrgPos();
                 
                 MT_DoHome(miPSB_XMrk);
                 MT_DoHome(miPSB_YMrk);

                 Step.iHome++ ;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miPSB_XMrk)) return false ;
                 if(!MT_GetHomeEnd(miPSB_YMrk)) return false ;

                 MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWaitPs)) ;
                 MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWaitPs)) ;

                 Step.iHome++ ;
                 return false ;

        case 13: if(!MT_GoAbsMan(miPSB_XMrk , PM.GetValue(miPSB_XMrk,pvPSB_XMrkWaitPs)))return false ;
                 if(!MT_GoAbsMan(miPSB_YMrk , PM.GetValue(miPSB_YMrk,pvPSB_YMrkWaitPs)))return false ;
                 AT_MoveCyl(aiPSB_Marking  , ccFwd);  // 0에 있어도 부딪힐건 없을꺼 같네요.
                 Step.iHome++ ;
                 return false ;

        case 14: if(!AT_MoveCyl(aiPSB_Marking    , ccFwd)) return false ;
                 if(!OM.CmnOptn.bVsMrkSkip) VC.SendReset(vrVisn4);
                 Step.iHome = 0;
                 return true ;
    }
}

bool CPstBuff::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //Clear Timer.
//    m_ToStopTimer.Clear();

    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CPstBuff::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CPstBuff::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPSB_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart);
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: IO_SetY(yWR2_FeedingAC , false ) ;
                 Step.iToStart ++ ;
                 return false ;

        case 11:
                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CPstBuff::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiPSB_ToStopTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStop=%02d" , Step.iToStop );
    if(Step.iToStop != PreStep.iToStop) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStop = Step.iToStop ;

    Stat.bReqStop = false ;

    bool bWaitPos = MT_CmprPos(miPSB_XMrk,GetMotrPos(miPSB_XMrk,piPSB_XMrkWait)) && MT_CmprPos(miPSB_YMrk,GetMotrPos(miPSB_YMrk,piPSB_YMrkWait)) ;

    //Move Home.
    switch (Step.iToStop) {
        default: Step.iToStop = 0;
                 return true ;

        case 10: if(RAL.GetSeqStep() != CRail::scMvAuto) IO_SetY(yWR2_FeedingAC , false ) ;
                 MoveActr(aiPSB_Align,ccBwd);
                 MoveActr(aiPSB_Lift ,ccBwd);
                 Step.iToStop++ ;
                 return false ;

        case 11: if(!MoveActr(aiPSB_Align,ccBwd)) return false ;
                 if(!MoveActr(aiPSB_Lift ,ccBwd)) return false ;
                 if(!bWaitPos) { MoveActr(aiPSB_Marking  , ccBwd); }
                 Step.iToStop++ ;
                 return false ;

        case 12: if(!bWaitPos) { if(!MoveActr(aiPSB_Marking    , ccBwd)) return false ; }
                 if(!bWaitPos) { MT_GoAbsMan(miPSB_XMrk , GetMotrPos(miPSB_XMrk,piPSB_XMrkWait)) ; }
                 if(!bWaitPos) { MT_GoAbsMan(miPSB_YMrk , GetMotrPos(miPSB_YMrk,piPSB_YMrkWait)) ; }

                 Step.iToStop++ ;
                 return false ;

        case 13: if(!bWaitPos) { if(!MT_GoAbsMan(miPSB_XMrk , GetMotrPos(miPSB_XMrk,piPSB_XMrkWait)))return false ; }
                 if(!bWaitPos) { if(!MT_GoAbsMan(miPSB_YMrk , GetMotrPos(miPSB_YMrk,piPSB_YMrkWait)))return false ; }
                 MoveActr(aiPSB_Marking  , ccFwd);  // 0에 있어도 부딪힐건 없을꺼 같네요.
                 Step.iToStop++ ;
                 return false ;

        case 14: if(!MoveActr(aiPSB_Marking    , ccFwd)) return false ;
                 Step.iToStop = 0   ;
                 return true ;
    }
}


bool CPstBuff::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
    if(Step.iSeq != PreStep.iSeq) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        bool isExstFail = DM.ARAY[riPSB].GetCntStat(csFail  ) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt0 ) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt1 ) && !OM.CmnOptn.bR1FailSkip ) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt2 )  && !OM.CmnOptn.bR2FailSkip ) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt3 )  && !OM.CmnOptn.bR3FailSkip ) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt4 )  && !OM.CmnOptn.bR4FailSkip ) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt5 )  && !OM.CmnOptn.bR5FailSkip ) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt6 )  && !OM.CmnOptn.bR6FailSkip ) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt7 )  && !OM.CmnOptn.bR7FailSkip ) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt8 )  && !OM.CmnOptn.bR8FailSkip ) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt9 )  && !OM.CmnOptn.bR9FailSkip ) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt10)  && !OM.CmnOptn.bR10FailSkip) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt11)  && !OM.CmnOptn.bR11FailSkip) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt12)  && !OM.CmnOptn.bR12FailSkip) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt13)  && !OM.CmnOptn.bR13FailSkip) ||
                          (DM.ARAY[riPSB].GetCntStat(csRslt14)  && !OM.CmnOptn.bR14FailSkip) ||
                          DM.ARAY[riPSB].GetCntStat(csRslt15)  ||
                          DM.ARAY[riPSB].GetCntStat(csRslt16)  ;

        bool isNeedVsn    = !DM.ARAY[riPSB].CheckAllStat(csNone) &&  DM.ARAY[riVS4].GetCntStat(csUnkwn)  && isExstFail && !OM.CmnOptn.bVsMrkSkip ;
        bool isNeedMrk    = !DM.ARAY[riPSB].CheckAllStat(csNone) && !DM.ARAY[riPSB].CheckAllStat(csWork) && isExstFail && !OM.CmnOptn.bMrkAllSkip ;

        bool isCycleReady = !DM.ARAY[riPSB].GetCntExist() &&(!AT_Complete(aiPSB_Align    , ccBwd)||
                                                             !AT_Complete(aiPSB_Lift     , ccBwd)||
                                                             !AT_Complete(aiPSB_Marking  , ccFwd)||
                                                             !AT_Complete(aiPSB_Pusher   , ccBwd)||
                                                             !AT_Complete(aiPSB_Stopper  , ccFwd)) ;
//                                                              RAL.GetSeqStep() != CRail::scMvAuto;
//        if(DM.ARAY[riPSB].GetCntExist() && IO_GetY(yWR2_FeedingAC) && IO_GetX(xPSB_Pkg)) IO_SetY(yWR2_FeedingAC,false) ; //oh ;;;nn

        bool isCycleBind  =( isNeedMrk || isNeedVsn ) && IO_GetX(xPSB_Pkg) && AT_Complete(aiPSB_Lift,ccBwd) ;
        bool isCycleWork  =  isNeedMrk && !isNeedVsn  && AT_Complete(aiPSB_Lift,ccFwd);
        bool isCycleVisn  =  isNeedVsn                && AT_Complete(aiPSB_Lift,ccFwd);

        bool isCycleOut   = !DM.ARAY[riPSB].CheckAllStat(csNone) && (!isExstFail || OM.CmnOptn.bMrkAllSkip) &&  DM.ARAY[riULD].GetCntStat(csEmpty) &&
                            ((IO_GetX(xPSB_Pkg) || IO_GetX(xPSB_PkgOut3) || IO_GetX(xPSB_PkgOut4) || AT_Complete(aiPSB_Lift,ccFwd) ) ) &&
                             RAL.GetSeqStep() != CRail::scMvAuto &&
                             ULD.GetSeqStep() == CUnLoader::scIdle ;
        bool isConEnd     =  DM.ARAY[riPSB].CheckAllStat(csNone) && !IO_GetX(xPSB_PkgOut3) && !IO_GetX(xPSB_PkgOut4);

        //모르는 스트립에러.
        if( DM.ARAY[riPSB ].CheckAllStat(csNone) &&  IO_GetX(xPSB_Pkg) ) EM_SetErr(eiPSB_PkgUnknown ) ;

        //스트립 사라짐 에러.
        if(!DM.ARAY[riPSB ].CheckAllStat(csNone) && RAL.GetSeqStep() != CRail::scMvAuto && (!IO_GetX(xPSB_Pkg) && !IO_GetX(xPSB_PkgOut3) && !IO_GetX(xPSB_PkgOut4))&& AT_Complete(aiPSB_Lift , ccBwd) &&!OM.CmnOptn.bDryRun) EM_SetErr(eiPSB_PkgDispr ) ;

        if(EM_IsErr()) return false ;

        //Normal Decide Step.
             if (isCycleReady) {Trace(m_sPartName.c_str(),"CycleReady Stt"); Step.iSeq = scReady ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleBind ) {Trace(m_sPartName.c_str(),"CycleBind  Stt"); Step.iSeq = scBind  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleVisn ) {Trace(m_sPartName.c_str(),"CycleVisn  Stt"); Step.iSeq = scVisn  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleWork ) {Trace(m_sPartName.c_str(),"CycleWork  Stt"); Step.iSeq = scWork  ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isCycleOut  ) {Trace(m_sPartName.c_str(),"CycleOut   Stt"); Step.iSeq = scOut   ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default      :              /*Trace(m_sPartName.c_str(),"default    End");Step.iSeq = scIdle ;*/  return false ;
        case  scReady: if(CycleReady()){Trace(m_sPartName.c_str(),"CycleReady End");Step.iSeq = scIdle ;} return false ;
        case  scBind : if(CycleBind ()){Trace(m_sPartName.c_str(),"CycleBind  End");Step.iSeq = scIdle ;} return false ;
        case  scVisn : if(CycleVisn ()){Trace(m_sPartName.c_str(),"CycleVisn  End");Step.iSeq = scIdle ;} return false ;
        case  scWork : if(CycleWork ()){Trace(m_sPartName.c_str(),"CycleWork  End");Step.iSeq = scIdle ;} return false ;
        case  scOut  : if(CycleOut  ()){Trace(m_sPartName.c_str(),"CycleOut   End");Step.iSeq = scIdle ;} return false ;
    }
}
bool CPstBuff::CycleReady(void) //스트립 Ready.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
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
        //Step.iCycle = 0;
        //return true ;
    }

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSB_Align   , ccBwd              );
                  MoveActr(aiPSB_Lift    , ccBwd              );
                  MoveActr(aiPSB_Marking , ccBwd              );
                  MoveActr(aiPSB_Pusher  , ccBwd              );
                  MoveActr(aiPSB_Stopper , ccFwd              );
//                  if(AT_Complete(aiPSB_Marking,ccBwd)
                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSB_Align   , ccBwd              )) return false ;
                  if(!MoveActr(aiPSB_Lift    , ccBwd              )) return false ;
                  if(!MoveActr(aiPSB_Marking , ccBwd              )) return false ;
                  if(!MoveActr(aiPSB_Pusher  , ccBwd              )) return false ;
                  if(!MoveActr(aiPSB_Stopper , ccFwd              )) return false ;
                  MoveMotr(miPSB_XMrk,piPSB_XMrkWait);
                  MoveMotr(miPSB_YMrk,piPSB_YMrkWait);
//                  MT_GoAbsMan(miPSB_XMrk , GetMotrPos(miPSB_XMrk,piPSB_XMrkWait)) ;
//                  MT_GoAbsMan(miPSB_YMrk , GetMotrPos(miPSB_YMrk,piPSB_YMrkWait)) ;
                  Step.iCycle++;
                  return false ;

        case 12 : //if(!MT_GoAbsMan(miPSB_XMrk , GetMotrPos(miPSB_XMrk,piPSB_XMrkWait)))return false ;
                  //if(!MT_GoAbsMan(miPSB_YMrk , GetMotrPos(miPSB_YMrk,piPSB_YMrkWait)))return false ;
                  if(!MoveMotr(miPSB_XMrk,piPSB_XMrkWait)) return false; 
                  if(!MoveMotr(miPSB_YMrk,piPSB_YMrkWait)) return false; 
                  MoveActr(aiPSB_Marking  , ccFwd);  // 0에 있어도 부딪힐건 없을꺼 같네요.
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSB_Marking    , ccFwd)) return false ;
                  Step.iCycle = 0;
                  return true ;


    }
}

bool CPstBuff::CycleBind(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

     bool r1 , r2 , r3 , r4 ;
//     bool NotUseVision ;
     static bool bNeedVsn = false ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: //NotUseVision = OM.CmnOptn.bVsMrkSkip ;
                  bNeedVsn    = DM.ARAY[riVS4].GetCntStat(csUnkwn) && !OM.CmnOptn.bVsMrkSkip ;
                  MoveActr(aiPSB_Stopper , ccFwd);
                  MoveActr(aiPSB_Marking , ccBwd);
                  MoveActr(aiPSB_Align   , ccBwd);

                  Step.iCycle++;
                  return false ;

        case  11: if(!MoveActr(aiPSB_Stopper , ccFwd)) return false ;
                  if(!MoveActr(aiPSB_Marking , ccBwd)) return false ;
                  if(!MoveActr(aiPSB_Align   , ccBwd)) return false ;
                  if(!bNeedVsn  ) MoveMotr(miPSB_XMrk , piPSB_XMrkWork); //비전 아니면 마킹 위치로 가있기.
                  if(!bNeedVsn  ) MoveMotr(miPSB_YMrk , piPSB_YMrkWork);
                  if(bNeedVsn   ) MoveMotr(miPSB_XMrk,piPSB_XVsnTrgStt     ); //비전 이면 비전 위치로 가있기.
                  if(bNeedVsn   ) MoveMotr(miPSB_YMrk,piPSB_YMrkVisnWork   );

                  IO_SetY(yWR2_FeedingAC , true);
                  m_tmTemp.Clear();

                  Step.iCycle++;
                  return false ;

        case  12: if(!m_tmTemp.OnDelay(true , 300)) return false ;
//                  IO_SetY(yWR2_FeedingAC , false);

                  MoveActr(aiPSB_Align , ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  13: if(!MoveActr(aiPSB_Align , ccFwd)) return false ;

                  MoveActr(aiPSB_Lift , ccFwd);

                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveActr(aiPSB_Lift , ccFwd)) return false ;
                  IO_SetY(yWR2_FeedingAC , false);
                  MoveActr(aiPSB_Align , ccBwd);
                  Step.iCycle++ ;
                  return false ;

        case  15: if(!MoveActr(aiPSB_Align , ccBwd)) return false ;
                  if(!bNeedVsn  ) if(!MoveMotr(miPSB_XMrk , piPSB_XMrkWork     )) return false ;
                  if(!bNeedVsn  ) if(!MoveMotr(miPSB_YMrk , piPSB_YMrkWork     )) return false ;
                  if(bNeedVsn   ) if(!MoveMotr(miPSB_XMrk,piPSB_XVsnTrgStt     )) return false ;
                  if(bNeedVsn   ) if(!MoveMotr(miPSB_YMrk,piPSB_YMrkVisnWork   )) return false ;

                  Step.iCycle = 0 ;
                  return true ;
    }
}


//One Cycle.
bool CPstBuff::CycleVisn(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
//        if(Step.iCycle == 17) {
//무조건 리셋 하고 
            //if(!OM.CmnOptn.bVsMrkSkip ) VC.SendReset(vrVisn4 ) ;

//            if(DM.ARAY[riVB4].GetCntExist()) DM.ShiftArrayData(riVB4,riVS4);

            if(DM.ARAY[riPSB].GetCntExist())
            {
//                DM.ARAY[riPSB].ChangeStat(csWork,csUnkwn) ;
                DM.ARAY[riVS4].ChangeStat(csWork, csUnkwn);
            }

//        }
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    //if(Stat.bReqStop) {
    //    Step.iCycle = 0;
    //    return true ;
    //}

    int r,c ;

    int iFailCnt ;
    bool r1 , r2 , r3 , r4 ;

    //메뉴얼 동작중에 에러 뜨면 메뉴얼이 중간에 멈춘다.
    bool bManualInsp = !Step.iSeq ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: MoveActr(aiPSB_Marking,ccBwd);
                  Step.iCycle++;
                  return false ;


        case  11: if(!MoveActr(aiPSB_Marking ,ccBwd)) return false ;
                  MoveMotr(miPSB_XMrk,piPSB_XVsnTrgStt );
                  MoveMotr(miPSB_YMrk,piPSB_YMrkVisnWork   );
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miPSB_XMrk,piPSB_XVsnTrgStt )) return false ;
                  if(!MoveMotr(miPSB_YMrk,piPSB_YMrkVisnWork   )) return false ;

                  ResetTrgPos() ; //트리거 세팅 클리어
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!IO_GetX(xHED_4Ready)&&!OM.CmnOptn.bVsMrkSkip){
                      EM_SetErr(eiHED_Visn4NotReady) ;
                      Step.iCycle = 0 ;
                      return true;
                  }

                  SetTrgPos(); //트리거 세팅.
                  //MoveMotr(miPSB_XMrk,piPSB_XVsnTrgEnd );
                  MT_GoAbsMan(miPSB_XMrk , GetMotrPos(miPSB_XMrk,piPSB_XVsnTrgEnd));
                  //MT_GoAbsRun(miWRK_XVsn , 13 ) ;
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveMotr(miPSB_XMrk,piPSB_XVsnTrgEnd )) return false ;
                  ResetTrgPos() ; //트리거 세팅 클리어
                  Step.iCycle++ ;
                  return false ;

        case  15: FindChipVsn(r , c ) ;
                  r = OM.DevInfo.iRowCnt - r/OM.DevInfo.iRowInspMrkCnt * OM.DevInfo.iRowInspMrkCnt -1;

//                  r = r/OM.DevInfo.iRowInspMrkCnt * OM.DevInfo.iRowInspMrkCnt ;
                  for(c = 0 ; c < OM.DevInfo.iColCnt ; c++ ) {
                      for(int i = 0 ; i < OM.DevInfo.iRowInspMrkCnt ; i ++) {
                          DM.ARAY[riVS4].SetStat(r-i,c,csWork) ;
                      }          //선택해놓은게 밑에서 2번째 이면 안되려나?
                  }


                  r1 = (/*DM.ARAY[riWK1].CheckAllExist() && */DM.ARAY[riVS4].CheckAllStat(csUnkwn));

                  //Stat.bReqAllInsp = true ;
                  if( r1 ) {
                      Step.iCycle=11 ; //Next Row Insp
                      return false ;

                  }
                  Step.iCycle++ ;
                  return false ;

        case  16: if(!OM.CmnOptn.bVsMrkSkip && !IO_GetX(xHED_4End)) return false ;
                  if(VC.SimpleReadResult(4)) {
                      if(!OM.MstOptn.bPsbVsErrIgnr) EM_SetErr(eiPSB_VsnFailCnt);
                  }
                  iFailCnt = VC.SimpleReadResult(4) ;
                  if(iFailCnt == -1) EM_SetErr(eiHED_Visn4Read);
                  Step.iCycle = 0 ;
                  return true;
        }

/*
        case  16: //한스트립을 다 검사했을 경우.
//                  MoveMotr(miPSB_YMrk,piPSB_YMrkVisn) ;
//                  MoveMotr(miPSB_XMrk,piPSB_XMrkVisn) ;
                  Step.iCycle++ ;
                  return false ;

        //17 Step Used When TimeOut
        case  17: //if(!MoveMotr(miPSB_YMrk,piPSB_YMrkVisn)) return false ;
                  //if(!MoveMotr(miPSB_XMrk,piPSB_XMrkVisn)) return false ;
                  if(!OM.CmnOptn.bVsMrkSkip && !IO_GetX(xHED_4End)) return false ;

                  VC.ClearErrCnt();

                  //Vision 1
                  if(OM.CmnOptn.bVsMrkSkip) {
                      if(DM.ARAY[riVS4].CheckAllExist())DM.ARAY[riVS4].ChangeStat(csUnkwn,csNone) ;
                  }
                  else {
                      if(DM.ARAY[riPSB].CheckAllExist()){
                          if(!VC.ReadResult(4,riPSB)){ //비젼 데이터 읽어 오기.
                              if(!bManualInsp)EM_SetErr(eiHED_Visn4Read);
                              DM.ShiftArrayData(riVB4 , riVS4);
                              VC.SendReset(vrVisn4 , true); //쓰레드를 켜서 비젼 리셑.
                              Step.iCycle=0 ;
                              return true ;
                          }
                      }

//                      //토탈 페일 갯수.
//                      if( OM.CmnOptn.iCheckFailCnt && !DM.ARAY[riWR1].CheckAllStat(csNone) ) {
//                          if(VC.GetLastTtlErrCnt(1)>= OM.CmnOptn.iCheckFailCnt && !bManualInsp) EM_SetErr(eiWK1_VsnFailCnt);
//                      }
//
//                      //같은 에러 갯수.
//                      bool bCheckSFailCnt = OM.CmnOptn.iCheckR0FailCnt || OM.CmnOptn.iCheckR1FailCnt || OM.CmnOptn.iCheckR2FailCnt || OM.CmnOptn.iCheckR3FailCnt || OM.CmnOptn.iCheckR4FailCnt ||
//                                            OM.CmnOptn.iCheckR5FailCnt || OM.CmnOptn.iCheckR6FailCnt || OM.CmnOptn.iCheckR7FailCnt || OM.CmnOptn.iCheckR8FailCnt || OM.CmnOptn.iCheckR9FailCnt ||
//                                            OM.CmnOptn.iCheckR10FailCnt|| OM.CmnOptn.iCheckR11FailCnt|| OM.CmnOptn.iCheckR12FailCnt|| OM.CmnOptn.iCheckR13FailCnt|| OM.CmnOptn.iCheckR14FailCnt||
//                                            OM.CmnOptn.iCheckR15FailCnt|| OM.CmnOptn.iCheckR16FailCnt ;
//
//                      if( bCheckSFailCnt && !DM.ARAY[riWR1].CheckAllStat(csNone)&& !bManualInsp ) {
//                          if(VC.GetLastErrCnt(1,csRslt0 )>= OM.CmnOptn.iCheckR0FailCnt ) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt1 )>= OM.CmnOptn.iCheckR1FailCnt ) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt2 )>= OM.CmnOptn.iCheckR2FailCnt ) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt3 )>= OM.CmnOptn.iCheckR3FailCnt ) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt4 )>= OM.CmnOptn.iCheckR4FailCnt ) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt5 )>= OM.CmnOptn.iCheckR5FailCnt ) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt6 )>= OM.CmnOptn.iCheckR6FailCnt ) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt7 )>= OM.CmnOptn.iCheckR7FailCnt ) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt8 )>= OM.CmnOptn.iCheckR8FailCnt ) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt9 )>= OM.CmnOptn.iCheckR9FailCnt ) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt10)>= OM.CmnOptn.iCheckR10FailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt11)>= OM.CmnOptn.iCheckR11FailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt12)>= OM.CmnOptn.iCheckR12FailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt13)>= OM.CmnOptn.iCheckR13FailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt14)>= OM.CmnOptn.iCheckR14FailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt15)>= OM.CmnOptn.iCheckR15FailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
//                          if(VC.GetLastErrCnt(1,csRslt16)>= OM.CmnOptn.iCheckR16FailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
//                      }

                  }

                  Step.iCycle=0 ;
                  return true  ;
*/
}


//One Cycle.
bool CPstBuff::CycleWork(void) //Marking
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 15000 )) {
        EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //Step.iCycle = 0;
        //return true ;
    }

    bool r1 , r2 , r3 , r4 ;
    int  iCol , iRow ;
    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSB_Marking,ccBwd);
                  Step.iCycle++;
                  return false ;


        case 11 : if(!MoveActr(aiPSB_Marking ,ccBwd)) return false ;
                  MoveMotr(miPSB_XMrk , piPSB_XMrkWork);
                  MoveMotr(miPSB_YMrk , piPSB_YMrkWork);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!MoveMotr(miPSB_XMrk , piPSB_XMrkWork)) return false ;
                  if(!MoveMotr(miPSB_YMrk , piPSB_YMrkWork)) return false ;
                  MoveActr(aiPSB_Marking ,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSB_Marking ,ccFwd)) return false ;
                  MoveActr(aiPSB_Marking ,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 14 : if(!MoveActr(aiPSB_Marking ,ccBwd)) return false ;
                  FindChipMrk( iRow , iCol) ;
                  DM.ARAY[riPSB].SetStat(iRow,iCol,csWork) ;
                  Step.iCycle=0;
                  return true ;
    }
}

bool CPstBuff::CycleOut(void) //자제 아웃.
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiPSB_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        Step.iCycle = 0 ;
        return true;
    }

    if(Step.iCycle != PreStep.iCycle) {
        sTemp = sTemp.sprintf("%s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iCycle = Step.iCycle ;

    if(Stat.bReqStop) {
        //Step.iCycle = 0 ;
        //return true ;
    }

    bool r1 , r2 , r3 , r4 ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case 10 : MoveActr(aiPSB_Align   , ccBwd          );
                  MoveActr(aiPSB_Lift    , ccBwd          );
                  MoveActr(aiPSB_Stopper , ccFwd          );

                  if(AT_Complete(aiPSB_Marking , ccBwd)) MoveMotr(miPSB_XMrk,piPSB_XMrkWait);
                  if(AT_Complete(aiPSB_Marking , ccBwd)) MoveMotr(miPSB_YMrk,piPSB_YMrkWait);


                  Step.iCycle++;
                  return false ;

        case 11 : if(!MoveActr(aiPSB_Align   , ccBwd          )) return false ;
                  if(!MoveActr(aiPSB_Lift    , ccBwd          )) return false ;
                  if(!MoveActr(aiPSB_Stopper , ccFwd          )) return false ;
                  IO_SetY(yWR2_FeedingAC,true);
                  Step.iCycle++;
                  return false ;

        case 12 : if(!ULD.GetStat().bReadyMgz || ULD.GetSeqStep() != CUnLoader :: scIdle) return false ;
                  MoveActr(aiPSB_Stopper   ,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 13 : if(!MoveActr(aiPSB_Stopper   ,ccBwd)) return false ;

                  //스토퍼 다운되면 ...데이터 넘김.
                  LT.WriteArayData(riLOT); //애 워크 이겟다. SPC 용 데이트 남김. 자재 뺄시 안남기려고 여기서 함.
                  LotInfoAdd ();
                  DM.ARAY[riPSB].SetStat(csNone) ;
                  if(DM.ARAY[riULD].CheckAllStat(csEmpty)) {
                      DM.ARAY[riULD].SetLotNo(DM.ARAY[riPSB].GetLotNo());
                      DM.ARAY[riULD].SetID(DM.ARAY[riPSB].GetID());
                  }
                  DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);

                  Step.iCycle++;
                  return false ;


        case 14 : if(!IO_GetX(xPSB_Pkg) || OM.CmnOptn.bDryRun) MoveActr(aiPSB_Stopper , ccFwd) ;
                  if(!AT_GetCmd(aiPSB_Stopper)) return false ;
                  Step.iCycle++;
                  return false ;

        case 15 : if(!MoveMotr(miPSB_XMrk,piPSB_XMrkWait)) return false ;
                  if(!MoveMotr(miPSB_YMrk,piPSB_YMrkWait)) return false ;
                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false;

        case 16 : if(m_tmTemp.OnDelay(true, 3000)){
                      EM_SetErr(eiPSB_SensorErr);
                      Step.iCycle = 0;
                      return true;
                  }
                  if(!IO_GetX(xPSB_PkgOut4)) return false ;
                  MoveActr(aiPSB_Marking , ccFwd);

                  m_tmTemp.Clear();
                  Step.iCycle++;
                  return false ;

        case 17 : if(m_tmTemp.OnDelay(true , OM.CmnOptn.iPsbToOutDelay)) {
                      EM_SetErr(eiPSB_OutStripExist);
                      //스토퍼 올라가기 시작 하면 레일에서 ::CycleMvAuto를 타고 있어서 데이터가 들어온다.
                      //무브 오토시에 레일을 끄면 끝까지 동작이 안됨.
                      if(!DM.ARAY[riPSB].GetCntExist() && RAL.GetSeqStep() != CRail::scMvAuto)IO_SetY(yWR2_FeedingAC,false);
                      return false ;
                  }
                  if(IO_GetX(xPSB_PkgOut4)) return false ;
 /*
        case 15 : if(!MoveActr(aiPSB_Stopper ,ccFwd)) return false ;
                  if(m_tmTemp.OnDelay(true , OM.CmnOptn.iPsbToOutDelay)) {
                      EM_SetErr(eiPSB_OutStripExist);
                      IO_SetY(yWR2_FeedingAC,false);
                      return false ;
                  }

                  if(IO_GetX(xPSB_PkgOut4)||IO_GetX(xPSB_Pkg) )return false ;//Doking ...X
 */
                  //스토퍼 올라가기 시작 하면 레일에서 ::CycleMvAuto를 타고 있어서 데이터가 들어온다.
                  //무브 오토시에 레일을 끄면 끝까지 동작이 안됨.
                  if(!DM.ARAY[riPSB].GetCntExist() && RAL.GetSeqStep() != CRail::scMvAuto)IO_SetY(yWR2_FeedingAC,false);
                  MoveActr(aiPSB_Pusher,ccFwd);
                  Step.iCycle++;
                  return false ;

        case 18 : if(!MoveActr(aiPSB_Pusher , ccFwd)) return false ;
                  if(IO_GetX(xPSB_PkgOut3) || IO_GetX(xPSB_PkgOut4)){
                      EM_SetErr(eiPSB_PusherMiss);
                      MoveActr(aiPSB_Pusher,ccBwd);
                      return false ;
                  }
//                  DM.ARAY[riPSB].SetStat(csNone) ;
//                  if(DM.ARAY[riULD].CheckAllStat(csEmpty)) {
//                      DM.ARAY[riULD].SetLotNo(DM.ARAY[riPSB].GetLotNo());
//                      DM.ARAY[riULD].SetID(DM.ARAY[riPSB].GetID());
//                  }
//                  DM.ARAY[riULD].SetStat(DM.ARAY[riULD].FindFrstRow(csEmpty),0 , csWork);

                  MoveActr(aiPSB_Pusher,ccBwd);
                  Step.iCycle++;
                  return false ;

        case 19 : if(!MoveActr(aiPSB_Pusher , ccBwd)) return false ;
                  Step.iCycle = 0;
                  return true ;

    }
}
bool CPstBuff::LotInfoAdd  ()
{
//    DM.ARAY[riLOT].ChangeStat(csUnkwn,csWork);
    LT.AddDayInfoWorkStrp(1);
    LT.AddDayInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

    LT.AddLotInfoWorkStrp(1);
    LT.AddLotInfoWorkChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt);

    LT.AddDayInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riLOT].GetCntStat(csWork));
    LT.AddLotInfoFailChip(OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riLOT].GetCntStat(csWork));
//    iVsFailCnt         = OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riLOT].GetCntStat(csWork);
    OM.iTotalChip     += OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt ;
    OM.iTotalFailChip += OM.DevInfo.iColCnt * OM.DevInfo.iRowCnt - DM.ARAY[riLOT].GetCntStat(csWork) ;

    if(OM.iTotalChip < 0 || OM.iTotalFailChip < 0) {//OverFlow
        OM.iTotalChip     = 0 ;
        OM.iTotalFailChip = 0 ;
    }
    LT.AddDayInfoChips(DM.ARAY[riLOT].GetCntStat(csRslt1 ) , //"V Empty"
                       DM.ARAY[riLOT].GetCntStat(csRslt2 ) , //"V Chip"
                       DM.ARAY[riLOT].GetCntStat(csRslt3 ) , //"V Dust"
                       DM.ARAY[riLOT].GetCntStat(csRslt4 ) , //"V LFlow"
                       DM.ARAY[riLOT].GetCntStat(csRslt5 ) , //"V Broken"
                       DM.ARAY[riLOT].GetCntStat(csRslt6 ) , //"V Crack"
                       DM.ARAY[riLOT].GetCntStat(csRslt7 ) , //"V Burr"
                       DM.ARAY[riLOT].GetCntStat(csRslt8 ) , //"V Zener"
                       DM.ARAY[riLOT].GetCntStat(csRslt9 ) , //"V Wire"
                       DM.ARAY[riLOT].GetCntStat(csRslt10) , //"V Orient"
                       DM.ARAY[riLOT].GetCntStat(csRslt11) , //"V Pmeasure"
                       DM.ARAY[riLOT].GetCntStat(csRslt12) , //"V Pdist"
                       DM.ARAY[riLOT].GetCntStat(csRslt13) , //"V Match"
                       DM.ARAY[riLOT].GetCntStat(csRslt14) , //"V Flow"
                       DM.ARAY[riLOT].GetCntStat(csFail  ) ); //"Heat Fail");

    LT.AddLotInfoChips(DM.ARAY[riLOT].GetCntStat(csRslt1 ) , //"V Empty"
                       DM.ARAY[riLOT].GetCntStat(csRslt2 ) , //"V Chip"
                       DM.ARAY[riLOT].GetCntStat(csRslt3 ) , //"V Dust"
                       DM.ARAY[riLOT].GetCntStat(csRslt4 ) , //"V LFlow"
                       DM.ARAY[riLOT].GetCntStat(csRslt5 ) , //"V Broken"
                       DM.ARAY[riLOT].GetCntStat(csRslt6 ) , //"V Crack"
                       DM.ARAY[riLOT].GetCntStat(csRslt7 ) , //"V Burr"
                       DM.ARAY[riLOT].GetCntStat(csRslt8 ) , //"V Zener"
                       DM.ARAY[riLOT].GetCntStat(csRslt9 ) , //"V Wire"
                       DM.ARAY[riLOT].GetCntStat(csRslt10) , //"V Orient"
                       DM.ARAY[riLOT].GetCntStat(csRslt11) , //"V Pmeasure"
                       DM.ARAY[riLOT].GetCntStat(csRslt12) , //"V Pdist"
                       DM.ARAY[riLOT].GetCntStat(csRslt13) , //"V Match"
                       DM.ARAY[riLOT].GetCntStat(csRslt14) ,
                       DM.ARAY[riLOT].GetCntStat(csFail  ) ); //"Heat Fail");//"V Flow"
                  //!DM.ARAY[riWK2].CheckAllStat(csNone) &&

    if(GetLastStrip()) {
        m_bLotEnded = true ;
        memcpy(&EndedLot , &LT.LotInfo , sizeof(CLot::SLotInfo));

        Trace("LotEnd",DM.ARAY[riLOT].GetLotNo().c_str());
        LT.LotInfo.dEndTime = Now();
        LT.WriteLotLog   ();
        LT.WriteLotDayLog();
        Trace("WriteLotLog",DM.ARAY[riLOT].GetLotNo().c_str());
        //LT.LotEnd();
    }
    DM.ARAY[riLOT].SetStat(csNone) ;
}
bool CPstBuff::GetLastStrip()
{
    bool bLDRExist = !DM.ARAY[riLDR].CheckAllStat(csNone) ;
    bool bPRBExist = !DM.ARAY[riPRB].CheckAllStat(csNone) ;
    bool bWR1Exist = !DM.ARAY[riWR1].CheckAllStat(csNone) ;
    bool bWR2Exist = !DM.ARAY[riWR2].CheckAllStat(csNone) ;
    bool bWR3Exist = !DM.ARAY[riWR3].CheckAllStat(csNone) ;

    bool bLDRDiff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riLDR].GetLotNo() ;
    bool bPRBDiff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riPRB].GetLotNo() ;
    bool bWR1Diff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riWR1].GetLotNo() ;
    bool bWR2Diff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riWR2].GetLotNo() ;
    bool bWR3Diff  = DM.ARAY[riPSB].GetLotNo() != DM.ARAY[riWR3].GetLotNo() ;

    bool bLastMgz  = LDR._iMgzCnt >= OM.DevOptn.iLotEndMgzCnt ;

         if(bWR3Exist) return bWR3Diff ;
    else if(bWR2Exist) return bWR2Diff ;
    else if(bWR1Exist) return bWR1Diff ;
    else if(bPRBExist) return bPRBDiff ;
    else if(bLDRExist) return bLDRDiff ;
    else if(!bLastMgz) return false    ;
    else               return true     ;
}


void CPstBuff::SetLastCmd()
{
    return ; //이파트는 필요 없다.

}

bool CPstBuff::CheckMoved()
{
    return true ; //이파트는 필요 없다.
}

bool CPstBuff::CheckStop()
{
    if(!AT_Done(aiPSB_Align  )) return false ;
    if(!AT_Done(aiPSB_Lift   )) return false ;
    if(!AT_Done(aiPSB_Marking)) return false ;
    if(!AT_Done(aiPSB_Pusher )) return false ;
    if(!AT_Done(aiPSB_Stopper)) return false ;

    if(!MT_GetStop(miPSB_XMrk)) return false ;
    if(!MT_GetStop(miPSB_YMrk)) return false ;

    return true ;
}

void CPstBuff::Load(bool IsLoad )
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    if(IsLoad) {
        UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded " , &m_bLotEnded  );
    }
    else {
        UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "m_bLotEnded " ,  m_bLotEnded  );
    }
}

















