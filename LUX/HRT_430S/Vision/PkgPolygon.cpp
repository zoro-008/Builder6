//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgPolygon.h"
#include "BaseMaths.h"
#include "UnitArea.h"

#include "UserINI.h"
#include "GlobalMan.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

REGIST_STATICLLINK_CPP(CPolygon_V01)
CPolygon_V01::CPolygon_V01()
{

}

CPolygon_V01::~CPolygon_V01()
{
}

void CPolygon_V01::OnTrackerChanged()
{
     //ȭ�鿡�� �ٽ� �׸��Ƿ� ���⼭ �޼��� ���� �ʿ� ����.
}


//���� ���� �Լ�.
//==============================================================================
bool CPolygon_V01::Init()
{
    Prop.bUseTrain = false ;
    Prop.bCamera   = false ;

    //�� �� �� ���߿� Ʈ��Ŀ �������� ���� �����ؾ� ��..
    CTracker * Tracker     = new CTracker();
    Tracker -> TrackerType = CTracker::ttPolygon ;
    Tracker -> Caption     = "";
    Tracker -> BandEnabled = true ;
    Tracker -> BandWidth   = 50 ;
    Tracker -> SetOnPaint(OnTrackerChanged);
    m_lTrckInsp.PushBack(Tracker);

    m_lLPara   .PushBack(new TLocalPara ());

    return true ;

}
bool CPolygon_V01::Close()
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
bool CPolygon_V01::Train(CImage * _pImg)
{
    m_sErrMsg = "";

    return true ;
}

void CPolygon_V01::RsltClear() //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
{
    Rslt.Clear();
}

bool CPolygon_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}



