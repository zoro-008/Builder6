//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "Head.h"
#include "Rail.h"
#include "PreBuff.h"

#include "SLogUnit.h"
#include "SMInterfaceUnit.h"
#include "UtilDefine.h"
#include "PstnMan.h"
#include "OptionMan.h"
#include "VisnComUnit.h"
#include "UserINI.h"
//---------------------------------------------------------------------------
#include "Math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define Visn_One 1
#define Visn_Two 2
//sun MT.AllDone
//sun AT.AllDone만들기.
//---------------------------------------------------------------------------
CHead  HED;

enum EN_INSP_DIRC{idLeftTop = 0 , idLeftBtm = 1 };
EN_INSP_DIRC HEAD_INSP_DIRECTION = idLeftBtm ;

int GetRightStartCol() //Col이 홀수 열일때 왼쪽이 한줄많게 할당 되야 한다.
{
    int iColCnt = (OM.DevInfo.iColCnt % 2) ? OM.DevInfo.iColCnt + 1 : OM.DevInfo.iColCnt ;

    iColCnt = iColCnt /2 ;

    return iColCnt ;
}

int GetVtlColCnt ()
{
    //2개로 해드가 나눠졌을때.. 홀수 이면
    int iColVtlCnt = (OM.DevInfo.iColCnt % 2) && (!OM.CmnOptn.bCam1Skip&&!OM.CmnOptn.bCam2Skip)  ? OM.DevInfo.iColCnt  : OM.DevInfo.iColCnt + 1 ;

    int iColCnt = /*(OM.CmnOptn.bCam1Skip||OM.CmnOptn.bCam2Skip) ? iColVtlCnt/2 :*/ OM.DevInfo.iColCnt ;

    int iRet = iColCnt ;
    if(OM.DevInfo.iColInspCnt) {
        iRet = (iColCnt % OM.DevInfo.iColInspCnt) ?
              ((iColCnt / OM.DevInfo.iColInspCnt) * OM.DevInfo.iColInspCnt + OM.DevInfo.iColInspCnt) :
              ((iColCnt / OM.DevInfo.iColInspCnt) * OM.DevInfo.iColInspCnt) ;
    }
    return iRet ;
}
int GetVtlRowCnt () //마지막 찌끄레기 열까지 감안한 RowCnt.
{
    int iRet = 0 ;
    if(OM.DevInfo.iRowInspCnt) {
        iRet = (OM.DevInfo.iRowCnt % OM.DevInfo.iRowInspCnt) ?
              ((OM.DevInfo.iRowCnt / OM.DevInfo.iRowInspCnt) * OM.DevInfo.iRowInspCnt) :
              ((OM.DevInfo.iRowCnt / OM.DevInfo.iRowInspCnt) * OM.DevInfo.iRowInspCnt + OM.DevInfo.iRowInspCnt) ;
    }
    return iRet ;
}

//double GetLeftPos ()
//{
//    //사용 안함.
////    double dRet = (PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStartPs) + (OM.DevInfo.iColInspCnt - 1) * OM.DevInfo.dColPitch / 2.0) ;
//    return dRet ;
//}
//double GetRightPos ()
//{
//    //사용 안함.
//    int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
//    int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;
//    double dRet = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStartPs) +
//                  OM.DevInfo.dColPitch * GetVtlColCnt() +
//                  (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) *        (iColOneGrCnt ? (GetVtlColCnt()/iColOneGrCnt) : 0) +
//                  (OM.DevInfo.iColInspCnt - 1) * OM.DevInfo.dColPitch / 2.0 ;
//    return dRet ;
//}
//
//double GetLeftPosCt()
//{
//    double dRet = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStartPs) ;
//    return dRet ;
//}
//
//double GetRightPosCt()
//{
//    int iColOneGrCnt = OM.DevInfo.iColGrCnt ? OM.DevInfo.iColCnt / OM.DevInfo.iColGrCnt : 0;
//    int iRowOneGrCnt = OM.DevInfo.iRowGrCnt ? OM.DevInfo.iRowCnt / OM.DevInfo.iRowGrCnt : 0;
//
//    double dRet = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStartPs) + OM.DevInfo.dColPitch * GetVtlColCnt() +
//                  (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch) * (iColOneGrCnt ? (GetVtlColCnt()/iColOneGrCnt) : 0) -
//                  OM.DevInfo.iColInspCnt * OM.DevInfo.dColPitch ;
//                  //(OM.DevOptn.iColInspCnt - 1) * OM.DevInfo.dColPitch / 2.0 ;
//    return dRet ;
//}

bool GetLtToRt(int r)
{
    //이렇게 하면 디바이스 셑업 할때 검사 열수에 따라 병신짓함.
    //if(HEAD_INSP_DIRECTION == idLeftBtm) return !((OM.DevInfo.iRowInspCnt ? r/OM.DevInfo.iRowInspCnt : r) %2);
    //else                                 return  ((OM.DevInfo.iRowInspCnt ? r/OM.DevInfo.iRowInspCnt : r) %2);

    //왼쪽 앞으로 재정의.
    int br ;

    if(HEAD_INSP_DIRECTION == idLeftTop) br = r                         ;
    else                                 br = OM.DevInfo.iRowCnt - r -1 ;

    return !((OM.DevInfo.iRowInspCnt ? br/OM.DevInfo.iRowInspCnt : br) %2);
}




CHead::CHead(void)
{
    Init();
}

CHead::~CHead (void)
{
    //Close();
}

