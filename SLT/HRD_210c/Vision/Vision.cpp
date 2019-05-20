//---------------------------------------------------------------------------

#include <stdio.h>

#pragma hdrstop

#include "Vision.h"
#include "Common.h"
#include "UserINI.h"
#include "UserFile.h"
#include "SLogUnit.h"



//---------------------------------------------------------------------------

#pragma package(smart_init)


CVision * VISN[MAX_VISN_ID] ;
CVision::CVision()
{

}

CVision::~CVision()
{
    //Close();
    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;
    //close();  //???
}

bool CVision::Init(AnsiString _sVisnName)
{

    m_sVisnName = _sVisnName ;


    Img = IMG_New() ;

    PaintCallback = NULL ;

    m_bGrabbing = false ;

    Cal = new CCal () ;

    m_tRsltPnt.x = 0.0 ;
    m_tRsltPnt.y = 0.0 ;


    tmLive = new TTimer(Application); //Delete �ʿ� ����.
    tmLive -> Enabled = false ;
    tmLive -> Interval = 300 ;
    tmLive -> OnTimer = tmLiveTimer ;


    return true ;
}

void CVision::close()
{
    tmLive -> Enabled = false ;

    m_bGrabbing = false ;

    ProcPkg.DelProcList();
    delete Cal ;

    //��Ʈ �Ҵ�Ǿ� ������ �˾Ƽ� ������.
    //delete ParaUi.pcParaBase ;

    IMG_Del(Img);

    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;
}



void __fastcall CVision::PageControlChange(TObject *Sender)
{
    SetViewMode(vmRslt);
    PaintCallback();
}

void CVision::SetViewMode(EN_VIEW_MODE _iMode)
{
    //if(m_iViewMode == _iMode) return ;
    m_iViewMode = _iMode ;

    for(int i = 0 ; i < ProcPkg.m_iProcCnt ; i++) {
        ProcPkg.SetTrackerVisible(i , false);
    }
    Cal -> SetTrackerVisible(false);


    if(m_iViewMode ==vmNone){
    }

    else if(m_iViewMode ==vmCal ){
        Cal -> SetTrackerVisible(true);
    }

    else if(m_iViewMode == vmAll ){
    }
    else if(m_iViewMode == vmRslt){
        ProcPkg.SetTrackerVisible(ParaUi.pcParaBase -> ActivePageIndex , true);
    }

    
}

int CVision::GetViewMode()
{
    return m_iViewMode ;

}

int CVision::GetCenterX()
{
    return Img->GetWidth () / 2 + Para.iCntOffsetX ;
}

int CVision::GetCenterY()
{
    return Img->GetHeight() / 2 + Para.iCntOffsetY ;
}

void CVision::SetPaintCallback(PaintCallbackFunc _pCalback)
{
    PaintCallback = _pCalback ;
    ProcPkg.SetPaintCallback(_pCalback) ;
    Cal -> SetPaintCallback(_pCalback) ;

}

void __fastcall CVision::tmLiveTimer(TObject *Sender)
{
    tmLive -> Enabled = false ;                             

    if(!m_bGrabbing)Grab();

    tmLive -> Enabled = true ;

}

//ī�޶� ������ִ� �ݹ�.
void CVision::CamCallback (int _iCamNo)
{


    unsigned char * pImg ;
    int iImgWidth  ;
    int iImgHeight ;
    int iImgBit    ;


    Cam[Para.iUseCamNo] -> GetImg(pImg , iImgWidth , iImgHeight , iImgBit);

    Img->SetImage(pImg , iImgWidth , iImgHeight , iImgBit);

    //m_bGrabbing = false ;

    if(PaintCallback)PaintCallback();

    m_bGrabbing = false ;
}

//�Ķ� �� ����.
void CVision::MakeVisnParaFrm(TWinControl * _wcBase)
{
    ParaUi.pcParaBase = new TPageControl(_wcBase);
    ParaUi.pcParaBase -> Parent = _wcBase ;  //ParaUi.pcParaBase -> Visible = false ;
    ParaUi.pcParaBase -> TabPosition = tpRight ;
    ParaUi.pcParaBase -> Align = alClient ;

    ProcPkg.ShowParaForm(ParaUi.pcParaBase) ;

    ParaUi.pcParaBase -> OnChange = PageControlChange ;

}

void CVision::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 70  ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;

}


void CVision::MoveVisnParaFrm(TWinControl * _wcBase)
{
    ParaUi.pcParaBase -> Parent = _wcBase ;
}


void CVision::Calibration()
{
    CAL_Rslt Rslt ;
    bool bRet ;


    bRet = Cal -> Process(Img , &Rslt) ;

    if(!bRet) ShowMessage(SVL_GetErrMsg());

    PaintCallback();
}


