//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgThreshold.h"
#include "BaseMaths.h"

#include "UserINI.h"
#include "GlobalMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

REGIST_STATICLLINK_CPP(CThreshold_V01)
CThreshold_V01::CThreshold_V01()
{

}

CThreshold_V01::~CThreshold_V01()
{
}

void CThreshold_V01::OnTrackerChanged()
{
     //화면에서 다시 그리므로 여기서 메세지 날릴 필요 없다.
}


//순수 가상 함수.
//==============================================================================
bool CThreshold_V01::Init()
{
    Prop.bUseTrain = false ;
    Prop.bCamera   = false ;

    //이 블럭 은 나중에 트렉커 많아지면 같이 증가해야 함..
    CTracker * Tracker = new CTracker();
    Tracker -> SetOnPaint(OnTrackerChanged);
    m_lTrckInsp.PushBack(Tracker);
    m_lLPara   .PushBack(new TLocalPara ());

    return true ;

}
bool CThreshold_V01::Close()
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

    m_lFailPoint.DeleteAll();

    return true ;
}


        //검사 및 러닝 관련.
bool CThreshold_V01::Train(CImage * _pImg)
{
    m_sErrMsg = "";

    return true ;
}

void CThreshold_V01::RsltClear() //검사 결과값을 검사전에 클리어 한번 하고 한다.
{
    Rslt.Clear();
    m_lFailPoint.DeleteAll();
}

bool CThreshold_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}



bool CThreshold_V01::Run(CImage * _pImg)
{
    if(m_bSkip) {
        Rslt.bRsltOk = true ;
        Rslt.bInspEnd = true ;
        return true ;
    }

    m_sErrMsg = "";
    Rslt.Clear();  //결과값 클리어.



    //벨류 테이블에서 기준값 오프셑 가져옴.
    String sRefOfsXAdd = MPara.sRefOfsXAdd ;
    String sRefOfsYAdd = MPara.sRefOfsYAdd ;

    double dInspOfsX , dInspOfsY ;

    CValue2 *  Val ;
    Val = m_pValueList -> GetValueByName(MPara.sRefOfsXAdd);
    if(Val)dInspOfsX = Val -> GetDouble();
    Val = m_pValueList -> GetValueByName(MPara.sRefOfsYAdd);
    if(Val)dInspOfsY = Val -> GetDouble();

    TRect tInspRect ;
    CTracker * Tracker = m_lTrckInsp.GetCrntData(true) ;
    tInspRect = Tracker -> GetRect() ;
    tInspRect.left   += dInspOfsX ;
    tInspRect.top    += dInspOfsY ;
    tInspRect.right  += dInspOfsX ;
    tInspRect.bottom += dInspOfsY ;

    _pImg -> SetRect(&tInspRect) ;

    int iSum = 0 ;
    int cPx ;
    int iPxCnt = 0 ;
    int iTotalCnt = 0 ;

    iTotalCnt = tInspRect.Width () * tInspRect.Height() ;
    for(int x = tInspRect.left ; x < tInspRect.right ; x++) {
        for(int y = tInspRect.top ; y < tInspRect.bottom ; y++) {
            cPx = _pImg -> GetPixel(x,y);
            if(CPara.iThresholdLow <= cPx && cPx <= CPara.iThresholdHigh) {
                iPxCnt ++ ;
            }
            else {
                m_lFailPoint.PushBack(TPoint(x,y));
            }
            iSum += cPx ;
        }
    }


    //무게 중심.
    int iPxCntY =0;
    int iPxSumY =0;
    for (int f = tInspRect.top ; f < tInspRect.bottom ; f ++) {
        for (int g = tInspRect.left ; g < tInspRect.right ; g ++) {
            cPx = _pImg->GetPixel(g,f) ;
            if(CPara.iThresholdLow <= cPx && cPx <= CPara.iThresholdHigh) {
                cPx = 255 - cPx ;
                iPxCntY += cPx ;
                iPxSumY += cPx * f ;

            }
        }
    }
    int iPxCntX = 0 ;
    int iPxSumX = 0 ;
    for (int g = tInspRect.left ; g < tInspRect.right ; g ++) {
        for (int f = tInspRect.top ; f < tInspRect.bottom ; f ++) {
            cPx = _pImg->GetPixel(g,f) ;
            if(CPara.iThresholdLow <= cPx && cPx <= CPara.iThresholdHigh) {
                cPx = 255 - cPx ;
                iPxCntX += cPx ;
                iPxSumX += cPx * g ;

            }
        }
    }

    //무게중심 찾기.
    if(iPxCntX)Rslt. dCentroidX = iPxSumX / (double)iPxCntX ;
    if(iPxCntY)Rslt. dCentroidY = iPxSumY / (double)iPxCntY ;


    double dTempPntX ;
    double dTempPntY ;
    double dCntPntX = _pImg->GetWidth ()/2.0 ;
    double dCntPntY = _pImg->GetHeight()/2.0 ;
    if(m_fpGetCalPos){
        m_fpGetCalPos(Rslt.dCentroidX , Rslt.dCentroidY , &dTempPntX ,&dTempPntY );
        Rslt.dCentroidX = dTempPntX ;
        Rslt.dCentroidY = dTempPntY ;
        m_fpGetCalPos(_pImg->GetWidth()/2.0 , _pImg->GetHeight()/2.0  , &dCntPntX ,&dCntPntY ); //원래 캘영상중심이 기준이기때문에 사실 필요 없다.
    }

    Rslt.dCentroidOfsCntX = Rslt.dCentroidX - dCntPntX ;
    Rslt.dCentroidOfsCntY = Rslt.dCentroidY - dCntPntY ;
    if(m_fpGetPxResol){
        Rslt.dCentroidOfsCntX *= m_fpGetPxResol() ;
        Rslt.dCentroidOfsCntY *= m_fpGetPxResol() ;
    }


    if(MPara.bOutOfsXInverse) Rslt.dCentroidOfsCntX *=-1 ;
    if(MPara.bOutOfsYInverse) Rslt.dCentroidOfsCntY *=-1 ;


    Val = m_pValueList -> GetValueByName(MPara.sOutCntrdXAdd);
    if(Val)Val -> Set(Rslt. dCentroidX);
    Val = m_pValueList -> GetValueByName(MPara.sOutCntrdYAdd);
    if(Val)Val -> Set(Rslt. dCentroidY);

    Val = m_pValueList -> GetValueByName(MPara.sOutCntrdCntOfsXAdd);
    if(Val)Val -> Set(Rslt. dCentroidOfsCntX);
    Val = m_pValueList -> GetValueByName(MPara.sOutCntrdCntOfsYAdd);
    if(Val)Val -> Set(Rslt. dCentroidOfsCntY);






    Rslt.tRect    = tInspRect ;
    Rslt.iPxCnt   = iPxCnt ;
    Rslt.dAverage = iSum / iTotalCnt ;
    Rslt.dPercent = iPxCnt * 100 /(double)iTotalCnt ;

    Rslt.bRsltOk  = Rslt.dPercent >= CPara.dPercent ;

    Val = m_pValueList -> GetValueByName(MPara.sOutRsltAdd);
    if(Val)Val -> Set(Rslt.bRsltOk ? String("OK"):String("NG"));


    //찾았을때 NG 옵션.
    Rslt.bInspEnd = true ;

    return true ;

}

