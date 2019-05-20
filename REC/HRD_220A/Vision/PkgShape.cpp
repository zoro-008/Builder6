//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgShape.h"
#include "BaseMaths.h"
#include "UnitArea.h"
#include "SLogUnit.h"

#include "aCircle.h"
#include "aLine.h"

#include "UserINI.h"
#include "UnitGdi.h"
#include "GlobalMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

REGIST_STATICLLINK_CPP(CShape_V01)
CShape_V01::CShape_V01()
{

}

CShape_V01::~CShape_V01()
{
}

void CShape_V01::OnTrackerMenuItemClick(EMenuItem _iItemNo, CPopupTracker* _pTracker)
{
    int iTrackerInspCnt = m_lTrckInsp.GetDataCnt() ;
    int iTrackerIgnrCnt = m_lTrckIgnr.GetDataCnt() ;
    CPopupTracker* Tracker ;
    TLocalParaCircle * pNewLParaCircle , pLParaCircle ;
    TLocalParaPoly   * pNewLParaPoly   , pLParaPoly   ;

    if(_iItemNo == miNewInsp) {
        Tracker = new CPopupTracker();
        Tracker -> Tag = iTrackerInspCnt ;
        Tracker -> Caption = "Insp"+String(iTrackerInspCnt);
        Tracker -> SetOnPaint(OnTrackerChanged);
        Tracker -> SetOnMenuItemClick(OnTrackerMenuItemClick) ;
        m_lTrckInsp.PushBack(Tracker);

        pNewLParaCircle = new TLocalParaCircle();
        m_lLParaCircle.PushBack(pNewLParaCircle);

        pNewLParaPoly = new TLocalParaPoly();
        m_lLParaPoly.PushBack(pNewLParaPoly);

    }
    else if(_iItemNo == miNewIgnr) {
    }
    else if(_iItemNo == miDelete) {
        if(_pTracker-> Caption.Pos("Insp")){
            m_lTrckInsp   .Delete(_pTracker->Tag);
            m_lLParaCircle.Delete(_pTracker->Tag);
            m_lLParaPoly  .Delete(_pTracker->Tag);
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
            Tracker -> BandEnabled = true ;

            Tracker -> Caption = "Insp"+String(iTrackerInspCnt);
            Tracker -> SetOnPaint(OnTrackerChanged);
            Tracker -> SetOnMenuItemClick(OnTrackerMenuItemClick) ;
            m_lTrckInsp.PushBack(Tracker);

            pNewLParaCircle = new TLocalParaCircle();
            *pNewLParaCircle = *m_lLParaCircle.GetData(_pTracker->Tag);
            m_lLParaCircle.PushBack(pNewLParaCircle);

            pNewLParaPoly = new TLocalParaPoly();
            *pNewLParaPoly = *m_lLParaPoly.GetData(_pTracker->Tag);
            m_lLParaPoly.PushBack(pNewLParaPoly);
        }
        else {
        }
    }
    if(m_fpOnPaint)m_fpOnPaint();
}

void CShape_V01::OnTrackerChanged()
{
     //화면에서 다시 그리므로 여기서 메세지 날릴 필요 없다.
}


//순수 가상 함수.
//==============================================================================
bool CShape_V01::Init()
{
    Prop.bUseTrain = false ;
    Prop.bCamera   = false ;

    //이 블럭 은 나중에 트렉커 많아지면 같이 증가해야 함..
    CPopupTracker * Tracker = new CPopupTracker();
    Tracker -> TrackerType  = CTracker::ttRect ;
    Tracker -> Tag          = 0 ;
    Tracker -> Caption      = "Insp0";
    Tracker -> BandEnabled  = true ;
    Tracker -> BandWidth    = 50 ;
    Tracker -> SetOnPaint(OnTrackerChanged);
    Tracker -> SetOnMenuItemClick(OnTrackerMenuItemClick) ;
    m_lTrckInsp.PushBack(Tracker);

    m_lLParaCircle.PushBack(new TLocalParaCircle());
    m_lLParaPoly.PushBack(new TLocalParaPoly  ());

    
    m_pActiveLParaCircle= NULL ;
    m_pActiveLParaPoly = NULL ;

    return true ;

}
bool CShape_V01::Close()
{
    m_pActiveLParaCircle= NULL ;
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

    int iLParaCnt = m_lLParaCircle.GetDataCnt();
    for(int i = 0 ; i < iLParaCnt ; i++) {
        delete m_lLParaCircle.GetCrntData(!i);
    }
    m_lLParaCircle.DeleteAll();

    int iLParaPolyCnt = m_lLParaPoly.GetDataCnt();
    for(int i = 0 ; i < iLParaPolyCnt ; i++) {
        delete m_lLParaPoly.GetCrntData(!i);
    }
    m_lLParaPoly.DeleteAll();

    return true ;
}


        //검사 및 러닝 관련.
