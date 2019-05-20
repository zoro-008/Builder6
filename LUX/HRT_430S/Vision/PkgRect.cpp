//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgRect.h"
#include "BaseMaths.h"

#include "UserINI.h"
#include "GlobalMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

REGIST_STATICLLINK_CPP(CRect_V01)
CRect_V01::CRect_V01()
{

}

CRect_V01::~CRect_V01()
{
}

void CRect_V01::OnTrackerChanged()
{
     //화면에서 다시 그리므로 여기서 메세지 날릴 필요 없다.
}


//순수 가상 함수.
//==============================================================================
bool CRect_V01::Init()
{
    Prop.bUseTrain = false ;
    Prop.bCamera   = false ;

    //이 블럭 은 나중에 트렉커 많아지면 같이 증가해야 함..
    CTracker * Tracker = new CTracker();
    Tracker -> Caption = "";
    Tracker -> BandEnabled = true ;
    Tracker -> BandWidth = 100 ;
    Tracker -> SetOnPaint(OnTrackerChanged);
    m_lTrckInsp.PushBack(Tracker);

    m_lLPara   .PushBack(new TLocalPara ());

    return true ;

}
bool CRect_V01::Close()
{
    int iTrackerCnt = m_lTrckInsp.GetDataCnt();
    for(int i = 0 ; i < iTrackerCnt ; i++) {
        delete m_lTrckInsp.GetCrntData(!i);
    }
    m_lTrckInsp.DeleteAll();

    int iLParaCnt = m_lLPara.GetDataCnt();
    for(int i = 0 ; i < iLParaCnt ; i++) {
        delete m_lLPara.GetCrntData(!i);
    }
    m_lLPara.DeleteAll();

    return true ;
}


        //검사 및 러닝 관련.
bool CRect_V01::Train(CImage * _pImg)
{
    m_sErrMsg = "";

    return true ;
}

void CRect_V01::RsltClear() //검사 결과값을 검사전에 클리어 한번 하고 한다.
{
    Rslt.Clear();
}

bool CRect_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}



