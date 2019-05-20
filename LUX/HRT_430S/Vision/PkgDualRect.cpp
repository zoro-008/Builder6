//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgDualRect.h"
#include "BaseMaths.h"

#include "UserINI.h"
#include "GlobalMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

REGIST_STATICLLINK_CPP(CDualRect_V01)


CDualRect_V01::CDualRect_V01()
{

}

CDualRect_V01::~CDualRect_V01()
{
}

void CDualRect_V01::OnTrackerChanged()                             
{
     //화면에서 다시 그리므로 여기서 메세지 날릴 필요 없다.
}


//순수 가상 함수.
//==============================================================================
bool CDualRect_V01::Init()
{
    m_fpGetCalPos  = NULL ;
    m_fpOnPaint    = NULL ;

    Prop.bUseTrain = false ;
    Prop.bCamera   = false ;

    //이 블럭 은 나중에 트렉커 많아지면 같이 증가해야 함..
    CTracker * Tracker ;
    for(int i = 0 ; i < DUAL_RECT_INSP_CNT ; i++) {
        Tracker = new CTracker();
        if(i!=0){
            Tracker -> LineColor = clYellow ;
        }
        Tracker -> Caption = i==0 ? String("MainRect") : "Rect" + String(i);
        Tracker -> BandEnabled = true ;
        Tracker -> BandWidth = 100 ;
        Tracker -> SetOnPaint(OnTrackerChanged);
        m_lTrckInsp.PushBack(Tracker);

        m_lLPara   .PushBack(new TLocalPara ());
    }

    m_pActiveLPara = NULL ;


    return true ;

}
bool CDualRect_V01::Close()
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
bool CDualRect_V01::Train(CImage * _pImg)
{
    m_sErrMsg = "";

    return true ;
}

void CDualRect_V01::RsltClear() //검사 결과값을 검사전에 클리어 한번 하고 한다.
{
    Rslt.Clear();
}

bool CDualRect_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}



