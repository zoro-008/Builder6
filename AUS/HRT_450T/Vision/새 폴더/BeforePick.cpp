//---------------------------------------------------------------------------
#pragma hdrstop
#include "BeforePick.h"
#include "ParaUnit.h"
#include "VOptnMan.h"
#include "UserFile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


CBfPick::CBfPick()
{
}

CBfPick::~CBfPick()
{
}

bool CBfPick::Init(TVisnStaticPara _tVisnStaticPara)
{
    CVisionBase::Init(_tVisnStaticPara);

    BfpInsp.pTrckInsp1 = new TRectTracker();
    BfpInsp.pTrckInsp2 = new TRectTracker();
    BfpInsp.pTrckInsp2 -> SizeEnable = false ;

    BfpInsp.pInspRslt = NULL ; // 나중에 잡파일 로드 세이브 할때 할당.

    m_iActivPage = (EN_PAGE_ID) 0 ;

    return true ;
}

void CBfPick::Close()
{
    CVisionBase::Close();
    delete  BfpInsp.pTrckInsp1 ;
    delete  BfpInsp.pTrckInsp2 ;
    if(BfpInsp.pInspRslt) {
        delete [] BfpInsp.pInspRslt ;
        BfpInsp.pInspRslt = NULL;
    }
}

void CBfPick::Inspection()
{
}

void CBfPick::Paint(HDC _hDc , EN_DISP_OPTN _iDpOptn ,
                               int _iDstX, int _iDstY, int _iDstW, int _iDstH ,
                               int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH ,
                               float _fScaleX , float _fScaleY ,int _iThreshold , bool _bCntrLine)
{
    CVisionBase::Paint(_hDc , _iDpOptn ,  _iDstX,  _iDstY,  _iDstW,  _iDstH , _iSrcX, _iSrcY, _iSrcW, _iSrcH , _fScaleX , _fScaleY ,_iThreshold , _bCntrLine);
/*
    if(g_bSettingMode)BfpInsp.pTrckInsp1 -> Paint(_hDc,_fScaleX , _fScaleY);
    if(g_bSettingMode)BfpInsp.pTrckInsp2 -> Paint(_hDc,_fScaleX , _fScaleY);

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

void CBfPick::SetPaintCallback(PaintCallbackFunc _pCalback)
{
    CVisionBase::SetPaintCallback (_pCalback);

    BfpInsp.pTrckInsp1 -> SetFunc(_pCalback);
    BfpInsp.pTrckInsp2 -> SetFunc(_pCalback);

}


bool CBfPick::OnMouseDw(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if(BfpInsp.pTrckInsp1 -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY)) return true;
    if(BfpInsp.pTrckInsp2 -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY)) return true;

    return false ;
}

bool CBfPick::OnMouseMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if(BfpInsp.pTrckInsp1 -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY))return true;
    if(BfpInsp.pTrckInsp2 -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY))return true;

    return false ;
}

void CBfPick::OnMouseUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    BfpInsp.pTrckInsp1 -> TrackerUp(Button, Shift);
    BfpInsp.pTrckInsp2 -> TrackerUp(Button, Shift);
}

void CBfPick::ShowAlgoPara(TValueListEditor * _vePara)
{
    static TValueListEditor * _veLastPara ;

    if(_vePara != NULL)_veLastPara = _vePara ;

    UpdateAlgoPara(true , _veLastPara);
}

void CBfPick::UpdateAlgoPara(bool _bTable , TValueListEditor * _vePara)
{
    if(m_iActivPage == piEpxTsd) {
        UpdateParaTsd(_bTable , _vePara , VOM.VDevPara.EpxTsd);
    }
}

void CBfPick::Load(bool _bLoad)
{
    AnsiString sJobPath = g_sJobFilePath + "BeforePick.INI" ;
    LoadParaTsd(_bLoad , VOM.VDevPara.BfpTsd , sJobPath , "BfpTsd");

    BfpInsp.pTrckInsp1 -> OnLoadSave(_bLoad,sJobPath,"pTrckInsp1");
    BfpInsp.pTrckInsp2 -> OnLoadSave(_bLoad,sJobPath,"pTrckInsp2");

}

void CBfPick::ApplyDevOptn()
{

    if(BfpInsp.pInspRslt) {
        delete [] BfpInsp.pInspRslt ;
        BfpInsp.pInspRslt = NULL;
    }

    int iAreaWidth  = BfpInsp.pTrckInsp1 -> Width  ;
    int iAreaHeight = BfpInsp.pTrckInsp1 -> Height ;
    BfpInsp.pInspRslt = new TSD_Rslt[VOM.VDevOptn.iCountX * VOM.VDevOptn.iCountY] ;
}

void CBfPick::SaveImage()
{
    //옮겨담아놓는다.   쓰레드 돌릴때 삑사리 날까봐.
    AnsiString sSaveFolderPath , sSavePath;

    sSaveFolderPath = AnsiString("d:\\Image\\"+m_tVisnStaticPara.sVisnName+"\\");

    if(!UserFile.DirExists(sSaveFolderPath)) UserFile.CreateDir(sSaveFolderPath) ;

    sSavePath = sSaveFolderPath + Now().FormatString("yyyymmdd_hhnnss_zzz");
    m_pImg -> SaveBmp(sSavePath.c_str());
}

void CBfPick::SetActivPage(EN_PAGE_ID _iActivPage)
{
    m_iActivPage = _iActivPage ;
}

EN_PAGE_ID CBfPick::GetActivPage()
{
    return m_iActivPage ;
}

