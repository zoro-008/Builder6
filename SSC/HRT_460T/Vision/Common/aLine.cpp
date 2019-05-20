
//---------------------------------------------------------------------------
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "aLine.h"
#include "BaseMaths.h"
#include "SLogUnit.h"


CLine::CLine()
{
}

CLine::~CLine()
{

}


bool CLine::Inspect(CImage * _pImg  , TRect _tRect , TPara _tPara , TRslt * _pRslt , FGetCalPos _fpCalPos)
{

    TDPoint tFrstStt ;
    TDPoint tFrstEnd ;
    TDPoint tLastStt ;
    TDPoint tLastEnd ;
    
    if(_tPara.iScanDirect == lsLtToRt){
        tFrstStt.x = _tRect.left  ; tFrstStt.y = _tRect.bottom;
        tFrstEnd.x = _tRect.right ; tFrstEnd.y = _tRect.bottom;
        tLastStt.x = _tRect.left  ; tLastStt.y = _tRect.top   ;
        tLastEnd.x = _tRect.right ; tLastEnd.y = _tRect.top   ;
    }
    else if(_tPara.iScanDirect == lsRtToLt){
        tFrstStt.x = _tRect.right ; tFrstStt.y = _tRect.top   ;
        tFrstEnd.x = _tRect.left  ; tFrstEnd.y = _tRect.top   ;
        tLastStt.x = _tRect.right ; tLastStt.y = _tRect.bottom;
        tLastEnd.x = _tRect.left  ; tLastEnd.y = _tRect.bottom;
    }
    else if(_tPara.iScanDirect == lsUpToDn){
        tFrstStt.x = _tRect.left  ; tFrstStt.y = _tRect.top   ;
        tFrstEnd.x = _tRect.left  ; tFrstEnd.y = _tRect.bottom;
        tLastStt.x = _tRect.right ; tLastStt.y = _tRect.top   ;
        tLastEnd.x = _tRect.right ; tLastEnd.y = _tRect.bottom;
    }
    else if(_tPara.iScanDirect == lsDnToUp){
        tFrstStt.x = _tRect.right ; tFrstStt.y = _tRect.bottom;
        tFrstEnd.x = _tRect.right ; tFrstEnd.y = _tRect.top   ;
        tLastStt.x = _tRect.left  ; tLastStt.y = _tRect.bottom;
        tLastEnd.x = _tRect.left  ; tLastEnd.y = _tRect.top   ;
    }

    return Inspect (_pImg  , tFrstStt , tFrstEnd , tLastStt , tLastEnd , _tPara , _pRslt , _fpCalPos);





//정수 연산이라 조금 빠른 데 그냥 쓰지 말자... 그냥 플로팅 연산으로 통일 하자...
/*
    DWORD dwTime =GetTime();
    _pRslt->Clear();

    //Para Check & Err Check
    if(_pImg == NULL            ) {_pRslt->sErrMsg = "Image Buffer is NULL."   ; return false ; }
    if(_pImg -> SetRect(&_tRect)) {_pRslt->sErrMsg = "Rect Overed Image Size." ; return false ; }

    if(_tPara.iSampleGap < 1) _tPara.iSampleGap = 1 ;
    if(_tPara.iLineGap   < 1) _tPara.iLineGap   = 1 ;

    int      iCntr ;
    TPoint   SttPnt , EndPnt ;
    TDPoint  RsltPnt ;
    TDPoint  BfCalPnt ;

    CEdge        Edge ;
    CEdge::TRslt EdgRslt ;


    if(_tPara.iScanDirect == lsLtToRt || _tPara.iScanDirect == lsRtToLt){
        iCntr = _tRect.top + _tRect.Height() / 2 ;
        for(int y = _tRect.top + _tPara.iEndOffset ; y < _tRect.bottom - _tPara.iEndOffset ;y+=_tPara.iSampleGap){
            if(iCntr - _tPara.iCntrOffset < y && y < iCntr + _tPara.iCntrOffset) continue ;
            if(_tPara.iScanDirect == lsLtToRt) {
                SttPnt.x = _tRect.left  ; SttPnt.y = y ;
                EndPnt.x = _tRect.right ; EndPnt.y = y ;
            }
            else {
                SttPnt.x = _tRect.right ; SttPnt.y = y ;
                EndPnt.x = _tRect.left  ; EndPnt.y = y ;
            }
            if(!Edge.Inspect(_pImg , SttPnt , EndPnt , _tPara.EdgPara , &EdgRslt)) continue ;
            if(_tPara.bLtToDk) {RsltPnt.x = EdgRslt.LtToDkPnt.x ; RsltPnt.y = EdgRslt.LtToDkPnt.y;}
            else               {RsltPnt.x = EdgRslt.DkToLtPnt.x ; RsltPnt.y = EdgRslt.DkToLtPnt.y;}
            if(_fpCalPos) {
                BfCalPnt.x = RsltPnt.x ;
                BfCalPnt.y = RsltPnt.y ;
                _fpCalPos(BfCalPnt.x , BfCalPnt.y , &RsltPnt.x , &RsltPnt.y);
            }
            _pRslt->EdgList.PushBack(RsltPnt) ;
        }

    }
    else if(_tPara.iScanDirect == lsUpToDn||_tPara.iScanDirect == lsDnToUp) {
        iCntr = _tRect.left + _tRect.Width() / 2 ;
        for(int x = _tRect.left + _tPara.iEndOffset ; x < _tRect.right - _tPara.iEndOffset ;x+=_tPara.iSampleGap){
            if(iCntr - _tPara.iCntrOffset < x && x < iCntr + _tPara.iCntrOffset) continue ;
            if(_tPara.iScanDirect == lsUpToDn) {
                SttPnt.x = x ; SttPnt.y = _tRect.top    ;
                EndPnt.x = x ; EndPnt.y = _tRect.bottom ;
            }
            else {
                SttPnt.x = x ; SttPnt.y = _tRect.bottom ;
                EndPnt.x = x ; EndPnt.y = _tRect.top    ;
            }
            if(!Edge.Inspect(_pImg , SttPnt , EndPnt , _tPara.EdgPara , &EdgRslt)) continue ;
            if(_tPara.bLtToDk) {RsltPnt.x = EdgRslt.LtToDkPnt.x ; RsltPnt.y = EdgRslt.LtToDkPnt.y;}
            else               {RsltPnt.x = EdgRslt.DkToLtPnt.x ; RsltPnt.y = EdgRslt.DkToLtPnt.y;}
            if(_fpCalPos) {
                BfCalPnt.x = RsltPnt.x ;
                BfCalPnt.y = RsltPnt.y ;
                _fpCalPos(BfCalPnt.x , BfCalPnt.y , &RsltPnt.x , &RsltPnt.y);
            }
            _pRslt->EdgList.PushBack(RsltPnt) ;
        }

    }
    else {
        _pRslt->sErrMsg = "Para.iScanDirect is Wrong"   ;
        return false ;
    }


    //쫌 병신 같네... 나중에 배열로 바꾸자
    const int iPntCnt = _pRslt->EdgList.GetDataCnt() ;
    TDPoint * Pnt  = new TDPoint[iPntCnt];
    TDPoint TempPnt ;
    for(int i = 0 ; i < iPntCnt ; i++) {
        TempPnt = _pRslt->EdgList.GetCrntData(!i);
        Pnt[i]= TempPnt ;
        Pnt[i] = Pnt[i] ;
    }

    //fomula of Line.
    float a , b ;
    int iMaxIncPntCnt , iIncPntCnt;
    float fLengthPntToLine ;
    int I , J ;
    I=J=0;
    iMaxIncPntCnt = 0 ;
    for (int i = 0 ; i < iPntCnt ; i++) {
       for (int j = iPntCnt-1 ; j > i ; j--) {
          if (i + 1 > j - 1        ) break ; //중간점에서 만나면 더이상은 의미 없음.
          if (j - i < iMaxIncPntCnt) break ; //i,j사이가 현재 최고 큰 포인트포함값 보다 적으면 크게 의미 없음.

          iIncPntCnt = 0 ;
          if(Pnt[i].x == Pnt[j].x) { //기울기가 무한대일경우만 따로 계산.
              for (int k = i + 1 ; k < j ; k++) {
                  if(Pnt[k].x <= Pnt[i].x + _tPara.iLineGap && Pnt[k].x >= Pnt[i].x - _tPara.iLineGap ) iIncPntCnt++ ;
              }
          }
          else {
              //Trace("Pnt[j]", AnsiString(Pnt[j]).c_str()));
              //Trace("Pnt[i]", AnsiString(Pnt[i]).c_str()));
              //
              a = CMath::GetLineA(Pnt[j] , Pnt[i]); // (Pnt[j].y - Pnt[i].y)/(double)(Pnt[j].x - Pnt[i].x) ;
              b = CMath::GetLineB(Pnt[j] , Pnt[i]); //  Pnt[i].y - Pnt[i].x * a ;
              //
              //Trace("Pnt[j]=a", AnsiString(a).c_str()));
              //Trace("Pnt[i]=b", AnsiString(b).c_str()));

              for (int k = 0 ; k < iPntCnt ; k++) {
                 fLengthPntToLine = CMath::GetLengthPntToLine(Pnt[k],a,b);
                 if(fLengthPntToLine <= _tPara.iLineGap) iIncPntCnt++;
              }
          }

          if (iMaxIncPntCnt < iIncPntCnt) {
             iMaxIncPntCnt = iIncPntCnt ;
             I             = i          ;
             J             = j          ;
          }
       }
    }

    _pRslt->StartPnt = Pnt[I] ;
    _pRslt->EndPnt   = Pnt[J] ;

    _pRslt->dA = CMath::GetLineA(_pRslt->StartPnt , _pRslt->EndPnt);
    _pRslt->dB = CMath::GetLineB(_pRslt->StartPnt , _pRslt->EndPnt);

///////////////////////////////////////

    if(_tPara.bUseLstSqure) {
        TDPoint * InPnt = new TDPoint[iMaxIncPntCnt];
        int iIncPntIdx = 0 ;
        if (Pnt[I].x == Pnt[J].x) {
            for(int l = 0 ; l < iPntCnt ; l++) {
                if (Pnt[iIncPntIdx].x <= Pnt[I].x + _tPara.iLineGap && Pnt[iIncPntIdx].x >= Pnt[I].x - _tPara.iLineGap ) {
                    InPnt[iIncPntIdx] = Pnt[l] ;
                    iIncPntIdx++ ;
                }
            }
        }
        else {
            a = CMath::GetLineA(Pnt[J] , Pnt[I]); // (Pnt[j].y - Pnt[i].y)/(double)(Pnt[j].x - Pnt[i].x) ;
            b = CMath::GetLineB(Pnt[J] , Pnt[I]); //  Pnt[i].y - Pnt[i].x * a ;
            for(int l = 0 ; l < iPntCnt ; l++) {
                fLengthPntToLine = CMath::GetLengthPntToLine(Pnt[l],a,b);
                if(fLengthPntToLine <= _tPara.iLineGap) {
                    InPnt[iIncPntIdx] = Pnt[l] ;
                    iIncPntIdx++;
                }
            }
        }



        TDPoint tRetSttPnt ;
        TDPoint tRetEndPnt ;
        bool bSttPntOk = false ;
        bool bEndPntOk = false ;
        double dLeastSqureA ;
        double dLeastSqureB ;
        if(iMaxIncPntCnt)CMath::GetLineABFromPntByLeastSqure(iMaxIncPntCnt , InPnt , dLeastSqureA , dLeastSqureB);

        //스타트 포인트
        if(_tFrstStt.x == _tFrstEnd.x){ //처음 시작 라인의 기울기가 무한대면 따로 계산.
            tRetSttPnt.x = _tFrstStt.x  ;
            tRetSttPnt.y = dLeastSqureA * _tFrstStt.x + dLeastSqureB ;
            bSttPntOk = true ;
        }
        else { //보통 상태..
            double dFrstA = CMath::GetLineA(_tFrstStt , _tFrstEnd);
            double dFrstB = CMath::GetLineB(_tFrstStt , _tFrstEnd);
            if(CMath::GetLineCrossPnt(dLeastSqureA , dLeastSqureB , dFrstA , dFrstB ,tRetSttPnt)){
                bSttPntOk = true  ;
            }
        }
        //엔드 포인트
        if(_tLastStt.x == _tLastStt.x){ //처음 시작 라인의 기울기가 무한대면 따로 계산.
            tRetEndPnt.x = _tLastStt.x  ;
            tRetEndPnt.y = dLeastSqureA * _tLastStt.x + dLeastSqureB ;
            bEndPntOk = true ;
        }
        else { //보통 상태..
            double dLastA = CMath::GetLineA(_tLastStt , _tLastEnd);
            double dLastB = CMath::GetLineB(_tLastStt , _tLastEnd);
            if(CMath::GetLineCrossPnt(dLeastSqureA , dLeastSqureB , dLastA , dLastB ,tRetEndPnt)){
                bEndPntOk = true  ;
            }
        }

        if(bSttPntOk && bEndPntOk){
            _pRslt->StartPnt = tRetSttPnt ;
            _pRslt->EndPnt   = tRetEndPnt ;
            _pRslt->dA = CMath::GetLineA(tRetSttPnt , tRetEndPnt);
            _pRslt->dB = CMath::GetLineB(tRetSttPnt , tRetEndPnt);

        }
        else {
            _pRslt->StartPnt = Pnt[I] ;
            _pRslt->EndPnt   = Pnt[J] ;
            _pRslt->dA = CMath::GetLineA(_pRslt->StartPnt , _pRslt->EndPnt);
            _pRslt->dB = CMath::GetLineB(_pRslt->StartPnt , _pRslt->EndPnt);

        }
        delete [] InPnt ;
    }

//////////////////////////////////
    delete [] Pnt   ;

    _pRslt->dInspTime = CMath::RoundOff(GetTime()- dwTime , 2) ;

    return true;
*/

}