bool CDualRect_V01::Run(CImage * _pImg)
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
    double dInspOfsX =0, dInspOfsY=0;
    CValue2 *  Val ;
    Val = m_pValueList -> GetValueByName(MPara.sRefOfsXAdd);
    if(Val)dInspOfsX = Val -> GetDouble();
    Val = m_pValueList -> GetValueByName(MPara.sRefOfsYAdd);
    if(Val)dInspOfsY = Val -> GetDouble();


    //Rect.
    CTracker     * Tracker   ;
    TLocalPara   * LPara     ;
    CRect::TRslt * pRectRslt ;

    //미리 OK로 만들고 스펙 벋어나면 NG로 바꿈.
    Rslt.bRsltOk  = true ;

    for(int i = 0 ; i < DUAL_RECT_INSP_CNT ; i++) {
        Tracker   = m_lTrckInsp.GetCrntData(!i) ;
        LPara     = m_lLPara   .GetCrntData(!i) ;
        pRectRslt = &Rslt.RectRslt[i] ;

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


        //Para.
        CRect::TPara RectPara ;
        RectPara.bLtOnDk           = LPara->bLtOnDk      ;

        RectPara.iSampleGap        = MPara.iLnSampleGap  ;
        RectPara.iLineGap          = MPara.iLnLineGap    ;
        RectPara.iEndOffset        = MPara.iLnEndOffset  ;
        RectPara.iCntrOffset       = MPara.iLnCntrOffset ;

        RectPara.EdgPara.iAverage  = MPara.iEgAverage    ;
        RectPara.EdgPara.iCompare  = MPara.iEgCompare    ;

        RectPara.EdgPara.iChkPxLow = MPara.iEgChkPxLow   ;
        RectPara.EdgPara.iChkPxHgh = MPara.iEgChkPxHgh   ;
        RectPara.EdgPara.iOfsGap   = MPara.iEgOfsGap     ;
        RectPara.EdgPara.iOfsPxLow = MPara.iEgOfsPxLow   ;
        RectPara.EdgPara.iOfsPxHgh = MPara.iEgOfsPxHgh   ;

       //CImage * _pImg  , TRect _tRectL , TRect _tRectT , TRect _tRectR , TRect _tRectB , TPara _tPara , TRslt * _pRslt);
        if(!CRect::Inspect(_pImg , tLRect , tTRect , tRRect , tBRect , RectPara , pRectRslt)){
            m_sErrMsg = String(i) + "-" + pRectRslt->sErrMsg ;
            return false ;
        }
        if(m_fpGetPxResol) {
            Rslt.dLengthL[i] = pRectRslt->dLengthL * m_fpGetPxResol() ;
            Rslt.dLengthT[i] = pRectRslt->dLengthT * m_fpGetPxResol() ;
            Rslt.dLengthR[i] = pRectRslt->dLengthR * m_fpGetPxResol() ;
            Rslt.dLengthB[i] = pRectRslt->dLengthB * m_fpGetPxResol() ;
        }
        if(LPara->dTolerance!=0.0) {
                 if(LPara->dHeight + LPara->dTolerance < Rslt.dLengthL[i] || Rslt.dLengthL[i] < LPara->dHeight - LPara->dTolerance) {m_sErrMsg.sprintf("%d L = %.3f" ,i, Rslt.dLengthL[i]);Rslt.bRsltOk = false ;}
            else if(LPara->dWidth  + LPara->dTolerance < Rslt.dLengthT[i] || Rslt.dLengthT[i] < LPara->dWidth  - LPara->dTolerance) {m_sErrMsg.sprintf("%d T = %.3f" ,i, Rslt.dLengthT[i]);Rslt.bRsltOk = false ;}
            else if(LPara->dHeight + LPara->dTolerance < Rslt.dLengthR[i] || Rslt.dLengthR[i] < LPara->dHeight - LPara->dTolerance) {m_sErrMsg.sprintf("%d R = %.3f" ,i, Rslt.dLengthR[i]);Rslt.bRsltOk = false ;}
            else if(LPara->dWidth  + LPara->dTolerance < Rslt.dLengthB[i] || Rslt.dLengthB[i] < LPara->dWidth  - LPara->dTolerance) {m_sErrMsg.sprintf("%d B = %.3f" ,i, Rslt.dLengthB[i]);Rslt.bRsltOk = false ;}
        }
    }

    Rslt.dOffsetX = Rslt.RectRslt[1].CenterPnt.x - Rslt.RectRslt[0].CenterPnt.x ;
    Rslt.dOffsetY = Rslt.RectRslt[1].CenterPnt.y - Rslt.RectRslt[0].CenterPnt.y ;
    Rslt.dOffsetT = Rslt.RectRslt[1].dAngle      - Rslt.RectRslt[0].dAngle      ;

    if(m_fpGetPxResol) {
        Rslt.dOffsetX *=  m_fpGetPxResol() ;
        Rslt.dOffsetY *=  m_fpGetPxResol() ;
    }

    

    Rslt.dOffsetX = ((int)(Rslt.dOffsetX * 100000)) / 100000.0 ; //소수5자리만 표현.
    Rslt.dOffsetY = ((int)(Rslt.dOffsetY * 100000)) / 100000.0 ; //소수5자리만 표현.

    //포지션 확인.
    //각도 확인.

    //Val = m_pValueList -> GetValueByName(MPara.sOutOfsXAdd);
    //if(Val) Val -> Set(Rslt.dOffsetX);
    //Val = m_pValueList -> GetValueByName(MPara.sOutOfsYAdd);
    //if(Val) Val -> Set(Rslt.dOffsetY);
    //Val = m_pValueList -> GetValueByName(MPara.sOutAngleAdd);
    //if(Val) Val -> Set(Rslt.RectRslt.dAngle);


    Rslt.bInspEnd = true ;

    return true ;

}

String CDualRect_V01::GetErrMsg()
{
    return m_sErrMsg ;
}


        //화면 출력.