void CHead::Init()
{
    m_sPartName = "Head " ;

    Reset();

    //Make Dir.
    AnsiString  DevName ;
    AnsiString  BinPath ;
    AnsiString  IniPath ;
    AnsiString  sTemp1  ;
    AnsiString  sTemp2  ;

    //Read&Write.
    sTemp1 =  AnsiString("HeadDpAray1")+ AnsiString(".dat");
    sTemp2 =  AnsiString("HeadDpAray1")+ AnsiString(".ini");
    BinPath = EXE_FOLDER + "SeqData\\"+sTemp1 ;
    IniPath = EXE_FOLDER + "SeqData\\"+sTemp2 ;
    DpAray1.Load(true , BinPath , IniPath , 1);

    sTemp1 =  AnsiString("HeadDpAray2")+ AnsiString(".dat");
    sTemp2 =  AnsiString("HeadDpAray2")+ AnsiString(".ini");
    BinPath = EXE_FOLDER + "SeqData\\"+sTemp1 ;
    IniPath = EXE_FOLDER + "SeqData\\"+sTemp2 ;
    DpAray2.Load(true , BinPath , IniPath , 2);

    sTemp1 =  AnsiString("HeadDpAray3")+ AnsiString(".dat");
    sTemp2 =  AnsiString("HeadDpAray3")+ AnsiString(".ini");
    BinPath = EXE_FOLDER + "SeqData\\"+sTemp1 ;
    IniPath = EXE_FOLDER + "SeqData\\"+sTemp2 ;
    DpAray3.Load(true , BinPath , IniPath , 3);

    Load(true);
}