String CThreshold_V01::GetErrMsg()
{
    return m_sErrMsg ;
}


        //화면 출력.
void CThreshold_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if(!Rslt.bInspEnd) return ;
    if(m_bSkip) return ;

    CGdi *Gdi = new CGdi(_hDc,_dScaleX , _dScaleY , _dOffsetX , _dOffsetY);

    Gdi -> m_tText  .Color = Rslt.bRsltOk ? clLime : clRed ;
    Gdi -> m_tPen   .Color = Rslt.bRsltOk ? clLime : clRed ;
    Gdi -> m_tBrush .Color = Rslt.bRsltOk ? clLime : clRed ;

    TRect Rect ;
    Rect.left    = Rslt.tRect.left   ;//(Rslt.tRect.left - _dOffsetX) * _dScaleX;
    Rect.top     = Rslt.tRect.top    ;//(Rslt.tRect.top  - _dOffsetY) * _dScaleY;
    Rect.right   = Rslt.tRect.right  ;//(Rslt.tRect.left - _dOffsetX+ Rslt.tRect.Width() ) * _dScaleX  ;
    Rect.bottom  = Rslt.tRect.bottom ;//(Rslt.tRect.top  - _dOffsetY+ Rslt.tRect.Height()) * _dScaleY  ;

    String sTemp1 ;
    String sTemp2 ;
    String sTemp3 ;
    sTemp1 = String("Px Cnt : ") + Rslt.iPxCnt ;
    sTemp2 = sTemp2.sprintf("Average : %.2f" , Rslt.dAverage) ;
    sTemp3 = sTemp3.sprintf("Percent : %.2f" , Rslt.dPercent) ;

    Gdi -> Text(Rect.left + 5, Rect.top - 65 ,sTemp1.c_str()) ;
    Gdi -> Text(Rect.left + 5, Rect.top - 45 ,sTemp2.c_str()) ;
    Gdi -> Text(Rect.left + 5, Rect.top - 25 ,sTemp3.c_str()) ;

    int iFailPxCnt = m_lFailPoint.GetDataCnt() ;
    TPoint FailPx ;
    for(int i = 0 ; i < iFailPxCnt ; i++){
        FailPx = m_lFailPoint.GetCrntData(!i);
        Gdi -> Rect(true , FailPx.x-1 , FailPx.y-1 , FailPx.x+1 , FailPx.y+1 , 0);
    }

    Gdi -> Rect(false , Rect);

    if(!Rslt.bRsltOk){
        Gdi -> m_tText.Color = clRed ;
        Gdi -> m_tFont  .Height = 30       ;
        Gdi -> Text(5, 5 ,m_sErrMsg.c_str()) ;
    }

    delete Gdi;
    

}




void CThreshold_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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
bool CThreshold_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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


bool CThreshold_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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

void CThreshold_V01::MouseUp(TMouseButton Button,TShiftState Shift )
{
    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> TrackerUp(Button, Shift);
    }
}

        //파라미터 디스플레이
void CThreshold_V01::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
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

void CThreshold_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
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
void CThreshold_V01::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
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
void CThreshold_V01::LoadSave(bool _bLoad , String _sVisnName)
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




