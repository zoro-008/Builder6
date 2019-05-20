//---------------------------------------------------------------------------
#include <stdio.h>
#pragma hdrstop
#include "Orientation.h"
#include "ParaUnit.h"
#include "VOptnMan.h"
#include "UserFile.h"



//---------------------------------------------------------------------------
#pragma package(smart_init)


COrientation::COrientation()
{
}

COrientation::~COrientation()
{
}

bool COrientation::Init(TVisnStaticPara _tVisnStaticPara)
{
    CVisionBase::Init(_tVisnStaticPara);

    //ORIGIN
    Orig.pTrckInsp  = new TOutBandRectTracker() ;
    Orig.pTrckInsp -> HeightMax = 80 ;
    Orig.pTrckInsp -> WidthMax  = 80 ;
    memset(&Orig.tRslt , 0 , sizeof(Orig.tRslt));
    Orig.pTrArea   = ARA_New();
    Orig.pRsltArea = ARA_New();
    Orig.pTrImage  = IMG_New();

    m_iActivPage = (EN_PAGE_ID) 0 ;

    return true ;
}

void COrientation::Close()
{
    CVisionBase::Close();
    delete Orig.pTrckInsp ;
    ARA_Del(Orig.pTrArea  );
    ARA_Del(Orig.pRsltArea);
    IMG_Del(Orig.pTrImage);
}


void COrientation::Inspection()
{
    DWORD dwStartTime = GetTickCount() ;

    memset(&Rslt , 0 , sizeof(TRetResult));

    EN_VISN_ERR_ID iInspRslt    = veOriOk ;
    int            iInspFailCnt = 0       ;
    bool           bRet         = true    ;


    while(1) { //������ ���.
        //OCV Inspection.
        int iTrackerXOffset ;
        int iTrackerYOffset ;
                        //IImage * _pImg , IArea * _pTrainArea , IImage * _pTrainImg , TRect _tInspRect , OCV_Para _tPara , IArea * _pRsltArea , OCV_Rslt * _pRslt) ;
        //for(int i = 0 ; i < 15*11 ; i++) {
        bRet = OCV_Inspect(m_pImg , Orig.pTrArea , Orig.pTrImage , Orig.pTrckInsp->GetOutRect() , VOM.VDevPara.OriOcv , Orig.pRsltArea , &Orig.tRslt) ;
        //}
        if(!bRet) {
            iInspRslt = veOriOcvFail ;
            m_sRsltMsg = AnsiString("FAILED ORIGIN : ") + SVL_GetErrMsg();
            break ;
        }

        if(Orig.tRslt.fSinc < VOM.VDevOptn.dOriOriSinc) {
            iInspRslt = veOriOcvNg ;
            m_sRsltMsg = m_sRsltMsg.sprintf("NG OCV Sinc : %.1f" , Orig.tRslt.fSinc );
            break ;
        }

        iTrackerXOffset = Orig.tRslt.tRect.left - Orig.pTrckInsp -> Left ;
        iTrackerYOffset = Orig.tRslt.tRect.top  - Orig.pTrckInsp -> Top  ;

        //���� ��.
        m_sRsltMsg = m_sRsltMsg.sprintf("OK OCV Sinc : %.1f" , Orig.tRslt.fSinc );


        break ; //While�� ��.
    }

    //�˻� ��� ����.
    Rslt.iRet     = iInspRslt    ;
    Rslt.bInspEnd = true ;


    //ȭ������ �ѷ��ֱ�.
    Stat.bDispRslt = true ;

    m_pPaintCallback();

    TimeInfo.iInsp = GetTickCount() - dwStartTime ;
}

void COrientation::Train()
{
    if(m_iActivPage == piOriOcv) {
        //(IImage * _pImg , TRect _Rect , OCV_Para _tPara ,  IArea * _pTrainArea , IImage * _pTrainImg) ;
        if(!OCV_Train(GetImg() , Orig.pTrckInsp->GetTRect() , VOM.VDevPara.OriOcv , Orig.pTrArea , Orig.pTrImage)) {
            ShowMessage(SVL_GetErrMsg());
        }
    }
}

