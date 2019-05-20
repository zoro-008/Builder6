//---------------------------------------------------------------------------
#include <stdio.h>
#pragma hdrstop
#include "AtPlce.h"
#include "ParaUnit.h"
#include "VOptnMan.h"
#include "UserFile.h"



//---------------------------------------------------------------------------
#pragma package(smart_init)


CAtPlce::CAtPlce()
{
}

CAtPlce::~CAtPlce()
{
}

int CAtPlce::GetInspRowCnt(){return VOM.VDevOptn.iCountY ;}
int CAtPlce::GetInspColCnt(){return VOM.VDevOptn.iCountX ;}
void CAtPlce::SetInspRowCnt(int _iVal){VOM.VDevOptn.iCountY = _iVal;}
void CAtPlce::SetInspColCnt(int _iVal){VOM.VDevOptn.iCountX = _iVal;}

bool CAtPlce::Init(TVisnStaticPara _tVisnStaticPara)
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
    Insp.pRslt  = NULL ; // 나중에 잡파일 로드 세이브 할때 갯수 맞게 할당.

    m_iActivPage = (EN_PAGE_ID) 0 ;

    return true ;
}

void CAtPlce::Close()
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
    /*
    BfPickVs
    Pick
    RwPlce  
    AtPickVs
    BfPlceVs
    Plce    
    AtPlceVs
    */
}


void CAtPlce::Inspection()
{
    DWORD dwStartTime = GetTickCount() ;

    memset(&Rslt , 0 , sizeof(TRetResult));

    EN_VISN_ERR_ID iInspRslt    = veAtPlceOk ;
    int            iInspFailCnt = 0       ;
    bool           bRet         = true    ;


    while(1) { //고투문 대용.
        //OCV Inspection.
        int iTrackerXOffset ;
        int iTrackerYOffset ;
                        //IImage * _pImg , IArea * _pTrainArea , IImage * _pTrainImg , TRect _tInspRect , OCV_Para _tPara , IArea * _pRsltArea , OCV_Rslt * _pRslt) ;
        //for(int i = 0 ; i < 15*11 ; i++) {
        bRet = OCV_Inspect(m_pImg , Orig.pTrArea , Orig.pTrImage , Orig.pTrckInsp->GetOutRect() , VOM.VDevPara.AtPlceOcv , Orig.pRsltArea , &Orig.tRslt) ;
        //}
        if(!bRet) {
            iInspRslt = veAtPlceOriFail ;
            m_sRsltMsg = AnsiString("FAILED ORIGIN : ") + SVL_GetErrMsg();
            break ;
        }

        if(Orig.tRslt.fSinc < VOM.VDevOptn.dAtPlceOriSinc) {
            iInspRslt = veAtPlceOriNg ;
            m_sRsltMsg = m_sRsltMsg.sprintf("NG OCV Sinc : %.1f" , Orig.tRslt.fSinc );
            break ;
        }

        iTrackerXOffset = Orig.tRslt.tRect.left - Orig.pTrckInsp -> Left ;
        iTrackerYOffset = Orig.tRslt.tRect.top  - Orig.pTrckInsp -> Top  ;

        //Threshold Inspection.
        float fColGap = (Insp.pTrckInsp2->Left - Insp.pTrckInsp1->Left) / float(GetInspColCnt() - 1) ;
        float fRowGap = (Insp.pTrckInsp2->Top  - Insp.pTrckInsp1->Top ) / float(GetInspRowCnt() - 1) ;
        float fTracker1Left   = Insp.pTrckInsp1 -> Left + iTrackerXOffset ; //OCV 오프셑 포함.
        float fTracker1Top    = Insp.pTrckInsp1 -> Top  + iTrackerYOffset ;
        float fTracker1Width  = Insp.pTrckInsp1 -> Width  ;
        float fTracker1Height = Insp.pTrckInsp1 -> Height ;
        float fTracker1Right  = fTracker1Left + fTracker1Width  ;
        float fTracker1Bottom = fTracker1Top  + fTracker1Height ;

        TRect tInspRect ;

        int   iInspPxCnt ; // = fTracker1Width * fTracker1Height ;
        float fMinPxPer   = 100 ;
        float fMaxPxPer   = 0.0 ;
        float fPxPer      = 0.0 ;

        for(int r = 0 ; r < VOM.VDevOptn.iCountY ; r++) {
            for(int c = 0 ; c < VOM.VDevOptn.iCountX ; c++) {
                tInspRect.left   = fTracker1Left   + c * fColGap ;
                tInspRect.top    = fTracker1Top    + r * fRowGap ;
                tInspRect.right  = fTracker1Right  + c * fColGap ;
                tInspRect.bottom = fTracker1Bottom + r * fRowGap ;

                bRet = TSD_Inspect(m_pImg , tInspRect , VOM.VDevPara.AtPlceTsd , &Insp.pRslt[VOM.VDevOptn.iCountX * r + c]);

                iInspPxCnt = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.Height() * Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.Width() ;

                if(!bRet) {
                    iInspRslt = veAtPlceInspFail ;
                    m_sRsltMsg = AnsiString("FAILED Insp Y:") + r + AnsiString(" X:") + c +AnsiString(" ")+ SVL_GetErrMsg();
                    break ;
                }

                fPxPer = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].iPxCnt * 100 / (float)iInspPxCnt ;
                if(fMinPxPer > fPxPer) fMinPxPer = fPxPer ;
                if(fMaxPxPer < fPxPer) fMaxPxPer = fPxPer ;
                /*
                iInspPxCnt = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.Width() * Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.Height() ;
                fPercent = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].iPxCnt * 100 /(float)iInspPxCnt ;
                */


                if(fPxPer < VOM.VDevOptn.dAtPlcePxMinPer) {
                    iInspRslt = veAtPlceInspNg ;
                    Rslt.bFailMap[r][c] = true ;
                    iInspFailCnt++;
                }
            }
            if(!iInspRslt == veAtPlceInspFail) break ;
        }
        if(iInspRslt != veAtPlceOk) {
            m_sRsltMsg = AnsiString("NG : MinPer=") + fMinPxPer + AnsiString(" MaxPer=") + fMaxPxPer ;

        }
        else {
            m_sRsltMsg = AnsiString("OK : MinPer=") + fMinPxPer + AnsiString(" MaxPer=") + fMaxPxPer ;
        }

        break ; //While문 끝.
    }

    //검사 결과 세팅.
    Rslt.iRet     = iInspRslt    ;
    Rslt.iFailCnt = iInspFailCnt ;
    Rslt.bInspEnd = true ;


    //화면결과값 뿌려주기.
    Stat.bDispRslt = true ;

    m_pPaintCallback();

    TimeInfo.iInsp = GetTickCount() - dwStartTime ;
}

