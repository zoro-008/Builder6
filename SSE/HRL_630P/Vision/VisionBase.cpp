//---------------------------------------------------------------------------

#include <stdio.h>

#pragma hdrstop

#include "VisionBase.h"
#include "Common.h"

#include "LightUnit.h"
#include "CamUnit.h"
#include "UserINI.h"
#include "UserFile.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CVisionBase * VISN[MAX_VISN_ID] ;

CVisionBase::CVisionBase()
{
    m_pImg = NULL ;

}

CVisionBase::~CVisionBase()
{
}

bool CVisionBase::Init(TVisnStaticPara _tVisnStaticPara)
{
    m_pPaintCallback = NULL ;



    m_tmLive = new TTimer(Application); //Delete 필요 없다.
    m_tmLive -> Enabled = false ;
    m_tmLive -> Interval = 300 ;
    m_tmLive -> OnTimer = tmLiveTimer ;

    m_tVisnStaticPara = _tVisnStaticPara ;

    m_pImg = IMG_New() ;

    memset(&TimeInfo , 0 , sizeof(TTimeInfo));
    memset(&Stat     , 0 , sizeof(TStat    ));          

    return true ;
}

void CVisionBase::Close()
{
    m_tmLive -> Enabled = false ;

    delete m_tmLive ;

    IMG_Del(m_pImg);

    m_pImg = NULL ;

    Stat.bGrabbing = false ;
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

void __fastcall CVisionBase::tmLiveTimer(TObject *Sender)
{
    m_tmLive -> Enabled = false ;
    if(!Stat.bGrabbing)Grab();
    m_tmLive -> Enabled = true ;

}

int CVisionBase::GetCenterX()
{
    return m_pImg->GetWidth () / 2 + m_tCamLightPara.iCntOffsetX ;
}

int CVisionBase::GetCenterY()
{
    return m_pImg->GetHeight() / 2 + m_tCamLightPara.iCntOffsetY ;
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

    if(Stat.bGrabbing) return ;

    TimeInfo.iGrabStart = GetTickCount() ;

    //Lights Setting
    SetLight();

    //Camera Setting.
    Cam[m_tVisnStaticPara.iUseCamId] -> SetExposure(m_tCamLightPara.iCamExposure); //카메라 노출 세팅 하고.
    Cam[m_tVisnStaticPara.iUseCamId] -> SetGrabFunc(CamCallback) ; //카메라 콜백 등록.

    //Sleep(50);

    //Grab
    Cam[m_tVisnStaticPara.iUseCamId] -> Grab() ;
    Stat.bGrabbing = true ;
}

void CVisionBase::Live(bool _bOn)
{
    m_tmLive -> Enabled = _bOn ;
}
bool CVisionBase::GetGrabEnd()
{
    return !Stat.bGrabbing ;
}

//카메라에 등록해주는 콜백.
void CVisionBase::CamCallback (int _iCamNo)
{
    unsigned char * pImg ;
    int iImgWidth  ;
    int iImgHeight ;
    int iImgBit    ;

    TimeInfo.iGrab = GetTickCount() - TimeInfo.iGrabStart ;


    int iImgCpyStartTime = GetTickCount() ;
    Cam[m_tVisnStaticPara.iUseCamId] -> GetImg(pImg , iImgWidth , iImgHeight , iImgBit);
    m_pImg->SetImage(pImg , iImgWidth , iImgHeight , iImgBit);
    TimeInfo.iImgCpy =  GetTickCount() - iImgCpyStartTime ;

    Stat.bDispRslt = g_bSettingMode ; //검사시에는 결과값 디피 안되게. m_bDispRslt 인스펙션시에 On시킴.

    if(m_pPaintCallback)m_pPaintCallback();

    Stat.bGrabbing = false ;


}

void CVisionBase::LoadImg(bool _bLoad)   //카메라 이미지 로드세이브
{
    AnsiString sJobFolderPath ;
    sJobFolderPath = g_sJobFilePath + m_tVisnStaticPara.sVisnName ;

    TUserINI UserINI ;
    AnsiString sVisnName = "Visn" ;
    AnsiString sBitmapPath ;

    int iTemp ;
    if(_bLoad) {
        sBitmapPath = sJobFolderPath + ".Bmp" ;
        m_pImg->LoadBmp(sBitmapPath.c_str());
    }
    else {
        sBitmapPath = sJobFolderPath + ".Bmp" ;
        m_pImg->SaveBmp(sBitmapPath.c_str());
    }
}


TCamLightPara CVisionBase::GetCamLightPara()
{
    return m_tCamLightPara ; 

}

void CVisionBase::SetCamLightPara(TCamLightPara _tCamLightPara)
{
    m_tCamLightPara = _tCamLightPara ;
}

TVisnStaticPara CVisionBase::GetVisnStaticPara()
{
    return m_tVisnStaticPara ;

}

void CVisionBase::LoadCamLightPara(bool _bLoad)
{
    AnsiString sJobFilePath ;
    sJobFilePath = g_sJobFilePath + m_tVisnStaticPara.sVisnName + ".ini" ;

    TUserINI UserINI ;
    AnsiString sVisnName = "CamLight" ;
    AnsiString sBitmapPath ;

    int iTemp ;
    if(_bLoad) {
        UserINI.Load(sJobFilePath , sVisnName , "iCamExposure", m_tCamLightPara.iCamExposure );
        UserINI.Load(sJobFilePath , sVisnName , "iCntOffsetX ", m_tCamLightPara.iCntOffsetX  );
        UserINI.Load(sJobFilePath , sVisnName , "iCntOffsetY ", m_tCamLightPara.iCntOffsetY  );

        UserINI.Load(sJobFilePath , sVisnName , "iUseLightCh1", m_tCamLightPara.iUseLightCh1 );
        UserINI.Load(sJobFilePath , sVisnName , "iLtBright1  ", m_tCamLightPara.iLtBright1   );
        UserINI.Load(sJobFilePath , sVisnName , "iUseLightCh2", m_tCamLightPara.iUseLightCh2 );
        UserINI.Load(sJobFilePath , sVisnName , "iLtBright2  ", m_tCamLightPara.iLtBright2   );
        UserINI.Load(sJobFilePath , sVisnName , "iUseLightCh3", m_tCamLightPara.iUseLightCh3 );
        UserINI.Load(sJobFilePath , sVisnName , "iLtBright3  ", m_tCamLightPara.iLtBright3   );
    }
    else {
        UserINI.Save(sJobFilePath , sVisnName , "iCamExposure", m_tCamLightPara.iCamExposure );
        UserINI.Save(sJobFilePath , sVisnName , "iCntOffsetX ", m_tCamLightPara.iCntOffsetX  );
        UserINI.Save(sJobFilePath , sVisnName , "iCntOffsetY ", m_tCamLightPara.iCntOffsetY  );

        UserINI.Save(sJobFilePath , sVisnName , "iUseLightCh1", m_tCamLightPara.iUseLightCh1 );
        UserINI.Save(sJobFilePath , sVisnName , "iLtBright1  ", m_tCamLightPara.iLtBright1   );
        UserINI.Save(sJobFilePath , sVisnName , "iUseLightCh2", m_tCamLightPara.iUseLightCh2 );
        UserINI.Save(sJobFilePath , sVisnName , "iLtBright2  ", m_tCamLightPara.iLtBright2   );
        UserINI.Save(sJobFilePath , sVisnName , "iUseLightCh3", m_tCamLightPara.iUseLightCh3 );
        UserINI.Save(sJobFilePath , sVisnName , "iLtBright3  ", m_tCamLightPara.iLtBright3   );
    }
}

void CVisionBase::PaintCntLine(HDC _hDc , int _iWidth , int _iHeight)
{
    IGdi *Gdi = GDI_New(_hDc);

    int iCntrX = _iWidth /2 + m_tCamLightPara.iCntOffsetX ;
    int iCntrY = _iHeight/2 + m_tCamLightPara.iCntOffsetY ;
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
    }

    GDI_Del(Gdi);

}

