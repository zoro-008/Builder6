//---------------------------------------------------------------------------
 //#include <system.hpp>
#pragma hdrstop
#include "CamUnit.h"
#include "sLogUnit.h"



#include "UserINI.h"
//#include "SLogUnit.h" 이거 넣으면 메모리 누수 난다. 이유는 나중에 찾자.
//---------------------------------------------------------------------------
#pragma package(smart_init)
#define CLIP(t) (((t)>255)?255:(((t)<0)?0:(t)))//값의 범위가  0 <= t < 256 되도록 한다

CNeptune Cam ;//MAX_CAM_ID] ;
/*
void InitCam (EN_CAM_TYPE * _iCamType)
{
    for(int i = 0 ; i < MAX_CAM_ID ; i++) {
             if(_iCamType[i] == ctNept     ) { Cam[i] = new CNeptune() ; }
        else if(_iCamType[i] == ctImi      ) { Cam[i] = new CImi() ; }
        else                                 { ShowMessage("Unknown Cam Type!");}
    }
}
void CloseCam()
{
    for(int i = 0 ; i < MAX_CAM_ID ; i++) {
        delete Cam [i] ;
    }
}
*/

void ConvertYUV422toRGB(int width,int height , unsigned char** pSrc, unsigned char** pDst)
{
    int i,j;
    unsigned char* dst=*pDst;
    unsigned char* src=*pSrc;
    int Y1,Y2,U,V;
    int R1,G1,B1 , R2,G2,B2;

    double dTime = GetTickCount() ;

    for(i=0;i<width*height;i=i+2){
        Y1=src[i*2+1];
        Y2=src[i*2+3];
        U=src[i*2];
        V=src[i*2+2];


        //성공.  300ms     1600*1200
        //R1=CLIP((1.164*(Y1 - 16)) + (2.018*(U - 128)));
        //G1=CLIP((1.164*(Y1 - 16)) - (0.813*(V - 128)) - (0.391*(U - 128)));
        //B1=CLIP((1.164*(Y1 - 16)) + (1.596*(V - 128)));
        //R2=CLIP((1.164*(Y2 - 16)) + (2.018*(U - 128)));
        //G2=CLIP((1.164*(Y2 - 16)) - (0.813*(V - 128)) - (0.391*(U - 128)));
        //B2=CLIP((1.164*(Y2 - 16)) + (1.596*(V - 128)));

        //성공.. floating 연산 제거.  80ms  1600*1200
        R1=CLIP(( 76284*(Y1-16) + 132252*(U-128)                  ) >> 16);
        G1=CLIP(( 76284*(Y1-16) -  53281*(V-128) -  25625*(U-128) ) >> 16);
        B1=CLIP(( 76284*(Y1-16) + 104595*(V-128)                  ) >> 16);
        R2=CLIP(( 76284*(Y2-16) + 132252*(U-128)                  ) >> 16);
        G2=CLIP(( 76284*(Y2-16) -  53281*(V-128) -  25625*(U-128) ) >> 16);
        B2=CLIP(( 76284*(Y2-16) + 104595*(V-128)                  ) >> 16);

        dst[i*3+2]= B1;
        dst[i*3+1]= G1;
        dst[i*3  ]= R1;
        dst[i*3+5]= B2;
        dst[i*3+4]= G2;
        dst[i*3+3]= R2;
    }

    dTime = GetTickCount() - dTime ;
}

#define CLIP(t) (((t)>255)?255:(((t)<0)?0:(t)))//값의 범위가  0 <= t < 256 되도록 한다













