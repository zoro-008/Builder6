//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgLineCrossPnt.h"
#include "BaseMaths.h"

#include "UserINI.h"
#include "GlobalMan.h"

#include "UnitGdi.h"

//#include "SLogUnit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

REGIST_STATICLLINK_CPP(CLineCrossPnt_V01)
CLineCrossPnt_V01::CLineCrossPnt_V01()
{

}

CLineCrossPnt_V01::~CLineCrossPnt_V01()
{
}

void CLineCrossPnt_V01::OnTrackerChanged()
{
     //화면에서 다시 그리므로 여기서 메세지 날릴 필요 없다.
}


//순수 가상 함수.
//==============================================================================
bool CLineCrossPnt_V01::Init()
{
    Prop.bUseTrain = true  ;
    Prop.bCamera   = false ;

    //이 블럭 은 나중에 트렉커 많아지면 같이 증가해야 함..
    CTracker * Tracker ;
    for(int i = 0 ; i < LINE_CROSSPNT_INSP_CNT ; i++) {
        Tracker = new CTracker();
        if(i!=0){
            Tracker -> LineColor = clYellow ;
        }
        Tracker -> Caption = i==0 ? String("Main") : String("Sub") ;
        Tracker -> SetOnPaint(OnTrackerChanged);
        m_lTrckInsp.PushBack(Tracker);

        m_lLPara   .PushBack(new TLocalPara ());
    }

    m_pActiveLPara = NULL ;

    return true ;

}
bool CLineCrossPnt_V01::Close()
{
    m_pActiveLPara = NULL ;
    
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
bool CLineCrossPnt_V01::Train(CImage * _pImg)
{
    m_sErrMsg = "";
    if(!Run(_pImg)){ //기준값 저장을 위해 한번 돌려놓는다.
        return false ;
    }

    Orig.dPosX  = Rslt.dX ; //저장할 절대 기준값을 뽑아 놓고.
    Orig.dPosY  = Rslt.dY ;
    Orig.dAngle = Rslt.dT ;

    if(!Run(_pImg)){   //실제 오프셑값을 추출 하기 위해 한번 더 돌림.
        return false ;
    }

    return true ;
}

void CLineCrossPnt_V01::RsltClear() //검사 결과값을 검사전에 클리어 한번 하고 한다.
{
    Rslt.Clear();
}

bool CLineCrossPnt_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}



