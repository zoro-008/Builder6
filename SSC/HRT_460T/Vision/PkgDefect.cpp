//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgDefect.h"
#include "BaseMaths.h"
#include "UnitArea.h"

#include "aCircle.h"
#include "aLine.h"

#include "UserINI.h"
#include "UnitGdi.h"
#include "GlobalMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

REGIST_STATICLLINK_CPP(CDefect_V01)
CDefect_V01::CDefect_V01()
{

}

CDefect_V01::~CDefect_V01()
{
}

void CDefect_V01::OnTrackerMenuItemClick(EMenuItem _iItemNo, CPopupTracker* _pTracker)
{
    int iTrackerInspCnt = m_lTrckInsp.GetDataCnt() ;
    int iTrackerIgnrCnt = m_lTrckIgnr.GetDataCnt() ;
    CPopupTracker* Tracker ;
    TLocalPara     * pNewLPara     , pLPara     ;
    TLocalParaPoly * pNewLParaPoly , pLParaPoly ;

    if(_iItemNo == miNewInsp) {
        Tracker = new CPopupTracker();
        Tracker -> Tag = iTrackerInspCnt ;
        Tracker -> Caption = "Insp"+String(iTrackerInspCnt);
        Tracker -> SetOnPaint(OnTrackerChanged);
        Tracker -> SetOnMenuItemClick(OnTrackerMenuItemClick) ;
        m_lTrckInsp.PushBack(Tracker);

        pNewLPara = new TLocalPara();
        m_lLPara    .PushBack(pNewLPara    );

        pNewLParaPoly = new TLocalParaPoly();
        m_lLParaPoly.PushBack(pNewLParaPoly);

    }
    else if(_iItemNo == miNewIgnr) {
        Tracker = new CPopupTracker();
        Tracker -> LineColor = clSilver ;
        Tracker -> Tag = iTrackerIgnrCnt ;
        Tracker -> Caption = "Ignr"+String(iTrackerIgnrCnt);
        Tracker -> SetOnPaint(OnTrackerChanged);
        Tracker -> SetOnMenuItemClick(OnTrackerMenuItemClick) ;
        m_lTrckIgnr.PushBack(Tracker);
    }
    else if(_iItemNo == miDelete) {
        if(_pTracker-> Caption.Pos("Insp")){
            m_lTrckInsp .Delete(_pTracker->Tag);
            m_lLPara    .Delete(_pTracker->Tag);
            m_lLParaPoly.Delete(_pTracker->Tag);
            int iTrackerCnt = m_lTrckInsp.GetDataCnt();
            for(int i = 0 ; i < iTrackerCnt ; i++) {
                Tracker =  m_lTrckInsp.GetCrntData(!i);
                Tracker -> Tag = i ;
                Tracker -> Caption = "Insp"+String(i);
            }
        }
        else {
            m_lTrckIgnr .Delete(_pTracker->Tag);
            int iTrackerCnt = m_lTrckIgnr.GetDataCnt();
            for(int i = 0 ; i < iTrackerCnt ; i++) {
                Tracker =  m_lTrckInsp.GetCrntData(!i);
                Tracker -> Tag = i ;
                Tracker -> Caption = "Insp"+String(i);
            }
        }
    }
    else if(_iItemNo == miCopyAndPaste) {
        if(_pTracker-> Caption.Pos("Insp")){
            Tracker = new CPopupTracker();
            //*Tracker = *_pTracker ;
            Tracker -> Copy(_pTracker);
            Tracker -> Tag     = iTrackerInspCnt ;
            TDRect Rect = Tracker -> GetDRect() ;
            Rect.left   += 10 ;
            Rect.top    += 10 ;
            Rect.right  += 10 ;
            Rect.bottom += 10 ;
            Tracker -> SetDRect(Rect);

            Tracker -> Caption = "Insp"+String(iTrackerInspCnt);
            Tracker -> SetOnPaint(OnTrackerChanged);
            Tracker -> SetOnMenuItemClick(OnTrackerMenuItemClick) ;
            m_lTrckInsp.PushBack(Tracker);

            pNewLPara = new TLocalPara();
            *pNewLPara = *m_lLPara.GetData(_pTracker->Tag);
            m_lLPara    .PushBack(pNewLPara    );

            pNewLParaPoly = new TLocalParaPoly();
            *pNewLParaPoly = *m_lLParaPoly.GetData(_pTracker->Tag);
            m_lLParaPoly.PushBack(pNewLParaPoly);
        }
        else {
            Tracker = new CPopupTracker();
            //*Tracker = *_pTracker ;
            Tracker -> Copy(_pTracker);
            Tracker -> Tag     = iTrackerIgnrCnt ;
            TDRect Rect = Tracker -> GetDRect() ;
            Rect.left   += 10 ;
            Rect.top    += 10 ;
            Rect.right  += 10 ;
            Rect.bottom += 10 ;
            Tracker -> SetDRect(Rect);

            Tracker -> Caption = "Ignr"+String(iTrackerIgnrCnt);
            Tracker -> SetOnPaint(OnTrackerChanged);
            Tracker -> SetOnMenuItemClick(OnTrackerMenuItemClick) ;
            m_lTrckIgnr.PushBack(Tracker);
        }
    }
    if(m_fpOnPaint)m_fpOnPaint();
}

void CDefect_V01::OnTrackerChanged()
{
     //화면에서 다시 그리므로 여기서 메세지 날릴 필요 없다.
}