//Neptune....
//==============================================================================
bool CNeptune::m_bDllLoaded = false ;
int  CNeptune::m_iMaxCam = 0 ;
PNEPTUNE_CAM_INFO CNeptune::m_pCamInfo = NULL ;
void CNeptune::MakeUI()
{
    ParaUi.pnBase = new TPanel(Application);

    ParaUi.lbVideoFormat = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbVideoFormat , 0 , 0 ); ParaUi.cbVideoFormat = new TComboBox(ParaUi.pnBase); SetArrange(ParaUi.cbVideoFormat , 1 , 0 );
    ParaUi.lbVideoMode   = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbVideoMode   , 0 , 1 ); ParaUi.cbVideoMode   = new TComboBox(ParaUi.pnBase); SetArrange(ParaUi.cbVideoMode   , 1 , 1 );
    ParaUi.lbFrameRate   = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbFrameRate   , 0 , 2 ); ParaUi.cbFrameRate   = new TComboBox(ParaUi.pnBase); SetArrange(ParaUi.cbFrameRate   , 1 , 2 );

    ParaUi.lbBrightness  = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbBrightness  , 0 , 3 ); ParaUi.edBrightness  = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edBrightness  , 1 , 3 );
    ParaUi.lbSharpness   = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbSharpness   , 0 , 4 ); ParaUi.edSharpness   = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edSharpness   , 1 , 4 );
    ParaUi.lbGamma       = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbGamma       , 0 , 5 ); ParaUi.edGamma       = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edGamma       , 1 , 5 );
    ParaUi.lbExposure    = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbExposure    , 0 , 6 ); ParaUi.edExposure    = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edExposure    , 1 , 6 );
    ParaUi.lbShutter     = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbShutter     , 0 , 7 ); ParaUi.edShutter     = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edShutter     , 1 , 7 );
    ParaUi.lbGain        = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbGain        , 0 , 8 ); ParaUi.edGain        = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edGain        , 1 , 8 );
    ParaUi.lbPan         = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbPan         , 0 , 9 ); ParaUi.edPan         = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edPan         , 1 , 9 );
    ParaUi.lbTilt        = new TLabel(ParaUi.pnBase); SetArrange(ParaUi.lbTilt        , 0 , 10); ParaUi.edTilt        = new TEdit    (ParaUi.pnBase); SetArrange(ParaUi.edTilt        , 1 , 10);


    ParaUi.lbVideoFormat -> Parent = ParaUi.pnBase ; ParaUi.cbVideoFormat -> Parent = ParaUi.pnBase ;
    ParaUi.lbVideoMode   -> Parent = ParaUi.pnBase ; ParaUi.cbVideoMode   -> Parent = ParaUi.pnBase ;
    ParaUi.lbFrameRate   -> Parent = ParaUi.pnBase ; ParaUi.cbFrameRate   -> Parent = ParaUi.pnBase ;

    ParaUi.lbBrightness  -> Parent = ParaUi.pnBase ; ParaUi.edBrightness  -> Parent = ParaUi.pnBase ;
    ParaUi.lbSharpness   -> Parent = ParaUi.pnBase ; ParaUi.edSharpness   -> Parent = ParaUi.pnBase ;
    ParaUi.lbGamma       -> Parent = ParaUi.pnBase ; ParaUi.edGamma       -> Parent = ParaUi.pnBase ;
    ParaUi.lbExposure    -> Parent = ParaUi.pnBase ; ParaUi.edExposure    -> Parent = ParaUi.pnBase ;
    ParaUi.lbShutter     -> Parent = ParaUi.pnBase ; ParaUi.edShutter     -> Parent = ParaUi.pnBase ;
    ParaUi.lbGain        -> Parent = ParaUi.pnBase ; ParaUi.edGain        -> Parent = ParaUi.pnBase ;
    ParaUi.lbPan         -> Parent = ParaUi.pnBase ; ParaUi.edPan         -> Parent = ParaUi.pnBase ;
    ParaUi.lbTilt        -> Parent = ParaUi.pnBase ; ParaUi.edTilt        -> Parent = ParaUi.pnBase ;


    ParaUi.lbVideoFormat -> Caption = "Video Format" ;
    ParaUi.lbVideoMode   -> Caption = "VideoMode   " ;
    ParaUi.lbFrameRate   -> Caption = "FrameRate   " ;

    ParaUi.lbBrightness  -> Caption = "Brightness  " ;
    ParaUi.lbSharpness   -> Caption = "Sharpness   " ;
    ParaUi.lbGamma       -> Caption = "Gamma       " ;
    ParaUi.lbExposure    -> Caption = "Exposure    " ;
    ParaUi.lbShutter     -> Caption = "Shutter     " ;
    ParaUi.lbGain        -> Caption = "Gain        " ;
    ParaUi.lbPan         -> Caption = "Pan         " ;
    ParaUi.lbTilt        -> Caption = "Tilt        " ;

