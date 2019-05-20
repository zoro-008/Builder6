//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Head.h"
#include "Rail.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CHead  HED;

enum {idLeftTop = 0 , idLeftBtm = 1 };
int INSP_DIRECTION = idLeftBtm ;



int GetVtlColCnt ()
{
    int iRet = 0 ;
    if(OM.DevOptn.iColInspCnt) {
        iRet = (OM.DevInfo.iColCnt % OM.DevOptn.iColInspCnt) ?
              ((OM.DevInfo.iColCnt / OM.DevOptn.iColInspCnt) * OM.DevOptn.iColInspCnt + OM.DevOptn.iColInspCnt) :
              ((OM.DevInfo.iColCnt / OM.DevOptn.iColInspCnt) * OM.DevOptn.iColInspCnt) ;
    }
    return iRet ;
}
int GetVtlRowCnt ()
{
    int iRet = 0 ;
    if(OM.DevOptn.iColInspCnt) {
        iRet = (OM.DevInfo.iRowCnt % OM.DevOptn.iRowInspCnt) ?
              ((OM.DevInfo.iRowCnt / OM.DevOptn.iRowInspCnt) * OM.DevOptn.iRowInspCnt + OM.DevOptn.iRowInspCnt) :
              ((OM.DevInfo.iRowCnt / OM.DevOptn.iRowInspCnt) * OM.DevOptn.iRowInspCnt) ;
    }
    return iRet ;
}

double GetLeftPos ()
{
    double dRet = (PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkSttPs) + (OM.DevOptn.iColInspCnt - 1) * OM.DevInfo.dColPitch / 2.0) ;
    return dRet ;
}
double GetRightPos ()
{
    int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;
    double dRet = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkSttPs) +
                  OM.DevInfo.dColPitch * GetVtlColCnt() +
                  (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) *        (iColOneGrCnt ? (GetVtlColCnt()/iColOneGrCnt) : 0) +
                  (OM.DevOptn.iColInspCnt - 1) * OM.DevInfo.dColPitch / 2.0 ;
    return dRet ;
}

double GetLeftPosCt()
{
    double dRet = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkSttPs) ;
    return dRet ;
}

double GetRightPosCt()
{
    int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;

    double dRet = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkSttPs) + OM.DevInfo.dColPitch * GetVtlColCnt() +
                  (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * (iColOneGrCnt ? (GetVtlColCnt()/iColOneGrCnt) : 0) -
                  OM.DevOptn.iColInspCnt * OM.DevInfo.dColPitch ;
                  //(OM.DevOptn.iColInspCnt - 1) * OM.DevInfo.dColPitch / 2.0 ;
    return dRet ;
}

bool GetLtToRt(int r)
{
    if(INSP_DIRECTION == idLeftTop) return !((OM.DevOptn.iRowInspCnt ? r/OM.DevOptn.iRowInspCnt : r) %2);
    else                            return  ((OM.DevOptn.iRowInspCnt ? r/OM.DevOptn.iRowInspCnt : r) %2);
}




CHead::CHead(void)
{
    m_sPartName = "Head " ;
    Reset();
}

void CHead::Reset()
{
    ResetTimer();

    //Init. Buffers
    memset(&Stat    , 0 , sizeof(SStat ));
    memset(&Step    , 0 , sizeof(SStep ));
    memset(&PreStep , 0 , sizeof(SStep ));
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
}

CHead::~CHead (void)
{
    Trace("","");
}