//순수 가상 함수.
//==============================================================================
bool CDefect_V01::Init()
{
    Prop.bUseTrain = false ;
    Prop.bCamera   = false ;

    //이 블럭 은 나중에 트렉커 많아지면 같이 증가해야 함..
    CPopupTracker * Tracker = new CPopupTracker();
    Tracker -> TrackerType  = CTracker::ttRect ;
    Tracker -> Tag          = 0 ;
    Tracker -> Caption      = "Insp0";
    Tracker -> BandEnabled  = false ;
    Tracker -> BandWidth    = 50 ;
    Tracker -> SetOnPaint(OnTrackerChanged);
    Tracker -> SetOnMenuItemClick(OnTrackerMenuItemClick) ;
    m_lTrckInsp.PushBack(Tracker);

    m_lLPara    .PushBack(new TLocalPara    ());
    m_lLParaPoly.PushBack(new TLocalParaPoly());

    
    m_pActiveLPara     = NULL ;
    m_pActiveLParaPoly = NULL ;

    return true ;

}
bool CDefect_V01::Close()
{
    m_pActiveLPara     = NULL ;
    m_pActiveLParaPoly = NULL ;

    int iTrackerCnt = m_lTrckInsp.GetDataCnt();
    for(int i = 0 ; i < iTrackerCnt ; i++) {
        delete m_lTrckInsp.GetCrntData(!i);
    }
    m_lTrckInsp.DeleteAll();

    iTrackerCnt = m_lTrckIgnr.GetDataCnt();
    for(int i = 0 ; i < iTrackerCnt ; i++) {
        delete m_lTrckIgnr.GetCrntData(!i);
    }
    m_lTrckIgnr.DeleteAll();

    int iLParaCnt = m_lLPara.GetDataCnt();
    for(int i = 0 ; i < iLParaCnt ; i++) {
        delete m_lLPara.GetCrntData(!i);
    }
    m_lLPara.DeleteAll();

    int iLParaPolyCnt = m_lLParaPoly.GetDataCnt();
    for(int i = 0 ; i < iLParaPolyCnt ; i++) {
        delete m_lLParaPoly.GetCrntData(!i);
    }
    m_lLParaPoly.DeleteAll();

    return true ;
}


        //검사 및 러닝 관련.
bool CDefect_V01::Train(CImage * _pImg)
{
    m_sErrMsg = "";

    return true ;
}

void CDefect_V01::RsltClear() //검사 결과값을 검사전에 클리어 한번 하고 한다.
{
    Rslt.Clear();
}

bool CDefect_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}