/*
    ParaUi.cbVideoFormat -> AddItem("0 IMC_FORMAT_RGB24"  , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("1 IMC_FORMAT_Y800"   , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("2 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("3 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("4 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("5 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("6 IMC_FORMAT_Y1600"  , NULL) ;

    ParaUi.cbVideoMode   -> AddItem("1 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("2 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("3 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("4 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("5 IMC_MODE_640x480"  , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("6 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("7 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("8 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("9 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("10 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("11 ICM_NOT SUPPORTED" , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("12 IMC_MODE_1600x1200" , NULL) ;

    ParaUi.cbFrameRate   -> AddItem("1 IMC_FRATE_3_75" , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("1 IMC_FRATE_3_75" , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("2 IMC_FRATE_7_5"  , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("3 IMC_FRATE_10"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("4 IMC_FRATE_15"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("5 IMC_FRATE_20"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("6 IMC_FRATE_30"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("7 IMC_FRATE_40"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("8 IMC_FRATE_60"   , NULL) ;
*/
}

void CNeptune::SetArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    const int iControlWidth   = 60  ;
    const int iControlHeight  = 20  ;
    const int iStartRowOffset = 5   ;
    const int iStartColOffset = 5   ;

    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;

}

// callback which is used for grabbing original data from camera.
void CALLBACK CNeptune::NeptuneCallback(PNEPTUNE_IMAGE pImage, void *pContext)
{
    ((CNeptune *)pContext) -> m_iImgWidth  = pImage -> uiWidth    ;
    ((CNeptune *)pContext) -> m_iImgHeight = pImage -> uiHeight   ;
    ((CNeptune *)pContext) -> m_iImgBit    = pImage -> uiBitDepth ;

    ((CNeptune *)pContext) -> SetBuffer(pImage->pData  , pImage->uiSize) ;
}

void CNeptune::SetBuffer(unsigned char * pBuffer, int lBufferSize)
{
    m_fGrabTime   = GetTickCount() - m_fGrabOnTime;

    //m_iImgBit = lBufferSize * 8 / (m_iImgWidth * m_iImgHeight) ;
    m_pImgBuf = (unsigned char *)pBuffer ;

    //if(Para.iVideoFormat == IMC_FORMAT_Y422 && m_iImgBit == 16) { //IMI 현재 8비트 와 16비트 가 있는데 8비트는 256색흑백 , 16비트는 컬러 YUV211형식이라 24비트 RGB888로 컨버팅 해줘야 한다.
    //      //컨버젼
    //      //yuv_to_rgb(m_iImgWidth, m_iImgHeight, pBuffer, m_pImgRGBBuf);
    //      ConvertYUV422toRGB(m_iImgWidth, m_iImgHeight, &pBuffer, &m_pImgRGBBuf);
    //      //YUY422_to_RGB24(m_iImgWidth, m_iImgHeight, pBuffer, m_pImgRGBBuf);
    //      m_iImgBit = 24 ; //RGB24
    //      m_pImgBuf = m_pImgRGBBuf ;
    //  }

    if(m_pCallbackFunc) m_pCallbackFunc(m_iCamNo);
}

CNeptune::CNeptune(void)
{
    //Init();
}