bool CShape_V01::Train(CImage * _pImg)
{
    m_sErrMsg = "";

    return true ;
}

void CShape_V01::RsltClear() //검사 결과값을 검사전에 클리어 한번 하고 한다.
{
    Rslt.Clear();
}

bool CShape_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}


//double GetRound(double _dVal , int _iCnt)
//{
//
//
//}
bool CShape_V01::Run(CImage * _pImg)
{
    if(m_bSkip) {
        Rslt.bRsltOk = true ;
        return true ;
    }
    m_sErrMsg = "";
    Rslt.Clear();  //결과값 클리어.

    //Trace("1","1");

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

    CPopupTracker    * Tracker ;
    TLocalParaCircle * LParaCircle ;
    TLocalParaPoly   * LParaPoly ;
    TDPoint          * pTrackerPnt ;
    const int          iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    Rslt.iTrackerRsltCnt = iTrackerCnt ;
    Rslt.pTrackerRslts   = new TTrackerRslt[iTrackerCnt]; //Delete는 Rslt.Clear에서 수행한다.

    //Trace("2","2");
    for(int i = 0 ; i < iTrackerCnt ; i++) {
        Tracker     = m_lTrckInsp   .GetCrntData(!i) ;
        LParaCircle = m_lLParaCircle.GetCrntData(!i) ;
        LParaPoly   = m_lLParaPoly  .GetCrntData(!i) ;
        //Trace("3",String(i).c_str());
        if(Tracker -> TrackerType == CTracker::ttCircle) {
            Rslt.pTrackerRslts[i].eTrackerType = CTracker::ttCircle ;
            Rslt.pTrackerRslts[i].iPointCnt = 1 ; //원은 중심점 좌표만 가지고 있는다.
            Rslt.pTrackerRslts[i].pPoints   = new TDPoint[1] ;
            pTrackerPnt = Tracker -> GetPoints() ;
            CMath::GetRotPntFromCntrPnt(pTrackerPnt[0].x + dInspOfsX, pTrackerPnt[0].y + dInspOfsY, dInspOfsT ,
                                      dOriAbsX , dOriAbsY ,
                                      &Rslt.pTrackerRslts[i].pPoints[0].x , &Rslt.pTrackerRslts[i].pPoints[0].y);

            TCircle tInspCircle ;
            tInspCircle.cx  = Rslt.pTrackerRslts[i].pPoints[0].x ;
            tInspCircle.cy  = Rslt.pTrackerRslts[i].pPoints[0].y ;
            tInspCircle.rad = Tracker -> CircleRadian ;



            CCircle::TPara CirclePara ;
            CirclePara.bLtOnDk    = MParaShape.bLtOnDk      ;
            CirclePara.iGap       = MParaShape.iLnLineGap   ;
            CirclePara.iSearchGap = Tracker -> BandWidth    ;
            CirclePara.iSampleCnt = MParaShape.iCcSampleCnt ;

            //20170410 이해하지 않고 그냥 추가 한다.
            LParaCircle = m_lLParaCircle.GetCrntData(!i) ;
            CirclePara.iStartAng  = LParaCircle->dStartAng  ; //0
            CirclePara.iEndAng    = LParaCircle->dEndAng    ; //0

            CirclePara.EdgPara.iAverage  = MParaShape.iEgAverage    ;
            CirclePara.EdgPara.iCompare  = MParaShape.iEgCompare    ;
            CirclePara.EdgPara.iChkPxLow = MParaShape.iEgChkPxLow   ;
            CirclePara.EdgPara.iChkPxHgh = MParaShape.iEgChkPxHgh   ;
            CirclePara.EdgPara.iOfsGap   = MParaShape.iEgOfsGap     ;
            CirclePara.EdgPara.iOfsPxLow = MParaShape.iEgOfsPxLow   ;
            CirclePara.EdgPara.iOfsPxHgh = MParaShape.iEgOfsPxHgh   ;

            CCircle::TRslt CircleRslt ;

            if(!MPara.bUseRoiRslt){
                if(!CCircle::Inspect(_pImg , tInspCircle , CirclePara , &CircleRslt , m_fpGetCalPos)){
                    m_sErrMsg = CircleRslt.sErrMsg;
                    return false ;
                }

                for(int k = 0 ; k < CircleRslt.lEdges.GetDataCnt() ;k++){
                    Rslt.pTrackerRslts[i].lEdgeList.PushBack(CircleRslt.lEdges.GetCrntData(!k));
                }
                Rslt.pTrackerRslts[i].pPoints[0].x = CircleRslt.dCntrX  ;
                Rslt.pTrackerRslts[i].pPoints[0].y = CircleRslt.dCntrY  ;
                Rslt.pTrackerRslts[i].dRadius      = CircleRslt.dRadius ;

                Rslt.pTrackerRslts[i].tCntPnt.x =CircleRslt.dCntrX  ;
                Rslt.pTrackerRslts[i].tCntPnt.y =CircleRslt.dCntrY  ;
            }
            else {
                Rslt.pTrackerRslts[i].pPoints[0].x = Rslt.pTrackerRslts[i].pPoints[0].x ;
                Rslt.pTrackerRslts[i].pPoints[0].y = Rslt.pTrackerRslts[i].pPoints[0].y ;
                Rslt.pTrackerRslts[i].dRadius      = Tracker -> CircleRadian ;

                Rslt.pTrackerRslts[i].tCntPnt.x =CircleRslt.dCntrX  ;
                Rslt.pTrackerRslts[i].tCntPnt.x =CircleRslt.dCntrY  ;




            }

            if(m_fpGetPxResol()){
                Rslt.pTrackerRslts[i].dRadius *= m_fpGetPxResol() ;

            }

        }
        else { //사각형 및 다각형일때..
            const int iTrackerCornerCnt = Tracker -> GetPointCount() ;
            //결과 코너 포인트 할당. delete 는 이미 Rslt.Clear에서 해놨음.
            Rslt.pTrackerRslts[i].iPointCnt = iTrackerCornerCnt ;
            Rslt.pTrackerRslts[i].pPoints   = new TDPoint[iTrackerCornerCnt] ;

            Rslt.pTrackerRslts[i].pAngles   = new double[iTrackerCornerCnt] ;
            Rslt.pTrackerRslts[i].pLengths  = new double[iTrackerCornerCnt] ;

           pTrackerPnt = Tracker -> GetPoints() ;

            CLine::TRslt LineRslt ;
            CLine::TPara LinePara ;
            //LinePara.iScanDirect = lsUpToDn;  //이건 의미 없고 보내주는 4포인트에서 FrstStt , FrstEnd 등으로 순서가 이용된다.
            LinePara.bLtToDk      =!MParaShape.bLtOnDk;        //밝은 물체 볼때 는 DkToLt임.
            LinePara.iSampleGap   = MParaShape.iLnSampleGap  ;
            LinePara.iLineGap     = MParaShape.iLnLineGap    ;
            LinePara.iEndOffset   = MParaShape.iLnEndOffset  ;
            LinePara.iCntrOffset  = MParaShape.iLnCntrOffset ;
            LinePara.bUseLstSqure = MParaShape.bUseLstSqure  ;

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
            double dDebugTemp=0.0 ;


            for(int j = 0 ; j < iTrackerCornerCnt ; j++){
                //각도 보정.
                //Trace("pTrackerPnt[j].x",String(pTrackerPnt[j].x).c_str());
                //Trace("dInspOfsX"       ,String(dInspOfsX       ).c_str());

                //Trace("pTrackerPnt[j].y",String(pTrackerPnt[j].y).c_str());
                //Trace("dInspOfsY"       ,String(dInspOfsY       ).c_str());

                //Trace("dInspOfsT"       ,String(dInspOfsT       ).c_str());
                //Trace("dOriAbsX"        ,String(dOriAbsX        ).c_str());
                //Trace("dOriAbsY"        ,String(dOriAbsY        ).c_str());


                CMath::GetRotPntFromCntrPnt(pTrackerPnt[j].x + dInspOfsX, pTrackerPnt[j].y + dInspOfsY, dInspOfsT ,
                                            dOriAbsX , dOriAbsY ,
                                           &tSttPnt.x , &tSttPnt.y);
                CMath::GetRotPntFromCntrPnt(pTrackerPnt[(j+1)%iTrackerCornerCnt].x + dInspOfsX, pTrackerPnt[(j+1)%iTrackerCornerCnt].y + dInspOfsY, dInspOfsT ,
                                            dOriAbsX , dOriAbsY ,
                                           &tEndPnt.x , &tEndPnt.y);
                dAngle = CMath::GetLineAngle(tSttPnt.x , tSttPnt.y , tEndPnt.x , tEndPnt.y) ;


                CMath::GetPntFromPntByAngLen(  tSttPnt.x , tSttPnt.y ,   //입력 시작점.
                                             dAngle + 90             ,   //입력 각도.
                                             Tracker -> BandWidth    ,   //입력 길이.
                                            &tFrstStt.x , &tFrstStt.y ); //출력 포인트.
                CMath::GetPntFromPntByAngLen(  tSttPnt.x , tSttPnt.y ,   //입력 시작점.
                                             dAngle - 90             ,   //입력 각도.
                                             Tracker -> BandWidth    ,   //입력 길이.
                                            &tFrstEnd.x , &tFrstEnd.y ); //출력 포인트.
                CMath::GetPntFromPntByAngLen(  tEndPnt.x , tEndPnt.y ,   //입력 시작점.
                                             dAngle + 90             ,   //입력 각도.
                                             Tracker -> BandWidth    ,   //입력 길이.
                                            &tLastStt.x , &tLastStt.y ); //출력 포인트.
                CMath::GetPntFromPntByAngLen(  tEndPnt.x , tEndPnt.y ,   //입력 시작점.
                                             dAngle - 90             ,   //입력 각도.
                                             Tracker -> BandWidth    ,   //입력 길이.
                                            &tLastEnd.x , &tLastEnd.y ); //출력 포인트.

                //Trace("11","GetPntFromPntByAngLen End");

                //한번을 검사함.
                if(!MPara.bUseRoiRslt){
                    if(!CLine::Inspect(_pImg  , tFrstStt , tFrstEnd , tLastStt , tLastEnd , LinePara , &LineRslt , m_fpGetCalPos)) {
                        m_sErrMsg = LineRslt.sErrMsg;
                        delete [] aLineSttPnt ;
                        delete [] aLineEndPnt ;

                        return false ;
                    }

                    //Trace("11","Line End");

                    //한변에서의 엣지값을 복사.
                    for(int k = 0 ; k < LineRslt.EdgList.GetDataCnt() ;k++){
                        Rslt.pTrackerRslts[i].lEdgeList.PushBack(LineRslt.EdgList.GetCrntData(!k));
                    }

                    aLineSttPnt[j] = LineRslt.StartPnt ;
                    aLineEndPnt[j] = LineRslt.EndPnt   ;
                }
                else {
                    aLineSttPnt[j] = tSttPnt ;
                    aLineEndPnt[j] = tEndPnt ;

                }
                //Trace("11","for End");
            } //for(int j = 0 ; j < iTrackerCornerCnt ; j++){ 끝.




            //Trace("12","0");
            //꼭지점을 뽑는다.
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
            //Trace("13","0");

            //각도및 길이를 뽑는다.
            double dPosSumX = 0.0;
            double dPosSumY = 0.0;
            for(int j = 0 ; j < iTrackerCornerCnt ; j++){

                //각도 순서는 0번 포인트 서부터..              Rslt.pTrackerRslts[i].pPoints[j]
                Rslt.pTrackerRslts[i].pAngles[j] = CMath::GetLinesIncAngle(Rslt.pTrackerRslts[i].pPoints[(iTrackerCornerCnt + j - 1)%iTrackerCornerCnt] , // aLineSttPnt[(iTrackerCornerCnt + j - 1)%iTrackerCornerCnt] ,
                                                                           Rslt.pTrackerRslts[i].pPoints[j]                                             , // aLineEndPnt[(iTrackerCornerCnt + j - 1)%iTrackerCornerCnt] ,
                                                                           Rslt.pTrackerRslts[i].pPoints[(iTrackerCornerCnt + j + 1)%iTrackerCornerCnt] );//  aLineSttPnt[(                    j    )%iTrackerCornerCnt] ,
                                                                           //Rslt.pTrackerRslts[i].pPoints[j]   aLineEndPnt[(                    j    )%iTrackerCornerCnt] );

                //길이 순서는 0-1번 포인트간 길이 부터.
                Rslt.pTrackerRslts[i].pLengths[j] = CMath::GetLineLenth(Rslt.pTrackerRslts[i].pPoints[j].x,
                                                                        Rslt.pTrackerRslts[i].pPoints[j].y,
                                                                        Rslt.pTrackerRslts[i].pPoints[(j + 1)%iTrackerCornerCnt].x,
                                                                        Rslt.pTrackerRslts[i].pPoints[(j + 1)%iTrackerCornerCnt].y);

                if(m_fpGetPxResol()){
                    Rslt.pTrackerRslts[i].pLengths[j] *= m_fpGetPxResol() ;
                }

                dPosSumX += Rslt.pTrackerRslts[i].pPoints[j].x ;
                dPosSumY += Rslt.pTrackerRslts[i].pPoints[j].y ;

            }

            Rslt.pTrackerRslts[i].tCntPnt.x = dPosSumX/(double)iTrackerCornerCnt ;
            Rslt.pTrackerRslts[i].tCntPnt.y = dPosSumY/(double)iTrackerCornerCnt ;

            delete [] aLineSttPnt ;
            delete [] aLineEndPnt ;
            //폴리곤 모서리 포인트들 찾는것 끝.
        }
        //Trace("14","0");
        Rslt.pTrackerRslts[i].dCntOfsX = Rslt.pTrackerRslts[i].tCntPnt.x - _pImg->GetWidth ()/2.0;
        Rslt.pTrackerRslts[i].dCntOfsY = Rslt.pTrackerRslts[i].tCntPnt.y - _pImg->GetHeight()/2.0;

        if(m_fpGetPxResol){
            Rslt.pTrackerRslts[i].dCntOfsX *= m_fpGetPxResol() ;
            Rslt.pTrackerRslts[i].dCntOfsY *= m_fpGetPxResol() ;
        }
    }

    //Trace("5","5");



    //먼저 OK해놓고 검색해서 NG있으면 바꿈.
    Rslt.bRsltOk  = true  ;
    for(int i = 0 ; i < iTrackerCnt ; i++) {
        LParaCircle = m_lLParaCircle.GetCrntData(!i) ;
        LParaPoly   = m_lLParaPoly  .GetCrntData(!i) ;
        Tracker     = m_lTrckInsp   .GetCrntData(!i) ;
        pTrackerPnt = Tracker -> GetPoints() ;
        const int iTrackerCornerCnt = Tracker -> GetPointCount() ;

        Rslt.pTrackerRslts[i].bRsltOk = true ;
        if(Tracker -> TrackerType == CTracker::ttCircle) {
            if(LParaCircle->dRadTolerance>0){
                if(Rslt.pTrackerRslts[i].dRadius < LParaCircle->dRadian - LParaCircle->dRadTolerance ||
                   Rslt.pTrackerRslts[i].dRadius > LParaCircle->dRadian + LParaCircle->dRadTolerance ){
                    m_sErrMsg.sprintf("%d Tracker Radius is Spec Out", i);
                    Rslt.bRsltOk=false ;
                    Rslt.pTrackerRslts[i].bRsltOk = false ;
                }
            }
        }
        else { //사각형 및 다각형일때..

            //길이 확인.
            String sTempLengths = LParaPoly -> sLengths ;
            String sLength ;

            if(LParaPoly->dLengTolerance>0.0) {
                int iMaxCnt = 1;
                while(sTempLengths.Pos(",")){
                    sTempLengths.Delete(sTempLengths.Pos(","),1);
                    iMaxCnt++;
                }
                sTempLengths = LParaPoly -> sLengths ;
                int iCnt = 0 ;
                double dLength ;
                do{
                    if(iTrackerCornerCnt <= iCnt) break ;
                    sLength = sTempLengths.SubString(1,sTempLengths.Pos(",")-1);
                    dLength = StrToFloatDef(sLength , 0.0);

                    if(Rslt.pTrackerRslts[i].pLengths[iCnt] < dLength - LParaPoly->dLengTolerance ||
                       Rslt.pTrackerRslts[i].pLengths[iCnt] > dLength + LParaPoly->dLengTolerance ){
                        m_sErrMsg.sprintf("%d Tracker %d Length is Spec Out", i , iCnt);
                        Rslt.bRsltOk=false ;
                        Rslt.pTrackerRslts[i].bRsltOk = false ;
                    }
                    sTempLengths.Delete(1,sTempLengths.Pos(","));
                    iCnt++;
                }while(sTempLengths.Pos(","));
            }


            //각도 확인.
            if(LParaPoly->dLengTolerance>0.0){
                String sTempAngles = LParaPoly -> sAngles ;
                String sAngle ;
                int iMaxCnt = 1;
                while(sTempAngles.Pos(",")){
                    sTempAngles.Delete(sTempAngles.Pos(","),1);
                    iMaxCnt++;
                }
                sTempAngles = LParaPoly -> sAngles ;
                int iCnt = 0 ;
                double dAngle ;
                do{
                    if(iTrackerCornerCnt <= iCnt) break ;
                    sAngle = sTempAngles.SubString(1,sTempAngles.Pos(",")-1);
                    dAngle = StrToFloatDef(sAngle , 0.0);
                    if(Rslt.pTrackerRslts[i].pAngles[iCnt] < dAngle - LParaPoly->dAngleTolerance ||
                       Rslt.pTrackerRslts[i].pAngles[iCnt] > dAngle + LParaPoly->dAngleTolerance ){
                        m_sErrMsg.sprintf("%d Tracker %d Angle is Spec Out", i , iCnt);
                        //Trace("Angle Spec Out" , m_sErrMsg.c_str());
                        //Trace("Angle" , sAngle.c_str());
                        //Trace("Spec", ("min:" + String(dAngle - LParaPoly->dLengTolerance) + " max:" + String(dAngle + LParaPoly->dLengTolerance)).c_str());
                        Rslt.bRsltOk=false ;
                        Rslt.pTrackerRslts[i].bRsltOk = false ;
                    }
                    sTempAngles.Delete(1,sTempAngles.Pos(","));
                    iCnt++;
                }while(sTempAngles.Pos(","));
            }
        }
    }

    //0번 검사만 넣는다.
    Val = m_pValueList -> GetValueByName(MPara.sOutCntOfsXAdd);
    if(Val) Val -> Set(Rslt.pTrackerRslts[0].dCntOfsX);
    Val = m_pValueList -> GetValueByName(MPara.sOutCntOfsYAdd);
    if(Val) Val -> Set(Rslt.pTrackerRslts[0].dCntOfsY);

    //각도 임시로...나중에 바꾸자.
    if(Rslt.pTrackerRslts[0].iPointCnt == 4){ //4각형용.
        double dAngleTop    = CMath::GetLineAngle(Rslt.pTrackerRslts[0].pPoints[1].x , Rslt.pTrackerRslts[0].pPoints[1].y,
                                                  Rslt.pTrackerRslts[0].pPoints[0].x , Rslt.pTrackerRslts[0].pPoints[0].y) - 180.0;

        double dAngleRight  = CMath::GetLineAngle(Rslt.pTrackerRslts[0].pPoints[1].x , Rslt.pTrackerRslts[0].pPoints[1].y,
                                                  Rslt.pTrackerRslts[0].pPoints[2].x , Rslt.pTrackerRslts[0].pPoints[2].y) - 270.0;

        double dAngleBottom = CMath::GetLineAngle(Rslt.pTrackerRslts[0].pPoints[2].x , Rslt.pTrackerRslts[0].pPoints[2].y,
                                                  Rslt.pTrackerRslts[0].pPoints[3].x , Rslt.pTrackerRslts[0].pPoints[3].y) - 180.0;

        double dAngleLeft   = CMath::GetLineAngle(Rslt.pTrackerRslts[0].pPoints[3].x , Rslt.pTrackerRslts[0].pPoints[3].y,
                                                  Rslt.pTrackerRslts[0].pPoints[0].x , Rslt.pTrackerRslts[0].pPoints[0].y) - 90.0 ;

        double dAvrAngle = (dAngleTop + dAngleRight + dAngleBottom + dAngleLeft) / 4.0 ;
        Val = m_pValueList -> GetValueByName(MPara.sOutAngleAdd);
        if(Val) Val -> Set(dAvrAngle);
    }





    Rslt.bInspEnd = true ;
    return true ;

}

