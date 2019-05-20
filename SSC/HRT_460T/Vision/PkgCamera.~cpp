//---------------------------------------------------------------------------
#pragma hdrstop
#include "PkgCamera.h"
#include "UserINI.h"
#include "GlobalMan.h"
#include "SLogUnit.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
REGIST_STATICLLINK_CPP(CCamera_V01)
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
    Prop.bUseTrain = false ;
    Prop.bCamera   = true  ;

    //m_pCam = new CImi() ;
    //m_pCam -> Init();

    CamPara   = NULL ;
    LightPara = NULL ;


    m_pTrainImg  = new CImage() ;

    m_pCrntImg   = new CImage() ;




    //m_bGrabEnd = false ;

    //m_hGrabEnd = CreateEvent(NULL,TRUE, FALSE,NULL);
    m_hGrabEnd = CreateEvent(NULL,FALSE, TRUE,NULL); //auto mode signaled


    Cal.Init();

    return true ;

}
bool CCamera_V01::Close()
{
    if(m_pTrainImg){delete m_pTrainImg  ; m_pTrainImg  = NULL ;}
    if(m_pCrntImg ){delete m_pCrntImg   ; m_pCrntImg   = NULL ;}

    if(CamPara   ){delete CamPara   ; CamPara   =NULL;}
    if(LightPara ){delete LightPara ; LightPara =NULL;}

    CloseHandle(m_hGrabEnd);
    //delete m_pCam       ; m_pCam       = NULL ;

    Cal.Close();

    return true ;
}


        //검사 및 러닝 관련.
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

CCamera_V01::TMasterPara * CCamera_V01::GetMPara()
{
    return &MPara ;
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


    CCam::m_apCam[MPara.iCamId] -> GetImg(pImg , iImgWidth , iImgHeight , iImgBit);
    m_pCrntImg->SetImage(pImg , iImgWidth , iImgHeight , iImgBit);

    //m_bGrabEnd = true ;

    SetEvent(m_hGrabEnd);
}

bool CCamera_V01::Ready() //카메라 같은 경우.._pImg 이것을 안쓴다.
{
    CLight::m_apLight[MPara.iLightId] -> ApplyUserPara(LightPara  ) ;
    
    CCam::m_apCam  [MPara.iCamId  ] -> ApplyUserPara(CamPara    ) ;

    return true ;
}