void CAtPlce::Train()
{
    if(m_iActivPage == piAtPlceOcv) {
        //(IImage * _pImg , TRect _Rect , OCV_Para _tPara ,  IArea * _pTrainArea , IImage * _pTrainImg) ;
        if(!OCV_Train(GetImg() , Orig.pTrckInsp->GetTRect() , VOM.VDevPara.AtPlceOcv , Orig.pTrArea , Orig.pTrImage)) {
            ShowMessage(SVL_GetErrMsg());
        }
    }
}

void CAtPlce::Paint(HDC _hDc , EN_DISP_OPTN _iDpOptn ,
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

    float fColGap = (Insp.pTrckInsp2->Left - Insp.pTrckInsp1->Left) * _fScaleX / float(GetInspColCnt() - 1) ;
    float fRowGap = (Insp.pTrckInsp2->Top  - Insp.pTrckInsp1->Top ) * _fScaleY / float(GetInspRowCnt() - 1) ;
    float fTracker1Left   =(Insp.pTrckInsp1 -> Left  - _iSrcX)* _fScaleX;
    float fTracker1Top    =(Insp.pTrckInsp1 -> Top   - _iSrcY)* _fScaleY;
    float fTracker1Width  =(Insp.pTrckInsp1 -> Width         )* _fScaleX;
    float fTracker1Height =(Insp.pTrckInsp1 -> Height        )* _fScaleY;
    float fTracker1Right  = fTracker1Left + fTracker1Width ;
    float fTracker1Bottom = fTracker1Top  + fTracker1Height;

    OCV_Disp OcvDisp ;

    OcvDisp.clInspPx = clBlue ;
    OcvDisp.clNGPx   = clRed  ;
    OcvDisp.clText   = clLime ;
    OcvDisp.clCenter = clLime ;
    OcvDisp.clRect   = clLime ;


    if(g_bSettingMode){
        if(m_iActivPage == piAtPlceLCS) {
            PaintCntLine(_hDc , _iDstW , _iDstH);
        }
        else if(m_iActivPage == piAtPlceOcv) {
            //IImage * _pImg , HDC      _hHdc      ,IArea * _pRsltArea , OCV_Rslt *_pRslt , OCV_Disp   _tDisp
            OCV_PaintRslt(m_pImg , _hDc , Orig.pRsltArea , &Orig.tRslt , OcvDisp) ;
            Orig.pTrckInsp -> Paint(_hDc,_fScaleX , _fScaleY,_iSrcX , _iSrcY);
        }
        else if(m_iActivPage == piAtPlceTsd) {
            for(int r = 0 ; r < VOM.VDevOptn.iCountY ; r++) {
                for(int c = 0 ; c < VOM.VDevOptn.iCountX ; c++) {
                    //Insp.pRslt[VOM.VDevOptn.iCountX * r + c];
                    Gdi -> Rect(false,fTracker1Left + c * fColGap , fTracker1Top + r * fRowGap , fTracker1Right + c * fColGap , fTracker1Bottom + r * fRowGap , 0);
                    //Gdi -> Rect(false,fTracker1Left + c * fColGap , fTracker1Top + r * fRowGap , fTracker1Right + c * fColGap , fTracker1Bottom + r * fRowGap , 0);
                }
            }
            Insp.pTrckInsp1 -> Paint(_hDc,_fScaleX , _fScaleY ,_iSrcX , _iSrcY);
            Insp.pTrckInsp2 -> Paint(_hDc,_fScaleX , _fScaleY ,_iSrcX , _iSrcY);
        }
    }


    TRect tScaledRect ;
    char str[16];
    float fPercent ;
    if(Stat.bDispRslt){
        OCV_PaintRslt(m_pImg , _hDc , Orig.pRsltArea , &Orig.tRslt , OcvDisp) ;

        /*
        dPxPer = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].iPxCnt * 100 / iInspPxCnt ;
                if(dMinPxPer > dPxPer) dMinPxPer = dPxPer ;
                if(dMaxPxPer < dPxPer) dMaxPxPer = dPxPer ;


                if(dPxPer < VOM.VDevOptn.dAtPlcePxMinPer) {
                    iInspRslt = veAtPlceInspNg ;
                    iInspFailCnt++;
                }



        char str[500];
        sprintf(str, "Width : %d"          , FWidth    );
        Gdi -> Text(x1+10, y2+35, str);
        */



        int iInspPxCnt ;
        for(int r = 0 ; r < VOM.VDevOptn.iCountY ; r++) {
            for(int c = 0 ; c < VOM.VDevOptn.iCountX ; c++) {
                iInspPxCnt = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.Width() * Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.Height() ;
                fPercent = Insp.pRslt[VOM.VDevOptn.iCountX * r + c].iPxCnt * 100 /(float)iInspPxCnt ;
                if(fPercent  < VOM.VDevOptn.dAtPlcePxMinPer) {Gdi -> m_tPen.Color = clRed  ; Gdi -> m_tText.Color = clRed  ;}
                else                                      {Gdi -> m_tPen.Color = clLime ; Gdi -> m_tText.Color = clLime ;}

                tScaledRect.left   =(Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.left   - _iSrcX)* _fScaleX;
                tScaledRect.top    =(Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.top    - _iSrcY)* _fScaleY;
                tScaledRect.right  =(Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.right  - _iSrcX)* _fScaleX;
                tScaledRect.bottom =(Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.bottom - _iSrcY)* _fScaleY;
                Gdi -> Rect(false,tScaledRect , 0);
                sprintf(str, "%0.1f"  , fPercent    );
                Gdi -> Text((Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.left- _iSrcX) * _fScaleX , (Insp.pRslt[VOM.VDevOptn.iCountX * r + c].tRect.bottom - _iSrcY) * _fScaleY + 5 , str);
            }
        }

        //필요 없다.
        //Gdi -> SetScale(_fScaleX , _fScaleY);

        Gdi -> m_tText.Color = Rslt.iRet == veOk ? clLime : clRed ;
        Gdi -> m_tFont.Height = 40 ;
        Gdi -> Text(5 , 5 , m_sRsltMsg.c_str());

    }
    GDI_Del(Gdi);
}

