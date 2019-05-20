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
#include "UserINI.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CHead  HED;

enum EN_INSP_DIRC{idLeftTop = 0 , idLeftBtm = 1 };
EN_INSP_DIRC HEAD_INSP_DIRECTION = idLeftBtm ;



int GetVtlColCnt ()
{
    int iRet = 0 ;
    if(OM.DevInfo.iColInspCnt) {
        iRet = (OM.DevInfo.iColCnt % OM.DevInfo.iColInspCnt) ?
              ((OM.DevInfo.iColCnt / OM.DevInfo.iColInspCnt) * OM.DevInfo.iColInspCnt + OM.DevInfo.iColInspCnt) :
              ((OM.DevInfo.iColCnt / OM.DevInfo.iColInspCnt) * OM.DevInfo.iColInspCnt) ;
    }
    return iRet ;
}
int GetVtlRowCnt ()
{
    int iRet = 0 ;
    if(OM.DevInfo.iRowInspCnt) {
        iRet = (OM.DevInfo.iRowCnt % OM.DevInfo.iRowInspCnt) ?
              ((OM.DevInfo.iRowCnt / OM.DevInfo.iRowInspCnt) * OM.DevInfo.iRowInspCnt + OM.DevInfo.iRowInspCnt) :
              ((OM.DevInfo.iRowCnt / OM.DevInfo.iRowInspCnt) * OM.DevInfo.iRowInspCnt) ;
    }
    return iRet ;
}

double GetLeftPos ()
{
    double dRet = (PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStartPs) + (OM.DevInfo.iColInspCnt - 1) * OM.DevInfo.dColPitch / 2.0) ;
    return dRet ;
}
double GetRightPos ()
{
    int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;
    double dRet = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStartPs) +
                  OM.DevInfo.dColPitch * GetVtlColCnt() +
                  (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) *        (iColOneGrCnt ? (GetVtlColCnt()/iColOneGrCnt) : 0) +
                  (OM.DevInfo.iColInspCnt - 1) * OM.DevInfo.dColPitch / 2.0 ;
    return dRet ;
}

double GetLeftPosCt()
{
    double dRet = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStartPs) ;
    return dRet ;
}

double GetRightPosCt()
{
    int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;

    double dRet = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStartPs) + OM.DevInfo.dColPitch * GetVtlColCnt() +
                  (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * (iColOneGrCnt ? (GetVtlColCnt()/iColOneGrCnt) : 0) -
                  OM.DevInfo.iColInspCnt * OM.DevInfo.dColPitch ;
                  //(OM.DevOptn.iColInspCnt - 1) * OM.DevInfo.dColPitch / 2.0 ;
    return dRet ;
}

bool GetLtToRt(int r)
{
//    if(HEAD_INSP_DIRECTION == idLeftTop ||
//       HEAD_INSP_DIRECTION == idLeftBtm) return !((OM.DevInfo.iRowInspCnt ? r/OM.DevInfo.iRowInspCnt : r) %2);
    if(HEAD_INSP_DIRECTION == idLeftBtm) return !((OM.DevInfo.iRowInspCnt ? r/OM.DevInfo.iRowInspCnt : r) %2);
    else                            return  ((OM.DevInfo.iRowInspCnt ? r/OM.DevInfo.iRowInspCnt : r) %2);
}




CHead::CHead(void)
{
    m_sPartName = "Head " ;
    m_bLeftToRight = true ;
    LoadDpAray();

    Reset();
}

void CHead::LoadDpAray()
{
    //Local Var.
    FILE       *fp;
    //Make Dir.
    AnsiString  DevName;
    AnsiString  Path = ExtractFilePath(Application -> ExeName) + "SeqData\\DispMap.DAT";

    //File Open.
    //File Open.
    fp = fopen(Path.c_str() , "rb");
    if (fp == NULL) {
        fp = fopen(Path.c_str() , "wb");
        return ;
    }

    //Read&Write.
    DpAray1.Load(true , fp , 1);
    DpAray2.Load(true , fp , 2);
    DpAray3.Load(true , fp , 3);

    fclose(fp);
}

void CHead::SaveDpAray()
{
    //Local Var.
    FILE       *fp;
    //Make Dir.
    AnsiString  DevName;
    AnsiString  Path =   ExtractFilePath(Application -> ExeName) + "SeqData\\DispMap.DAT";

    //File Open.
    fp = fopen(Path.c_str() , "wb");
    if (fp == NULL) return;

    //Read&Write.
    DpAray1.Load(false , fp , 1);
    DpAray2.Load(false , fp , 2);
    DpAray3.Load(false , fp , 3);

    fclose(fp);
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
    //FormMain으로
}

