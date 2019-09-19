//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgCamera.h"
#include "UserINI.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)

CCamera_V01::CCamera_V01()
{

}

CCamera_V01::~CCamera_V01()
{
}

void CCamera_V01::OnTrackerChanged()
{
   //화면에서 다시 그리므로 여기서 메세지 날릴 필요 없다.
}




//순수 가상 함수.
//==============================================================================
bool CCamera_V01::Init()
{

    m_sCrntJobFilePath = "" ;

    //m_pCam = new CImi() ;
    //m_pCam -> Init();

    m_pTrainImg  = new CImage() ;

    m_pCrntImg   = new CImage() ;

    //imi 카메라 총갯수 받아서 다시 세팅한다.    일단 카메라 끊어졌을때 문제의 소지가 있어서 .... 주석.
    //MPara.iCamId.SetMaxVal(m_pCam -> GetMaxCamCnt()) ;

    //m_bGrabEnd = false ;

    //m_hGrabEnd = CreateEvent(NULL,TRUE, FALSE,NULL);
    //bManualReset == false면 ResetEvent로 false 시켜줘야함
    //bManualReset == true면 자동으로 false 됨
    m_hGrabEnd = CreateEvent(NULL,FALSE, TRUE,NULL); //auto mode signaled

    return true ;

}
bool CCamera_V01::Close()
{
    if(m_pTrainImg){delete m_pTrainImg  ; m_pTrainImg  = NULL ;}
    if(m_pCrntImg ){delete m_pCrntImg   ; m_pCrntImg   = NULL ;}

    CloseHandle(m_hGrabEnd);
    //delete m_pCam       ; m_pCam       = NULL ;

    return true ;
}


        //검사 및 러닝 관련.
bool CCamera_V01::GetTrainAvailable()
{
    return false ; //영상을 취득한다.
}
bool CCamera_V01::Train(CImage * _pImg) //카메라 파트의 경우 이미지를 Train 이미지에 복사를 해둔다.
{
    m_sErrMsg = "";
    return true ;


}

bool CCamera_V01::CaptureTrainImg() //카메라 파트의 경우 이미지를 Train 이미지에 복사를 해둔다.
{
    m_sErrMsg = "";

    *m_pTrainImg = *m_pCrntImg ;

    return true ;
}


void CCamera_V01::RsltClear() //검사 결과값을 검사전에 클리어 한번 하고 한다.
{
    Rslt.Clear();
}

bool CCamera_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}

//카메라에 등록해주는 콜백.
void CCamera_V01::CamCallback (int _iCamNo)
{
     //영상 취득.
    unsigned char * pImg ;
    int iImgWidth  ;
    int iImgHeight ;
    int iImgBit    ;


    //Imi[MPara.iCamId.GetEnum()] -> GetImg(pImg , iImgWidth , iImgHeight , iImgBit);
    Cam[MPara.iCamId.GetEnum()] -> GetImg(pImg , iImgWidth , iImgHeight , iImgBit);
    m_pCrntImg->SetImage(pImg , iImgWidth , iImgHeight , iImgBit);

    //m_bGrabEnd = true ;

    SetEvent(m_hGrabEnd);

}