bool CDefect_V01::Run(CImage * _pImg)
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
    String sRefOfsTAdd = MPara.sRefOfsTAdd ;

    String sRefAbsXAdd = MPara.sRefAbsXAdd ;
    String sRefAbsYAdd = MPara.sRefAbsYAdd ;

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

    //결과값 검사영역 사이즈 맞추고.
    Rslt.InspArea.SetSize(_pImg->GetWidth() , _pImg->GetHeight());


    CPopupTracker  * Tracker ;
    TLocalPara     * LPara ;
    TLocalParaPoly * LParaPoly ;
    TDPoint        * pTrackerPnt ;
    const int        iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    Rslt.iTrackerRsltCnt = iTrackerCnt ;
    Rslt.pTrackerRslts   = new TTrackerRslt[iTrackerCnt]; //Delete는 Rslt.Clear에서 수행한다.

    TDRect InspAreaRect ; //여러검사영역을 감싸는 최소사각형.
    InspAreaRect.left   = _pImg -> GetWidth ()/2.0 ;
    InspAreaRect.top    = _pImg -> GetHeight()/2.0 ;
    InspAreaRect.right  = _pImg -> GetWidth ()/2.0 ;
    InspAreaRect.bottom = _pImg -> GetHeight()/2.0 ;

    for(int i = 0 ; i < iTrackerCnt ; i++) {
        Tracker   = m_lTrckInsp.GetCrntData(!i) ;
        LPara     = m_lLPara     .GetCrntData(!i) ;
        LParaPoly = m_lLParaPoly .GetCrntData(!i) ;

        if(Tracker -> TrackerType == CTracker::ttCircle) {

            Rslt.pTrackerRslts[i].eTrackerType = CTracker::ttCircle ;
            Rslt.pTrackerRslts[i].iPointCnt = 1 ; //원은 중심점 좌표만 가지고 있는다.
            Rslt.pTrackerRslts[i].pPoints   = new TDPoint[1] ;
            pTrackerPnt = Tracker -> GetPoints() ;
            CMath::GetRotPntFromCntrPnt(pTrackerPnt[0].x + dInspOfsX, pTrackerPnt[0].y + dInspOfsY, dInspOfsT ,
                                      dOriAbsX , dOriAbsY ,
                                      &Rslt.pTrackerRslts[i].pPoints[0].x , &Rslt.pTrackerRslts[i].pPoints[0].y);

            Rslt.pTrackerRslts[i].iRadius   = Tracker -> CircleRadian + LPara->iInspOffset ;

            if(MPara.bUsePreEdge) {//원일때...나중에 하자.... 원형으로 엣지 따는 것을 만들어야 하는데...
                TCircle tInspCircle ;
                tInspCircle.cx  = Rslt.pTrackerRslts[i].pPoints[0].x ;
                tInspCircle.cy  = Rslt.pTrackerRslts[i].pPoints[0].y ;
                tInspCircle.rad = Tracker -> CircleRadian ;

                CCircle::TPara CirclePara ;
                CirclePara.iGap       = MParaShape.iLnLineGap   ;
                CirclePara.iSearchGap = Tracker -> BandWidth   ;
                CirclePara.iSampleCnt = MParaShape.iCcSampleCnt ;
                CirclePara.iStartAng  = 0 ;
                CirclePara.iEndAng    = 0 ;

                CirclePara.EdgPara.iAverage  = MParaShape.iEgAverage    ;
                CirclePara.EdgPara.iCompare  = MParaShape.iEgCompare    ;
                CirclePara.EdgPara.iChkPxLow = MParaShape.iEgChkPxLow   ;
                CirclePara.EdgPara.iChkPxHgh = MParaShape.iEgChkPxHgh   ;
                CirclePara.EdgPara.iOfsGap   = MParaShape.iEgOfsGap     ;
                CirclePara.EdgPara.iOfsPxLow = MParaShape.iEgOfsPxLow   ;
                CirclePara.EdgPara.iOfsPxHgh = MParaShape.iEgOfsPxHgh   ;

                CCircle::TRslt CircleRslt ;

                if(!CCircle::Inspect(_pImg , tInspCircle , CirclePara , &CircleRslt )){
                    m_sErrMsg = CircleRslt.sErrMsg;
                    return false ;
                }
                Rslt.pTrackerRslts[i].pPoints[0].x = CircleRslt.dCntrX  ;
                Rslt.pTrackerRslts[i].pPoints[0].y = CircleRslt.dCntrY  ;
                Rslt.pTrackerRslts[i].iRadius      = CircleRslt.dRadius + LPara->iInspOffset ;

            }




            //Area 영역 칠하는것 시작.

            TCircle Circle ;
            Circle.cx  = Rslt.pTrackerRslts[i].pPoints[0].x ;
            Circle.cy  = Rslt.pTrackerRslts[i].pPoints[0].y ;
            Circle.rad = Rslt.pTrackerRslts[i].iRadius      ;
            const unsigned char cPolyIn = 1 ;
            Rslt.InspArea.AddCircle(Circle , cPolyIn );

            if(InspAreaRect.left  >Rslt.pTrackerRslts[i].pPoints[0].x - Rslt.pTrackerRslts[i].iRadius)InspAreaRect.left   =Rslt.pTrackerRslts[i].pPoints[0].x - Rslt.pTrackerRslts[i].iRadius ;
            if(InspAreaRect.right <Rslt.pTrackerRslts[i].pPoints[0].x + Rslt.pTrackerRslts[i].iRadius)InspAreaRect.right  =Rslt.pTrackerRslts[i].pPoints[0].x + Rslt.pTrackerRslts[i].iRadius ;
            if(InspAreaRect.top   >Rslt.pTrackerRslts[i].pPoints[0].y - Rslt.pTrackerRslts[i].iRadius)InspAreaRect.top    =Rslt.pTrackerRslts[i].pPoints[0].y - Rslt.pTrackerRslts[i].iRadius ;
            if(InspAreaRect.bottom<Rslt.pTrackerRslts[i].pPoints[0].y + Rslt.pTrackerRslts[i].iRadius)InspAreaRect.bottom =Rslt.pTrackerRslts[i].pPoints[0].y + Rslt.pTrackerRslts[i].iRadius ;
        }
        else { //사각형 및 다각형일때..
            const int iTrackerCornerCnt = Tracker -> GetPointCount() ;
            //결과 코너 포인트 할당. delete 는 이미 Rslt.Clear에서 해놨음.
            Rslt.pTrackerRslts[i].iPointCnt = iTrackerCornerCnt ;
            Rslt.pTrackerRslts[i].pPoints   = new TDPoint[iTrackerCornerCnt] ;
            pTrackerPnt = Tracker -> GetPoints() ;


            if(MPara.bUsePreEdge) {//검사전에 검사영역을 폴리곤을 이용하여 정밀 하게 엣지를 딴다.
                CLine::TRslt LineRslt ;
                CLine::TPara LinePara ;
                //LinePara.iScanDirect = lsUpToDn;  //이건 의미 없고 보내주는 4포인트에서 FrstStt , FrstEnd 등으로 순서가 이용된다.
                LinePara.bLtToDk     =!MParaShape.bLtOnDk;        //밝은 물체 볼때 는 DkToLt임.
                LinePara.iSampleGap  = MParaShape.iLnSampleGap  ;
                LinePara.iLineGap    = MParaShape.iLnLineGap    ;
                LinePara.iEndOffset  = MParaShape.iLnEndOffset  ;
                LinePara.iCntrOffset = MParaShape.iLnCntrOffset ;

                LinePara.EdgPara.iAverage  = MParaShape.iEgAverage    ;
                LinePara.EdgPara.iCompare  = MParaShape.iEgCompare    ;

                LinePara.EdgPara.iChkPxLow = MParaShape.iEgChkPxLow   ;
                LinePara.EdgPara.iChkPxHgh = MParaShape.iEgChkPxHgh   ;
                LinePara.EdgPara.iOfsGap   = MParaShape.iEgOfsGap     ;
                LinePara.EdgPara.iOfsPxLow = MParaShape.iEgOfsPxLow   ;
                LinePara.EdgPara.iOfsPxHgh = MParaShape.iEgOfsPxHgh   ;

                TDPoint tFrstStt , tFrstEnd ;
                TDPoint tLastStt , tLastEnd ;

                TDPoint tSttPnt , tEndPnt ;
                double  dAngle ;

                TDPoint * aLineSttPnt = new TDPoint[iTrackerCornerCnt];
                TDPoint * aLineEndPnt = new TDPoint[iTrackerCornerCnt];
                for(int j = 0 ; j < iTrackerCornerCnt ; j++){
                    //각도 보정.
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

                    //한변에서의 엣지값을 복사.
                    for(int k = 0 ; k < LineRslt.EdgList.GetDataCnt() ;k++){
                        Rslt.pTrackerRslts[i].lLineEdgeList.PushBack(LineRslt.EdgList.GetCrntData(!k));
                    }

                    aLineSttPnt[j] = LineRslt.StartPnt ;
                    aLineEndPnt[j] = LineRslt.EndPnt   ;
                } //for(int j = 0 ; j < iTrackerCornerCnt ; j++){ 끝.

                for(int j = 0 ; j < iTrackerCornerCnt ; j++){
                    if(!CMath::GetLineCrossPnt(aLineSttPnt[(iTrackerCornerCnt + j - 1)%iTrackerCornerCnt] ,
                                             aLineEndPnt[(iTrackerCornerCnt + j - 1)%iTrackerCornerCnt] ,
                                             aLineSttPnt[(                    j    )%iTrackerCornerCnt] ,
                                             aLineEndPnt[(                    j    )%iTrackerCornerCnt] ,
                                             Rslt.pTrackerRslts[i].pPoints[j])){
                         m_sErrMsg = "Lines are never make cross pnt";
                        delete [] aLineSttPnt ;
                        delete [] aLineEndPnt ;

                        return false ;
                    }
                }

                delete [] aLineSttPnt ;
                delete [] aLineEndPnt ;
            }//LPara.bUsePreEdge 끝.
            else { //그냥 트렉커의 포인트만 이용하는 것.
                for(int j = 0 ; j < iTrackerCornerCnt ; j++){ //나중에 확인 해보고 memcpy로 바꿔보자.

                    CMath::GetRotPntFromCntrPnt(pTrackerPnt[j].x + dInspOfsX, pTrackerPnt[j].y + dInspOfsY, dInspOfsT ,
                                              dOriAbsX , dOriAbsY ,
                                              &Rslt.pTrackerRslts[i].pPoints[j].x , &Rslt.pTrackerRslts[i].pPoints[j].y);
                }
            }
            //폴리곤 모서리 포인트들 찾는것 끝.

            //Area 영역 칠하는것 시작.
            if(LPara->iInspOffset != 0) { //오프셑 적용.
                TDPoint * tMarginedPnts = new TDPoint [Rslt.pTrackerRslts[i].iPointCnt];
                CMath::GetMarginPntsFromPnts(Rslt.pTrackerRslts[i].pPoints ,tMarginedPnts , Rslt.pTrackerRslts[i].iPointCnt , LPara->iInspOffset) ;
                memcpy(Rslt.pTrackerRslts[i].pPoints ,tMarginedPnts , Rslt.pTrackerRslts[i].iPointCnt * sizeof(TDPoint));
                delete [] tMarginedPnts ;
            }
            const unsigned char cPolyIn = 1 ;
            Rslt.InspArea.AddPolygon(Rslt.pTrackerRslts[i].pPoints , Rslt.pTrackerRslts[i].iPointCnt , cPolyIn);
            //CMath::GetOutRectFromPnts(Rslt.pTrackerRslts[i].pPoints , Rslt.pTrackerRslts[i].iPointCnt , &InspAreaRect);

            for(int j = 0 ; j < iTrackerCornerCnt ; j++){
                if(InspAreaRect.left  >Rslt.pTrackerRslts[i].pPoints[j].x)InspAreaRect.left   = Rslt.pTrackerRslts[i].pPoints[j].x ;
                if(InspAreaRect.right <Rslt.pTrackerRslts[i].pPoints[j].x)InspAreaRect.right  = Rslt.pTrackerRslts[i].pPoints[j].x ;
                if(InspAreaRect.top   >Rslt.pTrackerRslts[i].pPoints[j].y)InspAreaRect.top    = Rslt.pTrackerRslts[i].pPoints[j].y ;
                if(InspAreaRect.bottom<Rslt.pTrackerRslts[i].pPoints[j].y)InspAreaRect.bottom = Rslt.pTrackerRslts[i].pPoints[j].y ;
            }
        }
    }//각 트렉커들 검사영역 설정끝....

    //이그 노어 넣어야 함...
    const unsigned char cIgnr = 0 ;
    int iTrackerIgnrCnt = m_lTrckIgnr.GetDataCnt() ;
    TDPoint * pPoints ;
    for(int i = 0 ; i < iTrackerIgnrCnt ; i++){
        //if(Rslt.pTrackerRslts[i].eTrackerType =
        Tracker = m_lTrckIgnr.GetCrntData(!i) ;

        pPoints = new TDPoint[Tracker -> GetPointCount()];

        for(int j = 0 ; j < Tracker->GetPointCount() ; j++) {
            CMath::GetRotPntFromCntrPnt(Tracker->GetPoints()[j].x + dInspOfsX, Tracker->GetPoints()[j].y + dInspOfsY, dInspOfsT ,
                                      dOriAbsX , dOriAbsY ,
                                      &pPoints[j].x , &pPoints[j].y);
        }

        if(Tracker -> TrackerType == CTracker::ttCircle) {
            TCircle Circle ;
            Circle.cx  = pPoints[0].x ;
            Circle.cy  = pPoints[0].y ;
            Circle.rad = Tracker -> CircleRadian ;
            Rslt.InspArea.AddCircle(Circle , cIgnr );
        }
        else {
            Rslt.InspArea.AddPolygon(pPoints , Tracker->GetPointCount() , cIgnr);
        }

        delete [] pPoints ;
    }

    //디펙트영역
    //=========================================
    const unsigned char cDefect = 1 ;
    Rslt.DefectArea.SetSize(_pImg->GetWidth() , _pImg->GetHeight());
    if(MPara.iInspMathod == 0 && (CParaThres.iLowThreshold != CParaThres.iHighThreshold)){ //Threshold
        unsigned char cCrntPx = 0 ;
        for(int y = InspAreaRect.top ; y < InspAreaRect.bottom ; y++){
            for(int x = InspAreaRect.left ; x < InspAreaRect.right ; x++){
                if(Rslt.InspArea.GetPixel(x,y)){
                    cCrntPx = _pImg -> GetPixel(x,y);
                    if(CParaThres.iLowThreshold <= cCrntPx && cCrntPx < CParaThres.iHighThreshold){
                        Rslt.DefectArea.SetPixel(x,y,cDefect) ;    //Height:990 , Width:1300 일경우 노트북에서 22ms 100회 평균.
                        //AreaPx(Rslt.DefectArea.GetArea()  , Rslt.DefectArea.GetWidth() , x , y) = cDefect ;   //Height:990 , Width:1300 일경우 노트북에서 22ms 100회 평균.
                    }
                }
            }
        }
    }

    //Mophology===================================================================
    if(MPara.iFrstMorpMeth == 1){ // Erosion.
        Rslt.DefectArea.ErosionTrace(InspAreaRect,MPara.iFrstMorpSize);
    }
    else if(MPara.iFrstMorpMeth == 2){ // Dilation.
        Rslt.DefectArea.DilationTrace(InspAreaRect,MPara.iFrstMorpSize,cDefect);
    }

    if(MPara.iScndMorpMeth == 1){ // Erosion.
        Rslt.DefectArea.ErosionTrace(InspAreaRect,MPara.iScndMorpSize);
    }
    else if(MPara.iScndMorpMeth == 2){ // Dilation.
        Rslt.DefectArea.DilationTrace(InspAreaRect,MPara.iScndMorpSize,cDefect);
    }

    if(MPara.iThrdMorpMeth == 1){ // Erosion.
        Rslt.DefectArea.ErosionTrace(InspAreaRect,MPara.iThrdMorpSize);
    }
    else if(MPara.iThrdMorpMeth == 2){ // Dilation.
        Rslt.DefectArea.DilationTrace(InspAreaRect,MPara.iThrdMorpSize,cDefect);
    }

    //블럽 ============blob
    //처음불럽 추출.
    CBlob::TPara BLPara ;
    CBlob::TFltr BLFltr ;

    BLPara.dPitchX     = 1                  ; //블랍 검색 X 피치
    BLPara.dPitchY     = CPara.iMinHeight   ; //블랍 검색 Y 피치
    BLPara.iThreshold  = 0                  ; //검색 트레숄드.
    BLPara.bFindLtOnDk = true               ; //검은바탕흰물체인지 흰바탕 검은물체인지.
    BLPara.bIgnrInside = false              ; //블랍 내부 블랍 무시.
    BLPara.bIgnrSide   = false              ; //사각형 테투리 경계에 있는 놈들 무시.
    BLPara.iStartXOfs  = 0                  ; //검사 시작 오프셑
    BLPara.iStartYOfs  = 0                  ; //검사 시작 오프셑.

    BLFltr.iMinArea    = CPara.iMinArea     ;
    BLFltr.iMaxArea    = CPara.iMaxArea     ; //에리아 민맥스값
    BLFltr.iMinWidth   = CPara.iMinWidth    ;
    BLFltr.iMaxWidth   = CPara.iMaxWidth    ; //넓이
    BLFltr.iMinHeight  = CPara.iMinHeight   ;
    BLFltr.iMaxHeight  = CPara.iMaxHeight   ; //놉이
    BLFltr.dMinBoundary= CPara.dMinBoundary ;
    BLFltr.dMaxBoundary= CPara.dMaxBoundary ; //테두리
    BLFltr.dMinCompact = CPara.dMinCompact  ;
    BLFltr.dMaxCompact = CPara.dMaxCompact  ; //에리아/테두리
    if(!CBlob::Inspect(&Rslt.DefectArea ,  InspAreaRect.GetTRect()  , BLPara , BLFltr , &Rslt.BlobRslt)){
        m_sErrMsg = Rslt.BlobRslt.sErrMsg ;
        return false ;
    }

    Rslt.iDefectCnt = Rslt.BlobRslt.iBlobCnt ;
    //0번 검사만 넣는다.
    Val = m_pValueList -> GetValueByName(MPara.sOutDefectCntAdd);
    if(Val) Val -> Set(Rslt.iDefectCnt);

    if(CPara.iMinTotalCnt < Rslt.iDefectCnt) {
        m_sErrMsg = "Total Defect Count Over :" + String(Rslt.iDefectCnt);
        Rslt.bRsltOk  = false ;
    }
    else {
        Rslt.bRsltOk  = true  ;
    }


    Rslt.bInspEnd = true ;
    return true ;

}

