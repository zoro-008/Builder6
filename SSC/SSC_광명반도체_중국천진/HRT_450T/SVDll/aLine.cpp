
//---------------------------------------------------------------------------
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include "aLine.h"
#include "SVMaths.h"
#include "GdiUnit.h"
#include "Common.h"

#include "SLogUnit.h"

CLine::CLine()
{
}

CLine::~CLine()
{

}

bool CLine::Inspect(CImage * _pImg  , TRect _tRect , LIN_Para _tPara , LIN_Rslt * _pRslt)
{
    DWORD dwTime =GetTickTimeDouble();
    g_sErrMsg = "";
    memset(_pRslt , 0 , sizeof(*_pRslt));

    //Para Check & Err Check
    if(_pImg == NULL            ) {g_sErrMsg = "Image Buffer is NULL."   ; return false ; }
    if(_pImg -> SetRect(&_tRect)) {g_sErrMsg = "Rect Overed Image Size." ; return false ; }

    if(_tPara.iSampleGap < 1) _tPara.iSampleGap = 1 ;
    if(_tPara.iLineGap   < 1) _tPara.iLineGap   = 1 ;



    EdgList.DeleteAll();

    int      iCntr ;
    TPoint   SttPnt , EndPnt ;
    TFPoint  RsltPnt ;
    EDG_Rslt EdgRslt ;
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
            if(_tPara.bLtToDk) {RsltPnt.x = EdgRslt.LtToDkPntS.x ; RsltPnt.y = EdgRslt.LtToDkPntS.y;}
            else               {RsltPnt.x = EdgRslt.DkToLtPntS.x ; RsltPnt.y = EdgRslt.DkToLtPntS.y;}
            EdgList.PushBack(RsltPnt) ;
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
            if(_tPara.bLtToDk) {RsltPnt.x = EdgRslt.LtToDkPntS.x ; RsltPnt.y = EdgRslt.LtToDkPntS.y;}
            else               {RsltPnt.x = EdgRslt.DkToLtPntS.x ; RsltPnt.y = EdgRslt.DkToLtPntS.y;}
            EdgList.PushBack(RsltPnt) ;
        }

    }
    else {
        g_sErrMsg = "Para.iScanDirect is Wrong"   ;
        return false ;
    }


    TFPoint * Pnt  = new TFPoint[EdgList.GetDataCnt()];
    for(int i = 0 ; i < EdgList.GetDataCnt() ; i++) {
        Pnt[i]=EdgList.GetCrntData(!i);
    }

    //fomula of Line.
    float a , b ;
    int iMaxPntCnt , iIncPntCnt;
    float fLengthPntToLine ;
    int I , J ;
    I=J=0;
    iMaxPntCnt = 0 ;
    for (int i = 0 ; i < EdgList.GetDataCnt() ; i++) {
       for (int j = EdgList.GetDataCnt()-1 ; j > i ; j--) {
          if (i + 1 > j - 1     ) break ; //중간점에서 만나면 더이상은 의미 없음.
          if (j - i < iMaxPntCnt) break ; //i,j사이가 현재 최고 큰 포인트포함값 보다 적으면 크게 의미 없음.

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
              a = MATH_GetLineA(Pnt[j] , Pnt[i]); // (Pnt[j].y - Pnt[i].y)/(double)(Pnt[j].x - Pnt[i].x) ;
              b = MATH_GetLineB(Pnt[j] , Pnt[i]); //  Pnt[i].y - Pnt[i].x * a ;
              //
              //Trace("Pnt[j]=a", AnsiString(a).c_str()));
              //Trace("Pnt[i]=b", AnsiString(b).c_str()));

              for (int k = 0 ; k < EdgList.GetDataCnt() ; k++) {
                 fLengthPntToLine = MATH_GetLengthPntToLine(Pnt[k],a,b);
                 if(fLengthPntToLine < _tPara.iLineGap) iIncPntCnt++;
              }
          }

          if (iMaxPntCnt < iIncPntCnt) {
             iMaxPntCnt = iIncPntCnt ;
             I          = i          ;
             J          = j          ;
          }
       }
    }
    _pRslt->StartPnt = Pnt[I] ;
    _pRslt->EndPnt   = Pnt[J] ;

         if(_tPara.iScanDirect == lsLtToRt ) _pRslt->fAngle = MATH_GetLineAngle(Pnt[I].x , Pnt[I].y , Pnt[J].x , Pnt[J].y );
    else if(_tPara.iScanDirect == lsUpToDn ) {
        _pRslt->fAngle = MATH_GetLineAngle(Pnt[I].x , Pnt[I].y , Pnt[J].x , Pnt[J].y ); //요놈이 문제 359 도 에서 1도 플러스 되면 0도임.
        if(_pRslt->fAngle >= 270) _pRslt -> fAngle = _pRslt -> fAngle - 360 ;
    }
    else if(_tPara.iScanDirect == lsRtToLt ) _pRslt->fAngle = MATH_GetLineAngle(Pnt[J].x , Pnt[J].y , Pnt[I].x , Pnt[I].y );
    else if(_tPara.iScanDirect == lsDnToUp ) _pRslt->fAngle = MATH_GetLineAngle(Pnt[J].x , Pnt[J].y , Pnt[I].x , Pnt[I].y );

    TFPoint * InPnt = new TFPoint[iMaxPntCnt];
    int iIncPntIdx = 0 ;
    if (Pnt[I].x == Pnt[J].x) {
        for(int l = 0 ; l < iMaxPntCnt ; l++) {
            if (Pnt[iIncPntIdx].x <= Pnt[I].x + _tPara.iLineGap && Pnt[iIncPntIdx].x >= Pnt[I].x - _tPara.iLineGap ) InPnt[l] = Pnt[iIncPntIdx] ;
            iIncPntIdx++ ;
        }
    }
    else {
        a = MATH_GetLineA(Pnt[J] , Pnt[I]); // (Pnt[j].y - Pnt[i].y)/(double)(Pnt[j].x - Pnt[i].x) ;
        b = MATH_GetLineB(Pnt[J] , Pnt[I]); //  Pnt[i].y - Pnt[i].x * a ;
        for(int l = 0 ; l < iMaxPntCnt ; l++) {
            fLengthPntToLine = MATH_GetLengthPntToLine(Pnt[iIncPntIdx],a,b);
            if(fLengthPntToLine < _tPara.iLineGap) InPnt[l] = Pnt[iIncPntIdx] ;
            iIncPntIdx++;
        }
    }

    //나중에 한번 검증해야함. 제대로 동작하고 있는 건지.. 오차범위..
    if(iMaxPntCnt)MATH_GetLineABFromPntByLeastSqure(iIncPntCnt , InPnt , _pRslt->fA , _pRslt->fB);

    delete [] InPnt ;
    delete [] Pnt   ;

    _pRslt->fInspTime = MATH_RoundOff(GetTickTimeDouble()- dwTime , 2) ;

    return true;

}

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

