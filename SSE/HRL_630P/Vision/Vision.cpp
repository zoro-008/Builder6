//---------------------------------------------------------------------------

#include <stdio.h>

#pragma hdrstop

#include "Vision.h"
#include "UserINI.h"
#include "UserFile.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


CVisionBase::CVisionBase()
{

}

CVisionBase::~CVisionBase()
{
}

bool CVisionBase::Init(TVisnStaticPara _tVisnStaticPara)
{
    m_pPaintCallback = NULL ;
    m_bGrabbing = false ;

    m_tmLive = new TTimer(Application); //Delete 필요 없다.
    m_tmLive -> Enabled = false ;
    m_tmLive -> Interval = 300 ;
    m_tmLive -> OnTimer = tmLiveTimer ;

    m_tVisnStaticPara = _tVisnStaticPara ;

    m_pImg = IMG_New() ;

    return true ;
}

void CVisionBase::close()
{
    m_tmLive -> Enabled = false ;

    delete m_tmLive ;

    IMG_Del(m_pImg);

    m_bGrabbing = false ;
}

void CVisionBase::SetCamLightPara(TCamLightPara _tCamLightPara)
{
    m_tCamLightPara = _tCamLightPara ;
}

/*
void CVisionBase::LoadDynPara(bool _bLoad)
{
    AnsiString sJobFolderPath ;
    sJobFolderPath = g_sJobFilePath + m_sVisnName ;

    TUserINI UserINI ;
    AnsiString sVisnName = "Visn" ;
    AnsiString sBitmapPath ;

    int iTemp ;
    if(_bLoad) {
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iCamExposure", m_tCamLightPara.iCamExposure );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iCntOffsetX ", m_tCamLightPara.iCntOffsetX  );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iCntOffsetY ", m_tCamLightPara.iCntOffsetY  );

        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iUseLightCh1", m_tCamLightPara.iUseLightCh1 );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iLtBright1  ", m_tCamLightPara.iLtBright1   );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iUseLightCh2", m_tCamLightPara.iUseLightCh2 );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iLtBright2  ", m_tCamLightPara.iLtBright2   );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iUseLightCh3", m_tCamLightPara.iUseLightCh3 );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iLtBright3  ", m_tCamLightPara.iLtBright3   );
    }
    else {
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iCamExposure", m_tCamLightPara.iCamExposure );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iCntOffsetX ", m_tCamLightPara.iCntOffsetX  );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iCntOffsetY ", m_tCamLightPara.iCntOffsetY  );

        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iUseLightCh1", m_tCamLightPara.iUseLightCh1 );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iLtBright1  ", m_tCamLightPara.iLtBright1   );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iUseLightCh2", m_tCamLightPara.iUseLightCh2 );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iLtBright2  ", m_tCamLightPara.iLtBright2   );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iUseLightCh3", m_tCamLightPara.iUseLightCh3 );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iLtBright3  ", m_tCamLightPara.iLtBright3   );
    }
}
*/

void CVisionBase::SetPaintCallback(PaintCallbackFunc _pCalback)
{
    m_pPaintCallback = _pCalback ;
}

void __fastcall CVisionBase::tmLiveTimer(TObject *Sender)
{
    m_tmLive -> Enabled = false ;
    if(!m_bGrabbing)Grab();
    m_tmLive -> Enabled = true ;

}

int CVisionBase::GetCenterX()
{
    return m_pImg[m_tVisnStaticPara.iImgId]->GetWidth () / 2 + m_tCamLightPara.iCntOffsetX ;
}

int CVisionBase::GetCenterY()
{
    return m_pImg[m_tVisnStaticPara.iImgId]->GetHeight() / 2 + m_tCamLightPara.iCntOffsetY ;
}


//비젼 관련.
void CVisionBase::SetLight()
{
    //Lights Setting
    if(m_tVisnStaticPara.iUseLightId < 0 || m_tVisnStaticPara.iUseLightId >= MAX_LIGHT_ID) return ;

    if(m_tCamLightPara.iUseLightCh1)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh1 , m_tCamLightPara.iLtBright1);
    if(m_tCamLightPara.iUseLightCh2)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh2 , m_tCamLightPara.iLtBright2);
    if(m_tCamLightPara.iUseLightCh3)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh3 , m_tCamLightPara.iLtBright3);
}