bool CLine::Inspect(CImage * _pImg  , TDPoint _tFrstStt , TDPoint _tFrstEnd , TDPoint _tLastStt , TDPoint _tLastEnd , TPara _tPara , TRslt * _pRslt , FGetCalPos _fpCalPos)
{
    DWORD dwTime =GetTime();
    _pRslt->sErrMsg = "";

    //Para Check & Err Check
    if(_pImg == NULL            ) {_pRslt->sErrMsg = "Image Buffer is NULL."   ; return false ; }
    if(_pImg -> SetPoint(&_tFrstStt.x ,&_tFrstStt.y )) {_pRslt->sErrMsg = "Rect Overed Image Size." ; return false ; }
    if(_pImg -> SetPoint(&_tFrstEnd.x ,&_tFrstEnd.y )) {_pRslt->sErrMsg = "Rect Overed Image Size." ; return false ; }
    if(_pImg -> SetPoint(&_tLastStt.x ,&_tLastStt.y )) {_pRslt->sErrMsg = "Rect Overed Image Size." ; return false ; }
    if(_pImg -> SetPoint(&_tLastEnd.x ,&_tLastEnd.y )) {_pRslt->sErrMsg = "Rect Overed Image Size." ; return false ; }


    if(_tPara.iSampleGap < 1) _tPara.iSampleGap = 1 ;
    if(_tPara.iLineGap   < 1) _tPara.iLineGap   = 1 ;

    _pRslt->Clear();

    int      iCntr ;
    TDPoint  RsltPnt ;

    CEdge        Edge ;
    CEdge::TRslt EdgRslt ;

    TPoint tSttPnt , tEndPnt ;


    double dMaxRange = CMath::GetLineLenth(_tFrstStt.x , _tFrstStt.y , _tLastStt.x , _tLastStt.y) ;
    double dAngle    = CMath::GetLineAngle(_tFrstStt.x , _tFrstStt.y , _tLastStt.x , _tLastStt.y) ;
    double dStep     = _tPara.iSampleGap ;
    TDPoint tEdgeStt , tEdgeEnd ;
    TDPoint  BfCalPnt ;
    for(double d = 0.0 ; d < dMaxRange ; d+=dStep) {
        CMath::GetPntFromPntByAngLen( _tFrstStt.x , _tFrstStt.y ,  //입력 시작점.
                                     dAngle                   ,  //입력 각도.
                                     d                        ,  //입력 길이.
                                    &tEdgeStt.x , &tEdgeStt.y ); //출력 포인트.

        CMath::GetPntFromPntByAngLen( _tFrstEnd.x , _tFrstEnd.y ,  //입력 시작점.
                                     dAngle                   ,  //입력 각도.
                                     d                        ,  //입력 길이.
                                    &tEdgeEnd.x , &tEdgeEnd.y ); //출력 포인트.
        tSttPnt.x = tEdgeStt.x ;
        tSttPnt.y = tEdgeStt.y ;

        tEndPnt.x = tEdgeEnd.x ;
        tEndPnt.y = tEdgeEnd.y ;


        if(!Edge.Inspect(_pImg , tSttPnt , tEndPnt , _tPara.EdgPara , &EdgRslt)) continue ;
        if(_tPara.bLtToDk) {RsltPnt.x = EdgRslt.LtToDkPnt.x ; RsltPnt.y = EdgRslt.LtToDkPnt.y;}
        else               {RsltPnt.x = EdgRslt.DkToLtPnt.x ; RsltPnt.y = EdgRslt.DkToLtPnt.y;}
        if(_fpCalPos) {
            BfCalPnt.x = RsltPnt.x ;
            BfCalPnt.y = RsltPnt.y ;
            _fpCalPos(BfCalPnt.x , BfCalPnt.y , &RsltPnt.x , &RsltPnt.y);
        }
        _pRslt->EdgList.PushBack(RsltPnt) ;

    }



    //쫌 병신 같네... 나중에 배열로 바꾸자
    const int iPntCnt = _pRslt->EdgList.GetDataCnt() ;
    TDPoint * Pnt  = new TDPoint[iPntCnt];
    TDPoint TempPnt ;
    for(int i = 0 ; i < iPntCnt ; i++) {
        TempPnt = _pRslt->EdgList.GetCrntData(!i);
        Pnt[i]= TempPnt ;
    }

    //fomula of Line.
    double a , b ;
    int iMaxIncPntCnt , iIncPntCnt;
    float fLengthPntToLine ;
    int I , J ;
    I=J=0;
    iMaxIncPntCnt = 0 ;
    for (int i = 0 ; i < iPntCnt ; i++) {
       for (int j = iPntCnt-1 ; j > i ; j--) {
          if (i + 1 > j - 1        ) break ; //중간점에서 만나면 더이상은 의미 없음.
          if (j - i < iMaxIncPntCnt) break ; //i,j사이가 현재 최고 큰 포인트포함값 보다 적으면 크게 의미 없음.

          iIncPntCnt = 0 ;
          if(Pnt[i].x == Pnt[j].x) { //기울기가 무한대일경우만 따로 계산.
              for (int k = 0 ; k < iPntCnt ; k++) {
                  if(Pnt[k].x <= Pnt[i].x + _tPara.iLineGap && Pnt[k].x >= Pnt[i].x - _tPara.iLineGap ) iIncPntCnt++ ;
              }
          }
          else {
              //Trace("Pnt[j]", AnsiString(Pnt[j]).c_str()));
              //Trace("Pnt[i]", AnsiString(Pnt[i]).c_str()));
              //
              a = CMath::GetLineA(Pnt[j] , Pnt[i]); // (Pnt[j].y - Pnt[i].y)/(double)(Pnt[j].x - Pnt[i].x) ;
              b = CMath::GetLineB(Pnt[j] , Pnt[i]); //  Pnt[i].y - Pnt[i].x * a ;
              //
              //Trace("Pnt[j]=a", AnsiString(a).c_str()));
              //Trace("Pnt[i]=b", AnsiString(b).c_str()));

              for (int k = 0 ; k < iPntCnt ; k++) {
                 fLengthPntToLine = CMath::GetLengthPntToLine(Pnt[k],a,b);
                 if(fLengthPntToLine <= _tPara.iLineGap) iIncPntCnt++;
              }
          }

          if (iMaxIncPntCnt < iIncPntCnt) {
             iMaxIncPntCnt = iIncPntCnt ;
             I             = i          ;
             J             = j          ;
          }
       }
    }

    _pRslt->StartPnt = Pnt[I] ;
    _pRslt->EndPnt   = Pnt[J] ;

    _pRslt->dA = CMath::GetLineA(_pRslt->StartPnt , _pRslt->EndPnt);
    _pRslt->dB = CMath::GetLineB(_pRslt->StartPnt , _pRslt->EndPnt);
    _pRslt->dAngle = CMath::GetLineAngle(_pRslt->StartPnt.x,_pRslt->StartPnt.y , _pRslt->EndPnt.x , _pRslt->EndPnt.y);

///////////////////////////////////////


    if(_tPara.bUseLstSqure) {
        TDPoint * InPnt = new TDPoint[iMaxIncPntCnt];
        double dLineAngle = CMath::GetLineAngle(Pnt[I].x,Pnt[I].y,Pnt[J].x,Pnt[J].y) ;
        int iIncPntIdx = 0 ;
        //if (Pnt[I].x == Pnt[J].x) {  //기울기 무한대 따로 처리..
        //    for(int l = 0 ; l < iPntCnt ; l++) {
        //        if (Pnt[l].x <= Pnt[I].x + _tPara.iLineGap && Pnt[l].x >= Pnt[I].x - _tPara.iLineGap ) {
        //
        //            //이거 이상함...
        //            //InPnt[iIncPntIdx].x = Pnt[l].y ; //최소자승법에 넣기 위해 X.Y를 뒤집어 넣어준다.
        //            //InPnt[iIncPntIdx].y = Pnt[l].x ;
        //            InPnt[iIncPntIdx].x = Pnt[l].x ;  //최소자승법에 넣기 위해 X.Y를 뒤집어 넣어준다.
        //            InPnt[iIncPntIdx].y = Pnt[l].y ;
        //
        //            iIncPntIdx++ ;
        //        }  //Pnt[k].x <= Pnt[i].x + _tPara.iLineGap && Pnt[k].x >= Pnt[i].x - _tPara.iLineGap
        //    }
        //}
        if ((45 < dLineAngle && dLineAngle < 135)|| //위로 솟아 있는 직선은 90도 회전 해서 최소자승법에 넣는다.
            (225< dLineAngle && dLineAngle < 315)){
            a = CMath::GetLineA(Pnt[J] , Pnt[I]); // (Pnt[j].y - Pnt[i].y)/(double)(Pnt[j].x - Pnt[i].x) ;
            b = CMath::GetLineB(Pnt[J] , Pnt[I]); //  Pnt[i].y - Pnt[i].x * a ;
            for(int l = 0 ; l < iPntCnt ; l++) {
                fLengthPntToLine = CMath::GetLengthPntToLine(Pnt[l],a,b);
                if(fLengthPntToLine <= _tPara.iLineGap) {
                    InPnt[iIncPntIdx].x = Pnt[l].y ;  //최소자승법 쓰기위해 x,y반전.
                    InPnt[iIncPntIdx].y = Pnt[l].x ;
                    iIncPntIdx++;
                }
            }
        }
        else {
            a = CMath::GetLineA(Pnt[J] , Pnt[I]); // (Pnt[j].y - Pnt[i].y)/(double)(Pnt[j].x - Pnt[i].x) ;
            b = CMath::GetLineB(Pnt[J] , Pnt[I]); //  Pnt[i].y - Pnt[i].x * a ;
            for(int l = 0 ; l < iPntCnt ; l++) {
                fLengthPntToLine = CMath::GetLengthPntToLine(Pnt[l],a,b);
                if(fLengthPntToLine <= _tPara.iLineGap) {
                    InPnt[iIncPntIdx] = Pnt[l] ;
                    iIncPntIdx++;
                }
            }
        }



        TDPoint tRetSttPnt ;
        TDPoint tRetEndPnt ;
        bool bSttPntOk = false ;
        bool bEndPntOk = false ;
        double dLeastSqureA ;
        double dLeastSqureB ;



        Trace("Start" ,"");
        for(int l = 0 ; l < iMaxIncPntCnt ; l++) {
            String sTemp = String(InPnt[l].x) +","+String(InPnt[l].y) ;
            Trace((String(l)+"xy:").c_str(), sTemp.c_str());

        }
        
        
        if(iMaxIncPntCnt){
            double dInvA ;
            double dInvB ;
            if ((45 < dLineAngle && dLineAngle < 135)|| //위로 솟아 있는 직선은 90도 회전 해서 최소자승법에 넣는다.
                (225< dLineAngle && dLineAngle < 315)){
                CMath::GetLineABFromPntByLeastSqure(iMaxIncPntCnt , InPnt , dInvA , dInvB);
                if(dInvA != 0) {
                    dLeastSqureA = 1/dInvA ;
                    dLeastSqureB = -dInvB/dInvA ;
                }
                else {
                    //일단 0으로 하고.... 모르겠다.
                    dLeastSqureA = 0 ;
                    dLeastSqureB = 0 ;

                }


            }
            else {
                CMath::GetLineABFromPntByLeastSqure(iMaxIncPntCnt , InPnt , dLeastSqureA , dLeastSqureB);
            }

        }

        //스타트 포인트
        if(_tFrstStt.x == _tFrstEnd.x){ //처음 시작 라인의 기울기가 무한대면 따로 계산.
            tRetSttPnt.x = _tFrstStt.x  ;
            tRetSttPnt.y = dLeastSqureA * _tFrstStt.x + dLeastSqureB ;
            bSttPntOk = true ;
        }
        else { //보통 상태..
            double dFrstA = CMath::GetLineA(_tFrstStt , _tFrstEnd);
            double dFrstB = CMath::GetLineB(_tFrstStt , _tFrstEnd);
            if(CMath::GetLineCrossPnt(dLeastSqureA , dLeastSqureB , dFrstA , dFrstB ,tRetSttPnt)){
                bSttPntOk = true  ;
            }
        }
        //엔드 포인트
        //if(_tLastStt.x == _tLastStt.x){ //처음 시작 라인의 기울기가 무한대면 따로 계산.
        if(_tLastStt.x == _tLastEnd.x){ //처음 시작 라인의 기울기가 무한대면 따로 계산.
            tRetEndPnt.x = _tLastStt.x  ;
            tRetEndPnt.y = dLeastSqureA * _tLastStt.x + dLeastSqureB ;
            bEndPntOk = true ;
        }
        else { //보통 상태..
            double dLastA = CMath::GetLineA(_tLastStt , _tLastEnd);
            double dLastB = CMath::GetLineB(_tLastStt , _tLastEnd);
            if(CMath::GetLineCrossPnt(dLeastSqureA , dLeastSqureB , dLastA , dLastB ,tRetEndPnt)){
                bEndPntOk = true  ;
            }
        }

        if(bSttPntOk && bEndPntOk){
            _pRslt->StartPnt = tRetSttPnt ;
            _pRslt->EndPnt   = tRetEndPnt ;
            _pRslt->dA = CMath::GetLineA(tRetSttPnt , tRetEndPnt);
            _pRslt->dB = CMath::GetLineB(tRetSttPnt , tRetEndPnt);

        }
        else {
            _pRslt->StartPnt = Pnt[I] ;
            _pRslt->EndPnt   = Pnt[J] ;
            _pRslt->dA = CMath::GetLineA(_pRslt->StartPnt , _pRslt->EndPnt);
            _pRslt->dB = CMath::GetLineB(_pRslt->StartPnt , _pRslt->EndPnt);

        }
        delete [] InPnt ;
    }

//////////////////////////////////


    delete [] Pnt   ;

//    _pRslt->dInspTime = CMath::RoundOff(GetTime()- dwTime , 2) ;

    return true;
}