String CDefect_V01::GetErrMsg()
{
    return m_sErrMsg ;
}


        //화면 출력.
void CDefect_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if(!Rslt.bInspEnd) return ;
    if(m_bSkip) return ;

    CGdi *Gdi = new CGdi(_hDc,_dScaleX , _dScaleY , _dOffsetX , _dOffsetY);


    AnsiString sTemp ;

    Gdi->m_tPen.Color = Rslt.bRsltOk ? clLime : clRed ;
    Gdi->m_tText.Color = Rslt.bRsltOk ? clLime : clRed ;
    Gdi->m_tPen.Style = PS_SOLID ;

    //Result Area.
    /*
    CValue2 * ValOutArea = m_pValueList -> GetValueByName(MPara.sOutAreaAdd);
    if(ValOutArea){
        CArea * pOutArea = (CArea *)ValOutArea -> GetValAdd() ;
        pOutArea -> ShowArea( _hDc , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY , 0, 0);
        //pOutArea
        //pOutArea -> SaveBmp("D:\\sunsun.bmp");
    }
    */
    Rslt.DefectArea.ShowArea( _hDc , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY , 0, 0);


    //  검사영역 표현 ... 나중에 디버깅 할때 쓰자.
    //int iX ;
    //int iY ;
    //double dLeghth ;
    //TDPoint tPreConerPnt , tConerPnt;
    //TDPoint tEdgePnt ;
    //for(int i = 0 ; i < Rslt.iTrackerRsltCnt ; i++) {
    //    Gdi -> m_tPen.Style = psDot ;
    //    if(Rslt.pTrackerRslts[i].eTrackerType == CTracker::ttCircle){
    //        Gdi -> Circle(false ,Rslt.pTrackerRslts[i].pPoints[0].GetTPoint(),Rslt.pTrackerRslts[i].iRadius , 0 );
    //    }
    //    else { //사각형 및 다각형.
    //        tPreConerPnt = Rslt.pTrackerRslts[i].pPoints[Rslt.pTrackerRslts[i].iPointCnt-1];
    //        for(int j = 0 ; j < Rslt.pTrackerRslts[i].iPointCnt ; j++) {
    //            tConerPnt = Rslt.pTrackerRslts[i].pPoints[j];
    //            Gdi->Line((int)tPreConerPnt.x , (int)tPreConerPnt.y ,(int)tConerPnt.x , (int)tConerPnt.y);
    //
    //            //iX = (tPreConerPnt.x + tConerPnt.x) / 2.0 ;
    //            //iY = (tPreConerPnt.y + tConerPnt.y) / 2.0 ;
    //            //dLeghth = CMath::GetLineLenth(tPreConerPnt.x , tPreConerPnt.y ,tConerPnt.x ,tConerPnt.y ) * m_dPxResol ;
    //            //sTemp = sTemp.sprintf("L:%.3f",dLeghth) ;
    //            //Gdi->Text(iX ,iY , sTemp.c_str()) ;
    //            //
    //            tPreConerPnt = tConerPnt ;
    //        }
    //    }
    //    Gdi -> m_tPen.Style = psSolid ;
    //
    //    for(int j = 0 ; j < Rslt.pTrackerRslts[i].lLineEdgeList.GetDataCnt() ; j++) {
    //        tEdgePnt = Rslt.pTrackerRslts[i].lLineEdgeList.GetCrntData(!j);
    //        Gdi -> Pixel((int)(tEdgePnt.x),(int)(tEdgePnt.y),clAqua);
    //    }
    //}


    /*  이그노어... 돌린값 표현 하려면 일이 커져서 표현 안함.

    for(int i = 0 ; i < iTrackerIgnrCnt ; i++) {
        IgnrTracker = m_lTrckIgnr.GetCrntData(!i);

        Gdi -> m_tPen.Style = psDot ;
        Gdi->m_tPen.Color = clSilver ;
        if(IgnrTracker -> TrackerType == CTracker::ttCircle){
            Gdi -> Circle(false ,IgnrTracker -> GetPoints()[0].GetTPoint() ,IgnrTracker -> CircleRadian , 0 );
        }
        else {

            iTemp = IgnrTracker -> GetPointCount()-1 ;
            tPreConerPnt = IgnrTracker -> GetPoints()[iTemp];
            for(int j = 0 ; j < IgnrTracker -> GetPointCount() ; j++) {
                tConerPnt = IgnrTracker -> GetPoints()[j];
                Gdi->Line((int)tPreConerPnt.x , (int)tPreConerPnt.y ,(int)tConerPnt.x , (int)tConerPnt.y);
                tPreConerPnt = tConerPnt ;
            }
        }
        Gdi -> m_tPen.Style = psSolid ;
        Gdi->m_tPen.Color = Rslt.bRsltOk ? clLime : clRed ;
    }
    */




    //밑에 놈들 페인트 칠하는 걸로 바꿔보자.
    Gdi -> m_tPen.Style = psSolid ;
    CBlob::TBlob * Blob ;
    TPoint tBoundarySttPnt ;
    TPoint tBoundaryEndPnt ;
    for(int i = 0 ; i < Rslt.BlobRslt.iBlobCnt ; i++) {
        Blob = Rslt.BlobRslt.plBlob.GetCrntData(!i);

        Gdi -> m_tPen.Color  = clRed ;
        Gdi -> m_tText.Color = clLime;

        Gdi -> Rect(false ,Blob->left,Blob->top,Blob->right,Blob->bottom);

        tBoundarySttPnt = Blob->plBoundary.GetCrntData(true);
        for(int j = 0 ; j < Blob->plBoundary.GetDataCnt() ; j++){
            tBoundaryEndPnt = Blob->plBoundary.GetCrntData(!j);
              Gdi -> Line(tBoundarySttPnt.x , tBoundarySttPnt.y , tBoundaryEndPnt.x , tBoundaryEndPnt.y);
            tBoundarySttPnt = tBoundaryEndPnt ;
        }
    }

    if(!Rslt.bRsltOk){
        Gdi -> m_tText.Color = clRed ;
        Gdi -> m_tFont  .Height = 30       ;
        Gdi -> Text(5, 5 ,m_sErrMsg.c_str()) ;
    }


    delete Gdi;
    

}