bool CCamera_V01::Run(CImage * _pImg) //카메라 같은 경우.._pImg 이것을 안쓴다.
{
    if(m_bSkip) {
        Rslt.bRsltOk = true ;
        return true ;
    }

    m_sErrMsg = "";
    Rslt.bRsltOk = true ;
    //Lights Setting
    //if(m_tCamLightPara.iUseLightCh1)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh1 , m_tCamLightPara.iLtBright1);
    //if(m_tCamLightPara.iUseLightCh2)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh2 , m_tCamLightPara.iLtBright2);
    //if(m_tCamLightPara.iUseLightCh3)Light[m_tVisnStaticPara.iUseLightId] -> SetBright(m_tCamLightPara.iUseLightCh3 , m_tCamLightPara.iLtBright3);

    Trace("<@SETBUFF>",(String(MPara.iCamId+35) + ",Cam" + MPara.iCamId + "_1").c_str());
    CLight::m_apLight[MPara.iLightId] -> ApplyUserPara(LightPara  ) ;
    Trace("<@SETBUFF>",(String(MPara.iCamId+35) + ",Cam" + MPara.iCamId + "_2").c_str());
    CCam::m_apCam  [MPara.iCamId  ] -> SetGrabFunc  (CamCallback) ;
    Trace("<@SETBUFF>",(String(MPara.iCamId+35) + ",Cam" + MPara.iCamId + "_3").c_str());
    CCam::m_apCam  [MPara.iCamId  ] -> ApplyUserPara(CamPara    ) ;
    Trace("<@SETBUFF>",(String(MPara.iCamId+35) + ",Cam" + MPara.iCamId + "_4").c_str());
    Sleep(MPara.iGrabDelay);
    Trace("<@SETBUFF>",(String(MPara.iCamId+35) + ",Cam" + MPara.iCamId + "_5").c_str());
    //Grab
    //m_bGrabEnd = false ;
    if(!CCam::m_apCam[MPara.iCamId] -> Grab()){
        //m_bGrabEnd = true ;
        Rslt.bRsltOk = false ;
        m_sErrMsg = "ERROR : Grab Failed!"  ;
        return false ;
    }

    //double dStartTime = GetTickCount();
    Trace("<@SETBUFF>",(String(MPara.iCamId+35) + ",Cam" + MPara.iCamId + "_6").c_str());
    ResetEvent(m_hGrabEnd);
    Trace("<@SETBUFF>",(String(MPara.iCamId+35) + ",Cam" + MPara.iCamId + "_7").c_str());
    DWORD dwRet = WaitForSingleObject(m_hGrabEnd , 1000) ;
    Trace("<@SETBUFF>",(String(MPara.iCamId+35) + ",Cam" + MPara.iCamId + "_8").c_str());

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
    Trace("<@SETBUFF>",(String(MPara.iCamId+35) + ",Cam" + MPara.iCamId + "_9").c_str());


    /*
    while(!m_bGrabEnd){
        Sleep(1);


        if(GetTickCount() - dStartTime > 5000){
            m_bGrabEnd = true ;
            m_sErrMsg = "Grab TimeOut(over 5sec)!" ;
            return false ;

        }
    }*/

    if(MPara.bSaveLastImg)m_pCrntImg -> SaveBmp("C:\\LastImg.bmp");


    Rslt.bRsltOk = true ;

    //2번 뿌리게 되어서 없앰.
    //SendPaintMsg();

    Trace("<@SETBUFF>",(String(MPara.iCamId+35) + ",Cam" + MPara.iCamId + "_End").c_str());
    return true ;

}

String CCamera_V01::GetErrMsg()
{
    return m_sErrMsg ;
}



        //화면 출력.
void CCamera_V01::PaintRslt(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    if(m_bSkip) return ;




    CGdi *Gdi = new CGdi(_hDc);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    if(!Rslt.bRsltOk){
        Gdi -> m_tText.Color = clRed ;
        Gdi -> m_tFont  .Height = 30       ;
        Gdi -> Text(5, 5 ,m_sErrMsg.c_str()) ;
    }






    delete Gdi;


}




void CCamera_V01::PaintTracker(HDC _hDc , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    CGdi *Gdi = new CGdi(_hDc);
    Gdi -> SetScale(_dScaleX , _dScaleY);

    delete Gdi;

}



        //이미지 화면 클릭 이벤트
bool CCamera_V01::MouseDw(TMouseButton Button,TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
{
    return NULL ;
}


bool CCamera_V01::MouseMv(TShiftState Shift ,int x, int y , double _dScaleX , double _dScaleY , double _dOffsetX , double _dOffsetY)
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
        UpdateListPara(_bToTable , _vePara , &MPara.List);
    }
    else {
        UpdateListPara(_bToTable , _vePara , &MPara.List);
        UpdateMasterPara(true , _vePara );
    }


}

void CCamera_V01::UpdateCommonPara(bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        UpdateListPara(_bToTable , _vePara , &CPara.List);
        CCam  ::m_apCam  [MPara.iCamId  ] -> UpdateUserPara(_bToTable , _vePara , CamPara  );
        CLight::m_apLight[MPara.iLightId] -> UpdateUserPara(_bToTable , _vePara , LightPara);

    }
    else {
        UpdateListPara(_bToTable , _vePara , &CPara.List);

        CCam  ::m_apCam  [MPara.iCamId  ] -> UpdateUserPara(_bToTable , _vePara , CamPara  );
        CLight::m_apLight[MPara.iLightId] -> UpdateUserPara(_bToTable , _vePara , LightPara);
        //UpdateListPara(_bToTable , _vePara , CamPara  );
        //UpdateListPara(_bToTable , _vePara , LightPara);

        UpdateCommonPara(true , _vePara );
    }
}
void CCamera_V01::UpdateLocalPara (bool _bToTable , TValueListEditor * _vePara)
{
    if(_bToTable) {
        _vePara -> Strings -> Clear() ;
        //for문
        //UpdateListPara(_bToTable , _vePara , &LPara.List);
        //CCam::m_apCam[CPara.iCamId] -> UpdateUserPara(_bToTable , _vePara , CamPara);

    }
    else {

        UpdateLocalPara(true , _vePara );
    }

}

        //로드 세이브.