bool CLineCrossPnt_V01::Run(CImage * _pImg)
{
    //Trace("ClossPnt ","stt");
    m_sErrMsg = "";
    Rslt.Clear();  //결과값 클리어.

    if(m_bSkip) {
        Rslt.bRsltOk = true ;
        Rslt.bInspEnd = true ;
        return true ;
    }

    //벨류 테이블에서 기준값 오프셑 가져옴.
    String sRefOfsXAdd = MPara.sRefOfsXAdd ;
    String sRefOfsYAdd = MPara.sRefOfsYAdd ;
    double dInspOfsX =0, dInspOfsY=0;
    CValue2 *  Val ;
    Val = m_pValueList -> GetValueByName(MPara.sRefOfsXAdd);
    if(Val)dInspOfsX = Val -> GetDouble();
    Val = m_pValueList -> GetValueByName(MPara.sRefOfsYAdd);
    if(Val)dInspOfsY = Val -> GetDouble();

    TRect tMainRect ;
    CTracker * Tracker = m_lTrckInsp.GetData(0) ;
    TDRect tTrackerRect = Tracker -> GetDRect() ;
    tMainRect.left   = tTrackerRect.Left   + dInspOfsX ;
    tMainRect.top    = tTrackerRect.top    + dInspOfsY ;
    tMainRect.right  = tTrackerRect.right  + dInspOfsX ;
    tMainRect.bottom = tTrackerRect.bottom + dInspOfsY ;

    TRect tSubRect ;
    Tracker = m_lTrckInsp.GetData(1) ;
    tTrackerRect = Tracker -> GetDRect() ;
    tSubRect.left   = tTrackerRect.Left   + dInspOfsX ;
    tSubRect.top    = tTrackerRect.top    + dInspOfsY ;
    tSubRect.right  = tTrackerRect.right  + dInspOfsX ;
    tSubRect.bottom = tTrackerRect.bottom + dInspOfsY ;

    //Main 
    CLine::TPara tMainLinePara ;
    TLocalPara * pLPara ;
    pLPara = m_lLPara.GetData(0);
    tMainLinePara.iScanDirect        =  pLPara->iLnScanDirection ;
    tMainLinePara.bLtToDk            =  pLPara->bLnLtToDk        ;
    tMainLinePara.iSampleGap         =  pLPara->iLnSampleGap     ;
    tMainLinePara.iLineGap           =  pLPara->iLnLineGap       ;
    tMainLinePara.iEndOffset         =  pLPara->iLnEndOffset     ;
    tMainLinePara.iCntrOffset        =  pLPara->iLnCntrOffset    ;

    tMainLinePara.EdgPara.iAverage   =  pLPara->iEgAverage       ;
    tMainLinePara.EdgPara.iCompare   =  pLPara->iEgCompare       ;

    tMainLinePara.EdgPara.iChkPxLow  =  pLPara->iEgChkPxLow      ;
    tMainLinePara.EdgPara.iChkPxHgh  =  pLPara->iEgChkPxHgh      ;
    tMainLinePara.EdgPara.iOfsGap    =  pLPara->iEgOfsGap        ;
    tMainLinePara.EdgPara.iOfsPxLow  =  pLPara->iEgOfsPxLow      ;
    tMainLinePara.EdgPara.iOfsPxHgh  =  pLPara->iEgOfsPxHgh      ;


    CLine::TPara tSubLinePara ;
    pLPara = m_lLPara.GetData(1);
    tSubLinePara.iScanDirect         =  pLPara->iLnScanDirection ;
    tSubLinePara.bLtToDk             =  pLPara->bLnLtToDk        ;
    tSubLinePara.iSampleGap          =  pLPara->iLnSampleGap     ;
    tSubLinePara.iLineGap            =  pLPara->iLnLineGap       ;
    tSubLinePara.iEndOffset          =  pLPara->iLnEndOffset     ;
    tSubLinePara.iCntrOffset         =  pLPara->iLnCntrOffset    ;

    tSubLinePara.EdgPara.iAverage    =  pLPara->iEgAverage       ;
    tSubLinePara.EdgPara.iCompare    =  pLPara->iEgCompare       ;

    tSubLinePara.EdgPara.iChkPxLow   =  pLPara->iEgChkPxLow      ;
    tSubLinePara.EdgPara.iChkPxHgh   =  pLPara->iEgChkPxHgh      ;
    tSubLinePara.EdgPara.iOfsGap     =  pLPara->iEgOfsGap        ;
    tSubLinePara.EdgPara.iOfsPxLow   =  pLPara->iEgOfsPxLow      ;
    tSubLinePara.EdgPara.iOfsPxHgh   =  pLPara->iEgOfsPxHgh      ;

    //Trace("ClossPnt Main","stt");
    if(!CLine::Inspect(_pImg  , tMainRect , tMainLinePara , &Rslt.tMainRslt , m_fpGetCalPos)) {
        m_sErrMsg = "Main-" + Rslt.tMainRslt.sErrMsg;
        return false ;
    }
    //Trace("ClossPnt Main","end");

    if(!CLine::Inspect(_pImg  , tSubRect , tSubLinePara  , &Rslt.tSubRslt , m_fpGetCalPos)) {
        m_sErrMsg = "Sub-" + Rslt.tSubRslt.sErrMsg ;
        return false ;
    }
    //Trace("ClossPnt Sub","end");

    /*
    double dd1 = 565  ;
    double dd2 = 305  ;
    double dd3 = 1273 ;
    double dd4 = 553  ;

    //double s;
    double ddTemp1 ;
    ddTemp1  = (dd1-dd2);
    ddTemp1 *= (dd3-dd4) ;
    */




    TDPoint tRsltPnt ;
    //Trace("CMath::GetLineCrossPnt","stt");
    if(!CMath::GetLineCrossPnt(Rslt.tMainRslt.StartPnt , Rslt.tMainRslt.EndPnt , Rslt.tSubRslt.StartPnt , Rslt.tSubRslt.EndPnt , tRsltPnt)){
        m_sErrMsg = "Lines are parallel";
        //Trace("CMath::GetLineCrossPnt","Lines are parallel");
        return false ;
    }

    //Trace("CMath::GetLineCrossPnt","stt");


    double dRotOffsetX ;
    double dRotOffsetY ;
    CMath::GetRotPntFromCntrPnt(CPara.dOffsetX , CPara.dOffsetY , Rslt.tMainRslt.dAngle - Orig.dAngle ,  //입력 x , 입력 y , 돌릴각도. , +가 반시계방향.
                                 0 , 0 ,                                       //돌리는 기준점 x , y
                                 &dRotOffsetX , &dRotOffsetY );                //결과값 x,y




    Rslt.dX = tRsltPnt.x +dRotOffsetX;
    Rslt.dY = tRsltPnt.y +dRotOffsetY;
    Rslt.dT = Rslt.tMainRslt.dAngle ;






    Rslt.dOffsetX = Rslt.dX - Orig.dPosX  ;
    Rslt.dOffsetY = Rslt.dY - Orig.dPosY  ;
    Rslt.dOffsetT = Rslt.dT - Orig.dAngle ;

    Rslt.dOffsetX = ((int)(Rslt.dOffsetX * 100000)) / 100000.0 ; //소수5자리만 표현.
    Rslt.dOffsetY = ((int)(Rslt.dOffsetY * 100000)) / 100000.0 ; //소수5자리만 표현.
    Rslt.dOffsetT = ((int)(Rslt.dOffsetT * 100000)) / 100000.0 ; //소수5자리만 표현.

    Rslt.dX = ((int)(Rslt.dX * 100000)) / 100000.0 ; //소수5자리만 표현.
    Rslt.dY = ((int)(Rslt.dY * 100000)) / 100000.0 ; //소수5자리만 표현.

    Val = m_pValueList -> GetValueByName(MPara.sOutOfsXAdd);
    if(Val) Val -> Set(Rslt.dOffsetX);
    Val = m_pValueList -> GetValueByName(MPara.sOutOfsYAdd);
    if(Val) Val -> Set(Rslt.dOffsetY);
    Val = m_pValueList -> GetValueByName(MPara.sOutOfsTAdd);
    if(Val) Val -> Set(Rslt.dOffsetT);
    Val = m_pValueList -> GetValueByName(MPara.sOutAbsXAdd);
    if(Val) Val -> Set(Rslt.dX);
    Val = m_pValueList -> GetValueByName(MPara.sOutAbsYAdd);
    if(Val) Val -> Set(Rslt.dY);

    Rslt.bRsltOk  = true ;
    Rslt.bInspEnd = true ;
    //Trace("ClossPnt End","End");

    return true ;

}