bool CRect_V01::Run(CImage * _pImg)
{
    if(m_bSkip) {
        Rslt.bRsltOk = true ;
        return true ;
    }
    
    m_sErrMsg = "";
    Rslt.Clear();  //결과값 클리어.


    //벨류 테이블에서 기준값 오프셑 가져옴.
    String sRefOfsXAdd = MPara.sRefOfsXAdd ;
    String sRefOfsYAdd = MPara.sRefOfsYAdd ;
    double dInspOfsX =0, dInspOfsY=0 , dInspOfsT=0;   //기준 티칭값에서 차이.
    double dOriAbsX  =0, dOriAbsY =0 ;                //절대값 x,y

    CValue2 *  Val ;
    Val = m_pValueList -> GetValueByName(MPara.sRefOfsXAdd);
    if(Val)dInspOfsX = Val -> GetDouble();
    Val = m_pValueList -> GetValueByName(MPara.sRefOfsYAdd);
    if(Val)dInspOfsY = Val -> GetDouble();
    Val = m_pValueList -> GetValueByName(MPara.sRefOfsTAdd);
    if(Val)dInspOfsT = Val -> GetDouble();

    Val = m_pValueList -> GetValueByName(MPara.sRefAbsXAdd);
    if(Val)dOriAbsX = Val -> GetDouble();
    Val = m_pValueList -> GetValueByName(MPara.sRefAbsYAdd);
    if(Val)dOriAbsY = Val -> GetDouble();



    CLine::TPara LinePara  ;
    CLine::TRslt LineRsltL ;
    CLine::TRslt LineRsltT ;
    CLine::TRslt LineRsltR ;
    CLine::TRslt LineRsltB ;

    LinePara.bLtToDk           = !CPara.bLtOnDk       ;
    LinePara.iSampleGap        =  MPara.iLnSampleGap  ;
    LinePara.iLineGap          =  MPara.iLnLineGap    ;
    LinePara.iEndOffset        =  MPara.iLnEndOffset  ;
    LinePara.iCntrOffset       =  MPara.iLnCntrOffset ;

    LinePara.EdgPara.iAverage  =  MPara.iEgAverage    ;
    LinePara.EdgPara.iCompare  =  MPara.iEgCompare    ;

    LinePara.EdgPara.iChkPxLow =  MPara.iEgChkPxLow   ;
    LinePara.EdgPara.iChkPxHgh =  MPara.iEgChkPxHgh   ;
    LinePara.EdgPara.iOfsGap   =  MPara.iEgOfsGap     ;
    LinePara.EdgPara.iOfsPxLow =  MPara.iEgOfsPxLow   ;
    LinePara.EdgPara.iOfsPxHgh =  MPara.iEgOfsPxHgh   ;



    TDPoint tFrstStt , tFrstEnd ;
    TDPoint tLastStt , tLastEnd ;

    TDPoint tSttPnt , tEndPnt ;
    double  dAngle ;

    CPopupTracker  * Tracker = ;
    int iTrackerCornerCnt = m_lTrckInsp.GetDataCnt() ;

    TDPoint * aLineSttPnt = new TDPoint[iTrackerCornerCnt];
    TDPoint * aLineEndPnt = new TDPoint[iTrackerCornerCnt];
    for(int j = 0 ; j < iTrackerCornerCnt ; j++){
        //각도 보정.
        pTrackerPnt = Tracker -> GetPoints() ;
        CMath::GetRotPntFromCntrPnt(pTrackerPnt[j].x + dInspOfsX, pTrackerPnt[j].y + dInspOfsY, dInspOfsT ,
                                  dOriAbsX , dOriAbsY ,
                                  &tSttPnt.x , &tSttPnt.y);
        CMath::GetRotPntFromCntrPnt(pTrackerPnt[(j+1)%iTrackerCornerCnt].x + dInspOfsX, pTrackerPnt[(j+1)%iTrackerCornerCnt].y + dInspOfsY, dInspOfsT ,
                                  dOriAbsX , dOriAbsY ,
                                  &tEndPnt.x , &tEndPnt.y);
        dAngle = CMath::GetLineAngle(tSttPnt.x , tSttPnt.y , tEndPnt.x , tEndPnt.y) ;


        CMath::GetPntFromPntByAngLen(  tSttPnt.x , tSttPnt.y   ,   //입력 시작점.
                                     dAngle + 90             ,   //입력 각도.
                                     Tracker -> BandWidth    ,   //입력 길이.
                                    &tFrstStt.x , &tFrstStt.y ); //출력 포인트.
        CMath::GetPntFromPntByAngLen(  tSttPnt.x , tSttPnt.y   ,   //입력 시작점.
                                     dAngle - 90             ,   //입력 각도.
                                     Tracker -> BandWidth    ,   //입력 길이.
                                    &tFrstEnd.x , &tFrstEnd.y ); //출력 포인트.
        CMath::GetPntFromPntByAngLen(  tEndPnt.x , tEndPnt.y   ,   //입력 시작점.
                                     dAngle + 90             ,   //입력 각도.
                                     Tracker -> BandWidth    ,   //입력 길이.
                                    &tLastStt.x , &tLastStt.y ); //출력 포인트.
        CMath::GetPntFromPntByAngLen(  tEndPnt.x , tEndPnt.y   ,   //입력 시작점.
                                     dAngle - 90             ,   //입력 각도.
                                     Tracker -> BandWidth    ,   //입력 길이.
                                    &tLastEnd.x , &tLastEnd.y ); //출력 포인트.

        //한번을 검사함.
        if(!CLine::Inspect(_pImg  , tFrstStt , tFrstEnd , tLastStt , tLastEnd , LinePara , &LineRslt)) {
            m_sErrMsg = LineRslt.sErrMsg;
            delete [] aLineSttPnt ;
            delete [] aLineEndPnt ;

            return false ;
        }
        for(int i = 0 ; i < LineRslt.EdgList.GetDataCnt() ;i++){
            Rslt.lEdgeList.PushBack(LineRslt.EdgList.GetCrntData(!i));
        }
    }


    //Rect.
    /*
    CTracker * Tracker = m_lTrckInsp.GetData(0) ;
    TDRect tDLRect = Tracker -> GetBandDRect(CTracker::piRectLB , CTracker::piRectLT) ;
    TRect  tLRect ;
    tLRect.left   = tDLRect.left   + dInspOfsX ;
    tLRect.top    = tDLRect.top    + dInspOfsY ;
    tLRect.right  = tDLRect.right  + dInspOfsX ;
    tLRect.bottom = tDLRect.bottom + dInspOfsY ;

    TDRect tDTRect = Tracker -> GetBandDRect(CTracker::piRectLT , CTracker::piRectRT) ;
    TRect tTRect ;
    tTRect.left   = tDTRect.left   + dInspOfsX ;
    tTRect.top    = tDTRect.top    + dInspOfsY ;
    tTRect.right  = tDTRect.right  + dInspOfsX ;
    tTRect.bottom = tDTRect.bottom + dInspOfsY ;

    TDRect tDRRect = Tracker -> GetBandDRect(CTracker::piRectRT , CTracker::piRectRB) ;
    TRect tRRect ;
    tRRect.left   = tDRRect.left   + dInspOfsX ;
    tRRect.top    = tDRRect.top    + dInspOfsY ;
    tRRect.right  = tDRRect.right  + dInspOfsX ;
    tRRect.bottom = tDRRect.bottom + dInspOfsY ;

    TDRect tDBRect = Tracker -> GetBandDRect(CTracker::piRectRB , CTracker::piRectLB) ;
    TRect tBRect ;
    tBRect.left   = tDBRect.left  + dInspOfsX ;
    tBRect.top    = tDBRect.top   + dInspOfsY ;
    tBRect.right  = tDBRect.right + dInspOfsX ;
    tBRect.bottom = tDBRect.bottom+ dInspOfsY ;

    _pImg -> SetRect(&tLRect) ;
    _pImg -> SetRect(&tTRect) ;
    _pImg -> SetRect(&tRRect) ;
    _pImg -> SetRect(&tBRect) ;
    */

    //Para.
    //CRect::TPara RectPara ;
    //RectPara.bLtOnDk           = CPara.bLtOnDk       ;
    //
    //RectPara.iSampleGap        = MPara.iLnSampleGap  ;
    //RectPara.iLineGap          = MPara.iLnLineGap    ;
    //RectPara.iEndOffset        = MPara.iLnEndOffset  ;
    //RectPara.iCntrOffset       = MPara.iLnCntrOffset ;
    //
    //RectPara.EdgPara.iAverage  = MPara.iEgAverage    ;
    //RectPara.EdgPara.iCompare  = MPara.iEgCompare    ;
    //
    //RectPara.EdgPara.iChkPxLow = MPara.iEgChkPxLow   ;
    //RectPara.EdgPara.iChkPxHgh = MPara.iEgChkPxHgh   ;
    //RectPara.EdgPara.iOfsGap   = MPara.iEgOfsGap     ;
    //RectPara.EdgPara.iOfsPxLow = MPara.iEgOfsPxLow   ;
    //RectPara.EdgPara.iOfsPxHgh = MPara.iEgOfsPxHgh   ;
    //
    //if(!CRect::Inspect(_pImg , tLRect , tTRect , tRRect , tBRect , RectPara , &Rslt.RectRslt)){
    //    m_sErrMsg = Rslt.RectRslt.sErrMsg ;
    //    return false ;
    //}





    //TDPoint tFrstStt , tFrstEnd ;
    //TDPoint tLastStt , tLastEnd ;
    //
    ////Left
    //tFrstStt
    //
    /*
    LinePara.iScanDirect =  CLine::lsLtToRt ;
    if(!CLine::Inspect(_pImg  , tLRect , LinePara , &LineRsltL , m_fpGetCalPos)) {m_sErrMsg = LineRsltL.sErrMsg;  return false ;}
    for(int i = 0 ; i < LineRsltL.EdgList.GetDataCnt() ;i++){
        Rslt.lEdgeList.PushBack(LineRsltL.EdgList.GetCrntData(!i));
    }

    //Top
    LinePara.iScanDirect = CLine::lsUpToDn ;
    if(!CLine::Inspect(_pImg  , tTRect , LinePara , &LineRsltT, m_fpGetCalPos)) {m_sErrMsg = LineRsltT.sErrMsg;  return false ;}
    for(int i = 0 ; i < LineRsltT.EdgList.GetDataCnt() ;i++){
        Rslt.lEdgeList.PushBack(LineRsltT.EdgList.GetCrntData(!i));
    }

    //Right
    LinePara.iScanDirect = CLine::lsRtToLt ;
    if(!CLine::Inspect(_pImg  , tRRect , LinePara , &LineRsltR, m_fpGetCalPos)) {m_sErrMsg = LineRsltR.sErrMsg;  return false ;}
    for(int i = 0 ; i < LineRsltR.EdgList.GetDataCnt() ;i++){
        Rslt.lEdgeList.PushBack(LineRsltR.EdgList.GetCrntData(!i));
    }

    //Bottom
    LinePara.iScanDirect = CLine::lsDnToUp ;
    if(!CLine::Inspect(_pImg  , tBRect , LinePara , &LineRsltB, m_fpGetCalPos)) {m_sErrMsg = LineRsltB.sErrMsg;  return false ;}
    for(int i = 0 ; i < LineRsltB.EdgList.GetDataCnt() ;i++){
        Rslt.lEdgeList.PushBack(LineRsltB.EdgList.GetCrntData(!i));
    }     */

    if(!CMath::GetLineCrossPnt(LineRsltL.StartPnt , LineRsltL.EndPnt , LineRsltT.StartPnt , LineRsltT.EndPnt , Rslt.CrossPntLT)){m_sErrMsg = "LT Lines are parallel"; return false ; }
    if(!CMath::GetLineCrossPnt(LineRsltT.StartPnt , LineRsltT.EndPnt , LineRsltR.StartPnt , LineRsltR.EndPnt , Rslt.CrossPntRT)){m_sErrMsg = "RT Lines are parallel"; return false ; }
    if(!CMath::GetLineCrossPnt(LineRsltR.StartPnt , LineRsltR.EndPnt , LineRsltB.StartPnt , LineRsltB.EndPnt , Rslt.CrossPntRB)){m_sErrMsg = "RB Lines are parallel"; return false ; }
    if(!CMath::GetLineCrossPnt(LineRsltB.StartPnt , LineRsltB.EndPnt , LineRsltL.StartPnt , LineRsltL.EndPnt , Rslt.CrossPntLB)){m_sErrMsg = "LB Lines are parallel"; return false ; }

    Rslt.CenterPnt.x = (Rslt.CrossPntLT.x + Rslt.CrossPntRT.x + Rslt.CrossPntRB.x + Rslt.CrossPntLB.x) / 4.0 ;
    Rslt.CenterPnt.y = (Rslt.CrossPntLT.y + Rslt.CrossPntRT.y + Rslt.CrossPntRB.y + Rslt.CrossPntLB.y) / 4.0 ;


    //기준이 0도 이다.
    double dAngleSum = 0.0;
    Rslt.dAngleL = CMath::GetLineAngle(LineRsltL.StartPnt.x , LineRsltL.StartPnt.y , LineRsltL.EndPnt.x   , LineRsltL.EndPnt.y  ) - 270 ; dAngleSum += Rslt.dAngleL ;
    Rslt.dAngleT = CMath::GetLineAngle(LineRsltT.EndPnt.x   , LineRsltT.EndPnt.y   , LineRsltT.StartPnt.x , LineRsltT.StartPnt.y) - 180 ; dAngleSum += Rslt.dAngleT ;
    Rslt.dAngleR = CMath::GetLineAngle(LineRsltR.StartPnt.x , LineRsltR.StartPnt.y , LineRsltR.EndPnt.x   , LineRsltR.EndPnt.y  ) - 270 ; dAngleSum += Rslt.dAngleR ;
    Rslt.dAngleB = CMath::GetLineAngle(LineRsltB.EndPnt.x   , LineRsltB.EndPnt.y   , LineRsltB.StartPnt.x , LineRsltB.StartPnt.y) - 180 ; dAngleSum += Rslt.dAngleB ;
    Rslt.dAngle = dAngleSum / 4.0 ;

    Rslt.dLengthL = CMath::GetLineLenth(Rslt.CrossPntLB.x , Rslt.CrossPntLB.y , Rslt.CrossPntLT.x , Rslt.CrossPntLT.y); //왼쪽변.
    Rslt.dLengthT = CMath::GetLineLenth(Rslt.CrossPntLT.x , Rslt.CrossPntLT.y , Rslt.CrossPntRT.x , Rslt.CrossPntRT.y); //위쪽 변
    Rslt.dLengthR = CMath::GetLineLenth(Rslt.CrossPntRT.x , Rslt.CrossPntRT.y , Rslt.CrossPntRB.x , Rslt.CrossPntRB.y); //오른쪽변
    Rslt.dLengthB = CMath::GetLineLenth(Rslt.CrossPntRB.x , Rslt.CrossPntRB.y , Rslt.CrossPntLB.x , Rslt.CrossPntLB.y); //아래변
    //=================스펙 판정.===============================================
    //미리 OK로 만들고 스펙 벋어나면 NG로 바꿈.
    Rslt.bRsltOk  = true ;

    //기럭지들 확인.
    if(m_fpGetPxResol){
        Rslt.dLengthL *= m_fpGetPxResol() ;
        Rslt.dLengthT *= m_fpGetPxResol() ;
        Rslt.dLengthR *= m_fpGetPxResol() ;
        Rslt.dLengthB *= m_fpGetPxResol() ;
    }
    if(CPara.dTolerance!=0.0) {
             if(CPara.dHeight + CPara.dTolerance < Rslt.dLengthL || Rslt.dLengthL < CPara.dHeight - CPara.dTolerance) {m_sErrMsg.sprintf("L = %.3f" , Rslt.dLengthL);Rslt.bRsltOk = false ;}
        else if(CPara.dWidth  + CPara.dTolerance < Rslt.dLengthT || Rslt.dLengthT < CPara.dWidth  - CPara.dTolerance) {m_sErrMsg.sprintf("T = %.3f" , Rslt.dLengthT);Rslt.bRsltOk = false ;}
        else if(CPara.dHeight + CPara.dTolerance < Rslt.dLengthR || Rslt.dLengthR < CPara.dHeight - CPara.dTolerance) {m_sErrMsg.sprintf("R = %.3f" , Rslt.dLengthR);Rslt.bRsltOk = false ;}
        else if(CPara.dWidth  + CPara.dTolerance < Rslt.dLengthB || Rslt.dLengthB < CPara.dWidth  - CPara.dTolerance) {m_sErrMsg.sprintf("B = %.3f" , Rslt.dLengthB);Rslt.bRsltOk = false ;}
    }

    Rslt.dOffsetX = Rslt.CenterPnt.x - _pImg->GetWidth ()/2.0;
    Rslt.dOffsetY = Rslt.CenterPnt.y - _pImg->GetHeight()/2.0;
    if(m_fpGetPxResol){
        Rslt.dOffsetX *= m_fpGetPxResol() ;
        Rslt.dOffsetY *= m_fpGetPxResol() ;
    }

    Rslt.dOffsetX = ((int)(Rslt.dOffsetX * 100000)) / 100000.0 ; //소수5자리만 표현.
    Rslt.dOffsetY = ((int)(Rslt.dOffsetY * 100000)) / 100000.0 ; //소수5자리만 표현.

    if(MPara.bOutOfsXInverse) Rslt.dOffsetX *= -1 ;
    if(MPara.bOutOfsYInverse) Rslt.dOffsetY *= -1 ;



    //포지션 확인.
    if(CPara.dPosMax!=0.0){
        if(CPara.dPosMax < fabs(Rslt.dOffsetX)){m_sErrMsg.sprintf("X = %.5f" , Rslt.dOffsetX);Rslt.bRsltOk = false ;}
        if(CPara.dPosMax < fabs(Rslt.dOffsetY)){m_sErrMsg.sprintf("Y = %.5f" , Rslt.dOffsetY);Rslt.bRsltOk = false ;}
    }

    //각도 확인.
    Rslt.dAngle = ((int)(Rslt.dAngle * 100000)) / 100000.0 ; //소수5자리만 표현.
    if(MPara.bOutAngleInverse) Rslt.dAngle *= -1 ;
    if(CPara.dAngleMax != 0.0){  //여기부터.
        if(CPara.dAngleMax < fabs(Rslt.dAngle)){m_sErrMsg.sprintf("T = %.5f" , Rslt.dAngle);Rslt.bRsltOk = false ;}
    }

    Val = m_pValueList -> GetValueByName(MPara.sOutOfsXAdd);
    if(Val) Val -> Set(Rslt.dOffsetX);
    Val = m_pValueList -> GetValueByName(MPara.sOutOfsYAdd);
    if(Val) Val -> Set(Rslt.dOffsetY);
    Val = m_pValueList -> GetValueByName(MPara.sOutAngleAdd);
    if(Val) Val -> Set(Rslt.dAngle);


    Rslt.bInspEnd = true ;

    return true ;

}