bool CHead::FindChip( int &r , int &c )
{
    int r1 , c1 , r2 , c2 ;

    if(INSP_DIRECTION == idLeftTop) {
        r = DM.ARAY[riVS1].GetMaxRow() ;
        c = DM.ARAY[riVS1].GetMaxCol() ;

        r1 = DM.ARAY[riVS1].GetMaxRow() ;
        c1 = DM.ARAY[riVS1].GetMaxCol() ;
        r2 = DM.ARAY[riVS2].GetMaxRow() ;
        c2 = DM.ARAY[riVS2].GetMaxCol() ;
        if(DM.ARAY[riVS1].CheckAllExist()&&DM.ARAY[riVS1].FindFrstRowCol(csUnkwn  , r1 , c1)) {r = r1 ; c = c1 ;}
        if(DM.ARAY[riVS2].CheckAllExist()&&DM.ARAY[riVS2].FindFrstRowCol(csUnkwn  , r2 , c2)) {if(r > r2) { r = r2 ; c = c2 ;}}
    }

    if(INSP_DIRECTION == idLeftBtm) {
        r = -1 ;
        c = -1 ;
        r1 = 0 ;
        c1 = DM.ARAY[riVS1].GetMaxCol() ;
        r2 = 0 ;
        c2 = DM.ARAY[riVS2].GetMaxCol() ;

        if(DM.ARAY[riVS1].CheckAllExist()&&DM.ARAY[riVS1].FindLastRowFrstCol(csUnkwn  , r1 , c1)) {r = r1 ; c = c1 ; }
        if(DM.ARAY[riVS2].CheckAllExist()&&DM.ARAY[riVS2].FindLastRowFrstCol(csUnkwn  , r2 , c2)) {if(r < r2) { r = r2 ; c = c2 ;}}
    }

    if(r == -1 && c == -1) return false ;

    return true ;
}

//메뉴얼 동작등을 위해.
double CHead::GetMotrPosTable(EN_MOTR_ID _iMotr , int _iRow , int _iCol)
{
    int r , c ;
    double dStartYPos = 0.0 ;
    double dStartXPos = 0.0 ;
    double dYPos      ;
    double dXPos      ;

    r = OM.DevOptn.iRowInspCnt ? _iRow/OM.DevOptn.iRowInspCnt * OM.DevOptn.iRowInspCnt : _iRow ;
    c = OM.DevOptn.iColInspCnt ? _iCol/OM.DevOptn.iColInspCnt * OM.DevOptn.iColInspCnt : _iCol ;

    dStartXPos = PM.GetValue(_iMotr , pvWRK_XVsnWorkSttPs ) ;

    if     (_iMotr == miWK1_YVsn) dStartYPos = PM.GetValue(_iMotr , pvWK1_YVsnWorkSttPs ) ;
    else if(_iMotr == miWK2_YVsn) dStartYPos = PM.GetValue(_iMotr , pvWK2_YVsnWorkSttPs ) ;



    if(_iMotr != miWK1_YVsn && _iMotr != miWK2_YVsn && _iMotr != miWRK_XVsn) return MT_GetCmdPos(_iMotr) ;



    dXPos = dStartXPos + c * OM.DevInfo.dColPitch ;
    dYPos = dStartYPos + r * OM.DevInfo.dRowPitch ;

    if(OM.DevInfo.dColGrGap && OM.DevInfo.iColGrCnt) {
        dXPos = dXPos + (c / OM.DevInfo.iColGrCnt) * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) ;
    }

    if(OM.DevInfo.dRowGrGap && OM.DevInfo.iRowGrCnt) {
        dYPos = dYPos + (r / OM.DevInfo.iRowGrCnt) * (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) ;
    }


    if     (_iMotr == miWK1_YVsn) return dYPos ;
    else if(_iMotr == miWK2_YVsn) return dYPos ;
    else if(_iMotr == miWRK_XVsn) return dXPos ;
}