String CLineCrossPnt_V01::GetErrMsg()
{
    return m_sErrMsg ;
}


        //화면 출력.
void CLineCrossPnt_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
//    여기부터 하자.


    if(!Rslt.bInspEnd) return ;
    if(m_bSkip) return ;

//Trace("PaintRslt","Stt");

    CGdi *Gdi = new CGdi(_hDc,_dScaleX , _dScaleY , _dOffsetX , _dOffsetY);


    AnsiString sTemp ;

    Gdi->m_tPen.Color   = Rslt.bRsltOk ? clLime : clRed ;
    Gdi->m_tText.Color  = Rslt.bRsltOk ? clLime : clRed ;
    Gdi->m_tPen.Style   = PS_SOLID ;

    Gdi->LineX((int)Rslt.dX ,(int)Rslt.dY , 5) ;

    sTemp = sTemp.sprintf("X:%.5f Y:%.5f",Rslt.dX ,Rslt.dY) ;
    Gdi->Text((int)Rslt.dX + 5,(int)Rslt.dY + 5  , sTemp.c_str()) ;
    sTemp = sTemp.sprintf("Angle:%.5f",Rslt.dT) ;
    Gdi->Text((int)Rslt.dX + 5,(int)Rslt.dY + 20 , sTemp.c_str()) ;

    TDPoint tEdgePnt;