String CRect_V01::GetErrMsg()
{
    return m_sErrMsg ;
}


        //화면 출력.
void CRect_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if(!Rslt.bInspEnd) return ;
    if(m_bSkip) return ;

    CGdi *Gdi = new CGdi(_hDc,_dScaleX , _dScaleY , _dOffsetX , _dOffsetY);


    AnsiString sTemp ;

    Gdi->m_tPen.Color   = Rslt.bRsltOk ? clLime : clRed ;
    Gdi->m_tText.Color  = Rslt.bRsltOk ? clLime : clRed ;
    Gdi->m_tPen.Style   = PS_SOLID ;
    Gdi->Line((int)Rslt.CrossPntLT.x , (int)Rslt.CrossPntLT.y ,(int)Rslt.CrossPntRT.x , (int)Rslt.CrossPntRT.y);
    Gdi->Line((int)Rslt.CrossPntRT.x , (int)Rslt.CrossPntRT.y ,(int)Rslt.CrossPntRB.x , (int)Rslt.CrossPntRB.y);
    Gdi->Line((int)Rslt.CrossPntRB.x , (int)Rslt.CrossPntRB.y ,(int)Rslt.CrossPntLB.x , (int)Rslt.CrossPntLB.y);
    Gdi->Line((int)Rslt.CrossPntLB.x , (int)Rslt.CrossPntLB.y ,(int)Rslt.CrossPntLT.x , (int)Rslt.CrossPntLT.y);


    int iX ;
    int iY ;

    iX = (Rslt.CrossPntLT.x + Rslt.CrossPntLB.x) / 2.0 ;
    iY = (Rslt.CrossPntLT.y + Rslt.CrossPntLB.y) / 2.0 ;
    sTemp = sTemp.sprintf("L:%.5f",Rslt.dLengthL) ;
    Gdi->Text(iX - 15 ,iY , sTemp.c_str()) ;

    iX = (Rslt.CrossPntLT.x + Rslt.CrossPntRT.x) / 2.0 ;
    iY = (Rslt.CrossPntLT.y + Rslt.CrossPntRT.y) / 2.0 ;
    sTemp = sTemp.sprintf("T:%.5f",Rslt.dLengthT) ;
    Gdi->Text(iX  ,iY - 15, sTemp.c_str()) ;

    iX = (Rslt.CrossPntRT.x + Rslt.CrossPntRB.x) / 2.0 ;
    iY = (Rslt.CrossPntRT.y + Rslt.CrossPntRB.y) / 2.0 ;
    sTemp = sTemp.sprintf("R:%.5f",Rslt.dLengthR) ;
    Gdi->Text(iX + 3 ,iY , sTemp.c_str()) ;

    iX = (Rslt.CrossPntLB.x + Rslt.CrossPntRB.x) / 2.0 ;
    iY = (Rslt.CrossPntLB.y + Rslt.CrossPntRB.y) / 2.0 ;
    sTemp = sTemp.sprintf("B:%.5f",Rslt.dLengthB) ;
    Gdi->Text(iX  ,iY + 3, sTemp.c_str()) ;







    Gdi->LineX((int)Rslt.CenterPnt .x ,(int)Rslt.CenterPnt.y  , 5) ;

    Gdi->LineX((int)Rslt.CrossPntLT.x ,(int)Rslt.CrossPntLT.y , 5) ;
    Gdi->LineX((int)Rslt.CrossPntRT.x ,(int)Rslt.CrossPntRT.y , 5) ;
    Gdi->LineX((int)Rslt.CrossPntRB.x ,(int)Rslt.CrossPntRB.y , 5) ;
    Gdi->LineX((int)Rslt.CrossPntLB.x ,(int)Rslt.CrossPntLB.y , 5) ;


    TDPoint tEdgePnt ;
    for(int i = 0 ; i < Rslt.lEdgeList.GetDataCnt() ; i++) {
        tEdgePnt = Rslt.lEdgeList.GetCrntData(!i);
        Gdi -> Pixel((int)(tEdgePnt.x+0.5),(int)(tEdgePnt.y+0.5),clYellow);
    }




    sTemp = sTemp.sprintf("X:%.5f Y:%.5f",Rslt.CenterPnt.x , Rslt.CenterPnt.y) ;
    Gdi->Text((int)Rslt.CenterPnt.x + 5 ,(int)Rslt.CenterPnt.y + 5 , sTemp.c_str()) ;
    sTemp = sTemp.sprintf("Angle:%.5f",Rslt.dAngle) ;
    Gdi->Text((int)Rslt.CenterPnt.x + 5 ,(int)Rslt.CenterPnt.y + 20 , sTemp.c_str()) ;

    sTemp = sTemp.sprintf("Offset X:%.5f Y:%.5f",Rslt.dOffsetX  , Rslt.dOffsetY ) ;
    Gdi->Text((int)Rslt.CenterPnt.x + 5 ,(int)Rslt.CenterPnt.y + 35 , sTemp.c_str()) ;

    delete Gdi;
    

}