void COrientation::Paint(HDC _hDc , EN_DISP_OPTN _iDpOptn ,
                              int _iDstX, int _iDstY, int _iDstW, int _iDstH ,
                              int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH ,
                              float _fScaleX , float _fScaleY ,int _iThreshold , bool _bCntrLine)
{
    CVisionBase::Paint(_hDc , _iDpOptn ,
                       _iDstX, _iDstY , _iDstW , _iDstH ,
                       _iSrcX, _iSrcY , _iSrcW , _iSrcH ,
                       _fScaleX , _fScaleY ,_iThreshold , _bCntrLine);


    IGdi  * Gdi = GDI_New(_hDc);

    Gdi -> m_tPen.Style = psSolid ;
    Gdi -> m_tPen.Color = clYellow ;
    Gdi -> m_tPen.Width = 1  ;

    OCV_Disp OcvDisp ;

    OcvDisp.clInspPx = clBlue ;
    OcvDisp.clNGPx   = clRed  ;
    OcvDisp.clText   = clLime ;
    OcvDisp.clCenter = clLime ;
    OcvDisp.clRect   = clLime ;


    int iImgCntPntX = (GetImg()->GetWidth() /2.0 - _iSrcX)*_fScaleX;
    int iImgCntPntY = (GetImg()->GetHeight()/2.0 - _iSrcY)*_fScaleY;

    if(g_bSettingMode){
        if(m_iActivPage == piOriLCS) {
            PaintCntLineCnt(_hDc , iImgCntPntX , iImgCntPntY);

            //PaintCntLine(_hDc , _iDstW , _iDstH);
            //PaintCntLine(_hDc , _iSrcW , _iSrcH);
        }
        else if(m_iActivPage == piOriOcv) {
            //IImage * _pImg , HDC      _hHdc      ,IArea * _pRsltArea , OCV_Rslt *_pRslt , OCV_Disp   _tDisp
            OCV_PaintRslt(m_pImg , _hDc , Orig.pRsltArea , &Orig.tRslt , OcvDisp) ;
            Orig.pTrckInsp -> Paint(_hDc,_fScaleX , _fScaleY,_iSrcX , _iSrcY);
        }
    }

    TRect tScaledRect ;
    char str[16];
    float fPercent ;
    if(Stat.bDispRslt){
        OCV_PaintRslt(m_pImg , _hDc , Orig.pRsltArea , &Orig.tRslt , OcvDisp) ;

        Gdi -> m_tText.Color = Rslt.iRet == veOk ? clLime : clRed ;
        Gdi -> m_tFont.Height = 40 ;
        Gdi -> Text(5 , 5 , m_sRsltMsg.c_str());

    }
    GDI_Del(Gdi);
}

void COrientation::SetPaintCallback(PaintCallbackFunc _pCalback)
{
    CVisionBase::SetPaintCallback (_pCalback);

    Orig.pTrckInsp  -> SetFunc(_pCalback);

}


bool COrientation::OnMouseDw(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if(g_bSettingMode){
        if(m_iActivPage == piOriLCS) {
        }
        else if(m_iActivPage == piOriOcv) {
            if(Orig.pTrckInsp  -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY , _fOffsetX , _fOffsetY)) return true;
        }
    }
    return false ;
}

bool COrientation::OnMouseMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if(Orig.pTrckInsp -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY , _fOffsetX , _fOffsetY))return true;

    return false ;
}

void COrientation::OnMouseUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    Orig.pTrckInsp  -> TrackerUp(Button, Shift);
}

void COrientation::ShowAlgoPara(TValueListEditor * _vePara)
{
    static TValueListEditor * _veLastPara ;

    if(_vePara != NULL)_veLastPara = _vePara ;

    UpdateAlgoPara(true , _veLastPara);
}

void COrientation::UpdateAlgoPara(bool _bTable , TValueListEditor * _vePara)
{
    if(m_iActivPage == piOriOcv) {
        UpdateParaOcv(_bTable , _vePara , VOM.VDevPara.OriOcv);
    }
}

void COrientation::Load(bool _bLoad)
{
    AnsiString sJobPath = g_sJobFilePath + "Epoxy.INI" ;

    //�̹��� & ī�޶� ����Ʈ ����.
    LoadImg(_bLoad);
    LoadCamLightPara(_bLoad);

    //Origin
    LoadParaOcv(_bLoad , VOM.VDevPara.OriOcv , sJobPath , "Orig");
    Orig.pTrckInsp -> OnLoadSave(_bLoad , sJobPath , "Orig_pTrckInsp");
    if(_bLoad)Orig.pTrArea   -> LoadBmp((g_sJobFilePath + "Orig_TrainArea" ).c_str());
    else      Orig.pTrArea   -> SaveBmp((g_sJobFilePath + "Orig_TrainArea" ).c_str());
    if(_bLoad)Orig.pTrImage  -> LoadBmp((g_sJobFilePath + "Orig_TrainImage").c_str());
    else      Orig.pTrImage  -> SaveBmp((g_sJobFilePath + "Orig_TrainImage").c_str());

    //Ʈ��Ŀ ������ ���� ���� ���. ������ Ʈ����忡�� �ٲٰ� �س���.
    ApplyDevOptn();

}

void COrientation::ApplyDevOptn()
{
}

void COrientation::TrackerInit()
{
    Orig.pTrckInsp -> Left = 10 ;
    Orig.pTrckInsp -> Top  = 10 ;
}

void COrientation::SaveImage()
{
    //�Űܴ�Ƴ��´�.   ������ ������ ��縮 �����.
    AnsiString sSaveFolderPath , sSavePath;

    sSaveFolderPath = AnsiString("d:\\Image\\"+m_tVisnStaticPara.sVisnName+"\\");

    if(!UserFile.DirExists(sSaveFolderPath)) UserFile.CreateDir(sSaveFolderPath) ;

    sSavePath = sSaveFolderPath + Now().FormatString("yyyymmdd_hhnnss_zzz");
    m_pImg -> SaveBmp(sSavePath.c_str());
}