bool CPolygon_V01::Run(CImage * _pImg)
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



    CLine::TPara LinePara ;
    //LinePara.iScanDirect = lsUpToDn;  //�̰� �ǹ� ���� �����ִ� 4����Ʈ���� FrstStt , FrstEnd ������ ������ �̿�ȴ�.
    LinePara.bLtToDk     =!CPara.bLtOnDk;        //���� ��ü ���� �� DkToLt��.
    LinePara.iSampleGap  = MPara.iLnSampleGap  ;
    LinePara.iLineGap    = MPara.iLnLineGap    ;
    LinePara.iEndOffset  = MPara.iLnEndOffset  ;
    LinePara.iCntrOffset = MPara.iLnCntrOffset ;

    LinePara.EdgPara.iAverage  = MPara.iEgAverage    ;
    LinePara.EdgPara.iCompare  = MPara.iEgCompare    ;

    LinePara.EdgPara.iChkPxLow = MPara.iEgChkPxLow   ;
    LinePara.EdgPara.iChkPxHgh = MPara.iEgChkPxHgh   ;
    LinePara.EdgPara.iOfsGap   = MPara.iEgOfsGap     ;
    LinePara.EdgPara.iOfsPxLow = MPara.iEgOfsPxLow   ;
    LinePara.EdgPara.iOfsPxHgh = MPara.iEgOfsPxHgh   ;


    CTracker * Tracker     = m_lTrckInsp.GetData(0) ;
    TDPoint  * pTrackerPnt = Tracker -> GetPoints() ;
    TDPoint tFrstStt , tFrstEnd ;
    TDPoint tLastStt , tLastEnd ;

    TDPoint      tSttPnt , tEndPnt ;
    double       dAngle ;
    CLine        Line ;
    CLine::TRslt LineRslt ;
    const int    iTrackerPointCnt  = Tracker -> GetPointCount() ;
    TDPoint   *  aLineSttPnt = new TDPoint[iTrackerPointCnt];
    TDPoint   *  aLineEndPnt = new TDPoint[iTrackerPointCnt];





    for(int i = 0 ; i < iTrackerPointCnt ; i++){

        tSttPnt.x = pTrackerPnt[i].x + dInspOfsX;
        tSttPnt.y = pTrackerPnt[i].y + dInspOfsY;

        tEndPnt.x = pTrackerPnt[(i+1)%iTrackerPointCnt].x + dInspOfsX;
        tEndPnt.y = pTrackerPnt[(i+1)%iTrackerPointCnt].y + dInspOfsY;

        dAngle = CMath::GetLineAngle(tSttPnt.x , tSttPnt.y , tEndPnt.x , tEndPnt.y) ;


        CMath::GetPntFromPntByAngLen(  tSttPnt.x , tSttPnt.y   ,   //�Է� ������.
                                     dAngle + 90             ,   //�Է� ����.
                                     Tracker -> BandWidth    ,   //�Է� ����.
                                    &tFrstStt.x , &tFrstStt.y ); //��� ����Ʈ.

        CMath::GetPntFromPntByAngLen(  tSttPnt.x , tSttPnt.y   ,   //�Է� ������.
                                     dAngle - 90             ,   //�Է� ����.
                                     Tracker -> BandWidth    ,   //�Է� ����.
                                    &tFrstEnd.x , &tFrstEnd.y ); //��� ����Ʈ.

        CMath::GetPntFromPntByAngLen(  tEndPnt.x , tEndPnt.y   ,   //�Է� ������.
                                     dAngle + 90             ,   //�Է� ����.
                                     Tracker -> BandWidth    ,   //�Է� ����.
                                    &tLastStt.x , &tLastStt.y ); //��� ����Ʈ.

        CMath::GetPntFromPntByAngLen(  tEndPnt.x , tEndPnt.y   ,   //�Է� ������.
                                     dAngle - 90             ,   //�Է� ����.
                                     Tracker -> BandWidth    ,   //�Է� ����.
                                    &tLastEnd.x , &tLastEnd.y ); //��� ����Ʈ.


        if(!Line.Inspect(_pImg  , tFrstStt , tFrstEnd , tLastStt , tLastEnd , LinePara , &LineRslt)) {
            m_sErrMsg = LineRslt.sErrMsg;
            delete [] aLineSttPnt ;
            delete [] aLineEndPnt ;

            return false ;
        }

        for(int i = 0 ; i < LineRslt.EdgList.GetDataCnt() ;i++){
            Rslt.lEdgeList.PushBack(LineRslt.EdgList.GetCrntData(!i));
        }
        
        aLineSttPnt[i] = LineRslt.StartPnt ;
        aLineEndPnt[i] = LineRslt.EndPnt   ;
    }

    //��� �ڳ� ����Ʈ �Ҵ�. delete �� �̹� Rslt.Clear���� �س���.
    Rslt.iConnerCnt = iTrackerPointCnt ;
    Rslt.aConners   = new TDPoint[iTrackerPointCnt] ;
    for(int i = 0 ; i < iTrackerPointCnt ; i++){
        if(!CMath::GetLineCrossPnt(aLineSttPnt[(iTrackerPointCnt + i - 1)%iTrackerPointCnt] ,
                                 aLineEndPnt[(iTrackerPointCnt + i - 1)%iTrackerPointCnt] ,
                                 aLineSttPnt[(                   i    )%iTrackerPointCnt] ,
                                 aLineEndPnt[(                   i    )%iTrackerPointCnt] ,
                                 Rslt.aConners[i])){
            m_sErrMsg = "Lines are never make cross pnt";
            delete [] aLineSttPnt ;
            delete [] aLineEndPnt ;

            return false ;
        }
    }

    delete [] aLineSttPnt ;
    delete [] aLineEndPnt ;

    //aCrossPnt[i] LineRslt


    //Result Area.
    CValue2 * ValOutArea = m_pValueList -> GetValueByName(MPara.sOutAreaAdd);

    const unsigned char cPolyIn = 1 ;
    if(ValOutArea){
        CArea * pOutArea = (CArea *)ValOutArea -> GetValAdd() ;
        pOutArea -> SetSize(_pImg -> GetWidth() , _pImg -> GetHeight());
        pOutArea -> AddPolygon(Rslt.aConners , Rslt.iConnerCnt , cPolyIn);
        //pOutArea -> SaveBmp("D:\\sunsun.bmp");
    }

    //Rslt.dGapFromPntX = dOriX - Rslt.RectRslt.CenterPnt.x ;
    //Rslt.dGapFromPntY = dOriY - Rslt.RectRslt.CenterPnt.y ;
    //
    //Rslt.dGapFromPnt = CMath::GetLineLenth(dOriX,dOriY,Rslt.RectRslt.CenterPnt.x,Rslt.RectRslt.CenterPnt.y);

    Rslt.bRsltOk  = true ;

    //ã������ NG �ɼ�.
    Rslt.bInspEnd = true ;

    return true ;

}