//���� ����.
void CVision::LoadJobFile(bool _bLoad)
{

    //ī�޶�,����Ʈ �Ķ� �ε�.
    LoadCamPara(_bLoad );

    //�̸��극�̼� �Ķ� �ε�.
    LoadCalPara(_bLoad ) ;

    //�˰����� �Ķ� �ε�.
    AnsiString sJobFolderPath = g_sJobFilePath + m_sVisnName ;
    ProcPkg.LoadPara(_bLoad , sJobFolderPath);

}

void CVision::LoadCamPara(bool _bLoad)
{
    AnsiString sJobFolderPath ;
    sJobFolderPath = g_sJobFilePath + m_sVisnName ;

    TUserINI UserINI ;
    AnsiString sVisnName = "Visn" ;
    AnsiString sBitmapPath ;

    int iTemp ;
    if(_bLoad) {                                                            
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iUseCamNo   ", iTemp             );Para.iUseCamNo   =(EN_CAM_ID)iTemp ;
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iCamExposure", Para.iCamExposure );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iCntOffsetX ", Para.iCntOffsetX  );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iCntOffsetY ", Para.iCntOffsetY  );

        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iUseLightNo ", iTemp             );Para.iUseLightNo =(EN_LIGHT_ID)iTemp ;
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iUseLightCh1", Para.iUseLightCh1 );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iLtBright1  ", Para.iLtBright1   );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iUseLightCh2", Para.iUseLightCh2 );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iLtBright2  ", Para.iLtBright2   );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iUseLightCh3", Para.iUseLightCh3 );
        UserINI.Load(sJobFolderPath + ".ini", sVisnName , "iLtBright3  ", Para.iLtBright3   );
        if(sJobFolderPath != ""){
            sBitmapPath = sJobFolderPath + ".Bmp" ;
            Img->LoadBmp(sBitmapPath.c_str());
        }
    }
    else {
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iUseCamNo   ", Para.iUseCamNo    );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iCamExposure", Para.iCamExposure );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iCntOffsetX ", Para.iCntOffsetX  );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iCntOffsetY ", Para.iCntOffsetY  );

        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iUseLightNo ", Para.iUseLightNo  );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iUseLightCh1", Para.iUseLightCh1 );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iLtBright1  ", Para.iLtBright1   );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iUseLightCh2", Para.iUseLightCh2 );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iLtBright2  ", Para.iLtBright2   );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iUseLightCh3", Para.iUseLightCh3 );
        UserINI.Save(sJobFolderPath + ".ini", sVisnName , "iLtBright3  ", Para.iLtBright3   );
        if(sJobFolderPath != ""){
            sBitmapPath = sJobFolderPath + ".Bmp" ;
            Img->SaveBmp(sBitmapPath.c_str());
        }
    }
}

void CVision::LoadCalPara(bool _bLoad )
{
    AnsiString sSavePath = GetExeDir("Util\\Vision\\Cal_") + m_sVisnName ;
    Cal->LoadPara(_bLoad ,sSavePath.c_str(),0) ;
}


//���� ����.
void CVision::SetLight()
{
    //Lights Setting
    if(Para.iUseLightNo < 0 || Para.iUseLightNo >= MAX_LIGHT_ID) return ;

    if(Para.iUseLightCh1)Light[Para.iUseLightNo] -> SetBright(Para.iUseLightCh1 , Para.iLtBright1);
    if(Para.iUseLightCh2)Light[Para.iUseLightNo] -> SetBright(Para.iUseLightCh2 , Para.iLtBright2);
    if(Para.iUseLightCh3)Light[Para.iUseLightNo] -> SetBright(Para.iUseLightCh3 , Para.iLtBright3);
}

void CVision::Grab()
{
    //Live Off.
    tmLive -> Enabled = false ;

    if(m_bGrabbing) return ;

    //Lights Setting
    SetLight();

    //Camera Setting.
    Cam[Para.iUseCamNo] -> SetExposure(Para.iCamExposure);
    Cam[Para.iUseCamNo] -> SetGrabFunc(CamCallback) ;

    //Sleep(50);

    //Grab
    Cam[Para.iUseCamNo] -> Grab() ;
    m_bGrabbing = true ;
}