/*
void CLine::PaintRslt(HDC _hHdc , LIN_Rslt * _pRslt , LIN_Disp _tDisp , float _fScaleX , float _fScaleY)
{
    CGdi *Gdi = new CGdi(_hHdc);
    AnsiString sTemp ;
    Gdi -> SetScale(_fScaleX , _fScaleY);

    if(_tDisp.clLine != clNone) {
        Gdi->m_tPen.Color = _tDisp.clLine ;
        Gdi->Line((int)_pRslt->StartPnt.x ,(int)_pRslt->StartPnt.y ,(int)_pRslt->EndPnt.x ,(int)_pRslt->EndPnt.y) ;
    }

    if(_tDisp.clText != clNone) {
        Gdi->m_tText.Color = _tDisp.clText ;
        sTemp = sTemp.sprintf("A:%.3f B:%.3f Angle:%.3f",_pRslt->fA , _pRslt->fB , _pRslt->fAngle) ;

        Gdi->Text((int)_pRslt->StartPnt.x + 5,(int)_pRslt->StartPnt.y + 5 , sTemp.c_str()) ;
    }

    TFPoint Pnt ;
    if(_tDisp.clEdge != clNone) {
        Gdi->m_tPen.Color = _tDisp.clEdge ;

        for(int i = 0 ; i < EdgList.GetDataCnt() ; i++) {
            //Pnt = EdgList.GetCrntData(!i) ;
            //Gdi->Pixel((int)Pnt.x , (int)Pnt.y , _tDisp.clEdge);
        }
    }

    delete Gdi;
}
*/