void CDefect_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CGdi *Gdi = new CGdi(_hDc);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    CPopupTracker * Tracker ;
    int iTrackerInspCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerInspCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> Paint(_hDc ,  _dScaleX ,  _dScaleY ,  _dOffsetX ,  _dOffsetY);

    }

    int iTrackerIgnrCnt = m_lTrckIgnr.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerIgnrCnt ; i++){
        Tracker = m_lTrckIgnr.GetCrntData(!i) ;
        Tracker -> Paint(_hDc ,  _dScaleX ,  _dScaleY ,  _dOffsetX ,  _dOffsetY);

    }

    delete Gdi;

}

        //이미지 화면 클릭 이벤트
bool CDefect_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CPopupTracker * Tracker ;
    bool bRet = false ;
    int iTrackerInspCnt = m_lTrckInsp.GetDataCnt() ;
    int iTrackerIgnrCnt = m_lTrckIgnr.GetDataCnt() ;

    for(int i = 0 ; i < iTrackerInspCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> SetSelected(false);
    }
    for(int i = 0 ; i < iTrackerIgnrCnt ; i++){
        Tracker = m_lTrckIgnr.GetCrntData(!i) ;
        Tracker -> SetSelected(false);
    }

    for(int i = 0 ; i < iTrackerInspCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerDw(Button,Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY)){
            bRet = true;

            m_pActiveLPara     = m_lLPara    .GetData(i);
            m_pActiveLParaPoly = m_lLParaPoly.GetData(i);

            return bRet ;
        }
    }

    for(int i = 0 ; i < iTrackerIgnrCnt ; i++){
        Tracker = m_lTrckIgnr.GetCrntData(!i) ;
        if(Tracker -> TrackerDw(Button,Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY)){
            bRet = true;

            m_pActiveLPara     = NULL;
            m_pActiveLParaPoly = NULL;

            return bRet ;
        }
    }
    return bRet;
}


