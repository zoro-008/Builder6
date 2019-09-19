//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgThreshold.h"
#include "BaseMaths.h"

#include "UserINI.h"
//#include "Global.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


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
    //이 블럭 은 나중에 트렉커 많아지면 같이 증가해야 함..
    TRectTracker * Tracker = new TRectTracker();
    Tracker -> SetFunc(OnTrackerChanged);
    m_lTrckInsp.PushBack(Tracker);
    m_lLPara   .PushBack(new TLocalPara ());

    m_sCrntJobFilePath = "" ;

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


}


        //검사 및 러닝 관련.
bool CThreshold_V01::GetTrainAvailable()
{
    return false ;
}
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
    m_sErrMsg = "";
    Rslt.Clear();  //결과값 클리어.

    bool   bSkip          = CPara.bSkip         .GetBool   () ;
    double dPercent       = CPara.dPercent      .GetDouble () ;
    int    iThresholdHigh = CPara.iThresholdHigh.GetInt    () ;
    int    iThresholdLow  = CPara.iThresholdLow .GetInt    () ;

    if(bSkip   ) {
        //찾았을때 NG 옵션.
        Rslt.bRsltOk = true ;

        Rslt.bInspEnd = true ;

        return true ;

    }



    //벨류 테이블에서 기준값 오프셑 가져옴.
    String aRefOfsXAdd = MPara.aRefOfsXAdd  .GetAddress() ;
    String aRefOfsYAdd = MPara.aRefOfsYAdd  .GetAddress() ;
    double dInspOfsX =0, dInspOfsY=0;
    CValue *  Val ;
    Val = m_pValueList -> GetValueByName(MPara.aRefOfsXAdd.GetAddress());
    if(Val)dInspOfsX = Val -> GetDouble();
    Val = m_pValueList -> GetValueByName(MPara.aRefOfsYAdd.GetAddress());
    if(Val)dInspOfsY = Val -> GetDouble();

    TRect tInspRect ;
    TRectTracker * Tracker = m_lTrckInsp.GetCrntData(true) ;
    tInspRect.left   = Tracker -> Left         + dInspOfsX;
    tInspRect.top    = Tracker -> Top          + dInspOfsY;
    tInspRect.right  = Tracker -> GetRight ()  + dInspOfsX;
    tInspRect.bottom = Tracker -> GetBottom()  + dInspOfsY;

    _pImg -> SetRect(&tInspRect) ;

    int iSum = 0 ;
    int cPx ;
    int iPxCnt = 0 ;
    int iTotalCnt = 0 ;

    int iSumCntrX = 0;
    int iSumCntrY = 0;

    iTotalCnt = tInspRect.Width () * tInspRect.Height() ;
    for(int x = tInspRect.left ; x < tInspRect.right ; x++) {
        for(int y = tInspRect.top ; y < tInspRect.bottom ; y++) {
            cPx = _pImg -> GetPixel(x,y);
            if(iThresholdLow <= cPx && cPx <= iThresholdHigh) {
                iPxCnt ++ ;
            }
            else {
                m_lFailPoint.PushBack(TPoint(x,y));
            }
            iSum += cPx ;
        }
    }

    Rslt.tRect    = tInspRect ;
    Rslt.iPxCnt   = iPxCnt ;
    Rslt.dAverage = iSum / iTotalCnt ;
    Rslt.dPercent = iPxCnt * 100 /(double)iTotalCnt ;

    Rslt.bRsltOk  = Rslt.dPercent >= dPercent ;

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
    if(CPara.bSkip.GetBool()) return ;

    //나중에 Gdi에도 오프셑 적용 하게 해서... 스케일 오프셑 관련 다 걷어내자.......
    CGdi *Gdi = new CGdi(_hDc);
    //Gdi -> SetScale(_dScaleX , _dScaleY);

    Gdi -> m_tText.Color = Rslt.bRsltOk ? clLime : clRed ;
    Gdi -> m_tPen .Color = Rslt.bRsltOk ? clLime : clRed ;

    TRect Rect ;
    Rect.left    = (Rslt.tRect.left - _dOffsetX) * _dScaleX;
    Rect.top     = (Rslt.tRect.top  - _dOffsetY) * _dScaleY;
    Rect.right   = (Rslt.tRect.left - _dOffsetX+ Rslt.tRect.Width() ) * _dScaleX  ;
    Rect.bottom  = (Rslt.tRect.top  - _dOffsetY+ Rslt.tRect.Height()) * _dScaleY  ;

    int    iThresholdHigh = CPara.iThresholdHigh.GetInt    () ;
    int    iThresholdLow  = CPara.iThresholdLow .GetInt    () ;

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
    TPoint ScaledPx ;
    for(int i = 0 ; i < iFailPxCnt ; i++){
        FailPx = m_lFailPoint.GetCrntData(!i);
        ScaledPx.x = (FailPx.x - _dOffsetX) * _dScaleX ;
        ScaledPx.y = (FailPx.y - _dOffsetY) * _dScaleY ;

        Gdi -> Rect(true , ScaledPx.x , ScaledPx.y , ScaledPx.x+2 , ScaledPx.y+2 , 0);
    }

    Gdi -> Rect(false , Rect);

    delete Gdi;
    

}




void CThreshold_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CGdi *Gdi = new CGdi(_hDc);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    TRectTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> Paint(_hDc ,  _dScaleX ,  _dScaleY ,  _dOffsetX ,  _dOffsetY);

    }

    delete Gdi;

}



        //이미지 화면 클릭 이벤트