void CRect_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CGdi *Gdi = new CGdi(_hDc);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> Paint(_hDc ,  _dScaleX ,  _dScaleY ,  _dOffsetX ,  _dOffsetY);

    }

    delete Gdi;

}



        //이미지 화면 클릭 이벤트
bool CRect_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CTracker * Tracker ;
    bool bRet = false ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerDw(Button,Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY))bRet = true;
    }
    return bRet;
}


bool CRect_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CTracker * Tracker ;
    bool bRet = false ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerMv(Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY)) bRet = true ;
    }
    return bRet ;
}

void CRect_V01::MouseUp(TMouseButton Button,TShiftState Shift )
{
    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> TrackerUp(Button, Shift);
    }
}

        //파라미터 디스플레이
void CRect_V01::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        UpdateListPara(_bToTable , _vePara , &MPara.List);
    }
    else {
        UpdateListPara(_bToTable , _vePara , &MPara.List);
        UpdateMasterPara(true , _vePara );
    }
}

void CRect_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        UpdateListPara(_bToTable , _vePara , &CPara.List);
    }
    else {
        UpdateListPara(_bToTable , _vePara , &CPara.List);

        UpdateCommonPara(true , _vePara );
    }

}
void CRect_V01::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;

    }
    else {

        UpdateLocalPara(true , _vePara );
    }

}

        //로드 세이브.
