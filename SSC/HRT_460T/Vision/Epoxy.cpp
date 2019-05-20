//---------------------------------------------------------------------------
#include <stdio.h>
#pragma hdrstop
#include "Epoxy.h"
#include "ParaUnit.h"
#include "VOptnMan.h"
#include "UserFile.h"



//---------------------------------------------------------------------------
#pragma package(smart_init)


CEpoxy::CEpoxy()
{
}

CEpoxy::~CEpoxy()
{
}

bool CEpoxy::Init(TVisnStaticPara _tVisnStaticPara)
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

    //INSP
    Insp.pTrckInsp1 = new TRectTracker();
    Insp.pTrckInsp2 = new TRectTracker();
    Insp.pTrckInsp2 -> SizeEnable = false ;
    Insp.pRslt  = NULL ; // ���߿� ������ �ε� ���̺� �Ҷ� ���� �°� �Ҵ�.

    m_iActivPage = (EN_PAGE_ID) 0 ;

    return true ;
}

void CEpoxy::Close()
{
    CVisionBase::Close();
    delete Orig.pTrckInsp ;
    ARA_Del(Orig.pTrArea  );
    ARA_Del(Orig.pRsltArea);
    IMG_Del(Orig.pTrImage);

    delete  Insp.pTrckInsp1 ;
    delete  Insp.pTrckInsp2 ;
    if(Insp.pRslt) {
        delete [] Insp.pRslt ;
        Insp.pRslt = NULL;
    }
}


void CEpoxy::Inspection()
{
    DWORD dwStartTime = GetTickCount() ;


    EN_VISN_ERR_ID iInspRslt    = veEpxOk ;
    int            iInspFailCnt = 0       ;
    bool           bRet         = true    ;


    while(1) { //������ ���.
        //OCV Inspection.
        int iTrackerXOffset ;
        int iTrackerYOffset ;
                        //IImage * _pImg , IArea * _pTrainArea , IImage * _pTrainImg , TRect _tInspRect , OCV_Para _tPara , IArea * _pRsltArea , OCV_Rslt * _pRslt) ;
        for(int i = 0 ; i < 15*11 ; i++) {
            bRet = OCV_Inspect(m_pImg , Orig.pTrArea , Orig.pTrImage , Orig.pTrckInsp->GetOutRect() , VOM.VDevPara.EpxOcv , Orig.pRsltArea , &Orig.tRslt) ;
        }
        if(!bRet) {
            iInspRslt = veEpxOriFail ;
            m_sRsltMsg = AnsiString("FAILED ORIGIN : ") + SVL_GetErrMsg();
            break ;
        }

        if(Orig.tRslt.fSinc < VOM.VDevOptn.dEpxOriSinc) {
            iInspRslt = veEpxOriNg ;
            m_sRsltMsg = m_sRsltMsg.sprintf("NG OCV Sinc : %.1f" , Orig.tRslt.fSinc );
            break ;
        }

        iTrackerXOffset = Orig.tRslt.tRect.left - Orig.pTrckInsp -> Left ;
        iTrackerYOffset = Orig.tRslt.tRect.top  - Orig.pTrckInsp -> Top  ;

        //Threshold Inspection.
        int iRowCnt = VOM.VDevOptn.iCountY ;
        int iColCnt = VOM.VDevOptn.iCountX ;
        float fColGap = (Insp.pTrckInsp2->Left - Insp.pTrckInsp1->Left) / float(iColCnt - 1) ;
        float fRowGap = (Insp.pTrckInsp2->Top  - Insp.pTrckInsp1->Top ) / float(iRowCnt - 1) ;
        float fTracker1Left   = Insp.pTrckInsp1 -> Left + iTrackerXOffset ; //OCV �����V ����.
        float fTracker1Top    = Insp.pTrckInsp1 -> Top  + iTrackerYOffset ;
        float fTracker1Width  = Insp.pTrckInsp1 -> Width  ;
        float fTracker1Height = Insp.pTrckInsp1 -> Height ;
        float fTracker1Right  = fTracker1Left + fTracker1Width  ;
        float fTracker1Bottom = fTracker1Top  + fTracker1Height ;

        TRect tInspRect ;

        int    iInspPxCnt  = fTracker1Width * fTracker1Height ;
        double dMinPxPer   = iInspPxCnt ;
        double dMaxPxPer   = 0.0 ;
        double dPxPer      = 0.0 ;

        for(int r = 0 ; r < VOM.VDevOptn.iCountY ; r++) {
            for(int c = 0 ; c < VOM.VDevOptn.iCountX ; c++) {
                tInspRect.left   = fTracker1Left   + c * fColGap ;
                tInspRect.top    = fTracker1Top    + r * fRowGap ;
                tInspRect.right  = fTracker1Right  + c * fColGap ;
                tInspRect.bottom = fTracker1Bottom + r * fRowGap ;

                bRet = TSD_Inspect(m_pImg , tInspRect , VOM.VDevPara.EpxTsd , &Insp.pRslt[VOM.VDevOptn.iCountX * r + c]);

                if(!bRet) {
                    iInspRslt = veEpxInspFail ;
                    m_sRsltMsg = AnsiString("FAILED Insp Y:") + r + AnsiString(" X:") + c +AnsiString(" ")+ SVL_GetErrMsg();
                    break ;
                }

                dPxPer = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].iPxCnt * 100 / iInspPxCnt ;
                if(dMinPxPer > dPxPer) dMinPxPer = dPxPer ;
                if(dMaxPxPer < dPxPer) dMaxPxPer = dPxPer ;


                if(dPxPer < VOM.VDevOptn.dEpxPxMinPer) {
                    iInspRslt = veEpxInspNg ;
                    iInspFailCnt++;
                }
            }
            if(!iInspRslt == veEpxInspFail) break ;
        }
        if(iInspRslt != veEpxOk) {
            m_sRsltMsg = AnsiString("NG : MinPer=") + dMinPxPer + AnsiString(" MaxPer=") + dMaxPxPer ;

        }
        else {
            m_sRsltMsg = AnsiString("OK : MinPer=") + dMinPxPer + AnsiString(" MaxPer=") + dMaxPxPer ;
        }

        break ; //While�� ��.
    }

    //�˻� ��� ����.
    Rslt.iRet     = iInspRslt    ;
    Rslt.iFailCnt = iInspFailCnt ;
    Rslt.bInspEnd = true ;


    //ȭ������ �ѷ��ֱ�.
    Stat.bDispRslt = true ;

    m_pPaintCallback();

    TimeInfo.iInsp = GetTickCount() - dwStartTime ;
}