void CAtPlce::SetPaintCallback(PaintCallbackFunc _pCalback)
{
    CVisionBase::SetPaintCallback (_pCalback);

    Orig.pTrckInsp  -> SetFunc(_pCalback);

    Insp.pTrckInsp1 -> SetFunc(_pCalback);
    Insp.pTrckInsp2 -> SetFunc(_pCalback);

}


bool CAtPlce::OnMouseDw(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    if(g_bSettingMode){
        if(m_iActivPage == piAtPlceLCS) {
        }
        else if(m_iActivPage == piAtPlceOcv) {
            if(Orig.pTrckInsp  -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY , _fOffsetX , _fOffsetY)) return true;
        }
        else if(m_iActivPage == piAtPlceTsd) {
            if(Insp.pTrckInsp2 -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY , _fOffsetX , _fOffsetY)) return true;
            if(Insp.pTrckInsp1 -> TrackerDw(Button, Shift, X, Y , _fScaleX , _fScaleY , _fOffsetX , _fOffsetY)) return true;

        }
    }
    return false ;
}

bool CAtPlce::OnMouseMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
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

void CAtPlce::OnMouseUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY, float _fOffsetX, float _fOffsetY)
{
    Orig.pTrckInsp  -> TrackerUp(Button, Shift);

    Insp.pTrckInsp1 -> TrackerUp(Button, Shift);
    Insp.pTrckInsp2 -> TrackerUp(Button, Shift);
}