CNeptune::~CNeptune(void)
{
    //Close();
    static int i = 0 ;
    i++;
}

bool CNeptune::Init()
{

    static bool bFirst = true ;
    static int iCamNo = 0 ;

    if(bFirst) {
        if(!m_bDllLoaded){
            //m_bDllLoaded = LoadNeptuneApi("D:\\Bc6Works\\ITW\\HRT_430S_3\\bin\\NeptuneC_MD_VC80.dll");//LoadImCamApi("ImCam.dll") ;
            m_bDllLoaded = LoadNeptuneApi("NeptuneC_MD_VC80.dll");//LoadImCamApi("ImCam.dll") ;
            //m_bDllLoaded = LoadNeptuneApi("D:\\Bc6Works\\SSC\\베트남\\HRT_450T\\bin\\NeptuneC_MD_VC80.dll");//LoadImCamApi("ImCam.dll") ;
            //m_bDllLoaded = LoadNeptuneApi("AXL.dll");//LoadImCamApi("ImCam.dll") ;

            if(!m_bDllLoaded) {ShowMessage("LoadNeptuneApi Failed"); return false ; }
        }


        if(ntcInit()!= NEPTUNE_ERR_Success) {ShowMessage("Neptune Initial Failed"); return false ; }

        //ImCamUninit();
        unsigned int iCamCnt ;
        if(ntcGetCameraCount(&iCamCnt) != NEPTUNE_ERR_Success) return false ;
        m_iMaxCam = iCamCnt;

        // allocate camera information with new count
        m_pCamInfo = new NEPTUNE_CAM_INFO[m_iMaxCam];

        // get camera information
        ntcGetCameraInfo(m_pCamInfo, m_iMaxCam);
    }




    m_pCallbackFunc = NULL ;
    m_pImgBuf    = NULL ;
    m_pImgRGBBuf = NULL ;
    m_iNeptuneCamNo = iCamNo ;
    iCamNo++;
    MakeUI();

    m_iCamType = ctNept ;
    LoadPara(true , EXE_FOLDER + "\\Util\\Vision\\ImCamPara.ini");
    //Para    .iVideoFormat= 1   ; //;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
    //Para    .iVideoMode  = 5   ; //;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
    //Para    .iFrameRate  = 8   ; //;1-IMC_FRATE_3_75, 2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60
    //UserPara.iBrightness = 490 ;
    //UserPara.iSharpness  = 550 ;
    //UserPara.iGamma      = 10  ;
    //UserPara.iExposure   = 50  ; //요건 동작 안함....
    //UserPara.iShutter    = 1100;
    //UserPara.iGain       = 0   ;
    //UserPara.iPan        = 326 ;
    //UserPara.iTilt       = 246 ;
    //LoadPara(false ,EXE_FOLDER + "\\Util\\Vision\\ImCamPara" + AnsiString(m_iImiCamNo) + ".ini");

    //ImCamSetMemoryChannel
    //
    //if(ImCamSetVideoFormat(m_iImiCamNo, Para.iVideoFormat)!= IMC_SUCCESS) {ShowMessage("Format not found!"   ); return false ; }//;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
    //if(ImCamSetVideoMode  (m_iImiCamNo, Para.iVideoMode  )!= IMC_SUCCESS) {ShowMessage("Mode not found!"     ); return false ; }//;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
    //if(ImCamSetFrameRate  (m_iImiCamNo, Para.iFrameRate  )!= IMC_SUCCESS) {ShowMessage("FrameRate not found!"); return false ; }//;1-IMC_FRATE_3_75, 2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60
    if ( ntcOpen(m_pCamInfo[m_iNeptuneCamNo].strCamID, &m_hCamHandle) != NEPTUNE_ERR_Success )
    {
    	ShowMessage("Can not Open Neptune camera"+String(m_pCamInfo[m_iNeptuneCamNo].strCamID));
    	return false;
    }


    ApplyPara();

    ntcSetFrameCallback(m_hCamHandle,NeptuneCallback,this);
    ntcSetAcquisition(m_hCamHandle, NEPTUNE_BOOL_FALSE);

    //로딩시에 유저셑 체널 5번을 로딩한다.
    //F2F1011C 레지스터 500000 으로 세팅 하고 전원오프시에 5번채널을 로딩하라는 설정임.
    //세팅 변경시에 Userset 5번 채널 로딩 해서 바꾸고 해야 함.
    NEPTUNE_USERSET_BC6 UserSet ;
    int iSize = sizeof(NEPTUNE_USERSET);
    iSize = sizeof(_uint16_t);
    iSize = sizeof(ENeptuneUserSet      );   //이넘.
    iSize = sizeof(ENeptuneUserSetCommand);  //이넘.


    UserSet.SupportUserSet = (_uint32_t)0 ;
    UserSet.UserSetIndex   = (_uint32_t)NEPTUNE_USERSET_5 ;      //ENeptuneUserSet.
    UserSet.Command        = (_uint32_t)NEPTUNE_USERSET_CMD_LOAD ;   //ENeptuneUserSetCommand.
    ENeptuneError ErrCode  = ntcSetUserSet(m_hCamHandle, UserSet );
    if (ErrCode != NEPTUNE_ERR_Success)  //ENeptuneError.
    {
        ShowMessage("Memory Channel Set Failed");
    	//return false;
    }

    NEPTUNE_IMAGE_SIZE tSize ;
    if(ntcGetImageSize    (m_hCamHandle,&tSize) != NEPTUNE_ERR_Success){
        tSize.nSizeX = 640 ;
        tSize.nSizeY = 480 ;
        ShowMessage("Can not Get ImageSize");
    }
    unsigned int iBit;
    if(ntcGetBitsPerPixel (m_hCamHandle,&iBit ) != NEPTUNE_ERR_Success ){
        iBit    = 8   ;
        ShowMessage("Can not Get ImageBit");
    }

    m_iImgWidth  = tSize.nSizeX ;
    m_iImgHeight = tSize.nSizeY ;
    m_iImgBit    = iBit    ;

    m_pImgRGBBuf = new unsigned char [m_iImgWidth * m_iImgHeight * 3] ;

    ntcSetAcquisition(m_hCamHandle, NEPTUNE_BOOL_TRUE);

    SetTrgMode(tmSw);

    //트리거 모드.

    return true ;
}