bool CCamera_V01::Run(CImage * _pImg) //카메라 같은 경우.._pImg 이것을 안쓴다.
{
    m_sErrMsg = "";
    Rslt.bRsltOk = true ;
    //Lights Setting
    //if(m_tCamLightPara.iUseLightCh1)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh1 , m_tCamLightPara.iLtBright1);
    //if(m_tCamLightPara.iUseLightCh2)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh2 , m_tCamLightPara.iLtBright2);
    //if(m_tCamLightPara.iUseLightCh3)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh3 , m_tCamLightPara.iLtBright3);
    //rs232일 경우..적절한 슬립 필요.



    //Camera Setting.
    //Imi[MPara.iCamId.GetEnum()] -> SetBrightness(CPara.iBrightness.GetInt());
    //Imi[MPara.iCamId.GetEnum()] -> SetSharpness (CPara.iSharpness .GetInt());
    //Imi[MPara.iCamId.GetEnum()] -> SetGain      (CPara.iGain      .GetInt());
    //Imi[MPara.iCamId.GetEnum()] -> SetShutter   (CPara.iShutter   .GetInt());

    //Imi[MPara.iCamId.GetEnum()] -> SetGrabFunc(CamCallback) ; //카메라 콜백 등록.

    //Cam[MPara.iCamId.GetEnum()] -> SetBrightness(UserPara.iBrightness.GetInt());
    //Cam[MPara.iCamId.GetEnum()] -> SetSharpness (CPara.iSharpness .GetInt());
    //Cam[MPara.iCamId.GetEnum()] -> SetGain      (CPara.iGain      .GetInt());
    //Cam[MPara.iCamId.GetEnum()] -> SetShutter   (CPara.iShutter   .GetInt());

    //Cam[MPara.iCamId.GetEnum()] -> SetExposure(m_tCamLightPara.iCamExposure);



//    CLight::m_apLight[MPara.iLightId] -> ApplyUserPara(LightPara  ) ;
    //Cam[MPara.iCamId.GetEnum()] -> SetGrabFunc  (CamCallback) ;
    Cam[MPara.iCamId.GetEnum()] -> ApplyPara(    ) ;


    Cam[MPara.iCamId.GetEnum()] -> SetGrabFunc(CamCallback) ; //카메라 콜백 등록.


    //Grab
    //m_bGrabEnd = false ;
    //if(!Imi[MPara.iCamId.GetEnum()] -> Grab()){
    //    //m_bGrabEnd = true ;
    //    Rslt.bRsltOk = false ;
    //    m_sErrMsg = "ERROR : Grab Failed!"  ;
    //    return false ;
    //}

    //if(!Cam[MPara.iCamId.GetEnum()] -> Grab()){
    //    //m_bGrabEnd = true ;
    //    Rslt.bRsltOk = false ;double dStartTime = GetTickCount();
    //    m_sErrMsg = "ERROR : Grab Failed!"  ;
    //    return false ;ResetEvent(m_hGrabEnd);
    //}DWORD dwRet = WaitForSingleObject(m_hGrabEnd , 5000) ;

    //m_hGrabEnd을 false 시켜줌
    ResetEvent(m_hGrabEnd);

    if(!Cam[MPara.iCamId.GetEnum()] -> Grab()){
        //m_bGrabEnd = true ;
        Rslt.bRsltOk = false ;double dStartTime = GetTickCount();
        m_sErrMsg = "ERROR : Grab Failed!"  ;
        return false ;
    }


    DWORD dwRet = WaitForSingleObject(m_hGrabEnd , 5000) ;


    if(WAIT_FAILED ==dwRet){
        //m_bGrabEnd = true ;
        Rslt.bRsltOk = false ;
        m_sErrMsg = "ERROR : WaitFailed !" ;
        return false ;
    }
    else if(WAIT_TIMEOUT ==dwRet){
        Rslt.bRsltOk = false ;
        m_sErrMsg = "ERROR : TimeOver(over 5sec)!" ;
        return false ;

    }


    /*
    while(!m_bGrabEnd){
        Sleep(1);


        if(GetTickCount() - dStartTime > 5000){
            m_bGrabEnd = true ;
            m_sErrMsg = "Grab TimeOut(over 5sec)!" ;
            return false ;

        }
    }*/

    Rslt.bRsltOk = true ;

    //2번 뿌리게 되어서 없앰.
    //SendPaintMsg();


    return true ;

}

String CCamera_V01::GetErrMsg()
{
    return m_sErrMsg ;
}



        //화면 출력.