void CEpoxy::Train()
{
    if(m_iActivPage == piEpxOcv) {
        //(IImage * _pImg , TRect _Rect , OCV_Para _tPara ,  IArea * _pTrainArea , IImage * _pTrainImg) ;
        if(!OCV_Train(GetImg() , Orig.pTrckInsp->GetTRect() , VOM.VDevPara.EpxOcv , Orig.pTrArea , Orig.pTrImage)) {
            ShowMessage(SVL_GetErrMsg());
        }
    }
}

void CEpoxy::Paint(HDC _hDc , EN_DISP_OPTN _iDpOptn ,
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

    int iRowCnt = VOM.VDevOptn.iCountY ;
    int iColCnt = VOM.VDevOptn.iCountX ;
    float fColGap = (Insp.pTrckInsp2->Left - Insp.pTrckInsp1->Left) * _fScaleX / float(iColCnt - 1) ;
    float fRowGap = (Insp.pTrckInsp2->Top  - Insp.pTrckInsp1->Top ) * _fScaleY / float(iRowCnt - 1) ;
    float fTracker1Left   = Insp.pTrckInsp1 -> Left * _fScaleX;
    float fTracker1Top    = Insp.pTrckInsp1 -> Top  * _fScaleY;
    float fTracker1Width  = Insp.pTrckInsp1 -> Width  * _fScaleX;
    float fTracker1Height = Insp.pTrckInsp1 -> Height * _fScaleY;
    float fTracker1Right  = fTracker1Left + fTracker1Width ;
    float fTracker1Bottom = fTracker1Top  + fTracker1Height;

    OCV_Disp OcvDisp ;

    OcvDisp.clInspPx = clBlue ;
    OcvDisp.clNGPx   = clRed  ;
    OcvDisp.clText   = clLime ;
    OcvDisp.clCenter = clLime ;
    OcvDisp.clRect   = clLime ;


    if(g_bSettingMode){
        if(m_iActivPage == piEpxLCS) {
            PaintCntLine(_hDc , _iDstW , _iDstH);
        }
        else if(m_iActivPage == piEpxOcv) {
            //IImage * _pImg , HDC      _hHdc      ,IArea * _pRsltArea , OCV_Rslt *_pRslt , OCV_Disp   _tDisp
            OCV_PaintRslt(m_pImg , _hDc , Orig.pRsltArea , &Orig.tRslt , OcvDisp) ;
            Orig.pTrckInsp -> Paint(_hDc,_fScaleX , _fScaleY,_iSrcX , _iSrcY);
        }
        else if(m_iActivPage == piEpxTsd) {
            for(int r = 0 ; r < VOM.VDevOptn.iCountY ; r++) {
                for(int c = 0 ; c < VOM.VDevOptn.iCountX ; c++) {
                    Insp.pRslt[VOM.VDevOptn.iCountX * r + c];
                    Gdi -> Rect(false,fTracker1Left + c * fColGap , fTracker1Top + r * fRowGap , fTracker1Right + c * fColGap , fTracker1Bottom + r * fRowGap , 0);
                    //Gdi -> Rect(false,fTracker1Left + c * fColGap , fTracker1Top + r * fRowGap , fTracker1Right + c * fColGap , fTracker1Bottom + r * fRowGap , 0);
                }
            }
            Insp.pTrckInsp1 -> Paint(_hDc,_fScaleX , _fScaleY ,_iSrcX , _iSrcY);
            Insp.pTrckInsp2 -> Paint(_hDc,_fScaleX , _fScaleY ,_iSrcX , _iSrcY);
        }
    }

    int iInspPxCnt = Insp.pTrckInsp1 -> Width * Insp.pTrckInsp1 -> Height ;
    TRect tScaledRect ;
    char str[16];
    float fPercent ;
    if(Stat.bDispRslt){
        OCV_PaintRslt(m_pImg , _hDc , Orig.pRsltArea , &Orig.tRslt , OcvDisp) ;

        /*
        dPxPer = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].iPxCnt * 100 / iInspPxCnt ;
                if(dMinPxPer > dPxPer) dMinPxPer = dPxPer ;
                if(dMaxPxPer < dPxPer) dMaxPxPer = dPxPer ;


                if(dPxPer < VOM.VDevOptn.dEpxPxMinPer) {
                    iInspRslt = veEpxInspNg ;
                    iInspFailCnt++;
                }



        char str[500];
        sprintf(str, "Width : %d"          , FWidth    );
        Gdi -> Text(x1+10, y2+35, str);
        */



        for(int r = 0 ; r < VOM.VDevOptn.iCountY ; r++) {
            for(int c = 0 ; c < VOM.VDevOptn.iCountX ; c++) {
                fPercent = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].iPxCnt * 100 / iInspPxCnt;
                if(fPercent  < VOM.VDevOptn.dEpxPxMinPer) Gdi -> m_tPen.Color = clRed  ;
                else                                      Gdi -> m_tPen.Color = clLime ;

                tScaledRect.left   = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.left   * _fScaleX;
                tScaledRect.top    = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.top    * _fScaleY;
                tScaledRect.right  = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.right  * _fScaleX;
                tScaledRect.bottom = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.bottom * _fScaleY;
                Gdi -> Rect(false,tScaledRect , 0);
                sprintf(str, "%0.1f"  , fPercent    );
                Gdi -> Text(Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.left   * _fScaleX , Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.bottom * _fScaleY + 5 , str);
            }
        }

        //�ʿ� ����.
        //Gdi -> SetScale(_fScaleX , _fScaleY);

        Gdi -> m_tText.Color = Rslt.iRet == veOk ? clLime : clRed ;
        Gdi -> m_tFont.Height = 40 ;
        Gdi -> Text(5 , 5 , m_sRsltMsg.c_str());

    }
    GDI_Del(Gdi);
}