bool CDefect_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CPopupTracker * Tracker ;
    bool bRet = false ;
    int iTrackerInspCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerInspCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerMv(Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY)) bRet = true ;
    }

    int iTrackerIgnrCnt = m_lTrckIgnr.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerIgnrCnt ; i++){
        Tracker = m_lTrckIgnr.GetCrntData(!i) ;
        if(Tracker -> TrackerMv(Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY)) bRet = true ;
    }
    return bRet ;
}

void CDefect_V01::MouseUp(TMouseButton Button,TShiftState Shift )
{
    CPopupTracker * Tracker ;
    int iTrackerInspCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerInspCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerUp(Button, Shift)){// && Button == mbRight) {
            Tracker -> Popup() ;
            break ;
        }
    }

    int iTrackerIgnrCnt = m_lTrckIgnr.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerIgnrCnt ; i++){
        Tracker = m_lTrckIgnr.GetCrntData(!i) ;
        if(Tracker -> TrackerUp(Button, Shift)){// && Button == mbRight) {
            Tracker -> Popup() ;
            break ;
        }
    }
}

        //파라미터 디스플레이
void CDefect_V01::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        UpdateListPara(_bToTable , _vePara , &MPara.List);
        if(MPara.bUsePreEdge) {
            UpdateListPara(_bToTable , _vePara , &MParaShape.List);
        }

    }
    else {
        UpdateListPara(_bToTable , _vePara , &MPara.List);
        if(MPara.bUsePreEdge) {
            UpdateListPara(_bToTable , _vePara , &MParaShape.List);
        }
        UpdateMasterPara(true , _vePara );

    }

    //검사 트렉커 밴드 인에이블..
    CPopupTracker * Tracker ;
    int iTrackerInspCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerInspCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> BandEnabled = MPara.bUsePreEdge ;
    }
    if(m_fpOnPaint)m_fpOnPaint();
}