void CHead::Close()
{
    //Make Dir.
    AnsiString  DevName ;
    AnsiString  BinPath ;
    AnsiString  IniPath ;
    AnsiString  sTemp1  ;
    AnsiString  sTemp2  ;

    //Read&Write.
    sTemp1 =  AnsiString("HeadDpAray1")+ AnsiString(".dat");
    sTemp2 =  AnsiString("HeadDpAray1")+ AnsiString(".ini");
    BinPath = EXE_FOLDER + "SeqData\\"+sTemp1 ;
    IniPath = EXE_FOLDER + "SeqData\\"+sTemp2 ;
    DpAray1.Load(false , BinPath , IniPath , 1);

    sTemp1 =  AnsiString("HeadDpAray2")+ AnsiString(".dat");
    sTemp2 =  AnsiString("HeadDpAray2")+ AnsiString(".ini");
    BinPath = EXE_FOLDER + "SeqData\\"+sTemp1 ;
    IniPath = EXE_FOLDER + "SeqData\\"+sTemp2 ;
    DpAray2.Load(false , BinPath , IniPath , 2);

    sTemp1 =  AnsiString("HeadDpAray3")+ AnsiString(".dat");
    sTemp2 =  AnsiString("HeadDpAray3")+ AnsiString(".ini");
    BinPath = EXE_FOLDER + "SeqData\\"+sTemp1 ;
    IniPath = EXE_FOLDER + "SeqData\\"+sTemp2 ;
    DpAray3.Load(false , BinPath , IniPath , 3);

    Load(false);
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
        //r = DM.ARAY[riVS3].GetMaxRow() -r -1 ;
        //c = DM.ARAY[riVS3].GetMaxCol() -c -1 ;
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
    int r  , c  ,rr;
    const double dOfsforTrg = OM.MstOptn.dTrigerOffset ; //아진 인터럽트 트리거를 위한 옵셑.
    const double defaultOfs = 1;  //트리거 옵션만으로도 위치 안맞음;;JS
    double dStartYPos ;
    const bool bPkgCntr = false ;

    //요놈. 플라잉 방식이라 맵핑 못씀.
    FindChip( r , c ) ;
    //r = OM.DevInfo.iRowInspCnt ? r/OM.DevInfo.iRowInspCnt  * OM.DevInfo.iRowInspCnt  :  r ;

    bool bLeftToRight = GetLtToRt(r);

    dStartYPos = PM.GetValue(miWRK_YVsn , pvWRK_YVsnWorkStart ) ;

    //스타트 포지션과 엔드 포지션 구분.
    double dXStartPos ;
    double dXEndPos   ;
    double dYPos      ;
    double dXOffsetWrkStt ;

/**/double dLeftPosCt  ;
/**/double dRightPosCt ;
    /*
    if(!OM.CmnOptn.bCam1Skip && !OM.CmnOptn.bCam2Skip) { //normal...
        dLeftPosCt  = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStart ) ;
        dRightPosCt = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkEnd   ) ;
    }
    else if (OM.CmnOptn.bCam1Skip && !OM.CmnOptn.bCam2Skip) { //not use left
        dLeftPosCt  = PM.GetValue(miWRK_XVsn , pvWRK_XVsnRWorkStart ) ;
        dRightPosCt = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkEnd    ) ;
    }
    else if (!OM.CmnOptn.bCam1Skip && OM.CmnOptn.bCam2Skip) { //not use Right
        dLeftPosCt  = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStart ) ;
        dRightPosCt = PM.GetValue(miWRK_XVsn , pvWRK_XVsnLWorkEnd  ) ;
    }
    else { //bing sin.
        dLeftPosCt  = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStart ) ;
        dRightPosCt = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkEnd   ) ;
    }
    */
    //double

    double Temp1 = (OM.DevInfo.iColGrCnt         ? OM.DevInfo.iColCnt / Visn_Two / OM.DevInfo.iColGrCnt         * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch):0) ;
    double dVisnOnePitch = (OM.DevInfo.iColCnt / Visn_One * OM.DevInfo.dColPitch) +
                           (OM.DevInfo.iColGrCnt         ? OM.DevInfo.iColCnt / Visn_One / OM.DevInfo.iColGrCnt         * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch):0) ;
                           //(OM.DevInfo.iColSubGroupCount ? OM.DevInfo.iColCnt / Visn_One / OM.DevInfo.iColSubGroupCount * (OM.DevInfo.dColSubGroupGap - OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch):0) ;

    double dVisnTwoPitch = (OM.DevInfo.iColCnt / Visn_Two * OM.DevInfo.dColPitch) +
                           (OM.DevInfo.iColGrCnt         ? OM.DevInfo.iColCnt / Visn_Two / OM.DevInfo.iColGrCnt         * (OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch):0) ;
                           //(OM.DevInfo.iColSubGroupCount ? OM.DevInfo.iColCnt / Visn_Two / OM.DevInfo.iColSubGroupCount * (OM.DevInfo.dColSubGroupGap - OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch):0) ;

    if(!OM.CmnOptn.bCam1Skip && !OM.CmnOptn.bCam2Skip) { //normal...
        dLeftPosCt  = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStart ) ;
        dRightPosCt = dLeftPosCt + dVisnTwoPitch;
    }
    else if (OM.CmnOptn.bCam1Skip && !OM.CmnOptn.bCam2Skip) { //not use left
        dLeftPosCt  = PM.GetValue(miWRK_XVsn , pvWRK_XVsnRWorkStart ) ;
        dRightPosCt = dLeftPosCt + dVisnTwoPitch ;
    }
    else if (!OM.CmnOptn.bCam1Skip && OM.CmnOptn.bCam2Skip) { //not use Right
        dLeftPosCt  = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStart ) ;
        dRightPosCt = dLeftPosCt + dVisnOnePitch ;
    }
    else { //bing sin.
        dLeftPosCt  = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStart ) ;
        dRightPosCt = dLeftPosCt + dVisnOnePitch ;
    }

    if(bLeftToRight) {
        dXStartPos = dLeftPosCt  - dOfsforTrg - defaultOfs;
        dXEndPos   = dRightPosCt + dOfsforTrg + defaultOfs;
    }
    else {
        dXStartPos = dRightPosCt + dOfsforTrg + defaultOfs;
        dXEndPos   = dLeftPosCt  - dOfsforTrg - defaultOfs;
    }

    dXOffsetWrkStt = dLeftPosCt  - dOfsforTrg ;

    //여기서수정함. 2012 11 19 김동우. Y로 서브 그룹 카운트 적용함.
    double dSubRowGroupGap = 0.0 ;

    if(OM.DevInfo.iRowSubGroupCount) dSubRowGroupGap = OM.DevInfo.dRowSubGroupGap - OM.DevInfo.dRowPitch ;
    else                             dSubRowGroupGap = 0.0 ;

    if(HEAD_INSP_DIRECTION == idLeftTop) {
        rr    = r ;
        dYPos = dStartYPos +                                           //첫포지션 더하고.
        rr * OM.DevInfo.dRowPitch +    //검사열을 감안해서 해당 로우 까지 거리계산해서 더하고.
        //(OM.DevOptn.iRowInspCnt - 1) * OM.DevInfo.dRowPitch / 2.0 +  // 요거는

        //여기서수정함. 2012 11 19 김동우. Y로 서브 그룹 카운트 적용함.
        //여기서 수정함. 2014 03 31 고준선. x서브 그룹 추가 함.
        //원본
//        (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * (OM.DevInfo.iRowGrCnt?(rr+1)/OM.DevInfo.iRowGrCnt : 0 ) ; //그룹 갯수 개산 해서 더함. // 기존에 이렇게 되있던거 수정.
        //수정본
        //너 ㅄ이다 안써서 다행인줄 아셈???????...rr+1 이거 아니잖아.dd
        (OM.DevInfo.dRowGrGap       - OM.DevInfo.dRowPitch - dSubRowGroupGap) * (OM.DevInfo.iRowGrCnt?(rr)/OM.DevInfo.iRowGrCnt : 0 ) + //그룹 갯수 개산 해서 더함.
        (dSubRowGroupGap                                                    ) * (OM.DevInfo.iRowSubGroupCount?(rr)/OM.DevInfo.iRowSubGroupCount : 0 ) ; //SUB 그룹 갯수 개산 해서 더함.


    }

    if(HEAD_INSP_DIRECTION == idLeftBtm) {
        rr    = OM.DevInfo.iRowCnt - r -1 ;
        dYPos = dStartYPos -                                           //첫포지션 더하고.
        rr * OM.DevInfo.dRowPitch -    //검사열을 감안해서 해당 로우 까지 거리계산해서 더하고.
        //(OM.DevOptn.iRowInspCnt - 1) * OM.DevInfo.dRowPitch / 2.0 +  // 요거는
        //여기서수정함. 2012 11 19 김동우. Y로 서브 그룹 카운트 적용함.
        //원본
//        (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch) * (OM.DevInfo.iRowGrCnt?(rr+1)/OM.DevInfo.iRowGrCnt : 0 ) ; //그룹 갯수 개산 해서 더함.
        //수정본
        (OM.DevInfo.dRowGrGap - OM.DevInfo.dRowPitch - dSubRowGroupGap) * (OM.DevInfo.iRowGrCnt?(rr)/OM.DevInfo.iRowGrCnt : 0 ) - //그룹 갯수 개산 해서 더함.
        (dSubRowGroupGap                                              ) * (OM.DevInfo.iRowSubGroupCount?(rr)/OM.DevInfo.iRowSubGroupCount : 0 ) ; //SUB 그룹 갯수 개산 해서 더함.

    }


    double dPos = 0.0 ;

    if(_iMotr == miWRK_XVsn){
        switch(_iPstnId) {
            default                     : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWRK_XVsnWait         : dPos = PM.GetValue (_iMotr , pvWRK_XVsnWait        ); break ;
            case piWRK_XVsnWorkStart    : dPos = PM.GetValue (_iMotr , pvWRK_XVsnWorkStart   ); break ;
            case piWRK_XVsnWorkEnd      : dPos = dRightPosCt                                  ; break ;
            case piWRK_XVsnRWorkStart   : dPos = PM.GetValue (_iMotr , pvWRK_XVsnRWorkStart  ); break ;
            case piWRK_XVsnLWorkEnd     : dPos = PM.GetValue (_iMotr , pvWRK_XVsnLWorkEnd    ); break ;
            case piWRK_XVsnTrgStt       : dPos = dXStartPos                                   ; break ;
            case piWRK_XVsnTrgEnd       : dPos = dXEndPos                                     ; break ;
            case piWRK_XVsnOffsetWrkStt : dPos = dXOffsetWrkStt                               ; break ;
        }
    }

    else if(_iMotr == miWRK_YVsn){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWRK_YVsnWait      : dPos = PM.GetValue (_iMotr , pvWRK_YVsnWait        ); break ;
            case piWRK_YVsnWorkStart : dPos = PM.GetValue (_iMotr , pvWRK_YVsnWorkStart   ); break ;
            case piWRK_YVsnWork      : dPos = dYPos                                        ; break ;
        }
    }

    else if(_iMotr == miWR1_XCvr){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWR1_XCvrWait      : dPos = PM.GetValue (_iMotr , pvWR1_XCvrWait        ); break ;
            case piWR1_XCvrWork      : dPos = PM.GetValue (_iMotr , pvWR1_XCvrWork        ); break ;
        }
    }

    else if(_iMotr == miWR2_XCvr){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWR2_XCvrWait      : dPos = PM.GetValue (_iMotr , pvWR2_XCvrWait        ); break ;
            case piWR2_XCvrWork      : dPos = PM.GetValue (_iMotr , pvWR2_XCvrWork        ); break ;
        }
    }

    else if(_iMotr == miWR3_XCvr){
        switch(_iPstnId) {
            default                  : dPos = MT_GetCmdPos(_iMotr                         ); break ;
            case piWR3_XCvrWait      : dPos = PM.GetValue (_iMotr , pvWR3_XCvrWait        ); break ;
            case piWR3_XCvrWork      : dPos = PM.GetValue (_iMotr , pvWR3_XCvrWork        ); break ;
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

    AnsiString sMsg ;
    bool       bRet = true ;


    if(_iMotr == miWRK_XVsn){
        switch(_iPstnId){
            default                  : break ;
            case piWRK_XVsnWait      : break ;
            case piWRK_XVsnWorkStart : break ;
            case piWRK_XVsnTrgStt    : break ;
            case piWRK_XVsnTrgEnd    : break ;
        }
    }

    else if(_iMotr == miWRK_YVsn){
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
    double dSttPos  ;
    double dTemp    ;
    int r,c ;

    AnsiString sTemp ;

    //카메라 한쪽만 쓸때 계산해야함...
         if(!OM.CmnOptn.bCam1Skip && !OM.CmnOptn.bCam2Skip) {iTrgCnt = iTrgCnt / 2 ; dSttPos = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStart ); }
    else if( OM.CmnOptn.bCam1Skip && !OM.CmnOptn.bCam2Skip) {iTrgCnt = iTrgCnt     ; dSttPos = PM.GetValue(miWRK_XVsn , pvWRK_XVsnRWorkStart); }
    else if(!OM.CmnOptn.bCam1Skip &&  OM.CmnOptn.bCam2Skip) {iTrgCnt = iTrgCnt     ; dSttPos = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStart ); }
    else                                                    {iTrgCnt = iTrgCnt / 2 ; dSttPos = PM.GetValue(miWRK_XVsn , pvWRK_XVsnWorkStart ); }

    double * dTrgPos = new double[iTrgCnt] ;

    FindChip(r,c );

    bool bLeftToRight = GetLtToRt(r);

    //2014.3.31 그룹 한개 더 추가 JS

    double dSubColGroupGap = 0.0 ;
    double dGrPitch        = OM.DevInfo.dColGrGap - OM.DevInfo.dColPitch ;
    int    iSubCnt         = 0;

    if(OM.DevInfo.iColSubGroupCount) dSubColGroupGap = OM.DevInfo.dColSubGroupGap - OM.DevInfo.dColPitch ;
    else                             dSubColGroupGap = 0.0 ;

    //r = r/OM.DevInfo.iRowInspCnt * OM.DevInfo.iRowInspCnt ;

    for(int i = 0 ; i < iTrgCnt ; i++) {
        dTemp = dSttPos +                    //첫포지션
                OM.DevInfo.dColPitch * OM.DevInfo.iColInspCnt * i +
               (OM.DevInfo.iColGrCnt ? dGrPitch * OM.DevInfo.iColInspCnt * (i) / OM.DevInfo.iColGrCnt : 0 ) ;


        if(OM.DevInfo.iColSubGroupCount){
            iSubCnt = (OM.DevInfo.iColInspCnt * i) / OM.DevInfo.iColSubGroupCount ;
            dTemp = dTemp + (iSubCnt * dSubColGroupGap) ;
        }


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

        default: if(Step.iHome) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iHome=%02d" , __FUNC__ , Step.iHome );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iHome = 0 ;
                 return true ;

        case 10:

                 MT_Reset(miWRK_XVsn);
                 MT_Reset(miWRK_YVsn);

                 MT_Reset(miWR1_XCvr);
                 MT_Reset(miWR2_XCvr);
                 MT_Reset(miWR3_XCvr);

                 MT_SetServo(miWRK_XVsn,true);
                 MT_SetServo(miWRK_YVsn,true);

                 MT_SetServo(miWR1_XCvr,true);
                 MT_SetServo(miWR2_XCvr,true);
                 MT_SetServo(miWR3_XCvr,true);

                 //트리거 삽질 방지.
                 SetTrgPos();
                 ResetTrgPos();

                 MT_DoHome(miWRK_YVsn) ;
                 MT_DoHome(miWRK_XVsn) ;

                 MT_DoHome(miWR1_XCvr) ;
                 MT_DoHome(miWR2_XCvr) ;
                 MT_DoHome(miWR3_XCvr) ;
                 Step.iHome++ ;
                 return false ;

        case 11: if(!MT_GetHomeEnd(miWRK_YVsn))return false ;
                 if(!MT_GetHomeEnd(miWRK_XVsn))return false ;
                 if(!MT_GetHomeEnd(miWR1_XCvr))return false ;
                 if(!MT_GetHomeEnd(miWR2_XCvr))return false ;
                 if(!MT_GetHomeEnd(miWR3_XCvr))return false ;

//                 MT_GoAbsMan(miWRK_YVsn , GetMotrPos(miWRK_YVsn , piWRK_YVsnWait));
                 MT_GoAbsMan(miWRK_XVsn , GetMotrPos(miWRK_XVsn , piWRK_XVsnWait));
                 MT_GoAbsMan(miWRK_YVsn , 0);


                 MT_GoAbsMan(miWR1_XCvr , GetMotrPos(miWR1_XCvr , piWR1_XCvrWait));
                 MT_GoAbsMan(miWR2_XCvr , GetMotrPos(miWR2_XCvr , piWR2_XCvrWait));
                 MT_GoAbsMan(miWR3_XCvr , GetMotrPos(miWR3_XCvr , piWR3_XCvrWait));


                 Step.iHome++ ;
                 return false ;


        case 12: //if(!MT_GoAbsMan(miWRK_YVsn , GetMotrPos(miWRK_YVsn , piWRK_YVsnWait))) return false ;
                 if(!MT_GoAbsMan(miWRK_XVsn , GetMotrPos(miWRK_XVsn , piWRK_XVsnWait))) return false ;
                 if(!MT_GoAbsMan(miWRK_YVsn , 0)) return false ;

                 if(!MT_GoAbsMan(miWR1_XCvr , GetMotrPos(miWR1_XCvr , piWR1_XCvrWait))) return false ;
                 if(!MT_GoAbsMan(miWR2_XCvr , GetMotrPos(miWR2_XCvr , piWR2_XCvrWait))) return false ;
                 if(!MT_GoAbsMan(miWR3_XCvr , GetMotrPos(miWR3_XCvr , piWR3_XCvrWait))) return false ;

                 if(!OM.CmnOptn.bVs1_1Skip) VC.SendReset(vsVisn1_1) ;
                 if(!OM.CmnOptn.bVs2_1Skip) VC.SendReset(vsVisn2_1) ;
                 if(!OM.CmnOptn.bVs3_1Skip) VC.SendReset(vsVisn3_1) ;

                 if(!OM.CmnOptn.bVs1_2Skip) VC.SendReset(vsVisn1_2) ;
                 if(!OM.CmnOptn.bVs2_2Skip) VC.SendReset(vsVisn2_2) ;
                 if(!OM.CmnOptn.bVs3_2Skip) VC.SendReset(vsVisn3_2) ;

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
        default: if(Step.iToStart) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStart=%02d" , __FUNC__ , Step.iToStart );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStart = 0 ;
                 return true ;

        case 10: MoveMotr(miWRK_XVsn , piWRK_XVsnWait);
                 MoveMotr(miWRK_YVsn , piWRK_YVsnWait);

                 MoveMotr(miWR1_XCvr , piWR1_XCvrWork);
                 MoveMotr(miWR2_XCvr , piWR2_XCvrWork);
                 MoveMotr(miWR3_XCvr , piWR3_XCvrWork);




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
                 if(!MoveMotr(miWRK_YVsn , piWRK_YVsnWait)) return false;

                 if(!MoveMotr(miWR1_XCvr , piWR1_XCvrWork)) return false;
                 if(!MoveMotr(miWR2_XCvr , piWR2_XCvrWork)) return false;
                 if(!MoveMotr(miWR3_XCvr , piWR3_XCvrWork)) return false;

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
        default: if(Step.iToStop) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iToStop=%02d" , __FUNC__ , Step.iToStop );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                 }
                 Step.iToStop = 0;
                 return true ;

        case 10:
                 Step.iToStop ++ ;
                 return false ;

        case 11: Step.iToStop = 0   ;
                 return true ;
    }
}