TTracker * CThreshold_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    TRectTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerDw(Button,Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY))
            return Tracker ;
    }

    return NULL ;
}


TTracker * CThreshold_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    TRectTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        if(Tracker -> TrackerMv(Shift ,x, y , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY))
            return Tracker ;
    }
    return NULL ;
}

void CThreshold_V01::MouseUp(TMouseButton Button,TShiftState Shift )
{
    TRectTracker * Tracker ;
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
        //_vePara -> DisplayOptions >> doColumnTitles ;
        _vePara -> Values[MPara.aRefOfsXAdd .GetName()] = MPara.aRefOfsXAdd .GetAddress();
        _vePara -> Values[MPara.aRefOfsYAdd .GetName()] = MPara.aRefOfsYAdd .GetAddress();
    }
    else {
        MPara.aRefOfsXAdd .SetAddress( _vePara -> Values[MPara.aRefOfsXAdd .GetName()]);
        MPara.aRefOfsYAdd .SetAddress( _vePara -> Values[MPara.aRefOfsYAdd .GetName()]);


        UpdateMasterPara(true , _vePara );
    }
}

void CThreshold_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;
        _vePara -> Values[CPara.bSkip          .GetName()] = (short)CPara.bSkip         .GetBool  ();
        _vePara -> Values[CPara.dPercent       .GetName()] = (short)CPara.dPercent      .GetDouble();
        _vePara -> Values[CPara.iThresholdLow  .GetName()] =        CPara.iThresholdLow .GetInt   ();
        _vePara -> Values[CPara.iThresholdHigh .GetName()] =        CPara.iThresholdHigh.GetInt   ();

    }
    else {
        CPara.bSkip         .SetBoolFromStr   (_vePara -> Values[CPara.bSkip         .GetName()]) ;
        CPara.dPercent      .SetDoubleFromStr (_vePara -> Values[CPara.dPercent      .GetName()]) ;
        CPara.iThresholdLow .SetIntFromStr    (_vePara -> Values[CPara.iThresholdLow .GetName()]) ;
        CPara.iThresholdHigh.SetIntFromStr    (_vePara -> Values[CPara.iThresholdHigh.GetName()]) ;


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
void CThreshold_V01::LoadSave(bool _bLoad , String _sFolderPath , String _sVisnName)
{
    String sIniPath        = _sFolderPath +"\\Vision\\" + _sVisnName + "_" + m_sName + "_" + GetPkgName() + ".ini" ;

    TUserINI Ini ;

    /*
    Tracker -> SetFunc(OnTrackerChanged);
    Tracker -> HeightMax = 100 ;
    Tracker -> WidthMax  = 100 ;
    m_lTrckInsp.PushBack(Tracker);
    m_lLPara   .PushBack(new TLocalPara ());
    */


    int iTrackerCnt ;
    TRectTracker* Tracker ;
    if(_bLoad) {
        //이파트는 무조건 1개만 되기 때문에 필요 없다.
        //m_lTrckInsp.DeleteAll();

        //Tracker ;
        Tracker = m_lTrckInsp.GetData(0);
        Tracker -> OnLoadSave(_bLoad , sIniPath , "Tracker0");

        Ini.Load(sIniPath, "MasterPara" , MPara.aRefOfsXAdd    .GetName(), MPara.aRefOfsXAdd .GetStrPnt());
        Ini.Load(sIniPath, "MasterPara" , MPara.aRefOfsYAdd    .GetName(), MPara.aRefOfsYAdd .GetStrPnt());

        Ini.Load(sIniPath, "CommonPara" , CPara.bSkip          .GetName(), CPara.bSkip          .GetStrPnt());
        Ini.Load(sIniPath, "CommonPara" , CPara.dPercent       .GetName(), CPara.dPercent       .GetStrPnt());
        Ini.Load(sIniPath, "CommonPara" , CPara.iThresholdLow  .GetName(), CPara.iThresholdLow  .GetStrPnt());
        Ini.Load(sIniPath, "CommonPara" , CPara.iThresholdHigh .GetName(), CPara.iThresholdHigh .GetStrPnt());

    }
    else {
        iTrackerCnt = m_lTrckInsp.GetDataCnt() ;

        if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath       ) ;

        //Tracker ;
        Tracker = m_lTrckInsp.GetData(0);
        Tracker -> OnLoadSave(_bLoad , sIniPath , "Tracker0");

        Ini.Save(sIniPath, "MasterPara" , MPara.aRefOfsXAdd .GetName(), MPara.aRefOfsXAdd .GetStrPnt());
        Ini.Save(sIniPath, "MasterPara" , MPara.aRefOfsYAdd .GetName(), MPara.aRefOfsYAdd .GetStrPnt());

        Ini.Save(sIniPath, "CommonPara" , CPara.bSkip          .GetName(), CPara.bSkip          .GetStrPnt());
        Ini.Save(sIniPath, "CommonPara" , CPara.dPercent       .GetName(), CPara.dPercent       .GetStrPnt());
        Ini.Save(sIniPath, "CommonPara" , CPara.iThresholdLow  .GetName(), CPara.iThresholdLow  .GetStrPnt());
        Ini.Save(sIniPath, "CommonPara" , CPara.iThresholdHigh .GetName(), CPara.iThresholdHigh .GetStrPnt());


    }

    m_sCrntJobFilePath = _sFolderPath ;



}