void CVisionBase::Paint (HDC _hDc , EN_DISP_OPTN _iDpOptn ,
                                    int _iDstX, int _iDstY, int _iDstW, int _iDstH ,
                                    int _iSrcX, int _iSrcY, int _iSrcW, int _iSrcH ,
                                    float _fScaleX , float _fScaleY ,int _iThreshold , bool _bCntrLine)
{
         if(doGray   == _iDpOptn) {g_iYameRet = m_pImg->ShowImgGrey  (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doColor  == _iDpOptn) {g_iYameRet = m_pImg->ShowImgColor (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doFalse  == _iDpOptn) {g_iYameRet = m_pImg->ShowImgFalse (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doBinary == _iDpOptn) {g_iYameRet = m_pImg->ShowImgBinary(_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH ,_iThreshold);}
    else if(doRed    == _iDpOptn) {g_iYameRet = m_pImg->ShowImgR     (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doGreen  == _iDpOptn) {g_iYameRet = m_pImg->ShowImgG     (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doBlue   == _iDpOptn) {g_iYameRet = m_pImg->ShowImgB     (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }

    if(_bCntrLine) PaintCntLine(_hDc , _iDstW , _iDstH);
}

void CVisionBase::SetPaintCallback(PaintCallbackFunc _pCalback)
{
    m_pPaintCallback = _pCalback ;
}

void CVisionBase::InspectionThread()
{
    CRunThread<InspectionFunc> * func = new CRunThread<InspectionFunc>(&Inspection) ;
}

void CVisionBase::SetActivPage(EN_PAGE_ID _iActivPage)
{
    m_iActivPage = _iActivPage ;
}

EN_PAGE_ID CVisionBase::GetActivPage()
{
    return m_iActivPage ;
}