bool CNeptune::Close()
{
    static int iClosedCamCnt = 0 ;

    if(m_bDllLoaded){
        ntcSetAcquisition(m_hCamHandle, NEPTUNE_BOOL_FALSE);
        ntcSetFrameCallback(m_hCamHandle,NULL,this);
        ntcClose(m_hCamHandle);
    }

    if(m_pImgRGBBuf) {
        delete [] m_pImgRGBBuf ;
        m_pImgRGBBuf = NULL ;
    }

    iClosedCamCnt++;
    if(iClosedCamCnt == m_iMaxCam && m_bDllLoaded) {
        delete [] m_pCamInfo ;
        m_pCamInfo = NULL ;

        UnloadNeptuneApi();
    }

    return true ;
}

bool CNeptune::GetImg(unsigned char *& _pImg, int &_iWidth , int & _iHeight , int & _iBitCnt)
{
    _pImg    = m_pImgBuf    ;
    _iWidth  = m_iImgWidth  ;
    _iHeight = m_iImgHeight ;
    _iBitCnt = m_iImgBit    ;

    return true ;
}

void CNeptune::LoadPara(bool _bLoad , AnsiString _sPath)
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;
    AnsiString sTemp  ;

    //Set Dir.
    sTemp = "ImiCamera" + AnsiString(m_iNeptuneCamNo);
    sPath = _sPath;

    if(_bLoad) {
        //Load Device.
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iVideoFormat"    , Para.iVideoFormat    ); //;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iVideoMode  "    , Para.iVideoMode      ); //;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "Para.iFrameRate  "    , Para.iFrameRate      ); //;2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60

        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iBrightness" , UserPara.iBrightness );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iSharpness " , UserPara.iSharpness  );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iGamma     " , UserPara.iGamma      );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iExposure  " , UserPara.iExposure   );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iShutter   " , UserPara.iShutter    );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iGain      " , UserPara.iGain       );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iPan       " , UserPara.iPan        );
        UserINI.Load(sPath.c_str() , sTemp.c_str() , "UserPara.iTilt      " , UserPara.iTilt       );
    }
    else {
        //Save Device.
        //UserINI.ClearFile(sPath) ;
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iVideoFormat"    , Para.iVideoFormat    ); //;0-IMC_FORMAT_RGB24, 1-IMC_FORMAT_Y800, 6-IMC_FORMAT_Y1600
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iVideoMode  "    , Para.iVideoMode      ); //;5-IMC_MODE_640x480, 12-IMC_MODE_1600x1200
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "Para.iFrameRate  "    , Para.iFrameRate      ); //;1-IMC_FRATE_3_75, 2-IMC_FRATE_7_5, 3-IMC_FRATE_10, 4-IMC_FRATE_15, 5-IMC_FRATE_20, 6-IMC_FRATE_30, 7-IMC_FRATE_40, 8-IMC_FRATE_60

        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iBrightness" , UserPara.iBrightness );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iSharpness " , UserPara.iSharpness  );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iGamma     " , UserPara.iGamma      );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iExposure  " , UserPara.iExposure   );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iShutter   " , UserPara.iShutter    );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iGain      " , UserPara.iGain       );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iPan       " , UserPara.iPan        );
        UserINI.Save(sPath.c_str() , sTemp.c_str() , "UserPara.iTilt      " , UserPara.iTilt       );
        ApplyPara();
    }

}