String CShape_V01::GetErrMsg()
{
    return m_sErrMsg ;
}


        //화면 출력.
void CShape_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    //if(!Rslt.bInspEnd) return ;
    if(m_bSkip) return ;

    CGdi *Gdi = new CGdi(_hDc,_dScaleX , _dScaleY , _dOffsetX , _dOffsetY);


    AnsiString sTemp ;




    int iX ;
    int iY ;
    double dLeghth ;
    double dAngle  ;

    TDPoint tEdgePnt ;
    for(int i = 0 ; i < Rslt.iTrackerRsltCnt ; i++) {
        Gdi->m_tPen.Color  = Rslt.pTrackerRslts[i].bRsltOk ? clLime : clRed ;
        Gdi->m_tText.Color = Rslt.pTrackerRslts[i].bRsltOk ? clLime : clRed ;
        if(Rslt.pTrackerRslts[i].eTrackerType == CTracker::ttCircle){
            Gdi -> Circle(false ,Rslt.pTrackerRslts[i].pPoints[0].GetTPoint(),Rslt.pTrackerRslts[i].dRadius , 0 );
            sTemp = sTemp.sprintf("RAD:%.3f",Rslt.pTrackerRslts[i].dRadius) ;
            Gdi->Text(Rslt.pTrackerRslts[i].pPoints[0].x ,Rslt.pTrackerRslts[i].pPoints[0].y , sTemp.c_str()) ;
        }
        else { //사각형 및 다각형.
            //길이 표시.
            TDPoint tConerPnt , tConerPntN;
            for(int j = 0 ; j < Rslt.pTrackerRslts[i].iPointCnt ; j++) {
                tConerPnt  = Rslt.pTrackerRslts[i].pPoints[j];
                tConerPntN = Rslt.pTrackerRslts[i].pPoints[(j+1)%Rslt.pTrackerRslts[i].iPointCnt];
                Gdi->Line((int)tConerPnt.x , (int)tConerPnt.y ,(int)tConerPntN.x , (int)tConerPntN.y);
                iX = (tConerPnt.x + tConerPntN.x) / 2.0 ;
                iY = (tConerPnt.y + tConerPntN.y) / 2.0 ;
                dLeghth = Rslt.pTrackerRslts[i].pLengths[j] ;
                sTemp = sTemp.sprintf("L:%.3f",dLeghth) ;
                Gdi->Text(iX ,iY , sTemp.c_str()) ;
            }
            //각도 표시
            for(int j = 0 ; j < Rslt.pTrackerRslts[i].iPointCnt ; j++) {
                tConerPnt = Rslt.pTrackerRslts[i].pPoints[j];
                dAngle = Rslt.pTrackerRslts[i].pAngles[j] ;
                sTemp = sTemp.sprintf("A:%.3f",dAngle) ;
                Gdi->Text((int)tConerPnt.x , (int)tConerPnt.y , sTemp.c_str()) ;
            }

            Gdi->LineCross(Rslt.pTrackerRslts[i].tCntPnt.x , Rslt.pTrackerRslts[i].tCntPnt.y , 5);
            sTemp = sTemp.sprintf("CntOfs X:%.5f Y:%.5f",Rslt.pTrackerRslts[i].dCntOfsX , Rslt.pTrackerRslts[i].dCntOfsY) ;
            Gdi->Text(Rslt.pTrackerRslts[i].tCntPnt.x+5 , Rslt.pTrackerRslts[i].tCntPnt.y+5 , sTemp.c_str());


        }
        Gdi -> m_tPen.Style = psSolid ;

        for(int j = 0 ; j < Rslt.pTrackerRslts[i].lEdgeList.GetDataCnt() ; j++) {
            tEdgePnt = Rslt.pTrackerRslts[i].lEdgeList.GetCrntData(!j);
            Gdi -> Pixel((int)(tEdgePnt.x),(int)(tEdgePnt.y),clAqua);
        }
    }

    if(!Rslt.bRsltOk){
        Gdi -> m_tFont  .Height = 30       ;
        Gdi -> Text(5, 5 ,m_sErrMsg.c_str()) ;
    }
    

    delete Gdi;


}