void CVision::Live(bool _bOn)
{
    tmLive -> Enabled = _bOn ;
}
bool CVision::GetGrabEnd()
{
    return !m_bGrabbing ;
}
bool CVision::Inspect()
{
    int iSel = ParaUi.pcParaBase -> ActivePageIndex ;
    //iSel = iSel -1 ; //ī�޶� ���� ��� ���ϱ� ������.
    if(iSel < 0) return true ;
    bool bRet ;

    EDG_Rslt EdgRslt ;
    LIN_Rslt LinRslt ;
    LCP_Rslt LcpRslt ;
    RCT_Rslt RctRslt ;
    TSD_Rslt TsdRslt ;
    BLB_Rslt BlbRslt ;
    CAL_Rslt CalRslt ;
    CCL_Rslt CclRslt ;
    OCV_Rslt OcvRslt ;
    WRT_Rslt WrtRslt ;

    switch(ProcPkg.GetProcType(iSel)) {
        default    : bRet = false ;                                 break ;
        case ptEdg : bRet = ProcPkg.Process(iSel , Img , &EdgRslt); break ;
        case ptLin : bRet = ProcPkg.Process(iSel , Img , &LinRslt); break ;
        case ptLcp : bRet = ProcPkg.Process(iSel , Img , &LcpRslt); break ;
        case ptRct : bRet = ProcPkg.Process(iSel , Img , &RctRslt); break ;
        case ptTsd : bRet = ProcPkg.Process(iSel , Img , &TsdRslt); break ;
        case ptBlb : bRet = ProcPkg.Process(iSel , Img , &BlbRslt); break ;
        case ptCal : bRet = ProcPkg.Process(iSel , Img , &CalRslt); break ;
        case ptCcl : bRet = ProcPkg.Process(iSel , Img , &CclRslt); break ;
        case ptOcv : bRet = ProcPkg.Process(iSel , Img , &OcvRslt); break ;
        case ptWrt : bRet = ProcPkg.Process(iSel , Img , &WrtRslt); break ;
    }

    return bRet ;
}

bool CVision::Train()
{
    int iSel = ParaUi.pcParaBase -> ActivePageIndex ;
    if(iSel < 0) return true ;
    bool bRet ;

    switch(ProcPkg.GetProcType(iSel)) {
        default    : ShowMessage("ƼĪ�� �ʿ�����ϴ�.!"); bRet = false ; break ;
        case ptOcv : bRet = ProcPkg.Train(iSel , Img );                   break ;
    }

    return bRet ;
}


bool CVision::MouseTrackerDn(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    //int iSel = ParaUi.pcParaBase -> ActivePageIndex ;
    //iSel = iSel -1 ; //ī�޶� ���� ��� ���ϱ� ������.
    //if(iSel < 0) return false ;

    if(Cal -> MouseTrackerDn(Button, Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    if(m_iViewMode < 0) return false ;
    return ProcPkg.MouseTrackerDn(ParaUi.pcParaBase -> ActivePageIndex , Button, Shift, X, Y , _fScaleX , _fScaleY) ;

}

bool CVision::MouseTrackerMv(TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    //if(ParaUi.pcParaBase==NULL) return false;
    //int iSel = ParaUi.pcParaBase -> ActivePageIndex ;
    //iSel = iSel -1 ; //ī�޶� ���� ��� ���ϱ� ������.

    if(Cal -> MouseTrackerMv(Shift, X, Y , _fScaleX , _fScaleY)) return true ;

    if(m_iViewMode < 0) return false ;
    return ProcPkg.MouseTrackerMv(ParaUi.pcParaBase -> ActivePageIndex , Shift, X, Y , _fScaleX , _fScaleY);
}

void CVision::MouseTrackerUp(TMouseButton Button, TShiftState Shift, int X, int Y , float _fScaleX , float _fScaleY)
{
    //if(ParaUi.pcParaBase==NULL) return ;
    //int iSel = ParaUi.pcParaBase -> ActivePageIndex ;
    //iSel = iSel -1 ; //ī�޶� ���� ��� ���ϱ� ������.
    //if(iSel < 0) return ;

    Cal -> MouseTrackerUp(Button, Shift, X, Y , _fScaleX , _fScaleY) ;
    if(m_iViewMode < 0) return ;
    ProcPkg.MouseTrackerUp(ParaUi.pcParaBase -> ActivePageIndex , Button, Shift, X, Y , _fScaleX , _fScaleY) ;
}

void CVision::PaintTracker(HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(ParaUi.pcParaBase==NULL) return ;

    Cal -> PaintTracker(_hHdc , _fScaleX , _fScaleY);
    if(m_iViewMode < 0) return ;
    ProcPkg.PaintTracker(ParaUi.pcParaBase -> ActivePageIndex , _hHdc , _fScaleX , _fScaleY) ;
}

void CVision::PaintRslt (HDC _hHdc , float _fScaleX , float _fScaleY)
{
    if(ParaUi.pcParaBase==NULL) return ;

    if(m_iViewMode == vmNone){
        return ;
    }

    else if(m_iViewMode ==vmCal ){
        Cal -> PaintRslt(Img , _hHdc );
    }
    else if(m_iViewMode ==vmAll ){
        ProcPkg.PaintRsltAll(Img , _hHdc);

        

        IGdi *Gdi = GDI_New(_hHdc);

        //�ʿ� ����.
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
        ProcPkg.PaintRslt   (ParaUi.pcParaBase -> ActivePageIndex , Img , _hHdc);
    }


}

void CVision::PaintCntLine (HDC _hHdc , int _iWidth , int _iHeight)
{

    IGdi *Gdi = GDI_New(_hHdc);

    int iCntrX = _iWidth /2 + Para.iCntOffsetX ;
    int iCntrY = _iHeight/2 + Para.iCntOffsetY ;
    float fRatioX = Img -> GetWidth()  / (float)_iWidth   ;
    float fRatioY = Img -> GetHeight() / (float)_iHeight  ;
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