void CAtPlce::ShowAlgoPara(TValueListEditor * _vePara)
{
    static TValueListEditor * _veLastPara ;

    if(_vePara != NULL)_veLastPara = _vePara ;

    UpdateAlgoPara(true , _veLastPara);
}

void CAtPlce::UpdateAlgoPara(bool _bTable , TValueListEditor * _vePara)
{
    if(m_iActivPage == piAtPlceOcv) {
        UpdateParaOcv(_bTable , _vePara , VOM.VDevPara.AtPlceOcv);
    }
    else if(m_iActivPage == piAtPlceTsd) {
        UpdateParaTsd(_bTable , _vePara , VOM.VDevPara.AtPlceTsd);
    }
}

void CAtPlce::Load(bool _bLoad)
{
    AnsiString sJobPath = g_sJobFilePath + "Epoxy.INI" ;

    //이미지 & 카메라 라이트 세팅.
    LoadImg(_bLoad);
    LoadCamLightPara(_bLoad);

    //Origin
    LoadParaOcv(_bLoad , VOM.VDevPara.AtPlceOcv , sJobPath , "Orig");
    Orig.pTrckInsp -> OnLoadSave(_bLoad , sJobPath , "Orig_pTrckInsp");
    if(_bLoad)Orig.pTrArea   -> LoadBmp((g_sJobFilePath + "Orig_TrainArea" ).c_str());
    else      Orig.pTrArea   -> SaveBmp((g_sJobFilePath + "Orig_TrainArea" ).c_str());
    if(_bLoad)Orig.pTrImage  -> LoadBmp((g_sJobFilePath + "Orig_TrainImage").c_str());
    else      Orig.pTrImage  -> SaveBmp((g_sJobFilePath + "Orig_TrainImage").c_str());

    //Insp
    LoadParaTsd(_bLoad , VOM.VDevPara.AtPlceTsd , sJobPath , "Insp");
    Insp.pTrckInsp1 -> OnLoadSave(_bLoad,sJobPath,"Insp_pTrckInsp1");
    Insp.pTrckInsp2 -> OnLoadSave(_bLoad,sJobPath,"Insp_pTrckInsp2");

    //트렉커 사이즈 변경 됐을 경우. 어차피 트레숄드에서 바꾸게 해놓음.
    ApplyDevOptn();

}

void CAtPlce::ApplyDevOptn()
{
    //DevOptn중에 바뀐 놈들의 크기와 갯수를 바꾼다.
    if(Insp.pRslt) {
        delete [] Insp.pRslt ;
        Insp.pRslt = NULL;
    }

    Insp.pRslt = new TSD_Rslt[VOM.VDevOptn.iCountY * VOM.VDevOptn.iCountX] ;
}

void CAtPlce::SaveImage()
{
    //옮겨담아놓는다.   쓰레드 돌릴때 삑사리 날까봐.
    AnsiString sSaveFolderPath , sSavePath;

    sSaveFolderPath = AnsiString("d:\\Image\\"+m_tVisnStaticPara.sVisnName+"\\");

    if(!UserFile.DirExists(sSaveFolderPath)) UserFile.CreateDir(sSaveFolderPath) ;

    sSavePath = sSaveFolderPath + Now().FormatString("yyyymmdd_hhnnss_zzz");
    m_pImg -> SaveBmp(sSavePath.c_str());
}