void CVisionBase::Grab()
{
    //Live Off.
    m_tmLive -> Enabled = false ;

    if(m_bGrabbing) return ;

    //Lights Setting
    SetLight();

    //Camera Setting.
    Cam[m_tVisnStaticPara.iUseCamId] -> SetExposure(m_tCamLightPara.iCamExposure); //카메라 노출 세팅 하고.
    Cam[m_tVisnStaticPara.iUseCamId] -> SetGrabFunc(CamCallback) ; //카메라 콜백 등록.

    //Sleep(50);

    //Grab
    Cam[m_tVisnStaticPara.iUseCamId] -> Grab() ;
    m_bGrabbing = true ;
}

void CVisionBase::Live(bool _bOn)
{
    m_tmLive -> Enabled = _bOn ;
}
bool CVisionBase::GetGrabEnd()
{
    return !m_bGrabbing ;
}

//카메라에 등록해주는 콜백.
void CVisionBase::CamCallback (int _iCamNo)
{
    unsigned char * pImg ;
    int iImgWidth  ;
    int iImgHeight ;
    int iImgBit    ;

    Cam[m_tVisnStaticPara.iUseCamId] -> GetImg(pImg , iImgWidth , iImgHeight , iImgBit);

    m_pImg[m_tVisnStaticPara.iImgId]->SetImage(pImg , iImgWidth , iImgHeight , iImgBit);

    if(m_pPaintCallback)m_pPaintCallback();

    m_bGrabbing = false ;
}



