String CPolygon_V01::GetErrMsg()
{
    return m_sErrMsg ;
}


        //ȭ�� ���.
void CPolygon_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if(!Rslt.bInspEnd) return ;
    if(m_bSkip) return ;

    CGdi *Gdi = new CGdi(_hDc,_dScaleX , _dScaleY , _dOffsetX , _dOffsetY);


    AnsiString sTemp ;

    Gdi->m_tPen.Color = Rslt.bRsltOk ? clLime : clRed ;
    Gdi->m_tText.Color = Rslt.bRsltOk ? clLime : clRed ;
    Gdi->m_tPen.Style = PS_SOLID ;

    //Result Area.
    CValue2 * ValOutArea = m_pValueList -> GetValueByName(MPara.sOutAreaAdd);
    if(ValOutArea){
        CArea * pOutArea = (CArea *)ValOutArea -> GetValAdd() ;
        pOutArea -> ShowArea( _hDc , _dScaleX , _dScaleY , _dOffsetX , _dOffsetY , 0, 0);
        //pOutArea
        //pOutArea -> SaveBmp("D:\\sunsun.bmp");
    }

    int iX ;
    int iY ;

    TDPoint tPreConerPnt , tConerPnt;
    tPreConerPnt = Rslt.aConners[Rslt.iConnerCnt-1];
    double dLength ;
    for(int i = 0 ; i < Rslt.iConnerCnt ; i++) {
        tConerPnt = Rslt.aConners[i];
        Gdi->Line((int)tPreConerPnt.x , (int)tPreConerPnt.y ,(int)tConerPnt.x , (int)tConerPnt.y);

        iX = (tPreConerPnt.x + tConerPnt.x) / 2.0 ;
        iY = (tPreConerPnt.y + tConerPnt.y) / 2.0 ;
        dLength = CMath::GetLineLenth(tPreConerPnt.x , tPreConerPnt.y ,tConerPnt.x ,tConerPnt.y ) ;
        if(m_fpGetPxResol) dLength * m_fpGetPxResol();
        sTemp = sTemp.sprintf("L:%.3f",dLength) ;
        Gdi->Text(iX ,iY , sTemp.c_str()) ;

        tPreConerPnt = tConerPnt ;
    }
                                                
    TDPoint tEdgePnt ;
    for(int i = 0 ; i < Rslt.lEdgeList.GetDataCnt() ; i++) {
        tEdgePnt = Rslt.lEdgeList.GetCrntData(!i);
        Gdi -> Pixel((int)(tEdgePnt.x+0.5),(int)(tEdgePnt.y+0.5),clAqua);
    }



    delete Gdi;
    

}




void CPolygon_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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
bool CPolygon_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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


bool CPolygon_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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

void CPolygon_V01::MouseUp(TMouseButton Button,TShiftState Shift )
{
    CTracker * Tracker ;
    int iTrackerCnt = m_lTrckInsp.GetDataCnt() ;
    for(int i = 0 ; i < iTrackerCnt ; i++){
        Tracker = m_lTrckInsp.GetCrntData(!i) ;
        Tracker -> TrackerUp(Button, Shift);
    }
}

        //�Ķ���� ���÷���
void CPolygon_V01::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
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

void CPolygon_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
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
void CPolygon_V01::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;

    }
    else {

        UpdateLocalPara(true , _vePara );
    }

}

        //�ε� ���̺�.
void CPolygon_V01::LoadSave(bool _bLoad , String _sVisnName)
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
        //����Ʈ�� ������ 1���� �Ǳ� ������ �ʿ� ����.
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