bool CHead::FindChip( int &r , int &c )
{
    int r1 ;
    int c1 ;
    int r2 ;
    int c2 ;
    int r3 ;
    int c3 ;

    if(HEAD_INSP_DIRECTION == idLeftTop) {
        r = DM.ARAY[riVS1].GetMaxRow() ;
        c = DM.ARAY[riVS1].GetMaxCol() ;

        r1 = DM.ARAY[riVS1].GetMaxRow() ;
        c1 = DM.ARAY[riVS1].GetMaxCol() ;
        r2 = DM.ARAY[riVS2].GetMaxRow() ;
        c2 = DM.ARAY[riVS2].GetMaxCol() ;
        r3 = DM.ARAY[riVS3].GetMaxRow() ;
        c3 = DM.ARAY[riVS3].GetMaxCol() ;
        if(DM.ARAY[riVS1].CheckAllExist()&&DM.ARAY[riVS1].FindFrstRowCol(csUnkwn  , r1 , c1)) {r = r1 ; c = c1 ;}
        if(DM.ARAY[riVS2].CheckAllExist()&&DM.ARAY[riVS2].FindFrstRowCol(csUnkwn  , r2 , c2)) {if(r > r2) { r = r2 ; c = c2 ;}}
        if(DM.ARAY[riVS3].CheckAllExist()&&DM.ARAY[riVS3].FindFrstRowCol(csUnkwn  , r3 , c3)) {if(r > r3) { r = r3 ; c = c3 ;}}
    }

    if(HEAD_INSP_DIRECTION == idLeftBtm) {
        r = -1 ;
        c = -1 ;
        r1 = 0 ;
        c1 = DM.ARAY[riVS1].GetMaxCol() ;
        r2 = 0 ;
        c2 = DM.ARAY[riVS2].GetMaxCol() ;
        r3 = 0 ;
        c3 = DM.ARAY[riVS3].GetMaxCol() ;

        if(DM.ARAY[riVS1].CheckAllExist()&&DM.ARAY[riVS1].FindLastRowFrstCol(csUnkwn  , r1 , c1)) {r = r1 ; c = c1 ; }
        if(DM.ARAY[riVS2].CheckAllExist()&&DM.ARAY[riVS2].FindLastRowFrstCol(csUnkwn  , r2 , c2)) {if(r < r2) { r = r2 ; c = c2 ;}}
        if(DM.ARAY[riVS3].CheckAllExist()&&DM.ARAY[riVS3].FindLastRowFrstCol(csUnkwn  , r3 , c3)) {if(r < r3) { r = r3 ; c = c3 ;}}
        r = DM.ARAY[riVS3].GetMaxRow() -r -1 ;
        c = DM.ARAY[riVS3].GetMaxCol() -c -1 ;
    }

    if(r == -1 && c == -1) return false ;

    return true ;
}

//메뉴얼 동작등을 위해.
/*
double CHead::GetMotrPosTable(EN_MOTR_ID _iMotr , int _iRow , int _iCol)
{
    int r , c ;
    double dStartYPos = 0.0 ;
    double dStartXPos = 0.0 ;
    double dYPos      ;
    double dXPos      ;

    r = OM.DevInfo.iRowInspCnt ? _iRow/OM.DevInfo.iRowInspCnt * OM.DevInfo.iRowInspCnt : _iRow ;
    c = OM.DevInfo.iColInspCnt ? _iCol/OM.DevInfo.iColInspCnt * OM.DevInfo.iColInspCnt : _iCol ;

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
*/

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

//    r = OM.DevInfo.iRowInspCnt ? r/OM.DevInfo.iRowInspCnt  * OM.DevInfo.iRowInspCnt  :  r ; // Insp Cnt 삑살.


    bool bLeftToRight = m_bLeftToRight;
//    m_bTemp = m_bLeftToRight ;

//    if(HEAD_INSP_DIRECTION == idLeftBtm) bLeftToRight = !((OM.DevInfo.iRowInspCnt ? r/OM.DevInfo.iRowInspCnt : r) %2);
//    else                                 bLeftToRight =  ((OM.DevInfo.iRowInspCnt ? r/OM.DevInfo.iRowInspCnt : r) %2);

//    m_bTemp1 = bLeftToRight ;



//    if     (_iMotr == miWK1_YVsn) dStartYPos = PM.GetValue(miWK1_YVsn , pvWK1_YVsnWorkSttPs ) ;
//    else if(_iMotr == miWK2_YVsn) dStartYPos = PM.GetValue(miWK2_YVsn , pvWK2_YVsnWorkSttPs ) ;
    dStartYPos = PM.GetValue(miWRK_YVsn , pvWRK_YVsnWorkStartPs ) ;

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
    //TODO :: JJ 수정요망.
//    if(OM.DevInfo.iRowGrCnt != 0 && OM.DevInfo.iRowGrCnt < r) r = r-1;