double CHead::GetMotrPos(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId )
{
    int r  , c  ;
    const double dOfsforTrg = 5.0 ; //아진 인터럽트 트리거를 위한 옵셑.
    double dStartYPos ;
    const bool bPkgCntr = false ;
    //int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    //int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;

    //요놈. 플라잉 방식이라 맵핑 못씀.
    FindChip( r , c ) ;

    r = OM.DevOptn.iRowInspCnt ? r/OM.DevOptn.iRowInspCnt  * OM.DevOptn.iRowInspCnt  :  r ;

    bool bLeftToRight = GetLtToRt(r);

    if     (_iMotr == miWK1_YVsn) dStartYPos = PM.GetValue(miWK1_YVsn , pvWK1_YVsnWorkSttPs ) ;
    else if(_iMotr == miWK2_YVsn) dStartYPos = PM.GetValue(miWK2_YVsn , pvWK2_YVsnWorkSttPs ) ;

    //스타트 포지션과 엔드 포지션 구분.
    double dXStartPos ;
    double dXEndPos   ;
    double dYPos      ;
//세트
/**/int iVtlColCnt = GetVtlColCnt();
/**/int iVtlRowCnt = GetVtlRowCnt();
/**/
/**/double dLeftPos  = GetLeftPos ();
/**/double dRightPos = GetRightPos();
/**/
/**/double dLeftPosCt  = GetLeftPosCt ();
/**/double dRightPosCt = GetRightPosCt();


    if(bLeftToRight) {
        dXStartPos = dLeftPosCt  - dOfsforTrg ;
        dXEndPos   = dRightPosCt + dOfsforTrg ;
    }
    else {
        dXStartPos = dRightPosCt + dOfsforTrg ;
        dXEndPos   = dLeftPosCt  - dOfsforTrg ;
    }

    dYPos = dStartYPos +                                           //첫포지션 더하고.
            r * OM.DevInfo.dRowPitch +    //검사열을 감안해서 해당 로우 까지 거리계산해서 더하고.
           //(OM.DevOptn.iRowInspCnt - 1) * OM.DevInfo.dRowPitch / 2.0 +  // 요거는
           (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * (OM.DevInfo.iRowGrCnt?(r+1)/OM.DevInfo.iRowGrCnt : 0 ) ; //그룹 갯수 개산 해서 더함.






    double dPos = 0.0 ;

    if(_iMotr == miWRK_XVsn){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWRK_XVsnWait    : dPos = PM.GetValue (_iMotr , pvWRK_XVsnWaitPs    ); break ;
            case piWRK_XVsnWorkStt : dPos = PM.GetValue (_iMotr , pvWRK_XVsnWorkSttPs ); break ;
            case piWRK_XVsnTrgStt  : dPos = dXStartPos                                 ; break ;
            case piWRK_XVsnTrgEnd  : dPos = dXEndPos                                   ; break ;
        }
    }

    else if(_iMotr == miWK1_YVsn){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWK1_YVsnWait    : dPos = PM.GetValue (_iMotr , pvWK1_YVsnWaitPs    ); break ;
            case piWK1_YVsnWorkStt : dPos = PM.GetValue (_iMotr , pvWK1_YVsnWorkSttPs ); break ;
            case piWK1_YVsnWork    : dPos = dYPos                                      ; break ;
        }
    }

    else if(_iMotr == miWK2_YVsn){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWK2_YVsnWait    : dPos = PM.GetValue (_iMotr , pvWK2_YVsnWaitPs    ); break ;
            case piWK2_YVsnWorkStt : dPos = PM.GetValue (_iMotr , pvWK2_YVsnWorkSttPs ); break ;
            case piWK2_YVsnWork    : dPos = dYPos                                      ; break ;
        }
    }

    else if(_iMotr == miWK1_ZVsn){
        switch(_iPstnId) {
            default                : dPos = MT_GetCmdPos(_iMotr                       ); break ;
            case piWK1_ZVsnWait    : dPos = PM.GetValue (_iMotr , pvWK1_ZVsnWaitPs    ); break ;
            case piWK1_ZVsnWork    : dPos = PM.GetValue (_iMotr , pvWK1_ZVsnWorkPs    ); break ;
            case piWK1_ZVsnUVWork  : dPos = PM.GetValue (_iMotr , pvWK1_ZVsnWorkPs    ); break ;
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

    if(_iMotr == miWRK_XVsn){
    }

    else if(_iMotr == miWK1_YVsn){
    }

    else if(_iMotr == miWK2_YVsn){
    }

    else if(_iMotr == miWK1_ZVsn){
    }

    return true ;
}

//---------------------------------------------------------------------------
bool CHead::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    if(_iActr == aiHED_UVLight){
    }
    else {
        return false ;
    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

void CHead::SetTrgPos()
{
    int iTrgCnt      = OM.DevOptn.iColInspCnt ? GetVtlColCnt() / OM.DevOptn.iColInspCnt : 0;
    //int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    //int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;
    double dTemp    ;
    int r,c ;

    AnsiString sTemp ;

    double * dTrgPos = new double[iTrgCnt] ;



    FindChip(r,c );

    bool bLeftToRight = GetLtToRt(r);

    r = r/OM.DevOptn.iRowInspCnt * OM.DevOptn.iRowInspCnt ;

    for(int i = 0 ; i < iTrgCnt ; i++) {
        dTemp      = GetLeftPosCt() +                                                                                                                              //첫포지션
                      OM.DevInfo.dColPitch * OM.DevOptn.iColInspCnt * i +

                     (OM.DevInfo.iColGrCnt ? (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch)* OM.DevOptn.iColInspCnt * (i + 1) / OM.DevInfo.iColGrCnt : 0 ) ;

                                                                                                      //빼기 검사 그룹별로
                     //(OM.DevOptn.iColInspCnt * (i + 1) /
                     //(iColOneGrCnt ? OM.DevOptn.iColInspCnt/2 / iColOneGrCnt : 0) * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch)) ;//그룹갭 계산.
        if(bLeftToRight) dTrgPos[i] = dTemp ;
        else             dTrgPos[iTrgCnt - 1 - i] = dTemp ;

        sTemp += AnsiString(dTemp) + AnsiString(" ") ;
    }

    Trace("Trigger" , sTemp.c_str());



    MT_SetAxtTrgPos(miWRK_XVsn , iTrgCnt , dTrgPos , 1000 , true , true) ;

    delete [] dTrgPos ;
}

void CHead::ResetTrgPos()
{
    MT_ResetAxtTrgPos(miWRK_XVsn) ;
}

bool CHead::MoveMotr(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId) // 모터를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iMotr , _iPstnId)) return false;

    double dPosition = GetMotrPos(_iMotr , _iPstnId);

    if(Step.iCycle) return MT_GoAbsRun(_iMotr , dPosition);
    else            return MT_GoAbsMan(_iMotr , dPosition);
}

bool CHead::MoveActr(EN_ACTR_ID _iActr , bool _bFwd) //실린더를 움직일때 쓰는 함수.
{
    if (!CheckSafe(_iActr, _bFwd)) return false;

    return AT_MoveCyl(_iActr , _bFwd );
}

bool CHead::CycleHome()
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iHome && Step.iHome == PreStep.iHome && CheckStop() && !OM.MstOptn.bDebugMode , 5000 )) {
        EM_SetErr(eiHED_HomeTO);
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

        case 10: MT_Reset(miWRK_XVsn);
                 MT_Reset(miWK1_YVsn);
                 MT_Reset(miWK2_YVsn);
                 MT_Reset(miWK1_ZVsn);

                 MT_SetServo(miWRK_XVsn,true);
                 MT_SetServo(miWK1_YVsn,true);
                 MT_SetServo(miWK2_YVsn,true);
                 MT_SetServo(miWK1_ZVsn,true);

                 MT_DoHome(miWK1_ZVsn) ;
                 Step.iHome++ ;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miWK1_ZVsn))return false ;
                 MT_DoHome(miWRK_XVsn) ;
                 MT_DoHome(miWK1_YVsn) ;
                 MT_DoHome(miWK2_YVsn) ;
                 Step.iHome++ ;
                 return false ;

        case 12: if(!MT_GetHomeEnd(miWRK_XVsn))return false ;
                 if(!MT_GetHomeEnd(miWK1_YVsn))return false ;
                 if(!MT_GetHomeEnd(miWK2_YVsn))return false ;

                 MT_GoAbsMan(miWRK_XVsn , GetMotrPos(miWRK_XVsn , piWRK_XVsnWait));
                 MT_GoAbsMan(miWK1_YVsn , GetMotrPos(miWK1_YVsn , piWK1_YVsnWait));
                 MT_GoAbsMan(miWK2_YVsn , GetMotrPos(miWK2_YVsn , piWK2_YVsnWait));
                 MT_GoAbsMan(miWK1_ZVsn , GetMotrPos(miWK1_ZVsn , piWK1_ZVsnWait));

                 Step.iHome++;
                 return false ;

        case 13: if(!MT_GoAbsMan(miWRK_XVsn , GetMotrPos(miWRK_XVsn , piWRK_XVsnWait))) return false ;
                 if(!MT_GoAbsMan(miWK1_YVsn , GetMotrPos(miWK1_YVsn , piWK1_YVsnWait))) return false ;
                 if(!MT_GoAbsMan(miWK2_YVsn , GetMotrPos(miWK2_YVsn , piWK2_YVsnWait))) return false ;
                 if(!MT_GoAbsMan(miWK1_ZVsn , GetMotrPos(miWK1_ZVsn , piWK1_ZVsnWait))) return false ;

                      if(!OM.CmnOptn.bVisn1Skip && !OM.CmnOptn.bVisn2Skip)VC.SendReset(vrBoth ) ;
                 else if( OM.CmnOptn.bVisn1Skip && !OM.CmnOptn.bVisn2Skip)VC.SendReset(vrVisn2) ;
                 else if(!OM.CmnOptn.bVisn1Skip &&  OM.CmnOptn.bVisn2Skip)VC.SendReset(vrVisn1) ;

                 Step.iHome = 0 ;
                 return true ;
    }
}

