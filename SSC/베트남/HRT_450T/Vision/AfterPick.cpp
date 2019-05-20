//---------------------------------------------------------------------------
#pragma hdrstop
#include "AfterPick.h"
#include "ParaUnit.h"
#include "VOptnMan.h"
#include "UserFile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


CAtPick::CAtPick()
{
}

CAtPick::~CAtPick()
{
}

bool CAtPick::Init(TVisnStaticPara _tVisnStaticPara)
{
    CVisionBase::Init(_tVisnStaticPara);

    AtpInsp.pTrckInsp1 = new TRectTracker();
    AtpInsp.pTrckInsp2 = new TRectTracker();
    AtpInsp.pTrckInsp2 -> SizeEnable = false ;

    AtpInsp.pInspRslt = NULL ; // 나중에 잡파일 로드 세이브 할때 할당.

    m_iActivPage = (EN_PAGE_ID) 0 ;

    return true ;
}

void CAtPick::Close()
{
    CVisionBase::Close();
    delete  AtpInsp.pTrckInsp1 ;
    delete  AtpInsp.pTrckInsp2 ;
    if(AtpInsp.pInspRslt) {
        delete [] AtpInsp.pInspRslt ;
        AtpInsp.pInspRslt = NULL;
    }
}

void CAtPick::Inspection()
{

}

void CAtPick::Paint(HDC _hDc , EN_DISP_OPTN _iDpOptn ,
                               int _iDstX, int _iDstY, int _iDstW, int _iDstH ,
                               int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH ,
                               float _fScaleX , float _fScaleY ,int _iThreshold , bool _bCntrLine)
{
    CVisionBase::Paint(_hDc , _iDpOptn ,  _iDstX,  _iDstY,  _iDstW,  _iDstH , _iSrcX, _iSrcY, _iSrcW, _iSrcH , _fScaleX , _fScaleY ,_iThreshold , _bCntrLine);

/*
    if(g_bSettingMode)AtpInsp.pTrckInsp1 -> Paint(_hDc,_fScaleX , _fScaleY);
    if(g_bSettingMode)AtpInsp.pTrckInsp2 -> Paint(_hDc,_fScaleX , _fScaleY);

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

void CAtPick::SetPaintCallback(PaintCallbackFunc _pCalback)
{
    CVisionBase::SetPaintCallback (_pCalback);

    AtpInsp.pTrckInsp1 -> SetFunc(_pCalback);
    AtpInsp.pTrckInsp2 -> SetFunc(_pCalback);

}


bool CAtPick::OnMouseDw(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if(AtpInsp.pTrckInsp1 -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY)) return true;
    if(AtpInsp.pTrckInsp2 -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY)) return true;

    return false ;
}

bool CAtPick::OnMouseMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if(AtpInsp.pTrckInsp1 -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY))return true;
    if(AtpInsp.pTrckInsp2 -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY))return true;

    return false ;
}

void CAtPick::OnMouseUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    AtpInsp.pTrckInsp1 -> TrackerUp(Button, Shift);
    AtpInsp.pTrckInsp2 -> TrackerUp(Button, Shift);
}

void CAtPick::ShowAlgoPara(TValueListEditor * _vePara)
{
    static TValueListEditor * _veLastPara ;

    if(_vePara != NULL)_veLastPara = _vePara ;

    UpdateAlgoPara(true , _veLastPara);
}

void CAtPick::UpdateAlgoPara(bool _bTable , TValueListEditor * _vePara)
{
    if(m_iActivPage == piEpxTsd) {
        UpdateParaTsd(_bTable , _vePara , VOM.VDevPara.EpxTsd);
    }
}

void CAtPick::Load(bool _bLoad)
{
    AnsiString sJobPath = g_sJobFilePath + "AfterPick.INI" ;
    LoadParaTsd(_bLoad , VOM.VDevPara.AtpTsd , sJobPath , "AtpTsd");

    AtpInsp.pTrckInsp1 -> OnLoadSave(_bLoad,sJobPath,"pTrckInsp1");
    AtpInsp.pTrckInsp2 -> OnLoadSave(_bLoad,sJobPath,"pTrckInsp2");
}

void CAtPick::ApplyDevOptn()
{
    if(AtpInsp.pInspRslt) {
        delete [] AtpInsp.pInspRslt ;
        AtpInsp.pInspRslt = NULL;
    }

    int iAreaWidth  = AtpInsp.pTrckInsp1 -> Width  ;
    int iAreaHeight = AtpInsp.pTrckInsp1 -> Height ;
    AtpInsp.pInspRslt = new TSD_Rslt[VOM.VDevOptn.iCountX * VOM.VDevOptn.iCountY] ;

}

void CAtPick::SaveImage()
{
    //옮겨담아놓는다.   쓰레드 돌릴때 삑사리 날까봐.
    AnsiString sSaveFolderPath , sSavePath;

    sSaveFolderPath = AnsiString("d:\\Image\\"+m_tVisnStaticPara.sVisnName+"\\");

    if(!UserFile.DirExists(sSaveFolderPath)) UserFile.CreateDir(sSaveFolderPath) ;

    sSavePath = sSaveFolderPath + Now().FormatString("yyyymmdd_hhnnss_zzz");
    m_pImg -> SaveBmp(sSavePath.c_str());
}

void CAtPick::SetActivPage(EN_PAGE_ID _iActivPage)
{
    m_iActivPage = _iActivPage ;
}

EN_PAGE_ID CAtPick::GetActivPage()
{
    return m_iActivPage ;
}