void CEpoxy::SetPaintCallback(PaintCallbackFunc _pCalback)
{
    CVisionBase::SetPaintCallback (_pCalback);

    Orig.pTrckInsp  -> SetFunc(_pCalback);

    Insp.pTrckInsp1 -> SetFunc(_pCalback);
    Insp.pTrckInsp2 -> SetFunc(_pCalback);

}


bool CEpoxy::OnMouseDw(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if(g_bSettingMode){
        if(m_iActivPage == piEpxLCS) {
        }
        else if(m_iActivPage == piEpxOcv) {
            if(Orig.pTrckInsp  -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY , _fOffsetX , _fOffsetY)) return true;
        }
        else if(m_iActivPage == piEpxTsd) {
            if(Insp.pTrckInsp2 -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY , _fOffsetX , _fOffsetY)) return true;
            if(Insp.pTrckInsp1 -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY , _fOffsetX , _fOffsetY)) return true;

        }
    }
    return false ;
}

bool CEpoxy::OnMouseMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if(Orig.pTrckInsp -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY , _fOffsetX , _fOffsetY))return true;
    if(Insp.pTrckInsp1 -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY , _fOffsetX , _fOffsetY)){
        Insp.pTrckInsp2 -> Width  = Insp.pTrckInsp1 -> Width  ;
        Insp.pTrckInsp2 -> Height = Insp.pTrckInsp1 -> Height ;
        return true;
    }
    if(Insp.pTrckInsp2 -> TrackerMv(Shift, X, Y , _fScaleX , _fScaleY , _fOffsetX , _fOffsetY))return true;




    return false ;
}