void CCamera_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
/* 트레인이랑 그냥 검사때랑 시점 구분이 어려워서... CVision에서 처리...

         if(doGray   == Disp.eMode) {m_pImg->ShowImgGrey  (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doColor  == Disp.eMode) {m_pImg->ShowImgColor (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doFalse  == Disp.eMode) {m_pImg->ShowImgFalse (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doBinary == Disp.eMode) {m_pImg->ShowImgBinary(_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH ,_iThreshold);}
    else if(doRed    == Disp.eMode) {m_pImg->ShowImgR     (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doGreen  == Disp.eMode) {m_pImg->ShowImgG     (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }
    else if(doBlue   == Disp.eMode) {m_pImg->ShowImgB     (_hDc, _iDstX , _iDstY , _iDstW , _iDstH , _iSrcX , _iSrcY , _iSrcW , _iSrcH);             }

    if(_bCntrLine) PaintCntLine(_hDc , _iDstW , _iDstH);
*/





    CGdi *Gdi = new CGdi(_hDc);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    delete Gdi;


}




void CCamera_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CGdi *Gdi = new CGdi(_hDc);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    delete Gdi;

}



        //이미지 화면 클릭 이벤트
TTracker * CCamera_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    return NULL ;
}


TTracker * CCamera_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    return NULL ;
}

void CCamera_V01::MouseUp(TMouseButton Button,TShiftState Shift )
{
}

        //파라미터 디스플레이
void CCamera_V01::UpdateMasterPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;

        _vePara -> Values[MPara.iCamId.GetName()] = MPara.iCamId.GetEnum();
    }
    else {
        MPara.iCamId.SetEnumFromStr(_vePara -> Values[MPara.iCamId.GetName()]) ;

        UpdateMasterPara(true , _vePara );
    }
}

void CCamera_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;

        _vePara -> Values[CPara.iBrightness.GetName()] = CPara.iBrightness .GetInt();
        _vePara -> Values[CPara.iSharpness .GetName()] = CPara.iSharpness  .GetInt();
        _vePara -> Values[CPara.iGain      .GetName()] = CPara.iGain       .GetInt();
        _vePara -> Values[CPara.iShutter   .GetName()] = CPara.iShutter    .GetInt();
    }
    else {
        CPara.iBrightness.SetIntFromStr(_vePara -> Values[CPara.iBrightness.GetName()]) ;
        CPara.iSharpness .SetIntFromStr(_vePara -> Values[CPara.iSharpness .GetName()]) ;
        CPara.iGain      .SetIntFromStr(_vePara -> Values[CPara.iGain      .GetName()]) ;
        CPara.iShutter   .SetIntFromStr(_vePara -> Values[CPara.iShutter   .GetName()]) ;


        UpdateCommonPara(true , _vePara );
    }

}
void CCamera_V01::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        //_vePara -> DisplayOptions >> doColumnTitles ;

    }
    else {

        UpdateLocalPara(true , _vePara );
    }

}

        //로드 세이브.
void CCamera_V01::LoadSave(bool _bLoad , String _sFolderPath , String _sVisnName)
{
    String sIniPath        = _sFolderPath + "\\Vision\\" + _sVisnName + "_" + m_sName + "_" + GetPkgName() + ".ini" ;
    String sCrntImagePath  = _sFolderPath + "\\Vision\\" + _sVisnName + "_" + m_sName + "_CrntImage.bmp"  ;
    String sTrainImagePath = _sFolderPath + "\\Vision\\" + _sVisnName + "_" + m_sName + "_TrainImage.bmp" ;

    TUserINI Ini ;
    if(_bLoad) {
        m_pCrntImg   -> LoadBmp(sCrntImagePath .c_str());
        m_pTrainImg  -> LoadBmp(sTrainImagePath.c_str());

        Ini.Load(sIniPath, "MasterPara" , MPara.iCamId     .GetName(), MPara.iCamId     .GetStrPnt());

        Ini.Load(sIniPath, "CommonPara" , CPara.iBrightness.GetName(), CPara.iBrightness.GetStrPnt());
        Ini.Load(sIniPath, "CommonPara" , CPara.iSharpness .GetName(), CPara.iSharpness .GetStrPnt());
        Ini.Load(sIniPath, "CommonPara" , CPara.iGain      .GetName(), CPara.iGain      .GetStrPnt());
        Ini.Load(sIniPath, "CommonPara" , CPara.iShutter   .GetName(), CPara.iShutter   .GetStrPnt());

    }
    else {
        if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath       ) ;
        if(m_sCrntJobFilePath != "")Ini.ClearFile(sCrntImagePath ) ;
        if(m_sCrntJobFilePath != "")Ini.ClearFile(sTrainImagePath) ;

        m_pCrntImg   -> SaveBmp(sCrntImagePath .c_str());
        m_pTrainImg  -> SaveBmp(sTrainImagePath.c_str());

        Ini.Save(sIniPath, "MasterPara" , MPara.iCamId     .GetName(), MPara.iCamId     .GetStrPnt());

        Ini.Save(sIniPath, "CommonPara" , CPara.iBrightness.GetName(), CPara.iBrightness.GetStrPnt());
        Ini.Save(sIniPath, "CommonPara" , CPara.iSharpness .GetName(), CPara.iSharpness .GetStrPnt());
        Ini.Save(sIniPath, "CommonPara" , CPara.iGain      .GetName(), CPara.iGain      .GetStrPnt());
        Ini.Save(sIniPath, "CommonPara" , CPara.iShutter   .GetName(), CPara.iShutter   .GetStrPnt());
    }

    m_sCrntJobFilePath = _sFolderPath ;

}

