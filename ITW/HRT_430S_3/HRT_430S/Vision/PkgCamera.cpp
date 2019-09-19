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
   //ȭ�鿡�� �ٽ� �׸��Ƿ� ���⼭ �޼��� ���� �ʿ� ����.
}




//���� ���� �Լ�.
//==============================================================================
bool CCamera_V01::Init()
{

    m_sCrntJobFilePath = "" ;

    //m_pCam = new CImi() ;
    //m_pCam -> Init();

    m_pTrainImg  = new CImage() ;

    m_pCrntImg   = new CImage() ;

    //imi ī�޶� �Ѱ��� �޾Ƽ� �ٽ� �����Ѵ�.    �ϴ� ī�޶� ���������� ������ ������ �־ .... �ּ�.
    //MPara.iCamId.SetMaxVal(m_pCam -> GetMaxCamCnt()) ;

    //m_bGrabEnd = false ;

    //m_hGrabEnd = CreateEvent(NULL,TRUE, FALSE,NULL);
    //bManualReset == false�� ResetEvent�� false ���������
    //bManualReset == true�� �ڵ����� false ��
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


        //�˻� �� ���� ����.
bool CCamera_V01::GetTrainAvailable()
{
    return false ; //������ ����Ѵ�.
}
bool CCamera_V01::Train(CImage * _pImg) //ī�޶� ��Ʈ�� ��� �̹����� Train �̹����� ���縦 �صд�.
{
    m_sErrMsg = "";
    return true ;


}

bool CCamera_V01::CaptureTrainImg() //ī�޶� ��Ʈ�� ��� �̹����� Train �̹����� ���縦 �صд�.
{
    m_sErrMsg = "";

    *m_pTrainImg = *m_pCrntImg ;

    return true ;
}


void CCamera_V01::RsltClear() //�˻� ������� �˻����� Ŭ���� �ѹ� �ϰ� �Ѵ�.
{
    Rslt.Clear();
}

bool CCamera_V01::GetRslt()
{
    return Rslt.bRsltOk ;
}

//ī�޶� ������ִ� �ݹ�.
void CCamera_V01::CamCallback (int _iCamNo)
{
     //���� ���.
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


bool CCamera_V01::Run(CImage * _pImg) //ī�޶� ���� ���.._pImg �̰��� �Ⱦ���.
{
    m_sErrMsg = "";
    Rslt.bRsltOk = true ;
    //Lights Setting
    //if(m_tCamLightPara.iUseLightCh1)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh1 , m_tCamLightPara.iLtBright1);
    //if(m_tCamLightPara.iUseLightCh2)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh2 , m_tCamLightPara.iLtBright2);
    //if(m_tCamLightPara.iUseLightCh3)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh3 , m_tCamLightPara.iLtBright3);
    //rs232�� ���..������ ���� �ʿ�.



    //Camera Setting.
    //Imi[MPara.iCamId.GetEnum()] -> SetBrightness(CPara.iBrightness.GetInt());
    //Imi[MPara.iCamId.GetEnum()] -> SetSharpness (CPara.iSharpness .GetInt());
    //Imi[MPara.iCamId.GetEnum()] -> SetGain      (CPara.iGain      .GetInt());
    //Imi[MPara.iCamId.GetEnum()] -> SetShutter   (CPara.iShutter   .GetInt());

    //Imi[MPara.iCamId.GetEnum()] -> SetGrabFunc(CamCallback) ; //ī�޶� �ݹ� ���.

    //Cam[MPara.iCamId.GetEnum()] -> SetBrightness(UserPara.iBrightness.GetInt());
    //Cam[MPara.iCamId.GetEnum()] -> SetSharpness (CPara.iSharpness .GetInt());
    //Cam[MPara.iCamId.GetEnum()] -> SetGain      (CPara.iGain      .GetInt());
    //Cam[MPara.iCamId.GetEnum()] -> SetShutter   (CPara.iShutter   .GetInt());

    //Cam[MPara.iCamId.GetEnum()] -> SetExposure(m_tCamLightPara.iCamExposure);



//    CLight::m_apLight[MPara.iLightId] -> ApplyUserPara(LightPara  ) ;
    //Cam[MPara.iCamId.GetEnum()] -> SetGrabFunc  (CamCallback) ;
    Cam[MPara.iCamId.GetEnum()] -> ApplyPara(    ) ;


    Cam[MPara.iCamId.GetEnum()] -> SetGrabFunc(CamCallback) ; //ī�޶� �ݹ� ���.


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

    //m_hGrabEnd�� false ������
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

    //2�� �Ѹ��� �Ǿ ����.
    //SendPaintMsg();


    return true ;

}

String CCamera_V01::GetErrMsg()
{
    return m_sErrMsg ;
}



        //ȭ�� ���.
void CCamera_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
/* Ʈ�����̶� �׳� �˻綧�� ���� ������ �������... CVision���� ó��...

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



        //�̹��� ȭ�� Ŭ�� �̺�Ʈ
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

        //�Ķ���� ���÷���
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

        //�ε� ���̺�.
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
        //�ε� ���̺�.
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



