//---------------------------------------------------------------------------
#pragma hdrstop
#include "Place.h"
#include "ParaUnit.h"
#include "VOptnMan.h"
#include "UserFile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


CPlace::CPlace()
{
}

CPlace::~CPlace()
{
}

bool CPlace::Init(TVisnStaticPara _tVisnStaticPara)
{
    CVisionBase::Init(_tVisnStaticPara);

    PlcInsp.pTrckInsp1 = new TRectTracker();
    PlcInsp.pTrckInsp2 = new TRectTracker();
    PlcInsp.pTrckInsp2 -> SizeEnable = false ;

    PlcInsp.pInspRslt = NULL ; // 나중에 잡파일 로드 세이브 할때 할당.

    m_iActivPage = (EN_PAGE_ID) 0 ;

    return true ;
}

void CPlace::Close()
{
    CVisionBase::Close();
    delete PlcInsp.pTrckInsp1 ;
    delete PlcInsp.pTrckInsp2 ;
    if(PlcInsp.pInspRslt) {
        delete [] PlcInsp.pInspRslt ;
        PlcInsp.pInspRslt = NULL;
    }
}

void CPlace::Inspection()
{
}

void CPlace::Paint(HDC _hDc , EN_DISP_OPTN _iDpOptn ,
                              int _iDstX, int _iDstY, int _iDstW, int _iDstH ,
                              int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH ,
                              float _fScaleX , float _fScaleY ,int _iThreshold , bool _bCntrLine)
{
    CVisionBase::Paint(_hDc , _iDpOptn ,  _iDstX,  _iDstY,  _iDstW,  _iDstH , _iSrcX, _iSrcY, _iSrcW, _iSrcH , _fScaleX , _fScaleY ,_iThreshold , _bCntrLine);
/*
    if(g_bSettingMode)PlcInsp.pTrckInsp1 -> Paint(_hDc,_fScaleX , _fScaleY);
    if(g_bSettingMode)PlcInsp.pTrckInsp2 -> Paint(_hDc,_fScaleX , _fScaleY);

    if(Stat.bDispRslt){

        IGdi *Gdi = GDI_New(_hDc);

        //필요 없다.
        //Gdi -> SetScale(_fScaleX , _fScaleY);

        Gdi -> m_tPen.Style = psSolid ;
        Gdi -> m_tPen.Color = clYellow ;
        Gdi -> m_tPen.Width = 1  ;

        Gdi -> m_tText.Color = Rslt.iRet == veOk ? clLime : clRed ;
        Gdi -> m_tFont.Height = 40 ;
        Gdi -> Text(5 , 5 , m_sRsltMsg.c_str());

        GDI_Del(Gdi);
    }
*/
}

void CPlace::SetPaintCallback(PaintCallbackFunc _pCalback)
{
    CVisionBase::SetPaintCallback (_pCalback);

    PlcInsp.pTrckInsp1 -> SetFunc(_pCalback);
    PlcInsp.pTrckInsp2 -> SetFunc(_pCalback);

}


bool CPlace::OnMouseDw(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if(PlcInsp.pTrckInsp1 -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY)) return true;
    if(PlcInsp.pTrckInsp2 -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY)) return true;

    return false ;
}

bool CPlace::OnMouseMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if(PlcInsp.pTrckInsp1 -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY))return true;
    if(PlcInsp.pTrckInsp2 -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY))return true;

    return false ;
}

void CPlace::OnMouseUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    PlcInsp.pTrckInsp1 -> TrackerUp(Button, Shift);
    PlcInsp.pTrckInsp2 -> TrackerUp(Button, Shift);
}

void CPlace::ShowAlgoPara(TValueListEditor * _vePara)
{
    static TValueListEditor * _veLastPara ;

    if(_vePara != NULL)_veLastPara = _vePara ;

    UpdateAlgoPara(true , _veLastPara);
}

void CPlace::UpdateAlgoPara(bool _bTable , TValueListEditor * _vePara)
{
    //if(m_iActivPage == piEpxTsd) {
    //    UpdateParaTsd(_bTable , _vePara , VOM.VDevPara.EpxTsd);
    //}
}

void CPlace::Load(bool _bLoad)
{
    AnsiString sJobPath = g_sJobFilePath + "Place.INI" ;
    LoadParaTsd(_bLoad , VOM.VDevPara.PlcTsd , sJobPath , "plcTsd");

    PlcInsp.pTrckInsp1 -> OnLoadSave(_bLoad,sJobPath,"pTrckInsp1");
    PlcInsp.pTrckInsp2 -> OnLoadSave(_bLoad,sJobPath,"pTrckInsp2");
}

void CPlace::ApplyDevOptn()
{
    //DevOptn중에 바뀐 놈들의 크기와 갯수를 바꾼다.
    if(PlcInsp.pInspRslt) {
        delete [] PlcInsp.pInspRslt ;
        PlcInsp.pInspRslt = NULL;
    }

    int iAreaWidth  = PlcInsp.pTrckInsp1 -> Width  ;
    int iAreaHeight = PlcInsp.pTrckInsp1 -> Height ;

    PlcInsp.pInspRslt = new TSD_Rslt[VOM.VDevOptn.iCountY * VOM.VDevOptn.iCountX] ;
}

void CPlace::SaveImage()
{
    //옮겨담아놓는다.   쓰레드 돌릴때 삑사리 날까봐.
    AnsiString sSaveFolderPath , sSavePath;

    sSaveFolderPath = AnsiString("d:\\Image\\"+m_tVisnStaticPara.sVisnName+"\\");

    if(!UserFile.DirExists(sSaveFolderPath)) UserFile.CreateDir(sSaveFolderPath) ;

    sSavePath = sSaveFolderPath + Now().FormatString("yyyymmdd_hhnnss_zzz");
    m_pImg -> SaveBmp(sSavePath.c_str());
}

void CPlace::SetActivPage(EN_PAGE_ID _iActivPage)
{
    m_iActivPage = _iActivPage ;
}

EN_PAGE_ID CPlace::GetActivPage()
{
    return m_iActivPage ;
}