void CCamera_V01::LoadSave(bool _bLoad , String _sVisnName)
{
    String sIniPath        = GM.GetJobFileFolder() + _sVisnName + "_" + m_sName + "_" + GetStaticPkgLink()->GetPkgName() + ".ini" ;
    String sCrntImagePath  = GM.GetJobFileFolder() + _sVisnName + "_" + m_sName + "_CrntImage.bmp"  ;
    String sTrainImagePath = GM.GetJobFileFolder() + _sVisnName + "_" + m_sName + "_TrainImage.bmp" ;

    TUserINI Ini ;
    int iCamIdBf     ;
    int iLightIdBf   ;
    Ini.Load(sIniPath , "MasterPara", "iCamId  " , iCamIdBf  );
    Ini.Load(sIniPath , "MasterPara", "iLightId" , iLightIdBf);

    if(_bLoad) {
        m_pCrntImg   -> LoadBmp(sCrntImagePath .c_str());
        m_pTrainImg  -> LoadBmp(sTrainImagePath.c_str());

        if(FileExists(sIniPath)){
            LoadSaveList(_bLoad , sIniPath , "MasterPara" , &MPara.List);
            LoadSaveList(_bLoad , sIniPath , "CommonPara" , &CPara.List);
        }
        else {
            MPara.SetDefault();
            CPara.SetDefault();
        }
    }
    else {
        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sIniPath       ) ;
        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sCrntImagePath ) ;
        //if(m_sCrntJobFilePath != "")Ini.ClearFile(sTrainImagePath) ;

        m_pCrntImg   -> SaveBmp(sCrntImagePath .c_str());
        m_pTrainImg  -> SaveBmp(sTrainImagePath.c_str());

        LoadSaveList(_bLoad , sIniPath , "MasterPara" , &MPara.List);
        LoadSaveList(_bLoad , sIniPath , "CommonPara" , &CPara.List);
    }

    Cal.LoadSave(_bLoad , _sVisnName);



    if(CCam::m_apCam[iCamIdBf] -> GetType() != CCam::m_apCam[MPara.iCamId] -> GetType() || CamPara == NULL) {
        if(CamPara) delete CamPara ;
        if(CCam::m_apCam[MPara.iCamId] -> GetType()  == ctImi ) CamPara = new CImi::TUserPara();
    }
    CCam::m_apCam[MPara.iCamId] -> LoadSaveUserPara(_bLoad , sIniPath , CamPara);
    //여기서 뻑나면 잡파일 로딩이 안됌...
    try{
        CCam::m_apCam[MPara.iCamId] -> ApplyUserPara(CamPara) ;
    }
    catch(...){
        MessageBox(0,"Camera Apply Para Failed",(String("Cam")+MPara.iCamId).c_str(),0);
    }


    if(CLight::m_apLight[iLightIdBf] -> GetType() != CLight::m_apLight[MPara.iLightId] -> GetType() || LightPara == NULL) {
        if(LightPara) delete LightPara ;
        if(CLight::m_apLight[MPara.iLightId] -> GetType()  == ltDkmSerial ) LightPara = new CDkmSerial::TUserPara();
    }
    CLight::m_apLight[MPara.iLightId] -> LoadSaveUserPara(_bLoad , sIniPath , LightPara);

    //일단 테스트 이기 때문에.. 주석.. 대겸 테스트 끝나면 풀어야 한다.
    //CLight::m_apLight[MPara.iLightId] -> ApplyUserPara(LightPara) ;




}