void CDefect_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        UpdateListPara(_bToTable , _vePara , &CPara .List);
             if(MPara.iInspMathod == 0) UpdateListPara(_bToTable , _vePara , &CParaThres.List);
        else if(MPara.iInspMathod == 1) UpdateListPara(_bToTable , _vePara , &CParaEdge .List);

    }
    else {
        UpdateListPara(_bToTable , _vePara , &CPara.List);
             if(MPara.iInspMathod == 0) UpdateListPara(_bToTable , _vePara , &CParaThres.List);
        else if(MPara.iInspMathod == 1) UpdateListPara(_bToTable , _vePara , &CParaEdge .List);

        UpdateCommonPara(true , _vePara );
    }

}
void CDefect_V01::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
{
    if(!m_pActiveLPara || !m_pActiveLParaPoly) {
        _vePara -> Strings -> Clear() ;  //m_pActiveLPara
        return ;
    }
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;  //m_pActiveLPara
        UpdateListPara(_bToTable , _vePara , &m_pActiveLPara->List);
        if(MPara.bUsePreEdge) {
            UpdateListPara(_bToTable , _vePara , &m_pActiveLParaPoly->List);
        }
    }
    else {
        UpdateListPara(_bToTable , _vePara , &m_pActiveLPara->List);
        if(MPara.bUsePreEdge) {
            UpdateListPara(_bToTable , _vePara , &m_pActiveLParaPoly->List);
        }
        UpdateLocalPara(true , _vePara );
    }
}

        //로드 세이브.