//Trace("tMainRslt","Stt");
    for(int i = 0 ; i < Rslt.tMainRslt.EdgList.GetDataCnt() ; i++) {
        tEdgePnt = Rslt.tMainRslt.EdgList.GetCrntData(!i);
        Gdi -> Pixel((int)(tEdgePnt.x+0.5),(int)(tEdgePnt.y+0.5),clYellow);
    }
//Trace("tMainRslt","End");
//Trace("tSubRslt","Stt");
    for(int i = 0 ; i < Rslt.tSubRslt.EdgList.GetDataCnt() ; i++) {
        tEdgePnt = Rslt.tSubRslt.EdgList.GetCrntData(!i);
        Gdi -> Pixel((int)(tEdgePnt.x+0.5),(int)(tEdgePnt.y+0.5),clYellow);
    }
//Trace("tSubRslt","End");
    if(!Rslt.bRsltOk){
        Gdi -> m_tText.Color = clRed ;
        Gdi -> m_tFont  .Height = 30       ;
        Gdi -> Text(5, 5 ,m_sErrMsg.c_str()) ;
    }


    delete Gdi;

//Trace("PaintRslt","End");


}




void CLineCrossPnt_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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
bool CLineCrossPnt_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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


bool CLineCrossPnt_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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

void CLineCrossPnt_V01::MouseUp(TMouseButton Button,TShiftState Shift )
{
    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> TrackerUp(Button, Shift);
    }
}

        //파라미터 디스플레이
void CLineCrossPnt_V01::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
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

void CLineCrossPnt_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
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
void CLineCrossPnt_V01::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
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
void CLineCrossPnt_V01::LoadSave(bool _bLoad , String _sVisnName)
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
            for(int i = 0 ; i < LINE_CROSSPNT_INSP_CNT ; i++) {
                Tracker = m_lTrckInsp.GetCrntData(!i);
                pLPara  = m_lLPara   .GetCrntData(!i);
                Tracker -> LoadSave(_bLoad , sIniPath , "Tracker"+String(i));
                LoadSaveList(_bLoad , sIniPath , "LocalPara"+String(i) , &pLPara->List);
            }

            LoadSaveList(_bLoad , sIniPath , "MasterPara" , &MPara.List);
            LoadSaveList(_bLoad , sIniPath , "CommonPara" , &CPara.List);

            Ini.Load(sIniPath, "TrainRslt"  , "dPosX " , Orig.dPosX  );
            Ini.Load(sIniPath, "TrainRslt"  , "dPosY " , Orig.dPosY  );
            Ini.Load(sIniPath, "TrainRslt"  , "dAngle" , Orig.dAngle );
        }
        else {
            MPara.SetDefault();
            CPara.SetDefault();
        }

    }
    else {
        iTrackerCnt = m_lTrckInsp.GetDataCnt() ;

        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath       ) ;

        for(int i = 0 ; i < LINE_CROSSPNT_INSP_CNT ; i++) {
            Tracker = m_lTrckInsp.GetCrntData(!i);
            pLPara  = m_lLPara   .GetCrntData(!i);
            Tracker -> LoadSave(_bLoad , sIniPath , "Tracker"+String(i));
            LoadSaveList(_bLoad , sIniPath , "LocalPara"+String(i) , &pLPara->List);
        }

        LoadSaveList(_bLoad , sIniPath , "MasterPara" , &MPara.List);
        LoadSaveList(_bLoad , sIniPath , "CommonPara" , &CPara.List);

        Ini.Save(sIniPath, "TrainRslt"  , "dPosX " , Orig.dPosX  );
        Ini.Save(sIniPath, "TrainRslt"  , "dPosY " , Orig.dPosY  );
        Ini.Save(sIniPath, "TrainRslt"  , "dAngle" , Orig.dAngle );

    }
}