void CDualRect_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if(!Rslt.bInspEnd) return ;
    if(m_bSkip) return ;

    CGdi *Gdi = new CGdi(_hDc,_dScaleX , _dScaleY , _dOffsetX , _dOffsetY);


    AnsiString sTemp ;

    Gdi->m_tPen.Color   = Rslt.bRsltOk ? clLime : clRed ;
    Gdi->m_tText.Color  = Rslt.bRsltOk ? clLime : clRed ;
    Gdi->m_tPen.Style   = PS_SOLID ;

    int iX ;
    int iY ;
    CRect::TRslt * pRectRslt ;
    int i ;
    for(i = 0 ; i < DUAL_RECT_INSP_CNT ; i++) {
        pRectRslt = &Rslt.RectRslt[i] ;
        Gdi->Line((int)pRectRslt->CrossPntLT.x , (int)pRectRslt->CrossPntLT.y ,(int)pRectRslt->CrossPntRT.x , (int)pRectRslt->CrossPntRT.y);
        Gdi->Line((int)pRectRslt->CrossPntRT.x , (int)pRectRslt->CrossPntRT.y ,(int)pRectRslt->CrossPntRB.x , (int)pRectRslt->CrossPntRB.y);
        Gdi->Line((int)pRectRslt->CrossPntRB.x , (int)pRectRslt->CrossPntRB.y ,(int)pRectRslt->CrossPntLB.x , (int)pRectRslt->CrossPntLB.y);
        Gdi->Line((int)pRectRslt->CrossPntLB.x , (int)pRectRslt->CrossPntLB.y ,(int)pRectRslt->CrossPntLT.x , (int)pRectRslt->CrossPntLT.y);

        iX = (pRectRslt->CrossPntLT.x + pRectRslt->CrossPntLB.x) / 2.0 ;
        iY = (pRectRslt->CrossPntLT.y + pRectRslt->CrossPntLB.y) / 2.0 ;
        sTemp = sTemp.sprintf("L:%.5f",Rslt.dLengthL[i]) ;
        Gdi->Text(iX - 15 ,iY , sTemp.c_str()) ;

        iX = (pRectRslt->CrossPntLT.x + pRectRslt->CrossPntRT.x) / 2.0 ;
        iY = (pRectRslt->CrossPntLT.y + pRectRslt->CrossPntRT.y) / 2.0 ;
        sTemp = sTemp.sprintf("T:%.5f",Rslt.dLengthT[i]) ;
        Gdi->Text(iX  ,iY - 15, sTemp.c_str()) ;

        iX = (pRectRslt->CrossPntRT.x + pRectRslt->CrossPntRB.x) / 2.0 ;
        iY = (pRectRslt->CrossPntRT.y + pRectRslt->CrossPntRB.y) / 2.0 ;
        sTemp = sTemp.sprintf("R:%.5f",Rslt.dLengthR[i]) ;
        Gdi->Text(iX + 3 ,iY , sTemp.c_str()) ;

        iX = (pRectRslt->CrossPntLB.x + pRectRslt->CrossPntRB.x) / 2.0 ;
        iY = (pRectRslt->CrossPntLB.y + pRectRslt->CrossPntRB.y) / 2.0 ;
        sTemp = sTemp.sprintf("B:%.5f",Rslt.dLengthB[i]) ;
        Gdi->Text(iX  ,iY + 3, sTemp.c_str()) ;

        
        Gdi->LineX((int)pRectRslt->CenterPnt .x ,(int)pRectRslt->CenterPnt.y  , 5) ;
        
        Gdi->LineX((int)pRectRslt->CrossPntLT.x ,(int)pRectRslt->CrossPntLT.y , 5) ;
        Gdi->LineX((int)pRectRslt->CrossPntRT.x ,(int)pRectRslt->CrossPntRT.y , 5) ;
        Gdi->LineX((int)pRectRslt->CrossPntRB.x ,(int)pRectRslt->CrossPntRB.y , 5) ;
        Gdi->LineX((int)pRectRslt->CrossPntLB.x ,(int)pRectRslt->CrossPntLB.y , 5) ;
        
        
        TDPoint tEdgePnt ;
        for(int i = 0 ; i < pRectRslt->lEdgeList.GetDataCnt() ; i++) {
            tEdgePnt = pRectRslt->lEdgeList.GetCrntData(!i);
            Gdi -> Pixel((int)(tEdgePnt.x+0.5),(int)(tEdgePnt.y+0.5),clYellow);
        }
    }

    sTemp = sTemp.sprintf("Offset X:%.5f Y:%.5f",Rslt.dOffsetX , Rslt.dOffsetY) ;
    Gdi->Text((int)Rslt.RectRslt[0].CenterPnt.x + 5 ,(int)Rslt.RectRslt[0].CenterPnt.y + 5 , sTemp.c_str()) ;
    sTemp = sTemp.sprintf("Angle Offset:%.5f",Rslt.dOffsetT) ;
    Gdi->Text((int)Rslt.RectRslt[0].CenterPnt.x + 5 ,(int)Rslt.RectRslt[0].CenterPnt.y + 20 , sTemp.c_str()) ;

    delete Gdi;


}