void CRect_V01::LoadSave(bool _bLoad , String _sVisnName)
{
    String sIniPath = GM.GetJobFileFolder() + _sVisnName + "_" + m_sName + "_" + GetStaticPkgLink()->GetPkgName() + ".ini" ;

    TUserINI Ini ;

    /*
    Tracker -> SetFunc(OnTrackerChanged);
    Tracker -> HeightMax = 100 ;
    Tracker -> WidthMax  = 100 ;
    m_lTrckInsp.PushBack(Tracker);
    m_lLPara   .PushBack(new TLocalPara ());
    */


    int iTrackerCnt ;
    CTracker* Tracker ;
    if(_bLoad) {
        //이파트는 무조건 1개만 되기 때문에 필요 없다.
        //m_lTrckInsp.DeleteAll();
        if(FileExists(sIniPath)){
            //Tracker ;
            Tracker = m_lTrckInsp.GetData(0);
            Tracker -> LoadSave(_bLoad , sIniPath , "Tracker0");

            LoadSaveList(_bLoad , sIniPath , "MasterPara" , &MPara.List);
            LoadSaveList(_bLoad , sIniPath , "CommonPara" , &CPara.List);
        }
        else {
            MPara.SetDefault();
            CPara.SetDefault();
        }

    }
    else {
        iTrackerCnt = m_lTrckInsp.GetDataCnt() ;

        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath       ) ;

        //Tracker ;
        Tracker = m_lTrckInsp.GetData(0);
        Tracker -> LoadSave(_bLoad , sIniPath , "Tracker0");

        LoadSaveList(_bLoad , sIniPath , "MasterPara" , &MPara.List);
        LoadSaveList(_bLoad , sIniPath , "CommonPara" , &CPara.List);


    }
}