bool CNeptune::ApplyPara()
{


    NEPTUNE_FEATURE_BC6 FeatureInfo          ;
    ntcGetFeature(m_hCamHandle, NEPTUNE_FEATURE_BLACKLEVEL, &FeatureInfo);
    FeatureInfo.Value = UserPara.iBrightness ;
    if(ntcSetFeature(m_hCamHandle, NEPTUNE_FEATURE_BLACKLEVEL, FeatureInfo)!= NEPTUNE_ERR_Success) {
        ShowMessage("Failed Set BRIGHTNESS_" + String(UserPara.iBrightness));
        return false ;
    }


    ntcGetFeature(m_hCamHandle, NEPTUNE_FEATURE_SHARPNESS, &FeatureInfo);
    FeatureInfo.Value = UserPara.iSharpness ;
    if(ntcSetFeature(m_hCamHandle, NEPTUNE_FEATURE_SHARPNESS, FeatureInfo)!= NEPTUNE_ERR_Success) {
        ShowMessage("Failed Set SHARPNESS_" + String(UserPara.iSharpness));
        return false ;
    }


    ntcGetFeature(m_hCamHandle, NEPTUNE_FEATURE_GAMMA, &FeatureInfo);
    FeatureInfo.Value = UserPara.iGamma ;
    if(ntcSetFeature(m_hCamHandle, NEPTUNE_FEATURE_GAMMA, FeatureInfo) != NEPTUNE_ERR_Success) {
        ShowMessage("Failed Set GAMMA_" + String(UserPara.iGamma));
        return false ;
    }


    String sSutter = String(UserPara.iShutter) + "us";
    if(ntcSetShutterString(m_hCamHandle, sSutter.c_str())!= NEPTUNE_ERR_Success) {
        ShowMessage("Failed Set SHUTTER_" + String(UserPara.iShutter));
        return false ;
    }

    ntcGetFeature(m_hCamHandle, NEPTUNE_FEATURE_GAIN, &FeatureInfo);
    FeatureInfo.Value = UserPara.iGain ;
    if(ntcSetFeature(m_hCamHandle, NEPTUNE_FEATURE_GAIN, FeatureInfo) != NEPTUNE_ERR_Success) {
        ShowMessage("Failed Set GAIN_" + String(UserPara.iGain));
        return false ;
    }

    return true ;
}