void CShape_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CGdi *Gdi = new CGdi(_hDc);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    CPopupTracker * Tracker ;
    int iTrackerInspCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerInspCnt ; i++){

        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker->BandEnabled = !MPara.bUseRoiRslt ; 
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
bool CShape_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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

            m_pActiveLParaCircle= m_lLParaCircle    .GetData(i);
            m_pActiveLParaPoly = m_lLParaPoly.GetData(i);

            return bRet ;
        }
    }

    for(int i = 0 ; i < iTrackerIgnrCnt ; i++){
        Tracker = m_lTrckIgnr.GetCrntData(!i) ;
        if(Tracker -> TrackerDw(Button,Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY)){
            bRet = true;

            m_pActiveLParaCircle= NULL;
            m_pActiveLParaPoly = NULL;

            return bRet ;
        }
    }
    return bRet;
}


bool CShape_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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

void CShape_V01::MouseUp(TMouseButton Button,TShiftState Shift )
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
void CShape_V01::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        UpdateListPara(_bToTable , _vePara , &MPara.List);
        UpdateListPara(_bToTable , _vePara , &MParaShape.List);

    }
    else {
        UpdateListPara(_bToTable , _vePara , &MPara.List);
        UpdateListPara(_bToTable , _vePara , &MParaShape.List);
        UpdateMasterPara(true , _vePara );

    }

}