//    int iTemp = 0 ;
//    if(OM.DevInfo.iRowGrCnt && OM.DevInfo.iRowGrCnt % OM.DevInfo.iRowInspCnt && (r)/OM.DevInfo.iRowGrCnt){
//        iTemp = OM.DevInfo.iRowGrCnt % OM.DevInfo.iRowInspCnt ;
//        iTemp = OM.DevInfo.iRowInspCnt - iTemp ;
//    }


    if(HEAD_INSP_DIRECTION == idLeftTop) {
        dYPos = dStartYPos +                                           //첫포지션 더하고.
                r * OM.DevInfo.dRowPitch +    //검사열을 감안해서 해당 로우 까지 거리계산해서 더하고.
               //(OM.DevOptn.iRowInspCnt - 1) * OM.DevInfo.dRowPitch / 2.0 +  // 요거는
               (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * (OM.DevInfo.iRowGrCnt?(r)/OM.DevInfo.iRowGrCnt : 0 ) ;//-
//               (iTemp * OM.DevInfo.dRowPitch);



    }

    if(HEAD_INSP_DIRECTION == idLeftBtm) {
        dYPos = dStartYPos -                                           //첫포지션 더하고.
                r * OM.DevInfo.dRowPitch -    //검사열을 감안해서 해당 로우 까지 거리계산해서 더하고.
               //(OM.DevOptn.iRowInspCnt - 1) * OM.DevInfo.dRowPitch / 2.0 +  // 요거는
               (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * (OM.DevInfo.iRowGrCnt?(r)/OM.DevInfo.iRowGrCnt : 0 );// +  //그룹 갯수 개산 해서 더함.
//               (iTemp * OM.DevInfo.dRowPitch);



//        (OM.DevInfo.dRowGrGap       - OM.DevInfo.dRowPitch - dSubColGroupGap) * (OM.DevInfo.iRowGrCnt?(rr)/OM.DevInfo.iRowGrCnt : 0 ) - //그룹 갯수 개산 해서 더함.
//        (dSubColGroupGap                                                    ) * (OM.DevInfo.iRowSubGroupCount?(rr)/OM.DevInfo.iRowSubGroupCount : 0 ) ; //SUB 그룹 갯수 개산 해서 더함.

    }


    double dPos = 0.0 ;

    if(_iMotr == miWRK_XVsn){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWRK_XVsnWait      : dPos = PM.GetValue (_iMotr , pvWRK_XVsnWaitPs      ); break ;
            case piWRK_XVsnWorkStart : dPos = PM.GetValue (_iMotr , pvWRK_XVsnWorkStartPs ); break ;
            case piWRK_XVsnTrgStt    : dPos = dXStartPos                                   ; break ;
            case piWRK_XVsnTrgEnd    : dPos = dXEndPos                                     ; break ;
        }
    }

    else if(_iMotr == miWRK_YVsn){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWRK_YVsnWait      : dPos = PM.GetValue (_iMotr , pvWRK_YVsnWaitPs      ); break ;
            case piWRK_YVsnWorkStart : dPos = PM.GetValue (_iMotr , pvWRK_YVsnWorkStartPs ); break ;
            case piWRK_YVsnWork      : dPos = dYPos                                        ; break ;
        }
    }

//    else if(_iMotr == miWK2_YVsn){
//        switch(_iPstnId) {
//            default                : dPos = MT_GetCmdPos(_iMotr                       ); break ;
//            case piWK2_YVsnWait    : dPos = PM.GetValue (_iMotr , pvWK2_YVsnWaitPs    ); break ;
//            case piWK2_YVsnWorkStt : dPos = PM.GetValue (_iMotr , pvWK2_YVsnWorkSttPs ); break ;
//            case piWK2_YVsnWork    : dPos = dYPos                                      ; break ;
//        }
//    }

//    else if(_iMotr == miWK1_ZVsn){
//        switch(_iPstnId) {
//            default                : dPos = MT_GetCmdPos(_iMotr                       ); break ;
//            case piWK1_ZVsnWait    : dPos = PM.GetValue (_iMotr , pvWK1_ZVsnWaitPs    ); break ;
//            case piWK1_ZVsnWork    : dPos = PM.GetValue (_iMotr , pvWK1_ZVsnWorkPs    ); break ;
//            case piWK1_ZVsnUVWork  : dPos = PM.GetValue (_iMotr , pvWK1_ZVsnWorkPs    ); break ;
//        }
//    }
    else {
        dPos = MT_GetCmdPos(_iMotr);
    }

    return dPos ;
}