void CNeptune::UpdatePara(bool _bToTable)
{
    if(_bToTable) {
         if(ParaUi.cbVideoFormat) ParaUi.cbVideoFormat -> ItemIndex = Para   .iVideoFormat ;
         if(ParaUi.cbVideoMode  ) ParaUi.cbVideoMode   -> ItemIndex = Para   .iVideoMode   ;
         if(ParaUi.cbFrameRate  ) ParaUi.cbFrameRate   -> ItemIndex = Para   .iFrameRate   ;

         if(ParaUi.edBrightness ) ParaUi.edBrightness  -> Text      = UserPara.iBrightness ;
         if(ParaUi.edSharpness  ) ParaUi.edSharpness   -> Text      = UserPara.iSharpness  ;
         if(ParaUi.edGamma      ) ParaUi.edGamma       -> Text      = UserPara.iGamma      ;
         if(ParaUi.edExposure   ) ParaUi.edExposure    -> Text      = UserPara.iExposure   ;
         if(ParaUi.edShutter    ) ParaUi.edShutter     -> Text      = UserPara.iShutter    ;
         if(ParaUi.edGain       ) ParaUi.edGain        -> Text      = UserPara.iGain       ;
         if(ParaUi.edPan        ) ParaUi.edPan         -> Text      = UserPara.iPan        ;
         if(ParaUi.edTilt       ) ParaUi.edTilt        -> Text      = UserPara.iTilt       ;
     }

     else {
         int iTemp;
                
         if(ParaUi.cbVideoFormat && ParaUi.cbVideoFormat -> ItemIndex != Para   .iVideoFormat) ; {
             ShowMessage("Please Restart Program.");
             //Trace("Operation" , "Both Imi_VideoFormat values are different.");

         }
         if(ParaUi.cbVideoMode   && ParaUi.cbVideoMode   -> ItemIndex != Para   .iVideoMode  ) ; {
             ShowMessage("Please Restart Program.");
             //Trace("Operation" , "Both Imi_VideoMode values are different.");
         }
         if(ParaUi.cbFrameRate   && ParaUi.cbFrameRate   -> ItemIndex != Para   .iFrameRate  ) ; {
             ShowMessage("Please Restart Program.");
             //Trace("Operation" , "Both Imi_FrameRate values are different.");
         }





         if(ParaUi.cbVideoFormat) Para    .iVideoFormat   = ParaUi.cbVideoFormat -> ItemIndex  ;
         if(ParaUi.cbVideoMode  ) Para    .iVideoMode     = ParaUi.cbVideoMode   -> ItemIndex  ;
         if(ParaUi.cbFrameRate  ) Para    .iFrameRate     = ParaUi.cbFrameRate   -> ItemIndex  ;

         if(ParaUi.edBrightness ) UserPara.iBrightness    = StrToIntDef(ParaUi.edBrightness -> Text , UserPara.iBrightness );
         if(ParaUi.edSharpness  ) UserPara.iSharpness     = StrToIntDef(ParaUi.edSharpness  -> Text , UserPara.iSharpness  );
         if(ParaUi.edGamma      ) UserPara.iGamma         = StrToIntDef(ParaUi.edGamma      -> Text , UserPara.iGamma      );
         if(ParaUi.edExposure   ) UserPara.iExposure      = StrToIntDef(ParaUi.edExposure   -> Text , UserPara.iExposure   );
         if(ParaUi.edShutter    ) UserPara.iShutter       = StrToIntDef(ParaUi.edShutter    -> Text , UserPara.iShutter    );
         if(ParaUi.edGain       ) UserPara.iGain          = StrToIntDef(ParaUi.edGain       -> Text , UserPara.iGain       );
         if(ParaUi.edPan        ) UserPara.iPan           = StrToIntDef(ParaUi.edPan        -> Text , UserPara.iPan        );
         if(ParaUi.edTilt       ) UserPara.iTilt          = StrToIntDef(ParaUi.edTilt       -> Text , UserPara.iTilt       );
     }


}