bool CHead::Autorun(void) //오토런닝시에 계속 타는 함수.
{
    PreStep.iSeq = Step.iSeq ;

    //Check Error & Decide Step.
    if (Step.iSeq == 0) {
        if (Stat.bReqStop)return false ;

        if(EM_IsErr()) return false ;

        bool isPrbOut    = PRB.GetSeqStep() != CPreBuff::scOut ;

        bool isExistStrip  = (DM.ARAY[riWR1].GetCntExist() && DM.ARAY[riWR1].GetStep()==asInspBind ) ||
                             (DM.ARAY[riWR2].GetCntExist() && DM.ARAY[riWR2].GetStep()==asInspBind ) ||
                             (DM.ARAY[riWR3].GetCntExist() && DM.ARAY[riWR3].GetStep()==asInspBind );

        bool isBinded    = AT_Complete(aiWK1_SttnUpDn , ccFwd) &&
                           AT_Complete(aiWK2_SttnUpDn , ccFwd) &&
                           AT_Complete(aiWK3_SttnUpDn , ccFwd) ;


        bool isCycleInsp = (DM.ARAY[riVS1].GetCntStat  (csUnkwn) || DM.ARAY[riVS2].GetCntStat  (csUnkwn) || DM.ARAY[riVS3].GetCntStat  (csUnkwn) )&&
                            isExistStrip && isBinded ;
//                            isPrbOut ;
        bool isConEnd    = !isCycleInsp ;

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

void VisnInspReset()
{
    if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat   (csUnkwn) ;
    if(DM.ARAY[riWR1].GetCntExist())DM.ARAY[riWR1].ChangeStat(csWork,csUnkwn) ;
    if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat   (csUnkwn) ;
    if(DM.ARAY[riWR2].GetCntExist())DM.ARAY[riWR2].ChangeStat(csWork,csUnkwn) ;
    if(DM.ARAY[riVS3].GetCntExist())DM.ARAY[riVS3].SetStat   (csUnkwn) ;
    if(DM.ARAY[riWR3].GetCntExist())DM.ARAY[riWR3].ChangeStat(csWork,csUnkwn) ;

    if(!OM.CmnOptn.bVs1_1Skip) VC.SendReset(vsVisn1_1); //쓰레드를 켜서 비젼 리셑.
    if(!OM.CmnOptn.bVs1_2Skip) VC.SendReset(vsVisn1_2); //쓰레드를 켜서 비젼 리셑.
    if(!OM.CmnOptn.bVs2_1Skip) VC.SendReset(vsVisn2_1) ;
    if(!OM.CmnOptn.bVs2_2Skip) VC.SendReset(vsVisn2_2) ;
    if(!OM.CmnOptn.bVs3_1Skip) VC.SendReset(vsVisn3_1) ;
    if(!OM.CmnOptn.bVs3_2Skip) VC.SendReset(vsVisn3_2) ;
}
bool CHead::CycleInsp(void) //
{
    //Check Cycle Time Out.
    AnsiString sTemp ;
    if (m_tmCycle.OnDelay(Step.iCycle && Step.iCycle == PreStep.iCycle && CheckStop() && !OM.MstOptn.bDebugMode , 6000 )) {
        EM_SetErr(eiHED_CycleTO);
        sTemp = sTemp.sprintf("%s TIMEOUT STATUS : Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
        Trace(m_sPartName.c_str(),sTemp.c_str());
        VisnInspReset();
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

    int iRanR , iRanC , iRanS;


    //메뉴얼 동작중에 에러 뜨면 메뉴얼이 중간에 멈춘다.
    bool bManualInsp = !Step.iSeq ;

    //Cycle.
    switch (Step.iCycle) {

        default : if(Step.iCycle) {
                      sTemp = sTemp.sprintf("Cycle Default Clear %s Step.iCycle=%02d" , __FUNC__ , Step.iCycle );
                      Trace(m_sPartName.c_str(), sTemp.c_str());
                  }
                  Step.iCycle = 0 ;
                  return true ;

        case  10: Step.iCycle++;
                  return false ;

        case  11: MoveMotr(miWRK_YVsn,piWRK_YVsnWork  );
                  MoveMotr(miWRK_XVsn,piWRK_XVsnTrgStt);

                  MoveMotr(miWR1_XCvr , piWR1_XCvrWork);
                  MoveMotr(miWR2_XCvr , piWR2_XCvrWork);
                  MoveMotr(miWR3_XCvr , piWR3_XCvrWork);

                  Step.iCycle++ ;
                  return false ;

        case  12: if(!MoveMotr(miWRK_YVsn,piWRK_YVsnWork  )) return false ;
                  if(!MoveMotr(miWRK_XVsn,piWRK_XVsnTrgStt)) return false ;

                  if(!MoveMotr(miWR1_XCvr , piWR1_XCvrWork)) return false ;
                  if(!MoveMotr(miWR2_XCvr , piWR2_XCvrWork)) return false ;
                  if(!MoveMotr(miWR3_XCvr , piWR3_XCvrWork)) return false ;

                  ResetTrgPos() ; //트리거 세팅 클리어
                  Step.iCycle++ ;
                  return false ;

        case  13: if(!IO_GetX(xVisn1_1Ready)&&!OM.CmnOptn.bVs1_1Skip){
                      EM_SetErr(eiHED_Visn1_1NotReady) ;
                      VC.SendReset(vsVisn1_1) ;
                      if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
                      if(DM.ARAY[riWR1].GetCntExist())DM.ARAY[riWR1].ChangeStat(csWork,csUnkwn) ;
                      Step.iCycle = 0 ;
                      return true;
                  }
                  if(!IO_GetX(xVisn1_2Ready)&&!OM.CmnOptn.bVs1_2Skip){
                      EM_SetErr(eiHED_Visn1_2NotReady) ;
                      VC.SendReset(vsVisn1_2) ;
                      if(DM.ARAY[riVS1].GetCntExist())DM.ARAY[riVS1].SetStat(csUnkwn) ;
                      if(DM.ARAY[riWR1].GetCntExist())DM.ARAY[riWR1].ChangeStat(csWork,csUnkwn) ;
                      Step.iCycle = 0 ;
                      return true;
                  }

                  if(!IO_GetX(xVisn2_1Ready)&&!OM.CmnOptn.bVs2_1Skip){
                      EM_SetErr(eiHED_Visn2_1NotReady) ;
                      VC.SendReset(vsVisn2_1) ;
                      if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
                      if(DM.ARAY[riWR2].GetCntExist())DM.ARAY[riWR2].ChangeStat(csWork,csUnkwn) ;
                      Step.iCycle = 0 ;
                      return true;
                  }
                  if(!IO_GetX(xVisn2_2Ready)&&!OM.CmnOptn.bVs2_2Skip){
                      EM_SetErr(eiHED_Visn2_2NotReady) ;
                      VC.SendReset(vsVisn2_2) ;
                      if(DM.ARAY[riVS2].GetCntExist())DM.ARAY[riVS2].SetStat(csUnkwn) ;
                      if(DM.ARAY[riWR2].GetCntExist())DM.ARAY[riWR2].ChangeStat(csWork,csUnkwn) ;
                      Step.iCycle = 0 ;
                      return true;
                  }

                  if(!IO_GetX(xVisn3_1Ready)&&!OM.CmnOptn.bVs3_1Skip){
                      EM_SetErr(eiHED_Visn3_1NotReady) ;
                      VC.SendReset(vsVisn3_1) ;
                      if(DM.ARAY[riVS3].GetCntExist())DM.ARAY[riVS3].SetStat(csUnkwn) ;
                      if(DM.ARAY[riWR3].GetCntExist())DM.ARAY[riWR3].ChangeStat(csWork,csUnkwn) ;
                      Step.iCycle = 0 ;
                      return true;
                  }
                  if(!IO_GetX(xVisn3_2Ready)&&!OM.CmnOptn.bVs3_2Skip){
                      EM_SetErr(eiHED_Visn3_2NotReady) ;
                      VC.SendReset(vsVisn3_2) ;
                      if(DM.ARAY[riVS3].GetCntExist())DM.ARAY[riVS3].SetStat(csUnkwn) ;
                      if(DM.ARAY[riWR3].GetCntExist())DM.ARAY[riWR3].ChangeStat(csWork,csUnkwn) ;
                      Step.iCycle = 0 ;
                      return true;
                  }
                  SetTrgPos(); //트리거 세팅.
                  MoveMotr(miWRK_XVsn,piWRK_XVsnTrgEnd );
                  Step.iCycle++ ;
                  return false ;

        case  14: if(!MoveMotr(miWRK_XVsn,piWRK_XVsnTrgEnd )) return false ;
                  ResetTrgPos() ; //트리거 세팅 클리어
                  Step.iCycle++ ;
                  return false ;

        case  15: FindChip(r , c ) ;
                  //r = OM.DevInfo.iRowCnt - r/OM.DevInfo.iRowInspCnt * OM.DevInfo.iRowInspCnt -1 ;

                  for(c = 0 ; c < DM.ARAY[riVS1].GetMaxCol() ; c++ ) {
                      for(int i = 0 ; i < OM.DevInfo.iRowInspCnt ; i ++) {
                          if(DM.ARAY[riVS1].CheckAllExist())DM.ARAY[riVS1].SetStat(r-i,c,csWork) ;
                          if(DM.ARAY[riVS2].CheckAllExist())DM.ARAY[riVS2].SetStat(r-i,c,csWork) ;
                          if(DM.ARAY[riVS3].CheckAllExist())DM.ARAY[riVS3].SetStat(r-i,c,csWork) ;
                      }
                  }

                  r1 = DM.ARAY[riVS1].GetCntStat(csUnkwn);
                  r2 = DM.ARAY[riVS2].GetCntStat(csUnkwn);
                  r3 = DM.ARAY[riVS3].GetCntStat(csUnkwn);


                  //Stat.bReqAllInsp = true ;
                  if(r1 || r2 || r3) {
                      Step.iCycle=11 ; //Next Row Insp
                      return false ;

                  }
                  Step.iCycle++ ;
                  return false ;

        case  16: //한스트립을 다 검사했을 경우.
                  MoveMotr(miWRK_YVsn,piWRK_YVsnWorkStart   ) ;
                  MoveMotr(miWRK_XVsn,piWRK_XVsnOffsetWrkStt) ;
                  Step.iCycle++ ;
                  return false ;

        case  17: //if(!MoveMotr(miWRK_YVsn,piWRK_YVsnWorkStart   )) return false ;
                  //if(!MoveMotr(miWRK_XVsn,piWRK_XVsnOffsetWrkStt)) return false ; //굳이 애 확인할 필요가 없을듯.dd
                  RAL.MoveActr(aiWK1_SttnUpDn,ccBwd);
                  RAL.MoveActr(aiWK2_SttnUpDn,ccBwd);
                  RAL.MoveActr(aiWK3_SttnUpDn,ccBwd);
                  //Test 추가해봄.dd
                  Step.iCycle++ ;
                  return false ;

        case  18: if(!OM.CmnOptn.bVs1_1Skip && !IO_GetX(xVisn1_1End)) return false ;
                  if(!OM.CmnOptn.bVs1_2Skip && !IO_GetX(xVisn1_2End)) return false ;

                  if(!OM.CmnOptn.bVs2_1Skip && !IO_GetX(xVisn2_1End)) return false ;
                  if(!OM.CmnOptn.bVs2_2Skip && !IO_GetX(xVisn2_2End)) return false ;

                  if(!OM.CmnOptn.bVs3_1Skip && !IO_GetX(xVisn3_1End)) return false ;
                  if(!OM.CmnOptn.bVs3_2Skip && !IO_GetX(xVisn3_2End)) return false ;
                  //VC.ClearErrCnt();

                  //씨발 초기화.
                  HED.DpAray1.SetStat(csUnkwn);
                  HED.DpAray2.SetStat(csUnkwn);
                  HED.DpAray3.SetStat(csUnkwn);

                  ////////////////////////////////////////////////////////////////////////////
                  if(OM.CmnOptn.bCam1Skip && OM.CmnOptn.bCam2Skip){ //쓸일 없지만...
                      DM.ARAY[riWR1].ChangeStat(csUnkwn,csWork) ;
                      DpAray1.SetStat(csWork);
                      DM.ARAY[riWR2].ChangeStat(csUnkwn,csWork) ;
                      DpAray2.SetStat(csWork);
                      DM.ARAY[riWR3].ChangeStat(csUnkwn,csWork) ;
                      DpAray3.SetStat(csWork);
                  }
                  else if(OM.CmnOptn.bCam2Skip){ //비젼우측고장일때,
                      if(DM.ARAY[riWR1].CheckAllExist()){
                          if(OM.CmnOptn.bVs1_1Skip) {
                              DM.ARAY[riWR1].ChangeStat(csUnkwn,csWork) ;
                              DpAray1.SetStat(csWork);
                          }
                          else {
                              VC.SimpleReadResult(vsVisn1_1,&HED.DpAray1);
                              if(!VC.ReadResult(vsVisn1_1,riWR1)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn1_1Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
                          }

                          if(OM.CmnOptn.bVs2_1Skip) {
                              DM.ARAY[riWR2].ChangeStat(csUnkwn,csWork) ;
                              DpAray2.SetStat(csWork);
                          }
                          else {
                              VC.SimpleReadResult(vsVisn2_1,&HED.DpAray2);
                              if(!VC.ReadResult(vsVisn2_1,riWR2)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn2_1Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
                          }

                          if(OM.CmnOptn.bVs3_1Skip) {
                              DM.ARAY[riWR3].ChangeStat(csUnkwn,csWork) ;
                              DpAray3.SetStat(csWork);
                          }
                          else {
                              VC.SimpleReadResult(vsVisn3_1,&HED.DpAray3);
                              if(!VC.ReadResult(vsVisn3_1,riWR3)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn3_1Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
                          }
                      }
                  }

                  else if(OM.CmnOptn.bCam1Skip){ //비젼좌측고장일때,
                      if(DM.ARAY[riWR1].CheckAllExist()){
                          if(OM.CmnOptn.bVs1_2Skip) {
                              DM.ARAY[riWR1].ChangeStat(csUnkwn,csWork) ;
                              DpAray1.SetStat(csWork);
                          }
                          else {
                              VC.SimpleReadResult(vsVisn1_2,&HED.DpAray1);
                              if(!VC.ReadResult(vsVisn1_2,riWR1)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn1_2Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
                          }

                          if(OM.CmnOptn.bVs2_2Skip) {
                              DM.ARAY[riWR2].ChangeStat(csUnkwn,csWork) ;
                              DpAray2.SetStat(csWork);
                          }
                          else {
                              VC.SimpleReadResult(vsVisn2_2,&HED.DpAray2);
                              if(!VC.ReadResult(vsVisn2_2,riWR2)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn2_2Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
                          }

                          if(OM.CmnOptn.bVs3_2Skip) {
                              DM.ARAY[riWR3].ChangeStat(csUnkwn,csWork) ;
                              DpAray3.SetStat(csWork);
                          }
                          else {
                              VC.SimpleReadResult(vsVisn3_2,&HED.DpAray3);
                              if(!VC.ReadResult(vsVisn3_2,riWR3)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn3_2Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
                          }
                      }
                  }




                  else { //정상 상태 일때...
                      if(DM.ARAY[riWR1].CheckAllExist()){
                          if(OM.CmnOptn.bVs1_1Skip) {
                              for(int c = 0 ; c < GetRightStartCol() ; c++) {
                                  for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
                                      if(DM.ARAY[riWR1].GetStat(r,c) == csUnkwn) DM.ARAY[riWR1].SetStat(r,c,csWork);
                                      DpAray1.SetStat(r,c,csWork);
                                  }
                              }
                          }
                          else {
                              VC.SimpleReadResult(vsVisn1_1,&HED.DpAray1);
                              if(!VC.ReadResult(vsVisn1_1,riWR1)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn1_1Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
                          }
                          if(OM.CmnOptn.bVs1_2Skip) {
                              for(int c = GetRightStartCol() ; c < OM.DevInfo.iColCnt ; c++) {
                                  for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
                                      if(DM.ARAY[riWR1].GetStat(r,c) == csUnkwn) DM.ARAY[riWR1].SetStat(r,c,csWork);
                                      DpAray1.SetStat(r,c,csWork);
                                  }
                              }
                          }
                          else {
                              VC.SimpleReadResult(vsVisn1_2,&HED.DpAray1);
                              if(!VC.ReadResult(vsVisn1_2,riWR1)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn1_2Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
                          }
                      }
                      if(DM.ARAY[riWR2].CheckAllExist()){
                          if(OM.CmnOptn.bVs2_1Skip) {
                              for(int c = 0 ; c < GetRightStartCol() ; c++) {
                                  for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
                                      if(DM.ARAY[riWR2].GetStat(r,c) == csUnkwn) DM.ARAY[riWR2].SetStat(r,c,csWork);
                                      DpAray2.SetStat(r,c,csWork);
                                  }
                              }
                          }
                          else {
                              VC.SimpleReadResult(vsVisn2_1,&HED.DpAray2);
                              if(!VC.ReadResult(vsVisn2_1,riWR2)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn2_1Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
                          }
                          if(OM.CmnOptn.bVs2_2Skip) {
                              for(int c = GetRightStartCol() ; c < OM.DevInfo.iColCnt ; c++) {
                                  for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
                                      if(DM.ARAY[riWR2].GetStat(r,c) == csUnkwn) DM.ARAY[riWR2].SetStat(r,c,csWork);
                                      DpAray2.SetStat(r,c,csWork);
                                  }
                              }
                          }
                          else {
                              VC.SimpleReadResult(vsVisn2_2,&HED.DpAray2);
                              if(!VC.ReadResult(vsVisn2_2,riWR2)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn2_2Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
                          }
                      }

                      if(DM.ARAY[riWR3].CheckAllExist()){
                          if(OM.CmnOptn.bVs3_1Skip) {
                              for(int c = 0 ; c < GetRightStartCol() ; c++) {
                                  for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
                                      if(DM.ARAY[riWR3].GetStat(r,c) == csUnkwn) DM.ARAY[riWR3].SetStat(r,c,csWork);
                                      DpAray3.SetStat(r,c,csWork);
                                  }
                              }
                          }
                          else {
                              VC.SimpleReadResult(vsVisn3_1,&HED.DpAray3);
                              if(!VC.ReadResult(vsVisn3_1,riWR3)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn3_1Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
                          }
                          if(OM.CmnOptn.bVs3_2Skip) {
                              for(int c = GetRightStartCol() ; c < OM.DevInfo.iColCnt ; c++) {
                                  for(int r = 0 ; r < OM.DevInfo.iRowCnt ; r++) {
                                      if(DM.ARAY[riWR3].GetStat(r,c) == csUnkwn) DM.ARAY[riWR3].SetStat(r,c,csWork);
                                      DpAray3.SetStat(r,c,csWork);
                                  }
                              }
                          }
                          else {
                              VC.SimpleReadResult(vsVisn3_2,&HED.DpAray3);
                              if(!VC.ReadResult(vsVisn3_2,riWR3)){ //비젼 데이터 읽어 오기.
                                  if(!bManualInsp)EM_SetErr(eiHED_Visn3_2Read);
                                  VisnInspReset();
                                  Step.iCycle=0 ;
                                  return true ;
                              }
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
    if(!MT_GetStop(miWR1_XCvr)) return false ;
    if(!MT_GetStop(miWR2_XCvr)) return false ;
    if(!MT_GetStop(miWR3_XCvr)) return false ;


    //if(!AT_Done(aiHED_UVLight)) return false ;

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
