bool CHead::ToStopCon(void) //스탑을 하기 위한 조건을 보는 함수.
{
    Stat.bReqStop = true ;
    //During the auto run, do not stop.
    if (Step.iSeq) return false;

    Step.iToStop = 10 ;

    //Ok.
    return true;

}

bool CHead::ToStartCon(void) //스타트를 하기 위한 조건을 보는 함수.
{
    Step.iToStart = 10 ;
    //Ok.
    return true;
}

bool CHead::ToStart(void) //스타트를 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStart.OnDelay(Step.iToStart && !PreStep.iToStart == Step.iToStart && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiHED_ToStartTO);

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("Step.iToStart=%02d" , Step.iToStart );
    if(Step.iToStart != PreStep.iToStart) {
        Trace(m_sPartName.c_str(),sTemp.c_str());
    }

    PreStep.iToStart = Step.iToStart ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: if(OM.DevOptn.bUseUv) {
                     MoveActr(aiHED_UVLight , ccFwd);
                     MoveMotr(miWK1_ZVsn    , piWK1_ZVsnUVWork);
                 }
                 else {
                     MoveActr(aiHED_UVLight , ccBwd);
                     MoveMotr(miWK1_ZVsn    , piWK1_ZVsnWork);
                 }

                 Step.iToStart ++ ;
                 return false ;

        case 11: if(OM.DevOptn.bUseUv) {
                     if(!MoveActr(aiHED_UVLight , ccFwd           )) return false ;
                     if(!MoveMotr(miWK1_ZVsn    , piWK1_ZVsnUVWork)) return false ;
                 }
                 else {
                     if(!MoveActr(aiHED_UVLight , ccBwd         )) return false ;
                     if(!MoveMotr(miWK1_ZVsn    , piWK1_ZVsnWork)) return false ;
                 }

                 Step.iToStart = 0 ;
                 return true ;
    }
}

