//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgLineCrossPnt.h"
#include "BaseMaths.h"

#include "UserINI.h"
#include "GlobalMan.h"

#include "UnitGdi.h"

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
     //ȭ�鿡�� �ٽ� �׸��Ƿ� ���⼭ �޼��� ���� �ʿ� ����.
}


//���� ���� �Լ�.
//==============================================================================
bool CLineCrossPnt_V01::Init()
{
    Prop.bUseTrain = true  ;
    Prop.bCamera   = false ;

    //�� �� �� ���߿� Ʈ��Ŀ �������� ���� �����ؾ� ��..
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


        //�˻� �� ���� ����.
bool CLineCrossPnt_V01::Train(CImage * _pImg)
{
    m_sErrMsg = "";

    return true ;
}

void CLineCrossPnt_V01::RsltClear() //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
{
    Rslt.Clear();
}

bool CLineCrossPnt_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}



bool CLineCrossPnt_V01::Run(CImage * _pImg)
{
    if(m_bSkip) {
        Rslt.bRsltOk = true ;
        return true ;
    }
    
    m_sErrMsg = "";
    Rslt.Clear();  //����� Ŭ����.


    //���� ���̺��� ���ذ� �����V ������.
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
    tMainRect.left   = tTrackerRect.Left   - Tracker -> RoiWidth + dInspOfsX ;
    tMainRect.top    = tTrackerRect.top    - Tracker -> RoiWidth + dInspOfsY ;
    tMainRect.right  = tTrackerRect.right  + Tracker -> RoiWidth + dInspOfsX ;
    tMainRect.bottom = tTrackerRect.bottom + Tracker -> RoiWidth + dInspOfsY ;

    TRect tSubRect ;
    Tracker = m_lTrckInsp.GetData(1) ;
    tTrackerRect = Tracker -> GetDRect() ;
    tSubRect.left   = tTrackerRect.Left   - Tracker -> RoiWidth + dInspOfsX ;
    tSubRect.top    = tTrackerRect.top    - Tracker -> RoiWidth + dInspOfsY ;
    tSubRect.right  = tTrackerRect.right  + Tracker -> RoiWidth + dInspOfsX ;
    tSubRect.bottom = tTrackerRect.bottom + Tracker -> RoiWidth + dInspOfsY ;

    //Main 
    CLine::TPara tMainLinePara ;
    TLocalPara * pLPara ;
    pLPara = m_lLPara.GetData(0);
    tMainLinePara.iScanDirect =  pLPara->iLnScanDirection ;
    tMainLinePara.bLtToDk     =  pLPara->bLnLtToDk        ;
    tMainLinePara.iSampleGap  =  pLPara->iLnSampleGap     ;
    tMainLinePara.iLineGap    =  pLPara->iLnLineGap       ;
    tMainLinePara.iEndOffset  =  pLPara->iLnEndOffset     ;
    tMainLinePara.iCntrOffset =  pLPara->iLnCntrOffset    ;

    CLine::TPara tSubLinePara ;
    pLPara = m_lLPara.GetData(1);
    tSubLinePara.iScanDirect =  pLPara->iLnScanDirection ;
    tSubLinePara.bLtToDk     =  pLPara->bLnLtToDk        ;
    tSubLinePara.iSampleGap  =  pLPara->iLnSampleGap     ;
    tSubLinePara.iLineGap    =  pLPara->iLnLineGap       ;
    tSubLinePara.iEndOffset  =  pLPara->iLnEndOffset     ;
    tSubLinePara.iCntrOffset =  pLPara->iLnCntrOffset    ;


    if(!CLine::Inspect(_pImg  , tMainRect , tMainLinePara , &Rslt.tMainRslt )) {
        m_sErrMsg = "Main-" + Rslt.tMainRslt.sErrMsg;
        return false ;
    }

    if(!CLine::Inspect(_pImg  , tSubRect , tSubLinePara  , &Rslt.tSubRslt )) {
        m_sErrMsg = "Sub-" + Rslt.tSubRslt.sErrMsg ;
        return false ;
    }

    TDPoint tRsltPnt ;
    if(!MATH_GetLineCrossPnt(Rslt.tMainRslt.StartPnt , Rslt.tMainRslt.EndPnt , Rslt.tSubRslt.StartPnt , Rslt.tSubRslt.EndPnt , tRsltPnt)){
        m_sErrMsg = "Lines are parallel";
        return false ;
    }

    Rslt.dX = tRsltPnt.x ;
    Rslt.dY = tRsltPnt.y ;
    Rslt.dT = Rslt.tMainRslt.dAngle ;


    Rslt.dOffsetX = Rslt.dX - Orig.dPosX  ;
    Rslt.dOffsetY = Rslt.dY - Orig.dPosY  ;
    Rslt.dOffsetT = Rslt.dT - Orig.dAngle ;

    Rslt.dOffsetX = ((int)(Rslt.dOffsetX * 100000)) / 100000.0 ; //�Ҽ�5�ڸ��� ǥ��.
    Rslt.dOffsetY = ((int)(Rslt.dOffsetY * 100000)) / 100000.0 ; //�Ҽ�5�ڸ��� ǥ��.
    Rslt.dOffsetT = ((int)(Rslt.dOffsetT * 100000)) / 100000.0 ; //�Ҽ�5�ڸ��� ǥ��.

    Val = m_pValueList -> GetValueByName(MPara.sOutOfsXAdd);
    if(Val) Val -> Set(Rslt.dOffsetX);
    Val = m_pValueList -> GetValueByName(MPara.sOutOfsYAdd);
    if(Val) Val -> Set(Rslt.dOffsetY);
    Val = m_pValueList -> GetValueByName(MPara.sOutAngleAdd);
    if(Val) Val -> Set(Rslt.dOffsetT);

    Rslt.bRsltOk  = true ;
    Rslt.bInspEnd = true ;

    return true ;

}

String CLineCrossPnt_V01::GetErrMsg()
{
    return m_sErrMsg ;
}


        //ȭ�� ���.
void CLineCrossPnt_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
//    ������� ����.
    if(!Rslt.bInspEnd) return ;
    if(m_bSkip) return ;

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
    for(int i = 0 ; i < Rslt.tMainRslt.EdgList.GetDataCnt() ; i++) {
        tEdgePnt = Rslt.tMainRslt.EdgList.GetCrntData(!i);
        Gdi -> Pixel((int)(tEdgePnt.x+0.5),(int)(tEdgePnt.y+0.5),clYellow);
    }
    for(int i = 0 ; i < Rslt.tSubRslt.EdgList.GetDataCnt() ; i++) {
        tEdgePnt = Rslt.tSubRslt.EdgList.GetCrntData(!i);
        Gdi -> Pixel((int)(tEdgePnt.x+0.5),(int)(tEdgePnt.y+0.5),clYellow);
    }


    delete Gdi;
    

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



        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
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

        //�Ķ���� ���÷���
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

        //�ε� ���̺�.
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
        //����Ʈ�� ������ 1���� �Ǳ� ������ �ʿ� ����.
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
        }
        else {
            MPara.SetDefualt();
            CPara.SetDefualt();
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


    }
}