void CDualRect_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CGdi *Gdi = new CGdi(_hDc , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;

    TRect Rect ;

    int iX , iY ;

    String sTemp ;

    Gdi -> m_tText.Color = clFuchsia ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Rect = Tracker -> GetRect();
        iX = (Rect.left + Rect.right)/2.0 ;
        iY = Rect.bottom + 5 ;
        Tracker -> Paint(_hDc ,  _dScaleX ,  _dScaleY ,  _dOffsetX ,  _dOffsetY);
        if(Tracker -> GetSelected()){
            if(m_fpGetPxResol) sTemp = sTemp.sprintf("Real Legnth W:%.5f H:%.5f",Rect.Width()*m_fpGetPxResol()  , Rect.Height()*m_fpGetPxResol()) ;
            else               sTemp = sTemp.sprintf("Real Legnth W:%.5f H:%.5f",Rect.Width()*m_fpGetPxResol()  , Rect.Height()*m_fpGetPxResol()) ;

            Gdi -> Text(iX,iY,sTemp.c_str()) ;
        }
    }

    delete Gdi;

}



        //이미지 화면 클릭 이벤트
bool CDualRect_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CTracker * Tracker ;                                             //
    bool bRet = false ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;

    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> SetSelected(false);
    }

    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerDw(Button,Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY)){
            bRet = true;

            m_pActiveLPara = m_lLPara.GetData(i);

            break ;
        }
    }
    return bRet;
}


bool CDualRect_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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

void CDualRect_V01::MouseUp(TMouseButton Button,TShiftState Shift )
{
    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> TrackerUp(Button, Shift);
    }
}

        //파라미터 디스플레이
void CDualRect_V01::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
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

void CDualRect_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
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
void CDualRect_V01::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;  //m_pActiveLPara
        if(m_pActiveLPara)UpdateListPara(_bToTable , _vePara , &m_pActiveLPara->List);
        //_vePara -> DisplayOptions >> doColumnTitles ;

    }
    else {
        if(m_pActiveLPara)UpdateListPara(_bToTable , _vePara , &m_pActiveLPara->List);
        UpdateLocalPara(true , _vePara );

    }

}

        //로드 세이브.
void CDualRect_V01::LoadSave(bool _bLoad , String _sVisnName)
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
    CTracker   * Tracker ;
    TLocalPara * pLPara  ;
    if(_bLoad) {
        //이파트는 무조건 1개만 되기 때문에 필요 없다.
        //m_lTrckInsp.DeleteAll();
        if(FileExists(sIniPath)){
            //Tracker ;
            for(int i = 0 ; i < DUAL_RECT_INSP_CNT ; i++) {
                Tracker = m_lTrckInsp.GetCrntData(!i);
                pLPara  = m_lLPara   .GetCrntData(!i);
                Tracker -> LoadSave(_bLoad , sIniPath , "Tracker"+String(i));
                LoadSaveList(_bLoad , sIniPath , "LocalPara"+String(i) , &pLPara->List);
            }

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

        for(int i = 0 ; i < DUAL_RECT_INSP_CNT ; i++) {
            Tracker = m_lTrckInsp.GetCrntData(!i);
            pLPara  = m_lLPara   .GetCrntData(!i);
            Tracker -> LoadSave(_bLoad , sIniPath , "Tracker"+String(i));
            LoadSaveList(_bLoad , sIniPath , "LocalPara"+String(i) , &pLPara->List);
        }

        LoadSaveList(_bLoad , sIniPath , "MasterPara" , &MPara.List);
        LoadSaveList(_bLoad , sIniPath , "CommonPara" , &CPara.List);


    }
}