void CDefect_V01::LoadSave(bool _bLoad , String _sVisnName)
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


    int iTrackerInspCnt = m_lTrckInsp.GetDataCnt() ;
    int iTrackerIgnrCnt = m_lTrckIgnr.GetDataCnt() ;
    CPopupTracker  * Tracker   ;
    TLocalPara     * LPara     ;
    TLocalParaPoly * LParaPoly ;

    if(_bLoad) {
        if(FileExists(sIniPath)){
            LoadSaveList(_bLoad , sIniPath , "MasterPara     " , &MPara     .List);
            LoadSaveList(_bLoad , sIniPath , "CommonPara     " , &CPara     .List);
            LoadSaveList(_bLoad , sIniPath , "CommonParaThres" , &CParaThres.List);
            LoadSaveList(_bLoad , sIniPath , "CommonParaEdge " , &CParaEdge .List);

            //Tracker ;
            for(int i = 0 ; i < iTrackerInspCnt ; i++) {
                delete m_lTrckInsp.GetCrntData(!i);
            }
            m_lTrckInsp.DeleteAll();

            int iLParaCnt = m_lLPara.GetDataCnt();
            for(int i = 0 ; i < iLParaCnt ; i++) {
                delete m_lLPara.GetCrntData(!i);
            }
            m_lLPara.DeleteAll();

            int iLParaPolyCnt = m_lLParaPoly.GetDataCnt();
            for(int i = 0 ; i < iLParaPolyCnt ; i++) {
                delete m_lLParaPoly.GetCrntData(!i);
            }
            m_lLParaPoly.DeleteAll();

            //Ignr Tracker ;
            for(int i = 0 ; i < iTrackerIgnrCnt ; i++) {
                delete m_lTrckIgnr.GetCrntData(!i);
            }
            m_lTrckIgnr.DeleteAll();

            Ini.Load(sIniPath ,"TrackerInsp" , "Count" , iTrackerInspCnt );
            for(int i=0 ; i < iTrackerInspCnt ; i++) {
                Tracker = new CPopupTracker();
                Tracker -> Tag         = i;
                Tracker -> Caption     = "Insp"+String(i);
                Tracker -> SetOnPaint(OnTrackerChanged);
                Tracker -> SetOnMenuItemClick(OnTrackerMenuItemClick) ;
                Tracker -> LoadSave(_bLoad , sIniPath , "TrackerInsp"+String(i));
                m_lTrckInsp.PushBack(Tracker);

                LPara     = new TLocalPara    ();
                LoadSaveList(_bLoad , sIniPath , "LocalPara"+String(i) , &LPara    ->List);
                m_lLPara    .PushBack(LPara    );

                LParaPoly = new TLocalParaPoly();
                LoadSaveList(_bLoad , sIniPath , "LocalParaPoly"+String(i), &LParaPoly->List);
                m_lLParaPoly.PushBack(LParaPoly);

                Tracker -> BandEnabled = MPara.bUsePreEdge ;
            }

            Ini.Load(sIniPath ,"TrackerIgnr" , "Count" , iTrackerIgnrCnt );
            for(int i=0 ; i < iTrackerIgnrCnt ; i++) {
                Tracker = new CPopupTracker();
                Tracker -> Tag         = i;
                Tracker -> Caption     = "Ignr"+String(i);
                Tracker -> LineColor   = clSilver ;
                Tracker -> SetOnPaint(OnTrackerChanged);
                Tracker -> SetOnMenuItemClick(OnTrackerMenuItemClick) ;
                Tracker -> LoadSave(_bLoad , sIniPath , "TrackerIgnr"+String(i));
                m_lTrckIgnr.PushBack(Tracker);
            }







        }
    }
    else {
        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath       ) ;
        LoadSaveList(_bLoad , sIniPath , "MasterPara     " , &MPara     .List);
        LoadSaveList(_bLoad , sIniPath , "CommonPara     " , &CPara     .List);
        LoadSaveList(_bLoad , sIniPath , "CommonParaThres" , &CParaThres.List);
        LoadSaveList(_bLoad , sIniPath , "CommonParaEdge " , &CParaEdge .List);

        //Tracker ;
        Ini.Save(sIniPath ,"TrackerInsp" , "Count" , iTrackerInspCnt );
        for(int i=0 ; i < iTrackerInspCnt ; i++) {
            Tracker = m_lTrckInsp.GetCrntData(!i) ;
            Tracker -> LoadSave(_bLoad , sIniPath , "TrackerInsp"+String(i));

            LPara = m_lLPara.GetCrntData(!i) ;
            LoadSaveList(_bLoad , sIniPath , "LocalPara"+String(i) , &LPara    ->List);

            LParaPoly = m_lLParaPoly.GetCrntData(!i) ;
            LoadSaveList(_bLoad , sIniPath , "LocalParaPoly"+String(i), &LParaPoly->List);

            Tracker -> BandEnabled = MPara.bUsePreEdge ;
        }

        Ini.Save(sIniPath ,"TrackerIgnr" , "Count" , iTrackerIgnrCnt );
        for(int i=0 ; i < iTrackerIgnrCnt ; i++) {
            Tracker = m_lTrckIgnr.GetCrntData(!i);
            Tracker -> LoadSave(_bLoad , sIniPath , "TrackerIgnr"+String(i));
        }
    }
}