/*
        //로드 세이브.
void CCamera_V01::LoadSave(bool _bLoad , String _sFolderPath , String _sVisnName)
{
    String sIniPath        = _sFolderPath + "\\Vision\\" + _sVisnName + "_" + m_sName + "_" + GetPkgName() + ".ini" ;
    String sCrntImagePath  = _sFolderPath + "\\Vision\\" + _sVisnName + "_" + m_sName + "_CrntImage.bmp"  ;
    String sTrainImagePath = _sFolderPath + "\\Vision\\" + _sVisnName + "_" + m_sName + "_TrainImage.bmp" ;

    TUserINI Ini ;
    if(_bLoad) {
        m_pCrntImg   -> LoadBmp(sCrntImagePath .c_str());
        m_pTrainImg  -> LoadBmp(sTrainImagePath.c_str());

        Ini.Load(sIniPath, "MasterPara" , MPara.iCamId     .GetName(), MPara.iCamId     .GetStrPnt());

        Ini.Load(sIniPath, "CommonPara" , CPara.iBrightness.GetName(), CPara.iBrightness.GetStrPnt());
        Ini.Load(sIniPath, "CommonPara" , CPara.iSharpness .GetName(), CPara.iSharpness .GetStrPnt());
        Ini.Load(sIniPath, "CommonPara" , CPara.iGain      .GetName(), CPara.iGain      .GetStrPnt());
        Ini.Load(sIniPath, "CommonPara" , CPara.iShutter   .GetName(), CPara.iShutter   .GetStrPnt());

    }
    else {
        if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath       ) ;
        if(m_sCrntJobFilePath != "")Ini.ClearFile(sCrntImagePath ) ;
        if(m_sCrntJobFilePath != "")Ini.ClearFile(sTrainImagePath) ;

        m_pCrntImg   -> SaveBmp(sCrntImagePath .c_str());
        m_pTrainImg  -> SaveBmp(sTrainImagePath.c_str());

        Ini.Save(sIniPath, "MasterPara" , MPara.iCamId     .GetName(), MPara.iCamId     .GetStrPnt());

        Ini.Save(sIniPath, "CommonPara" , CPara.iBrightness.GetName(), CPara.iBrightness.GetStrPnt());
        Ini.Save(sIniPath, "CommonPara" , CPara.iSharpness .GetName(), CPara.iSharpness .GetStrPnt());
        Ini.Save(sIniPath, "CommonPara" , CPara.iGain      .GetName(), CPara.iGain      .GetStrPnt());
        Ini.Save(sIniPath, "CommonPara" , CPara.iShutter   .GetName(), CPara.iShutter   .GetStrPnt());
    }

    m_sCrntJobFilePath = _sFolderPath ;

}
*/