bool CHead::ToStop(void) //스탑을 하기 위한 함수.
{
    //Check Time Out.
    if (m_tmToStop.OnDelay(Step.iToStop && !PreStep.iToStop == Step.iToStop && CheckStop() && !OM.MstOptn.bDebugMode , 5000)) EM_SetErr(eiHED_ToStopTO);

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

        case 10: Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}

bool CHead::Autorun(void) //오토런닝시에 계속 타는 함수.
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

        if(EM_IsErr()) return false ;

        bool isStrpExist     =  DM.ARAY[riPRB].GetCntExist (       ) ||
                                RAL.GetSeqStep() == CRail::scIn      ||
                                DM.ARAY[riLDR].GetCntStat  (csUnkwn) && OM.EqpOptn.bExistLoader ||
                                DM.ARAY[riWK1].GetCntExist (       ) ;
        bool isNeedInspStart =  DM.ARAY[riVS1].GetCntStat  (csUnkwn) || DM.ARAY[riVS2].GetCntStat  (csUnkwn) ;
        bool isWaitInsp1Pkg  =  DM.ARAY[riVS1].CheckAllStat(csEmpty) && isStrpExist ;

        bool isWK1Ready      = (DM.ARAY[riVS1].GetCntStat(csUnkwn) && AT_Complete(aiWK1_Lift , ccFwd)) || !DM.ARAY[riWK1].GetCntExist() ;
        bool isWK2Ready      = (DM.ARAY[riVS2].GetCntStat(csUnkwn) && AT_Complete(aiWK2_Lift , ccFwd)) || !DM.ARAY[riWK2].GetCntExist() ;


        bool isCycleInsp     =  isNeedInspStart && !isWaitInsp1Pkg && isWK1Ready && isWK2Ready ;
        bool isConEnd        = !isNeedInspStart ;

        //Normal Decide Step.
             if (isCycleInsp ) {Trace(m_sPartName.c_str(),"CycleInsp Stt"); Step.iSeq = scInsp ; Step.iCycle = 10 ; PreStep.iCycle = 0 ;} //
        else if (isConEnd    ) {Stat.bWorkEnd = true ; return true ;}
        Stat.bWorkEnd = false ;
    }

    //Cycle.
    switch (Step.iSeq) {
        default    :              /*Trace(m_sPartName.c_str(),"default   End");Step.iSeq = scIdle ;*/  return false ;
        case scInsp: if(CycleInsp()){ Trace(m_sPartName.c_str(),"CycleInsp End");Step.iSeq = scIdle ;} return false ;
    }
}