/*




void __fastcall CVisionBase::PageControlChange(TObject *Sender)
{
    SetViewMode(vmRslt);
    m_pPaintCallback();
}











void CVisionBase::MoveVisnParaFrm(TWinControl * _wcBase)
{
    ParaUi.pcParaBase -> Parent = _wcBase ;
}


void CVisionBase::Calibration()
{
    CAL_Rslt Rslt ;
    bool bRet ;


    bRet = Cal -> Process(m_pImg , &Rslt) ;

    if(!bRet) ShowMessage(SVL_GetErrMsg());

    m_pPaintCallback();
}


//파일 관련.
void CVisionBase::LoadJobFile(bool _bLoad)
{

    //카메라,라이트 파라 로딩.
    LoadCamPara(_bLoad );

    //켈리브레이션 파라 로딩.
    LoadCalPara(_bLoad ) ;

    //알고리즘 파라 로딩.
    AnsiString sJobFolderPath = g_sJobFilePath + m_sVisnName ;

}



void CVisionBase::LoadCalPara(bool _bLoad )
{
    AnsiString sSavePath = GetExeDir("Util\\Vision\\Cal_") + m_sVisnName ;
    Cal->LoaCamLightPara(_bLoad ,sSavePath.c_str(),0) ;
}





bool CVisionBase::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    //int iSel = ParaUi.pcParaBase -> ActivePageIndex ;
    //iSel = iSel -1 ; //카메라 폼은 출력 안하기 때문에.
    //if(iSel < 0) return false ;

    if(Cal -> MouseTrackerDn(Button, Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    if(m_iViewMode < 0) return false ;
    return ProcPkg.MouseTrackerDn(ParaUi.pcParaBase -> ActivePageIndex , Button, Shift, X, Y , _fScaleX , _fScaleY) ;

}

bool CVisionBase::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    //if(ParaUi.pcParaBase==NULL) return false;
    //int iSel = ParaUi.pcParaBase -> ActivePageIndex ;
    //iSel = iSel -1 ; //카메라 폼은 출력 안하기 때문에.

    if(Cal -> MouseTrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    if(m_iViewMode < 0) return false ;
    return ProcPkg.MouseTrackerMv(ParaUi.pcParaBase -> ActivePageIndex , Shift, X, Y , _fScaleX , _fScaleY);
}

void CVisionBase::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    //if(ParaUi.pcParaBase==NULL) return ;
    //int iSel = ParaUi.pcParaBase -> ActivePageIndex ;
    //iSel = iSel -1 ; //카메라 폼은 출력 안하기 때문에.
    //if(iSel < 0) return ;

    Cal -> MouseTrackerUp(Button, Shift, X, Y , _fScaleX , _fScaleY) ;
    if(m_iViewMode < 0) return ;
    ProcPkg.MouseTrackerUp(ParaUi.pcParaBase -> ActivePageIndex , Button, Shift, X, Y , _fScaleX , _fScaleY) ;
}

void CVisionBase::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(ParaUi.pcParaBase==NULL) return ;

    Cal -> PaintTracker(_hHdc , _fScaleX , _fScaleY);
    if(m_iViewMode < 0) return ;
    ProcPkg.PaintTracker(ParaUi.pcParaBase -> ActivePageIndex , _hHdc , _fScaleX , _fScaleY) ;
}

void CVisionBase::PaintRslt (HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(ParaUi.pcParaBase==NULL) return ;

    if(m_iViewMode == vmNone){
        return ;
    }

    else if(m_iViewMode ==vmCal ){
        Cal -> PaintRslt(m_pImg , _hHdc );
    }
    else if(m_iViewMode ==vmAll ){
        ProcPkg.PaintRsltAll(m_pImg , _hHdc);

        

        IGdi *Gdi = GDI_New(_hHdc);

        //필요 없다.
        //Gdi -> SetScale(_fScaleX , _fScaleY);

        Gdi -> m_tPen.Style = psSolid ;
        Gdi -> m_tPen.Color = clYellow ;
        Gdi -> m_tPen.Width = 1  ;

        Gdi -> m_tText.Color = m_bRlstOk ? clLime : clRed ;
        Gdi -> m_tFont.Height = 25 ;
        Gdi -> Text(2 , 2 , m_sRsltMsg.c_str());

        
        if(m_tRsltPnt.x != 0 || m_tRsltPnt.y != 0) {
            Gdi -> m_tPen.Color = clFuchsia ;
            Gdi -> LinePlus((int)m_tRsltPnt.x * _fScaleX , (int)m_tRsltPnt.y * _fScaleY , 5);
            Gdi -> m_tText.Color = clFuchsia ;
            Gdi -> m_tFont.Height = 10 ;
            Gdi -> Text(m_tRsltPnt.x * _fScaleX , m_tRsltPnt.y * _fScaleY , "Align Rslt");
        }

        GDI_Del(Gdi);
    }
    else {
        ProcPkg.PaintRslt   (ParaUi.pcParaBase -> ActivePageIndex , m_pImg , _hHdc);
    }


}

void CVisionBase::PaintCntLine (HDC _hHdc , int _iWidth , int _iHeight)
{

    IGdi *Gdi = GDI_New(_hHdc);

    int iCntrX = _iWidth /2 + Para.iCntOffsetX ;
    int iCntrY = _iHeight/2 + Para.iCntOffsetY ;
    float fRatioX = m_pImg -> GetWidth()  / (float)_iWidth   ;
    float fRatioY = m_pImg -> GetHeight() / (float)_iHeight  ;
    Gdi -> m_tPen.Style = psDot  ;
    Gdi -> m_tPen.Color = clBlue ;
    Gdi -> LinePlus(iCntrX , iCntrY , _iWidth);

    int iRectGap = 30 ;
    int iRectCnt = _iWidth /(iRectGap * 2) ;
    char str[20];

    for(int i = 0 ; i < iRectCnt ; i++ ) {
        if(i%2) {
            Gdi -> m_tPen.Color  = clBlue ;
            Gdi -> m_tText.Color = clBlue ;
        }
        else {
            Gdi -> m_tPen.Color = clRed  ;
            Gdi -> m_tText.Color = clRed ;


        }
        Gdi -> Rect(false , iCntrX - i*iRectGap , iCntrY - i*iRectGap , iCntrX + i*iRectGap , iCntrY + i*iRectGap);

        sprintf(str, "%.3fmm" , iRectGap* i * Cal->Rslt.fYPxRes * fRatioY );
        Gdi -> Text(iCntrX  , iCntrY + i*iRectGap + 5 , str );

        sprintf(str, "%.3fmm" , iRectGap* i * Cal->Rslt.fXPxRes * fRatioX );
        Gdi -> Text(iCntrX + i*iRectGap + 1 , iCntrY + (i%2 ? 5:-5)  , str);
    }

    GDI_Del(Gdi);
}
*/