void CShape_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        UpdateListPara(_bToTable , _vePara , &CPara .List);

    }
    else {
        UpdateListPara(_bToTable , _vePara , &CPara.List);
        UpdateCommonPara(true , _vePara );
    }

}
void CShape_V01::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
{
    if(!m_pActiveLParaCircle || !m_pActiveLParaPoly) {
        _vePara -> Strings -> Clear() ;  //m_pActiveLParaCircle
        return ;
    }
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;  //m_pActiveLParaCircle
        UpdateListPara(_bToTable , _vePara , &m_pActiveLParaCircle->List);
        UpdateListPara(_bToTable , _vePara , &m_pActiveLParaPoly->List);
    }
    else {
        UpdateListPara(_bToTable , _vePara , &m_pActiveLParaCircle->List);
        UpdateListPara(_bToTable , _vePara , &m_pActiveLParaPoly->List);
        UpdateLocalPara(true , _vePara );
    }
}

        //로드 세이브.
void CShape_V01::LoadSave(bool _bLoad , String _sVisnName)
{
    String sIniPath = GM.GetJobFileFolder() + _sVisnName + "_" + m_sName + "_" + GetStaticPkgLink()->GetPkgName() + ".ini" ;

    TUserINI Ini ;

    /*
    Tracker -> SetFunc(OnTrackerChanged);
    Tracker -> HeightMax = 100 ;
    Tracker -> WidthMax  = 100 ;
    m_lTrckInsp.PushBack(Tracker);
    m_lLParaCircle   .PushBack(new TLocalParaCircle ());
    */


    int iTrackerInspCnt = m_lTrckInsp.GetDataCnt() ;
    int iTrackerIgnrCnt = m_lTrckIgnr.GetDataCnt() ;
    CPopupTracker    * Tracker   ;
    TLocalParaCircle * LParaCircle     ;
    TLocalParaPoly   * LParaPoly ;

    if(_bLoad) {
        if(FileExists(sIniPath)){
            LoadSaveList(_bLoad , sIniPath , "MasterPara     " , &MPara     .List);
            LoadSaveList(_bLoad , sIniPath , "MasterPara     " , &MParaShape.List); 
            LoadSaveList(_bLoad , sIniPath , "CommonPara     " , &CPara     .List);

            //Tracker ;
            for(int i = 0 ; i < iTrackerInspCnt ; i++) {
                delete m_lTrckInsp.GetCrntData(!i);
            }
            m_lTrckInsp.DeleteAll();

            int iLParaCnt = m_lLParaCircle.GetDataCnt();
            for(int i = 0 ; i < iLParaCnt ; i++) {
                delete m_lLParaCircle.GetCrntData(!i);
            }
            m_lLParaCircle.DeleteAll();

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
                Tracker -> BandEnabled = true ;
                m_lTrckInsp.PushBack(Tracker);

                LParaCircle     = new TLocalParaCircle();
                LoadSaveList(_bLoad , sIniPath , "LocalParaCircle"+String(i) , &LParaCircle    ->List);
                m_lLParaCircle    .PushBack(LParaCircle    );

                LParaPoly = new TLocalParaPoly();
                LoadSaveList(_bLoad , sIniPath , "LocalParaPoly"+String(i), &LParaPoly->List);
                m_lLParaPoly.PushBack(LParaPoly);

                Tracker -> BandEnabled = true ;
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
        LoadSaveList(_bLoad , sIniPath , "MasterPara     " , &MParaShape.List);
        LoadSaveList(_bLoad , sIniPath , "CommonPara     " , &CPara     .List);

        //Tracker ;
        Ini.Save(sIniPath ,"TrackerInsp" , "Count" , iTrackerInspCnt );
        for(int i=0 ; i < iTrackerInspCnt ; i++) {
            Tracker = m_lTrckInsp.GetCrntData(!i) ;
            Tracker -> LoadSave(_bLoad , sIniPath , "TrackerInsp"+String(i));

            LParaCircle = m_lLParaCircle.GetCrntData(!i) ;
            LoadSaveList(_bLoad , sIniPath , "LocalParaCircle"+String(i) , &LParaCircle    ->List);

            LParaPoly = m_lLParaPoly.GetCrntData(!i) ;
            LoadSaveList(_bLoad , sIniPath , "LocalParaPoly"+String(i), &LParaPoly->List);

            Tracker -> BandEnabled = true ;
        }

        Ini.Save(sIniPath ,"TrackerIgnr" , "Count" , iTrackerIgnrCnt );
        for(int i=0 ; i < iTrackerIgnrCnt ; i++) {
            Tracker = m_lTrckIgnr.GetCrntData(!i);
            Tracker -> LoadSave(_bLoad , sIniPath , "TrackerIgnr"+String(i));
        }
    }
}