//---------------------------------------------------------------------------
bool CHead::CheckSafe(EN_MOTR_ID _iMotr , EN_PSTN_ID _iPstnId)
{
    if(MT_CmprPos(_iMotr , GetMotrPos(_iMotr , _iPstnId)) ) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

    bool isFlprTurning = !MT_GetStop(miWK1_YFlp) ;
    bool isInsMoveing  = !AT_Done(aiWK1_Ins) ;

    if(_iMotr == miWRK_XVsn){
        if(isFlprTurning) { sMsg = "Fliper Moving" ; bRet = false ;}
        if(isInsMoveing ) { sMsg = "Insert Moving" ; bRet = false ;}
        switch(_iPstnId){
            default                  : break ;
            case piWRK_XVsnWait      : break ;
            case piWRK_XVsnWorkStart : break ;
            case piWRK_XVsnTrgStt    : break ;
            case piWRK_XVsnTrgEnd    : break ;
        }
    }

    else if(_iMotr == miWRK_YVsn){
        if(isFlprTurning) { sMsg = "Fliper Moving" ; bRet = false ;}
        if(isInsMoveing ) { sMsg = "Insert Moving" ; bRet = false ;}
        switch(_iPstnId){
            default                  : break ;
            case piWRK_YVsnWait      : break ;
            case piWRK_YVsnWorkStart : break ;
            case piWRK_YVsnWork      : break ;
        }
    }

    if(!bRet){
        Trace(MT_GetName(_iMotr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(MT_GetName(_iMotr).c_str(),sMsg);
    }

    return bRet ;

//    else if(_iMotr == miWK2_YVsn){
//    }

//    else if(_iMotr == miWK1_ZVsn){
//    }
}

//---------------------------------------------------------------------------
bool CHead::CheckSafe(EN_ACTR_ID _iActr , bool _bFwd)
{
    if(AT_Complete(_iActr , _bFwd)) return true ;

    AnsiString sMsg ;
    bool       bRet = true ;

//    if(_iActr == aiHED_UVLight){
//    }
//    else {
//        return false ;
//    }

    if(!bRet){
        Trace(AT_GetName(_iActr).c_str(), sMsg.c_str());
        if(!Step.iCycle)FM_MsgOk(AT_GetName(_iActr).c_str(),sMsg);
    }

    return bRet ;
}

void CHead::SetTrgPos()
{
    int iTrgCnt      = OM.DevInfo.iColInspCnt ? GetVtlColCnt() / OM.DevInfo.iColInspCnt : 0;
    //int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
    //int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;
    double dTemp    ;
    int r,c ;

    AnsiString sTemp ;

    double * dTrgPos = new double[iTrgCnt] ;



    FindChip(r,c );

    bool bLeftToRight = m_bLeftToRight ; //GetLtToRt(r);

    r = r/OM.DevInfo.iRowInspCnt * OM.DevInfo.iRowInspCnt ;

    for(int i = 0 ; i < iTrgCnt ; i++) {
        dTemp      = GetLeftPosCt() +                                                                                                                              //첫포지션
                      OM.DevInfo.dColPitch * OM.DevInfo.iColInspCnt * i +

                     (OM.DevInfo.iColGrCnt ? (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch)* OM.DevInfo.iColInspCnt * (i + 1) / OM.DevInfo.iColGrCnt : 0 ) ;

                                                                                                      //빼기 검사 그룹별로
                     //(OM.DevOptn.iColInspCnt * (i + 1) /
                     //(iColOneGrCnt ? OM.DevOptn.iColInspCnt/2 / iColOneGrCnt : 0) * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch)) ;//그룹갭 계산.
//        if(bLeftToRight) dTrgPos[i] = dTemp ;
//        else             dTrgPos[iTrgCnt - 1 - i] = dTemp ;

//트리거 오프셋 마스터 옵션. 모터 속도에 따라 이미지 밀림.
          if(bLeftToRight) dTrgPos[i] = dTemp - OM.MstOptn.dTrigerOffset ;
          else             dTrgPos[iTrgCnt - 1 - i] = dTemp + OM.MstOptn.dTrigerOffset ;



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

        case 10: 

                 MT_Reset(miWRK_XVsn);
                 MT_Reset(miWRK_YVsn);

                 MT_SetServo(miWRK_XVsn,true);
                 MT_SetServo(miWRK_YVsn,true);

                 //트리거 삽질 방지.
                 SetTrgPos();
                 ResetTrgPos();

                 MT_DoHome(miWRK_YVsn) ;
                 MT_DoHome(miWRK_XVsn) ;
                 Step.iHome++ ;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miWRK_YVsn))return false ;
                 if(!MT_GetHomeEnd(miWRK_XVsn))return false ;



                 MT_GoAbsMan(miWRK_YVsn , GetMotrPos(miWRK_YVsn , piWRK_YVsnWait));
                 MT_GoAbsMan(miWRK_XVsn , GetMotrPos(miWRK_XVsn , piWRK_XVsnWait));
                 Step.iHome++ ;
                 return false ;


        case 12: if(!MT_GoAbsMan(miWRK_YVsn , GetMotrPos(miWRK_YVsn , piWRK_YVsnWait))) return false ;
                 if(!MT_GoAbsMan(miWRK_XVsn , GetMotrPos(miWRK_XVsn , piWRK_XVsnWait))) return false ;

//                 if(!OM.CmnOptn.bVs1Skip) VC.SendReset3(vrVisn1,true) ;
                 if(!OM.CmnOptn.bVs1Skip) VC.SendReset(vrVisn1) ;
                 if(!OM.CmnOptn.bVs2Skip) VC.SendReset(vrVisn2) ;
                 if(!OM.CmnOptn.bVs3Skip) VC.SendReset(vrVisn3) ;

                 //ResetTrgPos();

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

    //비젼 프로세스 맵이 이상할때. 클리어 시켜서 첨부터 하게 한다.
    bool bNotNormalEnded = false ;

    //Move Home.
    switch (Step.iToStart) {
        default: Step.iToStart = 0 ;
                 return true ;

        case 10: MoveMotr(miWRK_XVsn , piWRK_XVsnWait);
                 MoveMotr(miWRK_YVsn , piWRK_XVsnWait);

                 //if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
                 //if(DM.ARAY[riWR1].GetCntExist())DM.ARAY[riWR1].ChangeStat(csWork,csUnkwn) ;
                 //
                 //if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
                 //if(DM.ARAY[riWR2].GetCntExist())DM.ARAY[riWR2].ChangeStat(csWork,csUnkwn) ;
                 //
                 //if(DM.ARAY[riVS3].GetCntExist())DM.ARAY[riVS3].SetStat(csUnkwn) ;
                 //if(DM.ARAY[riWR3].GetCntExist())DM.ARAY[riWR3].ChangeStat(csWork,csUnkwn) ;

                 Step.iToStart ++ ;
                 return false ;

        case 11: if(!MoveMotr(miWRK_XVsn , piWRK_XVsnWait)) return false;
                 if(!MoveMotr(miWRK_YVsn , piWRK_XVsnWait)) return false;

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

        case 10: //if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
                 //if(DM.ARAY[riWR1].GetCntExist())DM.ARAY[riWR1].ChangeStat(csWork,csUnkwn) ;
                 //
                 //if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
                 //if(DM.ARAY[riWR2].GetCntExist())DM.ARAY[riWR2].ChangeStat(csWork,csUnkwn) ;
                 //
                 //if(DM.ARAY[riVS3].GetCntExist())DM.ARAY[riVS3].SetStat(csUnkwn) ;
                 //if(DM.ARAY[riWR3].GetCntExist())DM.ARAY[riWR3].ChangeStat(csWork,csUnkwn) ;

                 Step.iToStop ++ ;
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

//        bool isStrpExist     =  DM.ARAY[riPRB].GetCntExist (       ) ||
//                                RAL.GetSeqStep() == CRail::scIn      ||
//                                DM.ARAY[riLDR].GetCntStat  (csUnkwn) && OM.EqpOptn.bExistLoader ||
//                                DM.ARAY[riWK1].GetCntExist (       ) ;
        bool isNeedInspStart1 =  DM.ARAY[riVS1].GetCntStat  (csUnkwn) || DM.ARAY[riVS2].GetCntStat  (csUnkwn) || DM.ARAY[riVS3].GetCntStat  (csUnkwn) ;
        bool isNeedInspStart2 =  DM.ARAY[riWR1].GetCntExist (       ) || DM.ARAY[riWR2].GetCntExist (       ) || DM.ARAY[riWR3].GetCntExist (       ) ;
//        bool isWaitInsp1Pkg  =  DM.ARAY[riVS1].CheckAllStat(csEmpty) && isStrpExist ;
        bool isCycleMvPrb     =  DM.ARAY[riPRB].GetCntExist() && !DM.ARAY[riWR1].GetCntExist() ;

        bool isWK1Rdy     =  (MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpNormalPs )) ||
                              MT_CmprPos (miWK1_YFlp , PM.GetValue(miWK1_YFlp,pvWK1_YFlpInversePs)))&& AT_Complete(aiWK1_FlpCmp,ccFwd);
//                            AT_Complete(aiWK1_Align,ccBwd) && AT_Complete(aiWK1_FlpCmp,ccFwd) && AT_Complete(aiWK1_Ins,ccBwd) && AT_Complete(aiWK1_Stopper,ccFwd) ;

        bool isWK1Ready      = (DM.ARAY[riVS1].GetCntStat(csUnkwn) &&  isWK1Rdy                         ) || !DM.ARAY[riWR1].GetCntExist() ;
        bool isWK2Ready      = (DM.ARAY[riVS2].GetCntStat(csUnkwn) &&  AT_Complete(aiWK2_Lift   , ccFwd)) || !DM.ARAY[riWR2].GetCntExist() ;
        bool isWK3Ready      = (DM.ARAY[riVS3].GetCntStat(csUnkwn) &&  AT_Complete(aiWK3_Lift   , ccFwd)) || !DM.ARAY[riWR3].GetCntExist() ;

//        bool isCycleInsp     =  isNeedInspStart && !isWaitInsp1Pkg && isWK1Ready && isWK2Ready ;
//        bool isCycleInsp     =  isNeedInspStart1 && isNeedInspStart2 && (isWK1Ready ||( isWK2Ready && isWK3Ready)) &&
        bool isCycleInsp     =  isNeedInspStart1 && isNeedInspStart2 && (isWK1Ready && isWK2Ready && isWK3Ready) &&

                                RAL.GetSeqStep() != CRail::scMvAuto &&
                                RAL.GetSeqStep() != CRail::scMvPrb  &&
                                !isCycleMvPrb;

        bool isConEnd        = !isNeedInspStart1 || !isNeedInspStart2;

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
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 6000 )) {
        EM_SetErr(eiHED_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
            //if(!OM.CmnOptn.bVs1Skip) VC.SendReset(vrVisn1) ;
            //if(!OM.CmnOptn.bVs2Skip) VC.SendReset(vrVisn2) ;
            //if(!OM.CmnOptn.bVs3Skip) VC.SendReset(vrVisn3) ;

            if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
            if(DM.ARAY[riWR1].GetCntExist())DM.ARAY[riWR1].ChangeStat(csWork,csUnkwn) ;

            if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
            if(DM.ARAY[riWR2].GetCntExist())DM.ARAY[riWR2].ChangeStat(csWork,csUnkwn) ;

            if(DM.ARAY[riVS3].GetCntExist())DM.ARAY[riVS3].SetStat(csUnkwn) ;
            if(DM.ARAY[riWR3].GetCntExist())DM.ARAY[riWR3].ChangeStat(csWork,csUnkwn) ;

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
    int iRowCnt ;

    bool r1 , r2 , r3 , r4 ;

    //메뉴얼 동작중에 에러 뜨면 메뉴얼이 중간에 멈춘다.
    bool bManualInsp = !Step.iSeq ;

    //Cycle.
    switch (Step.iCycle) {

        default : sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                  if(Step.iHome != PreStep.iHome)Trace(m_sPartName.c_str(), sTemp.c_str());
                  Step.iCycle = 0 ;
                  return true ;

        case  10: m_bLeftToRight = true ;
                  Step.iCycle++;
                  return false ;

        case  11: MoveMotr(miWRK_YVsn,piWRK_YVsnWork  );
                  MoveMotr(miWRK_XVsn,piWRK_XVsnTrgStt);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miWRK_YVsn,piWRK_YVsnWork  )) return false ;
                  if(!MoveMotr(miWRK_XVsn,piWRK_XVsnTrgStt)) return false ;

                  ResetTrgPos() ; //트리거 세팅 클리어
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!IO_GetX(xHED_1Ready)&&!OM.CmnOptn.bVs1Skip){
                      EM_SetErr(eiHED_Visn1NotReady) ;
                      VC.SendReset(vrVisn1) ;
                      if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
                      if(DM.ARAY[riWR1].GetCntExist())DM.ARAY[riWR1].ChangeStat(csWork,csUnkwn) ;

                      Step.iCycle = 0 ;
                      return true;
                  }
                  if(!IO_GetX(xHED_2Ready)&&!OM.CmnOptn.bVs2Skip){
                      EM_SetErr(eiHED_Visn2NotReady) ;
                      VC.SendReset(vrVisn2) ;
                      if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
                      if(DM.ARAY[riWR2].GetCntExist())DM.ARAY[riWR2].ChangeStat(csWork,csUnkwn) ;

                      Step.iCycle = 0 ;
                      return true;
                  }
                  if(!IO_GetX(xHED_3Ready)&&!OM.CmnOptn.bVs3Skip){
                      EM_SetErr(eiHED_Visn3NotReady) ;
                      VC.SendReset(vrVisn3) ;
                      if(DM.ARAY[riVS3].GetCntExist())DM.ARAY[riVS3].SetStat(csUnkwn) ;
                      if(DM.ARAY[riWR3].GetCntExist())DM.ARAY[riWR3].ChangeStat(csWork,csUnkwn) ;

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
        //이세뀌 호완안되 아 배아파...패스...
                  //r = OM.DevInfo.iRowCnt - r/OM.DevInfo.iRowInspCnt * OM.DevInfo.iRowInspCnt -1;
                  // 10 - 0 / 3 * 3 - 1 = 9
                  // 10 - 1 / 3 * 3 - 1 = 9
                  // 10 - 2 / 3 * 3 - 1 = 9
                  // 10 - 3 / 3 * 3 - 1 = 6
                  // 10 - 4 / 3 * 3 - 1 = 6
                  // 10 - 5 / 3 * 3 - 1 = 6
                  // 10 - 6 / 3 * 3 - 1 = 3
                  // 10 - 7 / 3 * 3 - 1 = 3
                  // 10 - 8 / 3 * 3 - 1 = 3
                  // 10 - 9 / 3 * 3 - 1 = 0

                  //10줄에 5개 그룹 카운트 3개 2개 3개 2개 (Insp Cnt 3개) 라서 이렇게 수정함. dd
                  iRowCnt = OM.DevInfo.iRowInspCnt ;

                  for(int i=1; i< OM.DevInfo.iRowInspCnt ; i++ ) {
                      if(r &&OM.DevInfo.iRowGrCnt && (r + i )  % OM.DevInfo.iRowGrCnt  == 0 ){
                          iRowCnt = OM.DevInfo.iRowGrCnt % OM.DevInfo.iRowInspCnt ;
                      }
                  }


                  r = DM.ARAY[riVS3].GetMaxRow() -r -1 ;

                  for(c = 0 ; c < DM.ARAY[riVS1].GetMaxCol() ; c++ ) {
                      for(int i = 0 ; i < iRowCnt ; i ++) {
                          if(DM.ARAY[riVS1].CheckAllExist())DM.ARAY[riVS1].SetStat(r-i,c,csWork) ;
                          if(DM.ARAY[riVS2].CheckAllExist())DM.ARAY[riVS2].SetStat(r-i,c,csWork) ;
                          if(DM.ARAY[riVS3].CheckAllExist())DM.ARAY[riVS3].SetStat(r-i,c,csWork) ;
                      }
                  }
/*
                  r = OM.DevInfo.iRowCnt - r/OM.DevInfo.iRowInspCnt * OM.DevInfo.iRowInspCnt -1;


                  //r = DM.ARAY[riVS3].GetMaxRow() -r -1 ;
                  //c = DM.ARAY[riVS3].GetMaxCol() -c -1 ;


                  for(c = 0 ; c < DM.ARAY[riVS1].GetMaxCol() ; c++ ) {
                      for(int i = 0 ; i < OM.DevInfo.iRowInspCnt ; i ++) {
                          if(DM.ARAY[riVS1].CheckAllExist())DM.ARAY[riVS1].SetStat(r-i,c,csWork) ;
                          if(DM.ARAY[riVS2].CheckAllExist())DM.ARAY[riVS2].SetStat(r-i,c,csWork) ;
                          if(DM.ARAY[riVS3].CheckAllExist())DM.ARAY[riVS3].SetStat(r-i,c,csWork) ;
                      }
                  }
*/
                  r1 = (/*DM.ARAY[riWK1].CheckAllExist() && */DM.ARAY[riVS1].GetCntStat(csUnkwn));
                  r2 = (/*DM.ARAY[riWK2].CheckAllExist() && */DM.ARAY[riVS2].GetCntStat(csUnkwn));
                  r3 = (/*DM.ARAY[riWK2].CheckAllExist() && */DM.ARAY[riVS3].GetCntStat(csUnkwn));


                  //Stat.bReqAllInsp = true ;
                  if(r1 || r2 || r3) {
                      m_bLeftToRight = !m_bLeftToRight ;
                      Step.iCycle=11 ; //Next Row Insp
                      return false ;

                  }
                  Step.iCycle++ ;
                  return false ;

        case  16: //한스트립을 다 검사했을 경우.
                  MoveMotr(miWRK_YVsn,piWRK_YVsnWait) ;
                  Step.iCycle++ ;
                  return false ;

        //17 Step Used When TimeOut
        case  17: if(!MoveMotr(miWRK_YVsn,piWRK_YVsnWait)) return false ;
                  MoveMotr(miWRK_XVsn,piWRK_XVsnWait  ) ;
        
                  Step.iCycle++ ;
                  return false ;

        //17 Step Used When TimeOut
        case  18: if(!OM.CmnOptn.bVs1Skip && !IO_GetX(xHED_1End)) return false ;
                  if(!OM.CmnOptn.bVs2Skip && !IO_GetX(xHED_2End)) return false ;
                  if(!OM.CmnOptn.bVs3Skip && !IO_GetX(xHED_3End)) return false ;

                  VC.ClearErrCnt();

                  //Vision 1
                  if(OM.CmnOptn.bVs1Skip) {
                      if(DM.ARAY[riWR1].CheckAllExist())DM.ARAY[riWR1].ChangeStat(csUnkwn,csWork) ;
                  }
                  else {
                      if(DM.ARAY[riWR1].CheckAllExist()){
                          HED.DpAray1.SetStat(csWork);
                          /*iVs1FailCnt =*/
                          VC.SimpleReadResult(1,&HED.DpAray1);
                          if(!VC.ReadResult(1,riWR1)){ //비젼 데이터 읽어 오기.
                              if(!bManualInsp)EM_SetErr(eiHED_Visn1Read);
                              DM.ARAY[riVS1].SetStat(csUnkwn) ;
                              if(DM.ARAY[riVS2].GetCntExist()) DM.ARAY[riVS2].SetStat(csUnkwn) ;
                              if(DM.ARAY[riVS3].GetCntExist()) DM.ARAY[riVS3].SetStat(csUnkwn) ;
                              //VC.SendReset(vrVisn1); //쓰레드를 켜서 비젼 리셑.
                              //if(!OM.CmnOptn.bVs2Skip) VC.SendReset(vrVisn2) ;
                              //if(!OM.CmnOptn.bVs3Skip) VC.SendReset(vrVisn3) ;
                              Step.iCycle=0 ;
                              return true ;
                          }
                      }
                  }

                  //Vision2
                  if(OM.CmnOptn.bVs2Skip) {
                      if(DM.ARAY[riWR2].CheckAllExist())DM.ARAY[riWR2].ChangeStat(csUnkwn,csWork) ;
                  }
                  else{
                      if(DM.ARAY[riWR2].CheckAllExist() ){ //가라 같네요.밑에도.
                          HED.DpAray2.SetStat(csWork);

                          /*iVs2FailCnt =*/
                          VC.SimpleReadResult(2,&HED.DpAray2);
                          if(!VC.ReadResult(2,riWR2)){
                              if(!bManualInsp)EM_SetErr(eiHED_Visn2Read);
                              if(DM.ARAY[riVS1].GetCntExist()) DM.ARAY[riVS1].SetStat(csUnkwn) ;
                              DM.ARAY[riVS2].SetStat(csUnkwn) ;
                              if(DM.ARAY[riVS3].GetCntExist()) DM.ARAY[riVS3].SetStat(csUnkwn) ;
                              //VC.SendReset(vrVisn2); //쓰레드를 켜서 비젼 리셑.
                              //if(!OM.CmnOptn.bVs1Skip) VC.SendReset(vrVisn1) ;
                              //if(!OM.CmnOptn.bVs3Skip) VC.SendReset(vrVisn3) ;
                              Step.iCycle=0 ;
                              return true ;
                          }
                      }
                  }

                  //Vision3
                  if(OM.CmnOptn.bVs3Skip) {
                      if(DM.ARAY[riWR3].CheckAllExist())DM.ARAY[riWR3].ChangeStat(csUnkwn,csWork) ;
                  }
                  else {
                      if(DM.ARAY[riWR3].CheckAllExist() ){
                          HED.DpAray3.SetStat(csWork);
                          /*iVs3FailCnt =*/
                          VC.SimpleReadResult(3,&HED.DpAray3);
                          if(!VC.ReadResult(3,riWR3)){
                              if(!bManualInsp)EM_SetErr(eiHED_Visn2Read);
                              if(DM.ARAY[riVS1].GetCntExist()) DM.ARAY[riVS1].SetStat(csUnkwn) ;
                              if(DM.ARAY[riVS2].GetCntExist()) DM.ARAY[riVS2].SetStat(csUnkwn) ;
                              DM.ARAY[riVS3].SetStat(csUnkwn) ;
                              //VC.SendReset(vrVisn3); //쓰레드를 켜서 비젼 리셑.
                              //if(!OM.CmnOptn.bVs1Skip) VC.SendReset(vrVisn1) ;
                              //if(!OM.CmnOptn.bVs2Skip) VC.SendReset(vrVisn2) ;
                              Step.iCycle=0 ;
                              return true ;
                          }
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
    if(!MT_GetStop(miWRK_YVsn)) return false ;
    if(!MT_GetStop(miWRK_XVsn)) return false ;
//    if(!MT_GetStop(miWK2_YVsn)) return false ;
//    if(!MT_GetStop(miWRK_XVsn)) return false ;

//    if(!AT_Done(aiHED_UVLight)) return false ;

    return true ;
}

void CHead::Load(bool IsLoad)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "SeqData\\" + m_sPartName.Trim() +".INI";

    //Load Device.
    Trace( "CHead","`Load Start") ;

    if(IsLoad) {
        //UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "iVs1FailCnt " , iVs1FailCnt   );
        //UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "iVs2FailCnt " , iVs2FailCnt   );
        //UserINI.Load(sPath.c_str()  , m_sPartName.c_str()  , "iVs3FailCnt " , iVs3FailCnt   );

    }
    else {
        //UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "iVs1FailCnt " , iVs1FailCnt   );
        //UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "iVs2FailCnt " , iVs2FailCnt   );
        //UserINI.Save(sPath.c_str()  , m_sPartName.c_str()  , "iVs3FailCnt " , iVs3FailCnt   );
    }
    Trace( "CHead","Load End") ;
}
