bool CNeptune::Grab()
{
    if (ntcRunSWTrigger(m_hCamHandle) != NEPTUNE_ERR_Success ) {
	return false;
    }

    //sunsun 틱타임으로 바꾸자.
    m_fGrabOnTime = GetTickCount() ;

    return true ;
}

bool CNeptune::SetTrgMode(EN_TRG_MODE _iMode)
{
    NEPTUNE_TRIGGER TrigProp ;
    memset(&TrigProp, 0, sizeof(NEPTUNE_TRIGGER));

    if( _iMode == tmSw) {
	TrigProp.Source = NEPTUNE_TRIGGER_SOURCE_SW ;		// SW trigger
    }
    else if (_iMode == tmHw){
        TrigProp.Source = NEPTUNE_TRIGGER_SOURCE_LINE1;
    }
    else if (_iMode == tmLive) {
	TrigProp.Source = NEPTUNE_TRIGGER_SOURCE_LINE1;
    }

    TrigProp.Mode     = NEPTUNE_TRIGGER_MODE_0; // trigger mode 0
    TrigProp.Polarity = NEPTUNE_POLARITY_RISINGEDGE; // rising edge
    TrigProp.OnOff    = NEPTUNE_BOOL_TRUE; // trigger on

    if (ntcSetTrigger(m_hCamHandle, TrigProp) != NEPTUNE_ERR_Success ) {
        //이상하게 실행파일로 하면 메세지가 뜸.
    	ShowMessage("Can not Set Trigger Mode");
    	return false;
    }
    return true ;
}


void CNeptune::SetGrabFunc(CamCallbackFunc _pFunc)
{
    m_pCallbackFunc = _pFunc ;

}

bool CNeptune::ShowParaForm(TPanel * _pnBase)
{
    ParaUi.pnBase -> Parent = _pnBase ;
    ParaUi.pnBase -> Align  = alClient ;


    ParaUi.cbVideoFormat -> Items->Clear();
    ParaUi.cbVideoFormat -> AddItem("RGB24" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("Y800 " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("Y411 " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("Y422 " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("Y444 " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("VYUY " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("Y1600" , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("RAW8 " , NULL) ;
    ParaUi.cbVideoFormat -> AddItem("RAW16" , NULL) ;

    ParaUi.cbVideoMode   -> Items->Clear();
    ParaUi.cbVideoMode   -> AddItem("160x120  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("200x150  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("320x240  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("340x240  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("636x480  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("640x480  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("800x600  " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("1024x768 " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("1280x960 " , NULL) ;
    ParaUi.cbVideoMode   -> AddItem("1280x1024" , NULL) ;

    ParaUi.cbFrameRate   -> Items->Clear();
    ParaUi.cbFrameRate   -> AddItem("1.8"  , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("3.7"  , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("7.5"  , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("10"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("15"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("20"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("30"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("40"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("60"   , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("120"  , NULL) ;
    ParaUi.cbFrameRate   -> AddItem("240"  , NULL) ;
    UpdatePara(true);

    return true ;
}

void CNeptune::SetExposure (int _iTime )
{
    String sSutter = String(_iTime) + "us";
    if(ntcSetShutterString(m_hCamHandle, sSutter.c_str())!= NEPTUNE_ERR_Success) {
        //ShowMessage("Failed Set SHUTTER_" + String(UserPara.iShutter));
        return ;
    }
}

float CNeptune::GetGrabTime()
{
     return m_fGrabTime ;

}


