void CEpoxy::OnMouseUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    Orig.pTrckInsp  -> TrackerUp(Button, Shift);

    Insp.pTrckInsp1 -> TrackerUp(Button, Shift);
    Insp.pTrckInsp2 -> TrackerUp(Button, Shift);
}

void CEpoxy::ShowAlgoPara(TValueListEditor * _vePara)
{
    static TValueListEditor * _veLastPara ;

    if(_vePara != NULL)_veLastPara = _vePara ;

    UpdateAlgoPara(true , _veLastPara);
}

void CEpoxy::UpdateAlgoPara(bool _bTable , TValueListEditor * _vePara)
{
    if(m_iActivPage == piEpxOcv) {
        UpdateParaOcv(_bTable , _vePara , VOM.VDevPara.EpxOcv);
    }
    else if(m_iActivPage == piEpxTsd) {
        UpdateParaTsd(_bTable , _vePara , VOM.VDevPara.EpxTsd);
    }
}

void CEpoxy::Load(bool _bLoad)
{
    AnsiString sJobPath = g_sJobFilePath + "Epoxy.INI" ;

    //�̹��� & ī�޶� ����Ʈ ����.
    LoadImg(_bLoad);
    LoadCamLightPara(_bLoad);

    //Origin
    LoadParaOcv(_bLoad , VOM.VDevPara.EpxOcv , sJobPath , "Orig");
    Orig.pTrckInsp -> OnLoadSave(_bLoad , sJobPath , "Orig_pTrckInsp");
    if(_bLoad)Orig.pTrArea   -> LoadBmp((g_sJobFilePath + "Orig_TrainArea" ).c_str());
    else      Orig.pTrArea   -> SaveBmp((g_sJobFilePath + "Orig_TrainArea" ).c_str());
    if(_bLoad)Orig.pTrImage  -> LoadBmp((g_sJobFilePath + "Orig_TrainImage").c_str());
    else      Orig.pTrImage  -> SaveBmp((g_sJobFilePath + "Orig_TrainImage").c_str());

    //Insp
    LoadParaTsd(_bLoad , VOM.VDevPara.EpxTsd , sJobPath , "Insp");
    Insp.pTrckInsp1 -> OnLoadSave(_bLoad,sJobPath,"Insp_pTrckInsp1");
    Insp.pTrckInsp2 -> OnLoadSave(_bLoad,sJobPath,"Insp_pTrckInsp2");

    //Ʈ��Ŀ ������ ���� ���� ���. ������ Ʈ����忡�� �ٲٰ� �س���.
    ApplyDevOptn();

}

void CEpoxy::ApplyDevOptn()
{
    //DevOptn�߿� �ٲ� ����� ũ��� ������ �ٲ۴�.
    if(Insp.pRslt) {
        delete [] Insp.pRslt ;
        Insp.pRslt = NULL;
    }

    Insp.pRslt = new TSD_Rslt[VOM.VDevOptn.iCountY * VOM.VDevOptn.iCountX] ;
}

void CEpoxy::SaveImage()
{
    //�Űܴ�Ƴ��´�.   ������ ������ ��縮 �����.
    AnsiString sSaveFolderPath , sSavePath;

    sSaveFolderPath = AnsiString("d:\\Image\\"+m_tVisnStaticPara.sVisnName+"\\");

    if(!UserFile.DirExists(sSaveFolderPath)) UserFile.CreateDir(sSaveFolderPath) ;

    sSavePath = sSaveFolderPath + Now().FormatString("yyyymmdd_hhnnss_zzz");
    m_pImg -> SaveBmp(sSavePath.c_str());
}