//One Cycle.
bool CHead::CycleInsp(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 20000 )) {
        EM_SetErr(eiHED_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        if(Step.iCycle == 17) {

                 if(!OM.CmnOptn.bVisn1Skip && !OM.CmnOptn.bVisn2Skip)VC.SendReset(vrBoth ) ;
            else if( OM.CmnOptn.bVisn1Skip && !OM.CmnOptn.bVisn2Skip)VC.SendReset(vrVisn2) ;
            else if(!OM.CmnOptn.bVisn1Skip &&  OM.CmnOptn.bVisn2Skip)VC.SendReset(vrVisn1) ;


            if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
            if(DM.ARAY[riWK1].GetCntExist())DM.ARAY[riWK1].ChangeStat(csWork,csUnkwn) ;

            if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
            if(DM.ARAY[riWK2].GetCntExist())DM.ARAY[riWK2].ChangeStat(csWork,csUnkwn) ;
        }
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

    bool r1 , r2 , r3 , r4 ;

    //메뉴얼 동작중에 에러 뜨면 메뉴얼이 중간에 멈춘다.
    bool bManualInsp = !Step.iSeq ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: Step.iCycle++;
                  return false ;

        case  11:


                  MoveMotr(miWK1_YVsn,piWK1_YVsnWork  );
                  MoveMotr(miWK2_YVsn,piWK2_YVsnWork  );
                  MoveMotr(miWRK_XVsn,piWRK_XVsnTrgStt);

                  if(OM.DevOptn.bUseUv) {
                      MoveActr(aiHED_UVLight , ccFwd);
                      MoveMotr(miWK1_ZVsn    , piWK1_ZVsnUVWork);
                  }
                  else {
                      MoveActr(aiHED_UVLight , ccBwd);
                      MoveMotr(miWK1_ZVsn    , piWK1_ZVsnWork);
                  }
                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miWK1_YVsn,piWK1_YVsnWork  )) return false ;
                  if(!MoveMotr(miWK2_YVsn,piWK2_YVsnWork  )) return false ;
                  if(!MoveMotr(miWRK_XVsn,piWRK_XVsnTrgStt)) return false ;
                  if(OM.DevOptn.bUseUv) {
                      if(!MoveActr(aiHED_UVLight , ccFwd           )) return false ;
                      if(!MoveMotr(miWK1_ZVsn    , piWK1_ZVsnUVWork)) return false ;
                  }
                  else {
                      if(!MoveActr(aiHED_UVLight , ccBwd         )) return false ;
                      if(!MoveMotr(miWK1_ZVsn    , piWK1_ZVsnWork)) return false ;
                  }

                  ResetTrgPos() ; //트리거 세팅 클리어
                  Step.iCycle++ ;
                  return false ;  

        case  13: if(!IO_GetX(xHED_1Ready)&&!OM.CmnOptn.bVisn1Skip){
                      EM_SetErr(eiHED_Visn1NotReady) ;
                      Step.iCycle = 0 ;
                      return true;
                  }
                  if(!IO_GetX(xHED_2Ready)&&!OM.CmnOptn.bVisn2Skip){
                      EM_SetErr(eiHED_Visn2NotReady) ;
                      Step.iCycle = 0 ;
                      return true;
                  }

                  SetTrgPos(); //트리거 세팅.
                  MoveMotr(miWRK_XVsn,piWRK_XVsnTrgEnd );
                  //MT_GoAbsRun(miWRK_XVsn , 13 ) ;
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveMotr(miWRK_XVsn,piWRK_XVsnTrgEnd )) return false ;
                  ResetTrgPos() ; //트리거 세팅 클리어
                  Step.iCycle++ ;
                  return false ;

        case  15: FindChip(r , c ) ;
                  r = r/OM.DevOptn.iRowInspCnt * OM.DevOptn.iRowInspCnt ;
                  for(c = 0 ; c < DM.ARAY[riVS1].GetMaxCol() ; c++ ) {
                      for(int i = 0 ; i < OM.DevOptn.iRowInspCnt ; i ++) {
                          if(DM.ARAY[riVS1].CheckAllExist())DM.ARAY[riVS1].SetStat(r+i,c,csWork) ;
                          if(DM.ARAY[riVS2].CheckAllExist())DM.ARAY[riVS2].SetStat(r+i,c,csWork) ;
                      }
                  }

                  r1 = (/*DM.ARAY[riWK1].CheckAllExist() && */DM.ARAY[riVS1].CheckAllStat(csWork));
                  r2 = (/*DM.ARAY[riWK2].CheckAllExist() && */DM.ARAY[riVS2].CheckAllStat(csWork));

                  //Stat.bReqAllInsp = true ;
                  if(!r1 && !r2) {
                      Step.iCycle=11 ; //Next Row Insp
                      return false ;

                  }
                  Step.iCycle++ ;
                  return false ;

        case  16: //한스트립을 다 검사했을 경우.
                  MoveMotr(miWK1_YVsn,piWK1_YVsnWorkStt) ;
                  MoveMotr(miWK2_YVsn,piWK2_YVsnWorkStt) ;
                  MoveMotr(miWRK_XVsn,piWRK_XVsnWorkStt) ;
                  Step.iCycle++ ;
                  return false ;

        //17 Step Used When TimeOut
        case  17: if(!MoveMotr(miWK1_YVsn,piWK1_YVsnWorkStt)) return false ;
                  if(!MoveMotr(miWK2_YVsn,piWK2_YVsnWorkStt)) return false ;
                  if(!MoveMotr(miWRK_XVsn,piWRK_XVsnWorkStt)) return false ;
                  if(!OM.CmnOptn.bVisn1Skip && !IO_GetX(xHED_1Result)) return false ;
                  if(!OM.CmnOptn.bVisn2Skip && !IO_GetX(xHED_2Result)) return false ;



                  VC.ClearErrCnt();


                  //Vision 1
                  if(OM.CmnOptn.bVisn1Skip) {
                      if(DM.ARAY[riWK1].CheckAllExist())DM.ARAY[riWK1].ChangeStat(csUnkwn,csWork) ;
                  }
                  else {
                      if(DM.ARAY[riWK1].CheckAllExist()){
                          if(!VC.ReadResult(1,riWK1)){ //비젼 데이터 읽어 오기.
                              if(!bManualInsp)EM_SetErr(eiHED_Visn1Read);
                              DM.ARAY[riVS1].SetStat(csUnkwn) ;
                              DM.ARAY[riVS2].SetStat(csUnkwn) ;
                              VC.SendReset(true); //쓰레드를 켜서 비젼 리셑.
                              Step.iCycle=0 ;
                              return true ;
                          }
                      }
                      
                      //토탈 페일 갯수.
                      if(OM.CmnOptn.bCheckFailCnt && OM.CmnOptn.iCheckFailCnt && !DM.ARAY[riWK1].CheckAllStat(csNone) ) {
                          if(VC.GetLastTtlErrCnt(1)>= OM.CmnOptn.iCheckFailCnt && !bManualInsp) EM_SetErr(eiWK1_VsnFailCnt);
                      }

                      //같은 에러 갯수.
                      if(OM.CmnOptn.bCheckSFailCnt && OM.CmnOptn.iCheckSFailCnt && !DM.ARAY[riWK1].CheckAllStat(csNone)&& !bManualInsp ) {
                          if(VC.GetLastErrCnt(1,csRslt0 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt1 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt2 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt3 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt4 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt5 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt6 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt7 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt8 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt9 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt10)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt11)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt12)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt13)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt14)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt15)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                          if(VC.GetLastErrCnt(1,csRslt16)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK1_VsnSFailCnt);
                      }

                  }

                  //Vision2
                  if(OM.CmnOptn.bVisn2Skip) {
                      if(DM.ARAY[riWK2].CheckAllExist())DM.ARAY[riWK2].ChangeStat(csUnkwn,csWork) ;
                  }
                  else {
                      if(DM.ARAY[riWK2].CheckAllExist()){
                          if(!VC.ReadResult(2,riWK2)){
                              if(!bManualInsp)EM_SetErr(eiHED_Visn2Read);
                              DM.ARAY[riVS1].SetStat(csUnkwn) ;
                              DM.ARAY[riVS2].SetStat(csUnkwn) ;
                              VC.SendReset(true); //쓰레드를 켜서 비젼 리셑.
                              Step.iCycle=0 ;
                              return true ;
                          }
                      }

                      //토탈 페일 갯수.
                      if(OM.CmnOptn.bCheckFailCnt && OM.CmnOptn.iCheckFailCnt && !DM.ARAY[riWK2].CheckAllStat(csNone) ) {
                          if(VC.GetLastTtlErrCnt(2)>= OM.CmnOptn.iCheckFailCnt&& !bManualInsp) EM_SetErr(eiWK2_VsnFailCnt);
                      }

                      //같은 애러 갯수.
                      if(OM.CmnOptn.bCheckSFailCnt && OM.CmnOptn.iCheckSFailCnt && !DM.ARAY[riWK2].CheckAllStat(csNone)&& !bManualInsp ) {
                          if(VC.GetLastErrCnt(2,csRslt0 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt1 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt2 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt3 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt4 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt5 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt6 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt7 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt8 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt9 )>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt10)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt11)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt12)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt13)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt14)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt15)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                          if(VC.GetLastErrCnt(2,csRslt16)>= OM.CmnOptn.iCheckSFailCnt) EM_SetErr(eiWK2_VsnSFailCnt);
                      }
                  }

                  Step.iCycle=0 ;
                  return true  ;
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
    if(!MT_GetStop(miWK1_YVsn)) return false ;
    if(!MT_GetStop(miWK1_ZVsn)) return false ;
    if(!MT_GetStop(miWK2_YVsn)) return false ;
    if(!MT_GetStop(miWRK_XVsn)) return false ;

    if(!AT_Done(aiHED_UVLight)) return false ;

    return true ;
}

void CHead::Load(bool _bLoad , FILE *fp)
{

}

















